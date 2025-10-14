#ifndef WCN_RISCV_RVV_H
#define WCN_RISCV_RVV_H

#include "../../wcn_types.h"

#ifdef WCN_RISCV_RVV

/* ========== Load/Store Operations ========== */

WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_i32_load(const int32_t* ptr, size_t vl) {
    wcn_vscalable_i32_t result;
    result.raw = __riscv_vle32_v_i32m1(ptr, vl);
    return result;
}

WCN_INLINE void wcn_vscalable_i32_store(int32_t* ptr, wcn_vscalable_i32_t vec, size_t vl) {
    __riscv_vse32_v_i32m1(ptr, vec.raw, vl);
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_load(const float* ptr, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vle32_v_f32m1(ptr, vl);
    return result;
}

WCN_INLINE void wcn_vscalable_f32_store(float* ptr, wcn_vscalable_f32_t vec, size_t vl) {
    __riscv_vse32_v_f32m1(ptr, vec.raw, vl);
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_load(const double* ptr, size_t vl) {
    wcn_vscalable_f64_t result;
    result.raw = __riscv_vle64_v_f64m1(ptr, vl);
    return result;
}

WCN_INLINE void wcn_vscalable_f64_store(double* ptr, wcn_vscalable_f64_t vec, size_t vl) {
    __riscv_vse64_v_f64m1(ptr, vec.raw, vl);
}

/* ========== Vector Length Query ========== */

WCN_INLINE size_t wcn_rvv_vsetvl_i32(size_t avl) {
    return __riscv_vsetvl_e32m1(avl);
}

WCN_INLINE size_t wcn_rvv_vsetvl_f32(size_t avl) {
    return __riscv_vsetvl_e32m1(avl);
}

WCN_INLINE size_t wcn_rvv_vsetvl_f64(size_t avl) {
    return __riscv_vsetvl_e64m1(avl);
}

/* ========== Initialization ========== */

WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_i32_set1(int32_t value, size_t vl) {
    wcn_vscalable_i32_t result;
    result.raw = __riscv_vmv_v_x_i32m1(value, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_set1(float value, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfmv_v_f_f32m1(value, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_set1(double value, size_t vl) {
    wcn_vscalable_f64_t result;
    result.raw = __riscv_vfmv_v_f_f64m1(value, vl);
    return result;
}

/* ========== Arithmetic Operations ========== */

WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_i32_add(wcn_vscalable_i32_t a, wcn_vscalable_i32_t b, size_t vl) {
    wcn_vscalable_i32_t result;
    result.raw = __riscv_vadd_vv_i32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_i32_sub(wcn_vscalable_i32_t a, wcn_vscalable_i32_t b, size_t vl) {
    wcn_vscalable_i32_t result;
    result.raw = __riscv_vsub_vv_i32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_i32_mul(wcn_vscalable_i32_t a, wcn_vscalable_i32_t b, size_t vl) {
    wcn_vscalable_i32_t result;
    result.raw = __riscv_vmul_vv_i32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_add(wcn_vscalable_f32_t a, wcn_vscalable_f32_t b, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfadd_vv_f32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_sub(wcn_vscalable_f32_t a, wcn_vscalable_f32_t b, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfsub_vv_f32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_mul(wcn_vscalable_f32_t a, wcn_vscalable_f32_t b, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfmul_vv_f32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_div(wcn_vscalable_f32_t a, wcn_vscalable_f32_t b, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfdiv_vv_f32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_add(wcn_vscalable_f64_t a, wcn_vscalable_f64_t b, size_t vl) {
    wcn_vscalable_f64_t result;
    result.raw = __riscv_vfadd_vv_f64m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_sub(wcn_vscalable_f64_t a, wcn_vscalable_f64_t b, size_t vl) {
    wcn_vscalable_f64_t result;
    result.raw = __riscv_vfsub_vv_f64m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_mul(wcn_vscalable_f64_t a, wcn_vscalable_f64_t b, size_t vl) {
    wcn_vscalable_f64_t result;
    result.raw = __riscv_vfmul_vv_f64m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_div(wcn_vscalable_f64_t a, wcn_vscalable_f64_t b, size_t vl) {
    wcn_vscalable_f64_t result;
    result.raw = __riscv_vfdiv_vv_f64m1(a.raw, b.raw, vl);
    return result;
}

/* ========== FMA (Fused Multiply-Add) ========== */

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_fmadd(wcn_vscalable_f32_t a, wcn_vscalable_f32_t b, wcn_vscalable_f32_t c, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfmadd_vv_f32m1(a.raw, b.raw, c.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_fmadd(wcn_vscalable_f64_t a, wcn_vscalable_f64_t b, wcn_vscalable_f64_t c, size_t vl) {
    wcn_vscalable_f64_t result;
    result.raw = __riscv_vfmadd_vv_f64m1(a.raw, b.raw, c.raw, vl);
    return result;
}

/* ========== Min/Max Operations ========== */

WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_i32_max(wcn_vscalable_i32_t a, wcn_vscalable_i32_t b, size_t vl) {
    wcn_vscalable_i32_t result;
    result.raw = __riscv_vmax_vv_i32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_i32_min(wcn_vscalable_i32_t a, wcn_vscalable_i32_t b, size_t vl) {
    wcn_vscalable_i32_t result;
    result.raw = __riscv_vmin_vv_i32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_max(wcn_vscalable_f32_t a, wcn_vscalable_f32_t b, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfmax_vv_f32m1(a.raw, b.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_min(wcn_vscalable_f32_t a, wcn_vscalable_f32_t b, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfmin_vv_f32m1(a.raw, b.raw, vl);
    return result;
}

/* ========== Square Root ========== */

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_sqrt(wcn_vscalable_f32_t vec, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfsqrt_v_f32m1(vec.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_sqrt(wcn_vscalable_f64_t vec, size_t vl) {
    wcn_vscalable_f64_t result;
    result.raw = __riscv_vfsqrt_v_f64m1(vec.raw, vl);
    return result;
}

/* ========== Conversions ========== */

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_i32_to_f32(wcn_vscalable_i32_t vec, size_t vl) {
    wcn_vscalable_f32_t result;
    result.raw = __riscv_vfcvt_f_x_v_f32m1(vec.raw, vl);
    return result;
}

WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_f32_to_i32(wcn_vscalable_f32_t vec, size_t vl) {
    wcn_vscalable_i32_t result;
    result.raw = __riscv_vfcvt_x_f_v_i32m1(vec.raw, vl);
    return result;
}

#endif /* WCN_RISCV_RVV */

#endif /* WCN_RISCV_RVV_H */
