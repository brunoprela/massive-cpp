#pragma once

#include "massive/rest/models.hpp"
#include <optional>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace massive::rest {

// Enums
enum class Direction {
    Gainers,
    Losers
};

enum class SnapshotMarketType {
    Stocks,
    Forex,
    Crypto,
    Indices
};

// Minute Snapshot
struct MinuteSnapshot {
    std::optional<double> accumulated_volume;
    std::optional<double> open;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> close;
    std::optional<double> volume;
    std::optional<double> vwap;
    std::optional<bool> otc;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> transactions;
};

// Ticker Snapshot
struct TickerSnapshot {
    std::optional<Agg> day;
    std::optional<LastQuote> last_quote;
    std::optional<LastTrade> last_trade;
    std::optional<MinuteSnapshot> min;
    std::optional<Agg> prev_day;
    std::optional<std::string> ticker;
    std::optional<double> todays_change;
    std::optional<double> todays_change_percent;
    std::optional<std::int64_t> updated;
    std::optional<double> fair_market_value;
};

// Indices Session
struct IndicesSession {
    std::optional<double> change;
    std::optional<double> change_percent;
    std::optional<double> close;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> open;
    std::optional<double> previous_close;
};

// Indices Snapshot
struct IndicesSnapshot {
    std::optional<double> value;
    std::optional<std::string> name;
    std::optional<std::string> type;
    std::optional<std::string> ticker;
    std::optional<std::string> market_status;
    std::optional<IndicesSession> session;
    std::optional<std::string> error;
    std::optional<std::string> message;
};

// Order Book Quote
struct OrderBookQuote {
    std::optional<double> price;
    std::optional<std::map<std::string, double>> exchange_shares;
};

// Snapshot Ticker Full Book
struct SnapshotTickerFullBook {
    std::optional<std::string> ticker;
    std::optional<std::vector<OrderBookQuote>> bids;
    std::optional<std::vector<OrderBookQuote>> asks;
    std::optional<double> bid_count;
    std::optional<double> ask_count;
    std::optional<double> spread;
    std::optional<std::int64_t> updated;
};

// Universal Snapshot Session
struct UniversalSnapshotSession {
    std::optional<double> price;
    std::optional<double> change;
    std::optional<double> change_percent;
    std::optional<double> early_trading_change;
    std::optional<double> early_trading_change_percent;
    std::optional<double> regular_trading_change;
    std::optional<double> regular_trading_change_percent;
    std::optional<double> late_trading_change;
    std::optional<double> late_trading_change_percent;
    std::optional<double> open;
    std::optional<double> close;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> previous_close;
    std::optional<double> volume;
};

// Universal Snapshot Last Quote
struct UniversalSnapshotLastQuote {
    std::optional<double> ask;
    std::optional<double> ask_size;
    std::optional<double> bid;
    std::optional<double> bid_size;
    std::optional<double> midpoint;
    std::optional<std::int64_t> exchange;
    std::optional<std::string> timeframe;
    std::optional<std::int64_t> last_updated;
};

// Universal Snapshot Last Trade
struct UniversalSnapshotLastTrade {
    std::optional<std::int64_t> id;
    std::optional<double> price;
    std::optional<std::int64_t> size;
    std::optional<std::int64_t> exchange;
    std::optional<std::vector<std::int64_t>> conditions;
    std::optional<std::string> timeframe;
    std::optional<std::int64_t> last_updated;
    std::optional<std::int64_t> participant_timestamp;
    std::optional<std::int64_t> sip_timestamp;
};

// Greeks
struct Greeks {
    std::optional<double> delta;
    std::optional<double> gamma;
    std::optional<double> theta;
    std::optional<double> vega;
};

// Universal Snapshot Underlying Asset
struct UniversalSnapshotUnderlyingAsset {
    std::optional<std::string> ticker;
    std::optional<double> price;
    std::optional<double> value;
    std::optional<double> change_to_break_even;
    std::optional<std::string> timeframe;
    std::optional<std::int64_t> last_updated;
};

// Universal Snapshot Details
struct UniversalSnapshotDetails {
    std::optional<std::string> contract_type;
    std::optional<std::string> exercise_style;
    std::optional<std::string> expiration_date;
    std::optional<double> shares_per_contract;
    std::optional<double> strike_price;
};

// Universal Snapshot
struct UniversalSnapshot {
    std::optional<std::string> ticker;
    std::optional<std::string> type;
    std::optional<UniversalSnapshotSession> session;
    std::optional<UniversalSnapshotLastQuote> last_quote;
    std::optional<UniversalSnapshotLastTrade> last_trade;
    std::optional<Greeks> greeks;
    std::optional<UniversalSnapshotUnderlyingAsset> underlying_asset;
    std::optional<UniversalSnapshotDetails> details;
    std::optional<double> break_even_price;
    std::optional<double> implied_volatility;
    std::optional<double> open_interest;
    std::optional<std::string> market_status;
    std::optional<std::string> name;
    std::optional<double> fair_market_value;
    std::optional<std::string> error;
    std::optional<std::string> message;
};

// Helper functions for enum conversion
inline std::string to_string(Direction dir) {
    return dir == Direction::Gainers ? "gainers" : "losers";
}

inline std::string to_string(SnapshotMarketType type) {
    switch (type) {
        case SnapshotMarketType::Stocks: return "stocks";
        case SnapshotMarketType::Forex: return "forex";
        case SnapshotMarketType::Crypto: return "crypto";
        case SnapshotMarketType::Indices: return "indices";
        default: return "stocks";
    }
}

inline std::string get_locale(SnapshotMarketType type) {
    return type == SnapshotMarketType::Stocks ? "us" : "global";
}

}  // namespace massive::rest

