#ifndef WCN_ARM_SVE_H
#define WCN_ARM_SVE_H

#include "../../wcn_types.h"

#ifdef WCN_ARM_SVE

/* ARM SVE uses scalable vectors - size determined at runtime */
/* All operations require predicates for masking */

/* ========== Predicate Operations ========== */

/* Get a predicate that selects all elements */
WCN_INLINE svbool_t wcn_sve_ptrue_all(void) {
    return svptrue_b32();
}

/* Get a predicate for the first N elements */
WCN_INLINE svbool_t wcn_sve_ptrue_count(uint64_t count) {
    return svwhilelt_b32(0, count);
}

/* ========== Vector Length Query ========== */

WCN_INLINE uint64_t wcn_sve_cntw(void) {
    return svcntw();
}

WCN_INLINE uint64_t wcn_sve_cntd(void) {
    return svcntd();
}

/* ========== Load/Store Operations ========== */

WCN_INLINE svint32_t wcn_sve_load_i32(svbool_t pg, const int32_t* ptr) {
    return svld1_s32(pg, ptr);
}

WCN_INLINE void wcn_sve_store_i32(svbool_t pg, int32_t* ptr, svint32_t vec) {
    svst1_s32(pg, ptr, vec);
}

WCN_INLINE svfloat32_t wcn_sve_load_f32(svbool_t pg, const float* ptr) {
    return svld1_f32(pg, ptr);
}

WCN_INLINE void wcn_sve_store_f32(svbool_t pg, float* ptr, svfloat32_t vec) {
    svst1_f32(pg, ptr, vec);
}

WCN_INLINE svfloat64_t wcn_sve_load_f64(svbool_t pg, const double* ptr) {
    return svld1_f64(pg, ptr);
}

WCN_INLINE void wcn_sve_store_f64(svbool_t pg, double* ptr, svfloat64_t vec) {
    svst1_f64(pg, ptr, vec);
}

/* ========== Contiguous Load/Store (non-temporal) ========== */

WCN_INLINE svint32_t wcn_sve_ldnt_i32(svbool_t pg, const int32_t* ptr) {
    return svldnt1_s32(pg, ptr);
}

WCN_INLINE void wcn_sve_stnt_i32(svbool_t pg, int32_t* ptr, svint32_t vec) {
    svstnt1_s32(pg, ptr, vec);
}

/* ========== Initialization ========== */

WCN_INLINE svint32_t wcn_sve_dup_i32(int32_t value) {
    return svdup_n_s32(value);
}

WCN_INLINE svfloat32_t wcn_sve_dup_f32(float value) {
    return svdup_n_f32(value);
}

WCN_INLINE svfloat64_t wcn_sve_dup_f64(double value) {
    return svdup_n_f64(value);
}

WCN_INLINE svint32_t wcn_sve_index_i32(int32_t base, int32_t step) {
    return svindex_s32(base, step);
}

/* ========== Arithmetic Operations ========== */

WCN_INLINE svint32_t wcn_sve_add_i32(svint32_t a, svint32_t b) {
    return svadd_s32_x(svptrue_b32(), a, b);
}

WCN_INLINE svint32_t wcn_sve_add_i32_m(svbool_t pg, svint32_t a, svint32_t b) {
    return svadd_s32_m(pg, a, b);
}

WCN_INLINE svint32_t wcn_sve_sub_i32(svint32_t a, svint32_t b) {
    return svsub_s32_x(svptrue_b32(), a, b);
}

WCN_INLINE svint32_t wcn_sve_mul_i32(svint32_t a, svint32_t b) {
    return svmul_s32_x(svptrue_b32(), a, b);
}

WCN_INLINE svfloat32_t wcn_sve_add_f32(svfloat32_t a, svfloat32_t b) {
    return svadd_f32_x(svptrue_b32(), a, b);
}

WCN_INLINE svfloat32_t wcn_sve_add_f32_m(svbool_t pg, svfloat32_t a, svfloat32_t b) {
    return svadd_f32_m(pg, a, b);
}

WCN_INLINE svfloat32_t wcn_sve_sub_f32(svfloat32_t a, svfloat32_t b) {
    return svsub_f32_x(svptrue_b32(), a, b);
}

WCN_INLINE svfloat32_t wcn_sve_mul_f32(svfloat32_t a, svfloat32_t b) {
    return svmul_f32_x(svptrue_b32(), a, b);
}

WCN_INLINE svfloat32_t wcn_sve_div_f32(svfloat32_t a, svfloat32_t b) {
    return svdiv_f32_x(svptrue_b32(), a, b);
}

WCN_INLINE svfloat64_t wcn_sve_add_f64(svfloat64_t a, svfloat64_t b) {
    return svadd_f64_x(svptrue_b64(), a, b);
}

WCN_INLINE svfloat64_t wcn_sve_sub_f64(svfloat64_t a, svfloat64_t b) {
    return svsub_f64_x(svptrue_b64(), a, b);
}

WCN_INLINE svfloat64_t wcn_sve_mul_f64(svfloat64_t a, svfloat64_t b) {
    return svmul_f64_x(svptrue_b64(), a, b);
}

WCN_INLINE svfloat64_t wcn_sve_div_f64(svfloat64_t a, svfloat64_t b) {
    return svdiv_f64_x(svptrue_b64(), a, b);
}

/* ========== FMA (Fused Multiply-Add) ========== */

WCN_INLINE svfloat32_t wcn_sve_fmad_f32(svfloat32_t a, svfloat32_t b, svfloat32_t c) {
    return svmad_f32_x(svptrue_b32(), a, b, c);
}

WCN_INLINE svfloat64_t wcn_sve_fmad_f64(svfloat64_t a, svfloat64_t b, svfloat64_t c) {
    return svmad_f64_x(svptrue_b64(), a, b, c);
}

/* ========== Comparison Operations (return predicate) ========== */

WCN_INLINE svbool_t wcn_sve_cmpeq_i32(svint32_t a, svint32_t b) {
    return svcmpeq_s32(svptrue_b32(), a, b);
}

WCN_INLINE svbool_t wcn_sve_cmpgt_i32(svint32_t a, svint32_t b) {
    return svcmpgt_s32(svptrue_b32(), a, b);
}

WCN_INLINE svbool_t wcn_sve_cmplt_i32(svint32_t a, svint32_t b) {
    return svcmplt_s32(svptrue_b32(), a, b);
}

WCN_INLINE svbool_t wcn_sve_cmpeq_f32(svfloat32_t a, svfloat32_t b) {
    return svcmpeq_f32(svptrue_b32(), a, b);
}

WCN_INLINE svbool_t wcn_sve_cmpgt_f32(svfloat32_t a, svfloat32_t b) {
    return svcmpgt_f32(svptrue_b32(), a, b);
}

WCN_INLINE svbool_t wcn_sve_cmplt_f32(svfloat32_t a, svfloat32_t b) {
    return svcmplt_f32(svptrue_b32(), a, b);
}

/* ========== Min/Max Operations ========== */

WCN_INLINE svint32_t wcn_sve_max_i32(svint32_t a, svint32_t b) {
    return svmax_s32_x(svptrue_b32(), a, b);
}

WCN_INLINE svint32_t wcn_sve_min_i32(svint32_t a, svint32_t b) {
    return svmin_s32_x(svptrue_b32(), a, b);
}

WCN_INLINE svfloat32_t wcn_sve_max_f32(svfloat32_t a, svfloat32_t b) {
    return svmax_f32_x(svptrue_b32(), a, b);
}

WCN_INLINE svfloat32_t wcn_sve_min_f32(svfloat32_t a, svfloat32_t b) {
    return svmin_f32_x(svptrue_b32(), a, b);
}

WCN_INLINE svfloat64_t wcn_sve_max_f64(svfloat64_t a, svfloat64_t b) {
    return svmax_f64_x(svptrue_b64(), a, b);
}

WCN_INLINE svfloat64_t wcn_sve_min_f64(svfloat64_t a, svfloat64_t b) {
    return svmin_f64_x(svptrue_b64(), a, b);
}

/* ========== Square Root ========== */

WCN_INLINE svfloat32_t wcn_sve_sqrt_f32(svfloat32_t vec) {
    return svsqrt_f32_x(svptrue_b32(), vec);
}

WCN_INLINE svfloat64_t wcn_sve_sqrt_f64(svfloat64_t vec) {
    return svsqrt_f64_x(svptrue_b64(), vec);
}

/* ========== Absolute Value ========== */

WCN_INLINE svint32_t wcn_sve_abs_i32(svint32_t vec) {
    return svabs_s32_x(svptrue_b32(), vec);
}

WCN_INLINE svfloat32_t wcn_sve_abs_f32(svfloat32_t vec) {
    return svabs_f32_x(svptrue_b32(), vec);
}

/* ========== Conversions ========== */

WCN_INLINE svfloat32_t wcn_sve_cvt_i32_f32(svint32_t vec) {
    return svcvt_f32_s32_x(svptrue_b32(), vec);
}

WCN_INLINE svint32_t wcn_sve_cvt_f32_i32(svfloat32_t vec) {
    return svcvt_s32_f32_x(svptrue_b32(), vec);
}

/* ========== Reduction Operations ========== */

WCN_INLINE int32_t wcn_sve_addv_i32(svint32_t vec) {
    return svaddv_s32(svptrue_b32(), vec);
}

WCN_INLINE int32_t wcn_sve_maxv_i32(svint32_t vec) {
    return svmaxv_s32(svptrue_b32(), vec);
}

WCN_INLINE int32_t wcn_sve_minv_i32(svint32_t vec) {
    return svminv_s32(svptrue_b32(), vec);
}

WCN_INLINE float wcn_sve_addv_f32(svfloat32_t vec) {
    return svaddv_f32(svptrue_b32(), vec);
}

WCN_INLINE float wcn_sve_maxv_f32(svfloat32_t vec) {
    return svmaxv_f32(svptrue_b32(), vec);
}

WCN_INLINE float wcn_sve_minv_f32(svfloat32_t vec) {
    return svminv_f32(svptrue_b32(), vec);
}

/* ========== Gather/Scatter Operations ========== */

WCN_INLINE svint32_t wcn_sve_gather_i32(svbool_t pg, const int32_t* base, svint32_t offsets) {
    return svld1_gather_s32offset_s32(pg, base, offsets);
}

WCN_INLINE void wcn_sve_scatter_i32(svbool_t pg, int32_t* base, svint32_t offsets, svint32_t data) {
    svst1_scatter_s32offset_s32(pg, base, offsets, data);
}

WCN_INLINE svfloat32_t wcn_sve_gather_f32(svbool_t pg, const float* base, svint32_t offsets) {
    return svld1_gather_s32offset_f32(pg, base, offsets);
}

WCN_INLINE void wcn_sve_scatter_f32(svbool_t pg, float* base, svint32_t offsets, svfloat32_t data) {
    svst1_scatter_s32offset_f32(pg, base, offsets, data);
}

/* ========== Select (Conditional) ========== */

WCN_INLINE svint32_t wcn_sve_sel_i32(svbool_t pg, svint32_t a, svint32_t b) {
    return svsel_s32(pg, a, b);
}

WCN_INLINE svfloat32_t wcn_sve_sel_f32(svbool_t pg, svfloat32_t a, svfloat32_t b) {
    return svsel_f32(pg, a, b);
}

/* ========== Logical Operations ========== */

WCN_INLINE svint32_t wcn_sve_and_i32(svint32_t a, svint32_t b) {
    return svand_s32_x(svptrue_b32(), a, b);
}

WCN_INLINE svint32_t wcn_sve_or_i32(svint32_t a, svint32_t b) {
    return svorr_s32_x(svptrue_b32(), a, b);
}

WCN_INLINE svint32_t wcn_sve_xor_i32(svint32_t a, svint32_t b) {
    return sveor_s32_x(svptrue_b32(), a, b);
}

WCN_INLINE svint32_t wcn_sve_not_i32(svint32_t a) {
    return svnot_s32_x(svptrue_b32(), a);
}

/* ========== Bit Manipulation ========== */

WCN_INLINE svint32_t wcn_sve_lsl_i32(svint32_t a, svuint32_t shift) {
    return svlsl_s32_x(svptrue_b32(), a, shift);
}

WCN_INLINE svint32_t wcn_sve_lsr_i32(svint32_t a, svuint32_t shift) {
    return svlsr_s32_x(svptrue_b32(), a, shift);
}

WCN_INLINE svint32_t wcn_sve_asr_i32(svint32_t a, svuint32_t shift) {
    return svasr_s32_x(svptrue_b32(), a, shift);
}

/* ========== Reverse Operations ========== */

WCN_INLINE svint32_t wcn_sve_rev_i32(svint32_t vec) {
    return svrev_s32(vec);
}

WCN_INLINE svfloat32_t wcn_sve_rev_f32(svfloat32_t vec) {
    return svrev_f32(vec);
}

/* ========== Compact (remove inactive elements) ========== */

WCN_INLINE svint32_t wcn_sve_compact_i32(svbool_t pg, svint32_t vec) {
    return svcompact_s32(pg, vec);
}

WCN_INLINE svfloat32_t wcn_sve_compact_f32(svbool_t pg, svfloat32_t vec) {
    return svcompact_f32(pg, vec);
}

#ifdef WCN_ARM_SVE2
/* ========== SVE2 Additional Operations ========== */

/* Multiply-Add Long */
WCN_INLINE svint32_t wcn_sve2_mlal_i32(svint32_t acc, svint16_t a, svint16_t b) {
    return svmlalb_s32(acc, a, b);
}

/* Absolute Difference */
WCN_INLINE svuint32_t wcn_sve2_abd_u32(svuint32_t a, svuint32_t b) {
    return svabd_u32_x(svptrue_b32(), a, b);
}

/* Bitwise Clear */
WCN_INLINE svint32_t wcn_sve2_bic_i32(svint32_t a, svint32_t b) {
    return svbic_s32_x(svptrue_b32(), a, b);
}

/* Count Leading Zeros */
WCN_INLINE svuint32_t wcn_sve2_clz_u32(svuint32_t vec) {
    return svclz_u32_x(svptrue_b32(), vec);
}

#endif /* WCN_ARM_SVE2 */

#endif /* WCN_ARM_SVE */

#endif /* WCN_ARM_SVE_H */
