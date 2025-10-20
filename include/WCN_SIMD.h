#ifndef WCN_SIMD_H
#define WCN_SIMD_H

/*
 * WCN_SIMD - High-Performance Cross-Platform SIMD Library
 *
 * Unified SIMD abstraction layer supporting:
 * - x86/x86_64: SSE2, SSE4, AVX, AVX2, AVX-512
 * - ARM: NEON, SVE, SVE2
 * - LoongArch: LSX, LASX
 * - RISC-V: RVV
 * - PowerPC: AltiVec, VSX
 * - MIPS: MSA
 * - WebAssembly: SIMD128
 *
 * Design Principles:
 * - Zero-overhead abstraction
 * - Compile-time platform detection
 * - Performance-first design
 * - Easy to use and maintain
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Portable export macro for public APIs (Standalone Wasm export on Emscripten)
 */
#ifndef WCN_API_EXPORT
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define WCN_API_EXPORT EMSCRIPTEN_KEEPALIVE
#else
#define WCN_API_EXPORT
#endif
#endif

#include "wcn_simd/wcn_detect.h"
#include "wcn_simd/wcn_types.h"

/* Include platform-specific implementations */
#if defined(WCN_X86_SSE2)
#include "wcn_simd/platform/x86/wcn_x86_sse2.h"
#endif

#if defined(WCN_X86_SSE3)
#include "wcn_simd/platform/x86/wcn_x86_sse3.h"
#endif

#if defined(WCN_X86_SSSE3)
#include "wcn_simd/platform/x86/wcn_x86_ssse3.h"
#endif

#if defined(WCN_X86_SSE4_1)
#include "wcn_simd/platform/x86/wcn_x86_sse4_1.h"
#endif

#if defined(WCN_X86_AVX2)
/* AVX2 is a superset of AVX, include only AVX2 to avoid redefinition */
#include "wcn_simd/platform/x86/wcn_x86_avx2.h"
#elif defined(WCN_X86_AVX)
#include "wcn_simd/platform/x86/wcn_x86_avx.h"
#endif

/* Temporarily disabled until AVX512F is tested */
/*
#if defined(WCN_X86_AVX512F)
    #include "wcn_simd/platform/x86/wcn_x86_avx512f.h"
#endif
*/

#if defined(WCN_ARM_NEON)
#include "wcn_simd/platform/arm/wcn_arm_neon.h"
#endif

#if defined(WCN_ARM_SVE)
#include "wcn_simd/platform/arm/wcn_arm_sve.h"
#endif

#if defined(WCN_LOONGARCH_LSX)
#include "wcn_simd/platform/loongarch/wcn_loongarch_lsx.h"
#endif

#if defined(WCN_LOONGARCH_LASX)
#include "wcn_simd/platform/loongarch/wcn_loongarch_lasx.h"
#endif

#if defined(WCN_RISCV_RVV)
#include "wcn_simd/platform/riscv/wcn_riscv_rvv.h"
#endif

#if defined(WCN_POWERPC_ALTIVEC)
#include "wcn_simd/platform/powerpc/wcn_powerpc_altivec.h"
#endif

#if defined(WCN_WASM_SIMD128)
#include "wcn_simd/platform/wasm/wcn_wasm_simd128.h"
#endif

#if defined(WCN_MIPS_MSA)
#include "wcn_simd/platform/mips/wcn_mips_msa.h"
#endif

/* Include atomic operations */
#include "wcn_simd/wcn_atomic.h"

/* Include platform-specific atomic implementations */
#if defined(WCN_X86_SSE2)
#include "wcn_simd/platform/x86/wcn_x86_sse2_atomic.h"
#endif

#if defined(WCN_X86_AVX2)
#include "wcn_simd/platform/x86/wcn_x86_avx2_atomic.h"
#endif

#if defined(WCN_ARM_NEON)
#include "wcn_simd/platform/arm/wcn_arm_neon_atomic.h"
#endif

#if defined(WCN_LOONGARCH_LSX)
#include "wcn_simd/platform/loongarch/wcn_loongarch_lsx_atomic.h"
#endif

#if defined(WCN_RISCV_RVV)
#include "wcn_simd/platform/riscv/wcn_riscv_rvv_atomic.h"
#endif

#if defined(WCN_POWERPC_ALTIVEC)
#include "wcn_simd/platform/powerpc/wcn_powerpc_altivec_atomic.h"
#endif

#if defined(WCN_WASM_SIMD128)
#include "wcn_simd/platform/wasm/wcn_wasm_simd128_atomic.h"
#endif

/* ========== Library Information ========== */

#define WCN_SIMD_VERSION_MAJOR 1
#define WCN_SIMD_VERSION_MINOR 0
#define WCN_SIMD_VERSION_PATCH 0

/* Get SIMD implementation name */
WCN_INLINE const char *wcn_simd_get_impl(void) { return WCN_SIMD_IMPL; }

/* Get SIMD vector width in bits (0 for scalable) */
WCN_INLINE int wcn_simd_get_vector_width(void) { return WCN_VECTOR_WIDTH; }

/* Get version string */
WCN_INLINE const char *wcn_simd_get_version(void) { return "1.0.0"; }

/* ========== Runtime Feature Detection ========== */

typedef struct {
  /* x86 features */
  int has_sse2;
  int has_sse3;
  int has_ssse3;
  int has_sse4_1;
  int has_sse4_2;
  int has_avx;
  int has_avx2;
  int has_avx512f;
  int has_avx512bw;
  int has_avx512dq;
  int has_avx512vl;
  int has_fma;

  /* ARM features */
  int has_neon;
  int has_sve;
  int has_sve2;

  /* LoongArch features */
  int has_lsx;
  int has_lasx;

  /* RISC-V features */
  int has_rvv;

  /* PowerPC features */
  int has_altivec;
  int has_vsx;

  /* MIPS features */
  int has_msa;

  /* WebAssembly features */
  int has_wasm_simd128;

  /* Atomic operation features */
  int has_atomic_operations;
  int has_gcc_atomic;
  int has_msvc_atomic;
} wcn_simd_features_t;

/* Initialize feature detection (call once at startup) */
WCN_API_EXPORT void wcn_simd_init(void);

/* Get detected SIMD features */
WCN_API_EXPORT const wcn_simd_features_t *wcn_simd_get_features(void);

/* ========== Unified Platform-Agnostic SIMD Operations ========== */

/* These macros provide a unified interface that automatically maps to the best
   available platform-specific implementation. Users don't need to care about
   which SIMD instruction set is being used. */

/* ========== Load/Store Operations (Unified) ========== */

/* Load 128-bit integer vector from memory (unaligned) */
#define wcn_simd_load_i128(ptr) wcn_v128i_load(ptr)

/* Load 128-bit integer vector from memory (aligned) */
#define wcn_simd_load_i128_aligned(ptr) wcn_v128i_load_aligned(ptr)

/* Store 128-bit integer vector to memory (unaligned) */
#define wcn_simd_store_i128(ptr, vec) wcn_v128i_store((ptr), (vec))

/* Store 128-bit integer vector to memory (aligned) */
#define wcn_simd_store_i128_aligned(ptr, vec)                                  \
  wcn_v128i_store_aligned((ptr), (vec))

/* Load 128-bit float vector from memory */
#define wcn_simd_load_f32(ptr) wcn_v128f_load(ptr)

/* Store 128-bit float vector to memory */
#define wcn_simd_store_f32(ptr, vec) wcn_v128f_store((ptr), (vec))

/* Load 128-bit double vector from memory */
#define wcn_simd_load_f64(ptr) wcn_v128d_load(ptr)

/* Store 128-bit double vector to memory */
#define wcn_simd_store_f64(ptr, vec) wcn_v128d_store((ptr), (vec))

/* ========== Initialization (Unified) ========== */

/* Set all 8-bit integer lanes to the same value */
#define wcn_simd_set1_i8(value) wcn_v128i_set1_i8(value)

/* Set all 16-bit integer lanes to the same value */
#define wcn_simd_set1_i16(value) wcn_v128i_set1_i16(value)

/* Set all 32-bit integer lanes to the same value */
#define wcn_simd_set1_i32(value) wcn_v128i_set1_i32(value)

/* Set all 64-bit integer lanes to the same value */
#define wcn_simd_set1_i64(value) wcn_v128i_set1_i64(value)

/* Set all lanes to zero (integer) */
#define wcn_simd_setzero_i128() wcn_v128i_setzero()

/* Set all float lanes to the same value */
#define wcn_simd_set1_f32(value) wcn_v128f_set1(value)

/* Set all lanes to zero (float) */
#define wcn_simd_setzero_f32() wcn_v128f_setzero()

/* Set all double lanes to the same value */
#define wcn_simd_set1_f64(value) wcn_v128d_set1(value)

/* Set all lanes to zero (double) */
#define wcn_simd_setzero_f64() wcn_v128d_setzero()

/* ========== Arithmetic Operations (Unified) ========== */

/* 8-bit integer addition */
#define wcn_simd_add_i8(a, b) wcn_v128i_add_i8((a), (b))

/* 8-bit integer subtraction */
#define wcn_simd_sub_i8(a, b) wcn_v128i_sub_i8((a), (b))

/* 16-bit integer addition */
#define wcn_simd_add_i16(a, b) wcn_v128i_add_i16((a), (b))

/* 16-bit integer subtraction */
#define wcn_simd_sub_i16(a, b) wcn_v128i_sub_i16((a), (b))

/* 32-bit integer addition */
#define wcn_simd_add_i32(a, b) wcn_v128i_add_i32((a), (b))

/* 32-bit integer subtraction */
#define wcn_simd_sub_i32(a, b) wcn_v128i_sub_i32((a), (b))

/* 64-bit integer addition */
#define wcn_simd_add_i64(a, b) wcn_v128i_add_i64((a), (b))

/* 64-bit integer subtraction */
#define wcn_simd_sub_i64(a, b) wcn_v128i_sub_i64((a), (b))

/* Float addition */
#define wcn_simd_add_f32(a, b) wcn_v128f_add((a), (b))

/* Float subtraction */
#define wcn_simd_sub_f32(a, b) wcn_v128f_sub((a), (b))

/* Float multiplication */
#define wcn_simd_mul_f32(a, b) wcn_v128f_mul((a), (b))

/* Float division */
#define wcn_simd_div_f32(a, b) wcn_v128f_div((a), (b))

/* Double addition */
#define wcn_simd_add_f64(a, b) wcn_v128d_add((a), (b))

/* Double subtraction */
#define wcn_simd_sub_f64(a, b) wcn_v128d_sub((a), (b))

/* Double multiplication */
#define wcn_simd_mul_f64(a, b) wcn_v128d_mul((a), (b))

/* Double division */
#define wcn_simd_div_f64(a, b) wcn_v128d_div((a), (b))

/* ========== Saturating Arithmetic (Unified) ========== */

/* 8-bit signed saturating addition */
#define wcn_simd_adds_i8(a, b) wcn_v128i_adds_i8((a), (b))

/* 8-bit signed saturating subtraction */
#define wcn_simd_subs_i8(a, b) wcn_v128i_subs_i8((a), (b))

/* 8-bit unsigned saturating addition */
#define wcn_simd_adds_u8(a, b) wcn_v128i_adds_u8((a), (b))

/* 8-bit unsigned saturating subtraction */
#define wcn_simd_subs_u8(a, b) wcn_v128i_subs_u8((a), (b))

/* 16-bit signed saturating addition */
#define wcn_simd_adds_i16(a, b) wcn_v128i_adds_i16((a), (b))

/* 16-bit signed saturating subtraction */
#define wcn_simd_subs_i16(a, b) wcn_v128i_subs_i16((a), (b))

/* 16-bit unsigned saturating addition */
#define wcn_simd_adds_u16(a, b) wcn_v128i_adds_u16((a), (b))

/* 16-bit unsigned saturating subtraction */
#define wcn_simd_subs_u16(a, b) wcn_v128i_subs_u16((a), (b))

/* ========== Shift Operations (Unified) ========== */

/* 16-bit left shift (variable count) */
#define wcn_simd_sll_i16(a, count) wcn_v128i_sll_i16((a), (count))

/* 16-bit logical right shift (variable count) */
#define wcn_simd_srl_i16(a, count) wcn_v128i_srl_i16((a), (count))

/* 16-bit arithmetic right shift (variable count) */
#define wcn_simd_sra_i16(a, count) wcn_v128i_sra_i16((a), (count))

/* 16-bit left shift (immediate count) */
#define wcn_simd_slli_i16(a, imm) wcn_v128i_slli_i16((a), (imm))

/* 16-bit logical right shift (immediate count) */
#define wcn_simd_srli_i16(a, imm) wcn_v128i_srli_i16((a), (imm))

/* 16-bit arithmetic right shift (immediate count) */
#define wcn_simd_srai_i16(a, imm) wcn_v128i_srai_i16((a), (imm))

/* 32-bit left shift (variable count) */
#define wcn_simd_sll_i32(a, count) wcn_v128i_sll_i32((a), (count))

/* 32-bit logical right shift (variable count) */
#define wcn_simd_srl_i32(a, count) wcn_v128i_srl_i32((a), (count))

/* 32-bit arithmetic right shift (variable count) */
#define wcn_simd_sra_i32(a, count) wcn_v128i_sra_i32((a), (count))

/* 32-bit left shift (immediate count) */
#define wcn_simd_slli_i32(a, imm) wcn_v128i_slli_i32((a), (imm))

/* 32-bit logical right shift (immediate count) */
#define wcn_simd_srli_i32(a, imm) wcn_v128i_srli_i32((a), (imm))

/* 32-bit arithmetic right shift (immediate count) */
#define wcn_simd_srai_i32(a, imm) wcn_v128i_srai_i32((a), (imm))

/* 64-bit left shift (variable count) */
#define wcn_simd_sll_i64(a, count) wcn_v128i_sll_i64((a), (count))

/* 64-bit logical right shift (variable count) */
#define wcn_simd_srl_i64(a, count) wcn_v128i_srl_i64((a), (count))

/* 64-bit left shift (immediate count) */
#define wcn_simd_slli_i64(a, imm) wcn_v128i_slli_i64((a), (imm))

/* 64-bit logical right shift (immediate count) */
#define wcn_simd_srli_i64(a, imm) wcn_v128i_srli_i64((a), (imm))

/* Shift entire 128-bit vector left by bytes (immediate count) */
#define wcn_simd_slli_si128(a, imm) wcn_v128i_slli_si128((a), (imm))

/* Shift entire 128-bit vector right by bytes (immediate count) */
#define wcn_simd_srli_si128(a, imm) wcn_v128i_srli_si128((a), (imm))

/* ========== Logical Operations (Unified) ========== */

/* Bitwise AND */
#define wcn_simd_and_i128(a, b) wcn_v128i_and((a), (b))

/* Bitwise OR */
#define wcn_simd_or_i128(a, b) wcn_v128i_or((a), (b))

/* Bitwise XOR */
#define wcn_simd_xor_i128(a, b) wcn_v128i_xor((a), (b))

/* Bitwise ANDNOT (a AND NOT b) */
#define wcn_simd_andnot_i128(a, b) wcn_v128i_andnot((a), (b))

/* ========== Comparison Operations (Unified) ========== */

/* 8-bit integer equal comparison */
#define wcn_simd_cmpeq_i8(a, b) wcn_v128i_cmpeq_i8((a), (b))

/* 8-bit integer greater than comparison */
#define wcn_simd_cmpgt_i8(a, b) wcn_v128i_cmpgt_i8((a), (b))

/* 8-bit integer less than comparison */
#define wcn_simd_cmplt_i8(a, b) wcn_v128i_cmplt_i8((a), (b))

/* 16-bit integer equal comparison */
#define wcn_simd_cmpeq_i16(a, b) wcn_v128i_cmpeq_i16((a), (b))

/* 16-bit integer greater than comparison */
#define wcn_simd_cmpgt_i16(a, b) wcn_v128i_cmpgt_i16((a), (b))

/* 16-bit integer less than comparison */
#define wcn_simd_cmplt_i16(a, b) wcn_v128i_cmplt_i16((a), (b))

/* 32-bit integer equal comparison */
#define wcn_simd_cmpeq_i32(a, b) wcn_v128i_cmpeq_i32((a), (b))

/* 32-bit integer greater than comparison */
#define wcn_simd_cmpgt_i32(a, b) wcn_v128i_cmpgt_i32((a), (b))

/* 32-bit integer less than comparison */
#define wcn_simd_cmplt_i32(a, b) wcn_v128i_cmplt_i32((a), (b))

/* Float equal comparison */
#define wcn_simd_cmpeq_f32(a, b) wcn_v128f_cmpeq((a), (b))

/* Float less than comparison */
#define wcn_simd_cmplt_f32(a, b) wcn_v128f_cmplt((a), (b))

/* Float less than or equal comparison */
#define wcn_simd_cmple_f32(a, b) wcn_v128f_cmple((a), (b))

/* Float greater than comparison */
#define wcn_simd_cmpgt_f32(a, b) wcn_v128f_cmpgt((a), (b))

/* Float greater than or equal comparison */
#define wcn_simd_cmpge_f32(a, b) wcn_v128f_cmpge((a), (b))

/* Double equal comparison */
#define wcn_simd_cmpeq_f64(a, b) wcn_v128d_cmpeq((a), (b))

/* Double less than comparison */
#define wcn_simd_cmplt_f64(a, b) wcn_v128d_cmplt((a), (b))

/* Double less than or equal comparison */
#define wcn_simd_cmple_f64(a, b) wcn_v128d_cmple((a), (b))

/* Double greater than comparison */
#define wcn_simd_cmpgt_f64(a, b) wcn_v128d_cmpgt((a), (b))

/* Double greater than or equal comparison */
#define wcn_simd_cmpge_f64(a, b) wcn_v128d_cmpge((a), (b))

/* Double not equal comparison */
#define wcn_simd_cmpneq_f64(a, b) wcn_v128d_cmpneq((a), (b))

/* ========== Min/Max Operations (Unified) ========== */

/* 8-bit signed integer minimum */
#define wcn_simd_min_i8(a, b) wcn_v128i_min_i8((a), (b))

/* 8-bit signed integer maximum */
#define wcn_simd_max_i8(a, b) wcn_v128i_max_i8((a), (b))

/* 8-bit unsigned integer minimum */
#define wcn_simd_min_u8(a, b) wcn_v128i_min_u8((a), (b))

/* 8-bit unsigned integer maximum */
#define wcn_simd_max_u8(a, b) wcn_v128i_max_u8((a), (b))

/* 16-bit signed integer minimum */
#define wcn_simd_min_i16(a, b) wcn_v128i_min_i16((a), (b))

/* 16-bit signed integer maximum */
#define wcn_simd_max_i16(a, b) wcn_v128i_max_i16((a), (b))

/* 32-bit integer maximum */
#define wcn_simd_max_i32(a, b) wcn_v128i_max_i32((a), (b))

/* 32-bit integer minimum */
#define wcn_simd_min_i32(a, b) wcn_v128i_min_i32((a), (b))

/* Float maximum */
#define wcn_simd_max_f32(a, b) wcn_v128f_max((a), (b))

/* Float minimum */
#define wcn_simd_min_f32(a, b) wcn_v128f_min((a), (b))

/* Double maximum */
#define wcn_simd_max_f64(a, b) wcn_v128d_max((a), (b))

/* Double minimum */
#define wcn_simd_min_f64(a, b) wcn_v128d_min((a), (b))

/* ========== Square Root (Unified) ========== */

/* Float square root */
#define wcn_simd_sqrt_f32(vec) wcn_v128f_sqrt(vec)

/* Double square root */
#define wcn_simd_sqrt_f64(vec) wcn_v128d_sqrt(vec)

/* ========== Reciprocal Approximations (Unified) ========== */

/* Float reciprocal approximation (1/x) */
#define wcn_simd_rcp_f32(vec) wcn_v128f_rcp(vec)

/* Float reciprocal square root approximation (1/sqrt(x)) */
#define wcn_simd_rsqrt_f32(vec) wcn_v128f_rsqrt(vec)

/* ========== Absolute Value (Unified) ========== */

/* 8-bit integer absolute value */
#define wcn_simd_abs_i8(vec) wcn_v128i_abs_i8(vec)

/* 16-bit integer absolute value */
#define wcn_simd_abs_i16(vec) wcn_v128i_abs_i16(vec)

/* 32-bit integer absolute value */
#define wcn_simd_abs_i32(vec) wcn_v128i_abs_i32(vec)

/* Float absolute value */
#define wcn_simd_abs_f32(vec) wcn_v128f_abs(vec)

/* Double absolute value */
#define wcn_simd_abs_f64(vec) wcn_v128d_abs(vec)

/* ========== Negation (Unified) ========== */

/* 8-bit integer negation */
#define wcn_simd_neg_i8(vec) wcn_v128i_neg_i8(vec)

/* 16-bit integer negation */
#define wcn_simd_neg_i16(vec) wcn_v128i_neg_i16(vec)

/* 32-bit integer negation */
#define wcn_simd_neg_i32(vec) wcn_v128i_neg_i32(vec)

/* 64-bit integer negation */
#define wcn_simd_neg_i64(vec) wcn_v128i_neg_i64(vec)

/* Float negation */
#define wcn_simd_neg_f32(vec) wcn_v128f_neg(vec)

/* Double negation */
#define wcn_simd_neg_f64(vec) wcn_v128d_neg(vec)

/* ========== Broadcast (Unified) ========== */

/* Broadcast 32-bit integer from memory */
#define wcn_simd_broadcast_i32(ptr) wcn_v128i_broadcast_i32(ptr)

/* Broadcast float from memory */
#define wcn_simd_broadcast_f32(ptr) wcn_v128f_broadcast(ptr)

/* Broadcast double from memory */
#define wcn_simd_broadcast_f64(ptr) wcn_v128d_broadcast(ptr)

/* ========== FMA - Fused Multiply-Add (Unified) ========== */

/* Float FMA: a * b + c */
#define wcn_simd_fmadd_f32(a, b, c) wcn_v128f_fmadd((a), (b), (c))

/* Double FMA: a * b + c */
#define wcn_simd_fmadd_f64(a, b, c) wcn_v128d_fmadd((a), (b), (c))

/* ========== Rounding Operations (Unified) ========== */

/* Round floats toward negative infinity (floor) */
#define wcn_simd_floor_f32(vec) wcn_v128f_floor(vec)

/* Round floats toward positive infinity (ceil) */
#define wcn_simd_ceil_f32(vec) wcn_v128f_ceil(vec)

/* Round floats to nearest integer */
#define wcn_simd_round_f32(vec) wcn_v128f_round_nearest(vec)

/* Round floats toward zero (truncate) */
#define wcn_simd_trunc_f32(vec) wcn_v128f_trunc(vec)

/* Round doubles toward negative infinity (floor) */
#define wcn_simd_floor_f64(vec) wcn_v128d_floor(vec)

/* Round doubles toward positive infinity (ceil) */
#define wcn_simd_ceil_f64(vec) wcn_v128d_ceil(vec)

/* Round doubles to nearest integer */
#define wcn_simd_round_f64(vec) wcn_v128d_round_nearest(vec)

/* Round doubles toward zero (truncate) */
#define wcn_simd_trunc_f64(vec) wcn_v128d_trunc(vec)

/* ========== Type Conversions (Unified) ========== */

/* Convert 32-bit integer to float */
#define wcn_simd_cvt_i32_f32(vec) wcn_v128i_to_v128f(vec)

/* Convert float to 32-bit integer */
#define wcn_simd_cvt_f32_i32(vec) wcn_v128f_to_v128i(vec)

/* ========== Integer Multiplication (Unified) ========== */

/* 16-bit integer multiplication (low 16 bits) */
#define wcn_simd_mullo_i16(a, b) wcn_v128i_mullo_i16((a), (b))

/* 16-bit integer multiplication (high 16 bits, signed) */
#define wcn_simd_mulhi_i16(a, b) wcn_v128i_mulhi_i16((a), (b))

/* 16-bit integer multiplication (high 16 bits, unsigned) */
#define wcn_simd_mulhi_u16(a, b) wcn_v128i_mulhi_u16((a), (b))

/* 32-bit integer multiplication (low 32 bits) */
#define wcn_simd_mullo_i32(a, b) wcn_v128i_mullo_i32((a), (b))

/* 32-bit unsigned multiplication to 64-bit */
#define wcn_simd_mul_u32(a, b) wcn_v128i_mul_u32((a), (b))

/* ========== Pack/Unpack Operations (Unified) ========== */

/* Pack 32-bit signed integers to 16-bit with saturation */
#define wcn_simd_packs_i32(a, b) wcn_v128i_packs_i32((a), (b))

/* Pack 16-bit signed integers to 8-bit with saturation */
#define wcn_simd_packs_i16(a, b) wcn_v128i_packs_i16((a), (b))

/* Pack 16-bit signed integers to 8-bit unsigned with saturation */
#define wcn_simd_packus_i16(a, b) wcn_v128i_packus_i16((a), (b))

/* Pack 32-bit signed integers to 16-bit unsigned with saturation */
#define wcn_simd_packus_i32(a, b) wcn_v128i_packus_i32((a), (b))

/* Unpack and interleave low 8-bit integers */
#define wcn_simd_unpacklo_i8(a, b) wcn_v128i_unpacklo_i8((a), (b))

/* Unpack and interleave high 8-bit integers */
#define wcn_simd_unpackhi_i8(a, b) wcn_v128i_unpackhi_i8((a), (b))

/* Unpack and interleave low 16-bit integers */
#define wcn_simd_unpacklo_i16(a, b) wcn_v128i_unpacklo_i16((a), (b))

/* Unpack and interleave high 16-bit integers */
#define wcn_simd_unpackhi_i16(a, b) wcn_v128i_unpackhi_i16((a), (b))

/* Unpack and interleave low 32-bit integers */
#define wcn_simd_unpacklo_i32(a, b) wcn_v128i_unpacklo_i32((a), (b))

/* Unpack and interleave high 32-bit integers */
#define wcn_simd_unpackhi_i32(a, b) wcn_v128i_unpackhi_i32((a), (b))

/* Unpack and interleave low 64-bit integers */
#define wcn_simd_unpacklo_i64(a, b) wcn_v128i_unpacklo_i64((a), (b))

/* Unpack and interleave high 64-bit integers */
#define wcn_simd_unpackhi_i64(a, b) wcn_v128i_unpackhi_i64((a), (b))

/* Unpack and interleave floats */
#define wcn_simd_unpacklo_f32(a, b) wcn_v128f_unpacklo((a), (b))
#define wcn_simd_unpackhi_f32(a, b) wcn_v128f_unpackhi((a), (b))

/* ========== Extract/Insert Operations (Unified) ========== */

/* Extract 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_simd_extract_i16(vec, index) wcn_v128i_extract_i16((vec), (index))

/* Insert 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_simd_insert_i16(vec, val, index)                                   \
  wcn_v128i_insert_i16((vec), (val), (index))

/* ========== Movemask Operations (Unified) ========== */

/* Create mask from most significant bit of each 8-bit element */
#define wcn_simd_movemask_i8(vec) wcn_v128i_movemask_i8(vec)

/* Create mask from most significant bit of each float */
#define wcn_simd_movemask_f32(vec) wcn_v128f_movemask(vec)

/* Create mask from most significant bit of each double */
#define wcn_simd_movemask_f64(vec) wcn_v128d_movemask(vec)

/* ========== Blend/Select Operations (Unified) ========== */

/* Variable blend floats (blends based on sign bit of mask) */
#define wcn_simd_blendv_f32(a, b, mask) wcn_v128f_blendv((a), (b), (mask))

/* Variable blend doubles (blends based on sign bit of mask) */
#define wcn_simd_blendv_f64(a, b, mask) wcn_v128d_blendv((a), (b), (mask))

/* Variable blend 8-bit integers */
#define wcn_simd_blendv_i8(a, b, mask) wcn_v128i_blendv_i8((a), (b), (mask))

/* ========== Byte Shuffle (Unified) ========== */

/* Shuffle bytes according to indices in control mask */
#define wcn_simd_shuffle_i8(vec, control) wcn_v128i_shuffle_i8((vec), (control))

/* ========== Common Array Algorithms ========== */

/* Vector dot product (sum of element-wise multiplication) */
WCN_API_EXPORT float wcn_simd_dot_product_f32(const float *a, const float *b,
                                              size_t count);

WCN_API_EXPORT float
wcn_simd_dot_product_kahan_f32(const float *a, const float *b, size_t count);

/* Vector addition: c[i] = a[i] + b[i] */
WCN_API_EXPORT void wcn_simd_add_array_f32(const float *a, const float *b,
                                           float *c, size_t count);

/* Vector multiplication: c[i] = a[i] * b[i] */
WCN_API_EXPORT void wcn_simd_mul_array_f32(const float *a, const float *b,
                                           float *c, size_t count);

/* Scalar multiplication: b[i] = a[i] * scalar */
WCN_API_EXPORT void wcn_simd_scale_array_f32(const float *a, float scalar,
                                             float *b, size_t count);

/* FMA: c[i] = a[i] * b[i] + c[i] */
WCN_API_EXPORT void wcn_simd_fmadd_array_f32(const float *a, const float *b,
                                             float *c, size_t count);

/* Find maximum value in array */
WCN_API_EXPORT float wcn_simd_reduce_max_f32(const float *data, size_t count);

/* Find minimum value in array */
WCN_API_EXPORT float wcn_simd_reduce_min_f32(const float *data, size_t count);

/* Sum all elements in array */
WCN_API_EXPORT float wcn_simd_reduce_sum_f32(const float *data, size_t count);

/* Memory operations */
WCN_API_EXPORT void wcn_simd_memcpy_aligned(void *dest, const void *src,
                                            size_t bytes);
WCN_API_EXPORT void wcn_simd_memset_aligned(void *dest, int value,
                                            size_t bytes);

#ifdef __cplusplus
}
#endif

#endif /* WCN_SIMD_H */