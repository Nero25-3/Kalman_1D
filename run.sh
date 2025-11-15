#!/bin/bash

# Simple run script for Kalman_1D project executables.
# Usage: ./run.sh <target> [config]
# Available targets: demo, tests, benchmark

TARGET=$1
CONFIG=$2

if [ -z "$TARGET" ]; then
    echo "Usage: ./run.sh <target> [config]"
    echo "Targets:"
    echo "  demo         # Run main pipeline demo"
    echo "  tests        # Run GoogleTest test suite"
    echo "  benchmark    # Run performance benchmark"
    exit 1
fi

cd build

case "$TARGET" in
    demo)
        if [ -z "$CONFIG" ]; then
            ./kalman1d_demo
        else
            ./kalman1d_demo "$CONFIG"
        fi
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
