#!/bin/bash
# WCN_SIMD Build Script for WebAssembly using Emscripten

BUILD_DIR=build_wasm_emcc
BUILD_TYPE=Release
BUILD_TARGET=WCN_SIMD
BUILD_ALL=0
EXTRA_CMAKE_FLAGS=""

echo "================================================"
echo "WCN_SIMD WebAssembly Build Script"
echo "================================================"
echo ""

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        clean)
            echo "Cleaning build directory..."
            rm -rf "$BUILD_DIR"
            shift
            ;;
        debug)
            BUILD_TYPE=Debug
            shift
            ;;
        examples)
            # Build all targets including examples
            BUILD_ALL=1
            shift
            ;;
        wasmlib)
            # Build standalone Wasm module (no JS glue)
            BUILD_TARGET=WCN_SIMD_wasm_module
            BUILD_ALL=0
            EXTRA_CMAKE_FLAGS="-DBUILD_WASM_MODULE=ON"
            shift
            ;;
        wasmmodule)
            # Alias of wasmlib
            BUILD_TARGET=WCN_SIMD_wasm_module
            BUILD_ALL=0
            EXTRA_CMAKE_FLAGS="-DBUILD_WASM_MODULE=ON"
            shift
            ;;
        *)
            shift
            ;;
    esac
done

# Check required tools
echo "Checking required tools..."

# Check if Emscripten is available
if ! command -v emcc &> /dev/null; then
    echo "ERROR: Emscripten compiler (emcc) not found."
    echo "Please install Emscripten and ensure it's in your PATH."
    echo "Visit https://emscripten.org/docs/getting_started/downloads.html for installation instructions."
    exit 1
fi

# Check if CMake is available
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake not found. Please install CMake."
    exit 1
fi

echo "Build configuration:"
echo "  Build Type: $BUILD_TYPE"
echo "  Emscripten: $(emcc --version | head -n 1)"
if [ $BUILD_ALL -eq 1 ]; then
    echo "  Target: all (includes examples)"
else
    echo "  Target: $BUILD_TARGET (examples skipped)"
fi
if [ -n "$EXTRA_CMAKE_FLAGS" ]; then
    echo "  Extra CMake flags: $EXTRA_CMAKE_FLAGS"
fi
echo ""

# Create build directory
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# Configure with CMake using Emscripten
echo "Configuring project with CMake for WebAssembly..."
cd "$BUILD_DIR"

# Use Emscripten's CMake toolchain file
emcmake cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE $EXTRA_CMAKE_FLAGS ..

if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed"
    cd ..
    exit 1
fi

# Build
echo ""
echo "Building project for WebAssembly..."
if [ $BUILD_ALL -eq 1 ]; then
    cmake --build .
else
    cmake --build . --target "$BUILD_TARGET"
fi

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed"
    cd ..
    exit 1
fi

cd ..

echo ""
echo "================================================"
echo "WebAssembly build completed successfully!"
echo "================================================"
echo ""
echo "Output files can be found in the $BUILD_DIR directory."
echo ""