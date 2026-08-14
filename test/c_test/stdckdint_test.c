#include "c_test.h"

#include <stdckdint.h>
#include <limits.h>

TEST_CASE(stdckdint, version_macro) {
#ifdef __STDC_VERSION_STDCKDINT_H__
    test_true( __STDC_VERSION_STDCKDINT_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_STDCKDINT_H__");
#else
    TEST_SKIP1(); TEST_NOTE("__STDC_VERSION_STDCKDINT_H__ not defined");
    test_skip("c2023:__STDC_VERSION_STDCKDINT_H__");
#endif
}

TEST_CASE(stdckdint, add_no_overflow) {
    int r = 0;
    long lr = 0;
    signed char cr = 0;
    short sr = 0;

    test_true( !ckd_add(&r, 2, 3) );
    test_eq( r, 5 );
    test_true( !ckd_add(&r, -5, 3) );
    test_eq( r, -2 );
    test_true( !ckd_add(&r, 0, 0) );
    test_eq( r, 0 );

    test_true( !ckd_add(&lr, 100000L, 200000L) );
    test_eq( lr, 300000L );

    test_true( !ckd_add(&cr, 100, 27) );
    test_eq( (int)cr, 127 );
    test_true( !ckd_add(&sr, 32000, 767) );
    test_eq( (int)sr, 32767 );
    test_pass("c2023:ckd_add(result,a,b) no-overflow");
}

TEST_CASE(stdckdint, add_overflow_is_reported) {
    int r = 0;
    signed char cr = 0;
    short sr = 0;

    test_true( ckd_add(&cr, 127, 1) );
    test_eq( (int)cr, -128 );
    test_true( ckd_add(&cr, -128, -1) );
    test_eq( (int)cr, 127 );

    test_true( ckd_add(&sr, 32767, 1) );
    test_eq( (int)sr, -32768 );

    test_true( ckd_add(&r, INT_MAX, 1) );
    test_eq( r, INT_MIN );
    test_true( ckd_add(&r, INT_MIN, -1) );
    test_eq( r, INT_MAX );

    test_true( !ckd_add(&r, INT_MAX, 0) );
    test_eq( r, INT_MAX );
    test_true( !ckd_add(&r, INT_MAX - 1, 1) );
    test_eq( r, INT_MAX );
    test_pass("c2023:ckd_add(result,a,b) overflow-reported");
}

TEST_CASE(stdckdint, sub) {
    int r = 0;
    signed char cr = 0;

    test_true( !ckd_sub(&r, 10, 4) );
    test_eq( r, 6 );
    test_true( !ckd_sub(&r, 4, 10) );
    test_eq( r, -6 );

    test_true( ckd_sub(&cr, -128, 1) );
    test_eq( (int)cr, 127 );
    test_true( ckd_sub(&cr, 127, -1) );
    test_eq( (int)cr, -128 );
    test_true( !ckd_sub(&cr, 127, 1) );
    test_eq( (int)cr, 126 );

    test_true( ckd_sub(&r, INT_MIN, 1) );
    test_true( !ckd_sub(&r, INT_MIN, 0) );
    test_eq( r, INT_MIN );
    test_pass("c2023:ckd_sub(result,a,b)");
}

TEST_CASE(stdckdint, mul) {
    int r = 0;
    signed char cr = 0;
    long long llr = 0;

    test_true( !ckd_mul(&r, 6, 7) );
    test_eq( r, 42 );
    test_true( !ckd_mul(&r, -6, 7) );
    test_eq( r, -42 );
    test_true( !ckd_mul(&r, 0, INT_MAX) );
    test_eq( r, 0 );

    test_true( ckd_mul(&r, 100000, 100000) );
    test_true( ckd_mul(&cr, 100, 2) );
    test_true( !ckd_mul(&cr, 63, 2) );
    test_eq( (int)cr, 126 );

    test_true( !ckd_mul(&llr, 100000LL, 100000LL) );
    test_eq( llr, 10000000000LL );
#if TEST_TARGET_C >= 1999
    test_true( ckd_mul(&llr, LLONG_MAX, 2LL) );
    test_pass("c2023:ckd_mul(result,a,b)");
#else
    TEST_SKIP1();
    test_skip("c2023:ckd_mul(result,a,b)");
#endif
}

TEST_CASE(stdckdint, mixed_operand_widths) {
    int r = 0;
    long lr = 0;

    test_true( !ckd_add(&r, (signed char)100, (signed char)100) );
    test_eq( r, 200 );
    test_true( !ckd_mul(&lr, (short)1000, (short)1000) );
    test_eq( lr, 1000000L );

    {
        signed char cr = 0;
        test_true( ckd_add(&cr, (signed char)100, (signed char)100) );
    }
    test_pass("c2023:mixed operand width promotion");
}

TEST_CASE(stdckdint, unsigned_result_type_subset) {
    unsigned int u = 0;
    unsigned char uc = 0;

    test_true( !ckd_add(&u, 1, 2) );
    test_eq( (long)u, 3L );
    test_true( !ckd_mul(&u, 3, 4) );
    test_eq( (long)u, 12L );

    TEST_SKIP_WAT("unsigned result range-checked as signed of its width (detail/stdckdint_c.h subset)");
    TEST_SKIP_VC("unsigned result range-checked as signed of its width (detail/stdckdint_c.h subset)");
    test_true( !ckd_add(&uc, 100, 100) );
    test_eq( (int)uc, 200 );

    TEST_SKIP_WAT("unsigned result range-checked as signed of its width (detail/stdckdint_c.h subset)");
    TEST_SKIP_VC("unsigned result range-checked as signed of its width (detail/stdckdint_c.h subset)");
    test_true( !ckd_add(&u, INT_MAX, 1) );
    test_eq( u, 2147483648u );
    test_pass("c2023:ckd_add unsigned-result type (subset)");
}

TEST_SUITE(stdckdint)
    TEST_ADD(version_macro)
    TEST_ADD(add_no_overflow)
    TEST_ADD(add_overflow_is_reported)
    TEST_ADD(sub)
    TEST_ADD(mul)
    TEST_ADD(mixed_operand_widths)
    TEST_ADD(unsigned_result_type_subset)
TEST_SUITE_END(stdckdint)
