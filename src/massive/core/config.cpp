#include "massive/core/config.hpp"
#include "massive/core/dotenv.hpp"
#include "massive/core/logging.hpp"
#include <cstdlib>
#include <stdexcept>

namespace massive::core {

ClientConfig ClientConfig::WithApiKey(std::string api_key) {
    ClientConfig cfg;
    cfg.set_api_key(std::move(api_key));
    return cfg;
}

ClientConfig ClientConfig::FromEnv() {
    load_env_file();
    
    const char* api_key = std::getenv("MASSIVE_API_KEY");
    if (!api_key) {
        throw std::runtime_error("MASSIVE_API_KEY environment variable not set");
    }
    
    ClientConfig cfg;
    cfg.set_api_key(std::string(api_key));
    
    const char* base_url = std::getenv("MASSIVE_BASE_URL");
    if (base_url) {
        cfg.set_base_url(std::string(base_url));
    }
    
    return cfg;
}

ClientConfig& ClientConfig::set_api_key(std::string api_key) {
    api_key_ = std::move(api_key);
    return *this;
}

ClientConfig& ClientConfig::set_base_url(std::string base_url) {
    base_url_ = std::move(base_url);
    return *this;
}

ClientConfig& ClientConfig::set_retry_policy(RetryPolicy policy) {
    retry_policy_ = policy;
    return *this;
}

ClientConfig& ClientConfig::set_pagination(bool enabled) {
    pagination_ = enabled;
    return *this;
}

ClientConfig& ClientConfig::set_verbose(bool enabled) {
    verbose_ = enabled;
    // Auto-configure logger level based on verbose flag
    if (enabled && !logger_ && !trace_) {
        logger_ = create_logger(LogLevel::INFO);
    }
    return *this;
}

ClientConfig& ClientConfig::set_trace(bool enabled) {
    trace_ = enabled;
    // Auto-configure logger level based on trace/verbose flags
    if (enabled && !logger_) {
        logger_ = create_logger(LogLevel::DEBUG);
    } else if (verbose_ && !logger_) {
        logger_ = create_logger(LogLevel::INFO);
    }
    return *this;
}

ClientConfig& ClientConfig::set_logger(std::shared_ptr<ILogger> logger) {
    logger_ = std::move(logger);
    return *this;
}

std::string_view ClientConfig::api_key() const noexcept {
    return api_key_;
}

std::string_view ClientConfig::base_url() const noexcept {
    return base_url_;
}

const RetryPolicy& ClientConfig::retry_policy() const noexcept {
    return retry_policy_;
}

bool ClientConfig::pagination() const noexcept {
    return pagination_;
}

bool ClientConfig::verbose() const noexcept {
    return verbose_;
}

bool ClientConfig::trace() const noexcept {
    return trace_;
}

std::shared_ptr<ILogger> ClientConfig::logger() const noexcept {
    if (logger_) {
        return logger_;
    }
    // Return null logger if not set
    static auto null_logger = create_null_logger();
    return null_logger;
}

}  // namespace massive::core

