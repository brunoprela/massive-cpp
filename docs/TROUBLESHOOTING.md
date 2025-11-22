# Troubleshooting Guide

Common issues and solutions when using `massive-cpp`.

## Build Issues

### CMake Not Found

**Error:** `cmake: command not found`

**Solution:**
```bash
# Ubuntu/Debian
sudo apt-get install cmake

# macOS
brew install cmake

# Windows
# Download from https://cmake.org/download/
```

### OpenSSL Not Found

**Error:** `Could not find OpenSSL`

**Solution:**
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev

# macOS
brew install openssl
export OPENSSL_ROOT_DIR=$(brew --prefix openssl)

# Windows (vcpkg)
vcpkg install openssl
```

### Boost Not Found

**Error:** `Could not find Boost`

**Solution:**
- Enable automatic dependency fetching (default): `MASSIVE_VENDOR_DEPS=ON`
- Or install system Boost:
  ```bash
  # Ubuntu/Debian
  sudo apt-get install libboost-all-dev
  
  # macOS
  brew install boost
  ```

### simdjson Not Found

**Error:** `Could not find simdjson`

**Solution:**
- Enable automatic dependency fetching (default)
- Or install system simdjson:
  ```bash
  # macOS
  brew install simdjson
  ```

## Compilation Errors

### C++20 Not Supported

**Error:** `error: 'concepts' in namespace 'std' does not name a type`

**Solution:**
- Use a C++20 compatible compiler:
  - GCC 10+
  - Clang 12+
  - MSVC 2019+

### Template Errors

**Error:** Various template-related errors

**Solution:**
- Ensure you're using the latest compiler version
- Check that all headers are included
- Verify CMake is finding the correct compiler

## Runtime Issues

### API Key Not Set

**Error:** `AuthError: API key is required`

**Solution:**
```bash
export MASSIVE_API_KEY=your_api_key_here
```

Or create a `.env` file:
```
MASSIVE_API_KEY=your_api_key_here
```

### Connection Timeout

**Error:** `Connection timeout` or `Network error`

**Solution:**
- Check your internet connection
- Verify firewall settings
- Try increasing timeout in `RequestOptions`

### SSL/TLS Errors

**Error:** `SSL handshake failed`

**Solution:**
- Update OpenSSL to latest version
- Check system certificates
- Verify date/time is correct

## Test Issues

### Tests Not Found

**Error:** `No tests were found`

**Solution:**
- Ensure `MASSIVE_BUILD_TESTS=ON` when configuring
- Check that tests directory exists
- Verify Catch2 is being fetched correctly

### Test Failures

**Error:** Tests fail with assertion errors

**Solution:**
- Check API key is set correctly
- Verify network connectivity
- Review test output for specific errors

## Integration Issues

### CMake find_package Not Working

**Error:** `Could not find package massive-cpp`

**Solution:**
- Install the library first: `cmake --install build`
- Set `CMAKE_PREFIX_PATH` to installation directory
- Verify `massive-cpp-config.cmake` is in the correct location

### FetchContent Issues

**Error:** `FetchContent` fails to download

**Solution:**
- Check internet connection
- Verify Git is installed
- Try clearing CMake cache: `rm -rf build`

## Performance Issues

### Slow JSON Parsing

**Solution:**
- Ensure Release build: `-DCMAKE_BUILD_TYPE=Release`
- Check CPU supports SIMD instructions
- Profile with `perf` or similar tools

### Memory Leaks

**Solution:**
- Build with sanitizers: `-DCMAKE_CXX_FLAGS="-fsanitize=address"`
- Run with Valgrind: `valgrind --leak-check=full ./your_program`
- Check for proper smart pointer usage

## Getting Help

1. Check this troubleshooting guide
2. Review [README.md](../README.md)
3. Check [GitHub Issues](https://github.com/your-org/massive-cpp/issues)
4. Review [CONTRIBUTING.md](../CONTRIBUTING.md) for development help

## Reporting Issues

When reporting issues, please include:
- OS and version
- Compiler and version
- CMake version
- Full error message
- Steps to reproduce
- Minimal code example (if applicable)

