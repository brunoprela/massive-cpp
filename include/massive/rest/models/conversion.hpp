#pragma once

#include <optional>
#include <string>
#include <cstdint>

namespace massive::rest {

// Forex Conversion
struct ForexConversion {
    std::optional<std::string> from;
    std::optional<std::string> to;
    std::optional<double> initial_amount;
    std::optional<double> converted;
    std::optional<std::int64_t> timestamp;
};

// Crypto Conversion
struct CryptoConversion {
    std::optional<std::string> from;
    std::optional<std::string> to;
    std::optional<double> initial_amount;
    std::optional<double> converted;
    std::optional<std::int64_t> timestamp;
};

} // namespace massive::rest

