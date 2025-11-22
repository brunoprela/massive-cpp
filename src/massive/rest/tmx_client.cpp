#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

// TMX - Trades
std::vector<TmxTrade> RESTClient::list_tmx_trades(const std::string &ticker,
                                                   const std::optional<std::string> &timestamp,
                                                   std::optional<int> limit,
                                                   const std::optional<std::string> &sort) {
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
    params["exchange"] = "T";
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

    std::vector<TmxTrade> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    TmxTrade trade;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        trade.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto price_field = obj.find_field_unordered("price");
                    if (!price_field.error()) {
                        trade.price = price_field.value().get_double().value();
                    }

                    auto size_field = obj.find_field_unordered("size");
                    if (!size_field.error()) {
                        trade.size = size_field.value().get_int64().value();
                    }

                    auto timestamp_field = obj.find_field_unordered("timestamp");
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

TmxTrade RESTClient::get_tmx_last_trade(const std::string &ticker) {
    std::map<std::string, std::string> params;
    params["exchange"] = "T";
    std::string path = "/v2/last/trade/" + ticker;
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

    TmxTrade trade;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto ticker_field = obj.find_field_unordered("T");
            if (!ticker_field.error()) {
                trade.ticker = std::string(ticker_field.value().get_string().value());
            }

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
        }
    }

    return trade;
}

// TMX - Quotes
std::vector<TmxQuote> RESTClient::list_tmx_quotes(const std::string &ticker,
                                                  const std::optional<std::string> &timestamp,
                                                  std::optional<int> limit,
                                                  const std::optional<std::string> &sort) {
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
    params["exchange"] = "T";
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

    std::vector<TmxQuote> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    TmxQuote quote;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        quote.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto bid_price_field = obj.find_field_unordered("bid_price");
                    if (!bid_price_field.error()) {
                        quote.bid_price = bid_price_field.value().get_double().value();
                    }

                    auto ask_price_field = obj.find_field_unordered("ask_price");
                    if (!ask_price_field.error()) {
                        quote.ask_price = ask_price_field.value().get_double().value();
                    }

                    auto timestamp_field = obj.find_field_unordered("timestamp");
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

TmxQuote RESTClient::get_tmx_last_quote(const std::string &ticker) {
    std::map<std::string, std::string> params;
    params["exchange"] = "T";
    std::string path = "/v2/last/quote/" + ticker;
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

    TmxQuote quote;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto ticker_field = obj.find_field_unordered("T");
            if (!ticker_field.error()) {
                quote.ticker = std::string(ticker_field.value().get_string().value());
            }

            auto bid_price_field = obj.find_field_unordered("bp");
            if (!bid_price_field.error()) {
                quote.bid_price = bid_price_field.value().get_double().value();
            }

            auto ask_price_field = obj.find_field_unordered("ap");
            if (!ask_price_field.error()) {
                quote.ask_price = ask_price_field.value().get_double().value();
            }

            auto timestamp_field = obj.find_field_unordered("t");
            if (!timestamp_field.error()) {
                quote.timestamp = timestamp_field.value().get_int64().value();
            }
        }
    }

    return quote;
}

// TMX - Aggregates
std::vector<TmxAgg> RESTClient::list_tmx_aggs(const std::string &ticker, int multiplier,
                                              const std::string &timespan, const std::string &from,
                                              const std::string &to, std::optional<bool> adjusted,
                                              std::optional<std::string> sort,
                                              std::optional<int> limit) {
    std::map<std::string, std::string> params;
    params["exchange"] = "T";
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

    std::vector<TmxAgg> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    TmxAgg agg;

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

                    auto timestamp_field = obj.find_field_unordered("t");
                    if (!timestamp_field.error()) {
                        agg.timestamp = timestamp_field.value().get_int64().value();
                    }

                    results.push_back(agg);
                }
            }
        }
    }

    return results;
}

// TMX - Ticker Details
TmxTickerDetails RESTClient::get_tmx_ticker_details(const std::string &ticker) {
    std::map<std::string, std::string> params;
    params["exchange"] = "T";
    std::string path = "/v3/reference/tickers/" + ticker;
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

    TmxTickerDetails details;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto ticker_field = obj.find_field_unordered("ticker");
            if (!ticker_field.error()) {
                details.ticker = std::string(ticker_field.value().get_string().value());
            }

            auto name_field = obj.find_field_unordered("name");
            if (!name_field.error()) {
                details.name = std::string(name_field.value().get_string().value());
            }

            auto exchange_field = obj.find_field_unordered("exchange");
            if (!exchange_field.error()) {
                details.exchange = std::string(exchange_field.value().get_string().value());
            }

            auto market_field = obj.find_field_unordered("market");
            if (!market_field.error()) {
                details.market = std::string(market_field.value().get_string().value());
            }

            auto type_field = obj.find_field_unordered("type");
            if (!type_field.error()) {
                details.type = std::string(type_field.value().get_string().value());
            }

            auto active_field = obj.find_field_unordered("active");
            if (!active_field.error()) {
                details.active = active_field.value().get_bool().value();
            }

            auto currency_field = obj.find_field_unordered("currency");
            if (!currency_field.error()) {
                details.currency = std::string(currency_field.value().get_string().value());
            }
        }
    }

    return details;
}

// TMX - Corporate Events
std::vector<TmxCorporateEvent> RESTClient::list_tmx_corporate_events(
    const std::optional<std::string>& date,
    const std::optional<std::string>& date_any_of,
    const std::optional<std::string>& date_gt,
    const std::optional<std::string>& date_gte,
    const std::optional<std::string>& date_lt,
    const std::optional<std::string>& date_lte,
    const std::optional<std::string>& type,
    const std::optional<std::string>& ticker,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (date_any_of.has_value()) {
        params["date.any_of"] = date_any_of.value();
    }
    if (date_gt.has_value()) {
        params["date.gt"] = date_gt.value();
    }
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (date_lt.has_value()) {
        params["date.lt"] = date_lt.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
    }
    if (type.has_value()) {
        params["type"] = type.value();
    }
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/tmx/v1/corporate-events";
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

    std::vector<TmxCorporateEvent> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    TmxCorporateEvent event;

                    auto company_name_field = obj.find_field_unordered("company_name");
                    if (!company_name_field.error()) {
                        event.company_name = std::string(company_name_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        event.date = std::string(date_field.value().get_string().value());
                    }

                    auto isin_field = obj.find_field_unordered("isin");
                    if (!isin_field.error()) {
                        event.isin = std::string(isin_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        event.name = std::string(name_field.value().get_string().value());
                    }

                    auto status_field = obj.find_field_unordered("status");
                    if (!status_field.error()) {
                        event.status = std::string(status_field.value().get_string().value());
                    }

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        event.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto tmx_company_id_field = obj.find_field_unordered("tmx_company_id");
                    if (!tmx_company_id_field.error()) {
                        event.tmx_company_id = tmx_company_id_field.value().get_int64().value();
                    }

                    auto tmx_record_id_field = obj.find_field_unordered("tmx_record_id");
                    if (!tmx_record_id_field.error()) {
                        event.tmx_record_id = std::string(tmx_record_id_field.value().get_string().value());
                    }

                    auto trading_venue_field = obj.find_field_unordered("trading_venue");
                    if (!trading_venue_field.error()) {
                        event.trading_venue = std::string(trading_venue_field.value().get_string().value());
                    }

                    auto type_field = obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        event.type = std::string(type_field.value().get_string().value());
                    }

                    auto url_field = obj.find_field_unordered("url");
                    if (!url_field.error()) {
                        event.url = std::string(url_field.value().get_string().value());
                    }

                    results.push_back(event);
                }
            }
        }
    }

    return results;
}

} // namespace massive::rest

