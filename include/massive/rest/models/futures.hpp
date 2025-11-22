#pragma once

#include <optional>
#include <string>
#include <cstdint>

namespace massive::rest {

// Futures Aggregate
struct FuturesAgg {
    std::optional<std::string> ticker;
    std::optional<double> open;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> close;
    std::optional<double> volume;
    std::optional<double> dollar_volume;
    std::optional<std::int64_t> transactions;
    std::optional<std::int64_t> window_start;
    std::optional<std::string> session_end_date;
    std::optional<double> settlement_price;
};

// Futures Contract
struct FuturesContract {
    std::optional<std::string> ticker;
    std::optional<std::string> product_code;
    std::optional<std::string> trading_venue;
    std::optional<std::string> name;
    std::optional<std::string> type;
    std::optional<std::string> as_of;
    std::optional<bool> active;
    std::optional<std::string> first_trade_date;
    std::optional<std::string> last_trade_date;
    std::optional<int> days_to_maturity;
    std::optional<int> min_order_quantity;
    std::optional<int> max_order_quantity;
    std::optional<std::string> settlement_date;
    std::optional<double> settlement_tick_size;
    std::optional<double> spread_tick_size;
    std::optional<double> trade_tick_size;
    std::optional<std::string> maturity;
};

// Futures Quote
struct FuturesQuote {
    std::optional<std::string> ticker;
    std::optional<double> bid_price;
    std::optional<std::int64_t> bid_size;
    std::optional<std::int64_t> bid_timestamp;
    std::optional<double> ask_price;
    std::optional<std::int64_t> ask_size;
    std::optional<std::int64_t> ask_timestamp;
    std::optional<std::int64_t> sip_timestamp;
};

// Futures Trade
struct FuturesTrade {
    std::optional<std::string> ticker;
    std::optional<double> price;
    std::optional<std::int64_t> size;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> sequence_number;
};

// Futures Product
struct FuturesProduct {
    std::optional<std::string> product_code;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<std::string> exchange;
    std::optional<std::string> type;
    std::optional<std::string> currency;
    std::optional<std::string> unit;
    std::optional<double> tick_size;
    std::optional<double> contract_size;
    std::optional<std::string> settlement_type;
};

// Futures Schedule
struct FuturesSchedule {
    std::optional<std::string> session_open;
    std::optional<std::string> session_close;
    std::optional<std::string> timezone;
    std::vector<std::string> trading_days;
};

// Futures Market Status
struct FuturesMarketStatus {
    std::optional<std::string> market;
    std::optional<std::string> server_time;
    std::optional<std::string> exchanges;
    std::optional<std::string> currencies;
};

// Futures Snapshot
struct FuturesSnapshot {
    std::optional<std::string> ticker;
    std::optional<double> day_open;
    std::optional<double> day_high;
    std::optional<double> day_low;
    std::optional<double> day_close;
    std::optional<double> day_volume;
    std::optional<double> prev_day_open;
    std::optional<double> prev_day_high;
    std::optional<double> prev_day_low;
    std::optional<double> prev_day_close;
    std::optional<double> prev_day_volume;
    std::optional<std::int64_t> updated;
};

// Futures Exchange
struct FuturesExchange {
    std::optional<std::string> id;
    std::optional<std::string> type;
    std::optional<std::string> market;
    std::optional<std::string> name;
    std::optional<std::string> mic;
};

}  // namespace massive::rest

