#!/bin/bash
# Development environment setup script

set -e

echo "🚀 Setting up massive-cpp development environment..."

# Check for required tools
command -v cmake >/dev/null 2>&1 || { echo "❌ cmake is required but not installed. Aborting." >&2; exit 1; }
command -v git >/dev/null 2>&1 || { echo "❌ git is required but not installed. Aborting." >&2; exit 1; }

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "📦 Detected Linux"
    if command -v apt-get >/dev/null 2>&1; then
        echo "Installing dependencies..."
        sudo apt-get update
        sudo apt-get install -y build-essential cmake libssl-dev
    elif command -v yum >/dev/null 2>&1; then
        echo "Installing dependencies..."
        sudo yum install -y gcc-c++ cmake openssl-devel
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "📦 Detected macOS"
    if command -v brew >/dev/null 2>&1; then
        echo "Installing dependencies..."
        brew install cmake openssl
    else
        echo "⚠️  Homebrew not found. Please install dependencies manually."
    fi
fi

# Setup pre-commit hooks
if command -v pre-commit >/dev/null 2>&1; then
    echo "🔧 Installing pre-commit hooks..."
    pre-commit install
else
    echo "⚠️  pre-commit not found. Install with: pip install pre-commit"
fi

# Create build directory
echo "📁 Creating build directory..."
mkdir -p build

# Configure CMake
echo "⚙️  Configuring CMake..."
cmake -B build -DMASSIVE_BUILD_TESTS=ON

echo "✅ Setup complete! Run 'cmake --build build' to build the project."

