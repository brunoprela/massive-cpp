# Examples

This directory contains example code demonstrating how to use `massive-cpp`.

## Available Examples

### REST API Examples

- **get_aggs.cpp** - Fetch aggregate bars (OHLCV) for a ticker
- **get_snapshot.cpp** - Get snapshot data for tickers

### WebSocket Examples

- **websocket_example.cpp** - Real-time market data streaming

## Building Examples

Examples are built automatically when building the project:

```bash
cmake -B build
cmake --build build
```

Executables will be in `build/examples/` or `build/Release/examples/` (Windows).

## Running Examples

1. Set your API key in a `.env` file:
   ```
   MASSIVE_API_KEY=your_api_key_here
   ```

2. Run an example:
   ```bash
   ./build/examples/massive_example_get_aggs
   ```

## More Examples

For more comprehensive examples, see the [massive-python examples](https://github.com/massive-com/client-python/tree/main/examples) which demonstrate similar patterns.

