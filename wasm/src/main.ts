import { init } from "./wcn_simd";
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
			resultWasm = wasm.dotProductF32(aPtr, bPtr, size);
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
			wasm.addArrayF32(dPtr, aPtr, bPtr, size);
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
			wasm.mulArrayF32(dPtr, aPtr, bPtr, size);
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
			wasm.scaleArrayF32(dPtr, aPtr, scalar, size);
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
	const timeWasmFMA = timeWasmOp(
		[
			[dPtr, size * 4],
			[aPtr, size * 4],
			[bPtr, size * 4],
			[cPtr, size * 4],
		],
		iterations,
		() => {
			wasm.fmaddArrayF32(dPtr, aPtr, bPtr, cPtr, size);
		}
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
