#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <memory>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace massive::core {

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    NONE = 4
};

// Logger interface
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(LogLevel level, const std::string& message) = 0;
    virtual void debug(const std::string& message) = 0;
    virtual void info(const std::string& message) = 0;
    virtual void warn(const std::string& message) = 0;
    virtual void error(const std::string& message) = 0;
    virtual void set_level(LogLevel level) = 0;
    virtual LogLevel level() const = 0;
};

// Console logger implementation
class ConsoleLogger : public ILogger {
public:
    explicit ConsoleLogger(LogLevel min_level = LogLevel::INFO);
    void log(LogLevel level, const std::string& message) override;
    void debug(const std::string& message) override;
    void info(const std::string& message) override;
    void warn(const std::string& message) override;
    void error(const std::string& message) override;
    void set_level(LogLevel level) override;
    LogLevel level() const override;

private:
    LogLevel min_level_;
    std::string level_to_string(LogLevel level) const;
    std::string timestamp() const;
};

// Null logger (no-op implementation)
class NullLogger : public ILogger {
public:
    void log(LogLevel /*level*/, const std::string& /*message*/) override {}
    void debug(const std::string& /*message*/) override {}
    void info(const std::string& /*message*/) override {}
    void warn(const std::string& /*message*/) override {}
    void error(const std::string& /*message*/) override {}
    void set_level(LogLevel /*level*/) override {}
    LogLevel level() const override { return LogLevel::NONE; }
};

// Logger factory
std::shared_ptr<ILogger> create_logger(LogLevel level = LogLevel::INFO);
std::shared_ptr<ILogger> create_null_logger();

// Convenience macros for logging
#define MASSIVE_LOG_DEBUG(logger, msg) \
    do { if (logger && logger->level() <= massive::core::LogLevel::DEBUG) { \
        std::ostringstream oss; oss << msg; \
        logger->debug(oss.str()); \
    } } while(0)

#define MASSIVE_LOG_INFO(logger, msg) \
    do { if (logger && logger->level() <= massive::core::LogLevel::INFO) { \
        std::ostringstream oss; oss << msg; \
        logger->info(oss.str()); \
    } } while(0)

#define MASSIVE_LOG_WARN(logger, msg) \
    do { if (logger && logger->level() <= massive::core::LogLevel::WARN) { \
        std::ostringstream oss; oss << msg; \
        logger->warn(oss.str()); \
    } } while(0)

#define MASSIVE_LOG_ERROR(logger, msg) \
    do { if (logger && logger->level() <= massive::core::LogLevel::ERROR) { \
        std::ostringstream oss; oss << msg; \
        logger->error(oss.str()); \
    } } while(0)

} // namespace massive::core

