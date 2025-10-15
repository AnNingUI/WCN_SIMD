# WCN_SIMD 扩展API实现指南

## 已完成的平台

### ✅ x86 SSE2 (100% 完成)
**文件**: `include/wcn_simd/platform/x86/wcn_x86_sse2.h`
**新增代码**: 约456行

**实现内容**:
- 64位整数比较 (`wcn_v128i_cmpgt_i64`, `wcn_v128i_cmpeq_i64`, `wcn_v128i_cmplt_i64`)
- 扩展比较操作 (>=, <=, !=) for i8/i16/i32/i64/f32
- 无符号min/max (u16, u32)
- 64位整数min/max
- 8位和64位整数乘法
- 水平操作 (hadd/hsub for f32/f64/i16/i32)
- 水平饱和操作 (hadds/hsubs_i16)
- 符号操作 (sign_i8/i16/i32)
- 绝对差值 (sad_u8, absdiff_u8)
- Shuffle操作
- 反转操作 (reverse_i8/i16/i32/f32)
- 点积 (dp_f32/f64)

**实现策略**:
- 使用条件编译 `#if !defined(WCN_X86_SSE4_1)` 避免覆盖高级指令集
- 64位比较使用32位比较组合
- 无符号比较使用符号位翻转技巧
- 水平操作使用shuffle+加减指令组合
- 大量使用`_mm_shuffle_*`和位操作

### ✅ ARM NEON (100% 完成)
**文件**: `include/wcn_simd/platform/arm/wcn_arm_neon.h`
**新增代码**: 约510行

**实现内容**: 与SSE2相同的所有扩展API

**实现策略**:
- AArch64原生支持64位操作（`vcgtq_s64`, `vceqq_s64`, `vcgeq_s64`）
- ARMv7使用标量回退实现64位操作
- 大量原生指令支持: `vminq_u16/u32`, `vmaxq_u16/u32`, `vmulq_s8`, `vabdq_u8`
- 水平操作使用`vpaddq_*`（pairwise add）
- 反转使用`vrev64q_*` + `vextq_*`组合
- 符号操作使用`vcgtq/vcltq + vbslq`（bit select）

---

## 剩余平台实现策略

### 📋 LoongArch LSX/LASX
**预计工作量**: 300-400行

**平台特性**:
- LSX (128-bit SIMD) 类似于SSE
- LASX (256-bit SIMD) 类似于AVX
- 龙芯架构，指令集设计接近MIPS MSA

**实现建议**:
1. 64位操作: 使用`__lsx_vseq_d`, `__lsx_vslt_d`, `__lsx_vsle_d`（原生支持）
2. 无符号min/max: `__lsx_vmin_wu`, `__lsx_vmax_wu`（原生支持）
3. 水平操作: 使用`__lsx_vhaddw_*`, `__lsx_vhsubw_*`（原生支持）
4. 符号操作: 使用`__lsx_vsigncov_*`
5. Shuffle: `__lsx_vshuf_*`
6. 点积: 手动实现（乘法+reduce）

**参考文档**: [LoongArch SIMD指令集手册](https://github.com/loongson/LoongArch-Documentation)

---

### 📋 RISC-V RVV (Vector Extension)
**预计工作量**: 400-500行

**平台特性**:
- 可变向量长度（VLS）
- 非常灵活的向量架构
- Mask-based操作

**实现建议**:
1. 64位操作: `vmseq_vv_i64m1`, `vmsgt_vv_i64m1`（原生支持）
2. Min/Max: `vminu/vmaxu_vv_*`（原生支持）
3. 水平操作: 使用vector reduction指令 `vfredusum`, `vredsum`
4. Shuffle: 使用`vrgather_vv`（gather操作）
5. 点积: `vfmul + vfredusum`

**复杂度**: RVV的动态向量长度需要特别处理，可能需要循环展开

**参考文档**: [RISC-V Vector Extension Spec](https://github.com/riscv/riscv-v-spec)

---

### 📋 PowerPC AltiVec/VSX
**预计工作量**: 350-450行

**平台特性**:
- AltiVec: 128-bit向量单元
- VSX: 扩展的向量标量单元（双精度浮点）
- IBM Power架构

**实现建议**:
1. 64位操作: VSX有`vcmpequd`, `vcmpgtsd`（VSX 2.06+）
2. 无符号min/max: `vec_min`, `vec_max`（原生支持）
3. 水平操作: 使用permute+add (`vec_perm + vec_add`)
4. 符号操作: 使用`vec_sel` + 比较
5. Shuffle: `vec_perm`
6. 点积: VSX 3.0有`xvmaddsp`

**兼容性注意**: 
- AltiVec (PowerPC G4+)
- VSX (POWER7+)
- 需要条件编译区分不同版本

---

### 📋 MIPS MSA (MIPS SIMD Architecture)
**预计工作量**: 300-400行

**平台特性**:
- 128-bit向量
- 类似NEON的设计
- MIPS架构（较少使用）

**实现建议**:
1. 64位操作: `__msa_ceq_d`, `__msa_clt_s_d`（原生支持）
2. 无符号min/max: `__msa_min_u_*`, `__msa_max_u_*`
3. 水平操作: `__msa_hadd_*`（原生支持）
4. 符号操作: `__msa_adds_s_*` 条件组合
5. Shuffle: `__msa_vshf_*`
6. 点积: 手动实现

**参考**: [MIPS MSA指令集](https://www.mips.com/products/architectures/ase/simd/)

---

### 📋 WebAssembly SIMD128
**预计工作量**: 400-500行

**平台特性**:
- 固定128-bit向量
- 浏览器环境
- 指令集较基础

**实现建议**:
1. 64位操作: **没有原生支持**，需完全用标量模拟
2. 无符号min/max: `i16x8_min_u`, `i32x4_min_u`（原生支持）
3. 水平操作: **没有hadd**，需手动shuffle+add
4. 符号操作: 使用`i8x16_abs` + 比较 + `v128_bitselect`
5. Shuffle: `i8x16_shuffle`
6. 点积: 手动实现（WASM没有dot product）

**限制**: 
- 没有64位整数比较
- 没有水平操作
- 性能依赖浏览器优化

**参考**: [WASM SIMD Proposal](https://github.com/WebAssembly/simd)

---

## 实现模板

### 通用实现步骤

对于每个剩余平台，按以下步骤实现：

```c
/* ========== Extended Operations (PLATFORM_NAME) ========== */

/* 1. 64-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i64(wcn_v128i_t a, wcn_v128i_t b) {
    // 如果有原生指令，直接使用
    // 否则，参考SSE2的组合方法或NEON的标量回退
}

/* 2. Extended comparison operators */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpge_i8(wcn_v128i_t a, wcn_v128i_t b) {
    // 如果有原生vcge指令，使用
    // 否则: return OR(cmpgt(a,b), cmpeq(a,b))
}

/* 3. Unsigned min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_u16(wcn_v128i_t a, wcn_v128i_t b) {
    // 大多数平台有原生指令
    // 否则参考SSE2的饱和运算技巧
}

/* 4. 64-bit integer min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i64(wcn_v128i_t a, wcn_v128i_t b) {
    // 使用cmpgt_i64 + blend
    // 或标量回退
}

/* 5. Multiplications */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i8(wcn_v128i_t a, wcn_v128i_t b) {
    // 如果有原生指令最好
    // 否则: unpack to 16-bit, multiply, pack back
}

/* 6. Horizontal operations */
WCN_INLINE wcn_v128f_t wcn_v128f_hadd(wcn_v128f_t a, wcn_v128f_t b) {
    // 优先使用pairwise add指令
    // 否则: shuffle + add组合
}

/* 7. Sign operations */
WCN_INLINE wcn_v128i_t wcn_v128i_sign_i8(wcn_v128i_t a, wcn_v128i_t b) {
    // 逻辑: if b[i] < 0: -a[i], elif b[i] == 0: 0, else: a[i]
    // 使用比较 + blend/select指令
}

/* 8. Absolute difference */
WCN_INLINE wcn_v128i_t wcn_v128i_absdiff_u8(wcn_v128i_t a, wcn_v128i_t b) {
    // 大多数平台有vabd指令
    // 否则: max(a,b) - min(a,b)
}

/* 9. Shuffle/Reverse */
WCN_INLINE wcn_v128f_t wcn_v128f_shuffle(wcn_v128f_t a, wcn_v128f_t b, int imm) {
    // 使用平台的shuffle/permute指令
    // 最坏情况: 标量load/store回退
}

/* 10. Dot product */
WCN_INLINE wcn_v128f_t wcn_v128f_dp(wcn_v128f_t a, wcn_v128f_t b, int imm) {
    // 如果有dot product指令最好
    // 否则: multiply + horizontal sum
}
```

---

## 实现优先级建议

### 高优先级（立即实现）
1. ✅ **x86 SSE2** - 已完成，最广泛使用
2. ✅ **ARM NEON** - 已完成，移动端主流

### 中优先级（推荐实现）
3. **LoongArch LSX** - 国产龙芯平台，指令集友好
4. **PowerPC VSX** - 服务器市场（IBM Power）
5. **RISC-V RVV** - 新兴开源架构，长期前景好

### 低优先级（可选实现）
6. **MIPS MSA** - 使用较少，但实现简单
7. **WebAssembly SIMD** - 限制较多，性能依赖浏览器

---

## 测试建议

每个平台实现后，应创建单元测试：

```c
// test/test_extended_ops.c
void test_cmpgt_i64_platform() {
    wcn_v128i_t a = wcn_simd_set1_i64(10);
    wcn_v128i_t b = wcn_simd_set1_i64(5);
    wcn_v128i_t result = wcn_simd_cmpgt_i64(a, b);
    
    int64_t vals[2];
    wcn_simd_store_i128(vals, result);
    
    assert(vals[0] == -1); // all bits set
    assert(vals[1] == -1);
}
```

---

## 性能考虑

### 原生指令 vs 软件回退

| 操作类型 | 原生指令性能 | 软件回退性能 | 性能差距 |
|---------|------------|------------|---------|
| 64位比较 | 1 cycle | 5-10 cycles | 5-10x |
| 无符号min/max | 1 cycle | 3-5 cycles | 3-5x |
| 水平操作 | 3 cycles | 5-8 cycles | 2-3x |
| 符号操作 | 2 cycles | 4-6 cycles | 2-3x |
| 点积 | 3-5 cycles | 8-12 cycles | 2-4x |

**建议**: 性能关键代码应提供多个代码路径，运行时检测CPU特性选择最优实现。

---

## 编译测试命令

### x86
```bash
gcc -msse2 -DWCN_X86_SSE2 test.c
gcc -msse4.1 -DWCN_X86_SSE4_1 test.c
```

### ARM
```bash
gcc -march=armv7-a -mfpu=neon test.c
gcc -march=armv8-a test.c
```

### LoongArch
```bash
gcc -march=loongarch64 -mlsx test.c
```

### RISC-V
```bash
riscv64-unknown-linux-gnu-gcc -march=rv64gcv test.c
```

### PowerPC
```bash
powerpc64-linux-gnu-gcc -maltivec -mvsx test.c
```

---

## 总结

当前状态:
- ✅ **SSE2平台**: 100%完成（456行）
- ✅ **ARM NEON平台**: 100%完成（510行）
- ⏳ **剩余5个平台**: 参考已完成平台实现，预计总计1500-2000行代码

所有扩展API已在`WCN_SIMD.h`中定义，剩余工作是为其他平台补充底层实现。采用相同的实现模式可确保代码一致性和可维护性。
