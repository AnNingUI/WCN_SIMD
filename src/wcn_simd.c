#include "WCN_SIMD.h"
#include <string.h>
#include <stdlib.h>

/* Global feature detection result */
static wcn_simd_features_t g_features = {0};
static int g_initialized = 0;

/* ========== Feature Detection ========== */

#ifdef WCN_ARCH_X86

#ifdef _MSC_VER
#include <intrin.h>

static void cpuid(int info[4], int function_id) {
    __cpuid(info, function_id);
}

static void cpuidex(int info[4], int function_id, int subfunction_id) {
    __cpuidex(info, function_id, subfunction_id);
}
#else
#include <cpuid.h>

static void cpuid(int info[4], int function_id) {
    __get_cpuid(function_id, (unsigned int*)&info[0], (unsigned int*)&info[1],
                (unsigned int*)&info[2], (unsigned int*)&info[3]);
}

static void cpuidex(int info[4], int function_id, int subfunction_id) {
    __get_cpuid_count(function_id, subfunction_id, (unsigned int*)&info[0],
                      (unsigned int*)&info[1], (unsigned int*)&info[2], (unsigned int*)&info[3]);
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
#include <sys/auxv.h>
#include <asm/hwcap.h>

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
const wcn_simd_features_t* wcn_simd_get_features(void) {
    if (!g_initialized) {
        wcn_simd_init();
    }
    return &g_features;
}

/* ========== Common Algorithms Implementation ========== */

WCN_API_EXPORT
float wcn_simd_dot_product_f32(const float* a, const float* b, size_t count) {
    float sum = 0.0f;
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

    #elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON) || defined(WCN_WASM_SIMD128)
    wcn_v128f_t sum_vec = wcn_v128f_setzero();
    for (; i + 4 <= count; i += 4) {
        wcn_v128f_t va = wcn_v128f_load(a + i);
        wcn_v128f_t vb = wcn_v128f_load(b + i);
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
void wcn_simd_add_array_f32(const float* a, const float* b, float* c, size_t count) {
    size_t i = 0;

    #if defined(WCN_X86_AVX2)
    for (; i + 8 <= count; i += 8) {
        wcn_v256f_t va = wcn_v256f_load(a + i);
        wcn_v256f_t vb = wcn_v256f_load(b + i);
        wcn_v256f_t vc = wcn_v256f_add(va, vb);
        wcn_v256f_store(c + i, vc);
    }

    #elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON) || defined(WCN_WASM_SIMD128)
    for (; i + 4 <= count; i += 4) {
        wcn_v128f_t va = wcn_v128f_load(a + i);
        wcn_v128f_t vb = wcn_v128f_load(b + i);
        wcn_v128f_t vc = wcn_v128f_add(va, vb);
        wcn_v128f_store(c + i, vc);
    }

    #elif defined(WCN_RISCV_RVV)
    size_t vl = wcn_rvv_vsetvl_f32(count);
    for (; i + vl <= count; i += vl) {
        vl = wcn_rvv_vsetvl_f32(count - i);
        wcn_vscalable_f32_t va = wcn_vscalable_f32_load(a + i, vl);
        wcn_vscalable_f32_t vb = wcn_vscalable_f32_load(b + i, vl);
        wcn_vscalable_f32_t vc = wcn_vscalable_f32_add(va, vb, vl);
        wcn_vscalable_f32_store(c + i, vc, vl);
    }
    #endif

    /* Scalar tail */
    for (; i < count; i++) {
        c[i] = a[i] + b[i];
    }
}

WCN_API_EXPORT
void wcn_simd_mul_array_f32(const float* a, const float* b, float* c, size_t count) {
    size_t i = 0;

    #if defined(WCN_X86_AVX2)
    for (; i + 8 <= count; i += 8) {
        wcn_v256f_t va = wcn_v256f_load(a + i);
        wcn_v256f_t vb = wcn_v256f_load(b + i);
        wcn_v256f_t vc = wcn_v256f_mul(va, vb);
        wcn_v256f_store(c + i, vc);
    }

    #elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON) || defined(WCN_WASM_SIMD128)
    for (; i + 4 <= count; i += 4) {
        wcn_v128f_t va = wcn_v128f_load(a + i);
        wcn_v128f_t vb = wcn_v128f_load(b + i);
        wcn_v128f_t vc = wcn_v128f_mul(va, vb);
        wcn_v128f_store(c + i, vc);
    }

    #elif defined(WCN_RISCV_RVV)
    size_t vl = wcn_rvv_vsetvl_f32(count);
    for (; i + vl <= count; i += vl) {
        vl = wcn_rvv_vsetvl_f32(count - i);
        wcn_vscalable_f32_t va = wcn_vscalable_f32_load(a + i, vl);
        wcn_vscalable_f32_t vb = wcn_vscalable_f32_load(b + i, vl);
        wcn_vscalable_f32_t vc = wcn_vscalable_f32_mul(va, vb, vl);
        wcn_vscalable_f32_store(c + i, vc, vl);
    }
    #endif

    /* Scalar tail */
    for (; i < count; i++) {
        c[i] = a[i] * b[i];
    }
}

WCN_API_EXPORT
void wcn_simd_scale_array_f32(const float* a, float scalar, float* b, size_t count) {
    size_t i = 0;

    #if defined(WCN_X86_AVX2)
    wcn_v256f_t vs = wcn_v256f_set1(scalar);
    for (; i + 8 <= count; i += 8) {
        wcn_v256f_t va = wcn_v256f_load(a + i);
        wcn_v256f_t vb = wcn_v256f_mul(va, vs);
        wcn_v256f_store(b + i, vb);
    }

    #elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON) || defined(WCN_WASM_SIMD128)
    wcn_v128f_t vs = wcn_v128f_set1(scalar);
    for (; i + 4 <= count; i += 4) {
        wcn_v128f_t va = wcn_v128f_load(a + i);
        wcn_v128f_t vb = wcn_v128f_mul(va, vs);
        wcn_v128f_store(b + i, vb);
    }

    #elif defined(WCN_RISCV_RVV)
    size_t vl = wcn_rvv_vsetvl_f32(count);
    wcn_vscalable_f32_t vs = wcn_vscalable_f32_set1(scalar, vl);
    for (; i + vl <= count; i += vl) {
        vl = wcn_rvv_vsetvl_f32(count - i);
        wcn_vscalable_f32_t va = wcn_vscalable_f32_load(a + i, vl);
        wcn_vscalable_f32_t vb = wcn_vscalable_f32_mul(va, vs, vl);
        wcn_vscalable_f32_store(b + i, vb, vl);
    }
    #endif

    /* Scalar tail */
    for (; i < count; i++) {
        b[i] = a[i] * scalar;
    }
}

WCN_API_EXPORT
void wcn_simd_fmadd_array_f32(const float* a, const float* b, float* c, size_t count) {
    size_t i = 0;

    #if defined(WCN_X86_AVX2)
    for (; i + 8 <= count; i += 8) {
        wcn_v256f_t va = wcn_v256f_load(a + i);
        wcn_v256f_t vb = wcn_v256f_load(b + i);
        wcn_v256f_t vc = wcn_v256f_load(c + i);
        vc = wcn_v256f_fmadd(va, vb, vc);
        wcn_v256f_store(c + i, vc);
    }

    #elif (defined(WCN_ARM_NEON) && defined(WCN_ARM_AARCH64))
    for (; i + 4 <= count; i += 4) {
        wcn_v128f_t va = wcn_v128f_load(a + i);
        wcn_v128f_t vb = wcn_v128f_load(b + i);
        wcn_v128f_t vc = wcn_v128f_load(c + i);
        vc = wcn_v128f_fmadd(va, vb, vc);
        wcn_v128f_store(c + i, vc);
    }

    #elif defined(WCN_RISCV_RVV)
    size_t vl = wcn_rvv_vsetvl_f32(count);
    for (; i + vl <= count; i += vl) {
        vl = wcn_rvv_vsetvl_f32(count - i);
        wcn_vscalable_f32_t va = wcn_vscalable_f32_load(a + i, vl);
        wcn_vscalable_f32_t vb = wcn_vscalable_f32_load(b + i, vl);
        wcn_vscalable_f32_t vc = wcn_vscalable_f32_load(c + i, vl);
        vc = wcn_vscalable_f32_fmadd(va, vb, vc, vl);
        wcn_vscalable_f32_store(c + i, vc, vl);
    }

    #else
    /* Fallback: manual FMA */
    for (; i < count; i++) {
        c[i] = a[i] * b[i] + c[i];
    }
    return;
    #endif

    /* Scalar tail */
    for (; i < count; i++) {
        c[i] = a[i] * b[i] + c[i];
    }
}

WCN_API_EXPORT
float wcn_simd_reduce_max_f32(const float* data, size_t count) {
    if (count == 0) return 0.0f;

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
            if (temp[j] > max_val) max_val = temp[j];
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
float wcn_simd_reduce_min_f32(const float* data, size_t count) {
    if (count == 0) return 0.0f;

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
            if (temp[j] < min_val) min_val = temp[j];
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
float wcn_simd_reduce_sum_f32(const float* data, size_t count) {
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

    #elif defined(WCN_X86_SSE2) || defined(WCN_ARM_NEON) || defined(WCN_WASM_SIMD128)
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
void wcn_simd_memcpy_aligned(void* dest, const void* src, size_t bytes) {
    memcpy(dest, src, bytes);
}

WCN_API_EXPORT
void wcn_simd_memset_aligned(void* dest, int value, size_t bytes) {
    memset(dest, value, bytes);
}

WCN_API_EXPORT
void* wcn_alloc(size_t bytes) {
    return malloc(bytes);
}

WCN_API_EXPORT
void wcn_free(void* ptr) {
    free(ptr);
}
