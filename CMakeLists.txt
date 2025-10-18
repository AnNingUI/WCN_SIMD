# 极致优化的跨平台 SIMD 库配置
cmake_minimum_required(VERSION 3.18)  # 提升最低版本以支持现代特性

project(WCN_SIMD
    VERSION 1.0.0
    LANGUAGES C
    DESCRIPTION "High-performance Cross-Platform SIMD Library"
)

# 生成 compile_commands.json 用于 IDE 支持
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 构建选项配置
option(WCN_SIMD_BUILD_EXAMPLES "Build example programs" ON)
option(WCN_SIMD_BUILD_TESTS "Build test suite" OFF)
option(WCN_SIMD_ENABLE_NATIVE "Enable native CPU optimization" ON)
option(WCN_SIMD_ENABLE_LTO "Enable Link Time Optimization" ON)
option(WCN_SIMD_ENABLE_PGO "Enable Profile Guided Optimization" OFF)
option(BUILD_WASM_MODULE "Build standalone WebAssembly module" OFF)

# 如果没有设置构建类型，默认为 Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# 目录结构
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# 源文件列表
set(SRC_FILES
    ${SRC_DIR}/wcn_simd.c
    ${SRC_DIR}/wcn_atomic.c
)

# WebAssembly 特定文件
if(EMSCRIPTEN)
    list(APPEND SRC_FILES ${SRC_DIR}/wcn_simd_wasm_exports.c)
endif()

# 创建库目标
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# 包含目录设置
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# 编译器特性检测
include(CheckCCompilerFlag)
include(CheckCSourceCompiles)

# 全局优化标志配置
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    # Release 模式的激进优化
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /O2 /fp:fast)
        target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O3 -ffast-math)
        target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug 模式配置
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /Od /Zi)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g3)
    endif()
endif()

# 架构特定的极致 SIMD 优化
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|AMD64|amd64|x86|i686)")
    message(STATUS "Detected x86/x86_64 architecture - enabling maximum SIMD optimization")
    
    if(WCN_SIMD_ENABLE_NATIVE)
        # 原生优化 - 针对当前CPU生成最优代码
        if(MSVC)
            check_c_compiler_flag("/arch:AVX512" COMPILER_SUPPORTS_AVX512)
            if(COMPILER_SUPPORTS_AVX512)
                target_compile_options(${PROJECT_NAME} PRIVATE /arch:AVX512)
                message(STATUS "Enabled AVX-512 instruction set for maximum x86 performance")
            else()
                target_compile_options(${PROJECT_NAME} PRIVATE /arch:AVX2)
                message(STATUS "Enabled AVX2 instruction set (AVX-512 not available)")
            endif()
        else()
            check_c_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
            if(COMPILER_SUPPORTS_MARCH_NATIVE)
                target_compile_options(${PROJECT_NAME} PRIVATE -march=native)
                message(STATUS "Enabled native CPU optimization for x86")
            else()
                # 分层级检测现代指令集
                check_c_compiler_flag("-mavx512f -mavx512bw -mavx512dq -mavx512vl" COMPILER_SUPPORTS_AVX512_FULL)
                if(COMPILER_SUPPORTS_AVX512_FULL)
                    target_compile_options(${PROJECT_NAME} PRIVATE -mavx512f -mavx512bw -mavx512dq -mavx512vl)
                    message(STATUS "Enabled full AVX-512 instruction set")
                else()
                    check_c_compiler_flag("-mavx2 -mfma" COMPILER_SUPPORTS_AVX2_FMA)
                    if(COMPILER_SUPPORTS_AVX2_FMA)
                        target_compile_options(${PROJECT_NAME} PRIVATE -mavx2 -mfma)
                        message(STATUS "Enabled AVX2 + FMA instruction set")
                    else()
                        target_compile_options(${PROJECT_NAME} PRIVATE -msse4.2 -mavx)
                        message(STATUS "Enabled SSE4.2 + AVX as fallback")
                    endif()
                endif()
            endif()
        endif()
    else()
        # 非原生模式的基线配置（仍保持高性能）
        if(MSVC)
            target_compile_options(${PROJECT_NAME} PRIVATE /arch:AVX2)
        else()
            target_compile_options(${PROJECT_NAME} PRIVATE -msse4.2 -mavx2 -mfma)
        endif()
    endif()
    
    # x86 特定优化标志
    if(NOT MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE 
            -malign-data=cacheline
            -mstackrealign
        )
    endif()

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(aarch64|arm64|ARM64)")
    message(STATUS "Detected ARM64 architecture - enabling advanced SIMD optimization")
    
    if(NOT MSVC)
        if(WCN_SIMD_ENABLE_NATIVE)
            check_c_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
            if(COMPILER_SUPPORTS_MARCH_NATIVE)
                target_compile_options(${PROJECT_NAME} PRIVATE -march=native)
                message(STATUS "Enabled native CPU optimization for ARM64")
            endif()
        else()
            # 针对常见 ARM 微架构优化
            check_c_compiler_flag("-mcpu=cortex-a78" COMPILER_SUPPORTS_A78)
            if(COMPILER_SUPPORTS_A78)
                target_compile_options(${PROJECT_NAME} PRIVATE -mcpu=cortex-a78)
            endif()
        endif()
        
        # ARM SVE 检测（如果可用）
        check_c_compiler_flag("-march=armv8.2-a+sve" COMPILER_SUPPORTS_SVE)
        if(COMPILER_SUPPORTS_SVE)
            target_compile_options(${PROJECT_NAME} PRIVATE -march=armv8.2-a+sve)
            message(STATUS "Enabled ARM SVE vector extensions")
        endif()
        
        # 确保 NEON 启用
        target_compile_options(${PROJECT_NAME} PRIVATE -mfpu=neon)
    endif()

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    message(STATUS "Detected ARM32 architecture - enabling NEON optimization")
    
    if(NOT MSVC)
        check_c_compiler_flag("-mfpu=neon" COMPILER_SUPPORTS_NEON)
        if(COMPILER_SUPPORTS_NEON)
            target_compile_options(${PROJECT_NAME} PRIVATE -mfpu=neon -mfloat-abi=hard)
            message(STATUS "Enabled NEON SIMD for ARM32")
            
            # ARM32 特定优化
            check_c_compiler_flag("-march=armv7-a" COMPILER_SUPPORTS_ARMV7)
            if(COMPILER_SUPPORTS_ARMV7)
                target_compile_options(${PROJECT_NAME} PRIVATE -march=armv7-a)
            endif()
        endif()
    endif()

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "loongarch")
    message(STATUS "Detected LoongArch architecture - enabling LSX/LASX optimization")
    
    if(WCN_SIMD_ENABLE_NATIVE)
        target_compile_options(${PROJECT_NAME} PRIVATE -march=native)
    else()
        # 启用 LoongArch 所有 SIMD 扩展
        target_compile_options(${PROJECT_NAME} PRIVATE -mlsx -mlasx)
    endif()
    message(STATUS "Enabled LoongArch SIMD extensions (LSX+LASX)")

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "riscv")
    message(STATUS "Detected RISC-V architecture - enabling RVV optimization")
    
    if(WCN_SIMD_ENABLE_NATIVE)
        target_compile_options(${PROJECT_NAME} PRIVATE -march=native)
    else()
        # RISC-V 向量扩展
        check_c_compiler_flag("-march=rv64gcv" COMPILER_SUPPORTS_RVV)
        if(COMPILER_SUPPORTS_RVV)
            target_compile_options(${PROJECT_NAME} PRIVATE -march=rv64gcv)
            message(STATUS "Enabled RISC-V V extension (RVV)")
        else()
            check_c_compiler_flag("-march=rv64gc" COMPILER_SUPPORTS_RV64GC)
            if(COMPILER_SUPPORTS_RV64GC)
                target_compile_options(${PROJECT_NAME} PRIVATE -march=rv64gc)
            endif()
        endif()
    endif()

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "ppc|powerpc")
    message(STATUS "Detected PowerPC architecture - enabling VSX optimization")
    
    target_compile_options(${PROJECT_NAME} PRIVATE -maltivec -mvsx -mcpu=power8)
    message(STATUS "Enabled AltiVec and VSX for PowerPC")

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "mips")
    message(STATUS "Detected MIPS architecture - enabling MSA optimization")
    
    target_compile_options(${PROJECT_NAME} PRIVATE -mmsa -mfp64)
    message(STATUS "Enabled MIPS SIMD Architecture (MSA)")

else()
    message(STATUS "Unknown architecture: ${CMAKE_SYSTEM_PROCESSOR} - using generic optimization")
endif()

# WebAssembly 极致优化
if(EMSCRIPTEN)
    message(STATUS "Detected WebAssembly (Emscripten) - enabling WASM SIMD optimization")
    
    # 清理可能不兼容的 x86 标志
    get_target_property(CURRENT_OPTIONS ${PROJECT_NAME} COMPILE_OPTIONS)
    if(CURRENT_OPTIONS)
        foreach(OPTION ${CURRENT_OPTIONS})
            if(OPTION MATCHES "^-m(avx|sse|fma|x86)" OR OPTION MATCHES "^/arch:")
                list(REMOVE_ITEM CURRENT_OPTIONS ${OPTION})
            endif()
        endforeach()
        set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_OPTIONS "${CURRENT_OPTIONS}")
    endif()
    
    # WebAssembly SIMD 优化
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -msimd128 
        -mrelaxed-simd
        -munimplemented-simd128
        -fno-exceptions
    )
    message(STATUS "Enabled WebAssembly SIMD128 with relaxed semantics")

elseif(WASI)
    message(STATUS "Detected WebAssembly (WASI) - enabling WASI optimization")
    # WASI 特定优化可以在这里添加
endif()

# 链接时优化 (LTO)
if(WCN_SIMD_ENABLE_LTO)
    include(CheckCCompilerFlag)
    check_c_compiler_flag("-flto" COMPILER_SUPPORTS_LTO)
    
    if(COMPILER_SUPPORTS_LTO)
        target_compile_options(${PROJECT_NAME} PRIVATE -flto)
        target_link_libraries(${PROJECT_NAME} PRIVATE -flto)
        message(STATUS "Enabled Link Time Optimization (LTO)")
    elseif(MSVC)
        check_c_compiler_flag("/GL" COMPILER_SUPPORTS_GL)
        if(COMPILER_SUPPORTS_GL)
            target_compile_options(${PROJECT_NAME} PRIVATE /GL)
            target_link_libraries(${PROJECT_NAME} PRIVATE /LTCG)
            message(STATUS "Enabled Link Time Code Generation (LTCG) for MSVC")
        endif()
    endif()
endif()

# 反馈导向优化 (PGO) - 高级特性
if(WCN_SIMD_ENABLE_PGO)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        if(MSVC)
            target_compile_options(${PROJECT_NAME} PRIVATE /GL)
            target_link_libraries(${PROJECT_NAME} PRIVATE /LTCG)
            message(STATUS "PGO instrumentation enabled for MSVC - requires two-stage build")
        else()
            target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-generate)
            target_link_libraries(${PROJECT_NAME} PRIVATE -fprofile-generate)
            message(STATUS "PGO instrumentation enabled. Build, run with workload, then rebuild with -fprofile-use")
        endif()
    else()
        message(WARNING "PGO is most effective in Release builds. Current build type: ${CMAKE_BUILD_TYPE}")
    endif()
endif()

# 编译器警告和代码质量
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic
        -Wshadow
        -Wstrict-prototypes
    )
endif()

# 独立 WebAssembly 模块构建
if(EMSCRIPTEN AND BUILD_WASM_MODULE)
    message(STATUS "Building standalone Wasm module target: WCN_SIMD_wasm_module")
    
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/bin/WCN_SIMD.wasm"
        COMMAND ${CMAKE_C_COMPILER}
                ${SRC_DIR}/wcn_simd.c
                ${SRC_DIR}/wcn_atomic.c
                ${SRC_DIR}/wcn_simd_wasm_exports.c
                -I "${INCLUDE_DIR}"
                -O3
                -sSTANDALONE_WASM=1
                --no-entry
                -sEXPORT_ALL=1
                -msimd128
                -mrelaxed-simd
                -o "${CMAKE_BINARY_DIR}/bin/WCN_SIMD.wasm"
        DEPENDS ${SRC_FILES}
        COMMENT "Building optimized standalone Wasm module WCN_SIMD.wasm"
        VERBATIM
    )
    
    add_custom_target(WCN_SIMD_wasm_module
        DEPENDS "${CMAKE_BINARY_DIR}/bin/WCN_SIMD.wasm"
    )
endif()

# 构建示例程序
if(WCN_SIMD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 安装规则
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${INCLUDE_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# 包配置
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

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
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# 配置总结
message(STATUS "")
message(STATUS "=== WCN_SIMD Ultimate Optimization Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME} / ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "SIMD Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Native optimization: ${WCN_SIMD_ENABLE_NATIVE}")
message(STATUS "LTO enabled: ${WCN_SIMD_ENABLE_LTO}")
message(STATUS "PGO enabled: ${WCN_SIMD_ENABLE_PGO}")
message(STATUS "Examples: ${WCN_SIMD_BUILD_EXAMPLES}")
message(STATUS "Standalone WASM: ${BUILD_WASM_MODULE}")

# 显示具体启用的优化标志
get_target_property(OPTIONS ${PROJECT_NAME} COMPILE_OPTIONS)
if(OPTIONS)
    message(STATUS "Active compile options: ${OPTIONS}")
endif()

message(STATUS "===================================================")
message(STATUS "")