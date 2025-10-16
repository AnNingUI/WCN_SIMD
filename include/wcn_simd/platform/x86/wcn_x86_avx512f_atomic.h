#ifndef WCN_X86_AVX512F_ATOMIC_H
#define WCN_X86_AVX512F_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_X86_AVX512F

#include <immintrin.h>

/* AVX-512 atomic operations using scalar fallback
 * True 512-bit atomic operations are not available in hardware */

/* ========== Memory Fencing ========== */

WCN_INLINE void wcn_avx512_memory_fence(wcn_memory_order_t order) {
    switch (order) {
        case WCN_MEMORY_ORDER_RELAXED:
            break;
        case WCN_MEMORY_ORDER_CONSUME:
        case WCN_MEMORY_ORDER_ACQUIRE:
            __asm__ volatile("" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_RELEASE:
            __asm__ volatile("" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_ACQ_REL:
        case WCN_MEMORY_ORDER_SEQ_CST:
            _mm_mfence();
            break;
    }
}

/* ========== 512-bit Atomic Load/Store (Scalar Fallback) ========== */

WCN_INLINE __m512i wcn_v512i_atomic_load(const __m512i* ptr, wcn_memory_order_t order) {
    wcn_avx512_memory_fence(order);
    
    /* Use volatile to prevent compiler optimizations */
    volatile int64_t* src = (volatile int64_t*)ptr;
    __m512i result;
    int64_t* dst = (int64_t*)&result;
    
    for (int i = 0; i < 8; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE void wcn_v512i_atomic_store(__m512i* ptr, __m512i value, wcn_memory_order_t order) {
    volatile int64_t* dst = (volatile int64_t*)ptr;
    int64_t* src = (int64_t*)&value;
    
    for (int i = 0; i < 8; i++) {
        dst[i] = src[i];
    }
    
    wcn_avx512_memory_fence(order);
}

WCN_INLINE __m512 wcn_v512f_atomic_load(const __m512* ptr, wcn_memory_order_t order) {
    wcn_avx512_memory_fence(order);
    
    volatile float* src = (volatile float*)ptr;
    __m512 result;
    float* dst = (float*)&result;
    
    for (int i = 0; i < 16; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE void wcn_v512f_atomic_store(__m512* ptr, __m512 value, wcn_memory_order_t order) {
    volatile float* dst = (volatile float*)ptr;
    float* src = (float*)&value;
    
    for (int i = 0; i < 16; i++) {
        dst[i] = src[i];
    }
    
    wcn_avx512_memory_fence(order);
}

WCN_INLINE __m512d wcn_v512d_atomic_load(const __m512d* ptr, wcn_memory_order_t order) {
    wcn_avx512_memory_fence(order);
    
    volatile double* src = (volatile double*)ptr;
    __m512d result;
    double* dst = (double*)&result;
    
    for (int i = 0; i < 8; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE void wcn_v512d_atomic_store(__m512d* ptr, __m512d value, wcn_memory_order_t order) {
    volatile double* dst = (volatile double*)ptr;
    double* src = (double*)&value;
    
    for (int i = 0; i < 8; i++) {
        dst[i] = src[i];
    }
    
    wcn_avx512_memory_fence(order);
}

/* ========== 512-bit Atomic Operations ========== */

/* Atomic compare-exchange (weak, scalar fallback) */
WCN_INLINE int wcn_v512i_atomic_compare_exchange_weak(__m512i* ptr, __m512i* expected, 
                                                       __m512i desired,
                                                       wcn_memory_order_t success,
                                                       wcn_memory_order_t failure) {
    (void)success;
    (void)failure;
    
    /* Simple scalar comparison - not truly atomic for 512-bit */
    int64_t* p = (int64_t*)ptr;
    int64_t* e = (int64_t*)expected;
    int64_t* d = (int64_t*)&desired;
    
    _mm_mfence();
    
    int all_match = 1;
    for (int i = 0; i < 8; i++) {
        if (p[i] != e[i]) {
            all_match = 0;
            e[i] = p[i];
        }
    }
    
    if (all_match) {
        for (int i = 0; i < 8; i++) {
            p[i] = d[i];
        }
    }
    
    _mm_mfence();
    return all_match;
}

/* Atomic fetch-add (elementwise, scalar fallback) */
WCN_INLINE __m512i wcn_v512i_atomic_fetch_add(__m512i* ptr, __m512i value, 
                                               wcn_memory_order_t order) {
    (void)order;
    
    int32_t* p = (int32_t*)ptr;
    int32_t* v = (int32_t*)&value;
    __m512i old_val;
    int32_t* old = (int32_t*)&old_val;
    
    _mm_mfence();
    
    for (int i = 0; i < 16; i++) {
        old[i] = p[i];
        p[i] += v[i];
    }
    
    _mm_mfence();
    return old_val;
}

#endif /* WCN_X86_AVX512F */

#endif /* WCN_X86_AVX512F_ATOMIC_H */
