#ifndef WCN_POWERPC_ALTIVEC_H
#define WCN_POWERPC_ALTIVEC_H

#include "../../wcn_types.h"

#ifdef WCN_POWERPC_ALTIVEC

/* ========== Load/Store Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_load(const void* ptr) {
    wcn_v128i_t result;
    result.raw = vec_vsx_ld(0, (const int*)ptr);
    return result;
}

WCN_INLINE void wcn_v128i_store(void* ptr, wcn_v128i_t vec) {
    vec_vsx_st(vec.raw, 0, (int*)ptr);
}

WCN_INLINE wcn_v128f_t wcn_v128f_load(const float* ptr) {
    wcn_v128f_t result;
    result.raw = vec_vsx_ld(0, ptr);
    return result;
}

WCN_INLINE void wcn_v128f_store(float* ptr, wcn_v128f_t vec) {
    vec_vsx_st(vec.raw, 0, ptr);
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_load(const double* ptr) {
    wcn_v128d_t result;
    result.raw = vec_vsx_ld(0, ptr);
    return result;
}

WCN_INLINE void wcn_v128d_store(double* ptr, wcn_v128d_t vec) {
    vec_vsx_st(vec.raw, 0, ptr);
}
#endif

/* ========== Aligned Load/Store Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_load_aligned(const void* ptr) {
    wcn_v128i_t result;
    result.raw = vec_vsx_ld(0, (const int*)ptr);
    return result;
}

WCN_INLINE void wcn_v128i_store_aligned(void* ptr, wcn_v128i_t vec) {
    vec_vsx_st(vec.raw, 0, (int*)ptr);
}

WCN_INLINE wcn_v128f_t wcn_v128f_load_aligned(const float* ptr) {
    wcn_v128f_t result;
    result.raw = vec_vsx_ld(0, ptr);
    return result;
}

WCN_INLINE void wcn_v128f_store_aligned(float* ptr, wcn_v128f_t vec) {
    vec_vsx_st(vec.raw, 0, ptr);
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_load_aligned(const double* ptr) {
    wcn_v128d_t result;
    result.raw = vec_vsx_ld(0, ptr);
    return result;
}

WCN_INLINE void wcn_v128d_store_aligned(double* ptr, wcn_v128d_t vec) {
    vec_vsx_st(vec.raw, 0, ptr);
}
#endif

/* ========== Initialization ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i32(int32_t value) {
    wcn_v128i_t result;
    result.raw = vec_splats(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i8(int8_t value) {
    wcn_v128i_t result;
    result.raw = (vector signed int)vec_splats((signed char)value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i16(int16_t value) {
    wcn_v128i_t result;
    result.raw = (vector signed int)vec_splats((signed short)value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i64(int64_t value) {
    wcn_v128i_t result;
    result.raw = (vector signed int)vec_splats((long long)value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_setzero(void) {
    wcn_v128i_t result;
    result.raw = vec_splats(0);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_set1(float value) {
    wcn_v128f_t result;
    result.raw = vec_splats(value);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_setzero(void) {
    wcn_v128f_t result;
    result.raw = vec_splats(0.0f);
    return result;
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_set1(double value) {
    wcn_v128d_t result;
    result.raw = vec_splats(value);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_setzero(void) {
    wcn_v128d_t result;
    result.raw = vec_splats(0.0);
    return result;
}
#endif

/* ========== Arithmetic Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_add_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_add(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_sub(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_add_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_add(a16, b16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_sub(a16, b16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_add_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_add(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_sub(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_add_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector long long a64 = (vector long long)a.raw;
    vector long long b64 = (vector long long)b.raw;
    result.raw = (vector signed int)vec_add(a64, b64);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector long long a64 = (vector long long)a.raw;
    vector long long b64 = (vector long long)b.raw;
    result.raw = (vector signed int)vec_sub(a64, b64);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_add(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vec_add(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_sub(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vec_sub(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_mul(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vec_mul(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_div(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vec_div(a.raw, b.raw);
    return result;
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_add(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_add(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sub(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_sub(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_mul(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_mul(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_div(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_div(a.raw, b.raw);
    return result;
}
#endif

/* ========== Saturating Arithmetic Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_adds_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_adds(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_subs_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_subs(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_adds_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector unsigned char a8 = (vector unsigned char)a.raw;
    vector unsigned char b8 = (vector unsigned char)b.raw;
    result.raw = (vector signed int)vec_adds(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_subs_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector unsigned char a8 = (vector unsigned char)a.raw;
    vector unsigned char b8 = (vector unsigned char)b.raw;
    result.raw = (vector signed int)vec_subs(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_adds_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_adds(a16, b16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_subs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_subs(a16, b16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_adds_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector unsigned short a16 = (vector unsigned short)a.raw;
    vector unsigned short b16 = (vector unsigned short)b.raw;
    result.raw = (vector signed int)vec_adds(a16, b16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_subs_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector unsigned short a16 = (vector unsigned short)a.raw;
    vector unsigned short b16 = (vector unsigned short)b.raw;
    result.raw = (vector signed int)vec_subs(a16, b16);
    return result;
}

/* ========== Shift Operations ========== */

/* 16-bit shifts */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector unsigned short cnt = (vector unsigned short)count.raw;
    result.raw = (vector signed int)vec_sl(a16, cnt);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    vector unsigned short a16 = (vector unsigned short)a.raw;
    vector unsigned short cnt = (vector unsigned short)count.raw;
    result.raw = (vector signed int)vec_sr(a16, cnt);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sra_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector unsigned short cnt = (vector unsigned short)count.raw;
    result.raw = (vector signed int)vec_sra(a16, cnt);
    return result;
}

/* 16-bit immediate shifts */
#define wcn_v128i_slli_i16(a, imm) ({ \
    wcn_v128i_t _result; \
    vector signed short a16 = (vector signed short)(a).raw; \
    _result.raw = (vector signed int)vec_sl(a16, vec_splats((unsigned short)(imm))); \
    _result; \
})

#define wcn_v128i_srli_i16(a, imm) ({ \
    wcn_v128i_t _result; \
    vector unsigned short a16 = (vector unsigned short)(a).raw; \
    _result.raw = (vector signed int)vec_sr(a16, vec_splats((unsigned short)(imm))); \
    _result; \
})

#define wcn_v128i_srai_i16(a, imm) ({ \
    wcn_v128i_t _result; \
    vector signed short a16 = (vector signed short)(a).raw; \
    _result.raw = (vector signed int)vec_sra(a16, vec_splats((unsigned short)(imm))); \
    _result; \
})

/* 32-bit shifts */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    vector unsigned int cnt = (vector unsigned int)count.raw;
    result.raw = (vector signed int)vec_sl(a.raw, cnt);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    vector unsigned int a32 = (vector unsigned int)a.raw;
    vector unsigned int cnt = (vector unsigned int)count.raw;
    result.raw = (vector signed int)vec_sr(a32, cnt);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sra_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    vector unsigned int cnt = (vector unsigned int)count.raw;
    result.raw = vec_sra(a.raw, cnt);
    return result;
}

/* 32-bit immediate shifts */
#define wcn_v128i_slli_i32(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = (vector signed int)vec_sl((a).raw, vec_splats((unsigned int)(imm))); \
    _result; \
})

#define wcn_v128i_srli_i32(a, imm) ({ \
    wcn_v128i_t _result; \
    vector unsigned int a32 = (vector unsigned int)(a).raw; \
    _result.raw = (vector signed int)vec_sr(a32, vec_splats((unsigned int)(imm))); \
    _result; \
})

#define wcn_v128i_srai_i32(a, imm) ({ \
    wcn_v128i_t _result; \
    _result.raw = vec_sra((a).raw, vec_splats((unsigned int)(imm))); \
    _result; \
})

/* 64-bit shifts */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    vector long long a64 = (vector long long)a.raw;
    vector unsigned long long cnt = (vector unsigned long long)count.raw;
    result.raw = (vector signed int)vec_sl(a64, cnt);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_srl_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    vector unsigned long long a64 = (vector unsigned long long)a.raw;
    vector unsigned long long cnt = (vector unsigned long long)count.raw;
    result.raw = (vector signed int)vec_sr(a64, cnt);
    return result;
}

/* 64-bit immediate shifts */
#define wcn_v128i_slli_i64(a, imm) ({ \
    wcn_v128i_t _result; \
    vector long long a64 = (vector long long)(a).raw; \
    _result.raw = (vector signed int)vec_sl(a64, vec_splats((unsigned long long)(imm))); \
    _result; \
})

#define wcn_v128i_srli_i64(a, imm) ({ \
    wcn_v128i_t _result; \
    vector unsigned long long a64 = (vector unsigned long long)(a).raw; \
    _result.raw = (vector signed int)vec_sr(a64, vec_splats((unsigned long long)(imm))); \
    _result; \
})

/* 128-bit byte shifts */
#define wcn_v128i_slli_si128(a, imm) ({ \
    wcn_v128i_t _result; \
    vector unsigned char zero = vec_splats((unsigned char)0); \
    _result.raw = (vector signed int)vec_sld((vector unsigned char)(a).raw, zero, 16 - (imm)); \
    _result; \
})

#define wcn_v128i_srli_si128(a, imm) ({ \
    wcn_v128i_t _result; \
    vector unsigned char zero = vec_splats((unsigned char)0); \
    _result.raw = (vector signed int)vec_sld(zero, (vector unsigned char)(a).raw, (imm)); \
    _result; \
})

/* ========== FMA (Fused Multiply-Add) ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_fmadd(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = vec_madd(a.raw, b.raw, c.raw);
    return result;
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_fmadd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = vec_madd(a.raw, b.raw, c.raw);
    return result;
}
#endif

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_and(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_and(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_or(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_or(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_xor(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_xor(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_andnot(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_andc(b.raw, a.raw);
    return result;
}

/* ========== Comparison Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_cmpeq(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_cmpgt(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_cmplt(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_cmpeq(a16, b16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_cmpgt(a16, b16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_cmplt(a16, b16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (vector signed int)vec_cmpeq(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (vector signed int)vec_cmpgt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (vector signed int)vec_cmplt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpeq(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (vector float)vec_cmpeq(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpgt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (vector float)vec_cmpgt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmplt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (vector float)vec_cmplt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmple(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (vector float)vec_cmple(a.raw, b.raw);
    return result;
}

#ifdef WCN_POWERPC_VSX
/* Double comparison operations */
WCN_INLINE wcn_v128d_t wcn_v128d_cmpeq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmpeq(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmplt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmplt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmple(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmple(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpgt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmpgt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpge(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = (vector double)vec_cmpge(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpneq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    vector bool long long eq = vec_cmpeq(a.raw, b.raw);
    /* NOT of equal gives not-equal */
    result.raw = (vector double)vec_nor(eq, eq);
    return result;
}
#endif

/* ========== Min/Max Operations ========== */

/* 8-bit signed min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_min(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_max(a8, b8);
    return result;
}

/* 8-bit unsigned min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector unsigned char a8 = (vector unsigned char)a.raw;
    vector unsigned char b8 = (vector unsigned char)b.raw;
    result.raw = (vector signed int)vec_min(a8, b8);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector unsigned char a8 = (vector unsigned char)a.raw;
    vector unsigned char b8 = (vector unsigned char)b.raw;
    result.raw = (vector signed int)vec_max(a8, b8);
    return result;
}

/* 16-bit signed min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_min(a16, b16);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_max(a16, b16);
    return result;
}

/* 32-bit signed min/max */
WCN_INLINE wcn_v128i_t wcn_v128i_max_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_max(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_min(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_max(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vec_max(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_min(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vec_min(a.raw, b.raw);
    return result;
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_max(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_max(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_min(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_min(a.raw, b.raw);
    return result;
}
#endif

/* ========== Square Root ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_sqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vec_sqrt(vec.raw);
    return result;
}

#ifdef WCN_POWERPC_VSX
WCN_INLINE wcn_v128d_t wcn_v128d_sqrt(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_sqrt(vec.raw);
    return result;
}
#endif

/* ========== Reciprocal Approximations ========== */

/* Reciprocal approximation (1/x) for packed single-precision floats */
WCN_INLINE wcn_v128f_t wcn_v128f_rcp(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vec_re(vec.raw);
    return result;
}

/* Reciprocal square root approximation (1/sqrt(x)) for packed single-precision floats */
WCN_INLINE wcn_v128f_t wcn_v128f_rsqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vec_rsqrte(vec.raw);
    return result;
}

/* ========== Absolute Value Operations ========== */

/* 8-bit signed integer absolute value */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i8(wcn_v128i_t a) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    result.raw = (vector signed int)vec_abs(a8);
    return result;
}

/* 16-bit signed integer absolute value */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i16(wcn_v128i_t a) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    result.raw = (vector signed int)vec_abs(a16);
    return result;
}

/* 32-bit signed integer absolute value */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i32(wcn_v128i_t a) {
    wcn_v128i_t result;
    result.raw = vec_abs(a.raw);
    return result;
}

/* Float absolute value */
WCN_INLINE wcn_v128f_t wcn_v128f_abs(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vec_abs(vec.raw);
    return result;
}

#ifdef WCN_POWERPC_VSX
/* Double absolute value */
WCN_INLINE wcn_v128d_t wcn_v128d_abs(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vec_abs(vec.raw);
    return result;
}
#endif

/* ========== Negation Operations ========== */

/* 8-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i8(wcn_v128i_t a) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char zero = vec_splats((signed char)0);
    result.raw = (vector signed int)vec_sub(zero, a8);
    return result;
}

/* 16-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i16(wcn_v128i_t a) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short zero = vec_splats((signed short)0);
    result.raw = (vector signed int)vec_sub(zero, a16);
    return result;
}

/* 32-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i32(wcn_v128i_t a) {
    wcn_v128i_t result;
    vector signed int zero = vec_splats(0);
    result.raw = vec_sub(zero, a.raw);
    return result;
}

/* 64-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i64(wcn_v128i_t a) {
    wcn_v128i_t result;
    vector long long a64 = (vector long long)a.raw;
    vector long long zero = vec_splats(0LL);
    result.raw = (vector signed int)vec_sub(zero, a64);
    return result;
}

/* Float negation */
WCN_INLINE wcn_v128f_t wcn_v128f_neg(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* XOR with sign bit mask 0x80000000 */
    vector unsigned int sign_mask = vec_splats(0x80000000u);
    result.raw = (vector float)vec_xor((vector unsigned int)vec.raw, sign_mask);
    return result;
}

#ifdef WCN_POWERPC_VSX
/* Double negation */
WCN_INLINE wcn_v128d_t wcn_v128d_neg(wcn_v128d_t vec) {
    wcn_v128d_t result;
    /* XOR with sign bit mask 0x8000000000000000 */
    vector unsigned long long sign_mask = vec_splats(0x8000000000000000ULL);
    result.raw = (vector double)vec_xor((vector unsigned long long)vec.raw, sign_mask);
    return result;
}
#endif

/* ========== Broadcast Operations ========== */

/* Load scalar and broadcast to all 32-bit integer lanes */
WCN_INLINE wcn_v128i_t wcn_v128i_broadcast_i32(const int32_t* ptr) {
    wcn_v128i_t result;
    result.raw = vec_splats(*ptr);
    return result;
}

/* Load scalar and broadcast to all float lanes */
WCN_INLINE wcn_v128f_t wcn_v128f_broadcast(const float* ptr) {
    wcn_v128f_t result;
    result.raw = vec_splats(*ptr);
    return result;
}

#ifdef WCN_POWERPC_VSX
/* Load scalar and broadcast to all double lanes */
WCN_INLINE wcn_v128d_t wcn_v128d_broadcast(const double* ptr) {
    wcn_v128d_t result;
    result.raw = vec_splats(*ptr);
    return result;
}
#endif

/* ========== Conversions ========== */

WCN_INLINE wcn_v128f_t wcn_v128i_to_v128f(wcn_v128i_t vec) {
    wcn_v128f_t result;
    result.raw = vec_ctf(vec.raw, 0);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128f_to_v128i(wcn_v128f_t vec) {
    wcn_v128i_t result;
    result.raw = vec_cts(vec.raw, 0);
    return result;
}

/* ========== Integer Multiplication ========== */

/* 16-bit integer multiplication (low 16 bits of result) */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_mladd(a16, b16, vec_splats((signed short)0));
    return result;
}

/* 16-bit integer multiplication (high 16 bits of result, signed) */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;

    /* Use mule/mulo to get even/odd 32-bit products, then extract high 16 bits */
    vector signed int prod_even = vec_mule(a16, b16);
    vector signed int prod_odd = vec_mulo(a16, b16);

    /* Shift right by 16 to get high 16 bits */
    prod_even = vec_sr(prod_even, vec_splats(16u));
    prod_odd = vec_sr(prod_odd, vec_splats(16u));

    /* Pack back to 16-bit */
    result.raw = (vector signed int)vec_pack(prod_even, prod_odd);
    return result;
}

/* 16-bit integer multiplication (high 16 bits of result, unsigned) */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector unsigned short a16 = (vector unsigned short)a.raw;
    vector unsigned short b16 = (vector unsigned short)b.raw;

    /* Use mule/mulo to get even/odd 32-bit products */
    vector unsigned int prod_even = vec_mule(a16, b16);
    vector unsigned int prod_odd = vec_mulo(a16, b16);

    /* Shift right by 16 to get high 16 bits */
    prod_even = vec_sr(prod_even, vec_splats(16u));
    prod_odd = vec_sr(prod_odd, vec_splats(16u));

    /* Pack back to 16-bit */
    result.raw = (vector signed int)vec_pack(prod_even, prod_odd);
    return result;
}

/* 32-bit integer multiplication (low 32 bits) */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    /* AltiVec/VSX doesn't have direct 32-bit multiply, emulate it */
    vector unsigned int a_u = (vector unsigned int)a.raw;
    vector unsigned int b_u = (vector unsigned int)b.raw;

    /* Split into 16-bit parts and multiply */
    vector unsigned short a_lo = (vector unsigned short)vec_mergeh((vector unsigned char)a_u, vec_splats((unsigned char)0));
    vector unsigned short b_lo = (vector unsigned short)vec_mergeh((vector unsigned char)b_u, vec_splats((unsigned char)0));

    /* Use mullo approach: (a_hi << 16 + a_lo) * (b_hi << 16 + b_lo) =
       a_hi*b_lo << 16 + a_lo*b_hi << 16 + a_lo*b_lo */
    vector unsigned int a_even = vec_mule((vector unsigned short)a_u, (vector unsigned short)b_u);
    vector unsigned int a_odd = vec_mulo((vector unsigned short)a_u, (vector unsigned short)b_u);

    /* Interleave even and odd */
    result.raw = (vector signed int)vec_perm(a_even, a_odd,
        (vector unsigned char){0,1,2,3, 16,17,18,19, 8,9,10,11, 24,25,26,27});

    return result;
}

/* Multiply packed unsigned 32-bit integers, produce 64-bit results (only low 2 elements) */
WCN_INLINE wcn_v128i_t wcn_v128i_mul_u32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector unsigned int a_u = (vector unsigned int)a.raw;
    vector unsigned int b_u = (vector unsigned int)b.raw;

    /* Multiply elements 0 and 2 to get 64-bit results */
    vector unsigned long long prod_even = vec_mule(a_u, b_u);

    result.raw = (vector signed int)prod_even;
    return result;
}

/* ========== Pack/Unpack Operations ========== */

/* Pack 32-bit signed integers to 16-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (vector signed int)vec_packs(a.raw, b.raw);
    return result;
}

/* Pack 16-bit signed integers to 8-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_packs(a16, b16);
    return result;
}

/* Pack 16-bit signed integers to 8-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_packsu(a16, b16);
    return result;
}

/* Pack 32-bit signed integers to 16-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = (vector signed int)vec_packsu(a.raw, b.raw);
    return result;
}

/* Unpack and interleave low 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_mergeh(a8, b8);
    return result;
}

/* Unpack and interleave high 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_mergel(a8, b8);
    return result;
}

/* Unpack and interleave low 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_mergeh(a16, b16);
    return result;
}

/* Unpack and interleave high 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector signed short a16 = (vector signed short)a.raw;
    vector signed short b16 = (vector signed short)b.raw;
    result.raw = (vector signed int)vec_mergel(a16, b16);
    return result;
}

/* Unpack and interleave low 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_mergeh(a.raw, b.raw);
    return result;
}

/* Unpack and interleave high 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vec_mergel(a.raw, b.raw);
    return result;
}

/* Unpack and interleave low 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector long long a64 = (vector long long)a.raw;
    vector long long b64 = (vector long long)b.raw;
    result.raw = (vector signed int)vec_mergeh(a64, b64);
    return result;
}

/* Unpack and interleave high 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    vector long long a64 = (vector long long)a.raw;
    vector long long b64 = (vector long long)b.raw;
    result.raw = (vector signed int)vec_mergel(a64, b64);
    return result;
}

/* Unpack floats */
WCN_INLINE wcn_v128f_t wcn_v128f_unpacklo(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vec_mergeh(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_unpackhi(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vec_mergel(a.raw, b.raw);
    return result;
}

#ifdef WCN_POWERPC_VSX
/* Unpack doubles */
WCN_INLINE wcn_v128d_t wcn_v128d_unpacklo(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_mergeh(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_unpackhi(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vec_mergel(a.raw, b.raw);
    return result;
}
#endif

/* ========== Extract/Insert Operations ========== */

/* Extract 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_v128i_extract_i16(vec, index) ({ \
    vector signed short v16 = (vector signed short)(vec).raw; \
    vec_extract(v16, (index)); \
})

/* Insert 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_v128i_insert_i16(vec, val, index) ({ \
    wcn_v128i_t _result; \
    vector signed short v16 = (vector signed short)(vec).raw; \
    v16 = vec_insert((val), v16, (index)); \
    _result.raw = (vector signed int)v16; \
    _result; \
})

/* ========== Movemask Operations ========== */

/* Create mask from most significant bit of each 8-bit element */
WCN_INLINE int wcn_v128i_movemask_i8(wcn_v128i_t vec) {
    /* AltiVec doesn't have direct movemask, simulate with compares and extracts */
    vector unsigned char v = (vector unsigned char)vec.raw;
    vector unsigned char zero = vec_splats((unsigned char)0);
    vector bool char mask = vec_cmplt(v, zero); /* Check sign bit */

    /* Extract bits manually */
    unsigned char bytes[16];
    vec_vsx_st(mask, 0, (vector unsigned char*)bytes);

    int result = 0;
    for (int i = 0; i < 16; i++) {
        result |= (bytes[i] ? 1 : 0) << i;
    }
    return result;
}

/* Create mask from most significant bit of each float */
WCN_INLINE int wcn_v128f_movemask(wcn_v128f_t vec) {
    vector unsigned int bits = (vector unsigned int)vec.raw;
    vector unsigned int zero = vec_splats(0u);
    vector bool int mask = vec_cmplt(bits, zero); /* Check sign bit */

    unsigned int words[4];
    vec_vsx_st(mask, 0, (vector unsigned int*)words);

    int result = 0;
    for (int i = 0; i < 4; i++) {
        result |= (words[i] ? 1 : 0) << i;
    }
    return result;
}

#ifdef WCN_POWERPC_VSX
/* Create mask from most significant bit of each double */
WCN_INLINE int wcn_v128d_movemask(wcn_v128d_t vec) {
    vector unsigned long long bits = (vector unsigned long long)vec.raw;
    vector unsigned long long zero = vec_splats(0ull);
    vector bool long long mask = vec_cmplt(bits, zero);

    unsigned long long dwords[2];
    vec_vsx_st(mask, 0, (vector unsigned long long*)dwords);

    int result = 0;
    for (int i = 0; i < 2; i++) {
        result |= (dwords[i] ? 1 : 0) << i;
    }
    return result;
}
#endif

/* ========== Blend/Select Operations ========== */

/* Variable blend floats (blends based on sign bit of mask) */
WCN_INLINE wcn_v128f_t wcn_v128f_blendv(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t mask) {
    wcn_v128f_t result;
    vector unsigned int m = (vector unsigned int)mask.raw;
    result.raw = vec_sel(a.raw, b.raw, m);
    return result;
}

#ifdef WCN_POWERPC_VSX
/* Variable blend doubles */
WCN_INLINE wcn_v128d_t wcn_v128d_blendv(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t mask) {
    wcn_v128d_t result;
    vector unsigned long long m = (vector unsigned long long)mask.raw;
    result.raw = vec_sel(a.raw, b.raw, m);
    return result;
}
#endif

/* Variable blend 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_blendv_i8(wcn_v128i_t a, wcn_v128i_t b, wcn_v128i_t mask) {
    wcn_v128i_t result;
    vector unsigned char m = (vector unsigned char)mask.raw;
    vector signed char a8 = (vector signed char)a.raw;
    vector signed char b8 = (vector signed char)b.raw;
    result.raw = (vector signed int)vec_sel(a8, b8, m);
    return result;
}

/* ========== Byte Shuffle ========== */

/* Shuffle bytes according to indices in control mask */
WCN_INLINE wcn_v128i_t wcn_v128i_shuffle_i8(wcn_v128i_t vec, wcn_v128i_t control) {
    wcn_v128i_t result;
    vector unsigned char src = (vector unsigned char)vec.raw;
    vector unsigned char idx = (vector unsigned char)control.raw;
    result.raw = (vector signed int)vec_perm(src, src, idx);
    return result;
}

/* ========== Reduction Operations ========== */

WCN_INLINE float wcn_v128f_reduce_add(wcn_v128f_t vec) {
    /* Sum all 4 elements */
    vector float v = vec.raw;
    vector float temp = vec_add(v, vec_sld(v, v, 8));
    temp = vec_add(temp, vec_sld(temp, temp, 4));
    return vec_extract(temp, 0);
}

WCN_INLINE float wcn_v128f_reduce_max(wcn_v128f_t vec) {
    vector float v = vec.raw;
    vector float temp = vec_max(v, vec_sld(v, v, 8));
    temp = vec_max(temp, vec_sld(temp, temp, 4));
    return vec_extract(temp, 0);
}

WCN_INLINE float wcn_v128f_reduce_min(wcn_v128f_t vec) {
    vector float v = vec.raw;
    vector float temp = vec_min(v, vec_sld(v, v, 8));
    temp = vec_min(temp, vec_sld(temp, temp, 4));
    return vec_extract(temp, 0);
}

/* ========== Comparison Extensions ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_cmplt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (vector float)vec_cmplt(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmple(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = (vector float)vec_cmple(a.raw, b.raw);
    return result;
}

#endif /* WCN_POWERPC_ALTIVEC */

#endif /* WCN_POWERPC_ALTIVEC_H */
