#!/bin/bash
# Code formatting script

set -e

echo "🎨 Formatting code..."

# Find all C++ files
find include src examples tests -name "*.hpp" -o -name "*.cpp" | while read file; do
    echo "Formatting $file"
    clang-format -i "$file"
done

echo "✅ Formatting complete!"

