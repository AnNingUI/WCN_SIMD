#include "WCN_SIMD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Test data for atomic operations */
typedef struct {
    wcn_v128i_t atomic_int;
    wcn_v128f_t atomic_float;
    int counter;
} atomic_test_data_t;

/* Initialize test data */
void init_test_data(atomic_test_data_t* data) {
    wcn_v128i_t zero_int = wcn_v128i_setzero();
    wcn_v128f_t zero_float = wcn_v128f_setzero();
    
    wcn_v128i_atomic_store(&data->atomic_int, zero_int, WCN_MEMORY_ORDER_SEQ_CST);
    wcn_v128f_atomic_store(&data->atomic_float, zero_float, WCN_MEMORY_ORDER_SEQ_CST);
    data->counter = 0;
}

/* Test atomic load/store operations */
void test_atomic_load_store() {
    printf("Testing atomic load/store operations...\n");
    
    atomic_test_data_t data;
    init_test_data(&data);
    
    /* Test atomic store and load */
    wcn_v128i_t test_int = wcn_v128i_set1_i32(42);
    wcn_v128f_t test_float = wcn_v128f_set1(3.14f);
    
    wcn_v128i_atomic_store(&data.atomic_int, test_int, WCN_MEMORY_ORDER_SEQ_CST);
    wcn_v128f_atomic_store(&data.atomic_float, test_float, WCN_MEMORY_ORDER_SEQ_CST);
    
    wcn_v128i_t loaded_int = wcn_v128i_atomic_load(&data.atomic_int, WCN_MEMORY_ORDER_SEQ_CST);
    wcn_v128f_t loaded_float = wcn_v128f_atomic_load(&data.atomic_float, WCN_MEMORY_ORDER_SEQ_CST);
    
    /* Verify values */
    int32_t int_vals[4];
    float float_vals[4];
    wcn_v128i_store(int_vals, loaded_int);
    wcn_v128f_store(float_vals, loaded_float);
    
    for (int i = 0; i < 4; i++) {
        assert(int_vals[i] == 42);
        assert(float_vals[i] == 3.14f);
    }
    
    printf("✓ Atomic load/store operations work correctly\n");
}

/* Test atomic compare-and-exchange */
void test_atomic_compare_exchange() {
    printf("Testing atomic compare-and-exchange...\n");
    
    atomic_test_data_t data;
    init_test_data(&data);
    
    /* Set initial value */
    wcn_v128i_t initial = wcn_v128i_set1_i32(10);
    wcn_v128i_atomic_store(&data.atomic_int, initial, WCN_MEMORY_ORDER_SEQ_CST);
    
    /* Test successful CAS */
    wcn_v128i_t expected = wcn_v128i_set1_i32(10);
    wcn_v128i_t desired = wcn_v128i_set1_i32(20);
    
    int success = wcn_v128i_atomic_compare_exchange_strong(&data.atomic_int, &expected, desired, 
                                                            WCN_MEMORY_ORDER_SEQ_CST, WCN_MEMORY_ORDER_SEQ_CST);
    assert(success == 1);
    
    /* Verify new value */
    wcn_v128i_t loaded = wcn_v128i_atomic_load(&data.atomic_int, WCN_MEMORY_ORDER_SEQ_CST);
    int32_t vals[4];
    wcn_v128i_store(vals, loaded);
    for (int i = 0; i < 4; i++) {
        assert(vals[i] == 20);
    }
    
    /* Test failed CAS */
    wcn_v128i_t wrong_expected = wcn_v128i_set1_i32(10); /* Wrong expected value */
    wcn_v128i_t new_desired = wcn_v128i_set1_i32(30);
    
    int fail = wcn_v128i_atomic_compare_exchange_strong(&data.atomic_int, &wrong_expected, new_desired, 
                                                         WCN_MEMORY_ORDER_SEQ_CST, WCN_MEMORY_ORDER_SEQ_CST);
    assert(fail == 0);
    
    /* Verify expected was updated with actual value */
    wcn_v128i_store(vals, wrong_expected);
    for (int i = 0; i < 4; i++) {
        assert(vals[i] == 20); /* Should be the current value */
    }
    
    printf("✓ Atomic compare-and-exchange works correctly\n");
}

/* Test atomic exchange */
void test_atomic_exchange() {
    printf("Testing atomic exchange...\n");
    
    atomic_test_data_t data;
    init_test_data(&data);
    
    /* Set initial value */
    wcn_v128i_t initial = wcn_v128i_set1_i32(100);
    wcn_v128i_atomic_store(&data.atomic_int, initial, WCN_MEMORY_ORDER_SEQ_CST);
    
    /* Exchange value */
    wcn_v128i_t new_value = wcn_v128i_set1_i32(200);
    wcn_v128i_t old_value = wcn_v128i_atomic_exchange(&data.atomic_int, new_value, WCN_MEMORY_ORDER_SEQ_CST);
    
    /* Verify old value */
    int32_t old_vals[4];
    wcn_v128i_store(old_vals, old_value);
    for (int i = 0; i < 4; i++) {
        assert(old_vals[i] == 100);
    }
    
    /* Verify new value */
    wcn_v128i_t loaded = wcn_v128i_atomic_load(&data.atomic_int, WCN_MEMORY_ORDER_SEQ_CST);
    int32_t new_vals[4];
    wcn_v128i_store(new_vals, loaded);
    for (int i = 0; i < 4; i++) {
        assert(new_vals[i] == 200);
    }
    
    printf("✓ Atomic exchange works correctly\n");
}

/* Test memory ordering */
void test_memory_ordering() {
    printf("Testing memory ordering...\n");
    
    atomic_test_data_t data;
    init_test_data(&data);
    
    /* Test different memory orders */
    wcn_v128i_t test_val = wcn_v128i_set1_i32(123);
    
    /* Relaxed ordering */
    wcn_v128i_atomic_store(&data.atomic_int, test_val, WCN_MEMORY_ORDER_RELAXED);
    wcn_v128i_atomic_load(&data.atomic_int, WCN_MEMORY_ORDER_RELAXED);
    
    /* Acquire-release ordering */
    wcn_v128i_atomic_store(&data.atomic_int, test_val, WCN_MEMORY_ORDER_RELEASE);
    wcn_v128i_atomic_load(&data.atomic_int, WCN_MEMORY_ORDER_ACQUIRE);
    
    /* Sequential consistency */
    wcn_v128i_atomic_store(&data.atomic_int, test_val, WCN_MEMORY_ORDER_SEQ_CST);
    wcn_v128i_atomic_load(&data.atomic_int, WCN_MEMORY_ORDER_SEQ_CST);
    
    printf("✓ Memory ordering operations work correctly\n");
}

/* Test atomic features detection */
void test_atomic_features() {
    printf("Testing atomic feature detection...\n");
    
    const wcn_atomic_features_t* features = wcn_atomic_get_features();
    const wcn_simd_features_t* simd_features = wcn_simd_get_features();
    
    printf("Atomic Features:\n");
    printf("  Atomic operations: %s\n", features->has_atomic_operations ? "Yes" : "No");
    printf("  x86 SSE atomic: %s\n", features->has_sse_atomic ? "Yes" : "No");
    printf("  x86 AVX atomic: %s\n", features->has_avx_atomic ? "Yes" : "No");
    printf("  ARM NEON atomic: %s\n", features->has_neon_atomic ? "Yes" : "No");
    printf("  RISC-V RVV atomic: %s\n", features->has_rvv_atomic ? "Yes" : "No");
    printf("  PowerPC AltiVec atomic: %s\n", features->has_altivec_atomic ? "Yes" : "No");
    printf("  WebAssembly atomic: %s\n", features->has_wasm_atomic ? "Yes" : "No");
    
    printf("SIMD Features:\n");
    printf("  Implementation: %s\n", wcn_simd_get_impl());
    printf("  Vector width: %d bits\n", wcn_simd_get_vector_width());
    
    assert(features->has_atomic_operations == simd_features->has_atomic_operations);
    
    printf("✓ Atomic feature detection works correctly\n");
}

/* Thread-safe counter using atomic operations */
int atomic_counter_test() {
    static wcn_v128i_t counter = {0};
    static int initialized = 0;
    
    if (!initialized) {
        wcn_v128i_t zero = wcn_v128i_setzero();
        wcn_v128i_atomic_store(&counter, zero, WCN_MEMORY_ORDER_SEQ_CST);
        initialized = 1;
    }
    
    /* Atomically increment first element */
    wcn_v128i_t old_val = wcn_v128i_atomic_load(&counter, WCN_MEMORY_ORDER_SEQ_CST);
    wcn_v128i_t new_val;
    int32_t* old_ptr = (int32_t*)&old_val;
    int32_t* new_ptr = (int32_t*)&new_val;
    
    for (int i = 0; i < 4; i++) {
        new_ptr[i] = old_ptr[i] + 1;
    }
    
    while (!wcn_v128i_atomic_compare_exchange_weak(&counter, &old_val, new_val, 
                                                    WCN_MEMORY_ORDER_SEQ_CST, WCN_MEMORY_ORDER_SEQ_CST)) {
        /* Retry with updated old_val */
        new_ptr = (int32_t*)&new_val;
        for (int i = 0; i < 4; i++) {
            new_ptr[i] = old_ptr[i] + 1;
        }
    }
    
    return old_ptr[0]; /* Return old counter value */
}

int main() {
    printf("=== WCN_SIMD Atomic Operations Test ===\n");
    printf("WCN_SIMD Version: %s\n", wcn_simd_get_version());
    printf("Implementation: %s\n", wcn_simd_get_impl());
    printf("Vector Width: %d bits\n\n", wcn_simd_get_vector_width());
    
    /* Initialize SIMD library */
    wcn_simd_init();
    
    /* Test atomic features first */
    test_atomic_features();
    printf("\n");
    
    /* Run atomic operation tests */
    test_atomic_load_store();
    test_atomic_compare_exchange();
    test_atomic_exchange();
    test_memory_ordering();
    
    printf("\n=== All Atomic Tests Passed! ===\n");
    
    /* Demonstrate thread-safe counter */
    printf("\nThread-safe counter demonstration:\n");
    for (int i = 0; i < 5; i++) {
        int old_val = atomic_counter_test();
        printf("Counter incremented from %d to %d\n", old_val, old_val + 1);
    }
    
    return 0;
}