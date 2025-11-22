#pragma once

#include <optional>
#include <string>
#include <vector>
#include <cstdint>
#include <variant>

namespace massive::websocket {

// Enums
enum class Feed {
    Delayed,
    RealTime,
    Nasdaq,
    PolyFeed,
    PolyFeedPlus,
    StarterFeed,
    Launchpad,
    Business,
    EdgxBusiness,
    IEXBusiness,
    DelayedBusiness,
    DelayedEdgxBusiness,
    DelayedNasdaqLastSaleBusiness,
    DelayedNasdaqBasic,
    DelayedFullMarketBusiness,
    FullMarketBusiness,
    NasdaqLastSaleBusiness,
    NasdaqBasicBusiness
};

enum class Market {
    Stocks,
    Options,
    Forex,
    Crypto,
    Indices,
    Futures,
    FuturesCME,
    FuturesCBOT,
    FuturesNYMEX,
    FuturesCOMEX
};

enum class EventType {
    EquityAgg,
    EquityAggMin,
    CryptoAgg,
    CryptoAggSec,
    ForexAgg,
    ForexAggSec,
    EquityTrade,
    CryptoTrade,
    EquityQuote,
    ForexQuote,
    CryptoQuote,
    Imbalances,
    LimitUpLimitDown,
    CryptoL2,
    Value,
    LaunchpadValue,
    LaunchpadAggMin,
    BusinessFairMarketValue,
    FuturesTrade,
    FuturesQuote,
    FuturesAgg,
    FuturesAggMin
};

// WebSocket Message Models
struct EquityAgg {
    std::optional<std::string> event_type;
    std::optional<std::string> symbol;
    std::optional<double> volume;
    std::optional<double> accumulated_volume;
    std::optional<double> official_open_price;
    std::optional<double> vwap;
    std::optional<double> open;
    std::optional<double> close;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> aggregate_vwap;
    std::optional<double> average_size;
    std::optional<std::int64_t> start_timestamp;
    std::optional<std::int64_t> end_timestamp;
    std::optional<bool> otc;
};

struct EquityTrade {
    std::optional<std::string> event_type;
    std::optional<std::string> symbol;
    std::optional<std::int64_t> exchange;
    std::optional<std::string> id;
    std::optional<std::int64_t> tape;
    std::optional<double> price;
    std::optional<std::int64_t> size;
    std::optional<std::vector<std::int64_t>> conditions;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> sequence_number;
    std::optional<std::int64_t> trf_id;
    std::optional<std::int64_t> trf_timestamp;
};

struct EquityQuote {
    std::optional<std::string> event_type;
    std::optional<std::string> symbol;
    std::optional<std::int64_t> bid_exchange_id;
    std::optional<double> bid_price;
    std::optional<std::int64_t> bid_size;
    std::optional<std::int64_t> ask_exchange_id;
    std::optional<double> ask_price;
    std::optional<std::int64_t> ask_size;
    std::optional<std::int64_t> condition;
    std::optional<std::vector<std::int64_t>> indicators;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> tape;
    std::optional<std::int64_t> sequence_number;
    std::optional<std::int64_t> trf_id;
    std::optional<std::int64_t> trf_timestamp;
};

struct CryptoTrade {
    std::optional<std::string> event_type;
    std::optional<std::string> pair;
    std::optional<std::int64_t> exchange;
    std::optional<std::string> id;
    std::optional<double> price;
    std::optional<double> size;
    std::optional<std::vector<std::int64_t>> conditions;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> received_timestamp;
};

struct CryptoQuote {
    std::optional<std::string> event_type;
    std::optional<std::string> pair;
    std::optional<double> bid_price;
    std::optional<double> bid_size;
    std::optional<double> ask_price;
    std::optional<double> ask_size;
    std::optional<std::int64_t> timestamp;
    std::optional<std::int64_t> exchange_id;
    std::optional<std::int64_t> received_timestamp;
};

// WebSocket message variant type
using WebSocketMessage = std::variant<
    EquityAgg,
    EquityTrade,
    EquityQuote,
    CryptoTrade,
    CryptoQuote
>;

// Helper functions
inline std::string to_string(Feed feed) {
    switch (feed) {
        case Feed::Delayed: return "delayed.massive.com";
        case Feed::RealTime: return "socket.massive.com";
        case Feed::Nasdaq: return "nasdaqfeed.massive.com";
        case Feed::PolyFeed: return "polyfeed.massive.com";
        case Feed::PolyFeedPlus: return "polyfeedplus.massive.com";
        case Feed::StarterFeed: return "starterfeed.massive.com";
        case Feed::Launchpad: return "launchpad.massive.com";
        case Feed::Business: return "business.massive.com";
        default: return "socket.massive.com";
    }
}

inline std::string to_string(Market market) {
    switch (market) {
        case Market::Stocks: return "stocks";
        case Market::Options: return "options";
        case Market::Forex: return "forex";
        case Market::Crypto: return "crypto";
        case Market::Indices: return "indices";
        case Market::Futures: return "futures";
        case Market::FuturesCME: return "futures/cme";
        case Market::FuturesCBOT: return "futures/cbot";
        case Market::FuturesNYMEX: return "futures/nymex";
        case Market::FuturesCOMEX: return "futures/comex";
        default: return "stocks";
    }
}

}  // namespace massive::websocket

