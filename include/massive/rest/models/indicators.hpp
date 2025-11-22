#pragma once

#include "massive/rest/models.hpp"
#include <optional>
#include <string>
#include <vector>
#include <cstdint>

namespace massive::rest {

// Indicator Value (for SMA, EMA, RSI)
struct IndicatorValue {
    std::optional<std::int64_t> timestamp;
    std::optional<double> value;
};

// MACD Indicator Value
struct MACDIndicatorValue {
    std::optional<std::int64_t> timestamp;
    std::optional<double> value;
    std::optional<double> signal;
    std::optional<double> histogram;
};

// Indicator Underlying
struct IndicatorUnderlying {
    std::optional<std::string> url;
    std::optional<std::vector<Agg>> aggregates;
};

// Single Indicator Results (SMA, EMA, RSI)
struct SingleIndicatorResults {
    std::optional<std::vector<IndicatorValue>> values;
    std::optional<IndicatorUnderlying> underlying;
};

// MACD Indicator Results
struct MACDIndicatorResults {
    std::optional<std::vector<MACDIndicatorValue>> values;
    std::optional<IndicatorUnderlying> underlying;
};

// Type aliases
using SMAIndicatorResults = SingleIndicatorResults;
using EMAIndicatorResults = SingleIndicatorResults;
using RSIIndicatorResults = SingleIndicatorResults;

}  // namespace massive::rest

