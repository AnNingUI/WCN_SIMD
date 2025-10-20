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

#elif defined(__AVX512F__)
  // AVX-512 path with dual accumulation for ILP
  __m512 sum0 = _mm512_setzero_ps();
  __m512 sum1 = _mm512_setzero_ps();

  for (; i + 32 <= count; i += 32) {
    __m512 va0 = _mm512_loadu_ps(a + i);
    __m512 vb0 = _mm512_loadu_ps(b + i);
    __m512 va1 = _mm512_loadu_ps(a + i + 16);
    __m512 vb1 = _mm512_loadu_ps(b + i + 16);
    sum0 = _mm512_fmadd_ps(va0, vb0, sum0);
    sum1 = _mm512_fmadd_ps(va1, vb1, sum1);
  }

  __m512 sumv = _mm512_add_ps(sum0, sum1);
  sum = _mm512_reduce_add_ps(sumv);

  for (; i < count; ++i)
    sum = fmaf(a[i], b[i], sum);
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

#elif defined(__loongarch_asx)
  // LoongArch LASX (256-bit)
  __m256 acc0 = (__m256)__lasx_xvldi(0);
  for (; i + 8 <= count; i += 8) {
    __m256 va = (__m256)__lasx_xvld(a + i, 0);
    __m256 vb = (__m256)__lasx_xvld(b + i, 0);
    __m256 prod = (__m256)__lasx_xvfmul_s((__m256i)va, (__m256i)vb);
    acc0 = (__m256)__lasx_xvfadd_s((__m256i)acc0, (__m256i)prod);
  }
  float temp[8];
  __lasx_xvst((__m256i)acc0, temp, 0);
  for (int j = 0; j < 8; j++)
    sum += temp[j];
  for (; i < count; ++i)
    sum += a[i] * b[i];
  return sum;

#elif defined(__loongarch_sx)
  // LoongArch LSX (128-bit)
  __m128 acc0 = (__m128)__lsx_vldi(0);
  for (; i + 4 <= count; i += 4) {
    __m128 va = (__m128)__lsx_vld(a + i, 0);
    __m128 vb = (__m128)__lsx_vld(b + i, 0);
    __m128 prod = (__m128)__lsx_vfmul_s((__m128i)va, (__m128i)vb);
    acc0 = (__m128)__lsx_vfadd_s((__m128i)acc0, (__m128i)prod);
  }
  float temp[4];
  __lsx_vst((__m128i)acc0, temp, 0);
  for (int j = 0; j < 4; j++)
    sum += temp[j];
  for (; i < count; ++i)
    sum += a[i] * b[i];
  return sum;

#elif defined(__VSX__)
  // PowerPC VSX
  __vector float acc0 = vec_splats(0.0f);
  for (; i + 4 <= count; i += 4) {
    __vector float va = vec_ld(0, a + i);
    __vector float vb = vec_ld(0, b + i);
    acc0 = vec_madd(va, vb, acc0);
  }
  float temp[4];
  vec_st(acc0, 0, temp);
  for (int j = 0; j < 4; j++)
    sum += temp[j];
  for (; i < count; ++i)
    sum += a[i] * b[i];
  return sum;

#elif defined(__ALTIVEC__)
  // PowerPC AltiVec
  __vector float acc0 = vec_splats(0.0f);
  for (; i + 4 <= count; i += 4) {
    __vector float va = vec_ld(0, a + i);
    __vector float vb = vec_ld(0, b + i);
    __vector float prod = vec_mul(va, vb);
    acc0 = vec_add(acc0, prod);
  }
  float temp[4];
  vec_st(acc0, 0, temp);
  for (int j = 0; j < 4; j++)
    sum += temp[j];
  for (; i < count; ++i)
    sum += a[i] * b[i];
  return sum;

#elif defined(__mips_msa)
  // MIPS MSA
  v4f32 acc0 = __msa_fill_w(0);
  for (; i + 4 <= count; i += 4) {
    v4f32 va = __msa_ld_w((void *)(a + i), 0);
    v4f32 vb = __msa_ld_w((void *)(b + i), 0);
    v4f32 prod = __msa_fmul_w(va, vb);
    acc0 = __msa_fadd_w(acc0, prod);
  }
  float temp[4];
  __msa_st_w((v4i32)acc0, temp, 0);
  for (int j = 0; j < 4; j++)
    sum += temp[j];
  for (; i < count; ++i)
    sum += a[i] * b[i];
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

/* Forward declaration */
static inline void neumaier_sum(float *sum, float *c, float input);

/* Helper: Kahan reduction for 4 floats with improved precision */
static inline void kahan_reduce_4(const float *values, float *out_sum,
                                  float *out_c) {
  // Use pairwise summation for better numerical stability
  float s0 = values[0] + values[1];
  float c0 = (fabsf(values[0]) >= fabsf(values[1]))
                 ? ((values[0] - s0) + values[1])
                 : ((values[1] - s0) + values[0]);

  float s1 = values[2] + values[3];
  float c1 = (fabsf(values[2]) >= fabsf(values[3]))
                 ? ((values[2] - s1) + values[3])
                 : ((values[3] - s1) + values[2]);

  // Combine pairs with Neumaier's algorithm
  float sum = s0;
  float c = c0;
  neumaier_sum(&sum, &c, s1);
  neumaier_sum(&sum, &c, c1);

  *out_sum = sum;
  *out_c = c;
}

/* Helper: Parallel Kahan reduction for 8 floats using divide-and-conquer */
static inline void kahan_reduce_8(const float *values, float *out_sum,
                                  float *out_c) {
#if defined(__AVX2__) || defined(__AVX__)
  /* Strategy: Use double precision with Kahan summation for maximum accuracy
   * Convert 8 floats -> double precision and perform Kahan reduction */

  /* Load as floats, convert to doubles for reduction */
  __m128 lo_f = _mm_loadu_ps(values);     /* values[0..3] */
  __m128 hi_f = _mm_loadu_ps(values + 4); /* values[4..7] */

  /* Convert to double precision (2x4 = 8 doubles total) */
  __m256d lo_d = _mm256_cvtps_pd(lo_f);
  __m256d hi_d = _mm256_cvtps_pd(hi_f);

  /* Extract individual doubles for Kahan summation */
  double vals_d[8];
  _mm256_storeu_pd(vals_d, lo_d);
  _mm256_storeu_pd(vals_d + 4, hi_d);

  /* Perform Kahan summation in double precision for maximum accuracy */
  double sum_d = 0.0;
  double c_d = 0.0;

  for (int i = 0; i < 8; i++) {
    double y = vals_d[i] - c_d;
    double t = sum_d + y;
    c_d = (t - sum_d) - y;
    sum_d = t;
  }

  /* Convert back to float, preserving both sum and compensation */
  *out_sum = (float)sum_d;
  *out_c = (float)c_d;

#else
  /* Fallback: Divide-and-conquer with 2 parallel Kahan reductions */
  float sum1, c1, sum2, c2;

  /* Reduce first 4 values */
  kahan_reduce_4(values, &sum1, &c1);

  /* Reduce second 4 values */
  kahan_reduce_4(values + 4, &sum2, &c2);

  /* Combine results with Neumaier's algorithm */
  float sum = sum1;
  float c = c1;
  neumaier_sum(&sum, &c, sum2);
  neumaier_sum(&sum, &c, c2);

  *out_sum = sum;
  *out_c = c;
#endif
}

/* Helper: Neumaier's improved Kahan summation for scalars */
static inline void neumaier_sum(float *sum, float *c, float input) {
  float t = *sum + input;
  if (fabsf(*sum) >= fabsf(input)) {
    *c += (*sum - t) + input; // sum is larger, use standard compensation
  } else {
    *c += (input - t) + *sum; // input is larger, swap order
  }
  *sum = t;
}

WCN_API_EXPORT
float wcn_simd_dot_product_kahan_f32(const float *a, const float *b,
                                     size_t count) {
  size_t i = 0;
  float sum, c = 0.0f;

#if defined(WCN_X86_AVX2)
  // Use double precision accumulators for maximum precision
  // Process 4 floats at a time, convert to double, accumulate in double
  // precision
  __m256d sum_d0 = _mm256_setzero_pd();
  __m256d c_d0 = _mm256_setzero_pd();
  __m256d sum_d1 = _mm256_setzero_pd();
  __m256d c_d1 = _mm256_setzero_pd();
  __m256d sum_d2 = _mm256_setzero_pd();
  __m256d c_d2 = _mm256_setzero_pd();
  __m256d sum_d3 = _mm256_setzero_pd();
  __m256d c_d3 = _mm256_setzero_pd();

  // Main vectorized loop: process 16 floats (4x4) per iteration with double
  // precision
  for (; i + 16 <= count; i += 16) {
    // Aggressive prefetching for next iteration
    if (i + 32 <= count) {
      _mm_prefetch((const char *)(a + i + 32), _MM_HINT_T0);
      _mm_prefetch((const char *)(b + i + 32), _MM_HINT_T0);
    }

    // Load 4 floats at a time, convert to double, and accumulate
    // Chunk 0: elements [i, i+4)
    __m128 va0_f = _mm_loadu_ps(a + i);
    __m128 vb0_f = _mm_loadu_ps(b + i);
    __m256d va0_d = _mm256_cvtps_pd(va0_f);
    __m256d vb0_d = _mm256_cvtps_pd(vb0_f);
    __m256d prod0_d = _mm256_mul_pd(va0_d, vb0_d);

    __m256d y0 = _mm256_sub_pd(prod0_d, c_d0);
    __m256d t0 = _mm256_add_pd(sum_d0, y0);
    c_d0 = _mm256_sub_pd(_mm256_sub_pd(t0, sum_d0), y0);
    sum_d0 = t0;

    // Chunk 1: elements [i+4, i+8)
    __m128 va1_f = _mm_loadu_ps(a + i + 4);
    __m128 vb1_f = _mm_loadu_ps(b + i + 4);
    __m256d va1_d = _mm256_cvtps_pd(va1_f);
    __m256d vb1_d = _mm256_cvtps_pd(vb1_f);
    __m256d prod1_d = _mm256_mul_pd(va1_d, vb1_d);

    __m256d y1 = _mm256_sub_pd(prod1_d, c_d1);
    __m256d t1 = _mm256_add_pd(sum_d1, y1);
    c_d1 = _mm256_sub_pd(_mm256_sub_pd(t1, sum_d1), y1);
    sum_d1 = t1;

    // Chunk 2: elements [i+8, i+12)
    __m128 va2_f = _mm_loadu_ps(a + i + 8);
    __m128 vb2_f = _mm_loadu_ps(b + i + 8);
    __m256d va2_d = _mm256_cvtps_pd(va2_f);
    __m256d vb2_d = _mm256_cvtps_pd(vb2_f);
    __m256d prod2_d = _mm256_mul_pd(va2_d, vb2_d);

    __m256d y2 = _mm256_sub_pd(prod2_d, c_d2);
    __m256d t2 = _mm256_add_pd(sum_d2, y2);
    c_d2 = _mm256_sub_pd(_mm256_sub_pd(t2, sum_d2), y2);
    sum_d2 = t2;

    // Chunk 3: elements [i+12, i+16)
    __m128 va3_f = _mm_loadu_ps(a + i + 12);
    __m128 vb3_f = _mm_loadu_ps(b + i + 12);
    __m256d va3_d = _mm256_cvtps_pd(va3_f);
    __m256d vb3_d = _mm256_cvtps_pd(vb3_f);
    __m256d prod3_d = _mm256_mul_pd(va3_d, vb3_d);

    __m256d y3 = _mm256_sub_pd(prod3_d, c_d3);
    __m256d t3 = _mm256_add_pd(sum_d3, y3);
    c_d3 = _mm256_sub_pd(_mm256_sub_pd(t3, sum_d3), y3);
    sum_d3 = t3;
  }

  // Reduce double precision accumulators to scalar
  double sum_d_vals[16], c_d_vals[16];
  _mm256_storeu_pd(sum_d_vals, sum_d0);
  _mm256_storeu_pd(sum_d_vals + 4, sum_d1);
  _mm256_storeu_pd(sum_d_vals + 8, sum_d2);
  _mm256_storeu_pd(sum_d_vals + 12, sum_d3);
  _mm256_storeu_pd(c_d_vals, c_d0);
  _mm256_storeu_pd(c_d_vals + 4, c_d1);
  _mm256_storeu_pd(c_d_vals + 8, c_d2);
  _mm256_storeu_pd(c_d_vals + 12, c_d3);

  // Reduce in double precision using Kahan summation
  double sum_d = 0.0;
  double c_d = 0.0;

  for (int j = 0; j < 16; j++) {
    double y = sum_d_vals[j] - c_d;
    double t = sum_d + y;
    c_d = (t - sum_d) - y;
    sum_d = t;
  }

  // Add compensation terms
  for (int j = 0; j < 16; j++) {
    double y = c_d_vals[j] - c_d;
    double t = sum_d + y;
    c_d = (t - sum_d) - y;
    sum_d = t;
  }

  // Scalar tail in double precision
  for (; i < count; i++) {
    double prod_d = (double)a[i] * (double)b[i];
    double y = prod_d - c_d;
    double t = sum_d + y;
    c_d = (t - sum_d) - y;
    sum_d = t;
  }

  return (float)(sum_d + c_d);

#elif defined(WCN_X86_AVX512F)
  // Use 4 independent Kahan accumulators for better ILP
  __m512 sum_vec0 = _mm512_setzero_ps();
  __m512 c_vec0 = _mm512_setzero_ps();
  __m512 sum_vec1 = _mm512_setzero_ps();
  __m512 c_vec1 = _mm512_setzero_ps();
  __m512 sum_vec2 = _mm512_setzero_ps();
  __m512 c_vec2 = _mm512_setzero_ps();
  __m512 sum_vec3 = _mm512_setzero_ps();
  __m512 c_vec3 = _mm512_setzero_ps();

  // Main vectorized loop: process 64 floats (4x16) per iteration
  for (; i + 64 <= count; i += 64) {
    // Aggressive prefetching for next iteration
    if (i + 128 <= count) {
      _mm_prefetch((const char *)(a + i + 128), _MM_HINT_T0);
      _mm_prefetch((const char *)(b + i + 128), _MM_HINT_T0);
    }

    // Process 4 chunks of 16 floats in parallel for maximum ILP
    __m512 va0 = _mm512_loadu_ps(a + i);
    __m512 vb0 = _mm512_loadu_ps(b + i);
    __m512 va1 = _mm512_loadu_ps(a + i + 16);
    __m512 vb1 = _mm512_loadu_ps(b + i + 16);
    __m512 va2 = _mm512_loadu_ps(a + i + 32);
    __m512 vb2 = _mm512_loadu_ps(b + i + 32);
    __m512 va3 = _mm512_loadu_ps(a + i + 48);
    __m512 vb3 = _mm512_loadu_ps(b + i + 48);

    __m512 prod0 = _mm512_mul_ps(va0, vb0);
    __m512 prod1 = _mm512_mul_ps(va1, vb1);
    __m512 prod2 = _mm512_mul_ps(va2, vb2);
    __m512 prod3 = _mm512_mul_ps(va3, vb3);

    // Kahan summation for all 4 accumulators
    __m512 y0 = _mm512_sub_ps(prod0, c_vec0);
    __m512 t0 = _mm512_add_ps(sum_vec0, y0);
    c_vec0 = _mm512_sub_ps(_mm512_sub_ps(t0, sum_vec0), y0);
    sum_vec0 = t0;

    __m512 y1 = _mm512_sub_ps(prod1, c_vec1);
    __m512 t1 = _mm512_add_ps(sum_vec1, y1);
    c_vec1 = _mm512_sub_ps(_mm512_sub_ps(t1, sum_vec1), y1);
    sum_vec1 = t1;

    __m512 y2 = _mm512_sub_ps(prod2, c_vec2);
    __m512 t2 = _mm512_add_ps(sum_vec2, y2);
    c_vec2 = _mm512_sub_ps(_mm512_sub_ps(t2, sum_vec2), y2);
    sum_vec2 = t2;

    __m512 y3 = _mm512_sub_ps(prod3, c_vec3);
    __m512 t3 = _mm512_add_ps(sum_vec3, y3);
    c_vec3 = _mm512_sub_ps(_mm512_sub_ps(t3, sum_vec3), y3);
    sum_vec3 = t3;
  }

  // Process remaining 16-element chunks
  for (; i + 16 <= count; i += 16) {
    __m512 va = _mm512_loadu_ps(a + i);
    __m512 vb = _mm512_loadu_ps(b + i);
    __m512 prod = _mm512_mul_ps(va, vb);
    __m512 y = _mm512_sub_ps(prod, c_vec0);
    __m512 t = _mm512_add_ps(sum_vec0, y);
    c_vec0 = _mm512_sub_ps(_mm512_sub_ps(t, sum_vec0), y);
    sum_vec0 = t;
  }

  // High-precision reduction
  float sum_vals[64], c_vals[64];
  _mm512_storeu_ps(sum_vals, sum_vec0);
  _mm512_storeu_ps(sum_vals + 16, sum_vec1);
  _mm512_storeu_ps(sum_vals + 32, sum_vec2);
  _mm512_storeu_ps(sum_vals + 48, sum_vec3);
  _mm512_storeu_ps(c_vals, c_vec0);
  _mm512_storeu_ps(c_vals + 16, c_vec1);
  _mm512_storeu_ps(c_vals + 32, c_vec2);
  _mm512_storeu_ps(c_vals + 48, c_vec3);

  // Hierarchical Kahan reduction for all 8 groups of 8
  float sum_parts[8], c_parts[8];
  for (int j = 0; j < 8; j++) {
    kahan_reduce_8(sum_vals + j * 8, &sum_parts[j], &c_parts[j]);
  }

  sum = c = 0.0f;
  for (int j = 0; j < 8; j++) {
    neumaier_sum(&sum, &c, sum_parts[j]);
  }
  for (int j = 0; j < 8; j++) {
    neumaier_sum(&sum, &c, c_parts[j]);
  }

  // Add compensation terms
  float c_sum_parts[8], c_c_parts[8];
  for (int j = 0; j < 8; j++) {
    kahan_reduce_8(c_vals + j * 8, &c_sum_parts[j], &c_c_parts[j]);
  }

  for (int j = 0; j < 8; j++) {
    neumaier_sum(&sum, &c, c_sum_parts[j]);
  }
  for (int j = 0; j < 8; j++) {
    c += c_c_parts[j];
  }

  for (; i < count; i++) {
    float prod = fmaf(a[i], b[i], 0.0f);
    neumaier_sum(&sum, &c, prod);
  }

  return sum + c;

#elif defined(WCN_X86_SSE2)
  // Use dual accumulators for better ILP
  __m128 sum_vec0 = _mm_setzero_ps();
  __m128 c_vec0 = _mm_setzero_ps();
  __m128 sum_vec1 = _mm_setzero_ps();
  __m128 c_vec1 = _mm_setzero_ps();

  // Main loop: process 8 elements per iteration with unrolling
  for (; i + 8 <= count; i += 8) {
    // Prefetch next cache lines
    _mm_prefetch((const char *)(a + i + 8), _MM_HINT_T0);
    _mm_prefetch((const char *)(b + i + 8), _MM_HINT_T0);

    // First 4 elements
    __m128 va0 = _mm_loadu_ps(a + i);
    __m128 vb0 = _mm_loadu_ps(b + i);
#if defined(__FMA__)
    __m128 prod0 = _mm_fmadd_ps(va0, vb0, _mm_setzero_ps());
#else
    __m128 prod0 = _mm_mul_ps(va0, vb0);
#endif
    __m128 y0 = _mm_sub_ps(prod0, c_vec0);
    __m128 t0 = _mm_add_ps(sum_vec0, y0);
    __m128 c_temp0 = _mm_sub_ps(_mm_sub_ps(t0, sum_vec0), y0);
    c_vec0 = c_temp0;
    sum_vec0 = t0;

    // Second 4 elements
    __m128 va1 = _mm_loadu_ps(a + i + 4);
    __m128 vb1 = _mm_loadu_ps(b + i + 4);
#if defined(__FMA__)
    __m128 prod1 = _mm_fmadd_ps(va1, vb1, _mm_setzero_ps());
#else
    __m128 prod1 = _mm_mul_ps(va1, vb1);
#endif
    __m128 y1 = _mm_sub_ps(prod1, c_vec1);
    __m128 t1 = _mm_add_ps(sum_vec1, y1);
    __m128 c_temp1 = _mm_sub_ps(_mm_sub_ps(t1, sum_vec1), y1);
    c_vec1 = c_temp1;
    sum_vec1 = t1;
  }

  // Process remaining 4 elements
  for (; i + 4 <= count; i += 4) {
    __m128 va = _mm_loadu_ps(a + i);
    __m128 vb = _mm_loadu_ps(b + i);
#if defined(__FMA__)
    __m128 prod = _mm_fmadd_ps(va, vb, _mm_setzero_ps());
#else
    __m128 prod = _mm_mul_ps(va, vb);
#endif
    __m128 y = _mm_sub_ps(prod, c_vec0);
    __m128 t = _mm_add_ps(sum_vec0, y);
    __m128 c_temp = _mm_sub_ps(_mm_sub_ps(t, sum_vec0), y);
    c_vec0 = c_temp;
    sum_vec0 = t;
  }

  // Combine and reduce accumulators
  float sum_vals0[4], c_vals0[4];
  float sum_vals1[4], c_vals1[4];
  _mm_storeu_ps(sum_vals0, sum_vec0);
  _mm_storeu_ps(c_vals0, c_vec0);
  _mm_storeu_ps(sum_vals1, sum_vec1);
  _mm_storeu_ps(c_vals1, c_vec1);

  // Reduce first accumulator
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, sum_vals0[j]);
  }
  // Reduce second accumulator
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, sum_vals1[j]);
  }
  // Add compensation terms
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, c_vals0[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, c_vals1[j]);
  }

  // Scalar tail
  for (; i < count; i++) {
#if defined(__FMA__)
    float prod = fmaf(a[i], b[i], 0.0f);
#else
    float prod = a[i] * b[i];
#endif
    neumaier_sum(&sum, &c, prod);
  }

  return sum + c;

#elif defined(WCN_ARM_NEON)
  // Use dual accumulators for better ILP
  float32x4_t sum_vec0 = vdupq_n_f32(0.0f);
  float32x4_t c_vec0 = vdupq_n_f32(0.0f);
  float32x4_t sum_vec1 = vdupq_n_f32(0.0f);
  float32x4_t c_vec1 = vdupq_n_f32(0.0f);

  // Main loop: process 8 elements per iteration with unrolling
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(a + i + 8, 0, 0);
    __builtin_prefetch(b + i + 8, 0, 0);

    // First 4 elements
    float32x4_t va0 = vld1q_f32(a + i);
    float32x4_t vb0 = vld1q_f32(b + i);
#if defined(__ARM_FEATURE_FMA)
    float32x4_t prod0 = vfmaq_f32(vdupq_n_f32(0.0f), va0, vb0);
#else
    float32x4_t prod0 = vmulq_f32(va0, vb0);
#endif
    float32x4_t y0 = vsubq_f32(prod0, c_vec0);
    float32x4_t t0 = vaddq_f32(sum_vec0, y0);
    float32x4_t c_temp0 = vsubq_f32(vsubq_f32(t0, sum_vec0), y0);
    c_vec0 = c_temp0;
    sum_vec0 = t0;

    // Second 4 elements
    float32x4_t va1 = vld1q_f32(a + i + 4);
    float32x4_t vb1 = vld1q_f32(b + i + 4);
#if defined(__ARM_FEATURE_FMA)
    float32x4_t prod1 = vfmaq_f32(vdupq_n_f32(0.0f), va1, vb1);
#else
    float32x4_t prod1 = vmulq_f32(va1, vb1);
#endif
    float32x4_t y1 = vsubq_f32(prod1, c_vec1);
    float32x4_t t1 = vaddq_f32(sum_vec1, y1);
    float32x4_t c_temp1 = vsubq_f32(vsubq_f32(t1, sum_vec1), y1);
    c_vec1 = c_temp1;
    sum_vec1 = t1;
  }

  // Process remaining 4 elements
  for (; i + 4 <= count; i += 4) {
    float32x4_t va = vld1q_f32(a + i);
    float32x4_t vb = vld1q_f32(b + i);
#if defined(__ARM_FEATURE_FMA)
    float32x4_t prod = vfmaq_f32(vdupq_n_f32(0.0f), va, vb);
#else
    float32x4_t prod = vmulq_f32(va, vb);
#endif
    float32x4_t y = vsubq_f32(prod, c_vec0);
    float32x4_t t = vaddq_f32(sum_vec0, y);
    float32x4_t c_temp = vsubq_f32(vsubq_f32(t, sum_vec0), y);
    c_vec0 = c_temp;
    sum_vec0 = t;
  }

  // Combine and reduce accumulators
  float sum_vals0[4], c_vals0[4];
  float sum_vals1[4], c_vals1[4];
  vst1q_f32(sum_vals0, sum_vec0);
  vst1q_f32(c_vals0, c_vec0);
  vst1q_f32(sum_vals1, sum_vec1);
  vst1q_f32(c_vals1, c_vec1);

  sum = c = 0.0f;
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, sum_vals0[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, sum_vals1[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, c_vals0[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, c_vals1[j]);
  }

  // Scalar tail
  for (; i < count; i++) {
#if defined(__ARM_FEATURE_FMA)
    float prod = fmaf(a[i], b[i], 0.0f);
#else
    float prod = a[i] * b[i];
#endif
    neumaier_sum(&sum, &c, prod);
  }

  return sum + c;

#elif defined(WCN_WASM_SIMD128)
  // Use dual accumulators for better ILP
  v128_t sum_vec0 = wasm_f32x4_splat(0.0f);
  v128_t c_vec0 = wasm_f32x4_splat(0.0f);
  v128_t sum_vec1 = wasm_f32x4_splat(0.0f);
  v128_t c_vec1 = wasm_f32x4_splat(0.0f);

  // Main loop: process 8 elements per iteration with unrolling
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(a + i + 8);
    __builtin_prefetch(b + i + 8);

    // First 4 elements
    v128_t va0 = wasm_v128_load(a + i);
    v128_t vb0 = wasm_v128_load(b + i);
    v128_t prod0 = wasm_f32x4_mul(va0, vb0);
    v128_t y0 = wasm_f32x4_sub(prod0, c_vec0);
    v128_t t0 = wasm_f32x4_add(sum_vec0, y0);
    v128_t c_temp0 = wasm_f32x4_sub(wasm_f32x4_sub(t0, sum_vec0), y0);
    c_vec0 = c_temp0;
    sum_vec0 = t0;

    // Second 4 elements
    v128_t va1 = wasm_v128_load(a + i + 4);
    v128_t vb1 = wasm_v128_load(b + i + 4);
    v128_t prod1 = wasm_f32x4_mul(va1, vb1);
    v128_t y1 = wasm_f32x4_sub(prod1, c_vec1);
    v128_t t1 = wasm_f32x4_add(sum_vec1, y1);
    v128_t c_temp1 = wasm_f32x4_sub(wasm_f32x4_sub(t1, sum_vec1), y1);
    c_vec1 = c_temp1;
    sum_vec1 = t1;
  }

  // Process remaining 4 elements
  for (; i + 4 <= count; i += 4) {
    v128_t va = wasm_v128_load(a + i);
    v128_t vb = wasm_v128_load(b + i);
    v128_t prod = wasm_f32x4_mul(va, vb);
    v128_t y = wasm_f32x4_sub(prod, c_vec0);
    v128_t t = wasm_f32x4_add(sum_vec0, y);
    v128_t c_temp = wasm_f32x4_sub(wasm_f32x4_sub(t, sum_vec0), y);
    c_vec0 = c_temp;
    sum_vec0 = t;
  }

  // Combine and reduce accumulators
  float sum_vals0[4], c_vals0[4];
  float sum_vals1[4], c_vals1[4];
  wasm_v128_store(sum_vals0, sum_vec0);
  wasm_v128_store(c_vals0, c_vec0);
  wasm_v128_store(sum_vals1, sum_vec1);
  wasm_v128_store(c_vals1, c_vec1);

  sum = c = 0.0f;
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, sum_vals0[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, sum_vals1[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, c_vals0[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, c_vals1[j]);
  }

  // Scalar tail
  for (; i < count; i++) {
    float prod = fmaf(a[i], b[i], 0.0f);
    neumaier_sum(&sum, &c, prod);
  }

  return sum + c;

#elif defined(WCN_LOONGARCH_LASX)
  // Use dual accumulators for better ILP
  __m256 sum_vec0 = (__m256)__lasx_xvldi(0);
  __m256 c_vec0 = (__m256)__lasx_xvldi(0);
  __m256 sum_vec1 = (__m256)__lasx_xvldi(0);
  __m256 c_vec1 = (__m256)__lasx_xvldi(0);

  // Main loop: process 16 elements per iteration with unrolling
  for (; i + 16 <= count; i += 16) {
    // First 8 elements
    __m256 va0 = (__m256)__lasx_xvld(a + i, 0);
    __m256 vb0 = (__m256)__lasx_xvld(b + i, 0);
    __m256 prod0 = (__m256)__lasx_xvfmul_s((__m256i)va0, (__m256i)vb0);
    __m256 y0 = (__m256)__lasx_xvfsub_s((__m256i)prod0, (__m256i)c_vec0);
    __m256 t0 = (__m256)__lasx_xvfadd_s((__m256i)sum_vec0, (__m256i)y0);
    __m256 c_temp0 = (__m256)__lasx_xvfsub_s(
        (__m256i)__lasx_xvfsub_s((__m256i)t0, (__m256i)sum_vec0), (__m256i)y0);
    c_vec0 = c_temp0;
    sum_vec0 = t0;

    // Second 8 elements
    __m256 va1 = (__m256)__lasx_xvld(a + i + 8, 0);
    __m256 vb1 = (__m256)__lasx_xvld(b + i + 8, 0);
    __m256 prod1 = (__m256)__lasx_xvfmul_s((__m256i)va1, (__m256i)vb1);
    __m256 y1 = (__m256)__lasx_xvfsub_s((__m256i)prod1, (__m256i)c_vec1);
    __m256 t1 = (__m256)__lasx_xvfadd_s((__m256i)sum_vec1, (__m256i)y1);
    __m256 c_temp1 = (__m256)__lasx_xvfsub_s(
        (__m256i)__lasx_xvfsub_s((__m256i)t1, (__m256i)sum_vec1), (__m256i)y1);
    c_vec1 = c_temp1;
    sum_vec1 = t1;
  }

  // Process remaining 8 elements
  for (; i + 8 <= count; i += 8) {
    __m256 va = (__m256)__lasx_xvld(a + i, 0);
    __m256 vb = (__m256)__lasx_xvld(b + i, 0);
    __m256 prod = (__m256)__lasx_xvfmul_s((__m256i)va, (__m256i)vb);
    __m256 y = (__m256)__lasx_xvfsub_s((__m256i)prod, (__m256i)c_vec0);
    __m256 t = (__m256)__lasx_xvfadd_s((__m256i)sum_vec0, (__m256i)y);
    __m256 c_temp = (__m256)__lasx_xvfsub_s(
        (__m256i)__lasx_xvfsub_s((__m256i)t, (__m256i)sum_vec0), (__m256i)y);
    c_vec0 = c_temp;
    sum_vec0 = t;
  }

  // Combine and reduce accumulators
  float sum_vals0[8], c_vals0[8];
  float sum_vals1[8], c_vals1[8];
  __lasx_xvst((__m256i)sum_vec0, sum_vals0, 0);
  __lasx_xvst((__m256i)c_vec0, c_vals0, 0);
  __lasx_xvst((__m256i)sum_vec1, sum_vals1, 0);
  __lasx_xvst((__m256i)c_vec1, c_vals1, 0);

  // Hierarchical reduction
  float sum0, c0, sum1, c1;
  kahan_reduce_8(sum_vals0, &sum0, &c0);
  kahan_reduce_8(sum_vals1, &sum1, &c1);

  sum = sum0;
  c = c0;
  neumaier_sum(&sum, &c, sum1);
  neumaier_sum(&sum, &c, c1);

  // Add compensation terms
  float c_sum0, c_c0, c_sum1, c_c1;
  kahan_reduce_8(c_vals0, &c_sum0, &c_c0);
  kahan_reduce_8(c_vals1, &c_sum1, &c_c1);
  neumaier_sum(&sum, &c, c_sum0);
  neumaier_sum(&sum, &c, c_sum1);
  c += c_c0 + c_c1;

  // Scalar tail
  for (; i < count; i++) {
    float prod = a[i] * b[i];
    neumaier_sum(&sum, &c, prod);
  }

  return sum + c;

#elif defined(WCN_LOONGARCH_LSX)
  // Use dual accumulators for better ILP
  __m128 sum_vec0 = (__m128)__lsx_vldi(0);
  __m128 c_vec0 = (__m128)__lsx_vldi(0);
  __m128 sum_vec1 = (__m128)__lsx_vldi(0);
  __m128 c_vec1 = (__m128)__lsx_vldi(0);

  // Main loop: process 8 elements per iteration with unrolling
  for (; i + 8 <= count; i += 8) {
    // First 4 elements
    __m128 va0 = (__m128)__lsx_vld(a + i, 0);
    __m128 vb0 = (__m128)__lsx_vld(b + i, 0);
    __m128 prod0 = (__m128)__lsx_vfmul_s((__m128i)va0, (__m128i)vb0);
    __m128 y0 = (__m128)__lsx_vfsub_s((__m128i)prod0, (__m128i)c_vec0);
    __m128 t0 = (__m128)__lsx_vfadd_s((__m128i)sum_vec0, (__m128i)y0);
    __m128 c_temp0 = (__m128)__lsx_vfsub_s(
        (__m128i)__lsx_vfsub_s((__m128i)t0, (__m128i)sum_vec0), (__m128i)y0);
    c_vec0 = c_temp0;
    sum_vec0 = t0;

    // Second 4 elements
    __m128 va1 = (__m128)__lsx_vld(a + i + 4, 0);
    __m128 vb1 = (__m128)__lsx_vld(b + i + 4, 0);
    __m128 prod1 = (__m128)__lsx_vfmul_s((__m128i)va1, (__m128i)vb1);
    __m128 y1 = (__m128)__lsx_vfsub_s((__m128i)prod1, (__m128i)c_vec1);
    __m128 t1 = (__m128)__lsx_vfadd_s((__m128i)sum_vec1, (__m128i)y1);
    __m128 c_temp1 = (__m128)__lsx_vfsub_s(
        (__m128i)__lsx_vfsub_s((__m128i)t1, (__m128i)sum_vec1), (__m128i)y1);
    c_vec1 = c_temp1;
    sum_vec1 = t1;
  }

  // Process remaining 4 elements
  for (; i + 4 <= count; i += 4) {
    __m128 va = (__m128)__lsx_vld(a + i, 0);
    __m128 vb = (__m128)__lsx_vld(b + i, 0);
    __m128 prod = (__m128)__lsx_vfmul_s((__m128i)va, (__m128i)vb);
    __m128 y = (__m128)__lsx_vfsub_s((__m128i)prod, (__m128i)c_vec0);
    __m128 t = (__m128)__lsx_vfadd_s((__m128i)sum_vec0, (__m128i)y);
    __m128 c_temp = (__m128)__lsx_vfsub_s(
        (__m128i)__lsx_vfsub_s((__m128i)t, (__m128i)sum_vec0), (__m128i)y);
    c_vec0 = c_temp;
    sum_vec0 = t;
  }

  // Combine and reduce accumulators
  float sum_vals0[4], c_vals0[4];
  float sum_vals1[4], c_vals1[4];
  __lsx_vst((__m128i)sum_vec0, sum_vals0, 0);
  __lsx_vst((__m128i)c_vec0, c_vals0, 0);
  __lsx_vst((__m128i)sum_vec1, sum_vals1, 0);
  __lsx_vst((__m128i)c_vec1, c_vals1, 0);

  sum = c = 0.0f;
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, sum_vals0[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, sum_vals1[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, c_vals0[j]);
  }
  for (int j = 0; j < 4; j++) {
    neumaier_sum(&sum, &c, c_vals1[j]);
  }

  // Scalar tail
  for (; i < count; i++) {
    float prod = a[i] * b[i];
    neumaier_sum(&sum, &c, prod);
  }

  return sum + c;

#elif defined(WCN_RISCV_RVV)
  // RISC-V Vector - use scalar Kahan for better precision
  sum = c = 0.0f;

  // Process with SIMD but reduce carefully
  size_t vl;
  for (; i < count; i += vl) {
    vl = __riscv_vsetvl_e32m1(count - i);
    vfloat32m1_t va = __riscv_vle32_v_f32m1(a + i, vl);
    vfloat32m1_t vb = __riscv_vle32_v_f32m1(b + i, vl);
    vfloat32m1_t prod = __riscv_vfmul_vv_f32m1(va, vb, vl);

    // Extract and accumulate with Neumaier
    float prods[32]; // RVV max practical VL
    __riscv_vse32_v_f32m1(prods, prod, vl);
    for (size_t j = 0; j < vl; j++) {
      neumaier_sum(&sum, &c, prods[j]);
    }
  }

  return sum + c;

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
#if defined(WCN_X86_AVX512F)
  if (((size_t)pc & 63) == 0 && total_bytes >= (128 * 1024))
    use_nt = 1;
#elif defined(WCN_X86_AVX2)
  if (((size_t)pc & 31) == 0 && total_bytes >= (64 * 1024))
    use_nt = 1;
#elif defined(WCN_X86_SSE2)
  if (((size_t)pc & 15) == 0 && total_bytes >= (64 * 1024))
    use_nt = 1;
#endif

#if defined(WCN_X86_AVX512F)
  if (use_nt) {
    for (; i + 16 <= count; i += 16) {
      __m512 va = _mm512_load_ps(pa);
      __m512 vb = _mm512_load_ps(pb);
      _mm512_stream_ps(pc, _mm512_add_ps(va, vb));
      pa += 16;
      pb += 16;
      pc += 16;
    }
    _mm_sfence();
  } else {
    for (; i + 16 <= count; i += 16) {
      __m512 va = _mm512_load_ps(pa);
      __m512 vb = _mm512_load_ps(pb);
      _mm512_store_ps(pc, _mm512_add_ps(va, vb));
      pa += 16;
      pb += 16;
      pc += 16;
    }
  }
  for (; i + 8 <= count; i += 8) {
    __m256 va = _mm256_load_ps(pa);
    __m256 vb = _mm256_load_ps(pb);
    _mm256_store_ps(pc, _mm256_add_ps(va, vb));
    pa += 8;
    pb += 8;
    pc += 8;
  }

#elif defined(WCN_X86_AVX2)
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

#elif defined(WCN_LOONGARCH_LASX)
  for (; i + 8 <= count; i += 8) {
    __m256 va = (__m256)__lasx_xvld(pa, 0);
    __m256 vb = (__m256)__lasx_xvld(pb, 0);
    __lasx_xvst((__m256i)__lasx_xvfadd_s((__m256i)va, (__m256i)vb), pc, 0);
    pa += 8;
    pb += 8;
    pc += 8;
  }

#elif defined(WCN_LOONGARCH_LSX)
  for (; i + 4 <= count; i += 4) {
    __m128 va = (__m128)__lsx_vld(pa, 0);
    __m128 vb = (__m128)__lsx_vld(pb, 0);
    __lsx_vst((__m128i)__lsx_vfadd_s((__m128i)va, (__m128i)vb), pc, 0);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_POWERPC_VSX)
  for (; i + 4 <= count; i += 4) {
    __vector float va = vec_ld(0, pa);
    __vector float vb = vec_ld(0, pb);
    vec_st(vec_add(va, vb), 0, pc);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_POWERPC_ALTIVEC)
  for (; i + 4 <= count; i += 4) {
    __vector float va = vec_ld(0, pa);
    __vector float vb = vec_ld(0, pb);
    vec_st(vec_add(va, vb), 0, pc);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_MIPS_MSA)
  for (; i + 4 <= count; i += 4) {
    v4f32 va = __msa_ld_w((void *)pa, 0);
    v4f32 vb = __msa_ld_w((void *)pb, 0);
    __msa_st_w((v4i32)__msa_fadd_w(va, vb), (void *)pc, 0);
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

#if defined(WCN_X86_AVX512F)
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 64);
    __builtin_prefetch(pb + 64);
    __m512 va = _mm512_load_ps(pa);
    __m512 vb = _mm512_load_ps(pb);
    __m512 vc = _mm512_mul_ps(va, vb);
    _mm512_store_ps(pc, vc);
    pa += 16;
    pb += 16;
    pc += 16;
  }
  for (; i + 8 <= count; i += 8) {
    __m256 va = _mm256_load_ps(pa);
    __m256 vb = _mm256_load_ps(pb);
    __m256 vc = _mm256_mul_ps(va, vb);
    _mm256_store_ps(pc, vc);
    pa += 8;
    pb += 8;
    pc += 8;
  }

#elif defined(WCN_X86_AVX2)
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 32);
    __builtin_prefetch(pb + 32);
    __m256 va = _mm256_load_ps(pa);
    __m256 vb = _mm256_load_ps(pb);
    __m256 vc = _mm256_mul_ps(va, vb);
    _mm256_store_ps(pc, vc);
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

#elif defined(WCN_X86_SSE2)
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 64);
    __builtin_prefetch(pb + 64);
    __m128 va0 = _mm_load_ps(pa);
    __m128 vb0 = _mm_load_ps(pb);
    __m128 vc0 = _mm_mul_ps(va0, vb0);
    _mm_store_ps(pc, vc0);
    __m128 va1 = _mm_load_ps(pa + 4);
    __m128 vb1 = _mm_load_ps(pb + 4);
    __m128 vc1 = _mm_mul_ps(va1, vb1);
    _mm_store_ps(pc + 4, vc1);
    pa += 8;
    pb += 8;
    pc += 8;
  }
  for (; i + 4 <= count; i += 4) {
    __m128 va = _mm_load_ps(pa);
    __m128 vb = _mm_load_ps(pb);
    __m128 vc = _mm_mul_ps(va, vb);
    _mm_store_ps(pc, vc);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_ARM_NEON)
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 64);
    __builtin_prefetch(pb + 64);
    float32x4_t va0 = vld1q_f32(pa);
    float32x4_t vb0 = vld1q_f32(pb);
    float32x4_t vc0 = vmulq_f32(va0, vb0);
    vst1q_f32(pc, vc0);
    float32x4_t va1 = vld1q_f32(pa + 4);
    float32x4_t vb1 = vld1q_f32(pb + 4);
    float32x4_t vc1 = vmulq_f32(va1, vb1);
    vst1q_f32(pc + 4, vc1);
    pa += 8;
    pb += 8;
    pc += 8;
  }
  for (; i + 4 <= count; i += 4) {
    float32x4_t va = vld1q_f32(pa);
    float32x4_t vb = vld1q_f32(pb);
    float32x4_t vc = vmulq_f32(va, vb);
    vst1q_f32(pc, vc);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_LOONGARCH_LASX)
  for (; i + 8 <= count; i += 8) {
    __m256 va = (__m256)__lasx_xvld(pa, 0);
    __m256 vb = (__m256)__lasx_xvld(pb, 0);
    __m256 vc = (__m256)__lasx_xvfmul_s((__m256i)va, (__m256i)vb);
    __lasx_xvst((__m256i)vc, pc, 0);
    pa += 8;
    pb += 8;
    pc += 8;
  }

#elif defined(WCN_LOONGARCH_LSX)
  for (; i + 4 <= count; i += 4) {
    __m128 va = (__m128)__lsx_vld(pa, 0);
    __m128 vb = (__m128)__lsx_vld(pb, 0);
    __m128 vc = (__m128)__lsx_vfmul_s((__m128i)va, (__m128i)vb);
    __lsx_vst((__m128i)vc, pc, 0);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_POWERPC_VSX)
  for (; i + 4 <= count; i += 4) {
    __vector float va = vec_ld(0, pa);
    __vector float vb = vec_ld(0, pb);
    __vector float vc = vec_mul(va, vb);
    vec_st(vc, 0, pc);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_POWERPC_ALTIVEC)
  for (; i + 4 <= count; i += 4) {
    __vector float va = vec_ld(0, pa);
    __vector float vb = vec_ld(0, pb);
    __vector float vc = vec_mul(va, vb);
    vec_st(vc, 0, pc);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_MIPS_MSA)
  for (; i + 4 <= count; i += 4) {
    v4f32 va = __msa_ld_w(pa, 0);
    v4f32 vb = __msa_ld_w(pb, 0);
    v4f32 vc = __msa_fmul_w(va, vb);
    __msa_st_w((v4i32)vc, pc, 0);
    pa += 4;
    pb += 4;
    pc += 4;
  }

#elif defined(WCN_RISCV_RVV)
  size_t vl = __riscv_vsetvl_e32m1(count);
  for (; i + vl <= count; i += vl) {
    vl = __riscv_vsetvl_e32m1(count - i);
    vfloat32m1_t va = __riscv_vle32_v_f32m1(pa, vl);
    vfloat32m1_t vb = __riscv_vle32_v_f32m1(pb, vl);
    vfloat32m1_t vc = __riscv_vfmul_vv_f32m1(va, vb, vl);
    __riscv_vse32_v_f32m1(pc, vc, vl);
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

#if defined(WCN_X86_AVX512F)
  __m512 vs512 = _mm512_set1_ps(scalar);
  for (; i + 16 <= count; i += 16) {
    __builtin_prefetch(pa + 64);
    __m512 va = _mm512_load_ps(pa);
    __m512 vb = _mm512_mul_ps(va, vs512);
    _mm512_store_ps(pb, vb);
    pa += 16;
    pb += 16;
  }
  __m256 vs256 = _mm256_set1_ps(scalar);
  for (; i + 8 <= count; i += 8) {
    __m256 va = _mm256_load_ps(pa);
    __m256 vb = _mm256_mul_ps(va, vs256);
    _mm256_store_ps(pb, vb);
    pa += 8;
    pb += 8;
  }

#elif defined(WCN_X86_AVX2)
  __m256 vs = _mm256_set1_ps(scalar);
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 32);
    __m256 va = _mm256_load_ps(pa);
    __m256 vb = _mm256_mul_ps(va, vs);
    _mm256_store_ps(pb, vb);
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

#elif defined(WCN_X86_SSE2)
  __m128 vs = _mm_set1_ps(scalar);
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 64);
    __m128 va0 = _mm_load_ps(pa);
    __m128 vb0 = _mm_mul_ps(va0, vs);
    _mm_store_ps(pb, vb0);
    __m128 va1 = _mm_load_ps(pa + 4);
    __m128 vb1 = _mm_mul_ps(va1, vs);
    _mm_store_ps(pb + 4, vb1);
    pa += 8;
    pb += 8;
  }
  for (; i + 4 <= count; i += 4) {
    __m128 va = _mm_load_ps(pa);
    __m128 vb = _mm_mul_ps(va, vs);
    _mm_store_ps(pb, vb);
    pa += 4;
    pb += 4;
  }

#elif defined(WCN_ARM_NEON)
  float32x4_t vs = vdupq_n_f32(scalar);
  for (; i + 8 <= count; i += 8) {
    __builtin_prefetch(pa + 64);
    float32x4_t va0 = vld1q_f32(pa);
    float32x4_t vb0 = vmulq_f32(va0, vs);
    vst1q_f32(pb, vb0);
    float32x4_t va1 = vld1q_f32(pa + 4);
    float32x4_t vb1 = vmulq_f32(va1, vs);
    vst1q_f32(pb + 4, vb1);
    pa += 8;
    pb += 8;
  }
  for (; i + 4 <= count; i += 4) {
    float32x4_t va = vld1q_f32(pa);
    float32x4_t vb = vmulq_f32(va, vs);
    vst1q_f32(pb, vb);
    pa += 4;
    pb += 4;
  }

#elif defined(WCN_LOONGARCH_LASX)
  __m256 vs = (__m256)__lasx_xvfreplgr2vr_s(*(int *)&scalar);
  for (; i + 8 <= count; i += 8) {
    __m256 va = (__m256)__lasx_xvld(pa, 0);
    __m256 vb = (__m256)__lasx_xvfmul_s((__m256i)va, (__m256i)vs);
    __lasx_xvst((__m256i)vb, pb, 0);
    pa += 8;
    pb += 8;
  }

#elif defined(WCN_LOONGARCH_LSX)
  __m128 vs = (__m128)__lsx_vfreplgr2vr_s(*(int *)&scalar);
  for (; i + 4 <= count; i += 4) {
    __m128 va = (__m128)__lsx_vld(pa, 0);
    __m128 vb = (__m128)__lsx_vfmul_s((__m128i)va, (__m128i)vs);
    __lsx_vst((__m128i)vb, pb, 0);
    pa += 4;
    pb += 4;
  }

#elif defined(WCN_POWERPC_VSX)
  __vector float vs = vec_splats(scalar);
  for (; i + 4 <= count; i += 4) {
    __vector float va = vec_ld(0, pa);
    __vector float vb = vec_mul(va, vs);
    vec_st(vb, 0, pb);
    pa += 4;
    pb += 4;
  }

#elif defined(WCN_POWERPC_ALTIVEC)
  __vector float vs = vec_splats(scalar);
  for (; i + 4 <= count; i += 4) {
    __vector float va = vec_ld(0, pa);
    __vector float vb = vec_mul(va, vs);
    vec_st(vb, 0, pb);
    pa += 4;
    pb += 4;
  }

#elif defined(WCN_MIPS_MSA)
  v4f32 vs = __msa_fill_w(*(int *)&scalar);
  for (; i + 4 <= count; i += 4) {
    v4f32 va = __msa_ld_w(pa, 0);
    v4f32 vb = __msa_fmul_w(va, vs);
    __msa_st_w((v4i32)vb, pb, 0);
    pa += 4;
    pb += 4;
  }

#elif defined(WCN_RISCV_RVV)
  size_t vl = __riscv_vsetvl_e32m1(count);
  vfloat32m1_t vs = __riscv_vfmv_v_f_f32m1(scalar, vl);
  for (; i + vl <= count; i += vl) {
    vl = __riscv_vsetvl_e32m1(count - i);
    vfloat32m1_t va = __riscv_vle32_v_f32m1(pa, vl);
    vfloat32m1_t vb = __riscv_vfmul_vv_f32m1(va, vs, vl);
    __riscv_vse32_v_f32m1(pb, vb, vl);
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

#if defined(WCN_X86_AVX512F)
  if (count >= 16) {
    __m512 max_vec = _mm512_set1_ps(data[0]);
    for (; i + 16 <= count; i += 16) {
      __m512 v = _mm512_loadu_ps(data + i);
      max_vec = _mm512_max_ps(max_vec, v);
    }
    max_val = _mm512_reduce_max_ps(max_vec);
  }

#elif defined(WCN_X86_AVX2)
  if (count >= 8) {
    __m256 max_vec = _mm256_set1_ps(data[0]);
    for (; i + 8 <= count; i += 8) {
      __m256 v = _mm256_loadu_ps(data + i);
      max_vec = _mm256_max_ps(max_vec, v);
    }
    __m128 lo = _mm256_castps256_ps128(max_vec);
    __m128 hi = _mm256_extractf128_ps(max_vec, 1);
    __m128 m = _mm_max_ps(lo, hi);
    m = _mm_max_ps(m, _mm_movehl_ps(m, m));
    m = _mm_max_ps(m, _mm_shuffle_ps(m, m, 1));
    max_val = _mm_cvtss_f32(m);
  }

#elif defined(WCN_X86_SSE2)
  if (count >= 4) {
    __m128 max_vec = _mm_load_ss(&data[0]);
    max_vec = _mm_shuffle_ps(max_vec, max_vec, 0);
    for (; i + 4 <= count; i += 4) {
      __m128 v = _mm_loadu_ps(data + i);
      max_vec = _mm_max_ps(max_vec, v);
    }
    max_vec = _mm_max_ps(max_vec, _mm_movehl_ps(max_vec, max_vec));
    max_vec = _mm_max_ps(max_vec, _mm_shuffle_ps(max_vec, max_vec, 1));
    max_val = _mm_cvtss_f32(max_vec);
  }

#elif defined(WCN_ARM_NEON)
  if (count >= 4) {
    float32x4_t max_vec = vld1q_dup_f32(&data[0]);
    for (; i + 4 <= count; i += 4) {
      float32x4_t v = vld1q_f32(data + i);
      max_vec = vmaxq_f32(max_vec, v);
    }
    max_val = vmaxvq_f32(max_vec);
  }

#elif defined(WCN_WASM_SIMD128)
  if (count >= 4) {
    v128_t max_vec = wasm_f32x4_splat(data[0]);
    for (; i + 4 <= count; i += 4) {
      v128_t v = wasm_v128_load(data + i);
      max_vec = wasm_f32x4_max(max_vec, v);
    }
    float temp[4];
    wasm_v128_store(temp, max_vec);
    max_val = temp[0];
    for (int j = 1; j < 4; j++) {
      if (temp[j] > max_val)
        max_val = temp[j];
    }
  }

#elif defined(WCN_RISCV_RVV)
  if (count > 1) {
    size_t vl = __riscv_vsetvl_e32m1(count - i);
    vfloat32m1_t max_vec = __riscv_vfmv_v_f_f32m1(data[0], vl);
    for (; i < count;) {
      vl = __riscv_vsetvl_e32m1(count - i);
      vfloat32m1_t v = __riscv_vle32_v_f32m1(data + i, vl);
      max_vec = __riscv_vfmax_vv_f32m1(max_vec, v, vl);
      i += vl;
    }
    vfloat32m1_t red = __riscv_vfredmax_vs_f32m1_f32m1(
        max_vec, vfmv_s_f_f32m1(data[0], 1), vl);
    max_val = vfmv_f_s_f32m1_f32(red);
    return max_val;
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

#if defined(WCN_X86_AVX512F)
  if (count >= 16) {
    __m512 min_vec = _mm512_set1_ps(data[0]);
    for (; i + 16 <= count; i += 16) {
      __m512 v = _mm512_loadu_ps(data + i);
      min_vec = _mm512_min_ps(min_vec, v);
    }
    min_val = _mm512_reduce_min_ps(min_vec);
  }

#elif defined(WCN_X86_AVX2)
  if (count >= 8) {
    __m256 min_vec = _mm256_set1_ps(data[0]);
    for (; i + 8 <= count; i += 8) {
      __m256 v = _mm256_loadu_ps(data + i);
      min_vec = _mm256_min_ps(min_vec, v);
    }
    __m128 lo = _mm256_castps256_ps128(min_vec);
    __m128 hi = _mm256_extractf128_ps(min_vec, 1);
    __m128 m = _mm_min_ps(lo, hi);
    m = _mm_min_ps(m, _mm_movehl_ps(m, m));
    m = _mm_min_ps(m, _mm_shuffle_ps(m, m, 1));
    min_val = _mm_cvtss_f32(m);
  }

#elif defined(WCN_X86_SSE2)
  if (count >= 4) {
    __m128 min_vec = _mm_load_ss(&data[0]);
    min_vec = _mm_shuffle_ps(min_vec, min_vec, 0);
    for (; i + 4 <= count; i += 4) {
      __m128 v = _mm_loadu_ps(data + i);
      min_vec = _mm_min_ps(min_vec, v);
    }
    min_vec = _mm_min_ps(min_vec, _mm_movehl_ps(min_vec, min_vec));
    min_vec = _mm_min_ps(min_vec, _mm_shuffle_ps(min_vec, min_vec, 1));
    min_val = _mm_cvtss_f32(min_vec);
  }

#elif defined(WCN_ARM_NEON)
  if (count >= 4) {
    float32x4_t min_vec = vld1q_dup_f32(&data[0]);
    for (; i + 4 <= count; i += 4) {
      float32x4_t v = vld1q_f32(data + i);
      min_vec = vminq_f32(min_vec, v);
    }
    min_val = vminvq_f32(min_vec);
  }

#elif defined(WCN_WASM_SIMD128)
  if (count >= 4) {
    v128_t min_vec = wasm_f32x4_splat(data[0]);
    for (; i + 4 <= count; i += 4) {
      v128_t v = wasm_v128_load(data + i);
      min_vec = wasm_f32x4_min(min_vec, v);
    }
    float temp[4];
    wasm_v128_store(temp, min_vec);
    min_val = temp[0];
    for (int j = 1; j < 4; j++) {
      if (temp[j] < min_val)
        min_val = temp[j];
    }
  }

#elif defined(WCN_RISCV_RVV)
  if (count > 1) {
    size_t vl = __riscv_vsetvl_e32m1(count - i);
    vfloat32m1_t min_vec = __riscv_vfmv_v_f_f32m1(data[0], vl);
    for (; i < count;) {
      vl = __riscv_vsetvl_e32m1(count - i);
      vfloat32m1_t v = __riscv_vle32_v_f32m1(data + i, vl);
      min_vec = __riscv_vfmin_vv_f32m1(min_vec, v, vl);
      i += vl;
    }
    vfloat32m1_t red = __riscv_vfredmin_vs_f32m1_f32m1(
        min_vec, vfmv_s_f_f32m1(data[0], 1), vl);
    min_val = vfmv_f_s_f32m1_f32(red);
    return min_val;
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

#if defined(WCN_X86_AVX512F)
  __m512 sum_vec = _mm512_setzero_ps();
  for (; i + 16 <= count; i += 16) {
    __m512 v = _mm512_loadu_ps(data + i);
    sum_vec = _mm512_add_ps(sum_vec, v);
  }
  sum = _mm512_reduce_add_ps(sum_vec);

#elif defined(WCN_X86_AVX2)
  __m256 sum_vec = _mm256_setzero_ps();
  for (; i + 8 <= count; i += 8) {
    __m256 v = _mm256_loadu_ps(data + i);
    sum_vec = _mm256_add_ps(sum_vec, v);
  }
  float temp[8];
  _mm256_storeu_ps(temp, sum_vec);
  for (int j = 0; j < 8; j++) {
    sum += temp[j];
  }

#elif defined(WCN_X86_SSE2)
  __m128 sum_vec = _mm_setzero_ps();
  for (; i + 4 <= count; i += 4) {
    __m128 v = _mm_loadu_ps(data + i);
    sum_vec = _mm_add_ps(sum_vec, v);
  }
  sum_vec = _mm_add_ps(sum_vec, _mm_movehl_ps(sum_vec, sum_vec));
  sum_vec = _mm_add_ps(sum_vec, _mm_shuffle_ps(sum_vec, sum_vec, 1));
  sum = _mm_cvtss_f32(sum_vec);

#elif defined(WCN_ARM_NEON)
  float32x4_t sum_vec = vdupq_n_f32(0.0f);
  for (; i + 4 <= count; i += 4) {
    float32x4_t v = vld1q_f32(data + i);
    sum_vec = vaddq_f32(sum_vec, v);
  }
  sum = vaddvq_f32(sum_vec);

#elif defined(WCN_WASM_SIMD128)
  v128_t sum_vec = wasm_f32x4_splat(0.0f);
  for (; i + 4 <= count; i += 4) {
    v128_t v = wasm_v128_load(data + i);
    sum_vec = wasm_f32x4_add(sum_vec, v);
  }
  float temp[4];
  wasm_v128_store(temp, sum_vec);
  for (int j = 0; j < 4; j++) {
    sum += temp[j];
  }

#elif defined(WCN_RISCV_RVV)
  size_t vl = __riscv_vsetvl_e32m1(count);
  vfloat32m1_t sum_vec = __riscv_vfmv_v_f_f32m1(0.0f, vl);
  for (; i < count;) {
    vl = __riscv_vsetvl_e32m1(count - i);
    vfloat32m1_t v = __riscv_vle32_v_f32m1(data + i, vl);
    sum_vec = __riscv_vfadd_vv_f32m1(sum_vec, v, vl);
    i += vl;
  }
  vfloat32m1_t red =
      __riscv_vfredsum_vs_f32m1_f32m1(sum_vec, vfmv_s_f_f32m1(0.0f, 1), vl);
  sum = vfmv_f_s_f32m1_f32(red);
  return sum;
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
