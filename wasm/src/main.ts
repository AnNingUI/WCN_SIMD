import { init } from "./wcn_simd";

/**
wasm.fmaddArrayF32
// c
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
  //...
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
  //...
#endif

  // RISC-V Vector Extension implementation
#if defined(WCN_RISCV_RVV)
  //...
#endif

  // Scalar tail for remaining elements
  for (; i < count; i++) {
    *pc = (*pa) * (*pb) + (*pc);
    pa++;
    pb++;
    pc++;
  }
}
// wat

 */

const alignTo16 = (ptr: number): number => (ptr + 15) & ~15;
async function run() {
  const wasm = await init();
  wasm.init();

  // timing variables
  let start = 0;
  let end = 0;

  // Example usage
  const size = 10000;
  const iterations = 1000;

  // v128 state and timing
  let timeV128Add = 0;
  let timeJsV128Add = 0;
  let timeV128Mul = 0;
  let timeJsV128Mul = 0;
  let usedV128Bindings = false;

  // Reduction result variables (declare early to avoid TDZ when assigned inside callbacks)
  let sumWasm = 0;
  let sumJs = 0;
  let maxWasm = 0;
  let maxJs = -Infinity;

  // Allocate memory for arrays
  // Allocate extra space (2x) to avoid potential out-of-bounds from WASM routines
  const aBase = wasm.alloc(size * 4 * 2 + 15);
  const aPtr = alignTo16(aBase); // 4 bytes per float
  const bBase = wasm.alloc(size * 4 * 2 + 15);
  const bPtr = alignTo16(bBase);
  const cBase = wasm.alloc(size * 4 * 2 + 15);
  const cPtr = alignTo16(cBase);
  const dBase = wasm.alloc(size * 4 * 2 + 15);
  const dPtr = alignTo16(dBase);

  // Get views of the memory
  let memory = new Float32Array(wasm.memory.buffer, aPtr, size);
  let memoryB = new Float32Array(wasm.memory.buffer, bPtr, size);
  let memoryC = new Float32Array(wasm.memory.buffer, cPtr, size);
  let memoryD = new Float32Array(wasm.memory.buffer, dPtr, size);

  // Ensure wasm memory buffer is large enough for our allocations; grow if necessary and recreate views
  function ensureMemoryCapacity(requiredEnd: number) {
    const pageSize = 65536; // wasm page size in bytes
    const buf = wasm.memory.buffer;
    if (requiredEnd > buf.byteLength) {
      const needed = requiredEnd - buf.byteLength;
      const pages = Math.ceil(needed / pageSize);
      try {
        wasm.memory.grow(pages);
      } catch (e) {
        console.error("Failed to grow wasm memory:", e);
        throw e;
      }
    }
    // Recreate views after potential growth
    memory = new Float32Array(wasm.memory.buffer, aPtr, size);
    memoryB = new Float32Array(wasm.memory.buffer, bPtr, size);
    memoryC = new Float32Array(wasm.memory.buffer, cPtr, size);
    memoryD = new Float32Array(wasm.memory.buffer, dPtr, size);
  }

  // Ensure a specific byte range is available in wasm memory
  function ensureRange(ptrByte: number, byteLen: number) {
    const end = ptrByte + byteLen;
    ensureMemoryCapacity(end);
  }

  // Call once to ensure buffer is sufficient
  ensureMemoryCapacity(Math.max(aPtr, bPtr, cPtr, dPtr) + size * 4);

  // Initialize with random data
  for (let i = 0; i < size; i++) {
    memory[i] = Math.random();
    memoryB[i] = Math.random();
    memoryC[i] = Math.random();
  }

  console.log(
    `Benchmark Configuration: ${size} elements, ${iterations} iterations\n`,
  );

  // Helper: measure a WASM operation with range checks
  function timeWasmOp(
    rangePairs: Array<[number, number]>,
    iterations: number,
    iterFn: (i: number) => void,
  ) {
    for (const [ptr, len] of rangePairs) ensureRange(ptr, len);
    start = performance.now();
    for (let i = 0; i < iterations; i++) iterFn(i);
    end = performance.now();
    return end - start;
  }

  // Helper: measure a JS operation
  function timeJsOp(iterations: number, iterFn: (i: number) => void) {
    start = performance.now();
    for (let i = 0; i < iterations; i++) iterFn(i);
    end = performance.now();
    return end - start;
  }

  // Concise benchmarks using helpers
  // Dot product
  console.log("=== Dot Product Benchmark ===");
  let resultWasm = 0;
  let resultJs = 0;
  const timeWasmDot = timeWasmOp(
    [
      [aPtr, size * 4],
      [bPtr, size * 4],
    ],
    iterations,
    () => {
      resultWasm = wasm.dotProductF32(aPtr, bPtr, size);
    },
  );
  const timeJsDot = timeJsOp(iterations, () => {
    resultJs = 0;
    for (let i = 0; i < size; i++) resultJs += memory[i] * memoryB[i];
  });

  console.log(
    `WASM result: ${resultWasm.toFixed(6)} (${timeWasmDot.toFixed(6)} ms)`,
  );
  console.log(`JS result: ${resultJs.toFixed(6)} (${timeJsDot.toFixed(6)} ms)`);
  console.log(`Speedup: ${(timeJsDot / timeWasmDot).toFixed(6)}x\n`);

  // Vector add
  console.log("=== Vector Addition Benchmark ===");
  const timeWasmAdd = timeWasmOp(
    [
      [dPtr, size * 4],
      [aPtr, size * 4],
      [bPtr, size * 4],
    ],
    iterations,
    () => {
      wasm.addArrayF32(dPtr, aPtr, bPtr, size);
    },
  );
  const timeJsAdd = timeJsOp(iterations, () => {
    for (let i = 0; i < size; i++) memoryD[i] = memory[i] + memoryB[i];
  });
  console.log(`WASM time: ${timeWasmAdd.toFixed(6)} ms`);
  console.log(`JS time: ${timeJsAdd.toFixed(6)} ms`);
  console.log(`Speedup: ${(timeJsAdd / timeWasmAdd).toFixed(6)}x\n`);

  // Vector mul
  console.log("=== Vector Multiplication Benchmark ===");
  const timeWasmMul = timeWasmOp(
    [
      [dPtr, size * 4],
      [aPtr, size * 4],
      [bPtr, size * 4],
    ],
    iterations,
    () => {
      wasm.mulArrayF32(dPtr, aPtr, bPtr, size);
    },
  );
  const timeJsMul = timeJsOp(iterations, () => {
    for (let i = 0; i < size; i++) memoryD[i] = memory[i] * memoryB[i];
  });
  console.log(`WASM time: ${timeWasmMul.toFixed(6)} ms`);
  console.log(`JS time: ${timeJsMul.toFixed(6)} ms`);
  console.log(`Speedup: ${(timeJsMul / timeWasmMul).toFixed(6)}x\n`);

  // Scalar scale
  console.log("=== Scalar Multiplication Benchmark ===");
  const scalar = 2.5;
  console.log(wasm.fmaddArrayF32);
  const timeWasmScale = timeWasmOp(
    [
      [dPtr, size * 4],
      [aPtr, size * 4],
    ],
    iterations,
    () => {
      wasm.scaleArrayF32(dPtr, aPtr, scalar, size);
    },
  );
  const timeJsScale = timeJsOp(iterations, () => {
    for (let i = 0; i < size; i++) memoryD[i] = memory[i] * scalar;
  });
  console.log(`WASM time: ${timeWasmScale.toFixed(6)} ms`);
  console.log(`JS time: ${timeJsScale.toFixed(6)} ms`);
  console.log(`Speedup: ${(timeJsScale / timeWasmScale).toFixed(6)}x\n`);

  // FMA
  console.log("=== Fused Multiply-Add Benchmark ===");
  const timeWasmFMA = timeWasmOp(
    [
      [dPtr, size * 4],
      [aPtr, size * 4],
      [bPtr, size * 4],
      [cPtr, size * 4],
    ],
    iterations,
    () => {
      /** wat
       *
       */
      wasm.fmaddArrayF32(dPtr, aPtr, bPtr, cPtr, size);
    },
  );
  const timeJsFMA = timeJsOp(iterations, () => {
    for (let i = 0; i < size; i++)
      memoryD[i] = memory[i] * memoryB[i] + memoryC[i];
  });
  console.log(`WASM time: ${timeWasmFMA.toFixed(6)} ms`);
  console.log(`JS time: ${timeJsFMA.toFixed(6)} ms`);
  console.log(`Speedup: ${(timeJsFMA / timeWasmFMA).toFixed(6)}x\n`);

  // Reductions
  console.log("=== Reduction Operations Benchmark ===");
  const timeWasmSum = timeWasmOp([[aPtr, size * 4]], iterations, () => {
    sumWasm = wasm.reduceSumF32(aPtr, size);
  });
  const timeJsSum = timeJsOp(iterations, () => {
    sumJs = 0;
    for (let i = 0; i < size; i++) sumJs += memory[i];
  });
  console.log(
    `Sum - WASM: ${sumWasm.toFixed(6)} (${timeWasmSum.toFixed(6)} ms)`,
  );
  console.log(`Sum - JS: ${sumJs.toFixed(6)} (${timeJsSum.toFixed(6)} ms)`);
  console.log(`Sum Speedup: ${(timeJsSum / timeWasmSum).toFixed(6)}x`);

  const timeWasmMax = timeWasmOp([[aPtr, size * 4]], iterations, () => {
    maxWasm = wasm.reduceMaxF32(aPtr, size);
  });
  const timeJsMax = timeJsOp(iterations, () => {
    maxJs = -Infinity;
    for (let i = 0; i < size; i++) if (memory[i] > maxJs) maxJs = memory[i];
  });
  console.log(
    `Max - WASM: ${maxWasm.toFixed(6)} (${timeWasmMax.toFixed(6)} ms)`,
  );
  console.log(`Max - JS: ${maxJs.toFixed(6)} (${timeJsMax.toFixed(6)} ms)`);
  console.log(`Max Speedup: ${(timeJsMax / timeWasmMax).toFixed(6)}x\n`);

  // V128 low-level operations (chunked batched fallback)
  console.log("=== V128 Low-Level Operations Benchmark ===");
  const chunkElems = 1024; // must be multiple of 4
  const chunkSize = Math.max(4, Math.floor(chunkElems / 4) * 4);
  const bytesTotal = size * 4;
  // Ensure entire region once
  ensureRange(aPtr, bytesTotal);
  ensureRange(bPtr, bytesTotal);
  ensureRange(dPtr, bytesTotal);

  if (!usedV128Bindings) {
    // WASM: process chunks
    start = performance.now();
    for (let offset = 0; offset < size; offset += chunkSize) {
      const len = Math.min(chunkSize, size - offset);
      const bytePtr = offset * 4;
      wasm.addArrayF32(dPtr + bytePtr, aPtr + bytePtr, bPtr + bytePtr, len);
    }
    end = performance.now();
    timeV128Add = end - start;

    // JS reference
    start = performance.now();
    for (let offset = 0; offset < size; offset += chunkSize) {
      const len = Math.min(chunkSize, size - offset);
      for (let i = 0; i < len; i++) {
        const idx = offset + i;
        memoryD[idx] = memory[idx] + memoryB[idx];
      }
    }
    end = performance.now();
    timeJsV128Add = end - start;
    console.log(`v128f add - WASM (chunked): ${timeV128Add.toFixed(6)} ms`);
    console.log(`v128f add - JS: ${timeJsV128Add.toFixed(6)} ms`);
    console.log(
      `v128f add Speedup: ${(timeJsV128Add / timeV128Add).toFixed(6)}x`,
    );

    // Multiply: WASM
    start = performance.now();
    for (let offset = 0; offset < size; offset += chunkSize) {
      const len = Math.min(chunkSize, size - offset);
      const bytePtr = offset * 4;
      wasm.mulArrayF32(dPtr + bytePtr, aPtr + bytePtr, bPtr + bytePtr, len);
    }
    end = performance.now();
    timeV128Mul = end - start;

    // Multiply: JS
    start = performance.now();
    for (let offset = 0; offset < size; offset += chunkSize) {
      const len = Math.min(chunkSize, size - offset);
      for (let i = 0; i < len; i++) {
        const idx = offset + i;
        memoryD[idx] = memory[idx] * memoryB[idx];
      }
    }
    end = performance.now();
    timeJsV128Mul = end - start;
    console.log(`v128f mul - WASM (chunked): ${timeV128Mul.toFixed(6)} ms`);
    console.log(`v128f mul - JS: ${timeJsV128Mul.toFixed(6)} ms`);
    console.log(
      `v128f mul Speedup: ${(timeJsV128Mul / timeV128Mul).toFixed(6)}x`,
    );
  }

  // Summary
  console.log("\n=== Benchmark Summary ===");
  console.log(`Dot Product Speedup: ${(timeJsDot / timeWasmDot).toFixed(6)}x`);
  console.log(`Vector Add Speedup: ${(timeJsAdd / timeWasmAdd).toFixed(6)}x`);
  console.log(`Vector Mul Speedup: ${(timeJsMul / timeWasmMul).toFixed(6)}x`);
  console.log(
    `Scalar Mul Speedup: ${(timeJsScale / timeWasmScale).toFixed(6)}x`,
  );
  console.log(`FMA Speedup: ${(timeJsFMA / timeWasmFMA).toFixed(6)}x`);
  console.log(
    `Sum Reduction Speedup: ${(timeJsSum / timeWasmSum).toFixed(6)}x`,
  );
  console.log(
    `Max Reduction Speedup: ${(timeJsMax / timeWasmMax).toFixed(6)}x`,
  );
  console.log(
    `v128f Add Speedup: ${(timeJsV128Add / timeV128Add).toFixed(6)}x`,
  );
  console.log(
    `v128f Mul Speedup: ${(timeJsV128Mul / timeV128Mul).toFixed(6)}x`,
  );

  // Cleanup
  // Skipping explicit frees in this micro-benchmark to avoid runtime free-related errors.
  console.log("Skipping wasm.free calls (benchmark teardown).");
}

run();
