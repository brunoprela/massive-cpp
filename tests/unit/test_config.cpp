#include "massive/core/config.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ClientConfig creation", "[config]") {
    SECTION("Create with API key") {
        auto config = massive::core::ClientConfig::WithApiKey("test_key");
        REQUIRE(config.api_key() == "test_key");
    }

    SECTION("Set API key") {
        massive::core::ClientConfig config;
        config.set_api_key("test_key");
        REQUIRE(config.api_key() == "test_key");
    }

    SECTION("Set retry policy") {
        massive::core::ClientConfig config;
        massive::core::RetryPolicy policy;
        policy.max_attempts = 5;
        policy.initial_backoff = std::chrono::milliseconds(100);
        config.set_retry_policy(policy);

        auto retrieved = config.retry_policy();
        REQUIRE(retrieved.max_attempts == 5);
        REQUIRE(retrieved.initial_backoff == std::chrono::milliseconds(100));
    }
}
