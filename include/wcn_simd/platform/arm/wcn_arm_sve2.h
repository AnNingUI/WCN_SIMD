#ifndef WCN_ARM_SVE2_H
#define WCN_ARM_SVE2_H

#include "../../wcn_types.h"

#ifdef WCN_ARM_SVE2

#include <arm_sve.h>

/* ARM SVE2 extends SVE with additional instructions for:
 * - Integer multiply-add operations
 * - Narrowing and widening operations
 * - Complex arithmetic
 * - Polynomial arithmetic
 * - Bit manipulation
 * - Match and histogram operations
 * Note: Basic SVE operations are inherited from wcn_arm_sve.h */

/* ========== Integer Multiply-Add/Subtract ========== */

/* Multiply-add: a + (b * c) */
WCN_INLINE svint32_t wcn_sve2_mla_i32(svint32_t a, svint32_t b, svint32_t c) {
    return svmla_s32_x(svptrue_b32(), a, b, c);
}

/* Multiply-subtract: a - (b * c) */
WCN_INLINE svint32_t wcn_sve2_mls_i32(svint32_t a, svint32_t b, svint32_t c) {
    return svmls_s32_x(svptrue_b32(), a, b, c);
}

/* Multiply-add long (bottom): a + (b[0:n] * c[0:n]) */
WCN_INLINE svint64_t wcn_sve2_mlalb_i64(svint64_t a, svint32_t b, svint32_t c) {
    return svmlalb_s64(a, b, c);
}

/* Multiply-add long (top): a + (b[n:2n] * c[n:2n]) */
WCN_INLINE svint64_t wcn_sve2_mlalt_i64(svint64_t a, svint32_t b, svint32_t c) {
    return svmlalt_s64(a, b, c);
}

/* Multiply-subtract long (bottom) */
WCN_INLINE svint64_t wcn_sve2_mlslb_i64(svint64_t a, svint32_t b, svint32_t c) {
    return svmlslb_s64(a, b, c);
}

/* Multiply-subtract long (top) */
WCN_INLINE svint64_t wcn_sve2_mlslt_i64(svint64_t a, svint32_t b, svint32_t c) {
    return svmlslt_s64(a, b, c);
}

/* ========== Widening Multiply ========== */

/* Multiply long (bottom half) - signed */
WCN_INLINE svint64_t wcn_sve2_mullb_i64(svint32_t a, svint32_t b) {
    return svmullb_s64(a, b);
}

/* Multiply long (top half) - signed */
WCN_INLINE svint64_t wcn_sve2_mullt_i64(svint32_t a, svint32_t b) {
    return svmullt_s64(a, b);
}

/* Multiply long (bottom half) - unsigned */
WCN_INLINE svuint64_t wcn_sve2_mullb_u64(svuint32_t a, svuint32_t b) {
    return svmullb_u64(a, b);
}

/* Multiply long (top half) - unsigned */
WCN_INLINE svuint64_t wcn_sve2_mullt_u64(svuint32_t a, svuint32_t b) {
    return svmullt_u64(a, b);
}

/* ========== Narrowing Operations ========== */

/* Narrow from 32-bit to 16-bit (bottom half) */
WCN_INLINE svint16_t wcn_sve2_narrow_i16_b(svint32_t a) {
    return svqxtnb_s16(a);
}

/* Narrow from 32-bit to 16-bit (top half) */
WCN_INLINE svint16_t wcn_sve2_narrow_i16_t(svint16_t r, svint32_t a) {
    return svqxtnt_s16(r, a);
}

/* Unsigned narrow with saturation (bottom) */
WCN_INLINE svuint16_t wcn_sve2_qnarrow_u16_b(svuint32_t a) {
    return svqxtnb_u16(a);
}

/* Unsigned narrow with saturation (top) */
WCN_INLINE svuint16_t wcn_sve2_qnarrow_u16_t(svuint16_t r, svuint32_t a) {
    return svqxtnt_u16(r, a);
}

/* ========== Widening Add/Subtract ========== */

/* Add wide (bottom): a + extend(b[0:n]) */
WCN_INLINE svint32_t wcn_sve2_addwb_i32(svint32_t a, svint16_t b) {
    return svaddwb_s32(a, b);
}

/* Add wide (top): a + extend(b[n:2n]) */
WCN_INLINE svint32_t wcn_sve2_addwt_i32(svint32_t a, svint16_t b) {
    return svaddwt_s32(a, b);
}

/* Subtract wide (bottom) */
WCN_INLINE svint32_t wcn_sve2_subwb_i32(svint32_t a, svint16_t b) {
    return svsubwb_s32(a, b);
}

/* Subtract wide (top) */
WCN_INLINE svint32_t wcn_sve2_subwt_i32(svint32_t a, svint16_t b) {
    return svsubwt_s32(a, b);
}

/* ========== Polynomial Arithmetic ========== */

/* Polynomial multiply long (bottom) */
WCN_INLINE svuint16_t wcn_sve2_pmullb_u16(svuint8_t a, svuint8_t b) {
    return svpmullb_u16(a, b);
}

/* Polynomial multiply long (top) */
WCN_INLINE svuint16_t wcn_sve2_pmullt_u16(svuint8_t a, svuint8_t b) {
    return svpmullt_u16(a, b);
}

/* Polynomial multiply (pair) */
WCN_INLINE svuint64_t wcn_sve2_pmullb_pair_u64(svuint32_t a, svuint32_t b) {
    return svpmullb_pair_u64(a, b);
}

/* ========== Complex Arithmetic ========== */

/* Complex add with rotation */
WCN_INLINE svfloat32_t wcn_sve2_cadd_f32_90(svfloat32_t a, svfloat32_t b) {
    return svcadd_f32_x(svptrue_b32(), a, b, 90);
}

WCN_INLINE svfloat32_t wcn_sve2_cadd_f32_270(svfloat32_t a, svfloat32_t b) {
    return svcadd_f32_x(svptrue_b32(), a, b, 270);
}

/* Complex multiply-add with rotation */
WCN_INLINE svfloat32_t wcn_sve2_cmla_f32_0(svfloat32_t a, svfloat32_t b, svfloat32_t c) {
    return svcmla_f32_x(svptrue_b32(), a, b, c, 0);
}

WCN_INLINE svfloat32_t wcn_sve2_cmla_f32_90(svfloat32_t a, svfloat32_t b, svfloat32_t c) {
    return svcmla_f32_x(svptrue_b32(), a, b, c, 90);
}

WCN_INLINE svfloat32_t wcn_sve2_cmla_f32_180(svfloat32_t a, svfloat32_t b, svfloat32_t c) {
    return svcmla_f32_x(svptrue_b32(), a, b, c, 180);
}

WCN_INLINE svfloat32_t wcn_sve2_cmla_f32_270(svfloat32_t a, svfloat32_t b, svfloat32_t c) {
    return svcmla_f32_x(svptrue_b32(), a, b, c, 270);
}

/* ========== Bit Manipulation ========== */

/* Bitwise clear: a AND NOT b */
WCN_INLINE svint32_t wcn_sve2_bic_i32(svint32_t a, svint32_t b) {
    return svbic_s32_x(svptrue_b32(), a, b);
}

/* Bitwise select: (a AND b) OR (c AND NOT b) */
WCN_INLINE svint32_t wcn_sve2_bsl_i32(svint32_t a, svint32_t b, svint32_t c) {
    return svbsl_s32(a, b, c);
}

/* Exclusive OR and rotate: XOR with rotated elements */
WCN_INLINE svint32_t wcn_sve2_eor3_i32(svint32_t a, svint32_t b, svint32_t c) {
    return sveor3_s32(a, b, c);
}

/* Bit count (population count) */
WCN_INLINE svuint32_t wcn_sve2_cnt_u32(svuint32_t a) {
    return svcnt_u32_x(svptrue_b32(), a);
}

/* ========== Match Operations ========== */

/* Match any: test if any element equals */
WCN_INLINE svbool_t wcn_sve2_match_u32(svbool_t pg, svuint32_t a, svuint32_t b) {
    return svmatch_u32(pg, a, b);
}

/* No match: test if no element equals */
WCN_INLINE svbool_t wcn_sve2_nmatch_u32(svbool_t pg, svuint32_t a, svuint32_t b) {
    return svnmatch_u32(pg, a, b);
}

/* ========== Histogram Operations ========== */

/* Histogram count (increment elements indexed by another vector) */
WCN_INLINE svuint32_t wcn_sve2_histcnt_u32(svbool_t pg, svuint32_t indices, svuint32_t data) {
    return svhistcnt_u32_z(pg, indices, data);
}

/* Histogram segment: gather histogram into segments */
WCN_INLINE svuint32_t wcn_sve2_histseg_u32(svuint32_t a, svuint32_t b) {
    return svhistseg_u32(a, b);
}

/* ========== Pairwise Operations ========== */

/* Pairwise add (even and odd elements) */
WCN_INLINE svint32_t wcn_sve2_addp_i32(svbool_t pg, svint32_t a, svint32_t b) {
    return svaddp_s32_x(pg, a, b);
}

/* Pairwise maximum */
WCN_INLINE svint32_t wcn_sve2_maxp_i32(svbool_t pg, svint32_t a, svint32_t b) {
    return svmaxp_s32_x(pg, a, b);
}

/* Pairwise minimum */
WCN_INLINE svint32_t wcn_sve2_minp_i32(svbool_t pg, svint32_t a, svint32_t b) {
    return svminp_s32_x(pg, a, b);
}

/* ========== Shift and Insert ========== */

/* Shift right and insert */
WCN_INLINE svint32_t wcn_sve2_sri_i32(svint32_t a, svint32_t b, uint64_t shift) {
    return svsri_n_s32(a, b, shift);
}

/* Shift left and insert */
WCN_INLINE svint32_t wcn_sve2_sli_i32(svint32_t a, svint32_t b, uint64_t shift) {
    return svsli_n_s32(a, b, shift);
}

/* ========== Absolute Difference ========== */

/* Absolute difference (signed) */
WCN_INLINE svint32_t wcn_sve2_abd_i32(svint32_t a, svint32_t b) {
    return svabd_s32_x(svptrue_b32(), a, b);
}

/* Absolute difference (unsigned) */
WCN_INLINE svuint32_t wcn_sve2_abd_u32(svuint32_t a, svuint32_t b) {
    return svabd_u32_x(svptrue_b32(), a, b);
}

/* Absolute difference accumulate long (bottom) */
WCN_INLINE svint64_t wcn_sve2_adalb_i64(svint64_t a, svint32_t b, svint32_t c) {
    return svadalb_s64(a, b, c);
}

/* Absolute difference accumulate long (top) */
WCN_INLINE svint64_t wcn_sve2_adalt_i64(svint64_t a, svint32_t b, svint32_t c) {
    return svadalt_s64(a, b, c);
}

/* ========== Rounding ========== */

/* Rounding shift right narrow (bottom) */
WCN_INLINE svint16_t wcn_sve2_rshrn_i16_b(svint32_t a, uint64_t shift) {
    return svqrshrn_n_s16(a, shift);
}

/* Rounding add narrow (bottom): (a + b) >> 1 */
WCN_INLINE svint16_t wcn_sve2_raddhn_i16_b(svint32_t a, svint32_t b) {
    return svraddhnb_s16(a, b);
}

/* Rounding add narrow (top) */
WCN_INLINE svint16_t wcn_sve2_raddhn_i16_t(svint16_t r, svint32_t a, svint32_t b) {
    return svraddhnt_s16(r, a, b);
}

/* ========== Reciprocal Estimates (Enhanced) ========== */

/* Reciprocal estimate with extended precision */
WCN_INLINE svfloat32_t wcn_sve2_recpe_f32(svfloat32_t a) {
    return svrecpe_f32(a);
}

/* Reciprocal square root estimate with extended precision */
WCN_INLINE svfloat32_t wcn_sve2_rsqrte_f32(svfloat32_t a) {
    return svrsqrte_f32(a);
}

/* Reciprocal step: (2.0 - a * b) */
WCN_INLINE svfloat32_t wcn_sve2_recps_f32(svfloat32_t a, svfloat32_t b) {
    return svrecps_f32(a, b);
}

/* Reciprocal square root step: (3.0 - a * b) / 2.0 */
WCN_INLINE svfloat32_t wcn_sve2_rsqrts_f32(svfloat32_t a, svfloat32_t b) {
    return svrsqrts_f32(a, b);
}

/* ========== Table Lookup ========== */

/* Table lookup with one table vector */
WCN_INLINE svuint8_t wcn_sve2_tbl_u8(svuint8_t table, svuint8_t indices) {
    return svtbl_u8(table, indices);
}

/* Table lookup with two table vectors */
WCN_INLINE svuint8_t wcn_sve2_tbl2_u8(svuint8x2_t table, svuint8_t indices) {
    return svtbl2_u8(table, indices);
}

/* Table lookup extension */
WCN_INLINE svuint8_t wcn_sve2_tbx_u8(svuint8_t fallback, svuint8_t table, svuint8_t indices) {
    return svtbx_u8(fallback, table, indices);
}

/* ========== Reverse Operations ========== */

/* Reverse bits in each element */
WCN_INLINE svuint32_t wcn_sve2_rbit_u32(svuint32_t a) {
    return svrbit_u32_x(svptrue_b32(), a);
}

/* Reverse bytes in each element */
WCN_INLINE svuint32_t wcn_sve2_revb_u32(svuint32_t a) {
    return svrevb_u32_x(svptrue_b32(), a);
}

/* Reverse halfwords in each word */
WCN_INLINE svuint32_t wcn_sve2_revh_u32(svuint32_t a) {
    return svrevh_u32_x(svptrue_b32(), a);
}

#endif /* WCN_ARM_SVE2 */

#endif /* WCN_ARM_SVE2_H */
