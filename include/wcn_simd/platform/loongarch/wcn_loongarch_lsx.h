#ifndef WCN_LOONGARCH_LSX_H
#define WCN_LOONGARCH_LSX_H

#include "../../wcn_types.h"

#ifdef WCN_LOONGARCH_LSX

/* ========== Load/Store Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_load(const void* ptr) {
    wcn_v128i_t result;
    result.raw = __lsx_vld((const __m128i*)ptr, 0);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_load_aligned(const void* ptr) {
    wcn_v128i_t result;
    result.raw = __lsx_vld((const __m128i*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v128i_store(void* ptr, wcn_v128i_t vec) {
    __lsx_vst(vec.raw, (__m128i*)ptr, 0);
}

WCN_INLINE void wcn_v128i_store_aligned(void* ptr, wcn_v128i_t vec) {
    __lsx_vst(vec.raw, (__m128i*)ptr, 0);
}

WCN_INLINE wcn_v128f_t wcn_v128f_load(const float* ptr) {
    wcn_v128f_t result;
    result.raw = __lsx_vld((const __m128*)ptr, 0);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_load_aligned(const float* ptr) {
    wcn_v128f_t result;
    result.raw = __lsx_vld((const __m128*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v128f_store(float* ptr, wcn_v128f_t vec) {
    __lsx_vst(vec.raw, (__m128*)ptr, 0);
}

WCN_INLINE void wcn_v128f_store_aligned(float* ptr, wcn_v128f_t vec) {
    __lsx_vst(vec.raw, (__m128*)ptr, 0);
}

WCN_INLINE wcn_v128d_t wcn_v128d_load(const double* ptr) {
    wcn_v128d_t result;
    result.raw = __lsx_vld((const __m128d*)ptr, 0);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_load_aligned(const double* ptr) {
    wcn_v128d_t result;
    result.raw = __lsx_vld((const __m128d*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v128d_store(double* ptr, wcn_v128d_t vec) {
    __lsx_vst(vec.raw, (__m128d*)ptr, 0);
}

WCN_INLINE void wcn_v128d_store_aligned(double* ptr, wcn_v128d_t vec) {
    __lsx_vst(vec.raw, (__m128d*)ptr, 0);
}

/* ========== Initialization ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i8(int8_t value) {
    wcn_v128i_t result;
    result.raw = __lsx_vreplgr2vr_b(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i16(int16_t value) {
    wcn_v128i_t result;
    result.raw = __lsx_vreplgr2vr_h(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i32(int32_t value) {
    wcn_v128i_t result;
    result.raw = __lsx_vreplgr2vr_w(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i64(int64_t value) {
    wcn_v128i_t result;
    result.raw = __lsx_vreplgr2vr_d(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_setzero(void) {
    wcn_v128i_t result;
    result.raw = __lsx_vldi(0);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_set1(float value) {
    wcn_v128f_t result;
    int32_t i = *((int32_t*)&value);
    result.raw = (__m128)__lsx_vreplgr2vr_w(i);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_setzero(void) {
    wcn_v128f_t result;
    result.raw = (__m128)__lsx_vldi(0);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_set1(double value) {
    wcn_v128d_t result;
    int64_t i = *((int64_t*)&value);
    result.raw = (__m128d)__lsx_vreplgr2vr_d(i);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_setzero(void) {
    wcn_v128d_t result;
    result.raw = (__m128d)__lsx_vldi(0);
    return result;
}

/* ========== Arithmetic Operations ========== */

/* 8-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vadd_b(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsub_b(a.raw, b.raw);
    return result;
}

/* 16-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vadd_h(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsub_h(a.raw, b.raw);
    return result;
}

/* 32-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vadd_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsub_w(a.raw, b.raw);
    return result;
}

/* 64-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vadd_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsub_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_mul_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmul_w(a.raw, b.raw);
    return result;
}

/* ========== Saturating Arithmetic ========== */

/* 8-bit signed saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsadd_b(a.raw, b.raw);
    return result;
}

/* 8-bit signed saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vssub_b(a.raw, b.raw);
    return result;
}

/* 8-bit unsigned saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsadd_bu(a.raw, b.raw);
    return result;
}

/* 8-bit unsigned saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vssub_bu(a.raw, b.raw);
    return result;
}

/* 16-bit signed saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsadd_h(a.raw, b.raw);
    return result;
}

/* 16-bit signed saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vssub_h(a.raw, b.raw);
    return result;
}

/* 16-bit unsigned saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsadd_hu(a.raw, b.raw);
    return result;
}

/* 16-bit unsigned saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vssub_hu(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_add(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __lsx_vfadd_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_sub(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __lsx_vfsub_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_mul(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __lsx_vfmul_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_div(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __lsx_vfdiv_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_add(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __lsx_vfadd_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sub(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __lsx_vfsub_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_mul(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __lsx_vfmul_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_div(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __lsx_vfdiv_d(a.raw, b.raw);
    return result;
}

/* ========== FMA (Fused Multiply-Add) ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_fmadd(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = __lsx_vfmadd_s(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_fmadd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = __lsx_vfmadd_d(a.raw, b.raw, c.raw);
    return result;
}

/* ========== Shift Operations ========== */

/* 16-bit shifts */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __lsx_vsll_h(a.raw, count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __lsx_vsrl_h(a.raw, count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sra_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __lsx_vsra_h(a.raw, count.raw);
    return result;
}

/* 16-bit immediate shifts */
#define wcn_v128i_slli_i16(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vslli_h((vec).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srli_i16(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vsrli_h((vec).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srai_i16(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vsrai_h((vec).raw, (imm)); \
    _result; \
})

/* 32-bit shifts */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __lsx_vsll_w(a.raw, count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __lsx_vsrl_w(a.raw, count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sra_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __lsx_vsra_w(a.raw, count.raw);
    return result;
}

/* 32-bit immediate shifts */
#define wcn_v128i_slli_i32(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vslli_w((vec).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srli_i32(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vsrli_w((vec).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srai_i32(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vsrai_w((vec).raw, (imm)); \
    _result; \
})

/* 64-bit shifts */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __lsx_vsll_d(a.raw, count.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    result.raw = __lsx_vsrl_d(a.raw, count.raw);
    return result;
}

/* 64-bit immediate shifts */
#define wcn_v128i_slli_i64(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vslli_d((vec).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srli_i64(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vsrli_d((vec).raw, (imm)); \
    _result; \
})

/* 128-bit byte shifts */
#define wcn_v128i_slli_si128(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vbsll_v((vec).raw, (imm)); \
    _result; \
})

#define wcn_v128i_srli_si128(vec, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vbsrl_v((vec).raw, (imm)); \
    _result; \
})

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_and(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vand_v(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_or(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vor_v(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_xor(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vxor_v(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_andnot(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vandn_v(a.raw, b.raw);
    return result;
}

/* ========== Comparison Operations ========== */

/* 8-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vseq_b(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vslt_b(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vslt_b(a.raw, b.raw);
    return result;
}

/* 16-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vseq_h(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vslt_h(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vslt_h(a.raw, b.raw);
    return result;
}

/* 32-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vseq_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vslt_w(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vslt_w(a.raw, b.raw);
    return result;
}

/* Float comparisons */
WCN_INLINE wcn_v128f_t wcn_v128f_cmpeq(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (__m128)__lsx_vfcmp_ceq_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmplt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (__m128)__lsx_vfcmp_clt_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmple(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (__m128)__lsx_vfcmp_cle_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpgt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (__m128)__lsx_vfcmp_clt_s(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpge(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (__m128)__lsx_vfcmp_cle_s(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpneq(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (__m128)__lsx_vfcmp_cne_s(a.raw, b.raw);
    return result;
}

/* Double comparisons */
WCN_INLINE wcn_v128d_t wcn_v128d_cmpeq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (__m128d)__lsx_vfcmp_ceq_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmplt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (__m128d)__lsx_vfcmp_clt_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmple(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (__m128d)__lsx_vfcmp_cle_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpgt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (__m128d)__lsx_vfcmp_clt_d(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpge(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (__m128d)__lsx_vfcmp_cle_d(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpneq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (__m128d)__lsx_vfcmp_cne_d(a.raw, b.raw);
    return result;
}

/* ========== Min/Max Operations ========== */

/* 8-bit signed min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmin_b(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmax_b(a.raw, b.raw);
    return result;
}

/* 8-bit unsigned min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmin_bu(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmax_bu(a.raw, b.raw);
    return result;
}

/* 16-bit signed min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmin_h(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmax_h(a.raw, b.raw);
    return result;
}

/* 32-bit signed min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_max_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmax_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmin_w(a.raw, b.raw);
    return result;
}

/* Float min/max */
WCN_INLINE wcn_v128f_t wcn_v128f_max(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __lsx_vfmax_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_min(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = __lsx_vfmin_s(a.raw, b.raw);
    return result;
}

/* Double min/max */
WCN_INLINE wcn_v128d_t wcn_v128d_max(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __lsx_vfmax_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_min(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = __lsx_vfmin_d(a.raw, b.raw);
    return result;
}

/* ========== Square Root ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_sqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = __lsx_vfsqrt_s(vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sqrt(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = __lsx_vfsqrt_d(vec.raw);
    return result;
}

/* ========== Reciprocal Approximations ========== */

/* Reciprocal approximation (1/x) */
WCN_INLINE wcn_v128f_t wcn_v128f_rcp(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = __lsx_vfrecip_s(vec.raw);
    return result;
}

/* Reciprocal square root approximation (1/sqrt(x)) */
WCN_INLINE wcn_v128f_t wcn_v128f_rsqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = __lsx_vfrsqrt_s(vec.raw);
    return result;
}

/* ========== Absolute Value Operations ========== */

/* 8-bit absolute value */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i8(wcn_v128i_t vec) {
    wcn_v128i_t result;
    /* LSX uses vsigncov for abs: sign-change-on-negative */
    result.raw = __lsx_vsigncov_b(vec.raw, vec.raw);
    return result;
}

/* 16-bit absolute value */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i16(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = __lsx_vsigncov_h(vec.raw, vec.raw);
    return result;
}

/* 32-bit absolute value */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i32(wcn_v128i_t vec) {
    wcn_v128i_t result;
    result.raw = __lsx_vsigncov_w(vec.raw, vec.raw);
    return result;
}

/* Float absolute value (clear sign bit) */
WCN_INLINE wcn_v128f_t wcn_v128f_abs(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Clear sign bit by ANDing with 0x7FFFFFFF */
    __m128i mask = __lsx_vreplgr2vr_w(0x7FFFFFFF);
    result.raw = (__m128)__lsx_vand_v((__m128i)vec.raw, mask);
    return result;
}

/* Double absolute value (clear sign bit) */
WCN_INLINE wcn_v128d_t wcn_v128d_abs(wcn_v128d_t vec) {
    wcn_v128d_t result;
    /* Clear sign bit by ANDing with 0x7FFFFFFFFFFFFFFF */
    __m128i mask = __lsx_vreplgr2vr_d(0x7FFFFFFFFFFFFFFFLL);
    result.raw = (__m128d)__lsx_vand_v((__m128i)vec.raw, mask);
    return result;
}

/* ========== Negation Operations ========== */

/* 8-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i8(wcn_v128i_t a) {
    wcn_v128i_t result;
    result.raw = __lsx_vneg_b(a.raw);
    return result;
}

/* 16-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i16(wcn_v128i_t a) {
    wcn_v128i_t result;
    result.raw = __lsx_vneg_h(a.raw);
    return result;
}

/* 32-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i32(wcn_v128i_t a) {
    wcn_v128i_t result;
    result.raw = __lsx_vneg_w(a.raw);
    return result;
}

/* 64-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i64(wcn_v128i_t a) {
    wcn_v128i_t result;
    result.raw = __lsx_vneg_d(a.raw);
    return result;
}

/* Float negation */
WCN_INLINE wcn_v128f_t wcn_v128f_neg(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* XOR with sign bit mask 0x80000000 */
    __m128i sign_mask = __lsx_vreplgr2vr_w(0x80000000);
    result.raw = (__m128)__lsx_vxor_v((__m128i)vec.raw, sign_mask);
    return result;
}

/* Double negation */
WCN_INLINE wcn_v128d_t wcn_v128d_neg(wcn_v128d_t vec) {
    wcn_v128d_t result;
    /* XOR with sign bit mask 0x8000000000000000 */
    __m128i sign_mask = __lsx_vreplgr2vr_d(0x8000000000000000LL);
    result.raw = (__m128d)__lsx_vxor_v((__m128i)vec.raw, sign_mask);
    return result;
}

/* ========== Broadcast Operations ========== */

/* Load scalar and broadcast to all 32-bit integer lanes */
WCN_INLINE wcn_v128i_t wcn_v128i_broadcast_i32(const int32_t* ptr) {
    wcn_v128i_t result;
    result.raw = __lsx_vreplgr2vr_w(*ptr);
    return result;
}

/* Load scalar and broadcast to all float lanes */
WCN_INLINE wcn_v128f_t wcn_v128f_broadcast(const float* ptr) {
    wcn_v128f_t result;
    int32_t i = *((int32_t*)ptr);
    result.raw = (__m128)__lsx_vreplgr2vr_w(i);
    return result;
}

/* Load scalar and broadcast to all double lanes */
WCN_INLINE wcn_v128d_t wcn_v128d_broadcast(const double* ptr) {
    wcn_v128d_t result;
    int64_t i = *((int64_t*)ptr);
    result.raw = (__m128d)__lsx_vreplgr2vr_d(i);
    return result;
}

/* ========== Conversions ========== */

WCN_INLINE wcn_v128f_t wcn_v128i_to_v128f(wcn_v128i_t vec) {
    wcn_v128f_t result;
    result.raw = __lsx_vffint_s_w(vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128f_to_v128i(wcn_v128f_t vec) {
    wcn_v128i_t result;
    result.raw = __lsx_vftint_w_s(vec.raw);
    return result;
}

/* ========== Integer Multiplication ========== */

/* 16-bit integer multiplication (low 16 bits) */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmul_h(a.raw, b.raw);
    return result;
}

/* 16-bit integer multiplication (high 16 bits, signed) */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* LSX has mulwev (multiply even) and mulwod (multiply odd) for widening multiply */
    __m128i prod_even = __lsx_vmulwev_w_h(a.raw, b.raw);
    __m128i prod_odd = __lsx_vmulwod_w_h(a.raw, b.raw);

    /* Shift right by 16 to get high 16 bits */
    prod_even = __lsx_vsrli_w(prod_even, 16);
    prod_odd = __lsx_vsrli_w(prod_odd, 16);

    /* Pack back to 16-bit (interleave even and odd) */
    result.raw = __lsx_vpackev_h(prod_odd, prod_even);
    return result;
}

/* 16-bit integer multiplication (high 16 bits, unsigned) */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    __m128i prod_even = __lsx_vmulwev_w_hu(a.raw, b.raw);
    __m128i prod_odd = __lsx_vmulwod_w_hu(a.raw, b.raw);

    prod_even = __lsx_vsrli_w(prod_even, 16);
    prod_odd = __lsx_vsrli_w(prod_odd, 16);

    result.raw = __lsx_vpackev_h(prod_odd, prod_even);
    return result;
}

/* 32-bit integer multiplication (low 32 bits) */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vmul_w(a.raw, b.raw);
    return result;
}

/* Multiply packed unsigned 32-bit integers, produce 64-bit results */
WCN_INLINE wcn_v128i_t wcn_v128i_mul_u32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* Multiply even elements to 64-bit */
    result.raw = __lsx_vmulwev_d_wu(a.raw, b.raw);
    return result;
}

/* ========== Pack/Unpack Operations ========== */

/* Pack 32-bit signed integers to 16-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsat_w(a.raw, 15);
    __m128i b_sat = __lsx_vsat_w(b.raw, 15);
    result.raw = __lsx_vpackev_h(b_sat, result.raw);
    return result;
}

/* Pack 16-bit signed integers to 8-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsat_h(a.raw, 7);
    __m128i b_sat = __lsx_vsat_h(b.raw, 7);
    result.raw = __lsx_vpackev_b(b_sat, result.raw);
    return result;
}

/* Pack 16-bit signed integers to 8-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsat_hu(a.raw, 7);
    __m128i b_sat = __lsx_vsat_hu(b.raw, 7);
    result.raw = __lsx_vpackev_b(b_sat, result.raw);
    return result;
}

/* Pack 32-bit signed integers to 16-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vsat_wu(a.raw, 15);
    __m128i b_sat = __lsx_vsat_wu(b.raw, 15);
    result.raw = __lsx_vpackev_h(b_sat, result.raw);
    return result;
}

/* Unpack and interleave low 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vilvl_b(b.raw, a.raw);
    return result;
}

/* Unpack and interleave high 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vilvh_b(b.raw, a.raw);
    return result;
}

/* Unpack and interleave low 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vilvl_h(b.raw, a.raw);
    return result;
}

/* Unpack and interleave high 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vilvh_h(b.raw, a.raw);
    return result;
}

/* Unpack and interleave low 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vilvl_w(b.raw, a.raw);
    return result;
}

/* Unpack and interleave high 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vilvh_w(b.raw, a.raw);
    return result;
}

/* Unpack and interleave low 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vilvl_d(b.raw, a.raw);
    return result;
}

/* Unpack and interleave high 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = __lsx_vilvh_d(b.raw, a.raw);
    return result;
}

/* Unpack floats */
WCN_INLINE wcn_v128f_t wcn_v128f_unpacklo(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (__m128)__lsx_vilvl_w((__m128i)b.raw, (__m128i)a.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_unpackhi(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (__m128)__lsx_vilvh_w((__m128i)b.raw, (__m128i)a.raw);
    return result;
}

/* Unpack doubles */
WCN_INLINE wcn_v128d_t wcn_v128d_unpacklo(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (__m128d)__lsx_vilvl_d((__m128i)b.raw, (__m128i)a.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_unpackhi(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (__m128d)__lsx_vilvh_d((__m128i)b.raw, (__m128i)a.raw);
    return result;
}

/* ========== Extract/Insert Operations ========== */

/* Extract 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_v128i_extract_i16(vec, index) __lsx_vpickve2gr_h((vec).raw, (index))

/* Insert 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_v128i_insert_i16(vec, val, index) ({ \
    wcn_v128i_t _result; \
    _result.raw = __lsx_vinsgr2vr_h((vec).raw, (val), (index)); \
    _result; \
})

/* ========== Movemask Operations ========== */

/* Create mask from most significant bit of each 8-bit element */
WCN_INLINE int wcn_v128i_movemask_i8(wcn_v128i_t vec) {
    /* LSX doesn't have direct movemask, simulate it */
    __m128i sign_bits = __lsx_vsrli_b(vec.raw, 7);
    unsigned char bytes[16];
    __lsx_vst(sign_bits, bytes, 0);

    int result = 0;
    for (int i = 0; i < 16; i++) {
        result |= (bytes[i] & 1) << i;
    }
    return result;
}

/* Create mask from most significant bit of each float */
WCN_INLINE int wcn_v128f_movemask(wcn_v128f_t vec) {
    __m128i bits = (__m128i)vec.raw;
    __m128i sign_bits = __lsx_vsrli_w(bits, 31);

    unsigned int words[4];
    __lsx_vst(sign_bits, words, 0);

    int result = 0;
    for (int i = 0; i < 4; i++) {
        result |= (words[i] & 1) << i;
    }
    return result;
}

/* Create mask from most significant bit of each double */
WCN_INLINE int wcn_v128d_movemask(wcn_v128d_t vec) {
    __m128i bits = (__m128i)vec.raw;
    __m128i sign_bits = __lsx_vsrli_d(bits, 63);

    unsigned long long dwords[2];
    __lsx_vst(sign_bits, dwords, 0);

    int result = 0;
    for (int i = 0; i < 2; i++) {
        result |= (dwords[i] & 1) << i;
    }
    return result;
}

/* ========== Blend/Select Operations ========== */

/* Variable blend floats (blends based on sign bit of mask) */
WCN_INLINE wcn_v128f_t wcn_v128f_blendv(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t mask) {
    wcn_v128f_t result;
    __m128i m = (__m128i)mask.raw;
    result.raw = (__m128)__lsx_vbitsel_v((__m128i)a.raw, (__m128i)b.raw, m);
    return result;
}

/* Variable blend doubles */
WCN_INLINE wcn_v128d_t wcn_v128d_blendv(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t mask) {
    wcn_v128d_t result;
    __m128i m = (__m128i)mask.raw;
    result.raw = (__m128d)__lsx_vbitsel_v((__m128i)a.raw, (__m128i)b.raw, m);
    return result;
}

/* Variable blend 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_blendv_i8(wcn_v128i_t a, wcn_v128i_t b, wcn_v128i_t mask) {
    wcn_v128i_t result;
    result.raw = __lsx_vbitsel_v(a.raw, b.raw, mask.raw);
    return result;
}

/* ========== Byte Shuffle ========== */

/* Shuffle bytes according to indices in control mask */
WCN_INLINE wcn_v128i_t wcn_v128i_shuffle_i8(wcn_v128i_t vec, wcn_v128i_t control) {
    wcn_v128i_t result;
    result.raw = __lsx_vshuf_b(control.raw, vec.raw, vec.raw);
    return result;
}

/* ========== Reduction Operations ========== */

WCN_INLINE float wcn_v128f_reduce_add(wcn_v128f_t vec) {
    /* Horizontal add all 4 elements */
    __m128 v = vec.raw;
    __m128 temp = __lsx_vfadd_s(v, __lsx_vshuf4i_w(v, 0xB1)); /* swap adjacent pairs */
    temp = __lsx_vfadd_s(temp, __lsx_vshuf4i_w(temp, 0x4E)); /* swap halves */

    float result;
    __lsx_vstelm_w(temp, &result, 0, 0);
    return result;
}

WCN_INLINE float wcn_v128f_reduce_max(wcn_v128f_t vec) {
    __m128 v = vec.raw;
    __m128 temp = __lsx_vfmax_s(v, __lsx_vshuf4i_w(v, 0xB1));
    temp = __lsx_vfmax_s(temp, __lsx_vshuf4i_w(temp, 0x4E));

    float result;
    __lsx_vstelm_w(temp, &result, 0, 0);
    return result;
}

WCN_INLINE float wcn_v128f_reduce_min(wcn_v128f_t vec) {
    __m128 v = vec.raw;
    __m128 temp = __lsx_vfmin_s(v, __lsx_vshuf4i_w(v, 0xB1));
    temp = __lsx_vfmin_s(temp, __lsx_vshuf4i_w(temp, 0x4E));

    float result;
    __lsx_vstelm_w(temp, &result, 0, 0);
    return result;
}

#endif /* WCN_LOONGARCH_LSX */

#endif /* WCN_LOONGARCH_LSX_H */
