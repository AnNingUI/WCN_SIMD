#ifndef WCN_LOONGARCH_LASX_H
#define WCN_LOONGARCH_LASX_H

#include "../../wcn_types.h"

#ifdef WCN_LOONGARCH_LASX

/* ========== Load/Store Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_load(const void* ptr) {
    wcn_v256i_t result;
    result.raw = __lasx_xvld((const __m256i*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v256i_store(void* ptr, wcn_v256i_t vec) {
    __lasx_xvst(vec.raw, (__m256i*)ptr, 0);
}

WCN_INLINE wcn_v256f_t wcn_v256f_load(const float* ptr) {
    wcn_v256f_t result;
    result.raw = __lasx_xvld((const __m256*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v256f_store(float* ptr, wcn_v256f_t vec) {
    __lasx_xvst(vec.raw, (__m256*)ptr, 0);
}

WCN_INLINE wcn_v256d_t wcn_v256d_load(const double* ptr) {
    wcn_v256d_t result;
    result.raw = __lasx_xvld((const __m256d*)ptr, 0);
    return result;
}

WCN_INLINE void wcn_v256d_store(double* ptr, wcn_v256d_t vec) {
    __lasx_xvst(vec.raw, (__m256d*)ptr, 0);
}

/* ========== Initialization ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_set1_i32(int32_t value) {
    wcn_v256i_t result;
    result.raw = __lasx_xvreplgr2vr_w(value);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_setzero(void) {
    wcn_v256i_t result;
    result.raw = __lasx_xvldi(0);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_set1(float value) {
    wcn_v256f_t result;
    int32_t i = *((int32_t*)&value);
    result.raw = (__m256)__lasx_xvreplgr2vr_w(i);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_setzero(void) {
    wcn_v256f_t result;
    result.raw = (__m256)__lasx_xvldi(0);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_set1(double value) {
    wcn_v256d_t result;
    int64_t i = *((int64_t*)&value);
    result.raw = (__m256d)__lasx_xvreplgr2vr_d(i);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_setzero(void) {
    wcn_v256d_t result;
    result.raw = (__m256d)__lasx_xvldi(0);
    return result;
}

/* ========== Arithmetic Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_add_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvadd_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_sub_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvsub_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_mullo_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvmul_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_add(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = __lasx_xvfadd_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_sub(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = __lasx_xvfsub_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_mul(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = __lasx_xvfmul_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_div(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = __lasx_xvfdiv_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_add(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = __lasx_xvfadd_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_sub(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = __lasx_xvfsub_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_mul(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = __lasx_xvfmul_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_div(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = __lasx_xvfdiv_d(a.raw, b.raw);
    return result;
}

/* ========== FMA (Fused Multiply-Add) ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_fmadd(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t c) {
    wcn_v256f_t result;
    result.raw = __lasx_xvfmadd_s(a.raw, b.raw, c.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_fmadd(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t c) {
    wcn_v256d_t result;
    result.raw = __lasx_xvfmadd_d(a.raw, b.raw, c.raw);
    return result;
}

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_and(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvand_v(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_or(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvor_v(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_xor(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvxor_v(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_andnot(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvandn_v(a.raw, b.raw);
    return result;
}

/* ========== Comparison Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_cmpeq_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvseq_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_cmpgt_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvslt_w(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_cmpeq(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = (__m256)__lasx_xvfcmp_ceq_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_cmplt(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = (__m256)__lasx_xvfcmp_clt_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_cmple(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = (__m256)__lasx_xvfcmp_cle_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_cmpgt(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = (__m256)__lasx_xvfcmp_clt_s(b.raw, a.raw);
    return result;
}

/* ========== Min/Max Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_max_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvmax_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_min_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvmin_w(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_max(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = __lasx_xvfmax_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_min(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = __lasx_xvfmin_s(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_max(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = __lasx_xvfmax_d(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_min(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = __lasx_xvfmin_d(a.raw, b.raw);
    return result;
}

/* ========== Square Root ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_sqrt(wcn_v256f_t vec) {
    wcn_v256f_t result;
    result.raw = __lasx_xvfsqrt_s(vec.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_sqrt(wcn_v256d_t vec) {
    wcn_v256d_t result;
    result.raw = __lasx_xvfsqrt_d(vec.raw);
    return result;
}

/* ========== Conversions ========== */

WCN_INLINE wcn_v256f_t wcn_v256i_to_v256f(wcn_v256i_t vec) {
    wcn_v256f_t result;
    result.raw = __lasx_xvffint_s_w(vec.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256f_to_v256i(wcn_v256f_t vec) {
    wcn_v256i_t result;
    result.raw = __lasx_xvftint_w_s(vec.raw);
    return result;
}

/* ========== Integer Multiplication ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_mullo_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvmul_h(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_mulhi_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    __m256i prod_even = __lasx_xvmulwev_w_h(a.raw, b.raw);
    __m256i prod_odd = __lasx_xvmulwod_w_h(a.raw, b.raw);
    prod_even = __lasx_xvsrli_w(prod_even, 16);
    prod_odd = __lasx_xvsrli_w(prod_odd, 16);
    result.raw = __lasx_xvpackev_h(prod_odd, prod_even);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_mulhi_u16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    __m256i prod_even = __lasx_xvmulwev_w_hu(a.raw, b.raw);
    __m256i prod_odd = __lasx_xvmulwod_w_hu(a.raw, b.raw);
    prod_even = __lasx_xvsrli_w(prod_even, 16);
    prod_odd = __lasx_xvsrli_w(prod_odd, 16);
    result.raw = __lasx_xvpackev_h(prod_odd, prod_even);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_mul_u32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvmulwev_d_wu(a.raw, b.raw);
    return result;
}

/* ========== Pack/Unpack Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_packs_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvsat_w(a.raw, 15);
    __m256i b_sat = __lasx_xvsat_w(b.raw, 15);
    result.raw = __lasx_xvpackev_h(b_sat, result.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_packs_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvsat_h(a.raw, 7);
    __m256i b_sat = __lasx_xvsat_h(b.raw, 7);
    result.raw = __lasx_xvpackev_b(b_sat, result.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_packus_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvsat_hu(a.raw, 7);
    __m256i b_sat = __lasx_xvsat_hu(b.raw, 7);
    result.raw = __lasx_xvpackev_b(b_sat, result.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_packus_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvsat_wu(a.raw, 15);
    __m256i b_sat = __lasx_xvsat_wu(b.raw, 15);
    result.raw = __lasx_xvpackev_h(b_sat, result.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpacklo_i8(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvilvl_b(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpackhi_i8(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvilvh_b(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpacklo_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvilvl_h(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpackhi_i16(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvilvh_h(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpacklo_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvilvl_w(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpackhi_i32(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvilvh_w(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpacklo_i64(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvilvl_d(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_unpackhi_i64(wcn_v256i_t a, wcn_v256i_t b) {
    wcn_v256i_t result;
    result.raw = __lasx_xvilvh_d(b.raw, a.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_unpacklo(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = (__m256)__lasx_xvilvl_w((__m256i)b.raw, (__m256i)a.raw);
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_unpackhi(wcn_v256f_t a, wcn_v256f_t b) {
    wcn_v256f_t result;
    result.raw = (__m256)__lasx_xvilvh_w((__m256i)b.raw, (__m256i)a.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_unpacklo(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = (__m256d)__lasx_xvilvl_d((__m256i)b.raw, (__m256i)a.raw);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_unpackhi(wcn_v256d_t a, wcn_v256d_t b) {
    wcn_v256d_t result;
    result.raw = (__m256d)__lasx_xvilvh_d((__m256i)b.raw, (__m256i)a.raw);
    return result;
}

/* ========== Movemask Operations ========== */

WCN_INLINE int wcn_v256i_movemask_i8(wcn_v256i_t vec) {
    __m256i sign_bits = __lasx_xvsrli_b(vec.raw, 7);
    unsigned char bytes[32];
    __lasx_xvst(sign_bits, bytes, 0);

    int result = 0;
    for (int i = 0; i < 32; i++) {
        result |= (bytes[i] & 1) << i;
    }
    return result;
}

WCN_INLINE int wcn_v256f_movemask(wcn_v256f_t vec) {
    __m256i bits = (__m256i)vec.raw;
    __m256i sign_bits = __lasx_xvsrli_w(bits, 31);

    unsigned int words[8];
    __lasx_xvst(sign_bits, words, 0);

    int result = 0;
    for (int i = 0; i < 8; i++) {
        result |= (words[i] & 1) << i;
    }
    return result;
}

WCN_INLINE int wcn_v256d_movemask(wcn_v256d_t vec) {
    __m256i bits = (__m256i)vec.raw;
    __m256i sign_bits = __lasx_xvsrli_d(bits, 63);

    unsigned long long dwords[4];
    __lasx_xvst(sign_bits, dwords, 0);

    int result = 0;
    for (int i = 0; i < 4; i++) {
        result |= (dwords[i] & 1) << i;
    }
    return result;
}

/* ========== Blend/Select Operations ========== */

WCN_INLINE wcn_v256f_t wcn_v256f_blendv(wcn_v256f_t a, wcn_v256f_t b, wcn_v256f_t mask) {
    wcn_v256f_t result;
    __m256i m = (__m256i)mask.raw;
    result.raw = (__m256)__lasx_xvbitsel_v((__m256i)a.raw, (__m256i)b.raw, m);
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_blendv(wcn_v256d_t a, wcn_v256d_t b, wcn_v256d_t mask) {
    wcn_v256d_t result;
    __m256i m = (__m256i)mask.raw;
    result.raw = (__m256d)__lasx_xvbitsel_v((__m256i)a.raw, (__m256i)b.raw, m);
    return result;
}

WCN_INLINE wcn_v256i_t wcn_v256i_blendv_i8(wcn_v256i_t a, wcn_v256i_t b, wcn_v256i_t mask) {
    wcn_v256i_t result;
    result.raw = __lasx_xvbitsel_v(a.raw, b.raw, mask.raw);
    return result;
}

/* ========== Byte Shuffle ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_shuffle_i8(wcn_v256i_t vec, wcn_v256i_t control) {
    wcn_v256i_t result;
    result.raw = __lasx_xvshuf_b(control.raw, vec.raw, vec.raw);
    return result;
}

#endif /* WCN_LOONGARCH_LASX */

#endif /* WCN_LOONGARCH_LASX_H */
