#include "massive/rest/pagination.hpp"
#include <simdjson/ondemand.h>
#include <stdexcept>

namespace massive::rest {

PaginationInfo extract_pagination_info(simdjson::ondemand::object& root_obj) {
    PaginationInfo info;

    // Try to extract next_url
    auto next_url_field = root_obj.find_field_unordered("next_url");
    if (!next_url_field.error()) {
        auto next_url_str = next_url_field.value().get_string();
        if (!next_url_str.error()) {
            info.next_url = std::string(next_url_str.value());
        }
    }

    // Try to extract next_page_token (alternative pagination method)
    auto next_token_field = root_obj.find_field_unordered("next_page_token");
    if (!next_token_field.error()) {
        auto next_token_str = next_token_field.value().get_string();
        if (!next_token_str.error()) {
            info.next_page_token = std::string(next_token_str.value());
        }
    }

    // Try to extract count
    auto count_field = root_obj.find_field_unordered("count");
    if (!count_field.error()) {
        auto count_val = count_field.value().get_int64();
        if (!count_val.error()) {
            info.count = static_cast<int>(count_val.value());
        }
    }

    // Try to extract total_count
    auto total_count_field = root_obj.find_field_unordered("totalCount");
    if (!total_count_field.error()) {
        auto total_count_val = total_count_field.value().get_int64();
        if (!total_count_val.error()) {
            info.total_count = static_cast<int>(total_count_val.value());
        }
    }

    // Try to extract status
    auto status_field = root_obj.find_field_unordered("status");
    if (!status_field.error()) {
        auto status_str = status_field.value().get_string();
        if (!status_str.error()) {
            info.status = std::string(status_str.value());
        }
    }

    // Try to extract request_id
    auto request_id_field = root_obj.find_field_unordered("request_id");
    if (!request_id_field.error()) {
        auto request_id_str = request_id_field.value().get_string();
        if (!request_id_str.error()) {
            info.request_id = std::string(request_id_str.value());
        }
    }

    return info;
}

bool has_pagination(simdjson::ondemand::object& root_obj) {
    // Check for common pagination fields
    auto next_url_field = root_obj.find_field_unordered("next_url");
    if (!next_url_field.error()) {
        return true;
    }

    auto next_token_field = root_obj.find_field_unordered("next_page_token");
    if (!next_token_field.error()) {
        return true;
    }

    return false;
}

} // namespace massive::rest

