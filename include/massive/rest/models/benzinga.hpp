#pragma once

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace massive::rest {

// Benzinga News Article
struct BenzingaNews {
    std::optional<std::string> id;
    std::optional<std::string> author;
    std::optional<std::string> created_at;
    std::optional<std::string> updated_at;
    std::optional<std::string> title;
    std::optional<std::string> teaser;
    std::optional<std::string> body;
    std::optional<std::string> url;
    std::optional<std::string> image_url;
    std::optional<std::string> channels;
    std::optional<std::vector<std::string>> stocks;
    std::optional<std::vector<std::string>> tags;
    std::optional<std::vector<std::string>> symbols;
    std::optional<std::int64_t> priority;
    std::optional<std::string> source;
    std::optional<std::string> type;
};

// Benzinga Event
struct BenzingaEvent {
    std::optional<std::string> id;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<std::string> start_date;
    std::optional<std::string> end_date;
    std::optional<std::string> created_at;
    std::optional<std::string> updated_at;
    std::optional<std::string> type;
    std::optional<std::string> subtype;
    std::optional<std::vector<std::string>> symbols;
    std::optional<std::vector<std::string>> stocks;
    std::optional<std::string> status;
    std::optional<std::string> importance;
    std::optional<std::string> timezone;
    std::optional<std::string> venue;
    std::optional<std::string> venue_url;
    std::optional<std::string> ticker;
    std::optional<std::string> exchange;
};

// Benzinga Press Release
struct BenzingaPressRelease {
    std::optional<std::string> id;
    std::optional<std::string> title;
    std::optional<std::string> body;
    std::optional<std::string> url;
    std::optional<std::string> created_at;
    std::optional<std::string> updated_at;
    std::optional<std::vector<std::string>> symbols;
    std::optional<std::vector<std::string>> stocks;
    std::optional<std::string> source;
    std::optional<std::string> type;
};

// Benzinga Analyst Rating
struct BenzingaAnalystRating {
    std::optional<std::string> id;
    std::optional<std::string> analyst_name;
    std::optional<std::string> analyst_firm;
    std::optional<std::string> action;
    std::optional<std::string> rating_current;
    std::optional<std::string> rating_prior;
    std::optional<double> price_target;
    std::optional<double> price_target_prior;
    std::optional<std::string> created_at;
    std::optional<std::string> updated_at;
    std::optional<std::string> ticker;
    std::optional<std::string> exchange;
    std::optional<std::vector<std::string>> symbols;
};

// Benzinga Rating (different from AnalystRating)
struct BenzingaRating {
    std::optional<double> adjusted_price_target;
    std::optional<std::string> analyst;
    std::optional<std::string> benzinga_analyst_id;
    std::optional<std::string> benzinga_calendar_url;
    std::optional<std::string> benzinga_firm_id;
    std::optional<std::string> benzinga_id;
    std::optional<std::string> benzinga_news_url;
    std::optional<std::string> company_name;
    std::optional<std::string> currency;
    std::optional<std::string> date;
    std::optional<std::string> firm;
    std::optional<int> importance;
    std::optional<std::string> last_updated;
    std::optional<std::string> notes;
    std::optional<double> previous_adjusted_price_target;
    std::optional<double> previous_price_target;
    std::optional<std::string> previous_rating;
    std::optional<double> price_percent_change;
    std::optional<double> price_target;
    std::optional<std::string> price_target_action;
    std::optional<std::string> rating;
    std::optional<std::string> rating_action;
    std::optional<std::string> ticker;
    std::optional<std::string> time;
};

// Benzinga Analyst Insight
struct BenzingaAnalystInsight {
    std::optional<std::string> benzinga_firm_id;
    std::optional<std::string> benzinga_id;
    std::optional<std::string> benzinga_rating_id;
    std::optional<std::string> company_name;
    std::optional<std::string> date;
    std::optional<std::string> firm;
    std::optional<std::string> insight;
    std::optional<std::string> last_updated;
    std::optional<double> price_target;
    std::optional<std::string> rating;
    std::optional<std::string> rating_action;
    std::optional<std::string> ticker;
};

// Benzinga Analyst
struct BenzingaAnalyst {
    std::optional<std::string> benzinga_firm_id;
    std::optional<std::string> benzinga_id;
    std::optional<std::string> firm_name;
    std::optional<std::string> full_name;
    std::optional<std::string> last_updated;
    std::optional<double> overall_avg_return;
    std::optional<double> overall_avg_return_percentile;
    std::optional<double> overall_success_rate;
    std::optional<double> smart_score;
    std::optional<double> total_ratings;
    std::optional<double> total_ratings_percentile;
};

// Benzinga Consensus Rating
struct BenzingaConsensusRating {
    std::optional<int> buy_ratings;
    std::optional<double> consensus_price_target;
    std::optional<std::string> consensus_rating;
    std::optional<double> consensus_rating_value;
    std::optional<double> high_price_target;
    std::optional<int> hold_ratings;
    std::optional<double> low_price_target;
    std::optional<int> price_target_contributors;
    std::optional<int> ratings_contributors;
    std::optional<int> sell_ratings;
    std::optional<int> strong_buy_ratings;
    std::optional<int> strong_sell_ratings;
    std::optional<std::string> ticker;
};

// Benzinga Earning
struct BenzingaEarning {
    std::optional<double> actual_eps;
    std::optional<double> actual_revenue;
    std::optional<std::string> benzinga_id;
    std::optional<std::string> company_name;
    std::optional<std::string> currency;
    std::optional<std::string> date;
    std::optional<std::string> date_status;
    std::optional<std::string> eps_method;
    std::optional<double> eps_surprise;
    std::optional<double> eps_surprise_percent;
    std::optional<double> estimated_eps;
    std::optional<double> estimated_revenue;
    std::optional<std::string> fiscal_period;
    std::optional<int> fiscal_year;
    std::optional<int> importance;
    std::optional<std::string> last_updated;
    std::optional<std::string> notes;
    std::optional<double> previous_eps;
    std::optional<double> previous_revenue;
    std::optional<std::string> revenue_method;
    std::optional<double> revenue_surprise;
    std::optional<double> revenue_surprise_percent;
    std::optional<std::string> ticker;
    std::optional<std::string> time;
};

// Benzinga Firm
struct BenzingaFirm {
    std::optional<std::string> benzinga_id;
    std::optional<std::string> currency;
    std::optional<std::string> last_updated;
    std::optional<std::string> name;
};

// Benzinga Guidance
struct BenzingaGuidance {
    std::optional<std::string> benzinga_id;
    std::optional<std::string> company_name;
    std::optional<std::string> currency;
    std::optional<std::string> date;
    std::optional<std::string> eps_method;
    std::optional<double> estimated_eps_guidance;
    std::optional<double> estimated_revenue_guidance;
    std::optional<std::string> fiscal_period;
    std::optional<int> fiscal_year;
    std::optional<int> importance;
    std::optional<std::string> last_updated;
    std::optional<double> max_eps_guidance;
    std::optional<double> max_revenue_guidance;
    std::optional<double> min_eps_guidance;
    std::optional<double> min_revenue_guidance;
    std::optional<std::string> notes;
    std::optional<std::string> positioning;
    std::optional<double> previous_max_eps_guidance;
    std::optional<double> previous_max_revenue_guidance;
    std::optional<double> previous_min_eps_guidance;
    std::optional<double> previous_min_revenue_guidance;
    std::optional<std::string> release_type;
    std::optional<std::string> revenue_method;
    std::optional<std::string> ticker;
    std::optional<std::string> time;
};

} // namespace massive::rest

