#include <WCN_SIMD.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <malloc.h>
#include <windows.h>
static double now_seconds(void) {
  LARGE_INTEGER freq, counter;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&counter);
  return (double)counter.QuadPart / (double)freq.QuadPart;
}
static float *aligned_alloc_f32(size_t count) {
  return (float *)_aligned_malloc(count * sizeof(float), 32);
}
static void aligned_free(void *p) { _aligned_free(p); }
#else
#include <stdint.h>
static double now_seconds(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec * 1e-9;
}
static float *aligned_alloc_f32(size_t count) {
  void *ptr = NULL;
  if (posix_memalign(&ptr, 32, count * sizeof(float)) != 0)
    return NULL;
  return (float *)ptr;
}
static void aligned_free(void *p) { free(p); }
#endif

#define ARRAY_SIZE 1000000
#define ITERATIONS 100

void print_features(void) {
  const wcn_simd_features_t *features = wcn_simd_get_features();

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
  float *a = aligned_alloc_f32(ARRAY_SIZE);
  float *b = aligned_alloc_f32(ARRAY_SIZE);
  if (!a || !b) {
    fprintf(stderr, "allocation failed\n");
    return;
  }

  /* Initialize with random data */
  for (size_t i = 0; i < ARRAY_SIZE; i++) {
    a[i] = (float)rand() / RAND_MAX;
    b[i] = (float)rand() / RAND_MAX;
  }

  // ensure pages are touched
  volatile float sink = 0.0f;
  for (size_t i = 0; i < ARRAY_SIZE; i += 4096 / sizeof(float))
    sink += a[i] + b[i];
  (void)sink;

  printf("=== Dot Product Benchmark ===\n");
  printf("Array size: %zu elements\n", (size_t)ARRAY_SIZE);
  printf("Iterations: %d\n", ITERATIONS);

  /* Warm-up */
  for (int iter = 0; iter < 10; iter++)
    wcn_simd_dot_product_f32(a, b, ARRAY_SIZE);

  /* SIMD version */
  double start = now_seconds();
  float result_simd = 0.0F;
  for (int iter = 0; iter < ITERATIONS; iter++) {
    result_simd = wcn_simd_dot_product_f32(a, b, ARRAY_SIZE);
  }
  double end = now_seconds();
  double time_simd = end - start;

  volatile float sink_simd = result_simd;
  (void)sink_simd;

  /* Scalar version */
  start = now_seconds();
  float result_scalar = 0.0F;
  for (int iter = 0; iter < ITERATIONS; iter++) {
    result_scalar = 0.0F;
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
      result_scalar += a[i] * b[i];
    }
  }
  end = now_seconds();
  double time_scalar = end - start;
  volatile float sink_scalar = result_scalar;
  (void)sink_scalar;

  printf("SIMD result: %.6f (total %.6f s, avg %.6f s)", result_simd, time_simd,
         time_simd / ITERATIONS);
  printf("\nScalar result: %.6f (total %.6f s, avg %.6f s)\n", result_scalar,
         time_scalar, time_scalar / ITERATIONS);
  if (time_simd > 0.0)
    printf("Speedup: %.2fx\n\n", time_scalar / time_simd);
  else
    printf("Speedup: inf\n\n");

  aligned_free(a);
  aligned_free(b);
}

void benchmark_vector_add(void) {
  float *a = aligned_alloc_f32(ARRAY_SIZE);
  float *b = aligned_alloc_f32(ARRAY_SIZE);
  float *c = aligned_alloc_f32(ARRAY_SIZE);
  if (!a || !b || !c) {
    fprintf(stderr, "allocation failed\n");
    return;
  }

  for (size_t i = 0; i < ARRAY_SIZE; i++) {
    a[i] = (float)rand() / RAND_MAX;
    b[i] = (float)rand() / RAND_MAX;
  }

  // touch memory
  volatile float sink = 0.0f;
  for (size_t i = 0; i < ARRAY_SIZE; i += 4096 / sizeof(float))
    sink += a[i] + b[i];
  (void)sink;

  printf("=== Vector Addition Benchmark ===\n");
  printf("Array size: %zu elements\n", (size_t)ARRAY_SIZE);
  printf("Iterations: %d\n", ITERATIONS);

  /* Warm-up */
  for (int iter = 0; iter < 10; iter++)
    wcn_simd_add_array_f32(a, b, c, ARRAY_SIZE);

  /* SIMD version */
  double start = now_seconds();
  for (int iter = 0; iter < ITERATIONS; iter++) {
    wcn_simd_add_array_f32(a, b, c, ARRAY_SIZE);
  }
  double end = now_seconds();
  double time_simd = end - start;

  double checksum_simd = 0.0;
  for (size_t i = 0; i < ARRAY_SIZE; i++)
    checksum_simd += c[i];
  volatile double use_simd = checksum_simd;
  (void)use_simd;

  /* Scalar version */
  start = now_seconds();
  for (int iter = 0; iter < ITERATIONS; iter++) {
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
      c[i] = a[i] + b[i];
    }
  }
  end = now_seconds();
  double time_scalar = end - start;

  double checksum_scalar = 0.0;
  for (size_t i = 0; i < ARRAY_SIZE; i++)
    checksum_scalar += c[i];
  volatile double use_scalar = checksum_scalar;
  (void)use_scalar;

  printf("SIMD time: %.6f s (avg %.6f s), checksum: %.6f\n", time_simd,
         time_simd / ITERATIONS, checksum_simd);
  printf("Scalar time: %.6f s (avg %.6f s), checksum: %.6f\n", time_scalar,
         time_scalar / ITERATIONS, checksum_scalar);
  if (time_simd > 0.0)
    printf("Speedup: %.2fx\n\n", time_scalar / time_simd);
  else
    printf("Speedup: inf\n\n");

  aligned_free(a);
  aligned_free(b);
  aligned_free(c);
}

void test_basic_operations(void) {
  printf("=== Basic Operations Test ===\n");

  float a[] = {1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F};
  float b[] = {8.0F, 7.0F, 6.0F, 5.0F, 4.0F, 3.0F, 2.0F, 1.0F};
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
  wcn_simd_scale_array_f32(a, 2.0F, c, 8);
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