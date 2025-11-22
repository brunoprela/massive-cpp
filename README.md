# massive-cpp

Modern C++20 SDK for the Massive (formerly Polygon.io) REST and WebSocket APIs. This library provides a high-performance, type-safe interface to the Massive API with zero-cost abstractions and native C++ performance.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-blue.svg)](https://cmake.org/)

## Installation

### Prerequisites

- CMake 3.20 or higher
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- OpenSSL (for TLS/SSL support)

### Quick Install

#### Method 1: CMake FetchContent (Recommended)

Add to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
    massive-cpp
    GIT_REPOSITORY https://github.com/brunoprela/massive-cpp.git
    GIT_TAG main
)

FetchContent_MakeAvailable(massive-cpp)

target_link_libraries(your_target PRIVATE massive::rest massive::websocket)
```

#### Method 2: System Installation

```bash
git clone https://github.com/brunoprela/massive-cpp.git
cd massive-cpp
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build . -j$(nproc)
sudo cmake --install .
```

Then in your `CMakeLists.txt`:

```cmake
find_package(massive-cpp REQUIRED)
target_link_libraries(your_target PRIVATE massive::rest massive::websocket)
```

#### Method 3: Subdirectory

```cmake
add_subdirectory(path/to/massive-cpp)
target_link_libraries(your_target PRIVATE massive::rest massive::websocket)
```

## Usage

### Basic Example

```cpp
#include "massive/rest/client.hpp"
#include "massive/core/config.hpp"
#include "massive/core/http/beast_transport.hpp"

int main() {
    // Create client
    auto config = massive::core::ClientConfig::WithApiKey("YOUR_API_KEY");
    auto transport = std::make_shared<massive::core::BeastHttpTransport>();
    massive::rest::RESTClient client(config, transport);
    
    // Get aggregates
    auto aggs = client.list_aggs("AAPL", 1, "day", "2024-01-01", "2024-01-31");
    
    for (const auto& agg : aggs) {
        if (agg.close) {
            std::cout << "Close: $" << *agg.close << std::endl;
        }
    }
    
    return 0;
}
```

### WebSocket Example

```cpp
#include "massive/websocket/client.hpp"

massive::websocket::WebSocketClient client("YOUR_API_KEY");

client.connect([](const std::vector<massive::websocket::WebSocketMessage>& messages) {
    for (const auto& msg : messages) {
        // Handle messages
    }
});

client.subscribe({"T.AAPL", "Q.AAPL"});
```

## Features

- ✅ Complete REST API client (92+ methods)
- ✅ WebSocket streaming support
- ✅ 100% feature parity with massive-python
- ✅ High-performance JSON parsing (simdjson)
- ✅ Automatic retry with exponential backoff
- ✅ Structured logging
- ✅ Request options builder
- ✅ Pagination iterators

## API Coverage

All Massive API endpoints are supported:
- Aggregates (Bars)
- Trades & Quotes
- Snapshots
- Reference Data
- Financials
- Benzinga
- Economy
- ETF Global
- Futures
- TMX
- Conversions
- WebSocket Streaming

## Dependencies

Dependencies are automatically fetched by default:
- **Boost** (1.70+): HTTP/WebSocket transport
- **simdjson** (3.0+): JSON parsing
- **OpenSSL**: TLS/SSL support

To use system packages, set `MASSIVE_VENDOR_DEPS=OFF` when configuring CMake.

## Building from Source

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

## CMake Targets

- `massive::core` - Core utilities
- `massive::rest` - REST API client
- `massive::websocket` - WebSocket client

## License

Licensed under the MIT License. See [LICENSE](LICENSE) file for details.

## Related Projects

- [massive-python](https://github.com/massive-com/client-python) - Official Python client
