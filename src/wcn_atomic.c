#include "WCN_SIMD.h"
#include "wcn_simd/wcn_atomic.h"
#include <string.h>

/* Global atomic feature detection result */
static wcn_atomic_features_t g_atomic_features = {0};
static int g_atomic_initialized = 0;

/* ========== Atomic Feature Detection ========== */

void wcn_atomic_init(void) {
    if (g_atomic_initialized) {
        return;
    }

    memset(&g_atomic_features, 0, sizeof(g_atomic_features));

    /* Detect platform-specific atomic capabilities */
    #ifdef WCN_ARCH_X86
        g_atomic_features.has_sse_atomic = 1;
        #ifdef WCN_X86_AVX2
        g_atomic_features.has_avx_atomic = 1;
        #endif
        #ifdef WCN_X86_AVX512F
        g_atomic_features.has_avx512_atomic = 1;
        #endif
    #endif

    #ifdef WCN_ARCH_ARM
        g_atomic_features.has_neon_atomic = 1;
        #ifdef WCN_ARM_SVE
        g_atomic_features.has_sve_atomic = 1;
        #endif
    #endif

    #ifdef WCN_LOONGARCH_LSX
        g_atomic_features.has_lsx_atomic = 1;
        #ifdef WCN_LOONGARCH_LASX
        g_atomic_features.has_lasx_atomic = 1;
        #endif
    #endif

    #ifdef WCN_RISCV_RVV
        g_atomic_features.has_rvv_atomic = 1;
    #endif

    #ifdef WCN_POWERPC_ALTIVEC
        g_atomic_features.has_altivec_atomic = 1;
        #ifdef WCN_POWERPC_VSX
        g_atomic_features.has_vsx_atomic = 1;
        #endif
    #endif

    #ifdef WCN_MIPS_MSA
        g_atomic_features.has_msa_atomic = 1;
    #endif

    #ifdef WCN_WASM_SIMD128
        g_atomic_features.has_wasm_atomic = 1;
        #ifdef WCN_WASM_ATOMIC
        g_atomic_features.has_wasm_atomic = 1;
        #endif
    #endif

    /* Generic atomic support */
    #if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
        g_atomic_features.has_atomic_operations = 1;
    #endif

    g_atomic_initialized = 1;
}

const wcn_atomic_features_t* wcn_atomic_get_features(void) {
    if (!g_atomic_initialized) {
        wcn_atomic_init();
    }
    return &g_atomic_features;
}