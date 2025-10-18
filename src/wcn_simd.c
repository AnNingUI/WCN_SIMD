#include "WCN_SIMD.h"
#include <math.h>
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
float wcn_simd_dot_product_f32(const float *WCN_RESTRICT a,
                               const float *WCN_RESTRICT b, size_t count) {
  size_t i = 0;
  float sum = 0.0f;

#if defined(__AVX2__) && defined(__FMA__)
  // --- AVX2 + FMA path with alignment prologue + unroll(2) for ILP ---
  const uintptr_t a_ptr = (uintptr_t)a;
  const size_t align_bytes = 32;
  const size_t mis = a_ptr & (align_bytes - 1);
  // process prologue until 'a' becomes 32-byte aligned
  if (mis != 0) {
    size_t to_align = (align_bytes - mis) / sizeof(float);
    if (to_align > count)
      to_align = count;
    for (size_t k = 0; k < to_align; ++k) {
      sum = fmaf(a[k], b[k], sum);
    }
    i += to_align;
  }

  // main vector loop: use two accumulators to increase ILP
  __m256 sum0 = _mm256_setzero_ps();
  __m256 sum1 = _mm256_setzero_ps();

  // prefetch hints (optional)
  for (; i + 16 <= count; i += 16) {
    // prefetch next cache lines (helpful for large arrays)
    _mm_prefetch((const char *)(a + i + 16), _MM_HINT_T0);
    _mm_prefetch((const char *)(b + i + 16), _MM_HINT_T0);

    // load aligned (we ensured 'a' is aligned; b may or may not be aligned - if
    // you can align b too, use load)
    __m256 va0 = _mm256_load_ps(a + i); // aligned load
    __m256 vb0 =
        _mm256_loadu_ps(b + i); // use loadu for b unless aligned guaranteed
    __m256 va1 = _mm256_load_ps(a + i + 8);
    __m256 vb1 = _mm256_loadu_ps(b + i + 8);

    // fused multiply-add into separate accumulators
    sum0 = _mm256_fmadd_ps(va0, vb0, sum0);
    sum1 = _mm256_fmadd_ps(va1, vb1, sum1);
  }

  // reduce vector accumulators
  __m256 sumv = _mm256_add_ps(sum0, sum1);
  __m128 low = _mm256_castps256_ps128(sumv);
  __m128 high = _mm256_extractf128_ps(sumv, 1);
  __m128 s = _mm_add_ps(low, high);
  s = _mm_add_ps(s, _mm_movehl_ps(s, s));
  __m128 shuf = _mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 3, 0, 1));
  s = _mm_add_ps(s, shuf);
  sum = _mm_cvtss_f32(s);

  // tail vector (if remaining >=8 but <16), process with 8-wide
  for (; i + 8 <= count; i += 8) {
    __m256 va = _mm256_load_ps(a + i); // aligned
    __m256 vb = _mm256_loadu_ps(b + i);
    __m256 t = _mm256_fmadd_ps(va, vb, _mm256_setzero_ps());
    // horizontal add
    __m128 lo = _mm256_castps256_ps128(t);
    __m128 hi = _mm256_extractf128_ps(t, 1);
    __m128 s2 = _mm_add_ps(lo, hi);
    s2 = _mm_add_ps(s2, _mm_movehl_ps(s2, s2));
    __m128 sh2 = _mm_shuffle_ps(s2, s2, _MM_SHUFFLE(2, 3, 0, 1));
    s2 = _mm_add_ps(s2, sh2);
    sum += _mm_cvtss_f32(s2);
  }

  // scalar tail
  for (; i < count; ++i) {
    sum = fmaf(a[i], b[i], sum);
  }

  return sum;

#elif defined(__AVX2__)
  // AVX2 without FMA: similar strategy but use mul+add
  const uintptr_t a_ptr = (uintptr_t)a;
  const size_t align_bytes = 32;
  const size_t mis = a_ptr & (align_bytes - 1);
  if (mis != 0) {
    size_t to_align = (align_bytes - mis) / sizeof(float);
    if (to_align > count)
      to_align = count;
    for (size_t k = 0; k < to_align; ++k)
      sum += a[k] * b[k];
    i += to_align;
  }

  __m256 sum0 = _mm256_setzero_ps();
  __m256 sum1 = _mm256_setzero_ps();

  for (; i + 16 <= count; i += 16) {
    _mm_prefetch((const char *)(a + i + 16), _MM_HINT_T0);
    _mm_prefetch((const char *)(b + i + 16), _MM_HINT_T0);

    __m256 va0 = _mm256_load_ps(a + i);
    __m256 vb0 = _mm256_loadu_ps(b + i);
    __m256 va1 = _mm256_load_ps(a + i + 8);
    __m256 vb1 = _mm256_loadu_ps(b + i + 8);

    sum0 = _mm256_add_ps(sum0, _mm256_mul_ps(va0, vb0));
    sum1 = _mm256_add_ps(sum1, _mm256_mul_ps(va1, vb1));
  }

  __m256 sumv = _mm256_add_ps(sum0, sum1);
  __m128 low = _mm256_castps256_ps128(sumv);
  __m128 high = _mm256_extractf128_ps(sumv, 1);
  __m128 s = _mm_add_ps(low, high);
  s = _mm_add_ps(s, _mm_movehl_ps(s, s));
  __m128 shuf = _mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 3, 0, 1));
  s = _mm_add_ps(s, shuf);
  sum = _mm_cvtss_f32(s);

  for (; i + 8 <= count; i += 8) {
    __m256 va = _mm256_load_ps(a + i);
    __m256 vb = _mm256_loadu_ps(b + i);
    __m256 t = _mm256_mul_ps(va, vb);
    __m128 lo = _mm256_castps256_ps128(t);
    __m128 hi = _mm256_extractf128_ps(t, 1);
    __m128 s2 = _mm_add_ps(lo, hi);
    s2 = _mm_add_ps(s2, _mm_movehl_ps(s2, s2));
    __m128 sh2 = _mm_shuffle_ps(s2, s2, _MM_SHUFFLE(2, 3, 0, 1));
    s2 = _mm_add_ps(s2, sh2);
    sum += _mm_cvtss_f32(s2);
  }

  for (; i < count; ++i)
    sum += a[i] * b[i];
  return sum;

#elif defined(__SSE2__)
  // SSE2: align to 16 bytes and unroll
  const uintptr_t a_ptr = (uintptr_t)a;
  const size_t align_bytes = 16;
  const size_t mis = a_ptr & (align_bytes - 1);
  if (mis != 0) {
    size_t to_align = (align_bytes - mis) / sizeof(float);
    if (to_align > count)
      to_align = count;
    for (size_t k = 0; k < to_align; ++k)
      sum += a[k] * b[k];
    i += to_align;
  }

  __m128 sumv = _mm_setzero_ps();
  for (; i + 4 <= count; i += 4) {
    __m128 va = _mm_load_ps(a + i); // aligned
    __m128 vb = _mm_loadu_ps(b + i);
#if defined(__FMA__)
    sumv = _mm_fmadd_ps(va, vb, sumv);
#else
    sumv = _mm_add_ps(sumv, _mm_mul_ps(va, vb));
#endif
  }

  __m128 s = _mm_add_ps(sumv, _mm_movehl_ps(sumv, sumv));
  __m128 sh = _mm_shuffle_ps(s, s, _MM_SHUFFLE(2, 3, 0, 1));
  s = _mm_add_ps(s, sh);
  sum = _mm_cvtss_f32(s);

  for (; i < count; ++i)
    sum += a[i] * b[i];
  return sum;

#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
  // NEON: we can do similar unroll; NEON loads don't require alignment on many
  // platforms
  float32x4_t acc0 = vdupq_n_f32(0.0f);
  for (; i + 8 <= count; i += 8) {
    float32x4_t va0 = vld1q_f32(a + i);
    float32x4_t vb0 = vld1q_f32(b + i);
    float32x4_t va1 = vld1q_f32(a + i + 4);
    float32x4_t vb1 = vld1q_f32(b + i + 4);
#if defined(__ARM_FEATURE_FMA)
    acc0 = vfmaq_f32(acc0, va0, vb0);
    acc0 = vfmaq_f32(acc0, va1, vb1);
#else
    acc0 = vaddq_f32(acc0, vmulq_f32(va0, vb0));
    acc0 = vaddq_f32(acc0, vmulq_f32(va1, vb1));
#endif
  }
  float32x2_t pair = vadd_f32(vget_low_f32(acc0), vget_high_f32(acc0));
  pair = vpadd_f32(pair, pair);
  sum = vget_lane_f32(pair, 0);

  for (; i < count; ++i)
    sum = fmaf(a[i], b[i], sum);
  return sum;

#elif defined(__wasm_simd128__)
  // WASM SIMD128 basic vectorized loop (no alignment control here)
  v128_t acc = wasm_f32x4_splat(0.0f);
  for (; i + 4 <= count; i += 4) {
    v128_t va = wasm_v128_load(a + i);
    v128_t vb = wasm_v128_load(b + i);
    acc = wasm_f32x4_add(acc, wasm_f32x4_mul(va, vb));
  }
  acc = wasm_f32x4_add(acc, wasm_i32x4_shuffle(acc, acc, 2, 3, 0, 1));
  acc = wasm_f32x4_add(acc, wasm_i32x4_shuffle(acc, acc, 1, 0, 3, 2));
  sum = wasm_f32x4_extract_lane(acc, 0);
  for (; i < count; ++i)
    sum = fmaf(a[i], b[i], sum);
  return sum;

#elif defined(__riscv_vector)
  // RVV accumulate
  size_t vl = __riscv_vsetvl_e32m1(count);
  vfloat32m1_t acc = __riscv_vfmv_v_f_f32m1(0.0f, vl);
  for (; i < count;) {
    vl = __riscv_vsetvl_e32m1(count - i);
    vfloat32m1_t va = __riscv_vle32_v_f32m1(a + i, vl);
    vfloat32m1_t vb = __riscv_vle32_v_f32m1(b + i, vl);
    vfloat32m1_t prod = __riscv_vfmul_vv_f32m1(va, vb, vl);
    acc = __riscv_vfadd_vv_f32m1(acc, prod, vl);
    i += vl;
  }
  vfloat32m1_t red =
      __riscv_vfredsum_vs_f32m1_f32m1(acc, vfmv_s_f_f32m1(0.0f, 1), vl);
  sum = vfmv_f_s_f32m1_f32(red);
  return sum;

#else
  // portable scalar fallback (with FMA if available)
  for (; i < count; ++i) {
#if defined(__FMA__) ||                                                        \
    defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    sum = fmaf(a[i], b[i], sum);
#else
    sum += a[i] * b[i];
#endif
  }
  return sum;
#endif
}

WCN_API_EXPORT
float wcn_simd_dot_product_kahan_f32(const float *a, const float *b,
                                     size_t count) {
  size_t i = 0;
  float sum = 0.0f;

#if defined(WCN_X86_AVX2)
  __m256 sum_vec = _mm256_setzero_ps();
  __m256 c_vec = _mm256_setzero_ps();

  for (; i + 8 <= count; i += 8) {
    __m256 va = _mm256_loadu_ps(a + i);
    __m256 vb = _mm256_loadu_ps(b + i);
#if defined(__FMA__)
    __m256 prod = _mm256_fmadd_ps(va, vb, _mm256_setzero_ps());
#else
    __m256 prod = _mm256_mul_ps(va, vb);
#endif
    __m256 y = _mm256_sub_ps(prod, c_vec);
    __m256 t = _mm256_add_ps(sum_vec, y);
    __m256 c_temp = _mm256_sub_ps(_mm256_sub_ps(t, sum_vec), y);
    c_vec = c_temp;
    sum_vec = t;
  }

  // 归约 sum_vec
  __m128 low = _mm256_castps256_ps128(sum_vec);
  __m128 high = _mm256_extractf128_ps(sum_vec, 1);
  __m128 s128 = _mm_add_ps(low, high);
  s128 = _mm_add_ps(s128, _mm_movehl_ps(s128, s128));
  __m128 shuf = _mm_shuffle_ps(s128, s128, _MM_SHUFFLE(2, 3, 0, 1));
  s128 = _mm_add_ps(s128, shuf);
  sum = _mm_cvtss_f32(s128);

  // 归约 c_vec
  __m128 cl = _mm256_castps256_ps128(c_vec);
  __m128 ch = _mm256_extractf128_ps(c_vec, 1);
  __m128 c128 = _mm_add_ps(cl, ch);
  c128 = _mm_add_ps(c128, _mm_movehl_ps(c128, c128));
  __m128 cshuf = _mm_shuffle_ps(c128, c128, _MM_SHUFFLE(2, 3, 0, 1));
  c128 = _mm_add_ps(c128, cshuf);
  float c = _mm_cvtss_f32(c128);

  // 尾部标量部分
  for (; i < count; i++) {
#if defined(__FMA__)
    float y = fmaf(a[i], b[i], -c);
#else
    float y = a[i] * b[i] - c;
#endif
    float t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }

  return sum - c;

#elif defined(WCN_X86_SSE2)
  __m128 sum_vec = _mm_setzero_ps();
  __m128 c_vec = _mm_setzero_ps();

  for (; i + 4 <= count; i += 4) {
    __m128 va = _mm_loadu_ps(a + i);
    __m128 vb = _mm_loadu_ps(b + i);
#if defined(__FMA__)
    __m128 prod = _mm_fmadd_ps(va, vb, _mm_setzero_ps());
#else
    __m128 prod = _mm_mul_ps(va, vb);
#endif
    __m128 y = _mm_sub_ps(prod, c_vec);
    __m128 t = _mm_add_ps(sum_vec, y);
    __m128 c_temp = _mm_sub_ps(_mm_sub_ps(t, sum_vec), y);
    c_vec = c_temp;
    sum_vec = t;
  }

  __m128 temp = _mm_add_ps(sum_vec, _mm_movehl_ps(sum_vec, sum_vec));
  __m128 shuf = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(2, 3, 0, 1));
  temp = _mm_add_ps(temp, shuf);
  sum = _mm_cvtss_f32(temp);

  __m128 ctemp = _mm_add_ps(c_vec, _mm_movehl_ps(c_vec, c_vec));
  __m128 cshuf = _mm_shuffle_ps(ctemp, ctemp, _MM_SHUFFLE(2, 3, 0, 1));
  ctemp = _mm_add_ps(ctemp, cshuf);
  float c = _mm_cvtss_f32(ctemp);

  for (; i < count; i++) {
#if defined(__FMA__)
    float y = fmaf(a[i], b[i], -c);
#else
    float y = a[i] * b[i] - c;
#endif
    float t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }

  return sum - c;

#elif defined(WCN_ARM_NEON)
  float32x4_t sum_vec = vdupq_n_f32(0.0f);
  float32x4_t c_vec = vdupq_n_f32(0.0f);

  for (; i + 4 <= count; i += 4) {
    float32x4_t va = vld1q_f32(a + i);
    float32x4_t vb = vld1q_f32(b + i);
#if defined(__ARM_FEATURE_FMA)
    float32x4_t prod = vfmaq_f32(vdupq_n_f32(0.0f), va, vb);
#else
    float32x4_t prod = vmulq_f32(va, vb);
#endif
    float32x4_t y = vsubq_f32(prod, c_vec);
    float32x4_t t = vaddq_f32(sum_vec, y);
    float32x4_t c_temp = vsubq_f32(vsubq_f32(t, sum_vec), y);
    c_vec = c_temp;
    sum_vec = t;
  }

  float32x2_t s2 = vadd_f32(vget_low_f32(sum_vec), vget_high_f32(sum_vec));
  s2 = vpadd_f32(s2, s2);
  sum = vget_lane_f32(s2, 0);

  float32x2_t c2 = vadd_f32(vget_low_f32(c_vec), vget_high_f32(c_vec));
  c2 = vpadd_f32(c2, c2);
  float c = vget_lane_f32(c2, 0);

  for (; i < count; i++) {
#if defined(__ARM_FEATURE_FMA)
    float y = fmaf(a[i], b[i], -c);
#else
    float y = a[i] * b[i] - c;
#endif
    float t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }

  return sum - c;

#elif defined(WCN_WASM_SIMD128)
  v128_t sum_vec = wasm_f32x4_splat(0.0f);
  v128_t c_vec = wasm_f32x4_splat(0.0f);

  for (; i + 4 <= count; i += 4) {
    v128_t va = wasm_v128_load(a + i);
    v128_t vb = wasm_v128_load(b + i);
    v128_t prod = wasm_f32x4_mul(va, vb);
    v128_t y = wasm_f32x4_sub(prod, c_vec);
    v128_t t = wasm_f32x4_add(sum_vec, y);
    v128_t c_temp = wasm_f32x4_sub(wasm_f32x4_sub(t, sum_vec), y);
    c_vec = c_temp;
    sum_vec = t;
  }

  sum_vec =
      wasm_f32x4_add(sum_vec, wasm_i32x4_shuffle(sum_vec, sum_vec, 2, 3, 0, 1));
  sum_vec =
      wasm_f32x4_add(sum_vec, wasm_i32x4_shuffle(sum_vec, sum_vec, 1, 0, 3, 2));
  sum = wasm_f32x4_extract_lane(sum_vec, 0);

  c_vec = wasm_f32x4_add(c_vec, wasm_i32x4_shuffle(c_vec, c_vec, 2, 3, 0, 1));
  c_vec = wasm_f32x4_add(c_vec, wasm_i32x4_shuffle(c_vec, c_vec, 1, 0, 3, 2));
  float c = wasm_f32x4_extract_lane(c_vec, 0);

  for (; i < count; i++) {
    float y = fmaf(a[i], b[i], -c);
    float t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }

  return sum - c;

#elif defined(WCN_RISCV_RVV)
  // RISC-V Vector Kahan求和实现
  size_t vl = __riscv_vsetvl_e32m1(count);
  vfloat32m1_t sum_vec = __riscv_vfmv_v_f_f32m1(0.0f, vl);
  vfloat32m1_t c_vec = __riscv_vfmv_v_f_f32m1(0.0f, vl);

  for (; i + vl <= count; i += vl) {
    vl = __riscv_vsetvl_e32m1(count - i);
    vfloat32m1_t va = __riscv_vle32_v_f32m1(a + i, vl);
    vfloat32m1_t vb = __riscv_vle32_v_f32m1(b + i, vl);
    vfloat32m1_t prod = __riscv_vfmul_vv_f32m1(va, vb, vl);

    // Kahan求和核心算法的SIMD版本
    vfloat32m1_t y = __riscv_vfsub_vv_f32m1(prod, c_vec, vl);
    vfloat32m1_t t = __riscv_vfadd_vv_f32m1(sum_vec, y, vl);
    vfloat32m1_t c_temp =
        __riscv_vfsub_vv_f32m1(__riscv_vfsub_vv_f32m1(t, sum_vec, vl), y, vl);
    c_vec = c_temp;
    sum_vec = t;
  }

  // 归约求和
  vfloat32m1_t red_sum =
      __riscv_vfredsum_vs_f32m1_f32m1(sum_vec, vfmv_s_f_f32m1(0.0f, 1), vl);
  sum = vfmv_f_s_f32m1_f32(red_sum);

  // 归约补偿值
  vfloat32m1_t red_c =
      __riscv_vfredsum_vs_f32m1_f32m1(c_vec, vfmv_s_f_f32m1(0.0f, 1), vl);
  float c = vfmv_f_s_f32m1_f32(red_c);

  // 处理剩余元素
  for (; i < count; i++) {
    float y = a[i] * b[i] - c;
    float t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }

  return sum - c;

#else
  float c = 0.0f;
  for (; i < count; i++) {
#if defined(__FMA__)
    float y = fmaf(a[i], b[i], -c);
#else
    float y = a[i] * b[i] - c;
#endif
    float t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }
  return sum - c;
#endif
}

WCN_API_EXPORT
void wcn_simd_add_array_f32(const float *WCN_RESTRICT a,
                            const float *WCN_RESTRICT b, float *WCN_RESTRICT c,
                            size_t count) {
  size_t i = 0;
  const float *pa = a;
  const float *pb = b;
  float *pc = c;

  /* --- prologue: advance to alignment boundary for streaming stores if
   * possible --- */
#if defined(WCN_X86_AVX2)
  const size_t _align_bytes = 32;
#else
  const size_t _align_bytes = 16;
#endif

  /* compute bytes-to-align for all three pointers at once (branch-light) */
  size_t combined = ((size_t)pa | (size_t)pb | (size_t)pc);
  size_t mis = combined & (_align_bytes - 1);
  size_t lead_bytes = ((_align_bytes - mis) & (_align_bytes - 1));
  size_t lead = lead_bytes / sizeof(float);
  if (lead > count)
    lead = count;

  for (size_t k = 0; k < lead; ++k) {
    *pc++ = *pa++ + *pb++;
    ++i;
  }

  /* decide whether to use non-temporal (stream) stores:
     use when total data size large and pc is aligned */
  int use_nt = 0;
  size_t total_bytes = count * sizeof(float);
#if defined(WCN_X86_AVX2)
  if (((size_t)pc & 31) == 0 && total_bytes >= (64 * 1024))
    use_nt = 1;
#elif defined(WCN_X86_SSE2)
  if (((size_t)pc & 15) == 0 && total_bytes >= (64 * 1024))
    use_nt = 1;
#endif

#if defined(WCN_X86_AVX2)
  /* AVX2 path: prefer aligned loads/stores after prologue.
     Use streaming stores if use_nt==1 (writes not reused). */
  if (use_nt) {
    /* bigger unroll: 32 floats per outer iter (4 * 256-bit stores) */
    for (; i + 32 <= count; i += 32) {
      __m256 a0 = _mm256_load_ps(pa + 0);
      __m256 a1 = _mm256_load_ps(pa + 8);
      __m256 a2 = _mm256_load_ps(pa + 16);
      __m256 a3 = _mm256_load_ps(pa + 24);

      __m256 b0 = _mm256_load_ps(pb + 0);
      __m256 b1 = _mm256_load_ps(pb + 8);
      __m256 b2 = _mm256_load_ps(pb + 16);
      __m256 b3 = _mm256_load_ps(pb + 24);

      _mm256_stream_ps(pc + 0, _mm256_add_ps(a0, b0));
      _mm256_stream_ps(pc + 8, _mm256_add_ps(a1, b1));
      _mm256_stream_ps(pc + 16, _mm256_add_ps(a2, b2));
      _mm256_stream_ps(pc + 24, _mm256_add_ps(a3, b3));

      pa += 32;
      pb += 32;
      pc += 32;
    }
    for (; i + 16 <= count; i += 16) {
      __m256 a0 = _mm256_load_ps(pa);
      __m256 b0 = _mm256_load_ps(pb);
      __m256 a1 = _mm256_load_ps(pa + 8);
      __m256 b1 = _mm256_load_ps(pb + 8);
      _mm256_stream_ps(pc, _mm256_add_ps(a0, b0));
      _mm256_stream_ps(pc + 8, _mm256_add_ps(a1, b1));
      pa += 16;
      pb += 16;
      pc += 16;
    }
    /* small remainder with aligned loads/stores */
    for (; i + 8 <= count; i += 8) {
      __m256 va = _mm256_load_ps(pa);
      __m256 vb = _mm256_load_ps(pb);
      _mm256_stream_ps(pc, _mm256_add_ps(va, vb));
      pa += 8;
      pb += 8;
      pc += 8;
    }
    /* fence to ensure stores are ordered */
    _mm_sfence();
  } else {
    /* normal (non-streaming) - use aligned loads/stores (prologue ensured
     * alignment) */
    for (; i + 16 <= count; i += 16) {
      __m256 a0 = _mm256_load_ps(pa + 0);
      __m256 b0 = _mm256_load_ps(pb + 0);
      __m256 a1 = _mm256_load_ps(pa + 8);
      __m256 b1 = _mm256_load_ps(pb + 8);
      _mm256_store_ps(pc + 0, _mm256_add_ps(a0, b0));
      _mm256_store_ps(pc + 8, _mm256_add_ps(a1, b1));
      pa += 16;
      pb += 16;
      pc += 16;
    }
    for (; i + 8 <= count; i += 8) {
      __m256 va = _mm256_load_ps(pa);
      __m256 vb = _mm256_load_ps(pb);
      _mm256_store_ps(pc, _mm256_add_ps(va, vb));
      pa += 8;
      pb += 8;
      pc += 8;
    }
  }

#elif defined(WCN_WASM_SIMD128)
  /* WASM SIMD128 - use v128 aligned loads/stores (we assume alignment from
   * prologue) */
  for (; i + 16 <= count; i += 16) {
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

#elif defined(WCN_X86_SSE2)
  /* SSE2: use streaming stores if enabled and aligned */
  if (use_nt) {
    for (; i + 16 <= count; i += 16) {
      __m128 a0 = _mm_load_ps(pa + 0);
      __m128 b0 = _mm_load_ps(pb + 0);
      __m128 a1 = _mm_load_ps(pa + 4);
      __m128 b1 = _mm_load_ps(pb + 4);
      _mm_stream_ps(pc + 0, _mm_add_ps(a0, b0));
      _mm_stream_ps(pc + 4, _mm_add_ps(a1, b1));
      __m128 a2 = _mm_load_ps(pa + 8);
      __m128 b2 = _mm_load_ps(pb + 8);
      __m128 a3 = _mm_load_ps(pa + 12);
      __m128 b3 = _mm_load_ps(pb + 12);
      _mm_stream_ps(pc + 8, _mm_add_ps(a2, b2));
      _mm_stream_ps(pc + 12, _mm_add_ps(a3, b3));
      pa += 16;
      pb += 16;
      pc += 16;
    }
    _mm_sfence();
  } else {
    for (; i + 8 <= count; i += 8) {
      __m128 a0 = _mm_load_ps(pa + 0);
      __m128 b0 = _mm_load_ps(pb + 0);
      __m128 a1 = _mm_load_ps(pa + 4);
      __m128 b1 = _mm_load_ps(pb + 4);
      _mm_store_ps(pc + 0, _mm_add_ps(a0, b0));
      _mm_store_ps(pc + 4, _mm_add_ps(a1, b1));
      pa += 8;
      pb += 8;
      pc += 8;
    }
    for (; i + 4 <= count; i += 4) {
      __m128 va = _mm_load_ps(pa);
      __m128 vb = _mm_load_ps(pb);
      _mm_store_ps(pc, _mm_add_ps(va, vb));
      pa += 4;
      pb += 4;
      pc += 4;
    }
  }

#elif defined(WCN_ARM_NEON)
  for (; i + 8 <= count; i += 8) {
    float32x4_t a0 = vld1q_f32(pa);
    float32x4_t b0 = vld1q_f32(pb);
    float32x4_t r0 = vaddq_f32(a0, b0);
    vst1q_f32(pc, r0);
    float32x4_t a1 = vld1q_f32(pa + 4);
    float32x4_t b1 = vld1q_f32(pb + 4);
    float32x4_t r1 = vaddq_f32(a1, b1);
    vst1q_f32(pc + 4, r1);
    pa += 8;
    pb += 8;
    pc += 8;
  }
  for (; i + 4 <= count; i += 4) {
    float32x4_t va = vld1q_f32(pa);
    float32x4_t vb = vld1q_f32(pb);
    vst1q_f32(pc, vaddq_f32(va, vb));
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_RISCV_RVV)
  while (i < count) {
    size_t vl = __riscv_vsetvl_e32m1(count - i);
    vfloat32m1_t va = __riscv_vle32_v_f32m1(pa, vl);
    vfloat32m1_t vb = __riscv_vle32_v_f32m1(pb, vl);
    vfloat32m1_t vr = __riscv_vfadd_vv_f32m1(va, vb, vl);
    __riscv_vse32_v_f32m1(pc, vr, vl);
    pa += vl;
    pb += vl;
    pc += vl;
    i += vl;
  }
#endif

  /* scalar tail */
  for (; i < count; ++i) {
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
#if defined(WCN_X86_AVX2) || defined(WCN_X86_AVX)
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 64, 0, 3); // Prefetch for read
    __builtin_prefetch(pb + 64, 0, 3);
    __builtin_prefetch(pc + 64, 1, 3); // Prefetch for write

    __m256 va0 = _mm256_load_ps(pa);
    __m256 vb0 = _mm256_load_ps(pb);
    __m256 vc0 = _mm256_load_ps(pc);
#if defined(WCN_X86_AVX)
    vc0 = _mm256_add_ps(_mm256_mul_ps(va0, vb0), vc0);
#else
    vc0 = _mm256_fmadd_ps(va0, vb0, vc0);
#endif
    _mm256_store_ps(pc, vc0);

    __m256 va1 = _mm256_load_ps(pa + 8);
    __m256 vb1 = _mm256_load_ps(pb + 8);
    __m256 vc1 = _mm256_load_ps(pc + 8);
    // vc1 = _mm256_fmadd_ps(va1, vb1, vc1);
#if defined(WCN_X86_AVX)
    vc1 = _mm256_add_ps(_mm256_mul_ps(va1, vb1), vc1);
#else
    vc1 = _mm256_fmadd_ps(va1, vb1, vc1);
#endif
    _mm256_store_ps(pc + 8, vc1);

    pa += 16;
    pb += 16;
    pc += 16;
  }

  for (; i + 8 <= count; i += 8) {
    __m256 va = _mm256_load_ps(pa);
    __m256 vb = _mm256_load_ps(pb);
    __m256 vc = _mm256_load_ps(pc);
    // vc = _mm256_fmadd_ps(va, vb, vc);
#if defined(WCN_X86_AVX)
    vc = _mm256_add_ps(_mm256_mul_ps(va, vb), vc);
#else
    vc = _mm256_fmadd_ps(va, vb, vc);
#endif
    _mm256_store_ps(pc, vc);

    pa += 8;
    pb += 8;
    pc += 8;
  }
#endif

  // WebAssembly SIMD128 implementation with loop unrolling
#if defined(WCN_WASM_SIMD128)
  // Process 64-element chunks
  for (; i + 64 <= count; i += 64) {
    __builtin_prefetch(pa + 128);
    __builtin_prefetch(pb + 128);
    __builtin_prefetch(pc + 128);

    // Process 4 vectors (64 bytes) at a time
    // 每个向量处理16字节（4个float）
    v128_t v0_0 = wasm_v128_load(pa);
    v128_t v1_0 = wasm_v128_load(pb);
    v128_t v2_0 = wasm_v128_load(pc);
    wasm_v128_store(pc, wasm_f32x4_qfma(v0_0, v1_0, v2_0));

    // 修正：+16字节偏移（4个float）
    v128_t v0_1 = wasm_v128_load(pa + 4); // ✅ pa + 4*sizeof(float)
    v128_t v1_1 = wasm_v128_load(pb + 4); // ✅
    v128_t v2_1 = wasm_v128_load(pc + 4); // ✅
    wasm_v128_store(pc + 4, wasm_f32x4_qfma(v0_1, v1_1, v2_1));

    // 继续修正后续偏移
    v128_t v0_2 = wasm_v128_load(pa + 8); // ✅
    v128_t v1_2 = wasm_v128_load(pb + 8); // ✅
    v128_t v2_2 = wasm_v128_load(pc + 8); // ✅
    wasm_v128_store(pc + 8, wasm_f32x4_qfma(v0_2, v1_2, v2_2));

    v128_t v0_3 = wasm_v128_load(pa + 12); // ✅
    v128_t v1_3 = wasm_v128_load(pb + 12); // ✅
    v128_t v2_3 = wasm_v128_load(pc + 12); // ✅
    wasm_v128_store(pc + 12, wasm_f32x4_qfma(v0_3, v1_3, v2_3));

    pa += 64;
    pb += 64;
    pc += 64;
  }

  // Process 16-element chunks
  for (; i + 16 <= count; i += 16) {
    v128_t va0 = wasm_v128_load(pa);
    v128_t vb0 = wasm_v128_load(pb);
    v128_t vc0 = wasm_v128_load(pc);
    wasm_v128_store(pc, wasm_f32x4_qfma(va0, vb0, vc0));

    v128_t va1 = wasm_v128_load(pa + 4);
    v128_t vb1 = wasm_v128_load(pb + 4);
    v128_t vc1 = wasm_v128_load(pc + 4);
    wasm_v128_store(pc + 4, wasm_f32x4_qfma(va1, vb1, vc1));

    v128_t va2 = wasm_v128_load(pa + 8);
    v128_t vb2 = wasm_v128_load(pb + 8);
    v128_t vc2 = wasm_v128_load(pc + 8);
    wasm_v128_store(pc + 8, wasm_f32x4_qfma(va2, vb2, vc2));

    v128_t va3 = wasm_v128_load(pa + 12);
    v128_t vb3 = wasm_v128_load(pb + 12);
    v128_t vc3 = wasm_v128_load(pc + 12);
    wasm_v128_store(pc + 12, wasm_f32x4_qfma(va3, vb3, vc3));

    pa += 16;
    pb += 16;
    pc += 16;
  }

  // Process 4-element chunks
  for (; i + 4 <= count; i += 4) {
    v128_t va = wasm_v128_load(pa);
    v128_t vb = wasm_v128_load(pb);
    v128_t vc = wasm_v128_load(pc);
    wasm_v128_store(pc, wasm_f32x4_qfma(va, vb, vc));

    pa += 4;
    pb += 4;
    pc += 4;
  }
#endif

  // SSE2 or ARM NEON implementation with loop unrolling
#if defined(WCN_X86_SSE2)
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 64);
    __builtin_prefetch(pb + 64);
    __builtin_prefetch(pc + 64);

    __m128 va0 = _mm_loadu_ps(pa);
    __m128 vb0 = _mm_loadu_ps(pb);
    __m128 vc0 = _mm_loadu_ps(pc);
    vc0 = _mm_add_ps(_mm_mul_ps(va0, vb0), vc0);
    _mm_store_ps(pc, vc0);

    __m128 va1 = _mm_loadu_ps(pa + 4);
    __m128 vb1 = _mm_loadu_ps(pb + 4);
    __m128 vc1 = _mm_loadu_ps(pc + 4);
    vc1 = _mm_add_ps(_mm_mul_ps(va1, vb1), vc1);
    _mm_store_ps(pc + 4, vc1);

    __m128 va2 = _mm_loadu_ps(pa + 8);
    __m128 vb2 = _mm_loadu_ps(pb + 8);
    __m128 vc2 = _mm_loadu_ps(pc + 8);
    vc2 = _mm_add_ps(_mm_mul_ps(va2, vb2), vc2);
    _mm_store_ps(pc + 8, vc2);

    __m128 va3 = _mm_loadu_ps(pa + 12);
    __m128 vb3 = _mm_loadu_ps(pb + 12);
    __m128 vc3 = _mm_loadu_ps(pc + 12);
    vc3 = _mm_add_ps(_mm_mul_ps(va3, vb3), vc3);
    _mm_store_ps(pc + 12, vc3);

    pa += 16;
    pb += 16;
    pc += 16;
  }

  for (; i + 8 <= count; i += 8) {
    __m128 va0 = _mm_loadu_ps(pa);
    __m128 vb0 = _mm_loadu_ps(pb);
    __m128 vc0 = _mm_loadu_ps(pc);
    vc0 = _mm_add_ps(_mm_mul_ps(va0, vb0), vc0);
    _mm_store_ps(pc, vc0);

    __m128 va1 = _mm_loadu_ps(pa + 4);
    __m128 vb1 = _mm_loadu_ps(pb + 4);
    __m128 vc1 = _mm_loadu_ps(pc + 4);
    vc1 = _mm_add_ps(_mm_mul_ps(va1, vb1), vc1);
    _mm_store_ps(pc + 4, vc1);

    pa += 8;
    pb += 8;
    pc += 8;
  }

  for (; i + 4 <= count; i += 4) {
    __m128 va0 = _mm_loadu_ps(pa);
    __m128 vb0 = _mm_loadu_ps(pb);
    __m128 vc0 = _mm_loadu_ps(pc);
    vc0 = _mm_add_ps(_mm_mul_ps(va0, vb0), vc0);
    _mm_store_ps(pc, vc0);

    pa += 4;
    pb += 4;
    pc += 4;
  }
#endif

#if defined(WCN_ARM_NEON)
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 64);
    __builtin_prefetch(pb + 64);
    __builtin_prefetch(pc + 64);

    float32x4_t va0 = vld1q_f32(pa);
    float32x4_t vb0 = vld1q_f32(pb);
    float32x4_t vc0 = vld1q_f32(pc);
    vc0 = vfmaq_f32(va0, vb0, vc0);
    vst1q_f32(pc, vc0);

    float32x4_t va1 = vld1q_f32(pa + 4);
    float32x4_t vb1 = vld1q_f32(pb + 4);
    float32x4_t vc1 = vld1q_f32(pc + 4);
    vc1 = vfmaq_f32(va1, vb1, vc1);
    vst1q_f32(pc + 4, vc1);

    float32x4_t va2 = vld1q_f32(pa + 8);
    float32x4_t vb2 = vld1q_f32(pb + 8);
    float32x4_t vc2 = vld1q_f32(pc + 8);
    vc2 = vfmaq_f32(va2, vb2, vc2);
    vst1q_f32(pc + 8, vc2);

    float32x4_t va3 = vld1q_f32(pa + 12);
    float32x4_t vb3 = vld1q_f32(pb + 12);
    float32x4_t vc3 = vld1q_f32(pc + 12);
    vc3 = vfmaq_f32(va3, vb3, vc3);
    vst1q_f32(pc + 12, vc3);

    pa += 16;
    pb += 16;
    pc += 16;
  }

  for (; i + 8 <= count; i += 8) {
    float32x4_t va0 = vld1q_f32(pa);
    float32x4_t vb0 = vld1q_f32(pb);
    float32x4_t vc0 = vld1q_f32(pc);
    vc0 = vfmaq_f32(va0, vb0, vc0);
    vst1q_f32(pc, vc0);

    float32x4_t va1 = vld1q_f32(pa + 4);
    float32x4_t vb1 = vld1q_f32(pb + 4);
    float32x4_t vc1 = vld1q_f32(pc + 4);
    vc1 = vfmaq_f32(va1, vb1, vc1);
    vst1q_f32(pc + 4, vc1);

    pa += 8;
    pb += 8;
    pc += 8;
  }

  for (; i + 4 <= count; i += 4) {
    float32x4_t va0 = vld1q_f32(pa);
    float32x4_t vb0 = vld1q_f32(pb);
    float32x4_t vc0 = vld1q_f32(pc);
    vc0 = vfmaq_f32(va0, vb0, vc0);
    vst1q_f32(pc, vc0);

    pa += 4;
    pb += 4;
    pc += 4;
  }
#endif

  // RISC-V Vector Extension implementation
#if defined(WCN_RISCV_RVV)
  size_t vl = __riscv_vsetvl_e32m1(count);
  for (; i + vl <= count; i += vl) {
    vl = __riscv_vsetvl_e32m1(count - i);
    svfloat32_t va = __riscv_vle32_v_f32m1(pa, vl);
    svfloat32_t vb = __riscv_vle32_v_f32m1(pb, vl);
    svfloat32_t vc = __riscv_vle32_v_f32m1(pc, vl);
    vc = __riscv_vfmadd_vv_f32m1(va, vb, vc, vl);
    __riscv_vse32_v_f32m1(pc, vc, vl);

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
