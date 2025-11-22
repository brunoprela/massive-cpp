#include "massive/core/logging.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

namespace massive::core {

ConsoleLogger::ConsoleLogger(LogLevel min_level)
    : min_level_(min_level) {}

void ConsoleLogger::log(LogLevel level, const std::string& message) {
    if (level < min_level_) {
        return;
    }

    std::string level_str = level_to_string(level);
    std::string ts = timestamp();
    
    std::cerr << "[" << ts << "] [" << level_str << "] " << message << std::endl;
}

void ConsoleLogger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void ConsoleLogger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void ConsoleLogger::warn(const std::string& message) {
    log(LogLevel::WARN, message);
}

void ConsoleLogger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void ConsoleLogger::set_level(LogLevel level) {
    min_level_ = level;
}

LogLevel ConsoleLogger::level() const {
    return min_level_;
}

std::string ConsoleLogger::level_to_string(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::NONE:  return "NONE ";
        default: return "UNKNOWN";
    }
}

std::string ConsoleLogger::timestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &time_t);
#else
    localtime_r(&time_t, &tm_buf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

std::shared_ptr<ILogger> create_logger(LogLevel level) {
    return std::make_shared<ConsoleLogger>(level);
}

std::shared_ptr<ILogger> create_null_logger() {
    return std::make_shared<NullLogger>();
}

} // namespace massive::core

