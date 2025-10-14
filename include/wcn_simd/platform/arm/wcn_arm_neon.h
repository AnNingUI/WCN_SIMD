#ifndef WCN_ARM_NEON_H
#define WCN_ARM_NEON_H

#include "../../wcn_types.h"

#ifdef WCN_ARM_NEON

/* ========== Load/Store Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_load(const void* ptr) {
    wcn_v128i_t result;
    result.raw = vld1q_s32((const int32_t*)ptr);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_load_aligned(const void* ptr) {
    wcn_v128i_t result;
    result.raw = vld1q_s32((const int32_t*)ptr);
    return result;
}

WCN_INLINE void wcn_v128i_store(void* ptr, wcn_v128i_t vec) {
    vst1q_s32((int32_t*)ptr, vec.raw);
}

WCN_INLINE void wcn_v128i_store_aligned(void* ptr, wcn_v128i_t vec) {
    vst1q_s32((int32_t*)ptr, vec.raw);
}

WCN_INLINE wcn_v128f_t wcn_v128f_load(const float* ptr) {
    wcn_v128f_t result;
    result.raw = vld1q_f32(ptr);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_load_aligned(const float* ptr) {
    wcn_v128f_t result;
    result.raw = vld1q_f32(ptr);
    return result;
}

WCN_INLINE void wcn_v128f_store(float* ptr, wcn_v128f_t vec) {
    vst1q_f32(ptr, vec.raw);
}

WCN_INLINE void wcn_v128f_store_aligned(float* ptr, wcn_v128f_t vec) {
    vst1q_f32(ptr, vec.raw);
}

#ifdef WCN_ARM_AARCH64
WCN_INLINE wcn_v128d_t wcn_v128d_load(const double* ptr) {
    wcn_v128d_t result;
    result.raw = vld1q_f64(ptr);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_load_aligned(const double* ptr) {
    wcn_v128d_t result;
    result.raw = vld1q_f64(ptr);
    return result;
}

WCN_INLINE void wcn_v128d_store(double* ptr, wcn_v128d_t vec) {
    vst1q_f64(ptr, vec.raw);
}

WCN_INLINE void wcn_v128d_store_aligned(double* ptr, wcn_v128d_t vec) {
    vst1q_f64(ptr, vec.raw);
}
#endif

/* ========== Initialization ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i8(int8_t value) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s8(vdupq_n_s8(value));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i16(int16_t value) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s16(vdupq_n_s16(value));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i32(int32_t value) {
    wcn_v128i_t result;
    result.raw = vdupq_n_s32(value);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_set1_i64(int64_t value) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s64(vdupq_n_s64(value));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_setzero(void) {
    wcn_v128i_t result;
    result.raw = vdupq_n_s32(0);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_set1(float value) {
    wcn_v128f_t result;
    result.raw = vdupq_n_f32(value);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_setzero(void) {
    wcn_v128f_t result;
    result.raw = vdupq_n_f32(0.0f);
    return result;
}

#ifdef WCN_ARM_AARCH64
WCN_INLINE wcn_v128d_t wcn_v128d_set1(double value) {
    wcn_v128d_t result;
    result.raw = vdupq_n_f64(value);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_setzero(void) {
    wcn_v128d_t result;
    result.raw = vdupq_n_f64(0.0);
    return result;
}
#endif

/* ========== Arithmetic Operations ========== */

/* 8-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s8(vaddq_s8(vreinterpretq_s8_s32(a.raw), vreinterpretq_s8_s32(b.raw)));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s8(vsubq_s8(vreinterpretq_s8_s32(a.raw), vreinterpretq_s8_s32(b.raw)));
    return result;
}

/* 16-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s16(vaddq_s16(vreinterpretq_s16_s32(a.raw), vreinterpretq_s16_s32(b.raw)));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s16(vsubq_s16(vreinterpretq_s16_s32(a.raw), vreinterpretq_s16_s32(b.raw)));
    return result;
}

/* 32-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vaddq_s32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vsubq_s32(a.raw, b.raw);
    return result;
}

/* 64-bit integer arithmetic */
WCN_INLINE wcn_v128i_t wcn_v128i_add_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s64(vaddq_s64(vreinterpretq_s64_s32(a.raw), vreinterpretq_s64_s32(b.raw)));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_sub_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s64(vsubq_s64(vreinterpretq_s64_s32(a.raw), vreinterpretq_s64_s32(b.raw)));
    return result;
}

/* ========== Saturating Arithmetic ========== */

/* 8-bit signed saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s8(vqaddq_s8(vreinterpretq_s8_s32(a.raw), vreinterpretq_s8_s32(b.raw)));
    return result;
}

/* 8-bit signed saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s8(vqsubq_s8(vreinterpretq_s8_s32(a.raw), vreinterpretq_s8_s32(b.raw)));
    return result;
}

/* 8-bit unsigned saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_u8(vqaddq_u8(vreinterpretq_u8_s32(a.raw), vreinterpretq_u8_s32(b.raw)));
    return result;
}

/* 8-bit unsigned saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_u8(vqsubq_u8(vreinterpretq_u8_s32(a.raw), vreinterpretq_u8_s32(b.raw)));
    return result;
}

/* 16-bit signed saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s16(vqaddq_s16(vreinterpretq_s16_s32(a.raw), vreinterpretq_s16_s32(b.raw)));
    return result;
}

/* 16-bit signed saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s16(vqsubq_s16(vreinterpretq_s16_s32(a.raw), vreinterpretq_s16_s32(b.raw)));
    return result;
}

/* 16-bit unsigned saturating addition */
WCN_INLINE wcn_v128i_t wcn_v128i_adds_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_u16(vqaddq_u16(vreinterpretq_u16_s32(a.raw), vreinterpretq_u16_s32(b.raw)));
    return result;
}

/* 16-bit unsigned saturating subtraction */
WCN_INLINE wcn_v128i_t wcn_v128i_subs_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_u16(vqsubq_u16(vreinterpretq_u16_s32(a.raw), vreinterpretq_u16_s32(b.raw)));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_mul_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vmulq_s32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_add(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vaddq_f32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_sub(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vsubq_f32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_mul(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vmulq_f32(a.raw, b.raw);
    return result;
}

#ifdef WCN_ARM_AARCH64
WCN_INLINE wcn_v128f_t wcn_v128f_div(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vdivq_f32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_add(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vaddq_f64(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sub(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vsubq_f64(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_mul(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vmulq_f64(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_div(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vdivq_f64(a.raw, b.raw);
    return result;
}
#endif

/* ========== FMA (Fused Multiply-Add) ========== */

WCN_INLINE wcn_v128f_t wcn_v128f_fmadd(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t c) {
    wcn_v128f_t result;
    result.raw = vfmaq_f32(c.raw, a.raw, b.raw);
    return result;
}

#ifdef WCN_ARM_AARCH64
WCN_INLINE wcn_v128d_t wcn_v128d_fmadd(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t c) {
    wcn_v128d_t result;
    result.raw = vfmaq_f64(c.raw, a.raw, b.raw);
    return result;
}
#endif

/* ========== Logical Operations ========== */

WCN_INLINE wcn_v128i_t wcn_v128i_and(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vandq_s32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_or(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vorrq_s32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_xor(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = veorq_s32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_andnot(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vbicq_s32(b.raw, a.raw);
    return result;
}

/* ========== Shift Operations ========== */

/* 16-bit logical shift left */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16_t shift = (int16_t)vgetq_lane_s32(count.raw, 0);
    result.raw = vreinterpretq_s32_s16(vshlq_s16(a16, vdupq_n_s16(shift)));
    return result;
}

#define wcn_v128i_slli_i16(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        int16x8_t _a16 = vreinterpretq_s16_s32((a).raw); \
        _result.raw = vreinterpretq_s32_s16(vshlq_n_s16(_a16, (imm))); \
        _result; \
    })

/* 16-bit logical shift right */
WCN_INLINE wcn_v128i_t wcn_v128i_srl_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    uint16x8_t a16 = vreinterpretq_u16_s32(a.raw);
    int16_t shift = -(int16_t)vgetq_lane_s32(count.raw, 0);
    result.raw = vreinterpretq_s32_u16(vshlq_u16(a16, vdupq_n_s16(shift)));
    return result;
}

#define wcn_v128i_srli_i16(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        uint16x8_t _a16 = vreinterpretq_u16_s32((a).raw); \
        _result.raw = vreinterpretq_s32_u16(vshrq_n_u16(_a16, (imm))); \
        _result; \
    })

/* 16-bit arithmetic shift right */
WCN_INLINE wcn_v128i_t wcn_v128i_sra_i16(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16_t shift = -(int16_t)vgetq_lane_s32(count.raw, 0);
    result.raw = vreinterpretq_s32_s16(vshlq_s16(a16, vdupq_n_s16(shift)));
    return result;
}

#define wcn_v128i_srai_i16(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        int16x8_t _a16 = vreinterpretq_s16_s32((a).raw); \
        _result.raw = vreinterpretq_s32_s16(vshrq_n_s16(_a16, (imm))); \
        _result; \
    })

/* 32-bit logical shift left */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    int32_t shift = vgetq_lane_s32(count.raw, 0);
    result.raw = vshlq_s32(a.raw, vdupq_n_s32(shift));
    return result;
}

#define wcn_v128i_slli_i32(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = vshlq_n_s32((a).raw, (imm)); \
        _result; \
    })

/* 32-bit logical shift right */
WCN_INLINE wcn_v128i_t wcn_v128i_srl_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    uint32x4_t a32 = vreinterpretq_u32_s32(a.raw);
    int32_t shift = -vgetq_lane_s32(count.raw, 0);
    result.raw = vreinterpretq_s32_u32(vshlq_u32(a32, vdupq_n_s32(shift)));
    return result;
}

#define wcn_v128i_srli_i32(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        uint32x4_t _a32 = vreinterpretq_u32_s32((a).raw); \
        _result.raw = vreinterpretq_s32_u32(vshrq_n_u32(_a32, (imm))); \
        _result; \
    })

/* 32-bit arithmetic shift right */
WCN_INLINE wcn_v128i_t wcn_v128i_sra_i32(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    int32_t shift = -vgetq_lane_s32(count.raw, 0);
    result.raw = vshlq_s32(a.raw, vdupq_n_s32(shift));
    return result;
}

#define wcn_v128i_srai_i32(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = vshrq_n_s32((a).raw, (imm)); \
        _result; \
    })

/* 64-bit logical shift left */
WCN_INLINE wcn_v128i_t wcn_v128i_sll_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    int64x2_t a64 = vreinterpretq_s64_s32(a.raw);
    int64_t shift = (int64_t)vgetq_lane_s32(count.raw, 0);
    result.raw = vreinterpretq_s32_s64(vshlq_s64(a64, vdupq_n_s64(shift)));
    return result;
}

#define wcn_v128i_slli_i64(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        int64x2_t _a64 = vreinterpretq_s64_s32((a).raw); \
        _result.raw = vreinterpretq_s32_s64(vshlq_n_s64(_a64, (imm))); \
        _result; \
    })

/* 64-bit logical shift right */
WCN_INLINE wcn_v128i_t wcn_v128i_srl_i64(wcn_v128i_t a, wcn_v128i_t count) {
    wcn_v128i_t result;
    uint64x2_t a64 = vreinterpretq_u64_s32(a.raw);
    int64_t shift = -(int64_t)vgetq_lane_s32(count.raw, 0);
    result.raw = vreinterpretq_s32_u64(vshlq_u64(a64, vdupq_n_s64(shift)));
    return result;
}

#define wcn_v128i_srli_i64(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        uint64x2_t _a64 = vreinterpretq_u64_s32((a).raw); \
        _result.raw = vreinterpretq_s32_u64(vshrq_n_u64(_a64, (imm))); \
        _result; \
    })

/* 128-bit byte shift left (shift by bytes) */
#define wcn_v128i_slli_si128(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        uint8x16_t _a8 = vreinterpretq_u8_s32((a).raw); \
        _result.raw = vreinterpretq_s32_u8(vextq_u8(vdupq_n_u8(0), _a8, 16 - (imm))); \
        _result; \
    })

/* 128-bit byte shift right (shift by bytes) */
#define wcn_v128i_srli_si128(a, imm) \
    ({ \
        wcn_v128i_t _result; \
        uint8x16_t _a8 = vreinterpretq_u8_s32((a).raw); \
        _result.raw = vreinterpretq_s32_u8(vextq_u8(_a8, vdupq_n_u8(0), (imm))); \
        _result; \
    })

/* ========== Comparison Operations ========== */

/* 8-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int8x16_t a8 = vreinterpretq_s8_s32(a.raw);
    int8x16_t b8 = vreinterpretq_s8_s32(b.raw);
    uint8x16_t cmp = vceqq_s8(a8, b8);
    result.raw = vreinterpretq_s32_u8(cmp);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int8x16_t a8 = vreinterpretq_s8_s32(a.raw);
    int8x16_t b8 = vreinterpretq_s8_s32(b.raw);
    uint8x16_t cmp = vcgtq_s8(a8, b8);
    result.raw = vreinterpretq_s32_u8(cmp);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int8x16_t a8 = vreinterpretq_s8_s32(a.raw);
    int8x16_t b8 = vreinterpretq_s8_s32(b.raw);
    uint8x16_t cmp = vcltq_s8(a8, b8);
    result.raw = vreinterpretq_s32_u8(cmp);
    return result;
}

/* 16-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16x8_t b16 = vreinterpretq_s16_s32(b.raw);
    uint16x8_t cmp = vceqq_s16(a16, b16);
    result.raw = vreinterpretq_s32_u16(cmp);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16x8_t b16 = vreinterpretq_s16_s32(b.raw);
    uint16x8_t cmp = vcgtq_s16(a16, b16);
    result.raw = vreinterpretq_s32_u16(cmp);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16x8_t b16 = vreinterpretq_s16_s32(b.raw);
    uint16x8_t cmp = vcltq_s16(a16, b16);
    result.raw = vreinterpretq_s32_u16(cmp);
    return result;
}

/* 32-bit integer comparisons */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpeq_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    uint32x4_t cmp = vceqq_s32(a.raw, b.raw);
    result.raw = vreinterpretq_s32_u32(cmp);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    uint32x4_t cmp = vcgtq_s32(a.raw, b.raw);
    result.raw = vreinterpretq_s32_u32(cmp);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_cmplt_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    uint32x4_t cmp = vcltq_s32(a.raw, b.raw);
    result.raw = vreinterpretq_s32_u32(cmp);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpeq(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    uint32x4_t cmp = vceqq_f32(a.raw, b.raw);
    result.raw = vreinterpretq_f32_u32(cmp);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmplt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    uint32x4_t cmp = vcltq_f32(a.raw, b.raw);
    result.raw = vreinterpretq_f32_u32(cmp);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmple(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    uint32x4_t cmp = vcleq_f32(a.raw, b.raw);
    result.raw = vreinterpretq_f32_u32(cmp);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpgt(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    uint32x4_t cmp = vcgtq_f32(a.raw, b.raw);
    result.raw = vreinterpretq_f32_u32(cmp);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_cmpge(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    uint32x4_t cmp = vcgeq_f32(a.raw, b.raw);
    result.raw = vreinterpretq_f32_u32(cmp);
    return result;
}

#ifdef WCN_ARM_AARCH64
/* Double comparison operations */
WCN_INLINE wcn_v128d_t wcn_v128d_cmpeq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64x2_t cmp = vceqq_f64(a.raw, b.raw);
    result.raw = vreinterpretq_f64_u64(cmp);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmplt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64x2_t cmp = vcltq_f64(a.raw, b.raw);
    result.raw = vreinterpretq_f64_u64(cmp);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmple(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64x2_t cmp = vcleq_f64(a.raw, b.raw);
    result.raw = vreinterpretq_f64_u64(cmp);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpgt(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64x2_t cmp = vcgtq_f64(a.raw, b.raw);
    result.raw = vreinterpretq_f64_u64(cmp);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpge(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64x2_t cmp = vcgeq_f64(a.raw, b.raw);
    result.raw = vreinterpretq_f64_u64(cmp);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_cmpneq(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    uint64x2_t cmp = vceqq_f64(a.raw, b.raw);
    result.raw = vreinterpretq_f64_u64(vmvnq_u64(cmp));  /* NOT of equal */
    return result;
}
#endif

/* ========== Min/Max Operations ========== */

/* 8-bit unsigned min/max (native) */
WCN_INLINE wcn_v128i_t wcn_v128i_min_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    uint8x16_t a8 = vreinterpretq_u8_s32(a.raw);
    uint8x16_t b8 = vreinterpretq_u8_s32(b.raw);
    result.raw = vreinterpretq_s32_u8(vminq_u8(a8, b8));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_u8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    uint8x16_t a8 = vreinterpretq_u8_s32(a.raw);
    uint8x16_t b8 = vreinterpretq_u8_s32(b.raw);
    result.raw = vreinterpretq_s32_u8(vmaxq_u8(a8, b8));
    return result;
}

/* 8-bit signed min/max (native in NEON) */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int8x16_t a8 = vreinterpretq_s8_s32(a.raw);
    int8x16_t b8 = vreinterpretq_s8_s32(b.raw);
    result.raw = vreinterpretq_s32_s8(vminq_s8(a8, b8));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int8x16_t a8 = vreinterpretq_s8_s32(a.raw);
    int8x16_t b8 = vreinterpretq_s8_s32(b.raw);
    result.raw = vreinterpretq_s32_s8(vmaxq_s8(a8, b8));
    return result;
}

/* 16-bit signed min/max (native) */
WCN_INLINE wcn_v128i_t wcn_v128i_min_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16x8_t b16 = vreinterpretq_s16_s32(b.raw);
    result.raw = vreinterpretq_s32_s16(vminq_s16(a16, b16));
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_max_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16x8_t b16 = vreinterpretq_s16_s32(b.raw);
    result.raw = vreinterpretq_s32_s16(vmaxq_s16(a16, b16));
    return result;
}

/* 32-bit signed min/max (native) */
WCN_INLINE wcn_v128i_t wcn_v128i_max_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vmaxq_s32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128i_min_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vminq_s32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_max(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vmaxq_f32(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_min(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    result.raw = vminq_f32(a.raw, b.raw);
    return result;
}

#ifdef WCN_ARM_AARCH64
WCN_INLINE wcn_v128d_t wcn_v128d_max(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vmaxq_f64(a.raw, b.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_min(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    result.raw = vminq_f64(a.raw, b.raw);
    return result;
}
#endif

/* ========== Square Root ========== */

#ifdef WCN_ARM_AARCH64
WCN_INLINE wcn_v128f_t wcn_v128f_sqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vsqrtq_f32(vec.raw);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_sqrt(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vsqrtq_f64(vec.raw);
    return result;
}
#endif

/* ========== Reciprocal and Reciprocal Square Root (Approximations) ========== */

/* Reciprocal approximation (1/x) for packed single-precision floats */
WCN_INLINE wcn_v128f_t wcn_v128f_rcp(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vrecpeq_f32(vec.raw);
    return result;
}

/* Reciprocal square root approximation (1/sqrt(x)) for packed single-precision floats */
WCN_INLINE wcn_v128f_t wcn_v128f_rsqrt(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vrsqrteq_f32(vec.raw);
    return result;
}

/* ========== Rounding Operations ========== */

#ifdef WCN_ARM_AARCH64
/* ARMv8/AArch64 has native rounding instructions */

/* Round floats toward negative infinity (floor) */
WCN_INLINE wcn_v128f_t wcn_v128f_floor(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vrndmq_f32(vec.raw);
    return result;
}

/* Round floats toward positive infinity (ceil) */
WCN_INLINE wcn_v128f_t wcn_v128f_ceil(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vrndpq_f32(vec.raw);
    return result;
}

/* Round floats to nearest integer */
WCN_INLINE wcn_v128f_t wcn_v128f_round_nearest(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vrndnq_f32(vec.raw);
    return result;
}

/* Round floats toward zero (truncate) */
WCN_INLINE wcn_v128f_t wcn_v128f_trunc(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vrndq_f32(vec.raw);
    return result;
}

/* Round doubles toward negative infinity (floor) */
WCN_INLINE wcn_v128d_t wcn_v128d_floor(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vrndmq_f64(vec.raw);
    return result;
}

/* Round doubles toward positive infinity (ceil) */
WCN_INLINE wcn_v128d_t wcn_v128d_ceil(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vrndpq_f64(vec.raw);
    return result;
}

/* Round doubles to nearest integer */
WCN_INLINE wcn_v128d_t wcn_v128d_round_nearest(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vrndnq_f64(vec.raw);
    return result;
}

/* Round doubles toward zero (truncate) */
WCN_INLINE wcn_v128d_t wcn_v128d_trunc(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vrndq_f64(vec.raw);
    return result;
}

#else
/* ARMv7 NEON doesn't have native rounding, use software emulation via conversion */

/* Round floats toward negative infinity (floor) - software emulation */
WCN_INLINE wcn_v128f_t wcn_v128f_floor(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Convert to int32 (truncates), then check if original was negative and had fraction */
    int32x4_t truncated = vcvtq_s32_f32(vec.raw);
    float32x4_t truncated_float = vcvtq_f32_s32(truncated);

    /* If vec < truncated_float, we need to subtract 1 */
    uint32x4_t needs_adjust = vcltq_f32(vec.raw, truncated_float);
    float32x4_t adjustment = vreinterpretq_f32_u32(vandq_u32(needs_adjust, vreinterpretq_u32_f32(vdupq_n_f32(1.0f))));

    result.raw = vsubq_f32(truncated_float, adjustment);
    return result;
}

/* Round floats toward positive infinity (ceil) - software emulation */
WCN_INLINE wcn_v128f_t wcn_v128f_ceil(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Convert to int32 (truncates), then check if original was positive and had fraction */
    int32x4_t truncated = vcvtq_s32_f32(vec.raw);
    float32x4_t truncated_float = vcvtq_f32_s32(truncated);

    /* If vec > truncated_float, we need to add 1 */
    uint32x4_t needs_adjust = vcgtq_f32(vec.raw, truncated_float);
    float32x4_t adjustment = vreinterpretq_f32_u32(vandq_u32(needs_adjust, vreinterpretq_u32_f32(vdupq_n_f32(1.0f))));

    result.raw = vaddq_f32(truncated_float, adjustment);
    return result;
}

/* Round floats to nearest integer - software emulation */
WCN_INLINE wcn_v128f_t wcn_v128f_round_nearest(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Add 0.5 and truncate (for positive), subtract 0.5 and truncate (for negative) */
    float32x4_t half = vdupq_n_f32(0.5f);

    /* Determine sign */
    uint32x4_t is_negative = vcltq_f32(vec.raw, vdupq_n_f32(0.0f));

    /* For positive: add 0.5, for negative: subtract 0.5 */
    float32x4_t adjusted = vbslq_f32(is_negative, vsubq_f32(vec.raw, half), vaddq_f32(vec.raw, half));

    /* Truncate */
    int32x4_t truncated = vcvtq_s32_f32(adjusted);
    result.raw = vcvtq_f32_s32(truncated);
    return result;
}

/* Round floats toward zero (truncate) - software emulation */
WCN_INLINE wcn_v128f_t wcn_v128f_trunc(wcn_v128f_t vec) {
    wcn_v128f_t result;
    /* Simple: convert to int32 and back (truncates toward zero) */
    int32x4_t truncated = vcvtq_s32_f32(vec.raw);
    result.raw = vcvtq_f32_s32(truncated);
    return result;
}

/* Note: ARMv7 NEON doesn't support f64, so no double rounding functions */
#endif

/* ========== Absolute Value Operations ========== */

/* 8-bit signed integer absolute value (native in NEON) */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i8(wcn_v128i_t a) {
    wcn_v128i_t result;
    int8x16_t a8 = vreinterpretq_s8_s32(a.raw);
    result.raw = vreinterpretq_s32_s8(vabsq_s8(a8));
    return result;
}

/* 16-bit signed integer absolute value (native in NEON) */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i16(wcn_v128i_t a) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    result.raw = vreinterpretq_s32_s16(vabsq_s16(a16));
    return result;
}

/* 32-bit signed integer absolute value (native in NEON) */
WCN_INLINE wcn_v128i_t wcn_v128i_abs_i32(wcn_v128i_t a) {
    wcn_v128i_t result;
    result.raw = vabsq_s32(a.raw);
    return result;
}

/* Float absolute value */
WCN_INLINE wcn_v128f_t wcn_v128f_abs(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vabsq_f32(vec.raw);
    return result;
}

#ifdef WCN_ARM_AARCH64
/* Double absolute value */
WCN_INLINE wcn_v128d_t wcn_v128d_abs(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vabsq_f64(vec.raw);
    return result;
}
#endif

/* ========== Negation Operations ========== */

/* 8-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i8(wcn_v128i_t a) {
    wcn_v128i_t result;
    int8x16_t a8 = vreinterpretq_s8_s32(a.raw);
    result.raw = vreinterpretq_s32_s8(vnegq_s8(a8));
    return result;
}

/* 16-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i16(wcn_v128i_t a) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    result.raw = vreinterpretq_s32_s16(vnegq_s16(a16));
    return result;
}

/* 32-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i32(wcn_v128i_t a) {
    wcn_v128i_t result;
    result.raw = vnegq_s32(a.raw);
    return result;
}

#ifdef WCN_ARM_AARCH64
/* 64-bit integer negation */
WCN_INLINE wcn_v128i_t wcn_v128i_neg_i64(wcn_v128i_t a) {
    wcn_v128i_t result;
    int64x2_t a64 = vreinterpretq_s64_s32(a.raw);
    result.raw = vreinterpretq_s32_s64(vnegq_s64(a64));
    return result;
}
#endif

/* Float negation */
WCN_INLINE wcn_v128f_t wcn_v128f_neg(wcn_v128f_t vec) {
    wcn_v128f_t result;
    result.raw = vnegq_f32(vec.raw);
    return result;
}

#ifdef WCN_ARM_AARCH64
/* Double negation */
WCN_INLINE wcn_v128d_t wcn_v128d_neg(wcn_v128d_t vec) {
    wcn_v128d_t result;
    result.raw = vnegq_f64(vec.raw);
    return result;
}
#endif

/* ========== Broadcast Operations ========== */

/* Load scalar and broadcast to all 32-bit integer lanes */
WCN_INLINE wcn_v128i_t wcn_v128i_broadcast_i32(const int32_t* ptr) {
    wcn_v128i_t result;
    result.raw = vld1q_dup_s32(ptr);
    return result;
}

/* Load scalar and broadcast to all float lanes */
WCN_INLINE wcn_v128f_t wcn_v128f_broadcast(const float* ptr) {
    wcn_v128f_t result;
    result.raw = vld1q_dup_f32(ptr);
    return result;
}

#ifdef WCN_ARM_AARCH64
/* Load scalar and broadcast to all double lanes */
WCN_INLINE wcn_v128d_t wcn_v128d_broadcast(const double* ptr) {
    wcn_v128d_t result;
    result.raw = vld1q_dup_f64(ptr);
    return result;
}
#endif

/* ========== Reduction Operations ========== */

WCN_INLINE float wcn_v128f_reduce_add(wcn_v128f_t vec) {
    return vaddvq_f32(vec.raw);
}

WCN_INLINE float wcn_v128f_reduce_max(wcn_v128f_t vec) {
    return vmaxvq_f32(vec.raw);
}

WCN_INLINE float wcn_v128f_reduce_min(wcn_v128f_t vec) {
    return vminvq_f32(vec.raw);
}

/* ========== Conversions ========== */

WCN_INLINE wcn_v128f_t wcn_v128i_to_v128f(wcn_v128i_t vec) {
    wcn_v128f_t result;
    result.raw = vcvtq_f32_s32(vec.raw);
    return result;
}

WCN_INLINE wcn_v128i_t wcn_v128f_to_v128i(wcn_v128f_t vec) {
    wcn_v128i_t result;
    result.raw = vcvtq_s32_f32(vec.raw);
    return result;
}

/* ========== Integer Multiplication ========== */

/* 16-bit integer multiplication (low 16 bits of result) */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vreinterpretq_s32_s16(vmulq_s16(vreinterpretq_s16_s32(a.raw), vreinterpretq_s16_s32(b.raw)));
    return result;
}

/* 16-bit integer multiplication (high 16 bits of result, signed) */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16x8_t b16 = vreinterpretq_s16_s32(b.raw);

    /* Split into low and high parts, multiply to 32-bit, extract high 16 bits */
    int32x4_t lo = vmull_s16(vget_low_s16(a16), vget_low_s16(b16));
    int32x4_t hi = vmull_high_s16(a16, b16);

    /* Shift right by 16 to get high 16 bits and narrow back to 16-bit */
    int16x4_t lo_high = vshrn_n_s32(lo, 16);
    int16x4_t hi_high = vshrn_n_s32(hi, 16);

    result.raw = vreinterpretq_s32_s16(vcombine_s16(lo_high, hi_high));
    return result;
}

/* 16-bit integer multiplication (high 16 bits of result, unsigned) */
WCN_INLINE wcn_v128i_t wcn_v128i_mulhi_u16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    uint16x8_t a16 = vreinterpretq_u16_s32(a.raw);
    uint16x8_t b16 = vreinterpretq_u16_s32(b.raw);

    /* Split into low and high parts, multiply to 32-bit, extract high 16 bits */
    uint32x4_t lo = vmull_u16(vget_low_u16(a16), vget_low_u16(b16));
    uint32x4_t hi = vmull_high_u16(a16, b16);

    /* Shift right by 16 to get high 16 bits and narrow back to 16-bit */
    uint16x4_t lo_high = vshrn_n_u32(lo, 16);
    uint16x4_t hi_high = vshrn_n_u32(hi, 16);

    result.raw = vreinterpretq_s32_u16(vcombine_u16(lo_high, hi_high));
    return result;
}

/* 32-bit integer multiplication (low 32 bits) - native in NEON */
WCN_INLINE wcn_v128i_t wcn_v128i_mullo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = vmulq_s32(a.raw, b.raw);
    return result;
}

/* Multiply packed unsigned 32-bit integers, produce 64-bit results */
WCN_INLINE wcn_v128i_t wcn_v128i_mul_u32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    uint32x4_t a32 = vreinterpretq_u32_s32(a.raw);
    uint32x4_t b32 = vreinterpretq_u32_s32(b.raw);

    /* Multiply low and high halves separately */
    uint64x2_t lo = vmull_u32(vget_low_u32(a32), vget_low_u32(b32));
    uint64x2_t hi = vmull_high_u32(a32, b32);

    /* Combine results (only store low 64-bit parts) */
    result.raw = vreinterpretq_s32_u64(vcombine_u64(vget_low_u64(lo), vget_low_u64(hi)));
    return result;
}

/* ========== Pack/Unpack Operations ========== */

/* Pack 32-bit signed integers to 16-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x4_t a_pack = vqmovn_s32(a.raw);
    int16x4_t b_pack = vqmovn_s32(b.raw);
    result.raw = vreinterpretq_s32_s16(vcombine_s16(a_pack, b_pack));
    return result;
}

/* Pack 16-bit signed integers to 8-bit with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packs_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16x8_t b16 = vreinterpretq_s16_s32(b.raw);
    int8x8_t a_pack = vqmovn_s16(a16);
    int8x8_t b_pack = vqmovn_s16(b16);
    result.raw = vreinterpretq_s32_s8(vcombine_s8(a_pack, b_pack));
    return result;
}

/* Pack 16-bit signed integers to 8-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x8_t a16 = vreinterpretq_s16_s32(a.raw);
    int16x8_t b16 = vreinterpretq_s16_s32(b.raw);
    uint8x8_t a_pack = vqmovun_s16(a16);
    uint8x8_t b_pack = vqmovun_s16(b16);
    result.raw = vreinterpretq_s32_u8(vcombine_u8(a_pack, b_pack));
    return result;
}

/* Pack 32-bit signed integers to 16-bit unsigned with saturation */
WCN_INLINE wcn_v128i_t wcn_v128i_packus_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    uint16x4_t a_pack = vqmovun_s32(a.raw);
    uint16x4_t b_pack = vqmovun_s32(b.raw);
    result.raw = vreinterpretq_s32_u16(vcombine_u16(a_pack, b_pack));
    return result;
}

/* Unpack and interleave low 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int8x8_t a_lo = vget_low_s8(vreinterpretq_s8_s32(a.raw));
    int8x8_t b_lo = vget_low_s8(vreinterpretq_s8_s32(b.raw));
    int8x8x2_t interleaved = vzip_s8(a_lo, b_lo);
    result.raw = vreinterpretq_s32_s8(vcombine_s8(interleaved.val[0], interleaved.val[1]));
    return result;
}

/* Unpack and interleave high 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i8(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int8x8_t a_hi = vget_high_s8(vreinterpretq_s8_s32(a.raw));
    int8x8_t b_hi = vget_high_s8(vreinterpretq_s8_s32(b.raw));
    int8x8x2_t interleaved = vzip_s8(a_hi, b_hi);
    result.raw = vreinterpretq_s32_s8(vcombine_s8(interleaved.val[0], interleaved.val[1]));
    return result;
}

/* Unpack and interleave low 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x4_t a_lo = vget_low_s16(vreinterpretq_s16_s32(a.raw));
    int16x4_t b_lo = vget_low_s16(vreinterpretq_s16_s32(b.raw));
    int16x4x2_t interleaved = vzip_s16(a_lo, b_lo);
    result.raw = vreinterpretq_s32_s16(vcombine_s16(interleaved.val[0], interleaved.val[1]));
    return result;
}

/* Unpack and interleave high 16-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i16(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int16x4_t a_hi = vget_high_s16(vreinterpretq_s16_s32(a.raw));
    int16x4_t b_hi = vget_high_s16(vreinterpretq_s16_s32(b.raw));
    int16x4x2_t interleaved = vzip_s16(a_hi, b_hi);
    result.raw = vreinterpretq_s32_s16(vcombine_s16(interleaved.val[0], interleaved.val[1]));
    return result;
}

/* Unpack and interleave low 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int32x2_t a_lo = vget_low_s32(a.raw);
    int32x2_t b_lo = vget_low_s32(b.raw);
    int32x2x2_t interleaved = vzip_s32(a_lo, b_lo);
    result.raw = vcombine_s32(interleaved.val[0], interleaved.val[1]);
    return result;
}

/* Unpack and interleave high 32-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i32(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int32x2_t a_hi = vget_high_s32(a.raw);
    int32x2_t b_hi = vget_high_s32(b.raw);
    int32x2x2_t interleaved = vzip_s32(a_hi, b_hi);
    result.raw = vcombine_s32(interleaved.val[0], interleaved.val[1]);
    return result;
}

/* Unpack and interleave low 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpacklo_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int64x1_t a_lo = vget_low_s64(vreinterpretq_s64_s32(a.raw));
    int64x1_t b_lo = vget_low_s64(vreinterpretq_s64_s32(b.raw));
    result.raw = vreinterpretq_s32_s64(vcombine_s64(a_lo, b_lo));
    return result;
}

/* Unpack and interleave high 64-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_unpackhi_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    int64x1_t a_hi = vget_high_s64(vreinterpretq_s64_s32(a.raw));
    int64x1_t b_hi = vget_high_s64(vreinterpretq_s64_s32(b.raw));
    result.raw = vreinterpretq_s32_s64(vcombine_s64(a_hi, b_hi));
    return result;
}

/* Unpack floats */
WCN_INLINE wcn_v128f_t wcn_v128f_unpacklo(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    float32x2_t a_lo = vget_low_f32(a.raw);
    float32x2_t b_lo = vget_low_f32(b.raw);
    float32x2x2_t interleaved = vzip_f32(a_lo, b_lo);
    result.raw = vcombine_f32(interleaved.val[0], interleaved.val[1]);
    return result;
}

WCN_INLINE wcn_v128f_t wcn_v128f_unpackhi(wcn_v128f_t a, wcn_v128f_t b) {
    wcn_v128f_t result;
    float32x2_t a_hi = vget_high_f32(a.raw);
    float32x2_t b_hi = vget_high_f32(b.raw);
    float32x2x2_t interleaved = vzip_f32(a_hi, b_hi);
    result.raw = vcombine_f32(interleaved.val[0], interleaved.val[1]);
    return result;
}

#ifdef WCN_ARM_AARCH64
/* Unpack doubles */
WCN_INLINE wcn_v128d_t wcn_v128d_unpacklo(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    float64x1_t a_lo = vget_low_f64(a.raw);
    float64x1_t b_lo = vget_low_f64(b.raw);
    result.raw = vcombine_f64(a_lo, b_lo);
    return result;
}

WCN_INLINE wcn_v128d_t wcn_v128d_unpackhi(wcn_v128d_t a, wcn_v128d_t b) {
    wcn_v128d_t result;
    float64x1_t a_hi = vget_high_f64(a.raw);
    float64x1_t b_hi = vget_high_f64(b.raw);
    result.raw = vcombine_f64(a_hi, b_hi);
    return result;
}
#endif

/* ========== Extract/Insert Operations ========== */

/* Extract 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_v128i_extract_i16(vec, index) vgetq_lane_s16(vreinterpretq_s16_s32((vec).raw), (index))

/* Insert 16-bit integer (index must be compile-time constant 0-7) */
#define wcn_v128i_insert_i16(vec, val, index) \
    ({ \
        wcn_v128i_t _result; \
        _result.raw = vreinterpretq_s32_s16(vsetq_lane_s16((val), vreinterpretq_s16_s32((vec).raw), (index))); \
        _result; \
    })

/* ========== Movemask Operations ========== */

/* Create mask from most significant bit of each 8-bit element */
WCN_INLINE int wcn_v128i_movemask_i8(wcn_v128i_t vec) {
    /* ARM NEON doesn't have direct movemask, simulate with shifts and extracts */
    uint8x16_t input = vreinterpretq_u8_s32(vec.raw);

    /* Shift to get sign bits, then extract and combine */
    uint8x16_t high_bits = vshrq_n_u8(input, 7);

    /* Pack into 64-bit values */
    uint64x2_t packed = vreinterpretq_u64_u8(high_bits);

    /* Extract and combine bits using scalar operations */
    uint8_t bytes[16];
    vst1q_u8(bytes, high_bits);

    int result = 0;
    for (int i = 0; i < 16; i++) {
        result |= (bytes[i] & 1) << i;
    }

    return result;
}

/* Create mask from most significant bit of each float */
WCN_INLINE int wcn_v128f_movemask(wcn_v128f_t vec) {
    /* Extract sign bits from floats */
    uint32x4_t bits = vreinterpretq_u32_f32(vec.raw);
    uint32x4_t sign_bits = vshrq_n_u32(bits, 31);

    /* Extract lanes and combine */
    int result = 0;
    result |= vgetq_lane_u32(sign_bits, 0) << 0;
    result |= vgetq_lane_u32(sign_bits, 1) << 1;
    result |= vgetq_lane_u32(sign_bits, 2) << 2;
    result |= vgetq_lane_u32(sign_bits, 3) << 3;

    return result;
}

#ifdef WCN_ARM_AARCH64
/* Create mask from most significant bit of each double */
WCN_INLINE int wcn_v128d_movemask(wcn_v128d_t vec) {
    /* Extract sign bits from doubles */
    uint64x2_t bits = vreinterpretq_u64_f64(vec.raw);
    uint64x2_t sign_bits = vshrq_n_u64(bits, 63);

    /* Extract lanes and combine */
    int result = 0;
    result |= (int)vgetq_lane_u64(sign_bits, 0) << 0;
    result |= (int)vgetq_lane_u64(sign_bits, 1) << 1;

    return result;
}
#endif

/* ========== Blend/Select Operations ========== */

/* Variable blend floats (blends based on sign bit of mask) */
WCN_INLINE wcn_v128f_t wcn_v128f_blendv(wcn_v128f_t a, wcn_v128f_t b, wcn_v128f_t mask) {
    wcn_v128f_t result;
    /* Use bitwise select: result = (mask & b) | (~mask & a) */
    result.raw = vbslq_f32(vreinterpretq_u32_f32(mask.raw), b.raw, a.raw);
    return result;
}

#ifdef WCN_ARM_AARCH64
/* Variable blend doubles */
WCN_INLINE wcn_v128d_t wcn_v128d_blendv(wcn_v128d_t a, wcn_v128d_t b, wcn_v128d_t mask) {
    wcn_v128d_t result;
    result.raw = vbslq_f64(vreinterpretq_u64_f64(mask.raw), b.raw, a.raw);
    return result;
}
#endif

/* Variable blend 8-bit integers */
WCN_INLINE wcn_v128i_t wcn_v128i_blendv_i8(wcn_v128i_t a, wcn_v128i_t b, wcn_v128i_t mask) {
    wcn_v128i_t result;
    uint8x16_t mask_u8 = vreinterpretq_u8_s32(mask.raw);
    int8x16_t a_i8 = vreinterpretq_s8_s32(a.raw);
    int8x16_t b_i8 = vreinterpretq_s8_s32(b.raw);
    result.raw = vreinterpretq_s32_s8(vbslq_s8(mask_u8, b_i8, a_i8));
    return result;
}

/* ========== Byte Shuffle ========== */

/* Shuffle bytes according to indices in control mask */
WCN_INLINE wcn_v128i_t wcn_v128i_shuffle_i8(wcn_v128i_t vec, wcn_v128i_t control) {
    wcn_v128i_t result;
    uint8x16_t src = vreinterpretq_u8_s32(vec.raw);
    uint8x16_t idx = vreinterpretq_u8_s32(control.raw);

#ifdef WCN_ARM_AARCH64
    /* Use NEON table lookup (supports full 16-byte table) */
    result.raw = vreinterpretq_s32_u8(vqtbl1q_u8(src, idx));
#else
    /* ARMv7 NEON uses vtbl which only supports 8-byte tables, need workaround */
    uint8x8x2_t table;
    table.val[0] = vget_low_u8(src);
    table.val[1] = vget_high_u8(src);

    uint8x8_t result_lo = vtbl2_u8(table, vget_low_u8(idx));
    uint8x8_t result_hi = vtbl2_u8(table, vget_high_u8(idx));

    result.raw = vreinterpretq_s32_u8(vcombine_u8(result_lo, result_hi));
#endif

    return result;
}

#endif /* WCN_ARM_NEON */

#endif /* WCN_ARM_NEON_H */
