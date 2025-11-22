#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

std::vector<Agg> RESTClient::list_aggs(const std::string &ticker, int multiplier,
                                       const std::string &timespan, const std::string &from,
                                       const std::string &to, std::optional<bool> adjusted,
                                       std::optional<std::string> sort, std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (adjusted.has_value()) {
        params["adjusted"] = adjusted.value() ? "true" : "false";
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/v2/aggs/ticker/" + ticker + "/range/" + std::to_string(multiplier) + "/" +
                       timespan + "/" + from + "/" + to;
    auto response = send_request(core::HttpMethod::Get, path, params);

    // Parse JSON response using simdjson
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<Agg> results;

    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result : results_array.value()) {
                Agg agg;
                auto obj_result = result.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();

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
                        agg.volume = volume_field.value().get_double().value();
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

                    auto otc_field = obj.find_field_unordered("otc");
                    if (!otc_field.error()) {
                        agg.otc = otc_field.value().get_bool().value();
                    }

                    results.push_back(agg);
                }
            }
        }
    }

    return results;
}

std::vector<Trade> RESTClient::list_trades(const std::string &ticker,
                                           std::optional<std::string> timestamp,
                                           std::optional<int> limit,
                                           std::optional<std::string> sort) {
    std::map<std::string, std::string> params;
    if (timestamp.has_value()) {
        params["timestamp"] = timestamp.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/v3/trades/" + ticker;
    auto response = send_request(core::HttpMethod::Get, path, params);

    // Parse JSON response
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
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

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
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

std::vector<Quote> RESTClient::list_quotes(const std::string &ticker,
                                           std::optional<std::string> timestamp,
                                           std::optional<int> limit,
                                           std::optional<std::string> sort) {
    std::map<std::string, std::string> params;
    if (timestamp.has_value()) {
        params["timestamp"] = timestamp.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/v3/quotes/" + ticker;
    auto response = send_request(core::HttpMethod::Get, path, params);

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<Quote> results;

    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result : results_array.value()) {
                Quote quote;
                auto obj_result = result.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();

                    auto ask_field = obj.find_field_unordered("ap");
                    if (!ask_field.error()) {
                        quote.ask = ask_field.value().get_double().value();
                    }

                    auto bid_field = obj.find_field_unordered("bp");
                    if (!bid_field.error()) {
                        quote.bid = bid_field.value().get_double().value();
                    }

                    auto timestamp_field = obj.find_field_unordered("t");
                    if (!timestamp_field.error()) {
                        quote.timestamp = timestamp_field.value().get_int64().value();
                    }

                    results.push_back(quote);
                }
            }
        }
    }

    return results;
}

LastQuote RESTClient::get_last_quote(const std::string &ticker) {
    std::string path = "/v2/last/quote/" + ticker;
    auto response = send_request(core::HttpMethod::Get, path);

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    LastQuote last_quote;
    auto result_field = root_obj.value().find_field_unordered("results");
    if (!result_field.error()) {
        auto obj_result = result_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto ticker_field = obj.find_field_unordered("T");
            if (!ticker_field.error()) {
                last_quote.ticker = std::string(ticker_field.value().get_string().value());
            }

            auto ask_field = obj.find_field_unordered("ap");
            if (!ask_field.error()) {
                last_quote.ask = ask_field.value().get_double().value();
            }

            auto bid_field = obj.find_field_unordered("bp");
            if (!bid_field.error()) {
                last_quote.bid = bid_field.value().get_double().value();
            }

            auto timestamp_field = obj.find_field_unordered("t");
            if (!timestamp_field.error()) {
                last_quote.timestamp = timestamp_field.value().get_int64().value();
            }
        }
    }

    return last_quote;
}

std::vector<GroupedDailyAgg> RESTClient::get_grouped_daily_aggs(const std::string &date,
                                                                std::optional<bool> adjusted,
                                                                const std::string &locale,
                                                                const std::string &market_type,
                                                                bool include_otc) {
    std::map<std::string, std::string> params;
    if (adjusted.has_value()) {
        params["adjusted"] = adjusted.value() ? "true" : "false";
    }
    if (include_otc) {
        params["include_otc"] = "true";
    }

    std::string path = "/v2/aggs/grouped/locale/" + locale + "/market/" + market_type + "/" + date;
    auto response = send_request(core::HttpMethod::Get, path, params);

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<GroupedDailyAgg> results;

    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result : results_array.value()) {
                GroupedDailyAgg agg;
                auto obj_result = result.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();

                    auto ticker_field = obj.find_field_unordered("T");
                    if (!ticker_field.error()) {
                        agg.ticker = std::string(ticker_field.value().get_string().value());
                    }

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
                        agg.volume = volume_field.value().get_double().value();
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

                    auto otc_field = obj.find_field_unordered("otc");
                    if (!otc_field.error()) {
                        agg.otc = otc_field.value().get_bool().value();
                    }

                    results.push_back(agg);
                }
            }
        }
    }

    return results;
}

DailyOpenCloseAgg RESTClient::get_daily_open_close_agg(const std::string &ticker,
                                                       const std::string &date,
                                                       std::optional<bool> adjusted) {
    std::map<std::string, std::string> params;
    if (adjusted.has_value()) {
        params["adjusted"] = adjusted.value() ? "true" : "false";
    }

    std::string path = "/v1/open-close/" + ticker + "/" + date;
    auto response = send_request(core::HttpMethod::Get, path, params);

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    DailyOpenCloseAgg agg;
    auto obj = root_obj.value();

    auto after_hours_field = obj.find_field_unordered("afterHours");
    if (!after_hours_field.error()) {
        agg.after_hours = after_hours_field.value().get_double().value();
    }

    auto close_field = obj.find_field_unordered("close");
    if (!close_field.error()) {
        agg.close = close_field.value().get_double().value();
    }

    auto from_field = obj.find_field_unordered("from");
    if (!from_field.error()) {
        agg.from = std::string(from_field.value().get_string().value());
    }

    auto high_field = obj.find_field_unordered("high");
    if (!high_field.error()) {
        agg.high = high_field.value().get_double().value();
    }

    auto low_field = obj.find_field_unordered("low");
    if (!low_field.error()) {
        agg.low = low_field.value().get_double().value();
    }

    auto open_field = obj.find_field_unordered("open");
    if (!open_field.error()) {
        agg.open = open_field.value().get_double().value();
    }

    auto pre_market_field = obj.find_field_unordered("preMarket");
    if (!pre_market_field.error()) {
        agg.pre_market = pre_market_field.value().get_double().value();
    }

    auto status_field = obj.find_field_unordered("status");
    if (!status_field.error()) {
        agg.status = std::string(status_field.value().get_string().value());
    }

    auto symbol_field = obj.find_field_unordered("symbol");
    if (!symbol_field.error()) {
        agg.symbol = std::string(symbol_field.value().get_string().value());
    }

    auto volume_field = obj.find_field_unordered("volume");
    if (!volume_field.error()) {
        agg.volume = volume_field.value().get_double().value();
    }

    auto otc_field = obj.find_field_unordered("otc");
    if (!otc_field.error()) {
        agg.otc = otc_field.value().get_bool().value();
    }

    return agg;
}

PreviousCloseAgg RESTClient::get_previous_close_agg(const std::string &ticker,
                                                    std::optional<bool> adjusted) {
    std::map<std::string, std::string> params;
    if (adjusted.has_value()) {
        params["adjusted"] = adjusted.value() ? "true" : "false";
    }

    std::string path = "/v2/aggs/ticker/" + ticker + "/prev";
    auto response = send_request(core::HttpMethod::Get, path, params);

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    PreviousCloseAgg agg;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto ticker_field = obj.find_field_unordered("T");
            if (!ticker_field.error()) {
                agg.ticker = std::string(ticker_field.value().get_string().value());
            }

            auto close_field = obj.find_field_unordered("c");
            if (!close_field.error()) {
                agg.close = close_field.value().get_double().value();
            }

            auto high_field = obj.find_field_unordered("h");
            if (!high_field.error()) {
                agg.high = high_field.value().get_double().value();
            }

            auto low_field = obj.find_field_unordered("l");
            if (!low_field.error()) {
                agg.low = low_field.value().get_double().value();
            }

            auto open_field = obj.find_field_unordered("o");
            if (!open_field.error()) {
                agg.open = open_field.value().get_double().value();
            }

            auto timestamp_field = obj.find_field_unordered("t");
            if (!timestamp_field.error()) {
                agg.timestamp = timestamp_field.value().get_double().value();
            }

            auto volume_field = obj.find_field_unordered("v");
            if (!volume_field.error()) {
                agg.volume = volume_field.value().get_double().value();
            }

            auto vwap_field = obj.find_field_unordered("vw");
            if (!vwap_field.error()) {
                agg.vwap = vwap_field.value().get_double().value();
            }
        }
    }

    return agg;
}

// Aggregates - get_aggs (non-iterator version)
std::vector<Agg> RESTClient::get_aggs(
    const std::string& ticker,
    int multiplier,
    const std::string& timespan,
    const std::string& from,
    const std::string& to,
    std::optional<bool> adjusted,
    std::optional<std::string> sort,
    std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (adjusted.has_value()) {
        params["adjusted"] = adjusted.value() ? "true" : "false";
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/v2/aggs/ticker/" + ticker + "/range/" + std::to_string(multiplier) + "/" + timespan + "/" + from + "/" + to;
    auto response = send_request(core::HttpMethod::Get, path, params);

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<Agg> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
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
                        agg.volume = volume_field.value().get_double().value();
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
        }
    }

    return results;
}

} // namespace massive::rest
