# WCN_SIMD WebAssembly TypeScript Wrapper

High-performance zero-overhead SIMD library wrapper for WebAssembly.

## Features

- **Zero Runtime Overhead**: No `checkInit()` calls, no try-catch blocks in hot paths
- **Direct WASM Access**: Public `wasm` property for advanced users
- **Type-Safe**: Full TypeScript type definitions
- **Memory Efficient**: Manual memory management with malloc/free
- **128-bit SIMD Operations**: Integer and float vector operations

## Installation

```bash
npm install
```

## Quick Start

```typescript
import { WCN_SIMD } from './src/main';

// Initialize (static factory pattern)
const simd = await WCN_SIMD.init('./public/WCN_SIMD.wasm');

// Simple vector operations
const vec1 = simd.v128f.set1(2.5);
const vec2 = simd.v128f.set1(3.0);
const result = simd.v128f.mul(vec1, vec2);

// Array operations
const dot = simd.dotProduct([1,2,3,4], [5,6,7,8]);
const sum = simd.reduceSum([1,2,3,4,5]);
```

## API Overview

### Initialization

```typescript
const simd = await WCN_SIMD.init(wasmPath?: string): Promise<WCN_SIMD>
```

### Vector Operations (v128i - 128-bit Integer)

```typescript
// Load/Store
simd.v128i.load(ptr: number): V128i
simd.v128i.store(ptr: number, vec: V128i): void

// Initialize
simd.v128i.set1_i32(value: number): V128i
simd.v128i.setzero(): V128i

// Arithmetic
simd.v128i.add_i32(a: V128i, b: V128i): V128i
simd.v128i.sub_i32(a: V128i, b: V128i): V128i
simd.v128i.mullo_i32(a: V128i, b: V128i): V128i

// Bitwise
simd.v128i.and(a: V128i, b: V128i): V128i
simd.v128i.or(a: V128i, b: V128i): V128i
simd.v128i.xor(a: V128i, b: V128i): V128i

// Min/Max
simd.v128i.min_i8(a: V128i, b: V128i): V128i
simd.v128i.max_i8(a: V128i, b: V128i): V128i
```

### Vector Operations (v128f - 128-bit Float)

```typescript
// Arithmetic
simd.v128f.add(a: V128f, b: V128f): V128f
simd.v128f.sub(a: V128f, b: V128f): V128f
simd.v128f.mul(a: V128f, b: V128f): V128f
simd.v128f.div(a: V128f, b: V128f): V128f

// Math
simd.v128f.sqrt(vec: V128f): V128f
simd.v128f.abs(vec: V128f): V128f
simd.v128f.neg(vec: V128f): V128f

// Rounding
simd.v128f.floor(vec: V128f): V128f
simd.v128f.ceil(vec: V128f): V128f
simd.v128f.round(vec: V128f): V128f
```

### High-Level Array Operations

```typescript
// Dot product
simd.dotProduct(a: number[], b: number[]): number

// Element-wise operations
simd.addArrays(a: number[], b: number[]): number[]
simd.mulArrays(a: number[], b: number[]): number[]
simd.scaleArray(a: number[], scalar: number): number[]

// Reductions
simd.reduceMax(data: number[]): number
simd.reduceMin(data: number[]): number
simd.reduceSum(data: number[]): number
```

### Memory Management

```typescript
// Allocate/free memory
const ptr = simd.malloc(sizeInBytes);
simd.free(ptr);

// Create typed arrays
const {ptr, view} = simd.createFloat32Array([1,2,3,4]);
simd.free(ptr); // Don't forget to free!

// Read from WASM memory
const arr = simd.readFloat32Array(ptr, length);
```

### Advanced: Direct WASM Access

```typescript
// Access WASM exports directly (zero overhead)
const result = simd.wasm.wcn_wasm_v128f_add(vec1, vec2);

// Access memory
const memory = simd.memory;
```

## Performance Tips

1. **Reuse Memory**: Allocate once, reuse multiple times
   ```typescript
   const ptr = simd.malloc(1024 * 4); // 1024 floats
   // ... use ptr multiple times
   simd.free(ptr);
   ```

2. **Direct WASM Calls**: For maximum performance, use `simd.wasm.*` directly
   ```typescript
   // Instead of
   const result = simd.v128f.add(a, b);
   
   // Use
   const result = simd.wasm.wcn_wasm_v128f_add(a, b);
   ```

3. **Batch Operations**: Process data in chunks
   ```typescript
   const chunkSize = 1024;
   for (let i = 0; i < data.length; i += chunkSize) {
     // process chunk
   }
   ```

## Example: Image Processing

```typescript
async function grayscale(imageData: ImageData) {
  const simd = await WCN_SIMD.init();
  const pixels = imageData.data;
  
  // Allocate WASM memory
  const ptr = simd.malloc(pixels.length);
  const view = new Uint8ClampedArray(simd.memory.buffer, ptr, pixels.length);
  view.set(pixels);
  
  // Process with SIMD (implement your algorithm)
  // ...
  
  // Copy back
  pixels.set(view);
  simd.free(ptr);
}
```

## Browser Compatibility

Requires WebAssembly SIMD support:
- Chrome 91+
- Firefox 89+
- Safari 16.4+
- Edge 91+

Check support:
```typescript
const simd = await WCN_SIMD.init();
const features = simd.getFeatures();
console.log('WASM SIMD128:', features.has_wasm_simd128);
```

## License

See LICENSE file in the root directory.
