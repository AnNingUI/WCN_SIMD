#ifndef WCN_POWERPC_ALTIVEC_ATOMIC_H
#define WCN_POWERPC_ALTIVEC_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_POWERPC_ALTIVEC

#include <altivec.h>

/* PowerPC AltiVec atomic operations with proper memory barriers */

/* ========== Memory Ordering Helpers ========== */

WCN_INLINE void wcn_powerpc_memory_fence(wcn_memory_order_t order) {
    switch (order) {
        case WCN_MEMORY_ORDER_RELAXED:
            break;
        case WCN_MEMORY_ORDER_CONSUME:
        case WCN_MEMORY_ORDER_ACQUIRE:
            /* Lightweight sync */
            __asm__ volatile("lwsync" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_RELEASE:
            /* Lightweight sync */
            __asm__ volatile("lwsync" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_ACQ_REL:
            /* Full sync */
            __asm__ volatile("sync" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_SEQ_CST:
            /* Full sync */
            __asm__ volatile("sync" ::: "memory");
            break;
    }
}

/* ========== Atomic Load/Store Operations ========== */

/* PowerPC AltiVec atomic loads with memory barriers */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_load(const wcn_v128i_t* ptr, wcn_memory_order_t order) {
    wcn_v128i_t result;
    
    /* Apply appropriate memory barrier before load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_powerpc_memory_fence(order);
    }
    
    /* Load the vector */
    result = wcn_v128i_load(ptr);
    
    /* Apply appropriate memory barrier after load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_powerpc_memory_fence(order);
    }
    
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_atomic_load(const wcn_v128f_t* ptr, wcn_memory_order_t order) {
    wcn_v128f_t result;
    
    /* Apply appropriate memory barrier before load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_powerpc_memory_fence(order);
    }
    
    /* Load the vector */
    result = wcn_v128f_load(ptr);
    
    /* Apply appropriate memory barrier after load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_powerpc_memory_fence(order);
    }
    
    return result;
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_load(const wcn_v128d_t* ptr, wcn_memory_order_t order) {
    wcn_v128d_t result;
    
    /* Apply appropriate memory barrier before load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_powerpc_memory_fence(order);
    }
    
    /* Load the vector */
    result = wcn_v128d_load(ptr);
    
    /* Apply appropriate memory barrier after load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_powerpc_memory_fence(order);
    }
    
    return result;
}
#endif

/* PowerPC AltiVec atomic stores with memory barriers */
WCN_INLINE void wcn_v128i_atomic_store(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order) {
    /* Apply appropriate memory barrier before store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_powerpc_memory_fence(order);
    }
    
    /* Store the vector */
    wcn_v128i_store(ptr, value);
    
    /* Apply appropriate memory barrier after store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_powerpc_memory_fence(order);
    }
}

WCN_INLINE void wcn_v128f_atomic_store(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order) {
    /* Apply appropriate memory barrier before store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_powerpc_memory_fence(order);
    }
    
    /* Store the vector */
    wcn_v128f_store(ptr, value);
    
    /* Apply appropriate memory barrier after store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_powerpc_memory_fence(order);
    }
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE void wcn_v128d_atomic_store(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order) {
    /* Apply appropriate memory barrier before store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_powerpc_memory_fence(order);
    }
    
    /* Store the vector */
    wcn_v128d_store(ptr, value);
    
    /* Apply appropriate memory barrier after store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_powerpc_memory_fence(order);
    }
}
#endif

/* Aligned atomic operations (same as unaligned for AltiVec) */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_load_aligned(const wcn_v128i_t* ptr, wcn_memory_order_t order) {
    return wcn_v128i_atomic_load(ptr, order);
}

WCN_INLINE wcn_v128f_t wcn_v128f_atomic_load_aligned(const wcn_v128f_t* ptr, wcn_memory_order_t order) {
    return wcn_v128f_atomic_load(ptr, order);
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_load_aligned(const wcn_v128d_t* ptr, wcn_memory_order_t order) {
    return wcn_v128d_atomic_load(ptr, order);
}
#endif

WCN_INLINE void wcn_v128i_atomic_store_aligned(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order) {
    wcn_v128i_atomic_store(ptr, value, order);
}

WCN_INLINE void wcn_v128f_atomic_store_aligned(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order) {
    wcn_v128f_atomic_store(ptr, value, order);
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE void wcn_v128d_atomic_store_aligned(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order) {
    wcn_v128d_atomic_store(ptr, value, order);
}
#endif

/* ========== Atomic Compare-and-Swap Operations ========== */

/* PowerPC AltiVec atomic compare-and-exchange using scalar CAS loop with lwarx/stwcx */
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
        wcn_powerpc_memory_fence(WCN_MEMORY_ORDER_ACQ_REL);
    }
    
    /* Compare each element using PowerPC atomic instructions */
    for (int i = 0; i < 4; i++) {
        int32_t expected_val = exp[i];
        int32_t old_val;
        int retry;
        
        do {
            retry = 0;
            /* Load word and reserve */
            __asm__ volatile("lwarx %0, 0, %1" : "=&r"(old_val) : "r"(&target[i]));
            
            if (old_val != expected_val) {
                /* Value changed, update expected and fail */
                exp[i] = old_val;
                success = 0;
                /* Clear reservation */
                __asm__ volatile("stwcx. %0, 0, %1" :: "r"(old_val), "r"(&target[i]));
                break;
            }
            
            /* Store word conditional */
            __asm__ volatile("stwcx. %0, 0, %1" :: "r"(des[i]), "r"(&target[i]));
            
            /* Check if store succeeded */
            __asm__ volatile("mfcr %0" : "=&r"(retry));
            retry = (retry >> 29) & 1; /* Check CR0[EQ] bit */
            
        } while (!retry); /* Retry if reservation was lost */
        
        if (!success) break;
    }
    
    /* Apply appropriate memory barrier after operation */
    if (success) {
        if (success_order == WCN_MEMORY_ORDER_SEQ_CST) {
            wcn_powerpc_memory_fence(WCN_MEMORY_ORDER_SEQ_CST);
        } else if (success_order == WCN_MEMORY_ORDER_RELEASE) {
            wcn_powerpc_memory_fence(WCN_MEMORY_ORDER_RELEASE);
        }
    } else {
        if (failure_order == WCN_MEMORY_ORDER_SEQ_CST) {
            wcn_powerpc_memory_fence(WCN_MEMORY_ORDER_SEQ_CST);
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

#ifdef WCN_POWERPC_VSX
WCN_INLINE int wcn_v128d_atomic_compare_exchange_strong(wcn_v128d_t* ptr, wcn_v128d_t* expected,
                                                         wcn_v128d_t desired, wcn_memory_order_t success_order,
                                                         wcn_memory_order_t failure_order) {
    /* Use integer CAS on the bit representation */
    wcn_v128i_t* int_ptr = (wcn_v128i_t*)ptr;
    wcn_v128i_t* int_expected = (wcn_v128i_t*)expected;
    wcn_v128i_t int_desired = *(wcn_v128i_t*)&desired;
    
    return wcn_v128i_atomic_compare_exchange_strong(int_ptr, int_expected, int_desired,
                                                     success_order, failure_order);
}
#endif

/* Weak CAS is the same as strong for PowerPC */
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

#ifdef WCN_POWERPC_VSX
WCN_INLINE int wcn_v128d_atomic_compare_exchange_weak(wcn_v128d_t* ptr, wcn_v128d_t* expected,
                                                       wcn_v128d_t desired, wcn_memory_order_t success_order,
                                                       wcn_memory_order_t failure_order) {
    return wcn_v128d_atomic_compare_exchange_strong(ptr, expected, desired, success_order, failure_order);
}
#endif

/* ========== Atomic Exchange Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_atomic_exchange(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order) {
    wcn_v128i_t old_value;
    volatile int32_t* target = (volatile int32_t*)ptr;
    int32_t* new_val = (int32_t*)&value;
    int32_t* old_val = (int32_t*)&old_value;
    
    /* Apply appropriate memory barrier before operation */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_powerpc_memory_fence(order);
    }
    
    /* Exchange each element using atomic operations */
    for (int i = 0; i < 4; i++) {
        old_val[i] = __sync_lock_test_and_set(&target[i], new_val[i]);
    }
    
    /* Apply appropriate memory barrier after operation */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_powerpc_memory_fence(order);
    }
    
    return old_value;
}

WCN_INLINE wcn_v128f_t wcn_v128f_atomic_exchange(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order) {
    /* Reinterpret as integers */
    wcn_v128i_t result = wcn_v128i_atomic_exchange((wcn_v128i_t*)ptr, *(wcn_v128i_t*)&value, order);
    return *(wcn_v128f_t*)&result;
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_exchange(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order) {
    /* Use integer exchange on the bit representation */
    wcn_v128i_t* int_ptr = (wcn_v128i_t*)ptr;
    wcn_v128i_t int_value = *(wcn_v128i_t*)&value;
    wcn_v128i_t int_result = wcn_v128i_atomic_exchange(int_ptr, int_value, order);
    return *(wcn_v128d_t*)&int_result;
}
#endif

/* ========== Memory Fence Operations ========== */

WCN_INLINE void wcn_atomic_thread_fence(wcn_memory_order_t order) {
    wcn_powerpc_memory_fence(order);
}

WCN_INLINE void wcn_atomic_signal_fence(wcn_memory_order_t order) {
    /* Signal fence is just a compiler barrier */
    __asm__ volatile("lwsync" ::: "memory");
}

#endif /* WCN_POWERPC_ALTIVEC */

#endif /* WCN_POWERPC_ALTIVEC_ATOMIC_H */