#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

// Reference Data - Markets
std::vector<MarketHoliday> RESTClient::get_market_holidays() {
    std::string path = "/v1/marketstatus/upcoming";
    auto response = send_request(core::HttpMethod::Get, path);

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_array = doc.get_array();
    if (root_array.error()) {
        throw std::runtime_error("Response is not a JSON array");
    }

    std::vector<MarketHoliday> results;
    for (auto elem : root_array.value()) {
        auto obj_result = elem.get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();
            MarketHoliday holiday;

            auto exchange_field = obj.find_field_unordered("exchange");
            if (!exchange_field.error()) {
                holiday.exchange = std::string(exchange_field.value().get_string().value());
            }

            auto name_field = obj.find_field_unordered("name");
            if (!name_field.error()) {
                holiday.name = std::string(name_field.value().get_string().value());
            }

            auto date_field = obj.find_field_unordered("date");
            if (!date_field.error()) {
                holiday.date = std::string(date_field.value().get_string().value());
            }

            auto status_field = obj.find_field_unordered("status");
            if (!status_field.error()) {
                holiday.status = std::string(status_field.value().get_string().value());
            }

            results.push_back(holiday);
        }
    }

    return results;
}

MarketStatus RESTClient::get_market_status() {
    std::string path = "/v1/marketstatus/now";
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

    MarketStatus status;
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

// Reference Data - Tickers
std::vector<Ticker> RESTClient::list_tickers(
    const std::optional<std::string> &ticker, const std::optional<std::string> &ticker_lt,
    const std::optional<std::string> &ticker_lte, const std::optional<std::string> &ticker_gt,
    const std::optional<std::string> &ticker_gte, const std::optional<std::string> &type,
    const std::optional<std::string> &market, const std::optional<std::string> &exchange,
    std::optional<int> cusip, std::optional<int> cik, const std::optional<std::string> &date,
    std::optional<bool> active, const std::optional<std::string> &search, std::optional<int> limit,
    const std::optional<std::string> &sort, const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
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
    if (type.has_value()) {
        params["type"] = type.value();
    }
    if (market.has_value()) {
        params["market"] = market.value();
    }
    if (exchange.has_value()) {
        params["exchange"] = exchange.value();
    }
    if (cusip.has_value()) {
        params["cusip"] = std::to_string(cusip.value());
    }
    if (cik.has_value()) {
        params["cik"] = std::to_string(cik.value());
    }
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (active.has_value()) {
        params["active"] = active.value() ? "true" : "false";
    }
    if (search.has_value()) {
        params["search"] = search.value();
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

    std::string path = "/v3/reference/tickers";
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

    std::vector<Ticker> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
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
                        tick.primary_exchange =
                            std::string(primary_exchange_field.value().get_string().value());
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
        }
    }

    return results;
}

TickerDetails RESTClient::get_ticker_details(const std::string &ticker,
                                             const std::optional<std::string> &date) {
    std::map<std::string, std::string> params;
    if (date.has_value()) {
        params["date"] = date.value();
    }
    std::string path = "/v3/reference/tickers/" + ticker;
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

    TickerDetails details;
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

            auto description_field = obj.find_field_unordered("description");
            if (!description_field.error()) {
                details.description = std::string(description_field.value().get_string().value());
            }

            auto market_cap_field = obj.find_field_unordered("market_cap");
            if (!market_cap_field.error()) {
                details.market_cap = market_cap_field.value().get_double().value();
            }

            auto primary_exchange_field = obj.find_field_unordered("primary_exchange");
            if (!primary_exchange_field.error()) {
                details.primary_exchange =
                    std::string(primary_exchange_field.value().get_string().value());
            }

            auto homepage_url_field = obj.find_field_unordered("homepage_url");
            if (!homepage_url_field.error()) {
                details.homepage_url = std::string(homepage_url_field.value().get_string().value());
            }

            auto total_employees_field = obj.find_field_unordered("total_employees");
            if (!total_employees_field.error()) {
                details.total_employees = total_employees_field.value().get_int64().value();
            }
        }
    }

    return details;
}

// Reference Data - Ticker News
std::vector<TickerNews> RESTClient::list_ticker_news(
    const std::string &ticker, std::optional<int> limit,
    const std::optional<std::string> &order, const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;
    params["ticker"] = ticker;
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/v2/reference/news";
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

    std::vector<TickerNews> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    TickerNews news;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        news.id = std::string(id_field.value().get_string().value());
                    }

                    auto publisher_name_field = obj.find_field_unordered("publisher.name");
                    if (!publisher_name_field.error()) {
                        news.publisher_name = std::string(publisher_name_field.value().get_string().value());
                    }

                    auto title_field = obj.find_field_unordered("title");
                    if (!title_field.error()) {
                        news.title = std::string(title_field.value().get_string().value());
                    }

                    auto author_field = obj.find_field_unordered("author");
                    if (!author_field.error()) {
                        news.author = std::string(author_field.value().get_string().value());
                    }

                    auto published_utc_field = obj.find_field_unordered("published_utc");
                    if (!published_utc_field.error()) {
                        news.published_utc = std::string(published_utc_field.value().get_string().value());
                    }

                    auto article_url_field = obj.find_field_unordered("article_url");
                    if (!article_url_field.error()) {
                        news.article_url = std::string(article_url_field.value().get_string().value());
                    }

                    auto tickers_field = obj.find_field_unordered("tickers");
                    if (!tickers_field.error()) {
                        auto tickers_array = tickers_field.value().get_array();
                        if (!tickers_array.error()) {
                            for (auto ticker_elem : tickers_array.value()) {
                                auto ticker_str = ticker_elem.get_string();
                                if (!ticker_str.error()) {
                                    news.tickers.push_back(std::string(ticker_str.value()));
                                }
                            }
                        }
                    }

                    auto description_field = obj.find_field_unordered("description");
                    if (!description_field.error()) {
                        news.description = std::string(description_field.value().get_string().value());
                    }

                    results.push_back(news);
                }
            }
        }
    }

    return results;
}

// Reference Data - Ticker Types
std::vector<TickerTypes> RESTClient::get_ticker_types() {
    std::string path = "/v3/reference/tickers/types";
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

    std::vector<TickerTypes> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    TickerTypes types;

                    auto asset_class_field = obj.find_field_unordered("asset_class");
                    if (!asset_class_field.error()) {
                        types.asset_class = std::string(asset_class_field.value().get_string().value());
                    }

                    auto code_field = obj.find_field_unordered("code");
                    if (!code_field.error()) {
                        types.code = std::string(code_field.value().get_string().value());
                    }

                    auto description_field = obj.find_field_unordered("description");
                    if (!description_field.error()) {
                        types.description = std::string(description_field.value().get_string().value());
                    }

                    auto locale_field = obj.find_field_unordered("locale");
                    if (!locale_field.error()) {
                        types.locale = locale_field.value().get_bool().value();
                    }

                    results.push_back(types);
                }
            }
        }
    }

    return results;
}

// Reference Data - Related Companies
std::vector<RelatedCompany> RESTClient::get_related_companies(const std::string &ticker) {
    std::string path = "/v2/reference/tickers/" + ticker + "/related";
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

    std::vector<RelatedCompany> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    RelatedCompany company;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        company.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        company.name = std::string(name_field.value().get_string().value());
                    }

                    auto market_field = obj.find_field_unordered("market");
                    if (!market_field.error()) {
                        company.market = std::string(market_field.value().get_string().value());
                    }

                    auto locale_field = obj.find_field_unordered("locale");
                    if (!locale_field.error()) {
                        company.locale = std::string(locale_field.value().get_string().value());
                    }

                    auto primary_exchange_field = obj.find_field_unordered("primary_exchange");
                    if (!primary_exchange_field.error()) {
                        company.primary_exchange = std::string(primary_exchange_field.value().get_string().value());
                    }

                    auto type_field = obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        company.type = std::string(type_field.value().get_string().value());
                    }

                    auto active_field = obj.find_field_unordered("active");
                    if (!active_field.error()) {
                        company.active = active_field.value().get_bool().value();
                    }

                    results.push_back(company);
                }
            }
        }
    }

    return results;
}

// Reference Data - Splits
std::vector<Split> RESTClient::list_splits(const std::optional<std::string> &ticker,
                                           std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/v3/reference/splits";
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

    std::vector<Split> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    Split split;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        split.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto execution_date_field = obj.find_field_unordered("execution_date");
                    if (!execution_date_field.error()) {
                        split.execution_date =
                            std::string(execution_date_field.value().get_string().value());
                    }

                    auto split_from_field = obj.find_field_unordered("split_from");
                    if (!split_from_field.error()) {
                        split.split_from = split_from_field.value().get_int64().value();
                    }

                    auto split_to_field = obj.find_field_unordered("split_to");
                    if (!split_to_field.error()) {
                        split.split_to = split_to_field.value().get_int64().value();
                    }

                    results.push_back(split);
                }
            }
        }
    }

    return results;
}

// Reference Data - Dividends
std::vector<Dividend> RESTClient::list_dividends(const std::optional<std::string> &ticker,
                                                 std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/v3/reference/dividends";
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

    std::vector<Dividend> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    Dividend dividend;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        dividend.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto cash_amount_field = obj.find_field_unordered("cash_amount");
                    if (!cash_amount_field.error()) {
                        dividend.cash_amount = cash_amount_field.value().get_double().value();
                    }

                    auto ex_dividend_date_field = obj.find_field_unordered("ex_dividend_date");
                    if (!ex_dividend_date_field.error()) {
                        dividend.ex_dividend_date =
                            std::string(ex_dividend_date_field.value().get_string().value());
                    }

                    auto pay_date_field = obj.find_field_unordered("pay_date");
                    if (!pay_date_field.error()) {
                        dividend.pay_date =
                            std::string(pay_date_field.value().get_string().value());
                    }

                    auto record_date_field = obj.find_field_unordered("record_date");
                    if (!record_date_field.error()) {
                        dividend.record_date =
                            std::string(record_date_field.value().get_string().value());
                    }

                    auto frequency_field = obj.find_field_unordered("frequency");
                    if (!frequency_field.error()) {
                        dividend.frequency = frequency_field.value().get_int64().value();
                    }

                    results.push_back(dividend);
                }
            }
        }
    }

    return results;
}

// Reference Data - Conditions
std::vector<Condition> RESTClient::list_conditions(std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/v3/reference/conditions";
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

    std::vector<Condition> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    Condition condition;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        condition.id = id_field.value().get_int64().value();
                    }

                    auto asset_class_field = obj.find_field_unordered("asset_class");
                    if (!asset_class_field.error()) {
                        condition.asset_class =
                            std::string(asset_class_field.value().get_string().value());
                    }

                    auto description_field = obj.find_field_unordered("description");
                    if (!description_field.error()) {
                        condition.description =
                            std::string(description_field.value().get_string().value());
                    }

                    results.push_back(condition);
                }
            }
        }
    }

    return results;
}

// Reference Data - Exchanges
std::vector<Exchange> RESTClient::get_exchanges() {
    std::string path = "/v3/reference/exchanges";
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

    std::vector<Exchange> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    Exchange exchange;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        exchange.id = std::string(id_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        exchange.name = std::string(name_field.value().get_string().value());
                    }

                    auto market_field = obj.find_field_unordered("market");
                    if (!market_field.error()) {
                        exchange.market = std::string(market_field.value().get_string().value());
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

// Reference Data - Contracts
OptionsContract RESTClient::get_options_contract(const std::string &ticker) {
    std::string path = "/v3/reference/options/contracts/" + ticker;
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

    OptionsContract contract;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto ticker_field = obj.find_field_unordered("ticker");
            if (!ticker_field.error()) {
                contract.ticker = std::string(ticker_field.value().get_string().value());
            }

            auto underlying_ticker_field = obj.find_field_unordered("underlying_ticker");
            if (!underlying_ticker_field.error()) {
                contract.underlying_ticker =
                    std::string(underlying_ticker_field.value().get_string().value());
            }

            auto contract_type_field = obj.find_field_unordered("contract_type");
            if (!contract_type_field.error()) {
                contract.contract_type =
                    std::string(contract_type_field.value().get_string().value());
            }

            auto expiration_date_field = obj.find_field_unordered("expiration_date");
            if (!expiration_date_field.error()) {
                contract.expiration_date =
                    std::string(expiration_date_field.value().get_string().value());
            }

            auto strike_price_field = obj.find_field_unordered("strike_price");
            if (!strike_price_field.error()) {
                contract.strike_price = strike_price_field.value().get_double().value();
            }
        }
    }

    return contract;
}

std::vector<OptionsContract> RESTClient::list_options_contracts(
    const std::optional<std::string> &underlying_ticker,
    const std::optional<std::string> &underlying_ticker_lt,
    const std::optional<std::string> &underlying_ticker_lte,
    const std::optional<std::string> &underlying_ticker_gt,
    const std::optional<std::string> &underlying_ticker_gte,
    const std::optional<std::string> &contract_type,
    const std::optional<std::string> &expiration_date,
    const std::optional<std::string> &expiration_date_lt,
    const std::optional<std::string> &expiration_date_lte,
    const std::optional<std::string> &expiration_date_gt,
    const std::optional<std::string> &expiration_date_gte,
    const std::optional<std::string> &as_of,
    std::optional<double> strike_price,
    std::optional<double> strike_price_lt,
    std::optional<double> strike_price_lte,
    std::optional<double> strike_price_gt,
    std::optional<double> strike_price_gte,
    std::optional<bool> expired,
    std::optional<int> limit,
    const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (underlying_ticker.has_value()) {
        params["underlying_ticker"] = underlying_ticker.value();
    }
    if (underlying_ticker_lt.has_value()) {
        params["underlying_ticker.lt"] = underlying_ticker_lt.value();
    }
    if (underlying_ticker_lte.has_value()) {
        params["underlying_ticker.lte"] = underlying_ticker_lte.value();
    }
    if (underlying_ticker_gt.has_value()) {
        params["underlying_ticker.gt"] = underlying_ticker_gt.value();
    }
    if (underlying_ticker_gte.has_value()) {
        params["underlying_ticker.gte"] = underlying_ticker_gte.value();
    }
    if (contract_type.has_value()) {
        params["contract_type"] = contract_type.value();
    }
    if (expiration_date.has_value()) {
        params["expiration_date"] = expiration_date.value();
    }
    if (expiration_date_lt.has_value()) {
        params["expiration_date.lt"] = expiration_date_lt.value();
    }
    if (expiration_date_lte.has_value()) {
        params["expiration_date.lte"] = expiration_date_lte.value();
    }
    if (expiration_date_gt.has_value()) {
        params["expiration_date.gt"] = expiration_date_gt.value();
    }
    if (expiration_date_gte.has_value()) {
        params["expiration_date.gte"] = expiration_date_gte.value();
    }
    if (as_of.has_value()) {
        params["as_of"] = as_of.value();
    }
    if (strike_price.has_value()) {
        params["strike_price"] = std::to_string(strike_price.value());
    }
    if (strike_price_lt.has_value()) {
        params["strike_price.lt"] = std::to_string(strike_price_lt.value());
    }
    if (strike_price_lte.has_value()) {
        params["strike_price.lte"] = std::to_string(strike_price_lte.value());
    }
    if (strike_price_gt.has_value()) {
        params["strike_price.gt"] = std::to_string(strike_price_gt.value());
    }
    if (strike_price_gte.has_value()) {
        params["strike_price.gte"] = std::to_string(strike_price_gte.value());
    }
    if (expired.has_value()) {
        params["expired"] = expired.value() ? "true" : "false";
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

    std::string path = "/v3/reference/options/contracts";
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

    std::vector<OptionsContract> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    OptionsContract contract;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        contract.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto underlying_ticker_field = obj.find_field_unordered("underlying_ticker");
                    if (!underlying_ticker_field.error()) {
                        contract.underlying_ticker =
                            std::string(underlying_ticker_field.value().get_string().value());
                    }

                    auto contract_type_field = obj.find_field_unordered("contract_type");
                    if (!contract_type_field.error()) {
                        contract.contract_type =
                            std::string(contract_type_field.value().get_string().value());
                    }

                    auto expiration_date_field = obj.find_field_unordered("expiration_date");
                    if (!expiration_date_field.error()) {
                        contract.expiration_date =
                            std::string(expiration_date_field.value().get_string().value());
                    }

                    auto strike_price_field = obj.find_field_unordered("strike_price");
                    if (!strike_price_field.error()) {
                        contract.strike_price = strike_price_field.value().get_double().value();
                    }

                    results.push_back(contract);
                }
            }
        }
    }

    return results;
}

// Reference Data - IPOs
std::vector<IPO> RESTClient::list_ipos(
    const std::optional<std::string> &ticker, const std::optional<std::string> &us_code,
    const std::optional<std::string> &isin, const std::optional<std::string> &listing_date,
    const std::optional<std::string> &listing_date_lt, const std::optional<std::string> &listing_date_lte,
    const std::optional<std::string> &listing_date_gt, const std::optional<std::string> &listing_date_gte,
    const std::optional<std::string> &ipo_status, std::optional<int> limit,
    const std::optional<std::string> &sort, const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (us_code.has_value()) {
        params["us_code"] = us_code.value();
    }
    if (isin.has_value()) {
        params["isin"] = isin.value();
    }
    if (listing_date.has_value()) {
        params["listing_date"] = listing_date.value();
    }
    if (listing_date_lt.has_value()) {
        params["listing_date.lt"] = listing_date_lt.value();
    }
    if (listing_date_lte.has_value()) {
        params["listing_date.lte"] = listing_date_lte.value();
    }
    if (listing_date_gt.has_value()) {
        params["listing_date.gt"] = listing_date_gt.value();
    }
    if (listing_date_gte.has_value()) {
        params["listing_date.gte"] = listing_date_gte.value();
    }
    if (ipo_status.has_value()) {
        params["ipo_status"] = ipo_status.value();
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

    std::string path = "/vX/reference/ipos";
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

    std::vector<IPO> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    IPO ipo;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        ipo.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        ipo.name = std::string(name_field.value().get_string().value());
                    }

                    auto market_field = obj.find_field_unordered("market");
                    if (!market_field.error()) {
                        ipo.market = std::string(market_field.value().get_string().value());
                    }

                    auto locale_field = obj.find_field_unordered("locale");
                    if (!locale_field.error()) {
                        ipo.locale = std::string(locale_field.value().get_string().value());
                    }

                    auto primary_exchange_field = obj.find_field_unordered("primary_exchange");
                    if (!primary_exchange_field.error()) {
                        ipo.primary_exchange = std::string(primary_exchange_field.value().get_string().value());
                    }

                    auto type_field = obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        ipo.type = std::string(type_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        ipo.date = std::string(date_field.value().get_string().value());
                    }

                    auto exchange_field = obj.find_field_unordered("exchange");
                    if (!exchange_field.error()) {
                        ipo.exchange = std::string(exchange_field.value().get_string().value());
                    }

                    auto exchange_symbol_field = obj.find_field_unordered("exchange_symbol");
                    if (!exchange_symbol_field.error()) {
                        ipo.exchange_symbol = std::string(exchange_symbol_field.value().get_string().value());
                    }

                    auto name_of_issuer_field = obj.find_field_unordered("name_of_issuer");
                    if (!name_of_issuer_field.error()) {
                        ipo.name_of_issuer = std::string(name_of_issuer_field.value().get_string().value());
                    }

                    auto shares_field = obj.find_field_unordered("shares");
                    if (!shares_field.error()) {
                        ipo.shares = std::string(shares_field.value().get_string().value());
                    }

                    auto price_field = obj.find_field_unordered("price");
                    if (!price_field.error()) {
                        ipo.price = price_field.value().get_double().value();
                    }

                    auto currency_field = obj.find_field_unordered("currency");
                    if (!currency_field.error()) {
                        ipo.currency = std::string(currency_field.value().get_string().value());
                    }

                    auto status_field = obj.find_field_unordered("status");
                    if (!status_field.error()) {
                        ipo.status = std::string(status_field.value().get_string().value());
                    }

                    results.push_back(ipo);
                }
            }
        }
    }

    return results;
}

// Reference Data - Short Interest
std::vector<ShortInterest> RESTClient::list_short_interest(
    const std::optional<std::string> &ticker,
    const std::optional<std::string> &days_to_cover,
    const std::optional<std::string> &days_to_cover_lt,
    const std::optional<std::string> &days_to_cover_lte,
    const std::optional<std::string> &days_to_cover_gt,
    const std::optional<std::string> &days_to_cover_gte,
    const std::optional<std::string> &settlement_date,
    const std::optional<std::string> &settlement_date_lt,
    const std::optional<std::string> &settlement_date_lte,
    const std::optional<std::string> &settlement_date_gt,
    const std::optional<std::string> &settlement_date_gte,
    const std::optional<std::string> &avg_daily_volume,
    const std::optional<std::string> &avg_daily_volume_lt,
    const std::optional<std::string> &avg_daily_volume_lte,
    const std::optional<std::string> &avg_daily_volume_gt,
    const std::optional<std::string> &avg_daily_volume_gte,
    std::optional<int> limit,
    const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (days_to_cover.has_value()) {
        params["days_to_cover"] = days_to_cover.value();
    }
    if (days_to_cover_lt.has_value()) {
        params["days_to_cover.lt"] = days_to_cover_lt.value();
    }
    if (days_to_cover_lte.has_value()) {
        params["days_to_cover.lte"] = days_to_cover_lte.value();
    }
    if (days_to_cover_gt.has_value()) {
        params["days_to_cover.gt"] = days_to_cover_gt.value();
    }
    if (days_to_cover_gte.has_value()) {
        params["days_to_cover.gte"] = days_to_cover_gte.value();
    }
    if (settlement_date.has_value()) {
        params["settlement_date"] = settlement_date.value();
    }
    if (settlement_date_lt.has_value()) {
        params["settlement_date.lt"] = settlement_date_lt.value();
    }
    if (settlement_date_lte.has_value()) {
        params["settlement_date.lte"] = settlement_date_lte.value();
    }
    if (settlement_date_gt.has_value()) {
        params["settlement_date.gt"] = settlement_date_gt.value();
    }
    if (settlement_date_gte.has_value()) {
        params["settlement_date.gte"] = settlement_date_gte.value();
    }
    if (avg_daily_volume.has_value()) {
        params["avg_daily_volume"] = avg_daily_volume.value();
    }
    if (avg_daily_volume_lt.has_value()) {
        params["avg_daily_volume.lt"] = avg_daily_volume_lt.value();
    }
    if (avg_daily_volume_lte.has_value()) {
        params["avg_daily_volume.lte"] = avg_daily_volume_lte.value();
    }
    if (avg_daily_volume_gt.has_value()) {
        params["avg_daily_volume.gt"] = avg_daily_volume_gt.value();
    }
    if (avg_daily_volume_gte.has_value()) {
        params["avg_daily_volume.gte"] = avg_daily_volume_gte.value();
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

    std::string path = "/v2/reference/short-interest";
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

    std::vector<ShortInterest> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    ShortInterest short_interest;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        short_interest.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto settlement_date_field = obj.find_field_unordered("settlement_date");
                    if (!settlement_date_field.error()) {
                        short_interest.settlement_date = std::string(settlement_date_field.value().get_string().value());
                    }

                    auto current_short_interest_field = obj.find_field_unordered("current_short_interest");
                    if (!current_short_interest_field.error()) {
                        short_interest.current_short_interest = current_short_interest_field.value().get_int64().value();
                    }

                    auto previous_short_interest_field = obj.find_field_unordered("previous_short_interest");
                    if (!previous_short_interest_field.error()) {
                        short_interest.previous_short_interest = previous_short_interest_field.value().get_int64().value();
                    }

                    auto change_field = obj.find_field_unordered("change");
                    if (!change_field.error()) {
                        short_interest.change = change_field.value().get_double().value();
                    }

                    auto change_percent_field = obj.find_field_unordered("change_percent");
                    if (!change_percent_field.error()) {
                        short_interest.change_percent = change_percent_field.value().get_double().value();
                    }

                    auto day_to_cover_field = obj.find_field_unordered("day_to_cover");
                    if (!day_to_cover_field.error()) {
                        short_interest.day_to_cover = day_to_cover_field.value().get_int64().value();
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        short_interest.name = std::string(name_field.value().get_string().value());
                    }

                    results.push_back(short_interest);
                }
            }
        }
    }

    return results;
}

// Reference Data - Short Volume
std::vector<ShortVolume> RESTClient::list_short_volume(
    const std::optional<std::string> &ticker,
    const std::optional<std::string> &date,
    const std::optional<std::string> &date_lt,
    const std::optional<std::string> &date_lte,
    const std::optional<std::string> &date_gt,
    const std::optional<std::string> &date_gte,
    const std::optional<std::string> &short_volume_ratio,
    const std::optional<std::string> &short_volume_ratio_lt,
    const std::optional<std::string> &short_volume_ratio_lte,
    const std::optional<std::string> &short_volume_ratio_gt,
    const std::optional<std::string> &short_volume_ratio_gte,
    const std::optional<std::string> &total_volume,
    const std::optional<std::string> &total_volume_lt,
    const std::optional<std::string> &total_volume_lte,
    const std::optional<std::string> &total_volume_gt,
    const std::optional<std::string> &total_volume_gte,
    std::optional<int> limit,
    const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (date_lt.has_value()) {
        params["date.lt"] = date_lt.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
    }
    if (date_gt.has_value()) {
        params["date.gt"] = date_gt.value();
    }
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (short_volume_ratio.has_value()) {
        params["short_volume_ratio"] = short_volume_ratio.value();
    }
    if (short_volume_ratio_lt.has_value()) {
        params["short_volume_ratio.lt"] = short_volume_ratio_lt.value();
    }
    if (short_volume_ratio_lte.has_value()) {
        params["short_volume_ratio.lte"] = short_volume_ratio_lte.value();
    }
    if (short_volume_ratio_gt.has_value()) {
        params["short_volume_ratio.gt"] = short_volume_ratio_gt.value();
    }
    if (short_volume_ratio_gte.has_value()) {
        params["short_volume_ratio.gte"] = short_volume_ratio_gte.value();
    }
    if (total_volume.has_value()) {
        params["total_volume"] = total_volume.value();
    }
    if (total_volume_lt.has_value()) {
        params["total_volume.lt"] = total_volume_lt.value();
    }
    if (total_volume_lte.has_value()) {
        params["total_volume.lte"] = total_volume_lte.value();
    }
    if (total_volume_gt.has_value()) {
        params["total_volume.gt"] = total_volume_gt.value();
    }
    if (total_volume_gte.has_value()) {
        params["total_volume.gte"] = total_volume_gte.value();
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

    std::string path = "/v2/reference/short-volume";
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

    std::vector<ShortVolume> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    ShortVolume short_volume;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        short_volume.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        short_volume.date = std::string(date_field.value().get_string().value());
                    }

                    auto short_volume_field = obj.find_field_unordered("short_volume");
                    if (!short_volume_field.error()) {
                        short_volume.short_volume = short_volume_field.value().get_int64().value();
                    }

                    auto total_volume_field = obj.find_field_unordered("total_volume");
                    if (!total_volume_field.error()) {
                        short_volume.total_volume = total_volume_field.value().get_int64().value();
                    }

                    auto short_volume_ratio_field = obj.find_field_unordered("short_volume_ratio");
                    if (!short_volume_ratio_field.error()) {
                        short_volume.short_volume_ratio = short_volume_ratio_field.value().get_double().value();
                    }

                    results.push_back(short_volume);
                }
            }
        }
    }

    return results;
}

// Reference Data - Ticker Events
TickerChangeResults RESTClient::get_ticker_events(
    const std::string& ticker,
    const std::optional<std::string>& types) {
    std::map<std::string, std::string> params;
    if (types.has_value()) {
        params["types"] = types.value();
    }

    std::string path = "/vX/reference/tickers/" + ticker + "/events";
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

    TickerChangeResults results;
    auto obj = root_obj.value();

    auto name_field = obj.find_field_unordered("name");
    if (!name_field.error()) {
        results.name = std::string(name_field.value().get_string().value());
    }

    auto composite_figi_field = obj.find_field_unordered("composite_figi");
    if (!composite_figi_field.error()) {
        results.composite_figi = std::string(composite_figi_field.value().get_string().value());
    }

    auto cik_field = obj.find_field_unordered("cik");
    if (!cik_field.error()) {
        results.cik = std::string(cik_field.value().get_string().value());
    }

    auto events_field = obj.find_field_unordered("events");
    if (!events_field.error()) {
        auto events_array = events_field.value().get_array();
        if (!events_array.error()) {
            std::vector<TickerChangeEvent> events;
            for (auto event_elem : events_array.value()) {
                auto event_obj_result = event_elem.get_object();
                if (!event_obj_result.error()) {
                    auto event_obj = event_obj_result.value();
                    TickerChangeEvent event;

                    auto type_field = event_obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        event.type = std::string(type_field.value().get_string().value());
                    }

                    auto date_field = event_obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        event.date = std::string(date_field.value().get_string().value());
                    }

                    auto ticker_change_field = event_obj.find_field_unordered("ticker_change");
                    if (!ticker_change_field.error()) {
                        auto ticker_change_obj = ticker_change_field.value().get_object();
                        if (!ticker_change_obj.error()) {
                            TickerChange ticker_change;
                            auto ticker_field = ticker_change_obj.value().find_field_unordered("ticker");
                            if (!ticker_field.error()) {
                                ticker_change.ticker = std::string(ticker_field.value().get_string().value());
                            }
                            event.ticker_change = ticker_change;
                        }
                    }

                    events.push_back(event);
                }
            }
            results.events = events;
        }
    }

    return results;
}

} // namespace massive::rest
