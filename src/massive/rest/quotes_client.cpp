#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

std::vector<Quote> RESTClient::list_quotes(const std::string &ticker,
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

    std::string path = "/v3/quotes/" + ticker;
    auto response = send_request(core::HttpMethod::Get, path, params);

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

// Quotes - Last Forex Quote
LastForexQuote RESTClient::get_last_forex_quote(const std::string& from, const std::string& to) {
    std::string path = "/v1/last_quote/currencies/" + from + "/" + to;
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

    LastForexQuote last_forex_quote;
    auto obj = root_obj.value();

    auto symbol_field = obj.find_field_unordered("symbol");
    if (!symbol_field.error()) {
        last_forex_quote.symbol = std::string(symbol_field.value().get_string().value());
    }

    auto last_field = obj.find_field_unordered("last");
    if (!last_field.error()) {
        auto last_obj = last_field.value().get_object();
        if (!last_obj.error()) {
            ForexQuote forex_quote;
            auto last_obj_val = last_obj.value();

            auto ask_field = last_obj_val.find_field_unordered("ask");
            if (!ask_field.error()) {
                forex_quote.ask = ask_field.value().get_double().value();
            }

            auto bid_field = last_obj_val.find_field_unordered("bid");
            if (!bid_field.error()) {
                forex_quote.bid = bid_field.value().get_double().value();
            }

            auto exchange_field = last_obj_val.find_field_unordered("exchange");
            if (!exchange_field.error()) {
                forex_quote.exchange = exchange_field.value().get_int64().value();
            }

            auto timestamp_field = last_obj_val.find_field_unordered("timestamp");
            if (!timestamp_field.error()) {
                forex_quote.timestamp = timestamp_field.value().get_int64().value();
            }

            last_forex_quote.last = forex_quote;
        }
    }

    return last_forex_quote;
}

// Quotes - Real-Time Currency Conversion
RealTimeCurrencyConversion RESTClient::get_real_time_currency_conversion(
    const std::string& from,
    const std::string& to,
    std::optional<double> amount,
    std::optional<int> precision) {
    std::map<std::string, std::string> params;
    if (amount.has_value()) {
        params["amount"] = std::to_string(amount.value());
    }
    if (precision.has_value()) {
        params["precision"] = std::to_string(precision.value());
    }

    std::string path = "/v1/conversion/" + from + "/" + to;
    auto response = send_request(core::HttpMethod::Get, path, params);

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

    RealTimeCurrencyConversion conversion;
    auto obj = root_obj.value();

    auto from_field = obj.find_field_unordered("from");
    if (!from_field.error()) {
        conversion.from = std::string(from_field.value().get_string().value());
    }

    auto to_field = obj.find_field_unordered("to");
    if (!to_field.error()) {
        conversion.to = std::string(to_field.value().get_string().value());
    }

    auto initial_amount_field = obj.find_field_unordered("initialAmount");
    if (!initial_amount_field.error()) {
        conversion.initial_amount = initial_amount_field.value().get_double().value();
    }

    auto converted_field = obj.find_field_unordered("converted");
    if (!converted_field.error()) {
        conversion.converted = converted_field.value().get_double().value();
    }

    auto last_field = obj.find_field_unordered("last");
    if (!last_field.error()) {
        auto last_obj = last_field.value().get_object();
        if (!last_obj.error()) {
            ForexQuote forex_quote;
            auto last_obj_val = last_obj.value();

            auto ask_field = last_obj_val.find_field_unordered("ask");
            if (!ask_field.error()) {
                forex_quote.ask = ask_field.value().get_double().value();
            }

            auto bid_field = last_obj_val.find_field_unordered("bid");
            if (!bid_field.error()) {
                forex_quote.bid = bid_field.value().get_double().value();
            }

            auto exchange_field = last_obj_val.find_field_unordered("exchange");
            if (!exchange_field.error()) {
                forex_quote.exchange = exchange_field.value().get_int64().value();
            }

            auto timestamp_field = last_obj_val.find_field_unordered("timestamp");
            if (!timestamp_field.error()) {
                forex_quote.timestamp = timestamp_field.value().get_int64().value();
            }

            conversion.last = forex_quote;
        }
    }

    return conversion;
}

} // namespace massive::rest
