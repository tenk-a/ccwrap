#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011
#include <cmath>
#include <cfloat>
#include <cerrno>
#include <climits>
#include <limits>
#include <type_traits>

#if _TST_HAS_DECLTYPE

TEST_CASE(cmath, float_t_double_t) {

#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_floating_point_v<STD_NS float_t>, "float_t is a floating type");
    static_assert(STD::is_floating_point_v<STD_NS double_t>, "double_t is a floating type");
#endif
    test_true( sizeof(STD_NS float_t) >= sizeof(float) );
    test_pass("cxx11:float_t");
    test_true( sizeof(STD_NS double_t) >= sizeof(double) );
    test_pass("cxx11:double_t");

#if FLT_EVAL_METHOD == 0
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<STD_NS float_t, float>, "float_t == float");
    static_assert(STD::is_same_v<STD_NS double_t, double>, "double_t == double");
#endif
#endif

    STD_NS float_t  f = 1.5f;
    STD_NS double_t d = 1.5;
    test_true( f == 1.5f );
    test_true( d == 1.5 );
}

TEST_CASE(cmath, huge_val_macros) {

    double hv = HUGE_VAL;
    test_true( STD_NS isinf(hv) );
    test_true( hv > 0 );
    test_true( hv == STD::numeric_limits<double>::infinity() );
    test_pass("cxx03:HUGE_VAL");

    float hvf = HUGE_VALF;
    test_true( STD_NS isinf(hvf) );
    test_true( hvf == STD::numeric_limits<float>::infinity() );
    test_pass("cxx11:HUGE_VALF");

    long double hvl = HUGE_VALL;
    test_true( STD_NS isinf(hvl) );
    test_true( hvl == STD::numeric_limits<long double>::infinity() );
    test_pass("cxx11:HUGE_VALL");

    float inf = INFINITY;
    test_true( STD_NS isinf(inf) );
    test_true( !STD_NS signbit(inf) );
    test_true( STD_NS isinf(-INFINITY) );
    test_true( STD_NS signbit(-INFINITY) );
    test_pass("cxx11:INFINITY");

    float nn = NAN;
    test_true( STD_NS isnan(nn) );
    TEST_SKIP_WAT();
    test_true( nn != nn );
    test_pass("cxx11:NAN");
}

TEST_CASE(cmath, fp_category_macros) {

    test_true( FP_INFINITE != FP_NAN );
    test_true( FP_NAN != FP_NORMAL );
    test_true( FP_NORMAL != FP_SUBNORMAL );
    test_true( FP_SUBNORMAL != FP_ZERO );
    test_true( FP_ZERO != FP_INFINITE );
    test_true( FP_INFINITE != FP_NORMAL );
    test_true( FP_NAN != FP_ZERO );

    test_eq( STD_NS fpclassify(HUGE_VAL), FP_INFINITE );
    test_eq( STD_NS fpclassify(NAN), FP_NAN );
    test_eq( STD_NS fpclassify(1.0), FP_NORMAL );
    test_eq( STD_NS fpclassify(0.0), FP_ZERO );
    test_eq( STD_NS fpclassify(STD::numeric_limits<double>::denorm_min()), FP_SUBNORMAL );
    test_pass("cxx11:FP_NORMAL");

    test_eq( STD_NS ilogb(0.0), FP_ILOGB0 );
    test_eq( STD_NS ilogb(STD::numeric_limits<double>::quiet_NaN()), FP_ILOGBNAN );
    TEST_SKIP_WAT();
    test_true( FP_ILOGB0 == INT_MIN || FP_ILOGB0 == -INT_MAX );
    test_pass("cxx11:FP_ILOGB0");
}

TEST_CASE(cmath, math_errhandling_macros) {

    test_eq( MATH_ERRNO, 1 );
    test_pass("cxx11:MATH_ERRNO");
    test_eq( MATH_ERREXCEPT, 2 );
    test_pass("cxx11:MATH_ERREXCEPT");
    test_true( (MATH_ERRNO & MATH_ERREXCEPT) == 0 );

    int meh = math_errhandling;
    test_true( (meh & ~(MATH_ERRNO | MATH_ERREXCEPT)) == 0 );
    TEST_SKIP_WAT("reports math errors through neither errno nor the FP flags, so 0");
    test_true( meh != 0 );

    if (math_errhandling & MATH_ERRNO) {
        volatile double neg = -1.0;
        errno = 0;
        double r = STD_NS log((double)neg);
        test_true( STD_NS isnan(r) );
        test_eq( errno, EDOM );
        errno = 0;
    } else {
        TEST_SKIP1();
    }
    test_pass("cxx11:math_errhandling");

#ifdef FP_FAST_FMA
    test_eq( FP_FAST_FMA, 1 );
#else
    TEST_SKIP1();
#endif
#ifdef FP_FAST_FMAF
    test_eq( FP_FAST_FMAF, 1 );
#else
    TEST_SKIP1();
#endif
#ifdef FP_FAST_FMAL
    test_eq( FP_FAST_FMAL, 1 );
    test_pass("cxx11:opt:FP_FAST_FMA");
#else
    TEST_SKIP1();
    test_skip("cxx11:opt:FP_FAST_FMA");
#endif
}

TEST_CASE(cmath, fpclassify) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS fpclassify(1.0f)), int>, "fpclassify(float)");
    static_assert(STD::is_same_v<decltype(STD_NS fpclassify(1.0)), int>, "fpclassify(double)");
    static_assert(STD::is_same_v<decltype(STD_NS fpclassify(1.0L)), int>, "fpclassify(long double)");
#endif

    test_eq( STD_NS fpclassify(1.0f), FP_NORMAL );
    test_eq( STD_NS fpclassify(0.0f), FP_ZERO );
    test_eq( STD_NS fpclassify(-0.0f), FP_ZERO );
    test_eq( STD_NS fpclassify(STD::numeric_limits<float>::infinity()), FP_INFINITE );
    test_eq( STD_NS fpclassify(STD::numeric_limits<float>::quiet_NaN()), FP_NAN );
    test_eq( STD_NS fpclassify(STD::numeric_limits<float>::denorm_min()), FP_SUBNORMAL );

    test_eq( STD_NS fpclassify(1.0), FP_NORMAL );
    test_eq( STD_NS fpclassify(0.0), FP_ZERO );
    test_eq( STD_NS fpclassify(STD::numeric_limits<double>::infinity()), FP_INFINITE );
    test_eq( STD_NS fpclassify(STD::numeric_limits<double>::quiet_NaN()), FP_NAN );
    test_eq( STD_NS fpclassify(STD::numeric_limits<double>::denorm_min()), FP_SUBNORMAL );

    test_eq( STD_NS fpclassify(1.0L), FP_NORMAL );
    test_eq( STD_NS fpclassify(0.0L), FP_ZERO );
    test_eq( STD_NS fpclassify(STD::numeric_limits<long double>::infinity()), FP_INFINITE );
    test_eq( STD_NS fpclassify(STD::numeric_limits<long double>::quiet_NaN()), FP_NAN );
    test_eq( STD_NS fpclassify(STD::numeric_limits<long double>::denorm_min()), FP_SUBNORMAL );
    test_pass("cxx11:fpclassify");
}

TEST_CASE(cmath, isfinite_isinf_isnan) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS isfinite(1.0f)), bool>, "isfinite(float)");
    static_assert(STD::is_same_v<decltype(STD_NS isfinite(1.0)), bool>, "isfinite(double)");
    static_assert(STD::is_same_v<decltype(STD_NS isfinite(1.0L)), bool>, "isfinite(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS isinf(1.0f)), bool>, "isinf(float)");
    static_assert(STD::is_same_v<decltype(STD_NS isinf(1.0)), bool>, "isinf(double)");
    static_assert(STD::is_same_v<decltype(STD_NS isinf(1.0L)), bool>, "isinf(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS isnan(1.0f)), bool>, "isnan(float)");
    static_assert(STD::is_same_v<decltype(STD_NS isnan(1.0)), bool>, "isnan(double)");
    static_assert(STD::is_same_v<decltype(STD_NS isnan(1.0L)), bool>, "isnan(long double)");
#endif

    const float  finf = STD::numeric_limits<float>::infinity();
    const double dinf = STD::numeric_limits<double>::infinity();
    const long double linf = STD::numeric_limits<long double>::infinity();
    const float  fnan = STD::numeric_limits<float>::quiet_NaN();
    const double dnan = STD::numeric_limits<double>::quiet_NaN();
    const long double lnan = STD::numeric_limits<long double>::quiet_NaN();

    test_true(  STD_NS isfinite(1.0f) );
    test_true(  STD_NS isfinite(1.0) );
    test_true(  STD_NS isfinite(1.0L) );
    test_true( !STD_NS isfinite(finf) );
    test_true( !STD_NS isfinite(dinf) );
    test_true( !STD_NS isfinite(linf) );
    test_true( !STD_NS isfinite(dnan) );
    test_true(  STD_NS isfinite(STD::numeric_limits<double>::denorm_min()) );
    test_pass("cxx11:isfinite");

    test_true(  STD_NS isinf(finf) );
    test_true(  STD_NS isinf(dinf) );
    test_true(  STD_NS isinf(linf) );
    test_true(  STD_NS isinf(-dinf) );
    test_true( !STD_NS isinf(1.0) );
    test_true( !STD_NS isinf(dnan) );
    test_pass("cxx11:isinf");

    test_true(  STD_NS isnan(fnan) );
    test_true(  STD_NS isnan(dnan) );
    test_true(  STD_NS isnan(lnan) );
    test_true( !STD_NS isnan(1.0) );
    test_true( !STD_NS isnan(dinf) );
    test_pass("cxx11:isnan");
}

TEST_CASE(cmath, isnormal_signbit) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS isnormal(1.0f)), bool>, "isnormal(float)");
    static_assert(STD::is_same_v<decltype(STD_NS isnormal(1.0)), bool>, "isnormal(double)");
    static_assert(STD::is_same_v<decltype(STD_NS isnormal(1.0L)), bool>, "isnormal(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS signbit(1.0f)), bool>, "signbit(float)");
    static_assert(STD::is_same_v<decltype(STD_NS signbit(1.0)), bool>, "signbit(double)");
    static_assert(STD::is_same_v<decltype(STD_NS signbit(1.0L)), bool>, "signbit(long double)");
#endif

    test_true(  STD_NS isnormal(1.0f) );
    test_true(  STD_NS isnormal(1.0) );
    test_true(  STD_NS isnormal(1.0L) );
    test_true(  STD_NS isnormal(STD::numeric_limits<double>::min()) );
    test_true( !STD_NS isnormal(0.0) );
    test_true( !STD_NS isnormal(STD::numeric_limits<double>::denorm_min()) );
    test_true( !STD_NS isnormal(STD::numeric_limits<float>::denorm_min()) );
    test_true( !STD_NS isnormal(STD::numeric_limits<double>::infinity()) );
    test_true( !STD_NS isnormal(STD::numeric_limits<double>::quiet_NaN()) );
    test_pass("cxx11:isnormal");

    test_true( !STD_NS signbit(1.0) );
    test_true(  STD_NS signbit(-1.0) );
    test_true( !STD_NS signbit(0.0) );
    TEST_SKIP_WAT();
    test_true(  STD_NS signbit(-0.0) );
    TEST_SKIP_WAT();
    test_true(  STD_NS signbit(-0.0f) );
    TEST_SKIP_WAT();
    test_true(  STD_NS signbit(-0.0L) );
    test_true( !STD_NS signbit(1.0f) );
    test_true(  STD_NS signbit(-1.0f) );
    test_true( !STD_NS signbit(1.0L) );
    test_true(  STD_NS signbit(-1.0L) );
    test_true(  STD_NS signbit(-STD::numeric_limits<double>::infinity()) );
    test_pass("cxx11:signbit");
}

TEST_CASE(cmath, isgreater_isless) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS isgreater(1.0f, 0.0f)), bool>, "isgreater(f,f)");
    static_assert(STD::is_same_v<decltype(STD_NS isgreater(1.0, 0.0)), bool>, "isgreater(d,d)");
    static_assert(STD::is_same_v<decltype(STD_NS isgreater(1.0L, 0.0L)), bool>, "isgreater(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS isgreaterequal(1.0f, 0.0f)), bool>, "isgreaterequal(f,f)");
    static_assert(STD::is_same_v<decltype(STD_NS isgreaterequal(1.0, 0.0)), bool>, "isgreaterequal(d,d)");
    static_assert(STD::is_same_v<decltype(STD_NS isgreaterequal(1.0L, 0.0L)), bool>, "isgreaterequal(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS isless(1.0f, 0.0f)), bool>, "isless(f,f)");
    static_assert(STD::is_same_v<decltype(STD_NS isless(1.0, 0.0)), bool>, "isless(d,d)");
    static_assert(STD::is_same_v<decltype(STD_NS isless(1.0L, 0.0L)), bool>, "isless(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS islessequal(1.0f, 0.0f)), bool>, "islessequal(f,f)");
    static_assert(STD::is_same_v<decltype(STD_NS islessequal(1.0, 0.0)), bool>, "islessequal(d,d)");
    static_assert(STD::is_same_v<decltype(STD_NS islessequal(1.0L, 0.0L)), bool>, "islessequal(ld,ld)");
#endif

    const double dnan = STD::numeric_limits<double>::quiet_NaN();
    const float  fnan = STD::numeric_limits<float>::quiet_NaN();
    const long double lnan = STD::numeric_limits<long double>::quiet_NaN();

    test_true(  STD_NS isgreater(2.0, 1.0) );
    test_true( !STD_NS isgreater(1.0, 2.0) );
    test_true( !STD_NS isgreater(1.0, 1.0) );
    test_true( !STD_NS isgreater(dnan, 1.0) );
    test_true(  STD_NS isgreater(2.0f, 1.0f) );
    test_true( !STD_NS isgreater(fnan, 1.0f) );
    test_true(  STD_NS isgreater(2.0L, 1.0L) );
    test_true( !STD_NS isgreater(lnan, 1.0L) );
    test_pass("cxx11:isgreater");

    test_true(  STD_NS isgreaterequal(2.0, 1.0) );
    test_true(  STD_NS isgreaterequal(1.0, 1.0) );
    test_true( !STD_NS isgreaterequal(1.0, 2.0) );
    test_true( !STD_NS isgreaterequal(dnan, 1.0) );
    test_true(  STD_NS isgreaterequal(1.0f, 1.0f) );
    test_true( !STD_NS isgreaterequal(fnan, 1.0f) );
    test_true(  STD_NS isgreaterequal(1.0L, 1.0L) );
    test_true( !STD_NS isgreaterequal(lnan, 1.0L) );
    test_pass("cxx11:isgreaterequal");

    test_true(  STD_NS isless(1.0, 2.0) );
    test_true( !STD_NS isless(2.0, 1.0) );
    test_true( !STD_NS isless(1.0, dnan) );
    test_true(  STD_NS isless(1.0f, 2.0f) );
    test_true( !STD_NS isless(1.0f, fnan) );
    test_true(  STD_NS isless(1.0L, 2.0L) );
    test_true( !STD_NS isless(1.0L, lnan) );
    test_pass("cxx11:isless");

    test_true(  STD_NS islessequal(1.0, 2.0) );
    test_true(  STD_NS islessequal(1.0, 1.0) );
    test_true( !STD_NS islessequal(2.0, 1.0) );
    test_true( !STD_NS islessequal(1.0, dnan) );
    test_true(  STD_NS islessequal(1.0f, 1.0f) );
    test_true( !STD_NS islessequal(1.0f, fnan) );
    test_true(  STD_NS islessequal(1.0L, 1.0L) );
    test_true( !STD_NS islessequal(1.0L, lnan) );
    test_pass("cxx11:islessequal");
}

TEST_CASE(cmath, islessgreater_isunordered) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS islessgreater(1.0f, 0.0f)), bool>, "islessgreater(f,f)");
    static_assert(STD::is_same_v<decltype(STD_NS islessgreater(1.0, 0.0)), bool>, "islessgreater(d,d)");
    static_assert(STD::is_same_v<decltype(STD_NS islessgreater(1.0L, 0.0L)), bool>, "islessgreater(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS isunordered(1.0f, 0.0f)), bool>, "isunordered(f,f)");
    static_assert(STD::is_same_v<decltype(STD_NS isunordered(1.0, 0.0)), bool>, "isunordered(d,d)");
    static_assert(STD::is_same_v<decltype(STD_NS isunordered(1.0L, 0.0L)), bool>, "isunordered(ld,ld)");
#endif

    const double dnan = STD::numeric_limits<double>::quiet_NaN();
    const float  fnan = STD::numeric_limits<float>::quiet_NaN();
    const long double lnan = STD::numeric_limits<long double>::quiet_NaN();

    test_true(  STD_NS islessgreater(1.0, 2.0) );
    test_true(  STD_NS islessgreater(2.0, 1.0) );
    test_true( !STD_NS islessgreater(1.0, 1.0) );
    test_true( !STD_NS islessgreater(dnan, 1.0) );
    test_true(  STD_NS islessgreater(1.0f, 2.0f) );
    test_true( !STD_NS islessgreater(1.0f, fnan) );
    test_true(  STD_NS islessgreater(1.0L, 2.0L) );
    test_true( !STD_NS islessgreater(lnan, 1.0L) );
    test_pass("cxx11:islessgreater");

    test_true( !STD_NS isunordered(1.0, 2.0) );
    test_true(  STD_NS isunordered(dnan, 1.0) );
    test_true(  STD_NS isunordered(1.0, dnan) );
    test_true(  STD_NS isunordered(dnan, dnan) );
    test_true( !STD_NS isunordered(1.0f, 2.0f) );
    test_true(  STD_NS isunordered(fnan, 1.0f) );
    test_true( !STD_NS isunordered(1.0L, 2.0L) );
    test_true(  STD_NS isunordered(lnan, 1.0L) );
    test_pass("cxx11:isunordered");
}
#else
TEST_CASE_SKIP(cmath, float_t_double_t)
TEST_CASE_SKIP(cmath, huge_val_macros)
TEST_CASE_SKIP(cmath, fp_category_macros)
TEST_CASE_SKIP(cmath, math_errhandling_macros)
TEST_CASE_SKIP(cmath, fpclassify)
TEST_CASE_SKIP(cmath, isfinite_isinf_isnan)
TEST_CASE_SKIP(cmath, isnormal_signbit)
TEST_CASE_SKIP(cmath, isgreater_isless)
TEST_CASE_SKIP(cmath, islessgreater_isunordered)
#endif

#else
TEST_CASE_SKIP(cmath, float_t_double_t)
TEST_CASE_SKIP(cmath, huge_val_macros)
TEST_CASE_SKIP(cmath, fp_category_macros)
TEST_CASE_SKIP(cmath, math_errhandling_macros)
TEST_CASE_SKIP(cmath, fpclassify)
TEST_CASE_SKIP(cmath, isfinite_isinf_isnan)
TEST_CASE_SKIP(cmath, isnormal_signbit)
TEST_CASE_SKIP(cmath, isgreater_isless)
TEST_CASE_SKIP(cmath, islessgreater_isunordered)
#endif
