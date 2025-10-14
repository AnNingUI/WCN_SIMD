#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <WCN_SIMD.h>

#define ARRAY_SIZE 10000
#define ITERATIONS 1000

void print_features(void) {
    const wcn_simd_features_t* features = wcn_simd_get_features();
    
    printf("=== WCN_SIMD Library Information ===\n");
    printf("Version: %s\n", wcn_simd_get_version());
    printf("Implementation: %s\n", wcn_simd_get_impl());
    printf("Vector Width: %d bits\n\n", wcn_simd_get_vector_width());
    
    printf("=== Detected SIMD Features ===\n");
    
    printf("x86 SIMD:\n");
    printf("  SSE2:       %s\n", features->has_sse2 ? "Yes" : "No");
    printf("  SSE3:       %s\n", features->has_sse3 ? "Yes" : "No");
    printf("  SSSE3:      %s\n", features->has_ssse3 ? "Yes" : "No");
    printf("  SSE4.1:     %s\n", features->has_sse4_1 ? "Yes" : "No");
    printf("  SSE4.2:     %s\n", features->has_sse4_2 ? "Yes" : "No");
    printf("  AVX:        %s\n", features->has_avx ? "Yes" : "No");
    printf("  AVX2:       %s\n", features->has_avx2 ? "Yes" : "No");
    printf("  AVX-512F:   %s\n", features->has_avx512f ? "Yes" : "No");
    printf("  FMA:        %s\n\n", features->has_fma ? "Yes" : "No");
    
    printf("ARM SIMD:\n");
    printf("  NEON:       %s\n", features->has_neon ? "Yes" : "No");
    printf("  SVE:        %s\n", features->has_sve ? "Yes" : "No");
    printf("  SVE2:       %s\n\n", features->has_sve2 ? "Yes" : "No");
    
    printf("LoongArch SIMD:\n");
    printf("  LSX:        %s\n", features->has_lsx ? "Yes" : "No");
    printf("  LASX:       %s\n\n", features->has_lasx ? "Yes" : "No");
    
    printf("RISC-V SIMD:\n");
    printf("  RVV:        %s\n\n", features->has_rvv ? "Yes" : "No");
    
    printf("PowerPC SIMD:\n");
    printf("  AltiVec:    %s\n", features->has_altivec ? "Yes" : "No");
    printf("  VSX:        %s\n\n", features->has_vsx ? "Yes" : "No");
    
    printf("Other:\n");
    printf("  MIPS MSA:   %s\n", features->has_msa ? "Yes" : "No");
    printf("  WASM SIMD:  %s\n\n", features->has_wasm_simd128 ? "Yes" : "No");
}

void benchmark_dot_product(void) {
    float* a = (float*)malloc(ARRAY_SIZE * sizeof(float));
    float* b = (float*)malloc(ARRAY_SIZE * sizeof(float));
    
    /* Initialize with random data */
    for (int i = 0; i < ARRAY_SIZE; i++) {
        a[i] = (float)rand() / RAND_MAX;
        b[i] = (float)rand() / RAND_MAX;
    }
    
    printf("=== Dot Product Benchmark ===\n");
    printf("Array size: %d elements\n", ARRAY_SIZE);
    printf("Iterations: %d\n", ITERATIONS);
    
    /* SIMD version */
    clock_t start = clock();
    float result_simd = 0.0f;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        result_simd = wcn_simd_dot_product_f32(a, b, ARRAY_SIZE);
    }
    clock_t end = clock();
    double time_simd = (double)(end - start) / CLOCKS_PER_SEC;
    
    /* Scalar version */
    start = clock();
    float result_scalar = 0.0f;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        result_scalar = 0.0f;
        for (int i = 0; i < ARRAY_SIZE; i++) {
            result_scalar += a[i] * b[i];
        }
    }
    end = clock();
    double time_scalar = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("SIMD result: %.6f (%.4f seconds)\n", result_simd, time_simd);
    printf("Scalar result: %.6f (%.4f seconds)\n", result_scalar, time_scalar);
    printf("Speedup: %.2fx\n\n", time_scalar / time_simd);
    
    free(a);
    free(b);
}

void benchmark_vector_add(void) {
    float* a = (float*)malloc(ARRAY_SIZE * sizeof(float));
    float* b = (float*)malloc(ARRAY_SIZE * sizeof(float));
    float* c = (float*)malloc(ARRAY_SIZE * sizeof(float));
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        a[i] = (float)rand() / RAND_MAX;
        b[i] = (float)rand() / RAND_MAX;
    }
    
    printf("=== Vector Addition Benchmark ===\n");
    printf("Array size: %d elements\n", ARRAY_SIZE);
    printf("Iterations: %d\n", ITERATIONS);
    
    /* SIMD version */
    clock_t start = clock();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        wcn_simd_add_array_f32(a, b, c, ARRAY_SIZE);
    }
    clock_t end = clock();
    double time_simd = (double)(end - start) / CLOCKS_PER_SEC;
    
    /* Scalar version */
    start = clock();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            c[i] = a[i] + b[i];
        }
    }
    end = clock();
    double time_scalar = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("SIMD time: %.4f seconds\n", time_simd);
    printf("Scalar time: %.4f seconds\n", time_scalar);
    printf("Speedup: %.2fx\n\n", time_scalar / time_simd);
    
    free(a);
    free(b);
    free(c);
}

void test_basic_operations(void) {
    printf("=== Basic Operations Test ===\n");
    
    float a[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
    float b[] = {8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
    float c[8];
    
    /* Test dot product */
    float dot = wcn_simd_dot_product_f32(a, b, 8);
    printf("Dot product: %.2f (expected: 120.00)\n", dot);
    
    /* Test vector addition */
    wcn_simd_add_array_f32(a, b, c, 8);
    printf("Vector addition: [");
    for (int i = 0; i < 8; i++) {
        printf("%.1f%s", c[i], i < 7 ? ", " : "");
    }
    printf("] (expected: all 9.0)\n");

    /* Test vector multiplication */
    wcn_simd_mul_array_f32(a, b, c, 8);
    printf("Vector multiplication: [");
    for (int i = 0; i < 8; i++) {
        printf("%.1f%s", c[i], i < 7 ? ", " : "");
    }
    printf("]\n");

    /* Test scalar multiplication */
    wcn_simd_scale_array_f32(a, 2.0f, c, 8);
    printf("Scalar multiplication (x2): [");
    for (int i = 0; i < 8; i++) {
        printf("%.1f%s", c[i], i < 7 ? ", " : "");
    }
    printf("]\n");

    /* Test max/min/sum */
    float max = wcn_simd_reduce_max_f32(a, 8);
    float min = wcn_simd_reduce_min_f32(a, 8);
    float sum = wcn_simd_reduce_sum_f32(a, 8);
    printf("Max: %.1f, Min: %.1f, Sum: %.1f\n\n", max, min, sum);
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    /* Initialize library */
    wcn_simd_init();
    
    /* Print library information */
    print_features();
    
    /* Test basic operations */
    test_basic_operations();
    
    /* Run benchmarks */
    benchmark_dot_product();
    benchmark_vector_add();
    
    printf("=== All tests completed successfully! ===\n");
    
    return 0;
}