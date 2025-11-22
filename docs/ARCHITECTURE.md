# Architecture

This document describes the architecture of `massive-cpp`.

## Overview

`massive-cpp` is organized into three main components:

1. **Core** (`massive::core`) - Foundation utilities
2. **REST** (`massive::rest`) - REST API client
3. **WebSocket** (`massive::websocket`) - WebSocket streaming client

## Core Components

### Configuration (`ClientConfig`)

Manages API credentials, base URLs, and client settings. Supports:
- Environment variable loading
- Retry policies
- Logging configuration

### HTTP Transport (`IHttpTransport`)

Abstract interface for HTTP communication. Implementations:
- `BeastHttpTransport` - Boost.Beast-based HTTPS client

### JSON Parsing

Uses `simdjson` for high-performance JSON parsing with:
- Zero-copy parsing where possible
- SIMD-optimized operations
- On-demand parsing

### Logging

Structured logging with:
- Multiple log levels (DEBUG, INFO, WARN, ERROR)
- Console and null loggers
- Request/response tracing

## REST Client

The REST client is organized by domain:

- `aggs_client.cpp` - Aggregate bars
- `trades_client.cpp` - Trade data
- `quotes_client.cpp` - Quote data
- `snapshot_client.cpp` - Snapshot data
- `reference_client.cpp` - Reference data
- `financials_client.cpp` - Financial statements
- `benzinga_client.cpp` - Benzinga data
- `economy_client.cpp` - Economic indicators
- `etf_client.cpp` - ETF data
- `futures_client.cpp` - Futures data
- `tmx_client.cpp` - TMX market data
- `vx_client.cpp` - VX experimental data
- `conversion_client.cpp` - Currency conversions

## WebSocket Client

Uses PIMPL pattern for implementation hiding:
- Separate thread for message processing
- Automatic reconnection support
- Subscription management

## Design Patterns

- **PIMPL** - Used in WebSocket client for ABI stability
- **Factory Methods** - For creating configurations
- **Builder Pattern** - Request options builder
- **Iterator Pattern** - Pagination iterators

## Dependencies

- **Boost** - HTTP/WebSocket transport, URL parsing
- **simdjson** - JSON parsing
- **OpenSSL** - TLS/SSL support

## Thread Safety

- REST client: Thread-safe for concurrent requests
- WebSocket client: Single connection per instance
- Configuration: Immutable after construction (recommended)

