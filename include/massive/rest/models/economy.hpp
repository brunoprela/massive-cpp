#pragma once

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace massive::rest {

// Economic Indicator
struct EconomicIndicator {
    std::optional<std::string> id;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<std::string> country;
    std::optional<std::string> unit;
    std::optional<std::string> frequency;
    std::optional<std::string> source;
    std::optional<std::string> last_updated;
    std::optional<double> value;
    std::optional<std::string> date;
    std::optional<std::string> period;
    std::optional<double> previous_value;
    std::optional<std::string> previous_date;
    std::optional<double> change;
    std::optional<double> change_percent;
    std::optional<std::string> category;
    std::optional<std::string> subcategory;
};

// Economic Calendar Event
struct EconomicCalendarEvent {
    std::optional<std::string> id;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<std::string> country;
    std::optional<std::string> date;
    std::optional<std::string> time;
    std::optional<std::string> timezone;
    std::optional<std::string> importance;
    std::optional<std::string> category;
    std::optional<std::string> subcategory;
    std::optional<double> actual;
    std::optional<double> forecast;
    std::optional<double> previous;
    std::optional<std::string> unit;
    std::optional<std::string> source;
    std::optional<std::string> ticker;
    std::optional<std::vector<std::string>> related_tickers;
};

// Economic Data Point
struct EconomicDataPoint {
    std::optional<std::string> date;
    std::optional<double> value;
    std::optional<std::string> period;
    std::optional<double> change;
    std::optional<double> change_percent;
    std::optional<std::string> unit;
};

// Treasury Yield
struct TreasuryYield {
    std::optional<std::string> date;
    std::optional<double> yield_1_month;
    std::optional<double> yield_3_month;
    std::optional<double> yield_6_month;
    std::optional<double> yield_1_year;
    std::optional<double> yield_2_year;
    std::optional<double> yield_3_year;
    std::optional<double> yield_5_year;
    std::optional<double> yield_7_year;
    std::optional<double> yield_10_year;
    std::optional<double> yield_20_year;
    std::optional<double> yield_30_year;
};

// Fed Inflation
struct FedInflation {
    std::optional<double> cpi;
    std::optional<double> cpi_core;
    std::optional<double> cpi_year_over_year;
    std::optional<std::string> date;
    std::optional<double> pce;
    std::optional<double> pce_core;
    std::optional<double> pce_spending;
};

// Fed Inflation Expectations
struct FedInflationExpectations {
    std::optional<std::string> date;
    std::optional<double> forward_years_5_to_10;
    std::optional<double> market_10_year;
    std::optional<double> market_5_year;
    std::optional<double> model_10_year;
    std::optional<double> model_1_year;
    std::optional<double> model_30_year;
    std::optional<double> model_5_year;
};

} // namespace massive::rest

