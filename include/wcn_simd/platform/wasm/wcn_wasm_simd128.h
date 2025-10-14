#ifndef WCN_WASM_SIMD128_H
#define WCN_WASM_SIMD128_H

#include "../../wcn_types.h"

#ifdef WCN_WASM_SIMD128

/* ========== Load/Store Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_load(const void* ptr) {
    wcn_v128i_t result;
    result.raw = wasm_v128_load(ptr);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_load_aligned(const void* ptr) {
    wcn_v128i_t result;
    result.raw = wasm_v128_load(ptr);  /* WASM doesn't distinguish aligned/unaligned */
    return result;
}

WCN_INLINE void wcn_v128i_store(void* ptr, wcn_v128i_t vec) {
    wasm_v128_store(ptr, vec.raw);
}

WCN_INLINE void wcn_v128i_store_aligned(void* ptr, wcn_v128i_t vec) {
    wasm_v128_store(ptr, vec.raw);  /* WASM doesn't distinguish aligned/unaligned */
}

WCN_INLINE wcn_v128f_t wcn_v128f_load(const float* ptr) {
    wcn_v128f_t result;
    result.raw = wasm_v128_load(ptr);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_load_aligned(const float* ptr) {
    wcn_v128f_t result;
    result.raw = wasm_v128_load(ptr);
    return result;
}

WCN_INLINE void wcn_v128f_store(float* ptr, wcn_v128f_t vec) {
    wasm_v128_store(ptr, vec.raw);
}

WCN_INLINE void wcn_v128f_store_aligned(float* ptr, wcn_v128f_t vec) {
    wasm_v128_store(ptr, vec.raw);
}

/* Note: WebAssembly SIMD doesn't support f64x2 (double) operations */

/* ========== Initialization ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i8(int8_t value) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_splat(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i16(int16_t value) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_splat(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i32(int32_t value) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_splat(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i64(int64_t value) {
    wcn_v128i_t result;
    result.raw = wasm_i64x2_splat(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_setzero(void) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_splat(0);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_set1(float value) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_splat(value);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_setzero(void) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_splat(0.0f);
    return result;
}

/* ========== Arithmetic Operations ========== */

/* 8-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_add(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_sub(a.raw, b.raw);
    return result;
}

/* 16-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_add(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_sub(a.raw, b.raw);
    return result;
}

/* 32-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_add(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_sub(a.raw, b.raw);
    return result;
}

/* 64-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i64x2_add(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i64x2_sub(a.raw, b.raw);
    return result;
}

/* ========== Saturating Arithmetic ========== */

/* 8-bit signed saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_add_sat(a.raw, b.raw);
    return result;
}

/* 8-bit signed saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_sub_sat(a.raw, b.raw);
    return result;
}

/* 8-bit unsigned saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_u8x16_add_sat(a.raw, b.raw);
    return result;
}

/* 8-bit unsigned saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_u8x16_sub_sat(a.raw, b.raw);
    return result;
}

/* 16-bit signed saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_add_sat(a.raw, b.raw);
    return result;
}

/* 16-bit signed saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_sub_sat(a.raw, b.raw);
    return result;
}

/* 16-bit unsigned saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_u16x8_add_sat(a.raw, b.raw);
    return result;
}

/* 16-bit unsigned saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_u16x8_sub_sat(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_mul_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_mul(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_add(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_add(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_sub(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_sub(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_mul(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_mul(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_div(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_div(a.raw, b.raw);
    return result;
}

/* ========== FMA (Fused Multiply-Add) - Software Emulation ========== */

/* Float FMA: a * b + c (WebAssembly emulation using mul + add) */
WCN_INLINE wcn_v128f_t wcn_v128f_fmadd(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    /* WebAssembly SIMD doesn't have FMA, emulate with mul + add */
    v128_t prod = wasm_f32x4_mul(a.raw, b.raw);
    result.raw = wasm_f32x4_add(prod, c.raw);
    return result;
}

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_and(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_v128_and(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_or(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_v128_or(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_xor(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_v128_xor(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_andnot(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_v128_andnot(b.raw, a.raw);
    return result;
}

/* ========== Shift Operations ========== */

/* 16-bit shifts */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    /* wasm_i16x8_shl takes scalar shift count from lane 0 */
    result.raw = wasm_i16x8_shl(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srli_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    /* Logical right shift (unsigned) */
    result.raw = wasm_u16x8_shr(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = wasm_u16x8_shr(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sra_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    /* Arithmetic right shift (signed) */
    result.raw = wasm_i16x8_shr(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

/* Immediate 16-bit shifts (count must be compile-time constant) */
#define wcn_v128i_srai_i16(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = wasm_i16x8_shr((a).raw, (imm)); \
        _result; \
    })

#define wcn_v128i_slli_i16(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = wasm_i16x8_shl((a).raw, (imm)); \
        _result; \
    })

#define wcn_v128i_srli_i16_imm(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = wasm_u16x8_shr((a).raw, (imm)); \
        _result; \
    })

/* 32-bit shifts */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_shl(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srli_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    /* Logical right shift (unsigned) */
    result.raw = wasm_u32x4_shr(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = wasm_u32x4_shr(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sra_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    /* Arithmetic right shift (signed) */
    result.raw = wasm_i32x4_shr(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

/* Immediate 32-bit shifts (count must be compile-time constant) */
#define wcn_v128i_srai_i32(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = wasm_i32x4_shr((a).raw, (imm)); \
        _result; \
    })

#define wcn_v128i_slli_i32(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = wasm_i32x4_shl((a).raw, (imm)); \
        _result; \
    })

#define wcn_v128i_srli_i32_imm(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = wasm_u32x4_shr((a).raw, (imm)); \
        _result; \
    })

/* 64-bit shifts */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = wasm_i64x2_shl(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srli_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    /* Logical right shift (unsigned) */
    result.raw = wasm_u64x2_shr(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = wasm_u64x2_shr(a.raw, wasm_i32x4_extract_lane(count.raw, 0));
    return result;
}

/* Immediate 64-bit shifts (count must be compile-time constant) */
#define wcn_v128i_slli_i64(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = wasm_i64x2_shl((a).raw, (imm)); \
        _result; \
    })

#define wcn_v128i_srli_i64_imm(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = wasm_u64x2_shr((a).raw, (imm)); \
        _result; \
    })

/* 128-bit byte shifts (shift entire vector by bytes) */
#define wcn_v128i_slli_si128(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        /* Shift left by bytes, filling with zeros */ \
        _result.raw = wasm_i8x16_shuffle((a).raw, wasm_i32x4_splat(0), \
            (imm) >= 16 ? 16 : 16-(imm), (imm) >= 15 ? 16 : 17-(imm), \
            (imm) >= 14 ? 16 : 18-(imm), (imm) >= 13 ? 16 : 19-(imm), \
            (imm) >= 12 ? 16 : 20-(imm), (imm) >= 11 ? 16 : 21-(imm), \
            (imm) >= 10 ? 16 : 22-(imm), (imm) >= 9 ? 16 : 23-(imm), \
            (imm) >= 8 ? 16 : 24-(imm), (imm) >= 7 ? 16 : 25-(imm), \
            (imm) >= 6 ? 16 : 26-(imm), (imm) >= 5 ? 16 : 27-(imm), \
            (imm) >= 4 ? 16 : 28-(imm), (imm) >= 3 ? 16 : 29-(imm), \
            (imm) >= 2 ? 16 : 30-(imm), (imm) >= 1 ? 16 : 31-(imm)); \
        _result; \
    })

#define wcn_v128i_srli_si128(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        /* Shift right by bytes, filling with zeros */ \
        _result.raw = wasm_i8x16_shuffle((a).raw, wasm_i32x4_splat(0), \
            (imm) >= 1 ? ((imm) >= 16 ? 16 : (imm)) : 0, \
            (imm) >= 2 ? ((imm) >= 15 ? 16 : (imm)+1) : 1, \
            (imm) >= 3 ? ((imm) >= 14 ? 16 : (imm)+2) : 2, \
            (imm) >= 4 ? ((imm) >= 13 ? 16 : (imm)+3) : 3, \
            (imm) >= 5 ? ((imm) >= 12 ? 16 : (imm)+4) : 4, \
            (imm) >= 6 ? ((imm) >= 11 ? 16 : (imm)+5) : 5, \
            (imm) >= 7 ? ((imm) >= 10 ? 16 : (imm)+6) : 6, \
            (imm) >= 8 ? ((imm) >= 9 ? 16 : (imm)+7) : 7, \
            (imm) >= 9 ? ((imm) >= 8 ? 16 : (imm)+8) : 8, \
            (imm) >= 10 ? ((imm) >= 7 ? 16 : (imm)+9) : 9, \
            (imm) >= 11 ? ((imm) >= 6 ? 16 : (imm)+10) : 10, \
            (imm) >= 12 ? ((imm) >= 5 ? 16 : (imm)+11) : 11, \
            (imm) >= 13 ? ((imm) >= 4 ? 16 : (imm)+12) : 12, \
            (imm) >= 14 ? ((imm) >= 3 ? 16 : (imm)+13) : 13, \
            (imm) >= 15 ? ((imm) >= 2 ? 16 : (imm)+14) : 14, \
            (imm) >= 16 ? ((imm) >= 1 ? 16 : (imm)+15) : 15); \
        _result; \
    })

/* ========== Comparison Operations ========== */

/* 8-bit comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_eq(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_gt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_lt(a.raw, b.raw);
    return result;
}

/* 16-bit comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_eq(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_gt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_lt(a.raw, b.raw);
    return result;
}

/* 32-bit comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_eq(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_gt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_lt(a.raw, b.raw);
    return result;
}

/* Float comparisons */
WCN_INLINE wcn_v128f_t wcn_v128f_cmpeq(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_eq(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpgt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_gt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmplt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_lt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmple(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_le(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpge(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_ge(a.raw, b.raw);
    return result;
}

/* ========== Min/Max Operations ========== */

/* 8-bit min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_min(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_max(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_u8x16_min(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_u8x16_max(a.raw, b.raw);
    return result;
}

/* 16-bit min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_min(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_max(a.raw, b.raw);
    return result;
}

/* 32-bit min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_max_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_max(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_min(a.raw, b.raw);
    return result;
}

/* Float min/max */
WCN_INLINE wcn_v128f_t wcn_v128f_max(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_max(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_min(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_min(a.raw, b.raw);
    return result;
}

/* ========== Square Root ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_sqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_sqrt(vec.raw);
    return result;
}

/* ========== Reciprocal Approximations ========== */

/* Reciprocal approximation (1/x) - WASM doesn't have native rcp, use division */
WCN_INLINE wcn_v128f_t wcn_v128f_rcp(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Software fallback: 1.0f / vec */
    result.raw = wasm_f32x4_div(wasm_f32x4_splat(1.0f), vec.raw);
    return result;
}

/* Reciprocal square root approximation (1/sqrt(x)) - WASM doesn't have native rsqrt */
WCN_INLINE wcn_v128f_t wcn_v128f_rsqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Software fallback: 1.0f / sqrt(vec) */
    v128_t sqrt_vec = wasm_f32x4_sqrt(vec.raw);
    result.raw = wasm_f32x4_div(wasm_f32x4_splat(1.0f), sqrt_vec);
    return result;
}

/* ========== Rounding Operations ========== */

/* WebAssembly SIMD has native rounding operations for floats */

/* Round floats toward negative infinity (floor) */
WCN_INLINE wcn_v128f_t wcn_v128f_floor(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_floor(vec.raw);
    return result;
}

/* Round floats toward positive infinity (ceil) */
WCN_INLINE wcn_v128f_t wcn_v128f_ceil(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_ceil(vec.raw);
    return result;
}

/* Round floats to nearest integer */
WCN_INLINE wcn_v128f_t wcn_v128f_round_nearest(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_nearest(vec.raw);
    return result;
}

/* Round floats toward zero (truncate) */
WCN_INLINE wcn_v128f_t wcn_v128f_trunc(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_trunc(vec.raw);
    return result;
}

/* ========== Conversions ========== */

WCN_INLINE wcn_v128f_t wcn_v128i_to_v128f(wcn_v128i_t vec) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_convert_i32x4(vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128f_to_v128i(wcn_v128f_t vec) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_trunc_sat_f32x4(vec.raw);
    return result;
}

/* ========== Absolute Value Operations ========== */

/* 8-bit absolute value */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i8(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_abs(vec.raw);
    return result;
}

/* 16-bit absolute value */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i16(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_abs(vec.raw);
    return result;
}

/* 32-bit absolute value */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_abs(vec.raw);
    return result;
}

/* Float absolute value */
WCN_INLINE wcn_v128f_t wcn_v128f_abs(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_abs(vec.raw);
    return result;
}

/* ========== Negation Operations ========== */

/* 8-bit negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i8(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_neg(vec.raw);
    return result;
}

/* 16-bit negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i16(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_neg(vec.raw);
    return result;
}

/* 32-bit negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_neg(vec.raw);
    return result;
}

/* 64-bit negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i64(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = wasm_i64x2_neg(vec.raw);
    return result;
}

/* Float negation */
WCN_INLINE wcn_v128f_t wcn_v128f_neg(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_neg(vec.raw);
    return result;
}

/* ========== Broadcast Operations ========== */

/* Broadcast 32-bit integer from memory */
WCN_INLINE wcn_v128i_t wcn_v128i_broadcast_i32(const int32_t* ptr) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_splat(*ptr);
    return result;
}

/* Broadcast float from memory */
WCN_INLINE wcn_v128f_t wcn_v128f_broadcast(const float* ptr) {
    wcn_v128f_t result;
    result.raw = wasm_f32x4_splat(*ptr);
    return result;
}

/* ========== Integer Multiplication ========== */

/* 16-bit integer multiplication (low 16 bits) */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_mul(a.raw, b.raw);
    return result;
}

/* 16-bit integer multiplication (high 16 bits, signed) - NOT directly available in WASM */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* WASM SIMD doesn't have mulhi, use extmul operations */
    v128_t lo_a = wasm_i16x8_extend_low_i8x16(a.raw);
    v128_t lo_b = wasm_i16x8_extend_low_i8x16(b.raw);
    v128_t hi_a = wasm_i16x8_extend_high_i8x16(a.raw);
    v128_t hi_b = wasm_i16x8_extend_high_i8x16(b.raw);

    /* Use extmul for wider multiplication then extract high parts */
    v128_t prod_lo = wasm_i32x4_extmul_low_i16x8(a.raw, b.raw);
    v128_t prod_hi = wasm_i32x4_extmul_high_i16x8(a.raw, b.raw);

    /* Shift right and narrow */
    prod_lo = wasm_i32x4_shr(prod_lo, 16);
    prod_hi = wasm_i32x4_shr(prod_hi, 16);

    result.raw = wasm_i16x8_narrow_i32x4(prod_lo, prod_hi);
    return result;
}

/* 16-bit integer multiplication (high 16 bits, unsigned) */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* Use unsigned extmul */
    v128_t prod_lo = wasm_u32x4_extmul_low_u16x8(a.raw, b.raw);
    v128_t prod_hi = wasm_u32x4_extmul_high_u16x8(a.raw, b.raw);

    /* Shift right and narrow */
    prod_lo = wasm_u32x4_shr(prod_lo, 16);
    prod_hi = wasm_u32x4_shr(prod_hi, 16);

    result.raw = wasm_u16x8_narrow_i32x4(prod_lo, prod_hi);
    return result;
}

/* 32-bit integer multiplication (low 32 bits) */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i32x4_mul(a.raw, b.raw);
    return result;
}

/* Multiply packed unsigned 32-bit integers, produce 64-bit results */
WCN_INLINE wcn_v128i_t wcn_v128i_mul_u32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* WASM has extmul for this */
    v128_t prod_lo = wasm_i64x2_extmul_low_i32x4(a.raw, b.raw);
    /* Only keep lower 64-bit parts packed */
    result.raw = prod_lo;
    return result;
}

/* ========== Pack/Unpack Operations ========== */

/* Pack 32-bit signed integers to 16-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i16x8_narrow_i32x4(a.raw, b.raw);
    return result;
}

/* Pack 16-bit signed integers to 8-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_narrow_i16x8(a.raw, b.raw);
    return result;
}

/* Pack 16-bit signed integers to 8-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_u8x16_narrow_i16x8(a.raw, b.raw);
    return result;
}

/* Pack 32-bit signed integers to 16-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_u16x8_narrow_i32x4(a.raw, b.raw);
    return result;
}

/* Unpack and interleave low 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* WASM doesn't have direct unpack, use shuffle */
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        0, 16, 1, 17, 2, 18, 3, 19, 4, 20, 5, 21, 6, 22, 7, 23);
    return result;
}

/* Unpack and interleave high 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        8, 24, 9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31);
    return result;
}

/* Unpack and interleave low 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        0, 1, 16, 17, 2, 3, 18, 19, 4, 5, 20, 21, 6, 7, 22, 23);
    return result;
}

/* Unpack and interleave high 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        8, 9, 24, 25, 10, 11, 26, 27, 12, 13, 28, 29, 14, 15, 30, 31);
    return result;
}

/* Unpack and interleave low 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        0, 1, 2, 3, 16, 17, 18, 19, 4, 5, 6, 7, 20, 21, 22, 23);
    return result;
}

/* Unpack and interleave high 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        8, 9, 10, 11, 24, 25, 26, 27, 12, 13, 14, 15, 28, 29, 30, 31);
    return result;
}

/* Unpack and interleave low 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 18, 19, 20, 21, 22, 23);
    return result;
}

/* Unpack and interleave high 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 26, 27, 28, 29, 30, 31);
    return result;
}

/* Unpack floats */
WCN_INLINE wcn_v128f_t wcn_v128f_unpacklo(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        0, 1, 2, 3, 16, 17, 18, 19, 4, 5, 6, 7, 20, 21, 22, 23);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_unpackhi(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = wasm_i8x16_shuffle(a.raw, b.raw,
        8, 9, 10, 11, 24, 25, 26, 27, 12, 13, 14, 15, 28, 29, 30, 31);
    return result;
}

/* ========== Extract/Insert Operations ========== */

/* Extract 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_v128i_extract_i16(vec, index) wasm_i16x8_extract_lane((vec).raw, (index))

/* Insert 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_v128i_insert_i16(vec, val, index) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = wasm_i16x8_replace_lane((vec).raw, (index), (val)); \
        _result; \
    })

/* ========== Movemask Operations ========== */

/* Create mask from most significant bit of each 8-bit element */
WCN_INLINE int wcn_v128i_movemask_i8(wcn_v128i_t vec) {
    /* WASM has bitmask instruction */
    return wasm_i8x16_bitmask(vec.raw);
}

/* Create mask from most significant bit of each float */
WCN_INLINE int wcn_v128f_movemask(wcn_v128f_t vec) {
    /* Use i32x4 bitmask */
    return wasm_i32x4_bitmask(vec.raw);
}

/* ========== Blend/Select Operations ========== */

/* Variable blend floats (blends based on sign bit of mask) */
WCN_INLINE wcn_v128f_t wcn_v128f_blendv(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t mask) {
    wcn_v128f_t result;
    /* WASM has bitselect */
    result.raw = wasm_v128_bitselect(b.raw, a.raw, mask.raw);
    return result;
}

/* Variable blend 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_blendv_i8(wcn_v128i_t a, wcn_v128i_t b, wcn_v128i_t mask) {
    wcn_v128i_t result;
    result.raw = wasm_v128_bitselect(b.raw, a.raw, mask.raw);
    return result;
}

/* ========== Byte Shuffle ========== */

/* Shuffle bytes according to indices in control mask */
WCN_INLINE wcn_v128i_t wcn_v128i_shuffle_i8(wcn_v128i_t vec, wcn_v128i_t control) {
    wcn_v128i_t result;
    /* WASM has swizzle for this */
    result.raw = wasm_i8x16_swizzle(vec.raw, control.raw);
    return result;
}

/* ========== Double Precision Operations (Software Fallback) ========== */
/* WebAssembly SIMD128 doesn't support f64x2 operations natively.
 * These are scalar-based software fallback implementations. */

/* Load/Store */
WCN_INLINE wcn_v128d_t wcn_v128d_load(const double* ptr) {
    wcn_v128d_t result;
    result.raw[0] = ptr[0];
    result.raw[1] = ptr[1];
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_load_aligned(const double* ptr) {
    return wcn_v128d_load(ptr);
}

WCN_INLINE void wcn_v128d_store(double* ptr, wcn_v128d_t vec) {
    ptr[0] = vec.raw[0];
    ptr[1] = vec.raw[1];
}

WCN_INLINE void wcn_v128d_store_aligned(double* ptr, wcn_v128d_t vec) {
    wcn_v128d_store(ptr, vec);
}

/* Initialization */
WCN_INLINE wcn_v128d_t wcn_v128d_set1(double value) {
    wcn_v128d_t result;
    result.raw[0] = value;
    result.raw[1] = value;
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_setzero(void) {
    wcn_v128d_t result;
    result.raw[0] = 0.0;
    result.raw[1] = 0.0;
    return result;
}

/* Arithmetic */
WCN_INLINE wcn_v128d_t wcn_v128d_add(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw[0] = a.raw[0] + b.raw[0];
    result.raw[1] = a.raw[1] + b.raw[1];
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sub(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw[0] = a.raw[0] - b.raw[0];
    result.raw[1] = a.raw[1] - b.raw[1];
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_mul(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw[0] = a.raw[0] * b.raw[0];
    result.raw[1] = a.raw[1] * b.raw[1];
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_div(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw[0] = a.raw[0] / b.raw[0];
    result.raw[1] = a.raw[1] / b.raw[1];
    return result;
}

/* FMA */
WCN_INLINE wcn_v128d_t wcn_v128d_fmadd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw[0] = a.raw[0] * b.raw[0] + c.raw[0];
    result.raw[1] = a.raw[1] * b.raw[1] + c.raw[1];
    return result;
}

/* Comparisons */
WCN_INLINE wcn_v128d_t wcn_v128d_cmpeq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64_t all_bits = 0xFFFFFFFFFFFFFFFFULL;
    result.raw[0] = (a.raw[0] == b.raw[0]) ? *(double*)&all_bits : 0.0;
    result.raw[1] = (a.raw[1] == b.raw[1]) ? *(double*)&all_bits : 0.0;
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmplt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64_t all_bits = 0xFFFFFFFFFFFFFFFFULL;
    result.raw[0] = (a.raw[0] < b.raw[0]) ? *(double*)&all_bits : 0.0;
    result.raw[1] = (a.raw[1] < b.raw[1]) ? *(double*)&all_bits : 0.0;
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmple(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64_t all_bits = 0xFFFFFFFFFFFFFFFFULL;
    result.raw[0] = (a.raw[0] <= b.raw[0]) ? *(double*)&all_bits : 0.0;
    result.raw[1] = (a.raw[1] <= b.raw[1]) ? *(double*)&all_bits : 0.0;
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpgt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64_t all_bits = 0xFFFFFFFFFFFFFFFFULL;
    result.raw[0] = (a.raw[0] > b.raw[0]) ? *(double*)&all_bits : 0.0;
    result.raw[1] = (a.raw[1] > b.raw[1]) ? *(double*)&all_bits : 0.0;
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpge(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64_t all_bits = 0xFFFFFFFFFFFFFFFFULL;
    result.raw[0] = (a.raw[0] >= b.raw[0]) ? *(double*)&all_bits : 0.0;
    result.raw[1] = (a.raw[1] >= b.raw[1]) ? *(double*)&all_bits : 0.0;
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpneq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64_t all_bits = 0xFFFFFFFFFFFFFFFFULL;
    result.raw[0] = (a.raw[0] != b.raw[0]) ? *(double*)&all_bits : 0.0;
    result.raw[1] = (a.raw[1] != b.raw[1]) ? *(double*)&all_bits : 0.0;
    return result;
}

/* Min/Max */
WCN_INLINE wcn_v128d_t wcn_v128d_min(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw[0] = (a.raw[0] < b.raw[0]) ? a.raw[0] : b.raw[0];
    result.raw[1] = (a.raw[1] < b.raw[1]) ? a.raw[1] : b.raw[1];
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_max(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw[0] = (a.raw[0] > b.raw[0]) ? a.raw[0] : b.raw[0];
    result.raw[1] = (a.raw[1] > b.raw[1]) ? a.raw[1] : b.raw[1];
    return result;
}

/* Square root */
#include <math.h>
WCN_INLINE wcn_v128d_t wcn_v128d_sqrt(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw[0] = sqrt(vec.raw[0]);
    result.raw[1] = sqrt(vec.raw[1]);
    return result;
}

/* Absolute value */
WCN_INLINE wcn_v128d_t wcn_v128d_abs(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw[0] = fabs(vec.raw[0]);
    result.raw[1] = fabs(vec.raw[1]);
    return result;
}

/* Negation */
WCN_INLINE wcn_v128d_t wcn_v128d_neg(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw[0] = -vec.raw[0];
    result.raw[1] = -vec.raw[1];
    return result;
}

/* Broadcast */
WCN_INLINE wcn_v128d_t wcn_v128d_broadcast(const double* ptr) {
    wcn_v128d_t result;
    result.raw[0] = *ptr;
    result.raw[1] = *ptr;
    return result;
}

/* Movemask */
WCN_INLINE int wcn_v128d_movemask(wcn_v128d_t vec) {
    int result = 0;
    uint64_t bits0, bits1;
    bits0 = *(uint64_t*)&vec.raw[0];
    bits1 = *(uint64_t*)&vec.raw[1];
    result |= ((bits0 >> 63) & 1) << 0;
    result |= ((bits1 >> 63) & 1) << 1;
    return result;
}

/* Blend */
WCN_INLINE wcn_v128d_t wcn_v128d_blendv(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t mask) {
    wcn_v128d_t result;
    uint64_t mask0 = *(uint64_t*)&mask.raw[0];
    uint64_t mask1 = *(uint64_t*)&mask.raw[1];
    result.raw[0] = (mask0 & (1ULL << 63)) ? b.raw[0] : a.raw[0];
    result.raw[1] = (mask1 & (1ULL << 63)) ? b.raw[1] : a.raw[1];
    return result;
}

/* Unpack */
WCN_INLINE wcn_v128d_t wcn_v128d_unpacklo(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw[0] = a.raw[0];
    result.raw[1] = b.raw[0];
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_unpackhi(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw[0] = a.raw[1];
    result.raw[1] = b.raw[1];
    return result;
}

/* Rounding Operations - scalar fallback */
WCN_INLINE wcn_v128d_t wcn_v128d_floor(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw[0] = floor(vec.raw[0]);
    result.raw[1] = floor(vec.raw[1]);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_ceil(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw[0] = ceil(vec.raw[0]);
    result.raw[1] = ceil(vec.raw[1]);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_round_nearest(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw[0] = round(vec.raw[0]);
    result.raw[1] = round(vec.raw[1]);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_trunc(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw[0] = trunc(vec.raw[0]);
    result.raw[1] = trunc(vec.raw[1]);
    return result;
}

#endif /* WCN_WASM_SIMD128 */

#endif /* WCN_WASM_SIMD128_H */
