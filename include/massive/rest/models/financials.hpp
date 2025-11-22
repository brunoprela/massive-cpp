#pragma once

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace massive::rest {

// Financial Statement Item
struct FinancialItem {
    std::optional<std::string> label;
    std::optional<std::int64_t> value;
    std::optional<std::int64_t> unit;
    std::optional<std::int64_t> order;
};

// Balance Sheet
struct BalanceSheet {
    std::optional<std::string> ticker;
    std::optional<std::string> period;
    std::optional<std::string> calendar_date;
    std::optional<std::string> report_period;
    std::optional<std::string> updated_utc;
    std::optional<std::int64_t> accumulated_other_comprehensive_income;
    std::optional<std::int64_t> assets;
    std::optional<std::int64_t> assets_current;
    std::optional<std::int64_t> assets_non_current;
    std::optional<std::int64_t> book_value;
    std::optional<std::int64_t> capital_lease_obligations;
    std::optional<std::int64_t> cash_and_equivalents;
    std::optional<std::int64_t> cash_and_short_term_investments;
    std::optional<std::int64_t> common_stock;
    std::optional<std::int64_t> equity;
    std::optional<std::int64_t> equity_attributable_to_noncontrolling_interest;
    std::optional<std::int64_t> equity_attributable_to_parent;
    std::optional<std::int64_t> goodwill;
    std::optional<std::int64_t> goodwill_and_intangible_assets;
    std::optional<std::int64_t> intangible_assets;
    std::optional<std::int64_t> inventory;
    std::optional<std::int64_t> investments;
    std::optional<std::int64_t> liabilities;
    std::optional<std::int64_t> liabilities_current;
    std::optional<std::int64_t> liabilities_non_current;
    std::optional<std::int64_t> market_capitalization;
    std::optional<std::int64_t> net_debt;
    std::optional<std::int64_t> net_tangible_assets;
    std::optional<std::int64_t> noncontrolling_interest;
    std::optional<std::int64_t> other_assets;
    std::optional<std::int64_t> other_current_assets;
    std::optional<std::int64_t> other_current_liabilities;
    std::optional<std::int64_t> other_non_current_assets;
    std::optional<std::int64_t> other_non_current_liabilities;
    std::optional<std::int64_t> other_stockholders_equity;
    std::optional<std::int64_t> preferred_stock;
    std::optional<std::int64_t> property_plant_equipment;
    std::optional<std::int64_t> receivables;
    std::optional<std::int64_t> retained_earnings;
    std::optional<std::int64_t> share_issuance;
    std::optional<std::int64_t> stockholders_equity;
    std::optional<std::int64_t> total_debt;
    std::optional<std::int64_t> working_capital;
    std::optional<std::vector<FinancialItem>> financials;
};

// Income Statement
struct IncomeStatement {
    std::optional<std::string> ticker;
    std::optional<std::string> period;
    std::optional<std::string> calendar_date;
    std::optional<std::string> report_period;
    std::optional<std::string> updated_utc;
    std::optional<std::int64_t> basic_earnings_per_share;
    std::optional<std::int64_t> basic_shares_outstanding;
    std::optional<std::int64_t> cost_of_revenue;
    std::optional<std::int64_t> cost_of_revenue_ratio;
    std::optional<std::int64_t> diluted_earnings_per_share;
    std::optional<std::int64_t> diluted_shares_outstanding;
    std::optional<std::int64_t> ebit;
    std::optional<std::int64_t> ebitda;
    std::optional<std::int64_t> ebitda_ratio;
    std::optional<std::int64_t> ebit_ratio;
    std::optional<std::int64_t> effective_tax_rate;
    std::optional<std::int64_t> gross_profit;
    std::optional<std::int64_t> gross_profit_ratio;
    std::optional<std::int64_t> income_before_equity_method_investments;
    std::optional<std::int64_t> income_from_continuing_operations;
    std::optional<std::int64_t> income_from_continuing_operations_after_tax;
    std::optional<std::int64_t> income_from_equity_method_investments;
    std::optional<std::int64_t> interest_expense;
    std::optional<std::int64_t> interest_income;
    std::optional<std::int64_t> net_income;
    std::optional<std::int64_t> net_income_common_stockholders;
    std::optional<std::int64_t> net_income_ratio;
    std::optional<std::int64_t> net_interest_income;
    std::optional<std::int64_t> non_operating_income_loss;
    std::optional<std::int64_t> operating_expenses;
    std::optional<std::int64_t> operating_income;
    std::optional<std::int64_t> operating_income_ratio;
    std::optional<std::int64_t> operating_revenue;
    std::optional<std::int64_t> other_income_expense;
    std::optional<std::int64_t> other_non_operating_income_expense;
    std::optional<std::int64_t> participating_securities_distributed_and_undistributed_earnings_loss_basic;
    std::optional<std::int64_t> preferred_stock_dividends;
    std::optional<std::int64_t> pretax_income;
    std::optional<std::int64_t> pretax_income_ratio;
    std::optional<std::int64_t> provision_for_income_taxes;
    std::optional<std::int64_t> research_and_development;
    std::optional<std::int64_t> revenues;
    std::optional<std::int64_t> revenues_usd;
    std::optional<std::int64_t> selling_general_and_administrative;
    std::optional<std::int64_t> total_expenses;
    std::optional<std::int64_t> total_operating_income_as_reported;
    std::optional<std::int64_t> weighted_average_shares;
    std::optional<std::int64_t> weighted_average_shares_diluted;
    std::optional<std::vector<FinancialItem>> financials;
};

// Cash Flow Statement
struct CashFlowStatement {
    std::optional<std::string> ticker;
    std::optional<std::string> period;
    std::optional<std::string> calendar_date;
    std::optional<std::string> report_period;
    std::optional<std::string> updated_utc;
    std::optional<std::int64_t> capital_expenditure;
    std::optional<std::int64_t> cash_and_equivalents_end_of_period;
    std::optional<std::int64_t> cash_and_equivalents_start_of_period;
    std::optional<std::int64_t> cash_from_financing_activities;
    std::optional<std::int64_t> cash_from_investing_activities;
    std::optional<std::int64_t> cash_from_operating_activities;
    std::optional<std::int64_t> change_in_cash_and_equivalents;
    std::optional<std::int64_t> changes_in_account_receivables;
    std::optional<std::int64_t> changes_in_assets;
    std::optional<std::int64_t> changes_in_cash;
    std::optional<std::int64_t> changes_in_liabilities;
    std::optional<std::int64_t> changes_in_operating_activities;
    std::optional<std::int64_t> depreciation_and_amortization;
    std::optional<std::int64_t> dividends_paid;
    std::optional<std::int64_t> effect_of_exchange_rate_changes;
    std::optional<std::int64_t> free_cash_flow;
    std::optional<std::int64_t> inventory;
    std::optional<std::int64_t> investing_activities_other;
    std::optional<std::int64_t> net_cash_from_discontinued_operations;
    std::optional<std::int64_t> net_cash_from_financing_activities;
    std::optional<std::int64_t> net_cash_from_investing_activities;
    std::optional<std::int64_t> net_cash_from_operating_activities;
    std::optional<std::int64_t> net_change_in_cash;
    std::optional<std::int64_t> net_income;
    std::optional<std::int64_t> operating_gains_losses;
    std::optional<std::int64_t> other_financing_activities;
    std::optional<std::int64_t> other_investing_activities;
    std::optional<std::int64_t> other_operating_activities;
    std::optional<std::int64_t> purchases_of_common_stock;
    std::optional<std::int64_t> sales_of_common_stock;
    std::optional<std::int64_t> stock_based_compensation;
    std::optional<std::vector<FinancialItem>> financials;
};

// Financial Ratios
struct FinancialRatios {
    std::optional<std::string> ticker;
    std::optional<std::string> period;
    std::optional<std::string> calendar_date;
    std::optional<std::string> report_period;
    std::optional<std::string> updated_utc;
    std::optional<double> current_ratio;
    std::optional<double> quick_ratio;
    std::optional<double> cash_ratio;
    std::optional<double> days_of_sales_outstanding;
    std::optional<double> days_of_inventory_outstanding;
    std::optional<double> operating_cycle;
    std::optional<double> days_of_payables_outstanding;
    std::optional<double> cash_conversion_cycle;
    std::optional<double> gross_profit_margin;
    std::optional<double> operating_profit_margin;
    std::optional<double> net_profit_margin;
    std::optional<double> effective_tax_rate;
    std::optional<double> return_on_assets;
    std::optional<double> return_on_equity;
    std::optional<double> return_on_invested_capital;
    std::optional<double> return_on_tangible_equity;
    std::optional<double> interest_coverage;
    std::optional<double> long_term_debt_to_equity;
    std::optional<double> total_debt_to_equity;
    std::optional<double> debt_to_equity;
    std::optional<double> debt_to_assets;
    std::optional<double> net_debt_to_ebitda;
    std::optional<double> current_debt_to_equity;
    std::optional<double> long_term_debt_to_assets;
    std::optional<double> total_debt_to_assets;
    std::optional<double> asset_turnover;
    std::optional<double> inventory_turnover;
    std::optional<double> receivables_turnover;
    std::optional<double> accounts_payable_turnover;
    std::optional<double> fixed_asset_turnover;
    std::optional<double> equity_turnover;
    std::optional<double> dividend_yield;
    std::optional<double> dividend_payout_ratio;
    std::optional<double> price_to_book;
    std::optional<double> price_to_earnings;
    std::optional<double> price_to_earnings_to_growth;
    std::optional<double> price_to_sales;
    std::optional<double> price_to_free_cash_flows;
    std::optional<double> price_to_operating_cash_flows;
    std::optional<double> price_to_tangible_book;
    std::optional<double> enterprise_value_multiple;
    std::optional<double> earnings_yield;
    std::optional<double> free_cash_flow_yield;
    std::optional<double> debt_to_ebitda;
    std::optional<double> market_cap_to_operating_cash_flow;
    std::optional<double> enterprise_value_to_operating_cash_flow;
    std::optional<double> enterprise_value_to_sales;
    std::optional<double> enterprise_value_to_ebitda;
    std::optional<double> enterprise_value_to_total_assets;
    std::optional<double> enterprise_value_to_working_capital;
    std::optional<double> enterprise_value_to_invested_capital;
    std::optional<double> enterprise_value_to_revenue;
    std::optional<double> enterprise_value_to_net_income;
    std::optional<double> enterprise_value_to_free_cash_flow;
    std::optional<double> enterprise_value_to_tangible_assets;
    std::optional<double> enterprise_value_to_equity;
    std::optional<double> enterprise_value_to_debt;
    std::optional<double> enterprise_value_to_cash;
    std::optional<double> enterprise_value_to_ebit;
    std::optional<double> enterprise_value_to_ebitda_ratio;
    std::optional<double> enterprise_value_to_operating_income;
    std::optional<double> enterprise_value_to_net_income_ratio;
    std::optional<double> enterprise_value_to_revenue_ratio;
    std::optional<double> enterprise_value_to_free_cash_flow_ratio;
    std::optional<double> enterprise_value_to_total_assets_ratio;
    std::optional<double> enterprise_value_to_working_capital_ratio;
    std::optional<double> enterprise_value_to_invested_capital_ratio;
    std::optional<double> enterprise_value_to_tangible_assets_ratio;
    std::optional<double> enterprise_value_to_equity_ratio;
    std::optional<double> enterprise_value_to_debt_ratio;
    std::optional<double> enterprise_value_to_cash_ratio;
    std::optional<double> enterprise_value_to_ebit_ratio;
    std::optional<double> enterprise_value_to_operating_income_ratio;
};

// Stock Financial (vX endpoint)
struct StockFinancial {
    std::optional<std::string> cik;
    std::optional<std::string> company_name;
    std::optional<std::string> end_date;
    std::optional<std::string> filing_date;
    std::optional<std::string> fiscal_period;
    std::optional<std::string> fiscal_year;
    std::optional<std::string> source_filing_file_url;
    std::optional<std::string> source_filing_url;
    std::optional<std::string> start_date;
    // Note: financials field would contain nested Financials object
    // For now, we'll parse it as a simplified structure
};

} // namespace massive::rest

