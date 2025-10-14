# Building WCN_SIMD

Complete guide for building WCN_SIMD on all supported platforms.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Quick Start](#quick-start)
- [Platform-Specific Instructions](#platform-specific-instructions)
- [Build Options](#build-options)
- [Compiler Selection](#compiler-selection)
- [Troubleshooting](#troubleshooting)

## Prerequisites

### Required

- **CMake** 3.10 or newer
- **C Compiler** with C11 support:
  - GCC 7+ / Clang 8+ (Linux, macOS, MSYS2)
  - MSVC 2017+ (Windows)
  - MinGW-w64 (Windows)

### Optional but Recommended

- **Ninja** build system (for faster builds)

## Quick Start

### Using Build Scripts (Easiest)

**Windows:**
```batch
# Build with Ninja (fast)
build.bat

# Clean rebuild
build.bat clean

# Debug build
build.bat clean debug
```

**Linux/macOS:**
```bash
# Make executable (first time only)
chmod +x build.sh

# Build
./build.sh

# Clean rebuild
./build.sh clean
```

### Using CMake Directly

**With Ninja (Recommended):**
```bash
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
```

**With default generator:**
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

## Platform-Specific Instructions

### Windows

#### Option 1: MSYS2 + GCC (Recommended)

```bash
# Install MSYS2 from https://www.msys2.org/

# Update package database
pacman -Syu

# Install build tools
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-ninja

# Build
./build.sh
```

#### Option 2: Visual Studio + MSVC

```batch
# Open "x64 Native Tools Command Prompt for VS 2022"

# Install Ninja (optional but recommended)
# Download from: https://github.com/ninja-build/ninja/releases

# Build
build.bat

# Or use MSBuild
build.bat msbuild
```

#### Option 3: MinGW-w64

```batch
# Install MinGW-w64 from https://www.mingw-w64.org/

# Add to PATH: C:\mingw-w64\bin

# Build
mkdir build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
```

### Linux

#### Debian/Ubuntu

```bash
# Install dependencies
sudo apt update
sudo apt install build-essential cmake ninja-build

# Build
./build.sh
```

#### Fedora

```bash
# Install dependencies
sudo dnf install gcc cmake ninja-build

# Build
./build.sh
```

#### Arch Linux

```bash
# Install dependencies
sudo pacman -S base-devel cmake ninja

# Build
./build.sh
```

### macOS

#### Using Homebrew

```bash
# Install dependencies
brew install cmake ninja

# Build
./build.sh
```

#### Using Xcode Command Line Tools

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Build with Make (Ninja recommended but not required)
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(sysctl -n hw.ncpu)
```

### Cross-Compilation

#### ARM Linux (from x86_64)

```bash
# Install cross-compiler
sudo apt install gcc-aarch64-linux-gnu

# Configure
mkdir build-arm64 && cd build-arm64
cmake -DCMAKE_SYSTEM_NAME=Linux \
      -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
      -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
      -DCMAKE_BUILD_TYPE=Release \
      ..

# Build
ninja
```

#### WebAssembly (with Emscripten)

```bash
# Install Emscripten
# Follow instructions at: https://emscripten.org/docs/getting_started/downloads.html
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Activate Emscripten environment
source ./emsdk_env.sh  # Linux/macOS
# or
emsdk_env.bat  # Windows

# Build using provided script
./script/build_wasm.sh  # Linux/macOS
# or
.\script\build_wasm.ps1  # Windows (PowerShell)

# Or build manually
mkdir build_wasm && cd build_wasm
emcmake cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

**Note:** WebAssembly builds automatically enable SIMD128 support when available in the target environment.

# Complete Emscripten setup
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

# Configure
mkdir build-wasm && cd build-wasm
emcmake cmake -G Ninja \
              -DCMAKE_BUILD_TYPE=Release \
              -DWCN_SIMD_BUILD_EXAMPLES=ON \
              ..

# Build
ninja
```

## Build Options

Configure using `-D` flags with CMake:

```bash
cmake -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DWCN_SIMD_BUILD_EXAMPLES=ON \
      -DWCN_SIMD_BUILD_TESTS=OFF \
      -DWCN_SIMD_ENABLE_NATIVE=ON \
      ..
```

### Available Options

| Option | Default | Description |
|--------|---------|-------------|
| `CMAKE_BUILD_TYPE` | (empty) | Build type: `Release`, `Debug`, `RelWithDebInfo`, `MinSizeRel` |
| `WCN_SIMD_BUILD_EXAMPLES` | `ON` | Build example programs |
| `WCN_SIMD_BUILD_TESTS` | `OFF` | Build test suite |
| `WCN_SIMD_ENABLE_NATIVE` | `ON` | Enable `-march=native` for optimal performance |

### Build Type Comparison

| Type | Optimization | Debug Info | Size | Speed | Use Case |
|------|--------------|------------|------|-------|----------|
| **Release** | `-O3` | No | Small | ⚡ Fastest | Production |
| **Debug** | `-O0` | Yes | Large | 🐌 Slow | Development |
| **RelWithDebInfo** | `-O2` | Yes | Medium | ⚡ Fast | Profiling |
| **MinSizeRel** | `-Os` | No | Smallest | Fast | Embedded |

## Compiler Selection

### Specify Compiler

```bash
# GCC
cmake -DCMAKE_C_COMPILER=gcc ..

# Clang
cmake -DCMAKE_C_COMPILER=clang ..

# Specific version
cmake -DCMAKE_C_COMPILER=gcc-13 ..
```

### Recommended Compilers by Platform

| Platform | Recommended | Alternative |
|----------|-------------|-------------|
| **Linux** | GCC 11+ | Clang 14+ |
| **macOS** | Apple Clang | GCC (via Homebrew) |
| **Windows** | GCC (MSYS2) | MSVC 2019+ |
| **ARM Linux** | GCC 11+ | Clang 14+ |

## Troubleshooting

### Ninja not found

**Solution:**
```bash
# Install Ninja or use default generator
cmake ..  # Without -G Ninja
```

### Compiler not found

**Windows (MSVC):**
```batch
# Use Visual Studio Developer Command Prompt
# or
set PATH=%PATH%;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.xx\bin\Hostx64\x64
```

**Linux:**
```bash
# Install GCC
sudo apt install build-essential  # Debian/Ubuntu
sudo dnf install gcc               # Fedora
```

### CMake version too old

**Solution:**
```bash
# Download newer CMake from https://cmake.org/download/
# Or use pip
pip install cmake --upgrade
```

### SIMD features not detected

**Check compiler flags:**
```bash
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_VERBOSE_MAKEFILE=ON ..
ninja -v  # See actual compile commands
```

**Verify CPU supports SIMD:**
```bash
# Linux
cat /proc/cpuinfo | grep -i "sse\|avx\|neon"

# macOS
sysctl machdep.cpu.features

# Windows (PowerShell)
Get-WmiObject -Class Win32_Processor | Select-Object -Property Name
```

### Build fails with "unrecognized option"

Some architectures may not support all flags. Disable native optimizations:

```bash
cmake -DWCN_SIMD_ENABLE_NATIVE=OFF ..
```

### Link errors

**Missing library:**
```bash
# Make sure to build library before examples
ninja WCN_SIMD        # Build library only
ninja wcn_simd_example  # Build example
```

### Permission denied (build scripts)

**Linux/macOS:**
```bash
chmod +x build.sh
```

## Advanced Build Scenarios

### Parallel Compilation

**Ninja (automatic):**
```bash
ninja  # Uses all CPU cores automatically
```

**Make:**
```bash
make -j$(nproc)  # Linux
make -j$(sysctl -n hw.ncpu)  # macOS
```

**MSBuild:**
```batch
cmake --build . --config Release -- /maxcpucount
```

### Verbose Build Output

```bash
# Ninja
ninja -v

# Make
make VERBOSE=1

# CMake (any generator)
cmake --build . --verbose
```

### Clean Build

```bash
# Using scripts
./build.sh clean
build.bat clean

# Manual
ninja clean
# or
rm -rf build && mkdir build
```

### Install

```bash
# Build
ninja

# Install (may require sudo on Linux/macOS)
ninja install

# Custom install prefix
cmake -DCMAKE_INSTALL_PREFIX=/custom/path ..
ninja install
```

## Performance Tips

1. **Use Ninja**: 2-3x faster than Make/MSBuild
2. **Enable native optimizations**: Add `-DWCN_SIMD_ENABLE_NATIVE=ON`
3. **Use Release build**: Much faster than Debug
4. **Link-Time Optimization (LTO)**:
   ```bash
   cmake -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON ..
   ```

## Verification

After building, verify the library works:

```bash
# Run example program
./build/bin/wcn_simd_example  # Linux/macOS
.\build\bin\wcn_simd_example.exe  # Windows

# Should output detected SIMD features and benchmark results
```

---

For more information, see [README.md](README.md) and [BUILD_COMPARISON.md](BUILD_COMPARISON.md).
