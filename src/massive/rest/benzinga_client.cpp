#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

// Benzinga - News
std::vector<BenzingaNews> RESTClient::list_benzinga_news(
    const std::optional<std::string> &ticker,
    const std::optional<std::vector<std::string>> &tickers_any_of, std::optional<int> limit,
    const std::optional<std::string> &published_utc_gte,
    const std::optional<std::string> &published_utc_lte, const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (tickers_any_of.has_value() && !tickers_any_of->empty()) {
        std::string tickers_str;
        for (size_t i = 0; i < tickers_any_of->size(); ++i) {
            if (i > 0)
                tickers_str += ",";
            tickers_str += (*tickers_any_of)[i];
        }
        params["tickers.any_of"] = tickers_str;
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (published_utc_gte.has_value()) {
        params["published_utc.gte"] = published_utc_gte.value();
    }
    if (published_utc_lte.has_value()) {
        params["published_utc.lte"] = published_utc_lte.value();
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/v1/news";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaNews> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaNews news;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        news.id = std::string(id_field.value().get_string().value());
                    }

                    auto title_field = obj.find_field_unordered("title");
                    if (!title_field.error()) {
                        news.title = std::string(title_field.value().get_string().value());
                    }

                    auto author_field = obj.find_field_unordered("author");
                    if (!author_field.error()) {
                        news.author = std::string(author_field.value().get_string().value());
                    }

                    auto created_at_field = obj.find_field_unordered("created_at");
                    if (!created_at_field.error()) {
                        news.created_at =
                            std::string(created_at_field.value().get_string().value());
                    }

                    auto url_field = obj.find_field_unordered("url");
                    if (!url_field.error()) {
                        news.url = std::string(url_field.value().get_string().value());
                    }

                    auto symbols_field = obj.find_field_unordered("symbols");
                    if (!symbols_field.error()) {
                        auto symbols_array = symbols_field.value().get_array();
                        if (!symbols_array.error()) {
                            std::vector<std::string> symbols;
                            for (auto sym_elem : symbols_array.value()) {
                                symbols.push_back(std::string(sym_elem.get_string().value()));
                            }
                            news.symbols = symbols;
                        }
                    }

                    results.push_back(news);
                }
            }
        }
    }

    return results;
}

// Benzinga - Events
std::vector<BenzingaEvent> RESTClient::list_benzinga_events(
    const std::optional<std::string> &ticker,
    const std::optional<std::vector<std::string>> &tickers_any_of, std::optional<int> limit,
    const std::optional<std::string> &start_date_gte,
    const std::optional<std::string> &start_date_lte, const std::optional<std::string> &types,
    const std::optional<std::string> &sort, const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (tickers_any_of.has_value() && !tickers_any_of->empty()) {
        std::string tickers_str;
        for (size_t i = 0; i < tickers_any_of->size(); ++i) {
            if (i > 0)
                tickers_str += ",";
            tickers_str += (*tickers_any_of)[i];
        }
        params["tickers.any_of"] = tickers_str;
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (start_date_gte.has_value()) {
        params["start_date.gte"] = start_date_gte.value();
    }
    if (start_date_lte.has_value()) {
        params["start_date.lte"] = start_date_lte.value();
    }
    if (types.has_value()) {
        params["types"] = types.value();
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/v1/events";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaEvent> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaEvent event;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        event.id = std::string(id_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        event.name = std::string(name_field.value().get_string().value());
                    }

                    auto type_field = obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        event.type = std::string(type_field.value().get_string().value());
                    }

                    auto start_date_field = obj.find_field_unordered("start_date");
                    if (!start_date_field.error()) {
                        event.start_date =
                            std::string(start_date_field.value().get_string().value());
                    }

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        event.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    results.push_back(event);
                }
            }
        }
    }

    return results;
}

// Benzinga - Press Releases
std::vector<BenzingaPressRelease> RESTClient::list_benzinga_press_releases(
    const std::optional<std::string> &ticker,
    const std::optional<std::vector<std::string>> &tickers_any_of, std::optional<int> limit,
    const std::optional<std::string> &published_utc_gte,
    const std::optional<std::string> &published_utc_lte, const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (tickers_any_of.has_value() && !tickers_any_of->empty()) {
        std::string tickers_str;
        for (size_t i = 0; i < tickers_any_of->size(); ++i) {
            if (i > 0)
                tickers_str += ",";
            tickers_str += (*tickers_any_of)[i];
        }
        params["tickers.any_of"] = tickers_str;
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (published_utc_gte.has_value()) {
        params["published_utc.gte"] = published_utc_gte.value();
    }
    if (published_utc_lte.has_value()) {
        params["published_utc.lte"] = published_utc_lte.value();
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/v1/press-releases";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaPressRelease> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaPressRelease release;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        release.id = std::string(id_field.value().get_string().value());
                    }

                    auto title_field = obj.find_field_unordered("title");
                    if (!title_field.error()) {
                        release.title = std::string(title_field.value().get_string().value());
                    }

                    auto url_field = obj.find_field_unordered("url");
                    if (!url_field.error()) {
                        release.url = std::string(url_field.value().get_string().value());
                    }

                    auto created_at_field = obj.find_field_unordered("created_at");
                    if (!created_at_field.error()) {
                        release.created_at =
                            std::string(created_at_field.value().get_string().value());
                    }

                    results.push_back(release);
                }
            }
        }
    }

    return results;
}

// Benzinga - Analyst Ratings
std::vector<BenzingaAnalystRating> RESTClient::list_benzinga_analyst_ratings(
    const std::optional<std::string> &ticker,
    const std::optional<std::vector<std::string>> &tickers_any_of, std::optional<int> limit,
    const std::optional<std::string> &published_utc_gte,
    const std::optional<std::string> &published_utc_lte, const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (tickers_any_of.has_value() && !tickers_any_of->empty()) {
        std::string tickers_str;
        for (size_t i = 0; i < tickers_any_of->size(); ++i) {
            if (i > 0)
                tickers_str += ",";
            tickers_str += (*tickers_any_of)[i];
        }
        params["tickers.any_of"] = tickers_str;
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (published_utc_gte.has_value()) {
        params["published_utc.gte"] = published_utc_gte.value();
    }
    if (published_utc_lte.has_value()) {
        params["published_utc.lte"] = published_utc_lte.value();
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/v1/ratings";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaAnalystRating> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaAnalystRating rating;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        rating.id = std::string(id_field.value().get_string().value());
                    }

                    auto analyst_name_field = obj.find_field_unordered("analyst_name");
                    if (!analyst_name_field.error()) {
                        rating.analyst_name =
                            std::string(analyst_name_field.value().get_string().value());
                    }

                    auto analyst_firm_field = obj.find_field_unordered("analyst_firm");
                    if (!analyst_firm_field.error()) {
                        rating.analyst_firm =
                            std::string(analyst_firm_field.value().get_string().value());
                    }

                    auto rating_current_field = obj.find_field_unordered("rating_current");
                    if (!rating_current_field.error()) {
                        rating.rating_current =
                            std::string(rating_current_field.value().get_string().value());
                    }

                    auto price_target_field = obj.find_field_unordered("price_target");
                    if (!price_target_field.error()) {
                        rating.price_target = price_target_field.value().get_double().value();
                    }

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        rating.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    results.push_back(rating);
                }
            }
        }
    }

    return results;
}

// Benzinga - Analyst Insights
std::vector<BenzingaAnalystInsight> RESTClient::list_benzinga_analyst_insights(
    const std::optional<std::string> &ticker, const std::optional<std::string> &date,
    const std::optional<std::string> &firm, std::optional<int> limit,
    const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (firm.has_value()) {
        params["firm"] = firm.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/benzinga/v1/analyst-insights";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaAnalystInsight> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaAnalystInsight insight;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        insight.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        insight.date = std::string(date_field.value().get_string().value());
                    }

                    auto firm_field = obj.find_field_unordered("firm");
                    if (!firm_field.error()) {
                        insight.firm = std::string(firm_field.value().get_string().value());
                    }

                    auto rating_field = obj.find_field_unordered("rating");
                    if (!rating_field.error()) {
                        insight.rating = std::string(rating_field.value().get_string().value());
                    }

                    auto price_target_field = obj.find_field_unordered("price_target");
                    if (!price_target_field.error()) {
                        insight.price_target = price_target_field.value().get_double().value();
                    }

                    auto insight_field = obj.find_field_unordered("insight");
                    if (!insight_field.error()) {
                        insight.insight = std::string(insight_field.value().get_string().value());
                    }

                    results.push_back(insight);
                }
            }
        }
    }

    return results;
}

// Benzinga - Analysts
std::vector<BenzingaAnalyst> RESTClient::list_benzinga_analysts(
    const std::optional<std::string> &benzinga_firm_id, const std::optional<std::string> &firm_name,
    const std::optional<std::string> &full_name, std::optional<int> limit,
    const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;
    if (benzinga_firm_id.has_value()) {
        params["benzinga_firm_id"] = benzinga_firm_id.value();
    }
    if (firm_name.has_value()) {
        params["firm_name"] = firm_name.value();
    }
    if (full_name.has_value()) {
        params["full_name"] = full_name.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/benzinga/v1/analysts";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaAnalyst> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaAnalyst analyst;

                    auto benzinga_id_field = obj.find_field_unordered("benzinga_id");
                    if (!benzinga_id_field.error()) {
                        analyst.benzinga_id =
                            std::string(benzinga_id_field.value().get_string().value());
                    }

                    auto firm_name_field = obj.find_field_unordered("firm_name");
                    if (!firm_name_field.error()) {
                        analyst.firm_name =
                            std::string(firm_name_field.value().get_string().value());
                    }

                    auto full_name_field = obj.find_field_unordered("full_name");
                    if (!full_name_field.error()) {
                        analyst.full_name =
                            std::string(full_name_field.value().get_string().value());
                    }

                    auto smart_score_field = obj.find_field_unordered("smart_score");
                    if (!smart_score_field.error()) {
                        analyst.smart_score = smart_score_field.value().get_double().value();
                    }

                    auto total_ratings_field = obj.find_field_unordered("total_ratings");
                    if (!total_ratings_field.error()) {
                        analyst.total_ratings = total_ratings_field.value().get_double().value();
                    }

                    results.push_back(analyst);
                }
            }
        }
    }

    return results;
}

// Benzinga - Consensus Ratings
std::vector<BenzingaConsensusRating> RESTClient::list_benzinga_consensus_ratings(
    const std::string &ticker, const std::optional<std::string> &date,
    const std::optional<std::string> &date_gte, const std::optional<std::string> &date_lte,
    std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/benzinga/v1/consensus-ratings/" + ticker;
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaConsensusRating> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaConsensusRating rating;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        rating.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto consensus_rating_field = obj.find_field_unordered("consensus_rating");
                    if (!consensus_rating_field.error()) {
                        rating.consensus_rating =
                            std::string(consensus_rating_field.value().get_string().value());
                    }

                    auto consensus_price_target_field =
                        obj.find_field_unordered("consensus_price_target");
                    if (!consensus_price_target_field.error()) {
                        rating.consensus_price_target =
                            consensus_price_target_field.value().get_double().value();
                    }

                    auto buy_ratings_field = obj.find_field_unordered("buy_ratings");
                    if (!buy_ratings_field.error()) {
                        rating.buy_ratings = buy_ratings_field.value().get_int64().value();
                    }

                    auto hold_ratings_field = obj.find_field_unordered("hold_ratings");
                    if (!hold_ratings_field.error()) {
                        rating.hold_ratings = hold_ratings_field.value().get_int64().value();
                    }

                    auto sell_ratings_field = obj.find_field_unordered("sell_ratings");
                    if (!sell_ratings_field.error()) {
                        rating.sell_ratings = sell_ratings_field.value().get_int64().value();
                    }

                    results.push_back(rating);
                }
            }
        }
    }

    return results;
}

// Benzinga - Earnings
std::vector<BenzingaEarning> RESTClient::list_benzinga_earnings(
    const std::optional<std::string> &ticker, const std::optional<std::string> &date,
    const std::optional<std::string> &date_gte, const std::optional<std::string> &date_lte,
    std::optional<int> limit, const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/benzinga/v1/earnings";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaEarning> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaEarning earning;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        earning.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        earning.date = std::string(date_field.value().get_string().value());
                    }

                    auto actual_eps_field = obj.find_field_unordered("actual_eps");
                    if (!actual_eps_field.error()) {
                        earning.actual_eps = actual_eps_field.value().get_double().value();
                    }

                    auto estimated_eps_field = obj.find_field_unordered("estimated_eps");
                    if (!estimated_eps_field.error()) {
                        earning.estimated_eps = estimated_eps_field.value().get_double().value();
                    }

                    auto actual_revenue_field = obj.find_field_unordered("actual_revenue");
                    if (!actual_revenue_field.error()) {
                        earning.actual_revenue = actual_revenue_field.value().get_double().value();
                    }

                    auto estimated_revenue_field = obj.find_field_unordered("estimated_revenue");
                    if (!estimated_revenue_field.error()) {
                        earning.estimated_revenue =
                            estimated_revenue_field.value().get_double().value();
                    }

                    results.push_back(earning);
                }
            }
        }
    }

    return results;
}

// Benzinga - Firms
std::vector<BenzingaFirm>
RESTClient::list_benzinga_firms(const std::optional<std::string> &benzinga_id,
                                const std::optional<std::string> &name, std::optional<int> limit,
                                const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;
    if (benzinga_id.has_value()) {
        params["benzinga_id"] = benzinga_id.value();
    }
    if (name.has_value()) {
        params["name"] = name.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/benzinga/v1/firms";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaFirm> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaFirm firm;

                    auto benzinga_id_field = obj.find_field_unordered("benzinga_id");
                    if (!benzinga_id_field.error()) {
                        firm.benzinga_id =
                            std::string(benzinga_id_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        firm.name = std::string(name_field.value().get_string().value());
                    }

                    auto currency_field = obj.find_field_unordered("currency");
                    if (!currency_field.error()) {
                        firm.currency = std::string(currency_field.value().get_string().value());
                    }

                    results.push_back(firm);
                }
            }
        }
    }

    return results;
}

// Benzinga - Guidance
std::vector<BenzingaGuidance> RESTClient::list_benzinga_guidance(
    const std::optional<std::string> &ticker, const std::optional<std::string> &date,
    const std::optional<std::string> &date_gte, const std::optional<std::string> &date_lte,
    std::optional<int> limit, const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/benzinga/v1/guidance";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaGuidance> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaGuidance guidance;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        guidance.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        guidance.date = std::string(date_field.value().get_string().value());
                    }

                    auto estimated_eps_guidance_field =
                        obj.find_field_unordered("estimated_eps_guidance");
                    if (!estimated_eps_guidance_field.error()) {
                        guidance.estimated_eps_guidance =
                            estimated_eps_guidance_field.value().get_double().value();
                    }

                    auto estimated_revenue_guidance_field =
                        obj.find_field_unordered("estimated_revenue_guidance");
                    if (!estimated_revenue_guidance_field.error()) {
                        guidance.estimated_revenue_guidance =
                            estimated_revenue_guidance_field.value().get_double().value();
                    }

                    auto min_eps_guidance_field = obj.find_field_unordered("min_eps_guidance");
                    if (!min_eps_guidance_field.error()) {
                        guidance.min_eps_guidance =
                            min_eps_guidance_field.value().get_double().value();
                    }

                    auto max_eps_guidance_field = obj.find_field_unordered("max_eps_guidance");
                    if (!max_eps_guidance_field.error()) {
                        guidance.max_eps_guidance =
                            max_eps_guidance_field.value().get_double().value();
                    }

                    results.push_back(guidance);
                }
            }
        }
    }

    return results;
}

// Benzinga - News V2
std::vector<BenzingaNews> RESTClient::list_benzinga_news_v2(
    const std::optional<std::string> &ticker,
    const std::optional<std::vector<std::string>> &tickers_any_of, std::optional<int> limit,
    const std::optional<std::string> &published_utc_gte,
    const std::optional<std::string> &published_utc_lte, const std::optional<std::string> &sort,
    const std::optional<std::string> &order) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (tickers_any_of.has_value() && !tickers_any_of->empty()) {
        std::string tickers_str;
        for (size_t i = 0; i < tickers_any_of->size(); ++i) {
            if (i > 0)
                tickers_str += ",";
            tickers_str += (*tickers_any_of)[i];
        }
        params["tickers.any_of"] = tickers_str;
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (published_utc_gte.has_value()) {
        params["published_utc.gte"] = published_utc_gte.value();
    }
    if (published_utc_lte.has_value()) {
        params["published_utc.lte"] = published_utc_lte.value();
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }
    if (order.has_value()) {
        params["order"] = order.value();
    }

    std::string path = "/benzinga/v2/news";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaNews> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaNews news;

                    auto id_field = obj.find_field_unordered("id");
                    if (!id_field.error()) {
                        news.id = std::string(id_field.value().get_string().value());
                    }

                    auto title_field = obj.find_field_unordered("title");
                    if (!title_field.error()) {
                        news.title = std::string(title_field.value().get_string().value());
                    }

                    auto author_field = obj.find_field_unordered("author");
                    if (!author_field.error()) {
                        news.author = std::string(author_field.value().get_string().value());
                    }

                    auto created_at_field = obj.find_field_unordered("created_at");
                    if (!created_at_field.error()) {
                        news.created_at =
                            std::string(created_at_field.value().get_string().value());
                    }

                    auto url_field = obj.find_field_unordered("url");
                    if (!url_field.error()) {
                        news.url = std::string(url_field.value().get_string().value());
                    }

                    auto teaser_field = obj.find_field_unordered("teaser");
                    if (!teaser_field.error()) {
                        news.teaser = std::string(teaser_field.value().get_string().value());
                    }

                    results.push_back(news);
                }
            }
        }
    }

    return results;
}

// Benzinga - Ratings (different from analyst_ratings)
std::vector<BenzingaRating> RESTClient::list_benzinga_ratings(
    const std::optional<std::string> &ticker, const std::optional<std::string> &date,
    const std::optional<std::string> &date_gte, const std::optional<std::string> &date_lte,
    const std::optional<std::string> &firm, std::optional<int> limit,
    const std::optional<std::string> &sort) {
    std::map<std::string, std::string> params;
    if (ticker.has_value()) {
        params["ticker"] = ticker.value();
    }
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (date_gte.has_value()) {
        params["date.gte"] = date_gte.value();
    }
    if (date_lte.has_value()) {
        params["date.lte"] = date_lte.value();
    }
    if (firm.has_value()) {
        params["firm"] = firm.value();
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }
    if (sort.has_value()) {
        params["sort"] = sort.value();
    }

    std::string path = "/benzinga/v1/ratings";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto& doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<BenzingaRating> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    BenzingaRating rating;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        rating.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto date_field = obj.find_field_unordered("date");
                    if (!date_field.error()) {
                        rating.date = std::string(date_field.value().get_string().value());
                    }

                    auto firm_field = obj.find_field_unordered("firm");
                    if (!firm_field.error()) {
                        rating.firm = std::string(firm_field.value().get_string().value());
                    }

                    auto analyst_field = obj.find_field_unordered("analyst");
                    if (!analyst_field.error()) {
                        rating.analyst = std::string(analyst_field.value().get_string().value());
                    }

                    auto rating_field = obj.find_field_unordered("rating");
                    if (!rating_field.error()) {
                        rating.rating = std::string(rating_field.value().get_string().value());
                    }

                    auto price_target_field = obj.find_field_unordered("price_target");
                    if (!price_target_field.error()) {
                        rating.price_target = price_target_field.value().get_double().value();
                    }

                    auto rating_action_field = obj.find_field_unordered("rating_action");
                    if (!rating_action_field.error()) {
                        rating.rating_action =
                            std::string(rating_action_field.value().get_string().value());
                    }

                    auto importance_field = obj.find_field_unordered("importance");
                    if (!importance_field.error()) {
                        rating.importance = importance_field.value().get_int64().value();
                    }

                    results.push_back(rating);
                }
            }
        }
    }

    return results;
}

} // namespace massive::rest
