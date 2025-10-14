# WCN_SIMD - Cross-Platform SIMD Library
cmake_minimum_required(VERSION 3.10)

project(WCN_SIMD
    VERSION 1.0.0
    LANGUAGES C
)

# Generate compile_commands.json for clangd/IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Build options
option(WCN_SIMD_BUILD_EXAMPLES "Build example programs" ON)
option(WCN_SIMD_BUILD_TESTS "Build test suite" OFF)
option(WCN_SIMD_ENABLE_NATIVE "Enable native CPU optimization" ON)
option(BUILD_WASM_MODULE "Build standalone WebAssembly (.wasm) module (no JS glue)" OFF)

# Directories
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Source files
set(SRC_FILES
    ${SRC_DIR}/wcn_simd.c
    ${SRC_DIR}/wcn_atomic.c
)

# Create library
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Platform-specific compiler flags
include(CheckCCompilerFlag)

# Detect and enable SIMD features
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64)|(amd64)|(x86)|(i686)")
    message(STATUS "Detected x86/x86_64 architecture")
    
    if(WCN_SIMD_ENABLE_NATIVE)
        # Enable native CPU features
        if(MSVC)
            target_compile_options(${PROJECT_NAME} PRIVATE /arch:AVX2)
        else()
            check_c_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
            if(COMPILER_SUPPORTS_MARCH_NATIVE)
                target_compile_options(${PROJECT_NAME} PRIVATE -march=native)
            else()
                # Fallback to specific features
                target_compile_options(${PROJECT_NAME} PRIVATE -msse2 -mavx -mavx2 -mfma)
            endif()
        endif()
    else()
        # Default to SSE2 (baseline for x86_64)
        if(MSVC)
            target_compile_options(${PROJECT_NAME} PRIVATE /arch:SSE2)
        else()
            target_compile_options(${PROJECT_NAME} PRIVATE -msse2)
        endif()
    endif()
    
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(aarch64)|(arm64)|(ARM64)")
    message(STATUS "Detected ARM64 architecture")
    
    if(NOT MSVC)
        # ARM NEON is enabled by default on AArch64
        if(WCN_SIMD_ENABLE_NATIVE)
            check_c_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
            if(COMPILER_SUPPORTS_MARCH_NATIVE)
                target_compile_options(${PROJECT_NAME} PRIVATE -march=native)
            endif()
        endif()
    endif()
    
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    message(STATUS "Detected ARM32 architecture")
    
    if(NOT MSVC)
        # Enable NEON for ARM32
        check_c_compiler_flag("-mfpu=neon" COMPILER_SUPPORTS_NEON)
        if(COMPILER_SUPPORTS_NEON)
            target_compile_options(${PROJECT_NAME} PRIVATE -mfpu=neon -mfloat-abi=hard)
        endif()
    endif()
    
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "loongarch")
    message(STATUS "Detected LoongArch architecture")
    
    if(WCN_SIMD_ENABLE_NATIVE)
        target_compile_options(${PROJECT_NAME} PRIVATE -march=native)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -mlsx -mlasx)
    endif()
    
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "riscv")
    message(STATUS "Detected RISC-V architecture")
    
    if(WCN_SIMD_ENABLE_NATIVE)
        target_compile_options(${PROJECT_NAME} PRIVATE -march=native)
    else()
        # RVV support requires specific march
        target_compile_options(${PROJECT_NAME} PRIVATE -march=rv64gcv)
    endif()
    
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "ppc")
    message(STATUS "Detected PowerPC architecture")
    
    target_compile_options(${PROJECT_NAME} PRIVATE -maltivec -mvsx)
    
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "mips")
    message(STATUS "Detected MIPS architecture")
    
    target_compile_options(${PROJECT_NAME} PRIVATE -mmsa)
endif()

# WebAssembly detection
if(EMSCRIPTEN)
    message(STATUS "Detected WebAssembly (Emscripten)")
    # Remove any x86-specific flags for WebAssembly builds
    get_target_property(CURRENT_OPTIONS ${PROJECT_NAME} COMPILE_OPTIONS)
    if(CURRENT_OPTIONS)
        foreach(OPTION ${CURRENT_OPTIONS})
            if(OPTION MATCHES "^-m(avx|sse|fma)" OR OPTION MATCHES "^/arch:")
                message(STATUS "Removing unsupported option for WebAssembly: ${OPTION}")
                list(REMOVE_ITEM CURRENT_OPTIONS ${OPTION})
            endif()
        endforeach()
        set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_OPTIONS "${CURRENT_OPTIONS}")
    endif()
    # Add WebAssembly SIMD support
    target_compile_options(${PROJECT_NAME} PRIVATE -msimd128)
elseif(WASI)
    message(STATUS "Detected WebAssembly (WASI)")
    # Add any WASI-specific compile options here if needed
    # target_compile_options(${PROJECT_NAME} PRIVATE -some-wasi-flag)
endif()

# Standalone Wasm module (no JS glue)
if(EMSCRIPTEN AND BUILD_WASM_MODULE)
    message(STATUS "Building standalone Wasm module target: WCN_SIMD_wasm_module")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/bin/WCN_SIMD.wasm"
        COMMAND ${CMAKE_C_COMPILER} ${SRC_FILES}
                -I "${INCLUDE_DIR}"
                -O3
                -sSTANDALONE_WASM=1
                --no-entry
                -sEXPORT_ALL=1
                -msimd128
                -o "${CMAKE_BINARY_DIR}/bin/WCN_SIMD.wasm"
        DEPENDS ${SRC_FILES}
        COMMENT "Linking standalone Wasm module WCN_SIMD.wasm"
        VERBATIM
    )
    add_custom_target(WCN_SIMD_wasm_module
        DEPENDS "${CMAKE_BINARY_DIR}/bin/WCN_SIMD.wasm"
    )
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif(WASI AND CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # Skip GCC-style warnings when using WASI with MSVC
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Build examples
if(WCN_SIMD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation rules
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers preserving directory structure
install(DIRECTORY ${INCLUDE_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Export configuration
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Create package config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Print configuration summary
message(STATUS "")
message(STATUS "=== WCN_SIMD Configuration Summary ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "Build examples: ${WCN_SIMD_BUILD_EXAMPLES}")
message(STATUS "Build tests: ${WCN_SIMD_BUILD_TESTS}")
message(STATUS "Enable native optimizations: ${WCN_SIMD_ENABLE_NATIVE}")
message(STATUS "Build standalone Wasm module: ${BUILD_WASM_MODULE}")
message(STATUS "")
