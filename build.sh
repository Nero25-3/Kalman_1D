#!/bin/bash

# Simple build script for Kalman_1D project.
# Usage: ./build.sh [clean]
# If 'clean' argument is provided, cleans build directory before building.

set -e

if [ "$1" == "clean" ]; then
    echo "Cleaning build..."
    rm -rf build
fi

if [ ! -d build ]; then
    mkdir build
fi

cd build
echo "Configuring project with CMake..."
cmake ..
echo "Building project..."
make -j$(nproc)
echo "Build finished!"
