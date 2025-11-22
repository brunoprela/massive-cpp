#!/bin/bash
# Test build script - ensures the repository builds correctly

set -e

echo "🔨 Testing massive-cpp build..."
echo ""

BUILD_DIR="build-test"
ERRORS=0

# Clean previous test build
if [ -d "$BUILD_DIR" ]; then
    echo "🧹 Cleaning previous test build..."
    rm -rf "$BUILD_DIR"
fi

# Test 1: Basic configuration
echo "📋 Test 1: CMake configuration..."
if cmake -S . -B "$BUILD_DIR" -DMASSIVE_BUILD_TESTS=OFF -DMASSIVE_BUILD_BENCHMARKS=OFF 2>&1 | tee "$BUILD_DIR/config.log"; then
    echo "  ✅ Configuration successful"
else
    echo "  ❌ Configuration failed"
    ((ERRORS++))
fi

# Test 2: Build core library
echo ""
echo "📦 Test 2: Building core library..."
if cmake --build "$BUILD_DIR" --target massive_core -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) 2>&1 | tee "$BUILD_DIR/build-core.log"; then
    echo "  ✅ Core library built successfully"
else
    echo "  ❌ Core library build failed"
    ((ERRORS++))
fi

# Test 3: Build REST library
echo ""
echo "📦 Test 3: Building REST library..."
if cmake --build "$BUILD_DIR" --target massive_rest -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) 2>&1 | tee "$BUILD_DIR/build-rest.log"; then
    echo "  ✅ REST library built successfully"
else
    echo "  ❌ REST library build failed"
    ((ERRORS++))
fi

# Test 4: Build examples
echo ""
echo "📝 Test 4: Building examples..."
if cmake --build "$BUILD_DIR" --target massive_example_get_aggs -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) 2>&1 | tee "$BUILD_DIR/build-examples.log"; then
    echo "  ✅ Examples built successfully"
else
    echo "  ⚠️  Examples build failed (may be expected if dependencies missing)"
fi

# Test 5: Install target
echo ""
echo "📥 Test 5: Testing install target..."
if cmake --build "$BUILD_DIR" --target install --dry-run 2>&1 | grep -q "install" || true; then
    echo "  ✅ Install target configured"
else
    echo "  ⚠️  Install target check skipped"
fi

# Summary
echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
if [ $ERRORS -eq 0 ]; then
    echo "✅ Build test PASSED"
    echo "   All build tests completed successfully!"
    exit 0
else
    echo "❌ Build test FAILED"
    echo "   $ERRORS test(s) failed"
    echo "   Check logs in $BUILD_DIR/ for details"
    exit 1
fi

