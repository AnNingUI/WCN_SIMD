#ifndef WCN_MIPS_MSA_ATOMIC_H
#define WCN_MIPS_MSA_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_MIPS_MSA

#include <msa.h>

/* MIPS MSA atomic operations using scalar fallback */

/* ========== Memory Fencing ========== */

WCN_INLINE void wcn_msa_memory_fence(wcn_memory_order_t order) {
    switch (order) {
        case WCN_MEMORY_ORDER_RELAXED:
            break;
        case WCN_MEMORY_ORDER_CONSUME:
        case WCN_MEMORY_ORDER_ACQUIRE:
        case WCN_MEMORY_ORDER_RELEASE:
        case WCN_MEMORY_ORDER_ACQ_REL:
        case WCN_MEMORY_ORDER_SEQ_CST:
            __asm__ volatile("sync" ::: "memory");
            break;
    }
}

/* ========== MSA Atomic Load/Store ========== */

WCN_INLINE v4i32 wcn_msa_atomic_load_i32(const v4i32* ptr, wcn_memory_order_t order) {
    wcn_msa_memory_fence(order);
    
    volatile int32_t* src = (volatile int32_t*)ptr;
    v4i32 result;
    int32_t* dst = (int32_t*)&result;
    
    for (int i = 0; i < 4; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE void wcn_msa_atomic_store_i32(v4i32* ptr, v4i32 value, wcn_memory_order_t order) {
    volatile int32_t* dst = (volatile int32_t*)ptr;
    int32_t* src = (int32_t*)&value;
    
    for (int i = 0; i < 4; i++) {
        dst[i] = src[i];
    }
    
    wcn_msa_memory_fence(order);
}

WCN_INLINE v4f32 wcn_msa_atomic_load_f32(const v4f32* ptr, wcn_memory_order_t order) {
    wcn_msa_memory_fence(order);
    
    volatile float* src = (volatile float*)ptr;
    v4f32 result;
    float* dst = (float*)&result;
    
    for (int i = 0; i < 4; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE void wcn_msa_atomic_store_f32(v4f32* ptr, v4f32 value, wcn_memory_order_t order) {
    volatile float* dst = (volatile float*)ptr;
    float* src = (float*)&value;
    
    for (int i = 0; i < 4; i++) {
        dst[i] = src[i];
    }
    
    wcn_msa_memory_fence(order);
}

WCN_INLINE v2f64 wcn_msa_atomic_load_f64(const v2f64* ptr, wcn_memory_order_t order) {
    wcn_msa_memory_fence(order);
    
    volatile double* src = (volatile double*)ptr;
    v2f64 result;
    double* dst = (double*)&result;
    
    for (int i = 0; i < 2; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE void wcn_msa_atomic_store_f64(v2f64* ptr, v2f64 value, wcn_memory_order_t order) {
    volatile double* dst = (volatile double*)ptr;
    double* src = (double*)&value;
    
    for (int i = 0; i < 2; i++) {
        dst[i] = src[i];
    }
    
    wcn_msa_memory_fence(order);
}

#endif /* WCN_MIPS_MSA */

#endif /* WCN_MIPS_MSA_ATOMIC_H */
