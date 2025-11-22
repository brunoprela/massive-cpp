#include "massive/websocket/client.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <simdjson/ondemand.h>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <atomic>

namespace massive::websocket {

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

// PIMPL structure for WebSocket implementation
struct Impl {
    net::io_context ioc;
    ssl::context ctx{ssl::context::sslv23_client};
    std::unique_ptr<websocket::stream<beast::ssl_stream<tcp::socket>>> ws;
    std::thread worker_thread;
    std::atomic<bool> running{false};
    std::atomic<bool> connected{false};
    MessageHandler message_handler;
    
    Impl() {
        ctx.set_default_verify_paths();
    }
};

WebSocketClient::WebSocketClient(
    const std::string& api_key,
    Feed feed,
    Market market,
    bool raw,
    bool verbose,
    std::optional<int> max_reconnects
)
    : api_key_(api_key)
    , feed_(feed)
    , market_(market)
    , raw_(raw)
    , verbose_(verbose)
    , max_reconnects_(max_reconnects)
    , current_subscriptions_()
    , scheduled_subscriptions_()
    , needs_resubscribe_(false)
    , connected_(false)
    , websocket_impl_(static_cast<void*>(new Impl()))
{
    if (api_key.empty()) {
        throw AuthError("API key is required for WebSocket connection");
    }
}

WebSocketClient::~WebSocketClient() {
    close();
    delete static_cast<Impl*>(websocket_impl_);
}

void WebSocketClient::connect(MessageHandler handler) {
    auto* impl = static_cast<Impl*>(websocket_impl_);
    
    if (impl->running) {
        throw std::runtime_error("WebSocket client is already running");
    }
    
    impl->message_handler = std::move(handler);
    impl->running = true;
    
    // Build WebSocket host and path
    std::string feed_str = to_string(feed_);
    std::string market_str = to_string(market_);
    std::string host = feed_str;
    std::string path = "/" + market_str;
    
    // Resolve host
    tcp::resolver resolver(impl->ioc);
    auto const results = resolver.resolve(host, "443");
    
    // Create WebSocket stream
    impl->ws = std::make_unique<websocket::stream<beast::ssl_stream<tcp::socket>>>(
        impl->ioc, impl->ctx
    );
    
    // Set SNI hostname
    if (!SSL_set_tlsext_host_name(impl->ws->next_layer().native_handle(), host.c_str())) {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        throw beast::system_error{ec};
    }
    
    // Connect
    net::connect(impl->ws->next_layer().next_layer(), results.begin(), results.end());
    
    // SSL handshake
    impl->ws->next_layer().handshake(ssl::stream_base::client);
    
    // WebSocket handshake
    impl->ws->handshake(host, path);
    
    impl->connected = true;
    connected_ = true;
    
    // Authenticate
    authenticate();
    
    // Reconcile subscriptions
    reconcile_subscriptions();
    
    // Start message processing in background thread
    impl->worker_thread = std::thread([this, impl]() {
        beast::flat_buffer buffer;
        while (impl->running && impl->connected) {
            try {
                buffer.clear();
                impl->ws->read(buffer);
                
                std::string message = beast::make_printable(buffer.data());
                process_message(message);
            } catch (const std::exception& e) {
                if (verbose_) {
                    std::cerr << "WebSocket read error: " << e.what() << std::endl;
                }
                break;
            }
        }
    });
}

void WebSocketClient::authenticate() {
    auto* impl = static_cast<Impl*>(websocket_impl_);
    
    if (!impl->connected) {
        throw std::runtime_error("Not connected");
    }
    
    // Send authentication message
    std::ostringstream auth_msg;
    auth_msg << R"({"action":"auth","params":")" << api_key_ << R"("})";
    std::string auth_str = auth_msg.str();
    
    net::const_buffer auth_buf(auth_str.data(), auth_str.size());
    impl->ws->write(auth_buf);
    
    // Read auth response
    beast::flat_buffer buffer;
    impl->ws->read(buffer);
    std::string response = beast::make_printable(buffer.data());
    
    // Parse response to check if auth succeeded
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response;
    auto doc_result = parser.iterate(json);
    if (!doc_result.error()) {
        auto doc = doc_result.value();
        auto arr = doc.get_array();
        if (!arr.error()) {
            for (auto elem : arr.value()) {
                auto obj = elem.get_object();
                if (!obj.error()) {
                    auto status_field = obj.value().find_field_unordered("status");
                    if (!status_field.error()) {
                        std::string status = std::string(status_field.value().get_string().value());
                        if (status == "auth_failed") {
                            auto msg_field = obj.value().find_field_unordered("message");
                            std::string msg = "Authentication failed";
                            if (!msg_field.error()) {
                                msg = std::string(msg_field.value().get_string().value());
                            }
                            throw AuthError(msg);
                        }
                    }
                }
            }
        }
    }
}

void WebSocketClient::reconcile_subscriptions() {
    auto* impl = static_cast<Impl*>(websocket_impl_);
    
    if (!impl->connected) {
        return;
    }
    
    // Subscribe to new subscriptions
    std::set<std::string> to_subscribe;
    std::set_difference(
        scheduled_subscriptions_.begin(), scheduled_subscriptions_.end(),
        current_subscriptions_.begin(), current_subscriptions_.end(),
        std::inserter(to_subscribe, to_subscribe.begin())
    );
    
    if (!to_subscribe.empty()) {
        std::ostringstream sub_msg;
        sub_msg << R"({"action":"subscribe","params":")";
        bool first = true;
        for (const auto& sub : to_subscribe) {
            if (!first) sub_msg << ",";
            sub_msg << sub;
            first = false;
        }
        sub_msg << R"("})";
        std::string sub_str = sub_msg.str();
        net::const_buffer sub_buf(sub_str.data(), sub_str.size());
        impl->ws->write(sub_buf);
    }
    
    // Unsubscribe from removed subscriptions
    std::set<std::string> to_unsubscribe;
    std::set_difference(
        current_subscriptions_.begin(), current_subscriptions_.end(),
        scheduled_subscriptions_.begin(), scheduled_subscriptions_.end(),
        std::inserter(to_unsubscribe, to_unsubscribe.begin())
    );
    
    if (!to_unsubscribe.empty()) {
        std::ostringstream unsub_msg;
        unsub_msg << R"({"action":"unsubscribe","params":")";
        bool first = true;
        for (const auto& sub : to_unsubscribe) {
            if (!first) unsub_msg << ",";
            unsub_msg << sub;
            first = false;
        }
        unsub_msg << R"("})";
        
        std::string unsub_str = unsub_msg.str();
        net::const_buffer unsub_buf(unsub_str.data(), unsub_str.size());
        impl->ws->write(unsub_buf);
    }
    
    current_subscriptions_ = scheduled_subscriptions_;
    needs_resubscribe_ = false;
}

void WebSocketClient::subscribe(const std::vector<std::string>& subscriptions) {
    for (const auto& sub : subscriptions) {
        scheduled_subscriptions_.insert(sub);
    }
    needs_resubscribe_ = true;
    
    if (connected_) {
        reconcile_subscriptions();
    }
}

void WebSocketClient::unsubscribe(const std::vector<std::string>& subscriptions) {
    for (const auto& sub : subscriptions) {
        scheduled_subscriptions_.erase(sub);
    }
    needs_resubscribe_ = true;
    
    if (connected_) {
        reconcile_subscriptions();
    }
}

void WebSocketClient::unsubscribe_all() {
    scheduled_subscriptions_.clear();
    needs_resubscribe_ = true;
    
    if (connected_) {
        reconcile_subscriptions();
    }
}

void WebSocketClient::process_message(const std::string& message) {
    auto* impl = static_cast<Impl*>(websocket_impl_);
    
    if (raw_) {
        // In raw mode, we'd pass the raw message, but for now we parse it
    }
    
    // Skip status messages
    if (message.find(R"("ev":"status")") != std::string::npos) {
        return;
    }
    
    auto parsed_messages = parse_messages(message);
    if (!parsed_messages.empty() && impl->message_handler) {
        impl->message_handler(parsed_messages);
    }
}

std::vector<WebSocketMessage> WebSocketClient::parse_messages(const std::string& json) {
    std::vector<WebSocketMessage> messages;
    
    simdjson::ondemand::parser parser;
    simdjson::padded_string json_data = json;
    auto doc_result = parser.iterate(json_data);
    if (doc_result.error()) {
        return messages;
    }
    
    auto doc = doc_result.value();
    auto arr = doc.get_array();
    if (arr.error()) {
        return messages;
    }
    
    for (auto elem : arr.value()) {
        auto obj_result = elem.get_object();
        if (obj_result.error()) {
            continue;
        }
        
        auto obj = obj_result.value();
        auto ev_field = obj.find_field_unordered("ev");
        if (ev_field.error()) {
            continue;
        }
        
        std::string event_type = std::string(ev_field.value().get_string().value());
        
        // Parse based on event type
        if (event_type == "T" || event_type == "XT") {
            // Trade
            EquityTrade trade;
            trade.event_type = event_type;
            
            auto sym_field = obj.find_field_unordered("sym");
            if (!sym_field.error()) {
                trade.symbol = std::string(sym_field.value().get_string().value());
            }
            
            auto p_field = obj.find_field_unordered("p");
            if (!p_field.error()) {
                trade.price = p_field.value().get_double().value();
            }
            
            auto s_field = obj.find_field_unordered("s");
            if (!s_field.error()) {
                trade.size = s_field.value().get_int64().value();
            }
            
            auto t_field = obj.find_field_unordered("t");
            if (!t_field.error()) {
                trade.timestamp = t_field.value().get_int64().value();
            }
            
            messages.push_back(trade);
        } else if (event_type == "Q" || event_type == "XQ") {
            // Quote
            EquityQuote quote;
            quote.event_type = event_type;
            
            auto sym_field = obj.find_field_unordered("sym");
            if (!sym_field.error()) {
                quote.symbol = std::string(sym_field.value().get_string().value());
            }
            
            auto bp_field = obj.find_field_unordered("bp");
            if (!bp_field.error()) {
                quote.bid_price = bp_field.value().get_double().value();
            }
            
            auto ap_field = obj.find_field_unordered("ap");
            if (!ap_field.error()) {
                quote.ask_price = ap_field.value().get_double().value();
            }
            
            auto t_field = obj.find_field_unordered("t");
            if (!t_field.error()) {
                quote.timestamp = t_field.value().get_int64().value();
            }
            
            messages.push_back(quote);
        } else if (event_type == "A" || event_type == "AM") {
            // Aggregate
            EquityAgg agg;
            agg.event_type = event_type;
            
            auto sym_field = obj.find_field_unordered("sym");
            if (!sym_field.error()) {
                agg.symbol = std::string(sym_field.value().get_string().value());
            }
            
            auto o_field = obj.find_field_unordered("o");
            if (!o_field.error()) {
                agg.open = o_field.value().get_double().value();
            }
            
            auto c_field = obj.find_field_unordered("c");
            if (!c_field.error()) {
                agg.close = c_field.value().get_double().value();
            }
            
            auto h_field = obj.find_field_unordered("h");
            if (!h_field.error()) {
                agg.high = h_field.value().get_double().value();
            }
            
            auto l_field = obj.find_field_unordered("l");
            if (!l_field.error()) {
                agg.low = l_field.value().get_double().value();
            }
            
            messages.push_back(agg);
        }
    }
    
    return messages;
}

void WebSocketClient::close() {
    auto* impl = static_cast<Impl*>(websocket_impl_);
    
    impl->running = false;
    connected_ = false;
    
    if (impl->ws && impl->connected) {
        try {
            impl->ws->close(websocket::close_code::normal);
        } catch (...) {
            // Ignore errors during close
        }
    }
    
    if (impl->worker_thread.joinable()) {
        impl->worker_thread.join();
    }
    
    impl->connected = false;
}

}  // namespace massive::websocket

