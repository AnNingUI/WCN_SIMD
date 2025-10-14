#ifndef WCN_WASM_SIMD128_ATOMIC_H
#define WCN_WASM_SIMD128_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_WASM_SIMD128

/* Check if atomic builtins are available */
#if defined(__has_builtin)
#  if __has_builtin(__builtin_wasm_atomic_fence) && \
      __has_builtin(__builtin_wasm_atomic_wait_i32) && \
      __has_builtin(__builtin_wasm_atomic_store_i32) && \
      __has_builtin(__builtin_wasm_atomic_compare_exchange_i32) && \
      __has_builtin(__builtin_wasm_atomic_exchange_i32)
#    define WCN_WASM_HAS_ATOMIC_BUILTINS 1
#  else
#    define WCN_WASM_HAS_ATOMIC_BUILTINS 0
#  endif
#else
#  define WCN_WASM_HAS_ATOMIC_BUILTINS 0
#endif

#if WCN_WASM_HAS_ATOMIC_BUILTINS

#include <wasm_simd128.h>

/* WebAssembly SIMD128 atomic operations */

/* ========== Memory Ordering Helpers ========== */

WCN_INLINE void wcn_wasm_memory_fence(wcn_memory_order_t order) {
    switch (order) {
        case WCN_MEMORY_ORDER_RELAXED:
            break;
        case WCN_MEMORY_ORDER_CONSUME:
        case WCN_MEMORY_ORDER_ACQUIRE:
            /* Compiler barrier for acquire */
            __asm__ volatile("" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_RELEASE:
            /* Compiler barrier for release */
            __asm__ volatile("" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_ACQ_REL:
            /* Full fence - use atomic.fence in WASM */
#ifdef __has_builtin
#if __has_builtin(__builtin_wasm_atomic_fence)
            __builtin_wasm_atomic_fence(0x0); /* seq_cst */
#else
            __asm__ volatile("" ::: "memory");
#endif
#else
            __asm__ volatile("" ::: "memory");
#endif
            break;
        case WCN_MEMORY_ORDER_SEQ_CST:
            /* Sequential consistency */
#ifdef __has_builtin
#if __has_builtin(__builtin_wasm_atomic_fence)
            __builtin_wasm_atomic_fence(0x0); /* seq_cst */
#else
            __asm__ volatile("" ::: "memory");
#endif
#else
            __asm__ volatile("" ::: "memory");
#endif
            break;
    }
}

/* ========== Atomic Load/Store Operations ========== */

/* WebAssembly SIMD128 atomic loads with memory barriers */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_load(const wcn_v128i_t* ptr, wcn_memory_order_t order) {
    wcn_v128i_t result;
    
    /* Apply appropriate memory barrier before load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_wasm_memory_fence(order);
    }
    
    /* Load the vector using atomic operations */
    /* Note: WASM doesn't have native SIMD atomic load, use scalar fallback */
    volatile int32_t* src = (volatile int32_t*)ptr;
    int32_t* dst = (int32_t*)&result;
    
    for (int i = 0; i < 4; i++) {
        dst[i] = __builtin_wasm_atomic_wait_i32((int*)(&src[i]), 0, -1); /* timeout = -1 for immediate return */
    }
    
    /* Apply appropriate memory barrier after load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_wasm_memory_fence(order);
    }
    
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_atomic_load(const wcn_v128f_t* ptr, wcn_memory_order_t order) {
    wcn_v128f_t result;
    
    /* Apply appropriate memory barrier before load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_wasm_memory_fence(order);
    }
    
    /* Load the vector using atomic operations */
    /* Note: WASM doesn't have native SIMD atomic load, use scalar fallback */
    volatile float* src = (volatile float*)ptr;
    float* dst = (float*)&result;
    
    for (int i = 0; i < 4; i++) {
        /* Use atomic load on each element */
        __builtin_wasm_atomic_wait_i32((int*)(&src[i]), 0, -1); /* timeout = -1 for immediate return */
        dst[i] = src[i];
    }
    
    /* Apply appropriate memory barrier after load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_wasm_memory_fence(order);
    }
    
    return result;
}

/* WebAssembly SIMD128 atomic stores with memory barriers */
WCN_INLINE void wcn_v128i_atomic_store(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order) {
    /* Apply appropriate memory barrier before store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_wasm_memory_fence(order);
    }
    
    /* Store the vector using atomic operations */
    /* Note: WASM doesn't have native SIMD atomic store, use scalar fallback */
    volatile int32_t* dst = (volatile int32_t*)ptr;
    int32_t* src = (int32_t*)&value;
    
    for (int i = 0; i < 4; i++) {
        /* Use atomic store on each element */
        __builtin_wasm_atomic_store_i32((int*)(&dst[i]), src[i], 0x0); /* seq_cst */
    }
    
    /* Apply appropriate memory barrier after store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_wasm_memory_fence(order);
    }
}

WCN_INLINE void wcn_v128f_atomic_store(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order) {
    /* Apply appropriate memory barrier before store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_wasm_memory_fence(order);
    }
    
    /* Store the vector using atomic operations */
    /* Note: WASM doesn't have native SIMD atomic store, use scalar fallback */
    volatile float* dst = (volatile float*)ptr;
    float* src = (float*)&value;
    
    for (int i = 0; i < 4; i++) {
        /* Use atomic store on each element */
        int32_t int_val = *(int32_t*)(&src[i]);
        __builtin_wasm_atomic_store_i32((int*)(&dst[i]), int_val, 0x0); /* seq_cst */
    }
    
    /* Apply appropriate memory barrier after store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_wasm_memory_fence(order);
    }
}

/* Aligned atomic operations (same as unaligned for WASM) */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_load_aligned(const wcn_v128i_t* ptr, wcn_memory_order_t order) {
    return wcn_v128i_atomic_load(ptr, order);
}

WCN_INLINE wcn_v128f_t wcn_v128f_atomic_load_aligned(const wcn_v128f_t* ptr, wcn_memory_order_t order) {
    return wcn_v128f_atomic_load(ptr, order);
}

WCN_INLINE void wcn_v128i_atomic_store_aligned(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order) {
    wcn_v128i_atomic_store(ptr, value, order);
}

WCN_INLINE void wcn_v128f_atomic_store_aligned(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order) {
    wcn_v128f_atomic_store(ptr, value, order);
}

/* ========== Atomic Compare-and-Swap Operations ========== */

/* WebAssembly SIMD128 atomic compare-and-exchange using scalar CAS loop */
WCN_INLINE int wcn_v128i_atomic_compare_exchange_strong(wcn_v128i_t* ptr, wcn_v128i_t* expected,
                                                         wcn_v128i_t desired, wcn_memory_order_t success_order,
                                                         wcn_memory_order_t failure_order) {
    int success = 1;
    volatile int32_t* target = (volatile int32_t*)ptr;
    int32_t* exp = (int32_t*)expected;
    int32_t* des = (int32_t*)&desired;
    
    /* Apply appropriate memory barrier before operation */
    if (success_order == WCN_MEMORY_ORDER_SEQ_CST || failure_order == WCN_MEMORY_ORDER_SEQ_CST ||
        success_order == WCN_MEMORY_ORDER_ACQ_REL || failure_order == WCN_MEMORY_ORDER_ACQ_REL) {
        wcn_wasm_memory_fence(WCN_MEMORY_ORDER_ACQ_REL);
    }
    
    /* Compare each element using WebAssembly atomic instructions */
    for (int i = 0; i < 4; i++) {
        int32_t expected_val = exp[i];
        int32_t old_val = __builtin_wasm_atomic_compare_exchange_i32((int*)(&target[i]), expected_val, des[i], 0x0);
        if (old_val != expected_val) {
            success = 0;
            /* Update expected value with actual value */
            exp[i] = old_val;
        }
    }
    
    /* Apply appropriate memory barrier after operation */
    if (success) {
        if (success_order == WCN_MEMORY_ORDER_SEQ_CST) {
            wcn_wasm_memory_fence(WCN_MEMORY_ORDER_SEQ_CST);
        } else if (success_order == WCN_MEMORY_ORDER_RELEASE) {
            wcn_wasm_memory_fence(WCN_MEMORY_ORDER_RELEASE);
        }
    } else {
        if (failure_order == WCN_MEMORY_ORDER_SEQ_CST) {
            wcn_wasm_memory_fence(WCN_MEMORY_ORDER_SEQ_CST);
        }
    }
    
    return success;
}

WCN_INLINE int wcn_v128f_atomic_compare_exchange_strong(wcn_v128f_t* ptr, wcn_v128f_t* expected,
                                                         wcn_v128f_t desired, wcn_memory_order_t success_order,
                                                         wcn_memory_order_t failure_order) {
    /* Reinterpret as integers for CAS */
    return wcn_v128i_atomic_compare_exchange_strong((wcn_v128i_t*)ptr, (wcn_v128i_t*)expected,
                                                     *(wcn_v128i_t*)&desired, success_order, failure_order);
}

/* Weak CAS is the same as strong for WebAssembly */
WCN_INLINE int wcn_v128i_atomic_compare_exchange_weak(wcn_v128i_t* ptr, wcn_v128i_t* expected,
                                                       wcn_v128i_t desired, wcn_memory_order_t success_order,
                                                       wcn_memory_order_t failure_order) {
    return wcn_v128i_atomic_compare_exchange_strong(ptr, expected, desired, success_order, failure_order);
}

WCN_INLINE int wcn_v128f_atomic_compare_exchange_weak(wcn_v128f_t* ptr, wcn_v128f_t* expected,
                                                       wcn_v128f_t desired, wcn_memory_order_t success_order,
                                                       wcn_memory_order_t failure_order) {
    return wcn_v128f_atomic_compare_exchange_strong(ptr, expected, desired, success_order, failure_order);
}

/* ========== Atomic Exchange Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_atomic_exchange(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order) {
    wcn_v128i_t old_value;
    volatile int32_t* target = (volatile int32_t*)ptr;
    int32_t* new_val = (int32_t*)&value;
    int32_t* old_val = (int32_t*)&old_value;
    
    /* Apply appropriate memory barrier before operation */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_wasm_memory_fence(order);
    }
    
    /* Exchange each element using atomic operations */
    for (int i = 0; i < 4; i++) {
        old_val[i] = __builtin_wasm_atomic_exchange_i32((int*)(&target[i]), new_val[i], 0x0);
    }
    
    /* Apply appropriate memory barrier after operation */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_wasm_memory_fence(order);
    }
    
    return old_value;
}

WCN_INLINE wcn_v128f_t wcn_v128f_atomic_exchange(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order) {
    /* Reinterpret as integers */
    wcn_v128i_t result = wcn_v128i_atomic_exchange((wcn_v128i_t*)ptr, *(wcn_v128i_t*)&value, order);
    return *(wcn_v128f_t*)&result;
}

/* ========== Memory Fence Operations ========== */

WCN_INLINE void wcn_atomic_thread_fence(wcn_memory_order_t order) {
    wcn_wasm_memory_fence(order);
}

WCN_INLINE void wcn_atomic_signal_fence(wcn_memory_order_t order) {
    /* Signal fence is just a compiler barrier */
    __asm__ volatile("" ::: "memory");
    (void)order; /* Avoid unused parameter warning */
}

#else /* !WCN_WASM_HAS_ATOMIC_BUILTINS */

/* Fallback: atomic operations not available, use regular memory operations with compiler barriers */

WCN_INLINE wcn_v128i_t wcn_v128i_atomic_load(const wcn_v128i_t* ptr, wcn_memory_order_t order) {
    __asm__ volatile("" ::: "memory");
    wcn_v128i_t result = *ptr;
    __asm__ volatile("" ::: "memory");
    (void)order;
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_atomic_load(const wcn_v128f_t* ptr, wcn_memory_order_t order) {
    __asm__ volatile("" ::: "memory");
    wcn_v128f_t result = *ptr;
    __asm__ volatile("" ::: "memory");
    (void)order;
    return result;
}

WCN_INLINE void wcn_v128i_atomic_store(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order) {
    __asm__ volatile("" ::: "memory");
    *ptr = value;
    __asm__ volatile("" ::: "memory");
    (void)order;
}

WCN_INLINE void wcn_v128f_atomic_store(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order) {
    __asm__ volatile("" ::: "memory");
    *ptr = value;
    __asm__ volatile("" ::: "memory");
    (void)order;
}

WCN_INLINE wcn_v128i_t wcn_v128i_atomic_load_aligned(const wcn_v128i_t* ptr, wcn_memory_order_t order) {
    return wcn_v128i_atomic_load(ptr, order);
}

WCN_INLINE wcn_v128f_t wcn_v128f_atomic_load_aligned(const wcn_v128f_t* ptr, wcn_memory_order_t order) {
    return wcn_v128f_atomic_load(ptr, order);
}

WCN_INLINE void wcn_v128i_atomic_store_aligned(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order) {
    wcn_v128i_atomic_store(ptr, value, order);
}

WCN_INLINE void wcn_v128f_atomic_store_aligned(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order) {
    wcn_v128f_atomic_store(ptr, value, order);
}

WCN_INLINE int wcn_v128i_atomic_compare_exchange_strong(wcn_v128i_t* ptr, wcn_v128i_t* expected,
                                                         wcn_v128i_t desired, wcn_memory_order_t success_order,
                                                         wcn_memory_order_t failure_order) {
    __asm__ volatile("" ::: "memory");
    int success = (ptr->raw[0] == expected->raw[0] && ptr->raw[1] == expected->raw[1]);
    if (success) {
        *ptr = desired;
    } else {
        *expected = *ptr;
    }
    __asm__ volatile("" ::: "memory");
    (void)success_order; (void)failure_order;
    return success;
}

WCN_INLINE int wcn_v128f_atomic_compare_exchange_strong(wcn_v128f_t* ptr, wcn_v128f_t* expected,
                                                         wcn_v128f_t desired, wcn_memory_order_t success_order,
                                                         wcn_memory_order_t failure_order) {
    return wcn_v128i_atomic_compare_exchange_strong((wcn_v128i_t*)ptr, (wcn_v128i_t*)expected,
                                                     *(wcn_v128i_t*)&desired, success_order, failure_order);
}

WCN_INLINE int wcn_v128i_atomic_compare_exchange_weak(wcn_v128i_t* ptr, wcn_v128i_t* expected,
                                                       wcn_v128i_t desired, wcn_memory_order_t success_order,
                                                       wcn_memory_order_t failure_order) {
    return wcn_v128i_atomic_compare_exchange_strong(ptr, expected, desired, success_order, failure_order);
}

WCN_INLINE int wcn_v128f_atomic_compare_exchange_weak(wcn_v128f_t* ptr, wcn_v128f_t* expected,
                                                       wcn_v128f_t desired, wcn_memory_order_t success_order,
                                                       wcn_memory_order_t failure_order) {
    return wcn_v128f_atomic_compare_exchange_strong(ptr, expected, desired, success_order, failure_order);
}

WCN_INLINE wcn_v128i_t wcn_v128i_atomic_exchange(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order) {
    __asm__ volatile("" ::: "memory");
    wcn_v128i_t old_value = *ptr;
    *ptr = value;
    __asm__ volatile("" ::: "memory");
    (void)order;
    return old_value;
}

WCN_INLINE wcn_v128f_t wcn_v128f_atomic_exchange(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order) {
    wcn_v128i_t result = wcn_v128i_atomic_exchange((wcn_v128i_t*)ptr, *(wcn_v128i_t*)&value, order);
    return *(wcn_v128f_t*)&result;
}

WCN_INLINE void wcn_atomic_thread_fence(wcn_memory_order_t order) {
    __asm__ volatile("" ::: "memory");
    (void)order;
}

WCN_INLINE void wcn_atomic_signal_fence(wcn_memory_order_t order) {
    __asm__ volatile("" ::: "memory");
    (void)order;
}

#endif /* WCN_WASM_HAS_ATOMIC_BUILTINS */

#endif /* WCN_WASM_SIMD128 */

#endif /* WCN_WASM_SIMD128_ATOMIC_H */