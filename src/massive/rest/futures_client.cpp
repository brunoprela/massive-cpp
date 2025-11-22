#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

// Futures - Aggregates
std::vector<FuturesAgg>
RESTClient::list_futures_aggregates(const std::string &ticker,
                                    const std::optional<std::string> &resolution,
                                    std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (resolution.has_value()) {
        params["resolution"] = resolution.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/futures/vX/aggs/" + ticker;
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

    std::vector<FuturesAgg> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FuturesAgg agg;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        agg.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto open_field = obj.find_field_unordered("open");
                    if (!open_field.error()) {
                        agg.open = open_field.value().get_double().value();
                    }

                    auto high_field = obj.find_field_unordered("high");
                    if (!high_field.error()) {
                        agg.high = high_field.value().get_double().value();
                    }

                    auto low_field = obj.find_field_unordered("low");
                    if (!low_field.error()) {
                        agg.low = low_field.value().get_double().value();
                    }

                    auto close_field = obj.find_field_unordered("close");
                    if (!close_field.error()) {
                        agg.close = close_field.value().get_double().value();
                    }

                    auto volume_field = obj.find_field_unordered("volume");
                    if (!volume_field.error()) {
                        agg.volume = volume_field.value().get_double().value();
                    }

                    auto dollar_volume_field = obj.find_field_unordered("dollar_volume");
                    if (!dollar_volume_field.error()) {
                        agg.dollar_volume = dollar_volume_field.value().get_double().value();
                    }

                    results.push_back(agg);
                }
            }
        }
    }

    return results;
}

// Futures - Contract Details
FuturesContract RESTClient::get_futures_contract_details(const std::string &ticker) {
    std::string path = "/futures/vX/contracts/" + ticker;
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

    FuturesContract contract;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto ticker_field = obj.find_field_unordered("ticker");
            if (!ticker_field.error()) {
                contract.ticker = std::string(ticker_field.value().get_string().value());
            }

            auto product_code_field = obj.find_field_unordered("product_code");
            if (!product_code_field.error()) {
                contract.product_code =
                    std::string(product_code_field.value().get_string().value());
            }

            auto name_field = obj.find_field_unordered("name");
            if (!name_field.error()) {
                contract.name = std::string(name_field.value().get_string().value());
            }

            auto trading_venue_field = obj.find_field_unordered("trading_venue");
            if (!trading_venue_field.error()) {
                contract.trading_venue =
                    std::string(trading_venue_field.value().get_string().value());
            }

            auto active_field = obj.find_field_unordered("active");
            if (!active_field.error()) {
                contract.active = active_field.value().get_bool().value();
            }

            auto first_trade_date_field = obj.find_field_unordered("first_trade_date");
            if (!first_trade_date_field.error()) {
                contract.first_trade_date =
                    std::string(first_trade_date_field.value().get_string().value());
            }

            auto last_trade_date_field = obj.find_field_unordered("last_trade_date");
            if (!last_trade_date_field.error()) {
                contract.last_trade_date =
                    std::string(last_trade_date_field.value().get_string().value());
            }
        }
    }

    return contract;
}

// Futures - List Contracts
std::vector<FuturesContract>
RESTClient::list_futures_contracts(const std::optional<std::string> &product_code,
                                   std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (product_code.has_value()) {
        params["product_code"] = product_code.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/futures/vX/contracts";
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

    std::vector<FuturesContract> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FuturesContract contract;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        contract.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto product_code_field = obj.find_field_unordered("product_code");
                    if (!product_code_field.error()) {
                        contract.product_code =
                            std::string(product_code_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        contract.name = std::string(name_field.value().get_string().value());
                    }

                    auto active_field = obj.find_field_unordered("active");
                    if (!active_field.error()) {
                        contract.active = active_field.value().get_bool().value();
                    }

                    results.push_back(contract);
                }
            }
        }
    }

    return results;
}

// Futures - Quotes
std::vector<FuturesQuote> RESTClient::list_futures_quotes(const std::string &ticker,
                                                          std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/futures/vX/quotes/" + ticker;
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

    std::vector<FuturesQuote> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FuturesQuote quote;

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

                    auto sip_timestamp_field = obj.find_field_unordered("sip_timestamp");
                    if (!sip_timestamp_field.error()) {
                        quote.sip_timestamp = sip_timestamp_field.value().get_int64().value();
                    }

                    results.push_back(quote);
                }
            }
        }
    }

    return results;
}

// Futures - Trades
std::vector<FuturesTrade> RESTClient::list_futures_trades(const std::string &ticker,
                                                          std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/futures/vX/trades/" + ticker;
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

    std::vector<FuturesTrade> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FuturesTrade trade;

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

// Futures - Additional Methods
std::vector<FuturesProduct> RESTClient::list_futures_products(
    const std::optional<std::string>& name,
    const std::optional<std::string>& name_search,
    const std::optional<std::string>& as_of,
    const std::optional<std::string>& trading_venue,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (name.has_value()) {
        params["name"] = name.value();
    }
    if (name_search.has_value()) {
        params["name_search"] = name_search.value();
    }
    if (as_of.has_value()) {
        params["as_of"] = as_of.value();
    }
    if (trading_venue.has_value()) {
        params["trading_venue"] = trading_venue.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/futures/vX/products";
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

    std::vector<FuturesProduct> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FuturesProduct product;

                    auto product_code_field = obj.find_field_unordered("product_code");
                    if (!product_code_field.error()) {
                        product.product_code = std::string(product_code_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        product.name = std::string(name_field.value().get_string().value());
                    }

                    auto description_field = obj.find_field_unordered("description");
                    if (!description_field.error()) {
                        product.description = std::string(description_field.value().get_string().value());
                    }

                    auto exchange_field = obj.find_field_unordered("exchange");
                    if (!exchange_field.error()) {
                        product.exchange = std::string(exchange_field.value().get_string().value());
                    }

                    auto type_field = obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        product.type = std::string(type_field.value().get_string().value());
                    }

                    auto currency_field = obj.find_field_unordered("currency");
                    if (!currency_field.error()) {
                        product.currency = std::string(currency_field.value().get_string().value());
                    }

                    results.push_back(product);
                }
            }
        }
    }

    return results;
}

FuturesProduct RESTClient::get_futures_product_details(
    const std::string& product_code,
    const std::optional<std::string>& type,
    const std::optional<std::string>& as_of) {
    std::map<std::string, std::string> params;
    if (type.has_value()) {
        params["type"] = type.value();
    }
    if (as_of.has_value()) {
        params["as_of"] = as_of.value();
    }

    std::string path = "/futures/vX/products/" + product_code;
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

    FuturesProduct product;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj = results_field.value().get_object();
        if (!obj.error()) {
            auto obj_val = obj.value();

            auto product_code_field = obj_val.find_field_unordered("product_code");
            if (!product_code_field.error()) {
                product.product_code = std::string(product_code_field.value().get_string().value());
            }

            auto name_field = obj_val.find_field_unordered("name");
            if (!name_field.error()) {
                product.name = std::string(name_field.value().get_string().value());
            }

            auto description_field = obj_val.find_field_unordered("description");
            if (!description_field.error()) {
                product.description = std::string(description_field.value().get_string().value());
            }

            auto exchange_field = obj_val.find_field_unordered("exchange");
            if (!exchange_field.error()) {
                product.exchange = std::string(exchange_field.value().get_string().value());
            }

            auto type_field = obj_val.find_field_unordered("type");
            if (!type_field.error()) {
                product.type = std::string(type_field.value().get_string().value());
            }

            auto currency_field = obj_val.find_field_unordered("currency");
            if (!currency_field.error()) {
                product.currency = std::string(currency_field.value().get_string().value());
            }
        }
    }

    return product;
}

FuturesSchedule RESTClient::get_futures_schedule(const std::string& ticker) {
    std::string path = "/v3/reference/futures/schedules/" + ticker;
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

    FuturesSchedule schedule;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto session_open_field = obj.find_field_unordered("session_open");
            if (!session_open_field.error()) {
                schedule.session_open = std::string(session_open_field.value().get_string().value());
            }

            auto session_close_field = obj.find_field_unordered("session_close");
            if (!session_close_field.error()) {
                schedule.session_close = std::string(session_close_field.value().get_string().value());
            }

            auto timezone_field = obj.find_field_unordered("timezone");
            if (!timezone_field.error()) {
                schedule.timezone = std::string(timezone_field.value().get_string().value());
            }

            auto trading_days_field = obj.find_field_unordered("trading_days");
            if (!trading_days_field.error()) {
                auto trading_days_array = trading_days_field.value().get_array();
                if (!trading_days_array.error()) {
                    for (auto day_elem : trading_days_array.value()) {
                        auto day_str = day_elem.get_string();
                        if (!day_str.error()) {
                            schedule.trading_days.push_back(std::string(day_str.value()));
                        }
                    }
                }
            }
        }
    }

    return schedule;
}

FuturesMarketStatus RESTClient::get_futures_market_status() {
    std::string path = "/v1/marketstatus/futures";
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

    FuturesMarketStatus status;
    auto obj = root_obj.value();

    auto market_field = obj.find_field_unordered("market");
    if (!market_field.error()) {
        status.market = std::string(market_field.value().get_string().value());
    }

    auto server_time_field = obj.find_field_unordered("serverTime");
    if (!server_time_field.error()) {
        status.server_time = std::string(server_time_field.value().get_string().value());
    }

    auto exchanges_field = obj.find_field_unordered("exchanges");
    if (!exchanges_field.error()) {
        status.exchanges = std::string(exchanges_field.value().get_string().value());
    }

    auto currencies_field = obj.find_field_unordered("currencies");
    if (!currencies_field.error()) {
        status.currencies = std::string(currencies_field.value().get_string().value());
    }

    return status;
}

FuturesSnapshot RESTClient::get_futures_snapshot(const std::string& ticker) {
    std::string path = "/v3/snapshot/futures/" + ticker;
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

    FuturesSnapshot snapshot;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto ticker_field = obj.find_field_unordered("ticker");
            if (!ticker_field.error()) {
                snapshot.ticker = std::string(ticker_field.value().get_string().value());
            }

            auto day_open_field = obj.find_field_unordered("day.open");
            if (!day_open_field.error()) {
                snapshot.day_open = day_open_field.value().get_double().value();
            }

            auto day_high_field = obj.find_field_unordered("day.high");
            if (!day_high_field.error()) {
                snapshot.day_high = day_high_field.value().get_double().value();
            }

            auto day_low_field = obj.find_field_unordered("day.low");
            if (!day_low_field.error()) {
                snapshot.day_low = day_low_field.value().get_double().value();
            }

            auto day_close_field = obj.find_field_unordered("day.close");
            if (!day_close_field.error()) {
                snapshot.day_close = day_close_field.value().get_double().value();
            }

            auto day_volume_field = obj.find_field_unordered("day.volume");
            if (!day_volume_field.error()) {
                snapshot.day_volume = day_volume_field.value().get_double().value();
            }

            auto prev_day_open_field = obj.find_field_unordered("prev_day.open");
            if (!prev_day_open_field.error()) {
                snapshot.prev_day_open = prev_day_open_field.value().get_double().value();
            }

            auto prev_day_high_field = obj.find_field_unordered("prev_day.high");
            if (!prev_day_high_field.error()) {
                snapshot.prev_day_high = prev_day_high_field.value().get_double().value();
            }

            auto prev_day_low_field = obj.find_field_unordered("prev_day.low");
            if (!prev_day_low_field.error()) {
                snapshot.prev_day_low = prev_day_low_field.value().get_double().value();
            }

            auto prev_day_close_field = obj.find_field_unordered("prev_day.close");
            if (!prev_day_close_field.error()) {
                snapshot.prev_day_close = prev_day_close_field.value().get_double().value();
            }

            auto prev_day_volume_field = obj.find_field_unordered("prev_day.volume");
            if (!prev_day_volume_field.error()) {
                snapshot.prev_day_volume = prev_day_volume_field.value().get_double().value();
            }

            auto updated_field = obj.find_field_unordered("updated");
            if (!updated_field.error()) {
                snapshot.updated = updated_field.value().get_int64().value();
            }
        }
    }

    return snapshot;
}

std::vector<FuturesExchange> RESTClient::list_futures_exchanges() {
    std::string path = "/v3/reference/futures/exchanges";
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

    std::vector<FuturesExchange> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FuturesExchange exchange;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        exchange.id = std::string(id_field.value().get_string().value());
                    }

                    auto type_field = obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        exchange.type = std::string(type_field.value().get_string().value());
                    }

                    auto market_field = obj.find_field_unordered("market");
                    if (!market_field.error()) {
                        exchange.market = std::string(market_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        exchange.name = std::string(name_field.value().get_string().value());
                    }

                    auto mic_field = obj.find_field_unordered("mic");
                    if (!mic_field.error()) {
                        exchange.mic = std::string(mic_field.value().get_string().value());
                    }

                    results.push_back(exchange);
                }
            }
        }
    }

    return results;
}

// Futures - List Schedules (plural)
std::vector<FuturesSchedule> RESTClient::list_futures_schedules(
    const std::optional<std::string>& session_end_date,
    const std::optional<std::string>& trading_venue,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (session_end_date.has_value()) {
        params["session_end_date"] = session_end_date.value();
    }
    if (trading_venue.has_value()) {
        params["trading_venue"] = trading_venue.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/futures/vX/schedules";
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

    std::vector<FuturesSchedule> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FuturesSchedule schedule;

                    auto session_open_field = obj.find_field_unordered("session_open");
                    if (!session_open_field.error()) {
                        schedule.session_open = std::string(session_open_field.value().get_string().value());
                    }

                    auto session_close_field = obj.find_field_unordered("session_close");
                    if (!session_close_field.error()) {
                        schedule.session_close = std::string(session_close_field.value().get_string().value());
                    }

                    auto session_end_date_field = obj.find_field_unordered("session_end_date");
                    if (!session_end_date_field.error()) {
                        schedule.session_end_date = std::string(session_end_date_field.value().get_string().value());
                    }

                    results.push_back(schedule);
                }
            }
        }
    }

    return results;
}

// Futures - List Schedules by Product Code
std::vector<FuturesSchedule> RESTClient::list_futures_schedules_by_product_code(
    const std::string& product_code,
    const std::optional<std::string>& session_end_date,
    const std::optional<std::string>& session_end_date_lt,
    const std::optional<std::string>& session_end_date_lte,
    const std::optional<std::string>& session_end_date_gt,
    const std::optional<std::string>& session_end_date_gte,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (session_end_date.has_value()) {
        params["session_end_date"] = session_end_date.value();
    }
    if (session_end_date_lt.has_value()) {
        params["session_end_date.lt"] = session_end_date_lt.value();
    }
    if (session_end_date_lte.has_value()) {
        params["session_end_date.lte"] = session_end_date_lte.value();
    }
    if (session_end_date_gt.has_value()) {
        params["session_end_date.gt"] = session_end_date_gt.value();
    }
    if (session_end_date_gte.has_value()) {
        params["session_end_date.gte"] = session_end_date_gte.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/futures/vX/products/" + product_code + "/schedules";
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

    std::vector<FuturesSchedule> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FuturesSchedule schedule;

                    auto session_open_field = obj.find_field_unordered("session_open");
                    if (!session_open_field.error()) {
                        schedule.session_open = std::string(session_open_field.value().get_string().value());
                    }

                    auto session_close_field = obj.find_field_unordered("session_close");
                    if (!session_close_field.error()) {
                        schedule.session_close = std::string(session_close_field.value().get_string().value());
                    }

                    auto session_end_date_field = obj.find_field_unordered("session_end_date");
                    if (!session_end_date_field.error()) {
                        schedule.session_end_date = std::string(session_end_date_field.value().get_string().value());
                    }

                    results.push_back(schedule);
                }
            }
        }
    }

    return results;
}

// Futures - List Market Statuses (plural)
std::vector<FuturesMarketStatus> RESTClient::list_futures_market_statuses(
    const std::optional<std::string>& product_code,
    const std::optional<std::string>& product_code_any_of,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (product_code.has_value()) {
        params["product_code"] = product_code.value();
    }
    if (product_code_any_of.has_value()) {
        params["product_code.any_of"] = product_code_any_of.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/futures/vX/market-status";
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

    std::vector<FuturesMarketStatus> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FuturesMarketStatus status;

                    auto market_status_field = obj.find_field_unordered("market_status");
                    if (!market_status_field.error()) {
                        status.market_status = std::string(market_status_field.value().get_string().value());
                    }

                    auto product_code_field = obj.find_field_unordered("product_code");
                    if (!product_code_field.error()) {
                        status.product_code = std::string(product_code_field.value().get_string().value());
                    }

                    auto session_open_field = obj.find_field_unordered("session_open");
                    if (!session_open_field.error()) {
                        status.session_open = std::string(session_open_field.value().get_string().value());
                    }

                    auto session_close_field = obj.find_field_unordered("session_close");
                    if (!session_close_field.error()) {
                        status.session_close = std::string(session_close_field.value().get_string().value());
                    }

                    results.push_back(status);
                }
            }
        }
    }

    return results;
}

} // namespace massive::rest
