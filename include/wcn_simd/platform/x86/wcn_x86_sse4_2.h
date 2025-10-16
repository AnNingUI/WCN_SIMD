#ifndef WCN_X86_SSE4_2_H
#define WCN_X86_SSE4_2_H

#include "../../wcn_types.h"

#ifdef WCN_X86_SSE4_2

#include <nmmintrin.h>

/* SSE4.2 adds string/text processing instructions and CRC32
 * Note: Basic SIMD operations are inherited from SSE4.1
 * This file contains SSE4.2-specific enhancements */

/* ========== String Comparison Operations ========== */

/* String comparison modes for _mm_cmpXstrX instructions */
#define WCN_SIMD_CMP_UBYTE_EQUAL_ANY             0x00  /* unsigned byte, equal any */
#define WCN_SIMD_CMP_UWORD_EQUAL_ANY             0x01  /* unsigned word, equal any */
#define WCN_SIMD_CMP_SBYTE_EQUAL_ANY             0x02  /* signed byte, equal any */
#define WCN_SIMD_CMP_SWORD_EQUAL_ANY             0x03  /* signed word, equal any */
#define WCN_SIMD_CMP_UBYTE_RANGES                0x04  /* unsigned byte, ranges */
#define WCN_SIMD_CMP_UWORD_RANGES                0x05  /* unsigned word, ranges */
#define WCN_SIMD_CMP_SBYTE_RANGES                0x06  /* signed byte, ranges */
#define WCN_SIMD_CMP_SWORD_RANGES                0x07  /* signed word, ranges */
#define WCN_SIMD_CMP_UBYTE_EQUAL_EACH            0x08  /* unsigned byte, equal each */
#define WCN_SIMD_CMP_UWORD_EQUAL_EACH            0x09  /* unsigned word, equal each */
#define WCN_SIMD_CMP_SBYTE_EQUAL_EACH            0x0A  /* signed byte, equal each */
#define WCN_SIMD_CMP_SWORD_EQUAL_EACH            0x0B  /* signed word, equal each */
#define WCN_SIMD_CMP_UBYTE_EQUAL_ORDERED         0x0C  /* unsigned byte, equal ordered */
#define WCN_SIMD_CMP_UWORD_EQUAL_ORDERED         0x0D  /* unsigned word, equal ordered */
#define WCN_SIMD_CMP_SBYTE_EQUAL_ORDERED         0x0E  /* signed byte, equal ordered */
#define WCN_SIMD_CMP_SWORD_EQUAL_ORDERED         0x0F  /* signed word, equal ordered */

/* String comparison result flags */
#define WCN_SIMD_CMP_NEGATIVE_POLARITY           0x10  /* negate results */
#define WCN_SIMD_CMP_MASKED_NEGATIVE_POLARITY    0x30  /* negate results before end of string */

/* ========== Explicit Length String Compare ========== */

/* Compare strings with explicit lengths
 * Returns index in mask, imm8 controls comparison mode */
#define wcn_v128i_cmpestri(a, la, b, lb, imm8) \
    _mm_cmpestri((a).raw, (la), (b).raw, (lb), (imm8))

/* Compare strings with explicit lengths, return mask */
#define wcn_v128i_cmpestrm(a, la, b, lb, imm8) ({ \
    wcn_v128i_t _result; \
    _result.raw = _mm_cmpestrm((a).raw, (la), (b).raw, (lb), (imm8)); \
    _result; \
})

/* Compare strings with explicit lengths, return 1 if any character matched */
#define wcn_v128i_cmpestra(a, la, b, lb, imm8) \
    _mm_cmpestra((a).raw, (la), (b).raw, (lb), (imm8))

/* Compare strings with explicit lengths, return 1 if any character in b is null */
#define wcn_v128i_cmpestrc(a, la, b, lb, imm8) \
    _mm_cmpestrc((a).raw, (la), (b).raw, (lb), (imm8))

/* Compare strings with explicit lengths, return 1 if resulting mask is all ones */
#define wcn_v128i_cmpestro(a, la, b, lb, imm8) \
    _mm_cmpestro((a).raw, (la), (b).raw, (lb), (imm8))

/* Compare strings with explicit lengths, return 1 if resulting mask is all zeros */
#define wcn_v128i_cmpestrs(a, la, b, lb, imm8) \
    _mm_cmpestrs((a).raw, (la), (b).raw, (lb), (imm8))

/* Compare strings with explicit lengths, return 1 if any character in a is null */
#define wcn_v128i_cmpestrz(a, la, b, lb, imm8) \
    _mm_cmpestrz((a).raw, (la), (b).raw, (lb), (imm8))

/* ========== Implicit Length String Compare (NULL-terminated) ========== */

/* Compare NULL-terminated strings, return index */
#define wcn_v128i_cmpistri(a, b, imm8) \
    _mm_cmpistri((a).raw, (b).raw, (imm8))

/* Compare NULL-terminated strings, return mask */
#define wcn_v128i_cmpistrm(a, b, imm8) ({ \
    wcn_v128i_t _result; \
    _result.raw = _mm_cmpistrm((a).raw, (b).raw, (imm8)); \
    _result; \
})

/* Compare NULL-terminated strings, return 1 if any character matched */
#define wcn_v128i_cmpistra(a, b, imm8) \
    _mm_cmpistra((a).raw, (b).raw, (imm8))

/* Compare NULL-terminated strings, return 1 if any character in b is null */
#define wcn_v128i_cmpistrc(a, b, imm8) \
    _mm_cmpistrc((a).raw, (b).raw, (imm8))

/* Compare NULL-terminated strings, return 1 if resulting mask is all ones */
#define wcn_v128i_cmpistro(a, b, imm8) \
    _mm_cmpistro((a).raw, (b).raw, (imm8))

/* Compare NULL-terminated strings, return 1 if resulting mask is all zeros */
#define wcn_v128i_cmpistrs(a, b, imm8) \
    _mm_cmpistrs((a).raw, (b).raw, (imm8))

/* Compare NULL-terminated strings, return 1 if any character in a is null */
#define wcn_v128i_cmpistrz(a, b, imm8) \
    _mm_cmpistrz((a).raw, (b).raw, (imm8))

/* ========== CRC32 Instructions ========== */

/* Accumulate CRC32 on 8-bit value */
WCN_INLINE uint32_t wcn_crc32_u8(uint32_t crc, uint8_t value) {
    return _mm_crc32_u8(crc, value);
}

/* Accumulate CRC32 on 16-bit value */
WCN_INLINE uint32_t wcn_crc32_u16(uint32_t crc, uint16_t value) {
    return _mm_crc32_u16(crc, value);
}

/* Accumulate CRC32 on 32-bit value */
WCN_INLINE uint32_t wcn_crc32_u32(uint32_t crc, uint32_t value) {
    return _mm_crc32_u32(crc, value);
}

#ifdef __x86_64__
/* Accumulate CRC32 on 64-bit value (x64 only) */
WCN_INLINE uint64_t wcn_crc32_u64(uint64_t crc, uint64_t value) {
    return _mm_crc32_u64(crc, value);
}
#endif

/* ========== CRC32 Helper Functions ========== */

/* Calculate CRC32 for a buffer */
WCN_INLINE uint32_t wcn_crc32_buffer(const void* data, size_t length, uint32_t initial_crc) {
    const uint8_t* ptr = (const uint8_t*)data;
    uint32_t crc = initial_crc;
    size_t i = 0;

#ifdef __x86_64__
    /* Process 8 bytes at a time on x64 */
    for (; i + 8 <= length; i += 8) {
        uint64_t value;
        __builtin_memcpy(&value, ptr + i, sizeof(value));
        crc = (uint32_t)wcn_crc32_u64(crc, value);
    }
#else
    /* Process 4 bytes at a time on x86 */
    for (; i + 4 <= length; i += 4) {
        uint32_t value;
        __builtin_memcpy(&value, ptr + i, sizeof(value));
        crc = wcn_crc32_u32(crc, value);
    }
#endif

    /* Process remaining bytes */
    for (; i < length; i++) {
        crc = wcn_crc32_u8(crc, ptr[i]);
    }

    return crc;
}

/* ========== Enhanced Integer Comparison (64-bit) ========== */

/* Note: wcn_v128i_cmpeq_i64 is already defined in SSE4.1 */

/* ========== Population Count (Bit Count) ========== */

/* Count number of set bits in 32-bit integer */
WCN_INLINE int wcn_popcnt_u32(uint32_t value) {
#ifdef __POPCNT__
    return _mm_popcnt_u32(value);
#else
    /* Fallback implementation */
    value = value - ((value >> 1) & 0x55555555);
    value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
    return (((value + (value >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
#endif
}

#ifdef __x86_64__
/* Count number of set bits in 64-bit integer */
WCN_INLINE int wcn_popcnt_u64(uint64_t value) {
#ifdef __POPCNT__
    return (int)_mm_popcnt_u64(value);
#else
    /* Fallback implementation */
    value = value - ((value >> 1) & 0x5555555555555555ULL);
    value = (value & 0x3333333333333333ULL) + ((value >> 2) & 0x3333333333333333ULL);
    value = (value + (value >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (int)((value * 0x0101010101010101ULL) >> 56);
#endif
}
#endif

/* ========== Greater-Than Comparison for 64-bit Signed Integers ========== */

/* 64-bit signed integer greater-than comparison */
WCN_INLINE wcn_v128i_t wcn_v128i_cmpgt_i64(wcn_v128i_t a, wcn_v128i_t b) {
    wcn_v128i_t result;
    result.raw = _mm_cmpgt_epi64(a.raw, b.raw);
    return result;
}

/* ========== Helper Functions for String Processing ========== */

/* Find first occurrence of character in string (vectorized) */
WCN_INLINE int wcn_strchr_v128(const char* str, char ch) {
    wcn_v128i_t needle = wcn_v128i_set1_i8(ch);
    wcn_v128i_t haystack;
    haystack.raw = _mm_loadu_si128((const __m128i*)str);
    
    int index = wcn_v128i_cmpistri(haystack, needle, 
                                    WCN_SIMD_CMP_UBYTE_EQUAL_ANY);
    return index < 16 ? index : -1;
}

/* Get length of NULL-terminated string up to 16 bytes */
WCN_INLINE int wcn_strlen_v128(const char* str) {
    wcn_v128i_t zero = wcn_v128i_setzero();
    wcn_v128i_t data;
    data.raw = _mm_loadu_si128((const __m128i*)str);
    
    return wcn_v128i_cmpistri(data, zero, WCN_SIMD_CMP_UBYTE_EQUAL_EACH);
}

#endif /* WCN_X86_SSE4_2 */

#endif /* WCN_X86_SSE4_2_H */
