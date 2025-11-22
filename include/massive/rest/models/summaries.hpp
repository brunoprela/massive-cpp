#pragma once

#include "massive/rest/models/reference.hpp"
#include <optional>
#include <string>
#include <cstdint>

namespace massive::rest {

// Session (for summaries)
struct SummarySession {
    std::optional<double> change;
    std::optional<double> change_percent;
    std::optional<double> early_trading_change;
    std::optional<double> early_trading_change_percent;
    std::optional<double> late_trading_change;
    std::optional<double> late_trading_change_percent;
    std::optional<double> close;
    std::optional<double> high;
    std::optional<double> low;
    std::optional<double> open;
    std::optional<double> previous_close;
    std::optional<double> volume;
};

// Options (for summaries)
struct SummaryOptions {
    std::optional<std::string> contract_type;
    std::optional<std::string> exercise_style;
    std::optional<std::string> expiration_date;
    std::optional<double> shares_per_contract;
    std::optional<double> strike_price;
    std::optional<double> underlying_ticker;
};

// Summary Result
struct SummaryResult {
    std::optional<double> price;
    std::optional<std::string> name;
    std::optional<std::string> ticker;
    std::optional<Branding> branding;
    std::optional<std::string> market_status;
    std::optional<std::int64_t> last_updated;
    std::optional<std::string> type;
    std::optional<SummarySession> session;
    std::optional<SummaryOptions> options;
    std::optional<std::string> error;
    std::optional<std::string> message;
};

}  // namespace massive::rest

