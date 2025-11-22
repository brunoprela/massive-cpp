#pragma once

#include "massive/rest/models.hpp"
#include <optional>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace massive::rest {

// TMX Trade
struct TmxTrade {
    std::optional<std::string> ticker;
    std::optional<double> price;
    std::optional<std::int64_t> size;
    std::optional<std::int64_t> timestamp;
    std::optional<std::string> exchange;
    std::optional<std::string> conditions;
    std::optional<std::string> participant_timestamp;
};

// TMX Quote
struct TmxQuote {
    std::optional<std::string> ticker;
    std::optional<double> bid_price;
    std::optional<double> ask_price;
    std::optional<std::int64_t> bid_size;
    std::optional<std::int64_t> ask_size;
    std::optional<std::int64_t> timestamp;
    std::optional<std::string> exchange;
    std::optional<std::int64_t> participant_timestamp;
};

// TMX Agg
struct TmxAgg {
    std::optional<std::string> ticker;
    std::optional<double> open;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> close;
    std::optional<double> volume;
    std::optional<double> vwap;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> transactions;
};

// TMX Ticker Details
struct TmxTickerDetails {
    std::optional<std::string> ticker;
    std::optional<std::string> name;
    std::optional<std::string> exchange;
    std::optional<std::string> market;
    std::optional<std::string> locale;
    std::optional<std::string> primary_exchange;
    std::optional<std::string> type;
    std::optional<bool> active;
    std::optional<std::string> currency;
    std::optional<std::string> market_cap;
    std::optional<std::string> description;
    std::optional<std::string> homepage_url;
    std::optional<std::int64_t> total_employees;
    std::optional<std::string> list_date;
    std::optional<std::string> sic_code;
    std::optional<std::string> sic_description;
    std::optional<std::string> ticker_root;
    std::optional<std::string> ticker_suffix;
    std::optional<std::string> composite_figi;
    std::optional<std::string> share_class_figi;
};

// TMX Corporate Event
struct TmxCorporateEvent {
    std::optional<std::string> company_name;
    std::optional<std::string> date;
    std::optional<std::string> isin;
    std::optional<std::string> name;
    std::optional<std::string> status;
    std::optional<std::string> ticker;
    std::optional<std::int64_t> tmx_company_id;
    std::optional<std::string> tmx_record_id;
    std::optional<std::string> trading_venue;
    std::optional<std::string> type;
    std::optional<std::string> url;
};

} // namespace massive::rest

