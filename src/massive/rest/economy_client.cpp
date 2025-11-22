#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

// Economy - Economic Indicators
std::vector<EconomicIndicator> RESTClient::list_economic_indicators(
    const std::optional<std::string> &indicator, const std::optional<std::string> &country,
    std::optional<int> limit, const std::optional<std::string> &date_gte,
    const std::optional<std::string> &date_lte, const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (indicator.has_value()) {
        params["indicator"] = indicator.value();
    }
    if (country.has_value()) {
        params["country"] = country.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/v1/economic";
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

    std::vector<EconomicIndicator> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EconomicIndicator indicator;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        indicator.id = std::string(id_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        indicator.name = std::string(name_field.value().get_string().value());
                    }

                    auto country_field = obj.find_field_unordered("country");
                    if (!country_field.error()) {
                        indicator.country = std::string(country_field.value().get_string().value());
                    }

                    auto value_field = obj.find_field_unordered("value");
                    if (!value_field.error()) {
                        indicator.value = value_field.value().get_double().value();
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        indicator.date = std::string(date_field.value().get_string().value());
                    }

                    auto change_percent_field = obj.find_field_unordered("change_percent");
                    if (!change_percent_field.error()) {
                        indicator.change_percent = change_percent_field.value().get_double().value();
                    }

                    results.push_back(indicator);
                }
            }
        }
    }

    return results;
}

// Economy - Economic Calendar Events
std::vector<EconomicCalendarEvent> RESTClient::list_economic_calendar_events(
    const std::optional<std::string> &country, const std::optional<std::string> &importance,
    std::optional<int> limit, const std::optional<std::string> &date_gte,
    const std::optional<std::string> &date_lte, const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (country.has_value()) {
        params["country"] = country.value();
    }
    if (importance.has_value()) {
        params["importance"] = importance.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/v1/economic/calendar";
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

    std::vector<EconomicCalendarEvent> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EconomicCalendarEvent event;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        event.id = std::string(id_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        event.name = std::string(name_field.value().get_string().value());
                    }

                    auto country_field = obj.find_field_unordered("country");
                    if (!country_field.error()) {
                        event.country = std::string(country_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        event.date = std::string(date_field.value().get_string().value());
                    }

                    auto actual_field = obj.find_field_unordered("actual");
                    if (!actual_field.error()) {
                        event.actual = actual_field.value().get_double().value();
                    }

                    auto forecast_field = obj.find_field_unordered("forecast");
                    if (!forecast_field.error()) {
                        event.forecast = forecast_field.value().get_double().value();
                    }

                    auto previous_field = obj.find_field_unordered("previous");
                    if (!previous_field.error()) {
                        event.previous = previous_field.value().get_double().value();
                    }

                    results.push_back(event);
                }
            }
        }
    }

    return results;
}

// Economy - Economic Data Series
std::vector<EconomicDataPoint> RESTClient::get_economic_data_series(
    const std::string &indicator, const std::optional<std::string> &country,
    std::optional<int> limit, const std::optional<std::string> &date_gte,
    const std::optional<std::string> &date_lte, const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    params["indicator"] = indicator;
    if (country.has_value()) {
        params["country"] = country.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/v1/economic/series";
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

    std::vector<EconomicDataPoint> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    EconomicDataPoint data_point;

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        data_point.date = std::string(date_field.value().get_string().value());
                    }

                    auto value_field = obj.find_field_unordered("value");
                    if (!value_field.error()) {
                        data_point.value = value_field.value().get_double().value();
                    }

                    auto change_field = obj.find_field_unordered("change");
                    if (!change_field.error()) {
                        data_point.change = change_field.value().get_double().value();
                    }

                    auto change_percent_field = obj.find_field_unordered("change_percent");
                    if (!change_percent_field.error()) {
                        data_point.change_percent =
                            change_percent_field.value().get_double().value();
                    }

                    results.push_back(data_point);
                }
            }
        }
    }

    return results;
}

// Economy - Treasury Yields
std::vector<TreasuryYield> RESTClient::list_treasury_yields(
    const std::optional<std::string>& date,
    const std::optional<std::string>& date_any_of,
    const std::optional<std::string>& date_gt,
    const std::optional<std::string>& date_gte,
    const std::optional<std::string>& date_lt,
    const std::optional<std::string>& date_lte,
    std::optional<int> limit,
    const std::optional<std::string>& sort,
    const std::optional<std::string>& order) {
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
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/fed/v1/treasury-yields";
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

    std::vector<TreasuryYield> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    TreasuryYield yield;

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        yield.date = std::string(date_field.value().get_string().value());
                    }

                    auto yield_1_month_field = obj.find_field_unordered("yield_1_month");
                    if (!yield_1_month_field.error()) {
                        yield.yield_1_month = yield_1_month_field.value().get_double().value();
                    }

                    auto yield_3_month_field = obj.find_field_unordered("yield_3_month");
                    if (!yield_3_month_field.error()) {
                        yield.yield_3_month = yield_3_month_field.value().get_double().value();
                    }

                    auto yield_6_month_field = obj.find_field_unordered("yield_6_month");
                    if (!yield_6_month_field.error()) {
                        yield.yield_6_month = yield_6_month_field.value().get_double().value();
                    }

                    auto yield_1_year_field = obj.find_field_unordered("yield_1_year");
                    if (!yield_1_year_field.error()) {
                        yield.yield_1_year = yield_1_year_field.value().get_double().value();
                    }

                    auto yield_2_year_field = obj.find_field_unordered("yield_2_year");
                    if (!yield_2_year_field.error()) {
                        yield.yield_2_year = yield_2_year_field.value().get_double().value();
                    }

                    auto yield_3_year_field = obj.find_field_unordered("yield_3_year");
                    if (!yield_3_year_field.error()) {
                        yield.yield_3_year = yield_3_year_field.value().get_double().value();
                    }

                    auto yield_5_year_field = obj.find_field_unordered("yield_5_year");
                    if (!yield_5_year_field.error()) {
                        yield.yield_5_year = yield_5_year_field.value().get_double().value();
                    }

                    auto yield_7_year_field = obj.find_field_unordered("yield_7_year");
                    if (!yield_7_year_field.error()) {
                        yield.yield_7_year = yield_7_year_field.value().get_double().value();
                    }

                    auto yield_10_year_field = obj.find_field_unordered("yield_10_year");
                    if (!yield_10_year_field.error()) {
                        yield.yield_10_year = yield_10_year_field.value().get_double().value();
                    }

                    auto yield_20_year_field = obj.find_field_unordered("yield_20_year");
                    if (!yield_20_year_field.error()) {
                        yield.yield_20_year = yield_20_year_field.value().get_double().value();
                    }

                    auto yield_30_year_field = obj.find_field_unordered("yield_30_year");
                    if (!yield_30_year_field.error()) {
                        yield.yield_30_year = yield_30_year_field.value().get_double().value();
                    }

                    results.push_back(yield);
                }
            }
        }
    }

    return results;
}

// Economy - Inflation
std::vector<FedInflation> RESTClient::list_inflation(
    const std::optional<std::string>& date,
    const std::optional<std::string>& date_any_of,
    const std::optional<std::string>& date_gt,
    const std::optional<std::string>& date_gte,
    const std::optional<std::string>& date_lt,
    const std::optional<std::string>& date_lte,
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
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/fed/v1/inflation";
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

    std::vector<FedInflation> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FedInflation inflation;

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        inflation.date = std::string(date_field.value().get_string().value());
                    }

                    auto cpi_field = obj.find_field_unordered("cpi");
                    if (!cpi_field.error()) {
                        inflation.cpi = cpi_field.value().get_double().value();
                    }

                    auto cpi_core_field = obj.find_field_unordered("cpi_core");
                    if (!cpi_core_field.error()) {
                        inflation.cpi_core = cpi_core_field.value().get_double().value();
                    }

                    auto cpi_year_over_year_field = obj.find_field_unordered("cpi_year_over_year");
                    if (!cpi_year_over_year_field.error()) {
                        inflation.cpi_year_over_year = cpi_year_over_year_field.value().get_double().value();
                    }

                    auto pce_field = obj.find_field_unordered("pce");
                    if (!pce_field.error()) {
                        inflation.pce = pce_field.value().get_double().value();
                    }

                    auto pce_core_field = obj.find_field_unordered("pce_core");
                    if (!pce_core_field.error()) {
                        inflation.pce_core = pce_core_field.value().get_double().value();
                    }

                    auto pce_spending_field = obj.find_field_unordered("pce_spending");
                    if (!pce_spending_field.error()) {
                        inflation.pce_spending = pce_spending_field.value().get_double().value();
                    }

                    results.push_back(inflation);
                }
            }
        }
    }

    return results;
}

// Economy - Inflation Expectations
std::vector<FedInflationExpectations> RESTClient::list_inflation_expectations(
    const std::optional<std::string>& date,
    const std::optional<std::string>& date_any_of,
    const std::optional<std::string>& date_gt,
    const std::optional<std::string>& date_gte,
    const std::optional<std::string>& date_lt,
    const std::optional<std::string>& date_lte,
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
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/fed/v1/inflation-expectations";
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

    std::vector<FedInflationExpectations> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FedInflationExpectations expectations;

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        expectations.date = std::string(date_field.value().get_string().value());
                    }

                    auto forward_years_5_to_10_field = obj.find_field_unordered("forward_years_5_to_10");
                    if (!forward_years_5_to_10_field.error()) {
                        expectations.forward_years_5_to_10 = forward_years_5_to_10_field.value().get_double().value();
                    }

                    auto market_10_year_field = obj.find_field_unordered("market_10_year");
                    if (!market_10_year_field.error()) {
                        expectations.market_10_year = market_10_year_field.value().get_double().value();
                    }

                    auto market_5_year_field = obj.find_field_unordered("market_5_year");
                    if (!market_5_year_field.error()) {
                        expectations.market_5_year = market_5_year_field.value().get_double().value();
                    }

                    auto model_10_year_field = obj.find_field_unordered("model_10_year");
                    if (!model_10_year_field.error()) {
                        expectations.model_10_year = model_10_year_field.value().get_double().value();
                    }

                    auto model_1_year_field = obj.find_field_unordered("model_1_year");
                    if (!model_1_year_field.error()) {
                        expectations.model_1_year = model_1_year_field.value().get_double().value();
                    }

                    auto model_30_year_field = obj.find_field_unordered("model_30_year");
                    if (!model_30_year_field.error()) {
                        expectations.model_30_year = model_30_year_field.value().get_double().value();
                    }

                    auto model_5_year_field = obj.find_field_unordered("model_5_year");
                    if (!model_5_year_field.error()) {
                        expectations.model_5_year = model_5_year_field.value().get_double().value();
                    }

                    results.push_back(expectations);
                }
            }
        }
    }

    return results;
}

} // namespace massive::rest

