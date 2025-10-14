#ifndef WCN_X86_SSSE3_H
#define WCN_X86_SSSE3_H

#include "../../wcn_types.h"

#ifdef WCN_X86_SSSE3

/* SSSE3 (Supplemental SSE3) adds powerful shuffle, alignment, and sign operations
 * Note: Basic operations are inherited from SSE2 and SSE3
 * This file contains SSSE3-specific enhancements */

/* ========== Shuffle Operations ========== */

/* Shuffle bytes using SSSE3 native instruction (more efficient than SSE2 fallback) */
WCN_INLINE wcn_v128i_t wcn_v128i_shuffle_i8(wcn_v128i_t vec, wcn_v128i_t control) {
    wcn_v128i_t result;
    result.raw = _mm_shuffle_epi8(vec.raw, control.raw);
    return result;
}

/* ========== Alignment Operations ========== */

/* Concatenate two 128-bit values and extract a 128-bit result shifted by imm8 bytes
   result = (b:a) >> (imm8 * 8)
   imm8 must be a compile-time constant 0-31 */
#define wcn_v128i_alignr(a, b, imm8) ({ \
    wcn_v128i_t _result; \
    _result.raw = _mm_alignr_epi8((b).raw, (a).raw, (imm8)); \
    _result; \
})

/* ========== Horizontal Arithmetic (Integer) ========== */

/* Horizontal add 16-bit integers with saturation
   dst[0] = saturate(a[0] + a[1]), dst[1] = saturate(a[2] + a[3]), ...
   dst[4] = saturate(b[0] + b[1]), dst[5] = saturate(b[2] + b[3]), ... */
WCN_INLINE wcn_v128i_t wcn_v128i_hadds_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_hadds_epi16(a.raw, b.raw);
    return result;
}

/* Horizontal add 16-bit integers (without saturation) */
WCN_INLINE wcn_v128i_t wcn_v128i_hadd_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_hadd_epi16(a.raw, b.raw);
    return result;
}

/* Horizontal add 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_hadd_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_hadd_epi32(a.raw, b.raw);
    return result;
}

/* Horizontal subtract 16-bit integers with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_hsubs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_hsubs_epi16(a.raw, b.raw);
    return result;
}

/* Horizontal subtract 16-bit integers (without saturation) */
WCN_INLINE wcn_v128i_t wcn_v128i_hsub_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_hsub_epi16(a.raw, b.raw);
    return result;
}

/* Horizontal subtract 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_hsub_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_hsub_epi32(a.raw, b.raw);
    return result;
}

/* ========== Multiply and Add ========== */

/* Multiply 16-bit integers and horizontally add adjacent pairs
   dst[0] = a[0]*b[0] + a[1]*b[1], dst[1] = a[2]*b[2] + a[3]*b[3], ... */
WCN_INLINE wcn_v128i_t wcn_v128i_maddubs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_maddubs_epi16(a.raw, b.raw);
    return result;
}

/* Multiply packed 16-bit integers, produce intermediate 32-bit integers,
   horizontally add adjacent pairs with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_madd_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_madd_epi16(a.raw, b.raw);
    return result;
}

/* Multiply 16-bit integers and horizontally add with rounding */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhrs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_mulhrs_epi16(a.raw, b.raw);
    return result;
}

/* ========== Sign Operations ========== */

/* Negate elements based on sign of control:
   if control[i] < 0: result[i] = -vec[i]
   if control[i] == 0: result[i] = 0
   if control[i] > 0: result[i] = vec[i] */

WCN_INLINE wcn_v128i_t wcn_v128i_sign_i8(wcn_v128i_t vec, wcn_v128i_t control) {
    wcn_v128i_t result;
    result.raw = _mm_sign_epi8(vec.raw, control.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sign_i16(wcn_v128i_t vec, wcn_v128i_t control) {
    wcn_v128i_t result;
    result.raw = _mm_sign_epi16(vec.raw, control.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sign_i32(wcn_v128i_t vec, wcn_v128i_t control) {
    wcn_v128i_t result;
    result.raw = _mm_sign_epi32(vec.raw, control.raw);
    return result;
}

/* ========== Absolute Value ========== */

/* Compute absolute value of packed signed integers */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i8(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_abs_epi8(vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_abs_i16(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_abs_epi16(vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_abs_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_abs_epi32(vec.raw);
    return result;
}

/* ========== Advanced Shuffle Patterns ========== */

/* Create byte shuffle mask for reversing bytes in 32-bit words */
WCN_INLINE wcn_v128i_t wcn_v128i_shuffle_mask_reverse_i32(void) {
    return wcn_v128i_set_i8(
        15, 14, 13, 12,  11, 10, 9, 8,
        7, 6, 5, 4,  3, 2, 1, 0
    );
}

/* Create byte shuffle mask for reversing bytes in 16-bit words */
WCN_INLINE wcn_v128i_t wcn_v128i_shuffle_mask_reverse_i16(void) {
    return wcn_v128i_set_i8(
        14, 15, 12, 13,  10, 11, 8, 9,
        6, 7, 4, 5,  2, 3, 0, 1
    );
}

/* ========== Common SSSE3 Algorithms ========== */

/* Sum of absolute differences (SAD) - useful for block matching */
WCN_INLINE int wcn_v128i_sad_u8(wcn_v128i_t a, wcn_v128i_t b) {
    __m128i sad = _mm_sad_epu8(a.raw, b.raw);
    return _mm_cvtsi128_si32(sad) + _mm_extract_epi16(sad, 4);
}

/* Palignr-based rotation - rotate vector right by n bytes */
#define wcn_v128i_rotate_right_bytes(vec, n) wcn_v128i_alignr((vec), (vec), (n))

/* Palignr-based rotation - rotate vector left by n bytes */
#define wcn_v128i_rotate_left_bytes(vec, n) wcn_v128i_alignr((vec), (vec), 16-(n))

/* ========== String/Memory Operations ========== */

/* Convert ASCII to lowercase using SSSE3 shuffle
   Works for A-Z characters, others pass through */
WCN_INLINE wcn_v128i_t wcn_v128i_ascii_tolower(wcn_v128i_t vec) {
    /* Check if character is in range A-Z (0x41-0x5A) */
    wcn_v128i_t upper_bound = wcn_v128i_set1_i8(0x5A);
    wcn_v128i_t lower_bound = wcn_v128i_set1_i8(0x41);
    wcn_v128i_t offset = wcn_v128i_set1_i8(0x20);

    /* Create mask for uppercase letters */
    wcn_v128i_t cmp1;
    cmp1.raw = _mm_cmpgt_epi8(vec.raw, lower_bound.raw);
    wcn_v128i_t cmp2;
    cmp2.raw = _mm_cmplt_epi8(vec.raw, upper_bound.raw);
    wcn_v128i_t mask;
    mask.raw = _mm_and_si128(cmp1.raw, cmp2.raw);

    /* Add 0x20 to uppercase letters */
    wcn_v128i_t offset_masked;
    offset_masked.raw = _mm_and_si128(mask.raw, offset.raw);

    wcn_v128i_t result;
    result.raw = _mm_add_epi8(vec.raw, offset_masked.raw);
    return result;
}

/* Horizontal reduction using hadd for 32-bit integers */
WCN_INLINE int32_t wcn_v128i_reduce_add_i32_ssse3(wcn_v128i_t vec) {
    wcn_v128i_t temp = wcn_v128i_hadd_i32(vec, vec);  // [a0+a1, a2+a3, a0+a1, a2+a3]
    temp = wcn_v128i_hadd_i32(temp, temp);            // [sum, sum, sum, sum]
    return _mm_cvtsi128_si32(temp.raw);
}

#endif /* WCN_X86_SSSE3 */

#endif /* WCN_X86_SSSE3_H */
