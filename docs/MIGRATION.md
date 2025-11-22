# Migration Guide

This guide helps you migrate from other Massive API clients to `massive-cpp`.

## From massive-python

### Basic Usage

**Python:**
```python
from massive import RESTClient

client = RESTClient("your_api_key")
aggs = client.list_aggs("AAPL", 1, "day", "2024-01-01", "2024-01-31")
```

**C++:**
```cpp
#include "massive/rest/client.hpp"
#include "massive/core/config.hpp"
#include "massive/core/http/beast_transport.hpp"

auto config = massive::core::ClientConfig::WithApiKey("your_api_key");
auto transport = std::make_shared<massive::core::BeastHttpTransport>();
massive::rest::RESTClient client(config, transport);

auto aggs = client.list_aggs("AAPL", 1, "day", "2024-01-01", "2024-01-31");
```

### Key Differences

1. **Type Safety**: C++ provides compile-time type checking
2. **Memory Management**: Use smart pointers (`std::shared_ptr`, `std::unique_ptr`)
3. **Optional Values**: Use `std::optional<T>` instead of `None`
4. **Error Handling**: Use exceptions (`try/catch`) instead of error codes
5. **Pagination**: Use iterator-based pagination (`list_tickers_iter()`)

### Parameter Mapping

Most parameters map directly:
- Python `limit=None` → C++ `std::optional<int> limit = std::nullopt`
- Python `tickers=["AAPL", "MSFT"]` → C++ `std::vector<std::string> tickers`
- Python `published_utc_gte="2024-01-01"` → C++ `std::optional<std::string> published_utc_gte`

## From Other C++ Clients

### HTTP Transport

`massive-cpp` uses an abstract `IHttpTransport` interface, allowing you to:
- Use the default `BeastHttpTransport`
- Implement your own transport layer
- Mock for testing

### Configuration

Configuration is immutable-friendly:
```cpp
auto config = massive::core::ClientConfig::WithApiKey("key")
    .set_verbose(true)
    .set_retry_policy(policy);
```

## Best Practices

1. **Reuse Clients**: Create one client instance and reuse it
2. **Handle Errors**: Always wrap API calls in try/catch
3. **Use Iterators**: For paginated endpoints, use iterator methods
4. **Configure Logging**: Set up logging for debugging
5. **Set Retry Policy**: Configure retry behavior for your use case

