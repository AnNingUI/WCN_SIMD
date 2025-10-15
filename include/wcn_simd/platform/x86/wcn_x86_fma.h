#ifndef WCN_X86_FMA_H
#define WCN_X86_FMA_H

#include "../../wcn_types.h"

#ifdef WCN_X86_FMA

#include <immintrin.h>

/* x86 FMA (Fused Multiply-Add) Instructions
 * Provides fused multiply-add/subtract operations with higher precision
 * and performance than separate multiply and add operations.
 * 
 * FMA supports three operand forms:
 * - FMA132: a = a * b + c
 * - FMA213: a = b * a + c  
 * - FMA231: a = b * c + a
 */

/* ========== Single Precision FMA (128-bit) ========== */

/* Note: Redefine wcn_v128f_fmadd to use hardware FMA instead of SSE2 emulation */

/* a * b + c - Hardware FMA version */
static inline wcn_v128f_t __wcn_v128f_fmadd_fma(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fmadd_ps(a.raw, b.raw, c.raw);
    return result;
}
#define wcn_v128f_fmadd(a, b, c) __wcn_v128f_fmadd_fma((a), (b), (c))

/* a * b - c */
WCN_INLINE wcn_v128f_t wcn_v128f_fmsub(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fmsub_ps(a.raw, b.raw, c.raw);
    return result;
}

/* -(a * b) + c */
WCN_INLINE wcn_v128f_t wcn_v128f_fnmadd(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fnmadd_ps(a.raw, b.raw, c.raw);
    return result;
}

/* -(a * b) - c */
WCN_INLINE wcn_v128f_t wcn_v128f_fnmsub(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fnmsub_ps(a.raw, b.raw, c.raw);
    return result;
}

/* Alternate add/subtract: a * b + c[0], a * b - c[1], ... */
WCN_INLINE wcn_v128f_t wcn_v128f_fmaddsub(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fmaddsub_ps(a.raw, b.raw, c.raw);
    return result;
}

/* Alternate subtract/add: a * b - c[0], a * b + c[1], ... */
WCN_INLINE wcn_v128f_t wcn_v128f_fmsubadd(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fmsubadd_ps(a.raw, b.raw, c.raw);
    return result;
}

/* ========== Double Precision FMA (128-bit) ========== */

/* a * b + c - Hardware FMA version */
static inline wcn_v128d_t __wcn_v128d_fmadd_fma(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fmadd_pd(a.raw, b.raw, c.raw);
    return result;
}
#define wcn_v128d_fmadd(a, b, c) __wcn_v128d_fmadd_fma((a), (b), (c))

/* a * b - c */
WCN_INLINE wcn_v128d_t wcn_v128d_fmsub(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fmsub_pd(a.raw, b.raw, c.raw);
    return result;
}

/* -(a * b) + c */
WCN_INLINE wcn_v128d_t wcn_v128d_fnmadd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fnmadd_pd(a.raw, b.raw, c.raw);
    return result;
}

/* -(a * b) - c */
WCN_INLINE wcn_v128d_t wcn_v128d_fnmsub(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fnmsub_pd(a.raw, b.raw, c.raw);
    return result;
}

/* Alternate add/subtract */
WCN_INLINE wcn_v128d_t wcn_v128d_fmaddsub(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fmaddsub_pd(a.raw, b.raw, c.raw);
    return result;
}

/* Alternate subtract/add */
WCN_INLINE wcn_v128d_t wcn_v128d_fmsubadd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fmsubadd_pd(a.raw, b.raw, c.raw);
    return result;
}

/* ========== AVX 256-bit FMA (if AVX2 available) ========== */

#ifdef __AVX2__

/* Note: AVX2 already defines wcn_v256f_fmadd and wcn_v256d_fmadd with wcn_v256f_t/wcn_v256d_t types
 * We need to match those signatures or skip definition here.
 * Since AVX2 header is included after FMA, we skip FMA-specific 256-bit versions.
 * AVX2 will handle 256-bit FMA operations using its own types. */

/* Provide additional FMA variants not in AVX2 */

/* Single precision 256-bit FMA - additional variants */
WCN_INLINE wcn_v256f_t wcn_v256f_fmsub(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t c) {
    wcn_v256f_t result;
    result.raw = _mm256_fmsub_ps(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_fnmadd(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t c) {
    wcn_v256f_t result;
    result.raw = _mm256_fnmadd_ps(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_fnmsub(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t c) {
    wcn_v256f_t result;
    result.raw = _mm256_fnmsub_ps(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_fmaddsub(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t c) {
    wcn_v256f_t result;
    result.raw = _mm256_fmaddsub_ps(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_fmsubadd(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t c) {
    wcn_v256f_t result;
    result.raw = _mm256_fmsubadd_ps(a.raw, b.raw, c.raw);
    return result;
}

/* Double precision 256-bit FMA - additional variants */
WCN_INLINE wcn_v256d_t wcn_v256d_fmsub(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t c) {
    wcn_v256d_t result;
    result.raw = _mm256_fmsub_pd(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_fnmadd(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t c) {
    wcn_v256d_t result;
    result.raw = _mm256_fnmadd_pd(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_fnmsub(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t c) {
    wcn_v256d_t result;
    result.raw = _mm256_fnmsub_pd(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_fmaddsub(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t c) {
    wcn_v256d_t result;
    result.raw = _mm256_fmaddsub_pd(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_fmsubadd(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t c) {
    wcn_v256d_t result;
    result.raw = _mm256_fmsubadd_pd(a.raw, b.raw, c.raw);
    return result;
}

#endif /* __AVX2__ */

/* ========== Scalar FMA Operations ========== */

/* Scalar single precision FMA (operates on lowest element) */
WCN_INLINE wcn_v128f_t wcn_v128f_fmadd_ss(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fmadd_ss(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_fmsub_ss(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fmsub_ss(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_fnmadd_ss(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fnmadd_ss(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_fnmsub_ss(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = _mm_fnmsub_ss(a.raw, b.raw, c.raw);
    return result;
}

/* Scalar double precision FMA (operates on lowest element) */
WCN_INLINE wcn_v128d_t wcn_v128d_fmadd_sd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fmadd_sd(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_fmsub_sd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fmsub_sd(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_fnmadd_sd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fnmadd_sd(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_fnmsub_sd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = _mm_fnmsub_sd(a.raw, b.raw, c.raw);
    return result;
}

/* ========== Helper Functions ========== */

/* Dot product using FMA: result = sum(a[i] * b[i]) */
WCN_INLINE float wcn_fma_dot_product_f32(const float* a, const float* b, size_t count) {
    wcn_v128f_t sum = wcn_v128f_setzero();
    size_t i = 0;
    
    /* Process 4 elements at a time */
    for (; i + 4 <= count; i += 4) {
        wcn_v128f_t va = wcn_v128f_load(a + i);
        wcn_v128f_t vb = wcn_v128f_load(b + i);
        sum = wcn_v128f_fmadd(va, vb, sum);
    }
    
    /* Horizontal sum */
    float temp[4];
    wcn_v128f_store(temp, sum);
    float result = temp[0] + temp[1] + temp[2] + temp[3];
    
    /* Handle remaining elements */
    for (; i < count; i++) {
        result += a[i] * b[i];
    }
    
    return result;
}

/* Matrix-vector multiply using FMA */
WCN_INLINE void wcn_fma_matvec_f32(const float* matrix, const float* vec, float* result, 
                                   size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; i++) {
        result[i] = wcn_fma_dot_product_f32(&matrix[i * cols], vec, cols);
    }
}

/* Polynomial evaluation using FMA (Horner's method): a0 + x*(a1 + x*(a2 + x*a3)) */
WCN_INLINE wcn_v128f_t wcn_fma_poly_eval_f32(wcn_v128f_t x, const float* coeffs, int degree) {
    wcn_v128f_t result = wcn_v128f_set1(coeffs[degree]);
    
    for (int i = degree - 1; i >= 0; i--) {
        wcn_v128f_t coeff = wcn_v128f_set1(coeffs[i]);
        result = wcn_v128f_fmadd(result, x, coeff);
    }
    
    return result;
}

#endif /* WCN_X86_FMA */

#endif /* WCN_X86_FMA_H */
