#ifndef WCN_X86_SSE3_H
#define WCN_X86_SSE3_H

#include "../../wcn_types.h"

#ifdef WCN_X86_SSE3

/* SSE3 adds horizontal operations and specialized load/store on top of SSE2
 * Note: Basic operations are inherited from SSE2
 * This file contains SSE3-specific enhancements */

/* ========== Horizontal Arithmetic Operations ========== */

/* Horizontal add for floats: dst[0] = a[0]+a[1], dst[1] = a[2]+a[3],
                                dst[2] = b[0]+b[1], dst[3] = b[2]+b[3] */
WCN_INLINE wcn_v128f_t wcn_v128f_hadd(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_hadd_ps(a.raw, b.raw);
    return result;
}

/* Horizontal subtract for floats: dst[0] = a[0]-a[1], dst[1] = a[2]-a[3],
                                    dst[2] = b[0]-b[1], dst[3] = b[2]-b[3] */
WCN_INLINE wcn_v128f_t wcn_v128f_hsub(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_hsub_ps(a.raw, b.raw);
    return result;
}

/* Horizontal add for doubles: dst[0] = a[0]+a[1], dst[1] = b[0]+b[1] */
WCN_INLINE wcn_v128d_t wcn_v128d_hadd(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_hadd_pd(a.raw, b.raw);
    return result;
}

/* Horizontal subtract for doubles: dst[0] = a[0]-a[1], dst[1] = b[0]-b[1] */
WCN_INLINE wcn_v128d_t wcn_v128d_hsub(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_hsub_pd(a.raw, b.raw);
    return result;
}

/* ========== Specialized Addition/Subtraction ========== */

/* Add/Subtract packed floats: dst[0] = a[0]-b[0], dst[1] = a[1]+b[1],
                                dst[2] = a[2]-b[2], dst[3] = a[3]+b[3] */
WCN_INLINE wcn_v128f_t wcn_v128f_addsub(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_addsub_ps(a.raw, b.raw);
    return result;
}

/* Add/Subtract packed doubles: dst[0] = a[0]-b[0], dst[1] = a[1]+b[1] */
WCN_INLINE wcn_v128d_t wcn_v128d_addsub(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_addsub_pd(a.raw, b.raw);
    return result;
}

/* ========== Specialized Load Operations ========== */

/* Load 128-bit value from unaligned memory and duplicate across two 64-bit lanes
   Loads and duplicates: dst = [ptr[0], ptr[0]] */
WCN_INLINE wcn_v128d_t wcn_v128d_loaddup(const double* ptr) {
    wcn_v128d_t result;
    result.raw = _mm_loaddup_pd(ptr);
    return result;
}

/* Load 32 bytes (256 bits) into two 128-bit registers and duplicate low 64 bits
   ptr[0:127] -> dst_low, ptr[128:255] -> dst_high, with low 64-bit duplication */
WCN_INLINE void wcn_v128d_loaddup2(const double* ptr, wcn_v128d_t* dst_low, wcn_v128d_t* dst_high) {
    dst_low->raw = _mm_loaddup_pd(ptr);
    dst_high->raw = _mm_loaddup_pd(ptr + 2);
}

/* Load 128-bits from unaligned memory (more efficient than SSE2 unaligned loads) */
WCN_INLINE wcn_v128i_t wcn_v128i_lddqu(const void* ptr) {
    wcn_v128i_t result;
    result.raw = _mm_lddqu_si128((const __m128i*)ptr);
    return result;
}

/* ========== Move Operations ========== */

/* Move and duplicate high and low parts of a 128-bit value
   dst[0:63] = src[0:63], dst[64:127] = src[0:63] */
WCN_INLINE wcn_v128f_t wcn_v128f_moveldup(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_moveldup_ps(vec.raw);
    return result;
}

/* dst[0:63] = src[64:127], dst[64:127] = src[64:127] */
WCN_INLINE wcn_v128f_t wcn_v128f_movehdup(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_movehdup_ps(vec.raw);
    return result;
}

/* ========== Reduction Helpers (Common SSE3 Patterns) ========== */

/* Horizontal sum of all float elements using hadd */
WCN_INLINE float wcn_v128f_reduce_add_sse3(wcn_v128f_t vec) {
    wcn_v128f_t temp = wcn_v128f_hadd(vec, vec);  // [a0+a1, a2+a3, a0+a1, a2+a3]
    temp = wcn_v128f_hadd(temp, temp);            // [sum, sum, sum, sum]
    return _mm_cvtss_f32(temp.raw);
}

/* Horizontal sum of all double elements using hadd */
WCN_INLINE double wcn_v128d_reduce_add_sse3(wcn_v128d_t vec) {
    wcn_v128d_t temp = wcn_v128d_hadd(vec, vec);  // [a0+a1, a0+a1]
    return _mm_cvtsd_f64(temp.raw);
}

/* ========== Complex Number Operations (Common SSE3 Use Case) ========== */

/* Add/subtract for complex arithmetic
   Useful for: (a.real - b.real) and (a.imag + b.imag)
   Input: a = [real0, imag0, real1, imag1], b = [real0, imag0, real1, imag1]
   Output: [a.real0 - b.real0, a.imag0 + b.imag0, a.real1 - b.real1, a.imag1 + b.imag1] */
WCN_INLINE wcn_v128f_t wcn_v128f_complex_addsub(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_addsub(a, b);
}

/* Horizontal add for complex multiplication accumulation
   After complex multiply, hadd helps combine real and imaginary parts */
WCN_INLINE wcn_v128f_t wcn_v128f_complex_hadd(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_hadd(a, b);
}

#endif /* WCN_X86_SSE3 */

#endif /* WCN_X86_SSE3_H */
