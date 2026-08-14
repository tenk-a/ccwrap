#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020
#include <stdckdint.h>
#include <numeric>
#include <stdbit.h>
#include <source_location>
#include <climits>

TEST_CASE(stdckdint, checked_arithmetic) {
#if defined(_CCW_STD_CKD_DEFINED)
    int cr;
    test_true( !STD::ckd_add(&cr, 2, 3) );
    test_eq( cr, 5 );
    test_true( STD::ckd_add(&cr, INT_MAX, 1) );

#if !defined(_MSC_VER)
    test_true( !STD::ckd_add(&cr, -1, 1u) );
    test_eq( cr, 0 );
    test_pass("cxx26:ckd_add");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx26:ckd_add");
#endif

    test_true( !STD::ckd_mul(&cr, 6, 7) );
    test_eq( cr, 42 );
    test_true( STD::ckd_mul(&cr, INT_MIN, -1) );
    test_pass("cxx26:ckd_mul");

    unsigned cu;
    test_true( !STD::ckd_sub(&cu, 30u, 5u) );
    test_eq( cu, 25u );
    test_true( STD::ckd_sub(&cu, 1u, 2u) );
#if !defined(_MSC_VER)
    test_true( STD::ckd_sub(&cu, 5u, 10u) );
#else
    TEST_SKIP1();
#endif

#ifdef __STDC_VERSION_STDCKDINT_H__
    test_true( __STDC_VERSION_STDCKDINT_H__ >= 202311L );
#else
    test_true( false );
#endif
    test_pass("cxx26:ckd_sub");
#else
    test_skip("cxx26:ckd_add");
    test_skip("cxx26:ckd_mul");
    test_skip("cxx26:ckd_sub");
#endif

#if defined(__cpp_lib_stdckdint_h) && (__cpp_lib_stdckdint_h) >= 202603L
    test_true( (__cpp_lib_stdckdint_h) >= 202603L );
    test_pass("cxx26:__cpp_lib_stdckdint_h=202603L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_stdckdint_h=202603L");
#endif
}

#if defined(_CCW_STD_STDBIT_DEFINED)
TEST_CASE(stdbit, bit_queries) {
    test_eq( (int)STD::stdc_count_ones((unsigned)0x0Fu), 4 );
    test_pass("cxx26:stdc_count_ones");
    test_eq( (int)STD::stdc_bit_width((unsigned)0x0Fu), 4 );
    test_pass("cxx26:stdc_bit_width");
    test_eq( (unsigned)STD::stdc_bit_floor((unsigned)20u), 16u );
    test_pass("cxx26:stdc_bit_floor");
    test_eq( (unsigned)STD::stdc_bit_ceil((unsigned)17u), 32u );
    test_pass("cxx26:stdc_bit_ceil");
    test_true(  STD::stdc_has_single_bit((unsigned)16u) );
    test_true( !STD::stdc_has_single_bit((unsigned)18u) );
    test_pass("cxx26:stdc_has_single_bit");
    test_eq( (int)STD::stdc_leading_zeros_us((unsigned short)1), 15 );
    test_pass("cxx26:stdc_leading_zeros_us");
    test_eq( (int)STD::stdc_count_ones_ull((unsigned long long)0xFFu), 8 );
    test_pass("cxx26:stdc_count_ones_ull");
}

TEST_CASE(stdbit, first_and_run_queries) {
    test_eq( (int)STD::stdc_first_leading_one((unsigned)0x80000000u), 1 );
    test_eq( (int)STD::stdc_first_leading_one((unsigned)0u), 0 );
    test_pass("cxx26:stdc_first_leading_one");

    test_eq( (int)STD::stdc_first_leading_zero((unsigned)0x7FFFFFFFu), 1 );
    test_eq( (int)STD::stdc_first_leading_zero((unsigned)0xFFFFFFFFu), 0 );
    test_pass("cxx26:stdc_first_leading_zero");

    test_eq( (int)STD::stdc_first_trailing_one((unsigned)0x0Cu), 3 );
    test_eq( (int)STD::stdc_first_trailing_one((unsigned)0u), 0 );
    test_pass("cxx26:stdc_first_trailing_one");

    test_eq( (int)STD::stdc_first_trailing_zero((unsigned)0x0Bu), 3 );
    test_eq( (int)STD::stdc_first_trailing_zero((unsigned)0xFFFFFFFFu), 0 );
    test_pass("cxx26:stdc_first_trailing_zero");

    test_eq( (int)STD::stdc_leading_ones((unsigned)0xF0000000u), 4 );
    test_eq( (int)STD::stdc_leading_ones((unsigned)0u), 0 );
    test_pass("cxx26:stdc_leading_ones");

    test_eq( (int)STD::stdc_trailing_ones((unsigned)0x07u), 3 );
    test_eq( (int)STD::stdc_trailing_ones((unsigned)0u), 0 );
    test_pass("cxx26:stdc_trailing_ones");
}

TEST_CASE(stdbit, bit_queries_u64_constant) {
    typedef unsigned long long U64;
    test_eq( (int)STD::stdc_leading_zeros((U64)1 << 40), 23 );
    test_eq( (int)STD::stdc_leading_zeros((U64)1 << 63), 0 );
    test_eq( (int)STD::stdc_leading_zeros((U64)1), 63 );
    test_eq( (int)STD::stdc_leading_zeros((U64)0), 64 );
    test_eq( (int)STD::stdc_trailing_zeros((U64)1 << 40), 40 );
    test_eq( (int)STD::stdc_trailing_zeros((U64)0), 64 );
    test_eq( (int)STD::stdc_bit_width((U64)1 << 40), 41 );
    test_eq( (int)STD::stdc_count_ones((U64)0xF0F0F0F0F0F0F0F0ull), 32 );
    test_eq( (int)STD::stdc_count_zeros((U64)0xF0F0F0F0F0F0F0F0ull), 32 );
    test_true(  STD::stdc_has_single_bit((U64)1 << 40) );
    test_true( !STD::stdc_has_single_bit((U64)3) );
    test_true( STD::stdc_bit_floor((U64)5) == (U64)4 );
    test_true( STD::stdc_bit_ceil((U64)5) == (U64)8 );

    test_eq( (int)STD::stdc_leading_zeros_ull((U64)1 << 40), 23 );
    test_eq( (int)STD::stdc_count_ones_ull((U64)0xF0F0F0F0F0F0F0F0ull), 32 );
    test_pass("cxx26:64-bit constant-folded bit queries");
}
#else
TEST_CASE(stdbit, bit_queries) {
    TEST_NOTE("no std:: form of the C23 stdc_* functions here (the C header defines them as macros in the global scope)");
    test_skip("cxx26:stdc_count_ones");
    test_skip("cxx26:stdc_bit_width");
    test_skip("cxx26:stdc_bit_floor");
    test_skip("cxx26:stdc_bit_ceil");
    test_skip("cxx26:stdc_has_single_bit");
    test_skip("cxx26:stdc_leading_zeros_us");
    test_skip("cxx26:stdc_count_ones_ull");
}
TEST_CASE(stdbit, first_and_run_queries) {
    TEST_NOTE("no std:: form of the C23 stdc_* functions here (the C header defines them as macros in the global scope)");
    test_skip("cxx26:stdc_first_leading_one");
    test_skip("cxx26:stdc_first_leading_zero");
    test_skip("cxx26:stdc_first_trailing_one");
    test_skip("cxx26:stdc_first_trailing_zero");
    test_skip("cxx26:stdc_leading_ones");
    test_skip("cxx26:stdc_trailing_ones");
}
TEST_CASE(stdbit, bit_queries_u64_constant) {
    TEST_NOTE("no std:: form of the C23 stdc_* functions here (the C header defines them as macros in the global scope)");
    test_skip("cxx26:64-bit constant-folded bit queries");
}
#endif

TEST_CASE(source_location, current) {
    STD::source_location sl = STD::source_location::current();
#if defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER < 1926)
    test_eq( (int)sl.line(), 0 );
    test_eq( sl.file_name()[0], '\0' );
#else
    test_true( sl.line() > 0 );
    TEST_SKIP1();
#endif
    test_pass("cxx20:source_location::current()");
}
#endif

TEST_CASE(stdbit, feature_test_macros) {
#if defined(__cpp_lib_stdbit_h) && (__cpp_lib_stdbit_h) >= 202603L
    test_true( (__cpp_lib_stdbit_h) >= 202603L );
    test_pass("cxx26:__cpp_lib_stdbit_h=202603L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_stdbit_h=202603L");
#endif
}

#define _TST_SB_WIDTH(T) ((int)(sizeof(T) * CHAR_BIT))

#if defined(_CCW_HAS_STDBIT_VALUED_SUFFIXES)
#define _TST_SB_VALUED(suf, T)                                                 \
        test_true(  STD::stdc_has_single_bit_##suf((T)4) );                    \
        test_true( !STD::stdc_has_single_bit_##suf((T)3) );                    \
        test_true( !STD::stdc_has_single_bit_##suf((T)0) );                    \
        test_pass("cxx26:stdc_has_single_bit_" #suf);                          \
        test_eq( (long)STD::stdc_bit_floor_##suf((T)5), 4L );                  \
        test_eq( (long)STD::stdc_bit_floor_##suf((T)0), 0L );                  \
        test_pass("cxx26:stdc_bit_floor_" #suf);                               \
        test_eq( (long)STD::stdc_bit_ceil_##suf((T)5), 8L );                   \
        test_eq( (long)STD::stdc_bit_ceil_##suf((T)1), 1L );                   \
        test_pass("cxx26:stdc_bit_ceil_" #suf)
#else
#define _TST_SB_VALUED(suf, T)                                                 \
        TEST_NOTE("no per-width has_single_bit / bit_floor / bit_ceil here");   \
        test_skip("cxx26:stdc_has_single_bit_" #suf);                          \
        test_skip("cxx26:stdc_bit_floor_" #suf);                               \
        test_skip("cxx26:stdc_bit_ceil_" #suf)
#endif

#define _TST_SB_ALL(suf, T)                                                    \
    do {                                                                       \
        const int W = _TST_SB_WIDTH(T);                                        \
        const T all_ones = (T)~(T)0;                                           \
                                                                               \
        test_eq( (int)STD::stdc_leading_zeros_##suf((T)1), W - 1 );            \
        test_eq( (int)STD::stdc_leading_zeros_##suf((T)0), W );                \
        test_pass("cxx26:stdc_leading_zeros_" #suf);                           \
                                                                               \
        test_eq( (int)STD::stdc_leading_ones_##suf(all_ones), W );             \
        test_eq( (int)STD::stdc_leading_ones_##suf((T)0), 0 );                 \
        test_pass("cxx26:stdc_leading_ones_" #suf);                            \
                                                                               \
        test_eq( (int)STD::stdc_trailing_zeros_##suf((T)0), W );               \
        test_eq( (int)STD::stdc_trailing_zeros_##suf((T)4), 2 );               \
        test_pass("cxx26:stdc_trailing_zeros_" #suf);                          \
                                                                               \
        test_eq( (int)STD::stdc_trailing_ones_##suf((T)1), 1 );                \
        test_eq( (int)STD::stdc_trailing_ones_##suf((T)0), 0 );                \
        test_pass("cxx26:stdc_trailing_ones_" #suf);                           \
                                                                               \
        test_eq( (int)STD::stdc_first_leading_zero_##suf(all_ones), 0 );       \
        test_eq( (int)STD::stdc_first_leading_zero_##suf((T)0), 1 );           \
        test_pass("cxx26:stdc_first_leading_zero_" #suf);                      \
                                                                               \
        test_eq( (int)STD::stdc_first_leading_one_##suf((T)1), W );            \
        test_eq( (int)STD::stdc_first_leading_one_##suf((T)0), 0 );            \
        test_pass("cxx26:stdc_first_leading_one_" #suf);                       \
                                                                               \
        test_eq( (int)STD::stdc_first_trailing_zero_##suf((T)1), 2 );          \
        test_eq( (int)STD::stdc_first_trailing_zero_##suf(all_ones), 0 );      \
        test_pass("cxx26:stdc_first_trailing_zero_" #suf);                     \
                                                                               \
        test_eq( (int)STD::stdc_first_trailing_one_##suf((T)4), 3 );           \
        test_eq( (int)STD::stdc_first_trailing_one_##suf((T)0), 0 );           \
        test_pass("cxx26:stdc_first_trailing_one_" #suf);                      \
                                                                               \
        test_eq( (int)STD::stdc_count_ones_##suf((T)3), 2 );                   \
        test_eq( (int)STD::stdc_count_ones_##suf(all_ones), W );               \
        test_pass("cxx26:stdc_count_ones_" #suf);                              \
                                                                               \
        test_eq( (int)STD::stdc_count_zeros_##suf((T)3), W - 2 );              \
        test_eq( (int)STD::stdc_count_zeros_##suf(all_ones), 0 );              \
        test_pass("cxx26:stdc_count_zeros_" #suf);                             \
                                                                               \
        test_eq( (int)STD::stdc_bit_width_##suf((T)5), 3 );                    \
        test_eq( (int)STD::stdc_bit_width_##suf((T)0), 0 );                    \
        test_pass("cxx26:stdc_bit_width_" #suf);                               \
                                                                               \
        _TST_SB_VALUED(suf, T);                                                \
    } while (0)

#if defined(_CCW_STD_STDBIT_DEFINED)
TEST_CASE(stdbit, width_suffixed_uchar_ushort_cxx26) {
    _TST_SB_ALL(uc, unsigned char);
    _TST_SB_ALL(us, unsigned short);
}

TEST_CASE(stdbit, width_suffixed_uint_cxx26) {
    _TST_SB_ALL(ui, unsigned int);
}

TEST_CASE(stdbit, width_suffixed_ulong_cxx26) {
    _TST_SB_ALL(ul, unsigned long);
}

TEST_CASE(stdbit, width_suffixed_ulonglong_cxx26) {
    _TST_SB_ALL(ull, unsigned long long);
}

TEST_CASE(stdbit, generic_forms_cxx26) {
    test_eq( (int)STD::stdc_leading_zeros((unsigned char)1),
             _TST_SB_WIDTH(unsigned char) - 1 );
    test_eq( (int)STD::stdc_leading_zeros((unsigned)1),
             _TST_SB_WIDTH(unsigned) - 1 );
    test_pass("cxx26:stdc_leading_zeros");

    test_eq( (int)STD::stdc_trailing_zeros((unsigned char)0),
             _TST_SB_WIDTH(unsigned char) );
    test_eq( (int)STD::stdc_trailing_zeros((unsigned)0),
             _TST_SB_WIDTH(unsigned) );
    test_eq( (int)STD::stdc_trailing_zeros((unsigned)4), 2 );
    test_pass("cxx26:stdc_trailing_zeros");

    test_eq( (int)STD::stdc_count_zeros((unsigned char)3),
             _TST_SB_WIDTH(unsigned char) - 2 );
    test_eq( (int)STD::stdc_count_zeros((unsigned)3),
             _TST_SB_WIDTH(unsigned) - 2 );
    test_pass("cxx26:stdc_count_zeros");
}
#else
TEST_CASE(stdbit, width_suffixed_uchar_ushort_cxx26) {
    TEST_NOTE("no std:: form of the C23 stdc_* functions here (the C header defines them as macros in the global scope)");
    TEST_SKIP1();
}
TEST_CASE(stdbit, width_suffixed_uint_cxx26) {
    TEST_NOTE("no std:: form of the C23 stdc_* functions here (the C header defines them as macros in the global scope)");
    TEST_SKIP1();
}
TEST_CASE(stdbit, width_suffixed_ulong_cxx26) {
    TEST_NOTE("no std:: form of the C23 stdc_* functions here (the C header defines them as macros in the global scope)");
    TEST_SKIP1();
}
TEST_CASE(stdbit, width_suffixed_ulonglong_cxx26) {
    TEST_NOTE("no std:: form of the C23 stdc_* functions here (the C header defines them as macros in the global scope)");
    TEST_SKIP1();
}
TEST_CASE(stdbit, generic_forms_cxx26) {
    TEST_NOTE("no std:: form of the C23 stdc_* functions here (the C header defines them as macros in the global scope)");
    test_skip("cxx26:stdc_leading_zeros");
    test_skip("cxx26:stdc_trailing_zeros");
    test_skip("cxx26:stdc_count_zeros");
}
#endif

TEST_CASE(stdbit, endian_macros_cxx26) {
#if defined(__STDC_ENDIAN_BIG__) && defined(__STDC_ENDIAN_LITTLE__) && defined(__STDC_ENDIAN_NATIVE__)
    test_true( __STDC_ENDIAN_BIG__ != __STDC_ENDIAN_LITTLE__ );
    test_pass("cxx26:__STDC_ENDIAN_BIG__");
    test_true( __STDC_ENDIAN_LITTLE__ != __STDC_ENDIAN_BIG__ );
    test_pass("cxx26:__STDC_ENDIAN_LITTLE__");
    test_true( __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__
               || __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_BIG__ );
    {
        unsigned int probe = 1u;
        unsigned char first = *(unsigned char*)&probe;
        bool little = (first == 1);
        test_true( little == (__STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__) );
    }
    test_pass("cxx26:__STDC_ENDIAN_NATIVE__");
#else
    TEST_NOTE("the __STDC_ENDIAN_* macros are not defined here");
    test_skip("cxx26:__STDC_ENDIAN_BIG__");
    test_skip("cxx26:__STDC_ENDIAN_LITTLE__");
    test_skip("cxx26:__STDC_ENDIAN_NATIVE__");
#endif

#if defined(__STDC_VERSION_STDBIT_H__)
    test_true( __STDC_VERSION_STDBIT_H__ >= 202311L );
    test_pass("cxx26:__STDC_VERSION_STDBIT_H__");
#else
    TEST_NOTE("__STDC_VERSION_STDBIT_H__ is not defined here");
    test_skip("cxx26:__STDC_VERSION_STDBIT_H__");
#endif
}
