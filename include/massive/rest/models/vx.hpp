#pragma once

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace massive::rest {

// VX Volatility Data
struct VxVolatility {
    std::optional<std::string> ticker;
    std::optional<std::string> date;
    std::optional<double> implied_volatility;
    std::optional<double> historical_volatility;
    std::optional<double> vix;
    std::optional<double> vix_open;
    std::optional<double> vix_high;
    std::optional<double> vix_low;
    std::optional<double> vix_close;
    std::optional<double> vix_volume;
    std::optional<std::int64_t> timestamp;
};

// VX Experimental Data Point
struct VxDataPoint {
    std::optional<std::string> ticker;
    std::optional<std::string> date;
    std::optional<std::int64_t> timestamp;
    std::optional<double> value;
    std::optional<std::string> metric;
    std::optional<std::string> type;
    std::optional<std::map<std::string, std::string>> metadata;
};

} // namespace massive::rest

