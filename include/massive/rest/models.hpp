#pragma once

#include <optional>
#include <string>
#include <cstdint>
#include <vector>

namespace massive::rest {

// Aggregate (Bar) model
struct Agg {
    std::optional<double> open;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> close;
    std::optional<double> volume;
    std::optional<double> vwap;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> transactions;
    std::optional<bool> otc;
};

// Grouped Daily Aggregate model
struct GroupedDailyAgg {
    std::optional<std::string> ticker;
    std::optional<double> open;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> close;
    std::optional<double> volume;
    std::optional<double> vwap;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> transactions;
    std::optional<bool> otc;
};

// Daily Open Close Aggregate model
struct DailyOpenCloseAgg {
    std::optional<double> after_hours;
    std::optional<double> close;
    std::optional<std::string> from;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> open;
    std::optional<double> pre_market;
    std::optional<std::string> status;
    std::optional<std::string> symbol;
    std::optional<double> volume;
    std::optional<bool> otc;
};

// Previous Close Aggregate model
struct PreviousCloseAgg {
    std::optional<std::string> ticker;
    std::optional<double> close;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> open;
    std::optional<double> timestamp;
    std::optional<double> volume;
    std::optional<double> vwap;
};

// Trade model
struct Trade {
    std::optional<double> price;
    std::optional<std::int64_t> size;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> exchange;
    std::optional<std::int64_t> conditions;
    std::optional<std::string> participant_timestamp;
    std::optional<bool> sip;
    std::optional<std::string> trf_id;
    std::optional<std::int64_t> trf_timestamp;
};

// Last Trade model
struct LastTrade {
    std::optional<std::string> ticker;
    std::optional<double> price;
    std::optional<std::int64_t> size;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> exchange;
    std::optional<std::int64_t> conditions;
    std::optional<std::string> participant_timestamp;
    std::optional<bool> sip;
    std::optional<std::string> trf_id;
    std::optional<std::int64_t> trf_timestamp;
};

// Quote model
struct Quote {
    std::optional<double> ask;
    std::optional<std::int64_t> ask_size;
    std::optional<std::int64_t> ask_exchange;
    std::optional<double> bid;
    std::optional<std::int64_t> bid_size;
    std::optional<std::int64_t> bid_exchange;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> exchange;
    std::optional<std::int64_t> conditions;
    std::optional<std::string> participant_timestamp;
    std::optional<bool> sip;
    std::optional<std::string> trf_id;
    std::optional<std::int64_t> trf_timestamp;
};

// Last Quote model
struct LastQuote {
    std::optional<std::string> ticker;
    std::optional<double> ask;
    std::optional<std::int64_t> ask_size;
    std::optional<std::int64_t> ask_exchange;
    std::optional<double> bid;
    std::optional<std::int64_t> bid_size;
    std::optional<std::int64_t> bid_exchange;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> exchange;
    std::optional<std::int64_t> conditions;
    std::optional<std::string> participant_timestamp;
    std::optional<bool> sip;
    std::optional<std::string> trf_id;
    std::optional<std::int64_t> trf_timestamp;
};

// Crypto Trade model
struct CryptoTrade {
    std::optional<double> price;
    std::optional<double> size;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> exchange;
    std::optional<std::vector<std::int64_t>> conditions;
};

// Forex Quote model
struct ForexQuote {
    std::optional<double> ask;
    std::optional<double> bid;
    std::optional<std::int64_t> exchange;
    std::optional<std::int64_t> timestamp;
};

// Last Forex Quote model
struct LastForexQuote {
    std::optional<ForexQuote> last;
    std::optional<std::string> symbol;
};

// Real-Time Currency Conversion model
struct RealTimeCurrencyConversion {
    std::optional<double> converted;
    std::optional<std::string> from;
    std::optional<double> initial_amount;
    std::optional<ForexQuote> last;
    std::optional<std::string> to;
};

}  // namespace massive::rest

