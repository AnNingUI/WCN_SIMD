#ifndef WCN_X86_AVX512F_H
#define WCN_X86_AVX512F_H

#include "../../wcn_types.h"

#ifdef WCN_X86_AVX512F

/* ========== 512-bit Load/Store Operations ========== */

WCN_INLINE wcn_v512i_t wcn_v512i_load(const void* ptr) {
    wcn_v512i_t result;
    result.raw = _mm512_loadu_si512((const __m512i*)ptr);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_load_aligned(const void* ptr) {
    wcn_v512i_t result;
    result.raw = _mm512_load_si512((const __m512i*)ptr);
    return result;
}

WCN_INLINE void wcn_v512i_store(void* ptr, wcn_v512i_t vec) {
    _mm512_storeu_si512((__m512i*)ptr, vec.raw);
}

WCN_INLINE void wcn_v512i_store_aligned(void* ptr, wcn_v512i_t vec) {
    _mm512_store_si512((__m512i*)ptr, vec.raw);
}

WCN_INLINE wcn_v512f_t wcn_v512f_load(const float* ptr) {
    wcn_v512f_t result;
    result.raw = _mm512_loadu_ps(ptr);
    return result;
}

WCN_INLINE void wcn_v512f_store(float* ptr, wcn_v512f_t vec) {
    _mm512_storeu_ps(ptr, vec.raw);
}

WCN_INLINE wcn_v512d_t wcn_v512d_load(const double* ptr) {
    wcn_v512d_t result;
    result.raw = _mm512_loadu_pd(ptr);
    return result;
}

WCN_INLINE void wcn_v512d_store(double* ptr, wcn_v512d_t vec) {
    _mm512_storeu_pd(ptr, vec.raw);
}

/* ========== Masked Load/Store Operations ========== */

WCN_INLINE wcn_v512i_t wcn_v512i_maskz_load(uint16_t mask, const void* ptr) {
    wcn_v512i_t result;
    result.raw = _mm512_maskz_loadu_epi32(mask, ptr);
    return result;
}

WCN_INLINE void wcn_v512i_mask_store(void* ptr, uint16_t mask, wcn_v512i_t vec) {
    _mm512_mask_storeu_epi32(ptr, mask, vec.raw);
}

/* ========== Initialization ========== */

WCN_INLINE wcn_v512i_t wcn_v512i_set1_i32(int32_t value) {
    wcn_v512i_t result;
    result.raw = _mm512_set1_epi32(value);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_setzero(void) {
    wcn_v512i_t result;
    result.raw = _mm512_setzero_si512();
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_set1(float value) {
    wcn_v512f_t result;
    result.raw = _mm512_set1_ps(value);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_setzero(void) {
    wcn_v512f_t result;
    result.raw = _mm512_setzero_ps();
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_set1(double value) {
    wcn_v512d_t result;
    result.raw = _mm512_set1_pd(value);
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_setzero(void) {
    wcn_v512d_t result;
    result.raw = _mm512_setzero_pd();
    return result;
}

/* ========== Arithmetic Operations ========== */

WCN_INLINE wcn_v512i_t wcn_v512i_add_i32(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_add_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_sub_i32(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_sub_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_mullo_i32(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_mullo_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_add(wcn_v512f_t a, wcn_v512f_t b) {
    wcn_v512f_t result;
    result.raw = _mm512_add_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_sub(wcn_v512f_t a, wcn_v512f_t b) {
    wcn_v512f_t result;
    result.raw = _mm512_sub_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_mul(wcn_v512f_t a, wcn_v512f_t b) {
    wcn_v512f_t result;
    result.raw = _mm512_mul_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_div(wcn_v512f_t a, wcn_v512f_t b) {
    wcn_v512f_t result;
    result.raw = _mm512_div_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_add(wcn_v512d_t a, wcn_v512d_t b) {
    wcn_v512d_t result;
    result.raw = _mm512_add_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_sub(wcn_v512d_t a, wcn_v512d_t b) {
    wcn_v512d_t result;
    result.raw = _mm512_sub_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_mul(wcn_v512d_t a, wcn_v512d_t b) {
    wcn_v512d_t result;
    result.raw = _mm512_mul_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_div(wcn_v512d_t a, wcn_v512d_t b) {
    wcn_v512d_t result;
    result.raw = _mm512_div_pd(a.raw, b.raw);
    return result;
}

/* ========== Masked Arithmetic Operations ========== */

WCN_INLINE wcn_v512f_t wcn_v512f_mask_add(wcn_v512f_t src, uint16_t mask, wcn_v512f_t a, wcn_v512f_t b) {
    wcn_v512f_t result;
    result.raw = _mm512_mask_add_ps(src.raw, mask, a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_maskz_add(uint16_t mask, wcn_v512f_t a, wcn_v512f_t b) {
    wcn_v512f_t result;
    result.raw = _mm512_maskz_add_ps(mask, a.raw, b.raw);
    return result;
}

/* ========== FMA (Fused Multiply-Add) ========== */

WCN_INLINE wcn_v512f_t wcn_v512f_fmadd(wcn_v512f_t a, wcn_v512f_t b, wcn_v512f_t c) {
    wcn_v512f_t result;
    result.raw = _mm512_fmadd_ps(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_fmadd(wcn_v512d_t a, wcn_v512d_t b, wcn_v512d_t c) {
    wcn_v512d_t result;
    result.raw = _mm512_fmadd_pd(a.raw, b.raw, c.raw);
    return result;
}

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v512i_t wcn_v512i_and(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_and_si512(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_or(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_or_si512(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_xor(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_xor_si512(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_andnot(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_andnot_si512(a.raw, b.raw);
    return result;
}

/* ========== Comparison Operations (return mask) ========== */

WCN_INLINE uint16_t wcn_v512i_cmpeq_i32_mask(wcn_v512i_t a, wcn_v512i_t b) {
    return _mm512_cmpeq_epi32_mask(a.raw, b.raw);
}

WCN_INLINE uint16_t wcn_v512i_cmpgt_i32_mask(wcn_v512i_t a, wcn_v512i_t b) {
    return _mm512_cmpgt_epi32_mask(a.raw, b.raw);
}

WCN_INLINE uint16_t wcn_v512f_cmpeq_mask(wcn_v512f_t a, wcn_v512f_t b) {
    return _mm512_cmp_ps_mask(a.raw, b.raw, _CMP_EQ_OQ);
}

WCN_INLINE uint16_t wcn_v512f_cmplt_mask(wcn_v512f_t a, wcn_v512f_t b) {
    return _mm512_cmp_ps_mask(a.raw, b.raw, _CMP_LT_OQ);
}

WCN_INLINE uint16_t wcn_v512f_cmple_mask(wcn_v512f_t a, wcn_v512f_t b) {
    return _mm512_cmp_ps_mask(a.raw, b.raw, _CMP_LE_OQ);
}

WCN_INLINE uint16_t wcn_v512f_cmpgt_mask(wcn_v512f_t a, wcn_v512f_t b) {
    return _mm512_cmp_ps_mask(a.raw, b.raw, _CMP_GT_OQ);
}

/* ========== Min/Max Operations ========== */

WCN_INLINE wcn_v512i_t wcn_v512i_max_i32(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_max_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_min_i32(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_min_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_max(wcn_v512f_t a, wcn_v512f_t b) {
    wcn_v512f_t result;
    result.raw = _mm512_max_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_min(wcn_v512f_t a, wcn_v512f_t b) {
    wcn_v512f_t result;
    result.raw = _mm512_min_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_max(wcn_v512d_t a, wcn_v512d_t b) {
    wcn_v512d_t result;
    result.raw = _mm512_max_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_min(wcn_v512d_t a, wcn_v512d_t b) {
    wcn_v512d_t result;
    result.raw = _mm512_min_pd(a.raw, b.raw);
    return result;
}

/* ========== Square Root ========== */

WCN_INLINE wcn_v512f_t wcn_v512f_sqrt(wcn_v512f_t vec) {
    wcn_v512f_t result;
    result.raw = _mm512_sqrt_ps(vec.raw);
    return result;
}

WCN_INLINE wcn_v512d_t wcn_v512d_sqrt(wcn_v512d_t vec) {
    wcn_v512d_t result;
    result.raw = _mm512_sqrt_pd(vec.raw);
    return result;
}

/* ========== Reduction Operations ========== */

WCN_INLINE float wcn_v512f_reduce_add(wcn_v512f_t vec) {
    return _mm512_reduce_add_ps(vec.raw);
}

WCN_INLINE float wcn_v512f_reduce_max(wcn_v512f_t vec) {
    return _mm512_reduce_max_ps(vec.raw);
}

WCN_INLINE float wcn_v512f_reduce_min(wcn_v512f_t vec) {
    return _mm512_reduce_min_ps(vec.raw);
}

WCN_INLINE int32_t wcn_v512i_reduce_add_i32(wcn_v512i_t vec) {
    return _mm512_reduce_add_epi32(vec.raw);
}

WCN_INLINE int32_t wcn_v512i_reduce_max_i32(wcn_v512i_t vec) {
    return _mm512_reduce_max_epi32(vec.raw);
}

WCN_INLINE int32_t wcn_v512i_reduce_min_i32(wcn_v512i_t vec) {
    return _mm512_reduce_min_epi32(vec.raw);
}

/* ========== Permute/Shuffle Operations ========== */

WCN_INLINE wcn_v512i_t wcn_v512i_permutexvar(wcn_v512i_t idx, wcn_v512i_t a) {
    wcn_v512i_t result;
    result.raw = _mm512_permutexvar_epi32(idx.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_permutexvar(wcn_v512i_t idx, wcn_v512f_t a) {
    wcn_v512f_t result;
    result.raw = _mm512_permutexvar_ps(idx.raw, a.raw);
    return result;
}

/* ========== Gather/Scatter Operations ========== */

WCN_INLINE wcn_v512i_t wcn_v512i_gather_i32(const void* base, wcn_v512i_t vindex, int scale) {
    wcn_v512i_t result;
    result.raw = _mm512_i32gather_epi32(vindex.raw, base, scale);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_gather(const float* base, wcn_v512i_t vindex, int scale) {
    wcn_v512f_t result;
    result.raw = _mm512_i32gather_ps(vindex.raw, base, scale);
    return result;
}

WCN_INLINE void wcn_v512i_scatter_i32(void* base, wcn_v512i_t vindex, wcn_v512i_t vec, int scale) {
    _mm512_i32scatter_epi32(base, vindex.raw, vec.raw, scale);
}

WCN_INLINE void wcn_v512f_scatter(float* base, wcn_v512i_t vindex, wcn_v512f_t vec, int scale) {
    _mm512_i32scatter_ps(base, vindex.raw, vec.raw, scale);
}

/* ========== Mask Operations ========== */

WCN_INLINE wcn_v512f_t wcn_v512f_mask_blend(uint16_t mask, wcn_v512f_t a, wcn_v512f_t b) {
    wcn_v512f_t result;
    result.raw = _mm512_mask_blend_ps(mask, a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_mask_blend(uint16_t mask, wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_mask_blend_epi32(mask, a.raw, b.raw);
    return result;
}

/* ========== Conversions ========== */

WCN_INLINE wcn_v512f_t wcn_v512i_to_v512f(wcn_v512i_t vec) {
    wcn_v512f_t result;
    result.raw = _mm512_cvtepi32_ps(vec.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512f_to_v512i(wcn_v512f_t vec) {
    wcn_v512i_t result;
    result.raw = _mm512_cvtps_epi32(vec.raw);
    return result;
}

/* ========== Integer Multiplication ========== */

#ifdef WCN_X86_AVX512BW
/* 16-bit integer multiplication (requires AVX-512BW) */
WCN_INLINE wcn_v512i_t wcn_v512i_mullo_i16(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_mullo_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_mulhi_i16(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_mulhi_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_mulhi_u16(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_mulhi_epu16(a.raw, b.raw);
    return result;
}
#endif

/* 64-bit multiplication */
#ifdef WCN_X86_AVX512DQ
WCN_INLINE wcn_v512i_t wcn_v512i_mullo_i64(wcn_v512i_t a, wcn_v512i_t b) {
    wcn_v512i_t result;
    result.raw = _mm512_mullo_epi64(a.raw, b.raw);
    return result;
}
#endif

/* ========== Extract 256-bit and 128-bit lanes ========== */

#define wcn_v512i_extract_v256i(vec, index) _mm512_extracti64x4_epi64((vec).raw, (index))
#define wcn_v512f_extract_v256f(vec, index) _mm512_extractf32x8_ps((vec).raw, (index))

#define wcn_v512i_extract_v128i(vec, index) _mm512_extracti32x4_epi32((vec).raw, (index))
#define wcn_v512f_extract_v128f(vec, index) _mm512_extractf32x4_ps((vec).raw, (index))

/* ========== Conflict Detection (AVX512CD) ========== */

#ifdef WCN_X86_AVX512CD
WCN_INLINE wcn_v512i_t wcn_v512i_conflict_i32(wcn_v512i_t vec) {
    wcn_v512i_t result;
    result.raw = _mm512_conflict_epi32(vec.raw);
    return result;
}

WCN_INLINE wcn_v512i_t wcn_v512i_lzcnt_i32(wcn_v512i_t vec) {
    wcn_v512i_t result;
    result.raw = _mm512_lzcnt_epi32(vec.raw);
    return result;
}
#endif

/* ========== Ternlog (3-input logic) ========== */

#define wcn_v512i_ternlog(a, b, c, imm) ({ \
    wcn_v512i_t _result; \
    _result.raw = _mm512_ternarylogic_epi32((a).raw, (b).raw, (c).raw, (imm)); \
    _result; \
})

/* ========== Abs (absolute value) ========== */

WCN_INLINE wcn_v512i_t wcn_v512i_abs_i32(wcn_v512i_t vec) {
    wcn_v512i_t result;
    result.raw = _mm512_abs_epi32(vec.raw);
    return result;
}

WCN_INLINE wcn_v512f_t wcn_v512f_abs(wcn_v512f_t vec) {
    wcn_v512f_t result;
    result.raw = _mm512_abs_ps(vec.raw);
    return result;
}

#endif /* WCN_X86_AVX512F */

#endif /* WCN_X86_AVX512F_H */
