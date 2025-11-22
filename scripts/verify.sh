#!/bin/bash
# Repository verification script - ensures everything works out of the box

set -e

echo "🔍 Verifying massive-cpp repository setup..."
echo ""

ERRORS=0
WARNINGS=0

# Check for required files
echo "📁 Checking required files..."
REQUIRED_FILES=(
    "CMakeLists.txt"
    "README.md"
    "LICENSE"
    ".gitignore"
    "include/massive/rest/client.hpp"
    "src/massive/rest/client_base.cpp"
)

for file in "${REQUIRED_FILES[@]}"; do
    if [ -f "$file" ]; then
        echo "  ✅ $file"
    else
        echo "  ❌ Missing: $file"
        ((ERRORS++))
    fi
done

# Check for executable scripts
echo ""
echo "🔧 Checking scripts..."
for script in scripts/*.sh; do
    if [ -f "$script" ]; then
        if [ -x "$script" ]; then
            echo "  ✅ $script (executable)"
        else
            echo "  ⚠️  $script (not executable, fixing...)"
            chmod +x "$script"
            ((WARNINGS++))
        fi
    fi
done

# Validate YAML files
echo ""
echo "📋 Validating YAML files..."
if command -v python3 >/dev/null 2>&1; then
    python3 -c "
import yaml
import sys
import glob

errors = 0
for yaml_file in glob.glob('.github/workflows/*.yml') + glob.glob('.github/workflows/*.yaml'):
    try:
        with open(yaml_file, 'r') as f:
            yaml.safe_load(f)
        print(f'  ✅ {yaml_file}')
    except yaml.YAMLError as e:
        print(f'  ❌ {yaml_file}: {e}')
        errors += 1
    except Exception as e:
        print(f'  ⚠️  {yaml_file}: {e}')

sys.exit(errors)
" || ((ERRORS++))
else
    echo "  ⚠️  python3 not found, skipping YAML validation"
    ((WARNINGS++))
fi

# Check CMake configuration
echo ""
echo "⚙️  Checking CMake configuration..."
if [ -f "CMakeLists.txt" ]; then
    if grep -q "cmake_minimum_required" CMakeLists.txt; then
        echo "  ✅ CMakeLists.txt has minimum version"
    else
        echo "  ❌ CMakeLists.txt missing cmake_minimum_required"
        ((ERRORS++))
    fi
    
    if grep -q "project(" CMakeLists.txt; then
        echo "  ✅ CMakeLists.txt has project declaration"
    else
        echo "  ❌ CMakeLists.txt missing project declaration"
        ((ERRORS++))
    fi
fi

# Check for CMake presets
if [ -f "CMakePresets.json" ]; then
    echo "  ✅ CMakePresets.json found"
    if command -v python3 >/dev/null 2>&1; then
        python3 -c "import json; json.load(open('CMakePresets.json'))" 2>/dev/null && echo "  ✅ CMakePresets.json is valid JSON" || echo "  ❌ CMakePresets.json is invalid JSON"
    fi
else
    echo "  ⚠️  CMakePresets.json not found (optional)"
fi

# Check documentation
echo ""
echo "📚 Checking documentation..."
DOC_FILES=(
    "README.md"
    "CONTRIBUTING.md"
    "CODE_OF_CONDUCT.md"
    "CHANGELOG.md"
    ".github/SECURITY.md"
)

for doc in "${DOC_FILES[@]}"; do
    if [ -f "$doc" ]; then
        echo "  ✅ $doc"
    else
        echo "  ⚠️  Missing: $doc (recommended)"
        ((WARNINGS++))
    fi
done

# Check package manager files
echo ""
echo "📦 Checking package manager files..."
if [ -f "vcpkg.json" ]; then
    echo "  ✅ vcpkg.json"
    if command -v python3 >/dev/null 2>&1; then
        python3 -c "import json; json.load(open('vcpkg.json'))" 2>/dev/null && echo "  ✅ vcpkg.json is valid JSON" || echo "  ❌ vcpkg.json is invalid JSON"
    fi
fi

if [ -f "conanfile.py" ]; then
    echo "  ✅ conanfile.py"
fi

# Check Docker files
echo ""
echo "🐳 Checking Docker files..."
if [ -f "Dockerfile" ]; then
    echo "  ✅ Dockerfile"
fi
if [ -f ".dockerignore" ]; then
    echo "  ✅ .dockerignore"
fi

# Check CI workflows
echo ""
echo "🔄 Checking CI workflows..."
WORKFLOW_COUNT=$(find .github/workflows -name "*.yml" -o -name "*.yaml" 2>/dev/null | wc -l | tr -d ' ')
if [ "$WORKFLOW_COUNT" -gt 0 ]; then
    echo "  ✅ Found $WORKFLOW_COUNT workflow(s)"
else
    echo "  ⚠️  No workflows found"
    ((WARNINGS++))
fi

# Summary
echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
if [ $ERRORS -eq 0 ] && [ $WARNINGS -eq 0 ]; then
    echo "✅ Repository verification PASSED"
    echo "   All checks passed successfully!"
    exit 0
elif [ $ERRORS -eq 0 ]; then
    echo "⚠️  Repository verification PASSED with warnings"
    echo "   $WARNINGS warning(s) found (non-critical)"
    exit 0
else
    echo "❌ Repository verification FAILED"
    echo "   $ERRORS error(s) and $WARNINGS warning(s) found"
    exit 1
fi

