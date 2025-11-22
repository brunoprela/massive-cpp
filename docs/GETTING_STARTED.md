# Getting Started with massive-cpp

This guide will help you get started with `massive-cpp` in minutes.

## Prerequisites

Before you begin, ensure you have:

- **CMake 3.20+** - [Download](https://cmake.org/download/)
- **C++20 Compiler** - GCC 10+, Clang 12+, or MSVC 2019+
- **Git** - For cloning the repository
- **OpenSSL** - For TLS/SSL support (usually pre-installed)

## Quick Start (Recommended)

The fastest way to get started:

```bash
# Clone the repository
git clone https://github.com/your-org/massive-cpp.git
cd massive-cpp

# Run the quick start script
./scripts/quick-start.sh
```

This script will:
1. ✅ Check prerequisites
2. ✅ Verify repository setup
3. ✅ Configure CMake
4. ✅ Build the library
5. ✅ Run tests

## Manual Setup

If you prefer manual setup:

### 1. Clone and Build

```bash
git clone https://github.com/your-org/massive-cpp.git
cd massive-cpp
mkdir build && cd build
cmake .. -DMASSIVE_BUILD_TESTS=ON
cmake --build . -j$(nproc)
```

### 2. Verify Installation

```bash
# From the repository root
./scripts/verify.sh
```

### 3. Run Tests

```bash
cd build
ctest --output-on-failure
```

## Your First Program

Create a file `hello_massive.cpp`:

```cpp
#include "massive/rest/client.hpp"
#include "massive/core/config.hpp"
#include "massive/core/http/beast_transport.hpp"
#include <iostream>

int main() {
    // Set your API key
    auto config = massive::core::ClientConfig::WithApiKey("YOUR_API_KEY");
    auto transport = std::make_shared<massive::core::BeastHttpTransport>();
    massive::rest::RESTClient client(config, transport);
    
    try {
        // Get aggregates for AAPL
        auto aggs = client.list_aggs("AAPL", 1, "day", "2024-01-01", "2024-01-31");
        
        std::cout << "Found " << aggs.size() << " aggregate bars\n";
        for (const auto& agg : aggs) {
            if (agg.close) {
                std::cout << "Close: $" << *agg.close << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
```

Build and run:

```bash
# Build
g++ -std=c++20 hello_massive.cpp -I./include -L./build -lmassive_rest -lmassive_core -o hello_massive

# Or with CMake
cmake -B build
cmake --build build
./build/hello_massive
```

## Using in Your Project

### CMake FetchContent (Recommended)

```cmake
include(FetchContent)

FetchContent_Declare(
    massive-cpp
    GIT_REPOSITORY https://github.com/your-org/massive-cpp.git
    GIT_TAG v0.1.0
)

FetchContent_MakeAvailable(massive-cpp)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE massive::rest)
```

### System Installation

```bash
# Install
cmake --install build --prefix /usr/local

# Use in your CMakeLists.txt
find_package(massive-cpp REQUIRED)
target_link_libraries(my_app PRIVATE massive::rest)
```

## Configuration

### Environment Variables

Create a `.env` file:

```
MASSIVE_API_KEY=your_api_key_here
```

Or export:

```bash
export MASSIVE_API_KEY=your_api_key_here
```

### Programmatic Configuration

```cpp
auto config = massive::core::ClientConfig::FromEnv();  // Loads from .env
// or
auto config = massive::core::ClientConfig::WithApiKey("your_key");
```

## Next Steps

- 📖 Read the [README](../README.md) for full documentation
- 🔍 Check [API Examples](../examples/) for more code samples
- 🏗️ Review [Architecture Guide](ARCHITECTURE.md) for design details
- ⚡ See [Performance Guide](PERFORMANCE.md) for optimization tips
- 🐛 Check [Troubleshooting Guide](TROUBLESHOOTING.md) if you encounter issues

## Need Help?

- 📚 [Documentation](../README.md)
- 🐛 [Report Issues](https://github.com/your-org/massive-cpp/issues)
- 💬 [Discussions](https://github.com/your-org/massive-cpp/discussions)
- 📧 [Contact Maintainers](mailto:maintainers@your-org.com)

