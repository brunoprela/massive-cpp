#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

std::vector<Trade> RESTClient::list_trades(const std::string &ticker,
                                           const std::optional<std::string> &timestamp,
                                           const std::optional<std::string> &timestamp_lt,
                                           const std::optional<std::string> &timestamp_lte,
                                           const std::optional<std::string> &timestamp_gt,
                                           const std::optional<std::string> &timestamp_gte,
                                           std::optional<int> limit,
                                           const std::optional<std::string> &sort,
                                           const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (timestamp.has_value()) {
        params["timestamp"] = timestamp.value();
    }
    if (timestamp_lt.has_value()) {
        params["timestamp.lt"] = timestamp_lt.value();
    }
    if (timestamp_lte.has_value()) {
        params["timestamp.lte"] = timestamp_lte.value();
    }
    if (timestamp_gt.has_value()) {
        params["timestamp.gt"] = timestamp_gt.value();
    }
    if (timestamp_gte.has_value()) {
        params["timestamp.gte"] = timestamp_gte.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/v3/trades/" + ticker;
    auto response = send_request(core::HttpMethod::Get, path, params);

    // Parse JSON response
    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<Trade> results;

    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result : results_array.value()) {
                Trade trade;
                auto obj_result = result.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();

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
        }
    }

    return results;
}

LastTrade RESTClient::get_last_trade(const std::string &ticker) {
    std::string path = "/v2/last/trade/" + ticker;
    auto response = send_request(core::HttpMethod::Get, path);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    LastTrade last_trade;
    auto result_field = root_obj.value().find_field_unordered("results");
    if (!result_field.error()) {
        auto obj_result = result_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto ticker_field = obj.find_field_unordered("T");
            if (!ticker_field.error()) {
                last_trade.ticker = std::string(ticker_field.value().get_string().value());
            }

            auto price_field = obj.find_field_unordered("p");
            if (!price_field.error()) {
                last_trade.price = price_field.value().get_double().value();
            }

            auto size_field = obj.find_field_unordered("s");
            if (!size_field.error()) {
                last_trade.size = size_field.value().get_int64().value();
            }

            auto timestamp_field = obj.find_field_unordered("t");
            if (!timestamp_field.error()) {
                last_trade.timestamp = timestamp_field.value().get_int64().value();
            }
        }
    }

    return last_trade;
}

// Trades - Last Crypto Trade
CryptoTrade RESTClient::get_last_crypto_trade(const std::string& from, const std::string& to) {
    std::string path = "/v1/last/crypto/" + from + "/" + to;
    auto response = send_request(core::HttpMethod::Get, path);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    CryptoTrade trade;
    auto last_field = root_obj.value().find_field_unordered("last");
    if (!last_field.error()) {
        auto last_obj = last_field.value().get_object();
        if (!last_obj.error()) {
            auto obj = last_obj.value();

            auto price_field = obj.find_field_unordered("price");
            if (!price_field.error()) {
                trade.price = price_field.value().get_double().value();
            }

            auto size_field = obj.find_field_unordered("size");
            if (!size_field.error()) {
                trade.size = size_field.value().get_double().value();
            }

            auto timestamp_field = obj.find_field_unordered("timestamp");
            if (!timestamp_field.error()) {
                trade.timestamp = timestamp_field.value().get_int64().value();
            }

            auto exchange_field = obj.find_field_unordered("exchange");
            if (!exchange_field.error()) {
                trade.exchange = exchange_field.value().get_int64().value();
            }

            auto conditions_field = obj.find_field_unordered("conditions");
            if (!conditions_field.error()) {
                auto conditions_array = conditions_field.value().get_array();
                if (!conditions_array.error()) {
                    std::vector<std::int64_t> conditions;
                    for (auto cond_elem : conditions_array.value()) {
                        conditions.push_back(cond_elem.get_int64().value());
                    }
                    trade.conditions = conditions;
                }
            }
        }
    }

    return trade;
}

} // namespace massive::rest
