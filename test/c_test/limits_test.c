#include "c_test.h"

#include <limits.h>
#include <stdlib.h>

TEST_CASE(limits, char_bit_and_mb_len) {
    test_ge( CHAR_BIT, 8 );
    test_eq( sizeof(char), (size_t)1 );
    test_pass("c1990:CHAR_BIT");
    test_ge( MB_LEN_MAX, 1 );
    test_true( MB_LEN_MAX >= (int)MB_CUR_MAX );
    test_pass("c1990:MB_LEN_MAX");
}

TEST_CASE(limits, char_limits) {
    test_le( SCHAR_MIN, -127 ); test_true( SCHAR_MIN < 0 );
    test_true( SCHAR_MIN == -SCHAR_MAX || SCHAR_MIN == -SCHAR_MAX - 1 );
    test_pass("c1990:SCHAR_MIN");
    test_ge( SCHAR_MAX, 127 ); test_true( SCHAR_MAX > 0 );
    test_pass("c1990:SCHAR_MAX");
    test_ge( (unsigned long)UCHAR_MAX, 255UL );
    test_ge( (unsigned long)UCHAR_MAX,
             (unsigned long)SCHAR_MAX * 2UL + 1UL );
    test_pass("c1990:UCHAR_MAX");

    test_true( (CHAR_MIN == SCHAR_MIN && CHAR_MAX == SCHAR_MAX) ||
               (CHAR_MIN == 0 && (unsigned)CHAR_MAX == (unsigned)UCHAR_MAX) );
    test_true( CHAR_MIN <= CHAR_MAX );
    test_pass("c1990:CHAR_MIN");
    test_pass("c1990:CHAR_MAX");
}

TEST_CASE(limits, short_int_long) {
    test_le( SHRT_MIN, -32767 ); test_le( (long)SCHAR_MAX, (long)SHRT_MAX );
    test_true( SHRT_MIN == -SHRT_MAX || SHRT_MIN == -SHRT_MAX - 1 );
    test_pass("c1990:SHRT_MIN");
    test_ge( SHRT_MAX, 32767 );
    test_pass("c1990:SHRT_MAX");
    test_ge( (unsigned long)USHRT_MAX, 65535UL );
    test_ge( (unsigned long)USHRT_MAX, (unsigned long)SHRT_MAX * 2UL + 1UL );
    test_true( (unsigned long)UCHAR_MAX <= (unsigned long)USHRT_MAX );
    test_pass("c1990:USHRT_MAX");

    test_eq( INT_MIN, -2147483647 - 1 ); test_eq( INT_MIN, -INT_MAX - 1 );
    test_pass("c1990:INT_MIN");
    test_eq( INT_MAX, 2147483647 ); test_le( (long)SHRT_MAX, (long)INT_MAX );
    test_pass("c1990:INT_MAX");
    test_true( UINT_MAX == 4294967295u );
    test_true( (unsigned)INT_MAX * 2u + 1u == UINT_MAX );
    test_true( (unsigned long)USHRT_MAX <= (unsigned long)UINT_MAX );
    test_pass("c1990:UINT_MAX");

    test_le( LONG_MIN, -2147483647L );
    test_true( LONG_MIN == -LONG_MAX || LONG_MIN == -LONG_MAX - 1L );
    test_pass("c1990:LONG_MIN");
    test_ge( LONG_MAX, 2147483647L ); test_le( (long)INT_MAX, LONG_MAX );
    test_pass("c1990:LONG_MAX");
    test_ge( ULONG_MAX, 4294967295UL ); test_true( (unsigned long)UINT_MAX <= ULONG_MAX );
    test_pass("c1990:ULONG_MAX");
}

TEST_CASE(limits, long_long) {
#ifdef LLONG_MAX
    test_true( LLONG_MIN <= -9223372036854775807LL );
    test_true( LLONG_MIN == -LLONG_MAX || LLONG_MIN == -LLONG_MAX - 1LL );
    test_pass("c1999:LLONG_MIN");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("LLONG_MIN not defined");
    test_skip("c1999:LLONG_MIN");
#endif
#ifdef LLONG_MAX
    test_true( LLONG_MAX >= 9223372036854775807LL );
    test_true( (long long)LONG_MAX <= LLONG_MAX );
    test_pass("c1999:LLONG_MAX");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("LLONG_MAX not defined");
    test_skip("c1999:LLONG_MAX");
#endif
#ifdef ULLONG_MAX
    test_true( ULLONG_MAX >= 18446744073709551615ULL );
    test_pass("c1999:ULLONG_MAX");
#else
    TEST_SKIP1(); TEST_NOTE("ULLONG_MAX not defined");
    test_skip("c1999:ULLONG_MAX");
#endif
}

TEST_CASE(limits, widths) {
#ifdef CHAR_WIDTH
    test_eq( BOOL_WIDTH,   1 );                                    test_pass("c2023:BOOL_WIDTH");
    test_eq( CHAR_WIDTH,   CHAR_BIT );                             test_pass("c2023:CHAR_WIDTH");
    test_eq( SCHAR_WIDTH,  CHAR_BIT );                             test_pass("c2023:SCHAR_WIDTH");
    test_eq( UCHAR_WIDTH,  CHAR_BIT );                             test_pass("c2023:UCHAR_WIDTH");
    test_le( SHRT_WIDTH,   (int)(sizeof(short) * CHAR_BIT) );      test_pass("c2023:SHRT_WIDTH");
    test_eq( USHRT_WIDTH,  SHRT_WIDTH );                           test_pass("c2023:USHRT_WIDTH");
    test_le( INT_WIDTH,    (int)(sizeof(int) * CHAR_BIT) );        test_pass("c2023:INT_WIDTH");
    test_eq( UINT_WIDTH,   INT_WIDTH );                            test_pass("c2023:UINT_WIDTH");
    test_le( LONG_WIDTH,   (int)(sizeof(long) * CHAR_BIT) );       test_pass("c2023:LONG_WIDTH");
    test_eq( ULONG_WIDTH,  LONG_WIDTH );                           test_pass("c2023:ULONG_WIDTH");
    test_le( LLONG_WIDTH,  (int)(sizeof(long long) * CHAR_BIT) );  test_pass("c2023:LLONG_WIDTH");
    test_eq( ULLONG_WIDTH, LLONG_WIDTH );                          test_pass("c2023:ULLONG_WIDTH");

    test_le( CHAR_WIDTH, SHRT_WIDTH ); test_le( SHRT_WIDTH, INT_WIDTH );
    test_le( INT_WIDTH,  LONG_WIDTH ); test_le( LONG_WIDTH, LLONG_WIDTH );
    test_pass("c2023:*_WIDTH widening ladder property");
#else
    TEST_NOTE("C23 *_WIDTH not defined");
    TEST_SKIP1(); test_skip("c2023:BOOL_WIDTH");
    TEST_SKIP1(); test_skip("c2023:CHAR_WIDTH");
    TEST_SKIP1(); test_skip("c2023:SCHAR_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UCHAR_WIDTH");
    TEST_SKIP1(); test_skip("c2023:SHRT_WIDTH");
    TEST_SKIP1(); test_skip("c2023:USHRT_WIDTH");
    TEST_SKIP1(); test_skip("c2023:INT_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINT_WIDTH");
    TEST_SKIP1(); test_skip("c2023:LONG_WIDTH");
    TEST_SKIP1(); test_skip("c2023:ULONG_WIDTH");
    TEST_SKIP1(); test_skip("c2023:LLONG_WIDTH");
    TEST_SKIP1(); test_skip("c2023:ULLONG_WIDTH");
    TEST_SKIP_N(4); test_skip("c2023:*_WIDTH widening ladder property");
#endif
}


TEST_CASE(limits, c23_macros) {
#if defined(__STDC_VERSION_LIMITS_H__)
    test_true( __STDC_VERSION_LIMITS_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_LIMITS_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_LIMITS_H__");
#endif

#if defined(BOOL_MAX)
    test_eq( (int)BOOL_MAX, 1 );
#if defined(BOOL_WIDTH)
    test_eq( (int)BOOL_WIDTH, 1 );
#endif
    test_pass("c2023:BOOL_MAX");
#else
    TEST_NOTE("BOOL_MAX is C23; this library does not define it");
    test_skip("c2023:BOOL_MAX");
#endif

#if defined(BITINT_MAXWIDTH)
    test_true( BITINT_MAXWIDTH >= 64 );
    test_pass("c2023:BITINT_MAXWIDTH");
#else
    TEST_NOTE("_BitInt is C23; this compiler does not support it");
    test_skip("c2023:BITINT_MAXWIDTH");
#endif
}

TEST_SUITE(limits)
    TEST_ADD(char_bit_and_mb_len)
    TEST_ADD(char_limits)
    TEST_ADD(short_int_long)
    TEST_ADD(long_long)
    TEST_ADD(widths)
    TEST_ADD(c23_macros)
TEST_SUITE_END(limits)
