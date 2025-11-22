#include "massive/rest/request_options.hpp"

namespace massive::rest {

RequestOptionBuilder& RequestOptionBuilder::add_header(const std::string& key, const std::string& value) {
    options_.headers[key] = value;
    return *this;
}

RequestOptionBuilder& RequestOptionBuilder::add_headers(const std::map<std::string, std::string>& headers) {
    for (const auto& [key, value] : headers) {
        options_.headers[key] = value;
    }
    return *this;
}

RequestOptionBuilder& RequestOptionBuilder::timeout(std::chrono::milliseconds timeout) {
    options_.timeout = timeout;
    return *this;
}

RequestOptionBuilder& RequestOptionBuilder::skip_default_headers(bool skip) {
    options_.skip_default_headers = skip;
    return *this;
}

RequestOptionBuilder& RequestOptionBuilder::add_query_param(const std::string& key, const std::string& value) {
    options_.query_params[key] = value;
    return *this;
}

RequestOptionBuilder& RequestOptionBuilder::add_query_params(const std::map<std::string, std::string>& params) {
    for (const auto& [key, value] : params) {
        options_.query_params[key] = value;
    }
    return *this;
}

RequestOptionBuilder& RequestOptionBuilder::body(const std::string& body) {
    options_.body = body;
    return *this;
}

RequestOptions RequestOptionBuilder::build() const {
    return options_;
}

RequestOptionBuilder::operator RequestOptions() const {
    return build();
}

} // namespace massive::rest

