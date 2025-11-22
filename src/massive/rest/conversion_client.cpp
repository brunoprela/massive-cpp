#include "massive/rest/client.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>
#include <sstream>

namespace massive::rest {

// Conversion - Forex
ForexConversion RESTClient::get_forex_conversion(
    const std::string& from,
    const std::string& to,
    double amount,
    const std::optional<std::string>& date,
    const std::optional<std::string>& precision) {
    std::map<std::string, std::string> params;
    params["from"] = from;
    params["to"] = to;
    params["amount"] = std::to_string(amount);
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (precision.has_value()) {
        params["precision"] = precision.value();
    }

    std::string path = "/v1/conversion/" + from + "/" + to;
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

    ForexConversion conversion;
    auto obj = root_obj.value();

    auto from_field = obj.find_field_unordered("from");
    if (!from_field.error()) {
        conversion.from = std::string(from_field.value().get_string().value());
    }

    auto to_field = obj.find_field_unordered("to");
    if (!to_field.error()) {
        conversion.to = std::string(to_field.value().get_string().value());
    }

    auto initial_amount_field = obj.find_field_unordered("initialAmount");
    if (!initial_amount_field.error()) {
        conversion.initial_amount = initial_amount_field.value().get_double().value();
    }

    auto converted_field = obj.find_field_unordered("converted");
    if (!converted_field.error()) {
        conversion.converted = converted_field.value().get_double().value();
    }

    auto timestamp_field = obj.find_field_unordered("timestamp");
    if (!timestamp_field.error()) {
        conversion.timestamp = timestamp_field.value().get_int64().value();
    }

    return conversion;
}

// Conversion - Crypto
CryptoConversion RESTClient::get_crypto_conversion(
    const std::string& from,
    const std::string& to,
    double amount,
    const std::optional<std::string>& date,
    const std::optional<std::string>& precision) {
    std::map<std::string, std::string> params;
    params["from"] = from;
    params["to"] = to;
    params["amount"] = std::to_string(amount);
    if (date.has_value()) {
        params["date"] = date.value();
    }
    if (precision.has_value()) {
        params["precision"] = precision.value();
    }

    std::string path = "/v1/conversion/crypto/" + from + "/" + to;
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

    CryptoConversion conversion;
    auto obj = root_obj.value();

    auto from_field = obj.find_field_unordered("from");
    if (!from_field.error()) {
        conversion.from = std::string(from_field.value().get_string().value());
    }

    auto to_field = obj.find_field_unordered("to");
    if (!to_field.error()) {
        conversion.to = std::string(to_field.value().get_string().value());
    }

    auto initial_amount_field = obj.find_field_unordered("initialAmount");
    if (!initial_amount_field.error()) {
        conversion.initial_amount = initial_amount_field.value().get_double().value();
    }

    auto converted_field = obj.find_field_unordered("converted");
    if (!converted_field.error()) {
        conversion.converted = converted_field.value().get_double().value();
    }

    auto timestamp_field = obj.find_field_unordered("timestamp");
    if (!timestamp_field.error()) {
        conversion.timestamp = timestamp_field.value().get_int64().value();
    }

    return conversion;
}

} // namespace massive::rest

