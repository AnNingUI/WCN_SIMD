#ifndef WCN_X86_AVX_H
#define WCN_X86_AVX_H

#include "../../wcn_types.h"

#ifdef WCN_X86_AVX

/* AVX introduces 256-bit vectors and is the foundation for AVX2
 * Note: AVX primarily focuses on floating-point operations
 * Integer operations are mostly handled by SSE, full 256-bit integer support comes with AVX2 */

/* ========== 256-bit Load/Store Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_load(const float* ptr) {
    wcn_v256f_t result;
    result.raw = _mm256_loadu_ps(ptr);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_load_aligned(const float* ptr) {
    wcn_v256f_t result;
    result.raw = _mm256_load_ps(ptr);
    return result;
}

WCN_INLINE void wcn_v256f_store(float* ptr, wcn_v256f_t vec) {
    _mm256_storeu_ps(ptr, vec.raw);
}

WCN_INLINE void wcn_v256f_store_aligned(float* ptr, wcn_v256f_t vec) {
    _mm256_store_ps(ptr, vec.raw);
}

WCN_INLINE wcn_v256d_t wcn_v256d_load(const double* ptr) {
    wcn_v256d_t result;
    result.raw = _mm256_loadu_pd(ptr);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_load_aligned(const double* ptr) {
    wcn_v256d_t result;
    result.raw = _mm256_load_pd(ptr);
    return result;
}

WCN_INLINE void wcn_v256d_store(double* ptr, wcn_v256d_t vec) {
    _mm256_storeu_pd(ptr, vec.raw);
}

WCN_INLINE void wcn_v256d_store_aligned(double* ptr, wcn_v256d_t vec) {
    _mm256_store_pd(ptr, vec.raw);
}

/* ========== Initialization ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_set1(float value) {
    wcn_v256f_t result;
    result.raw = _mm256_set1_ps(value);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_setzero(void) {
    wcn_v256f_t result;
    result.raw = _mm256_setzero_ps();
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_set1(double value) {
    wcn_v256d_t result;
    result.raw = _mm256_set1_pd(value);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_setzero(void) {
    wcn_v256d_t result;
    result.raw = _mm256_setzero_pd();
    return result;
}

/* Set individual elements (high to low order) */
WCN_INLINE wcn_v256f_t wcn_v256f_set(float e7, float e6, float e5, float e4,
                                      float e3, float e2, float e1, float e0) {
    wcn_v256f_t result;
    result.raw = _mm256_set_ps(e7, e6, e5, e4, e3, e2, e1, e0);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_set(double e3, double e2, double e1, double e0) {
    wcn_v256d_t result;
    result.raw = _mm256_set_pd(e3, e2, e1, e0);
    return result;
}

/* ========== Arithmetic Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_add(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_add_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_sub(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_sub_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_mul(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_mul_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_div(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_div_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_add(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_add_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_sub(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_sub_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_mul(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_mul_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_div(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_div_pd(a.raw, b.raw);
    return result;
}

/* ========== FMA (Fused Multiply-Add) - Software Emulation ========== */

/* Float FMA: a * b + c (AVX emulation using mul + add) */
WCN_INLINE wcn_v256f_t wcn_v256f_fmadd(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t c) {
    wcn_v256f_t result;
    /* AVX doesn't have FMA, emulate with mul + add */
    __m256 prod = _mm256_mul_ps(a.raw, b.raw);
    result.raw = _mm256_add_ps(prod, c.raw);
    return result;
}

/* Double FMA: a * b + c (AVX emulation using mul + add) */
WCN_INLINE wcn_v256d_t wcn_v256d_fmadd(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t c) {
    wcn_v256d_t result;
    /* AVX doesn't have FMA, emulate with mul + add */
    __m256d prod = _mm256_mul_pd(a.raw, b.raw);
    result.raw = _mm256_add_pd(prod, c.raw);
    return result;
}

/* ========== Horizontal Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_hadd(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_hadd_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_hsub(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_hsub_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_hadd(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_hadd_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_hsub(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_hsub_pd(a.raw, b.raw);
    return result;
}

/* ========== Add/Sub Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_addsub(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_addsub_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_addsub(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_addsub_pd(a.raw, b.raw);
    return result;
}

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_and(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_and_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_or(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_or_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_xor(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_xor_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_andnot(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_andnot_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_and(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_and_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_or(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_or_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_xor(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_xor_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_andnot(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_andnot_pd(a.raw, b.raw);
    return result;
}

/* ========== Comparison Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_cmpeq(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_cmp_ps(a.raw, b.raw, _CMP_EQ_OQ);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_cmplt(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_cmp_ps(a.raw, b.raw, _CMP_LT_OQ);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_cmple(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_cmp_ps(a.raw, b.raw, _CMP_LE_OQ);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_cmpgt(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_cmp_ps(a.raw, b.raw, _CMP_GT_OQ);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_cmpeq(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_cmp_pd(a.raw, b.raw, _CMP_EQ_OQ);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_cmplt(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_cmp_pd(a.raw, b.raw, _CMP_LT_OQ);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_cmple(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_cmp_pd(a.raw, b.raw, _CMP_LE_OQ);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_cmpgt(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_cmp_pd(a.raw, b.raw, _CMP_GT_OQ);
    return result;
}

/* ========== Min/Max Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_max(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_max_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_min(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_min_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_max(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_max_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_min(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_min_pd(a.raw, b.raw);
    return result;
}

/* ========== Square Root ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_sqrt(wcn_v256f_t vec) {
    wcn_v256f_t result;
    result.raw = _mm256_sqrt_ps(vec.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_sqrt(wcn_v256d_t vec) {
    wcn_v256d_t result;
    result.raw = _mm256_sqrt_pd(vec.raw);
    return result;
}

/* Reciprocal square root (approximate) */
WCN_INLINE wcn_v256f_t wcn_v256f_rsqrt(wcn_v256f_t vec) {
    wcn_v256f_t result;
    result.raw = _mm256_rsqrt_ps(vec.raw);
    return result;
}

/* Reciprocal (approximate) */
WCN_INLINE wcn_v256f_t wcn_v256f_rcp(wcn_v256f_t vec) {
    wcn_v256f_t result;
    result.raw = _mm256_rcp_ps(vec.raw);
    return result;
}

/* ========== Rounding Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_round_nearest(wcn_v256f_t vec) {
    wcn_v256f_t result;
    result.raw = _mm256_round_ps(vec.raw, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_floor(wcn_v256f_t vec) {
    wcn_v256f_t result;
    result.raw = _mm256_floor_ps(vec.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_ceil(wcn_v256f_t vec) {
    wcn_v256f_t result;
    result.raw = _mm256_ceil_ps(vec.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_round_nearest(wcn_v256d_t vec) {
    wcn_v256d_t result;
    result.raw = _mm256_round_pd(vec.raw, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_floor(wcn_v256d_t vec) {
    wcn_v256d_t result;
    result.raw = _mm256_floor_pd(vec.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_ceil(wcn_v256d_t vec) {
    wcn_v256d_t result;
    result.raw = _mm256_ceil_pd(vec.raw);
    return result;
}

/* ========== Blend Operations ========== */

#define wcn_v256f_blend(a, b, imm8) ({ \
    wcn_v256f_t _result; \
    _result.raw = _mm256_blend_ps((a).raw, (b).raw, (imm8)); \
    _result; \
})

#define wcn_v256d_blend(a, b, imm8) ({ \
    wcn_v256d_t _result; \
    _result.raw = _mm256_blend_pd((a).raw, (b).raw, (imm8)); \
    _result; \
})

WCN_INLINE wcn_v256f_t wcn_v256f_blendv(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t mask) {
    wcn_v256f_t result;
    result.raw = _mm256_blendv_ps(a.raw, b.raw, mask.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_blendv(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t mask) {
    wcn_v256d_t result;
    result.raw = _mm256_blendv_pd(a.raw, b.raw, mask.raw);
    return result;
}

/* ========== Dot Product ========== */

#define wcn_v256f_dp(a, b, imm8) ({ \
    wcn_v256f_t _result; \
    _result.raw = _mm256_dp_ps((a).raw, (b).raw, (imm8)); \
    _result; \
})

/* ========== Permute Operations ========== */

#define wcn_v256f_permute(vec, imm8) ({ \
    wcn_v256f_t _result; \
    _result.raw = _mm256_permute_ps((vec).raw, (imm8)); \
    _result; \
})

#define wcn_v256d_permute(vec, imm8) ({ \
    wcn_v256d_t _result; \
    _result.raw = _mm256_permute_pd((vec).raw, (imm8)); \
    _result; \
})

WCN_INLINE wcn_v256f_t wcn_v256f_permutev(wcn_v256f_t vec, wcn_v256f_t control) {
    wcn_v256f_t result;
    result.raw = _mm256_permutevar_ps(vec.raw, (__m256i)control.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_permutev(wcn_v256d_t vec, wcn_v256d_t control) {
    wcn_v256d_t result;
    result.raw = _mm256_permutevar_pd(vec.raw, (__m256i)control.raw);
    return result;
}

/* ========== Shuffle Operations ========== */

#define wcn_v256f_shuffle(a, b, imm8) ({ \
    wcn_v256f_t _result; \
    _result.raw = _mm256_shuffle_ps((a).raw, (b).raw, (imm8)); \
    _result; \
})

#define wcn_v256d_shuffle(a, b, imm8) ({ \
    wcn_v256d_t _result; \
    _result.raw = _mm256_shuffle_pd((a).raw, (b).raw, (imm8)); \
    _result; \
})

/* ========== Unpack Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_unpacklo(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_unpacklo_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_unpackhi(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = _mm256_unpackhi_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_unpacklo(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_unpacklo_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_unpackhi(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = _mm256_unpackhi_pd(a.raw, b.raw);
    return result;
}

/* ========== Extract 128-bit Lanes ========== */

#define wcn_v256f_extract_v128f(vec, index) _mm256_extractf128_ps((vec).raw, (index))
#define wcn_v256d_extract_v128d(vec, index) _mm256_extractf128_pd((vec).raw, (index))

/* ========== Insert 128-bit Lanes ========== */

#define wcn_v256f_insert_v128f(vec, value, index) ({ \
    wcn_v256f_t _result; \
    _result.raw = _mm256_insertf128_ps((vec).raw, (value), (index)); \
    _result; \
})

#define wcn_v256d_insert_v128d(vec, value, index) ({ \
    wcn_v256d_t _result; \
    _result.raw = _mm256_insertf128_pd((vec).raw, (value), (index)); \
    _result; \
})

/* ========== Broadcast Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_broadcast_f128(const float* ptr) {
    wcn_v256f_t result;
    result.raw = _mm256_broadcast_ps((__m128 const*)ptr);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_broadcast_f128(const double* ptr) {
    wcn_v256d_t result;
    result.raw = _mm256_broadcast_pd((__m128d const*)ptr);
    return result;
}

/* ========== Movemask ========== */

WCN_INLINE int wcn_v256f_movemask(wcn_v256f_t vec) {
    return _mm256_movemask_ps(vec.raw);
}

WCN_INLINE int wcn_v256d_movemask(wcn_v256d_t vec) {
    return _mm256_movemask_pd(vec.raw);
}

/* ========== Test Operations ========== */

WCN_INLINE int wcn_v256f_testz(wcn_v256f_t a, wcn_v256f_t b) {
    return _mm256_testz_ps(a.raw, b.raw);
}

WCN_INLINE int wcn_v256f_testc(wcn_v256f_t a, wcn_v256f_t b) {
    return _mm256_testc_ps(a.raw, b.raw);
}

WCN_INLINE int wcn_v256d_testz(wcn_v256d_t a, wcn_v256d_t b) {
    return _mm256_testz_pd(a.raw, b.raw);
}

WCN_INLINE int wcn_v256d_testc(wcn_v256d_t a, wcn_v256d_t b) {
    return _mm256_testc_pd(a.raw, b.raw);
}

/* ========== Zero Upper 128 bits ========== */

WCN_INLINE void wcn_v256_zeroupper(void) {
    _mm256_zeroupper();
}

/* Zero all AVX state */
WCN_INLINE void wcn_v256_zeroall(void) {
    _mm256_zeroall();
}

/* ========== Conversions ========== */

WCN_INLINE wcn_v256f_t wcn_v256i_to_v256f(wcn_v256i_t vec) {
    wcn_v256f_t result;
    result.raw = _mm256_cvtepi32_ps(vec.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256f_to_v256i(wcn_v256f_t vec) {
    wcn_v256i_t result;
    result.raw = _mm256_cvtps_epi32(vec.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256f_to_v256i_trunc(wcn_v256f_t vec) {
    wcn_v256i_t result;
    result.raw = _mm256_cvttps_epi32(vec.raw);
    return result;
}

#endif /* WCN_X86_AVX */

#endif /* WCN_X86_AVX_H */
