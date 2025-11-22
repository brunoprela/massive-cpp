#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

// Indicators - SMA
SMAIndicatorResults RESTClient::get_sma(const std::string &ticker,
                                        const std::optional<std::string> &timespan,
                                        std::optional<int> window, std::optional<bool> adjusted,
                                        std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (timespan.has_value()) {
        params["timespan"] = timespan.value();
    }
    if (window.has_value()) {
        params["window"] = std::to_string(window.value());
    }
    if (adjusted.has_value()) {
        params["adjusted"] = adjusted.value() ? "true" : "false";
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/v1/indicators/sma/" + ticker;
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

    SMAIndicatorResults results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto values_field = obj.find_field_unordered("values");
            if (!values_field.error()) {
                auto values_array = values_field.value().get_array();
                if (!values_array.error()) {
                    std::vector<IndicatorValue> values;
                    for (auto val_elem : values_array.value()) {
                        auto val_obj_result = val_elem.get_object();
                        if (!val_obj_result.error()) {
                            auto val_obj = val_obj_result.value();
                            IndicatorValue val;

                            auto timestamp_field = val_obj.find_field_unordered("timestamp");
                            if (!timestamp_field.error()) {
                                val.timestamp = timestamp_field.value().get_int64().value();
                            }

                            auto value_field = val_obj.find_field_unordered("value");
                            if (!value_field.error()) {
                                val.value = value_field.value().get_double().value();
                            }

                            values.push_back(val);
                        }
                    }
                    results.values = values;
                }
            }
        }
    }

    return results;
}

// Indicators - EMA
EMAIndicatorResults RESTClient::get_ema(const std::string &ticker,
                                        const std::optional<std::string> &timespan,
                                        std::optional<int> window, std::optional<bool> adjusted,
                                        std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (timespan.has_value()) {
        params["timespan"] = timespan.value();
    }
    if (window.has_value()) {
        params["window"] = std::to_string(window.value());
    }
    if (adjusted.has_value()) {
        params["adjusted"] = adjusted.value() ? "true" : "false";
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/v1/indicators/ema/" + ticker;
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

    EMAIndicatorResults results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto values_field = obj.find_field_unordered("values");
            if (!values_field.error()) {
                auto values_array = values_field.value().get_array();
                if (!values_array.error()) {
                    std::vector<IndicatorValue> values;
                    for (auto val_elem : values_array.value()) {
                        auto val_obj_result = val_elem.get_object();
                        if (!val_obj_result.error()) {
                            auto val_obj = val_obj_result.value();
                            IndicatorValue val;

                            auto timestamp_field = val_obj.find_field_unordered("timestamp");
                            if (!timestamp_field.error()) {
                                val.timestamp = timestamp_field.value().get_int64().value();
                            }

                            auto value_field = val_obj.find_field_unordered("value");
                            if (!value_field.error()) {
                                val.value = value_field.value().get_double().value();
                            }

                            values.push_back(val);
                        }
                    }
                    results.values = values;
                }
            }
        }
    }

    return results;
}

// Indicators - RSI
RSIIndicatorResults RESTClient::get_rsi(const std::string &ticker,
                                        const std::optional<std::string> &timespan,
                                        std::optional<int> window, std::optional<bool> adjusted,
                                        std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (timespan.has_value()) {
        params["timespan"] = timespan.value();
    }
    if (window.has_value()) {
        params["window"] = std::to_string(window.value());
    }
    if (adjusted.has_value()) {
        params["adjusted"] = adjusted.value() ? "true" : "false";
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/v1/indicators/rsi/" + ticker;
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

    RSIIndicatorResults results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto values_field = obj.find_field_unordered("values");
            if (!values_field.error()) {
                auto values_array = values_field.value().get_array();
                if (!values_array.error()) {
                    std::vector<IndicatorValue> values;
                    for (auto val_elem : values_array.value()) {
                        auto val_obj_result = val_elem.get_object();
                        if (!val_obj_result.error()) {
                            auto val_obj = val_obj_result.value();
                            IndicatorValue val;

                            auto timestamp_field = val_obj.find_field_unordered("timestamp");
                            if (!timestamp_field.error()) {
                                val.timestamp = timestamp_field.value().get_int64().value();
                            }

                            auto value_field = val_obj.find_field_unordered("value");
                            if (!value_field.error()) {
                                val.value = value_field.value().get_double().value();
                            }

                            values.push_back(val);
                        }
                    }
                    results.values = values;
                }
            }
        }
    }

    return results;
}

// Indicators - MACD
MACDIndicatorResults RESTClient::get_macd(const std::string &ticker,
                                          const std::optional<std::string> &timespan,
                                          std::optional<int> short_window,
                                          std::optional<int> long_window,
                                          std::optional<int> signal_window,
                                          std::optional<bool> adjusted, std::optional<int> limit) {
    std::map<std::string, std::string> params;
    if (timespan.has_value()) {
        params["timespan"] = timespan.value();
    }
    if (short_window.has_value()) {
        params["short_window"] = std::to_string(short_window.value());
    }
    if (long_window.has_value()) {
        params["long_window"] = std::to_string(long_window.value());
    }
    if (signal_window.has_value()) {
        params["signal_window"] = std::to_string(signal_window.value());
    }
    if (adjusted.has_value()) {
        params["adjusted"] = adjusted.value() ? "true" : "false";
    }
    if (limit.has_value()) {
        params["limit"] = std::to_string(limit.value());
    }

    std::string path = "/v1/indicators/macd/" + ticker;
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

    MACDIndicatorResults results;
    auto results_field = root_obj.value().find_field_unordered("results");
    if (!results_field.error()) {
        auto obj_result = results_field.value().get_object();
        if (!obj_result.error()) {
            auto obj = obj_result.value();

            auto values_field = obj.find_field_unordered("values");
            if (!values_field.error()) {
                auto values_array = values_field.value().get_array();
                if (!values_array.error()) {
                    std::vector<MACDIndicatorValue> values;
                    for (auto val_elem : values_array.value()) {
                        auto val_obj_result = val_elem.get_object();
                        if (!val_obj_result.error()) {
                            auto val_obj = val_obj_result.value();
                            MACDIndicatorValue val;

                            auto timestamp_field = val_obj.find_field_unordered("timestamp");
                            if (!timestamp_field.error()) {
                                val.timestamp = timestamp_field.value().get_int64().value();
                            }

                            auto value_field = val_obj.find_field_unordered("value");
                            if (!value_field.error()) {
                                val.value = value_field.value().get_double().value();
                            }

                            auto signal_field = val_obj.find_field_unordered("signal");
                            if (!signal_field.error()) {
                                val.signal = signal_field.value().get_double().value();
                            }

                            auto histogram_field = val_obj.find_field_unordered("histogram");
                            if (!histogram_field.error()) {
                                val.histogram = histogram_field.value().get_double().value();
                            }

                            values.push_back(val);
                        }
                    }
                    results.values = values;
                }
            }
        }
    }

    return results;
}

} // namespace massive::rest
