#include "massive/rest/client.hpp"

#include "massive/core/http/beast_transport.hpp"
#include "massive/core/logging.hpp"
#include "massive/exceptions.hpp"
#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <cmath>

namespace massive::rest {

namespace {
void ensure_success(int status, std::string_view context, const std::string &body) {
    if (status >= 200 && status < 300) {
        return;
    }
    std::ostringstream oss;
    oss << context << " failed with status " << status;
    if (!body.empty()) {
        oss << ": " << body;
    }
    throw BadResponse(status, body);
}

std::string url_encode(const std::string &value) {
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;

    for (char c : value) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
        }
    }
    return encoded.str();
}

std::string method_to_string(core::HttpMethod method) {
    switch (method) {
        case core::HttpMethod::Get: return "GET";
        case core::HttpMethod::Post: return "POST";
        case core::HttpMethod::Put: return "PUT";
        case core::HttpMethod::Delete: return "DELETE";
        case core::HttpMethod::Patch: return "PATCH";
        default: return "UNKNOWN";
    }
}

// Check if a status code should be retried
bool should_retry(int status_code) {
    // Retry on server errors (5xx) and rate limiting (429)
    return status_code >= 500 || status_code == 429;
}

// Calculate exponential backoff delay
std::chrono::milliseconds calculate_backoff(
    std::size_t attempt,
    std::chrono::milliseconds initial_backoff,
    std::chrono::milliseconds max_backoff) {
    // Exponential backoff: initial_backoff * 2^(attempt-1)
    auto delay_ms = static_cast<std::int64_t>(
        initial_backoff.count() * std::pow(2.0, static_cast<double>(attempt - 1))
    );
    auto delay = std::chrono::milliseconds(delay_ms);
    
    // Cap at max_backoff
    if (delay > max_backoff) {
        delay = max_backoff;
    }
    
    return delay;
}
} // namespace

RESTClient::RESTClient(core::ClientConfig config, std::shared_ptr<core::IHttpTransport> transport)
    : config_(std::move(config)), transport_(std::move(transport)),
      json_codec_(core::make_passthrough_json_codec()) {
    if (config_.api_key().empty()) {
        throw std::runtime_error("API key is required");
    }
}

std::map<std::string, std::string> RESTClient::build_headers(const std::optional<RequestOptions>& options) const {
    std::map<std::string, std::string> headers;
    
    if (!options.has_value() || !options->skip_default_headers) {
        headers["Authorization"] = "Bearer " + std::string(config_.api_key());
        headers["Accept-Encoding"] = "gzip";
        headers["User-Agent"] = "Massive.com C++Client/0.1.0";
        headers["Content-Type"] = "application/json";
    }
    
    // Merge custom headers (override defaults if present)
    if (options.has_value()) {
        for (const auto& [key, value] : options->headers) {
            headers[key] = value;
        }
    }
    
    return headers;
}

std::string RESTClient::build_url(const std::string &path,
                                  const std::map<std::string, std::string> &params,
                                  const std::optional<RequestOptions>& options) const {
    std::string base_url = std::string(config_.base_url());
    std::string full_url = base_url + path;

    // Merge params with query params from options
    std::map<std::string, std::string> all_params = params;
    if (options.has_value()) {
        for (const auto& [key, value] : options->query_params) {
            all_params[key] = value;
        }
    }

    if (!all_params.empty()) {
        full_url += "?";
        bool first = true;
        for (const auto &[key, value] : all_params) {
            if (!first) {
                full_url += "&";
            }
            first = false;
            full_url += key + "=" + url_encode(value);
        }
    }

    return full_url;
}

core::HttpResponse RESTClient::send_request(core::HttpMethod method, const std::string &path,
                                            const std::map<std::string, std::string> &params,
                                            const std::optional<RequestOptions>& options) {
    auto logger = config_.logger();
    std::string full_url = build_url(path, params, options);
    
    MASSIVE_LOG_DEBUG(logger, "Sending " << method_to_string(method) << " request to " << full_url);
    
    if (config_.trace()) {
        // Merge params for logging
        std::map<std::string, std::string> all_params = params;
        if (options.has_value()) {
            for (const auto& [key, value] : options->query_params) {
                all_params[key] = value;
            }
        }
        
        std::ostringstream params_oss;
        bool first = true;
        for (const auto& [key, value] : all_params) {
            if (!first) params_oss << ", ";
            params_oss << key << "=" << value;
            first = false;
        }
        if (!all_params.empty()) {
            MASSIVE_LOG_DEBUG(logger, "Request parameters: " << params_oss.str());
        }
    }
    
    core::HttpRequest request;
    request.method = method;
    request.url = full_url;
    request.headers = build_headers(options);
    
    // Set timeout from options
    if (options.has_value() && options->timeout.has_value()) {
        request.timeout = options->timeout;
    }
    
    // Set body from options if present
    if (options.has_value() && options->body.has_value()) {
        request.body = options->body.value();
    }

    // Retry logic with exponential backoff
    const auto& retry_policy = config_.retry_policy();
    core::HttpResponse response;
    std::size_t attempt = 0;
    bool success = false;
    
    while (attempt < retry_policy.max_attempts && !success) {
        attempt++;
        
        auto start_time = std::chrono::steady_clock::now();
        try {
            response = transport_->send(request);
            auto end_time = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            
            MASSIVE_LOG_DEBUG(logger, "Received response: " << response.status_code 
                          << " (" << duration.count() << "ms) [Attempt " << attempt << "/" << retry_policy.max_attempts << "]");
            
            // Check if we should retry
            if (should_retry(response.status_code) && attempt < retry_policy.max_attempts) {
                auto backoff = calculate_backoff(attempt, retry_policy.initial_backoff, retry_policy.max_backoff);
                MASSIVE_LOG_WARN(logger, "Request failed with status " << response.status_code 
                              << ", retrying in " << backoff.count() << "ms (attempt " << attempt << "/" << retry_policy.max_attempts << ")");
                std::this_thread::sleep_for(backoff);
                continue;
            }
            
            // Success or non-retryable error
            success = true;
            
            if (config_.trace() && !response.body.empty()) {
                // Log first 500 chars of response body
                std::string body_preview = response.body.substr(0, 500);
                if (response.body.length() > 500) {
                    body_preview += "...";
                }
                MASSIVE_LOG_DEBUG(logger, "Response body: " << body_preview);
            }
        } catch (const std::exception& e) {
            // Network/transport errors - retry if we have attempts left
            if (attempt < retry_policy.max_attempts) {
                auto backoff = calculate_backoff(attempt, retry_policy.initial_backoff, retry_policy.max_backoff);
                MASSIVE_LOG_WARN(logger, "Request failed with exception: " << e.what() 
                              << ", retrying in " << backoff.count() << "ms (attempt " << attempt << "/" << retry_policy.max_attempts << ")");
                std::this_thread::sleep_for(backoff);
                continue;
            } else {
                // Out of retries, rethrow
                throw;
            }
        }
    }
    
    // Check final response status
    ensure_success(response.status_code, "HTTP request", response.body);
    return response;
}

} // namespace massive::rest

