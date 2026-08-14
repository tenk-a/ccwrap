#include "c_test.h"

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#if defined(INT8_WIDTH) || defined(INTPTR_WIDTH)
static int tst_uint_width(uintmax_t value) {
    int width = 0;
    do {
        ++width;
        value >>= 1;
    } while (value != 0);
    return width;
}
#endif

TEST_CASE(stdint, exact_sizes) {
    test_eq( sizeof(int8_t)   * CHAR_BIT, (size_t)8 );   test_pass("c1999:opt:int8_t");
    test_eq( sizeof(uint8_t)  * CHAR_BIT, (size_t)8 );   test_pass("c1999:opt:uint8_t");
    test_eq( sizeof(int16_t)  * CHAR_BIT, (size_t)16 );  test_pass("c1999:opt:int16_t");
    test_eq( sizeof(uint16_t) * CHAR_BIT, (size_t)16 );  test_pass("c1999:opt:uint16_t");
    test_eq( sizeof(int32_t)  * CHAR_BIT, (size_t)32 );  test_pass("c1999:opt:int32_t");
    test_eq( sizeof(uint32_t) * CHAR_BIT, (size_t)32 );  test_pass("c1999:opt:uint32_t");
    test_eq( sizeof(int64_t)  * CHAR_BIT, (size_t)64 );  test_pass("c1999:opt:int64_t");
    test_eq( sizeof(uint64_t) * CHAR_BIT, (size_t)64 );  test_pass("c1999:opt:uint64_t");
}

TEST_CASE(stdint, least_sizes) {
    test_true( sizeof(int_least8_t)   * CHAR_BIT >= 8u );  test_pass("c1999:int_least8_t");
    test_true( sizeof(uint_least8_t)  * CHAR_BIT >= 8u );  test_pass("c1999:uint_least8_t");
    test_true( sizeof(int_least16_t)  * CHAR_BIT >= 16u ); test_pass("c1999:int_least16_t");
    test_true( sizeof(uint_least16_t) * CHAR_BIT >= 16u ); test_pass("c1999:uint_least16_t");
    test_true( sizeof(int_least32_t)  * CHAR_BIT >= 32u ); test_pass("c1999:int_least32_t");
    test_true( sizeof(uint_least32_t) * CHAR_BIT >= 32u ); test_pass("c1999:uint_least32_t");
    test_true( sizeof(int_least64_t)  * CHAR_BIT >= 64u ); test_pass("c1999:int_least64_t");
    test_true( sizeof(uint_least64_t) * CHAR_BIT >= 64u ); test_pass("c1999:uint_least64_t");
}

TEST_CASE(stdint, fast_sizes) {
    test_true( sizeof(int_fast8_t)   * CHAR_BIT >= 8u );  test_pass("c1999:int_fast8_t");
    test_true( sizeof(uint_fast8_t)  * CHAR_BIT >= 8u );  test_pass("c1999:uint_fast8_t");
    test_true( sizeof(int_fast16_t)  * CHAR_BIT >= 16u ); test_pass("c1999:int_fast16_t");
    test_true( sizeof(uint_fast16_t) * CHAR_BIT >= 16u ); test_pass("c1999:uint_fast16_t");
    test_true( sizeof(int_fast32_t)  * CHAR_BIT >= 32u ); test_pass("c1999:int_fast32_t");
    test_true( sizeof(uint_fast32_t) * CHAR_BIT >= 32u ); test_pass("c1999:uint_fast32_t");
    test_true( sizeof(int_fast64_t)  * CHAR_BIT >= 64u ); test_pass("c1999:int_fast64_t");
    test_true( sizeof(uint_fast64_t) * CHAR_BIT >= 64u ); test_pass("c1999:uint_fast64_t");
}

TEST_CASE(stdint, ptr_max_sizes) {
    test_true( sizeof(intptr_t)  >= sizeof(void*) );  test_pass("c1999:opt:intptr_t");
    test_true( sizeof(uintptr_t) >= sizeof(void*) );  test_pass("c1999:opt:uintptr_t");
    test_true( sizeof(intmax_t)  * CHAR_BIT >= 64u ); test_pass("c1999:intmax_t");
    test_true( sizeof(uintmax_t) * CHAR_BIT >= 64u ); test_pass("c1999:uintmax_t");
}

TEST_CASE(stdint, exact_limit_values) {
    test_true( INT8_MIN == -128 );                 test_pass("c1999:opt:INT8_MIN");
    test_true( INT8_MAX == 127 );                  test_pass("c1999:opt:INT8_MAX");
    test_eq( (int)UINT8_MAX, 255 );                test_pass("c1999:opt:UINT8_MAX");
    test_true( INT16_MIN == -32768 );              test_pass("c1999:opt:INT16_MIN");
    test_true( INT16_MAX == 32767 );               test_pass("c1999:opt:INT16_MAX");
    test_eq( (int)UINT16_MAX, 65535 );             test_pass("c1999:opt:UINT16_MAX");
    test_true( INT32_MIN == -2147483647 - 1 );     test_pass("c1999:opt:INT32_MIN");
    test_true( INT32_MAX == 2147483647 );          test_pass("c1999:opt:INT32_MAX");
    test_true( UINT32_MAX == 4294967295u );        test_pass("c1999:opt:UINT32_MAX");
    test_true( INT64_MIN + INT64_MAX == -1 );      test_pass("c1999:opt:INT64_MIN");
    test_true( (long long)INT64_MAX == 9223372036854775807LL );  test_pass("c1999:opt:INT64_MAX");
    test_true( UINT64_MAX == 18446744073709551615ULL );          test_pass("c1999:opt:UINT64_MAX");
}

TEST_CASE(stdint, least_magnitudes) {
    test_le( (long long)INT_LEAST8_MIN, -127LL );                    test_pass("c1999:INT_LEAST8_MIN");
    test_ge( (long long)INT_LEAST8_MAX,  127LL );                    test_pass("c1999:INT_LEAST8_MAX");
    test_ge( (unsigned long long)UINT_LEAST8_MAX, 255ULL );          test_pass("c1999:UINT_LEAST8_MAX");
    test_le( (long long)INT_LEAST16_MIN, -32767LL );                 test_pass("c1999:INT_LEAST16_MIN");
    test_ge( (long long)INT_LEAST16_MAX,  32767LL );                 test_pass("c1999:INT_LEAST16_MAX");
    test_ge( (unsigned long long)UINT_LEAST16_MAX, 65535ULL );       test_pass("c1999:UINT_LEAST16_MAX");
    test_le( (long long)INT_LEAST32_MIN, -2147483647LL );            test_pass("c1999:INT_LEAST32_MIN");
    test_ge( (long long)INT_LEAST32_MAX,  2147483647LL );            test_pass("c1999:INT_LEAST32_MAX");
    test_ge( (unsigned long long)UINT_LEAST32_MAX, 4294967295ULL );  test_pass("c1999:UINT_LEAST32_MAX");
    test_le( (long long)INT_LEAST64_MIN, -9223372036854775807LL );   test_pass("c1999:INT_LEAST64_MIN");
    test_ge( (long long)INT_LEAST64_MAX,  9223372036854775807LL );   test_pass("c1999:INT_LEAST64_MAX");
    test_ge( (unsigned long long)UINT_LEAST64_MAX, 18446744073709551615ULL );  test_pass("c1999:UINT_LEAST64_MAX");
}

TEST_CASE(stdint, fast_magnitudes) {
    test_le( (long long)INT_FAST8_MIN, -127LL );                    test_pass("c1999:INT_FAST8_MIN");
    test_ge( (long long)INT_FAST8_MAX,  127LL );                    test_pass("c1999:INT_FAST8_MAX");
    test_ge( (unsigned long long)UINT_FAST8_MAX, 255ULL );          test_pass("c1999:UINT_FAST8_MAX");
    test_le( (long long)INT_FAST16_MIN, -32767LL );                 test_pass("c1999:INT_FAST16_MIN");
    test_ge( (long long)INT_FAST16_MAX,  32767LL );                 test_pass("c1999:INT_FAST16_MAX");
    test_ge( (unsigned long long)UINT_FAST16_MAX, 65535ULL );       test_pass("c1999:UINT_FAST16_MAX");
    test_le( (long long)INT_FAST32_MIN, -2147483647LL );            test_pass("c1999:INT_FAST32_MIN");
    test_ge( (long long)INT_FAST32_MAX,  2147483647LL );            test_pass("c1999:INT_FAST32_MAX");
    test_ge( (unsigned long long)UINT_FAST32_MAX, 4294967295ULL );  test_pass("c1999:UINT_FAST32_MAX");
    test_le( (long long)INT_FAST64_MIN, -9223372036854775807LL );   test_pass("c1999:INT_FAST64_MIN");
    test_ge( (long long)INT_FAST64_MAX,  9223372036854775807LL );   test_pass("c1999:INT_FAST64_MAX");
    test_ge( (unsigned long long)UINT_FAST64_MAX, 18446744073709551615ULL );  test_pass("c1999:UINT_FAST64_MAX");
}

TEST_CASE(stdint, max_and_ptr_limits) {
    test_le( (long long)INTMAX_MIN, -9223372036854775807LL );          test_pass("c1999:INTMAX_MIN");
    test_ge( (long long)INTMAX_MAX,  9223372036854775807LL );          test_pass("c1999:INTMAX_MAX");
    test_ge( (unsigned long long)UINTMAX_MAX, 18446744073709551615ULL );  test_pass("c1999:UINTMAX_MAX");
    test_le( (long long)INTPTR_MIN, 0LL );                             test_pass("c1999:opt:INTPTR_MIN");
    test_gt( (long long)INTPTR_MAX, 0LL );                             test_pass("c1999:opt:INTPTR_MAX");
    test_ge( (unsigned long long)UINTPTR_MAX, (unsigned long long)INTPTR_MAX );  test_pass("c1999:opt:UINTPTR_MAX");
}

TEST_CASE(stdint, other_limits) {
    test_le( (long long)PTRDIFF_MIN, -65535LL );      test_pass("c1999:PTRDIFF_MIN");
    test_ge( (long long)PTRDIFF_MAX,  65535LL );      test_pass("c1999:PTRDIFF_MAX");
    {
        size_t sm = (size_t)SIZE_MAX;
        size_t wrapped = sm + 1;
        test_ge( (unsigned long long)SIZE_MAX, 65535ULL );
        test_eq( (unsigned long long)wrapped, 0ULL );
    }
    test_pass("c1999:SIZE_MAX");
}

TEST_CASE(stdint, sig_atomic_wchar_wint_limits) {
    test_lt( (long long)SIG_ATOMIC_MIN, (long long)SIG_ATOMIC_MAX );  test_pass("c1999:SIG_ATOMIC_MIN");
    test_ge( (long long)SIG_ATOMIC_MAX, 127LL );                      test_pass("c1999:SIG_ATOMIC_MAX");
    test_lt( (long long)WCHAR_MIN, (long long)WCHAR_MAX );            test_pass("c1999:WCHAR_MIN");
    test_ge( (long long)WCHAR_MAX, 255LL );                           test_pass("c1999:WCHAR_MAX");
    test_lt( (long long)WINT_MIN, (long long)WINT_MAX );              test_pass("c1999:WINT_MIN");
    test_ge( (long long)WINT_MAX, 255LL );                            test_pass("c1999:WINT_MAX");
}

TEST_CASE(stdint, constant_macros) {
    static const int a[INT32_C(3)] = { 1, 2, 3 };

    test_eq( (long long)INT8_C(127), 127LL );                    test_pass("c1999:opt:INT8_C");
    test_eq( (unsigned long long)UINT8_C(255), 255ULL );
    test_eq( a[UINT8_C(2)], 3 );                                 test_pass("c1999:opt:UINT8_C");
    test_eq( (long long)INT16_C(-32768), -32768LL );             test_pass("c1999:opt:INT16_C");
    test_eq( (unsigned long long)UINT16_C(65535), 65535ULL );    test_pass("c1999:opt:UINT16_C");
    test_eq( (long long)INT32_C(2147483647), 2147483647LL );     test_pass("c1999:opt:INT32_C");
    test_true( UINT32_C(4294967295) == 4294967295u );            test_pass("c1999:opt:UINT32_C");
    test_eq( (long long)INT64_C(9223372036854775807), 9223372036854775807LL );
    test_ge( (int)sizeof(INT64_C(1)), 8 );                       test_pass("c1999:opt:INT64_C");
    test_true( UINT64_C(0xFFFFFFFFFFFFFFFF) == 18446744073709551615ULL );
    test_ge( (int)sizeof(UINT64_C(1)), 8 );                      test_pass("c1999:opt:UINT64_C");
    test_eq( (long long)INTMAX_C(9223372036854775807), (long long)INTMAX_MAX );  test_pass("c1999:INTMAX_C");
    test_true( UINTMAX_C(18446744073709551615) == (uintmax_t)UINTMAX_MAX );      test_pass("c1999:UINTMAX_C");
}

TEST_CASE(stdint, pointer_roundtrip) {
    int   x = 12345;
    void* p = &x;
    uintptr_t up;
    intptr_t  ip;

    up = (uintptr_t)p;
    test_true( (void*)up == p );
    test_eq( *(int*)(void*)up, 12345 );
    test_le( (unsigned long long)up, (unsigned long long)UINTPTR_MAX );

    ip = (intptr_t)p;
    test_true( (void*)ip == p );
    test_pass("c1999:(u)intptr_t pointer round-trip property");
}

TEST_CASE(stdint, exact_width_c23) {
#ifdef INT8_WIDTH
    test_eq( (int)INT8_WIDTH,   tst_uint_width((uintmax_t)INT8_MAX) + 1 );   test_pass("c2023:opt:INT8_WIDTH");
    test_eq( (int)UINT8_WIDTH,  tst_uint_width((uintmax_t)UINT8_MAX) );      test_pass("c2023:opt:UINT8_WIDTH");
    test_eq( (int)INT16_WIDTH,  tst_uint_width((uintmax_t)INT16_MAX) + 1 );  test_pass("c2023:opt:INT16_WIDTH");
    test_eq( (int)UINT16_WIDTH, tst_uint_width((uintmax_t)UINT16_MAX) );     test_pass("c2023:opt:UINT16_WIDTH");
    test_eq( (int)INT32_WIDTH,  tst_uint_width((uintmax_t)INT32_MAX) + 1 );  test_pass("c2023:opt:INT32_WIDTH");
    test_eq( (int)UINT32_WIDTH, tst_uint_width((uintmax_t)UINT32_MAX) );     test_pass("c2023:opt:UINT32_WIDTH");
    test_eq( (int)INT64_WIDTH,  tst_uint_width((uintmax_t)INT64_MAX) + 1 );  test_pass("c2023:opt:INT64_WIDTH");
    test_eq( (int)UINT64_WIDTH, tst_uint_width((uintmax_t)UINT64_MAX) );     test_pass("c2023:opt:UINT64_WIDTH");
#else
    TEST_SKIP1(); TEST_NOTE("C23 INT8_WIDTH not defined");   test_skip("c2023:opt:INT8_WIDTH");
    TEST_SKIP1(); test_skip("c2023:opt:UINT8_WIDTH");
    TEST_SKIP1(); test_skip("c2023:opt:INT16_WIDTH");
    TEST_SKIP1(); test_skip("c2023:opt:UINT16_WIDTH");
    TEST_SKIP1(); test_skip("c2023:opt:INT32_WIDTH");
    TEST_SKIP1(); test_skip("c2023:opt:UINT32_WIDTH");
    TEST_SKIP1(); test_skip("c2023:opt:INT64_WIDTH");
    TEST_SKIP1(); test_skip("c2023:opt:UINT64_WIDTH");
#endif
}

TEST_CASE(stdint, least_fast_width_c23) {
#ifdef INT_LEAST8_WIDTH
    test_ge( (int)INT_LEAST8_WIDTH,  8 );   test_pass("c2023:INT_LEAST8_WIDTH");
    test_ge( (int)UINT_LEAST8_WIDTH, 8 );   test_pass("c2023:UINT_LEAST8_WIDTH");
    test_ge( (int)INT_LEAST16_WIDTH, 16 );  test_pass("c2023:INT_LEAST16_WIDTH");
    test_ge( (int)UINT_LEAST16_WIDTH,16 );  test_pass("c2023:UINT_LEAST16_WIDTH");
    test_ge( (int)INT_LEAST32_WIDTH, 32 );  test_pass("c2023:INT_LEAST32_WIDTH");
    test_ge( (int)UINT_LEAST32_WIDTH,32 );  test_pass("c2023:UINT_LEAST32_WIDTH");
    test_ge( (int)INT_LEAST64_WIDTH, 64 );  test_pass("c2023:INT_LEAST64_WIDTH");
    test_ge( (int)UINT_LEAST64_WIDTH,64 );  test_pass("c2023:UINT_LEAST64_WIDTH");
    test_ge( (int)INT_FAST8_WIDTH,   8 );   test_pass("c2023:INT_FAST8_WIDTH");
    test_ge( (int)UINT_FAST8_WIDTH,  8 );   test_pass("c2023:UINT_FAST8_WIDTH");
    test_ge( (int)INT_FAST16_WIDTH,  16 );  test_pass("c2023:INT_FAST16_WIDTH");
    test_ge( (int)UINT_FAST16_WIDTH, 16 );  test_pass("c2023:UINT_FAST16_WIDTH");
    test_ge( (int)INT_FAST32_WIDTH,  32 );  test_pass("c2023:INT_FAST32_WIDTH");
    test_ge( (int)UINT_FAST32_WIDTH, 32 );  test_pass("c2023:UINT_FAST32_WIDTH");
    test_ge( (int)INT_FAST64_WIDTH,  64 );  test_pass("c2023:INT_FAST64_WIDTH");
    test_ge( (int)UINT_FAST64_WIDTH, 64 );  test_pass("c2023:UINT_FAST64_WIDTH");
#else
    TEST_SKIP1(); TEST_NOTE("C23 least/fast *_WIDTH not defined"); test_skip("c2023:INT_LEAST8_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINT_LEAST8_WIDTH");
    TEST_SKIP1(); test_skip("c2023:INT_LEAST16_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINT_LEAST16_WIDTH");
    TEST_SKIP1(); test_skip("c2023:INT_LEAST32_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINT_LEAST32_WIDTH");
    TEST_SKIP1(); test_skip("c2023:INT_LEAST64_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINT_LEAST64_WIDTH");
    TEST_SKIP1(); test_skip("c2023:INT_FAST8_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINT_FAST8_WIDTH");
    TEST_SKIP1(); test_skip("c2023:INT_FAST16_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINT_FAST16_WIDTH");
    TEST_SKIP1(); test_skip("c2023:INT_FAST32_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINT_FAST32_WIDTH");
    TEST_SKIP1(); test_skip("c2023:INT_FAST64_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINT_FAST64_WIDTH");
#endif
}

TEST_CASE(stdint, ptr_max_other_width_c23) {
#ifdef INTPTR_WIDTH
    test_eq( (int)INTPTR_WIDTH,  tst_uint_width((uintmax_t)INTPTR_MAX) + 1 );  test_pass("c2023:opt:INTPTR_WIDTH");
    test_eq( (int)UINTPTR_WIDTH, tst_uint_width((uintmax_t)UINTPTR_MAX) );     test_pass("c2023:opt:UINTPTR_WIDTH");
    test_eq( (int)INTMAX_WIDTH,  tst_uint_width((uintmax_t)INTMAX_MAX) + 1 );  test_pass("c2023:INTMAX_WIDTH");
    test_eq( (int)UINTMAX_WIDTH, tst_uint_width((uintmax_t)UINTMAX_MAX) );     test_pass("c2023:UINTMAX_WIDTH");
    test_eq( (int)PTRDIFF_WIDTH, tst_uint_width((uintmax_t)PTRDIFF_MAX) + 1 ); test_pass("c2023:PTRDIFF_WIDTH");
    test_eq( (int)SIZE_WIDTH,    tst_uint_width((uintmax_t)SIZE_MAX) );        test_pass("c2023:SIZE_WIDTH");
    test_eq( (int)SIG_ATOMIC_WIDTH, tst_uint_width((uintmax_t)SIG_ATOMIC_MAX) + (SIG_ATOMIC_MIN < 0) ); test_pass("c2023:SIG_ATOMIC_WIDTH");
    test_eq( (int)WCHAR_WIDTH,      tst_uint_width((uintmax_t)WCHAR_MAX) + (WCHAR_MIN < 0) );           test_pass("c2023:WCHAR_WIDTH");
    test_eq( (int)WINT_WIDTH,       tst_uint_width((uintmax_t)WINT_MAX) + (WINT_MIN < 0) );             test_pass("c2023:WINT_WIDTH");
#else
    TEST_SKIP1(); TEST_NOTE("C23 INTPTR/... _WIDTH not defined"); test_skip("c2023:opt:INTPTR_WIDTH");
    TEST_SKIP1(); test_skip("c2023:opt:UINTPTR_WIDTH");
    TEST_SKIP1(); test_skip("c2023:INTMAX_WIDTH");
    TEST_SKIP1(); test_skip("c2023:UINTMAX_WIDTH");
    TEST_SKIP1(); test_skip("c2023:PTRDIFF_WIDTH");
    TEST_SKIP1(); test_skip("c2023:SIZE_WIDTH");
    TEST_SKIP1(); test_skip("c2023:SIG_ATOMIC_WIDTH");
    TEST_SKIP1(); test_skip("c2023:WCHAR_WIDTH");
    TEST_SKIP1(); test_skip("c2023:WINT_WIDTH");
#endif
}

TEST_CASE(stdint, stdc_version_c23) {
#ifdef __STDC_VERSION_STDINT_H__
    test_ge( __STDC_VERSION_STDINT_H__, 202311L );
    test_pass("c2023:__STDC_VERSION_STDINT_H__");
#else
    TEST_SKIP1(); TEST_NOTE("__STDC_VERSION_STDINT_H__ not defined");
    test_skip("c2023:__STDC_VERSION_STDINT_H__");
#endif
}


TEST_CASE(stdint, rsize_max_c2011) {
#if defined(__STDC_LIB_EXT1__) && defined(RSIZE_MAX)
    test_true( RSIZE_MAX > 0 );
    test_true( (uintmax_t)RSIZE_MAX <= (uintmax_t)SIZE_MAX );
    test_pass("c2011:opt:RSIZE_MAX");
#else
    TEST_NOTE("Annex K is optional; this library does not provide it");
    test_skip("c2011:opt:RSIZE_MAX");
#endif
}

TEST_SUITE(stdint)
    TEST_ADD(exact_sizes)
    TEST_ADD(least_sizes)
    TEST_ADD(fast_sizes)
    TEST_ADD(ptr_max_sizes)
    TEST_ADD(exact_limit_values)
    TEST_ADD(least_magnitudes)
    TEST_ADD(fast_magnitudes)
    TEST_ADD(max_and_ptr_limits)
    TEST_ADD(other_limits)
    TEST_ADD(sig_atomic_wchar_wint_limits)
    TEST_ADD(constant_macros)
    TEST_ADD(pointer_roundtrip)
    TEST_ADD(exact_width_c23)
    TEST_ADD(least_fast_width_c23)
    TEST_ADD(ptr_max_other_width_c23)
    TEST_ADD(stdc_version_c23)
    TEST_ADD(rsize_max_c2011)
TEST_SUITE_END(stdint)
