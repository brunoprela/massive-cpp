# Contributing to massive-cpp

Thank you for your interest in contributing to `massive-cpp`! This document provides guidelines and instructions for contributing.

## Code of Conduct

This project adheres to a Code of Conduct that all contributors are expected to follow. Please be respectful and constructive in all interactions.

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/your-username/massive-cpp.git`
3. Create a branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Test your changes: `cmake --build build && ctest --test-dir build`
6. Commit your changes: `git commit -m "Add feature: description"`
7. Push to your fork: `git push origin feature/your-feature-name`
8. Open a Pull Request

## Development Setup

### Prerequisites

- CMake 3.20+
- C++20 compatible compiler
- Git

### Building

```bash
mkdir build && cd build
cmake .. -DMASSIVE_BUILD_TESTS=ON
cmake --build . -j$(nproc)
```

### Running Tests

```bash
cd build
ctest --output-on-failure
```

## Coding Standards

### Style

- Follow the existing code style
- Use `.clang-format` for formatting (run `clang-format -i <file>`)
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and small

### C++ Standards

- Use C++20 features appropriately
- Prefer `const` and `constexpr` where possible
- Use `std::optional` for nullable values
- Use smart pointers (`std::shared_ptr`, `std::unique_ptr`) appropriately
- Avoid raw pointers in public APIs

### Code Organization

- Keep header files in `include/massive/`
- Keep implementation files in `src/massive/`
- Group related functionality together
- Follow the existing directory structure

## Commit Messages

Write clear, descriptive commit messages:

```
Add feature: Support for financials v1 endpoints

- Updated financials client to use /stocks/financials/v1/ endpoints
- Added 170+ filtering parameters
- Updated models to match Python library
```

## Pull Request Process

1. Ensure your code builds and tests pass
2. Update documentation if needed
3. Add tests for new features
4. Update CHANGELOG.md with your changes
5. Ensure code is formatted with clang-format
6. Request review from maintainers

## Testing

- Add unit tests for new features
- Ensure existing tests still pass
- Test on multiple platforms if possible
- Include edge cases and error conditions

## Documentation

- Update README.md if adding new features
- Add code comments for public APIs
- Update examples if API changes
- Keep CHANGELOG.md up to date

## Questions?

Feel free to open an issue for questions or discussions about features.

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

