#include "test_cxx.hpp"
#include <cfloat>

TEST_CASE(cfloat, header_compiles) { test_true( true ); test_pass("cxx03:<cfloat> header compiles"); }

#if TEST_TARGET_CXX >= 2011
#include <cfloat>
#include <limits>
#include <cmath>

TEST_CASE(cfloat, radix_and_mant_dig) {

    test_ge( FLT_RADIX, 2 );
    test_eq( FLT_RADIX, STD::numeric_limits<float>::radix );
    test_eq( FLT_RADIX, STD::numeric_limits<double>::radix );
    test_eq( FLT_RADIX, STD::numeric_limits<long double>::radix );
    test_pass("cxx03:FLT_RADIX");

    test_ge( FLT_MANT_DIG,  6 );
    test_eq( FLT_MANT_DIG,  STD::numeric_limits<float>::digits );
    test_pass("cxx03:FLT_MANT_DIG");

    test_eq( DBL_MANT_DIG,  STD::numeric_limits<double>::digits );
    test_le( FLT_MANT_DIG, DBL_MANT_DIG );
    test_pass("cxx03:DBL_MANT_DIG");

    test_eq( LDBL_MANT_DIG, STD::numeric_limits<long double>::digits );
    test_le( DBL_MANT_DIG, LDBL_MANT_DIG );
    test_pass("cxx03:LDBL_MANT_DIG");
}

TEST_CASE(cfloat, decimal_digits) {

    test_ge( FLT_DIG,   6 );
    test_eq( FLT_DIG,  STD::numeric_limits<float>::digits10 );
    test_pass("cxx03:FLT_DIG");

    test_ge( DBL_DIG,  10 );
    test_eq( DBL_DIG,  STD::numeric_limits<double>::digits10 );
    test_le( FLT_DIG, DBL_DIG );
    test_pass("cxx03:DBL_DIG");

    test_ge( LDBL_DIG, 10 );
    test_eq( LDBL_DIG, STD::numeric_limits<long double>::digits10 );
    test_le( DBL_DIG, LDBL_DIG );
    test_pass("cxx03:LDBL_DIG");

#if TEST_TARGET_CXX >= 2017
    test_gt( FLT_DECIMAL_DIG,  FLT_DIG );
    test_ge( FLT_DECIMAL_DIG,  6 );
# if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( FLT_DECIMAL_DIG,  STD::numeric_limits<float>::max_digits10 );
# endif
    test_pass("cxx17:FLT_DECIMAL_DIG");
#else
    TEST_NOTE("the *_DECIMAL_DIG macros arrived with C11 / C++17");
    test_skip("cxx17:FLT_DECIMAL_DIG");
#endif

#if TEST_TARGET_CXX >= 2017
    test_ge( DBL_DECIMAL_DIG, 10 );
# if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( DBL_DECIMAL_DIG,  STD::numeric_limits<double>::max_digits10 );
# endif
    test_pass("cxx17:DBL_DECIMAL_DIG");
#else
    test_skip("cxx17:DBL_DECIMAL_DIG");
#endif

#ifdef LDBL_DECIMAL_DIG
    test_ge( LDBL_DECIMAL_DIG, 10 );
    test_eq( DECIMAL_DIG, LDBL_DECIMAL_DIG );
# if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( LDBL_DECIMAL_DIG, STD::numeric_limits<long double>::max_digits10 );
# endif
    test_pass("cxx17:LDBL_DECIMAL_DIG");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx17:LDBL_DECIMAL_DIG");
#endif

#if TEST_TARGET_CXX >= 2017
    test_ge( DECIMAL_DIG, DBL_DECIMAL_DIG );
#else
    test_gt( DECIMAL_DIG, 0 );
#endif
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( DECIMAL_DIG, STD::numeric_limits<long double>::max_digits10 );
#endif
    test_pass("cxx11:DECIMAL_DIG");
}

TEST_CASE(cfloat, exponent_ranges) {

    test_lt( FLT_MIN_EXP,  0 );
    test_eq( FLT_MIN_EXP,  STD::numeric_limits<float>::min_exponent );
    test_pass("cxx03:FLT_MIN_EXP");

    test_lt( DBL_MIN_EXP,  0 );
    test_eq( DBL_MIN_EXP,  STD::numeric_limits<double>::min_exponent );
    test_pass("cxx03:DBL_MIN_EXP");

    test_lt( LDBL_MIN_EXP, 0 );
    test_eq( LDBL_MIN_EXP, STD::numeric_limits<long double>::min_exponent );
    test_pass("cxx03:LDBL_MIN_EXP");

    test_gt( FLT_MAX_EXP,  0 );
    test_eq( FLT_MAX_EXP,  STD::numeric_limits<float>::max_exponent );
    test_pass("cxx03:FLT_MAX_EXP");

    test_gt( DBL_MAX_EXP,  0 );
    test_eq( DBL_MAX_EXP,  STD::numeric_limits<double>::max_exponent );
    test_pass("cxx03:DBL_MAX_EXP");

    test_gt( LDBL_MAX_EXP, 0 );
    test_eq( LDBL_MAX_EXP, STD::numeric_limits<long double>::max_exponent );
    test_pass("cxx03:LDBL_MAX_EXP");

    test_le( FLT_MIN_10_EXP,  -37 );
    test_eq( FLT_MIN_10_EXP,  STD::numeric_limits<float>::min_exponent10 );
    test_pass("cxx03:FLT_MIN_10_EXP");

    test_le( DBL_MIN_10_EXP,  -37 );
    test_eq( DBL_MIN_10_EXP,  STD::numeric_limits<double>::min_exponent10 );
    test_pass("cxx03:DBL_MIN_10_EXP");

    test_le( LDBL_MIN_10_EXP, -37 );
    test_eq( LDBL_MIN_10_EXP, STD::numeric_limits<long double>::min_exponent10 );
    test_pass("cxx03:LDBL_MIN_10_EXP");

    test_ge( FLT_MAX_10_EXP,  37 );
    test_eq( FLT_MAX_10_EXP,  STD::numeric_limits<float>::max_exponent10 );
    test_lt( FLT_MIN_10_EXP, FLT_MAX_10_EXP );
    test_pass("cxx03:FLT_MAX_10_EXP");

    test_ge( DBL_MAX_10_EXP,  37 );
    test_eq( DBL_MAX_10_EXP,  STD::numeric_limits<double>::max_exponent10 );
    test_lt( DBL_MIN_10_EXP, DBL_MAX_10_EXP );
    test_pass("cxx03:DBL_MAX_10_EXP");

    test_ge( LDBL_MAX_10_EXP, 37 );
    test_eq( LDBL_MAX_10_EXP, STD::numeric_limits<long double>::max_exponent10 );
    test_pass("cxx03:LDBL_MAX_10_EXP");
}

#if defined(__WATCOMC__)
TEST_CASE_SKIP(cfloat, max_min_epsilon)
#else
TEST_CASE(cfloat, max_min_epsilon) {

    test_eq( FLT_MAX,  STD::numeric_limits<float>::max() );
    test_ge( FLT_MAX,  1e37f );
    test_pass("cxx03:FLT_MAX");

    test_eq( DBL_MAX,  STD::numeric_limits<double>::max() );
    test_ge( DBL_MAX,  1e37 );
    test_true( FLT_MAX <= (float)DBL_MAX );
    test_pass("cxx03:DBL_MAX");

    test_eq( LDBL_MAX, STD::numeric_limits<long double>::max() );
    test_ge( LDBL_MAX, 1e37L );
    test_pass("cxx03:LDBL_MAX");

    test_eq( FLT_MIN,  STD::numeric_limits<float>::min() );
    test_gt( FLT_MIN,  0.0f );
    test_le( FLT_MIN,  1e-37f );
    test_lt( FLT_MIN, FLT_MAX );
    test_pass("cxx03:FLT_MIN");

    test_eq( DBL_MIN,  STD::numeric_limits<double>::min() );
    test_le( DBL_MIN,  1e-37 );
    test_pass("cxx03:DBL_MIN");

    test_eq( LDBL_MIN, STD::numeric_limits<long double>::min() );
    test_le( LDBL_MIN, 1e-37L );
    test_pass("cxx03:LDBL_MIN");

    test_eq( FLT_EPSILON,  STD::numeric_limits<float>::epsilon() );
    test_gt( FLT_EPSILON, 0.0f );
    test_le( FLT_EPSILON, 1e-5f );

    volatile float f1 = 1.0f;
    test_gt( f1 + FLT_EPSILON, 1.0f );
    test_eq( (float)(f1 + FLT_EPSILON / 2), 1.0f );
    test_lt( FLT_EPSILON, 1.0f );
    test_pass("cxx03:FLT_EPSILON");

    test_eq( DBL_EPSILON,  STD::numeric_limits<double>::epsilon() );
    test_le( DBL_EPSILON, 1e-9 );
    volatile double d1 = 1.0;
    test_gt( d1 + DBL_EPSILON, 1.0 );
    test_eq( (double)(d1 + DBL_EPSILON / 2), 1.0 );
    test_lt( DBL_EPSILON, 1.0 );
    test_pass("cxx03:DBL_EPSILON");

    test_eq( LDBL_EPSILON, STD::numeric_limits<long double>::epsilon() );
    test_le( LDBL_EPSILON, 1e-9L );
    test_pass("cxx03:LDBL_EPSILON");
}

#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(cfloat, subnormals) {

    test_eq( FLT_TRUE_MIN,  STD::numeric_limits<float>::denorm_min() );
    test_gt( FLT_TRUE_MIN,  0.0f );
    test_le( FLT_TRUE_MIN,  FLT_MIN );
    test_pass("cxx17:FLT_TRUE_MIN");

    test_eq( DBL_TRUE_MIN,  STD::numeric_limits<double>::denorm_min() );
    test_gt( DBL_TRUE_MIN,  0.0 );
    test_le( DBL_TRUE_MIN,  DBL_MIN );
    test_pass("cxx17:DBL_TRUE_MIN");

    test_eq( LDBL_TRUE_MIN, STD::numeric_limits<long double>::denorm_min() );
    test_gt( LDBL_TRUE_MIN, 0.0L );
    test_le( LDBL_TRUE_MIN, LDBL_MIN );
    test_pass("cxx17:LDBL_TRUE_MIN");

    test_lim( FLT_HAS_SUBNORM,  -1, 1 );
    if (FLT_HAS_SUBNORM == 1)
        test_lt( FLT_TRUE_MIN, FLT_MIN );
    test_pass("cxx17:FLT_HAS_SUBNORM");

    test_lim( DBL_HAS_SUBNORM,  -1, 1 );
    if (DBL_HAS_SUBNORM == 1)
        test_lt( DBL_TRUE_MIN, DBL_MIN );
    test_pass("cxx17:DBL_HAS_SUBNORM");

    test_lim( LDBL_HAS_SUBNORM, -1, 1 );
    if (LDBL_HAS_SUBNORM == 1)
        test_lt( LDBL_TRUE_MIN, LDBL_MIN );
    test_pass("cxx17:LDBL_HAS_SUBNORM");
}
#else
TEST_CASE(cfloat, subnormals) {
    TEST_NOTE("the *_TRUE_MIN / *_HAS_SUBNORM macros arrived with C11 / C++17");
    test_skip("cxx17:FLT_TRUE_MIN");
    test_skip("cxx17:DBL_TRUE_MIN");
    test_skip("cxx17:LDBL_TRUE_MIN");
    test_skip("cxx17:FLT_HAS_SUBNORM");
    test_skip("cxx17:DBL_HAS_SUBNORM");
    test_skip("cxx17:LDBL_HAS_SUBNORM");
}
#endif

TEST_CASE(cfloat, rounding_and_eval_method) {

    const int r = FLT_ROUNDS;
    test_lim( r, -1, 3 );
    test_eq( r, FLT_ROUNDS );
    test_pass("cxx03:FLT_ROUNDS");

    const int m = FLT_EVAL_METHOD;
    test_ge( m, -1 );
    test_le( m, 2 );
    test_pass("cxx11:FLT_EVAL_METHOD");

#if defined(INFINITY)
    const float inf = INFINITY;
    test_true( inf > 0.0f );
    test_true( inf > FLT_MAX );
    test_true( inf == inf * 2.0f );
    test_pass("cxx26:INFINITY");
#else
    TEST_NOTE("INFINITY is C23/C++26 in <cfloat>; this library keeps it in <cmath> only");
    test_skip("cxx26:INFINITY");
#endif
}
#else
TEST_CASE_SKIP(cfloat, radix_and_mant_dig)
TEST_CASE_SKIP(cfloat, decimal_digits)
TEST_CASE_SKIP(cfloat, exponent_ranges)
TEST_CASE_SKIP(cfloat, max_min_epsilon)
TEST_CASE_SKIP(cfloat, subnormals)
TEST_CASE_SKIP(cfloat, rounding_and_eval_method)
#endif


TEST_CASE(cfloat, decimal_dig_and_subnorm_cxx17) {
#if defined(FLT_DECIMAL_DIG)
    test_true( FLT_DECIMAL_DIG >= 6 );
    test_true( FLT_DECIMAL_DIG <= DBL_DECIMAL_DIG );
    test_pass("cxx17:FLT_DECIMAL_DIG");
    test_true( DBL_DECIMAL_DIG >= 10 );
    test_true( DBL_DECIMAL_DIG <= LDBL_DECIMAL_DIG );
    test_pass("cxx17:DBL_DECIMAL_DIG");
    test_true( LDBL_DECIMAL_DIG >= DBL_DECIMAL_DIG );
    test_eq( (int)LDBL_DECIMAL_DIG, (int)DECIMAL_DIG );
    test_pass("cxx17:LDBL_DECIMAL_DIG");
#else
    TEST_NOTE("the *_DECIMAL_DIG macros are C99/C++17; not in this library");
    test_skip("cxx17:FLT_DECIMAL_DIG");
    test_skip("cxx17:DBL_DECIMAL_DIG");
    test_skip("cxx17:LDBL_DECIMAL_DIG");
#endif

#if defined(FLT_HAS_SUBNORM)
    test_true( FLT_HAS_SUBNORM == 1 || FLT_HAS_SUBNORM == 0 || FLT_HAS_SUBNORM == -1 );
    test_pass("cxx17:FLT_HAS_SUBNORM");
    test_true( DBL_HAS_SUBNORM == 1 || DBL_HAS_SUBNORM == 0 || DBL_HAS_SUBNORM == -1 );
    test_pass("cxx17:DBL_HAS_SUBNORM");
    test_true( LDBL_HAS_SUBNORM == 1 || LDBL_HAS_SUBNORM == 0 || LDBL_HAS_SUBNORM == -1 );
    test_pass("cxx17:LDBL_HAS_SUBNORM");
#else
    TEST_NOTE("the *_HAS_SUBNORM macros are C99/C++17 (removed again in C23)");
    test_skip("cxx17:FLT_HAS_SUBNORM");
    test_skip("cxx17:DBL_HAS_SUBNORM");
    test_skip("cxx17:LDBL_HAS_SUBNORM");
#endif

#if defined(FLT_TRUE_MIN)
    test_true( FLT_TRUE_MIN > 0.0f );
    test_true( FLT_TRUE_MIN <= FLT_MIN );
    test_pass("cxx17:FLT_TRUE_MIN");
    test_true( DBL_TRUE_MIN > 0.0 );
    test_true( DBL_TRUE_MIN <= DBL_MIN );
    test_pass("cxx17:DBL_TRUE_MIN");
    test_true( LDBL_TRUE_MIN > 0.0L );
    test_true( LDBL_TRUE_MIN <= LDBL_MIN );
    test_pass("cxx17:LDBL_TRUE_MIN");
#else
    TEST_NOTE("the *_TRUE_MIN macros need a denormal literal, which some compilers reject");
    test_skip("cxx17:FLT_TRUE_MIN");
    test_skip("cxx17:DBL_TRUE_MIN");
    test_skip("cxx17:LDBL_TRUE_MIN");
#endif
}

TEST_CASE(cfloat, c23_additions_cxx26) {
#if defined(NAN)
    {
        float n = NAN;
        test_true( STD::isnan(n) );
        test_true( !STD::isgreaterequal(n, 0.0f) && !STD::isless(n, 0.0f) );
    }
    test_pass("cxx26:NAN");
#else
    TEST_NOTE("NAN reaches <cfloat> only in C23/C++26");
    test_skip("cxx26:NAN");
#endif

#if defined(FLT_SNAN)
    {
        float s = FLT_SNAN;
        test_true( s != s );
    }
    test_pass("cxx26:FLT_SNAN");
    {
        double s = DBL_SNAN;
        test_true( s != s );
    }
    test_pass("cxx26:DBL_SNAN");
    {
        long double s = LDBL_SNAN;
        test_true( s != s );
    }
    test_pass("cxx26:LDBL_SNAN");
#else
    TEST_NOTE("the *_SNAN macros are C23/C++26; not in this library");
    test_skip("cxx26:FLT_SNAN");
    test_skip("cxx26:DBL_SNAN");
    test_skip("cxx26:LDBL_SNAN");
#endif

#if defined(__STDC_VERSION_FLOAT_H__)
    test_true( __STDC_VERSION_FLOAT_H__ >= 202311L );
    test_pass("cxx26:__STDC_VERSION_FLOAT_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("cxx26:__STDC_VERSION_FLOAT_H__");
#endif
}
