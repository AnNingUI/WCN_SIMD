#ifndef WCN_X86_SSE4_1_H
#define WCN_X86_SSE4_1_H

#include "../../wcn_types.h"

#ifdef WCN_X86_SSE4_1

/* SSE4.1 adds blend, extended min/max, dot product, rounding, and more
 * Note: Basic operations are inherited from SSE2, SSE3, and SSSE3
 * This file contains SSE4.1-specific enhancements */

/* ========== Blend Operations ========== */

/* Blend floats using immediate 4-bit mask (compile-time constant)
   Each bit in imm8[3:0] selects between a (0) or b (1) for that element */
#define wcn_v128f_blend(a, b, imm8) ({ \
    wcn_v128f_t _result; \
    _result.raw = _mm_blend_ps((a).raw, (b).raw, (imm8)); \
    _result; \
})

/* Blend doubles using immediate 2-bit mask */
#define wcn_v128d_blend(a, b, imm8) ({ \
    wcn_v128d_t _result; \
    _result.raw = _mm_blend_pd((a).raw, (b).raw, (imm8)); \
    _result; \
})

/* Blend 16-bit integers using immediate 8-bit mask */
#define wcn_v128i_blend_i16(a, b, imm8) ({ \
    wcn_v128i_t _result; \
    _result.raw = _mm_blend_epi16((a).raw, (b).raw, (imm8)); \
    _result; \
})

/* Variable blend floats (based on sign bit of mask) */
WCN_INLINE wcn_v128f_t wcn_v128f_blendv(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t mask) {
    wcn_v128f_t result;
    result.raw = _mm_blendv_ps(a.raw, b.raw, mask.raw);
    return result;
}

/* Variable blend doubles (based on sign bit of mask) */
WCN_INLINE wcn_v128d_t wcn_v128d_blendv(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t mask) {
    wcn_v128d_t result;
    result.raw = _mm_blendv_pd(a.raw, b.raw, mask.raw);
    return result;
}

/* Variable blend 8-bit integers (based on sign bit of mask) */
WCN_INLINE wcn_v128i_t wcn_v128i_blendv_i8(wcn_v128i_t a, wcn_v128i_t b, wcn_v128i_t mask) {
    wcn_v128i_t result;
    result.raw = _mm_blendv_epi8(a.raw, b.raw, mask.raw);
    return result;
}

/* ========== Extended Min/Max Operations ========== */

/* Min/Max for different integer types */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_min_epi8(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_max_epi8(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_min_epu16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_max_epu16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_min_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_max_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_u32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_min_epu32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_u32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_max_epu32(a.raw, b.raw);
    return result;
}

/* ========== Dot Product ========== */

/* Dot product of floats with mask control
   imm8[7:4] controls which elements to multiply
   imm8[3:0] controls which result elements get the sum */
#define wcn_v128f_dp(a, b, imm8) ({ \
    wcn_v128f_t _result; \
    _result.raw = _mm_dp_ps((a).raw, (b).raw, (imm8)); \
    _result; \
})

/* Dot product of doubles */
#define wcn_v128d_dp(a, b, imm8) ({ \
    wcn_v128d_t _result; \
    _result.raw = _mm_dp_pd((a).raw, (b).raw, (imm8)); \
    _result; \
})

/* ========== Rounding Operations ========== */

/* Round floats to nearest integer */
WCN_INLINE wcn_v128f_t wcn_v128f_round_nearest(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_round_ps(vec.raw, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    return result;
}

/* Round floats toward negative infinity (floor) */
WCN_INLINE wcn_v128f_t wcn_v128f_floor(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_floor_ps(vec.raw);
    return result;
}

/* Round floats toward positive infinity (ceil) */
WCN_INLINE wcn_v128f_t wcn_v128f_ceil(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_ceil_ps(vec.raw);
    return result;
}

/* Round floats toward zero (truncate) */
WCN_INLINE wcn_v128f_t wcn_v128f_trunc(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_round_ps(vec.raw, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
    return result;
}

/* Round doubles to nearest integer */
WCN_INLINE wcn_v128d_t wcn_v128d_round_nearest(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = _mm_round_pd(vec.raw, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    return result;
}

/* Round doubles toward negative infinity (floor) */
WCN_INLINE wcn_v128d_t wcn_v128d_floor(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = _mm_floor_pd(vec.raw);
    return result;
}

/* Round doubles toward positive infinity (ceil) */
WCN_INLINE wcn_v128d_t wcn_v128d_ceil(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = _mm_ceil_pd(vec.raw);
    return result;
}

/* Round doubles toward zero (truncate) */
WCN_INLINE wcn_v128d_t wcn_v128d_trunc(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = _mm_round_pd(vec.raw, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
    return result;
}

/* ========== Extended Extract/Insert Operations ========== */

/* Extract 32-bit integer (index must be compile-time constant 0-3) */
#define wcn_v128i_extract_i32(vec, index) _mm_extract_epi32((vec).raw, (index))

/* Extract 64-bit integer (index must be compile-time constant 0-1) */
#define wcn_v128i_extract_i64(vec, index) _mm_extract_epi64((vec).raw, (index))

/* Insert 32-bit integer (index must be compile-time constant 0-3) */
#define wcn_v128i_insert_i32(vec, val, index) ({ \
    wcn_v128i_t _result; \
    _result.raw = _mm_insert_epi32((vec).raw, (val), (index)); \
    _result; \
})

/* Insert 64-bit integer (index must be compile-time constant 0-1) */
#define wcn_v128i_insert_i64(vec, val, index) ({ \
    wcn_v128i_t _result; \
    _result.raw = _mm_insert_epi64((vec).raw, (val), (index)); \
    _result; \
})

/* Extract 8-bit integer (index must be compile-time constant 0-15) */
#define wcn_v128i_extract_i8(vec, index) _mm_extract_epi8((vec).raw, (index))

/* Insert 8-bit integer (index must be compile-time constant 0-15) */
#define wcn_v128i_insert_i8(vec, val, index) ({ \
    wcn_v128i_t _result; \
    _result.raw = _mm_insert_epi8((vec).raw, (val), (index)); \
    _result; \
})

/* ========== Pack with Unsigned Saturation ========== */

/* Pack 32-bit signed integers to 16-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_packus_epi32(a.raw, b.raw);
    return result;
}

/* ========== Comparison Operations (Extended) ========== */

/* Compare 64-bit integers for equality */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmpeq_epi64(a.raw, b.raw);
    return result;
}

/* ========== Zero Extension ========== */

/* Zero-extend 8-bit integers to 16-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepu8_i16(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepu8_epi16(vec.raw);
    return result;
}

/* Sign-extend 8-bit integers to 16-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepi8_i16(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepi8_epi16(vec.raw);
    return result;
}

/* Zero-extend 8-bit integers to 32-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepu8_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepu8_epi32(vec.raw);
    return result;
}

/* Sign-extend 8-bit integers to 32-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepi8_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepi8_epi32(vec.raw);
    return result;
}

/* Zero-extend 16-bit integers to 32-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepu16_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepu16_epi32(vec.raw);
    return result;
}

/* Sign-extend 16-bit integers to 32-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepi16_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepi16_epi32(vec.raw);
    return result;
}

/* Zero-extend 16-bit integers to 64-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepu16_i64(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepu16_epi64(vec.raw);
    return result;
}

/* Sign-extend 16-bit integers to 64-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepi16_i64(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepi16_epi64(vec.raw);
    return result;
}

/* Zero-extend 32-bit integers to 64-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepu32_i64(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepu32_epi64(vec.raw);
    return result;
}

/* Sign-extend 32-bit integers to 64-bit */
WCN_INLINE wcn_v128i_t wcn_v128i_cvtepi32_i64(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtepi32_epi64(vec.raw);
    return result;
}

/* ========== Multiplication (32-bit) ========== */

/* Multiply 32-bit integers and produce low 32 bits of result */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_mullo_epi32(a.raw, b.raw);
    return result;
}

/* Multiply 32-bit signed integers and produce 64-bit results */
WCN_INLINE wcn_v128i_t wcn_v128i_mul_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_mul_epi32(a.raw, b.raw);
    return result;
}

/* ========== Test Operations ========== */

/* Test if all bits are zero: returns 1 if (a & b) == 0 */
WCN_INLINE int wcn_v128i_testz(wcn_v128i_t a, wcn_v128i_t b) {
    return _mm_testz_si128(a.raw, b.raw);
}

/* Test if all bits are one: returns 1 if (a & b) == b */
WCN_INLINE int wcn_v128i_testc(wcn_v128i_t a, wcn_v128i_t b) {
    return _mm_testc_si128(a.raw, b.raw);
}

/* Test if not all bits are zero: returns 1 if (a & b) != 0 and (a & b) != b */
WCN_INLINE int wcn_v128i_testnzc(wcn_v128i_t a, wcn_v128i_t b) {
    return _mm_testnzc_si128(a.raw, b.raw);
}

/* ========== Streaming Load ========== */

/* Load 128-bit value from memory using non-temporal hint */
WCN_INLINE wcn_v128i_t wcn_v128i_stream_load(const void* ptr) {
    wcn_v128i_t result;
    result.raw = _mm_stream_load_si128((const __m128i*)ptr);
    return result;
}

/* ========== Horizontal Min/Max ========== */

/* Find minimum value across all elements */
WCN_INLINE int8_t wcn_v128i_minpos_u16(wcn_v128i_t vec) {
    __m128i result = _mm_minpos_epu16(vec.raw);
    return _mm_extract_epi16(result, 0);
}

/* ========== Higher-level Algorithms using SSE4.1 ========== */

/* Clamping using min/max */
WCN_INLINE wcn_v128f_t wcn_v128f_clamp(wcn_v128f_t vec, wcn_v128f_t min_val, wcn_v128f_t max_val) {
    wcn_v128f_t result;
    result.raw = _mm_min_ps(_mm_max_ps(vec.raw, min_val.raw), max_val.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_clamp_i32(wcn_v128i_t vec, wcn_v128i_t min_val, wcn_v128i_t max_val) {
    return wcn_v128i_min_i32(wcn_v128i_max_i32(vec, min_val), max_val);
}

/* Select operation: return a where mask is true, b otherwise */
WCN_INLINE wcn_v128f_t wcn_v128f_select(wcn_v128f_t mask, wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_blendv(b, a, mask);
}

#endif /* WCN_X86_SSE4_1 */

#endif /* WCN_X86_SSE4_1_H */
