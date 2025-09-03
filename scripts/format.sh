#!/usr/bin/env bash

set -e

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format not found in PATH"
    exit 1
fi

# Get the script directory to find project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "Running clang-format on C/C++ files..."

# Find and format all C/C++ source files
find "$PROJECT_ROOT/src" "$PROJECT_ROOT/include" \
    -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \
    | xargs clang-format "$@"

echo "clang-format completed successfully!"
