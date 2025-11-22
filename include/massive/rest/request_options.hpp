#pragma once

#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <string_view>

namespace massive::rest {

// Request options for customizing individual API requests
struct RequestOptions {
    // Custom headers to add/override
    std::map<std::string, std::string> headers;
    
    // Request timeout
    std::optional<std::chrono::milliseconds> timeout;
    
    // Whether to skip default headers
    bool skip_default_headers{false};
    
    // Custom query parameters (merged with method parameters)
    std::map<std::string, std::string> query_params;
    
    // Request body (for POST/PUT/PATCH)
    std::optional<std::string> body;
    
    RequestOptions() = default;
};

// Builder for RequestOptions (fluent interface)
class RequestOptionBuilder {
public:
    RequestOptionBuilder() = default;
    
    // Add a custom header
    RequestOptionBuilder& add_header(const std::string& key, const std::string& value);
    
    // Add multiple headers
    RequestOptionBuilder& add_headers(const std::map<std::string, std::string>& headers);
    
    // Set request timeout
    RequestOptionBuilder& timeout(std::chrono::milliseconds timeout);
    
    // Skip default headers (Authorization, User-Agent, etc.)
    RequestOptionBuilder& skip_default_headers(bool skip = true);
    
    // Add a query parameter
    RequestOptionBuilder& add_query_param(const std::string& key, const std::string& value);
    
    // Add multiple query parameters
    RequestOptionBuilder& add_query_params(const std::map<std::string, std::string>& params);
    
    // Set request body
    RequestOptionBuilder& body(const std::string& body);
    
    // Build the RequestOptions
    RequestOptions build() const;
    
    // Implicit conversion to RequestOptions
    operator RequestOptions() const;

private:
    RequestOptions options_;
};

// Convenience function to create a builder
inline RequestOptionBuilder request_options() {
    return RequestOptionBuilder{};
}

} // namespace massive::rest

