# Performance Guide

This document provides performance benchmarks and optimization tips for `massive-cpp`.

## Benchmarks

Run benchmarks with:

```bash
cmake -B build -DMASSIVE_BUILD_BENCHMARKS=ON
cmake --build build
./build/benchmarks/benchmark_json_parsing
```

## Performance Tips

### 1. JSON Parsing

`massive-cpp` uses `simdjson` for high-performance JSON parsing. The library is optimized for:
- SIMD instructions (SSE, AVX, NEON)
- Zero-copy parsing where possible
- Efficient memory allocation

### 2. HTTP Transport

The HTTP transport uses Boost.Beast which provides:
- Efficient buffer management
- Zero-copy operations where possible
- Connection pooling (when implemented)

### 3. Memory Management

- Use `std::optional` for nullable values to avoid unnecessary allocations
- Prefer `std::string_view` for read-only string parameters
- Use move semantics for large objects

### 4. Concurrency

- The WebSocket client uses a separate thread for message processing
- REST API calls are synchronous but can be wrapped in async operations
- Consider using thread pools for batch operations

## Optimization Checklist

- [ ] Enable compiler optimizations (`-O3` or `Release` build)
- [ ] Use connection pooling for multiple requests
- [ ] Batch API calls when possible
- [ ] Cache frequently accessed data
- [ ] Profile with tools like `perf` or `valgrind`

## Known Performance Characteristics

- JSON parsing: ~2-5 GB/s (depending on CPU)
- HTTP request overhead: ~10-50ms (network dependent)
- WebSocket message processing: <1ms per message

