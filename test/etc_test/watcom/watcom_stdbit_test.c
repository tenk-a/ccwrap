#include <ccwrap_common.h>

#include <stdbit.h>
#include <stdio.h>

static int failures = 0;

static void ck(int cond, const char *what) {
    if (!cond) { printf("[watcom_stdbit] FAIL: %s\n", what); ++failures; }
}

int main(void) {
    const unsigned char      c8  = 0x0Fu;
    const unsigned short     s16 = 0x0F00u;
    const unsigned int       u32 = 0x0000FF00u;
    const unsigned long      l32 = 0x0000FF00uL;
    const _ccw_ullong        u64 = (_ccw_ullong)1 << 40;

#ifndef __STDC_VERSION_STDBIT_H__
    ck(0, "__STDC_VERSION_STDBIT_H__ defined");
#else
    ck(__STDC_VERSION_STDBIT_H__ >= 202311L, "__STDC_VERSION_STDBIT_H__ value");
#endif
    ck(__STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__, "__STDC_ENDIAN_NATIVE__");
    ck(__STDC_ENDIAN_LITTLE__ != __STDC_ENDIAN_BIG__, "endian macros differ");

    ck(stdc_leading_zeros(c8) == 4, "stdc_leading_zeros(unsigned char)");
    ck(stdc_leading_zeros(s16) == 4, "stdc_leading_zeros(unsigned short)");
    ck(stdc_leading_zeros(u32) == 16, "stdc_leading_zeros(unsigned int)");
    ck(stdc_leading_zeros(l32) == 16, "stdc_leading_zeros(unsigned long)");
    ck(stdc_leading_zeros(u64) == 23, "stdc_leading_zeros(unsigned long long)");
    ck(stdc_leading_zeros((unsigned char)0) == 8, "stdc_leading_zeros(0) is the width");
    ck(stdc_leading_zeros((unsigned int)0) == 32, "stdc_leading_zeros(0u) is the width");

    ck(stdc_leading_ones((unsigned char)0xF0u) == 4, "stdc_leading_ones");
    ck(stdc_leading_ones((unsigned char)0xFFu) == 8, "stdc_leading_ones(all ones)");
    ck(stdc_trailing_zeros(u32) == 8, "stdc_trailing_zeros");
    ck(stdc_trailing_zeros((unsigned int)0) == 32, "stdc_trailing_zeros(0)");
    ck(stdc_trailing_ones(c8) == 4, "stdc_trailing_ones");
    ck(stdc_trailing_ones((unsigned char)0) == 0, "stdc_trailing_ones(0)");

    ck(stdc_count_ones(u32) == 8, "stdc_count_ones");
    ck(stdc_count_ones((unsigned char)0xFFu) == 8, "stdc_count_ones(all ones)");
    ck(stdc_count_zeros(u32) == 24, "stdc_count_zeros");
    ck(stdc_count_zeros(c8) == 4, "stdc_count_zeros(unsigned char)");

    ck(stdc_first_leading_one(u32) == 17, "stdc_first_leading_one");
    ck(stdc_first_leading_one((unsigned int)0) == 0, "stdc_first_leading_one(0)");
    ck(stdc_first_leading_zero((unsigned char)0xFFu) == 0, "stdc_first_leading_zero(all ones)");
    ck(stdc_first_leading_zero((unsigned char)0x7Fu) == 1, "stdc_first_leading_zero");
    ck(stdc_first_trailing_one(u32) == 9, "stdc_first_trailing_one");
    ck(stdc_first_trailing_one((unsigned int)0) == 0, "stdc_first_trailing_one(0)");
    ck(stdc_first_trailing_zero(c8) == 5, "stdc_first_trailing_zero");
    ck(stdc_first_trailing_zero((unsigned char)0xFFu) == 0, "stdc_first_trailing_zero(all ones)");

    ck(stdc_has_single_bit(u64), "stdc_has_single_bit(1<<40)");
    ck(!stdc_has_single_bit(u32), "stdc_has_single_bit(0xFF00) is false");
    ck(!stdc_has_single_bit((unsigned int)0), "stdc_has_single_bit(0) is false");
    ck(stdc_has_single_bit((unsigned int)1), "stdc_has_single_bit(1)");

    ck(stdc_bit_width(u32) == 16, "stdc_bit_width");
    ck(stdc_bit_width((unsigned int)0) == 0, "stdc_bit_width(0)");
    ck(stdc_bit_width((unsigned int)1) == 1, "stdc_bit_width(1)");
    ck(stdc_bit_floor((unsigned int)5) == 4u, "stdc_bit_floor");
    ck(stdc_bit_floor((unsigned int)0) == 0u, "stdc_bit_floor(0)");
    ck(stdc_bit_ceil((unsigned int)5) == 8u, "stdc_bit_ceil");
    ck(stdc_bit_ceil((unsigned int)0) == 1u, "stdc_bit_ceil(0)");
    ck(stdc_bit_ceil((unsigned int)1) == 1u, "stdc_bit_ceil(1)");

    ck(stdc_leading_zeros_uc(c8) == 4, "stdc_leading_zeros_uc");
    ck(stdc_leading_zeros_us(s16) == 4, "stdc_leading_zeros_us");
    ck(stdc_leading_zeros_ui(u32) == 16, "stdc_leading_zeros_ui");
    ck(stdc_leading_zeros_ul(l32) == 16, "stdc_leading_zeros_ul");
    ck(stdc_leading_zeros_ull(u64) == 23, "stdc_leading_zeros_ull");
    ck(stdc_leading_ones_uc((unsigned char)0xF0u) == 4, "stdc_leading_ones_uc");
    ck(stdc_trailing_zeros_ui(u32) == 8, "stdc_trailing_zeros_ui");
    ck(stdc_trailing_ones_uc(c8) == 4, "stdc_trailing_ones_uc");
    ck(stdc_first_leading_zero_uc((unsigned char)0x7Fu) == 1, "stdc_first_leading_zero_uc");
    ck(stdc_first_leading_one_ui(u32) == 17, "stdc_first_leading_one_ui");
    ck(stdc_first_trailing_zero_uc(c8) == 5, "stdc_first_trailing_zero_uc");
    ck(stdc_first_trailing_one_ui(u32) == 9, "stdc_first_trailing_one_ui");
    ck(stdc_count_ones_ui(u32) == 8, "stdc_count_ones_ui");
    ck(stdc_count_zeros_uc(c8) == 4, "stdc_count_zeros_uc");
    ck(stdc_has_single_bit_ull(u64), "stdc_has_single_bit_ull");
    ck(!stdc_has_single_bit_ui(u32), "stdc_has_single_bit_ui is false");
    ck(stdc_bit_width_ui(u32) == 16, "stdc_bit_width_ui");
    ck(stdc_bit_floor_ui(5u) == 4u, "stdc_bit_floor_ui");
    ck(stdc_bit_ceil_ui(5u) == 8u, "stdc_bit_ceil_ui");
    ck(stdc_bit_floor_ull(u64 + 1) == u64, "stdc_bit_floor_ull");
    ck(stdc_bit_ceil_uc((unsigned char)5) == 8u, "stdc_bit_ceil_uc");

    ck(stdc_count_ones(u32) == stdc_count_ones_ui(u32), "generic == _ui");
    ck(stdc_count_ones(c8) == stdc_count_ones_uc(c8), "generic == _uc");
    ck(stdc_count_ones(u64) == stdc_count_ones_ull(u64), "generic == _ull");

    ck(stdc_leading_zeros((_ccw_ullong)1 << 40) == 23, "A26: clz(const 1<<40)");
    ck(stdc_leading_zeros((_ccw_ullong)1 << 63) == 0,  "A26: clz(const 1<<63)");
    ck(stdc_leading_zeros((_ccw_ullong)1) == 63,       "A26: clz(const 1)");
    ck(stdc_leading_zeros((_ccw_ullong)0) == 64,       "A26: clz(const 0)");
    ck(stdc_trailing_zeros((_ccw_ullong)1 << 40) == 40, "A26: ctz(const 1<<40)");
    ck(stdc_trailing_zeros((_ccw_ullong)0) == 64,       "A26: ctz(const 0)");
    ck(stdc_bit_width((_ccw_ullong)1 << 40) == 41,      "A26: bit_width(const)");
    ck(stdc_count_ones((_ccw_ullong)0xF0F0F0F0F0F0F0F0ULL) == 32, "A26: popcount(const)");
    ck(stdc_has_single_bit((_ccw_ullong)1 << 40),       "A26: has_single_bit(const 2^40)");
    ck(!stdc_has_single_bit((_ccw_ullong)3),            "A26: has_single_bit(const 3)");
    ck(stdc_bit_floor((_ccw_ullong)5) == 4, "A26: bit_floor(const 5)");
    ck(stdc_bit_ceil((_ccw_ullong)5) == 8,  "A26: bit_ceil(const 5)");

    if (failures == 0)
        printf("[watcom_stdbit] all checks passed\n");
    return failures;
}
