#ifndef WCN_X86_AVX2_H
#define WCN_X86_AVX2_H

#include "../../wcn_types.h"

#ifdef WCN_X86_AVX2

/* ========== 256-bit Load/Store Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_load(const void* ptr) {
    wcn_v256i_t result;
    result.raw = _mm256_loadu_si256((const __m256i*)ptr);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_load_aligned(const void* ptr) {
    wcn_v256i_t result;
    result.raw = _mm256_load_si256((const __m256i*)ptr);
    return result;
}

WCN_INLINE void wcn_v256i_store(void* ptr, wcn_v256i_t vec) {
    _mm256_storeu_si256((__m256i*)ptr, vec.raw);
}

WCN_INLINE void wcn_v256i_store_aligned(void* ptr, wcn_v256i_t vec) {
    _mm256_store_si256((__m256i*)ptr, vec.raw);
}

WCN_INLINE wcn_v256f_t wcn_v256f_load(const float* ptr) {
    wcn_v256f_t result;
    result.raw = _mm256_loadu_ps(ptr);
    return result;
}

WCN_INLINE void wcn_v256f_store(float* ptr, wcn_v256f_t vec) {
    _mm256_storeu_ps(ptr, vec.raw);
}

WCN_INLINE wcn_v256d_t wcn_v256d_load(const double* ptr) {
    wcn_v256d_t result;
    result.raw = _mm256_loadu_pd(ptr);
    return result;
}

WCN_INLINE void wcn_v256d_store(double* ptr, wcn_v256d_t vec) {
    _mm256_storeu_pd(ptr, vec.raw);
}

/* ========== Initialization ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_set1_i32(int32_t value) {
    wcn_v256i_t result;
    result.raw = _mm256_set1_epi32(value);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_setzero(void) {
    wcn_v256i_t result;
    result.raw = _mm256_setzero_si256();
    return result;
}

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

/* ========== Arithmetic Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_add_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_add_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_sub_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_sub_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_mullo_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_mullo_epi32(a.raw, b.raw);
    return result;
}

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

/* ========== FMA (Fused Multiply-Add) ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_fmadd(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t c) {
    wcn_v256f_t result;
    result.raw = _mm256_fmadd_ps(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_fmadd(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t c) {
    wcn_v256d_t result;
    result.raw = _mm256_fmadd_pd(a.raw, b.raw, c.raw);
    return result;
}

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_and(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_and_si256(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_or(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_or_si256(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_xor(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_xor_si256(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_andnot(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_andnot_si256(a.raw, b.raw);
    return result;
}

/* ========== Comparison Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_cmpeq_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_cmpeq_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_cmpgt_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_cmpgt_epi32(a.raw, b.raw);
    return result;
}

/* Note: Comparison with immediate requires compile-time constant */
#ifndef _MSC_VER
#define wcn_v256f_cmp(a, b, imm8) \
    ({ \
        wcn_v256f_t _result; \
        _result.raw = _mm256_cmp_ps((a).raw, (b).raw, (imm8)); \
        _result; \
    })
#else
#define WCN_CMP_PS(a, b, imm) _mm256_cmp_ps((a).raw, (b).raw, (imm))
#endif

/* Convenience comparison functions with fixed predicates */
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

/* ========== Min/Max Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_max_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_max_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_min_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_min_epi32(a.raw, b.raw);
    return result;
}

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

/* ========== Gather Operations ========== */

/* Note: Gather scale must be compile-time constant (1, 2, 4, or 8) */
#ifndef _MSC_VER
#define wcn_v256i_gather_i32(base, vindex, scale) \
    ({ \
        wcn_v256i_t _result; \
        _result.raw = _mm256_i32gather_epi32((base), (vindex).raw, (scale)); \
        _result; \
    })

#define wcn_v256f_gather(base, vindex, scale) \
    ({ \
        wcn_v256f_t _result; \
        _result.raw = _mm256_i32gather_ps((base), (vindex).raw, (scale)); \
        _result; \
    })

#define wcn_v256d_gather(base, vindex, scale) \
    ({ \
        wcn_v256d_t _result; \
        _result.raw = _mm256_i32gather_pd((base), (vindex).raw, (scale)); \
        _result; \
    })
#else
/* MSVC: Use direct macro expansion */
#define WCN_I32GATHER_EPI32(base, vindex, scale) _mm256_i32gather_epi32((base), (vindex).raw, (scale))
#define WCN_I32GATHER_PS(base, vindex, scale) _mm256_i32gather_ps((base), (vindex).raw, (scale))
#define WCN_I32GATHER_PD(base, vindex, scale) _mm256_i32gather_pd((base), (vindex).raw, (scale))
#endif

/* ========== Permute/Shuffle Operations ========== */

/* Note: Permute operations require compile-time constant */
#ifndef _MSC_VER
#define wcn_v256i_permute2x128(a, b, imm8) \
    ({ \
        wcn_v256i_t _result; \
        _result.raw = _mm256_permute2x128_si256((a).raw, (b).raw, (imm8)); \
        _result; \
    })

#define wcn_v256f_permute2f128(a, b, imm8) \
    ({ \
        wcn_v256f_t _result; \
        _result.raw = _mm256_permute2f128_ps((a).raw, (b).raw, (imm8)); \
        _result; \
    })
#else
/* MSVC: Use direct macro expansion */
#define WCN_PERMUTE2X128_SI256(a, b, imm) _mm256_permute2x128_si256((a).raw, (b).raw, (imm))
#define WCN_PERMUTE2F128_PS(a, b, imm) _mm256_permute2f128_ps((a).raw, (b).raw, (imm))
#endif

/* ========== Integer Multiplication ========== */

/* 16-bit integer multiplication */
WCN_INLINE wcn_v256i_t wcn_v256i_mullo_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_mullo_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_mulhi_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_mulhi_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_mulhi_u16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_mulhi_epu16(a.raw, b.raw);
    return result;
}

/* 32-bit unsigned multiplication to 64-bit */
WCN_INLINE wcn_v256i_t wcn_v256i_mul_u32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_mul_epu32(a.raw, b.raw);
    return result;
}

/* ========== Pack/Unpack Operations ========== */

/* Pack operations */
WCN_INLINE wcn_v256i_t wcn_v256i_packs_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_packs_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_packs_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_packs_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_packus_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_packus_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_packus_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_packus_epi32(a.raw, b.raw);
    return result;
}

/* Unpack operations */
WCN_INLINE wcn_v256i_t wcn_v256i_unpacklo_i8(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_unpacklo_epi8(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpackhi_i8(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_unpackhi_epi8(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpacklo_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_unpacklo_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpackhi_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_unpackhi_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpacklo_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_unpacklo_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpackhi_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_unpackhi_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpacklo_i64(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_unpacklo_epi64(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpackhi_i64(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = _mm256_unpackhi_epi64(a.raw, b.raw);
    return result;
}

/* Float/double unpack */
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

/* ========== Extract/Insert Operations ========== */

/* Extract 128-bit lane (index must be 0 or 1) */
#define wcn_v256i_extract_v128i(vec, index) _mm256_extracti128_si256((vec).raw, (index))
#define wcn_v256f_extract_v128f(vec, index) _mm256_extractf128_ps((vec).raw, (index))
#define wcn_v256d_extract_v128d(vec, index) _mm256_extractf128_pd((vec).raw, (index))

/* Insert 128-bit lane */
#ifndef _MSC_VER
#define wcn_v256i_insert_v128i(vec, val, index) \
    ({ \
        wcn_v256i_t _result; \
        _result.raw = _mm256_inserti128_si256((vec).raw, (val), (index)); \
        _result; \
    })

#define wcn_v256f_insert_v128f(vec, val, index) \
    ({ \
        wcn_v256f_t _result; \
        _result.raw = _mm256_insertf128_ps((vec).raw, (val), (index)); \
        _result; \
    })
#else
#define WCN_INSERTI128_SI256(vec, val, index) _mm256_inserti128_si256((vec).raw, (val), (index))
#define WCN_INSERTF128_PS(vec, val, index) _mm256_insertf128_ps((vec).raw, (val), (index))
#endif

/* ========== Movemask Operations ========== */

WCN_INLINE int wcn_v256i_movemask_i8(wcn_v256i_t vec) {
    return _mm256_movemask_epi8(vec.raw);
}

WCN_INLINE int wcn_v256f_movemask(wcn_v256f_t vec) {
    return _mm256_movemask_ps(vec.raw);
}

WCN_INLINE int wcn_v256d_movemask(wcn_v256d_t vec) {
    return _mm256_movemask_pd(vec.raw);
}

/* ========== Blend/Select Operations ========== */

/* Blend with immediate control */
#ifndef _MSC_VER
#define wcn_v256f_blend(a, b, mask) \
    ({ \
        wcn_v256f_t _result; \
        _result.raw = _mm256_blend_ps((a).raw, (b).raw, (mask)); \
        _result; \
    })

#define wcn_v256d_blend(a, b, mask) \
    ({ \
        wcn_v256d_t _result; \
        _result.raw = _mm256_blend_pd((a).raw, (b).raw, (mask)); \
        _result; \
    })

#define wcn_v256i_blend_i16(a, b, mask) \
    ({ \
        wcn_v256i_t _result; \
        _result.raw = _mm256_blend_epi16((a).raw, (b).raw, (mask)); \
        _result; \
    })

#define wcn_v256i_blend_i32(a, b, mask) \
    ({ \
        wcn_v256i_t _result; \
        _result.raw = _mm256_blend_epi32((a).raw, (b).raw, (mask)); \
        _result; \
    })
#else
#define WCN_BLEND_PS_256(a, b, mask) _mm256_blend_ps((a).raw, (b).raw, (mask))
#define WCN_BLEND_PD_256(a, b, mask) _mm256_blend_pd((a).raw, (b).raw, (mask))
#define WCN_BLEND_EPI16_256(a, b, mask) _mm256_blend_epi16((a).raw, (b).raw, (mask))
#define WCN_BLEND_EPI32_256(a, b, mask) _mm256_blend_epi32((a).raw, (b).raw, (mask))
#endif

/* Variable blend based on sign bit */
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

WCN_INLINE wcn_v256i_t wcn_v256i_blendv_i8(wcn_v256i_t a, wcn_v256i_t b, wcn_v256i_t mask) {
    wcn_v256i_t result;
    result.raw = _mm256_blendv_epi8(a.raw, b.raw, mask.raw);
    return result;
}

/* ========== Byte Shuffle ========== */

/* Shuffle bytes according to indices (SSSE3-style per-lane shuffle) */
WCN_INLINE wcn_v256i_t wcn_v256i_shuffle_i8(wcn_v256i_t vec, wcn_v256i_t control) {
    wcn_v256i_t result;
    result.raw = _mm256_shuffle_epi8(vec.raw, control.raw);
    return result;
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

#endif /* WCN_X86_AVX2 */

#endif /* WCN_X86_AVX2_H */
