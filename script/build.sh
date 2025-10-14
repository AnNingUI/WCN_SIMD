#!/bin/bash
# WCN_SIMD Build Script for Linux/macOS/MSYS2

set -e

BUILD_DIR="build"
BUILD_TYPE="Release"
USE_NINJA=1
ENABLE_NATIVE=ON

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

show_usage() {
    cat << EOF
WCN_SIMD Build Script

Usage: $0 [OPTIONS]

Options:
    clean               Clean build directory before building
    debug               Build in Debug mode (default: Release)
    no-ninja            Use Unix Makefiles instead of Ninja
    no-native           Disable native CPU optimizations
    help                Show this help message

Examples:
    $0                  # Build with default options (Release + Ninja)
    $0 clean debug      # Clean build in Debug mode
    $0 no-ninja         # Build with Make instead of Ninja

EOF
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        clean)
            print_info "Cleaning build directory..."
            rm -rf "$BUILD_DIR"
            shift
            ;;
        debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        no-ninja)
            USE_NINJA=0
            shift
            ;;
        no-native)
            ENABLE_NATIVE=OFF
            shift
            ;;
        help)
            show_usage
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

echo "================================================"
echo "WCN_SIMD Build Script"
echo "================================================"
echo ""

# Check required tools
print_info "Checking required tools..."
if ! command -v cmake &> /dev/null; then
    print_error "CMake not found. Please install CMake."
    exit 1
fi

if [ $USE_NINJA -eq 1 ]; then
    if ! command -v ninja &> /dev/null; then
        print_warning "Ninja not found. Falling back to Make."
        USE_NINJA=0
    fi
fi

# Determine generator
if [ $USE_NINJA -eq 1 ]; then
    GENERATOR="Ninja"
else
    GENERATOR="Unix Makefiles"
fi

echo "Build configuration:"
echo "  Generator: $GENERATOR"
echo "  Build Type: $BUILD_TYPE"
echo "  Native Optimization: $ENABLE_NATIVE"
echo ""

# Create build directory
if [ ! -d "$BUILD_DIR" ]; then
    print_info "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# Configure with CMake
print_info "Configuring project with CMake..."
cd "$BUILD_DIR"
cmake -G "$GENERATOR" \
      -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
      -DWCN_SIMD_ENABLE_NATIVE="$ENABLE_NATIVE" \
      ..

# Build
echo ""
print_info "Building project..."
if [ "$GENERATOR" = "Ninja" ]; then
    ninja
else
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
fi

cd ..

# Copy compile_commands.json for clangd/IDE support
if [ -f "$BUILD_DIR/compile_commands.json" ]; then
    cp -f "$BUILD_DIR/compile_commands.json" "compile_commands.json"
    print_info "Copied compile_commands.json for IDE support"
fi

echo ""
echo "================================================"
echo "Build completed successfully!"
echo "================================================"
echo ""
echo "Executable location:"
echo "  $BUILD_DIR/bin/wcn_simd_example"
echo ""
echo "Run example:"
echo "  $BUILD_DIR/bin/wcn_simd_example"
echo ""

# Show detected SIMD features
if [ -f "$BUILD_DIR/bin/wcn_simd_example" ]; then
    echo "Detected SIMD implementation:"
    "$BUILD_DIR/bin/wcn_simd_example" | grep "Implementation:" | head -1
fi
