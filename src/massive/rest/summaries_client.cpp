#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

// Summaries
std::vector<SummaryResult>
RESTClient::get_summaries(const std::vector<std::string> &ticker_any_of) {
    std::map<std::string, std::string> params;
    if (!ticker_any_of.empty()) {
        std::string tickers_str;
        for (size_t i = 0; i < ticker_any_of.size(); ++i) {
            if (i > 0)
                tickers_str += ",";
            tickers_str += ticker_any_of[i];
        }
        params["ticker.any_of"] = tickers_str;
    }

    std::string path = "/v1/summaries";
    auto response = send_request(core::HttpMethod::Get, path, params);

    ::simdjson::ondemand::parser parser;
    ::simdjson::padded_string json = response.body;
    auto doc_result = parser.iterate(json);
    if (doc_result.error()) {
        throw std::runtime_error("Failed to parse JSON response");
    }
    auto doc = doc_result.value();
    auto root_obj = doc.get_object();
    if (root_obj.error()) {
        throw std::runtime_error("Response is not a JSON object");
    }

    std::vector<SummaryResult> results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto results_array = results_field.value().get_array();
        if (!results_array.error()) {
            for (auto result_elem : results_array.value()) {
                auto obj_result = result_elem.get_object();
                if (!obj_result.error()) {
                    auto obj = obj_result.value();
                    SummaryResult summary;

                    auto ticker_field = obj.find_field_unordered("ticker");
                    if (!ticker_field.error()) {
                        summary.ticker = std::string(ticker_field.value().get_string().value());
                    }

                    auto name_field = obj.find_field_unordered("name");
                    if (!name_field.error()) {
                        summary.name = std::string(name_field.value().get_string().value());
                    }

                    auto price_field = obj.find_field_unordered("price");
                    if (!price_field.error()) {
                        summary.price = price_field.value().get_double().value();
                    }

                    auto type_field = obj.find_field_unordered("type");
                    if (!type_field.error()) {
                        summary.type = std::string(type_field.value().get_string().value());
                    }

                    auto market_status_field = obj.find_field_unordered("market_status");
                    if (!market_status_field.error()) {
                        summary.market_status =
                            std::string(market_status_field.value().get_string().value());
                    }

                    auto last_updated_field = obj.find_field_unordered("last_updated");
                    if (!last_updated_field.error()) {
                        summary.last_updated = last_updated_field.value().get_int64().value();
                    }

                    results.push_back(summary);
                }
            }
        }
    }

    return results;
}

} // namespace massive::rest
