#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

namespace {
// Helper function to parse Agg from JSON object
template<typename ObjectType>
Agg parse_agg_from_object(ObjectType& obj) {
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

    auto otc_field = obj.find_field_unordered("otc");
    if (!otc_field.error()) {
        agg.otc = otc_field.value().get_bool().value();
    }

    return agg;
}

// Helper function to parse LastTrade from JSON object
template<typename ObjectType>
LastTrade parse_last_trade_from_object(ObjectType& obj) {
    LastTrade trade;

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

    return trade;
}

// Helper function to parse LastQuote from JSON object
template<typename ObjectType>
LastQuote parse_last_quote_from_object(ObjectType& obj) {
    LastQuote quote;

    auto ticker_field = obj.find_field_unordered("T");
    if (!ticker_field.error()) {
        quote.ticker = std::string(ticker_field.value().get_string().value());
    }

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

    return quote;
}

// Helper function to parse TickerSnapshot from JSON object
template<typename ObjectType>
TickerSnapshot parse_ticker_snapshot_from_object(ObjectType& ticker_obj) {
    TickerSnapshot snapshot;

    auto ticker_name_field = ticker_obj.find_field_unordered("ticker");
    if (!ticker_name_field.error()) {
        snapshot.ticker = std::string(ticker_name_field.value().get_string().value());
    }

    auto day_field = ticker_obj.find_field_unordered("day");
    if (!day_field.error()) {
        auto day_obj_result = day_field.value().get_object();
        if (!day_obj_result.error()) {
            snapshot.day = parse_agg_from_object(day_obj_result.value());
        }
    }

    auto last_quote_field = ticker_obj.find_field_unordered("lastQuote");
    if (!last_quote_field.error()) {
        auto quote_obj_result = last_quote_field.value().get_object();
        if (!quote_obj_result.error()) {
            snapshot.last_quote = parse_last_quote_from_object(quote_obj_result.value());
        }
    }

    auto last_trade_field = ticker_obj.find_field_unordered("lastTrade");
    if (!last_trade_field.error()) {
        auto trade_obj_result = last_trade_field.value().get_object();
        if (!trade_obj_result.error()) {
            snapshot.last_trade = parse_last_trade_from_object(trade_obj_result.value());
        }
    }

    auto prev_day_field = ticker_obj.find_field_unordered("prevDay");
    if (!prev_day_field.error()) {
        auto prev_day_obj_result = prev_day_field.value().get_object();
        if (!prev_day_obj_result.error()) {
            snapshot.prev_day = parse_agg_from_object(prev_day_obj_result.value());
        }
    }

    auto todays_change_field = ticker_obj.find_field_unordered("todaysChange");
    if (!todays_change_field.error()) {
        snapshot.todays_change = todays_change_field.value().get_double().value();
    }

    auto todays_change_percent_field = ticker_obj.find_field_unordered("todaysChangePerc");
    if (!todays_change_percent_field.error()) {
        snapshot.todays_change_percent = todays_change_percent_field.value().get_double().value();
    }

    auto updated_field = ticker_obj.find_field_unordered("updated");
    if (!updated_field.error()) {
        snapshot.updated = updated_field.value().get_int64().value();
    }

    return snapshot;
}
} // namespace

TickerSnapshot RESTClient::get_snapshot_ticker(SnapshotMarketType market_type,
                                               const std::string &ticker) {
    std::string locale = get_locale(market_type);
    std::string market_type_str = to_string(market_type);
    std::string path =
        "/v2/snapshot/locale/" + locale + "/markets/" + market_type_str + "/tickers/" + ticker;
    auto response = send_request(core::HttpMethod::Get, path);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    TickerSnapshot snapshot;
    auto ticker_field = root_obj.value().find_field_unordered("ticker");
    if (!ticker_field.error()) {
        auto ticker_obj_result = ticker_field.value().get_object();
        if (!ticker_obj_result.error()) {
            auto ticker_obj = ticker_obj_result.value();

            auto ticker_name_field = ticker_obj.find_field_unordered("ticker");
            if (!ticker_name_field.error()) {
                snapshot.ticker = std::string(ticker_name_field.value().get_string().value());
            }

            auto day_field = ticker_obj.find_field_unordered("day");
            if (!day_field.error()) {
                auto day_obj_result = day_field.value().get_object();
                if (!day_obj_result.error()) {
                    snapshot.day = parse_agg_from_object(day_obj_result.value());
                }
            }

            auto last_quote_field = ticker_obj.find_field_unordered("lastQuote");
            if (!last_quote_field.error()) {
                auto quote_obj_result = last_quote_field.value().get_object();
                if (!quote_obj_result.error()) {
                    snapshot.last_quote = parse_last_quote_from_object(quote_obj_result.value());
                }
            }

            auto last_trade_field = ticker_obj.find_field_unordered("lastTrade");
            if (!last_trade_field.error()) {
                auto trade_obj_result = last_trade_field.value().get_object();
                if (!trade_obj_result.error()) {
                    snapshot.last_trade = parse_last_trade_from_object(trade_obj_result.value());
                }
            }

            auto prev_day_field = ticker_obj.find_field_unordered("prevDay");
            if (!prev_day_field.error()) {
                auto prev_day_obj_result = prev_day_field.value().get_object();
                if (!prev_day_obj_result.error()) {
                    snapshot.prev_day = parse_agg_from_object(prev_day_obj_result.value());
                }
            }

            auto todays_change_field = ticker_obj.find_field_unordered("todaysChange");
            if (!todays_change_field.error()) {
                snapshot.todays_change = todays_change_field.value().get_double().value();
            }

            auto todays_change_percent_field = ticker_obj.find_field_unordered("todaysChangePerc");
            if (!todays_change_percent_field.error()) {
                snapshot.todays_change_percent =
                    todays_change_percent_field.value().get_double().value();
            }

            auto updated_field = ticker_obj.find_field_unordered("updated");
            if (!updated_field.error()) {
                snapshot.updated = updated_field.value().get_int64().value();
            }
        }
    }

    return snapshot;
}

std::vector<TickerSnapshot> RESTClient::get_snapshot_all(SnapshotMarketType market_type,
                                                         const std::vector<std::string> &tickers,
                                                         bool include_otc) {
    std::string locale = get_locale(market_type);
    std::string market_type_str = to_string(market_type);
    std::string path = "/v2/snapshot/locale/" + locale + "/markets/" + market_type_str + "/tickers";

    std::map<std::string, std::string> params;
    if (!tickers.empty()) {
        std::string tickers_str;
        for (size_t i = 0; i < tickers.size(); ++i) {
            if (i > 0)
                tickers_str += ",";
            tickers_str += tickers[i];
        }
        params["tickers"] = tickers_str;
    }
    if (include_otc) {
        params["include_otc"] = "true";
    }

    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<TickerSnapshot> results;
    auto tickers_field = root_obj.value().find_field_unordered("tickers");
    if (!tickers_field.error()) {
        auto tickers_array = tickers_field.value().get_array();
        if (!tickers_array.error()) {
            for (auto ticker_elem : tickers_array.value()) {
                auto ticker_obj_result = ticker_elem.get_object();
                if (!ticker_obj_result.error()) {
                    auto ticker_obj = ticker_obj_result.value();
                    TickerSnapshot snapshot;

                    auto ticker_name_field = ticker_obj.find_field_unordered("ticker");
                    if (!ticker_name_field.error()) {
                        snapshot.ticker =
                            std::string(ticker_name_field.value().get_string().value());
                    }

                    auto day_field = ticker_obj.find_field_unordered("day");
                    if (!day_field.error()) {
                        auto day_obj_result = day_field.value().get_object();
                        if (!day_obj_result.error()) {
                            snapshot.day = parse_agg_from_object(day_obj_result.value());
                        }
                    }

                    auto last_quote_field = ticker_obj.find_field_unordered("lastQuote");
                    if (!last_quote_field.error()) {
                        auto quote_obj_result = last_quote_field.value().get_object();
                        if (!quote_obj_result.error()) {
                            snapshot.last_quote =
                                parse_last_quote_from_object(quote_obj_result.value());
                        }
                    }

                    auto last_trade_field = ticker_obj.find_field_unordered("lastTrade");
                    if (!last_trade_field.error()) {
                        auto trade_obj_result = last_trade_field.value().get_object();
                        if (!trade_obj_result.error()) {
                            snapshot.last_trade =
                                parse_last_trade_from_object(trade_obj_result.value());
                        }
                    }

                    auto prev_day_field = ticker_obj.find_field_unordered("prevDay");
                    if (!prev_day_field.error()) {
                        auto prev_day_obj_result = prev_day_field.value().get_object();
                        if (!prev_day_obj_result.error()) {
                            snapshot.prev_day = parse_agg_from_object(prev_day_obj_result.value());
                        }
                    }

                    auto todays_change_field = ticker_obj.find_field_unordered("todaysChange");
                    if (!todays_change_field.error()) {
                        snapshot.todays_change = todays_change_field.value().get_double().value();
                    }

                    auto todays_change_percent_field =
                        ticker_obj.find_field_unordered("todaysChangePerc");
                    if (!todays_change_percent_field.error()) {
                        snapshot.todays_change_percent =
                            todays_change_percent_field.value().get_double().value();
                    }

                    auto updated_field = ticker_obj.find_field_unordered("updated");
                    if (!updated_field.error()) {
                        snapshot.updated = updated_field.value().get_int64().value();
                    }

                    results.push_back(snapshot);
                }
            }
        }
    }

    return results;
}

std::vector<TickerSnapshot> RESTClient::get_snapshot_direction(SnapshotMarketType market_type,
                                                               Direction direction,
                                                               bool include_otc) {
    std::string locale = get_locale(market_type);
    std::string market_type_str = to_string(market_type);
    std::string direction_str = to_string(direction);
    std::string path =
        "/v2/snapshot/locale/" + locale + "/markets/" + market_type_str + "/" + direction_str;

    std::map<std::string, std::string> params;
    if (include_otc) {
        params["include_otc"] = "true";
    }

    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<TickerSnapshot> results;
    auto tickers_field = root_obj.value().find_field_unordered("tickers");
    if (!tickers_field.error()) {
        auto tickers_array = tickers_field.value().get_array();
        if (!tickers_array.error()) {
            for (auto ticker_elem : tickers_array.value()) {
                auto ticker_obj_result = ticker_elem.get_object();
                if (!ticker_obj_result.error()) {
                    auto ticker_obj = ticker_obj_result.value();
                    TickerSnapshot snapshot;

                    auto ticker_name_field = ticker_obj.find_field_unordered("ticker");
                    if (!ticker_name_field.error()) {
                        snapshot.ticker =
                            std::string(ticker_name_field.value().get_string().value());
                    }

                    auto day_field = ticker_obj.find_field_unordered("day");
                    if (!day_field.error()) {
                        auto day_obj_result = day_field.value().get_object();
                        if (!day_obj_result.error()) {
                            snapshot.day = parse_agg_from_object(day_obj_result.value());
                        }
                    }

                    auto last_quote_field = ticker_obj.find_field_unordered("lastQuote");
                    if (!last_quote_field.error()) {
                        auto quote_obj_result = last_quote_field.value().get_object();
                        if (!quote_obj_result.error()) {
                            snapshot.last_quote =
                                parse_last_quote_from_object(quote_obj_result.value());
                        }
                    }

                    auto last_trade_field = ticker_obj.find_field_unordered("lastTrade");
                    if (!last_trade_field.error()) {
                        auto trade_obj_result = last_trade_field.value().get_object();
                        if (!trade_obj_result.error()) {
                            snapshot.last_trade =
                                parse_last_trade_from_object(trade_obj_result.value());
                        }
                    }

                    auto prev_day_field = ticker_obj.find_field_unordered("prevDay");
                    if (!prev_day_field.error()) {
                        auto prev_day_obj_result = prev_day_field.value().get_object();
                        if (!prev_day_obj_result.error()) {
                            snapshot.prev_day = parse_agg_from_object(prev_day_obj_result.value());
                        }
                    }

                    auto todays_change_field = ticker_obj.find_field_unordered("todaysChange");
                    if (!todays_change_field.error()) {
                        snapshot.todays_change = todays_change_field.value().get_double().value();
                    }

                    auto todays_change_percent_field =
                        ticker_obj.find_field_unordered("todaysChangePerc");
                    if (!todays_change_percent_field.error()) {
                        snapshot.todays_change_percent =
                            todays_change_percent_field.value().get_double().value();
                    }

                    auto updated_field = ticker_obj.find_field_unordered("updated");
                    if (!updated_field.error()) {
                        snapshot.updated = updated_field.value().get_int64().value();
                    }

                    results.push_back(snapshot);
                }
            }
        }
    }

    return results;
}

std::vector<UniversalSnapshot> RESTClient::list_universal_snapshots(
    const std::optional<std::string> &type, const std::vector<std::string> &ticker_any_of,
    const std::optional<std::string> &ticker_lt, const std::optional<std::string> &ticker_lte,
    const std::optional<std::string> &ticker_gt, const std::optional<std::string> &ticker_gte,
    std::optional<int> limit, const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (type.has_value()) {
        params["type"] = type.value();
    }
    if (!ticker_any_of.empty()) {
        std::string tickers_str;
        for (size_t i = 0; i < ticker_any_of.size(); ++i) {
            if (i > 0)
                tickers_str += ",";
            tickers_str += ticker_any_of[i];
        }
        params["ticker.any_of"] = tickers_str;
    }
    if (ticker_lt.has_value()) {
        params["ticker.lt"] = ticker_lt.value();
    }
    if (ticker_lte.has_value()) {
        params["ticker.lte"] = ticker_lte.value();
    }
    if (ticker_gt.has_value()) {
        params["ticker.gt"] = ticker_gt.value();
    }
    if (ticker_gte.has_value()) {
        params["ticker.gte"] = ticker_gte.value();
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

    std::string path = "/v3/snapshot";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<UniversalSnapshot> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    UniversalSnapshot snapshot;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        snapshot.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto type_field = obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        snapshot.type = std::string(type_field.value().get_string().value());
                    }

                    // Note: Full parsing of nested structures (session, last_quote, etc.)
                    // would require more complex parsing. This is a simplified version.
                    // Full implementation would parse all nested fields.

                    results.push_back(snapshot);
                }
            }
        }
    }

    return results;
}

std::vector<IndicesSnapshot>
RESTClient::get_snapshot_indices(const std::vector<std::string> &ticker_any_of) {
    std::map<std::string, std::string> params;
    if (!ticker_any_of.empty()) {
        std::string tickers_str;
        for (size_t i = 0; i < ticker_any_of.size(); ++i) {
            if (i > 0)
                tickers_str += ",";
            tickers_str += ticker_any_of[i];
        }
        params["ticker.any_of"] = tickers_str;
    }

    std::string path = "/v3/snapshot/indices";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<IndicesSnapshot> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    IndicesSnapshot snapshot;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        snapshot.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto value_field = obj.find_field_unordered("value");
                    if (!value_field.error()) {
                        snapshot.value = value_field.value().get_double().value();
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        snapshot.name = std::string(name_field.value().get_string().value());
                    }

                    auto type_field = obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        snapshot.type = std::string(type_field.value().get_string().value());
                    }

                    results.push_back(snapshot);
                }
            }
        }
    }

    return results;
}

// Snapshot - Options
TickerSnapshot RESTClient::get_snapshot_option(const std::string& option_ticker) {
    std::string path = "/v2/snapshot/options/" + option_ticker;
    auto response = send_request(core::HttpMethod::Get, path);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    TickerSnapshot snapshot;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();
            snapshot = parse_ticker_snapshot_from_object(obj);
        }
    }

    return snapshot;
}

std::vector<TickerSnapshot> RESTClient::list_snapshot_options_chain(
    const std::string& underlying_ticker,
    const std::optional<std::string>& expiration_date,
    const std::optional<std::string>& contract_type) {
    std::map<std::string, std::string> params;
    params["underlying_ticker"] = underlying_ticker;
    if (expiration_date.has_value()) {
        params["expiration_date"] = expiration_date.value();
    }
    if (contract_type.has_value()) {
        params["contract_type"] = contract_type.value();
    }

    std::string path = "/v3/snapshot/options/" + underlying_ticker;
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<TickerSnapshot> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    results.push_back(parse_ticker_snapshot_from_object(obj));
                }
            }
        }
    }

    return results;
}

// Snapshot - Crypto Book
SnapshotTickerFullBook RESTClient::get_snapshot_crypto_book(const std::string& ticker) {
    std::string path = "/v2/snapshot/locale/global/markets/crypto/tickers/" + ticker + "/book";
    auto response = send_request(core::HttpMethod::Get, path);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    SnapshotTickerFullBook book;
    book.bids = std::vector<OrderBookQuote>();
    book.asks = std::vector<OrderBookQuote>();
    
    auto data_field = root_obj.value().find_field_unordered("data");
    if (!data_field.error()) {
        auto data_obj = data_field.value().get_object();
        if (!data_obj.error()) {
            auto obj = data_obj.value();

            auto ticker_field = obj.find_field_unordered("ticker");
            if (!ticker_field.error()) {
                book.ticker = std::string(ticker_field.value().get_string().value());
            }

            auto bids_field = obj.find_field_unordered("bids");
            if (!bids_field.error()) {
                auto bids_array = bids_field.value().get_array();
                if (!bids_array.error()) {
                    for (auto bid_elem : bids_array.value()) {
                        auto bid_obj = bid_elem.get_object();
                        if (!bid_obj.error()) {
                            OrderBookQuote quote;
                            auto price_field = bid_obj.value().find_field_unordered("p");
                            if (!price_field.error()) {
                                quote.price = price_field.value().get_double().value();
                            }
                            auto exchange_shares_field = bid_obj.value().find_field_unordered("x");
                            if (!exchange_shares_field.error()) {
                                auto exchange_obj = exchange_shares_field.value().get_object();
                                if (!exchange_obj.error()) {
                                    quote.exchange_shares = std::map<std::string, double>();
                                    for (auto exchange_field : exchange_obj.value()) {
                                        auto key_result = exchange_field.unescaped_key();
                                        auto key = std::string(key_result.value());
                                        auto value = exchange_field.value().get_double();
                                        if (!value.error()) {
                                            quote.exchange_shares.value()[key] = value.value();
                                        }
                                    }
                                }
                            }
                            book.bids->push_back(quote);
                        }
                    }
                }
            }

            auto asks_field = obj.find_field_unordered("asks");
            if (!asks_field.error()) {
                auto asks_array = asks_field.value().get_array();
                if (!asks_array.error()) {
                    for (auto ask_elem : asks_array.value()) {
                        auto ask_obj = ask_elem.get_object();
                        if (!ask_obj.error()) {
                            OrderBookQuote quote;
                            auto price_field = ask_obj.value().find_field_unordered("p");
                            if (!price_field.error()) {
                                quote.price = price_field.value().get_double().value();
                            }
                            auto exchange_shares_field = ask_obj.value().find_field_unordered("x");
                            if (!exchange_shares_field.error()) {
                                auto exchange_obj = exchange_shares_field.value().get_object();
                                if (!exchange_obj.error()) {
                                    quote.exchange_shares = std::map<std::string, double>();
                                    for (auto exchange_field : exchange_obj.value()) {
                                        auto key_result = exchange_field.unescaped_key();
                                        auto key = std::string(key_result.value());
                                        auto value = exchange_field.value().get_double();
                                        if (!value.error()) {
                                            quote.exchange_shares.value()[key] = value.value();
                                        }
                                    }
                                }
                            }
                            book.asks->push_back(quote);
                        }
                    }
                }
            }

            auto bid_count_field = obj.find_field_unordered("bidCount");
            if (!bid_count_field.error()) {
                book.bid_count = bid_count_field.value().get_double().value();
            }

            auto ask_count_field = obj.find_field_unordered("askCount");
            if (!ask_count_field.error()) {
                book.ask_count = ask_count_field.value().get_double().value();
            }

            auto spread_field = obj.find_field_unordered("spread");
            if (!spread_field.error()) {
                book.spread = spread_field.value().get_double().value();
            }

            auto updated_field = obj.find_field_unordered("updated");
            if (!updated_field.error()) {
                book.updated = updated_field.value().get_int64().value();
            }
        }
    }

    return book;
}

} // namespace massive::rest
