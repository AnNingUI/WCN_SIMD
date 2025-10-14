#ifndef WCN_X86_AVX2_ATOMIC_H
#define WCN_X86_AVX2_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_X86_AVX2

#include <immintrin.h>

/* x86 AVX2 has better atomic support but still limited - implement using scalar fallback with locks */

/* ========== Memory Ordering Helpers ========== */

WCN_INLINE void wcn_x86_avx2_memory_fence(wcn_memory_order_t order) {
    switch (order) {
        case WCN_MEMORY_ORDER_RELAXED:
            break;
        case WCN_MEMORY_ORDER_CONSUME:
        case WCN_MEMORY_ORDER_ACQUIRE:
            /* Use compiler barrier for acquire */
            __asm__ volatile("" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_RELEASE:
            /* Use compiler barrier for release */
            __asm__ volatile("" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_ACQ_REL:
            /* Full fence */
            _mm_mfence();
            break;
        case WCN_MEMORY_ORDER_SEQ_CST:
            /* Sequential consistency */
            _mm_mfence();
            break;
    }
}

/* ========== 256-bit Atomic Load/Store Operations ========== */

/* AVX2 doesn't have true atomic SIMD loads/stores - use scalar fallback */
WCN_INLINE wcn_v256i_t wcn_v256i_atomic_load(const wcn_v256i_t* ptr, wcn_memory_order_t order) {
    wcn_v256i_t result;
    wcn_x86_avx2_memory_fence(order);
    
    /* Use volatile to prevent compiler optimizations */
    volatile int32_t* src = (volatile int32_t*)ptr;
    int32_t* dst = (int32_t*)&result;
    
    for (int i = 0; i < 8; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE wcn_v256f_t wcn_v256f_atomic_load(const wcn_v256f_t* ptr, wcn_memory_order_t order) {
    wcn_v256f_t result;
    wcn_x86_avx2_memory_fence(order);
    
    /* Use volatile to prevent compiler optimizations */
    volatile float* src = (volatile float*)ptr;
    float* dst = (float*)&result;
    
    for (int i = 0; i < 8; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_atomic_load(const wcn_v256d_t* ptr, wcn_memory_order_t order) {
    wcn_v256d_t result;
    wcn_x86_avx2_memory_fence(order);
    
    /* Use volatile to prevent compiler optimizations */
    volatile double* src = (volatile double*)ptr;
    double* dst = (double*)&result;
    
    for (int i = 0; i < 4; i++) {
        dst[i] = src[i];
    }
    
    return result;
}

WCN_INLINE void wcn_v256i_atomic_store(wcn_v256i_t* ptr, wcn_v256i_t value, wcn_memory_order_t order) {
    /* Use volatile to prevent compiler optimizations */
    volatile int32_t* dst = (volatile int32_t*)ptr;
    int32_t* src = (int32_t*)&value;
    
    for (int i = 0; i < 8; i++) {
        dst[i] = src[i];
    }
    
    wcn_x86_avx2_memory_fence(order);
}

WCN_INLINE void wcn_v256f_atomic_store(wcn_v256f_t* ptr, wcn_v256f_t value, wcn_memory_order_t order) {
    /* Use volatile to prevent compiler optimizations */
    volatile float* dst = (volatile float*)ptr;
    float* src = (float*)&value;
    
    for (int i = 0; i < 8; i++) {
        dst[i] = src[i];
    }
    
    wcn_x86_avx2_memory_fence(order);
}

WCN_INLINE void wcn_v256d_atomic_store(wcn_v256d_t* ptr, wcn_v256d_t value, wcn_memory_order_t order) {
    /* Use volatile to prevent compiler optimizations */
    volatile double* dst = (volatile double*)ptr;
    double* src = (double*)&value;
    
    for (int i = 0; i < 4; i++) {
        dst[i] = src[i];
    }
    
    wcn_x86_avx2_memory_fence(order);
}

/* Aligned atomic operations (same as unaligned for AVX2) */
WCN_INLINE wcn_v256i_t wcn_v256i_atomic_load_aligned(const wcn_v256i_t* ptr, wcn_memory_order_t order) {
    return wcn_v256i_atomic_load(ptr, order);
}

WCN_INLINE wcn_v256f_t wcn_v256f_atomic_load_aligned(const wcn_v256f_t* ptr, wcn_memory_order_t order) {
    return wcn_v256f_atomic_load(ptr, order);
}

WCN_INLINE wcn_v256d_t wcn_v256d_atomic_load_aligned(const wcn_v256d_t* ptr, wcn_memory_order_t order) {
    return wcn_v256d_atomic_load(ptr, order);
}

WCN_INLINE void wcn_v256i_atomic_store_aligned(wcn_v256i_t* ptr, wcn_v256i_t value, wcn_memory_order_t order) {
    wcn_v256i_atomic_store(ptr, value, order);
}

WCN_INLINE void wcn_v256f_atomic_store_aligned(wcn_v256f_t* ptr, wcn_v256f_t value, wcn_memory_order_t order) {
    wcn_v256f_atomic_store(ptr, value, order);
}

WCN_INLINE void wcn_v256d_atomic_store_aligned(wcn_v256d_t* ptr, wcn_v256d_t value, wcn_memory_order_t order) {
    wcn_v256d_atomic_store(ptr, value, order);
}

/* ========== 256-bit Atomic Compare-and-Swap Operations ========== */

WCN_INLINE int wcn_v256i_atomic_compare_exchange_strong(wcn_v256i_t* ptr, wcn_v256i_t* expected,
                                                         wcn_v256i_t desired, wcn_memory_order_t success_order,
                                                         wcn_memory_order_t failure_order) {
    int success = 1;
    volatile int32_t* target = (volatile int32_t*)ptr;
    int32_t* exp = (int32_t*)expected;
    int32_t* des = (int32_t*)&desired;
    
    /* Compare each element */
    for (int i = 0; i < 8; i++) {
        int32_t expected_val = exp[i];
        if (__sync_val_compare_and_swap(&target[i], expected_val, des[i]) != expected_val) {
            success = 0;
            /* Update expected value with actual value */
            exp[i] = target[i];
        }
    }
    
    if (success) {
        wcn_x86_avx2_memory_fence(success_order);
    } else {
        wcn_x86_avx2_memory_fence(failure_order);
    }
    
    return success;
}

WCN_INLINE int wcn_v256f_atomic_compare_exchange_strong(wcn_v256f_t* ptr, wcn_v256f_t* expected,
                                                         wcn_v256f_t desired, wcn_memory_order_t success_order,
                                                         wcn_memory_order_t failure_order) {
    /* Reinterpret as integers for CAS */
    return wcn_v256i_atomic_compare_exchange_strong((wcn_v256i_t*)ptr, (wcn_v256i_t*)expected,
                                                     *(wcn_v256i_t*)&desired, success_order, failure_order);
}

WCN_INLINE int wcn_v256d_atomic_compare_exchange_strong(wcn_v256d_t* ptr, wcn_v256d_t* expected,
                                                         wcn_v256d_t desired, wcn_memory_order_t success_order,
                                                         wcn_memory_order_t failure_order) {
    /* Use integer CAS on the bit representation */
    wcn_v256i_t* int_ptr = (wcn_v256i_t*)ptr;
    wcn_v256i_t* int_expected = (wcn_v256i_t*)expected;
    wcn_v256i_t int_desired = *(wcn_v256i_t*)&desired;
    
    return wcn_v256i_atomic_compare_exchange_strong(int_ptr, int_expected, int_desired,
                                                     success_order, failure_order);
}

/* Weak CAS is the same as strong for AVX2 */
WCN_INLINE int wcn_v256i_atomic_compare_exchange_weak(wcn_v256i_t* ptr, wcn_v256i_t* expected,
                                                       wcn_v256i_t desired, wcn_memory_order_t success_order,
                                                       wcn_memory_order_t failure_order) {
    return wcn_v256i_atomic_compare_exchange_strong(ptr, expected, desired, success_order, failure_order);
}

WCN_INLINE int wcn_v256f_atomic_compare_exchange_weak(wcn_v256f_t* ptr, wcn_v256f_t* expected,
                                                       wcn_v256f_t desired, wcn_memory_order_t success_order,
                                                       wcn_memory_order_t failure_order) {
    return wcn_v256f_atomic_compare_exchange_strong(ptr, expected, desired, success_order, failure_order);
}

WCN_INLINE int wcn_v256d_atomic_compare_exchange_weak(wcn_v256d_t* ptr, wcn_v256d_t* expected,
                                                       wcn_v256d_t desired, wcn_memory_order_t success_order,
                                                       wcn_memory_order_t failure_order) {
    return wcn_v256d_atomic_compare_exchange_strong(ptr, expected, desired, success_order, failure_order);
}

/* ========== 256-bit Atomic Exchange Operations ========== */

WCN_INLINE wcn_v256i_t wcn_v256i_atomic_exchange(wcn_v256i_t* ptr, wcn_v256i_t value, wcn_memory_order_t order) {
    wcn_v256i_t old_value;
    volatile int32_t* target = (volatile int32_t*)ptr;
    int32_t* new_val = (int32_t*)&value;
    int32_t* old_val = (int32_t*)&old_value;
    
    /* Exchange each element using atomic operations */
    for (int i = 0; i < 8; i++) {
        old_val[i] = __sync_lock_test_and_set(&target[i], new_val[i]);
    }
    
    wcn_x86_avx2_memory_fence(order);
    return old_value;
}

WCN_INLINE wcn_v256f_t wcn_v256f_atomic_exchange(wcn_v256f_t* ptr, wcn_v256f_t value, wcn_memory_order_t order) {
    /* Reinterpret as integers */
    wcn_v256i_t result = wcn_v256i_atomic_exchange((wcn_v256i_t*)ptr, *(wcn_v256i_t*)&value, order);
    return *(wcn_v256f_t*)&result;
}

WCN_INLINE wcn_v256d_t wcn_v256d_atomic_exchange(wcn_v256d_t* ptr, wcn_v256d_t value, wcn_memory_order_t order) {
    /* Use integer exchange on the bit representation */
    wcn_v256i_t* int_ptr = (wcn_v256i_t*)ptr;
    wcn_v256i_t int_value = *(wcn_v256i_t*)&value;
    wcn_v256i_t int_result = wcn_v256i_atomic_exchange(int_ptr, int_value, order);
    return *(wcn_v256d_t*)&int_result;
}

/* ========== 128-bit Atomic Operations (fallback to SSE2) ========== */

/* Include SSE2 atomic operations for 128-bit vectors */
/* Note: SSE2 atomic header provides wcn_atomic_thread_fence and wcn_atomic_signal_fence */
#include "wcn_x86_sse2_atomic.h"

#endif /* WCN_X86_AVX2 */

#endif /* WCN_X86_AVX2_ATOMIC_H */