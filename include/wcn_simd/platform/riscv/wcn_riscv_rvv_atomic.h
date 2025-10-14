#ifndef WCN_RISCV_RVV_ATOMIC_H
#define WCN_RISCV_RVV_ATOMIC_H

#include "../../wcn_atomic.h"

#ifdef WCN_RISCV_RVV

#include <riscv_vector.h>

/* RISC-V RVV atomic operations with proper memory barriers */

/* ========== Memory Ordering Helpers ========== */

WCN_INLINE void wcn_riscv_memory_fence(wcn_memory_order_t order) {
    switch (order) {
        case WCN_MEMORY_ORDER_RELAXED:
            break;
        case WCN_MEMORY_ORDER_CONSUME:
        case WCN_MEMORY_ORDER_ACQUIRE:
            /* Load fence */
            __asm__ volatile("fence ir, iw" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_RELEASE:
            /* Store fence */
            __asm__ volatile("fence ow, ow" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_ACQ_REL:
            /* Full fence */
            __asm__ volatile("fence iorw, iorw" ::: "memory");
            break;
        case WCN_MEMORY_ORDER_SEQ_CST:
            /* Sequential consistency */
            __asm__ volatile("fence iorw, iorw" ::: "memory");
            break;
    }
}

/* ========== Scalable Atomic Load/Store Operations ========== */

/* RISC-V RVV atomic loads with memory barriers */
WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_i32_atomic_load(const wcn_vscalable_i32_t* ptr, 
                                                               wcn_memory_order_t order, size_t vl) {
    wcn_vscalable_i32_t result;
    
    /* Apply appropriate memory barrier before load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_riscv_memory_fence(order);
    }
    
    /* Load the vector */
    result = wcn_vscalable_i32_load((const int32_t*)ptr, vl);
    
    /* Apply appropriate memory barrier after load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_riscv_memory_fence(order);
    }
    
    return result;
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_atomic_load(const wcn_vscalable_f32_t* ptr, 
                                                               wcn_memory_order_t order, size_t vl) {
    wcn_vscalable_f32_t result;
    
    /* Apply appropriate memory barrier before load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_riscv_memory_fence(order);
    }
    
    /* Load the vector */
    result = wcn_vscalable_f32_load((const float*)ptr, vl);
    
    /* Apply appropriate memory barrier after load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_riscv_memory_fence(order);
    }
    
    return result;
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_atomic_load(const wcn_vscalable_f64_t* ptr, 
                                                               wcn_memory_order_t order, size_t vl) {
    wcn_vscalable_f64_t result;
    
    /* Apply appropriate memory barrier before load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_riscv_memory_fence(order);
    }
    
    /* Load the vector */
    result = wcn_vscalable_f64_load((const double*)ptr, vl);
    
    /* Apply appropriate memory barrier after load */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_riscv_memory_fence(order);
    }
    
    return result;
}

/* RISC-V RVV atomic stores with memory barriers */
WCN_INLINE void wcn_vscalable_i32_atomic_store(wcn_vscalable_i32_t* ptr, wcn_vscalable_i32_t value, 
                                                wcn_memory_order_t order, size_t vl) {
    /* Apply appropriate memory barrier before store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_riscv_memory_fence(order);
    }
    
    /* Store the vector */
    wcn_vscalable_i32_store((int32_t*)ptr, value, vl);
    
    /* Apply appropriate memory barrier after store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_riscv_memory_fence(order);
    }
}

WCN_INLINE void wcn_vscalable_f32_atomic_store(wcn_vscalable_f32_t* ptr, wcn_vscalable_f32_t value, 
                                                wcn_memory_order_t order, size_t vl) {
    /* Apply appropriate memory barrier before store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_riscv_memory_fence(order);
    }
    
    /* Store the vector */
    wcn_vscalable_f32_store((float*)ptr, value, vl);
    
    /* Apply appropriate memory barrier after store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_riscv_memory_fence(order);
    }
}

WCN_INLINE void wcn_vscalable_f64_atomic_store(wcn_vscalable_f64_t* ptr, wcn_vscalable_f64_t value, 
                                                wcn_memory_order_t order, size_t vl) {
    /* Apply appropriate memory barrier before store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_riscv_memory_fence(order);
    }
    
    /* Store the vector */
    wcn_vscalable_f64_store((double*)ptr, value, vl);
    
    /* Apply appropriate memory barrier after store */
    if (order == WCN_MEMORY_ORDER_SEQ_CST) {
        wcn_riscv_memory_fence(order);
    }
}

/* ========== Atomic Compare-and-Swap Operations ========== */

/* RISC-V RVV atomic compare-and-exchange using scalar CAS loop */
WCN_INLINE int wcn_vscalable_i32_atomic_compare_exchange_strong(wcn_vscalable_i32_t* ptr, 
                                                                   wcn_vscalable_i32_t* expected,
                                                                   wcn_vscalable_i32_t desired, 
                                                                   wcn_memory_order_t success_order,
                                                                   wcn_memory_order_t failure_order,
                                                                   size_t vl) {
    int success = 1;
    vint32m1_t* target = (vint32m1_t*)ptr;
    vint32m1_t* exp = (vint32m1_t*)expected;
    vint32m1_t* des = (vint32m1_t*)&desired;
    
    /* Apply appropriate memory barrier before operation */
    if (success_order == WCN_MEMORY_ORDER_SEQ_CST || failure_order == WCN_MEMORY_ORDER_SEQ_CST ||
        success_order == WCN_MEMORY_ORDER_ACQ_REL || failure_order == WCN_MEMORY_ORDER_ACQ_REL) {
        wcn_riscv_memory_fence(WCN_MEMORY_ORDER_ACQ_REL);
    }
    
    /* Compare each element using RISC-V atomic instructions */
    for (size_t i = 0; i < vl; i++) {
        int32_t expected_val = __riscv_vmv_x_s_i32m1_i32(*exp, i);
        int32_t old_val = __sync_val_compare_and_swap(&((int32_t*)target)[i], expected_val, 
                                                        __riscv_vmv_x_s_i32m1_i32(*des, i));
        if (old_val != expected_val) {
            success = 0;
            /* Update expected value with actual value */
            *exp = __riscv_vslide1up_vx_i32m1(*exp, old_val, vl);
        }
    }
    
    /* Apply appropriate memory barrier after operation */
    if (success) {
        if (success_order == WCN_MEMORY_ORDER_SEQ_CST) {
            wcn_riscv_memory_fence(WCN_MEMORY_ORDER_SEQ_CST);
        } else if (success_order == WCN_MEMORY_ORDER_RELEASE) {
            wcn_riscv_memory_fence(WCN_MEMORY_ORDER_RELEASE);
        }
    } else {
        if (failure_order == WCN_MEMORY_ORDER_SEQ_CST) {
            wcn_riscv_memory_fence(WCN_MEMORY_ORDER_SEQ_CST);
        }
    }
    
    return success;
}

WCN_INLINE int wcn_vscalable_f32_atomic_compare_exchange_strong(wcn_vscalable_f32_t* ptr, 
                                                                   wcn_vscalable_f32_t* expected,
                                                                   wcn_vscalable_f32_t desired, 
                                                                   wcn_memory_order_t success_order,
                                                                   wcn_memory_order_t failure_order,
                                                                   size_t vl) {
    /* Reinterpret as integers for CAS */
    return wcn_vscalable_i32_atomic_compare_exchange_strong((wcn_vscalable_i32_t*)ptr, 
                                                             (wcn_vscalable_i32_t*)expected,
                                                             *(wcn_vscalable_i32_t*)&desired, 
                                                             success_order, failure_order, vl);
}

WCN_INLINE int wcn_vscalable_f64_atomic_compare_exchange_strong(wcn_vscalable_f64_t* ptr, 
                                                                   wcn_vscalable_f64_t* expected,
                                                                   wcn_vscalable_f64_t desired, 
                                                                   wcn_memory_order_t success_order,
                                                                   wcn_memory_order_t failure_order,
                                                                   size_t vl) {
    /* Use integer CAS on the bit representation */
    wcn_vscalable_i32_t* int_ptr = (wcn_vscalable_i32_t*)ptr;
    wcn_vscalable_i32_t* int_expected = (wcn_vscalable_i32_t*)expected;
    wcn_vscalable_i32_t int_desired = *(wcn_vscalable_i32_t*)&desired;
    
    return wcn_vscalable_i32_atomic_compare_exchange_strong(int_ptr, int_expected, int_desired,
                                                             success_order, failure_order, vl);
}

/* Weak CAS is the same as strong for RISC-V */
WCN_INLINE int wcn_vscalable_i32_atomic_compare_exchange_weak(wcn_vscalable_i32_t* ptr, 
                                                                 wcn_vscalable_i32_t* expected,
                                                                 wcn_vscalable_i32_t desired, 
                                                                 wcn_memory_order_t success_order,
                                                                 wcn_memory_order_t failure_order,
                                                                 size_t vl) {
    return wcn_vscalable_i32_atomic_compare_exchange_strong(ptr, expected, desired, 
                                                             success_order, failure_order, vl);
}

WCN_INLINE int wcn_vscalable_f32_atomic_compare_exchange_weak(wcn_vscalable_f32_t* ptr, 
                                                                 wcn_vscalable_f32_t* expected,
                                                                 wcn_vscalable_f32_t desired, 
                                                                 wcn_memory_order_t success_order,
                                                                 wcn_memory_order_t failure_order,
                                                                 size_t vl) {
    return wcn_vscalable_f32_atomic_compare_exchange_strong(ptr, expected, desired, 
                                                             success_order, failure_order, vl);
}

WCN_INLINE int wcn_vscalable_f64_atomic_compare_exchange_weak(wcn_vscalable_f64_t* ptr, 
                                                                 wcn_vscalable_f64_t* expected,
                                                                 wcn_vscalable_f64_t desired, 
                                                                 wcn_memory_order_t success_order,
                                                                 wcn_memory_order_t failure_order,
                                                                 size_t vl) {
    return wcn_vscalable_f64_atomic_compare_exchange_strong(ptr, expected, desired, 
                                                             success_order, failure_order, vl);
}

/* ========== Atomic Exchange Operations ========== */

WCN_INLINE wcn_vscalable_i32_t wcn_vscalable_i32_atomic_exchange(wcn_vscalable_i32_t* ptr, 
                                                                   wcn_vscalable_i32_t value, 
                                                                   wcn_memory_order_t order, size_t vl) {
    wcn_vscalable_i32_t old_value;
    vint32m1_t* target = (vint32m1_t*)ptr;
    vint32m1_t* new_val = (vint32m1_t*)&value;
    vint32m1_t* old_val = (vint32m1_t*)&old_value;
    
    /* Apply appropriate memory barrier before operation */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_ACQ_REL || 
        order == WCN_MEMORY_ORDER_ACQUIRE || order == WCN_MEMORY_ORDER_CONSUME) {
        wcn_riscv_memory_fence(order);
    }
    
    /* Exchange each element using atomic operations */
    for (size_t i = 0; i < vl; i++) {
        int32_t old = __sync_lock_test_and_set(&((int32_t*)target)[i], 
                                                __riscv_vmv_x_s_i32m1_i32(*new_val, i));
        *old_val = __riscv_vslide1up_vx_i32m1(*old_val, old, vl);
    }
    
    /* Apply appropriate memory barrier after operation */
    if (order == WCN_MEMORY_ORDER_SEQ_CST || order == WCN_MEMORY_ORDER_RELEASE) {
        wcn_riscv_memory_fence(order);
    }
    
    return old_value;
}

WCN_INLINE wcn_vscalable_f32_t wcn_vscalable_f32_atomic_exchange(wcn_vscalable_f32_t* ptr, 
                                                                   wcn_vscalable_f32_t value, 
                                                                   wcn_memory_order_t order, size_t vl) {
    /* Reinterpret as integers */
    wcn_vscalable_i32_t result = wcn_vscalable_i32_atomic_exchange((wcn_vscalable_i32_t*)ptr, 
                                                                    *(wcn_vscalable_i32_t*)&value, order, vl);
    return *(wcn_vscalable_f32_t*)&result;
}

WCN_INLINE wcn_vscalable_f64_t wcn_vscalable_f64_atomic_exchange(wcn_vscalable_f64_t* ptr, 
                                                                   wcn_vscalable_f64_t value, 
                                                                   wcn_memory_order_t order, size_t vl) {
    /* Use integer exchange on the bit representation */
    wcn_vscalable_i32_t* int_ptr = (wcn_vscalable_i32_t*)ptr;
    wcn_vscalable_i32_t int_value = *(wcn_vscalable_i32_t*)&value;
    wcn_vscalable_i32_t int_result = wcn_vscalable_i32_atomic_exchange(int_ptr, int_value, order, vl);
    return *(wcn_vscalable_f64_t*)&int_result;
}

/* ========== Memory Fence Operations ========== */

WCN_INLINE void wcn_atomic_thread_fence(wcn_memory_order_t order) {
    wcn_riscv_memory_fence(order);
}

WCN_INLINE void wcn_atomic_signal_fence(wcn_memory_order_t order) {
    /* Signal fence is just a compiler barrier */
    __asm__ volatile("fence i, i" ::: "memory");
}

#endif /* WCN_RISCV_RVV */

#endif /* WCN_RISCV_RVV_ATOMIC_H */