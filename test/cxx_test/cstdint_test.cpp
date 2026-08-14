#include "test_cxx.hpp"

#include <cstdint>

TEST_CASE(cstdint, exact_sizes) {
    test_eq( sizeof(STD_NS int8_t),   1u );
    test_eq( sizeof(STD_NS int16_t),  2u );
    test_eq( sizeof(STD_NS int32_t),  4u );
    test_eq( sizeof(STD_NS int64_t),  8u );
    test_eq( sizeof(STD_NS uint8_t),  1u );
    test_eq( sizeof(STD_NS uint16_t), 2u );
    test_eq( sizeof(STD_NS uint32_t), 4u );
    test_eq( sizeof(STD_NS uint64_t), 8u );
    test_pass("cxx11:opt:int8_t..uint64_t exact-width types (sizes)");
}

TEST_CASE(cstdint, least_fast_ptr) {
    test_true( sizeof(STD_NS int_least8_t)  >= 1u );
    test_true( sizeof(STD_NS int_least16_t) >= 2u );
    test_true( sizeof(STD_NS int_least32_t) >= 4u );
    test_true( sizeof(STD_NS int_fast8_t)   >= 1u );
    test_true( sizeof(STD_NS int_fast32_t)  >= 4u );
    test_true( sizeof(STD_NS intmax_t)      >= 8u );
    test_true( sizeof(STD_NS uintmax_t)     >= 8u );
    test_eq( sizeof(STD_NS intptr_t),  sizeof(void*) );
    test_eq( sizeof(STD_NS uintptr_t), sizeof(void*) );
    test_pass("cxx11:<cstdint> least fast ptr max types reachable (smoke)");
}

TEST_CASE(cstdint, limits_macros) {
    STD_NS int32_t mx = INT32_MAX;
    test_eq( mx, 2147483647 );
    STD_NS int32_t mn = INT32_MIN;
    test_true( mn == -2147483647 - 1 );

    test_true( INT8_MAX == 127 );
    test_true( INT8_MIN == -128 );
    test_true( INT16_MAX == 32767 );

    STD_NS int64_t sc = INT64_C(-9000000000);
    test_eq( sc, (long long)-9000000000LL );

    test_eq( int(UINT8_MAX),  255 );
    test_eq( int(UINT16_MAX), 65535 );
    test_true( UINT32_MAX == 4294967295u );

    STD_NS uint64_t big = UINT64_C(0xFFFFFFFFFFFFFFFF);
    test_eq( big, 18446744073709551615ull );
    test_pass("cxx11:<cstdint> width-limit macros reachable (smoke)");
}

#if TEST_TARGET_CXX >= 2011
#include <cstdint>
#include <cstddef>
#include <climits>
#include <csignal>
#include <cwchar>
#include <limits>
#include <type_traits>

#define CHK_BITS(T)     ((int)(sizeof(T) * CHAR_BIT))

#define CHK_EXACT_S(T, BITS)                                                  \
    do {                                                                      \
        test_true( STD::is_integral<T>::value );                              \
        test_true( STD::is_signed<T>::value );                                \
        test_eq( CHK_BITS(T), (int)(BITS) );                                  \
    } while (0)
#define CHK_EXACT_U(T, BITS)                                                  \
    do {                                                                      \
        test_true( STD::is_integral<T>::value );                              \
        test_true( STD::is_unsigned<T>::value );                              \
        test_eq( CHK_BITS(T), (int)(BITS) );                                  \
    } while (0)

#define CHK_ATLEAST_S(T, BITS)                                                \
    do {                                                                      \
        test_true( STD::is_integral<T>::value );                              \
        test_true( STD::is_signed<T>::value );                                \
        test_ge( CHK_BITS(T), (int)(BITS) );                                  \
    } while (0)
#define CHK_ATLEAST_U(T, BITS)                                                \
    do {                                                                      \
        test_true( STD::is_integral<T>::value );                              \
        test_true( STD::is_unsigned<T>::value );                              \
        test_ge( CHK_BITS(T), (int)(BITS) );                                  \
    } while (0)

#define CHK_SLIMITS(T, MINM, MAXM)                                            \
    do {                                                                      \
        test_eq( (long long)(MINM), (long long)STD::numeric_limits<T>::min() );\
        test_eq( (long long)(MAXM), (long long)STD::numeric_limits<T>::max() );\
    } while (0)
#define CHK_ULIMIT(T, MAXM)                                                   \
    test_eq( (unsigned long long)(MAXM),                                      \
             (unsigned long long)STD::numeric_limits<T>::max() )

TEST_CASE(cstdint, exact_width_types) {
    CHK_EXACT_S(STD_NS int8_t,   8);
    CHK_EXACT_S(STD_NS int16_t, 16);
    CHK_EXACT_S(STD_NS int32_t, 32);
    CHK_EXACT_S(STD_NS int64_t, 64);

    CHK_EXACT_U(STD_NS uint8_t,   8);
    CHK_EXACT_U(STD_NS uint16_t, 16);
    CHK_EXACT_U(STD_NS uint32_t, 32);
    CHK_EXACT_U(STD_NS uint64_t, 64);

    STD_NS uint8_t u8 = 255; ++u8;
    test_eq( (int)u8, 0 );
    STD_NS uint16_t u16 = 65535; ++u16;
    test_eq( (int)u16, 0 );
}

TEST_CASE(cstdint, least_width_types) {
    CHK_ATLEAST_S(STD_NS int_least8_t,   8);
    CHK_ATLEAST_S(STD_NS int_least16_t, 16);
    CHK_ATLEAST_S(STD_NS int_least32_t, 32);
    CHK_ATLEAST_S(STD_NS int_least64_t, 64);

    test_le( CHK_BITS(STD_NS int_least8_t),  CHK_BITS(STD_NS int_least16_t) );
    test_le( CHK_BITS(STD_NS int_least16_t), CHK_BITS(STD_NS int_least32_t) );
    test_le( CHK_BITS(STD_NS int_least32_t), CHK_BITS(STD_NS int_least64_t) );
    test_pass("cxx11:int_leastN_t types");

    CHK_ATLEAST_U(STD_NS uint_least8_t,   8);
    CHK_ATLEAST_U(STD_NS uint_least16_t, 16);
    CHK_ATLEAST_U(STD_NS uint_least32_t, 32);
    CHK_ATLEAST_U(STD_NS uint_least64_t, 64);
    test_pass("cxx11:uint_leastN_t types");
}

TEST_CASE(cstdint, fast_width_types) {
    CHK_ATLEAST_S(STD_NS int_fast8_t,   8);
    CHK_ATLEAST_S(STD_NS int_fast16_t, 16);
    CHK_ATLEAST_S(STD_NS int_fast32_t, 32);
    CHK_ATLEAST_S(STD_NS int_fast64_t, 64);
    test_le( CHK_BITS(STD_NS int_fast8_t),  CHK_BITS(STD_NS int_fast64_t) );
    test_pass("cxx11:int_fastN_t types");

    CHK_ATLEAST_U(STD_NS uint_fast8_t,   8);
    CHK_ATLEAST_U(STD_NS uint_fast16_t, 16);
    CHK_ATLEAST_U(STD_NS uint_fast32_t, 32);
    CHK_ATLEAST_U(STD_NS uint_fast64_t, 64);
    test_le( CHK_BITS(STD_NS uint_fast8_t), CHK_BITS(STD_NS uint_fast64_t) );
    test_pass("cxx11:uint_fastN_t types");
}

TEST_CASE(cstdint, max_and_ptr_types) {

    CHK_ATLEAST_S(STD_NS intmax_t,  64);
    test_ge( CHK_BITS(STD_NS intmax_t),  CHK_BITS(STD_NS int64_t) );
    test_ge( CHK_BITS(STD_NS intmax_t),  CHK_BITS(STD_NS intptr_t) );
    test_pass("cxx11:intmax_t types");

    CHK_ATLEAST_U(STD_NS uintmax_t, 64);
    test_ge( CHK_BITS(STD_NS uintmax_t), CHK_BITS(STD_NS uint64_t) );
    test_eq( CHK_BITS(STD_NS intmax_t),  CHK_BITS(STD_NS uintmax_t) );
    test_pass("cxx11:uintmax_t types");

    test_true( STD::is_integral<STD_NS intptr_t>::value );
    test_true( STD::is_signed<STD_NS intptr_t>::value );
    test_ge( (int)sizeof(STD_NS intptr_t),  (int)sizeof(void*) );

    int x = 12345;
    void* p = &x;
    STD_NS intptr_t ip = (STD_NS intptr_t)p;
    void* back_i = (void*)ip;
    test_true( back_i == p );
    test_pass("cxx11:opt:intptr_t types");

    test_true( STD::is_integral<STD_NS uintptr_t>::value );
    test_true( STD::is_unsigned<STD_NS uintptr_t>::value );
    test_ge( (int)sizeof(STD_NS uintptr_t), (int)sizeof(void*) );

    STD_NS uintptr_t up = (STD_NS uintptr_t)p;
    void* back_u = (void*)up;
    test_true( back_u == p );
    test_eq( *(int*)back_u, 12345 );
    test_pass("cxx11:opt:uintptr_t types");
}

TEST_CASE(cstdint, exact_limit_macros) {

    CHK_SLIMITS(STD_NS int8_t,  INT8_MIN,  INT8_MAX);
    test_eq( (long long)INT8_MIN,  -128LL );
    test_pass("cxx11:opt:INT8_MIN");
    test_eq( (long long)INT8_MAX,   127LL );
    test_pass("cxx11:opt:INT8_MAX");

    CHK_SLIMITS(STD_NS int16_t,  INT16_MIN,  INT16_MAX);
    test_eq( (long long)INT16_MIN, -32768LL );
    test_pass("cxx11:opt:INT16_MIN");
    test_eq( (long long)INT16_MAX,  32767LL );
    test_pass("cxx11:opt:INT16_MAX");

    CHK_SLIMITS(STD_NS int32_t,  INT32_MIN,  INT32_MAX);
    test_eq( (long long)INT32_MIN, -2147483648LL );
    test_pass("cxx11:opt:INT32_MIN");
    test_eq( (long long)INT32_MAX,  2147483647LL );
    test_pass("cxx11:opt:INT32_MAX");

    CHK_SLIMITS(STD_NS int64_t,  INT64_MIN,  INT64_MAX);
    test_true( INT64_MIN + INT64_MAX == -1 );
    test_pass("cxx11:opt:INT64_MIN");
    test_eq( (long long)INT64_MAX,  9223372036854775807LL );
    test_pass("cxx11:opt:INT64_MAX");

    CHK_ULIMIT (STD_NS uint8_t, UINT8_MAX);
    test_eq( (unsigned long long)UINT8_MAX, 255ULL );
    test_pass("cxx11:opt:UINT8_MAX");
    CHK_ULIMIT (STD_NS uint16_t, UINT16_MAX);
    test_eq( (unsigned long long)UINT16_MAX, 65535ULL );
    test_pass("cxx11:opt:UINT16_MAX");
    CHK_ULIMIT (STD_NS uint32_t, UINT32_MAX);
    test_eq( (unsigned long long)UINT32_MAX, 4294967295ULL );
    test_pass("cxx11:opt:UINT32_MAX");
    CHK_ULIMIT (STD_NS uint64_t, UINT64_MAX);
    test_eq( (unsigned long long)UINT64_MAX, 18446744073709551615ULL );
    test_pass("cxx11:opt:UINT64_MAX");
}

TEST_CASE(cstdint, least_limit_macros) {

    CHK_SLIMITS(STD_NS int_least8_t,  INT_LEAST8_MIN,  INT_LEAST8_MAX);
    test_le( (long long)INT_LEAST8_MIN,  -127LL );
    test_pass("cxx11:INT_LEAST8_MIN");
    test_ge( (long long)INT_LEAST8_MAX,   127LL );
    test_pass("cxx11:INT_LEAST8_MAX");

    CHK_SLIMITS(STD_NS int_least16_t,  INT_LEAST16_MIN,  INT_LEAST16_MAX);
    test_le( (long long)INT_LEAST16_MIN, -32767LL );
    test_pass("cxx11:INT_LEAST16_MIN");
    test_ge( (long long)INT_LEAST16_MAX,  32767LL );
    test_pass("cxx11:INT_LEAST16_MAX");

    CHK_SLIMITS(STD_NS int_least32_t,  INT_LEAST32_MIN,  INT_LEAST32_MAX);
    test_le( (long long)INT_LEAST32_MIN, -2147483647LL );
    test_pass("cxx11:INT_LEAST32_MIN");
    test_ge( (long long)INT_LEAST32_MAX,  2147483647LL );
    test_pass("cxx11:INT_LEAST32_MAX");

    CHK_SLIMITS(STD_NS int_least64_t,  INT_LEAST64_MIN,  INT_LEAST64_MAX);
    test_le( (long long)INT_LEAST64_MIN, -9223372036854775807LL );
    test_pass("cxx11:INT_LEAST64_MIN");
    test_ge( (long long)INT_LEAST64_MAX,  9223372036854775807LL );
    test_pass("cxx11:INT_LEAST64_MAX");

    CHK_ULIMIT (STD_NS uint_least8_t, UINT_LEAST8_MAX);
    test_ge( (unsigned long long)UINT_LEAST8_MAX,  255ULL );
    test_pass("cxx11:UINT_LEAST8_MAX");
    CHK_ULIMIT (STD_NS uint_least16_t, UINT_LEAST16_MAX);
    test_ge( (unsigned long long)UINT_LEAST16_MAX, 65535ULL );
    test_pass("cxx11:UINT_LEAST16_MAX");
    CHK_ULIMIT (STD_NS uint_least32_t, UINT_LEAST32_MAX);
    test_ge( (unsigned long long)UINT_LEAST32_MAX, 4294967295ULL );
    test_pass("cxx11:UINT_LEAST32_MAX");
    CHK_ULIMIT (STD_NS uint_least64_t, UINT_LEAST64_MAX);
    test_ge( (unsigned long long)UINT_LEAST64_MAX, 18446744073709551615ULL );
    test_pass("cxx11:UINT_LEAST64_MAX");
}

TEST_CASE(cstdint, fast_limit_macros) {
    CHK_SLIMITS(STD_NS int_fast8_t,  INT_FAST8_MIN,  INT_FAST8_MAX);
    test_le( (long long)INT_FAST8_MIN,  -127LL );
    test_pass("cxx11:INT_FAST8_MIN");
    test_ge( (long long)INT_FAST8_MAX,   127LL );
    test_pass("cxx11:INT_FAST8_MAX");

    CHK_SLIMITS(STD_NS int_fast16_t,  INT_FAST16_MIN,  INT_FAST16_MAX);
    test_le( (long long)INT_FAST16_MIN, -32767LL );
    test_pass("cxx11:INT_FAST16_MIN");
    test_ge( (long long)INT_FAST16_MAX,  32767LL );
    test_pass("cxx11:INT_FAST16_MAX");

    CHK_SLIMITS(STD_NS int_fast32_t,  INT_FAST32_MIN,  INT_FAST32_MAX);
    test_le( (long long)INT_FAST32_MIN, -2147483647LL );
    test_pass("cxx11:INT_FAST32_MIN");
    test_ge( (long long)INT_FAST32_MAX,  2147483647LL );
    test_pass("cxx11:INT_FAST32_MAX");

    CHK_SLIMITS(STD_NS int_fast64_t,  INT_FAST64_MIN,  INT_FAST64_MAX);
    test_le( (long long)INT_FAST64_MIN, -9223372036854775807LL );
    test_pass("cxx11:INT_FAST64_MIN");
    test_ge( (long long)INT_FAST64_MAX,  9223372036854775807LL );
    test_pass("cxx11:INT_FAST64_MAX");

    CHK_ULIMIT (STD_NS uint_fast8_t, UINT_FAST8_MAX);
    test_ge( (unsigned long long)UINT_FAST8_MAX,  255ULL );
    test_pass("cxx11:UINT_FAST8_MAX");
    CHK_ULIMIT (STD_NS uint_fast16_t, UINT_FAST16_MAX);
    test_ge( (unsigned long long)UINT_FAST16_MAX, 65535ULL );
    test_pass("cxx11:UINT_FAST16_MAX");
    CHK_ULIMIT (STD_NS uint_fast32_t, UINT_FAST32_MAX);
    test_ge( (unsigned long long)UINT_FAST32_MAX, 4294967295ULL );
    test_pass("cxx11:UINT_FAST32_MAX");
    CHK_ULIMIT (STD_NS uint_fast64_t, UINT_FAST64_MAX);
    test_ge( (unsigned long long)UINT_FAST64_MAX, 18446744073709551615ULL );
    test_pass("cxx11:UINT_FAST64_MAX");
}

TEST_CASE(cstdint, max_and_ptr_limit_macros) {
    CHK_SLIMITS(STD_NS intmax_t, INTMAX_MIN, INTMAX_MAX);
    test_le( (long long)INTMAX_MIN, -9223372036854775807LL );
    test_pass("cxx11:INTMAX_MIN");
    test_ge( (long long)INTMAX_MAX,  9223372036854775807LL );
    test_pass("cxx11:INTMAX_MAX");

    CHK_ULIMIT (STD_NS uintmax_t, UINTMAX_MAX);
    test_ge( (unsigned long long)UINTMAX_MAX, 18446744073709551615ULL );
    test_pass("cxx11:UINTMAX_MAX");

    CHK_SLIMITS(STD_NS intptr_t, INTPTR_MIN, INTPTR_MAX);
    test_le( (long long)INTPTR_MIN, 0LL );
    test_pass("cxx11:opt:INTPTR_MIN");
    test_gt( (long long)INTPTR_MAX, 0LL );
    test_pass("cxx11:opt:INTPTR_MAX");

    CHK_ULIMIT (STD_NS uintptr_t, UINTPTR_MAX);
    int x = 0;
    STD_NS uintptr_t up = (STD_NS uintptr_t)(void*)&x;
    test_le( (unsigned long long)up, (unsigned long long)UINTPTR_MAX );
    test_pass("cxx11:opt:UINTPTR_MAX");
}

TEST_CASE(cstdint, other_limit_macros) {

    CHK_SLIMITS(STD::ptrdiff_t, PTRDIFF_MIN, PTRDIFF_MAX);
    test_le( (long long)PTRDIFF_MIN, -65535LL );
    test_pass("cxx11:PTRDIFF_MIN");
    test_ge( (long long)PTRDIFF_MAX,  65535LL );
    test_pass("cxx11:PTRDIFF_MAX");

    CHK_ULIMIT (STD::size_t, SIZE_MAX);
    test_ge( (unsigned long long)SIZE_MAX, 65535ULL );

    STD::size_t sm = (STD::size_t)SIZE_MAX;
    STD::size_t wrapped = sm + 1;
    test_eq( (unsigned long long)wrapped, 0ULL );
    test_pass("cxx11:SIZE_MAX");

    CHK_SLIMITS(STD::sig_atomic_t, SIG_ATOMIC_MIN, SIG_ATOMIC_MAX);
    test_le( (long long)SIG_ATOMIC_MIN, 0LL );
    test_pass("cxx11:SIG_ATOMIC_MIN");
    test_ge( (long long)SIG_ATOMIC_MAX, 127LL );
    test_pass("cxx11:SIG_ATOMIC_MAX");

    CHK_SLIMITS(wchar_t, WCHAR_MIN, WCHAR_MAX);

    test_le( (long long)WCHAR_MIN, 0LL );
    test_pass("cxx11:WCHAR_MIN");
    test_ge( (long long)WCHAR_MAX, 255LL );
    test_pass("cxx11:WCHAR_MAX");

    CHK_SLIMITS(STD::wint_t, WINT_MIN, WINT_MAX);
    test_le( (long long)WINT_MIN, 0LL );
    test_pass("cxx11:WINT_MIN");
    test_ge( (long long)WINT_MAX, 0LL );
    test_pass("cxx11:WINT_MAX");
}

TEST_CASE(cstdint, constant_macros) {

    test_eq( (long long)INT8_C(-128), -128LL );
    test_eq( (long long)INT8_C(127),   127LL );
    test_eq( (long long)INT16_C(-32768), -32768LL );
    test_eq( (long long)INT32_C(2147483647), 2147483647LL );
    test_eq( (long long)INT64_C(9223372036854775807), 9223372036854775807LL );
    test_ge( (int)sizeof(INT8_C(1)),   (int)sizeof(STD_NS int_least8_t) );
    test_ge( (int)sizeof(INT16_C(1)),  (int)sizeof(STD_NS int_least16_t) );
    test_ge( (int)sizeof(INT32_C(1)),  4 );
    test_ge( (int)sizeof(INT64_C(1)),  8 );

    static const int a[INT32_C(3)] = { 1, 2, 3 };
    test_eq( a[UINT8_C(2)], 3 );
    enum { E64 = (int)INT64_C(7) };
    test_eq( (int)E64, 7 );
#if _tst_cplusplus >= 201103L
    test_true( STD::is_signed<decltype(INT32_C(1))>::value );
    test_true( STD::is_signed<decltype(INT64_C(1))>::value );
    test_pass("cxx11:opt:INTn_C");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:opt:INTn_C");
#endif

    test_eq( (unsigned long long)UINT8_C(255), 255ULL );
    test_eq( (unsigned long long)UINT16_C(65535), 65535ULL );
    test_eq( (unsigned long long)UINT32_C(4294967295), 4294967295ULL );
    test_eq( (unsigned long long)UINT64_C(18446744073709551615), 18446744073709551615ULL );
    test_ge( (int)sizeof(UINT8_C(1)),  1 );
    test_ge( (int)sizeof(UINT16_C(1)), 2 );
    test_ge( (int)sizeof(UINT32_C(1)), 4 );
    test_ge( (int)sizeof(UINT64_C(1)), 8 );
#if _tst_cplusplus >= 201103L
    test_true( STD::is_unsigned<decltype(UINT32_C(1))>::value );
    test_true( STD::is_unsigned<decltype(UINT64_C(1))>::value );
    test_pass("cxx11:opt:UINTn_C");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:opt:UINTn_C");
#endif

    test_eq( (long long)INTMAX_C(9223372036854775807), (long long)INTMAX_MAX );
    test_ge( (int)sizeof(INTMAX_C(1)),  (int)sizeof(STD_NS intmax_t) );
#if _tst_cplusplus >= 201103L
    test_true( STD::is_signed<decltype(INTMAX_C(1))>::value );
    test_pass("cxx11:INTMAX_C");
#else
    TEST_SKIP1();
    test_skip("cxx11:INTMAX_C");
#endif

    test_eq( (unsigned long long)UINTMAX_C(18446744073709551615),
             (unsigned long long)UINTMAX_MAX );
    test_ge( (int)sizeof(UINTMAX_C(1)), (int)sizeof(STD_NS uintmax_t) );
#if _tst_cplusplus >= 201103L
    test_true( STD::is_unsigned<decltype(UINTMAX_C(1))>::value );
    test_pass("cxx11:UINTMAX_C");
#else
    TEST_SKIP1();
    test_skip("cxx11:UINTMAX_C");
#endif
}

#if defined(INT8_WIDTH)
TEST_CASE(cstdint, width_macros_chk) {
    test_eq( (int)INT8_WIDTH,   CHK_BITS(STD_NS int8_t) );
    test_eq( (int)UINT8_WIDTH,  CHK_BITS(STD_NS uint8_t) );
    test_eq( (int)INT16_WIDTH,  CHK_BITS(STD_NS int16_t) );
    test_eq( (int)UINT16_WIDTH, CHK_BITS(STD_NS uint16_t) );
    test_eq( (int)INT32_WIDTH,  CHK_BITS(STD_NS int32_t) );
    test_eq( (int)UINT32_WIDTH, CHK_BITS(STD_NS uint32_t) );
    test_eq( (int)INT64_WIDTH,  CHK_BITS(STD_NS int64_t) );
    test_eq( (int)UINT64_WIDTH, CHK_BITS(STD_NS uint64_t) );
    test_pass("cxx26:INTn_WIDTH");

    test_eq( (int)INT_LEAST8_WIDTH,   CHK_BITS(STD_NS int_least8_t) );
    test_pass("cxx26:INT_LEAST8_WIDTH");
    test_eq( (int)UINT_LEAST8_WIDTH,  CHK_BITS(STD_NS uint_least8_t) );
    test_pass("cxx26:UINT_LEAST8_WIDTH");
    test_eq( (int)INT_LEAST16_WIDTH,  CHK_BITS(STD_NS int_least16_t) );
    test_pass("cxx26:INT_LEAST16_WIDTH");
    test_eq( (int)UINT_LEAST16_WIDTH, CHK_BITS(STD_NS uint_least16_t) );
    test_pass("cxx26:UINT_LEAST16_WIDTH");
    test_eq( (int)INT_LEAST32_WIDTH,  CHK_BITS(STD_NS int_least32_t) );
    test_pass("cxx26:INT_LEAST32_WIDTH");
    test_eq( (int)UINT_LEAST32_WIDTH, CHK_BITS(STD_NS uint_least32_t) );
    test_pass("cxx26:UINT_LEAST32_WIDTH");
    test_eq( (int)INT_LEAST64_WIDTH,  CHK_BITS(STD_NS int_least64_t) );
    test_pass("cxx26:INT_LEAST64_WIDTH");
    test_eq( (int)UINT_LEAST64_WIDTH, CHK_BITS(STD_NS uint_least64_t) );
    test_pass("cxx26:UINT_LEAST64_WIDTH");

    test_eq( (int)INT_FAST8_WIDTH,   CHK_BITS(STD_NS int_fast8_t) );
    test_pass("cxx26:INT_FAST8_WIDTH");
    test_eq( (int)UINT_FAST8_WIDTH,  CHK_BITS(STD_NS uint_fast8_t) );
    test_pass("cxx26:UINT_FAST8_WIDTH");
    test_eq( (int)INT_FAST16_WIDTH,  CHK_BITS(STD_NS int_fast16_t) );
    test_pass("cxx26:INT_FAST16_WIDTH");
    test_eq( (int)UINT_FAST16_WIDTH, CHK_BITS(STD_NS uint_fast16_t) );
    test_pass("cxx26:UINT_FAST16_WIDTH");
    test_eq( (int)INT_FAST32_WIDTH,  CHK_BITS(STD_NS int_fast32_t) );
    test_pass("cxx26:INT_FAST32_WIDTH");
    test_eq( (int)UINT_FAST32_WIDTH, CHK_BITS(STD_NS uint_fast32_t) );
    test_pass("cxx26:UINT_FAST32_WIDTH");
    test_eq( (int)INT_FAST64_WIDTH,  CHK_BITS(STD_NS int_fast64_t) );
    test_pass("cxx26:INT_FAST64_WIDTH");
    test_eq( (int)UINT_FAST64_WIDTH, CHK_BITS(STD_NS uint_fast64_t) );
    test_pass("cxx26:UINT_FAST64_WIDTH");

    test_eq( (int)INTMAX_WIDTH,   CHK_BITS(STD_NS intmax_t) );
    test_pass("cxx26:INTMAX_WIDTH");
    test_eq( (int)UINTMAX_WIDTH,  CHK_BITS(STD_NS uintmax_t) );
    test_pass("cxx26:UINTMAX_WIDTH");
    test_eq( (int)INTPTR_WIDTH,   CHK_BITS(STD_NS intptr_t) );
    test_pass("cxx26:opt:INTPTR_WIDTH");
    test_eq( (int)UINTPTR_WIDTH,  CHK_BITS(STD_NS uintptr_t) );
    test_pass("cxx26:opt:UINTPTR_WIDTH");
    test_eq( (int)PTRDIFF_WIDTH,  CHK_BITS(STD::ptrdiff_t) );
    test_pass("cxx26:PTRDIFF_WIDTH");
    test_eq( (int)SIZE_WIDTH,     CHK_BITS(STD::size_t) );
    test_pass("cxx26:SIZE_WIDTH");
    test_eq( (int)SIG_ATOMIC_WIDTH, CHK_BITS(STD::sig_atomic_t) );
    test_pass("cxx26:SIG_ATOMIC_WIDTH");
    test_eq( (int)WCHAR_WIDTH,    CHK_BITS(wchar_t) );
    test_pass("cxx26:WCHAR_WIDTH");
    test_eq( (int)WINT_WIDTH,     CHK_BITS(STD::wint_t) );
    test_pass("cxx26:WINT_WIDTH");
}
#else
TEST_CASE_SKIP(cstdint, width_macros)
#endif

#if defined(__STDC_VERSION_STDINT_H__)
TEST_CASE(cstdint, stdc_version_stdint_h_chk) {
    test_ge( (long long)__STDC_VERSION_STDINT_H__, 202311LL );
    test_pass("cxx26:__STDC_VERSION_STDINT_H__");
}
#else
TEST_CASE_SKIP(cstdint, stdc_version_stdint_h)
#endif
#else
TEST_CASE_SKIP(cstdint, exact_width_types)
TEST_CASE_SKIP(cstdint, least_width_types)
TEST_CASE_SKIP(cstdint, fast_width_types)
TEST_CASE_SKIP(cstdint, max_and_ptr_types)
TEST_CASE_SKIP(cstdint, exact_limit_macros)
TEST_CASE_SKIP(cstdint, least_limit_macros)
TEST_CASE_SKIP(cstdint, fast_limit_macros)
TEST_CASE_SKIP(cstdint, max_and_ptr_limit_macros)
TEST_CASE_SKIP(cstdint, other_limit_macros)
TEST_CASE_SKIP(cstdint, constant_macros)
TEST_CASE_SKIP(cstdint, width_macros)
TEST_CASE_SKIP(cstdint, width_macros_chk)
TEST_CASE_SKIP(cstdint, stdc_version_stdint_h)
TEST_CASE_SKIP(cstdint, stdc_version_stdint_h_chk)
#endif


#if TEST_TARGET_CXX >= 2011
TEST_CASE(cstdint, least_and_fast_types_cxx11) {
    test_true( sizeof(STD_NS int_least8_t)  * 8 >= 8 );
    test_true( STD::numeric_limits<STD_NS int_least8_t>::is_signed );
    test_pass("cxx11:int_least8_t");
    test_true( sizeof(STD_NS int_least16_t) * 8 >= 16 );
    test_pass("cxx11:int_least16_t");
    test_true( sizeof(STD_NS int_least32_t) * 8 >= 32 );
    test_pass("cxx11:int_least32_t");
    test_true( sizeof(STD_NS int_least64_t) * 8 >= 64 );
    test_pass("cxx11:int_least64_t");

    test_true( sizeof(STD_NS uint_least8_t)  == sizeof(STD_NS int_least8_t) );
    test_true( !STD::numeric_limits<STD_NS uint_least8_t>::is_signed );
    test_pass("cxx11:uint_least8_t");
    test_true( sizeof(STD_NS uint_least16_t) == sizeof(STD_NS int_least16_t) );
    test_pass("cxx11:uint_least16_t");
    test_true( sizeof(STD_NS uint_least32_t) == sizeof(STD_NS int_least32_t) );
    test_pass("cxx11:uint_least32_t");
    test_true( sizeof(STD_NS uint_least64_t) == sizeof(STD_NS int_least64_t) );
    test_pass("cxx11:uint_least64_t");

    test_true( sizeof(STD_NS int_fast8_t)  * 8 >= 8 );
    test_true( STD::numeric_limits<STD_NS int_fast8_t>::is_signed );
    test_pass("cxx11:int_fast8_t");
    test_true( sizeof(STD_NS int_fast16_t) * 8 >= 16 );
    test_pass("cxx11:int_fast16_t");
    test_true( sizeof(STD_NS int_fast32_t) * 8 >= 32 );
    test_pass("cxx11:int_fast32_t");
    test_true( sizeof(STD_NS int_fast64_t) * 8 >= 64 );
    test_pass("cxx11:int_fast64_t");

    test_true( sizeof(STD_NS uint_fast8_t)  == sizeof(STD_NS int_fast8_t) );
    test_true( !STD::numeric_limits<STD_NS uint_fast8_t>::is_signed );
    test_pass("cxx11:uint_fast8_t");
    test_true( sizeof(STD_NS uint_fast16_t) == sizeof(STD_NS int_fast16_t) );
    test_pass("cxx11:uint_fast16_t");
    test_true( sizeof(STD_NS uint_fast32_t) == sizeof(STD_NS int_fast32_t) );
    test_pass("cxx11:uint_fast32_t");
    test_true( sizeof(STD_NS uint_fast64_t) == sizeof(STD_NS int_fast64_t) );
    test_pass("cxx11:uint_fast64_t");
}

TEST_CASE(cstdint, exact_width_types_cxx11) {
#if defined(INT16_MAX)
    test_true( sizeof(STD_NS int16_t) * 8 == 16 );
    test_true( STD::numeric_limits<STD_NS int16_t>::is_signed );
    test_pass("cxx11:opt:int16_t");
    test_true( sizeof(STD_NS uint16_t) * 8 == 16 );
    test_true( !STD::numeric_limits<STD_NS uint16_t>::is_signed );
    test_pass("cxx11:opt:uint16_t");
#else
    test_skip("cxx11:opt:int16_t");
    test_skip("cxx11:opt:uint16_t");
#endif
#if defined(INT32_MAX)
    test_true( sizeof(STD_NS int32_t) * 8 == 32 );
    test_pass("cxx11:opt:int32_t");
    test_true( sizeof(STD_NS uint32_t) * 8 == 32 );
    test_pass("cxx11:opt:uint32_t");
#else
    test_skip("cxx11:opt:int32_t");
    test_skip("cxx11:opt:uint32_t");
#endif
#if defined(INT64_MAX)
    test_true( sizeof(STD_NS int64_t) * 8 == 64 );
    test_pass("cxx11:opt:int64_t");
#else
    test_skip("cxx11:opt:int64_t");
#endif
#if defined(UINT8_MAX)
    test_true( sizeof(STD_NS uint8_t) * 8 == 8 );
    test_true( (int)STD::numeric_limits<STD_NS uint8_t>::max() == 255 );
    test_pass("cxx11:opt:uint8_t");
#else
    test_skip("cxx11:opt:uint8_t");
#endif
}
#else
TEST_CASE(cstdint, least_and_fast_types_cxx11) {
    TEST_SKIP_N(0);
    test_skip("cxx11:int_least8_t");   test_skip("cxx11:int_least16_t");
    test_skip("cxx11:int_least32_t");  test_skip("cxx11:int_least64_t");
    test_skip("cxx11:uint_least8_t");  test_skip("cxx11:uint_least16_t");
    test_skip("cxx11:uint_least32_t"); test_skip("cxx11:uint_least64_t");
    test_skip("cxx11:int_fast8_t");    test_skip("cxx11:int_fast16_t");
    test_skip("cxx11:int_fast32_t");   test_skip("cxx11:int_fast64_t");
    test_skip("cxx11:uint_fast8_t");   test_skip("cxx11:uint_fast16_t");
    test_skip("cxx11:uint_fast32_t");  test_skip("cxx11:uint_fast64_t");
}
TEST_CASE(cstdint, exact_width_types_cxx11) {
    test_skip("cxx11:opt:int16_t");  test_skip("cxx11:opt:uint16_t");
    test_skip("cxx11:opt:int32_t");  test_skip("cxx11:opt:uint32_t");
    test_skip("cxx11:opt:int64_t");  test_skip("cxx11:opt:uint8_t");
}
#endif


TEST_CASE(cstdint, generic_type_families_cxx11) {
#if defined(INT32_MAX)
    test_true( sizeof(STD_NS int32_t) * 8 == 32 );
    test_pass("cxx11:opt:intN_t");
    test_true( sizeof(STD_NS uint32_t) * 8 == 32 );
    test_pass("cxx11:opt:uintN_t");
#else
    test_skip("cxx11:opt:intN_t");
    test_skip("cxx11:opt:uintN_t");
#endif
#if defined(UINT64_MAX)
    test_true( sizeof(STD_NS uint64_t) * 8 == 64 );
    test_pass("cxx11:opt:uint64_t");
#else
    test_skip("cxx11:opt:uint64_t");
#endif

    test_true( sizeof(STD_NS int_least32_t) * 8 >= 32 );
    test_pass("cxx11:opt:int_leastN_t");
    test_true( sizeof(STD_NS uint_least32_t) * 8 >= 32 );
    test_pass("cxx11:opt:uint_leastN_t");
    test_true( sizeof(STD_NS int_fast32_t) * 8 >= 32 );
    test_pass("cxx11:opt:int_fastN_t");
    test_true( sizeof(STD_NS uint_fast32_t) * 8 >= 32 );
    test_pass("cxx11:opt:uint_fastN_t");

    test_true( sizeof(STD_NS intmax_t) >= sizeof(long long) );
    test_true( (STD_NS intmax_t)INTMAX_MAX >= (STD_NS intmax_t)0x7FFFFFFF );
    test_pass("cxx11:intmax_t");
    test_true( sizeof(STD_NS uintmax_t) == sizeof(STD_NS intmax_t) );
    test_true( (STD_NS uintmax_t)UINTMAX_MAX > (STD_NS uintmax_t)INTMAX_MAX );
    test_pass("cxx11:uintmax_t");

#if defined(INTPTR_MAX)
    {
        int obj = 7;
        int* p = &obj;
        STD_NS intptr_t iv = (STD_NS intptr_t)p;
        test_true( (int*)iv == p );
        STD_NS uintptr_t uv = (STD_NS uintptr_t)p;
        test_true( (int*)uv == p );
        test_true( sizeof(STD_NS intptr_t) >= sizeof(void*) );
    }
    test_pass("cxx11:opt:intptr_t");
    test_pass("cxx11:opt:uintptr_t");
#else
    TEST_NOTE("intptr_t / uintptr_t are optional and absent here");
    test_skip("cxx11:opt:intptr_t");
    test_skip("cxx11:opt:uintptr_t");
#endif
}

TEST_CASE(cstdint, width_macros_cxx26) {
#if defined(INTMAX_WIDTH)
    test_eq( (int)INTMAX_WIDTH, (int)(sizeof(STD_NS intmax_t) * CHAR_BIT) );
    test_pass("cxx26:INTMAX_WIDTH");
    test_eq( (int)UINTMAX_WIDTH, (int)INTMAX_WIDTH );
    test_pass("cxx26:UINTMAX_WIDTH");

    test_eq( (int)INT_LEAST8_WIDTH,  (int)(sizeof(STD_NS int_least8_t)  * CHAR_BIT) );
    test_pass("cxx26:INT_LEAST8_WIDTH");
    test_eq( (int)INT_LEAST16_WIDTH, (int)(sizeof(STD_NS int_least16_t) * CHAR_BIT) );
    test_pass("cxx26:INT_LEAST16_WIDTH");
    test_eq( (int)INT_LEAST32_WIDTH, (int)(sizeof(STD_NS int_least32_t) * CHAR_BIT) );
    test_pass("cxx26:INT_LEAST32_WIDTH");
    test_eq( (int)INT_LEAST64_WIDTH, (int)(sizeof(STD_NS int_least64_t) * CHAR_BIT) );
    test_pass("cxx26:INT_LEAST64_WIDTH");
    test_eq( (int)UINT_LEAST8_WIDTH,  (int)INT_LEAST8_WIDTH );
    test_pass("cxx26:UINT_LEAST8_WIDTH");
    test_eq( (int)UINT_LEAST16_WIDTH, (int)INT_LEAST16_WIDTH );
    test_pass("cxx26:UINT_LEAST16_WIDTH");
    test_eq( (int)UINT_LEAST32_WIDTH, (int)INT_LEAST32_WIDTH );
    test_pass("cxx26:UINT_LEAST32_WIDTH");
    test_eq( (int)UINT_LEAST64_WIDTH, (int)INT_LEAST64_WIDTH );
    test_pass("cxx26:UINT_LEAST64_WIDTH");

    test_eq( (int)INT_FAST8_WIDTH,  (int)(sizeof(STD_NS int_fast8_t)  * CHAR_BIT) );
    test_pass("cxx26:INT_FAST8_WIDTH");
    test_eq( (int)INT_FAST16_WIDTH, (int)(sizeof(STD_NS int_fast16_t) * CHAR_BIT) );
    test_pass("cxx26:INT_FAST16_WIDTH");
    test_eq( (int)INT_FAST32_WIDTH, (int)(sizeof(STD_NS int_fast32_t) * CHAR_BIT) );
    test_pass("cxx26:INT_FAST32_WIDTH");
    test_eq( (int)INT_FAST64_WIDTH, (int)(sizeof(STD_NS int_fast64_t) * CHAR_BIT) );
    test_pass("cxx26:INT_FAST64_WIDTH");
    test_eq( (int)UINT_FAST8_WIDTH,  (int)INT_FAST8_WIDTH );
    test_pass("cxx26:UINT_FAST8_WIDTH");
    test_eq( (int)UINT_FAST16_WIDTH, (int)INT_FAST16_WIDTH );
    test_pass("cxx26:UINT_FAST16_WIDTH");
    test_eq( (int)UINT_FAST32_WIDTH, (int)INT_FAST32_WIDTH );
    test_pass("cxx26:UINT_FAST32_WIDTH");
    test_eq( (int)UINT_FAST64_WIDTH, (int)INT_FAST64_WIDTH );
    test_pass("cxx26:UINT_FAST64_WIDTH");

    test_eq( (int)PTRDIFF_WIDTH, (int)(sizeof(STD::ptrdiff_t) * CHAR_BIT) );
    test_pass("cxx26:PTRDIFF_WIDTH");
    test_eq( (int)SIZE_WIDTH, (int)(sizeof(STD::size_t) * CHAR_BIT) );
    test_pass("cxx26:SIZE_WIDTH");
    test_eq( (int)WCHAR_WIDTH, (int)(sizeof(wchar_t) * CHAR_BIT) );
    test_pass("cxx26:WCHAR_WIDTH");
    test_true( WINT_WIDTH >= 16 );
    test_pass("cxx26:WINT_WIDTH");
    test_true( SIG_ATOMIC_WIDTH >= 8 );
    test_pass("cxx26:SIG_ATOMIC_WIDTH");

#if defined(INT32_WIDTH)
    test_eq( (int)INT32_WIDTH, 32 );
    test_eq( (int)UINT32_WIDTH, 32 );
    test_pass("cxx26:INTn_WIDTH");
#else
    test_skip("cxx26:INTn_WIDTH");
#endif
#if defined(INTPTR_WIDTH)
    test_eq( (int)INTPTR_WIDTH, (int)(sizeof(STD_NS intptr_t) * CHAR_BIT) );
    test_pass("cxx26:opt:INTPTR_WIDTH");
    test_eq( (int)UINTPTR_WIDTH, (int)INTPTR_WIDTH );
    test_pass("cxx26:opt:UINTPTR_WIDTH");
#else
    test_skip("cxx26:opt:INTPTR_WIDTH");
    test_skip("cxx26:opt:UINTPTR_WIDTH");
#endif
#else
    TEST_NOTE("the *_WIDTH macros are C23/C++26; not in this library");
    test_skip("cxx26:INTMAX_WIDTH");       test_skip("cxx26:UINTMAX_WIDTH");
    test_skip("cxx26:INT_LEAST8_WIDTH");   test_skip("cxx26:INT_LEAST16_WIDTH");
    test_skip("cxx26:INT_LEAST32_WIDTH");  test_skip("cxx26:INT_LEAST64_WIDTH");
    test_skip("cxx26:UINT_LEAST8_WIDTH");  test_skip("cxx26:UINT_LEAST16_WIDTH");
    test_skip("cxx26:UINT_LEAST32_WIDTH"); test_skip("cxx26:UINT_LEAST64_WIDTH");
    test_skip("cxx26:INT_FAST8_WIDTH");    test_skip("cxx26:INT_FAST16_WIDTH");
    test_skip("cxx26:INT_FAST32_WIDTH");   test_skip("cxx26:INT_FAST64_WIDTH");
    test_skip("cxx26:UINT_FAST8_WIDTH");   test_skip("cxx26:UINT_FAST16_WIDTH");
    test_skip("cxx26:UINT_FAST32_WIDTH");  test_skip("cxx26:UINT_FAST64_WIDTH");
    test_skip("cxx26:PTRDIFF_WIDTH");      test_skip("cxx26:SIZE_WIDTH");
    test_skip("cxx26:WCHAR_WIDTH");        test_skip("cxx26:WINT_WIDTH");
    test_skip("cxx26:SIG_ATOMIC_WIDTH");   test_skip("cxx26:INTn_WIDTH");
    test_skip("cxx26:opt:INTPTR_WIDTH");   test_skip("cxx26:opt:UINTPTR_WIDTH");
#endif

#if defined(__STDC_VERSION_STDINT_H__)
    test_true( __STDC_VERSION_STDINT_H__ >= 202311L );
    test_pass("cxx26:__STDC_VERSION_STDINT_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("cxx26:__STDC_VERSION_STDINT_H__");
#endif
}
