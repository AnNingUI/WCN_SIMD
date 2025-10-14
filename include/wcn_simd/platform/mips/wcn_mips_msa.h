#ifndef WCN_MIPS_MSA_H
#define WCN_MIPS_MSA_H

#include "../../wcn_types.h"

#ifdef WCN_MIPS_MSA

/* ========== Load/Store Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_load(const void* ptr) {
    wcn_v128i_t result;
    result.raw = __msa_ld_w((const int32_t*)ptr, 0);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_load_aligned(const void* ptr) {
    wcn_v128i_t result;
    result.raw = __msa_ld_w((const int32_t*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v128i_store(void* ptr, wcn_v128i_t vec) {
    __msa_st_w(vec.raw, (int32_t*)ptr, 0);
}

WCN_INLINE void wcn_v128i_store_aligned(void* ptr, wcn_v128i_t vec) {
    __msa_st_w(vec.raw, (int32_t*)ptr, 0);
}

WCN_INLINE wcn_v128f_t wcn_v128f_load(const float* ptr) {
    wcn_v128f_t result;
    result.raw = __msa_ld_w((const v4i32*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v128f_store(float* ptr, wcn_v128f_t vec) {
    __msa_st_w((v4i32)vec.raw, (v4i32*)ptr, 0);
}

WCN_INLINE wcn_v128f_t wcn_v128f_load_aligned(const float* ptr) {
    wcn_v128f_t result;
    result.raw = __msa_ld_w((const v4i32*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v128f_store_aligned(float* ptr, wcn_v128f_t vec) {
    __msa_st_w((v4i32)vec.raw, (v4i32*)ptr, 0);
}

WCN_INLINE wcn_v128d_t wcn_v128d_load(const double* ptr) {
    wcn_v128d_t result;
    result.raw = __msa_ld_d((const v2i64*)ptr, 0);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_load_aligned(const double* ptr) {
    wcn_v128d_t result;
    result.raw = __msa_ld_d((const v2i64*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v128d_store(double* ptr, wcn_v128d_t vec) {
    __msa_st_d((v2i64)vec.raw, (v2i64*)ptr, 0);
}

WCN_INLINE void wcn_v128d_store_aligned(double* ptr, wcn_v128d_t vec) {
    __msa_st_d((v2i64)vec.raw, (v2i64*)ptr, 0);
}

/* ========== Initialization ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i8(int8_t value) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_fill_b(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i16(int16_t value) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_fill_h(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i32(int32_t value) {
    wcn_v128i_t result;
    result.raw = __msa_fill_w(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i64(int64_t value) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_fill_d(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set_i32(int32_t v3, int32_t v2, int32_t v1, int32_t v0) {
    wcn_v128i_t result;
    int32_t vals[4] = {v0, v1, v2, v3};
    result.raw = __msa_ld_w(vals, 0);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_setzero(void) {
    wcn_v128i_t result;
    result.raw = __msa_ldi_w(0);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_set1(float value) {
    wcn_v128f_t result;
    int32_t i = *((int32_t*)&value);
    result.raw = (v4f32)__msa_fill_w(i);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_setzero(void) {
    wcn_v128f_t result;
    result.raw = (v4f32)__msa_ldi_w(0);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_set1(double value) {
    wcn_v128d_t result;
    int64_t i = *((int64_t*)&value);
    result.raw = (v2f64)__msa_fill_d(i);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_setzero(void) {
    wcn_v128d_t result;
    result.raw = (v2f64)__msa_ldi_d(0);
    return result;
}

/* ========== Arithmetic Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_add_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_addv_b((v16i8)a.raw, (v16i8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subv_b((v16i8)a.raw, (v16i8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_add_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_addv_h((v8i16)a.raw, (v8i16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subv_h((v8i16)a.raw, (v8i16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_add_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_addv_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_subv_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_add_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_addv_d((v2i64)a.raw, (v2i64)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subv_d((v2i64)a.raw, (v2i64)b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_add(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __msa_fadd_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_sub(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __msa_fsub_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_mul(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __msa_fmul_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_div(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __msa_fdiv_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_add(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __msa_fadd_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sub(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __msa_fsub_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_mul(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __msa_fmul_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_div(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __msa_fdiv_d(a.raw, b.raw);
    return result;
}

/* ========== FMA (Fused Multiply-Add) ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_fmadd(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = __msa_fmadd_w(c.raw, a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_fmadd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = __msa_fmadd_d(c.raw, a.raw, b.raw);
    return result;
}

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_and(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_and_v((v16u8)a.raw, (v16u8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_or(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_or_v((v16u8)a.raw, (v16u8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_xor(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_xor_v((v16u8)a.raw, (v16u8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_andnot(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    v16u8 not_a = __msa_nor_v((v16u8)a.raw, (v16u8)a.raw);
    result.raw = __msa_and_v(not_a, (v16u8)b.raw);
    return result;
}

/* ========== Comparison Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_ceq_b((v16i8)a.raw, (v16i8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_clt_s_b((v16i8)b.raw, (v16i8)a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_clt_s_b((v16i8)a.raw, (v16i8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_ceq_h((v8i16)a.raw, (v8i16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_clt_s_h((v8i16)b.raw, (v8i16)a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_clt_s_h((v8i16)a.raw, (v8i16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_ceq_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_clt_s_w(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_clt_s_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpeq(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (v4f32)__msa_fceq_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmplt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (v4f32)__msa_fclt_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmple(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (v4f32)__msa_fcle_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpgt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (v4f32)__msa_fclt_w(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpge(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (v4f32)__msa_fcle_w(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpeq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (v2f64)__msa_fceq_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmplt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (v2f64)__msa_fclt_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmple(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (v2f64)__msa_fcle_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpgt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (v2f64)__msa_fclt_d(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpge(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (v2f64)__msa_fcle_d(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpneq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    v2i64 eq = __msa_fceq_d(a.raw, b.raw);
    result.raw = (v2f64)__msa_nor_v((v16u8)eq, (v16u8)eq);
    return result;
}

/* ========== Min/Max Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_min_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_min_s_b((v16i8)a.raw, (v16i8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_max_s_b((v16i8)a.raw, (v16i8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_min_u_b((v16u8)a.raw, (v16u8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_max_u_b((v16u8)a.raw, (v16u8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_min_s_h((v8i16)a.raw, (v8i16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_max_s_h((v8i16)a.raw, (v8i16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_max_s_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_min_s_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_max(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __msa_fmax_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_min(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __msa_fmin_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_max(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __msa_fmax_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_min(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __msa_fmin_d(a.raw, b.raw);
    return result;
}

/* ========== Square Root ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_sqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = __msa_fsqrt_w(vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sqrt(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = __msa_fsqrt_d(vec.raw);
    return result;
}

/* ========== Reciprocal Approximations ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_rcp(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = __msa_frcp_w(vec.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_rsqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = __msa_frsqrt_w(vec.raw);
    return result;
}

/* ========== Absolute Value ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_abs_i8(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_add_a_b((v16i8)vec.raw, (v16i8)__msa_ldi_b(0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_abs_i16(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_add_a_h((v8i16)vec.raw, (v8i16)__msa_ldi_h(0));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_abs_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = __msa_add_a_w(vec.raw, __msa_ldi_w(0));
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_abs(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = __msa_fabs_w(vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_abs(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = __msa_fabs_d(vec.raw);
    return result;
}

/* ========== Negation ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_neg_i8(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subv_b((v16i8)__msa_ldi_b(0), (v16i8)vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_neg_i16(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subv_h((v8i16)__msa_ldi_h(0), (v8i16)vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_neg_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = __msa_subv_w(__msa_ldi_w(0), vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_neg_i64(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subv_d((v2i64)__msa_ldi_d(0), (v2i64)vec.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_neg(wcn_v128f_t vec) {
    wcn_v128f_t result;
    v4i32 sign_mask = __msa_fill_w(0x80000000);
    result.raw = (v4f32)__msa_xor_v((v16u8)vec.raw, (v16u8)sign_mask);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_neg(wcn_v128d_t vec) {
    wcn_v128d_t result;
    v2i64 sign_mask = __msa_fill_d(0x8000000000000000LL);
    result.raw = (v2f64)__msa_xor_v((v16u8)vec.raw, (v16u8)sign_mask);
    return result;
}

/* ========== Broadcast ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_broadcast_i32(const int32_t* ptr) {
    wcn_v128i_t result;
    result.raw = __msa_fill_w(*ptr);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_broadcast(const float* ptr) {
    wcn_v128f_t result;
    int32_t val = *((const int32_t*)ptr);
    result.raw = (v4f32)__msa_fill_w(val);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_broadcast(const double* ptr) {
    wcn_v128d_t result;
    int64_t val = *((const int64_t*)ptr);
    result.raw = (v2f64)__msa_fill_d(val);
    return result;
}

/* ========== Conversions ========== */

WCN_INLINE wcn_v128f_t wcn_v128i_to_v128f(wcn_v128i_t vec) {
    wcn_v128f_t result;
    result.raw = __msa_ffint_s_w(vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128f_to_v128i(wcn_v128f_t vec) {
    wcn_v128i_t result;
    result.raw = __msa_ftint_s_w(vec.raw);
    return result;
}

/* ========== Integer Multiplication ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_mulv_h((v8i16)a.raw, (v8i16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    v4i32 prod_even = __msa_dotp_s_w((v8i16)a.raw, (v8i16)b.raw);
    v4i32 prod_odd = __msa_dpadd_s_w(prod_even, (v8i16)a.raw, (v8i16)b.raw);
    /* Extract high 16 bits */
    result.raw = (v4i32)__msa_srli_w(prod_even, 16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    v4u32 prod_even = __msa_dotp_u_w((v8u16)a.raw, (v8u16)b.raw);
    result.raw = (v4i32)__msa_srli_w(prod_even, 16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_mulv_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_mul_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_mulv_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_mul_u32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_dotp_u_d((v4u32)a.raw, (v4u32)b.raw);
    return result;
}

/* ========== Saturating Arithmetic ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_adds_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_adds_s_b((v16i8)a.raw, (v16i8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_subs_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subs_s_b((v16i8)a.raw, (v16i8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_adds_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_adds_u_b((v16u8)a.raw, (v16u8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_subs_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subs_u_b((v16u8)a.raw, (v16u8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_adds_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_adds_s_h((v8i16)a.raw, (v8i16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_subs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subs_s_h((v8i16)a.raw, (v8i16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_adds_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_adds_u_h((v8u16)a.raw, (v8u16)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_subs_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_subs_u_h((v8u16)a.raw, (v8u16)b.raw);
    return result;
}

/* ========== Shift Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_sll_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_sll_h((v8i16)a.raw, (v8i16)count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_srl_h((v8i16)a.raw, (v8i16)count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sra_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_sra_h((v8i16)a.raw, (v8i16)count.raw);
    return result;
}

#define wcn_v128i_slli_i16(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = (v4i32)__msa_slli_h((v8i16)(a).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srli_i16(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = (v4i32)__msa_srli_h((v8i16)(a).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srai_i16(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = (v4i32)__msa_srai_h((v8i16)(a).raw, (imm)); \
    _result; \
})

WCN_INLINE wcn_v128i_t wcn_v128i_sll_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __msa_sll_w(a.raw, count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __msa_srl_w(a.raw, count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sra_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __msa_sra_w(a.raw, count.raw);
    return result;
}

#define wcn_v128i_slli_i32(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __msa_slli_w((a).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srli_i32(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __msa_srli_w((a).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srai_i32(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __msa_srai_w((a).raw, (imm)); \
    _result; \
})

WCN_INLINE wcn_v128i_t wcn_v128i_sll_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_sll_d((v2i64)a.raw, (v2i64)count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_srl_d((v2i64)a.raw, (v2i64)count.raw);
    return result;
}

#define wcn_v128i_slli_i64(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = (v4i32)__msa_slli_d((v2i64)(a).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srli_i64(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = (v4i32)__msa_srli_d((v2i64)(a).raw, (imm)); \
    _result; \
})

#define wcn_v128i_slli_si128(a, imm) ({ \
    wcn_v128i_t _result; \
    v16i8 _zero = __msa_ldi_b(0); \
    _result.raw = (v4i32)__msa_sldi_b(_zero, (v16i8)(a).raw, 16 - (imm)); \
    _result; \
})

#define wcn_v128i_srli_si128(a, imm) ({ \
    wcn_v128i_t _result; \
    v16i8 _zero = __msa_ldi_b(0); \
    _result.raw = (v4i32)__msa_sldi_b((v16i8)(a).raw, _zero, (imm)); \
    _result; \
})

/* ========== Pack/Unpack Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_packs_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_sat_s_w(a.raw, 15);
    v4i32 b_sat = __msa_sat_s_w(b.raw, 15);
    result.raw = (v4i32)__msa_pckev_h((v8i16)b_sat, (v8i16)result.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_packs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    v8i16 a_sat = __msa_sat_s_h((v8i16)a.raw, 7);
    v8i16 b_sat = __msa_sat_s_h((v8i16)b.raw, 7);
    result.raw = (v4i32)__msa_pckev_b((v16i8)b_sat, (v16i8)a_sat);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_packus_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    v8u16 a_sat = __msa_sat_u_h((v8u16)a.raw, 7);
    v8u16 b_sat = __msa_sat_u_h((v8u16)b.raw, 7);
    result.raw = (v4i32)__msa_pckev_b((v16i8)b_sat, (v16i8)a_sat);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_packus_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    v4u32 a_sat = __msa_sat_u_w((v4u32)a.raw, 15);
    v4u32 b_sat = __msa_sat_u_w((v4u32)b.raw, 15);
    result.raw = (v4i32)__msa_pckev_h((v8i16)b_sat, (v8i16)a_sat);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_ilvr_b((v16i8)b.raw, (v16i8)a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_ilvl_b((v16i8)b.raw, (v16i8)a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_ilvr_h((v8i16)b.raw, (v8i16)a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_ilvl_h((v8i16)b.raw, (v8i16)a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_ilvr_w(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __msa_ilvl_w(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_ilvr_d((v2i64)b.raw, (v2i64)a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_ilvl_d((v2i64)b.raw, (v2i64)a.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_unpacklo(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (v4f32)__msa_ilvr_w((v4i32)b.raw, (v4i32)a.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_unpackhi(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (v4f32)__msa_ilvl_w((v4i32)b.raw, (v4i32)a.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_unpacklo(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (v2f64)__msa_ilvr_d((v2i64)b.raw, (v2i64)a.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_unpackhi(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (v2f64)__msa_ilvl_d((v2i64)b.raw, (v2i64)a.raw);
    return result;
}

/* ========== Extract/Insert Operations ========== */

#define wcn_v128i_extract_i16(vec, index) __msa_copy_s_h((v8i16)(vec).raw, (index))

#define wcn_v128i_insert_i16(vec, val, index) ({ \
    wcn_v128i_t _result; \
    _result.raw = (v4i32)__msa_insert_h((v8i16)(vec).raw, (index), (val)); \
    _result; \
})

/* ========== Movemask Operations ========== */

WCN_INLINE int wcn_v128i_movemask_i8(wcn_v128i_t vec) {
    v16i8 signs = __msa_srli_b((v16u8)vec.raw, 7);
    unsigned char bytes[16];
    __msa_st_b(signs, bytes, 0);

    int result = 0;
    for (int i = 0; i < 16; i++) {
        result |= (bytes[i] & 1) << i;
    }
    return result;
}

WCN_INLINE int wcn_v128f_movemask(wcn_v128f_t vec) {
    v4i32 signs = __msa_srli_w((v4u32)vec.raw, 31);
    unsigned int words[4];
    __msa_st_w(signs, words, 0);

    int result = 0;
    for (int i = 0; i < 4; i++) {
        result |= (words[i] & 1) << i;
    }
    return result;
}

WCN_INLINE int wcn_v128d_movemask(wcn_v128d_t vec) {
    v2i64 signs = __msa_srli_d((v2u64)vec.raw, 63);
    unsigned long long dwords[2];
    __msa_st_d(signs, dwords, 0);

    int result = 0;
    for (int i = 0; i < 2; i++) {
        result |= (dwords[i] & 1) << i;
    }
    return result;
}

/* ========== Blend/Select Operations ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_blendv(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t mask) {
    wcn_v128f_t result;
    result.raw = (v4f32)__msa_bsel_v((v16u8)mask.raw, (v16u8)a.raw, (v16u8)b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_blendv(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t mask) {
    wcn_v128d_t result;
    result.raw = (v2f64)__msa_bsel_v((v16u8)mask.raw, (v16u8)a.raw, (v16u8)b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_blendv_i8(wcn_v128i_t a, wcn_v128i_t b, wcn_v128i_t mask) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_bsel_v((v16u8)mask.raw, (v16u8)a.raw, (v16u8)b.raw);
    return result;
}

/* ========== Byte Shuffle ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_shuffle_i8(wcn_v128i_t vec, wcn_v128i_t control) {
    wcn_v128i_t result;
    result.raw = (v4i32)__msa_vshf_b((v16i8)control.raw, (v16i8)vec.raw, (v16i8)vec.raw);
    return result;
}

/* ========== Reduction Operations ========== */

WCN_INLINE float wcn_v128f_reduce_add(wcn_v128f_t vec) {
    v4f32 v = vec.raw;
    v4f32 temp = __msa_fadd_w(v, (v4f32)__msa_shf_w((v4i32)v, 0xB1));
    temp = __msa_fadd_w(temp, (v4f32)__msa_shf_w((v4i32)temp, 0x4E));
    return __msa_copy_s_w((v4i32)temp, 0);
}

WCN_INLINE float wcn_v128f_reduce_max(wcn_v128f_t vec) {
    v4f32 v = vec.raw;
    v4f32 temp = __msa_fmax_w(v, (v4f32)__msa_shf_w((v4i32)v, 0xB1));
    temp = __msa_fmax_w(temp, (v4f32)__msa_shf_w((v4i32)temp, 0x4E));
    return __msa_copy_s_w((v4i32)temp, 0);
}

WCN_INLINE float wcn_v128f_reduce_min(wcn_v128f_t vec) {
    v4f32 v = vec.raw;
    v4f32 temp = __msa_fmin_w(v, (v4f32)__msa_shf_w((v4i32)v, 0xB1));
    temp = __msa_fmin_w(temp, (v4f32)__msa_shf_w((v4i32)temp, 0x4E));
    return __msa_copy_s_w((v4i32)temp, 0);
}

#endif /* WCN_MIPS_MSA */

#endif /* WCN_MIPS_MSA_H */
