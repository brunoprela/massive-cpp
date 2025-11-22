#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

// VX (Experimental) - Volatility Data
std::vector<VxVolatility> RESTClient::list_vx_volatility(
    const std::optional<std::string> &ticker, std::optional<int> limit,
    const std::optional<std::string> &date_gte, const std::optional<std::string> &date_lte,
    const std::optional<std::string> &sort, const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
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

    std::string path = "/vX/volatility";
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

    std::vector<VxVolatility> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    VxVolatility volatility;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        volatility.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        volatility.date = std::string(date_field.value().get_string().value());
                    }

                    auto implied_volatility_field = obj.find_field_unordered("implied_volatility");
                    if (!implied_volatility_field.error()) {
                        volatility.implied_volatility =
                            implied_volatility_field.value().get_double().value();
                    }

                    auto historical_volatility_field =
                        obj.find_field_unordered("historical_volatility");
                    if (!historical_volatility_field.error()) {
                        volatility.historical_volatility =
                            historical_volatility_field.value().get_double().value();
                    }

                    auto vix_field = obj.find_field_unordered("vix");
                    if (!vix_field.error()) {
                        volatility.vix = vix_field.value().get_double().value();
                    }

                    auto timestamp_field = obj.find_field_unordered("timestamp");
                    if (!timestamp_field.error()) {
                        volatility.timestamp = timestamp_field.value().get_int64().value();
                    }

                    results.push_back(volatility);
                }
            }
        }
    }

    return results;
}

// VX (Experimental) - Generic Experimental Endpoint
std::vector<VxDataPoint> RESTClient::list_vx_experimental(
    const std::string &endpoint, const std::optional<std::string> &ticker,
    std::optional<int> limit, const std::optional<std::string> &date_gte,
    const std::optional<std::string> &date_lte,
    const std::map<std::string, std::string> &additional_params) {
    std::map<std::string, std::string> params = additional_params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
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

    std::string path = "/vX/" + endpoint;
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

    std::vector<VxDataPoint> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    VxDataPoint data_point;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        data_point.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        data_point.date = std::string(date_field.value().get_string().value());
                    }

                    auto value_field = obj.find_field_unordered("value");
                    if (!value_field.error()) {
                        data_point.value = value_field.value().get_double().value();
                    }

                    auto metric_field = obj.find_field_unordered("metric");
                    if (!metric_field.error()) {
                        data_point.metric = std::string(metric_field.value().get_string().value());
                    }

                    auto timestamp_field = obj.find_field_unordered("timestamp");
                    if (!timestamp_field.error()) {
                        data_point.timestamp = timestamp_field.value().get_int64().value();
                    }

                    results.push_back(data_point);
                }
            }
        }
    }

    return results;
}

// VX - Stock Financials
std::vector<StockFinancial> RESTClient::list_stock_financials(
    const std::optional<std::string> &ticker, const std::optional<std::string> &cik,
    const std::optional<std::string> &company_name, const std::optional<std::string> &company_name_search,
    const std::optional<std::string> &sic, const std::optional<std::string> &filing_date,
    const std::optional<std::string> &filing_date_lt, const std::optional<std::string> &filing_date_lte,
    const std::optional<std::string> &filing_date_gt, const std::optional<std::string> &filing_date_gte,
    const std::optional<std::string> &period_of_report_date,
    const std::optional<std::string> &period_of_report_date_lt,
    const std::optional<std::string> &period_of_report_date_lte,
    const std::optional<std::string> &period_of_report_date_gt,
    const std::optional<std::string> &period_of_report_date_gte,
    const std::optional<std::string> &timeframe, std::optional<bool> include_sources,
    std::optional<int> limit, const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (cik.has_value()) {
        params["cik"] = cik.value();
    }
    if (company_name.has_value()) {
        params["company_name"] = company_name.value();
    }
    if (company_name_search.has_value()) {
        params["company_name_search"] = company_name_search.value();
    }
    if (sic.has_value()) {
        params["sic"] = sic.value();
    }
    if (filing_date.has_value()) {
        params["filing_date"] = filing_date.value();
    }
    if (filing_date_lt.has_value()) {
        params["filing_date.lt"] = filing_date_lt.value();
    }
    if (filing_date_lte.has_value()) {
        params["filing_date.lte"] = filing_date_lte.value();
    }
    if (filing_date_gt.has_value()) {
        params["filing_date.gt"] = filing_date_gt.value();
    }
    if (filing_date_gte.has_value()) {
        params["filing_date.gte"] = filing_date_gte.value();
    }
    if (period_of_report_date.has_value()) {
        params["period_of_report_date"] = period_of_report_date.value();
    }
    if (period_of_report_date_lt.has_value()) {
        params["period_of_report_date.lt"] = period_of_report_date_lt.value();
    }
    if (period_of_report_date_lte.has_value()) {
        params["period_of_report_date.lte"] = period_of_report_date_lte.value();
    }
    if (period_of_report_date_gt.has_value()) {
        params["period_of_report_date.gt"] = period_of_report_date_gt.value();
    }
    if (period_of_report_date_gte.has_value()) {
        params["period_of_report_date.gte"] = period_of_report_date_gte.value();
    }
    if (timeframe.has_value()) {
        params["timeframe"] = timeframe.value();
    }
    if (include_sources.has_value()) {
        params["include_sources"] = include_sources.value() ? "true" : "false";
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

    std::string path = "/vX/reference/financials";
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

    std::vector<StockFinancial> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    StockFinancial financial;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        // Note: ticker might not be in the response, but we'll check
                    }

                    auto cik_field = obj.find_field_unordered("cik");
                    if (!cik_field.error()) {
                        financial.cik = std::string(cik_field.value().get_string().value());
                    }

                    auto company_name_field = obj.find_field_unordered("company_name");
                    if (!company_name_field.error()) {
                        financial.company_name = std::string(company_name_field.value().get_string().value());
                    }

                    auto filing_date_field = obj.find_field_unordered("filing_date");
                    if (!filing_date_field.error()) {
                        financial.filing_date = std::string(filing_date_field.value().get_string().value());
                    }

                    auto end_date_field = obj.find_field_unordered("end_date");
                    if (!end_date_field.error()) {
                        financial.end_date = std::string(end_date_field.value().get_string().value());
                    }

                    auto start_date_field = obj.find_field_unordered("start_date");
                    if (!start_date_field.error()) {
                        financial.start_date = std::string(start_date_field.value().get_string().value());
                    }

                    auto fiscal_period_field = obj.find_field_unordered("fiscal_period");
                    if (!fiscal_period_field.error()) {
                        financial.fiscal_period = std::string(fiscal_period_field.value().get_string().value());
                    }

                    auto fiscal_year_field = obj.find_field_unordered("fiscal_year");
                    if (!fiscal_year_field.error()) {
                        financial.fiscal_year = std::string(fiscal_year_field.value().get_string().value());
                    }

                    auto source_filing_url_field = obj.find_field_unordered("source_filing_url");
                    if (!source_filing_url_field.error()) {
                        financial.source_filing_url = std::string(source_filing_url_field.value().get_string().value());
                    }

                    auto source_filing_file_url_field = obj.find_field_unordered("source_filing_file_url");
                    if (!source_filing_file_url_field.error()) {
                        financial.source_filing_file_url = std::string(source_filing_file_url_field.value().get_string().value());
                    }

                    results.push_back(financial);
                }
            }
        }
    }

    return results;
}

} // namespace massive::rest

