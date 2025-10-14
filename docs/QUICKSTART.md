# WCN_SIMD Quick Start Guide

Get up and running with WCN_SIMD in 5 minutes!

## 🚀 Installation & Build (30 seconds)

### Windows
```batch
# Clone the repository
git clone <repository-url>
cd WCN_SIMD

# Build (uses Ninja for fast builds)
build.bat

# Done! Binary is at: build\bin\wcn_simd_example.exe
```

### Linux/macOS
```bash
# Clone the repository
git clone <repository-url>
cd WCN_SIMD

# Build
chmod +x build.sh
./build.sh

# Done! Binary is at: build/bin/wcn_simd_example
```

## 📝 Basic Usage (2 minutes)

> **🎉 Phase 1.2 Update**: WCN_SIMD now provides both **high-level array algorithms** and **low-level unified vector operations**. Choose the API that fits your needs!

### High-Level Array Operations (Recommended for Most Users)

```c
#include <WCN_SIMD.h>
#include <stdio.h>

int main(void) {
    // Initialize library (detects CPU features)
    wcn_simd_init();

    // Print what SIMD we're using
    printf("Using: %s\n", wcn_simd_get_impl());
    printf("Vector width: %d bits\n", wcn_simd_get_vector_width());

    // Create some data
    float a[100] = {1.0f, 2.0f, 3.0f, /* ... */};
    float b[100] = {0.5f, 1.5f, 2.5f, /* ... */};
    float result[100];

    // High-level array operations (automatically uses best SIMD)
    wcn_simd_add_array_f32(a, b, result, 100);  // result = a + b
    float dot = wcn_simd_dot_product_f32(a, b, 100);  // a·b
    float max = wcn_simd_reduce_max_f32(a, 100);     // max(a)

    printf("Dot product: %.2f\n", dot);
    printf("Max value: %.2f\n", max);

    return 0;
}
```

### Low-Level Unified Vector Operations (Phase 1.2 New Feature)

For performance-critical code, use the **unified vector API** that works identically on **all platforms** (SSE2, NEON, LSX, AltiVec, MSA, WASM):

```c
#include <WCN_SIMD.h>
#include <stdio.h>

int main(void) {
    wcn_simd_init();
    printf("Using: %s\n", wcn_simd_get_impl());

    // Allocate aligned arrays (better performance)
    float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float b[4] = {5.0f, 6.0f, 7.0f, 8.0f};
    float result[4];

    // Load vectors using unified API
    wcn_v128f_t va = wcn_simd_load_f32(a);
    wcn_v128f_t vb = wcn_simd_load_f32(b);

    // Perform operations - this code works on ALL platforms!
    wcn_v128f_t vresult = wcn_simd_add_f32(va, vb);    // Vector add
    vresult = wcn_simd_mul_f32(vresult, wcn_simd_set1_f32(2.0f));  // Multiply by 2

    // Store result
    wcn_simd_store_f32(result, vresult);

    // Print results
    printf("Results: %.1f, %.1f, %.1f, %.1f\n",
           result[0], result[1], result[2], result[3]);

    return 0;
}
```

**Why use the unified vector API?**
- ✅ **Cross-platform**: Same code works on x86, ARM, LoongArch, PowerPC, MIPS, and WebAssembly
- ✅ **Zero overhead**: Compiles directly to hardware instructions
- ✅ **94+ operations**: Complete set of SIMD operations available
- ✅ **Type safety**: Wrapper types prevent accidental misuse

### Compile Your Code

**Using CMake (Recommended):**

Create `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyApp C)

# Find WCN_SIMD (if installed)
find_package(WCN_SIMD REQUIRED)

add_executable(myapp main.c)
target_link_libraries(myapp PRIVATE WCN_SIMD::WCN_SIMD)
```

Or add as subdirectory:
```cmake
add_subdirectory(WCN_SIMD)
add_executable(myapp main.c)
target_link_libraries(myapp PRIVATE WCN_SIMD)
```

**Using GCC directly:**
```bash
gcc main.c -I./WCN_SIMD/include -L./WCN_SIMD/build -lWCN_SIMD -o myapp
```

## 🎯 Common Operations

### High-Level Array Operations

```c
// Addition: c = a + b
wcn_simd_add_array_f32(a, b, c, count);

// Multiplication: c = a * b
wcn_simd_mul_array_f32(a, b, c, count);

// Scalar multiply: b = a * 2.5
wcn_simd_scale_array_f32(a, 2.5f, b, count);

// Fused multiply-add: c = a * b + c
wcn_simd_fmadd_array_f32(a, b, c, count);

// Reductions
float dot = wcn_simd_dot_product_f32(a, b, count);
float sum = wcn_simd_reduce_sum_f32(data, count);
float max = wcn_simd_reduce_max_f32(data, count);
float min = wcn_simd_reduce_min_f32(data, count);
```

### Low-Level Vector Operations (Phase 1.2 Unified API)

**94+ operations available** - Here are the most commonly used:

**Load/Store:**
```c
wcn_v128i_t vi = wcn_simd_load_i128(int_ptr);     // Load 128-bit integer
wcn_v128f_t vf = wcn_simd_load_f32(float_ptr);    // Load 4 floats
wcn_v128d_t vd = wcn_simd_load_f64(double_ptr);   // Load 2 doubles
wcn_simd_store_i128(ptr, vi);                     // Store integer vector
wcn_simd_store_f32(ptr, vf);                      // Store float vector
```

**Initialization:**
```c
wcn_v128i_t zeros = wcn_simd_setzero_i128();      // All zeros
wcn_v128i_t fours = wcn_simd_set1_i32(4);         // All lanes = 4
wcn_v128f_t twos = wcn_simd_set1_f32(2.0f);       // All lanes = 2.0
```

**Arithmetic (8/16/32/64-bit):**
```c
// Integer operations
wcn_v128i_t sum = wcn_simd_add_i32(a, b);         // 32-bit int add
wcn_v128i_t diff = wcn_simd_sub_i16(a, b);        // 16-bit int subtract
wcn_v128i_t prod = wcn_simd_mullo_i32(a, b);      // 32-bit int multiply

// Float operations
wcn_v128f_t sum = wcn_simd_add_f32(a, b);         // Float add
wcn_v128f_t prod = wcn_simd_mul_f32(a, b);        // Float multiply
wcn_v128f_t fma = wcn_simd_fmadd_f32(a, b, c);    // a * b + c

// Double operations
wcn_v128d_t sum = wcn_simd_add_f64(a, b);         // Double add
```

**Saturating Arithmetic:**
```c
wcn_v128i_t sat_sum = wcn_simd_adds_i8(a, b);     // Saturating 8-bit add
wcn_v128i_t sat_diff = wcn_simd_subs_u16(a, b);   // Saturating unsigned 16-bit sub
```

**Comparisons:**
```c
wcn_v128i_t eq = wcn_simd_cmpeq_i32(a, b);        // a == b (per lane)
wcn_v128i_t gt = wcn_simd_cmpgt_i32(a, b);        // a > b
wcn_v128f_t lt = wcn_simd_cmplt_f32(a, b);        // a < b (float)
```

**Logical Operations:**
```c
wcn_v128i_t result = wcn_simd_and_i128(a, b);     // Bitwise AND
wcn_v128i_t result = wcn_simd_or_i128(a, b);      // Bitwise OR
wcn_v128i_t result = wcn_simd_xor_i128(a, b);     // Bitwise XOR
```

**Shifts:**
```c
wcn_v128i_t shifted = wcn_simd_slli_i32(a, 2);    // Left shift by 2
wcn_v128i_t shifted = wcn_simd_srli_i32(a, 3);    // Logical right shift by 3
wcn_v128i_t shifted = wcn_simd_srai_i16(a, 1);    // Arithmetic right shift by 1
```

**Min/Max:**
```c
wcn_v128i_t min = wcn_simd_min_i32(a, b);         // Minimum (signed 32-bit)
wcn_v128f_t max = wcn_simd_max_f32(a, b);         // Maximum (float)
```

**Math Functions:**
```c
wcn_v128f_t sq = wcn_simd_sqrt_f32(a);            // Square root
wcn_v128f_t rcp = wcn_simd_rcp_f32(a);            // Reciprocal (1/x)
wcn_v128i_t abs = wcn_simd_abs_i32(a);            // Absolute value
wcn_v128f_t neg = wcn_simd_neg_f32(a);            // Negation
```

**See [roadmap.md](../roadmap.md) for complete list of 94+ operations!**

### Feature Detection
```c
const wcn_simd_features_t* features = wcn_simd_get_features();

if (features->has_avx2) {
    printf("AVX2 available!\n");
}
if (features->has_neon) {
    printf("ARM NEON available!\n");
}
```

## 💡 Performance Tips

### 1. Enable Native Optimizations
```bash
# When building, use:
cmake -DWCN_SIMD_ENABLE_NATIVE=ON ..
```

### 2. Use Aligned Data (Optional)
```c
// Allocate aligned memory for best performance
float* data = aligned_alloc(32, size * sizeof(float));
wcn_simd_add_f32(data, other, result, size);
free(data);
```

### 3. Process in Bulk
```c
// Good: Process 10000 elements at once
wcn_simd_add_f32(a, b, c, 10000);

// Bad: Call for each element
for (int i = 0; i < 10000; i++) {
    wcn_simd_add_f32(&a[i], &b[i], &c[i], 1);
}
```

## 🔍 Checking Performance

### Run Built-in Benchmark
```bash
./build/bin/wcn_simd_example
```

Output shows speedup vs scalar code:
```
=== Dot Product Benchmark ===
SIMD result: 2504.46 (0.0010 seconds)
Scalar result: 2504.46 (0.0080 seconds)
Speedup: 8.00x  ← SIMD is 8x faster!
```

### Measure Your Own Code
```c
#include <time.h>

clock_t start = clock();
// Your SIMD code here
clock_t end = clock();

double time_sec = (double)(end - start) / CLOCKS_PER_SEC;
printf("Time: %.4f seconds\n", time_sec);
```

## 📚 Platform Support

| Platform | Status | Implementation | Operations |
|----------|--------|----------------|------------|
| x86_64 (SSE2/AVX2) | ✅ Complete | SSE2 → AVX2 | 94+ |
| ARM64 (NEON) | ✅ Complete | NEON | 94+ |
| LoongArch (LSX) | ✅ Complete | LSX | 94+ |
| PowerPC (AltiVec) | ✅ Complete | AltiVec/VSX | 94+ |
| MIPS (MSA) | ✅ Complete | MSA | 94+ |
| WebAssembly | ✅ Complete | SIMD128 | 94+ |

**Achievement**: 100% API parity across all platforms! Every operation works identically everywhere.

## 🛠️ Troubleshooting

### "Ninja not found"
```bash
# Option 1: Install Ninja
choco install ninja  # Windows
brew install ninja   # macOS
sudo apt install ninja-build  # Linux

# Option 2: Use default build system
cmake ..  # Without -G Ninja
```

### "CMake not found"
```bash
# Download from https://cmake.org/download/
# Or use package manager
choco install cmake  # Windows
brew install cmake   # macOS
sudo apt install cmake  # Linux
```

### "No SIMD detected"
```c
// Check what's available
wcn_simd_init();
printf("Implementation: %s\n", wcn_simd_get_impl());
printf("Vector width: %d bits\n", wcn_simd_get_vector_width());

// If says "scalar", your CPU may not support SIMD
// or compiler flags are wrong
```

### Build fails with "unrecognized option"
```bash
# Disable native optimizations
cmake -DWCN_SIMD_ENABLE_NATIVE=OFF ..
```

## 🎓 Next Steps

1. **Explore the unified API**: Check [roadmap.md](../roadmap.md) for all 94+ operations
2. **Learn about build systems**: See [BUILDING.md](BUILDING.md)
3. **Compare performance**: See [BUILD_COMPARISON.md](BUILD_COMPARISON.md)
4. **Check examples**: Look in `examples/main.c`
5. **Read implementation details**: Check [README.md](../README.md)
6. **Explore platform-specific code**: Check `include/wcn_simd/platform/`

## 💬 Getting Help

- Check [BUILDING.md](BUILDING.md) for detailed build instructions
- See [README.md](../README.md) for complete API reference
- Read [roadmap.md](../roadmap.md) for development status and future plans
- Review example in `examples/main.c`

## 🎉 You're Ready!

That's it! You now have a high-performance SIMD library ready to use. Start accelerating your code!

---

**Quick Command Reference:**

```bash
# Build
./build.sh          # Linux/macOS
build.bat           # Windows

# Clean build
./build.sh clean
build.bat clean

# Debug build
./build.sh debug
build.bat debug

# Run example
./build/bin/wcn_simd_example
.\build\bin\wcn_simd_example.exe
```

Happy optimizing! 🚀
