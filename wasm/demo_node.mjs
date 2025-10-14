import { createWcnSimd } from './wcn_simd.js';

function nowNs() {
  return process.hrtime.bigint();
}
function nsToMs(ns) {
  return Number(ns) / 1e6;
}
function bench(fn) {
  const t0 = nowNs();
  const res = fn();
  const t1 = nowNs();
  return { ms: nsToMs(t1 - t0), res };
}

const simd = await createWcnSimd();
await simd.init();

// 小规模功能演示
console.log('sum (small):', simd.reduceSumF32([1,2,3,4]));                    // 10
console.log('dot (small):', simd.dotProductF32([1,2,3], [4,5,6]));            // 32
console.log('add (small):', simd.addArrayF32([1,2], [3,4]));                  // [4,6]
console.log('mul (small):', simd.mulArrayF32([2,3], [4,5]));                  // [8,15]
console.log('scale (small):', simd.scaleArrayF32([1,2,3], 0.5));              // [0.5,1,1.5]
console.log('fmadd (small):', simd.fmaddArrayF32([1,2],[3,4],[5,6]));         // [8,14]
console.log('max (small):', simd.reduceMaxF32([1,9,3,7]));                    // 9
console.log('min (small):', simd.reduceMinF32([1,9,3,7]));                    // 1
console.log('features:', simd.getFeatures());

// 大数组基准对比
const N = 1_000_000; // 100万元素，约4MB，适配当前内存页数
const a = new Float32Array(N);
const b = new Float32Array(N);
// 使用全1，避免精度问题：sum(a)=N，dot(a,b)=N
a.fill(1);
b.fill(1);

// JS baseline: reduce sum
const jsSum = bench(() => {
  let s = 0.0;
  for (let i = 0; i < a.length; i++) s += a[i];
  return s;
});

// Wasm SIMD: reduce sum
const wasmSum = bench(() => simd.reduceSumF32(a));

// 对比结果
const expectedSum = N;
const eps = 1e-3;
const okSum = Math.abs(wasmSum.res - expectedSum) <= eps && Math.abs(jsSum.res - expectedSum) <= eps;

console.log('\n== Reduce Sum F32 (N=1,000,000) ==');
console.log(`JS:   ${jsSum.res} in ${jsSum.ms.toFixed(2)} ms`);
console.log(`Wasm: ${wasmSum.res} in ${wasmSum.ms.toFixed(2)} ms`);
console.log(`Speedup: ${(jsSum.ms / wasmSum.ms).toFixed(2)}x`);
console.log(`Correct: ${okSum}`);

// JS baseline: dot product
const jsDot = bench(() => {
  let s = 0.0;
  for (let i = 0; i < a.length; i++) s += a[i] * b[i];
  return s;
});

// Wasm SIMD: dot product
const wasmDot = bench(() => simd.dotProductF32(a, b));

const expectedDot = N;
const okDot = Math.abs(wasmDot.res - expectedDot) <= eps && Math.abs(jsDot.res - expectedDot) <= eps;

console.log('\n== Dot Product F32 (N=1,000,000) ==');
console.log(`JS:   ${jsDot.res} in ${jsDot.ms.toFixed(2)} ms`);
console.log(`Wasm: ${wasmDot.res} in ${wasmDot.ms.toFixed(2)} ms`);
console.log(`Speedup: ${(jsDot.ms / wasmDot.ms).toFixed(2)}x`);
console.log(`Correct: ${okDot}`);

// 结束