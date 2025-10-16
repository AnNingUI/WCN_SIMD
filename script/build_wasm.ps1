# WCN_SIMD Build Script for WebAssembly using Emscripten

$BUILD_DIR = "build_wasm_emcc"
$BUILD_TYPE = "Release"
$BUILD_TARGET = "WCN_SIMD"
$BUILD_ALL = 0
$EXTRA_CMAKE_FLAGS = ""

Write-Host "================================================"
Write-Host "WCN_SIMD WebAssembly Build Script"
Write-Host "================================================"
Write-Host ""

# Parse command line arguments
foreach ($arg in $args) {
    if ($arg -eq "clean") {
        Write-Host "Cleaning build directory..."
        if (Test-Path $BUILD_DIR) {
            Remove-Item -Path $BUILD_DIR -Recurse -Force
        }
    }
    elseif ($arg -eq "debug") {
        $BUILD_TYPE = "Debug"
    }
    elseif ($arg -eq "examples") {
        # Build all targets including examples
        $BUILD_ALL = 1
    }
    elseif ($arg -eq "wasmlib" -or $arg -eq "wasmmodule") {
        # Build standalone Wasm module (no JS glue)
        $BUILD_TARGET = "WCN_SIMD_wasm_module"
        $BUILD_ALL = 0
        $EXTRA_CMAKE_FLAGS = "-DBUILD_WASM_MODULE=ON -DCMAKE_C_FLAGS='-sEXPORT_ALL=1"
    }
}

# Check required tools
Write-Host "Checking required tools..."

# Check if Emscripten is available
$emccExists = $null -ne (Get-Command emcc -ErrorAction SilentlyContinue)
if (-not $emccExists) {
    Write-Host "ERROR: Emscripten compiler (emcc) not found." -ForegroundColor Red
    Write-Host "Please install Emscripten and ensure it's in your PATH." -ForegroundColor Yellow
    Write-Host "Visit https://emscripten.org/docs/getting_started/downloads.html for installation instructions." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "After installing Emscripten, make sure to activate the environment:" -ForegroundColor Yellow
    Write-Host "  1. Navigate to your Emscripten SDK directory" -ForegroundColor Cyan
    Write-Host "  2. Run: .\emsdk install latest" -ForegroundColor Cyan
    Write-Host "  3. Run: .\emsdk activate latest" -ForegroundColor Cyan
    Write-Host "  4. Run: .\emsdk_env.bat" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Then try running this script again." -ForegroundColor Yellow
    exit 1
}

# Check if CMake is available
$cmakeExists = $null -ne (Get-Command cmake -ErrorAction SilentlyContinue)
if (-not $cmakeExists) {
    Write-Host "ERROR: CMake required. Please install CMake."
    exit 1
}

Write-Host "Build configuration:"
Write-Host "  Build Type: $BUILD_TYPE"
Write-Host "  Emscripten: "
$emccVersion = emcc --version
foreach ($line in $emccVersion) {
    Write-Host "  $line"
}
if ($BUILD_ALL -eq 1) {
    Write-Host "  Target: all (includes examples)"
}
else {
    Write-Host "  Target: $BUILD_TARGET (examples skipped)"
}
if ($EXTRA_CMAKE_FLAGS -ne "") {
    Write-Host "  Extra CMake flags: $EXTRA_CMAKE_FLAGS"
}
Write-Host ""

# Create build directory
if (Test-Path $BUILD_DIR) {
    Write-Host "Using existing build directory: $BUILD_DIR"
}
else {
    Write-Host "Creating build directory: $BUILD_DIR"
    New-Item -Path $BUILD_DIR -ItemType Directory | Out-Null
}

# Configure with CMake using Emscripten
Write-Host "Configuring project with CMake for WebAssembly..."
Push-Location $BUILD_DIR

# Use Emscripten's CMake toolchain file
$configResult = emcmake cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE $EXTRA_CMAKE_FLAGS ..
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: CMake configuration failed"
    Pop-Location
    exit 1
}

# Build
Write-Host ""
Write-Host "Building project for WebAssembly..."
if ($BUILD_ALL -eq 1) {
    $buildResult = cmake --build .
}
else {
    $buildResult = cmake --build . --target $BUILD_TARGET
}
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Build failed"
    Pop-Location
    exit 1
}

Pop-Location

Write-Host ""
Write-Host "================================================"
Write-Host "WebAssembly build completed successfully!"
Write-Host "================================================"
Write-Host ""
Write-Host "Output files can be found in the $BUILD_DIR directory."
Write-Host ""