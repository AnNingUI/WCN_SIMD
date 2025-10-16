/**
 * WCN_SIMD TypeScript Wrapper - High Performance Version
 * Optimized for minimal runtime overhead and maximum performance
 */

/* ========== Type Definitions ========== */

/**
 * 128-bit SIMD vector handle (opaque type for better optimization)
 */
export type V128 = number;

// typedef struct {
//     /* x86 features */
//     int has_sse2;
//     int has_sse3;
//     int has_ssse3;
//     int has_sse4_1;
//     int has_sse4_2;
//     int has_avx;
//     int has_avx2;
//     int has_avx512f;
//     int has_avx512bw;
//     int has_avx512dq;
//     int has_avx512vl;
//     int has_fma;

//     /* ARM features */
//     int has_neon;
//     int has_sve;
//     int has_sve2;

//     /* LoongArch features */
//     int has_lsx;
//     int has_lasx;

//     /* RISC-V features */
//     int has_rvv;

//     /* PowerPC features */
//     int has_altivec;
//     int has_vsx;

//     /* MIPS features */
//     int has_msa;

//     /* WebAssembly features */
//     int has_wasm_simd128;

//     /* Atomic operation features */
//     int has_atomic_operations;
//     int has_gcc_atomic;
//     int has_msvc_atomic;
// } wcn_simd_features_t;
/**
 * SIMD feature detection results
 */
export interface SimdFeatures {
  has_sse2: boolean;
  has_sse3: boolean;
  has_ssse3: boolean;
  has_sse4_1: boolean;
  has_sse4_2: boolean;
  has_avx: boolean;
  has_avx2: boolean;
  has_avx512f: boolean;
  has_avx512bw: boolean;
  has_avx512dq: boolean;
  has_avx512vl: boolean;
  has_fma: boolean;
  has_neon: boolean;
  has_sve: boolean;
  has_sve2: boolean;
  has_lsx: boolean;
  has_lasx: boolean;
  has_rvv: boolean;
  has_altivec: boolean;
  has_vsx: boolean;
  has_msa: boolean;
  has_wasm_simd128: boolean;
  has_atomic_operations: boolean;
  has_gcc_atomic: boolean;
  has_msvc_atomic: boolean;
}

/**
 * Memory manager for efficient WASM memory operations
 */
export interface MemoryManager {
  alloc(size: number): number;
  free(ptr: number): void;
  realloc(ptr: number, newSize: number): number;
}

/**!
emscripten_stack_get_current: ƒ $emscripten_stack_get_current()
memory: Memory(258)
wcn_alloc: ƒ $wcn_alloc()
wcn_free: ƒ $wcn_free()
wcn_simd_add_array_f32: ƒ $wcn_simd_add_array_f32()
wcn_simd_dot_product_f32: ƒ $wcn_simd_dot_product_f32()
wcn_simd_fmadd_array_f32: ƒ $wcn_simd_fmadd_array_f32()
wcn_simd_get_features: ƒ $wcn_simd_get_features()
wcn_simd_init: ƒ $wcn_simd_init()
wcn_simd_memcpy_aligned: ƒ $wcn_simd_memcpy_aligned()
wcn_simd_memset_aligned: ƒ $wcn_simd_memset_aligned()
wcn_simd_mul_array_f32: ƒ $wcn_simd_mul_array_f32()
wcn_simd_reduce_max_f32: ƒ $wcn_simd_reduce_max_f32()
wcn_simd_reduce_min_f32: ƒ $wcn_simd_reduce_min_f32()
wcn_simd_reduce_sum_f32: ƒ $wcn_simd_reduce_sum_f32()
wcn_simd_scale_array_f32: ƒ $wcn_simd_scale_array_f32()
wcn_wasm_v128f_abs: ƒ $wcn_wasm_v128f_abs()
wcn_wasm_v128f_add: ƒ $wcn_wasm_v128f_add()
wcn_wasm_v128f_ceil: ƒ $wcn_wasm_v128f_ceil()
wcn_wasm_v128f_cmpeq: ƒ $wcn_wasm_v128f_cmpeq()
wcn_wasm_v128f_cmpge: ƒ $wcn_wasm_v128f_cmpge()
wcn_wasm_v128f_cmpgt: ƒ $wcn_wasm_v128f_cmpgt()
wcn_wasm_v128f_cmple: ƒ $wcn_wasm_v128f_cmple()
wcn_wasm_v128f_cmplt: ƒ $wcn_wasm_v128f_cmplt()
wcn_wasm_v128f_div: ƒ $wcn_wasm_v128f_div()
wcn_wasm_v128f_floor: ƒ $wcn_wasm_v128f_floor()
wcn_wasm_v128f_load: ƒ $wcn_wasm_v128i_load()
wcn_wasm_v128f_max: ƒ $wcn_wasm_v128f_max()
wcn_wasm_v128f_min: ƒ $wcn_wasm_v128f_min()
wcn_wasm_v128f_mul: ƒ $wcn_wasm_v128f_mul()
wcn_wasm_v128f_neg: ƒ $wcn_wasm_v128f_neg()
wcn_wasm_v128f_round_nearest: ƒ $wcn_wasm_v128f_round_nearest()
wcn_wasm_v128f_set1: ƒ $wcn_wasm_v128f_set1()
wcn_wasm_v128f_setzero: ƒ $wcn_wasm_v128i_setzero()
wcn_wasm_v128f_sqrt: ƒ $wcn_wasm_v128f_sqrt()
wcn_wasm_v128f_store: ƒ $wcn_wasm_v128i_store()
wcn_wasm_v128f_sub: ƒ $wcn_wasm_v128f_sub()
wcn_wasm_v128f_trunc: ƒ $wcn_wasm_v128f_trunc()
wcn_wasm_v128f_unpackhi: ƒ $wcn_wasm_v128i_unpackhi_i32()
wcn_wasm_v128f_unpacklo: ƒ $wcn_wasm_v128i_unpacklo_i32()
wcn_wasm_v128i_abs_i8: ƒ $wcn_wasm_v128i_abs_i8()
wcn_wasm_v128i_abs_i16: ƒ $wcn_wasm_v128i_abs_i16()
wcn_wasm_v128i_abs_i32: ƒ $wcn_wasm_v128i_abs_i32()
wcn_wasm_v128i_add_i8: ƒ $wcn_wasm_v128i_add_i8()
wcn_wasm_v128i_add_i16: ƒ $wcn_wasm_v128i_add_i16()
wcn_wasm_v128i_add_i32: ƒ $wcn_wasm_v128i_add_i32()
wcn_wasm_v128i_add_i64: ƒ $wcn_wasm_v128i_add_i64()
wcn_wasm_v128i_adds_i8: ƒ $wcn_wasm_v128i_adds_i8()
wcn_wasm_v128i_adds_i16: ƒ $wcn_wasm_v128i_adds_i16()
wcn_wasm_v128i_adds_u8: ƒ $wcn_wasm_v128i_adds_u8()
wcn_wasm_v128i_adds_u16: ƒ $wcn_wasm_v128i_adds_u16()
wcn_wasm_v128i_and: ƒ $wcn_wasm_v128i_and()
wcn_wasm_v128i_andnot: ƒ $wcn_wasm_v128i_andnot()
wcn_wasm_v128i_cmpeq_i8: ƒ $wcn_wasm_v128i_cmpeq_i8()
wcn_wasm_v128i_cmpeq_i16
:
ƒ $wcn_wasm_v128i_cmpeq_i16()
wcn_wasm_v128i_cmpeq_i32
:
ƒ $wcn_wasm_v128i_cmpeq_i32()
wcn_wasm_v128i_cmpgt_i8
:
ƒ $wcn_wasm_v128i_cmpgt_i8()
wcn_wasm_v128i_cmpgt_i16
:
ƒ $wcn_wasm_v128i_cmpgt_i16()
wcn_wasm_v128i_cmpgt_i32
:
ƒ $wcn_wasm_v128i_cmpgt_i32()
wcn_wasm_v128i_cmplt_i8
:
ƒ $wcn_wasm_v128i_cmplt_i8()
wcn_wasm_v128i_cmplt_i16
:
ƒ $wcn_wasm_v128i_cmplt_i16()
wcn_wasm_v128i_cmplt_i32
:
ƒ $wcn_wasm_v128i_cmplt_i32()
wcn_wasm_v128i_load
:
ƒ $wcn_wasm_v128i_load()
wcn_wasm_v128i_load_aligned
:
ƒ $wcn_wasm_v128i_load()
wcn_wasm_v128i_max_i8
:
ƒ $wcn_wasm_v128i_max_i8()
wcn_wasm_v128i_max_i16
:
ƒ $wcn_wasm_v128i_max_i16()
wcn_wasm_v128i_max_i32
:
ƒ $wcn_wasm_v128i_max_i32()
wcn_wasm_v128i_max_u8
:
ƒ $wcn_wasm_v128i_max_u8()
wcn_wasm_v128i_min_i8
:
ƒ $wcn_wasm_v128i_min_i8()
wcn_wasm_v128i_min_i16
:
ƒ $wcn_wasm_v128i_min_i16()
wcn_wasm_v128i_min_i32
:
ƒ $wcn_wasm_v128i_min_i32()
wcn_wasm_v128i_min_u8
:
ƒ $wcn_wasm_v128i_min_u8()
wcn_wasm_v128i_mullo_i16
:
ƒ $wcn_wasm_v128i_mullo_i16()
wcn_wasm_v128i_mullo_i32
:
ƒ $wcn_wasm_v128i_mullo_i32()
wcn_wasm_v128i_neg_i8
:
ƒ $wcn_wasm_v128i_neg_i8()
wcn_wasm_v128i_neg_i16
:
ƒ $wcn_wasm_v128i_neg_i16()
wcn_wasm_v128i_neg_i32
:
ƒ $wcn_wasm_v128i_neg_i32()
wcn_wasm_v128i_neg_i64
:
ƒ $wcn_wasm_v128i_neg_i64()
wcn_wasm_v128i_or
:
ƒ $wcn_wasm_v128i_or()
wcn_wasm_v128i_set1_i8
:
ƒ $wcn_wasm_v128i_set1_i8()
wcn_wasm_v128i_set1_i16
:
ƒ $wcn_wasm_v128i_set1_i16()
wcn_wasm_v128i_set1_i32
:
ƒ $wcn_wasm_v128i_set1_i32()
wcn_wasm_v128i_set1_i64
:
ƒ $wcn_wasm_v128i_set1_i64()
wcn_wasm_v128i_setzero
:
ƒ $wcn_wasm_v128i_setzero()
wcn_wasm_v128i_shuffle_i8
:
ƒ $wcn_wasm_v128i_shuffle_i8()
wcn_wasm_v128i_sll_i16
:
ƒ $wcn_wasm_v128i_sll_i16()
wcn_wasm_v128i_sll_i32
:
ƒ $wcn_wasm_v128i_sll_i32()
wcn_wasm_v128i_sll_i64
:
ƒ $wcn_wasm_v128i_sll_i64()
wcn_wasm_v128i_slli_i16
:
ƒ $wcn_wasm_v128i_slli_i16()
wcn_wasm_v128i_slli_i32
:
ƒ $wcn_wasm_v128i_slli_i32()
wcn_wasm_v128i_slli_i64
:
ƒ $wcn_wasm_v128i_slli_i64()
wcn_wasm_v128i_sra_i16
:
ƒ $wcn_wasm_v128i_sra_i16()
wcn_wasm_v128i_sra_i32
:
ƒ $wcn_wasm_v128i_sra_i32()
wcn_wasm_v128i_srai_i16
:
ƒ $wcn_wasm_v128i_srai_i16()
wcn_wasm_v128i_srai_i32
:
ƒ $wcn_wasm_v128i_srai_i32()
wcn_wasm_v128i_srl_i16
:
ƒ $wcn_wasm_v128i_srl_i16()
wcn_wasm_v128i_srl_i32
:
ƒ $wcn_wasm_v128i_srl_i32()
wcn_wasm_v128i_srl_i64
:
ƒ $wcn_wasm_v128i_srl_i64()
wcn_wasm_v128i_srli_i16
:
ƒ $wcn_wasm_v128i_srli_i16()
wcn_wasm_v128i_srli_i32
:
ƒ $wcn_wasm_v128i_srli_i32()
wcn_wasm_v128i_srli_i64
:
ƒ $wcn_wasm_v128i_srli_i64()
wcn_wasm_v128i_store
:
ƒ $wcn_wasm_v128i_store()
wcn_wasm_v128i_store_aligned
:
ƒ $wcn_wasm_v128i_store()
wcn_wasm_v128i_sub_i8
:
ƒ $wcn_wasm_v128i_sub_i8()
wcn_wasm_v128i_sub_i16
:
ƒ $wcn_wasm_v128i_sub_i16()
wcn_wasm_v128i_sub_i32
:
ƒ $wcn_wasm_v128i_sub_i32()
wcn_wasm_v128i_sub_i64
:
ƒ $wcn_wasm_v128i_sub_i64()
wcn_wasm_v128i_subs_i8
:
ƒ $wcn_wasm_v128i_subs_i8()
wcn_wasm_v128i_subs_i16
:
ƒ $wcn_wasm_v128i_subs_i16()
wcn_wasm_v128i_subs_u8
:
ƒ $wcn_wasm_v128i_subs_u8()
wcn_wasm_v128i_subs_u16
:
ƒ $wcn_wasm_v128i_subs_u16()
wcn_wasm_v128i_unpackhi_i8
:
ƒ $wcn_wasm_v128i_unpackhi_i8()
wcn_wasm_v128i_unpackhi_i16
:
ƒ $wcn_wasm_v128i_unpackhi_i16()
wcn_wasm_v128i_unpackhi_i32
:
ƒ $wcn_wasm_v128i_unpackhi_i32()
wcn_wasm_v128i_unpacklo_i8
:
ƒ $wcn_wasm_v128i_unpacklo_i8()
wcn_wasm_v128i_unpacklo_i16
:
ƒ $wcn_wasm_v128i_unpacklo_i16()
wcn_wasm_v128i_unpacklo_i32
:
ƒ $wcn_wasm_v128i_unpacklo_i32()
wcn_wasm_v128i_xor
:
ƒ $wcn_wasm_v128i_xor()
__indirect_function_table
:
Table {length: 2}
_emscripten_stack_restore
:
ƒ $_emscripten_stack_restore()
_initialize
:
ƒ $_initialize()
 */
/**
 * WebAssembly exports interface - optimized for direct access
 */
interface WasmExports extends WebAssembly.Exports {
  /** --- Core --- */
  memory: WebAssembly.Memory;
  emscripten_stack_get_current(): number;

  /** --- Memory --- */
  wcn_alloc(size: number): number;
  wcn_free(ptr: number): void;
  wcn_realloc(ptr: number, size: number): number;
  wcn_simd_memcpy_aligned(dst: number, src: number, size: number): void;
  wcn_simd_memset_aligned(dst: number, value: number, size: number): void;

  /** --- Init & Features --- */
  wcn_simd_init(): number;
  wcn_simd_get_features(ptr: number): void;

  /** --- Array operations (f32) --- */
  wcn_simd_add_array_f32(dst: number, a: number, b: number, len: number): void;
  wcn_simd_mul_array_f32(dst: number, a: number, b: number, len: number): void;
  wcn_simd_fmadd_array_f32(
    dst: number,
    a: number,
    b: number,
    c: number,
    len: number,
  ): void;
  wcn_simd_scale_array_f32(
    dst: number,
    src: number,
    scale: number,
    len: number,
  ): void;
  wcn_simd_dot_product_f32(a: number, b: number, len: number): number;
  wcn_simd_reduce_sum_f32(src: number, len: number): number;
  wcn_simd_reduce_min_f32(src: number, len: number): number;
  wcn_simd_reduce_max_f32(src: number, len: number): number;

  /** --- V128 Float ops --- */
  wcn_wasm_v128f_setzero(): V128;
  wcn_wasm_v128f_set1(value: number): V128;
  wcn_wasm_v128f_add(a: V128, b: V128): V128;
  wcn_wasm_v128f_sub(a: V128, b: V128): V128;
  wcn_wasm_v128f_mul(a: V128, b: V128): V128;
  wcn_wasm_v128f_div(a: V128, b: V128): V128;
  wcn_wasm_v128f_abs(a: V128): V128;
  wcn_wasm_v128f_neg(a: V128): V128;
  wcn_wasm_v128f_sqrt(a: V128): V128;
  wcn_wasm_v128f_min(a: V128, b: V128): V128;
  wcn_wasm_v128f_max(a: V128, b: V128): V128;
  wcn_wasm_v128f_floor(a: V128): V128;
  wcn_wasm_v128f_ceil(a: V128): V128;
  wcn_wasm_v128f_round_nearest(a: V128): V128;
  wcn_wasm_v128f_trunc(a: V128): V128;

  wcn_wasm_v128f_load(ptr: number): V128;
  wcn_wasm_v128f_store(ptr: number, value: V128): void;
  wcn_wasm_v128f_unpacklo(a: V128, b: V128): V128;
  wcn_wasm_v128f_unpackhi(a: V128, b: V128): V128;

  /** --- V128 Integer ops (partial set) --- */
  wcn_wasm_v128i_setzero(): V128;
  wcn_wasm_v128i_set1_i32(value: number): V128;
  wcn_wasm_v128i_add_i32(a: V128, b: V128): V128;
  wcn_wasm_v128i_sub_i32(a: V128, b: V128): V128;
  wcn_wasm_v128i_mullo_i32(a: V128, b: V128): V128;
  wcn_wasm_v128i_and(a: V128, b: V128): V128;
  wcn_wasm_v128i_or(a: V128, b: V128): V128;
  wcn_wasm_v128i_xor(a: V128, b: V128): V128;
  wcn_wasm_v128i_load(ptr: number): V128;
  wcn_wasm_v128i_store(ptr: number, value: V128): void;
}

/* ========== Performance Optimized WCN_SIMD Class ========== */
// Base64 解码为 Uint8Array
function base64ToUint8Array(base64: string) {
  const binaryString = atob(base64); // 解码 Base64 为二进制字符串
  const len = binaryString.length;
  const bytes = new Uint8Array(len);
  for (let i = 0; i < len; i++) {
    bytes[i] = binaryString.charCodeAt(i);
  }
  return bytes;
}

type WCN_SIMD = {
  alloc: WasmExports["wcn_alloc"];
  free: WasmExports["wcn_free"];
  realloc: WasmExports["wcn_realloc"];
  init: WasmExports["wcn_simd_init"];
  memcpy: WasmExports["wcn_simd_memcpy_aligned"];
  memset: WasmExports["wcn_simd_memset_aligned"];
  addArrayF32: WasmExports["wcn_simd_add_array_f32"];
  mulArrayF32: WasmExports["wcn_simd_mul_array_f32"];
  fmaddArrayF32: WasmExports["wcn_simd_fmadd_array_f32"];
  scaleArrayF32: WasmExports["wcn_simd_scale_array_f32"];
  dotProductF32: WasmExports["wcn_simd_dot_product_f32"];
  reduceSumF32: WasmExports["wcn_simd_reduce_sum_f32"];
  reduceMinF32: WasmExports["wcn_simd_reduce_min_f32"];
  reduceMaxF32: WasmExports["wcn_simd_reduce_max_f32"];
  v128i: {
    setZero: WasmExports["wcn_wasm_v128i_setzero"];
    set1I32: WasmExports["wcn_wasm_v128i_set1_i32"];
    addI32: WasmExports["wcn_wasm_v128i_add_i32"];
    subI32: WasmExports["wcn_wasm_v128i_sub_i32"];
    mulLoI32: WasmExports["wcn_wasm_v128i_mullo_i32"];
    and: WasmExports["wcn_wasm_v128i_and"];
    or: WasmExports["wcn_wasm_v128i_or"];
    xor: WasmExports["wcn_wasm_v128i_xor"];
    load: WasmExports["wcn_wasm_v128i_load"];
    store: WasmExports["wcn_wasm_v128i_store"];
  };
  v128f: {
    setZero: WasmExports["wcn_wasm_v128f_setzero"];
    set1: WasmExports["wcn_wasm_v128f_set1"];
    add: WasmExports["wcn_wasm_v128f_add"];
    sub: WasmExports["wcn_wasm_v128f_sub"];
    mul: WasmExports["wcn_wasm_v128f_mul"];
    div: WasmExports["wcn_wasm_v128f_div"];
    abs: WasmExports["wcn_wasm_v128f_abs"];
    neg: WasmExports["wcn_wasm_v128f_neg"];
    sqrt: WasmExports["wcn_wasm_v128f_sqrt"];
    min: WasmExports["wcn_wasm_v128f_min"];
    max: WasmExports["wcn_wasm_v128f_max"];
    floor: WasmExports["wcn_wasm_v128f_floor"];
    ceil: WasmExports["wcn_wasm_v128f_ceil"];
    roundNearest: WasmExports["wcn_wasm_v128f_round_nearest"];
    trunc: WasmExports["wcn_wasm_v128f_trunc"];
    load: WasmExports["wcn_wasm_v128f_load"];
    store: WasmExports["wcn_wasm_v128f_store"];
    unpackLo: WasmExports["wcn_wasm_v128f_unpacklo"];
    unpackHi: WasmExports["wcn_wasm_v128f_unpackhi"];
  };
};

// 解码 Wasm 二进制
export async function init(): Promise<
  Readonly<WCN_SIMD> & {
    memory: WebAssembly.Memory;
  }
> {
  // 实例化并初始化 WCN_SIMD 模块
  const { wasmBase64 } = await import("./wcn_simd.base64");
  const wasmBytes = base64ToUint8Array(wasmBase64);
  // 实例化 Wasm 模块
  const { instance } = await WebAssembly.instantiate(wasmBytes, {});
  const _exports = instance.exports as WasmExports;
  return {
    alloc: _exports.wcn_alloc,
    free: _exports.wcn_free,
    realloc: _exports.wcn_realloc,
    init: _exports.wcn_simd_init,
    memcpy: _exports.wcn_simd_memcpy_aligned,
    memset: _exports.wcn_simd_memset_aligned,
    addArrayF32: _exports.wcn_simd_add_array_f32,
    mulArrayF32: _exports.wcn_simd_mul_array_f32,
    fmaddArrayF32: _exports.wcn_simd_fmadd_array_f32,
    scaleArrayF32: _exports.wcn_simd_scale_array_f32,
    dotProductF32: _exports.wcn_simd_dot_product_f32,
    reduceSumF32: _exports.wcn_simd_reduce_sum_f32,
    reduceMinF32: _exports.wcn_simd_reduce_min_f32,
    reduceMaxF32: _exports.wcn_simd_reduce_max_f32,
    v128i: {
      setZero: _exports.wcn_wasm_v128i_setzero,
      set1I32: _exports.wcn_wasm_v128i_set1_i32,
      addI32: _exports.wcn_wasm_v128i_add_i32,
      subI32: _exports.wcn_wasm_v128i_sub_i32,
      mulLoI32: _exports.wcn_wasm_v128i_mullo_i32,
      and: _exports.wcn_wasm_v128i_and,
      or: _exports.wcn_wasm_v128i_or,
      xor: _exports.wcn_wasm_v128i_xor,
      load: _exports.wcn_wasm_v128i_load,
      store: _exports.wcn_wasm_v128i_store,
    },
    v128f: {
      setZero: _exports.wcn_wasm_v128f_setzero,
      set1: _exports.wcn_wasm_v128f_set1,
      add: _exports.wcn_wasm_v128f_add,
      sub: _exports.wcn_wasm_v128f_sub,
      mul: _exports.wcn_wasm_v128f_mul,
      div: _exports.wcn_wasm_v128f_div,
      abs: _exports.wcn_wasm_v128f_abs,
      neg: _exports.wcn_wasm_v128f_neg,
      sqrt: _exports.wcn_wasm_v128f_sqrt,
      min: _exports.wcn_wasm_v128f_min,
      max: _exports.wcn_wasm_v128f_max,
      floor: _exports.wcn_wasm_v128f_floor,
      ceil: _exports.wcn_wasm_v128f_ceil,
      roundNearest: _exports.wcn_wasm_v128f_round_nearest,
      trunc: _exports.wcn_wasm_v128f_trunc,
      load: _exports.wcn_wasm_v128f_load,
      store: _exports.wcn_wasm_v128f_store,
      unpackLo: _exports.wcn_wasm_v128f_unpacklo,
      unpackHi: _exports.wcn_wasm_v128f_unpackhi,
    },
    // Expose WebAssembly.Memory so JS can copy data into/out of wasm for benchmarks
    memory: _exports.memory,
  } as const;
}

// class WcnSimd {
// 	simd: Readonly<WCN_SIMD>;
// 	constructor(simd: Readonly<WCN_SIMD>) {
// 		this.simd = simd;
// 	}

// 	initV128i(value: number): V128 {

// 	}

// 	initV128f(value: number): V128 {

// 	}

// }
