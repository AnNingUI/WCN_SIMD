#ifndef WCN_SIMD_ATOMIC_H
#define WCN_SIMD_ATOMIC_H

/*
 * WCN_SIMD Atomic Operations
 * 
 * Cross-platform atomic SIMD operations for thread-safe vector processing
 * Supports atomic memory operations, fetch-and-modify, and compare-and-swap
 */

#include "wcn_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========== Memory Ordering Enums ========== */

typedef enum {
    WCN_MEMORY_ORDER_RELAXED = 0,  /* No ordering constraints */
    WCN_MEMORY_ORDER_CONSUME = 1,  /* Consume ordering */
    WCN_MEMORY_ORDER_ACQUIRE = 2,  /* Acquire ordering */
    WCN_MEMORY_ORDER_RELEASE = 3,  /* Release ordering */
    WCN_MEMORY_ORDER_ACQ_REL = 4,  /* Acquire-release ordering */
    WCN_MEMORY_ORDER_SEQ_CST = 5   /* Sequential consistency */
} wcn_memory_order_t;

/* ========== Atomic Load/Store Operations ========== */

/* Atomic load with specified memory ordering */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_load(const wcn_v128i_t* ptr, wcn_memory_order_t order);
WCN_INLINE wcn_v128f_t wcn_v128f_atomic_load(const wcn_v128f_t* ptr, wcn_memory_order_t order);
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_load(const wcn_v128d_t* ptr, wcn_memory_order_t order);

/* Atomic store with specified memory ordering */
WCN_INLINE void wcn_v128i_atomic_store(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);
WCN_INLINE void wcn_v128f_atomic_store(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order);
WCN_INLINE void wcn_v128d_atomic_store(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order);

/* Aligned atomic operations */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_load_aligned(const wcn_v128i_t* ptr, wcn_memory_order_t order);
WCN_INLINE wcn_v128f_t wcn_v128f_atomic_load_aligned(const wcn_v128f_t* ptr, wcn_memory_order_t order);
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_load_aligned(const wcn_v128d_t* ptr, wcn_memory_order_t order);

WCN_INLINE void wcn_v128i_atomic_store_aligned(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);
WCN_INLINE void wcn_v128f_atomic_store_aligned(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order);
WCN_INLINE void wcn_v128d_atomic_store_aligned(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order);

/* ========== Atomic Fetch-and-Modify Operations ========== */

/* Atomic fetch-and-add (returns previous value) */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_fetch_add(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128f_t wcn_v128f_atomic_fetch_add(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_fetch_add(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order);

/* Atomic fetch-and-subtract (returns previous value) */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_fetch_sub(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128f_t wcn_v128f_atomic_fetch_sub(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_fetch_sub(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order);

/* Atomic fetch-and-bitwise-and (returns previous value) */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_fetch_and(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);

/* Atomic fetch-and-bitwise-or (returns previous value) */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_fetch_or(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);

/* Atomic fetch-and-bitwise-xor (returns previous value) */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_fetch_xor(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);

/* Atomic fetch-and-minimum (returns previous value) */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_fetch_min(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128f_t wcn_v128f_atomic_fetch_min(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_fetch_min(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order);

/* Atomic fetch-and-maximum (returns previous value) */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_fetch_max(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128f_t wcn_v128f_atomic_fetch_max(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_fetch_max(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order);

/* ========== Atomic Compare-and-Swap Operations ========== */

/* Atomic compare-and-exchange (strong) - returns 1 if successful, 0 if failed */
WCN_INLINE int wcn_v128i_atomic_compare_exchange_strong(wcn_v128i_t* ptr, wcn_v128i_t* expected,
                                              wcn_v128i_t desired, wcn_memory_order_t success_order,
                                              wcn_memory_order_t failure_order);
WCN_INLINE int wcn_v128f_atomic_compare_exchange_strong(wcn_v128f_t* ptr, wcn_v128f_t* expected,
                                              wcn_v128f_t desired, wcn_memory_order_t success_order,
                                              wcn_memory_order_t failure_order);
WCN_INLINE int wcn_v128d_atomic_compare_exchange_strong(wcn_v128d_t* ptr, wcn_v128d_t* expected,
                                              wcn_v128d_t desired, wcn_memory_order_t success_order,
                                              wcn_memory_order_t failure_order);

/* Atomic compare-and-exchange (weak) - may fail spuriously */
WCN_INLINE int wcn_v128i_atomic_compare_exchange_weak(wcn_v128i_t* ptr, wcn_v128i_t* expected,
                                            wcn_v128i_t desired, wcn_memory_order_t success_order,
                                            wcn_memory_order_t failure_order);
WCN_INLINE int wcn_v128f_atomic_compare_exchange_weak(wcn_v128f_t* ptr, wcn_v128f_t* expected,
                                            wcn_v128f_t desired, wcn_memory_order_t success_order,
                                            wcn_memory_order_t failure_order);
WCN_INLINE int wcn_v128d_atomic_compare_exchange_weak(wcn_v128d_t* ptr, wcn_v128d_t* expected,
                                            wcn_v128d_t desired, wcn_memory_order_t success_order,
                                            wcn_memory_order_t failure_order);

/* ========== Atomic Exchange Operations ========== */

/* Atomic exchange (swap) - returns previous value */
WCN_INLINE wcn_v128i_t wcn_v128i_atomic_exchange(wcn_v128i_t* ptr, wcn_v128i_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128f_t wcn_v128f_atomic_exchange(wcn_v128f_t* ptr, wcn_v128f_t value, wcn_memory_order_t order);
WCN_INLINE wcn_v128d_t wcn_v128d_atomic_exchange(wcn_v128d_t* ptr, wcn_v128d_t value, wcn_memory_order_t order);

/* ========== Memory Fence Operations ========== */

/* Memory fence/barrier */
WCN_INLINE void wcn_atomic_thread_fence(wcn_memory_order_t order);
WCN_INLINE void wcn_atomic_signal_fence(wcn_memory_order_t order);

/* ========== Platform-Specific Atomic Features ========== */

typedef struct {
    /* x86 features */
    int has_sse_atomic;
    int has_avx_atomic;
    int has_avx512_atomic;
    int has_clflush;
    int has_clflushopt;
    int has_clwb;
    
    /* ARM features */
    int has_neon_atomic;
    int has_sve_atomic;
    int has_lse;  /* Large System Extensions */
    
    /* LoongArch features */
    int has_lsx_atomic;
    int has_lasx_atomic;
    
    /* RISC-V features */
    int has_rvv_atomic;
    int has_zam;  /* Atomic memory operations */
    int has_ztso; /* Total Store Ordering */
    
    /* PowerPC features */
    int has_altivec_atomic;
    int has_vsx_atomic;
    int has_lwsync;
    int has_isync;
    
    /* MIPS features */
    int has_msa_atomic;
    
    /* WebAssembly features */
    int has_wasm_atomic;
    
    /* Generic atomic availability */
    int has_atomic_operations;
} wcn_atomic_features_t;

/* Get atomic feature detection */
const wcn_atomic_features_t* wcn_atomic_get_features(void);

/* ========== Utility Functions ========== */

/* Check if atomic operations are supported on current platform */
WCN_INLINE int wcn_atomic_is_supported(void) {
    const wcn_atomic_features_t* features = wcn_atomic_get_features();
    return features && features->has_atomic_operations;
}

/* Platform-specific memory alignment for atomic operations */
#define WCN_ATOMIC_ALIGNMENT 16

#ifdef __cplusplus
}
#endif

#endif /* WCN_SIMD_ATOMIC_H */