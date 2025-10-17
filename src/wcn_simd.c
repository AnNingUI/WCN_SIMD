#include "WCN_SIMD.h"
#include <stdlib.h>
#include <string.h>

#ifdef WCN_WASM_SIMD128
#include <wasm_simd128.h>
#endif

#ifndef WCN_RESTRICT
#ifdef _MSC_VER
#define WCN_RESTRICT __restrict
#else
#define WCN_RESTRICT restrict
#endif
#endif

/* Global feature detection result */
static wcn_simd_features_t g_features = {0};
static int g_initialized = 0;

/* ========== Feature Detection ========== */

#ifdef WCN_ARCH_X86

#ifdef _MSC_VER
#include <intrin.h>

static void cpuid(int info[4], int function_id) { __cpuid(info, function_id); }

static void cpuidex(int info[4], int function_id, int subfunction_id) {
  __cpuidex(info, function_id, subfunction_id);
}
#else
#include <cpuid.h>

static void cpuid(int info[4], int function_id) {
  __get_cpuid(function_id, (unsigned int *)&info[0], (unsigned int *)&info[1],
              (unsigned int *)&info[2], (unsigned int *)&info[3]);
}

static void cpuidex(int info[4], int function_id, int subfunction_id) {
  __get_cpuid_count(function_id, subfunction_id, (unsigned int *)&info[0],
                    (unsigned int *)&info[1], (unsigned int *)&info[2],
                    (unsigned int *)&info[3]);
}
#endif

static void detect_x86_features(void) {
  int info[4] = {0, 0, 0, 0};

  /* Check CPUID support - leaf 1 */
  cpuid(info, 1);
  int ecx = info[2];
  int edx = info[3];

  /* EDX features */
  g_features.has_sse2 = (edx & (1 << 26)) != 0;

  /* ECX features */
  g_features.has_sse3 = (ecx & (1 << 0)) != 0;
  g_features.has_ssse3 = (ecx & (1 << 9)) != 0;
  g_features.has_sse4_1 = (ecx & (1 << 19)) != 0;
  g_features.has_sse4_2 = (ecx & (1 << 20)) != 0;
  g_features.has_avx = (ecx & (1 << 28)) != 0;
  g_features.has_fma = (ecx & (1 << 12)) != 0;

  /* Extended features (leaf 7, subleaf 0) */
  cpuidex(info, 7, 0);
  int ebx = info[1];

  g_features.has_avx2 = (ebx & (1 << 5)) != 0;
  g_features.has_avx512f = (ebx & (1 << 16)) != 0;
  g_features.has_avx512dq = (ebx & (1 << 17)) != 0;
  g_features.has_avx512bw = (ebx & (1 << 30)) != 0;
  g_features.has_avx512vl = (ebx & (1 << 31)) != 0;
}
#endif

#ifdef WCN_ARCH_ARM
#ifdef __linux__
#include <asm/hwcap.h>
#include <sys/auxv.h>

static void detect_arm_features(void) {
  unsigned long hwcaps = getauxval(AT_HWCAP);

#ifdef HWCAP_NEON
  g_features.has_neon = (hwcaps & HWCAP_NEON) != 0;
#endif

#ifdef HWCAP_SVE
  g_features.has_sve = (hwcaps & HWCAP_SVE) != 0;
#endif

#ifdef HWCAP2_SVE2
  unsigned long hwcaps2 = getauxval(AT_HWCAP2);
  g_features.has_sve2 = (hwcaps2 & HWCAP2_SVE2) != 0;
#endif
}
#else
static void detect_arm_features(void) {
#ifdef WCN_ARM_NEON
  g_features.has_neon = 1;
#endif
#ifdef WCN_ARM_SVE
  g_features.has_sve = 1;
#endif
#ifdef WCN_ARM_SVE2
  g_features.has_sve2 = 1;
#endif
}
#endif
#endif

WCN_API_EXPORT
void wcn_simd_init(void) {
  if (g_initialized) {
    return;
  }

  memset(&g_features, 0, sizeof(g_features));

#ifdef WCN_ARCH_X86
  detect_x86_features();
#endif

#ifdef WCN_ARCH_ARM
  detect_arm_features();
#endif

#ifdef WCN_LOONGARCH_LSX
  g_features.has_lsx = 1;
#endif

#ifdef WCN_LOONGARCH_LASX
  g_features.has_lasx = 1;
#endif

#ifdef WCN_RISCV_RVV
  g_features.has_rvv = 1;
#endif

#ifdef WCN_POWERPC_ALTIVEC
  g_features.has_altivec = 1;
#endif

#ifdef WCN_POWERPC_VSX
  g_features.has_vsx = 1;
#endif

#ifdef WCN_MIPS_MSA
  g_features.has_msa = 1;
#endif

#ifdef WCN_WASM_SIMD128
  g_features.has_wasm_simd128 = 1;
#endif

#ifdef WCN_WASM_ATOMIC
  g_features.has_atomic_operations = 1;
#endif

#ifdef WCN_HAS_GCC_ATOMIC
  g_features.has_gcc_atomic = 1;
  g_features.has_atomic_operations = 1;
#endif

#ifdef WCN_HAS_MSVC_ATOMIC
  g_features.has_msvc_atomic = 1;
  g_features.has_atomic_operations = 1;
#endif

  g_initialized = 1;
}

WCN_API_EXPORT
const wcn_simd_features_t *wcn_simd_get_features(void) {
  if (!g_initialized) {
    wcn_simd_init();
  }
  return &g_features;
}

/* ========== Common Algorithms Implementation ========== */

WCN_API_EXPORT
float wcn_simd_dot_product_f32(const float *a, const float *b, size_t count) {
  float sum = 0.0F;
  size_t i = 0;

#if defined(WCN_X86_AVX2)
  wcn_v256f_t sum_vec = wcn_v256f_setzero();
  for (; i + 8 <= count; i += 8) {
    wcn_v256f_t va = wcn_v256f_load(a + i);
    wcn_v256f_t vb = wcn_v256f_load(b + i);
    wcn_v256f_t prod = wcn_v256f_mul(va, vb);
    sum_vec = wcn_v256f_add(sum_vec, prod);
  }
  /* Horizontal sum of 8 elements */
  float temp[8];
  wcn_v256f_store(temp, sum_vec);
  for (int j = 0; j < 8; j++) {
    sum += temp[j];
  }

#elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON) ||                        \
    defined(WCN_WASM_SIMD128)
  wcn_v128f_t sum_vec = wcn_v128f_setzero();
  for (; i + 4 <= count; i += 4) {
    const wcn_v128f_t va = wcn_v128f_load(a + i);
    const wcn_v128f_t vb = wcn_v128f_load(b + i);
    wcn_v128f_t prod = wcn_v128f_mul(va, vb);
    sum_vec = wcn_v128f_add(sum_vec, prod);
  }
#if defined(WCN_X86_SSE2)
  sum = wcn_v128f_reduce_add(sum_vec);
#elif defined(WCN_ARM_NEON)
  sum = wcn_v128f_reduce_add(sum_vec);
#else
  float temp[4];
  wcn_v128f_store(temp, sum_vec);
  for (int j = 0; j < 4; j++) {
    sum += temp[j];
  }
#endif

#elif defined(WCN_RISCV_RVV)
  size_t vl = wcn_rvv_vsetvl_f32(count);
  wcn_vscalable_f32_t sum_vec = wcn_vscalable_f32_set1(0.0f, vl);
  for (; i + vl <= count; i += vl) {
    vl = wcn_rvv_vsetvl_f32(count - i);
    wcn_vscalable_f32_t va = wcn_vscalable_f32_load(a + i, vl);
    wcn_vscalable_f32_t vb = wcn_vscalable_f32_load(b + i, vl);
    wcn_vscalable_f32_t prod = wcn_vscalable_f32_mul(va, vb, vl);
    sum_vec = wcn_vscalable_f32_add(sum_vec, prod, vl);
  }
  /* Manual reduction for RVV */
  float temp[256];
  wcn_vscalable_f32_store(temp, sum_vec, vl);
  for (size_t j = 0; j < vl; j++) {
    sum += temp[j];
  }
#endif

  /* Scalar tail */
  for (; i < count; i++) {
    sum += a[i] * b[i];
  }

  return sum;
}

WCN_API_EXPORT
void wcn_simd_add_array_f32(const float *WCN_RESTRICT a,
                            const float *WCN_RESTRICT b, float *WCN_RESTRICT c,
                            size_t count) {
  size_t i = 0;
  const float *pa = a;
  const float *pb = b;
  float *pc = c;

#if defined(__GNUC__)
  pa = (const float *)__builtin_assume_aligned(pa, 16);
  pb = (const float *)__builtin_assume_aligned(pb, 16);
  pc = (float *)__builtin_assume_aligned(pc, 16);
#endif

#if defined(WCN_X86_AVX2) && defined(__GNUC__) && defined(__x86_64__)
  /* AVX2 path implemented with GNU inline assembly for tight loop */
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 32);
    __builtin_prefetch(pb + 32);
    __asm__ volatile("vmovups (%[pa]), %%ymm0\n\t"
                     "vmovups (%[pb]), %%ymm1\n\t"
                     "vaddps %%ymm1, %%ymm0, %%ymm0\n\t"
                     "vmovups %%ymm0, (%[pc])\n\t"
                     : /* no outputs */
                     : [pa] "r"(pa), [pb] "r"(pb), [pc] "r"(pc)
                     : "ymm0", "ymm1", "memory");
    pa += 8;
    pb += 8;
    pc += 8;
  }
  /* avoid upper-latency on older CPUs when mixing SSE and AVX */
  __asm__ volatile("vzeroupper" ::: "memory");

#elif defined(WCN_X86_AVX2)
  /* Fallback AVX2 wrapper if not using GNU inline asm */
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 32);
    __builtin_prefetch(pb + 32);
    wcn_v256f_t va = wcn_v256f_load(pa);
    wcn_v256f_t vb = wcn_v256f_load(pb);
    wcn_v256f_t vc = wcn_v256f_add(va, vb);
    wcn_v256f_store(pc, vc);
    pa += 8;
    pb += 8;
    pc += 8;
  }

#endif

  /* Other SIMD/backends unchanged */
#if defined(WCN_WASM_SIMD128)
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 256);
    __builtin_prefetch(pb + 256);
    v128_t a0 = wasm_v128_load(pa);
    v128_t b0 = wasm_v128_load(pb);
    wasm_v128_store(pc, wasm_f32x4_add(a0, b0));
    v128_t a1 = wasm_v128_load(pa + 4);
    v128_t b1 = wasm_v128_load(pb + 4);
    wasm_v128_store(pc + 4, wasm_f32x4_add(a1, b1));
    v128_t a2 = wasm_v128_load(pa + 8);
    v128_t b2 = wasm_v128_load(pb + 8);
    wasm_v128_store(pc + 8, wasm_f32x4_add(a2, b2));
    v128_t a3 = wasm_v128_load(pa + 12);
    v128_t b3 = wasm_v128_load(pb + 12);
    wasm_v128_store(pc + 12, wasm_f32x4_add(a3, b3));
    pa += 16;
    pb += 16;
    pc += 16;
  }
  for (; i + 8 <= count; i += 8) {
    v128_t a0 = wasm_v128_load(pa);
    v128_t b0 = wasm_v128_load(pb);
    wasm_v128_store(pc, wasm_f32x4_add(a0, b0));
    v128_t a1 = wasm_v128_load(pa + 4);
    v128_t b1 = wasm_v128_load(pb + 4);
    wasm_v128_store(pc + 4, wasm_f32x4_add(a1, b1));
    pa += 8;
    pb += 8;
    pc += 8;
  }
  for (; i + 4 <= count; i += 4) {
    v128_t a0 = wasm_v128_load(pa);
    v128_t b0 = wasm_v128_load(pb);
    wasm_v128_store(pc, wasm_f32x4_add(a0, b0));
    pa += 4;
    pb += 4;
    pc += 4;
  }
#elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON)
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 64);
    __builtin_prefetch(pb + 64);
    wcn_v128f_t va0 = wcn_v128f_load(pa);
    wcn_v128f_t vb0 = wcn_v128f_load(pb);
    wcn_v128f_t vc0 = wcn_v128f_add(va0, vb0);
    wcn_v128f_store(pc, vc0);
    wcn_v128f_t va1 = wcn_v128f_load(pa + 4);
    wcn_v128f_t vb1 = wcn_v128f_load(pb + 4);
    wcn_v128f_t vc1 = wcn_v128f_add(va1, vb1);
    wcn_v128f_store(pc + 4, vc1);
    pa += 8;
    pb += 8;
    pc += 8;
  }
  for (; i + 4 <= count; i += 4) {
    wcn_v128f_t va = wcn_v128f_load(pa);
    wcn_v128f_t vb = wcn_v128f_load(pb);
    wcn_v128f_t vc = wcn_v128f_add(va, vb);
    wcn_v128f_store(pc, vc);
    pa += 4;
    pb += 4;
    pc += 4;
  }
#elif defined(WCN_RISCV_RVV)
  size_t vl = wcn_rvv_vsetvl_f32(count);
  for (; i + vl <= count; i += vl) {
    vl = wcn_rvv_vsetvl_f32(count - i);
    wcn_vscalable_f32_t va = wcn_vscalable_f32_load(pa, vl);
    wcn_vscalable_f32_t vb = wcn_vscalable_f32_load(pb, vl);
    wcn_vscalable_f32_t vc = wcn_vscalable_f32_add(va, vb, vl);
    wcn_vscalable_f32_store(pc, vc, vl);
    pa += vl;
    pb += vl;
    pc += vl;
  }
#endif

  /* Scalar tail */
  for (; i < count; i++) {
    *pc++ = *pa++ + *pb++;
  }
}

WCN_API_EXPORT
void wcn_simd_mul_array_f32(const float *WCN_RESTRICT a,
                            const float *WCN_RESTRICT b, float *WCN_RESTRICT c,
                            size_t count) {
  size_t i = 0;
  const float *pa = a;
  const float *pb = b;
  float *pc = c;

#if defined(__GNUC__)
  pa = (const float *)__builtin_assume_aligned(pa, 16);
  pb = (const float *)__builtin_assume_aligned(pb, 16);
  pc = (float *)__builtin_assume_aligned(pc, 16);
#endif

#if defined(WCN_X86_AVX2)
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 32);
    __builtin_prefetch(pb + 32);
    wcn_v256f_t va = wcn_v256f_load(pa);
    wcn_v256f_t vb = wcn_v256f_load(pb);
    wcn_v256f_t vc = wcn_v256f_mul(va, vb);
    wcn_v256f_store(pc, vc);
    pa += 8;
    pb += 8;
    pc += 8;
  }

#elif defined(WCN_WASM_SIMD128)
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 256);
    __builtin_prefetch(pb + 256);
    v128_t a0 = wasm_v128_load(pa);
    v128_t b0 = wasm_v128_load(pb);
    wasm_v128_store(pc, wasm_f32x4_mul(a0, b0));
    v128_t a1 = wasm_v128_load(pa + 4);
    v128_t b1 = wasm_v128_load(pb + 4);
    wasm_v128_store(pc + 4, wasm_f32x4_mul(a1, b1));
    v128_t a2 = wasm_v128_load(pa + 8);
    v128_t b2 = wasm_v128_load(pb + 8);
    wasm_v128_store(pc + 8, wasm_f32x4_mul(a2, b2));
    v128_t a3 = wasm_v128_load(pa + 12);
    v128_t b3 = wasm_v128_load(pb + 12);
    wasm_v128_store(pc + 12, wasm_f32x4_mul(a3, b3));
    pa += 16;
    pb += 16;
    pc += 16;
  }
  for (; i + 8 <= count; i += 8) {
    v128_t a0 = wasm_v128_load(pa);
    v128_t b0 = wasm_v128_load(pb);
    wasm_v128_store(pc, wasm_f32x4_mul(a0, b0));
    v128_t a1 = wasm_v128_load(pa + 4);
    v128_t b1 = wasm_v128_load(pb + 4);
    wasm_v128_store(pc + 4, wasm_f32x4_mul(a1, b1));
    pa += 8;
    pb += 8;
    pc += 8;
  }
  for (; i + 4 <= count; i += 4) {
    v128_t a0 = wasm_v128_load(pa);
    v128_t b0 = wasm_v128_load(pb);
    wasm_v128_store(pc, wasm_f32x4_mul(a0, b0));
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON)
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 64);
    __builtin_prefetch(pb + 64);
    wcn_v128f_t va0 = wcn_v128f_load(pa);
    wcn_v128f_t vb0 = wcn_v128f_load(pb);
    wcn_v128f_t vc0 = wcn_v128f_mul(va0, vb0);
    wcn_v128f_store(pc, vc0);
    wcn_v128f_t va1 = wcn_v128f_load(pa + 4);
    wcn_v128f_t vb1 = wcn_v128f_load(pb + 4);
    wcn_v128f_t vc1 = wcn_v128f_mul(va1, vb1);
    wcn_v128f_store(pc + 4, vc1);
    pa += 8;
    pb += 8;
    pc += 8;
  }
  for (; i + 4 <= count; i += 4) {
    wcn_v128f_t va = wcn_v128f_load(pa);
    wcn_v128f_t vb = wcn_v128f_load(pb);
    wcn_v128f_t vc = wcn_v128f_mul(va, vb);
    wcn_v128f_store(pc, vc);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_RISCV_RVV)
  size_t vl = wcn_rvv_vsetvl_f32(count);
  for (; i + vl <= count; i += vl) {
    vl = wcn_rvv_vsetvl_f32(count - i);
    wcn_vscalable_f32_t va = wcn_vscalable_f32_load(pa, vl);
    wcn_vscalable_f32_t vb = wcn_vscalable_f32_load(pb, vl);
    wcn_vscalable_f32_t vc = wcn_vscalable_f32_mul(va, vb, vl);
    wcn_vscalable_f32_store(pc, vc, vl);
    pa += vl;
    pb += vl;
    pc += vl;
  }
#endif

  for (; i < count; i++) {
    *pc++ = *pa++ * *pb++;
  }
}

WCN_API_EXPORT
void wcn_simd_scale_array_f32(const float *WCN_RESTRICT a, float scalar,
                              float *WCN_RESTRICT b, size_t count) {
  size_t i = 0;
  const float *pa = a;
  float *pb = b;

#if defined(__GNUC__)
  pa = (const float *)__builtin_assume_aligned(pa, 16);
  pb = (float *)__builtin_assume_aligned(pb, 16);
#endif

#if defined(WCN_X86_AVX2)
  wcn_v256f_t vs = wcn_v256f_set1(scalar);
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 32);
    wcn_v256f_t va = wcn_v256f_load(pa);
    wcn_v256f_t vb = wcn_v256f_mul(va, vs);
    wcn_v256f_store(pb, vb);
    pa += 8;
    pb += 8;
  }

#elif defined(WCN_WASM_SIMD128)
  v128_t vsplat = wasm_f32x4_splat(scalar);
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 256);
    v128_t a0 = wasm_v128_load(pa);
    wasm_v128_store(pb, wasm_f32x4_mul(a0, vsplat));
    v128_t a1 = wasm_v128_load(pa + 4);
    wasm_v128_store(pb + 4, wasm_f32x4_mul(a1, vsplat));
    v128_t a2 = wasm_v128_load(pa + 8);
    wasm_v128_store(pb + 8, wasm_f32x4_mul(a2, vsplat));
    v128_t a3 = wasm_v128_load(pa + 12);
    wasm_v128_store(pb + 12, wasm_f32x4_mul(a3, vsplat));
    pa += 16;
    pb += 16;
  }
  for (; i + 8 <= count; i += 8) {
    v128_t a0 = wasm_v128_load(pa);
    wasm_v128_store(pb, wasm_f32x4_mul(a0, vsplat));
    v128_t a1 = wasm_v128_load(pa + 4);
    wasm_v128_store(pb + 4, wasm_f32x4_mul(a1, vsplat));
    pa += 8;
    pb += 8;
  }
  for (; i + 4 <= count; i += 4) {
    v128_t a0 = wasm_v128_load(pa);
    wasm_v128_store(pb, wasm_f32x4_mul(a0, vsplat));
    pa += 4;
    pb += 4;
  }

#elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON)
  wcn_v128f_t vs = wcn_v128f_set1(scalar);
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 64);
    wcn_v128f_t va0 = wcn_v128f_load(pa);
    wcn_v128f_t vb0 = wcn_v128f_mul(va0, vs);
    wcn_v128f_store(pb, vb0);
    wcn_v128f_t va1 = wcn_v128f_load(pa + 4);
    wcn_v128f_t vb1 = wcn_v128f_mul(va1, vs);
    wcn_v128f_store(pb + 4, vb1);
    pa += 8;
    pb += 8;
  }
  for (; i + 4 <= count; i += 4) {
    wcn_v128f_t va = wcn_v128f_load(pa);
    wcn_v128f_t vb = wcn_v128f_mul(va, vs);
    wcn_v128f_store(pb, vb);
    pa += 4;
    pb += 4;
  }

#elif defined(WCN_RISCV_RVV)
  size_t vl = wcn_rvv_vsetvl_f32(count);
  wcn_vscalable_f32_t vs = wcn_vscalable_f32_set1(scalar, vl);
  for (; i + vl <= count; i += vl) {
    vl = wcn_rvv_vsetvl_f32(count - i);
    wcn_vscalable_f32_t va = wcn_vscalable_f32_load(pa, vl);
    wcn_vscalable_f32_t vb = wcn_vscalable_f32_mul(va, vs, vl);
    wcn_vscalable_f32_store(pb, vb, vl);
    pa += vl;
    pb += vl;
  }
#endif

  for (; i < count; i++) {
    *pb++ = *pa++ * scalar;
  }
}
// Eg: -mrelaxed-simd
#if defined(WCN_WASM_SIMD128)
#if defined(__wasm_relaxed_simd__)
static inline v128_t wasm_f32x4_qfma(v128_t a, v128_t b, v128_t c) {
  return __builtin_wasm_relaxed_madd_f32x4(a, b, c);
}
#else
static inline v128_t wasm_f32x4_qfma(v128_t a, v128_t b, v128_t c) {
  return wasm_f32x4_add(wasm_f32x4_mul(a, b), c);
}
#endif
#endif

WCN_API_EXPORT
void wcn_simd_fmadd_array_f32(const float *WCN_RESTRICT a,
                              const float *WCN_RESTRICT b,
                              float *WCN_RESTRICT c, size_t count) {
  size_t i = 0;

  // Ensure alignment for SIMD operations
#if defined(__GNUC__)
  const float *pa = (const float *)__builtin_assume_aligned(a, 16);
  const float *pb = (const float *)__builtin_assume_aligned(b, 16);
  float *pc = (float *)__builtin_assume_aligned(c, 16);
#else
  const float *pa = a;
  const float *pb = b;
  float *pc = c;
#endif

  // AVX2 implementation with loop unrolling and prefetching
#if defined(WCN_X86_AVX2)
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 64); // Prefetch next block of 'a'
    __builtin_prefetch(pb + 64); // Prefetch next block of 'b'
    __builtin_prefetch(pc + 64); // Prefetch next block of 'c'

    wcn_v256f_t va0 = wcn_v256f_load(pa);
    wcn_v256f_t vb0 = wcn_v256f_load(pb);
    wcn_v256f_t vc0 = wcn_v256f_load(pc);
    vc0 = wcn_v256f_fmadd(va0, vb0, vc0);
    wcn_v256f_store(pc, vc0);

    wcn_v256f_t va1 = wcn_v256f_load(pa + 8);
    wcn_v256f_t vb1 = wcn_v256f_load(pb + 8);
    wcn_v256f_t vc1 = wcn_v256f_load(pc + 8);
    vc1 = wcn_v256f_fmadd(va1, vb1, vc1);
    wcn_v256f_store(pc + 8, vc1);

    pa += 16;
    pb += 16;
    pc += 16;
  }

  for (; i + 8 <= count; i += 8) {
    wcn_v256f_t va = wcn_v256f_load(pa);
    wcn_v256f_t vb = wcn_v256f_load(pb);
    wcn_v256f_t vc = wcn_v256f_load(pc);
    vc = wcn_v256f_fmadd(va, vb, vc);
    wcn_v256f_store(pc, vc);

    pa += 8;
    pb += 8;
    pc += 8;
  }
#endif

  // WebAssembly SIMD128 implementation with loop unrolling
#if defined(WCN_WASM_SIMD128)
  for (; i + 64 <= count; i += 64) {
    __builtin_prefetch(pa + 128);
    __builtin_prefetch(pb + 128);
    __builtin_prefetch(pc + 128);

    // Process 4 vectors (64 bytes) at a time
    v128_t v0_0 = wasm_v128_load(pa);
    v128_t v1_0 = wasm_v128_load(pb);
    v128_t v2_0 = wasm_v128_load(pc);

    v128_t v0_1 = wasm_v128_load(pa + 16);
    v128_t v1_1 = wasm_v128_load(pb + 16);
    v128_t v2_1 = wasm_v128_load(pc + 16);

    v128_t v0_2 = wasm_v128_load(pa + 32);
    v128_t v1_2 = wasm_v128_load(pb + 32);
    v128_t v2_2 = wasm_v128_load(pc + 32);

    v128_t v0_3 = wasm_v128_load(pa + 48);
    v128_t v1_3 = wasm_v128_load(pb + 48);
    v128_t v2_3 = wasm_v128_load(pc + 48);

    wasm_v128_store(pc, wasm_f32x4_qfma(v0_0, v1_0, v2_0));
    wasm_v128_store(pc + 16, wasm_f32x4_qfma(v0_1, v1_1, v2_1));
    wasm_v128_store(pc + 32, wasm_f32x4_qfma(v0_2, v1_2, v2_2));
    wasm_v128_store(pc + 48, wasm_f32x4_qfma(v0_3, v1_3, v2_3));

    pa += 64;
    pb += 64;
    pc += 64;
  }

  // Process remaining 16-byte chunks
  for (; i + 16 <= count; i += 16) {
    v128_t v0_0 = wasm_v128_load(pa);
    v128_t v1_0 = wasm_v128_load(pb);
    v128_t v2_0 = wasm_v128_load(pc);

    v128_t v0_1 = wasm_v128_load(pa + 4);
    v128_t v1_1 = wasm_v128_load(pb + 4);
    v128_t v2_1 = wasm_v128_load(pc + 4);

    v128_t v0_2 = wasm_v128_load(pa + 8);
    v128_t v1_2 = wasm_v128_load(pb + 8);
    v128_t v2_2 = wasm_v128_load(pc + 8);

    v128_t v0_3 = wasm_v128_load(pa + 12);
    v128_t v1_3 = wasm_v128_load(pb + 12);
    v128_t v2_3 = wasm_v128_load(pc + 12);

    wasm_v128_store(pc, wasm_f32x4_qfma(v0_0, v1_0, v2_0));
    wasm_v128_store(pc + 4, wasm_f32x4_qfma(v0_1, v1_1, v2_1));
    wasm_v128_store(pc + 8, wasm_f32x4_qfma(v0_2, v1_2, v2_2));
    wasm_v128_store(pc + 12, wasm_f32x4_qfma(v0_3, v1_3, v2_3));

    pa += 16;
    pb += 16;
    pc += 16;
  }

  // Process remaining 4-byte chunks
  for (; i + 4 <= count; i += 4) {
    v128_t v0 = wasm_v128_load(pa);
    v128_t v1 = wasm_v128_load(pb);
    v128_t v2 = wasm_v128_load(pc);

    wasm_v128_store(pc, wasm_f32x4_add(wasm_f32x4_mul(v0, v1), v2));

    pa += 4;
    pb += 4;
    pc += 4;
  }
#endif

  // SSE2 or ARM NEON implementation with loop unrolling
#if defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON)
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 64);
    __builtin_prefetch(pb + 64);
    __builtin_prefetch(pc + 64);

    wcn_v128f_t va0 = wcn_v128f_load(pa);
    wcn_v128f_t vb0 = wcn_v128f_load(pb);
    wcn_v128f_t vc0 = wcn_v128f_load(pc);
    vc0 = wcn_v128f_fmadd(va0, vb0, vc0);
    wcn_v128f_store(pc, vc0);

    wcn_v128f_t va1 = wcn_v128f_load(pa + 4);
    wcn_v128f_t vb1 = wcn_v128f_load(pb + 4);
    wcn_v128f_t vc1 = wcn_v128f_load(pc + 4);
    vc1 = wcn_v128f_fmadd(va1, vb1, vc1);
    wcn_v128f_store(pc + 4, vc1);

    wcn_v128f_t va2 = wcn_v128f_load(pa + 8);
    wcn_v128f_t vb2 = wcn_v128f_load(pb + 8);
    wcn_v128f_t vc2 = wcn_v128f_load(pc + 8);
    vc2 = wcn_v128f_fmadd(va2, vb2, vc2);
    wcn_v128f_store(pc + 8, vc2);

    wcn_v128f_t va3 = wcn_v128f_load(pa + 12);
    wcn_v128f_t vb3 = wcn_v128f_load(pb + 12);
    wcn_v128f_t vc3 = wcn_v128f_load(pc + 12);
    vc3 = wcn_v128f_fmadd(va3, vb3, vc3);
    wcn_v128f_store(pc + 12, vc3);

    pa += 16;
    pb += 16;
    pc += 16;
  }

  for (; i + 8 <= count; i += 8) {
    wcn_v128f_t va0 = wcn_v128f_load(pa);
    wcn_v128f_t vb0 = wcn_v128f_load(pb);
    wcn_v128f_t vc0 = wcn_v128f_load(pc);
    vc0 = wcn_v128f_fmadd(va0, vb0, vc0);
    wcn_v128f_store(pc, vc0);

    wcn_v128f_t va1 = wcn_v128f_load(pa + 4);
    wcn_v128f_t vb1 = wcn_v128f_load(pb + 4);
    wcn_v128f_t vc1 = wcn_v128f_load(pc + 4);
    vc1 = wcn_v128f_fmadd(va1, vb1, vc1);
    wcn_v128f_store(pc + 4, vc1);

    pa += 8;
    pb += 8;
    pc += 8;
  }

  for (; i + 4 <= count; i += 4) {
    wcn_v128f_t va = wcn_v128f_load(pa);
    wcn_v128f_t vb = wcn_v128f_load(pb);
    wcn_v128f_t vc = wcn_v128f_load(pc);
    vc = wcn_v128f_fmadd(va, vb, vc);
    wcn_v128f_store(pc, vc);

    pa += 4;
    pb += 4;
    pc += 4;
  }
#endif

  // RISC-V Vector Extension implementation
#if defined(WCN_RISCV_RVV)
  size_t vl = wcn_rvv_vsetvl_f32(count);
  for (; i + vl <= count; i += vl) {
    vl = wcn_rvv_vsetvl_f32(count - i);
    wcn_vscalable_f32_t va = wcn_vscalable_f32_load(pa, vl);
    wcn_vscalable_f32_t vb = wcn_vscalable_f32_load(pb, vl);
    wcn_vscalable_f32_t vc = wcn_vscalable_f32_load(pc, vl);
    vc = wcn_vscalable_f32_fmadd(va, vb, vc, vl);
    wcn_vscalable_f32_store(pc, vc, vl);

    pa += vl;
    pb += vl;
    pc += vl;
  }
#endif

  // Scalar tail for remaining elements
  for (; i < count; i++) {
    *pc = (*pa) * (*pb) + (*pc);
    pa++;
    pb++;
    pc++;
  }
}

WCN_API_EXPORT
float wcn_simd_reduce_max_f32(const float *data, size_t count) {
  if (count == 0)
    return 0.0f;

  float max_val = data[0];
  size_t i = 1;

#if defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON) || defined(WCN_WASM_SIMD128)
  if (count >= 4) {
    wcn_v128f_t max_vec = wcn_v128f_load(data);
    for (; i + 4 <= count; i += 4) {
      wcn_v128f_t v = wcn_v128f_load(data + i);
      max_vec = wcn_v128f_max(max_vec, v);
    }
#if defined(WCN_X86_SSE2)
    max_val = wcn_v128f_reduce_max(max_vec);
#elif defined(WCN_ARM_NEON)
    max_val = wcn_v128f_reduce_max(max_vec);
#else
    float temp[4];
    wcn_v128f_store(temp, max_vec);
    max_val = temp[0];
    for (int j = 1; j < 4; j++) {
      if (temp[j] > max_val)
        max_val = temp[j];
    }
#endif
  }
#endif

  /* Scalar tail */
  for (; i < count; i++) {
    if (data[i] > max_val) {
      max_val = data[i];
    }
  }

  return max_val;
}

WCN_API_EXPORT
float wcn_simd_reduce_min_f32(const float *data, size_t count) {
  if (count == 0)
    return 0.0f;

  float min_val = data[0];
  size_t i = 1;

#if defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON) || defined(WCN_WASM_SIMD128)
  if (count >= 4) {
    wcn_v128f_t min_vec = wcn_v128f_load(data);
    for (; i + 4 <= count; i += 4) {
      wcn_v128f_t v = wcn_v128f_load(data + i);
      min_vec = wcn_v128f_min(min_vec, v);
    }
#if defined(WCN_X86_SSE2)
    min_val = wcn_v128f_reduce_min(min_vec);
#elif defined(WCN_ARM_NEON)
    min_val = wcn_v128f_reduce_min(min_vec);
#else
    float temp[4];
    wcn_v128f_store(temp, min_vec);
    min_val = temp[0];
    for (int j = 1; j < 4; j++) {
      if (temp[j] < min_val)
        min_val = temp[j];
    }
#endif
  }
#endif

  /* Scalar tail */
  for (; i < count; i++) {
    if (data[i] < min_val) {
      min_val = data[i];
    }
  }

  return min_val;
}

WCN_API_EXPORT
float wcn_simd_reduce_sum_f32(const float *data, size_t count) {
  float sum = 0.0f;
  size_t i = 0;

#if defined(WCN_X86_AVX2)
  wcn_v256f_t sum_vec = wcn_v256f_setzero();
  for (; i + 8 <= count; i += 8) {
    wcn_v256f_t v = wcn_v256f_load(data + i);
    sum_vec = wcn_v256f_add(sum_vec, v);
  }
  float temp[8];
  wcn_v256f_store(temp, sum_vec);
  for (int j = 0; j < 8; j++) {
    sum += temp[j];
  }

#elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON) ||                        \
    defined(WCN_WASM_SIMD128)
  wcn_v128f_t sum_vec = wcn_v128f_setzero();
  for (; i + 4 <= count; i += 4) {
    wcn_v128f_t v = wcn_v128f_load(data + i);
    sum_vec = wcn_v128f_add(sum_vec, v);
  }
#if defined(WCN_X86_SSE2)
  sum = wcn_v128f_reduce_add(sum_vec);
#elif defined(WCN_ARM_NEON)
  sum = wcn_v128f_reduce_add(sum_vec);
#else
  float temp[4];
  wcn_v128f_store(temp, sum_vec);
  for (int j = 0; j < 4; j++) {
    sum += temp[j];
  }
#endif
#endif

  /* Scalar tail */
  for (; i < count; i++) {
    sum += data[i];
  }

  return sum;
}

WCN_API_EXPORT
void wcn_simd_memcpy_aligned(void *dest, const void *src, size_t bytes) {
  memcpy(dest, src, bytes);
}

WCN_API_EXPORT
void wcn_simd_memset_aligned(void *dest, int value, size_t bytes) {
  memset(dest, value, bytes);
}

WCN_API_EXPORT
void *wcn_alloc(size_t bytes) { return malloc(bytes); }

WCN_API_EXPORT
void wcn_free(void *ptr) { free(ptr); }
