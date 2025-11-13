#!/bin/bash

# Script to auto-format and check style for C++ sources in Kalman_1D project

set -e

# Set your source directories
SRC_DIR=src
TEST_DIR=tests

# --- clang-format: auto-format all headers and cpp sources ---
echo "Running clang-format..."
clang-format -i $SRC_DIR/*.cpp $SRC_DIR/*.hpp $TEST_DIR/*.cpp

# --- clang-tidy: static analysis on all implementation files ---
echo "Running clang-tidy..."
for file in $SRC_DIR/*.cpp $TEST_DIR/*.cpp; do
    echo "Analyzing $file"
    clang-tidy $file -- -I$SRC_DIR || true
done

echo "Formatting and static analysis finished!"
