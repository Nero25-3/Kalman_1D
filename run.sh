#!/bin/bash

# Simple run script for Kalman_1D project executables.
# Usage: ./run.sh <target>
# Available targets: demo, tests, benchmark

TARGET=$1

if [ -z "$TARGET" ]; then
    echo "Usage: ./run.sh <target>"
    echo "Targets:"
    echo "  demo       # Run main pipeline demo"
    echo "  tests      # Run GoogleTest test suite"
    echo "  benchmark  # Run performance benchmark"
    exit 1
fi

cd build

case "$TARGET" in
    demo)
        ./kalman1d_demo
        ;;
    tests)
        ./kalman1d_tests
        ;;
    benchmark)
        ./kalman1d_benchmark
        ;;
    *)
        echo "Unknown target: $TARGET"
        exit 2
        ;;
esac
