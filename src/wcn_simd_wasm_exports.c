/*
 * WCN_SIMD WebAssembly Export Wrappers
 * 
 * This file provides non-inline export wrappers for SIMD operations
 * to make them accessible from JavaScript when compiled to WebAssembly.
 * 
 * All platform-specific SIMD functions are normally defined as static inline
 * for performance, but WebAssembly exports require non-static functions.
 */

#include "WCN_SIMD.h"

#ifdef WCN_WASM_SIMD128

/* ========== Load/Store Operations ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_load(const void* ptr) {
    return wcn_v128i_load(ptr);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_load_aligned(const void* ptr) {
    return wcn_v128i_load_aligned(ptr);
}

WCN_API_EXPORT void wcn_wasm_v128i_store(void* ptr, wcn_v128i_t vec) {
    wcn_v128i_store(ptr, vec);
}

WCN_API_EXPORT void wcn_wasm_v128i_store_aligned(void* ptr, wcn_v128i_t vec) {
    wcn_v128i_store_aligned(ptr, vec);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_load(const float* ptr) {
    return wcn_v128f_load(ptr);
}

WCN_API_EXPORT void wcn_wasm_v128f_store(float* ptr, wcn_v128f_t vec) {
    wcn_v128f_store(ptr, vec);
}

/* ========== Initialization ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_set1_i8(int8_t value) {
    return wcn_v128i_set1_i8(value);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_set1_i16(int16_t value) {
    return wcn_v128i_set1_i16(value);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_set1_i32(int32_t value) {
    return wcn_v128i_set1_i32(value);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_set1_i64(int64_t value) {
    return wcn_v128i_set1_i64(value);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_setzero(void) {
    return wcn_v128i_setzero();
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_set1(float value) {
    return wcn_v128f_set1(value);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_setzero(void) {
    return wcn_v128f_setzero();
}

/* ========== Integer Arithmetic ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_add_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_add_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_sub_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_sub_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_add_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_add_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_sub_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_sub_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_add_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_add_i32(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_sub_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_sub_i32(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_add_i64(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_add_i64(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_sub_i64(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_sub_i64(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_mullo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_mullo_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_mullo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_mullo_i32(a, b);
}

/* ========== Float Arithmetic ========== */

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_add(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_add(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_sub(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_sub(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_mul(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_mul(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_div(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_div(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_sqrt(wcn_v128f_t vec) {
    return wcn_v128f_sqrt(vec);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_min(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_min(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_max(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_max(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_abs(wcn_v128f_t vec) {
    return wcn_v128f_abs(vec);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_neg(wcn_v128f_t vec) {
    return wcn_v128f_neg(vec);
}

/* ========== Saturating Arithmetic ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_adds_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_adds_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_subs_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_subs_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_adds_u8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_adds_u8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_subs_u8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_subs_u8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_adds_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_adds_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_subs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_subs_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_adds_u16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_adds_u16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_subs_u16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_subs_u16(a, b);
}

/* ========== Shift Operations ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_sll_i16(wcn_v128i_t a, wcn_v128i_t count) {
    return wcn_v128i_sll_i16(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_srl_i16(wcn_v128i_t a, wcn_v128i_t count) {
    return wcn_v128i_srl_i16(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_sra_i16(wcn_v128i_t a, wcn_v128i_t count) {
    return wcn_v128i_sra_i16(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_slli_i16(wcn_v128i_t a, int imm) {
    return wcn_v128i_slli_i16(a, imm);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_srli_i16(wcn_v128i_t a, int imm) {
    /* Use variable shift version since immediate version is not available as function */
    wcn_v128i_t count = wcn_v128i_set1_i32(imm);
    return wcn_v128i_srli_i16(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_srai_i16(wcn_v128i_t a, int imm) {
    return wcn_v128i_srai_i16(a, imm);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_sll_i32(wcn_v128i_t a, wcn_v128i_t count) {
    return wcn_v128i_sll_i32(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_srl_i32(wcn_v128i_t a, wcn_v128i_t count) {
    return wcn_v128i_srl_i32(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_sra_i32(wcn_v128i_t a, wcn_v128i_t count) {
    return wcn_v128i_sra_i32(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_slli_i32(wcn_v128i_t a, int imm) {
    return wcn_v128i_slli_i32(a, imm);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_srli_i32(wcn_v128i_t a, int imm) {
    /* Use variable shift version since immediate version is not available as function */
    wcn_v128i_t count = wcn_v128i_set1_i32(imm);
    return wcn_v128i_srli_i32(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_srai_i32(wcn_v128i_t a, int imm) {
    return wcn_v128i_srai_i32(a, imm);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_sll_i64(wcn_v128i_t a, wcn_v128i_t count) {
    return wcn_v128i_sll_i64(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_srl_i64(wcn_v128i_t a, wcn_v128i_t count) {
    return wcn_v128i_srl_i64(a, count);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_slli_i64(wcn_v128i_t a, int imm) {
    return wcn_v128i_slli_i64(a, imm);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_srli_i64(wcn_v128i_t a, int imm) {
    /* Use variable shift version since immediate version is not available as function */
    wcn_v128i_t count = wcn_v128i_set1_i32(imm);
    return wcn_v128i_srli_i64(a, count);
}

/* ========== Logical Operations ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_and(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_and(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_or(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_or(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_xor(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_xor(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_andnot(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_andnot(a, b);
}

/* ========== Comparison Operations ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_cmpeq_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_cmpeq_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_cmpgt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_cmpgt_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_cmplt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_cmplt_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_cmpeq_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_cmpeq_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_cmpgt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_cmpgt_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_cmplt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_cmplt_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_cmpeq_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_cmpeq_i32(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_cmpgt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_cmpgt_i32(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_cmplt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_cmplt_i32(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_cmpeq(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_cmpeq(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_cmplt(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_cmplt(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_cmple(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_cmple(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_cmpgt(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_cmpgt(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_cmpge(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_cmpge(a, b);
}

/* ========== Min/Max Operations ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_min_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_min_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_max_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_max_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_min_u8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_min_u8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_max_u8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_max_u8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_min_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_min_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_max_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_max_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_min_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_min_i32(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_max_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_max_i32(a, b);
}

/* ========== Absolute Value ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_abs_i8(wcn_v128i_t vec) {
    return wcn_v128i_abs_i8(vec);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_abs_i16(wcn_v128i_t vec) {
    return wcn_v128i_abs_i16(vec);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_abs_i32(wcn_v128i_t vec) {
    return wcn_v128i_abs_i32(vec);
}

/* ========== Negation ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_neg_i8(wcn_v128i_t vec) {
    return wcn_v128i_neg_i8(vec);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_neg_i16(wcn_v128i_t vec) {
    return wcn_v128i_neg_i16(vec);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_neg_i32(wcn_v128i_t vec) {
    return wcn_v128i_neg_i32(vec);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_neg_i64(wcn_v128i_t vec) {
    return wcn_v128i_neg_i64(vec);
}

/* ========== Pack/Unpack Operations ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_unpacklo_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_unpacklo_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_unpackhi_i8(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_unpackhi_i8(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_unpacklo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_unpacklo_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_unpackhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_unpackhi_i16(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_unpacklo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_unpacklo_i32(a, b);
}

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_unpackhi_i32(wcn_v128i_t a, wcn_v128i_t b) {
    return wcn_v128i_unpackhi_i32(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_unpacklo(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_unpacklo(a, b);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_unpackhi(wcn_v128f_t a, wcn_v128f_t b) {
    return wcn_v128f_unpackhi(a, b);
}

/* ========== Rounding Operations ========== */

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_floor(wcn_v128f_t vec) {
    return wcn_v128f_floor(vec);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_ceil(wcn_v128f_t vec) {
    return wcn_v128f_ceil(vec);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_trunc(wcn_v128f_t vec) {
    return wcn_v128f_trunc(vec);
}

WCN_API_EXPORT wcn_v128f_t wcn_wasm_v128f_round_nearest(wcn_v128f_t vec) {
    return wcn_v128f_round_nearest(vec);
}

/* ========== Byte Shuffle ========== */

WCN_API_EXPORT wcn_v128i_t wcn_wasm_v128i_shuffle_i8(wcn_v128i_t vec, wcn_v128i_t control) {
    return wcn_v128i_shuffle_i8(vec, control);
}

#endif /* WCN_WASM_SIMD128 */
