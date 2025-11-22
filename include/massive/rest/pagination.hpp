#pragma once

#include <string>
#include <optional>
#include <vector>
#include <functional>
#include <memory>
#include <stdexcept>

namespace massive::rest {

// Forward declaration
namespace simdjson {
namespace ondemand {
class object;
}
}

// Pagination metadata from API responses
struct PaginationInfo {
    std::optional<std::string> next_url;
    std::optional<std::string> next_page_token;
    std::optional<int> count;
    std::optional<int> total_count;
    std::optional<std::string> status;
    std::optional<std::string> request_id;
};

// Generic paginated iterator for API responses
template <typename T>
class PaginatedIterator {
public:
    using value_type = T;
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    // Function type for fetching next page
    using FetchFunction = std::function<std::pair<std::vector<T>, PaginationInfo>(const std::optional<std::string>&)>;

    // End iterator
    class EndIterator {};

    // Constructors
    PaginatedIterator() : is_end_(true) {}
    
    PaginatedIterator(FetchFunction fetch_fn, const std::optional<std::string>& initial_url = std::nullopt)
        : fetch_fn_(std::move(fetch_fn))
        , current_url_(initial_url)
        , is_end_(false)
        , current_index_(0) {
        load_next_page();
    }

    // Copy constructor
    PaginatedIterator(const PaginatedIterator& other)
        : fetch_fn_(other.fetch_fn_)
        , current_url_(other.current_url_)
        , is_end_(other.is_end_)
        , current_index_(other.current_index_)
        , current_page_(other.current_page_)
        , pagination_info_(other.pagination_info_) {}

    // Assignment operator
    PaginatedIterator& operator=(const PaginatedIterator& other) {
        if (this != &other) {
            fetch_fn_ = other.fetch_fn_;
            current_url_ = other.current_url_;
            is_end_ = other.is_end_;
            current_index_ = other.current_index_;
            current_page_ = other.current_page_;
            pagination_info_ = other.pagination_info_;
        }
        return *this;
    }

    // Dereference operator
    reference operator*() const {
        if (is_end_ || current_index_ >= current_page_.size()) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return current_page_[current_index_];
    }

    // Arrow operator
    pointer operator->() const {
        return &(**this);
    }

    // Pre-increment
    PaginatedIterator& operator++() {
        if (is_end_) {
            return *this;
        }

        ++current_index_;

        // If we've exhausted the current page, try to load the next page
        if (current_index_ >= current_page_.size()) {
            if (has_next_page()) {
                load_next_page();
            } else {
                is_end_ = true;
            }
        }

        return *this;
    }

    // Post-increment
    PaginatedIterator operator++(int) {
        PaginatedIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // Equality comparison
    bool operator==(const PaginatedIterator& other) const {
        if (is_end_ && other.is_end_) {
            return true;
        }
        if (is_end_ != other.is_end_) {
            return false;
        }
        return current_index_ == other.current_index_ &&
               current_url_ == other.current_url_;
    }

    bool operator==(const EndIterator&) const {
        return is_end_;
    }

    bool operator!=(const PaginatedIterator& other) const {
        return !(*this == other);
    }

    bool operator!=(const EndIterator&) const {
        return !is_end_;
    }

    // Check if there are more pages
    bool has_next_page() const {
        if (is_end_) {
            return false;
        }
        return pagination_info_.next_url.has_value() ||
               pagination_info_.next_page_token.has_value();
    }

    // Get pagination info
    const PaginationInfo& pagination_info() const {
        return pagination_info_;
    }

private:
    void load_next_page() {
        if (is_end_) {
            return;
        }

        try {
            auto [items, info] = fetch_fn_(current_url_);
            current_page_ = std::move(items);
            pagination_info_ = info;
            current_index_ = 0;

            // Update URL for next page
            if (info.next_url.has_value()) {
                current_url_ = info.next_url;
            } else if (info.next_page_token.has_value()) {
                // For token-based pagination, we'd need to modify the fetch function
                // For now, we'll use the token as the URL
                current_url_ = info.next_page_token;
            } else {
                current_url_ = std::nullopt;
            }

            // If the page is empty and there's no next page, mark as end
            if (current_page_.empty() && !has_next_page()) {
                is_end_ = true;
            }
        } catch (...) {
            is_end_ = true;
            throw;
        }
    }

    FetchFunction fetch_fn_;
    std::optional<std::string> current_url_;
    bool is_end_;
    size_t current_index_;
    std::vector<T> current_page_;
    PaginationInfo pagination_info_;
};

// Helper function to extract pagination info from JSON response
// Note: Implementation in .cpp file to avoid including simdjson headers
PaginationInfo extract_pagination_info(simdjson::ondemand::object& root_obj);

// Helper function to check if response has pagination
bool has_pagination(simdjson::ondemand::object& root_obj);

} // namespace massive::rest

