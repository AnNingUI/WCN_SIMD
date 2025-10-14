#!/usr/bin/env node

const fs = require('fs');
const path = require('path');

// Unified API operations that all platforms should implement
const unifiedOps = [
    // Load/Store
    'wcn_v128i_load', 'wcn_v128i_load_aligned', 'wcn_v128i_store', 'wcn_v128i_store_aligned',
    'wcn_v128f_load', 'wcn_v128f_load_aligned', 'wcn_v128f_store', 'wcn_v128f_store_aligned',
    'wcn_v128d_load', 'wcn_v128d_load_aligned', 'wcn_v128d_store', 'wcn_v128d_store_aligned',

    // Initialization
    'wcn_v128i_set1_i8', 'wcn_v128i_set1_i16', 'wcn_v128i_set1_i32', 'wcn_v128i_set1_i64',
    'wcn_v128i_setzero', 'wcn_v128f_set1', 'wcn_v128f_setzero',
    'wcn_v128d_set1', 'wcn_v128d_setzero',

    // Arithmetic
    'wcn_v128i_add_i8', 'wcn_v128i_sub_i8',
    'wcn_v128i_add_i16', 'wcn_v128i_sub_i16',
    'wcn_v128i_add_i32', 'wcn_v128i_sub_i32',
    'wcn_v128i_add_i64', 'wcn_v128i_sub_i64',
    'wcn_v128f_add', 'wcn_v128f_sub', 'wcn_v128f_mul', 'wcn_v128f_div',
    'wcn_v128d_add', 'wcn_v128d_sub', 'wcn_v128d_mul', 'wcn_v128d_div',

    // Saturating Arithmetic
    'wcn_v128i_adds_i8', 'wcn_v128i_subs_i8', 'wcn_v128i_adds_u8', 'wcn_v128i_subs_u8',
    'wcn_v128i_adds_i16', 'wcn_v128i_subs_i16', 'wcn_v128i_adds_u16', 'wcn_v128i_subs_u16',

    // FMA
    'wcn_v128f_fmadd', 'wcn_v128d_fmadd',

    // Logical
    'wcn_v128i_and', 'wcn_v128i_or', 'wcn_v128i_xor', 'wcn_v128i_andnot',

    // Shift
    'wcn_v128i_sll_i16', 'wcn_v128i_srl_i16', 'wcn_v128i_sra_i16',
    'wcn_v128i_slli_i16', 'wcn_v128i_srli_i16', 'wcn_v128i_srai_i16',
    'wcn_v128i_sll_i32', 'wcn_v128i_srl_i32', 'wcn_v128i_sra_i32',
    'wcn_v128i_slli_i32', 'wcn_v128i_srli_i32', 'wcn_v128i_srai_i32',
    'wcn_v128i_sll_i64', 'wcn_v128i_srl_i64',
    'wcn_v128i_slli_i64', 'wcn_v128i_srli_i64',
    'wcn_v128i_slli_si128', 'wcn_v128i_srli_si128',

    // Comparison
    'wcn_v128i_cmpeq_i8', 'wcn_v128i_cmpgt_i8', 'wcn_v128i_cmplt_i8',
    'wcn_v128i_cmpeq_i16', 'wcn_v128i_cmpgt_i16', 'wcn_v128i_cmplt_i16',
    'wcn_v128i_cmpeq_i32', 'wcn_v128i_cmpgt_i32', 'wcn_v128i_cmplt_i32',
    'wcn_v128f_cmpeq', 'wcn_v128f_cmplt', 'wcn_v128f_cmple', 'wcn_v128f_cmpgt', 'wcn_v128f_cmpge',
    'wcn_v128d_cmpeq', 'wcn_v128d_cmplt', 'wcn_v128d_cmple', 'wcn_v128d_cmpgt', 'wcn_v128d_cmpge', 'wcn_v128d_cmpneq',

    // Min/Max
    'wcn_v128i_min_i8', 'wcn_v128i_max_i8', 'wcn_v128i_min_u8', 'wcn_v128i_max_u8',
    'wcn_v128i_min_i16', 'wcn_v128i_max_i16',
    'wcn_v128i_min_i32', 'wcn_v128i_max_i32',
    'wcn_v128f_max', 'wcn_v128f_min',
    'wcn_v128d_max', 'wcn_v128d_min',

    // Square Root
    'wcn_v128f_sqrt', 'wcn_v128d_sqrt',

    // Reciprocal
    'wcn_v128f_rcp', 'wcn_v128f_rsqrt',

    // Rounding
    'wcn_v128f_floor', 'wcn_v128f_ceil', 'wcn_v128f_round_nearest', 'wcn_v128f_trunc',
    'wcn_v128d_floor', 'wcn_v128d_ceil', 'wcn_v128d_round_nearest', 'wcn_v128d_trunc',

    // Absolute Value
    'wcn_v128i_abs_i8', 'wcn_v128i_abs_i16', 'wcn_v128i_abs_i32',
    'wcn_v128f_abs', 'wcn_v128d_abs',

    // Negation
    'wcn_v128i_neg_i8', 'wcn_v128i_neg_i16', 'wcn_v128i_neg_i32', 'wcn_v128i_neg_i64',
    'wcn_v128f_neg', 'wcn_v128d_neg',

    // Broadcast
    'wcn_v128i_broadcast_i32', 'wcn_v128f_broadcast', 'wcn_v128d_broadcast',

    // Integer Multiplication
    'wcn_v128i_mullo_i16', 'wcn_v128i_mulhi_i16', 'wcn_v128i_mulhi_u16',
    'wcn_v128i_mullo_i32', 'wcn_v128i_mul_u32',

    // Pack/Unpack
    'wcn_v128i_packs_i32', 'wcn_v128i_packs_i16', 'wcn_v128i_packus_i16', 'wcn_v128i_packus_i32',
    'wcn_v128i_unpacklo_i8', 'wcn_v128i_unpackhi_i8',
    'wcn_v128i_unpacklo_i16', 'wcn_v128i_unpackhi_i16',
    'wcn_v128i_unpacklo_i32', 'wcn_v128i_unpackhi_i32',
    'wcn_v128i_unpacklo_i64', 'wcn_v128i_unpackhi_i64',
    'wcn_v128f_unpacklo', 'wcn_v128f_unpackhi',
    'wcn_v128d_unpacklo', 'wcn_v128d_unpackhi',

    // Extract/Insert
    'wcn_v128i_extract_i16', 'wcn_v128i_insert_i16',

    // Movemask
    'wcn_v128i_movemask_i8', 'wcn_v128f_movemask', 'wcn_v128d_movemask',

    // Blend
    'wcn_v128f_blendv', 'wcn_v128d_blendv', 'wcn_v128i_blendv_i8',

    // Byte Shuffle
    'wcn_v128i_shuffle_i8',

    // Conversions
    'wcn_v128i_to_v128f', 'wcn_v128f_to_v128i'
];

// Platform files to check
const platforms = [
    { name: 'x86/SSE2', file: 'include/wcn_simd/platform/x86/wcn_x86_sse2.h', guard: 'WCN_X86_SSE2' },
    { name: 'x86/SSE3', file: 'include/wcn_simd/platform/x86/wcn_x86_sse3.h', guard: 'WCN_X86_SSE3' },
    { name: 'x86/SSSE3', file: 'include/wcn_simd/platform/x86/wcn_x86_ssse3.h', guard: 'WCN_X86_SSSE3' },
    { name: 'x86/SSE4.1', file: 'include/wcn_simd/platform/x86/wcn_x86_sse4_1.h', guard: 'WCN_X86_SSE4_1' },
    { name: 'ARM/NEON', file: 'include/wcn_simd/platform/arm/wcn_arm_neon.h', guard: 'WCN_ARM_NEON' },
    { name: 'WASM/SIMD128', file: 'include/wcn_simd/platform/wasm/wcn_wasm_simd128.h', guard: 'WCN_WASM_SIMD128' }
];

function checkPlatform(platformInfo) {
    const filePath = path.join(__dirname, '..', platformInfo.file);

    if (!fs.existsSync(filePath)) {
        return { platform: platformInfo.name, missing: unifiedOps, error: 'File not found' };
    }

    const content = fs.readFileSync(filePath, 'utf-8');
    const missing = [];

    for (const op of unifiedOps) {
        // Check for function definition or macro definition
        const funcRegex = new RegExp(`WCN_INLINE\\s+\\w+\\s+${op}\\s*\\(`, 'm');
        const macroRegex = new RegExp(`#define\\s+${op}\\s*\\(`, 'm');

        if (!funcRegex.test(content) && !macroRegex.test(content)) {
            missing.push(op);
        }
    }

    return {
        platform: platformInfo.name,
        total: unifiedOps.length,
        implemented: unifiedOps.length - missing.length,
        missing: missing,
        coverage: ((unifiedOps.length - missing.length) / unifiedOps.length * 100).toFixed(2)
    };
}

console.log('='.repeat(80));
console.log('WCN_SIMD Platform Implementation Verification');
console.log('='.repeat(80));
console.log();

const results = [];

for (const platform of platforms) {
    const result = checkPlatform(platform);
    results.push(result);

    console.log(`Platform: ${result.platform}`);
    console.log(`Coverage: ${result.implemented}/${result.total} (${result.coverage}%)`);

    if (result.missing.length > 0) {
        console.log(`Missing operations (${result.missing.length}):`);
        result.missing.forEach(op => console.log(`  - ${op}`));
    } else {
        console.log('✓ All operations implemented!');
    }
    console.log();
}

console.log('='.repeat(80));
console.log('Summary');
console.log('='.repeat(80));

const avgCoverage = results.reduce((sum, r) => sum + parseFloat(r.coverage), 0) / results.length;
console.log(`Average coverage: ${avgCoverage.toFixed(2)}%`);

const fullyCovered = results.filter(r => r.missing.length === 0);
console.log(`Fully implemented platforms: ${fullyCovered.length}/${results.length}`);

if (fullyCovered.length > 0) {
    console.log('Platforms with 100% coverage:');
    fullyCovered.forEach(r => console.log(`  ✓ ${r.platform}`));
}

const incomplete = results.filter(r => r.missing.length > 0);
if (incomplete.length > 0) {
    console.log('\nPlatforms needing attention:');
    incomplete.forEach(r => {
        console.log(`  ! ${r.platform}: ${r.missing.length} missing (${r.coverage}% coverage)`);
    });
}
