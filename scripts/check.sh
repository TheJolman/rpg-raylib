#!/usr/bin/env bash

if command -v cppcheck &> /dev/null; then
    echo "Running cppcheck..."
    cppcheck --project=build/compile_commands.json "$@"
else
    echo "Error: cppcheck not found in PATH"
    exit 1
fi
