#pragma once

#include "massive/core/logging.hpp"
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace massive::core {

struct RetryPolicy {
    std::size_t max_attempts{3};
    std::chrono::milliseconds initial_backoff{200};
    std::chrono::milliseconds max_backoff{1500};
};

class ClientConfig {
public:
    ClientConfig() = default;

    static ClientConfig WithApiKey(std::string api_key);
    static ClientConfig FromEnv();

    ClientConfig &set_api_key(std::string api_key);
    ClientConfig &set_base_url(std::string base_url);
    ClientConfig &set_retry_policy(RetryPolicy policy);
    ClientConfig &set_pagination(bool enabled);
    ClientConfig &set_verbose(bool enabled);
    ClientConfig &set_trace(bool enabled);
    ClientConfig &set_logger(std::shared_ptr<ILogger> logger);

    [[nodiscard]] std::string_view api_key() const noexcept;
    [[nodiscard]] std::string_view base_url() const noexcept;
    [[nodiscard]] const RetryPolicy &retry_policy() const noexcept;
    [[nodiscard]] bool pagination() const noexcept;
    [[nodiscard]] bool verbose() const noexcept;
    [[nodiscard]] bool trace() const noexcept;
    [[nodiscard]] std::shared_ptr<ILogger> logger() const noexcept;

private:
    std::string api_key_;
    std::string base_url_{"https://api.massive.com"};
    RetryPolicy retry_policy_{};
    bool pagination_{true};
    bool verbose_{false};
    bool trace_{false};
    std::shared_ptr<ILogger> logger_;
};

} // namespace massive::core
