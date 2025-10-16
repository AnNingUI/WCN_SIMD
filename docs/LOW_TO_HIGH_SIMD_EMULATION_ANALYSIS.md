# 用低位SIMD模拟高位SIMD的可行性分析

## 概述

本文档探讨使用低位SIMD（如128-bit SSE2/NEON）模拟高位SIMD（如256-bit AVX2、512-bit AVX-512）来解决跨平台映射问题的可行性。

---

## 核心思路

### 基本原理

```
高位SIMD = 多个低位SIMD并行执行

例如：256-bit AVX = 2 × 128-bit SSE
     512-bit AVX-512 = 4 × 128-bit SSE
```

### 示例：AVX加法用SSE模拟

```c
// 原始AVX代码（一条指令）
__m256 avx_add(__m256 a, __m256 b) {
    return _mm256_add_ps(a, b);  // 8个float同时相加
}

// SSE模拟（两条指令）
__m256 sse_emulate_avx_add(__m256 a, __m256 b) {
    __m128 a_lo = _mm256_extractf128_ps(a, 0);  // 低128位
    __m128 a_hi = _mm256_extractf128_ps(a, 1);  // 高128位
    __m128 b_lo = _mm256_extractf128_ps(b, 0);
    __m128 b_hi = _mm256_extractf128_ps(b, 1);
    
    __m128 res_lo = _mm_add_ps(a_lo, b_lo);     // SSE加法
    __m128 res_hi = _mm_add_ps(a_hi, b_hi);     // SSE加法
    
    return _mm256_insertf128_ps(
        _mm256_castps128_ps256(res_lo), res_hi, 1
    );
}
```

---

## 可行性分析：按操作类型

### ✅ **完全可行** (性能损失 <20%)

#### 1. **基础算术操作** (加减乘除)

**可行性**: ⭐⭐⭐⭐⭐ (100%)

```c
// AVX2: 256-bit整数加法
__m256i avx2_add_epi32(__m256i a, __m256i b);

// SSE2模拟
struct v256i_sse2 {
    __m128i lo, hi;
};

v256i_sse2 sse2_emulate_add_epi32(v256i_sse2 a, v256i_sse2 b) {
    v256i_sse2 result;
    result.lo = _mm_add_epi32(a.lo, b.lo);  // 1条SSE指令
    result.hi = _mm_add_epi32(a.hi, b.hi);  // 1条SSE指令
    return result;  // 总共2条指令 vs AVX的1条
}
```

**性能分析**:
- **理论开销**: 2x指令数
- **实际开销**: ~10-15%（因为现代CPU超标量执行）
- **内存带宽**: 相同（都是加载256位数据）

**结论**: ✅ 非常适合模拟，性能损失可接受

---

#### 2. **Load/Store操作**

**可行性**: ⭐⭐⭐⭐⭐ (100%)

```c
// AVX: 256-bit加载
__m256 avx_load(const float* ptr);

// SSE模拟
v256f_sse sse_emulate_load(const float* ptr) {
    v256f_sse result;
    result.lo = _mm_load_ps(ptr);       // 加载前128位
    result.hi = _mm_load_ps(ptr + 4);   // 加载后128位
    return result;
}
```

**性能分析**:
- **内存访问次数**: 相同（都是读取256位）
- **额外开销**: 几乎为0（只是拆分地址）
- **缓存行为**: 相同

**结论**: ✅ 完全可行，几乎无性能损失

---

#### 3. **逻辑操作** (AND/OR/XOR)

**可行性**: ⭐⭐⭐⭐⭐ (100%)

```c
// AVX2: 256-bit AND
__m256i avx2_and(__m256i a, __m256i b);

// SSE2模拟
v256i_sse2 sse2_emulate_and(v256i_sse2 a, v256i_sse2 b) {
    v256i_sse2 result;
    result.lo = _mm_and_si128(a.lo, b.lo);
    result.hi = _mm_and_si128(a.hi, b.hi);
    return result;
}
```

**结论**: ✅ 完全可行，性能损失 <10%

---

#### 4. **比较操作**

**可行性**: ⭐⭐⭐⭐⭐ (100%)

```c
// AVX2: 256-bit比较
__m256i avx2_cmpeq_epi32(__m256i a, __m256i b);

// SSE2模拟
v256i_sse2 sse2_emulate_cmpeq_epi32(v256i_sse2 a, v256i_sse2 b) {
    v256i_sse2 result;
    result.lo = _mm_cmpeq_epi32(a.lo, b.lo);
    result.hi = _mm_cmpeq_epi32(a.hi, b.hi);
    return result;
}
```

**结论**: ✅ 完全可行

---

### ⚠️ **部分可行** (性能损失 20-50%)

#### 5. **Permute/Shuffle跨通道操作**

**可行性**: ⭐⭐⭐ (60%)

```c
// AVX2: 跨128位边界的shuffle
__m256i avx2_permute4x64(__m256i a, int imm);

// SSE2模拟（复杂）
v256i_sse2 sse2_emulate_permute4x64(v256i_sse2 a, int imm) {
    // 需要在lo和hi之间交换数据
    v256i_sse2 result;
    
    // 根据imm提取不同的64位块
    // 可能需要额外的shuffle和blend指令
    // 最坏情况：4-8条SSE指令
    
    result.lo = /* 复杂的shuffle组合 */;
    result.hi = /* 复杂的shuffle组合 */;
    return result;
}
```

**性能分析**:
- **指令数**: AVX 1条 → SSE 4-8条
- **额外开销**: ~30-50%
- **复杂度**: 高（需要处理跨通道数据移动）

**结论**: ⚠️ 可行但复杂，性能损失显著

---

#### 6. **水平操作** (Horizontal Add/Sub)

**可行性**: ⭐⭐⭐ (60%)

```c
// AVX: 256-bit水平加法
__m256 avx_hadd_ps(__m256 a, __m256 b);
// 输入: a=[a0,a1,a2,a3, a4,a5,a6,a7]
//       b=[b0,b1,b2,b3, b4,b5,b6,b7]
// 输出: [a0+a1, a2+a3, b0+b1, b2+b3,
//        a4+a5, a6+a7, b4+b5, b6+b7]

// SSE3模拟
v256f_sse sse_emulate_hadd_ps(v256f_sse a, v256f_sse b) {
    v256f_sse result;
    result.lo = _mm_hadd_ps(a.lo, b.lo);  // SSE3 hadd
    result.hi = _mm_hadd_ps(a.hi, b.hi);  // SSE3 hadd
    return result;
}

// 如果只有SSE2（无hadd指令）
v256f_sse sse2_emulate_hadd_ps(v256f_sse a, v256f_sse b) {
    // 需要用shuffle + add模拟hadd
    // 每个hadd需要2-3条指令
    // 总共4-6条SSE2指令
    v256f_sse result;
    result.lo = /* 3条SSE2指令模拟hadd */;
    result.hi = /* 3条SSE2指令模拟hadd */;
    return result;
}
```

**性能分析**:
- **有SSE3**: 性能损失 ~15%
- **仅SSE2**: 性能损失 ~40-50%

**结论**: ⚠️ 可行，但SSE2需要较多指令

---

### ❌ **不可行/效率极低** (性能损失 >50%)

#### 7. **Gather操作** (间接索引加载)

**可行性**: ⭐ (20%)

```c
// AVX2: Gather（一条指令）
__m256i avx2_gather_epi32(const int* base, __m256i indices, int scale);
// 等价于: result[i] = base[indices[i] * scale]

// SSE2模拟（标量回退）
v256i_sse2 sse2_emulate_gather_epi32(
    const int* base, v256i_sse2 indices, int scale
) {
    // 必须退化为标量操作！
    int idx[8];
    int result[8];
    
    // 提取索引（需要store到内存）
    _mm_storeu_si128((__m128i*)&idx[0], indices.lo);
    _mm_storeu_si128((__m128i*)&idx[4], indices.hi);
    
    // 标量gather（8次内存随机访问）
    for (int i = 0; i < 8; i++) {
        result[i] = base[idx[i] * scale];
    }
    
    // 重新加载
    v256i_sse2 res;
    res.lo = _mm_loadu_si128((__m128i*)&result[0]);
    res.hi = _mm_loadu_si128((__m128i*)&result[4]);
    return res;
}
```

**性能分析**:
- **AVX2 Gather**: ~10 cycles（硬件优化）
- **SSE2 模拟**: ~80-100 cycles（8次随机内存访问）
- **性能损失**: 8-10x ❌

**结论**: ❌ 不可行，完全失去SIMD优势

---

#### 8. **掩码操作** (AVX-512特性)

**可行性**: ⭐ (10%)

```c
// AVX-512: 掩码加法
__m512i avx512_mask_add_epi32(__m512i src, __mmask16 k, 
                              __m512i a, __m512i b);
// 只对mask为1的元素执行加法

// SSE2模拟
v512i_sse2 sse2_emulate_mask_add(
    v512i_sse2 src, uint16_t mask,
    v512i_sse2 a, v512i_sse2 b
) {
    // 需要为4个128位块分别处理16位mask
    v512i_sse2 result;
    
    for (int block = 0; block < 4; block++) {
        __m128i block_mask = /* 从mask提取4位并扩展 */;
        __m128i sum = _mm_add_epi32(a.blocks[block], b.blocks[block]);
        result.blocks[block] = _mm_blendv_epi8(
            src.blocks[block], sum, block_mask
        );
    }
    
    // 需要大量指令（>20条）
    return result;
}
```

**性能分析**:
- **AVX-512**: 1条指令
- **SSE2**: 20+条指令
- **性能损失**: 10-20x ❌

**结论**: ❌ 严重不可行

---

#### 9. **向量化分支** (AVX-512特性)

**可行性**: ⭐ (5%)

```c
// AVX-512: 向量化条件执行
__mmask16 k = _mm512_cmp_epi32_mask(a, b, _MM_CMPINT_LT);
if (_mm512_kortestz(k, k) == 0) {  // 如果任意元素<
    // 向量化路径A
} else {
    // 向量化路径B
}

// SSE2模拟
// 无法高效模拟，必须转为标量分支或全执行两路径
```

**结论**: ❌ 无法有效模拟

---

## 跨平台映射策略矩阵

### 操作类型 vs 模拟可行性

| 操作类型 | 模拟可行性 | 性能损失 | 推荐策略 |
|---------|-----------|---------|---------|
| **基础算术** | ✅ 100% | <15% | **直接模拟** |
| **Load/Store** | ✅ 100% | <5% | **直接模拟** |
| **逻辑操作** | ✅ 100% | <10% | **直接模拟** |
| **比较操作** | ✅ 100% | <15% | **直接模拟** |
| **Min/Max** | ✅ 100% | <15% | **直接模拟** |
| **位移操作** | ✅ 100% | <15% | **直接模拟** |
| **Pack/Unpack** | ✅ 90% | <20% | **直接模拟** |
| **Shuffle（通道内）** | ✅ 85% | <25% | **直接模拟** |
| **Shuffle（跨通道）** | ⚠️ 60% | 30-50% | **条件模拟** |
| **水平操作** | ⚠️ 60% | 20-50% | **条件模拟** |
| **FMA** | ✅ 95% | <10% | **直接模拟** (mul+add) |
| **Blend/Select** | ✅ 90% | <20% | **直接模拟** |
| **Broadcast** | ✅ 100% | <10% | **直接模拟** |
| **Extract/Insert** | ✅ 95% | <15% | **直接模拟** |
| **Gather** | ❌ 20% | 800-1000% | **标量回退** |
| **Scatter** | ❌ 15% | 1000%+ | **标量回退** |
| **掩码操作** | ❌ 10% | 1000%+ | **不提供** |
| **向量化分支** | ❌ 5% | N/A | **不提供** |

---

## 实现方案设计

### 方案A: 结构体模拟（推荐）

```c
// 用结构体表示高位SIMD
typedef struct {
    wcn_v128i_t lo;  // 低128位
    wcn_v128i_t hi;  // 高128位
} wcn_v256i_t;

typedef struct {
    wcn_v128i_t part[4];  // 4个128位块
} wcn_v512i_t;

// 统一API
#define wcn_simd_add_v256i(a, b) \
    ((wcn_v256i_t){ \
        .lo = wcn_simd_add_i32((a).lo, (b).lo), \
        .hi = wcn_simd_add_i32((a).hi, (b).hi)  \
    })

// AVX2平台直接使用原生指令
#ifdef WCN_X86_AVX2
typedef __m256i wcn_v256i_native_t;
#define wcn_v256i_to_native(v) _mm256_setr_m128i((v).lo, (v).hi)
#define wcn_v256i_from_native(n) ((wcn_v256i_t){ \
    .lo = _mm256_extracti128_si256(n, 0), \
    .hi = _mm256_extracti128_si256(n, 1)  \
})
#endif
```

**优点**:
- ✅ 类型安全
- ✅ 编译器可以优化
- ✅ AVX2平台可以零开销切换到原生类型
- ✅ API统一

**缺点**:
- ⚠️ 代码量增加
- ⚠️ 调试稍复杂

---

### 方案B: 宏展开（高性能）

```c
// 为每个操作定义模拟版本
#ifdef WCN_HAS_AVX2
#define WCN_V256_ADD(a, b) _mm256_add_epi32((a), (b))
#else
#define WCN_V256_ADD(a, b) \
    _mm256_setr_m128i( \
        _mm_add_epi32(_mm256_extracti128_si256(a, 0), \
                      _mm256_extracti128_si256(b, 0)), \
        _mm_add_epi32(_mm256_extracti128_si256(a, 1), \
                      _mm256_extracti128_si256(b, 1))  \
    )
#endif
```

**优点**:
- ✅ 零运行时开销
- ✅ 编译时选择最优路径

**缺点**:
- ❌ 宏调试困难
- ❌ 编译错误信息不友好

---

### 方案C: 运行时分发（灵活）

```c
// 函数指针表
typedef wcn_v256i_t (*v256_add_fn)(wcn_v256i_t, wcn_v256i_t);

v256_add_fn wcn_v256_add_impl;

// SSE2实现
wcn_v256i_t wcn_v256_add_sse2(wcn_v256i_t a, wcn_v256i_t b) {
    return (wcn_v256i_t){
        .lo = wcn_simd_add_i32(a.lo, b.lo),
        .hi = wcn_simd_add_i32(a.hi, b.hi)
    };
}

// AVX2实现
wcn_v256i_t wcn_v256_add_avx2(wcn_v256i_t a, wcn_v256i_t b) {
    __m256i native_a = wcn_v256i_to_native(a);
    __m256i native_b = wcn_v256i_to_native(b);
    return wcn_v256i_from_native(_mm256_add_epi32(native_a, native_b));
}

// 初始化时检测
void wcn_simd_init() {
    if (wcn_cpu_has_avx2()) {
        wcn_v256_add_impl = wcn_v256_add_avx2;
    } else {
        wcn_v256_add_impl = wcn_v256_add_sse2;
    }
}

// 统一调用
#define wcn_v256_add(a, b) wcn_v256_add_impl((a), (b))
```

**优点**:
- ✅ 运行时自适应
- ✅ 同一二进制支持多平台
- ✅ 易于添加新平台

**缺点**:
- ⚠️ 函数调用开销（~2-3 cycles）
- ⚠️ 内存占用（函数指针表）

---

## 性能对比

### 实测数据（假设）

以向量加法为例（1000万次迭代）:

| 平台 | 实现方式 | 耗时 | 相对性能 |
|------|---------|------|---------|
| **AVX2 (原生)** | `_mm256_add_epi32` | 100ms | 1.00x ✅ |
| **SSE2 模拟** | 2×`_mm_add_epi32` | 112ms | 0.89x ⚠️ |
| **标量** | 8次标量加法 | 450ms | 0.22x ❌ |

**结论**: 模拟方案性能是标量的4倍，是AVX2的89%

---

## 适用场景

### ✅ **推荐使用模拟的场景**

1. **跨平台库开发**
   - 一套代码适配多平台
   - 低端设备也能用（虽然慢一点）

2. **基础算术密集型**
   - 图像处理（像素加减乘除）
   - 音频处理（采样点运算）
   - 矩阵运算（基础乘加）

3. **对开发效率要求高**
   - 不想维护多个平台特定版本
   - 性能损失10-20%可接受

### ❌ **不推荐使用模拟的场景**

1. **Gather/Scatter密集型**
   - 稀疏矩阵运算
   - 间接索引访问

2. **极致性能要求**
   - 高频交易系统
   - 实时游戏引擎核心循环
   - 科学计算关键路径

3. **使用AVX-512特性**
   - 掩码向量化
   - 512位操作

---

## 最佳实践建议

### 1. **分层设计**

```
┌─────────────────────────────────┐
│  应用层 API (wcn_simd_*)        │  ← 用户使用
├─────────────────────────────────┤
│  256/512-bit 抽象层 (v256/v512) │  ← 模拟层
├─────────────────────────────────┤
│  128-bit 基础层 (v128)          │  ← 完全跨平台
└─────────────────────────────────┘
```

### 2. **按需启用**

```c
// 默认只提供128-bit API
#include <WCN_SIMD.h>

// 显式启用256-bit模拟
#define WCN_ENABLE_V256_EMULATION
#include <WCN_SIMD_v256.h>
```

### 3. **提供性能提示**

```c
// 编译时警告
#if !defined(WCN_X86_AVX2) && defined(WCN_USING_V256)
#warning "Using v256 emulation on SSE2, performance may be degraded"
#endif

// 运行时查询
bool wcn_v256_is_native() {
    #ifdef WCN_X86_AVX2
    return true;
    #else
    return false;
    #endif
}
```

### 4. **关键路径优化**

```c
// 提供多个版本
void process_image_v128(/* ... */);  // 始终可用
void process_image_v256(/* ... */);  // 可能是模拟

// 自动选择
void process_image(/* ... */) {
    if (wcn_v256_is_native()) {
        process_image_v256(/* ... */);  // 快
    } else {
        process_image_v128(/* ... */);  // 可靠
    }
}
```

---

## 结论

### 可行性总结

| 维度 | 评价 |
|------|------|
| **技术可行性** | ⭐⭐⭐⭐ (80%) |
| **性能可接受性** | ⭐⭐⭐ (65%) |
| **开发复杂度** | ⭐⭐⭐ (60%) |
| **维护成本** | ⭐⭐⭐⭐ (75%) |
| **跨平台价值** | ⭐⭐⭐⭐⭐ (95%) |

### 推荐策略

**✅ 建议模拟的操作** (~80%，性能损失<20%):
- 基础算术、逻辑、比较
- Load/Store
- Min/Max、Abs
- 通道内Shuffle
- FMA（mul+add分解）

**⚠️ 谨慎模拟的操作** (~15%，性能损失20-50%):
- 跨通道Shuffle
- 水平操作（无SSE3时）
- 复杂Blend

**❌ 不推荐模拟的操作** (~5%，性能损失>50%):
- Gather/Scatter
- 掩码操作
- 向量化分支

### 最终建议

对于WCN_SIMD库：

1. **保持当前128-bit为核心** ✅
   - 已有的153个API完全跨平台
   - 性能稳定可靠

2. **可选添加256-bit模拟层** ⚠️
   - 仅模拟基础操作（~50个API）
   - 编译时可选启用
   - 提供性能警告

3. **不提供512-bit模拟** ❌
   - 性能损失太大
   - 使用场景太少
   - 维护成本高

**这种方案平衡了跨平台性和性能，是最实用的选择。**
