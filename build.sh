#!/bin/bash

# Modern build script for Kalman_1D project.
# Usage: ./build.sh clean
#        ./build.sh build [config.yaml]

set -e

ACTION=$1
CONFIG_SRC=$2
BUILD_DIR="build"
CONFIG_TARGET="$BUILD_DIR/config.yaml"

if [ "$ACTION" == "clean" ]; then
    echo "Cleaning build..."
    rm -rf "$BUILD_DIR"
    exit 0
fi

if [ "$ACTION" == "build" ]; then
    if [ ! -d "$BUILD_DIR" ]; then
        mkdir "$BUILD_DIR"
    fi
    # Si se pasa config, copiar, si no, generar la default
    if [ -n "$CONFIG_SRC" ]; then
        cp "$CONFIG_SRC" "$CONFIG_TARGET"
        echo "Copied config.yaml from $CONFIG_SRC to $CONFIG_TARGET"
    else
        cat > "$CONFIG_TARGET" <<EOF
N: 100
dt: 0.1
velocity: 1.5
noise: 0.3
initial_uncertainty: 1.0
process_var: 0.03
outfile: filtered.csv
loglevel: 1
logfile: kalman.log
log_max_size_mb: 2
log_rotate_count: 3
EOF
        echo "Default config.yaml generated in $CONFIG_TARGET"
    fi

    cd "$BUILD_DIR"
    echo "Configuring project with CMake..."
    cmake ..
    echo "Building project..."
    make -j$(nproc)
    echo "Build finished!"
    exit 0
fi

echo "Usage: ./build.sh clean"
echo "   or: ./build.sh build [config.yaml]"
exit 1
