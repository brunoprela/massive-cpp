#include "massive/core/json.hpp"

namespace massive::core {

std::shared_ptr<JsonCodec> make_passthrough_json_codec() {
    return std::make_shared<PassthroughJsonCodec>();
}

}  // namespace massive::core

