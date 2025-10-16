#ifndef WCN_POWERPC_VSX_H
#define WCN_POWERPC_VSX_H

#include "../../wcn_types.h"

#ifdef WCN_POWERPC_VSX

#include <altivec.h>

/* PowerPC VSX (Vector Scalar Extension) extends AltiVec with:
 * - Double precision floating-point operations
 * - Additional scalar operations
 * - Enhanced load/store capabilities
 * - More flexible register access
 * Note: Basic AltiVec operations are inherited from wcn_powerpc_altivec.h */

/* ========== Double Precision Load/Store ========== */

WCN_INLINE wcn_v128d_t wcn_vsx_load_f64(const double* ptr) {
    wcn_v128d_t result;
    result.raw = vec_vsx_ld(0, ptr);
    return result;
}

WCN_INLINE void wcn_vsx_store_f64(double* ptr, wcn_v128d_t vec) {
    vec_vsx_st(vec.raw, 0, ptr);
}

WCN_INLINE wcn_v128d_t wcn_vsx_load_f64_aligned(const double* ptr) {
    wcn_v128d_t result;
    result.raw = vec_ld(0, ptr);
    return result;
}

WCN_INLINE void wcn_vsx_store_f64_aligned(double* ptr, wcn_v128d_t vec) {
    vec_st(vec.raw, 0, ptr);
}

/* Splat load - load single element and broadcast */
WCN_INLINE wcn_v128d_t wcn_vsx_splat_f64(const double* ptr) {
    wcn_v128d_t result;
    result.raw = vec_splats(*ptr);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_vsx_splat_f32(const float* ptr) {
    wcn_v128f_t result;
    result.raw = vec_splats(*ptr);
    return result;
}

/* ========== Double Precision Initialization ========== */

WCN_INLINE wcn_v128d_t wcn_vsx_set1_f64(double value) {
    wcn_v128d_t result;
    result.raw = vec_splats(value);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_setzero_f64(void) {
    wcn_v128d_t result;
    result.raw = vec_splats(0.0);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_set_f64(double d1, double d0) {
    wcn_v128d_t result;
    double temp[2] = {d0, d1};
    result.raw = vec_vsx_ld(0, temp);
    return result;
}

/* ========== Double Precision Arithmetic ========== */

WCN_INLINE wcn_v128d_t wcn_vsx_add_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_add(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_sub_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_sub(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_mul_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_mul(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_div_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_div(a.raw, b.raw);
    return result;
}

/* ========== Double Precision FMA ========== */

/* Fused multiply-add: a * b + c */
WCN_INLINE wcn_v128d_t wcn_vsx_fmadd_f64(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = vec_madd(a.raw, b.raw, c.raw);
    return result;
}

/* Fused multiply-subtract: a * b - c */
WCN_INLINE wcn_v128d_t wcn_vsx_fmsub_f64(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = vec_msub(a.raw, b.raw, c.raw);
    return result;
}

/* Negative fused multiply-add: -(a * b) + c */
WCN_INLINE wcn_v128d_t wcn_vsx_fnmadd_f64(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = vec_nmadd(a.raw, b.raw, c.raw);
    return result;
}

/* Negative fused multiply-subtract: -(a * b) - c */
WCN_INLINE wcn_v128d_t wcn_vsx_fnmsub_f64(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = vec_nmsub(a.raw, b.raw, c.raw);
    return result;
}

/* Enhanced single precision FMA */
WCN_INLINE wcn_v128f_t wcn_vsx_fmadd_f32(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = vec_madd(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_vsx_fmsub_f32(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = vec_msub(a.raw, b.raw, c.raw);
    return result;
}

/* ========== Double Precision Comparison ========== */

WCN_INLINE wcn_v128d_t wcn_vsx_cmpeq_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmpeq(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_cmpgt_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmpgt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_cmpge_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmpge(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_cmplt_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmplt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_cmple_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmple(a.raw, b.raw);
    return result;
}

/* ========== Double Precision Min/Max ========== */

WCN_INLINE wcn_v128d_t wcn_vsx_min_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_min(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_max_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_max(a.raw, b.raw);
    return result;
}

/* ========== Double Precision Math Functions ========== */

WCN_INLINE wcn_v128d_t wcn_vsx_abs_f64(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_abs(vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_neg_f64(wcn_v128d_t vec) {
    wcn_v128d_t result;
    vector double zero = vec_splats(0.0);
    result.raw = vec_sub(zero, vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_sqrt_f64(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_sqrt(vec.raw);
    return result;
}

/* Reciprocal estimate (approximate) */
WCN_INLINE wcn_v128d_t wcn_vsx_re_f64(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_re(vec.raw);
    return result;
}

/* Reciprocal square root estimate (approximate) */
WCN_INLINE wcn_v128d_t wcn_vsx_rsqrte_f64(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_rsqrte(vec.raw);
    return result;
}

/* ========== Double Precision Rounding ========== */

WCN_INLINE wcn_v128d_t wcn_vsx_round_f64(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_round(vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_floor_f64(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_floor(vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_ceil_f64(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_ceil(vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_trunc_f64(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_trunc(vec.raw);
    return result;
}

/* ========== Enhanced Single Precision Operations ========== */

/* Enhanced sqrt for single precision */
WCN_INLINE wcn_v128f_t wcn_vsx_sqrt_f32(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vec_sqrt(vec.raw);
    return result;
}

/* Reciprocal estimate */
WCN_INLINE wcn_v128f_t wcn_vsx_re_f32(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vec_re(vec.raw);
    return result;
}

/* Reciprocal square root estimate */
WCN_INLINE wcn_v128f_t wcn_vsx_rsqrte_f32(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vec_rsqrte(vec.raw);
    return result;
}

/* ========== Type Conversions ========== */

/* Convert double to signed int (with rounding toward zero) */
WCN_INLINE vector signed int wcn_vsx_cvt_f64_i32(wcn_v128d_t vec) {
    return vec_cts(vec.raw, 0);
}

/* Convert signed int to double */
WCN_INLINE wcn_v128d_t wcn_vsx_cvt_i32_f64(vector signed int vec) {
    wcn_v128d_t result;
    result.raw = vec_ctf(vec, 0);
    return result;
}

/* Convert double to float (narrowing) */
WCN_INLINE wcn_v128f_t wcn_vsx_cvt_f64_f32(wcn_v128d_t vec) {
    wcn_v128f_t result;
    result.raw = vec_floate(vec.raw);
    return result;
}

/* Convert float to double (widening, even elements) */
WCN_INLINE wcn_v128d_t wcn_vsx_cvt_f32_f64_even(wcn_v128f_t vec) {
    wcn_v128d_t result;
    result.raw = vec_doublee(vec.raw);
    return result;
}

/* Convert float to double (widening, odd elements) */
WCN_INLINE wcn_v128d_t wcn_vsx_cvt_f32_f64_odd(wcn_v128f_t vec) {
    wcn_v128d_t result;
    result.raw = vec_doubleo(vec.raw);
    return result;
}

/* ========== Permute and Shuffle ========== */

/* Permute using control vector */
WCN_INLINE wcn_v128d_t wcn_vsx_perm_f64(wcn_v128d_t a, wcn_v128d_t b, vector unsigned char ctrl) {
    wcn_v128d_t result;
    result.raw = vec_perm(a.raw, b.raw, ctrl);
    return result;
}

/* Splat element from vector (select and broadcast) */
WCN_INLINE wcn_v128d_t wcn_vsx_splat_f64_element(wcn_v128d_t vec, unsigned int index) {
    wcn_v128d_t result;
    result.raw = vec_splat(vec.raw, index);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_vsx_splat_f32_element(wcn_v128f_t vec, unsigned int index) {
    wcn_v128f_t result;
    result.raw = vec_splat(vec.raw, index);
    return result;
}

/* Merge high/low */
WCN_INLINE wcn_v128d_t wcn_vsx_mergeh_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_mergeh(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_mergel_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_mergel(a.raw, b.raw);
    return result;
}

/* ========== Logical Operations (Double) ========== */

WCN_INLINE wcn_v128d_t wcn_vsx_and_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_and(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_or_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_or(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_xor_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_xor(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_vsx_andc_f64(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_andc(a.raw, b.raw);
    return result;
}

/* ========== Select/Blend ========== */

/* Select based on mask (if mask bit set, use b, else use a) */
WCN_INLINE wcn_v128d_t wcn_vsx_sel_f64(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t mask) {
    wcn_v128d_t result;
    result.raw = vec_sel(a.raw, b.raw, (vector unsigned long long)mask.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_vsx_sel_f32(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t mask) {
    wcn_v128f_t result;
    result.raw = vec_sel(a.raw, b.raw, (vector unsigned int)mask.raw);
    return result;
}

/* ========== Test Operations ========== */

/* Test if all elements are zero */
WCN_INLINE int wcn_vsx_all_zero_f64(wcn_v128d_t vec) {
    return vec_all_eq(vec.raw, vec_splats(0.0));
}

/* Test if any element is NaN */
WCN_INLINE int wcn_vsx_any_nan_f64(wcn_v128d_t vec) {
    return vec_any_nan(vec.raw);
}

WCN_INLINE int wcn_vsx_any_nan_f32(wcn_v128f_t vec) {
    return vec_any_nan(vec.raw);
}

/* ========== Extended Integer Operations (64-bit) ========== */

/* 64-bit integer addition */
WCN_INLINE vector signed long long wcn_vsx_add_i64(vector signed long long a, vector signed long long b) {
    return vec_add(a, b);
}

/* 64-bit integer subtraction */
WCN_INLINE vector signed long long wcn_vsx_sub_i64(vector signed long long a, vector signed long long b) {
    return vec_sub(a, b);
}

/* 64-bit integer comparison */
WCN_INLINE vector bool long long wcn_vsx_cmpeq_i64(vector signed long long a, vector signed long long b) {
    return vec_cmpeq(a, b);
}

WCN_INLINE vector bool long long wcn_vsx_cmpgt_i64(vector signed long long a, vector signed long long b) {
    return vec_cmpgt(a, b);
}

/* ========== Horizontal Operations ========== */

/* Sum all elements in double vector */
WCN_INLINE double wcn_vsx_reduce_add_f64(wcn_v128d_t vec) {
    double temp[2];
    vec_vsx_st(vec.raw, 0, temp);
    return temp[0] + temp[1];
}

/* Max of all elements in double vector */
WCN_INLINE double wcn_vsx_reduce_max_f64(wcn_v128d_t vec) {
    double temp[2];
    vec_vsx_st(vec.raw, 0, temp);
    return (temp[0] > temp[1]) ? temp[0] : temp[1];
}

/* Min of all elements in double vector */
WCN_INLINE double wcn_vsx_reduce_min_f64(wcn_v128d_t vec) {
    double temp[2];
    vec_vsx_st(vec.raw, 0, temp);
    return (temp[0] < temp[1]) ? temp[0] : temp[1];
}

#endif /* WCN_POWERPC_VSX */

#endif /* WCN_POWERPC_VSX_H */
