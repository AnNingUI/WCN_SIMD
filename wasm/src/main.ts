import { init } from "./wcn_simd";

/**
wasm.fmaddArrayF32
// c
// Eg: -mrelaxed-simd
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
    v128_t v0_0 = wasm_v128_load(pa);
    v128_t v1_0 = wasm_v128_load(pb);
    v128_t v2_0 = wasm_v128_load(pc);
    wasm_v128_store(pc, wasm_f32x4_qfma(v0_0, v1_0, v2_0));

    v128_t v0_1 = wasm_v128_load(pa + 16);
    v128_t v1_1 = wasm_v128_load(pb + 16);
    v128_t v2_1 = wasm_v128_load(pc + 16);
    wasm_v128_store(pc + 16, wasm_f32x4_qfma(v0_1, v1_1, v2_1));

    v128_t v0_2 = wasm_v128_load(pa + 32);
    v128_t v1_2 = wasm_v128_load(pb + 32);
    v128_t v2_2 = wasm_v128_load(pc + 32);
    wasm_v128_store(pc + 32, wasm_f32x4_qfma(v0_2, v1_2, v2_2));

    v128_t v0_3 = wasm_v128_load(pa + 48);
    v128_t v1_3 = wasm_v128_load(pb + 48);
    v128_t v2_3 = wasm_v128_load(pc + 48);
    wasm_v128_store(pc + 48, wasm_f32x4_qfma(v0_3, v1_3, v2_3));

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
		`Benchmark Configuration: ${size} elements, ${iterations} iterations\n`
	);

	// Helper: measure a WASM operation with range checks
	function timeWasmOp(
		rangePairs: Array<[number, number]>,
		iterations: number,
		iterFn: (i: number) => void
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
			resultWasm = wasm.dotProductKahanF32(aPtr, bPtr, size);
		}
	);
	const timeJsDot = timeJsOp(iterations, () => {
		resultJs = 0;
		for (let i = 0; i < size; i++) resultJs += memory[i] * memoryB[i];
	});

	console.log(
		`WASM result: ${resultWasm.toFixed(6)} (${timeWasmDot.toFixed(6)} ms)`
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
			wasm.addArrayF32(aPtr, bPtr, dPtr, size);
		}
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
			wasm.mulArrayF32(aPtr, bPtr, dPtr, size);
		}
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
	const timeWasmScale = timeWasmOp(
		[
			[dPtr, size * 4],
			[aPtr, size * 4],
		],
		iterations,
		() => {
			wasm.scaleArrayF32(aPtr, scalar, dPtr, size);
		}
	);
	const timeJsScale = timeJsOp(iterations, () => {
		for (let i = 0; i < size; i++) memoryD[i] = memory[i] * scalar;
	});
	console.log(`WASM time: ${timeWasmScale.toFixed(6)} ms`);
	console.log(`JS time: ${timeJsScale.toFixed(6)} ms`);
	console.log(`Speedup: ${(timeJsScale / timeWasmScale).toFixed(6)}x\n`);

	// FMA
	console.log("=== Fused Multiply-Add Benchmark ===");
	memoryD.fill(1);
	memory.fill(2);
	memoryB.fill(3);
	const timeWasmFMA = timeWasmOp(
		[
			[aPtr, size * 4],
			[bPtr, size * 4],
			[dPtr, size * 4],
		],
		iterations,
		() => {
			wasm.fmaddArrayF32(aPtr, bPtr, dPtr, size);
		}
	);
	console.log(`WASM result check (first 5): ${memoryD.slice(0, 5)}`);
	memoryD.fill(1);
	memory.fill(2);
	memoryB.fill(3);
	const timeJsFMA = timeJsOp(iterations, () => {
		for (let i = 0; i < size; i++)
			memoryD[i] = memory[i] * memoryB[i] + memoryD[i];
	});
	console.log(`JS result check (first 5): ${memoryD.slice(0, 5)}`);
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
		`Sum - WASM: ${sumWasm.toFixed(6)} (${timeWasmSum.toFixed(6)} ms)`
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
		`Max - WASM: ${maxWasm.toFixed(6)} (${timeWasmMax.toFixed(6)} ms)`
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
			`v128f add Speedup: ${(timeJsV128Add / timeV128Add).toFixed(6)}x`
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
			`v128f mul Speedup: ${(timeJsV128Mul / timeV128Mul).toFixed(6)}x`
		);
	}

	// Summary
	console.log("\n=== Benchmark Summary ===");
	console.log(`Dot Product Speedup: ${(timeJsDot / timeWasmDot).toFixed(6)}x`);
	console.log(`Vector Add Speedup: ${(timeJsAdd / timeWasmAdd).toFixed(6)}x`);
	console.log(`Vector Mul Speedup: ${(timeJsMul / timeWasmMul).toFixed(6)}x`);
	console.log(
		`Scalar Mul Speedup: ${(timeJsScale / timeWasmScale).toFixed(6)}x`
	);
	console.log(`FMA Speedup: ${(timeJsFMA / timeWasmFMA).toFixed(6)}x`);
	console.log(
		`Sum Reduction Speedup: ${(timeJsSum / timeWasmSum).toFixed(6)}x`
	);
	console.log(
		`Max Reduction Speedup: ${(timeJsMax / timeWasmMax).toFixed(6)}x`
	);
	console.log(
		`v128f Add Speedup: ${(timeJsV128Add / timeV128Add).toFixed(6)}x`
	);
	console.log(
		`v128f Mul Speedup: ${(timeJsV128Mul / timeV128Mul).toFixed(6)}x`
	);

	// Cleanup
	// Skipping explicit frees in this micro-benchmark to avoid runtime free-related errors.
	console.log("Skipping wasm.free calls (benchmark teardown).");
}

run();
