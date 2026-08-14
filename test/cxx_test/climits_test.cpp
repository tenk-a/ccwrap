#include "test_cxx.hpp"

#include <climits>
#include <cfloat>

TEST_CASE(climits, integer) {
    test_eq( CHAR_BIT, 8 );
    test_true( CHAR_MAX >= 127 );
    test_true( SCHAR_MIN == -128 );
    test_true( SCHAR_MAX == 127 );
    test_true( UCHAR_MAX == 255 );
    test_true( SHRT_MIN == -32768 );
    test_true( SHRT_MAX == 32767 );
    test_true( USHRT_MAX == 65535 );
    test_true( INT_MAX == 2147483647 );
    test_true( INT_MIN == -2147483647 - 1 );
    test_true( UINT_MAX == 4294967295u );
    test_true( LONG_MAX >= 2147483647L );
    test_true( LONG_MIN < 0 );
    test_true( ULONG_MAX >= 4294967295UL );
    test_true( MB_LEN_MAX >= 1 );
    test_pass("cxx03:<climits> integer-limit macros reachable (smoke)");

    test_true( LLONG_MAX > 0 );
    test_pass("cxx11:LLONG_MAX");
    test_true( LLONG_MIN < 0 );
    test_pass("cxx11:LLONG_MIN");
    test_true( ULLONG_MAX > 0ull );
    test_pass("cxx11:ULLONG_MAX");
}

TEST_CASE(climits, floating) {
    test_true( FLT_RADIX == 2 );
    test_true( FLT_MAX > 1e30f );
    test_true( FLT_MIN > 0.0f );
    test_true( FLT_EPSILON > 0.0f );
    test_true( FLT_DIG >= 6 );
    test_pass("cxx03:FLT_* (via <cfloat>)");
    test_true( DBL_MAX > 1e300 );
    test_true( DBL_MIN > 0.0 );
    test_true( DBL_EPSILON > 0.0 );
    test_true( DBL_DIG >= 10 );
    test_true( DBL_MANT_DIG >= 24 );
    test_pass("cxx03:DBL_* (via <cfloat>)");
    test_true( LDBL_MAX >= DBL_MAX );
    test_pass("cxx03:LDBL_* (via <cfloat>)");
}

#include <climits>
#include <cstdlib>
#include <limits>

TEST_CASE(climits, char_bit_and_mb_len) {

    test_ge( CHAR_BIT, 8 );
    test_eq( CHAR_BIT, 8 );
    test_eq( (int)(sizeof(char) * CHAR_BIT), STD::numeric_limits<unsigned char>::digits );
    test_pass("cxx03:CHAR_BIT (+ numeric_limits cross-check)");

    test_ge( MB_LEN_MAX, 1 );
    test_true( MB_LEN_MAX >= MB_CUR_MAX );
    test_pass("cxx03:MB_LEN_MAX");
}

TEST_CASE(climits, char_limits) {

    test_le( SCHAR_MIN, -127 );
    test_eq( SCHAR_MIN, STD::numeric_limits<signed char>::min() );
    test_lt( SCHAR_MIN, 0 );
    test_pass("cxx03:SCHAR_MIN");
    test_ge( SCHAR_MAX,  127 );
    test_eq( SCHAR_MAX, STD::numeric_limits<signed char>::max() );
    test_gt( SCHAR_MAX, 0 );
    test_pass("cxx03:SCHAR_MAX");

    test_ge( UCHAR_MAX, 255 );
    test_eq( UCHAR_MAX, STD::numeric_limits<unsigned char>::max() );
    test_eq( (int)UCHAR_MAX, (1 << CHAR_BIT) - 1 );
    test_pass("cxx03:UCHAR_MAX");

    test_eq( CHAR_MIN, STD::numeric_limits<char>::min() );
    test_pass("cxx03:CHAR_MIN");
    test_eq( CHAR_MAX, STD::numeric_limits<char>::max() );
    test_pass("cxx03:CHAR_MAX");
    test_true( (CHAR_MIN == SCHAR_MIN && CHAR_MAX == SCHAR_MAX) ||
               (CHAR_MIN == 0         && (unsigned)CHAR_MAX == (unsigned)UCHAR_MAX) );
    test_eq( CHAR_MIN < 0, STD::numeric_limits<char>::is_signed );
    test_pass("cxx03:CHAR limits vs numeric_limits property");
}

TEST_CASE(climits, short_int_long_limits) {

    test_le( SHRT_MIN, -32767 );
    test_eq( SHRT_MIN,  STD::numeric_limits<short>::min() );
    test_pass("cxx03:SHRT_MIN");
    test_ge( SHRT_MAX,  32767 );
    test_eq( SHRT_MAX,  STD::numeric_limits<short>::max() );
    test_pass("cxx03:SHRT_MAX");
    test_ge( USHRT_MAX, 65535 );
    test_eq( USHRT_MAX, STD::numeric_limits<unsigned short>::max() );
    test_eq( (unsigned)SHRT_MAX * 2u + 1u, (unsigned)USHRT_MAX );
    test_pass("cxx03:USHRT_MAX");

    test_le( INT_MIN, -32767 );
    test_eq( INT_MIN,  STD::numeric_limits<int>::min() );
    test_pass("cxx03:INT_MIN");
    test_ge( INT_MAX,  32767 );
    test_eq( INT_MAX,  STD::numeric_limits<int>::max() );
    test_pass("cxx03:INT_MAX");
    test_ge( UINT_MAX, 65535u );
    test_eq( UINT_MAX, STD::numeric_limits<unsigned int>::max() );
    test_eq( (unsigned)INT_MAX * 2u + 1u, UINT_MAX );
    test_pass("cxx03:UINT_MAX");

    test_le( LONG_MIN, -2147483647L );
    test_eq( LONG_MIN,  STD::numeric_limits<long>::min() );
    test_pass("cxx03:LONG_MIN");
    test_ge( LONG_MAX,  2147483647L );
    test_eq( LONG_MAX,  STD::numeric_limits<long>::max() );
    test_pass("cxx03:LONG_MAX");
    test_ge( ULONG_MAX, 4294967295UL );
    test_eq( ULONG_MAX, STD::numeric_limits<unsigned long>::max() );
    test_eq( (unsigned long)LONG_MAX * 2uL + 1uL, ULONG_MAX );
    test_pass("cxx03:ULONG_MAX");

    test_le( LLONG_MIN, -9223372036854775807LL );
    test_ge( LLONG_MAX,  9223372036854775807LL );
    test_ge( ULLONG_MAX, 18446744073709551615ULL );
    test_eq( LLONG_MIN,  STD::numeric_limits<long long>::min() );
    test_eq( LLONG_MAX,  STD::numeric_limits<long long>::max() );
    test_eq( ULLONG_MAX, STD::numeric_limits<unsigned long long>::max() );
    test_eq( (unsigned long long)LLONG_MAX * 2uLL + 1uLL, ULLONG_MAX );

    test_le( (long long)SCHAR_MAX, (long long)SHRT_MAX );
    test_le( (long long)SHRT_MAX,  (long long)INT_MAX );
    test_le( (long long)INT_MAX,   (long long)LONG_MAX );
    test_le( (long long)LONG_MAX,  LLONG_MAX );
    test_le( (unsigned long long)UCHAR_MAX, (unsigned long long)USHRT_MAX );
    test_le( (unsigned long long)USHRT_MAX, (unsigned long long)UINT_MAX );
    test_le( (unsigned long long)UINT_MAX,  (unsigned long long)ULONG_MAX );
    test_le( (unsigned long long)ULONG_MAX, ULLONG_MAX );

    test_eq( INT_MIN,   -INT_MAX - 1 );
    test_eq( LONG_MIN,  -LONG_MAX - 1L );
    test_eq( LLONG_MIN, -LLONG_MAX - 1LL );
    TEST_NOTE("two's complement mandated since C++20; widening ladder");
    test_pass("cxx11:LLONG limits (+ numeric_limits, widening ladder, two's complement)");
}

#ifdef BOOL_WIDTH
TEST_CASE(climits, widths_chk) {

    test_eq( BOOL_WIDTH,  1 );
    test_pass("cxx26:BOOL_WIDTH");
    test_eq( CHAR_WIDTH,  CHAR_BIT );
    test_pass("cxx26:CHAR_WIDTH");
    test_eq( SCHAR_WIDTH, CHAR_BIT );
    test_pass("cxx26:SCHAR_WIDTH");
    test_eq( UCHAR_WIDTH, CHAR_BIT );
    test_eq( UCHAR_WIDTH,  STD::numeric_limits<unsigned char>::digits );
    test_pass("cxx26:UCHAR_WIDTH");

    test_eq( SHRT_WIDTH,  (int)(sizeof(short) * CHAR_BIT) );
    test_pass("cxx26:SHRT_WIDTH");
    test_eq( USHRT_WIDTH, SHRT_WIDTH );
    test_pass("cxx26:USHRT_WIDTH");
    test_eq( INT_WIDTH,   (int)(sizeof(int) * CHAR_BIT) );
    test_eq( INT_WIDTH,    STD::numeric_limits<int>::digits + 1 );
    test_pass("cxx26:INT_WIDTH");
    test_eq( UINT_WIDTH,  INT_WIDTH );
    test_eq( UINT_WIDTH,   STD::numeric_limits<unsigned int>::digits );
    test_pass("cxx26:UINT_WIDTH");
    test_eq( LONG_WIDTH,  (int)(sizeof(long) * CHAR_BIT) );
    test_pass("cxx26:LONG_WIDTH");
    test_eq( ULONG_WIDTH, LONG_WIDTH );
    test_pass("cxx26:ULONG_WIDTH");
    test_eq( LLONG_WIDTH, (int)(sizeof(long long) * CHAR_BIT) );
    test_eq( LLONG_WIDTH,  STD::numeric_limits<long long>::digits + 1 );
    test_pass("cxx26:LLONG_WIDTH");
    test_eq( ULLONG_WIDTH, LLONG_WIDTH );
    test_eq( ULLONG_WIDTH, STD::numeric_limits<unsigned long long>::digits );
    test_pass("cxx26:ULLONG_WIDTH");

    test_le( SHRT_WIDTH, INT_WIDTH );
    test_le( INT_WIDTH,  LONG_WIDTH );
    test_le( LONG_WIDTH, LLONG_WIDTH );
    test_pass("cxx26:*_WIDTH widening ladder property");
}
#else
TEST_CASE(climits, widths_chk) {

    int __i;
    TEST_NOTE("C++23 *_WIDTH not provided by this toolchain");
    for (__i = 0; __i < 20; ++__i) TEST_SKIP1();
}
#endif


TEST_CASE(climits, width_macros_cxx26) {
#if defined(CHAR_BIT) && defined(INT_WIDTH)
    test_eq( (int)BOOL_WIDTH, 1 );
    test_pass("cxx26:BOOL_WIDTH");

    test_eq( (int)CHAR_WIDTH, (int)CHAR_BIT );
    test_pass("cxx26:CHAR_WIDTH");
    test_eq( (int)SCHAR_WIDTH, (int)CHAR_BIT );
    test_pass("cxx26:SCHAR_WIDTH");
    test_eq( (int)UCHAR_WIDTH, (int)CHAR_BIT );
    test_pass("cxx26:UCHAR_WIDTH");

    test_eq( (int)SHRT_WIDTH, (int)(sizeof(short) * CHAR_BIT) );
    test_pass("cxx26:SHRT_WIDTH");
    test_eq( (int)USHRT_WIDTH, (int)SHRT_WIDTH );
    test_pass("cxx26:USHRT_WIDTH");

    test_eq( (int)INT_WIDTH, (int)(sizeof(int) * CHAR_BIT) );
    test_pass("cxx26:INT_WIDTH");
    test_eq( (int)UINT_WIDTH, (int)INT_WIDTH );
    test_pass("cxx26:UINT_WIDTH");

    test_eq( (int)LONG_WIDTH, (int)(sizeof(long) * CHAR_BIT) );
    test_pass("cxx26:LONG_WIDTH");
    test_eq( (int)ULONG_WIDTH, (int)LONG_WIDTH );
    test_pass("cxx26:ULONG_WIDTH");

    test_eq( (int)LLONG_WIDTH, (int)(sizeof(long long) * CHAR_BIT) );
    test_pass("cxx26:LLONG_WIDTH");
    test_eq( (int)ULLONG_WIDTH, (int)LLONG_WIDTH );
    test_pass("cxx26:ULLONG_WIDTH");

    test_true( CHAR_WIDTH <= SHRT_WIDTH );
    test_true( SHRT_WIDTH <= INT_WIDTH );
    test_true( INT_WIDTH  <= LONG_WIDTH );
    test_true( LONG_WIDTH <= LLONG_WIDTH );
    test_true( BOOL_WIDTH <= CHAR_WIDTH );
    test_pass("cxx26:*_WIDTH widening ladder property");
#else
    TEST_NOTE("the *_WIDTH macros are C23/C++26; not in this library");
    test_skip("cxx26:BOOL_WIDTH");   test_skip("cxx26:CHAR_WIDTH");
    test_skip("cxx26:SCHAR_WIDTH");  test_skip("cxx26:UCHAR_WIDTH");
    test_skip("cxx26:SHRT_WIDTH");   test_skip("cxx26:USHRT_WIDTH");
    test_skip("cxx26:INT_WIDTH");    test_skip("cxx26:UINT_WIDTH");
    test_skip("cxx26:LONG_WIDTH");   test_skip("cxx26:ULONG_WIDTH");
    test_skip("cxx26:LLONG_WIDTH");  test_skip("cxx26:ULLONG_WIDTH");
    test_skip("cxx26:*_WIDTH widening ladder property");
#endif
}
