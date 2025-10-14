#ifndef WCN_SIMD_TYPES_H
#define WCN_SIMD_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include "wcn_detect.h"

/* Platform-specific intrinsics */
#ifdef WCN_ARCH_X86
    #ifdef WCN_X86_SSE2
        #include <emmintrin.h>  /* SSE2 */
    #endif
    #ifdef WCN_X86_SSE3
        #include <pmmintrin.h>  /* SSE3 */
    #endif
    #ifdef WCN_X86_SSSE3
        #include <tmmintrin.h>  /* SSSE3 */
    #endif
    #ifdef WCN_X86_SSE4_1
        #include <smmintrin.h>  /* SSE4.1 */
    #endif
    #ifdef WCN_X86_SSE4_2
        #include <nmmintrin.h>  /* SSE4.2 */
    #endif
    #ifdef WCN_X86_AVX
        #include <immintrin.h>  /* AVX, AVX2, AVX-512 */
    #endif
#endif

#ifdef WCN_ARCH_ARM
    #ifdef WCN_ARM_NEON
        #include <arm_neon.h>
    #endif
    #ifdef WCN_ARM_SVE
        #include <arm_sve.h>
    #endif
#endif

#ifdef WCN_LOONGARCH_LSX
    #include <lsxintrin.h>
#endif

#ifdef WCN_LOONGARCH_LASX
    #include <lasxintrin.h>
#endif

#ifdef WCN_RISCV_RVV
    #include <riscv_vector.h>
#endif

#ifdef WCN_POWERPC_ALTIVEC
    #include <altivec.h>
#endif

#ifdef WCN_MIPS_MSA
    #include <msa.h>
#endif

#ifdef WCN_WASM_SIMD128
    #include <wasm_simd128.h>
#endif

/* Unified Vector Types */

/* 128-bit vectors */
typedef struct {
#if defined(WCN_X86_SSE2)
    __m128i raw;
#elif defined(WCN_ARM_NEON)
    int32x4_t raw;
#elif defined(WCN_LOONGARCH_LSX)
    __m128i raw;
#elif defined(WCN_POWERPC_ALTIVEC)
    vector signed int raw;
#elif defined(WCN_MIPS_MSA)
    v4i32 raw;
#elif defined(WCN_WASM_SIMD128)
    v128_t raw;
#else
    int32_t raw[4];
#endif
} wcn_v128i_t;

typedef struct {
#if defined(WCN_X86_SSE2)
    __m128 raw;
#elif defined(WCN_ARM_NEON)
    float32x4_t raw;
#elif defined(WCN_LOONGARCH_LSX)
    __m128 raw;
#elif defined(WCN_POWERPC_ALTIVEC)
    vector float raw;
#elif defined(WCN_MIPS_MSA)
    v4f32 raw;
#elif defined(WCN_WASM_SIMD128)
    v128_t raw;
#else
    float raw[4];
#endif
} wcn_v128f_t;

typedef struct {
#if defined(WCN_X86_SSE2)
    __m128d raw;
#elif defined(WCN_ARM_NEON) && defined(WCN_ARM_AARCH64)
    float64x2_t raw;
#elif defined(WCN_LOONGARCH_LSX)
    __m128d raw;
#elif defined(WCN_POWERPC_VSX)
    vector double raw;
#else
    double raw[2];
#endif
} wcn_v128d_t;

/* 256-bit vectors */
typedef struct {
#if defined(WCN_X86_AVX2)
    __m256i raw;
#elif defined(WCN_LOONGARCH_LASX)
    __m256i raw;
#else
    wcn_v128i_t low, high;
#endif
} wcn_v256i_t;

typedef struct {
#if defined(WCN_X86_AVX)
    __m256 raw;
#elif defined(WCN_LOONGARCH_LASX)
    __m256 raw;
#else
    wcn_v128f_t low, high;
#endif
} wcn_v256f_t;

typedef struct {
#if defined(WCN_X86_AVX)
    __m256d raw;
#elif defined(WCN_LOONGARCH_LASX)
    __m256d raw;
#else
    wcn_v128d_t low, high;
#endif
} wcn_v256d_t;

/* 512-bit vectors */
typedef struct {
#if defined(WCN_X86_AVX512F)
    __m512i raw;
#else
    wcn_v256i_t low, high;
#endif
} wcn_v512i_t;

typedef struct {
#if defined(WCN_X86_AVX512F)
    __m512 raw;
#else
    wcn_v256f_t low, high;
#endif
} wcn_v512f_t;

typedef struct {
#if defined(WCN_X86_AVX512F)
    __m512d raw;
#else
    wcn_v256d_t low, high;
#endif
} wcn_v512d_t;

/* Scalable vectors (ARM SVE, RISC-V RVV) */
#ifdef WCN_ARM_SVE
typedef struct {
    svint32_t raw;
} wcn_vscalable_i32_t;

typedef struct {
    svfloat32_t raw;
} wcn_vscalable_f32_t;

typedef struct {
    svfloat64_t raw;
} wcn_vscalable_f64_t;
#endif

#ifdef WCN_RISCV_RVV
typedef struct {
    vint32m1_t raw;
} wcn_vscalable_i32_t;

typedef struct {
    vfloat32m1_t raw;
} wcn_vscalable_f32_t;

typedef struct {
    vfloat64m1_t raw;
} wcn_vscalable_f64_t;
#endif

/* Element types for explicit loads/stores */
typedef int8_t   wcn_i8_t;
typedef int16_t  wcn_i16_t;
typedef int32_t  wcn_i32_t;
typedef int64_t  wcn_i64_t;
typedef uint8_t  wcn_u8_t;
typedef uint16_t wcn_u16_t;
typedef uint32_t wcn_u32_t;
typedef uint64_t wcn_u64_t;
typedef float    wcn_f32_t;
typedef double   wcn_f64_t;

/* Lane count constants */
#define WCN_V128_I8_LANES  16
#define WCN_V128_I16_LANES 8
#define WCN_V128_I32_LANES 4
#define WCN_V128_I64_LANES 2
#define WCN_V128_F32_LANES 4
#define WCN_V128_F64_LANES 2

#define WCN_V256_I8_LANES  32
#define WCN_V256_I16_LANES 16
#define WCN_V256_I32_LANES 8
#define WCN_V256_I64_LANES 4
#define WCN_V256_F32_LANES 8
#define WCN_V256_F64_LANES 4

#define WCN_V512_I8_LANES  64
#define WCN_V512_I16_LANES 32
#define WCN_V512_I32_LANES 16
#define WCN_V512_I64_LANES 8
#define WCN_V512_F32_LANES 16
#define WCN_V512_F64_LANES 8

#endif /* WCN_SIMD_TYPES_H */
