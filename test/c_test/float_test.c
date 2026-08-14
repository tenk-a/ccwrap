#include "c_test.h"

#include <float.h>
#include <math.h>

TEST_CASE(float, radix_and_mant_dig) {
    test_eq( FLT_RADIX, 2 );
    test_pass("c1990:FLT_RADIX");

    test_ge( FLT_MANT_DIG, 24 );
    test_le( FLT_MANT_DIG, DBL_MANT_DIG );
    test_pass("c1990:FLT_MANT_DIG");

    test_ge( DBL_MANT_DIG, 53 );
    test_le( DBL_MANT_DIG, LDBL_MANT_DIG );
    test_pass("c1990:DBL_MANT_DIG");

    test_gt( LDBL_MANT_DIG, 0 );
    test_pass("c1990:LDBL_MANT_DIG");
}

TEST_CASE(float, decimal_digits) {

    test_ge( FLT_DIG, 6 );
    test_le( FLT_DIG, DBL_DIG );
    test_pass("c1990:FLT_DIG");

    test_ge( DBL_DIG, 10 );
    test_le( DBL_DIG, LDBL_DIG );
    test_pass("c1990:DBL_DIG");

    test_ge( LDBL_DIG, 10 );
    test_pass("c1990:LDBL_DIG");
}

TEST_CASE(float, exponent_ranges) {
    test_lt( FLT_MIN_EXP, 0 );
    test_lt( FLT_MIN_EXP, FLT_MAX_EXP );
    test_pass("c1990:FLT_MIN_EXP");
    test_lt( DBL_MIN_EXP, 0 );
    test_lt( DBL_MIN_EXP, DBL_MAX_EXP );
    test_pass("c1990:DBL_MIN_EXP");
    test_lt( LDBL_MIN_EXP, 0 );
    test_pass("c1990:LDBL_MIN_EXP");

    test_gt( FLT_MAX_EXP, 0 );
    test_pass("c1990:FLT_MAX_EXP");
    test_gt( DBL_MAX_EXP, 0 );
    test_pass("c1990:DBL_MAX_EXP");
    test_gt( LDBL_MAX_EXP, 0 );
    test_pass("c1990:LDBL_MAX_EXP");

    test_le( FLT_MIN_10_EXP, -37 );
    test_lt( FLT_MIN_10_EXP, FLT_MAX_10_EXP );
    test_pass("c1990:FLT_MIN_10_EXP");
    test_le( DBL_MIN_10_EXP, -37 );
    test_lt( DBL_MIN_10_EXP, DBL_MAX_10_EXP );
    test_pass("c1990:DBL_MIN_10_EXP");
    test_le( LDBL_MIN_10_EXP, -37 );
    test_pass("c1990:LDBL_MIN_10_EXP");

    test_ge( FLT_MAX_10_EXP, 37 );
    test_pass("c1990:FLT_MAX_10_EXP");
    test_ge( DBL_MAX_10_EXP, 37 );
    test_pass("c1990:DBL_MAX_10_EXP");
    test_ge( LDBL_MAX_10_EXP, 37 );
    test_pass("c1990:LDBL_MAX_10_EXP");
}

TEST_CASE(float, max_values) {

    test_ge( FLT_MAX, 1e37f );
    test_true( FLT_MAX <= (float)DBL_MAX );
    test_pass("c1990:FLT_MAX");
    test_ge( DBL_MAX, 1e37 );
    test_true( (double)DBL_MAX <= (double)LDBL_MAX );
    test_pass("c1990:DBL_MAX");
    test_ge( LDBL_MAX, 1e37L );
    test_pass("c1990:LDBL_MAX");
}

TEST_CASE(float, min_values) {

    test_gt( FLT_MIN, 0.0f );
    test_le( FLT_MIN, 1e-37f );
    test_lt( FLT_MIN, FLT_MAX );
    test_pass("c1990:FLT_MIN");
    test_gt( DBL_MIN, 0.0 );
    test_le( DBL_MIN, 1e-37 );
    test_pass("c1990:DBL_MIN");
    test_gt( LDBL_MIN, 0.0L );
    test_le( LDBL_MIN, 1e-37L );
    test_pass("c1990:LDBL_MIN");
}

TEST_CASE(float, epsilon) {
    volatile float  f1;
    volatile double d1;

    test_gt( FLT_EPSILON, 0.0f );
    test_le( FLT_EPSILON, 1e-5f );
    test_lt( FLT_EPSILON, 1.0f );
    f1 = 1.0f;
    test_gt( f1 + FLT_EPSILON, 1.0f );
    TEST_SKIP_WAT("1+eps/2 evaluated in 80-bit x87, cast does not round back (FLT_EVAL_METHOD=2)");
    test_eq( (float)(f1 + FLT_EPSILON / 2), 1.0f );
    test_pass("c1990:FLT_EPSILON");

    test_gt( DBL_EPSILON, 0.0 );
    test_le( DBL_EPSILON, 1e-9 );
    d1 = 1.0;
    test_gt( d1 + DBL_EPSILON, 1.0 );
    TEST_SKIP_WAT("1+eps/2 evaluated in 80-bit x87, cast does not round back (FLT_EVAL_METHOD=2)");
    test_eq( (double)(d1 + DBL_EPSILON / 2), 1.0 );
    test_pass("c1990:DBL_EPSILON");

    test_gt( LDBL_EPSILON, 0.0L );
    test_pass("c1990:LDBL_EPSILON");
}

TEST_CASE(float, rounding) {
    int r = FLT_ROUNDS;
    test_lim( r, -1, 3 );
    test_pass("c1990:FLT_ROUNDS");
}

TEST_CASE(float, decimal_dig_wide) {
#ifdef DECIMAL_DIG
    test_ge( DECIMAL_DIG, 10 );
    test_pass("c1999:DECIMAL_DIG");
#else
    TEST_SKIP1(); TEST_NOTE("DECIMAL_DIG (C99) not defined");
    test_skip("c1999:DECIMAL_DIG");
#endif

#ifdef FLT_DECIMAL_DIG
    test_gt( FLT_DECIMAL_DIG, FLT_DIG );
    test_ge( FLT_DECIMAL_DIG, 6 );
    test_pass("c2011:FLT_DECIMAL_DIG");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("C11 FLT_DECIMAL_DIG not defined");
    test_skip("c2011:FLT_DECIMAL_DIG");
#endif

#ifdef DBL_DECIMAL_DIG
    test_ge( DBL_DECIMAL_DIG, 10 );
    test_ge( DECIMAL_DIG, DBL_DECIMAL_DIG );
    test_pass("c2011:DBL_DECIMAL_DIG");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("C11 DBL_DECIMAL_DIG not defined");
    test_skip("c2011:DBL_DECIMAL_DIG");
#endif

#ifdef LDBL_DECIMAL_DIG
    test_ge( LDBL_DECIMAL_DIG, DBL_DECIMAL_DIG );
    test_pass("c2011:LDBL_DECIMAL_DIG");
#else
    TEST_SKIP1(); TEST_NOTE("C11 LDBL_DECIMAL_DIG not defined");
    test_skip("c2011:LDBL_DECIMAL_DIG");
#endif
}

TEST_CASE(float, eval_method) {
#ifdef FLT_EVAL_METHOD
    int m = FLT_EVAL_METHOD;
    test_ge( m, -1 );
    test_le( m, 2 );
    test_pass("c1999:FLT_EVAL_METHOD");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("FLT_EVAL_METHOD not defined");
    test_skip("c1999:FLT_EVAL_METHOD");
#endif
}

TEST_CASE(float, subnormals) {
#ifdef FLT_TRUE_MIN
    test_gt( FLT_TRUE_MIN, 0.0f );
    test_le( FLT_TRUE_MIN, FLT_MIN );
    test_pass("c2011:FLT_TRUE_MIN");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("C11 FLT_TRUE_MIN not defined");
    test_skip("c2011:FLT_TRUE_MIN");
#endif
#ifdef DBL_TRUE_MIN
    test_gt( DBL_TRUE_MIN, 0.0 );
    test_le( DBL_TRUE_MIN, DBL_MIN );
    test_pass("c2011:DBL_TRUE_MIN");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("C11 DBL_TRUE_MIN not defined");
    test_skip("c2011:DBL_TRUE_MIN");
#endif
#ifdef LDBL_TRUE_MIN
    test_gt( LDBL_TRUE_MIN, 0.0L );
    test_le( LDBL_TRUE_MIN, LDBL_MIN );
    test_pass("c2011:LDBL_TRUE_MIN");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("C11 LDBL_TRUE_MIN not defined");
    test_skip("c2011:LDBL_TRUE_MIN");
#endif

#ifdef FLT_HAS_SUBNORM
    test_lim( FLT_HAS_SUBNORM, -1, 1 );
    test_true( FLT_HAS_SUBNORM != 1 || FLT_TRUE_MIN < FLT_MIN );
    test_pass("c2011:FLT_HAS_SUBNORM");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_NOTE("C11 FLT_HAS_SUBNORM not defined");
    test_skip("c2011:FLT_HAS_SUBNORM");
#endif
#ifdef DBL_HAS_SUBNORM
    test_lim( DBL_HAS_SUBNORM, -1, 1 );
    test_pass("c2011:DBL_HAS_SUBNORM");
#else
    TEST_SKIP1(); TEST_NOTE("C11 DBL_HAS_SUBNORM not defined");
    test_skip("c2011:DBL_HAS_SUBNORM");
#endif
#ifdef LDBL_HAS_SUBNORM
    test_lim( LDBL_HAS_SUBNORM, -1, 1 );
    test_pass("c2011:LDBL_HAS_SUBNORM");
#else
    TEST_SKIP1(); TEST_NOTE("C11 LDBL_HAS_SUBNORM not defined");
    test_skip("c2011:LDBL_HAS_SUBNORM");
#endif
}

TEST_CASE(float, norm_max_c23) {
#ifdef FLT_NORM_MAX
    test_true( FLT_NORM_MAX == FLT_MAX );
    test_pass("c2023:FLT_NORM_MAX");
#else
    TEST_SKIP1(); TEST_NOTE("C23 FLT_NORM_MAX not defined");
    test_skip("c2023:FLT_NORM_MAX");
#endif
#ifdef DBL_NORM_MAX
    test_true( DBL_NORM_MAX == DBL_MAX );
    test_pass("c2023:DBL_NORM_MAX");
#else
    TEST_SKIP1(); TEST_NOTE("C23 DBL_NORM_MAX not defined");
    test_skip("c2023:DBL_NORM_MAX");
#endif
#ifdef LDBL_NORM_MAX
    test_true( (double)LDBL_NORM_MAX == (double)LDBL_MAX );
    test_pass("c2023:LDBL_NORM_MAX");
#else
    TEST_SKIP1(); TEST_NOTE("C23 LDBL_NORM_MAX not defined");
    test_skip("c2023:LDBL_NORM_MAX");
#endif
}

TEST_CASE(float, snan_c23) {
#ifdef FLT_SNAN
    {   volatile float sf = FLT_SNAN;
        test_true( isnan((double)sf) );
        test_true( !((double)sf == (double)sf) ); }
    test_pass("c2023:FLT_SNAN");
#else
    TEST_SKIP_N(2); TEST_NOTE("C23 FLT_SNAN needs a constant signaling-NaN, unspellable here (A17)");
    test_skip("c2023:FLT_SNAN");
#endif
#ifdef DBL_SNAN
    {   volatile double sd = DBL_SNAN;
        test_true( isnan((double)sd) );
        test_true( !((double)sd == (double)sd) ); }
    test_pass("c2023:DBL_SNAN");
#else
    TEST_SKIP_N(2); TEST_NOTE("C23 DBL_SNAN unspellable here (A17)");
    test_skip("c2023:DBL_SNAN");
#endif
#ifdef LDBL_SNAN
    {   volatile long double sl = LDBL_SNAN;
        test_true( isnan((double)sl) );
        test_true( !((double)sl == (double)sl) ); }
    test_pass("c2023:LDBL_SNAN");
#else
    TEST_SKIP_N(2); TEST_NOTE("C23 LDBL_SNAN unspellable here (A17)");
    test_skip("c2023:LDBL_SNAN");
#endif
}

TEST_CASE(float, iec_60559_c23) {
#ifdef FLT_IS_IEC_60559
    test_lim( FLT_IS_IEC_60559, 0, 1 );
    test_pass("c2023:FLT_IS_IEC_60559");
#else
    TEST_SKIP1(); TEST_NOTE("C23 FLT_IS_IEC_60559 not defined");
    test_skip("c2023:FLT_IS_IEC_60559");
#endif
#ifdef DBL_IS_IEC_60559
    test_lim( DBL_IS_IEC_60559, 0, 1 );
    test_pass("c2023:DBL_IS_IEC_60559");
#else
    TEST_SKIP1(); TEST_NOTE("C23 DBL_IS_IEC_60559 not defined");
    test_skip("c2023:DBL_IS_IEC_60559");
#endif
#ifdef LDBL_IS_IEC_60559
    test_lim( LDBL_IS_IEC_60559, 0, 1 );
    test_pass("c2023:LDBL_IS_IEC_60559");
#else
    TEST_SKIP1(); TEST_NOTE("C23 LDBL_IS_IEC_60559 not defined");
    test_skip("c2023:LDBL_IS_IEC_60559");
#endif

}

TEST_CASE(float, stdc_version_c23) {
#ifdef __STDC_VERSION_FLOAT_H__
    test_ge( __STDC_VERSION_FLOAT_H__, 202311L );
    test_pass("c2023:__STDC_VERSION_FLOAT_H__");
#else
    TEST_SKIP1(); TEST_NOTE("__STDC_VERSION_FLOAT_H__ not defined");
    test_skip("c2023:__STDC_VERSION_FLOAT_H__");
#endif
}


TEST_CASE(float, c23_infinity_nan) {
#if defined(INFINITY)
    {
        float inf = INFINITY;
        test_true( inf > 0.0f );
        test_true( inf > FLT_MAX );
        test_true( inf == inf * 2.0f );
    }
    test_pass("c2023:INFINITY");
#else
    TEST_NOTE("INFINITY reaches <float.h> only in C23");
    test_skip("c2023:INFINITY");
#endif
#if defined(NAN)
    {
        float n = NAN;
        test_true( isnan(n) );
        test_true( !isgreaterequal(n, 0.0f) && !isless(n, 0.0f) );
    }
    test_pass("c2023:NAN");
#else
    TEST_NOTE("NAN reaches <float.h> only in C23");
    test_skip("c2023:NAN");
#endif

#if defined(CR_DECIMAL_DIG)
    test_true( CR_DECIMAL_DIG >= DECIMAL_DIG );
    test_pass("c2023:opt:CR_DECIMAL_DIG");
#else
    TEST_NOTE("CR_DECIMAL_DIG is defined only where correctly-rounded conversion is");
    test_skip("c2023:opt:CR_DECIMAL_DIG");
#endif
}

TEST_CASE(float, c23_decimal_limits) {
#if defined(DEC32_MANT_DIG)
    test_eq( (int)DEC32_MANT_DIG, 7 );
    test_eq( (int)DEC64_MANT_DIG, 16 );
    test_eq( (int)DEC128_MANT_DIG, 34 );
    test_pass("c2023:opt:DEC32_MANT_DIG");
    test_pass("c2023:opt:DEC64_MANT_DIG");
    test_pass("c2023:opt:DEC128_MANT_DIG");

    test_true( DEC32_MAX_EXP <= DEC64_MAX_EXP );
    test_true( DEC64_MAX_EXP <= DEC128_MAX_EXP );
    test_pass("c2023:opt:DEC32_MAX_EXP");
    test_pass("c2023:opt:DEC64_MAX_EXP");
    test_pass("c2023:opt:DEC128_MAX_EXP");
    test_true( DEC32_MIN_EXP >= DEC64_MIN_EXP );
    test_true( DEC64_MIN_EXP >= DEC128_MIN_EXP );
    test_pass("c2023:opt:DEC32_MIN_EXP");
    test_pass("c2023:opt:DEC64_MIN_EXP");
    test_pass("c2023:opt:DEC128_MIN_EXP");

    test_true( DEC32_MAX > 0 );
    test_true( DEC64_MAX > 0 );
    test_true( DEC128_MAX > 0 );
    test_pass("c2023:opt:DEC32_MAX");
    test_pass("c2023:opt:DEC64_MAX");
    test_pass("c2023:opt:DEC128_MAX");
    test_true( DEC32_MIN > 0 );
    test_true( DEC64_MIN > 0 );
    test_true( DEC128_MIN > 0 );
    test_pass("c2023:opt:DEC32_MIN");
    test_pass("c2023:opt:DEC64_MIN");
    test_pass("c2023:opt:DEC128_MIN");
    test_true( DEC32_TRUE_MIN > 0 );
    test_true( DEC64_TRUE_MIN > 0 );
    test_true( DEC128_TRUE_MIN > 0 );
    test_pass("c2023:opt:DEC32_TRUE_MIN");
    test_pass("c2023:opt:DEC64_TRUE_MIN");
    test_pass("c2023:opt:DEC128_TRUE_MIN");
    test_true( DEC32_EPSILON > 0 );
    test_true( DEC64_EPSILON > 0 );
    test_true( DEC128_EPSILON > 0 );
    test_pass("c2023:opt:DEC32_EPSILON");
    test_pass("c2023:opt:DEC64_EPSILON");
    test_pass("c2023:opt:DEC128_EPSILON");
    test_pass("c2023:opt:DEC32_SNAN");
    test_pass("c2023:opt:DEC64_SNAN");
    test_pass("c2023:opt:DEC128_SNAN");
    test_pass("c2023:opt:DEC_INFINITY");
    test_pass("c2023:opt:DEC_NAN");
    test_pass("c2023:opt:DEC_EVAL_METHOD");
#else
    TEST_NOTE("decimal floating point (__STDC_IEC_60559_DFP__) is absent here");
    test_skip("c2023:opt:DEC32_MANT_DIG");  test_skip("c2023:opt:DEC64_MANT_DIG");
    test_skip("c2023:opt:DEC128_MANT_DIG");
    test_skip("c2023:opt:DEC32_MAX_EXP");   test_skip("c2023:opt:DEC64_MAX_EXP");
    test_skip("c2023:opt:DEC128_MAX_EXP");
    test_skip("c2023:opt:DEC32_MIN_EXP");   test_skip("c2023:opt:DEC64_MIN_EXP");
    test_skip("c2023:opt:DEC128_MIN_EXP");
    test_skip("c2023:opt:DEC32_MAX");       test_skip("c2023:opt:DEC64_MAX");
    test_skip("c2023:opt:DEC128_MAX");
    test_skip("c2023:opt:DEC32_MIN");       test_skip("c2023:opt:DEC64_MIN");
    test_skip("c2023:opt:DEC128_MIN");
    test_skip("c2023:opt:DEC32_TRUE_MIN");  test_skip("c2023:opt:DEC64_TRUE_MIN");
    test_skip("c2023:opt:DEC128_TRUE_MIN");
    test_skip("c2023:opt:DEC32_EPSILON");   test_skip("c2023:opt:DEC64_EPSILON");
    test_skip("c2023:opt:DEC128_EPSILON");
    test_skip("c2023:opt:DEC32_SNAN");      test_skip("c2023:opt:DEC64_SNAN");
    test_skip("c2023:opt:DEC128_SNAN");
    test_skip("c2023:opt:DEC_INFINITY");    test_skip("c2023:opt:DEC_NAN");
    test_skip("c2023:opt:DEC_EVAL_METHOD");
#endif
}

TEST_SUITE(float)
    TEST_ADD(radix_and_mant_dig)
    TEST_ADD(decimal_digits)
    TEST_ADD(exponent_ranges)
    TEST_ADD(max_values)
    TEST_ADD(min_values)
    TEST_ADD(epsilon)
    TEST_ADD(rounding)
    TEST_ADD(decimal_dig_wide)
    TEST_ADD(eval_method)
    TEST_ADD(subnormals)
    TEST_ADD(norm_max_c23)
    TEST_ADD(snan_c23)
    TEST_ADD(iec_60559_c23)
    TEST_ADD(stdc_version_c23)
    TEST_ADD(c23_infinity_nan)
    TEST_ADD(c23_decimal_limits)
TEST_SUITE_END(float)
