#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <sstream>
#include <stdexcept>

namespace massive::rest {

// Helper function to add optional string parameter
static void add_param(std::map<std::string, std::string> &params, const std::string &key,
                      const std::optional<std::string> &value) {
    if (value.has_value()) {
        params[key] = value.value();
    }
}

// Helper function to add optional double parameter
static void add_param(std::map<std::string, std::string> &params, const std::string &key,
                      const std::optional<double> &value) {
    if (value.has_value()) {
        params[key] = std::to_string(value.value());
    }
}

// Helper function to add optional int parameter
static void add_param(std::map<std::string, std::string> &params, const std::string &key,
                      const std::optional<int> &value) {
    if (value.has_value()) {
        params[key] = std::to_string(value.value());
    }
}

// Financials - Balance Sheets
std::vector<BalanceSheet> RESTClient::list_balance_sheets(
    const std::optional<std::string> &cik, const std::optional<std::string> &cik_any_of,
    const std::optional<std::string> &cik_gt, const std::optional<std::string> &cik_gte,
    const std::optional<std::string> &cik_lt, const std::optional<std::string> &cik_lte,
    const std::optional<std::string> &tickers, const std::optional<std::string> &tickers_all_of,
    const std::optional<std::string> &tickers_any_of, const std::optional<std::string> &period_end,
    const std::optional<std::string> &period_end_gt,
    const std::optional<std::string> &period_end_gte,
    const std::optional<std::string> &period_end_lt,
    const std::optional<std::string> &period_end_lte, const std::optional<std::string> &filing_date,
    const std::optional<std::string> &filing_date_gt,
    const std::optional<std::string> &filing_date_gte,
    const std::optional<std::string> &filing_date_lt,
    const std::optional<std::string> &filing_date_lte, std::optional<double> fiscal_year,
    std::optional<double> fiscal_year_gt, std::optional<double> fiscal_year_gte,
    std::optional<double> fiscal_year_lt, std::optional<double> fiscal_year_lte,
    std::optional<double> fiscal_quarter, std::optional<double> fiscal_quarter_gt,
    std::optional<double> fiscal_quarter_gte, std::optional<double> fiscal_quarter_lt,
    std::optional<double> fiscal_quarter_lte, const std::optional<std::string> &timeframe,
    const std::optional<std::string> &timeframe_any_of,
    const std::optional<std::string> &timeframe_gt, const std::optional<std::string> &timeframe_gte,
    const std::optional<std::string> &timeframe_lt, const std::optional<std::string> &timeframe_lte,
    std::optional<int> limit, const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;

    add_param(params, "cik", cik);
    add_param(params, "cik.any_of", cik_any_of);
    add_param(params, "cik.gt", cik_gt);
    add_param(params, "cik.gte", cik_gte);
    add_param(params, "cik.lt", cik_lt);
    add_param(params, "cik.lte", cik_lte);
    add_param(params, "tickers", tickers);
    add_param(params, "tickers.all_of", tickers_all_of);
    add_param(params, "tickers.any_of", tickers_any_of);
    add_param(params, "period_end", period_end);
    add_param(params, "period_end.gt", period_end_gt);
    add_param(params, "period_end.gte", period_end_gte);
    add_param(params, "period_end.lt", period_end_lt);
    add_param(params, "period_end.lte", period_end_lte);
    add_param(params, "filing_date", filing_date);
    add_param(params, "filing_date.gt", filing_date_gt);
    add_param(params, "filing_date.gte", filing_date_gte);
    add_param(params, "filing_date.lt", filing_date_lt);
    add_param(params, "filing_date.lte", filing_date_lte);
    add_param(params, "fiscal_year", fiscal_year);
    add_param(params, "fiscal_year.gt", fiscal_year_gt);
    add_param(params, "fiscal_year.gte", fiscal_year_gte);
    add_param(params, "fiscal_year.lt", fiscal_year_lt);
    add_param(params, "fiscal_year.lte", fiscal_year_lte);
    add_param(params, "fiscal_quarter", fiscal_quarter);
    add_param(params, "fiscal_quarter.gt", fiscal_quarter_gt);
    add_param(params, "fiscal_quarter.gte", fiscal_quarter_gte);
    add_param(params, "fiscal_quarter.lt", fiscal_quarter_lt);
    add_param(params, "fiscal_quarter.lte", fiscal_quarter_lte);
    add_param(params, "timeframe", timeframe);
    add_param(params, "timeframe.any_of", timeframe_any_of);
    add_param(params, "timeframe.gt", timeframe_gt);
    add_param(params, "timeframe.gte", timeframe_gte);
    add_param(params, "timeframe.lt", timeframe_lt);
    add_param(params, "timeframe.lte", timeframe_lte);
    add_param(params, "limit", limit);
    add_param(params, "sort", sort);

    std::string path = "/stocks/financials/v1/balance-sheets";
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

    std::vector<BalanceSheet> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BalanceSheet sheet;

                    // Parse basic fields - full implementation would parse all DataPoint fields
                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        sheet.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto period_field = obj.find_field_unordered("period");
                    if (!period_field.error()) {
                        sheet.period = std::string(period_field.value().get_string().value());
                    }

                    auto calendar_date_field = obj.find_field_unordered("calendar_date");
                    if (!calendar_date_field.error()) {
                        sheet.calendar_date =
                            std::string(calendar_date_field.value().get_string().value());
                    }

                    auto report_period_field = obj.find_field_unordered("report_period");
                    if (!report_period_field.error()) {
                        sheet.report_period =
                            std::string(report_period_field.value().get_string().value());
                    }

                    auto updated_utc_field = obj.find_field_unordered("updated_utc");
                    if (!updated_utc_field.error()) {
                        sheet.updated_utc =
                            std::string(updated_utc_field.value().get_string().value());
                    }

                    // Parse numeric fields (simplified - would need to parse all DataPoint fields)
                    auto assets_field = obj.find_field_unordered("assets");
                    if (!assets_field.error()) {
                        auto assets_obj = assets_field.value().get_object();
                        if (!assets_obj.error()) {
                            auto value_field = assets_obj.value().find_field_unordered("value");
                            if (!value_field.error()) {
                                sheet.assets = value_field.value().get_int64().value();
                            }
                        }
                    }

                    auto liabilities_field = obj.find_field_unordered("liabilities");
                    if (!liabilities_field.error()) {
                        auto liabilities_obj = liabilities_field.value().get_object();
                        if (!liabilities_obj.error()) {
                            auto value_field =
                                liabilities_obj.value().find_field_unordered("value");
                            if (!value_field.error()) {
                                sheet.liabilities = value_field.value().get_int64().value();
                            }
                        }
                    }

                    auto equity_field = obj.find_field_unordered("equity");
                    if (!equity_field.error()) {
                        auto equity_obj = equity_field.value().get_object();
                        if (!equity_obj.error()) {
                            auto value_field = equity_obj.value().find_field_unordered("value");
                            if (!value_field.error()) {
                                sheet.equity = value_field.value().get_int64().value();
                            }
                        }
                    }

                    results.push_back(sheet);
                }
            }
        }
    }

    return results;
}

// Financials - Income Statements
std::vector<IncomeStatement> RESTClient::list_income_statements(
    const std::optional<std::string> &cik, const std::optional<std::string> &cik_any_of,
    const std::optional<std::string> &cik_gt, const std::optional<std::string> &cik_gte,
    const std::optional<std::string> &cik_lt, const std::optional<std::string> &cik_lte,
    const std::optional<std::string> &tickers, const std::optional<std::string> &tickers_all_of,
    const std::optional<std::string> &tickers_any_of, const std::optional<std::string> &period_end,
    const std::optional<std::string> &period_end_gt,
    const std::optional<std::string> &period_end_gte,
    const std::optional<std::string> &period_end_lt,
    const std::optional<std::string> &period_end_lte, const std::optional<std::string> &filing_date,
    const std::optional<std::string> &filing_date_gt,
    const std::optional<std::string> &filing_date_gte,
    const std::optional<std::string> &filing_date_lt,
    const std::optional<std::string> &filing_date_lte, std::optional<double> fiscal_year,
    std::optional<double> fiscal_year_gt, std::optional<double> fiscal_year_gte,
    std::optional<double> fiscal_year_lt, std::optional<double> fiscal_year_lte,
    std::optional<double> fiscal_quarter, std::optional<double> fiscal_quarter_gt,
    std::optional<double> fiscal_quarter_gte, std::optional<double> fiscal_quarter_lt,
    std::optional<double> fiscal_quarter_lte, const std::optional<std::string> &timeframe,
    const std::optional<std::string> &timeframe_any_of,
    const std::optional<std::string> &timeframe_gt, const std::optional<std::string> &timeframe_gte,
    const std::optional<std::string> &timeframe_lt, const std::optional<std::string> &timeframe_lte,
    std::optional<int> limit, const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;

    add_param(params, "cik", cik);
    add_param(params, "cik.any_of", cik_any_of);
    add_param(params, "cik.gt", cik_gt);
    add_param(params, "cik.gte", cik_gte);
    add_param(params, "cik.lt", cik_lt);
    add_param(params, "cik.lte", cik_lte);
    add_param(params, "tickers", tickers);
    add_param(params, "tickers.all_of", tickers_all_of);
    add_param(params, "tickers.any_of", tickers_any_of);
    add_param(params, "period_end", period_end);
    add_param(params, "period_end.gt", period_end_gt);
    add_param(params, "period_end.gte", period_end_gte);
    add_param(params, "period_end.lt", period_end_lt);
    add_param(params, "period_end.lte", period_end_lte);
    add_param(params, "filing_date", filing_date);
    add_param(params, "filing_date.gt", filing_date_gt);
    add_param(params, "filing_date.gte", filing_date_gte);
    add_param(params, "filing_date.lt", filing_date_lt);
    add_param(params, "filing_date.lte", filing_date_lte);
    add_param(params, "fiscal_year", fiscal_year);
    add_param(params, "fiscal_year.gt", fiscal_year_gt);
    add_param(params, "fiscal_year.gte", fiscal_year_gte);
    add_param(params, "fiscal_year.lt", fiscal_year_lt);
    add_param(params, "fiscal_year.lte", fiscal_year_lte);
    add_param(params, "fiscal_quarter", fiscal_quarter);
    add_param(params, "fiscal_quarter.gt", fiscal_quarter_gt);
    add_param(params, "fiscal_quarter.gte", fiscal_quarter_gte);
    add_param(params, "fiscal_quarter.lt", fiscal_quarter_lt);
    add_param(params, "fiscal_quarter.lte", fiscal_quarter_lte);
    add_param(params, "timeframe", timeframe);
    add_param(params, "timeframe.any_of", timeframe_any_of);
    add_param(params, "timeframe.gt", timeframe_gt);
    add_param(params, "timeframe.gte", timeframe_gte);
    add_param(params, "timeframe.lt", timeframe_lt);
    add_param(params, "timeframe.lte", timeframe_lte);
    add_param(params, "limit", limit);
    add_param(params, "sort", sort);

    std::string path = "/stocks/financials/v1/income-statements";
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

    std::vector<IncomeStatement> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    IncomeStatement statement;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        statement.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto period_field = obj.find_field_unordered("period");
                    if (!period_field.error()) {
                        statement.period = std::string(period_field.value().get_string().value());
                    }

                    auto calendar_date_field = obj.find_field_unordered("calendar_date");
                    if (!calendar_date_field.error()) {
                        statement.calendar_date =
                            std::string(calendar_date_field.value().get_string().value());
                    }

                    auto revenues_field = obj.find_field_unordered("revenues");
                    if (!revenues_field.error()) {
                        auto revenues_obj = revenues_field.value().get_object();
                        if (!revenues_obj.error()) {
                            auto value_field = revenues_obj.value().find_field_unordered("value");
                            if (!value_field.error()) {
                                statement.revenues = value_field.value().get_int64().value();
                            }
                        }
                    }

                    auto net_income_field = obj.find_field_unordered("net_income");
                    if (!net_income_field.error()) {
                        auto net_income_obj = net_income_field.value().get_object();
                        if (!net_income_obj.error()) {
                            auto value_field = net_income_obj.value().find_field_unordered("value");
                            if (!value_field.error()) {
                                statement.net_income = value_field.value().get_int64().value();
                            }
                        }
                    }

                    results.push_back(statement);
                }
            }
        }
    }

    return results;
}

// Financials - Cash Flow Statements
std::vector<CashFlowStatement> RESTClient::list_cash_flow_statements(
    const std::optional<std::string> &cik, const std::optional<std::string> &cik_any_of,
    const std::optional<std::string> &cik_gt, const std::optional<std::string> &cik_gte,
    const std::optional<std::string> &cik_lt, const std::optional<std::string> &cik_lte,
    const std::optional<std::string> &period_end, const std::optional<std::string> &period_end_gt,
    const std::optional<std::string> &period_end_gte,
    const std::optional<std::string> &period_end_lt,
    const std::optional<std::string> &period_end_lte, const std::optional<std::string> &filing_date,
    const std::optional<std::string> &filing_date_gt,
    const std::optional<std::string> &filing_date_gte,
    const std::optional<std::string> &filing_date_lt,
    const std::optional<std::string> &filing_date_lte, const std::optional<std::string> &tickers,
    const std::optional<std::string> &tickers_all_of,
    const std::optional<std::string> &tickers_any_of, std::optional<double> fiscal_year,
    std::optional<double> fiscal_year_gt, std::optional<double> fiscal_year_gte,
    std::optional<double> fiscal_year_lt, std::optional<double> fiscal_year_lte,
    std::optional<double> fiscal_quarter, std::optional<double> fiscal_quarter_gt,
    std::optional<double> fiscal_quarter_gte, std::optional<double> fiscal_quarter_lt,
    std::optional<double> fiscal_quarter_lte, const std::optional<std::string> &timeframe,
    const std::optional<std::string> &timeframe_any_of,
    const std::optional<std::string> &timeframe_gt, const std::optional<std::string> &timeframe_gte,
    const std::optional<std::string> &timeframe_lt, const std::optional<std::string> &timeframe_lte,
    std::optional<int> limit, const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;

    add_param(params, "cik", cik);
    add_param(params, "cik.any_of", cik_any_of);
    add_param(params, "cik.gt", cik_gt);
    add_param(params, "cik.gte", cik_gte);
    add_param(params, "cik.lt", cik_lt);
    add_param(params, "cik.lte", cik_lte);
    add_param(params, "period_end", period_end);
    add_param(params, "period_end.gt", period_end_gt);
    add_param(params, "period_end.gte", period_end_gte);
    add_param(params, "period_end.lt", period_end_lt);
    add_param(params, "period_end.lte", period_end_lte);
    add_param(params, "filing_date", filing_date);
    add_param(params, "filing_date.gt", filing_date_gt);
    add_param(params, "filing_date.gte", filing_date_gte);
    add_param(params, "filing_date.lt", filing_date_lt);
    add_param(params, "filing_date.lte", filing_date_lte);
    add_param(params, "tickers", tickers);
    add_param(params, "tickers.all_of", tickers_all_of);
    add_param(params, "tickers.any_of", tickers_any_of);
    add_param(params, "fiscal_year", fiscal_year);
    add_param(params, "fiscal_year.gt", fiscal_year_gt);
    add_param(params, "fiscal_year.gte", fiscal_year_gte);
    add_param(params, "fiscal_year.lt", fiscal_year_lt);
    add_param(params, "fiscal_year.lte", fiscal_year_lte);
    add_param(params, "fiscal_quarter", fiscal_quarter);
    add_param(params, "fiscal_quarter.gt", fiscal_quarter_gt);
    add_param(params, "fiscal_quarter.gte", fiscal_quarter_gte);
    add_param(params, "fiscal_quarter.lt", fiscal_quarter_lt);
    add_param(params, "fiscal_quarter.lte", fiscal_quarter_lte);
    add_param(params, "timeframe", timeframe);
    add_param(params, "timeframe.any_of", timeframe_any_of);
    add_param(params, "timeframe.gt", timeframe_gt);
    add_param(params, "timeframe.gte", timeframe_gte);
    add_param(params, "timeframe.lt", timeframe_lt);
    add_param(params, "timeframe.lte", timeframe_lte);
    add_param(params, "limit", limit);
    add_param(params, "sort", sort);

    std::string path = "/stocks/financials/v1/cash-flow-statements";
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

    std::vector<CashFlowStatement> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    CashFlowStatement statement;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        statement.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto period_field = obj.find_field_unordered("period");
                    if (!period_field.error()) {
                        statement.period = std::string(period_field.value().get_string().value());
                    }

                    auto cash_from_operating_activities_field =
                        obj.find_field_unordered("cash_from_operating_activities");
                    if (!cash_from_operating_activities_field.error()) {
                        auto cash_obj = cash_from_operating_activities_field.value().get_object();
                        if (!cash_obj.error()) {
                            auto value_field = cash_obj.value().find_field_unordered("value");
                            if (!value_field.error()) {
                                statement.cash_from_operating_activities =
                                    value_field.value().get_int64().value();
                            }
                        }
                    }

                    auto free_cash_flow_field = obj.find_field_unordered("free_cash_flow");
                    if (!free_cash_flow_field.error()) {
                        auto fcf_obj = free_cash_flow_field.value().get_object();
                        if (!fcf_obj.error()) {
                            auto value_field = fcf_obj.value().find_field_unordered("value");
                            if (!value_field.error()) {
                                statement.free_cash_flow = value_field.value().get_int64().value();
                            }
                        }
                    }

                    results.push_back(statement);
                }
            }
        }
    }

    return results;
}

// Financials - Financial Ratios
std::vector<FinancialRatios> RESTClient::list_financial_ratios(
    const std::optional<std::string> &ticker, const std::optional<std::string> &ticker_any_of,
    const std::optional<std::string> &ticker_gt, const std::optional<std::string> &ticker_gte,
    const std::optional<std::string> &ticker_lt, const std::optional<std::string> &ticker_lte,
    const std::optional<std::string> &cik, const std::optional<std::string> &cik_any_of,
    const std::optional<std::string> &cik_gt, const std::optional<std::string> &cik_gte,
    const std::optional<std::string> &cik_lt, const std::optional<std::string> &cik_lte,
    std::optional<double> price, std::optional<double> price_gt, std::optional<double> price_gte,
    std::optional<double> price_lt, std::optional<double> price_lte,
    std::optional<double> average_volume, std::optional<double> average_volume_gt,
    std::optional<double> average_volume_gte, std::optional<double> average_volume_lt,
    std::optional<double> average_volume_lte, std::optional<double> market_cap,
    std::optional<double> market_cap_gt, std::optional<double> market_cap_gte,
    std::optional<double> market_cap_lt, std::optional<double> market_cap_lte,
    std::optional<double> earnings_per_share, std::optional<double> earnings_per_share_gt,
    std::optional<double> earnings_per_share_gte, std::optional<double> earnings_per_share_lt,
    std::optional<double> earnings_per_share_lte, std::optional<double> price_to_earnings,
    std::optional<double> price_to_earnings_gt, std::optional<double> price_to_earnings_gte,
    std::optional<double> price_to_earnings_lt, std::optional<double> price_to_earnings_lte,
    std::optional<double> price_to_book, std::optional<double> price_to_book_gt,
    std::optional<double> price_to_book_gte, std::optional<double> price_to_book_lt,
    std::optional<double> price_to_book_lte, std::optional<double> price_to_sales,
    std::optional<double> price_to_sales_gt, std::optional<double> price_to_sales_gte,
    std::optional<double> price_to_sales_lt, std::optional<double> price_to_sales_lte,
    std::optional<double> price_to_cash_flow, std::optional<double> price_to_cash_flow_gt,
    std::optional<double> price_to_cash_flow_gte, std::optional<double> price_to_cash_flow_lt,
    std::optional<double> price_to_cash_flow_lte, std::optional<double> price_to_free_cash_flow,
    std::optional<double> price_to_free_cash_flow_gt,
    std::optional<double> price_to_free_cash_flow_gte,
    std::optional<double> price_to_free_cash_flow_lt,
    std::optional<double> price_to_free_cash_flow_lte, std::optional<double> dividend_yield,
    std::optional<double> dividend_yield_gt, std::optional<double> dividend_yield_gte,
    std::optional<double> dividend_yield_lt, std::optional<double> dividend_yield_lte,
    std::optional<double> return_on_assets, std::optional<double> return_on_assets_gt,
    std::optional<double> return_on_assets_gte, std::optional<double> return_on_assets_lt,
    std::optional<double> return_on_assets_lte, std::optional<double> return_on_equity,
    std::optional<double> return_on_equity_gt, std::optional<double> return_on_equity_gte,
    std::optional<double> return_on_equity_lt, std::optional<double> return_on_equity_lte,
    std::optional<double> debt_to_equity, std::optional<double> debt_to_equity_gt,
    std::optional<double> debt_to_equity_gte, std::optional<double> debt_to_equity_lt,
    std::optional<double> debt_to_equity_lte, std::optional<double> current,
    std::optional<double> current_gt, std::optional<double> current_gte,
    std::optional<double> current_lt, std::optional<double> current_lte,
    std::optional<double> quick, std::optional<double> quick_gt, std::optional<double> quick_gte,
    std::optional<double> quick_lt, std::optional<double> quick_lte, std::optional<double> cash,
    std::optional<double> cash_gt, std::optional<double> cash_gte, std::optional<double> cash_lt,
    std::optional<double> cash_lte, std::optional<double> ev_to_sales,
    std::optional<double> ev_to_sales_gt, std::optional<double> ev_to_sales_gte,
    std::optional<double> ev_to_sales_lt, std::optional<double> ev_to_sales_lte,
    std::optional<double> ev_to_ebitda, std::optional<double> ev_to_ebitda_gt,
    std::optional<double> ev_to_ebitda_gte, std::optional<double> ev_to_ebitda_lt,
    std::optional<double> ev_to_ebitda_lte, std::optional<double> enterprise_value,
    std::optional<double> enterprise_value_gt, std::optional<double> enterprise_value_gte,
    std::optional<double> enterprise_value_lt, std::optional<double> enterprise_value_lte,
    std::optional<double> free_cash_flow, std::optional<double> free_cash_flow_gt,
    std::optional<double> free_cash_flow_gte, std::optional<double> free_cash_flow_lt,
    std::optional<double> free_cash_flow_lte, std::optional<int> limit,
    const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;

    add_param(params, "ticker", ticker);
    add_param(params, "ticker.any_of", ticker_any_of);
    add_param(params, "ticker.gt", ticker_gt);
    add_param(params, "ticker.gte", ticker_gte);
    add_param(params, "ticker.lt", ticker_lt);
    add_param(params, "ticker.lte", ticker_lte);
    add_param(params, "cik", cik);
    add_param(params, "cik.any_of", cik_any_of);
    add_param(params, "cik.gt", cik_gt);
    add_param(params, "cik.gte", cik_gte);
    add_param(params, "cik.lt", cik_lt);
    add_param(params, "cik.lte", cik_lte);
    add_param(params, "price", price);
    add_param(params, "price.gt", price_gt);
    add_param(params, "price.gte", price_gte);
    add_param(params, "price.lt", price_lt);
    add_param(params, "price.lte", price_lte);
    add_param(params, "average_volume", average_volume);
    add_param(params, "average_volume.gt", average_volume_gt);
    add_param(params, "average_volume.gte", average_volume_gte);
    add_param(params, "average_volume.lt", average_volume_lt);
    add_param(params, "average_volume.lte", average_volume_lte);
    add_param(params, "market_cap", market_cap);
    add_param(params, "market_cap.gt", market_cap_gt);
    add_param(params, "market_cap.gte", market_cap_gte);
    add_param(params, "market_cap.lt", market_cap_lt);
    add_param(params, "market_cap.lte", market_cap_lte);
    add_param(params, "earnings_per_share", earnings_per_share);
    add_param(params, "earnings_per_share.gt", earnings_per_share_gt);
    add_param(params, "earnings_per_share.gte", earnings_per_share_gte);
    add_param(params, "earnings_per_share.lt", earnings_per_share_lt);
    add_param(params, "earnings_per_share.lte", earnings_per_share_lte);
    add_param(params, "price_to_earnings", price_to_earnings);
    add_param(params, "price_to_earnings.gt", price_to_earnings_gt);
    add_param(params, "price_to_earnings.gte", price_to_earnings_gte);
    add_param(params, "price_to_earnings.lt", price_to_earnings_lt);
    add_param(params, "price_to_earnings.lte", price_to_earnings_lte);
    add_param(params, "price_to_book", price_to_book);
    add_param(params, "price_to_book.gt", price_to_book_gt);
    add_param(params, "price_to_book.gte", price_to_book_gte);
    add_param(params, "price_to_book.lt", price_to_book_lt);
    add_param(params, "price_to_book.lte", price_to_book_lte);
    add_param(params, "price_to_sales", price_to_sales);
    add_param(params, "price_to_sales.gt", price_to_sales_gt);
    add_param(params, "price_to_sales.gte", price_to_sales_gte);
    add_param(params, "price_to_sales.lt", price_to_sales_lt);
    add_param(params, "price_to_sales.lte", price_to_sales_lte);
    add_param(params, "price_to_cash_flow", price_to_cash_flow);
    add_param(params, "price_to_cash_flow.gt", price_to_cash_flow_gt);
    add_param(params, "price_to_cash_flow.gte", price_to_cash_flow_gte);
    add_param(params, "price_to_cash_flow.lt", price_to_cash_flow_lt);
    add_param(params, "price_to_cash_flow.lte", price_to_cash_flow_lte);
    add_param(params, "price_to_free_cash_flow", price_to_free_cash_flow);
    add_param(params, "price_to_free_cash_flow.gt", price_to_free_cash_flow_gt);
    add_param(params, "price_to_free_cash_flow.gte", price_to_free_cash_flow_gte);
    add_param(params, "price_to_free_cash_flow.lt", price_to_free_cash_flow_lt);
    add_param(params, "price_to_free_cash_flow.lte", price_to_free_cash_flow_lte);
    add_param(params, "dividend_yield", dividend_yield);
    add_param(params, "dividend_yield.gt", dividend_yield_gt);
    add_param(params, "dividend_yield.gte", dividend_yield_gte);
    add_param(params, "dividend_yield.lt", dividend_yield_lt);
    add_param(params, "dividend_yield.lte", dividend_yield_lte);
    add_param(params, "return_on_assets", return_on_assets);
    add_param(params, "return_on_assets.gt", return_on_assets_gt);
    add_param(params, "return_on_assets.gte", return_on_assets_gte);
    add_param(params, "return_on_assets.lt", return_on_assets_lt);
    add_param(params, "return_on_assets.lte", return_on_assets_lte);
    add_param(params, "return_on_equity", return_on_equity);
    add_param(params, "return_on_equity.gt", return_on_equity_gt);
    add_param(params, "return_on_equity.gte", return_on_equity_gte);
    add_param(params, "return_on_equity.lt", return_on_equity_lt);
    add_param(params, "return_on_equity.lte", return_on_equity_lte);
    add_param(params, "debt_to_equity", debt_to_equity);
    add_param(params, "debt_to_equity.gt", debt_to_equity_gt);
    add_param(params, "debt_to_equity.gte", debt_to_equity_gte);
    add_param(params, "debt_to_equity.lt", debt_to_equity_lt);
    add_param(params, "debt_to_equity.lte", debt_to_equity_lte);
    add_param(params, "current", current);
    add_param(params, "current.gt", current_gt);
    add_param(params, "current.gte", current_gte);
    add_param(params, "current.lt", current_lt);
    add_param(params, "current.lte", current_lte);
    add_param(params, "quick", quick);
    add_param(params, "quick.gt", quick_gt);
    add_param(params, "quick.gte", quick_gte);
    add_param(params, "quick.lt", quick_lt);
    add_param(params, "quick.lte", quick_lte);
    add_param(params, "cash", cash);
    add_param(params, "cash.gt", cash_gt);
    add_param(params, "cash.gte", cash_gte);
    add_param(params, "cash.lt", cash_lt);
    add_param(params, "cash.lte", cash_lte);
    add_param(params, "ev_to_sales", ev_to_sales);
    add_param(params, "ev_to_sales.gt", ev_to_sales_gt);
    add_param(params, "ev_to_sales.gte", ev_to_sales_gte);
    add_param(params, "ev_to_sales.lt", ev_to_sales_lt);
    add_param(params, "ev_to_sales.lte", ev_to_sales_lte);
    add_param(params, "ev_to_ebitda", ev_to_ebitda);
    add_param(params, "ev_to_ebitda.gt", ev_to_ebitda_gt);
    add_param(params, "ev_to_ebitda.gte", ev_to_ebitda_gte);
    add_param(params, "ev_to_ebitda.lt", ev_to_ebitda_lt);
    add_param(params, "ev_to_ebitda.lte", ev_to_ebitda_lte);
    add_param(params, "enterprise_value", enterprise_value);
    add_param(params, "enterprise_value.gt", enterprise_value_gt);
    add_param(params, "enterprise_value.gte", enterprise_value_gte);
    add_param(params, "enterprise_value.lt", enterprise_value_lt);
    add_param(params, "enterprise_value.lte", enterprise_value_lte);
    add_param(params, "free_cash_flow", free_cash_flow);
    add_param(params, "free_cash_flow.gt", free_cash_flow_gt);
    add_param(params, "free_cash_flow.gte", free_cash_flow_gte);
    add_param(params, "free_cash_flow.lt", free_cash_flow_lt);
    add_param(params, "free_cash_flow.lte", free_cash_flow_lte);
    add_param(params, "limit", limit);
    add_param(params, "sort", sort);

    std::string path = "/stocks/financials/v1/ratios";
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

    std::vector<FinancialRatios> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    FinancialRatios ratios;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        ratios.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto current_ratio_field = obj.find_field_unordered("current_ratio");
                    if (!current_ratio_field.error()) {
                        ratios.current_ratio = current_ratio_field.value().get_double().value();
                    }

                    auto return_on_equity_field = obj.find_field_unordered("return_on_equity");
                    if (!return_on_equity_field.error()) {
                        ratios.return_on_equity =
                            return_on_equity_field.value().get_double().value();
                    }

                    auto price_to_earnings_field = obj.find_field_unordered("price_to_earnings");
                    if (!price_to_earnings_field.error()) {
                        ratios.price_to_earnings =
                            price_to_earnings_field.value().get_double().value();
                    }

                    results.push_back(ratios);
                }
            }
        }
    }

    return results;
}

} // namespace massive::rest
