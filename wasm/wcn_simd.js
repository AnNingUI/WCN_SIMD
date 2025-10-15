/**
 * WCN_SIMD WebAssembly JS wrapper
 * ESM module with JSDoc types. Wraps a standalone .wasm built by Emscripten.
 *
 * Usage:
 *   import { createWcnSimd } from './wcn_simd.js';
 *   const simd = await createWcnSimd(); // loads ./WCN_SIMD.wasm by default
 *   await simd.init();
 *   const sum = simd.reduceSumF32([1,2,3,4]); // 10
 */

/**
 * @typedef {Object} WcnSimdOptions
 * @property {string} [wasmPath] - Path to WCN_SIMD.wasm. Default: same dir as this file.
 * @property {ArrayBuffer|Uint8Array} [wasmBytes] - If provided, instantiate from bytes instead of reading from path.
 * @property {WebAssembly.Imports} [imports] - Extra imports. Default: { env: {} }.
 */

/**
 * @typedef {Object} WcnSimdExports
 * @property {WebAssembly.Memory} memory
 * @property {function(): void} wcn_simd_init
 * @property {function(number, number): number} wcn_simd_reduce_sum_f32
 * @property {function(number): number} wcn_alloc
 * @property {function(number): void} wcn_free
 */

/**
 * @typedef {Object} WcnSimd
 * @property {()=> Promise<void>} init - Initialize the library (calls wcn_simd_init).
 * @property {(data: Float32Array|number[]) => number} reduceSumF32 - Returns sum of a float array using SIMD (allocates temporary Wasm memory).
 * @property {(a: Float32Array|number[], b: Float32Array|number[]) => number} dotProductF32 - Returns dot product of two float arrays using SIMD.
 * @property {(bytes: number) => number} alloc - Allocate bytes in Wasm heap (returns pointer).
 * @property {(ptr: number) => void} free - Free previously allocated pointer.
 * @property {WebAssembly.Memory} memory - Underlying Wasm memory for advanced use.
 * @property {()=> void} dispose - Release references for GC (optional helper).
 */

/**
 * Create and initialize the WCN_SIMD module.
 * @param {WcnSimdOptions} [options]
 * @returns {Promise<WcnSimd>}
 */
export async function createWcnSimd(options = {}) {
  const { wasmPath, wasmBytes } = options;
  /** @type {WebAssembly.Imports} */
  const extraImports = options.imports || {};
  const defaultImports = { env: {} };
  /** @type {WebAssembly.Imports} */
  const imports = mergeImports(defaultImports, extraImports);

  const { fileURLToPath } = await import("node:url");
  const pathMod = await import("node:path");
  const fs = await import("node:fs/promises");

  // Resolve wasm bytes
  let bytes;
  if (wasmBytes) {
    bytes =
      wasmBytes instanceof Uint8Array ? wasmBytes : new Uint8Array(wasmBytes);
  } else {
    const __filename = fileURLToPath(import.meta.url);
    const __dirname = pathMod.dirname(__filename);
    const resolvedPath = wasmPath
      ? resolveMaybeRelative(__dirname, wasmPath, pathMod)
      : pathMod.join(__dirname, "WCN_SIMD.wasm");
    bytes = await fs.readFile(resolvedPath);
  }

  /** @type {{ module: WebAssembly.Module, instance: WebAssembly.Instance }} */
  const { module, instance } = await WebAssembly.instantiate(bytes, imports);

  /** @type {WcnSimdExports} */
  const exp = /** @type {any} */ (instance.exports);

  ensureExport(exp, "memory", "object");
  ensureExport(exp, "wcn_simd_init", "function");
  ensureExport(exp, "wcn_simd_reduce_sum_f32", "function");
  ensureExport(exp, "wcn_alloc", "function");
  ensureExport(exp, "wcn_free", "function");
  // Optional exports (checked at call time)
  // wcn_simd_* helpers may be absent on non-supported builds; wrappers will validate before calling.

  /**
   * Allocate typed array in Wasm and copy data.
   * @param {Float32Array|number[]} data
   * @returns {{ ptr: number, len: number, view: Float32Array, free: () => void }}
   */
  function allocF32(data) {
    const src = Array.isArray(data) ? new Float32Array(data) : data;
    const bytes = src.byteLength;
    const ptr = exp.wcn_alloc(bytes);
    if (!ptr) throw new Error("wcn_alloc failed");
    const view = new Float32Array(exp.memory.buffer, ptr, src.length);
    view.set(src);
    return { ptr, len: src.length, view, free: () => exp.wcn_free(ptr) };
  }

  /** @type {WcnSimd} */
  const api = {
    async init() {
      exp.wcn_simd_init();
    },

    reduceSumF32(data) {
      const blk = allocF32(data);
      try {
        return exp.wcn_simd_reduce_sum_f32(blk.ptr, blk.len);
      } finally {
        blk.free();
      }
    },

    alloc(bytes) {
      return exp.wcn_alloc(bytes);
    },

    free(ptr) {
      exp.wcn_free(ptr);
    },

    get memory() {
      return exp.memory;
    },

    /**
     * Compute dot product of two float arrays.
     * @param {Float32Array|number[]} a
     * @param {Float32Array|number[]} b
     * @returns {number}
     */
    dotProductF32(a, b) {
      const aArr = Array.isArray(a) ? new Float32Array(a) : a;
      const bArr = Array.isArray(b) ? new Float32Array(b) : b;
      if (aArr.length !== bArr.length)
        throw new Error("dotProductF32: length mismatch");
      if (typeof exp.wcn_simd_dot_product_f32 !== "function") {
        throw new Error("wcn_simd_dot_product_f32 not exported");
      }
      const A = allocF32(aArr);
      const B = allocF32(bArr);
      try {
        return exp.wcn_simd_dot_product_f32(A.ptr, B.ptr, A.len);
      } finally {
        A.free();
        B.free();
      }
    },

    /**
     * Element-wise add: c = a + b
     * @param {Float32Array|number[]} a
     * @param {Float32Array|number[]} b
     * @returns {Float32Array}
     */
    addArrayF32(a, b) {
      const aArr = Array.isArray(a) ? new Float32Array(a) : a;
      const bArr = Array.isArray(b) ? new Float32Array(b) : b;
      if (aArr.length !== bArr.length)
        throw new Error("addArrayF32: length mismatch");
      if (typeof exp.wcn_simd_add_array_f32 !== "function") {
        throw new Error("wcn_simd_add_array_f32 not exported");
      }
      const A = allocF32(aArr);
      const B = allocF32(bArr);
      const bytes = aArr.byteLength;
      const outPtr = exp.wcn_alloc(bytes);
      if (!outPtr) {
        A.free();
        B.free();
        throw new Error("wcn_alloc failed");
      }
      try {
        exp.wcn_simd_add_array_f32(A.ptr, B.ptr, outPtr, aArr.length);
        // Copy back
        const out = new Float32Array(aArr.length);
        out.set(new Float32Array(exp.memory.buffer, outPtr, out.length));
        return out;
      } finally {
        exp.wcn_free(outPtr);
        A.free();
        B.free();
      }
    },

    /**
     * Element-wise mul: c = a * b
     * @param {Float32Array|number[]} a
     * @param {Float32Array|number[]} b
     * @returns {Float32Array}
     */
    mulArrayF32(a, b) {
      const aArr = Array.isArray(a) ? new Float32Array(a) : a;
      const bArr = Array.isArray(b) ? new Float32Array(b) : b;
      if (aArr.length !== bArr.length)
        throw new Error("mulArrayF32: length mismatch");
      if (typeof exp.wcn_simd_mul_array_f32 !== "function") {
        throw new Error("wcn_simd_mul_array_f32 not exported");
      }
      const A = allocF32(aArr);
      const B = allocF32(bArr);
      const bytes = aArr.byteLength;
      const outPtr = exp.wcn_alloc(bytes);
      if (!outPtr) {
        A.free();
        B.free();
        throw new Error("wcn_alloc failed");
      }
      try {
        exp.wcn_simd_mul_array_f32(A.ptr, B.ptr, outPtr, aArr.length);
        const out = new Float32Array(aArr.length);
        out.set(new Float32Array(exp.memory.buffer, outPtr, out.length));
        return out;
      } finally {
        exp.wcn_free(outPtr);
        A.free();
        B.free();
      }
    },

    /**
     * Scale: b = a * scalar
     * @param {Float32Array|number[]} a
     * @param {number} scalar
     * @returns {Float32Array}
     */
    scaleArrayF32(a, scalar) {
      const aArr = Array.isArray(a) ? new Float32Array(a) : a;
      if (typeof exp.wcn_simd_scale_array_f32 !== "function") {
        throw new Error("wcn_simd_scale_array_f32 not exported");
      }
      const A = allocF32(aArr);
      const bytes = aArr.byteLength;
      const outPtr = exp.wcn_alloc(bytes);
      if (!outPtr) {
        A.free();
        throw new Error("wcn_alloc failed");
      }
      try {
        exp.wcn_simd_scale_array_f32(A.ptr, scalar, outPtr, aArr.length);
        const out = new Float32Array(aArr.length);
        out.set(new Float32Array(exp.memory.buffer, outPtr, out.length));
        return out;
      } finally {
        exp.wcn_free(outPtr);
        A.free();
      }
    },

    /**
     * FMA: c = a * b + c
     * @param {Float32Array|number[]} a
     * @param {Float32Array|number[]} b
     * @param {Float32Array|number[]} c
     * @returns {Float32Array}
     */
    fmaddArrayF32(a, b, c) {
      const aArr = Array.isArray(a) ? new Float32Array(a) : a;
      const bArr = Array.isArray(b) ? new Float32Array(b) : b;
      const cArr = Array.isArray(c) ? new Float32Array(c) : c;
      if (aArr.length !== bArr.length || aArr.length !== cArr.length)
        throw new Error("fmaddArrayF32: length mismatch");
      if (typeof exp.wcn_simd_fmadd_array_f32 !== "function") {
        throw new Error("wcn_simd_fmadd_array_f32 not exported");
      }
      const A = allocF32(aArr);
      const B = allocF32(bArr);
      const C = allocF32(cArr);
      try {
        // In-place on C buffer to match signature
        exp.wcn_simd_fmadd_array_f32(A.ptr, B.ptr, C.ptr, aArr.length);
        const out = new Float32Array(cArr.length);
        out.set(new Float32Array(exp.memory.buffer, C.ptr, out.length));
        return out;
      } finally {
        A.free();
        B.free();
        C.free();
      }
    },

    /**
     * Reduce max of float array.
     * @param {Float32Array|number[]} data
     * @returns {number}
     */
    reduceMaxF32(data) {
      if (typeof exp.wcn_simd_reduce_max_f32 !== "function") {
        throw new Error("wcn_simd_reduce_max_f32 not exported");
      }
      const blk = allocF32(Array.isArray(data) ? new Float32Array(data) : data);
      try {
        return exp.wcn_simd_reduce_max_f32(blk.ptr, blk.len);
      } finally {
        blk.free();
      }
    },

    /**
     * Reduce min of float array.
     * @param {Float32Array|number[]} data
     * @returns {number}
     */
    reduceMinF32(data) {
      if (typeof exp.wcn_simd_reduce_min_f32 !== "function") {
        throw new Error("wcn_simd_reduce_min_f32 not exported");
      }
      const blk = allocF32(Array.isArray(data) ? new Float32Array(data) : data);
      try {
        return exp.wcn_simd_reduce_min_f32(blk.ptr, blk.len);
      } finally {
        blk.free();
      }
    },

    /**
     * Get detected SIMD features (reads C struct wcn_simd_features_t).
     * @returns {{[k: string]: number|boolean}}
     */
    getFeatures() {
      if (typeof exp.wcn_simd_get_features !== "function") {
        throw new Error("wcn_simd_get_features not exported");
      }
      const ptr = exp.wcn_simd_get_features();
      if (!ptr) throw new Error("wcn_simd_get_features returned null");
      // Layout per include/WCN_SIMD.h order (25 int fields)
      const names = [
        "has_sse2",
        "has_sse3",
        "has_ssse3",
        "has_sse4_1",
        "has_sse4_2",
        "has_avx",
        "has_avx2",
        "has_avx512f",
        "has_avx512bw",
        "has_avx512dq",
        "has_avx512vl",
        "has_fma",
        "has_neon",
        "has_sve",
        "has_sve2",
        "has_lsx",
        "has_lasx",
        "has_rvv",
        "has_altivec",
        "has_vsx",
        "has_msa",
        "has_wasm_simd128",
        "has_atomic_operations",
        "has_gcc_atomic",
        "has_msvc_atomic",
      ];
      const view = new Int32Array(exp.memory.buffer, ptr, names.length);
      /** @type {Record<string, boolean|number>} */
      const out = {};
      for (let i = 0; i < names.length; i++) {
        const v = view[i] | 0;
        out[names[i]] = v !== 0;
      }
      return out;
    },

    dispose() {
      // No explicit finalization required; this helper just releases references for GC.
      // Users can drop the returned api object to allow GC of instance/module.
    },
  };

  return api;
}

/**
 * Merge two WebAssembly imports objects (shallow, merging env namespace).
 * @param {WebAssembly.Imports} a
 * @param {WebAssembly.Imports} b
 * @returns {WebAssembly.Imports}
 */
function mergeImports(a, b) {
  const out = { ...a, ...b };
  const envA = /** @type {any} */ (a.env) || {};
  const envB = /** @type {any} */ (b.env) || {};
  out.env = { ...envA, ...envB };
  return out;
}

/**
 * @param {any} exp
 * @param {string} name
 * @param {'function'|'object'} kind
 */
function ensureExport(exp, name, kind) {
  if (typeof exp[name] !== kind) {
    const names = Object.keys(exp);
    throw new Error(
      `Required export '${name}' (${kind}) not found. Available: ${names.join(", ")}`,
    );
  }
}

/**
 * @param {string} baseDir
 * @param {string} p
 * @param {typeof import('node:path')} path
 */
function resolveMaybeRelative(baseDir, p, path) {
  if (p.startsWith("file://")) return fileURLToPath(p);
  if (path.isAbsolute(p)) return p;
  return path.join(baseDir, p);
}

// Helper to support file:// paths on resolve
import { fileURLToPath } from "node:url";
