#!/bin/bash
# Quick start script - get up and running in minutes

set -e

echo "🚀 massive-cpp Quick Start"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

# Check prerequisites
echo "📋 Checking prerequisites..."

MISSING_DEPS=()

command -v cmake >/dev/null 2>&1 || MISSING_DEPS+=("cmake")
command -v git >/dev/null 2>&1 || MISSING_DEPS+=("git")
command -v make >/dev/null 2>&1 || MISSING_DEPS+=("make")

if [ ${#MISSING_DEPS[@]} -gt 0 ]; then
    echo "❌ Missing required dependencies: ${MISSING_DEPS[*]}"
    echo ""
    echo "Install them with:"
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "  sudo apt-get install ${MISSING_DEPS[*]}"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "  brew install ${MISSING_DEPS[*]}"
    fi
    exit 1
fi

echo "✅ All prerequisites met"
echo ""

# Verify repository
echo "🔍 Verifying repository..."
if [ -f "scripts/verify.sh" ]; then
    ./scripts/verify.sh || echo "⚠️  Some verification checks failed, but continuing..."
else
    echo "⚠️  verify.sh not found, skipping verification"
fi
echo ""

# Create build directory
echo "📁 Setting up build directory..."
mkdir -p build
echo "✅ Build directory ready"
echo ""

# Configure CMake
echo "⚙️  Configuring CMake..."
if cmake -B build -DMASSIVE_BUILD_TESTS=ON 2>&1 | tee build/config.log; then
    echo "✅ CMake configuration successful"
else
    echo "❌ CMake configuration failed"
    echo "Check build/config.log for details"
    exit 1
fi
echo ""

# Build
echo "🔨 Building massive-cpp..."
if cmake --build build -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) 2>&1 | tee build/build.log; then
    echo "✅ Build successful"
else
    echo "❌ Build failed"
    echo "Check build/build.log for details"
    exit 1
fi
echo ""

# Run tests
echo "🧪 Running tests..."
if [ -f "build/CTestTestfile.cmake" ]; then
    if cd build && ctest --output-on-failure 2>&1 | tee ../build/test.log; then
        echo "✅ All tests passed"
    else
        echo "⚠️  Some tests failed (check build/test.log)"
    fi
    cd ..
else
    echo "⚠️  No tests found (this is normal if tests aren't built)"
fi
echo ""

# Summary
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "✅ Quick start complete!"
echo ""
echo "Next steps:"
echo "  1. Set your API key: export MASSIVE_API_KEY=your_key"
echo "  2. Try an example: ./build/examples/massive_example_get_aggs"
echo "  3. Read the docs: cat README.md"
echo ""
echo "For more information, see README.md"

