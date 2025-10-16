#ifndef WCN_SIMD_DETECT_H
#define WCN_SIMD_DETECT_H

/* Platform Detection Macros */

/* x86/x86_64 Architecture */
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
    #define WCN_ARCH_X86 1
    
    /* AVX-512 */
    #if defined(__AVX512F__)
        #define WCN_X86_AVX512F 1
    #endif
    #if defined(__AVX512BW__)
        #define WCN_X86_AVX512BW 1
    #endif
    #if defined(__AVX512DQ__)
        #define WCN_X86_AVX512DQ 1
    #endif
    #if defined(__AVX512VL__)
        #define WCN_X86_AVX512VL 1
    #endif
    
    /* AVX2 */
    #if defined(__AVX2__)
        #define WCN_X86_AVX2 1
    #endif
    
    /* FMA */
    #if defined(__FMA__)
        #define WCN_X86_FMA 1
    #endif
    
    /* AVX */
    #if defined(__AVX__)
        #define WCN_X86_AVX 1
    #endif
    
    /* SSE4.2 */
    #if defined(__SSE4_2__)
        #define WCN_X86_SSE4_2 1
    #endif
    
    /* SSE4.1 */
    #if defined(__SSE4_1__)
        #define WCN_X86_SSE4_1 1
    #endif
    
    /* SSSE3 */
    #if defined(__SSSE3__)
        #define WCN_X86_SSSE3 1
    #endif
    
    /* SSE3 */
    #if defined(__SSE3__)
        #define WCN_X86_SSE3 1
    #endif
    
    /* SSE2 (baseline for x86_64) */
    #if defined(__SSE2__) || defined(_M_X64)
        #define WCN_X86_SSE2 1
    #endif
#endif

/* ARM Architecture */
#if defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #define WCN_ARCH_ARM 1
    
    /* ARM64 */
    #if defined(__aarch64__) || defined(_M_ARM64)
        #define WCN_ARM_AARCH64 1
    #endif
    
    /* NEON */
    #if defined(__ARM_NEON) || defined(__ARM_NEON__)
        #define WCN_ARM_NEON 1
    #endif
    
    /* SVE (Scalable Vector Extension) */
    #if defined(__ARM_FEATURE_SVE)
        #define WCN_ARM_SVE 1
    #endif
    
    /* SVE2 */
    #if defined(__ARM_FEATURE_SVE2)
        #define WCN_ARM_SVE2 1
    #endif
#endif

/* LoongArch Architecture */
#if defined(__loongarch__)
    #define WCN_ARCH_LOONGARCH 1
    
    /* LSX (Loongson SIMD Extension) */
    #if defined(__loongarch_sx)
        #define WCN_LOONGARCH_LSX 1
    #endif
    
    /* LASX (Loongson Advanced SIMD Extension) */
    #if defined(__loongarch_asx)
        #define WCN_LOONGARCH_LASX 1
    #endif
#endif

/* RISC-V Architecture */
#if defined(__riscv)
    #define WCN_ARCH_RISCV 1
    
    /* RVV (RISC-V Vector Extension) */
    #if defined(__riscv_vector)
        #define WCN_RISCV_RVV 1
    #endif
#endif

/* PowerPC Architecture */
#if defined(__powerpc__) || defined(__ppc__) || defined(_ARCH_PPC)
    #define WCN_ARCH_POWERPC 1
    
    /* AltiVec/VMX */
    #if defined(__ALTIVEC__) || defined(__VEC__)
        #define WCN_POWERPC_ALTIVEC 1
    #endif
    
    /* VSX (Vector Scalar Extension) */
    #if defined(__VSX__)
        #define WCN_POWERPC_VSX 1
    #endif
#endif

/* MIPS Architecture */
#if defined(__mips__) || defined(__mips) || defined(__MIPS__)
    #define WCN_ARCH_MIPS 1
    
    /* MSA (MIPS SIMD Architecture) */
    #if defined(__mips_msa)
        #define WCN_MIPS_MSA 1
    #endif
#endif

/* WebAssembly */
#if defined(__wasm__) || defined(__wasm32__) || defined(__wasm64__)
    #define WCN_ARCH_WASM 1
    
    /* WASM SIMD128 */
    #if defined(__wasm_simd128__)
        #define WCN_WASM_SIMD128 1
    #endif
    
    /* WASM Atomic operations */
    #if defined(__wasm_atomics__)
        #define WCN_WASM_ATOMIC 1
    #endif
#endif

/* Atomic operations availability */
#if defined(__GNUC__) || defined(__clang__)
    #define WCN_HAS_GCC_ATOMIC 1
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1900)
    #define WCN_HAS_MSVC_ATOMIC 1
#endif

/* Compiler Detection */
#if defined(__GNUC__) || defined(__clang__)
    #define WCN_COMPILER_GCC_COMPATIBLE 1
    #define WCN_INLINE static inline __attribute__((always_inline))
    #define WCN_ALIGN(n) __attribute__((aligned(n)))
#elif defined(_MSC_VER)
    #define WCN_COMPILER_MSVC 1
    #define WCN_INLINE static __forceinline
    #define WCN_ALIGN(n) __declspec(align(n))
#else
    #define WCN_INLINE static inline
    #define WCN_ALIGN(n)
#endif

/* Select Best Available SIMD Implementation */
#if defined(WCN_X86_AVX512F)
    #define WCN_SIMD_IMPL "x86_avx512f"
    #define WCN_VECTOR_WIDTH 512
#elif defined(WCN_X86_AVX2)
    #define WCN_SIMD_IMPL "x86_avx2"
    #define WCN_VECTOR_WIDTH 256
#elif defined(WCN_X86_AVX)
    #define WCN_SIMD_IMPL "x86_avx"
    #define WCN_VECTOR_WIDTH 256
#elif defined(WCN_X86_FMA)
    #define WCN_SIMD_IMPL "x86_fma"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_X86_SSE4_2)
    #define WCN_SIMD_IMPL "x86_sse4_2"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_X86_SSE4_1)
    #define WCN_SIMD_IMPL "x86_sse4_1"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_X86_SSSE3)
    #define WCN_SIMD_IMPL "x86_ssse3"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_X86_SSE3)
    #define WCN_SIMD_IMPL "x86_sse3"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_X86_SSE2)
    #define WCN_SIMD_IMPL "x86_sse2"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_ARM_SVE2)
    #define WCN_SIMD_IMPL "arm_sve2"
    #define WCN_VECTOR_WIDTH 0  /* SVE is length-agnostic */
#elif defined(WCN_ARM_SVE)
    #define WCN_SIMD_IMPL "arm_sve"
    #define WCN_VECTOR_WIDTH 0  /* SVE is length-agnostic */
#elif defined(WCN_ARM_NEON)
    #define WCN_SIMD_IMPL "arm_neon"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_LOONGARCH_LASX)
    #define WCN_SIMD_IMPL "loongarch_lasx"
    #define WCN_VECTOR_WIDTH 256
#elif defined(WCN_LOONGARCH_LSX)
    #define WCN_SIMD_IMPL "loongarch_lsx"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_RISCV_RVV)
    #define WCN_SIMD_IMPL "riscv_rvv"
    #define WCN_VECTOR_WIDTH 0  /* RVV is length-agnostic */
#elif defined(WCN_POWERPC_VSX)
    #define WCN_SIMD_IMPL "powerpc_vsx"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_POWERPC_ALTIVEC)
    #define WCN_SIMD_IMPL "powerpc_altivec"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_MIPS_MSA)
    #define WCN_SIMD_IMPL "mips_msa"
    #define WCN_VECTOR_WIDTH 128
#elif defined(WCN_WASM_SIMD128)
    #define WCN_SIMD_IMPL "wasm_simd128"
    #define WCN_VECTOR_WIDTH 128
#else
    #define WCN_SIMD_IMPL "scalar"
    #define WCN_VECTOR_WIDTH 0
#endif

#endif /* WCN_SIMD_DETECT_H */
