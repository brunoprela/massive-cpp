#pragma once

#include "massive/core/config.hpp"
#include "massive/websocket/models.hpp"
#include "massive/exceptions.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <optional>
#include <atomic>

namespace massive::websocket {

// Message handler type
using MessageHandler = std::function<void(const std::vector<WebSocketMessage>&)>;

/**
 * WebSocket client for streaming real-time market data from Massive.com
 */
class WebSocketClient {
public:
    WebSocketClient(
        const std::string& api_key,
        Feed feed = Feed::RealTime,
        Market market = Market::Stocks,
        bool raw = false,
        bool verbose = false,
        std::optional<int> max_reconnects = 5
    );

    ~WebSocketClient();

    // Connection management
    void connect(MessageHandler handler);
    void close();

    // Subscription management
    void subscribe(const std::vector<std::string>& subscriptions);
    void unsubscribe(const std::vector<std::string>& subscriptions);
    void unsubscribe_all();

private:
    void ensure_connected();
    void authenticate();
    void reconcile_subscriptions();
    void process_message(const std::string& message);
    std::vector<WebSocketMessage> parse_messages(const std::string& json);
    
    std::string api_key_;
    Feed feed_;
    Market market_;
    bool raw_;
    bool verbose_;
    std::optional<int> max_reconnects_;
    
    std::set<std::string> current_subscriptions_;
    std::set<std::string> scheduled_subscriptions_;
    bool needs_resubscribe_;
    
    // WebSocket connection state
    std::atomic<bool> connected_{false};
    void* websocket_impl_;  // Opaque pointer to implementation
};

}  // namespace massive::websocket

