#ifndef WCN_X86_SSE2_H
#define WCN_X86_SSE2_H

#include "../../wcn_types.h"

#ifdef WCN_X86_SSE2

/* ========== Load/Store Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_load(const void* ptr) {
    wcn_v128i_t result;
    result.raw = _mm_loadu_si128((const __m128i*)ptr);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_load_aligned(const void* ptr) {
    wcn_v128i_t result;
    result.raw = _mm_load_si128((const __m128i*)ptr);
    return result;
}

WCN_INLINE void wcn_v128i_store(void* ptr, wcn_v128i_t vec) {
    _mm_storeu_si128((__m128i*)ptr, vec.raw);
}

WCN_INLINE void wcn_v128i_store_aligned(void* ptr, wcn_v128i_t vec) {
    _mm_store_si128((__m128i*)ptr, vec.raw);
}

WCN_INLINE wcn_v128f_t wcn_v128f_load(const float* ptr) {
    wcn_v128f_t result;
    result.raw = _mm_loadu_ps(ptr);
    return result;
}

WCN_INLINE void wcn_v128f_store(float* ptr, wcn_v128f_t vec) {
    _mm_storeu_ps(ptr, vec.raw);
}

WCN_INLINE wcn_v128d_t wcn_v128d_load(const double* ptr) {
    wcn_v128d_t result;
    result.raw = _mm_loadu_pd(ptr);
    return result;
}

WCN_INLINE void wcn_v128d_store(double* ptr, wcn_v128d_t vec) {
    _mm_storeu_pd(ptr, vec.raw);
}

/* ========== Initialization ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i8(int8_t value) {
    wcn_v128i_t result;
    result.raw = _mm_set1_epi8(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i32(int32_t value) {
    wcn_v128i_t result;
    result.raw = _mm_set1_epi32(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set_i8(int8_t e15, int8_t e14, int8_t e13, int8_t e12,
                                         int8_t e11, int8_t e10, int8_t e9, int8_t e8,
                                         int8_t e7, int8_t e6, int8_t e5, int8_t e4,
                                         int8_t e3, int8_t e2, int8_t e1, int8_t e0) {
    wcn_v128i_t result;
    result.raw = _mm_set_epi8(e15, e14, e13, e12, e11, e10, e9, e8,
                                e7, e6, e5, e4, e3, e2, e1, e0);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set_i32(int32_t v3, int32_t v2, int32_t v1, int32_t v0) {
    wcn_v128i_t result;
    result.raw = _mm_set_epi32(v3, v2, v1, v0);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_setzero(void) {
    wcn_v128i_t result;
    result.raw = _mm_setzero_si128();
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_set1(float value) {
    wcn_v128f_t result;
    result.raw = _mm_set1_ps(value);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_setzero(void) {
    wcn_v128f_t result;
    result.raw = _mm_setzero_ps();
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_set1(double value) {
    wcn_v128d_t result;
    result.raw = _mm_set1_pd(value);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_setzero(void) {
    wcn_v128d_t result;
    result.raw = _mm_setzero_pd();
    return result;
}

/* ========== Arithmetic Operations ========== */

/* 8-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_add_epi8(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_sub_epi8(a.raw, b.raw);
    return result;
}

/* 16-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_add_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_sub_epi16(a.raw, b.raw);
    return result;
}

/* 32-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_add_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_sub_epi32(a.raw, b.raw);
    return result;
}

/* 64-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_add_epi64(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_sub_epi64(a.raw, b.raw);
    return result;
}

/* ========== Saturating Arithmetic ========== */

/* 8-bit signed saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_adds_epi8(a.raw, b.raw);
    return result;
}

/* 8-bit signed saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_subs_epi8(a.raw, b.raw);
    return result;
}

/* 8-bit unsigned saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_adds_epu8(a.raw, b.raw);
    return result;
}

/* 8-bit unsigned saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_subs_epu8(a.raw, b.raw);
    return result;
}

/* 16-bit signed saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_adds_epi16(a.raw, b.raw);
    return result;
}

/* 16-bit signed saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_subs_epi16(a.raw, b.raw);
    return result;
}

/* 16-bit unsigned saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_adds_epu16(a.raw, b.raw);
    return result;
}

/* 16-bit unsigned saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_subs_epu16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_add(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_add_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_sub(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_sub_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_mul(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_mul_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_div(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_div_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_add(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_add_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sub(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_sub_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_mul(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_mul_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_div(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_div_pd(a.raw, b.raw);
    return result;
}

/* ========== FMA (Fused Multiply-Add) - Software Emulation ========== */

/* Float FMA: a * b + c (SSE2 emulation using mul + add) */
WCN_INLINE wcn_v128f_t wcn_v128f_fmadd(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    /* SSE2 doesn't have FMA, emulate with mul + add */
    __m128 prod = _mm_mul_ps(a.raw, b.raw);
    result.raw = _mm_add_ps(prod, c.raw);
    return result;
}

/* Double FMA: a * b + c (SSE2 emulation using mul + add) */
WCN_INLINE wcn_v128d_t wcn_v128d_fmadd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    /* SSE2 doesn't have FMA, emulate with mul + add */
    __m128d prod = _mm_mul_pd(a.raw, b.raw);
    result.raw = _mm_add_pd(prod, c.raw);
    return result;
}

/* ========== Rounding Operations - Software Emulation ========== */
/* SSE2 doesn't have native rounding, emulate using conversion */

#if !defined(WCN_X86_SSE4_1)
/* Round floats toward negative infinity (floor) - software emulation */
WCN_INLINE wcn_v128f_t wcn_v128f_floor(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Convert to int32 (truncates), then check if original was negative and had fraction */
    __m128i truncated = _mm_cvttps_epi32(vec.raw);
    __m128 truncated_float = _mm_cvtepi32_ps(truncated);

    /* If vec < truncated_float, we need to subtract 1 */
    __m128 needs_adjust = _mm_cmplt_ps(vec.raw, truncated_float);
    __m128 adjustment = _mm_and_ps(needs_adjust, _mm_set1_ps(1.0f));

    result.raw = _mm_sub_ps(truncated_float, adjustment);
    return result;
}

/* Round floats toward positive infinity (ceil) - software emulation */
WCN_INLINE wcn_v128f_t wcn_v128f_ceil(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Convert to int32 (truncates), then check if original was positive and had fraction */
    __m128i truncated = _mm_cvttps_epi32(vec.raw);
    __m128 truncated_float = _mm_cvtepi32_ps(truncated);

    /* If vec > truncated_float, we need to add 1 */
    __m128 needs_adjust = _mm_cmpgt_ps(vec.raw, truncated_float);
    __m128 adjustment = _mm_and_ps(needs_adjust, _mm_set1_ps(1.0f));

    result.raw = _mm_add_ps(truncated_float, adjustment);
    return result;
}

/* Round floats to nearest integer - software emulation */
WCN_INLINE wcn_v128f_t wcn_v128f_round_nearest(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Add/subtract 0.5 depending on sign, then truncate */
    __m128 half = _mm_set1_ps(0.5f);
    __m128 sign_mask = _mm_set1_ps(-0.0f);

    /* Check if negative */
    __m128 is_neg = _mm_and_ps(vec.raw, sign_mask);
    __m128 half_signed = _mm_or_ps(half, is_neg);

    /* Add signed 0.5 and truncate */
    __m128 adjusted = _mm_add_ps(vec.raw, half_signed);
    __m128i truncated = _mm_cvttps_epi32(adjusted);
    result.raw = _mm_cvtepi32_ps(truncated);
    return result;
}

/* Round floats toward zero (truncate) - software emulation */
WCN_INLINE wcn_v128f_t wcn_v128f_trunc(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Simple: convert to int32 and back (truncates toward zero) */
    __m128i truncated = _mm_cvttps_epi32(vec.raw);
    result.raw = _mm_cvtepi32_ps(truncated);
    return result;
}

/* Round doubles toward negative infinity (floor) - software emulation */
WCN_INLINE wcn_v128d_t wcn_v128d_floor(wcn_v128d_t vec) {
    wcn_v128d_t result;
    /* Convert to int32 (truncates), then check if original was negative and had fraction */
    __m128i truncated = _mm_cvttpd_epi32(vec.raw);
    __m128d truncated_double = _mm_cvtepi32_pd(truncated);

    /* If vec < truncated_double, we need to subtract 1 */
    __m128d needs_adjust = _mm_cmplt_pd(vec.raw, truncated_double);
    __m128d adjustment = _mm_and_pd(needs_adjust, _mm_set1_pd(1.0));

    result.raw = _mm_sub_pd(truncated_double, adjustment);
    return result;
}

/* Round doubles toward positive infinity (ceil) - software emulation */
WCN_INLINE wcn_v128d_t wcn_v128d_ceil(wcn_v128d_t vec) {
    wcn_v128d_t result;
    /* Convert to int32 (truncates), then check if original was positive and had fraction */
    __m128i truncated = _mm_cvttpd_epi32(vec.raw);
    __m128d truncated_double = _mm_cvtepi32_pd(truncated);

    /* If vec > truncated_double, we need to add 1 */
    __m128d needs_adjust = _mm_cmpgt_pd(vec.raw, truncated_double);
    __m128d adjustment = _mm_and_pd(needs_adjust, _mm_set1_pd(1.0));

    result.raw = _mm_add_pd(truncated_double, adjustment);
    return result;
}

/* Round doubles to nearest integer - software emulation */
WCN_INLINE wcn_v128d_t wcn_v128d_round_nearest(wcn_v128d_t vec) {
    wcn_v128d_t result;
    /* Add/subtract 0.5 depending on sign, then truncate */
    __m128d half = _mm_set1_pd(0.5);
    __m128d sign_mask = _mm_set1_pd(-0.0);

    /* Check if negative */
    __m128d is_neg = _mm_and_pd(vec.raw, sign_mask);
    __m128d half_signed = _mm_or_pd(half, is_neg);

    /* Add signed 0.5 and truncate */
    __m128d adjusted = _mm_add_pd(vec.raw, half_signed);
    __m128i truncated = _mm_cvttpd_epi32(adjusted);
    result.raw = _mm_cvtepi32_pd(truncated);
    return result;
}

/* Round doubles toward zero (truncate) - software emulation */
WCN_INLINE wcn_v128d_t wcn_v128d_trunc(wcn_v128d_t vec) {
    wcn_v128d_t result;
    /* Simple: convert to int32 and back (truncates toward zero) */
    __m128i truncated = _mm_cvttpd_epi32(vec.raw);
    result.raw = _mm_cvtepi32_pd(truncated);
    return result;
}
#endif

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_and(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_and_si128(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_or(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_or_si128(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_xor(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_xor_si128(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_andnot(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_andnot_si128(a.raw, b.raw);
    return result;
}

/* ========== Shift Operations ========== */

/* Logical shift left (16-bit) */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = _mm_sll_epi16(a.raw, count.raw);
    return result;
}

/* Logical shift left immediate (16-bit) */
#define wcn_v128i_slli_i16(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_slli_epi16((a).raw, (imm)); \
        _result; \
    })

/* Logical shift right (16-bit) */
WCN_INLINE wcn_v128i_t wcn_v128i_srl_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = _mm_srl_epi16(a.raw, count.raw);
    return result;
}

/* Logical shift right immediate (16-bit) */
#define wcn_v128i_srli_i16(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_srli_epi16((a).raw, (imm)); \
        _result; \
    })

/* Arithmetic shift right (16-bit) */
WCN_INLINE wcn_v128i_t wcn_v128i_sra_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = _mm_sra_epi16(a.raw, count.raw);
    return result;
}

/* Arithmetic shift right immediate (16-bit) */
#define wcn_v128i_srai_i16(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_srai_epi16((a).raw, (imm)); \
        _result; \
    })

/* Logical shift left (32-bit) */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = _mm_sll_epi32(a.raw, count.raw);
    return result;
}

/* Logical shift left immediate (32-bit) */
#define wcn_v128i_slli_i32(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_slli_epi32((a).raw, (imm)); \
        _result; \
    })

/* Logical shift right (32-bit) */
WCN_INLINE wcn_v128i_t wcn_v128i_srl_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = _mm_srl_epi32(a.raw, count.raw);
    return result;
}

/* Logical shift right immediate (32-bit) */
#define wcn_v128i_srli_i32(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_srli_epi32((a).raw, (imm)); \
        _result; \
    })

/* Arithmetic shift right (32-bit) */
WCN_INLINE wcn_v128i_t wcn_v128i_sra_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = _mm_sra_epi32(a.raw, count.raw);
    return result;
}

/* Arithmetic shift right immediate (32-bit) */
#define wcn_v128i_srai_i32(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_srai_epi32((a).raw, (imm)); \
        _result; \
    })

/* Logical shift left (64-bit) */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = _mm_sll_epi64(a.raw, count.raw);
    return result;
}

/* Logical shift left immediate (64-bit) */
#define wcn_v128i_slli_i64(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_slli_epi64((a).raw, (imm)); \
        _result; \
    })

/* Logical shift right (64-bit) */
WCN_INLINE wcn_v128i_t wcn_v128i_srl_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = _mm_srl_epi64(a.raw, count.raw);
    return result;
}

/* Logical shift right immediate (64-bit) */
#define wcn_v128i_srli_i64(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_srli_epi64((a).raw, (imm)); \
        _result; \
    })

/* Arithmetic shift right (64-bit) - SSE2 doesn't have native instruction, emulate with sign extension */
WCN_INLINE wcn_v128i_t wcn_v128i_sra_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    /* Extract shift amount from count (only use lower 64 bits) */
    int shift_amount = _mm_cvtsi128_si32(count.raw);

    if (shift_amount >= 64) {
        /* If shift >= 64, replicate sign bit */
        __m128i signs = _mm_srai_epi32(a.raw, 31);
        result.raw = _mm_shuffle_epi32(signs, _MM_SHUFFLE(3, 3, 1, 1));
    } else if (shift_amount == 0) {
        result.raw = a.raw;
    } else {
        /* Logical right shift */
        __m128i shifted = _mm_srl_epi64(a.raw, count.raw);

        /* Create sign extension mask */
        __m128i signs = _mm_srai_epi32(a.raw, 31);
        __m128i sign_ext = _mm_shuffle_epi32(signs, _MM_SHUFFLE(3, 3, 1, 1));

        /* Create mask for sign bits: (0xFFFFFFFFFFFFFFFF << (64 - shift)) */
        __m128i mask = _mm_sll_epi64(_mm_set1_epi64x(0xFFFFFFFFFFFFFFFFULL),
                                      _mm_cvtsi32_si128(64 - shift_amount));
        mask = _mm_and_si128(mask, sign_ext);

        /* Combine shifted value with sign extension */
        result.raw = _mm_or_si128(shifted, mask);
    }

    return result;
}

/* Arithmetic shift right immediate (64-bit) - SSE2 emulation */
#define wcn_v128i_srai_i64(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        if ((imm) >= 64) { \
            /* If shift >= 64, replicate sign bit */ \
            __m128i _signs = _mm_srai_epi32((a).raw, 31); \
            _result.raw = _mm_shuffle_epi32(_signs, _MM_SHUFFLE(3, 3, 1, 1)); \
        } else if ((imm) == 0) { \
            _result.raw = (a).raw; \
        } else { \
            /* Logical right shift */ \
            __m128i _shifted = _mm_srli_epi64((a).raw, (imm)); \
            /* Create sign extension mask */ \
            __m128i _signs = _mm_srai_epi32((a).raw, 31); \
            __m128i _sign_ext = _mm_shuffle_epi32(_signs, _MM_SHUFFLE(3, 3, 1, 1)); \
            /* Create mask for sign bits */ \
            __m128i _mask = _mm_slli_epi64(_mm_set1_epi64x(0xFFFFFFFFFFFFFFFFULL), 64 - (imm)); \
            _mask = _mm_and_si128(_mask, _sign_ext); \
            /* Combine shifted value with sign extension */ \
            _result.raw = _mm_or_si128(_shifted, _mask); \
        } \
        _result; \
    })

/* Byte shift left (128-bit) */
#define wcn_v128i_slli_si128(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_slli_si128((a).raw, (imm)); \
        _result; \
    })

/* Byte shift right (128-bit) */
#define wcn_v128i_srli_si128(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_srli_si128((a).raw, (imm)); \
        _result; \
    })

#ifdef _MSC_VER
/* MSVC versions */
#undef wcn_v128i_slli_i16
#undef wcn_v128i_srli_i16
#undef wcn_v128i_srai_i16
#undef wcn_v128i_slli_i32
#undef wcn_v128i_srli_i32
#undef wcn_v128i_srai_i32
#undef wcn_v128i_slli_i64
#undef wcn_v128i_srli_i64
#undef wcn_v128i_srai_i64
#undef wcn_v128i_slli_si128
#undef wcn_v128i_srli_si128

#define WCN_SLLI_EPI16(a, imm) _mm_slli_epi16((a).raw, (imm))
#define WCN_SRLI_EPI16(a, imm) _mm_srli_epi16((a).raw, (imm))
#define WCN_SRAI_EPI16(a, imm) _mm_srai_epi16((a).raw, (imm))
#define WCN_SLLI_EPI32(a, imm) _mm_slli_epi32((a).raw, (imm))
#define WCN_SRLI_EPI32(a, imm) _mm_srli_epi32((a).raw, (imm))
#define WCN_SRAI_EPI32(a, imm) _mm_srai_epi32((a).raw, (imm))
#define WCN_SLLI_EPI64(a, imm) _mm_slli_epi64((a).raw, (imm))
#define WCN_SRLI_EPI64(a, imm) _mm_srli_epi64((a).raw, (imm))
#define WCN_SRAI_EPI64(a, imm) wcn_v128i_srai_i64((a), (imm))
#define WCN_SLLI_SI128(a, imm) _mm_slli_si128((a).raw, (imm))
#define WCN_SRLI_SI128(a, imm) _mm_srli_si128((a).raw, (imm))
#endif

/* ========== Comparison Operations ========== */

/* 8-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmpeq_epi8(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmpgt_epi8(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmplt_epi8(a.raw, b.raw);
    return result;
}

/* 16-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmpeq_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmpgt_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmplt_epi16(a.raw, b.raw);
    return result;
}

/* 32-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmpeq_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmpgt_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmplt_epi32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpeq(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_cmpeq_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmplt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_cmplt_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmple(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_cmple_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpgt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_cmpgt_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpge(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_cmpge_ps(a.raw, b.raw);
    return result;
}

/* ========== Shuffle/Permute Operations ========== */

/* Note: Shuffle operations require compile-time constant, use macros instead */
#define wcn_v128i_shuffle_i32(a, imm8) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_shuffle_epi32((a).raw, (imm8)); \
        _result; \
    })

#define wcn_v128f_shuffle(a, b, imm8) \
    ({ \
        wcn_v128f_t _result; \
        _result.raw = _mm_shuffle_ps((a).raw, (b).raw, (imm8)); \
        _result; \
    })

/* MSVC doesn't support statement expressions, provide inline functions with template-like approach */
#ifdef _MSC_VER
#undef wcn_v128i_shuffle_i32
#undef wcn_v128f_shuffle

/* Use these as template-like helper, call with literal constant */
#define WCN_SHUFFLE_EPI32(vec, imm) _mm_shuffle_epi32((vec).raw, (imm))
#define WCN_SHUFFLE_PS(a, b, imm) _mm_shuffle_ps((a).raw, (b).raw, (imm))
#endif

/* ========== Reduction Operations ========== */

WCN_INLINE float wcn_v128f_reduce_add(wcn_v128f_t vec) {
    __m128 temp = _mm_add_ps(vec.raw, _mm_movehl_ps(vec.raw, vec.raw));
    temp = _mm_add_ss(temp, _mm_shuffle_ps(temp, temp, 1));
    return _mm_cvtss_f32(temp);
}

WCN_INLINE float wcn_v128f_reduce_max(wcn_v128f_t vec) {
    __m128 temp = _mm_max_ps(vec.raw, _mm_movehl_ps(vec.raw, vec.raw));
    temp = _mm_max_ps(temp, _mm_shuffle_ps(temp, temp, 1));
    return _mm_cvtss_f32(temp);
}

WCN_INLINE float wcn_v128f_reduce_min(wcn_v128f_t vec) {
    __m128 temp = _mm_min_ps(vec.raw, _mm_movehl_ps(vec.raw, vec.raw));
    temp = _mm_min_ps(temp, _mm_shuffle_ps(temp, temp, 1));
    return _mm_cvtss_f32(temp);
}

/* ========== Min/Max Operations ========== */

/* 8-bit unsigned min/max (SSE2 native) */
WCN_INLINE wcn_v128i_t wcn_v128i_min_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_min_epu8(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_max_epu8(a.raw, b.raw);
    return result;
}

/* 8-bit signed min/max (SSE2 fallback, SSE4.1 has native) */
#if !defined(WCN_X86_SSE4_1)
WCN_INLINE wcn_v128i_t wcn_v128i_min_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* SSE2 fallback: convert to signed comparison */
    __m128i mask = _mm_cmpgt_epi8(a.raw, b.raw);
    result.raw = _mm_or_si128(_mm_and_si128(mask, b.raw),
                               _mm_andnot_si128(mask, a.raw));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* SSE2 fallback: convert to signed comparison */
    __m128i mask = _mm_cmpgt_epi8(a.raw, b.raw);
    result.raw = _mm_or_si128(_mm_and_si128(mask, a.raw),
                               _mm_andnot_si128(mask, b.raw));
    return result;
}
#endif

/* 16-bit signed min/max (SSE2 native) */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_min_epi16(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_max_epi16(a.raw, b.raw);
    return result;
}

/* 32-bit signed min/max (SSE4.1, fallback for SSE2) */
#if !defined(WCN_X86_SSE4_1)
WCN_INLINE wcn_v128i_t wcn_v128i_min_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* SSE2 fallback using comparison and blend */
    __m128i mask = _mm_cmpgt_epi32(a.raw, b.raw);
    result.raw = _mm_or_si128(_mm_and_si128(mask, b.raw),
                               _mm_andnot_si128(mask, a.raw));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* SSE2 fallback using comparison and blend */
    __m128i mask = _mm_cmpgt_epi32(a.raw, b.raw);
    result.raw = _mm_or_si128(_mm_and_si128(mask, a.raw),
                               _mm_andnot_si128(mask, b.raw));
    return result;
}
#endif

/* Float min/max (SSE2 native) */
WCN_INLINE wcn_v128f_t wcn_v128f_max(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_max_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_min(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_min_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_max(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_max_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_min(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_min_pd(a.raw, b.raw);
    return result;
}

/* ========== Square Root ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_sqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_sqrt_ps(vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sqrt(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = _mm_sqrt_pd(vec.raw);
    return result;
}

/* ========== Reciprocal and Reciprocal Square Root (Approximations) ========== */

/* Reciprocal approximation (1/x) for packed single-precision floats */
WCN_INLINE wcn_v128f_t wcn_v128f_rcp(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_rcp_ps(vec.raw);
    return result;
}

/* Reciprocal square root approximation (1/sqrt(x)) for packed single-precision floats */
WCN_INLINE wcn_v128f_t wcn_v128f_rsqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_rsqrt_ps(vec.raw);
    return result;
}

/* ========== Absolute Value Operations ========== */

/* 8-bit signed integer absolute value (SSE2 fallback, SSSE3 has native) */
#if !defined(WCN_X86_SSSE3)
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i8(wcn_v128i_t a) {
    wcn_v128i_t result;
    /* SSE2 fallback: abs(x) = max(x, -x) */
    __m128i zero = _mm_setzero_si128();
    __m128i neg = _mm_sub_epi8(zero, a.raw);
    result.raw = _mm_max_epu8(a.raw, neg);  /* Treat as unsigned for max */
    return result;
}
#endif

/* 16-bit signed integer absolute value (SSE2 fallback, SSSE3 has native) */
#if !defined(WCN_X86_SSSE3)
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i16(wcn_v128i_t a) {
    wcn_v128i_t result;
    /* SSE2 fallback: abs(x) = (x XOR mask) - mask, where mask = x >> 15 */
    __m128i mask = _mm_srai_epi16(a.raw, 15);
    result.raw = _mm_sub_epi16(_mm_xor_si128(a.raw, mask), mask);
    return result;
}
#endif

/* 32-bit signed integer absolute value (SSE2 fallback, SSSE3 has native) */
#if !defined(WCN_X86_SSSE3)
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i32(wcn_v128i_t a) {
    wcn_v128i_t result;
    /* SSE2 fallback: abs(x) = (x XOR mask) - mask, where mask = x >> 31 */
    __m128i mask = _mm_srai_epi32(a.raw, 31);
    result.raw = _mm_sub_epi32(_mm_xor_si128(a.raw, mask), mask);
    return result;
}
#endif

/* Float absolute value - clear sign bit */
WCN_INLINE wcn_v128f_t wcn_v128f_abs(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Create mask with all bits set except sign bit (0x7FFFFFFF) */
    __m128i mask = _mm_set1_epi32(0x7FFFFFFF);
    result.raw = _mm_and_ps(vec.raw, _mm_castsi128_ps(mask));
    return result;
}

/* Double absolute value - clear sign bit */
WCN_INLINE wcn_v128d_t wcn_v128d_abs(wcn_v128d_t vec) {
    wcn_v128d_t result;
    /* Create mask with all bits set except sign bit */
    __m128i mask = _mm_set1_epi32(0xFFFFFFFF);
    __m128i mask_low = _mm_unpacklo_epi32(mask, _mm_set1_epi32(0x7FFFFFFF));
    __m128i mask_high = _mm_unpackhi_epi32(mask, _mm_set1_epi32(0x7FFFFFFF));
    __m128i full_mask = _mm_unpacklo_epi64(mask_low, mask_high);
    result.raw = _mm_and_pd(vec.raw, _mm_castsi128_pd(full_mask));
    return result;
}

/* ========== Negation Operations ========== */

/* 8-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i8(wcn_v128i_t a) {
    wcn_v128i_t result;
    __m128i zero = _mm_setzero_si128();
    result.raw = _mm_sub_epi8(zero, a.raw);
    return result;
}

/* 16-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i16(wcn_v128i_t a) {
    wcn_v128i_t result;
    __m128i zero = _mm_setzero_si128();
    result.raw = _mm_sub_epi16(zero, a.raw);
    return result;
}

/* 32-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i32(wcn_v128i_t a) {
    wcn_v128i_t result;
    __m128i zero = _mm_setzero_si128();
    result.raw = _mm_sub_epi32(zero, a.raw);
    return result;
}

/* 64-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i64(wcn_v128i_t a) {
    wcn_v128i_t result;
    __m128i zero = _mm_setzero_si128();
    result.raw = _mm_sub_epi64(zero, a.raw);
    return result;
}

/* Float negation - flip sign bit */
WCN_INLINE wcn_v128f_t wcn_v128f_neg(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* XOR with sign bit mask (0x80000000) */
    __m128i sign_mask = _mm_set1_epi32(0x80000000);
    result.raw = _mm_xor_ps(vec.raw, _mm_castsi128_ps(sign_mask));
    return result;
}

/* Double negation - flip sign bit */
WCN_INLINE wcn_v128d_t wcn_v128d_neg(wcn_v128d_t vec) {
    wcn_v128d_t result;
    /* XOR with sign bit mask */
    __m128i sign_mask_low = _mm_set_epi32(0x80000000, 0, 0x80000000, 0);
    result.raw = _mm_xor_pd(vec.raw, _mm_castsi128_pd(sign_mask_low));
    return result;
}

/* ========== Additional Set1 Operations ========== */

/* Set all 16-bit lanes to the same value */
WCN_INLINE wcn_v128i_t wcn_v128i_set1_i16(int16_t value) {
    wcn_v128i_t result;
    result.raw = _mm_set1_epi16(value);
    return result;
}

/* Set all 64-bit lanes to the same value */
WCN_INLINE wcn_v128i_t wcn_v128i_set1_i64(int64_t value) {
    wcn_v128i_t result;
    result.raw = _mm_set1_epi64x(value);
    return result;
}

/* ========== Additional Double Comparison Operations ========== */

/* Double equal comparison */
WCN_INLINE wcn_v128d_t wcn_v128d_cmpeq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_cmpeq_pd(a.raw, b.raw);
    return result;
}

/* Double less than comparison */
WCN_INLINE wcn_v128d_t wcn_v128d_cmplt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_cmplt_pd(a.raw, b.raw);
    return result;
}

/* Double less than or equal comparison */
WCN_INLINE wcn_v128d_t wcn_v128d_cmple(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_cmple_pd(a.raw, b.raw);
    return result;
}

/* Double greater than comparison */
WCN_INLINE wcn_v128d_t wcn_v128d_cmpgt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_cmpgt_pd(a.raw, b.raw);
    return result;
}

/* Double greater than or equal comparison */
WCN_INLINE wcn_v128d_t wcn_v128d_cmpge(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_cmpge_pd(a.raw, b.raw);
    return result;
}

/* Double not equal comparison */
WCN_INLINE wcn_v128d_t wcn_v128d_cmpneq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_cmpneq_pd(a.raw, b.raw);
    return result;
}

/* ========== Aligned Float/Double Load/Store ========== */

/* Load aligned float vector from memory */
WCN_INLINE wcn_v128f_t wcn_v128f_load_aligned(const float* ptr) {
    wcn_v128f_t result;
    result.raw = _mm_load_ps(ptr);
    return result;
}

/* Store aligned float vector to memory */
WCN_INLINE void wcn_v128f_store_aligned(float* ptr, wcn_v128f_t vec) {
    _mm_store_ps(ptr, vec.raw);
}

/* Load aligned double vector from memory */
WCN_INLINE wcn_v128d_t wcn_v128d_load_aligned(const double* ptr) {
    wcn_v128d_t result;
    result.raw = _mm_load_pd(ptr);
    return result;
}

/* Store aligned double vector to memory */
WCN_INLINE void wcn_v128d_store_aligned(double* ptr, wcn_v128d_t vec) {
    _mm_store_pd(ptr, vec.raw);
}

/* ========== Broadcast Operations ========== */

/* Load scalar and broadcast to all 32-bit integer lanes */
WCN_INLINE wcn_v128i_t wcn_v128i_broadcast_i32(const int32_t* ptr) {
    wcn_v128i_t result;
    result.raw = _mm_set1_epi32(*ptr);
    return result;
}

/* Load scalar and broadcast to all float lanes */
WCN_INLINE wcn_v128f_t wcn_v128f_broadcast(const float* ptr) {
    wcn_v128f_t result;
    result.raw = _mm_load1_ps(ptr);
    return result;
}

/* Load scalar and broadcast to all double lanes */
WCN_INLINE wcn_v128d_t wcn_v128d_broadcast(const double* ptr) {
    wcn_v128d_t result;
    result.raw = _mm_load1_pd(ptr);
    return result;
}

/* ========== Integer Multiplication ========== */

/* 16-bit integer multiplication (low 16 bits of result) */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_mullo_epi16(a.raw, b.raw);
    return result;
}

/* 16-bit integer multiplication (high 16 bits of result, signed) */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_mulhi_epi16(a.raw, b.raw);
    return result;
}

/* 16-bit integer multiplication (high 16 bits of result, unsigned) */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_mulhi_epu16(a.raw, b.raw);
    return result;
}

/* 32-bit integer multiplication (low 32 bits) - SSE4.1 required, fallback for SSE2 */
#if !defined(WCN_X86_SSE4_1)
/* SSE2 fallback for 32-bit multiplication */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    /* Fallback using shuffle and 32-bit multiply */
    __m128i tmp1 = _mm_mul_epu32(a.raw, b.raw);                    /* mul 0,2 */
    __m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(a.raw, 4),         /* mul 1,3 */
                                  _mm_srli_si128(b.raw, 4));
    wcn_v128i_t result;
    result.raw = _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(0,0,2,0)),
                                     _mm_shuffle_epi32(tmp2, _MM_SHUFFLE(0,0,2,0)));
    return result;
}
#endif

/* Multiply packed unsigned 32-bit integers, produce 64-bit results */
WCN_INLINE wcn_v128i_t wcn_v128i_mul_u32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_mul_epu32(a.raw, b.raw);
    return result;
}

/* ========== Pack/Unpack Operations ========== */

/* Pack 32-bit signed integers to 16-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_packs_epi32(a.raw, b.raw);
    return result;
}

/* Pack 16-bit signed integers to 8-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_packs_epi16(a.raw, b.raw);
    return result;
}

/* Pack 16-bit signed integers to 8-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_packus_epi16(a.raw, b.raw);
    return result;
}

/* Pack 32-bit signed integers to 16-bit unsigned with saturation (SSE4.1 has native, SSE2 fallback) */
#if !defined(WCN_X86_SSE4_1)
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* SSE2 fallback: clamp to [0, 65535] then pack */
    __m128i zero = _mm_setzero_si128();
    __m128i max_u16 = _mm_set1_epi32(0xFFFF);

    /* Clamp a and b to unsigned 16-bit range [0, 65535] */
    /* First clamp negative values to 0 */
    __m128i a_clamped = _mm_and_si128(a.raw, _mm_cmpgt_epi32(a.raw, zero));
    __m128i b_clamped = _mm_and_si128(b.raw, _mm_cmpgt_epi32(b.raw, zero));

    /* Then clamp values > 65535 to 65535 */
    __m128i a_mask = _mm_cmpgt_epi32(a_clamped, max_u16);
    __m128i b_mask = _mm_cmpgt_epi32(b_clamped, max_u16);
    a_clamped = _mm_or_si128(_mm_and_si128(a_mask, max_u16), _mm_andnot_si128(a_mask, a_clamped));
    b_clamped = _mm_or_si128(_mm_and_si128(b_mask, max_u16), _mm_andnot_si128(b_mask, b_clamped));

    /* Now pack: shuffle to get lower 16 bits of each 32-bit value */
    /* Use packs_epi32 which saturates to signed 16-bit, but since we clamped to [0,65535],
       values are in valid signed range [-32768, 32767] after reinterpret */
    /* Actually, we need to extract lower 16 bits and combine them */
    __m128i a_shifted = _mm_slli_epi32(a_clamped, 16);
    __m128i b_shifted = _mm_slli_epi32(b_clamped, 16);
    a_shifted = _mm_srai_epi32(a_shifted, 16);
    b_shifted = _mm_srai_epi32(b_shifted, 16);

    result.raw = _mm_packs_epi32(a_shifted, b_shifted);
    return result;
}
#endif

/* Unpack and interleave low 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_unpacklo_epi8(a.raw, b.raw);
    return result;
}

/* Unpack and interleave high 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_unpackhi_epi8(a.raw, b.raw);
    return result;
}

/* Unpack and interleave low 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_unpacklo_epi16(a.raw, b.raw);
    return result;
}

/* Unpack and interleave high 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_unpackhi_epi16(a.raw, b.raw);
    return result;
}

/* Unpack and interleave low 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_unpacklo_epi32(a.raw, b.raw);
    return result;
}

/* Unpack and interleave high 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_unpackhi_epi32(a.raw, b.raw);
    return result;
}

/* Unpack and interleave low 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_unpacklo_epi64(a.raw, b.raw);
    return result;
}

/* Unpack and interleave high 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_unpackhi_epi64(a.raw, b.raw);
    return result;
}

/* Unpack floats */
WCN_INLINE wcn_v128f_t wcn_v128f_unpacklo(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_unpacklo_ps(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_unpackhi(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = _mm_unpackhi_ps(a.raw, b.raw);
    return result;
}

/* Unpack doubles */
WCN_INLINE wcn_v128d_t wcn_v128d_unpacklo(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_unpacklo_pd(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_unpackhi(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = _mm_unpackhi_pd(a.raw, b.raw);
    return result;
}

/* ========== Extract/Insert Operations ========== */

/* Extract 16-bit integer (index must be compile-time constant) */
#define wcn_v128i_extract_i16(vec, index) _mm_extract_epi16((vec).raw, (index))

/* Insert 16-bit integer (index must be compile-time constant) */
#define wcn_v128i_insert_i16(vec, val, index) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = _mm_insert_epi16((vec).raw, (val), (index)); \
        _result; \
    })

#ifdef _MSC_VER
#undef wcn_v128i_insert_i16
#define WCN_INSERT_EPI16(vec, val, index) _mm_insert_epi16((vec).raw, (val), (index))
#endif

/* ========== Movemask Operations ========== */

/* Create mask from most significant bit of each 8-bit element */
WCN_INLINE int wcn_v128i_movemask_i8(wcn_v128i_t vec) {
    return _mm_movemask_epi8(vec.raw);
}

/* Create mask from most significant bit of each float */
WCN_INLINE int wcn_v128f_movemask(wcn_v128f_t vec) {
    return _mm_movemask_ps(vec.raw);
}

/* Create mask from most significant bit of each double */
WCN_INLINE int wcn_v128d_movemask(wcn_v128d_t vec) {
    return _mm_movemask_pd(vec.raw);
}

/* ========== Blend/Select Operations (SSE4.1) ========== */

#if !defined(WCN_X86_SSE4_1)
/* SSE2 fallback for blend operations using select */
WCN_INLINE wcn_v128f_t wcn_v128f_blendv(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t mask) {
    wcn_v128f_t result;
    __m128 m = _mm_castsi128_ps(_mm_srai_epi32(_mm_castps_si128(mask.raw), 31));
    result.raw = _mm_or_ps(_mm_and_ps(m, b.raw), _mm_andnot_ps(m, a.raw));
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_blendv(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t mask) {
    wcn_v128d_t result;
    __m128d m = _mm_castsi128_pd(_mm_srai_epi32(_mm_castpd_si128(mask.raw), 31));
    result.raw = _mm_or_pd(_mm_and_pd(m, b.raw), _mm_andnot_pd(m, a.raw));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_blendv_i8(wcn_v128i_t a, wcn_v128i_t b, wcn_v128i_t mask) {
    wcn_v128i_t result;
    result.raw = _mm_or_si128(_mm_and_si128(mask.raw, b.raw),
                               _mm_andnot_si128(mask.raw, a.raw));
    return result;
}
#endif

/* ========== Byte Shuffle (SSSE3) ========== */

#if !defined(WCN_X86_SSSE3)
/* SSE2 fallback - software implementation (slow!) */
WCN_INLINE wcn_v128i_t wcn_v128i_shuffle_i8(wcn_v128i_t vec, wcn_v128i_t control) {
    /* Software fallback using scalar operations */
    uint8_t src[16], ctrl[16], dst[16];
    _mm_storeu_si128((__m128i*)src, vec.raw);
    _mm_storeu_si128((__m128i*)ctrl, control.raw);

    for (int i = 0; i < 16; i++) {
        uint8_t idx = ctrl[i] & 0x0F;
        dst[i] = (ctrl[i] & 0x80) ? 0 : src[idx];
    }

    wcn_v128i_t result;
    result.raw = _mm_loadu_si128((const __m128i*)dst);
    return result;
}
#endif

/* ========== Conversions ========== */

WCN_INLINE wcn_v128f_t wcn_v128i_to_v128f(wcn_v128i_t vec) {
    wcn_v128f_t result;
    result.raw = _mm_cvtepi32_ps(vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128f_to_v128i(wcn_v128f_t vec) {
    wcn_v128i_t result;
    result.raw = _mm_cvtps_epi32(vec.raw);
    return result;
}

#endif /* WCN_X86_SSE2 */

#endif /* WCN_X86_SSE2_H */
