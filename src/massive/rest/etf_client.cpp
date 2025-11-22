#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

// ETF Global - List ETFs
std::vector<EtfListItem> RESTClient::list_etfs(const std::optional<std::string> &ticker,
                                               const std::optional<std::string> &exchange,
                                               const std::optional<std::string> &category,
                                               std::optional<int> limit,
                                               const std::optional<std::string> &sort,
                                               const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (exchange.has_value()) {
        params["exchange"] = exchange.value();
    }
    if (category.has_value()) {
        params["category"] = category.value();
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

    std::string path = "/v3/reference/etfs";
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

    std::vector<EtfListItem> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EtfListItem etf;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        etf.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        etf.name = std::string(name_field.value().get_string().value());
                    }

                    auto exchange_field = obj.find_field_unordered("exchange");
                    if (!exchange_field.error()) {
                        etf.exchange = std::string(exchange_field.value().get_string().value());
                    }

                    auto total_assets_field = obj.find_field_unordered("total_assets");
                    if (!total_assets_field.error()) {
                        etf.total_assets = total_assets_field.value().get_double().value();
                    }

                    auto nav_field = obj.find_field_unordered("nav");
                    if (!nav_field.error()) {
                        etf.nav = nav_field.value().get_double().value();
                    }

                    auto expense_ratio_field = obj.find_field_unordered("expense_ratio");
                    if (!expense_ratio_field.error()) {
                        etf.expense_ratio = expense_ratio_field.value().get_double().value();
                    }

                    results.push_back(etf);
                }
            }
        }
    }

    return results;
}

// ETF Global - Get ETF Details
EtfDetails RESTClient::get_etf_details(const std::string &ticker) {
    std::string path = "/v3/reference/etfs/" + ticker;
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

    EtfDetails details;
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
                details.description =
                    std::string(description_field.value().get_string().value());
            }

            auto total_assets_field = obj.find_field_unordered("total_assets");
            if (!total_assets_field.error()) {
                details.total_assets = total_assets_field.value().get_double().value();
            }

            auto nav_field = obj.find_field_unordered("nav");
            if (!nav_field.error()) {
                details.nav = nav_field.value().get_double().value();
            }

            auto expense_ratio_field = obj.find_field_unordered("expense_ratio");
            if (!expense_ratio_field.error()) {
                details.expense_ratio = expense_ratio_field.value().get_double().value();
            }

            auto dividend_yield_field = obj.find_field_unordered("dividend_yield");
            if (!dividend_yield_field.error()) {
                details.dividend_yield = dividend_yield_field.value().get_double().value();
            }

            auto inception_date_field = obj.find_field_unordered("inception_date");
            if (!inception_date_field.error()) {
                details.inception_date =
                    std::string(inception_date_field.value().get_string().value());
            }
        }
    }

    return details;
}

// ETF Global - List ETF Holdings
std::vector<EtfHolding> RESTClient::list_etf_holdings(const std::string &ticker,
                                                      std::optional<int> limit,
                                                      const std::optional<std::string> &date) {
    std::map<std::string, std::string> params;
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (date.has_value()) {
        params["date"] = date.value();
    }

    std::string path = "/v3/reference/etfs/" + ticker + "/holdings";
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

    std::vector<EtfHolding> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EtfHolding holding;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        holding.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        holding.name = std::string(name_field.value().get_string().value());
                    }

                    auto weight_field = obj.find_field_unordered("weight");
                    if (!weight_field.error()) {
                        holding.weight = weight_field.value().get_double().value();
                    }

                    auto shares_field = obj.find_field_unordered("shares");
                    if (!shares_field.error()) {
                        holding.shares = shares_field.value().get_int64().value();
                    }

                    auto market_value_field = obj.find_field_unordered("market_value");
                    if (!market_value_field.error()) {
                        holding.market_value = market_value_field.value().get_double().value();
                    }

                    results.push_back(holding);
                }
            }
        }
    }

    return results;
}

// ETF Global - Get ETF Performance
std::vector<EtfPerformance> RESTClient::get_etf_performance(
    const std::string &ticker, const std::optional<std::string> &date_gte,
    const std::optional<std::string> &date_lte, std::optional<int> limit,
    const std::optional<std::string> &sort, const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
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

    std::string path = "/v3/reference/etfs/" + ticker + "/performance";
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

    std::vector<EtfPerformance> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EtfPerformance performance;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        performance.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        performance.date = std::string(date_field.value().get_string().value());
                    }

                    auto nav_field = obj.find_field_unordered("nav");
                    if (!nav_field.error()) {
                        performance.nav = nav_field.value().get_double().value();
                    }

                    auto return_1d_field = obj.find_field_unordered("return_1d");
                    if (!return_1d_field.error()) {
                        performance.return_1d = return_1d_field.value().get_double().value();
                    }

                    auto return_1y_field = obj.find_field_unordered("return_1y");
                    if (!return_1y_field.error()) {
                        performance.return_1y = return_1y_field.value().get_double().value();
                    }

                    results.push_back(performance);
                }
            }
        }
    }

    return results;
}

// ETF Global - Analytics
std::vector<EtfGlobalAnalytics> RESTClient::get_etf_global_analytics(
    const std::optional<std::string>& composite_ticker,
    const std::optional<std::string>& composite_ticker_any_of,
    const std::optional<std::string>& processed_date,
    const std::optional<std::string>& effective_date,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (composite_ticker.has_value()) {
        params["composite_ticker"] = composite_ticker.value();
    }
    if (composite_ticker_any_of.has_value()) {
        params["composite_ticker.any_of"] = composite_ticker_any_of.value();
    }
    if (processed_date.has_value()) {
        params["processed_date"] = processed_date.value();
    }
    if (effective_date.has_value()) {
        params["effective_date"] = effective_date.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/etf-global/v1/analytics";
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

    std::vector<EtfGlobalAnalytics> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EtfGlobalAnalytics analytics;

                    auto composite_ticker_field = obj.find_field_unordered("composite_ticker");
                    if (!composite_ticker_field.error()) {
                        analytics.composite_ticker = std::string(composite_ticker_field.value().get_string().value());
                    }

                    auto effective_date_field = obj.find_field_unordered("effective_date");
                    if (!effective_date_field.error()) {
                        analytics.effective_date = std::string(effective_date_field.value().get_string().value());
                    }

                    auto processed_date_field = obj.find_field_unordered("processed_date");
                    if (!processed_date_field.error()) {
                        analytics.processed_date = std::string(processed_date_field.value().get_string().value());
                    }

                    auto quant_total_score_field = obj.find_field_unordered("quant_total_score");
                    if (!quant_total_score_field.error()) {
                        analytics.quant_total_score = quant_total_score_field.value().get_double().value();
                    }

                    auto reward_score_field = obj.find_field_unordered("reward_score");
                    if (!reward_score_field.error()) {
                        analytics.reward_score = reward_score_field.value().get_double().value();
                    }

                    auto risk_total_score_field = obj.find_field_unordered("risk_total_score");
                    if (!risk_total_score_field.error()) {
                        analytics.risk_total_score = risk_total_score_field.value().get_double().value();
                    }

                    auto quant_grade_field = obj.find_field_unordered("quant_grade");
                    if (!quant_grade_field.error()) {
                        analytics.quant_grade = std::string(quant_grade_field.value().get_string().value());
                    }

                    results.push_back(analytics);
                }
            }
        }
    }

    return results;
}

// ETF Global - Constituents
std::vector<EtfGlobalConstituent> RESTClient::get_etf_global_constituents(
    const std::optional<std::string>& composite_ticker,
    const std::optional<std::string>& composite_ticker_any_of,
    const std::optional<std::string>& constituent_ticker,
    const std::optional<std::string>& effective_date,
    const std::optional<std::string>& processed_date,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (composite_ticker.has_value()) {
        params["composite_ticker"] = composite_ticker.value();
    }
    if (composite_ticker_any_of.has_value()) {
        params["composite_ticker.any_of"] = composite_ticker_any_of.value();
    }
    if (constituent_ticker.has_value()) {
        params["constituent_ticker"] = constituent_ticker.value();
    }
    if (effective_date.has_value()) {
        params["effective_date"] = effective_date.value();
    }
    if (processed_date.has_value()) {
        params["processed_date"] = processed_date.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/etf-global/v1/constituents";
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

    std::vector<EtfGlobalConstituent> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EtfGlobalConstituent constituent;

                    auto composite_ticker_field = obj.find_field_unordered("composite_ticker");
                    if (!composite_ticker_field.error()) {
                        constituent.composite_ticker = std::string(composite_ticker_field.value().get_string().value());
                    }

                    auto constituent_ticker_field = obj.find_field_unordered("constituent_ticker");
                    if (!constituent_ticker_field.error()) {
                        constituent.constituent_ticker = std::string(constituent_ticker_field.value().get_string().value());
                    }

                    auto constituent_name_field = obj.find_field_unordered("constituent_name");
                    if (!constituent_name_field.error()) {
                        constituent.constituent_name = std::string(constituent_name_field.value().get_string().value());
                    }

                    auto weight_field = obj.find_field_unordered("weight");
                    if (!weight_field.error()) {
                        constituent.weight = weight_field.value().get_double().value();
                    }

                    auto shares_held_field = obj.find_field_unordered("shares_held");
                    if (!shares_held_field.error()) {
                        constituent.shares_held = shares_held_field.value().get_double().value();
                    }

                    auto market_value_field = obj.find_field_unordered("market_value");
                    if (!market_value_field.error()) {
                        constituent.market_value = market_value_field.value().get_double().value();
                    }

                    results.push_back(constituent);
                }
            }
        }
    }

    return results;
}

// ETF Global - Fund Flows
std::vector<EtfGlobalFundFlow> RESTClient::get_etf_global_fund_flows(
    const std::optional<std::string>& composite_ticker,
    const std::optional<std::string>& composite_ticker_any_of,
    const std::optional<std::string>& processed_date,
    const std::optional<std::string>& effective_date,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (composite_ticker.has_value()) {
        params["composite_ticker"] = composite_ticker.value();
    }
    if (composite_ticker_any_of.has_value()) {
        params["composite_ticker.any_of"] = composite_ticker_any_of.value();
    }
    if (processed_date.has_value()) {
        params["processed_date"] = processed_date.value();
    }
    if (effective_date.has_value()) {
        params["effective_date"] = effective_date.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/etf-global/v1/fund-flows";
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

    std::vector<EtfGlobalFundFlow> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EtfGlobalFundFlow fund_flow;

                    auto composite_ticker_field = obj.find_field_unordered("composite_ticker");
                    if (!composite_ticker_field.error()) {
                        fund_flow.composite_ticker = std::string(composite_ticker_field.value().get_string().value());
                    }

                    auto effective_date_field = obj.find_field_unordered("effective_date");
                    if (!effective_date_field.error()) {
                        fund_flow.effective_date = std::string(effective_date_field.value().get_string().value());
                    }

                    auto processed_date_field = obj.find_field_unordered("processed_date");
                    if (!processed_date_field.error()) {
                        fund_flow.processed_date = std::string(processed_date_field.value().get_string().value());
                    }

                    auto fund_flow_field = obj.find_field_unordered("fund_flow");
                    if (!fund_flow_field.error()) {
                        fund_flow.fund_flow = fund_flow_field.value().get_double().value();
                    }

                    auto nav_field = obj.find_field_unordered("nav");
                    if (!nav_field.error()) {
                        fund_flow.nav = nav_field.value().get_double().value();
                    }

                    auto shares_outstanding_field = obj.find_field_unordered("shares_outstanding");
                    if (!shares_outstanding_field.error()) {
                        fund_flow.shares_outstanding = shares_outstanding_field.value().get_double().value();
                    }

                    results.push_back(fund_flow);
                }
            }
        }
    }

    return results;
}

// ETF Global - Profiles
std::vector<EtfGlobalProfile> RESTClient::get_etf_global_profiles(
    const std::optional<std::string>& composite_ticker,
    const std::optional<std::string>& composite_ticker_any_of,
    const std::optional<std::string>& processed_date,
    const std::optional<std::string>& effective_date,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (composite_ticker.has_value()) {
        params["composite_ticker"] = composite_ticker.value();
    }
    if (composite_ticker_any_of.has_value()) {
        params["composite_ticker.any_of"] = composite_ticker_any_of.value();
    }
    if (processed_date.has_value()) {
        params["processed_date"] = processed_date.value();
    }
    if (effective_date.has_value()) {
        params["effective_date"] = effective_date.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/etf-global/v1/profiles";
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

    std::vector<EtfGlobalProfile> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EtfGlobalProfile profile;

                    auto composite_ticker_field = obj.find_field_unordered("composite_ticker");
                    if (!composite_ticker_field.error()) {
                        profile.composite_ticker = std::string(composite_ticker_field.value().get_string().value());
                    }

                    auto effective_date_field = obj.find_field_unordered("effective_date");
                    if (!effective_date_field.error()) {
                        profile.effective_date = std::string(effective_date_field.value().get_string().value());
                    }

                    auto processed_date_field = obj.find_field_unordered("processed_date");
                    if (!processed_date_field.error()) {
                        profile.processed_date = std::string(processed_date_field.value().get_string().value());
                    }

                    auto asset_class_field = obj.find_field_unordered("asset_class");
                    if (!asset_class_field.error()) {
                        profile.asset_class = std::string(asset_class_field.value().get_string().value());
                    }

                    auto category_field = obj.find_field_unordered("category");
                    if (!category_field.error()) {
                        profile.category = std::string(category_field.value().get_string().value());
                    }

                    auto description_field = obj.find_field_unordered("description");
                    if (!description_field.error()) {
                        profile.description = std::string(description_field.value().get_string().value());
                    }

                    auto aum_field = obj.find_field_unordered("aum");
                    if (!aum_field.error()) {
                        profile.aum = aum_field.value().get_double().value();
                    }

                    auto management_fee_field = obj.find_field_unordered("management_fee");
                    if (!management_fee_field.error()) {
                        profile.management_fee = management_fee_field.value().get_double().value();
                    }

                    results.push_back(profile);
                }
            }
        }
    }

    return results;
}

// ETF Global - Taxonomies
std::vector<EtfGlobalTaxonomy> RESTClient::get_etf_global_taxonomies(
    const std::optional<std::string>& composite_ticker,
    const std::optional<std::string>& composite_ticker_any_of,
    const std::optional<std::string>& processed_date,
    const std::optional<std::string>& effective_date,
    std::optional<int> limit,
    const std::optional<std::string>& sort) {
    std::map<std::string, std::string> params;
    if (composite_ticker.has_value()) {
        params["composite_ticker"] = composite_ticker.value();
    }
    if (composite_ticker_any_of.has_value()) {
        params["composite_ticker.any_of"] = composite_ticker_any_of.value();
    }
    if (processed_date.has_value()) {
        params["processed_date"] = processed_date.value();
    }
    if (effective_date.has_value()) {
        params["effective_date"] = effective_date.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/etf-global/v1/taxonomies";
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

    std::vector<EtfGlobalTaxonomy> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EtfGlobalTaxonomy taxonomy;

                    auto composite_ticker_field = obj.find_field_unordered("composite_ticker");
                    if (!composite_ticker_field.error()) {
                        taxonomy.composite_ticker = std::string(composite_ticker_field.value().get_string().value());
                    }

                    auto effective_date_field = obj.find_field_unordered("effective_date");
                    if (!effective_date_field.error()) {
                        taxonomy.effective_date = std::string(effective_date_field.value().get_string().value());
                    }

                    auto processed_date_field = obj.find_field_unordered("processed_date");
                    if (!processed_date_field.error()) {
                        taxonomy.processed_date = std::string(processed_date_field.value().get_string().value());
                    }

                    auto asset_class_field = obj.find_field_unordered("asset_class");
                    if (!asset_class_field.error()) {
                        taxonomy.asset_class = std::string(asset_class_field.value().get_string().value());
                    }

                    auto category_field = obj.find_field_unordered("category");
                    if (!category_field.error()) {
                        taxonomy.category = std::string(category_field.value().get_string().value());
                    }

                    auto country_field = obj.find_field_unordered("country");
                    if (!country_field.error()) {
                        taxonomy.country = std::string(country_field.value().get_string().value());
                    }

                    auto description_field = obj.find_field_unordered("description");
                    if (!description_field.error()) {
                        taxonomy.description = std::string(description_field.value().get_string().value());
                    }

                    auto objective_field = obj.find_field_unordered("objective");
                    if (!objective_field.error()) {
                        taxonomy.objective = std::string(objective_field.value().get_string().value());
                    }

                    auto product_type_field = obj.find_field_unordered("product_type");
                    if (!product_type_field.error()) {
                        taxonomy.product_type = std::string(product_type_field.value().get_string().value());
                    }

                    results.push_back(taxonomy);
                }
            }
        }
    }

    return results;
}

} // namespace massive::rest

