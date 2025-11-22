#pragma once

#include <stdexcept>
#include <string>

namespace massive {

class AuthError : public std::runtime_error {
public:
    explicit AuthError(const std::string& message)
        : std::runtime_error("AuthError: " + message) {}
};

class BadResponse : public std::runtime_error {
public:
    explicit BadResponse(const std::string& message)
        : std::runtime_error("BadResponse: " + message) {}
    
    BadResponse(int status_code, const std::string& body)
        : std::runtime_error("BadResponse: HTTP " + std::to_string(status_code) + " - " + body)
        , status_code_(status_code)
        , body_(body) {}
    
    [[nodiscard]] int status_code() const noexcept { return status_code_; }
    [[nodiscard]] const std::string& body() const noexcept { return body_; }

private:
    int status_code_{0};
    std::string body_;
};

}  // namespace massive

