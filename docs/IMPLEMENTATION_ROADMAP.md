# WCN_SIMD 完整性改进路线图

> **📜 Historical Document**: This document describes the roadmap created during Phase 1.1/1.2 development.
> Many tasks described here have been completed. For current project status, see [roadmap.md](../roadmap.md) and [CHANGELOG.md](CHANGELOG.md).

## 概述
本文档记录了WCN_SIMD库的完整性改进计划，包括缺失功能的优先级和实施步骤。

## 已完成的工作 ✅

### SSE2扩展 (部分完成)
- ✅ 添加了8/16/64位整数算术运算
- ✅ 添加了饱和算术运算（adds/subs for i8, u8, i16, u16）
- ✅ 添加了完整的移位操作（sll, srl, sra）
- ✅ 添加了8/16位整数比较操作
- ✅ 添加了8/16位整数min/max操作
- ✅ 为SSE2提供了32位min/max的fallback实现

## 实施优先级

### 🔴 高优先级（立即实施）

#### 阶段1：完善128位基础操作 (进行中)
**目标**: 让所有平台都有完整的128位基础功能

##### 1.1 SSE2完善 (50%完成)
需要添加:
- [ ] 绝对值操作 (`abs_i8/i16/i32`)
- [ ] 取反操作 (`neg`)
- [ ] 倒数近似 (`rcp_f32`) - 使用 `_mm_rcp_ps`
- [ ] 平方根倒数近似 (`rsqrt_f32`) - 使用 `_mm_rsqrt_ps`
- [ ] 对齐的float/double load/store
- [ ] Set1操作的其他位宽 (`set1_i16`, `set1_i64`)
- [ ] Double类型的完整比较操作 (`cmpeq/lt/le/gt_f64`)
- [ ] Broadcast操作（从内存加载单个值并广播）

##### 1.2 ARM NEON对齐 (0%完成)
需要为NEON添加SSE2中新增的所有操作:
- [ ] 8/16/64位算术（部分已有）
- [ ] 饱和算术
- [ ] 移位操作
- [ ] 完整比较操作
- [ ] Min/Max所有位宽
- [ ] 绝对值、取反
- [ ] 倒数/平方根倒数近似

##### 1.3 其他平台对齐 (0%完成)
- [ ] WebAssembly SIMD128
- [ ] LoongArch LSX/LASX
- [ ] RISC-V RVV
- [ ] PowerPC AltiVec
- [ ] MIPS MSA

#### 阶段2：顶层统一接口扩展
**目标**: 暴露所有128位基础操作到顶层

需要在 `WCN_SIMD.h` 添加:
```c
/* 所有位宽的算术 */
#define wcn_simd_add_i8(a, b) wcn_v128i_add_i8((a), (b))
#define wcn_simd_add_i16(a, b) wcn_v128i_add_i16((a), (b))
#define wcn_simd_add_i64(a, b) wcn_v128i_add_i64((a), (b))
// ... sub, mul等

/* 饱和算术 */
#define wcn_simd_adds_i8(a, b) wcn_v128i_adds_i8((a), (b))
#define wcn_simd_adds_i16(a, b) wcn_v128i_adds_i16((a), (b))
// ... subs, unsigned versions

/* 移位操作 */
#define wcn_simd_sll_i16(a, count) wcn_v128i_sll_i16((a), (count))
#define wcn_simd_slli_i16(a, imm) wcn_v128i_slli_i16((a), (imm))
// ... srl, sra for all widths

/* 完整比较 */
#define wcn_simd_cmpeq_i8(a, b) wcn_v128i_cmpeq_i8((a), (b))
#define wcn_simd_cmpgt_i8(a, b) wcn_v128i_cmpgt_i8((a), (b))
#define wcn_simd_cmplt_i8(a, b) wcn_v128i_cmplt_i8((a), (b))
// ... i16, i64, u8, u16, u32, u64

/* 绝对值和符号 */
#define wcn_simd_abs_i8(a) wcn_v128i_abs_i8(a)
#define wcn_simd_abs_f32(a) wcn_v128f_abs(a)
// ...

/* 倒数和估算 */
#define wcn_simd_rcp_f32(a) wcn_v128f_rcp(a)
#define wcn_simd_rsqrt_f32(a) wcn_v128f_rsqrt(a)
```

### 🟡 中优先级（3-6个月）

#### 阶段3：256位和512位统一接口
**目标**: 让用户能够使用更宽的向量

##### 3.1 256位接口层
创建新的统一宏:
```c
/* Load/Store */
#define wcn_simd256_load_i(ptr) ...
#define wcn_simd256_store_i(ptr, vec) ...

/* Arithmetic */
#define wcn_simd256_add_i32(a, b) ...
#define wcn_simd256_mul_f32(a, b) ...

/* 在不支持的平台上自动降级到128位 */
```

实施策略:
- x86: 使用AVX/AVX2原生支持
- ARM: 使用两个128位向量模拟
- 其他平台: 根据能力选择原生或模拟

##### 3.2 512位接口层
- 主要针对AVX-512
- 其他平台使用模拟

#### 阶段4：高级SIMD操作

##### 4.1 水平运算
```c
/* SSE3/SSSE3 */
wcn_v128f_hadd(a, b)  // 水平加法
wcn_v128f_hsub(a, b)  // 水平减法
wcn_v128i_hadd_i16(a, b)
wcn_v128i_hadd_i32(a, b)
wcn_v128i_hadds_i16(a, b)  // 饱和版本
```

##### 4.2 Gather/Scatter操作
```c
/* AVX2 */
wcn_v256i_gather_i32(base, vindex, scale)
wcn_v256f_gather_f32(base, vindex, scale)

/* AVX-512 */
wcn_v512i_gather_i32(base, vindex, scale)
wcn_v512i_scatter_i32(base, vindex, vec, scale)

/* 其他平台使用标量fallback */
```

##### 4.3 Permute/Shuffle扩展
```c
wcn_v128f_shuffle_f32(a, b, control)
wcn_v128i_shuffle_i32(a, b, control)
wcn_v256f_permute(vec, control)
wcn_v256i_permute2x128(a, b, imm)
```

##### 4.4 Mask操作
```c
wcn_v128i_testz(a, b)      // Test if all bits are zero
wcn_v128i_testc(a, b)      // Test if all bits are one
wcn_v128i_testnzc(a, b)    // Test if mixed
```

##### 4.5 Prefetch和Cache控制
```c
wcn_prefetch_t0(addr)
wcn_prefetch_t1(addr)
wcn_prefetch_t2(addr)
wcn_prefetch_nta(addr)
wcn_clflush(addr)
```

### 🟢 低优先级（6-12个月）

#### 阶段5：AVX-512完整支持

##### 5.1 启用AVX-512F
- 移除 `WCN_SIMD.h:54-59` 的注释
- 测试基本操作
- 修复可能的bug

##### 5.2 实现AVX-512子集
按优先级顺序:
1. **AVX-512BW** (字节/字操作) - 创建 `wcn_x86_avx512bw.h`
2. **AVX-512DQ** (双精度/四字) - 创建 `wcn_x86_avx512dq.h`
3. **AVX-512VL** (128/256位支持) - 创建 `wcn_x86_avx512vl.h`
4. **AVX-512CD** (冲突检测) - 创建 `wcn_x86_avx512cd.h`
5. **AVX-512VNNI** (神经网络) - 创建 `wcn_x86_avx512vnni.h`
6. **AVX-512VBMI** (向量字节操作) - 创建 `wcn_x86_avx512vbmi.h`
7. **AVX-512IFMA** (整数FMA) - 创建 `wcn_x86_avx512ifma.h`
8. **AVX-512VPOPCNTDQ** (向量popcount) - 创建 `wcn_x86_avx512vpopcntdq.h`

##### 5.3 Mask寄存器支持
AVX-512引入了专用的mask寄存器(k0-k7):
```c
typedef struct {
    __mmask8 mask;
} wcn_mask8_t;

typedef struct {
    __mmask16 mask;
} wcn_mask16_t;

/* Masked operations */
wcn_v512i_add_i32_mask(src, mask, a, b)
wcn_v512f_load_mask(ptr, mask)
```

#### 阶段6：专用指令集扩展

##### 6.1 SSE4.2支持
创建 `wcn_x86_sse4_2.h`:
```c
/* String/Text processing */
wcn_v128i_cmpistrm(a, b, imm)
wcn_v128i_cmpistri(a, b, imm)
wcn_v128i_cmpestrm(a, la, b, lb, imm)
wcn_v128i_cmpestri(a, la, b, lb, imm)

/* CRC32 */
uint32_t wcn_crc32_u8(uint32_t crc, uint8_t v)
uint32_t wcn_crc32_u16(uint32_t crc, uint16_t v)
uint32_t wcn_crc32_u32(uint32_t crc, uint32_t v)
uint64_t wcn_crc32_u64(uint64_t crc, uint64_t v)
```

##### 6.2 AES-NI支持
创建 `wcn_x86_aes.h`:
```c
wcn_v128i_aesenc(a, roundkey)
wcn_v128i_aesenclast(a, roundkey)
wcn_v128i_aesdec(a, roundkey)
wcn_v128i_aesdeclast(a, roundkey)
wcn_v128i_aesimc(a)
wcn_v128i_aeskeygenassist(a, imm)
```

##### 6.3 SHA扩展
创建 `wcn_x86_sha.h`:
```c
/* SHA1 */
wcn_v128i_sha1nexte(a, b)
wcn_v128i_sha1msg1(a, b)
wcn_v128i_sha1msg2(a, b)
wcn_v128i_sha1rnds4(a, b, func)

/* SHA256 */
wcn_v128i_sha256rnds2(a, b, wk)
wcn_v128i_sha256msg1(a, b)
wcn_v128i_sha256msg2(a, b)
```

##### 6.4 PCLMULQDQ支持
创建 `wcn_x86_pclmul.h`:
```c
wcn_v128i_clmulepi64(a, b, imm)
```

#### 阶段7：ARM高级扩展

##### 7.1 ARM v8.2+ FP16支持
创建 `wcn_arm_fp16.h`:
```c
typedef struct {
    float16x8_t raw;
} wcn_v128h_t;

wcn_v128h_t wcn_v128h_add(wcn_v128h_t a, wcn_v128h_t b)
wcn_v128h_t wcn_v128h_mul(wcn_v128h_t a, wcn_v128h_t b)
// ...
```

##### 7.2 Dot Product扩展
创建 `wcn_arm_dotprod.h`:
```c
wcn_v128i_t wcn_v128i_dot_i8(wcn_v128i_t a, wcn_v128i_t b, wcn_v128i_t c)
wcn_v128i_t wcn_v128i_dot_u8(wcn_v128i_t a, wcn_v128i_t b, wcn_v128i_t c)
```

##### 7.3 INT8矩阵乘法 (i8mm)
创建 `wcn_arm_i8mm.h`:
```c
wcn_v128i_t wcn_v128i_matmul_i8(...)
```

##### 7.4 BFloat16支持
创建 `wcn_arm_bf16.h`:
```c
typedef struct {
    bfloat16x8_t raw;
} wcn_v128bf_t;
```

##### 7.5 ARM Crypto扩展
创建 `wcn_arm_crypto.h`:
```c
/* AES */
wcn_v128i_t wcn_v128i_aese(wcn_v128i_t data, wcn_v128i_t key)
wcn_v128i_t wcn_v128i_aesd(wcn_v128i_t data, wcn_v128i_t key)
wcn_v128i_t wcn_v128i_aesmc(wcn_v128i_t data)
wcn_v128i_t wcn_v128i_aesimc(wcn_v128i_t data)

/* SHA1 */
uint32_t wcn_sha1c(uint32_t hash_e, uint32x4_t hash_abcd, uint32x4_t wk)
uint32_t wcn_sha1p(uint32_t hash_e, uint32x4_t hash_abcd, uint32x4_t wk)
// ...

/* SHA256 */
uint32x4_t wcn_sha256h(uint32x4_t hash_abcd, uint32x4_t hash_efgh, uint32x4_t wk)
uint32x4_t wcn_sha256h2(uint32x4_t hash_efgh, uint32x4_t hash_abcd, uint32x4_t wk)
// ...

/* PMULL */
wcn_v128i_t wcn_v128i_pmull(wcn_v128i_t a, wcn_v128i_t b)
wcn_v128i_t wcn_v128i_pmull_high(wcn_v128i_t a, wcn_v128i_t b)
```

#### 阶段8：统一原子操作接口

##### 8.1 设计统一API
创建 `wcn_atomic_unified.h`:
```c
/* Atomic loads/stores */
wcn_v128i_t wcn_atomic_load_v128i(const void* ptr)
void wcn_atomic_store_v128i(void* ptr, wcn_v128i_t vec)

/* Atomic RMW operations */
wcn_v128i_t wcn_atomic_fetch_add_v128i(void* ptr, wcn_v128i_t val)
wcn_v128i_t wcn_atomic_fetch_and_v128i(void* ptr, wcn_v128i_t val)
// ...

/* Compare-and-swap */
int wcn_atomic_cas_v128i(void* ptr, wcn_v128i_t expected, wcn_v128i_t desired)
```

##### 8.2 实现MIPS MSA原子操作
创建 `wcn_mips_msa_atomic.h`

##### 8.3 为其他SSE变种添加原子操作
- `wcn_x86_avx_atomic.h`
- `wcn_x86_sse3_atomic.h`
- `wcn_x86_ssse3_atomic.h`
- `wcn_x86_sse4_1_atomic.h`

#### 阶段9：新架构支持

##### 9.1 IBM S390X支持
创建 `platform/s390x/` 目录:
- `wcn_s390x_vector.h` - Vector Facility
- `wcn_s390x_vector_enhanced.h` - Vector Enhancements

##### 9.2 未来RISC-V扩展
监控并支持:
- RISC-V Vector Crypto扩展
- RISC-V Matrix扩展（当标准化后）

#### 阶段10：优化和工具

##### 10.1 性能优化工具
创建 `wcn_perf.h`:
```c
/* CPU Feature runtime selection */
typedef enum {
    WCN_PATH_SCALAR,
    WCN_PATH_SSE2,
    WCN_PATH_AVX2,
    WCN_PATH_AVX512,
    WCN_PATH_NEON,
    // ...
} wcn_execution_path_t;

wcn_execution_path_t wcn_get_best_path(void);
void wcn_set_execution_path(wcn_execution_path_t path);

/* Alignment helpers */
void* wcn_malloc_aligned(size_t size, size_t alignment);
void wcn_free_aligned(void* ptr);
int wcn_is_aligned(const void* ptr, size_t alignment);
```

##### 10.2 诊断和调试
创建 `wcn_debug.h`:
```c
/* Vector print functions */
void wcn_print_v128i(wcn_v128i_t vec, const char* format);
void wcn_print_v128f(wcn_v128f_t vec);

/* Performance counters */
void wcn_perf_start(void);
uint64_t wcn_perf_end(void);
```

## 测试策略

### 单元测试
为每个新增操作创建测试:
```
tests/
├── test_v128i_arithmetic.c
├── test_v128i_shift.c
├── test_v128i_compare.c
├── test_v128f_advanced.c
├── test_v256_ops.c
├── test_v512_ops.c
├── test_atomic.c
├── test_crypto.c
└── test_cross_platform.c
```

### 性能基准测试
创建 `benchmarks/` 目录:
```
benchmarks/
├── bench_dot_product.c
├── bench_matrix_multiply.c
├── bench_gather_scatter.c
├── bench_crypto.c
└── bench_memory.c
```

### 跨平台验证
确保所有平台的行为一致:
- 相同输入产生相同输出
- 边界情况处理一致
- NaN/Inf处理符合IEEE 754

## 文档计划

### API文档
为每个操作创建详细文档:
- 功能描述
- 参数说明
- 返回值
- 支持的平台
- 性能特征
- 使用示例
- 注意事项

### 迁移指南
为从其他SIMD库迁移的用户创建指南:
- `MIGRATION_FROM_INTEL_INTRINSICS.md`
- `MIGRATION_FROM_ARM_NEON.md`
- `MIGRATION_FROM_SIMDE.md`

### 最佳实践指南
- `BEST_PRACTICES.md` - 使用WCN_SIMD的最佳实践
- `PERFORMANCE_GUIDE.md` - 性能优化技巧
- `PORTABILITY_GUIDE.md` - 编写跨平台代码的指南

## 预期时间线

### 短期（1-2个月）
- ✅ 完成SSE2基础操作扩展
- ⏳ ARM NEON对齐
- ⏳ 更新顶层统一接口
- ⏳ 基本的单元测试

### 中期（3-6个月）
- 256位接口实现
- 高级SIMD操作（水平运算、Gather/Scatter）
- SSE4.2、AES-NI基础支持
- 完善的测试覆盖

### 长期（6-12个月）
- AVX-512完整支持
- ARM高级扩展
- 统一原子操作
- 完整文档和示例
- 性能基准测试套件

## 贡献指南

### 添加新操作的步骤
1. 在对应平台头文件中实现操作
2. 为其他平台提供fallback实现（如需要）
3. 更新顶层 `WCN_SIMD.h`
4. 编写单元测试
5. 更新文档
6. 提交Pull Request

### 代码规范
- 使用 `WCN_INLINE` 标记内联函数
- 遵循现有命名约定: `wcn_v<width><type>_<operation>`
- 为MSVC提供宏版本（当需要编译时常量时）
- 添加详细注释说明操作行为
- 包含平台特定的fallback实现

### 测试要求
- 所有新操作必须有单元测试
- 测试覆盖正常情况和边界情况
- 验证跨平台一致性
- 性能回归测试（对关键操作）

## 当前状态总结

### 完成度
- **128位基础操作**: 45% → 60%（本次更新）
- **256位操作**: 5%
- **512位操作**: 2%
- **高级特性**: 15%
- **专用指令**: 5%
- **文档**: 20%

### 下一步行动
1. 完成SSE2的剩余基础操作（绝对值、倒数等）
2. 为ARM NEON实现相同的操作集
3. 更新 `WCN_SIMD.h` 暴露新操作
4. 编写基础单元测试
5. 继续256位接口设计

---

**最后更新**: 2025-10-14
**维护者**: WCN Development Team
