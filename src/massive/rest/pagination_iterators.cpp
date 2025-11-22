#include "massive/rest/client.hpp"
#include "massive/rest/pagination.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>
#include <sstream>

namespace massive::rest {

namespace {
// Helper function to extract pagination info and results array from response
std::pair<simdjson::ondemand::array, PaginationInfo> parse_paginated_response_base(
    const std::string& response_body) {
    
    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response_body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    simdjson::ondemand::array results_array;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto arr_result = results_field.value().get_array();
        if (!arr_result.error()) {
            results_array = arr_result.value();
        }
    }

    PaginationInfo pagination = extract_pagination_info(root_obj.value());
    return {results_array, pagination};
}

// Paginated iterator for tickers
PaginatedIterator<Ticker> RESTClient::list_tickers_iter(
    const std::optional<std::string> &ticker, const std::optional<std::string> &ticker_lt,
    const std::optional<std::string> &ticker_lte, const std::optional<std::string> &ticker_gt,
    const std::optional<std::string> &ticker_gte, const std::optional<std::string> &type,
    const std::optional<std::string> &market, const std::optional<std::string> &exchange,
    std::optional<int> cusip, std::optional<int> cik, const std::optional<std::string> &date,
    std::optional<bool> active, const std::optional<std::string> &search,
    const std::optional<std::string> &sort, const std::optional<std::string> &order) {
    
    // Build initial parameters
    std::map<std::string, std::string> base_params;
    if (ticker.has_value()) {
        base_params["ticker"] = ticker.value();
    }
    if (ticker_lt.has_value()) {
        base_params["ticker.lt"] = ticker_lt.value();
    }
    if (ticker_lte.has_value()) {
        base_params["ticker.lte"] = ticker_lte.value();
    }
    if (ticker_gt.has_value()) {
        base_params["ticker.gt"] = ticker_gt.value();
    }
    if (ticker_gte.has_value()) {
        base_params["ticker.gte"] = ticker_gte.value();
    }
    if (type.has_value()) {
        base_params["type"] = type.value();
    }
    if (market.has_value()) {
        base_params["market"] = market.value();
    }
    if (exchange.has_value()) {
        base_params["exchange"] = exchange.value();
    }
    if (cusip.has_value()) {
        base_params["cusip"] = std::to_string(cusip.value());
    }
    if (cik.has_value()) {
        base_params["cik"] = std::to_string(cik.value());
    }
    if (date.has_value()) {
        base_params["date"] = date.value();
    }
    if (active.has_value()) {
        base_params["active"] = active.value() ? "true" : "false";
    }
    if (search.has_value()) {
        base_params["search"] = search.value();
    }
    if (sort.has_value()) {
        base_params["sort"] = sort.value();
    }
    if (order.has_value()) {
        base_params["order"] = order.value();
    }
    // Don't set limit for iterators - let pagination handle it

    // Create fetch function
    auto fetch_fn = [this, base_params](const std::optional<std::string>& next_url) 
        -> std::pair<std::vector<Ticker>, PaginationInfo> {
        
        std::string path;
        std::map<std::string, std::string> params = base_params;
        
        if (next_url.has_value()) {
            // If we have a next_url, use it directly
            // Extract path and params from URL
            std::string url = next_url.value();
            size_t query_pos = url.find('?');
            if (query_pos != std::string::npos) {
                path = url.substr(0, query_pos);
                std::string query = url.substr(query_pos + 1);
                // Simple query parsing (can be enhanced)
                std::istringstream iss(query);
                std::string pair;
                while (std::getline(iss, pair, '&')) {
                    size_t eq_pos = pair.find('=');
                    if (eq_pos != std::string::npos) {
                        params[pair.substr(0, eq_pos)] = pair.substr(eq_pos + 1);
                    }
                }
            } else {
                path = url;
            }
        } else {
            path = "/v3/reference/tickers";
        }

        auto response = send_request(core::HttpMethod::Get, path, params);
        
        auto [results_array, pagination] = parse_paginated_response_base(response.body);
        std::vector<Ticker> results;
        
        for (auto result_elem : results_array) {
            auto obj_result = result_elem.get_object();
            if (!obj_result.error()) {
                auto obj = obj_result.value();
            Ticker tick;
            auto ticker_field = obj.find_field_unordered("ticker");
            if (!ticker_field.error()) {
                tick.ticker = std::string(ticker_field.value().get_string().value());
            }
            auto name_field = obj.find_field_unordered("name");
            if (!name_field.error()) {
                tick.name = std::string(name_field.value().get_string().value());
            }
            auto market_field = obj.find_field_unordered("market");
            if (!market_field.error()) {
                tick.market = std::string(market_field.value().get_string().value());
            }
            auto locale_field = obj.find_field_unordered("locale");
            if (!locale_field.error()) {
                tick.locale = std::string(locale_field.value().get_string().value());
            }
            auto primary_exchange_field = obj.find_field_unordered("primary_exchange");
            if (!primary_exchange_field.error()) {
                tick.primary_exchange = std::string(primary_exchange_field.value().get_string().value());
            }
            auto type_field = obj.find_field_unordered("type");
            if (!type_field.error()) {
                tick.type = std::string(type_field.value().get_string().value());
            }
            auto active_field = obj.find_field_unordered("active");
            if (!active_field.error()) {
                tick.active = active_field.value().get_bool().value();
            }
            results.push_back(tick);
            }
        }
        
        return {std::move(results), pagination};
    };

    return PaginatedIterator<Ticker>(fetch_fn);
}

// Paginated iterator for aggregates
PaginatedIterator<Agg> RESTClient::list_aggs_iter(
    const std::string& ticker,
    int multiplier,
    const std::string& timespan,
    const std::string& from,
    const std::string& to,
    std::optional<bool> adjusted,
    const std::optional<std::string>& sort) {
    
    std::map<std::string, std::string> base_params;
    if (adjusted.has_value()) {
        base_params["adjusted"] = adjusted.value() ? "true" : "false";
    }
    if (sort.has_value()) {
        base_params["sort"] = sort.value();
    }

    std::string base_path = "/v2/aggs/ticker/" + ticker + "/range/" + 
                           std::to_string(multiplier) + "/" + timespan + "/" + from + "/" + to;

    auto fetch_fn = [this, base_path, base_params](const std::optional<std::string>& next_url) 
        -> std::pair<std::vector<Agg>, PaginationInfo> {
        
        std::string path = next_url.has_value() ? next_url.value() : base_path;
        std::map<std::string, std::string> params = base_params;
        
        // Extract params from URL if it's a full URL
        if (next_url.has_value() && next_url->find('?') != std::string::npos) {
            size_t query_pos = next_url->find('?');
            path = next_url->substr(0, query_pos);
            std::string query = next_url->substr(query_pos + 1);
            std::istringstream iss(query);
            std::string pair;
            while (std::getline(iss, pair, '&')) {
                size_t eq_pos = pair.find('=');
                if (eq_pos != std::string::npos) {
                    params[pair.substr(0, eq_pos)] = pair.substr(eq_pos + 1);
                }
            }
        }

        auto response = send_request(core::HttpMethod::Get, path, params);
        
        auto [results_array, pagination] = parse_paginated_response_base(response.body);
        std::vector<Agg> results;
        
        for (auto result_elem : results_array) {
            auto obj_result = result_elem.get_object();
            if (!obj_result.error()) {
                auto obj = obj_result.value();
            Agg agg;
            auto open_field = obj.find_field_unordered("o");
            if (!open_field.error()) {
                agg.open = open_field.value().get_double().value();
            }
            auto high_field = obj.find_field_unordered("h");
            if (!high_field.error()) {
                agg.high = high_field.value().get_double().value();
            }
            auto low_field = obj.find_field_unordered("l");
            if (!low_field.error()) {
                agg.low = low_field.value().get_double().value();
            }
            auto close_field = obj.find_field_unordered("c");
            if (!close_field.error()) {
                agg.close = close_field.value().get_double().value();
            }
            auto volume_field = obj.find_field_unordered("v");
            if (!volume_field.error()) {
                agg.volume = volume_field.value().get_int64().value();
            }
            auto vwap_field = obj.find_field_unordered("vw");
            if (!vwap_field.error()) {
                agg.vwap = vwap_field.value().get_double().value();
            }
            auto timestamp_field = obj.find_field_unordered("t");
            if (!timestamp_field.error()) {
                agg.timestamp = timestamp_field.value().get_int64().value();
            }
            auto transactions_field = obj.find_field_unordered("n");
            if (!transactions_field.error()) {
                agg.transactions = transactions_field.value().get_int64().value();
            }
            results.push_back(agg);
            }
        }
        
        return {std::move(results), pagination};
    };

    return PaginatedIterator<Agg>(fetch_fn);
}

// Paginated iterator for trades
PaginatedIterator<Trade> RESTClient::list_trades_iter(
    const std::string& ticker,
    const std::optional<std::string>& timestamp,
    const std::optional<std::string>& timestamp_lt,
    const std::optional<std::string>& timestamp_lte,
    const std::optional<std::string>& timestamp_gt,
    const std::optional<std::string>& timestamp_gte,
    const std::optional<std::string>& sort,
    const std::optional<std::string>& order) {
    
    std::map<std::string, std::string> base_params;
    if (timestamp.has_value()) {
        base_params["timestamp"] = timestamp.value();
    }
    if (timestamp_lt.has_value()) {
        base_params["timestamp.lt"] = timestamp_lt.value();
    }
    if (timestamp_lte.has_value()) {
        base_params["timestamp.lte"] = timestamp_lte.value();
    }
    if (timestamp_gt.has_value()) {
        base_params["timestamp.gt"] = timestamp_gt.value();
    }
    if (timestamp_gte.has_value()) {
        base_params["timestamp.gte"] = timestamp_gte.value();
    }
    if (sort.has_value()) {
        base_params["sort"] = sort.value();
    }
    if (order.has_value()) {
        base_params["order"] = order.value();
    }

    std::string base_path = "/v3/trades/" + ticker;

    auto fetch_fn = [this, base_path, base_params](const std::optional<std::string>& next_url) 
        -> std::pair<std::vector<Trade>, PaginationInfo> {
        
        std::string path = next_url.has_value() ? next_url.value() : base_path;
        std::map<std::string, std::string> params = base_params;
        
        if (next_url.has_value() && next_url->find('?') != std::string::npos) {
            size_t query_pos = next_url->find('?');
            path = next_url->substr(0, query_pos);
            std::string query = next_url->substr(query_pos + 1);
            std::istringstream iss(query);
            std::string pair;
            while (std::getline(iss, pair, '&')) {
                size_t eq_pos = pair.find('=');
                if (eq_pos != std::string::npos) {
                    params[pair.substr(0, eq_pos)] = pair.substr(eq_pos + 1);
                }
            }
        }

        auto response = send_request(core::HttpMethod::Get, path, params);
        
        auto [results_array, pagination] = parse_paginated_response_base(response.body);
        std::vector<Trade> results;
        
        for (auto result_elem : results_array) {
            auto obj_result = result_elem.get_object();
            if (!obj_result.error()) {
                auto obj = obj_result.value();
            Trade trade;
            auto price_field = obj.find_field_unordered("p");
            if (!price_field.error()) {
                trade.price = price_field.value().get_double().value();
            }
            auto size_field = obj.find_field_unordered("s");
            if (!size_field.error()) {
                trade.size = size_field.value().get_int64().value();
            }
            auto timestamp_field = obj.find_field_unordered("t");
            if (!timestamp_field.error()) {
                trade.timestamp = timestamp_field.value().get_int64().value();
            }
            results.push_back(trade);
            }
        }
        
        return {std::move(results), pagination};
    };

    return PaginatedIterator<Trade>(fetch_fn);
}

// Paginated iterator for quotes
PaginatedIterator<Quote> RESTClient::list_quotes_iter(
    const std::string& ticker,
    const std::optional<std::string>& timestamp,
    const std::optional<std::string>& timestamp_lt,
    const std::optional<std::string>& timestamp_lte,
    const std::optional<std::string>& timestamp_gt,
    const std::optional<std::string>& timestamp_gte,
    const std::optional<std::string>& sort,
    const std::optional<std::string>& order) {
    
    std::map<std::string, std::string> base_params;
    if (timestamp.has_value()) {
        base_params["timestamp"] = timestamp.value();
    }
    if (timestamp_lt.has_value()) {
        base_params["timestamp.lt"] = timestamp_lt.value();
    }
    if (timestamp_lte.has_value()) {
        base_params["timestamp.lte"] = timestamp_lte.value();
    }
    if (timestamp_gt.has_value()) {
        base_params["timestamp.gt"] = timestamp_gt.value();
    }
    if (timestamp_gte.has_value()) {
        base_params["timestamp.gte"] = timestamp_gte.value();
    }
    if (sort.has_value()) {
        base_params["sort"] = sort.value();
    }
    if (order.has_value()) {
        base_params["order"] = order.value();
    }

    std::string base_path = "/v3/quotes/" + ticker;

    auto fetch_fn = [this, base_path, base_params](const std::optional<std::string>& next_url) 
        -> std::pair<std::vector<Quote>, PaginationInfo> {
        
        std::string path = next_url.has_value() ? next_url.value() : base_path;
        std::map<std::string, std::string> params = base_params;
        
        if (next_url.has_value() && next_url->find('?') != std::string::npos) {
            size_t query_pos = next_url->find('?');
            path = next_url->substr(0, query_pos);
            std::string query = next_url->substr(query_pos + 1);
            std::istringstream iss(query);
            std::string pair;
            while (std::getline(iss, pair, '&')) {
                size_t eq_pos = pair.find('=');
                if (eq_pos != std::string::npos) {
                    params[pair.substr(0, eq_pos)] = pair.substr(eq_pos + 1);
                }
            }
        }

        auto response = send_request(core::HttpMethod::Get, path, params);
        
        auto [results_array, pagination] = parse_paginated_response_base(response.body);
        std::vector<Quote> results;
        
        for (auto result_elem : results_array) {
            auto obj_result = result_elem.get_object();
            if (!obj_result.error()) {
                auto obj = obj_result.value();
            Quote quote;
            auto ask_field = obj.find_field_unordered("ap");
            if (!ask_field.error()) {
                quote.ask = ask_field.value().get_double().value();
            }
            auto ask_size_field = obj.find_field_unordered("as");
            if (!ask_size_field.error()) {
                quote.ask_size = ask_size_field.value().get_int64().value();
            }
            auto ask_exchange_field = obj.find_field_unordered("ax");
            if (!ask_exchange_field.error()) {
                quote.ask_exchange = ask_exchange_field.value().get_int64().value();
            }
            auto bid_field = obj.find_field_unordered("bp");
            if (!bid_field.error()) {
                quote.bid = bid_field.value().get_double().value();
            }
            auto bid_size_field = obj.find_field_unordered("bs");
            if (!bid_size_field.error()) {
                quote.bid_size = bid_size_field.value().get_int64().value();
            }
            auto bid_exchange_field = obj.find_field_unordered("bx");
            if (!bid_exchange_field.error()) {
                quote.bid_exchange = bid_exchange_field.value().get_int64().value();
            }
            auto timestamp_field = obj.find_field_unordered("t");
            if (!timestamp_field.error()) {
                quote.timestamp = timestamp_field.value().get_int64().value();
            }
            auto exchange_field = obj.find_field_unordered("x");
            if (!exchange_field.error()) {
                quote.exchange = exchange_field.value().get_int64().value();
            }
            results.push_back(quote);
            }
        }
        
        return {std::move(results), pagination};
    };

    return PaginatedIterator<Quote>(fetch_fn);
}

} // namespace massive::rest

