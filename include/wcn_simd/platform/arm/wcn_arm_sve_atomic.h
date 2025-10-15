#ifndef WCN_ARM_SVE_ATOMIC_H
#define WCN_ARM_SVE_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_ARM_SVE

#include <arm_sve.h>

/* ARM SVE atomic operations using scalar fallback
 * SVE vectors are scalable, so atomic operations use element-wise approach */

/* ========== Memory Fencing ========== */

WCN_INLINE void wcn_sve_memory_fence(wcn_memory_order_t order) {
    switch (order) {
        case WCN_MEMORY_ORDER_RELAXED:
            break;
        case WCN_MEMORY_ORDER_CONSUME:
        case WCN_MEMORY_ORDER_ACQUIRE:
            __asm__ volatile("dmb ishld" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_RELEASE:
            __asm__ volatile("dmb ish" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_ACQ_REL:
        case WCN_MEMORY_ORDER_SEQ_CST:
            __asm__ volatile("dmb ish" ::: "memory");
            break;
    }
}

/* ========== SVE Atomic Load/Store ========== */

/* Load with memory ordering */
WCN_INLINE svint32_t wcn_sve_atomic_load_i32(svbool_t pg, const int32_t* ptr, 
                                              wcn_memory_order_t order) {
    wcn_sve_memory_fence(order);
    return svld1_s32(pg, ptr);
}

/* Store with memory ordering */
WCN_INLINE void wcn_sve_atomic_store_i32(svbool_t pg, int32_t* ptr, svint32_t value,
                                         wcn_memory_order_t order) {
    svst1_s32(pg, ptr, value);
    wcn_sve_memory_fence(order);
}

/* Float atomic operations */
WCN_INLINE svfloat32_t wcn_sve_atomic_load_f32(svbool_t pg, const float* ptr,
                                                wcn_memory_order_t order) {
    wcn_sve_memory_fence(order);
    return svld1_f32(pg, ptr);
}

WCN_INLINE void wcn_sve_atomic_store_f32(svbool_t pg, float* ptr, svfloat32_t value,
                                         wcn_memory_order_t order) {
    svst1_f32(pg, ptr, value);
    wcn_sve_memory_fence(order);
}

/* Double atomic operations */
WCN_INLINE svfloat64_t wcn_sve_atomic_load_f64(svbool_t pg, const double* ptr,
                                                wcn_memory_order_t order) {
    wcn_sve_memory_fence(order);
    return svld1_f64(pg, ptr);
}

WCN_INLINE void wcn_sve_atomic_store_f64(svbool_t pg, double* ptr, svfloat64_t value,
                                         wcn_memory_order_t order) {
    svst1_f64(pg, ptr, value);
    wcn_sve_memory_fence(order);
}

#endif /* WCN_ARM_SVE */

#endif /* WCN_ARM_SVE_ATOMIC_H */
