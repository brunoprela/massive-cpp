.PHONY: all build test clean install format docs help

CMAKE_BUILD_DIR = build
CMAKE_BUILD_TYPE = Release

all: build

build:
	@echo "Building massive-cpp..."
	@mkdir -p $(CMAKE_BUILD_DIR)
	@cmake -B $(CMAKE_BUILD_DIR) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
	@cmake --build $(CMAKE_BUILD_DIR) -j$$(nproc 2>/dev/null || echo 2)

test: build
	@echo "Running tests..."
	@cd $(CMAKE_BUILD_DIR) && ctest --output-on-failure

clean:
	@echo "Cleaning build directory..."
	@rm -rf $(CMAKE_BUILD_DIR)

install: build
	@echo "Installing massive-cpp..."
	@cmake --install $(CMAKE_BUILD_DIR)

format:
	@echo "Formatting code..."
	@find include src examples tests -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i

docs:
	@echo "Generating documentation..."
	@mkdir -p $(CMAKE_BUILD_DIR)
	@cmake -B $(CMAKE_BUILD_DIR) -DMASSIVE_BUILD_DOCS=ON
	@cmake --build $(CMAKE_BUILD_DIR) --target docs

help:
	@echo "Available targets:"
	@echo "  make build    - Build the project"
	@echo "  make test     - Build and run tests"
	@echo "  make clean    - Remove build directory"
	@echo "  make install  - Install the library"
	@echo "  make format   - Format code with clang-format"
	@echo "  make docs     - Generate API documentation"
	@echo "  make help     - Show this help message"

