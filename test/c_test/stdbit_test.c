#include "c_test.h"

#include <stdbit.h>
#include <limits.h>

TEST_CASE(stdbit, version_and_endian_macros) {
#ifdef __STDC_VERSION_STDBIT_H__
    test_true( __STDC_VERSION_STDBIT_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_STDBIT_H__");
#else
    TEST_SKIP1(); TEST_NOTE("__STDC_VERSION_STDBIT_H__ not defined");
    test_skip("c2023:__STDC_VERSION_STDBIT_H__");
#endif

    test_ne( __STDC_ENDIAN_LITTLE__, __STDC_ENDIAN_BIG__ );
    test_pass("c2023:__STDC_ENDIAN_LITTLE__");
    test_true( __STDC_ENDIAN_BIG__ != __STDC_ENDIAN_LITTLE__ );
    test_pass("c2023:__STDC_ENDIAN_BIG__");
    test_eq( __STDC_ENDIAN_NATIVE__, __STDC_ENDIAN_LITTLE__ );
    test_pass("c2023:__STDC_ENDIAN_NATIVE__");
}

TEST_CASE(stdbit, generic_dispatch_uses_operand_width) {
    const unsigned char  c8  = 0x0Fu;
    const unsigned short s16 = 0x0F00u;
    const unsigned int   u32 = 0x0000FF00u;
    const unsigned long  l32 = 0x0000FF00uL;
    const unsigned long long    u64 = (unsigned long long)1 << 40;

    test_eq( stdc_leading_zeros(c8), 4u );
    test_eq( stdc_leading_zeros(s16), 4u );
    test_eq( stdc_leading_zeros(u32), 16u );

    test_eq( stdc_leading_zeros(l32), (unsigned)(sizeof(l32)*CHAR_BIT - 16) );
    test_eq( stdc_leading_zeros(u64), 23u );

    test_eq( stdc_leading_zeros((unsigned char)0), 8u );
    test_eq( stdc_leading_zeros((unsigned int)0), 32u );
    test_eq( stdc_leading_zeros((unsigned long long)0), 64u );

    test_eq( stdc_count_ones(u32), stdc_count_ones_ui(u32) );
    test_eq( stdc_count_ones(c8), stdc_count_ones_uc(c8) );
    test_eq( stdc_count_ones(u64), stdc_count_ones_ull(u64) );
    test_pass("c2023:stdc_leading_zeros");
}

TEST_CASE(stdbit, leading_trailing) {
    const unsigned char c8  = 0x0Fu;
    const unsigned int  u32 = 0x0000FF00u;

    test_eq( stdc_leading_ones((unsigned char)0xF0u), 4u );
    test_eq( stdc_leading_ones((unsigned char)0xFFu), 8u );
    test_eq( stdc_leading_ones((unsigned char)0), 0u );
    test_pass("c2023:stdc_leading_ones");

    test_eq( stdc_trailing_zeros(u32), 8u );
    test_eq( stdc_trailing_zeros((unsigned int)0), 32u );
    test_eq( stdc_trailing_zeros((unsigned int)1), 0u );
    test_pass("c2023:stdc_trailing_zeros");

    test_eq( stdc_trailing_ones(c8), 4u );
    test_eq( stdc_trailing_ones((unsigned char)0), 0u );
    test_eq( stdc_trailing_ones((unsigned char)0xFFu), 8u );
    test_pass("c2023:stdc_trailing_ones");
}

TEST_CASE(stdbit, first_leading_trailing) {
    const unsigned char c8  = 0x0Fu;
    const unsigned int  u32 = 0x0000FF00u;

    test_eq( stdc_first_leading_one(u32), 17u );
    test_eq( stdc_first_leading_one((unsigned int)0), 0u );
    test_pass("c2023:stdc_first_leading_one");
    test_eq( stdc_first_leading_zero((unsigned char)0xFFu), 0u );
    test_eq( stdc_first_leading_zero((unsigned char)0x7Fu), 1u );
    test_pass("c2023:stdc_first_leading_zero");
    test_eq( stdc_first_trailing_one(u32), 9u );
    test_eq( stdc_first_trailing_one((unsigned int)0), 0u );
    test_pass("c2023:stdc_first_trailing_one");
    test_eq( stdc_first_trailing_zero(c8), 5u );
    test_eq( stdc_first_trailing_zero((unsigned char)0xFFu), 0u );
    test_pass("c2023:stdc_first_trailing_zero");
}

TEST_CASE(stdbit, counts_and_bit_shapes) {
    const unsigned char c8  = 0x0Fu;
    const unsigned int  u32 = 0x0000FF00u;
    const unsigned long long   u64 = (unsigned long long)1 << 40;

    test_eq( stdc_count_ones(u32), 8u );
    test_eq( stdc_count_ones((unsigned char)0xFFu), 8u );
    test_pass("c2023:stdc_count_ones");
    test_eq( stdc_count_zeros(u32), 24u );
    test_eq( stdc_count_zeros(c8), 4u );
    test_pass("c2023:stdc_count_zeros");

    test_true(  stdc_has_single_bit(u64) );
    test_true( !stdc_has_single_bit(u32) );
    test_true( !stdc_has_single_bit((unsigned int)0) );
    test_true(  stdc_has_single_bit((unsigned int)1) );
    test_pass("c2023:stdc_has_single_bit");

    test_eq( stdc_bit_width(u32), 16u );
    test_eq( stdc_bit_width((unsigned int)0), 0u );
    test_eq( stdc_bit_width((unsigned int)1), 1u );
    test_pass("c2023:stdc_bit_width");

    test_eq( stdc_bit_floor((unsigned int)5), 4u );
    test_eq( stdc_bit_floor((unsigned int)0), 0u );
    test_pass("c2023:stdc_bit_floor");
    test_eq( stdc_bit_ceil((unsigned int)5), 8u );
    test_eq( stdc_bit_ceil((unsigned int)0), 1u );
    test_eq( stdc_bit_ceil((unsigned int)1), 1u );
    test_pass("c2023:stdc_bit_ceil");
}

TEST_CASE(stdbit, per_type_spellings) {
    const unsigned char  c8  = 0x0Fu;
    const unsigned short s16 = 0x0F00u;
    const unsigned int   u32 = 0x0000FF00u;
    const unsigned long  l32 = 0x0000FF00uL;
    const unsigned long long    u64 = (unsigned long long)1 << 40;

    test_eq( stdc_leading_zeros_uc(c8), 4u );
    test_eq( stdc_leading_zeros_us(s16), 4u );
    test_eq( stdc_leading_zeros_ui(u32), 16u );
    test_eq( stdc_leading_zeros_ul(l32), (unsigned)(sizeof(l32)*CHAR_BIT - 16) );
    test_eq( stdc_leading_zeros_ull(u64), 23u );
    test_pass("c2023:stdc_leading_zeros per-type spellings");

    test_eq( stdc_leading_ones_uc((unsigned char)0xF0u), 4u );
    test_pass("c2023:stdc_leading_ones per-type spellings");
    test_eq( stdc_trailing_zeros_ui(u32), 8u );
    test_pass("c2023:stdc_trailing_zeros per-type spellings");
    test_eq( stdc_trailing_ones_uc(c8), 4u );
    test_pass("c2023:stdc_trailing_ones per-type spellings");
    test_eq( stdc_first_leading_zero_uc((unsigned char)0x7Fu), 1u );
    test_pass("c2023:stdc_first_leading_zero per-type spellings");
    test_eq( stdc_first_leading_one_ui(u32), 17u );
    test_pass("c2023:stdc_first_leading_one per-type spellings");
    test_eq( stdc_first_trailing_zero_uc(c8), 5u );
    test_pass("c2023:stdc_first_trailing_zero per-type spellings");
    test_eq( stdc_first_trailing_one_ui(u32), 9u );
    test_pass("c2023:stdc_first_trailing_one per-type spellings");
    test_eq( stdc_count_ones_ui(u32), 8u );
    test_pass("c2023:stdc_count_ones per-type spellings");
    test_eq( stdc_count_zeros_uc(c8), 4u );
    test_pass("c2023:stdc_count_zeros per-type spellings");

    test_true(  stdc_has_single_bit_ull(u64) );
    test_true( !stdc_has_single_bit_ui(u32) );
    test_pass("c2023:stdc_has_single_bit per-type spellings");

    test_eq( stdc_bit_width_ui(u32), 16u );
    test_pass("c2023:stdc_bit_width per-type spellings");
    test_eq( stdc_bit_floor_ui(5u), 4u );
    test_eq( stdc_bit_floor_ull(u64 + 1), u64 );
    test_pass("c2023:stdc_bit_floor per-type spellings");
    test_eq( stdc_bit_ceil_ui(5u), 8u );
    test_eq( stdc_bit_ceil_uc((unsigned char)5), 8u );
    test_pass("c2023:stdc_bit_ceil per-type spellings");
}

TEST_CASE(stdbit, a26_constant_folded_64bit) {
    test_eq( stdc_leading_zeros((unsigned long long)1 << 40), 23u );
    test_eq( stdc_leading_zeros((unsigned long long)1 << 63), 0u );
    test_eq( stdc_leading_zeros((unsigned long long)1), 63u );
    test_eq( stdc_leading_zeros((unsigned long long)0), 64u );

    test_eq( stdc_trailing_zeros((unsigned long long)1 << 40), 40u );
    test_eq( stdc_trailing_zeros((unsigned long long)0), 64u );

    test_eq( stdc_bit_width((unsigned long long)1 << 40), 41u );
    test_eq( stdc_count_ones((unsigned long long)0xF0F0F0F0F0F0F0F0ULL), 32u );

    test_true(  stdc_has_single_bit((unsigned long long)1 << 40) );
    test_true( !stdc_has_single_bit((unsigned long long)3) );

    test_eq( stdc_bit_floor((unsigned long long)5), (unsigned long long)4 );
    test_eq( stdc_bit_ceil((unsigned long long)5), (unsigned long long)8 );
    test_pass("c2023:64-bit compile-time-constant argument (A26 guard)");
}

TEST_CASE(stdbit, bit_ceil_family_c2023) {
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_ceil_uc((unsigned char)9), (int)(16) );
    test_pass("c2023:stdc_bit_ceil_uc");
#else
    test_skip("c2023:stdc_bit_ceil_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_ceil_ui((unsigned int)9), (int)(16) );
    test_pass("c2023:stdc_bit_ceil_ui");
#else
    test_skip("c2023:stdc_bit_ceil_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_ceil_ul((unsigned long)9), (int)(16) );
    test_pass("c2023:stdc_bit_ceil_ul");
#else
    test_skip("c2023:stdc_bit_ceil_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_ceil_ull((unsigned long long)9), (int)(16) );
    test_pass("c2023:stdc_bit_ceil_ull");
#else
    test_skip("c2023:stdc_bit_ceil_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_ceil_us((unsigned short)9), (int)(16) );
    test_pass("c2023:stdc_bit_ceil_us");
#else
    test_skip("c2023:stdc_bit_ceil_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_floor_uc((unsigned char)9), (int)(8) );
    test_pass("c2023:stdc_bit_floor_uc");
#else
    test_skip("c2023:stdc_bit_floor_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_floor_ui((unsigned int)9), (int)(8) );
    test_pass("c2023:stdc_bit_floor_ui");
#else
    test_skip("c2023:stdc_bit_floor_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_floor_ul((unsigned long)9), (int)(8) );
    test_pass("c2023:stdc_bit_floor_ul");
#else
    test_skip("c2023:stdc_bit_floor_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_floor_ull((unsigned long long)9), (int)(8) );
    test_pass("c2023:stdc_bit_floor_ull");
#else
    test_skip("c2023:stdc_bit_floor_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_floor_us((unsigned short)9), (int)(8) );
    test_pass("c2023:stdc_bit_floor_us");
#else
    test_skip("c2023:stdc_bit_floor_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_width_uc((unsigned char)8), (int)(4) );
    test_pass("c2023:stdc_bit_width_uc");
#else
    test_skip("c2023:stdc_bit_width_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_width_ui((unsigned int)8), (int)(4) );
    test_pass("c2023:stdc_bit_width_ui");
#else
    test_skip("c2023:stdc_bit_width_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_width_ul((unsigned long)8), (int)(4) );
    test_pass("c2023:stdc_bit_width_ul");
#else
    test_skip("c2023:stdc_bit_width_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_width_ull((unsigned long long)8), (int)(4) );
    test_pass("c2023:stdc_bit_width_ull");
#else
    test_skip("c2023:stdc_bit_width_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_bit_width_us((unsigned short)8), (int)(4) );
    test_pass("c2023:stdc_bit_width_us");
#else
    test_skip("c2023:stdc_bit_width_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_ones_uc((unsigned char)7), (int)(3) );
    test_pass("c2023:stdc_count_ones_uc");
#else
    test_skip("c2023:stdc_count_ones_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_ones_ui((unsigned int)7), (int)(3) );
    test_pass("c2023:stdc_count_ones_ui");
#else
    test_skip("c2023:stdc_count_ones_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_ones_ul((unsigned long)7), (int)(3) );
    test_pass("c2023:stdc_count_ones_ul");
#else
    test_skip("c2023:stdc_count_ones_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_ones_ull((unsigned long long)7), (int)(3) );
    test_pass("c2023:stdc_count_ones_ull");
#else
    test_skip("c2023:stdc_count_ones_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_ones_us((unsigned short)7), (int)(3) );
    test_pass("c2023:stdc_count_ones_us");
#else
    test_skip("c2023:stdc_count_ones_us");
#endif
}

TEST_CASE(stdbit, count_zeros_family_c2023) {
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_zeros_uc((unsigned char)0), (int)((int)(sizeof(unsigned char) * CHAR_BIT)) );
    test_pass("c2023:stdc_count_zeros_uc");
#else
    test_skip("c2023:stdc_count_zeros_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_zeros_ui((unsigned int)0), (int)((int)(sizeof(unsigned int) * CHAR_BIT)) );
    test_pass("c2023:stdc_count_zeros_ui");
#else
    test_skip("c2023:stdc_count_zeros_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_zeros_ul((unsigned long)0), (int)((int)(sizeof(unsigned long) * CHAR_BIT)) );
    test_pass("c2023:stdc_count_zeros_ul");
#else
    test_skip("c2023:stdc_count_zeros_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_zeros_ull((unsigned long long)0), (int)((int)(sizeof(unsigned long long) * CHAR_BIT)) );
    test_pass("c2023:stdc_count_zeros_ull");
#else
    test_skip("c2023:stdc_count_zeros_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_count_zeros_us((unsigned short)0), (int)((int)(sizeof(unsigned short) * CHAR_BIT)) );
    test_pass("c2023:stdc_count_zeros_us");
#else
    test_skip("c2023:stdc_count_zeros_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_one_uc((unsigned char)1), (int)((int)(sizeof(unsigned char) * CHAR_BIT)) );
    test_pass("c2023:stdc_first_leading_one_uc");
#else
    test_skip("c2023:stdc_first_leading_one_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_one_ui((unsigned int)1), (int)((int)(sizeof(unsigned int) * CHAR_BIT)) );
    test_pass("c2023:stdc_first_leading_one_ui");
#else
    test_skip("c2023:stdc_first_leading_one_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_one_ul((unsigned long)1), (int)((int)(sizeof(unsigned long) * CHAR_BIT)) );
    test_pass("c2023:stdc_first_leading_one_ul");
#else
    test_skip("c2023:stdc_first_leading_one_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_one_ull((unsigned long long)1), (int)((int)(sizeof(unsigned long long) * CHAR_BIT)) );
    test_pass("c2023:stdc_first_leading_one_ull");
#else
    test_skip("c2023:stdc_first_leading_one_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_one_us((unsigned short)1), (int)((int)(sizeof(unsigned short) * CHAR_BIT)) );
    test_pass("c2023:stdc_first_leading_one_us");
#else
    test_skip("c2023:stdc_first_leading_one_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_zero_uc((unsigned char)0), (int)(1) );
    test_pass("c2023:stdc_first_leading_zero_uc");
#else
    test_skip("c2023:stdc_first_leading_zero_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_zero_ui((unsigned int)0), (int)(1) );
    test_pass("c2023:stdc_first_leading_zero_ui");
#else
    test_skip("c2023:stdc_first_leading_zero_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_zero_ul((unsigned long)0), (int)(1) );
    test_pass("c2023:stdc_first_leading_zero_ul");
#else
    test_skip("c2023:stdc_first_leading_zero_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_zero_ull((unsigned long long)0), (int)(1) );
    test_pass("c2023:stdc_first_leading_zero_ull");
#else
    test_skip("c2023:stdc_first_leading_zero_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_leading_zero_us((unsigned short)0), (int)(1) );
    test_pass("c2023:stdc_first_leading_zero_us");
#else
    test_skip("c2023:stdc_first_leading_zero_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_one_uc((unsigned char)8), (int)(4) );
    test_pass("c2023:stdc_first_trailing_one_uc");
#else
    test_skip("c2023:stdc_first_trailing_one_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_one_ui((unsigned int)8), (int)(4) );
    test_pass("c2023:stdc_first_trailing_one_ui");
#else
    test_skip("c2023:stdc_first_trailing_one_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_one_ul((unsigned long)8), (int)(4) );
    test_pass("c2023:stdc_first_trailing_one_ul");
#else
    test_skip("c2023:stdc_first_trailing_one_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_one_ull((unsigned long long)8), (int)(4) );
    test_pass("c2023:stdc_first_trailing_one_ull");
#else
    test_skip("c2023:stdc_first_trailing_one_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_one_us((unsigned short)8), (int)(4) );
    test_pass("c2023:stdc_first_trailing_one_us");
#else
    test_skip("c2023:stdc_first_trailing_one_us");
#endif
}

TEST_CASE(stdbit, first_trailing_zero_family_c2023) {
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_zero_uc((unsigned char)1), (int)(2) );
    test_pass("c2023:stdc_first_trailing_zero_uc");
#else
    test_skip("c2023:stdc_first_trailing_zero_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_zero_ui((unsigned int)1), (int)(2) );
    test_pass("c2023:stdc_first_trailing_zero_ui");
#else
    test_skip("c2023:stdc_first_trailing_zero_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_zero_ul((unsigned long)1), (int)(2) );
    test_pass("c2023:stdc_first_trailing_zero_ul");
#else
    test_skip("c2023:stdc_first_trailing_zero_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_zero_ull((unsigned long long)1), (int)(2) );
    test_pass("c2023:stdc_first_trailing_zero_ull");
#else
    test_skip("c2023:stdc_first_trailing_zero_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_first_trailing_zero_us((unsigned short)1), (int)(2) );
    test_pass("c2023:stdc_first_trailing_zero_us");
#else
    test_skip("c2023:stdc_first_trailing_zero_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_has_single_bit_uc((unsigned char)8), (int)(1) );
    test_pass("c2023:stdc_has_single_bit_uc");
#else
    test_skip("c2023:stdc_has_single_bit_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_has_single_bit_ui((unsigned int)8), (int)(1) );
    test_pass("c2023:stdc_has_single_bit_ui");
#else
    test_skip("c2023:stdc_has_single_bit_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_has_single_bit_ul((unsigned long)8), (int)(1) );
    test_pass("c2023:stdc_has_single_bit_ul");
#else
    test_skip("c2023:stdc_has_single_bit_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_has_single_bit_ull((unsigned long long)8), (int)(1) );
    test_pass("c2023:stdc_has_single_bit_ull");
#else
    test_skip("c2023:stdc_has_single_bit_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_has_single_bit_us((unsigned short)8), (int)(1) );
    test_pass("c2023:stdc_has_single_bit_us");
#else
    test_skip("c2023:stdc_has_single_bit_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_ones_uc((unsigned char)0), (int)(0) );
    test_pass("c2023:stdc_leading_ones_uc");
#else
    test_skip("c2023:stdc_leading_ones_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_ones_ui((unsigned int)0), (int)(0) );
    test_pass("c2023:stdc_leading_ones_ui");
#else
    test_skip("c2023:stdc_leading_ones_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_ones_ul((unsigned long)0), (int)(0) );
    test_pass("c2023:stdc_leading_ones_ul");
#else
    test_skip("c2023:stdc_leading_ones_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_ones_ull((unsigned long long)0), (int)(0) );
    test_pass("c2023:stdc_leading_ones_ull");
#else
    test_skip("c2023:stdc_leading_ones_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_ones_us((unsigned short)0), (int)(0) );
    test_pass("c2023:stdc_leading_ones_us");
#else
    test_skip("c2023:stdc_leading_ones_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_zeros_uc((unsigned char)1), (int)((int)(sizeof(unsigned char) * CHAR_BIT) - 1) );
    test_pass("c2023:stdc_leading_zeros_uc");
#else
    test_skip("c2023:stdc_leading_zeros_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_zeros_ui((unsigned int)1), (int)((int)(sizeof(unsigned int) * CHAR_BIT) - 1) );
    test_pass("c2023:stdc_leading_zeros_ui");
#else
    test_skip("c2023:stdc_leading_zeros_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_zeros_ul((unsigned long)1), (int)((int)(sizeof(unsigned long) * CHAR_BIT) - 1) );
    test_pass("c2023:stdc_leading_zeros_ul");
#else
    test_skip("c2023:stdc_leading_zeros_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_zeros_ull((unsigned long long)1), (int)((int)(sizeof(unsigned long long) * CHAR_BIT) - 1) );
    test_pass("c2023:stdc_leading_zeros_ull");
#else
    test_skip("c2023:stdc_leading_zeros_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_leading_zeros_us((unsigned short)1), (int)((int)(sizeof(unsigned short) * CHAR_BIT) - 1) );
    test_pass("c2023:stdc_leading_zeros_us");
#else
    test_skip("c2023:stdc_leading_zeros_us");
#endif
}

TEST_CASE(stdbit, trailing_ones_family_c2023) {
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_ones_uc((unsigned char)7), (int)(3) );
    test_pass("c2023:stdc_trailing_ones_uc");
#else
    test_skip("c2023:stdc_trailing_ones_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_ones_ui((unsigned int)7), (int)(3) );
    test_pass("c2023:stdc_trailing_ones_ui");
#else
    test_skip("c2023:stdc_trailing_ones_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_ones_ul((unsigned long)7), (int)(3) );
    test_pass("c2023:stdc_trailing_ones_ul");
#else
    test_skip("c2023:stdc_trailing_ones_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_ones_ull((unsigned long long)7), (int)(3) );
    test_pass("c2023:stdc_trailing_ones_ull");
#else
    test_skip("c2023:stdc_trailing_ones_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_ones_us((unsigned short)7), (int)(3) );
    test_pass("c2023:stdc_trailing_ones_us");
#else
    test_skip("c2023:stdc_trailing_ones_us");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_zeros_uc((unsigned char)8), (int)(3) );
    test_pass("c2023:stdc_trailing_zeros_uc");
#else
    test_skip("c2023:stdc_trailing_zeros_uc");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_zeros_ui((unsigned int)8), (int)(3) );
    test_pass("c2023:stdc_trailing_zeros_ui");
#else
    test_skip("c2023:stdc_trailing_zeros_ui");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_zeros_ul((unsigned long)8), (int)(3) );
    test_pass("c2023:stdc_trailing_zeros_ul");
#else
    test_skip("c2023:stdc_trailing_zeros_ul");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_zeros_ull((unsigned long long)8), (int)(3) );
    test_pass("c2023:stdc_trailing_zeros_ull");
#else
    test_skip("c2023:stdc_trailing_zeros_ull");
#endif
#if defined(__STDC_VERSION_STDBIT_H__) || defined(_CCW_TARGET_C)
    test_eq( (int)stdc_trailing_zeros_us((unsigned short)8), (int)(3) );
    test_pass("c2023:stdc_trailing_zeros_us");
#else
    test_skip("c2023:stdc_trailing_zeros_us");
#endif
}

TEST_SUITE(stdbit)
    TEST_ADD(version_and_endian_macros)
    TEST_ADD(generic_dispatch_uses_operand_width)
    TEST_ADD(leading_trailing)
    TEST_ADD(first_leading_trailing)
    TEST_ADD(counts_and_bit_shapes)
    TEST_ADD(per_type_spellings)
    TEST_ADD(a26_constant_folded_64bit)
    TEST_ADD(bit_ceil_family_c2023)
    TEST_ADD(count_zeros_family_c2023)
    TEST_ADD(first_trailing_zero_family_c2023)
    TEST_ADD(trailing_ones_family_c2023)
TEST_SUITE_END(stdbit)
