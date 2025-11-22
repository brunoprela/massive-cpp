#pragma once

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace massive::rest {

// ETF Holding
struct EtfHolding {
    std::optional<std::string> ticker;
    std::optional<std::string> name;
    std::optional<double> weight;
    std::optional<std::int64_t> shares;
    std::optional<double> market_value;
    std::optional<std::string> currency;
    std::optional<std::string> exchange;
    std::optional<std::string> sector;
    std::optional<std::string> industry;
};

// ETF Details
struct EtfDetails {
    std::optional<std::string> ticker;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<std::string> exchange;
    std::optional<std::string> currency;
    std::optional<std::string> country;
    std::optional<std::string> category;
    std::optional<std::string> family;
    std::optional<std::string> brand;
    std::optional<double> total_assets;
    std::optional<double> net_assets;
    std::optional<double> nav;
    std::optional<std::string> nav_date;
    std::optional<double> expense_ratio;
    std::optional<double> dividend_yield;
    std::optional<std::string> dividend_frequency;
    std::optional<std::string> inception_date;
    std::optional<std::string> website;
    std::optional<std::string> isin;
    std::optional<std::string> cusip;
    std::optional<std::string> figi;
    std::optional<std::string> composite_figi;
    std::optional<std::string> share_class_figi;
    std::optional<bool> is_active;
    std::optional<std::string> last_updated_utc;
};

// ETF Performance
struct EtfPerformance {
    std::optional<std::string> ticker;
    std::optional<std::string> date;
    std::optional<double> nav;
    std::optional<double> price;
    std::optional<double> change;
    std::optional<double> change_percent;
    std::optional<double> volume;
    std::optional<double> total_assets;
    std::optional<double> expense_ratio;
    std::optional<double> dividend_yield;
    std::optional<double> return_1d;
    std::optional<double> return_1w;
    std::optional<double> return_1m;
    std::optional<double> return_3m;
    std::optional<double> return_6m;
    std::optional<double> return_1y;
    std::optional<double> return_3y;
    std::optional<double> return_5y;
    std::optional<double> return_10y;
    std::optional<double> return_ytd;
    std::optional<double> return_since_inception;
};

// ETF List Item
struct EtfListItem {
    std::optional<std::string> ticker;
    std::optional<std::string> name;
    std::optional<std::string> exchange;
    std::optional<std::string> currency;
    std::optional<std::string> category;
    std::optional<double> total_assets;
    std::optional<double> nav;
    std::optional<double> expense_ratio;
    std::optional<bool> is_active;
};

// ETF Global Analytics
struct EtfGlobalAnalytics {
    std::optional<std::string> composite_ticker;
    std::optional<std::string> effective_date;
    std::optional<std::string> processed_date;
    std::optional<double> quant_total_score;
    std::optional<double> reward_score;
    std::optional<double> risk_total_score;
    std::optional<std::string> quant_grade;
    std::optional<double> quant_composite_technical;
    std::optional<double> quant_composite_sentiment;
    std::optional<double> quant_composite_behavioral;
    std::optional<double> quant_composite_fundamental;
    std::optional<double> quant_composite_global;
    std::optional<double> quant_composite_quality;
};

// ETF Global Constituent
struct EtfGlobalConstituent {
    std::optional<std::string> composite_ticker;
    std::optional<std::string> constituent_ticker;
    std::optional<std::string> constituent_name;
    std::optional<double> weight;
    std::optional<double> shares_held;
    std::optional<double> market_value;
    std::optional<std::string> effective_date;
    std::optional<std::string> processed_date;
    std::optional<std::string> asset_class;
    std::optional<std::string> exchange;
    std::optional<std::string> country_of_exchange;
    std::optional<std::string> currency_traded;
    std::optional<std::string> security_type;
    std::optional<std::string> us_code;
    std::optional<std::string> isin;
    std::optional<std::string> figi;
    std::optional<std::string> sedol;
};

// ETF Global Fund Flow
struct EtfGlobalFundFlow {
    std::optional<std::string> composite_ticker;
    std::optional<std::string> effective_date;
    std::optional<std::string> processed_date;
    std::optional<double> fund_flow;
    std::optional<double> nav;
    std::optional<double> shares_outstanding;
};

// ETF Global Profile
struct EtfGlobalProfile {
    std::optional<std::string> composite_ticker;
    std::optional<std::string> effective_date;
    std::optional<std::string> processed_date;
    std::optional<std::string> asset_class;
    std::optional<std::string> category;
    std::optional<std::string> description;
    std::optional<double> aum;
    std::optional<double> management_fee;
    std::optional<double> net_expenses;
    std::optional<double> total_expenses;
    std::optional<std::string> inception_date;
    std::optional<std::string> listing_exchange;
    std::optional<std::string> issuer;
    std::optional<std::string> advisor;
    std::optional<std::string> administrator;
    std::optional<std::string> custodian;
    std::optional<std::string> distributor;
    std::optional<std::string> transfer_agent;
    std::optional<std::string> trustee;
    std::optional<std::string> portfolio_manager;
    std::optional<std::string> primary_benchmark;
    std::optional<std::string> product_type;
    std::optional<std::string> region;
    std::optional<std::string> focus;
    std::optional<std::string> leverage_style;
    std::optional<double> levered_amount;
    std::optional<std::string> management_classification;
    std::optional<std::string> tax_classification;
    std::optional<std::string> development_class;
    std::optional<double> num_holdings;
    std::optional<double> avg_daily_trading_volume;
    std::optional<double> bid_ask_spread;
    std::optional<double> discount_premium;
    std::optional<double> short_interest;
    std::optional<int> options_available;
    std::optional<double> options_volume;
    std::optional<double> call_volume;
    std::optional<double> put_volume;
    std::optional<double> put_call_ratio;
};

// ETF Global Taxonomy
struct EtfGlobalTaxonomy {
    std::optional<std::string> composite_ticker;
    std::optional<std::string> effective_date;
    std::optional<std::string> processed_date;
    std::optional<std::string> asset_class;
    std::optional<std::string> category;
    std::optional<std::string> country;
    std::optional<std::string> description;
    std::optional<std::string> development_class;
    std::optional<std::string> duration;
    std::optional<std::string> esg;
    std::optional<std::string> exposure_mechanism;
    std::optional<std::string> factor;
    std::optional<std::string> focus;
    std::optional<std::string> hedge_reset;
    std::optional<std::string> holdings_disclosure_frequency;
    std::optional<std::string> inception_date;
    std::optional<std::string> isin;
    std::optional<std::string> issuer;
    std::optional<std::string> leverage_reset;
    std::optional<std::string> leverage_style;
    std::optional<double> levered_amount;
    std::optional<std::string> management_classification;
    std::optional<std::string> management_style;
    std::optional<std::string> maturity;
    std::optional<std::string> objective;
    std::optional<std::string> primary_benchmark;
    std::optional<std::string> product_type;
    std::optional<std::string> rebalance_frequency;
    std::optional<std::string> reconstitution_frequency;
    std::optional<std::string> region;
    std::optional<std::string> secondary_objective;
    std::optional<std::string> selection_methodology;
    std::optional<std::string> selection_universe;
    std::optional<std::string> strategic_focus;
    std::optional<std::string> targeted_focus;
    std::optional<std::string> tax_classification;
    std::optional<std::string> us_code;
    std::optional<std::string> weighting_methodology;
    std::optional<std::string> credit_quality_rating;
};

} // namespace massive::rest

