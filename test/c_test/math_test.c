#include "c_test.h"

#include <math.h>
#include <limits.h>
#include <float.h>
#include <errno.h>

#if defined(__WATCOMC__) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
# define FE_ALL_EXCEPT       0
# define feclearexcept(mask) ((void)(mask))
#else
# include <fenv.h>
#endif

static int near_eq(double a, double b) {
    double d = a - b;
    return (d < 0 ? -d : d) < 1e-9;
}

static double make_nan(void) { return nan(""); }
static double make_inf(void) { return HUGE_VAL; }

TEST_CASE(math, c89_basic) {
    double ip = 0, fr;
    int ex = 0;
    double mant;

    test_true( near_eq(sqrt(16.0), 4.0) );
    test_eq( sqrt(0.0), 0.0 );
    test_gt( sqrt(DBL_MIN), 0.0 );
#if TEST_TARGET_C >= 1999
    {
        volatile double outside = -1.0;
        volatile double result;
        errno = 0;
        feclearexcept(FE_ALL_EXCEPT);
        result = sqrt(outside);
        (void)result;
        if (math_errhandling & MATH_ERRNO) test_eq( errno, EDOM );
# if defined(FE_INVALID)
        if (math_errhandling & MATH_ERREXCEPT)
            test_true( (fetestexcept(FE_INVALID) & FE_INVALID) != 0 );
# endif
    }
#endif
    test_pass("c1990:sqrt");
    test_true( near_eq(pow(2.0, 10.0), 1024.0) );
    test_true( near_eq(pow(9.0, 0.5), 3.0) );
    test_eq( pow(DBL_MAX, 1.0), DBL_MAX );
    test_eq( pow(1.0, DBL_MAX), 1.0 );
    test_pass("c1990:pow");
    test_true( near_eq(fabs(-2.5), 2.5) );
    test_eq( fabs(-DBL_MAX), DBL_MAX );
    test_eq( fabs(DBL_MIN), DBL_MIN );
    test_pass("c1990:fabs");
    test_true( near_eq(floor(3.7), 3.0) );
    test_true( near_eq(floor(-3.2), -4.0) );
    test_eq( floor(DBL_MAX), DBL_MAX );
    test_pass("c1990:floor");
    test_true( near_eq(ceil(3.2), 4.0) );
    test_true( near_eq(ceil(-3.7), -3.0) );
    test_eq( ceil(-DBL_MAX), -DBL_MAX );
    test_pass("c1990:ceil");
    test_true( near_eq(fmod(7.0, 3.0), 1.0) );
    test_true( near_eq(fmod(-7.0, 3.0), -1.0) );
    test_eq( fmod(DBL_MAX, DBL_MAX), 0.0 );
    test_pass("c1990:fmod");
    test_true( near_eq(exp(0.0), 1.0) );
#if TEST_TARGET_C >= 1999
    {
        volatile double outside = DBL_MAX;
        double result;
        errno = 0;
        feclearexcept(FE_ALL_EXCEPT);
        result = exp(outside);
        test_eq( result, HUGE_VAL );
        if (math_errhandling & MATH_ERRNO) test_eq( errno, ERANGE );
# if defined(FE_OVERFLOW)
        if (math_errhandling & MATH_ERREXCEPT)
            test_true( (fetestexcept(FE_OVERFLOW) & FE_OVERFLOW) != 0 );
# endif
    }
#endif
    test_pass("c1990:exp");
    test_true( near_eq(log(1.0), 0.0) );
    test_lt( log(DBL_MIN), 0.0 );
#if TEST_TARGET_C >= 1999
    {
        volatile double zero = 0.0;
        double result;
        errno = 0;
        feclearexcept(FE_ALL_EXCEPT);
        result = log(zero);
        test_eq( result, -HUGE_VAL );
        if (math_errhandling & MATH_ERRNO) test_eq( errno, ERANGE );
# if defined(FE_DIVBYZERO)
        if (math_errhandling & MATH_ERREXCEPT)
            test_true( (fetestexcept(FE_DIVBYZERO) & FE_DIVBYZERO) != 0 );
# endif
    }
#endif
    test_pass("c1990:log");
    test_true( near_eq(log10(1000.0), 3.0) );
    test_pass("c1990:log10");

    fr = modf(3.25, &ip);
    test_true( near_eq(ip, 3.0) );
    test_true( near_eq(fr, 0.25) );
    fr = modf(-3.25, &ip);
    test_true( near_eq(ip, -3.0) );
    test_true( near_eq(fr, -0.25) );
    fr = modf(DBL_MAX, &ip);
    test_eq( ip, DBL_MAX );
    test_eq( fr, 0.0 );
    test_pass("c1990:modf");

    mant = frexp(8.0, &ex);
    test_true( near_eq(mant, 0.5) );
    test_eq( ex, 4 );
    test_true( near_eq(ldexp(0.5, 4), 8.0) );
    test_pass("c1990:ldexp");
    mant = frexp(3.0, &ex);
    test_true( near_eq(ldexp(mant, ex), 3.0) );
    mant = frexp(DBL_MIN, &ex);
    test_true( mant >= 0.5 && mant < 1.0 );
    test_eq( ldexp(mant, ex), DBL_MIN );
    test_pass("c1990:frexp");
}

TEST_CASE(math, c89_trig) {
    double pi = 4.0 * atan(1.0);
    test_true( near_eq(sin(0.0), 0.0) );
    test_true( near_eq(cos(0.0), 1.0) );
    test_true( near_eq(tan(0.0), 0.0) );
    test_pass("c1990:tan");
    test_true( near_eq(sin(pi / 2.0), 1.0) );
    test_pass("c1990:sin");
    test_true( near_eq(cos(pi), -1.0) );
    test_pass("c1990:cos");
    test_true( near_eq(asin(0.0), 0.0) );
    test_true( near_eq(asin(1.0), pi / 2.0) );
    test_true( near_eq(asin(-1.0), -pi / 2.0) );
    test_pass("c1990:asin");
    test_true( near_eq(acos(1.0), 0.0) );
    test_true( near_eq(acos(-1.0), pi) );
    test_pass("c1990:acos");
    test_true( near_eq(atan(0.0), 0.0) );
    test_true( near_eq(atan(-DBL_MAX), -pi / 2.0) );
    test_pass("c1990:atan");
    test_true( near_eq(atan2(0.0, 1.0), 0.0) );
    test_true( near_eq(atan2(1.0, 0.0), pi / 2.0) );
    test_pass("c1990:atan2");
    test_true( near_eq(sinh(0.0), 0.0) );
    test_pass("c1990:sinh");
    test_true( near_eq(cosh(0.0), 1.0) );
    test_pass("c1990:cosh");
    test_true( near_eq(tanh(0.0), 0.0) );
    test_true( near_eq(sinh(1.0) / cosh(1.0), tanh(1.0)) );
    test_pass("c1990:tanh");
}

TEST_CASE(math, c99_round) {
    test_true( near_eq(round(2.5), 3.0) );
    test_true( near_eq(round(-2.5), -3.0) );
    test_true( near_eq(round(2.4), 2.0) );
    test_pass("c1999:round");
    test_true( near_eq(trunc(2.9), 2.0) );
    test_true( near_eq(trunc(-2.9), -2.0) );
    test_pass("c1999:trunc");
    test_eq( lround(2.5), 3L );
    test_eq( lround(-2.5), -3L );
    test_pass("c1999:lround");
    test_eq( llround(2.5), 3LL );
    test_eq( llround(-1000000000.5), -1000000001LL );
    test_pass("c1999:llround");
    test_eq( lrint(2.0), 2L );
    test_pass("c1999:lrint");
    test_eq( llrint(2.0), 2LL );
    test_pass("c1999:llrint");

    test_true( near_eq(nearbyint(2.4), 2.0) );
    test_true( near_eq(rint(2.6), 3.0) );
    TEST_SKIP_WAT("");
    test_true( near_eq(rint(2.5), 2.0) );
    TEST_SKIP_WAT("");
    test_true( near_eq(nearbyint(2.5), 2.0) );
    test_pass("c1999:nearbyint");
    test_true( near_eq(rint(3.5), 4.0) );
    test_pass("c1999:rint");

    test_true( near_eq(fdim(5.0, 2.0), 3.0) );
    test_true( near_eq(fdim(2.0, 5.0), 0.0) );
    test_pass("c1999:fdim");
}

TEST_CASE(math, c99_exp_pow) {
    test_true( near_eq(cbrt(27.0), 3.0) );
    test_true( near_eq(cbrt(-8.0), -2.0) );
    test_pass("c1999:cbrt");
    test_true( near_eq(log2(8.0), 3.0) );
    test_pass("c1999:log2");
    test_true( near_eq(exp2(4.0), 16.0) );
    test_pass("c1999:exp2");
    test_true( near_eq(expm1(0.0), 0.0) );
    test_pass("c1999:expm1");
    test_true( near_eq(log1p(0.0), 0.0) );
    test_pass("c1999:log1p");
    test_true( near_eq(hypot(3.0, 4.0), 5.0) );
    test_pass("c1999:hypot");
    test_true( near_eq(fma(2.0, 3.0, 4.0), 10.0) );
    test_pass("c1999:fma");
    test_true( near_eq(fmax(2.0, 5.0), 5.0) );
    test_pass("c1999:fmax");
    test_true( near_eq(fmin(2.0, 5.0), 2.0) );
    test_pass("c1999:fmin");
    test_true( near_eq(copysign(3.0, -1.0), -3.0) );
    test_true( near_eq(copysign(-3.0, 1.0), 3.0) );
    test_pass("c1999:copysign");
    test_true( near_eq(scalbn(1.0, 3), 8.0) );
    test_pass("c1999:scalbn");
    test_true( near_eq(scalbln(1.0, 3L), 8.0) );
    test_pass("c1999:scalbln");
    test_true( near_eq(acosh(1.0), 0.0) );
    test_pass("c1999:acosh");
    test_true( near_eq(asinh(0.0), 0.0) );
    test_pass("c1999:asinh");
    test_true( near_eq(atanh(0.0), 0.0) );
    test_pass("c1999:atanh");
    test_eq( ilogb(8.0), 3 );
    test_pass("c1999:ilogb");
    test_true( near_eq(logb(8.0), 3.0) );
    test_pass("c1999:logb");
    test_true( nextafter(1.0, 2.0) > 1.0 );
    test_true( nextafter(1.0, 0.0) < 1.0 );
    test_pass("c1999:nextafter");
    test_true( near_eq(nexttoward(1.0, 1.0L), 1.0) );
    test_pass("c1999:nexttoward");
    test_true( near_eq(erf(0.0), 0.0) );
    test_pass("c1999:erf");
    test_true( near_eq(erfc(0.0), 1.0) );
    test_pass("c1999:erfc");
    test_true( near_eq(tgamma(4.0), 6.0) );
    test_pass("c1999:tgamma");
    test_true( near_eq(lgamma(1.0), 0.0) );
    test_pass("c1999:lgamma");
}

TEST_CASE(math, c99_remainder) {
    int q = 0;
    double r;
    test_true( near_eq(remainder(5.0, 4.0), 1.0) );
    test_true( near_eq(remainder(9.0, 4.0), 1.0) );
    test_true( near_eq(remainder(2.0, 4.0), 2.0) );
    TEST_SKIP_WAT("");
    test_true( near_eq(remainder(7.0, 4.0), -1.0) );
    TEST_SKIP_WAT("");
    test_true( near_eq(remainder(6.0, 4.0), -2.0) );
    TEST_SKIP_WAT("");
    test_true( near_eq(remainder(-7.0, 4.0), 1.0) );
    test_pass("c1999:remainder");

    r = remquo(7.0, 4.0, &q);
    TEST_SKIP_WAT("");
    test_true( near_eq(r, -1.0) );
    TEST_SKIP_WAT("");
    test_eq( q, 2 );
    test_pass("c1999:remquo");
}

#if TEST_TARGET_C >= 1999
TEST_CASE(math, c99_classify) {
    double nn = make_nan();
    double inf = make_inf();

    test_true(  isnan(nn) );
    test_true( !isnan(1.0) );
    test_pass("c1999:isnan");
    test_true(  isinf(inf) );
    test_true(  isinf(-inf) );
    test_true( !isinf(1.0) );
    test_true( !isinf(nn) );
    test_pass("c1999:isinf");
    test_true(  isfinite(1.0) );
    test_true( !isfinite(inf) );
    test_true( !isfinite(nn) );
    test_pass("c1999:isfinite");
    test_true(  isnormal(1.0) );
    test_true( !isnormal(0.0) );
    test_pass("c1999:isnormal");
    test_true( !signbit(1.0) );
    test_true(  signbit(-1.0) );
    test_true(  signbit(copysign(0.0, -1.0)) );
    test_pass("c1999:signbit");

    test_eq( fpclassify(1.0), FP_NORMAL );
    test_eq( fpclassify(0.0), FP_ZERO );
    test_eq( fpclassify(nn), FP_NAN );
    test_eq( fpclassify(inf), FP_INFINITE );
    test_pass("c1999:fpclassify");
}
#else
TEST_CASE(math, c99_classify) {
    TEST_NOTE("fpclassify and the FP_* macros are C99");
    test_skip("c1999:isnan");
    test_skip("c1999:isinf");
    test_skip("c1999:isfinite");
    test_skip("c1999:isnormal");
    test_skip("c1999:signbit");
    test_skip("c1999:fpclassify");
}
#endif

TEST_CASE(math, c99_compare_macros) {
    double nn = make_nan();

    TEST_SKIP_PCC("pcc miscompiles glibc's isgreater macro");
    test_true(  isgreater(2.0, 1.0) );
    test_true( !isgreater(1.0, 2.0) );
    test_true( !isgreater(1.0, 1.0) );
    test_pass("c1999:isgreater");
    test_true(  isgreaterequal(1.0, 1.0) );
    test_pass("c1999:isgreaterequal");
    test_true(  isless(1.0, 2.0) );
    test_true( !isless(2.0, 1.0) );
    test_pass("c1999:isless");
    test_true(  islessequal(1.0, 1.0) );
    test_pass("c1999:islessequal");
    test_true(  islessgreater(1.0, 2.0) );
    TEST_SKIP_PCC("pcc miscompiles glibc's islessgreater macro for equal operands");
    test_true( !islessgreater(1.0, 1.0) );
    test_pass("c1999:islessgreater");
    test_true(  isunordered(nn, 1.0) );
    test_true(  isunordered(1.0, nn) );
    test_true( !isunordered(1.0, 2.0) );
    test_pass("c1999:isunordered");
    test_true( !isgreater(nn, 1.0) );
    test_true( !isless(nn, 1.0) );
    test_true( !islessgreater(nn, nn) );
}

TEST_CASE(math, c99_float_variants) {
    float fi = 0.0f;
    long double li = 0.0L;
    float ffr;
    int ex = 0;
    int q = 0;

    test_true( near_eq((double)sqrtf(16.0f), 4.0) );  test_pass("c1999:sqrtf");
    test_true( near_eq((double)sqrtl(16.0L), 4.0) );  test_pass("c1999:sqrtl");
    test_true( near_eq((double)fabsf(-2.5f), 2.5) );  test_pass("c1999:fabsf");
    test_true( near_eq((double)floorf(3.7f), 3.0) );  test_pass("c1999:floorf");
    test_true( near_eq((double)ceill(3.2L), 4.0) );   test_pass("c1999:ceill");
    test_true( near_eq((double)sinf(0.0f), 0.0) );    test_pass("c1999:sinf");
    test_true( near_eq((double)cosl(0.0L), 1.0) );    test_pass("c1999:cosl");
    test_true( near_eq((double)cbrtf(27.0f), 3.0) );  test_pass("c1999:cbrtf");
    test_true( near_eq((double)log2l(8.0L), 3.0) );   test_pass("c1999:log2l");

    test_true( near_eq((double)powf(2.0f, 10.0f), 1024.0) );  test_pass("c1999:powf");
    test_true( near_eq((double)atan2l(0.0L, 1.0L), 0.0) );    test_pass("c1999:atan2l");
    test_true( near_eq((double)hypotf(3.0f, 4.0f), 5.0) );    test_pass("c1999:hypotf");
    test_true( near_eq((double)fmaf(2.0f, 3.0f, 4.0f), 10.0) ); test_pass("c1999:fmaf");
    test_true( near_eq((double)ldexpf(0.5f, 4), 8.0) );      test_pass("c1999:ldexpf");
    test_true( near_eq((double)scalbnl(1.0L, 3), 8.0) );     test_pass("c1999:scalbnl");
    test_true( near_eq((double)scalblnf(1.0f, 3L), 8.0) );   test_pass("c1999:scalblnf");

    test_eq( ilogbf(8.0f), 3 );      test_pass("c1999:ilogbf");
    test_eq( lroundf(2.5f), 3L );    test_pass("c1999:lroundf");
    test_eq( lroundl(2.5L), 3L );    test_pass("c1999:lroundl");
    test_eq( llroundf(2.5f), 3LL );  test_pass("c1999:llroundf");
    test_eq( lrintf(2.0f), 2L );     test_pass("c1999:lrintf");

    ffr = modff(3.25f, &fi);
    test_true( near_eq((double)fi, 3.0) );
    test_true( near_eq((double)ffr, 0.25) );
    test_pass("c1999:modff");
    modfl(3.25L, &li);
    test_true( near_eq((double)li, 3.0) );
    test_pass("c1999:modfl");

    test_true( near_eq((double)frexpf(8.0f, &ex), 0.5) );
    test_eq( ex, 4 );
    test_pass("c1999:frexpf");
    remquof(5.0f, 4.0f, &q);
    test_true( q == q );
    test_pass("c1999:remquof");
    test_true( near_eq((double)nexttowardf(1.0f, 1.0L), 1.0) );
    test_pass("c1999:nexttowardf");
    test_true( isnan(nanf("")) );
    test_pass("c1999:nanf");
    test_true( isnan(nan("")) );
    test_pass("c1999:nan");
}

#if TEST_TARGET_C >= 1999
TEST_CASE(math, c99_macros) {
    test_true( FP_ZERO != FP_NORMAL );      test_pass("c1999:FP_ZERO");
    test_true( FP_NORMAL != FP_INFINITE );  test_pass("c1999:FP_NORMAL");
    test_true( FP_INFINITE != FP_NAN );     test_pass("c1999:FP_INFINITE");
    test_true( FP_NAN != FP_SUBNORMAL );    test_pass("c1999:FP_NAN");
    test_true( FP_SUBNORMAL != FP_ZERO );   test_pass("c1999:FP_SUBNORMAL");
#ifdef FP_ILOGB0
    { int v = FP_ILOGB0;   test_true( v == v ); }
    test_pass("c1999:FP_ILOGB0");
#else
    TEST_SKIP1(); TEST_NOTE("FP_ILOGB0 not defined");
    test_skip("c1999:FP_ILOGB0");
#endif
#ifdef FP_ILOGBNAN
    { int v = FP_ILOGBNAN; test_true( v == v ); }
    test_pass("c1999:FP_ILOGBNAN");
#else
    TEST_SKIP1(); TEST_NOTE("FP_ILOGBNAN not defined");
    test_skip("c1999:FP_ILOGBNAN");
#endif

    test_true( HUGE_VAL > 0.0 );            test_pass("c1999:HUGE_VAL");
#ifdef HUGE_VALF
    test_true( (double)HUGE_VALF > 0.0 );
    test_pass("c1999:HUGE_VALF");
#else
    TEST_SKIP1(); TEST_NOTE("HUGE_VALF not defined");
    test_skip("c1999:HUGE_VALF");
#endif
#ifdef HUGE_VALL
    test_true( (double)HUGE_VALL > 0.0 );
    test_pass("c1999:HUGE_VALL");
#else
    TEST_SKIP1(); TEST_NOTE("HUGE_VALL not defined");
    test_skip("c1999:HUGE_VALL");
#endif
#ifdef INFINITY
    test_true( isinf(INFINITY) );
    test_pass("c1999:INFINITY");
#else
    TEST_SKIP1(); TEST_NOTE("INFINITY not defined");
    test_skip("c1999:INFINITY");
#endif
#ifdef NAN
    test_true( isnan(NAN) );
    test_pass("c1999:NAN");
#else
    TEST_SKIP1(); TEST_NOTE("NAN not defined");
    test_skip("c1999:NAN");
#endif

#ifdef math_errhandling
    test_true( (math_errhandling & ~(MATH_ERRNO | MATH_ERREXCEPT)) == 0 );
    test_pass("c1999:math_errhandling");
    test_eq( MATH_ERRNO, 1 );      test_pass("c1999:MATH_ERRNO");
    test_eq( MATH_ERREXCEPT, 2 );  test_pass("c1999:MATH_ERREXCEPT");
#else
    TEST_SKIP1(); TEST_NOTE("math_errhandling not defined"); test_skip("c1999:math_errhandling");
    TEST_SKIP1(); test_skip("c1999:MATH_ERRNO");
    TEST_SKIP1(); test_skip("c1999:MATH_ERREXCEPT");
#endif

#ifdef FP_FAST_FMA
    test_eq( FP_FAST_FMA, 1 );
    test_pass("c1999:opt:FP_FAST_FMA");
#else
    TEST_SKIP1(); TEST_NOTE("FP_FAST_FMA not defined (optional)");
    test_skip("c1999:opt:FP_FAST_FMA");
#endif
#ifdef FP_FAST_FMAF
    test_eq( FP_FAST_FMAF, 1 );
    test_pass("c1999:opt:FP_FAST_FMAF");
#else
    TEST_SKIP1();
    test_skip("c1999:opt:FP_FAST_FMAF");
#endif
#ifdef FP_FAST_FMAL
    test_eq( FP_FAST_FMAL, 1 );
    test_pass("c1999:opt:FP_FAST_FMAL");
#else
    TEST_SKIP1();
    test_skip("c1999:opt:FP_FAST_FMAL");
#endif
}
#else
TEST_CASE(math, c99_macros) {
    TEST_NOTE("the FP_* classification macros are C99");
    test_skip("c1999:FP_ZERO");
    test_skip("c1999:FP_NORMAL");
    test_skip("c1999:FP_INFINITE");
    test_skip("c1999:FP_NAN");
    test_skip("c1999:FP_SUBNORMAL");
    test_skip("c1999:FP_ILOGB0");
    test_skip("c1999:FP_ILOGBNAN");
    test_skip("c1999:HUGE_VAL");
    test_skip("c1999:HUGE_VALF");
    test_skip("c1999:HUGE_VALL");
    test_skip("c1999:INFINITY");
    test_skip("c1999:NAN");
    test_skip("c1999:math_errhandling");
    test_skip("c1999:MATH_ERRNO");
    test_skip("c1999:MATH_ERREXCEPT");
    test_skip("c1999:opt:FP_FAST_FMA");
    test_skip("c1999:opt:FP_FAST_FMAF");
    test_skip("c1999:opt:FP_FAST_FMAL");
}
#endif

#if TEST_TARGET_C >= 1999
TEST_CASE(math, c99_eval_types) {
    float_t  f = 1.5f;
    double_t d = 2.5;
    test_true( near_eq((double)(f + d), 4.0) );

#if FLT_EVAL_METHOD == 0
    test_eq( (int)sizeof(float_t), (int)sizeof(float) );
    test_eq( (int)sizeof(double_t), (int)sizeof(double) );
#elif FLT_EVAL_METHOD == 1
    test_eq( (int)sizeof(float_t), (int)sizeof(double) );
    test_eq( (int)sizeof(double_t), (int)sizeof(double) );
#elif FLT_EVAL_METHOD == 2
    test_eq( (int)sizeof(float_t), (int)sizeof(long double) );
    test_eq( (int)sizeof(double_t), (int)sizeof(long double) );
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif

    test_ge( sizeof(float_t), sizeof(float) );
    test_pass("c1999:float_t");
    test_ge( sizeof(double_t), sizeof(double) );
    test_pass("c1999:double_t");
}
#else
TEST_CASE(math, c99_eval_types) {
    TEST_NOTE("float_t / double_t are C99");
    test_skip("c1999:float_t");
    test_skip("c1999:double_t");
}
#endif

TEST_CASE(math, c23_roundeven_extrema) {
#if TEST_TARGET_C >= 2023
    double nn = make_nan();
    double nz = copysign(0.0, -1.0);

    test_true( roundeven(2.5) == 2.0 );
    test_true( roundeven(3.5) == 4.0 );
    test_true( roundeven(-2.5) == -2.0 );
    test_true( roundeven(2.4) == 2.0 );
    test_true( roundeven(2.6) == 3.0 );
#if defined(__STDC_VERSION_MATH_H__)
    test_true( isnan(roundeven(nn)) );
    test_pass("c2023:roundeven");
#else
    test_skip("c2023:roundeven");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( roundevenf(1.5f) == 2.0f );
    test_pass("c2023:roundevenf");
#else
    test_skip("c2023:roundevenf");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( roundevenl(1.5L) == 2.0L );
    test_pass("c2023:roundevenl");
#else
    test_skip("c2023:roundevenl");
#endif

    test_true( fmaximum(3.0, 5.0) == 5.0 );
#if defined(__STDC_VERSION_MATH_H__)
    test_true( isnan(fmaximum(nn, 5.0)) );
    test_pass("c2023:fmaximum");
#else
    test_skip("c2023:fmaximum");
#endif
    test_true( fminimum(3.0, 5.0) == 3.0 );
#if defined(__STDC_VERSION_MATH_H__)
    test_true( isnan(fminimum(3.0, nn)) );
    test_pass("c2023:fminimum");
#else
    test_skip("c2023:fminimum");
#endif
    test_true( fmaximum_num(nn, 5.0) == 5.0 );
#if defined(__STDC_VERSION_MATH_H__)
    test_true( isnan(fmaximum_num(nn, nn)) );
    test_pass("c2023:fmaximum_num");
#else
    test_skip("c2023:fmaximum_num");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( fminimum_num(nn, 5.0) == 5.0 );
    test_pass("c2023:fminimum_num");
#else
    test_skip("c2023:fminimum_num");
#endif

#if defined(__STDC_VERSION_MATH_H__)
    test_true( fmaximum_mag(-5.0, 3.0) == -5.0 );
    test_pass("c2023:fmaximum_mag");
#else
    test_skip("c2023:fmaximum_mag");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( fminimum_mag(-5.0, 3.0) == 3.0 );
    test_pass("c2023:fminimum_mag");
#else
    test_skip("c2023:fminimum_mag");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( fmaximum_mag_num(nn, -5.0) == -5.0 );
    test_pass("c2023:fmaximum_mag_num");
#else
    test_skip("c2023:fmaximum_mag_num");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( fminimum_mag_num(nn, 3.0) == 3.0 );
    test_pass("c2023:fminimum_mag_num");
#else
    test_skip("c2023:fminimum_mag_num");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( fmaximumf(1.0f, 2.0f) == 2.0f );
    test_pass("c2023:fmaximumf");
#else
    test_skip("c2023:fmaximumf");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( fminimuml(1.0L, 2.0L) == 1.0L );
    test_pass("c2023:fminimuml");
#else
    test_skip("c2023:fminimuml");
#endif

    test_true( signbit(nz) );
    test_true( !signbit(fmaximum(nz, 0.0)) );
#if defined(__STDC_VERSION_MATH_H__)
    test_true( signbit(fminimum(0.0, nz)) );
    test_pass("c2023:signed-zero rules property");
#else
    test_skip("c2023:signed-zero rules property");
#endif

    test_true( !issignaling(5.0) );
#if defined(__STDC_VERSION_MATH_H__)
    test_true( !issignaling(nn) );
    test_pass("c2023:issignaling");
#else
    test_skip("c2023:issignaling");
#endif
    test_true( iscanonical(5.0) );
#if defined(__STDC_VERSION_MATH_H__)
    test_true( iscanonical(nn) );
    test_pass("c2023:iscanonical");
#else
    test_skip("c2023:iscanonical");
#endif
#else
    TEST_SKIP_CASE();
#endif
}

static int ccw_m_close(double a, double b, double tol) {
    double d = a - b; double m = b < 0 ? -b : b;
    if (d < 0) d = -d;
    return d <= tol * (1.0 + m);
}

TEST_CASE(math, float_long_double_a_c1999) {
    test_true( ccw_m_close((double)acosf(0.5f), acos(0.5), 1e-5) );
    test_pass("c1999:acosf");
    test_true( ccw_m_close((double)acoshf(2.0f), acosh(2.0), 1e-5) );
    test_pass("c1999:acoshf");
    test_true( ccw_m_close((double)acoshl(2.0L), acosh(2.0), 1e-9) );
    test_pass("c1999:acoshl");
    test_true( ccw_m_close((double)acosl(0.5L), acos(0.5), 1e-9) );
    test_pass("c1999:acosl");
    test_true( ccw_m_close((double)asinf(0.5f), asin(0.5), 1e-5) );
    test_pass("c1999:asinf");
    test_true( ccw_m_close((double)asinhf(0.5f), asinh(0.5), 1e-5) );
    test_pass("c1999:asinhf");
    test_true( ccw_m_close((double)asinhl(0.5L), asinh(0.5), 1e-9) );
    test_pass("c1999:asinhl");
    test_true( ccw_m_close((double)asinl(0.5L), asin(0.5), 1e-9) );
    test_pass("c1999:asinl");
    test_true( ccw_m_close((double)atan2f(1.0f, 2.0f), atan2(1.0, 2.0), 1e-5) );
    test_pass("c1999:atan2f");
    test_true( ccw_m_close((double)atanf(0.5f), atan(0.5), 1e-5) );
    test_pass("c1999:atanf");
    test_true( ccw_m_close((double)atanhf(0.5f), atanh(0.5), 1e-5) );
    test_pass("c1999:atanhf");
    test_true( ccw_m_close((double)atanhl(0.5L), atanh(0.5), 1e-9) );
    test_pass("c1999:atanhl");
    test_true( ccw_m_close((double)atanl(0.5L), atan(0.5), 1e-9) );
    test_pass("c1999:atanl");
    test_true( ccw_m_close((double)cbrtl(27.0L), cbrt(27.0), 1e-9) );
    test_pass("c1999:cbrtl");
    test_true( ccw_m_close((double)ceilf(3.2f), ceil(3.2), 1e-5) );
    test_pass("c1999:ceilf");
    test_true( ccw_m_close((double)copysignf(3.0f, -1.0f), copysign(3.0, -1.0), 1e-5) );
    test_pass("c1999:copysignf");
    test_true( ccw_m_close((double)copysignl(3.0L, -1.0L), copysign(3.0, -1.0), 1e-9) );
    test_pass("c1999:copysignl");
    test_true( ccw_m_close((double)cosf(0.5f), cos(0.5), 1e-5) );
    test_pass("c1999:cosf");
    test_true( ccw_m_close((double)coshf(0.5f), cosh(0.5), 1e-5) );
    test_pass("c1999:coshf");
    test_true( ccw_m_close((double)coshl(0.5L), cosh(0.5), 1e-9) );
    test_pass("c1999:coshl");
    test_true( ccw_m_close((double)erfcf(0.5f), erfc(0.5), 1e-5) );
    test_pass("c1999:erfcf");
    test_true( ccw_m_close((double)erfcl(0.5L), erfc(0.5), 1e-9) );
    test_pass("c1999:erfcl");
    test_true( ccw_m_close((double)erff(0.5f), erf(0.5), 1e-5) );
    test_pass("c1999:erff");
    test_true( ccw_m_close((double)erfl(0.5L), erf(0.5), 1e-9) );
    test_pass("c1999:erfl");
}

TEST_CASE(math, float_long_double_b_c1999) {
    test_true( ccw_m_close((double)exp2f(3.0f), exp2(3.0), 1e-5) );
    test_pass("c1999:exp2f");
    test_true( ccw_m_close((double)exp2l(3.0L), exp2(3.0), 1e-9) );
    test_pass("c1999:exp2l");
    test_true( ccw_m_close((double)expf(1.0f), exp(1.0), 1e-5) );
    test_pass("c1999:expf");
    test_true( ccw_m_close((double)expl(1.0L), exp(1.0), 1e-9) );
    test_pass("c1999:expl");
    test_true( ccw_m_close((double)expm1f(0.5f), expm1(0.5), 1e-5) );
    test_pass("c1999:expm1f");
    test_true( ccw_m_close((double)expm1l(0.5L), expm1(0.5), 1e-9) );
    test_pass("c1999:expm1l");
    test_true( ccw_m_close((double)fabsl(-2.5L), fabs(-2.5), 1e-9) );
    test_pass("c1999:fabsl");
    test_true( ccw_m_close((double)fdimf(5.0f, 2.0f), fdim(5.0, 2.0), 1e-5) );
    test_pass("c1999:fdimf");
    test_true( ccw_m_close((double)fdiml(5.0L, 2.0L), fdim(5.0, 2.0), 1e-9) );
    test_pass("c1999:fdiml");
    test_true( ccw_m_close((double)floorl(3.7L), floor(3.7), 1e-9) );
    test_pass("c1999:floorl");
    test_true( ccw_m_close((double)fmal(2.0L, 3.0L, 4.0L), fma(2.0, 3.0, 4.0), 1e-9) );
    test_pass("c1999:fmal");
    test_true( ccw_m_close((double)fmaxf(2.0f, 5.0f), fmax(2.0, 5.0), 1e-5) );
    test_pass("c1999:fmaxf");
    test_true( ccw_m_close((double)fmaxl(2.0L, 5.0L), fmax(2.0, 5.0), 1e-9) );
    test_pass("c1999:fmaxl");
    test_true( ccw_m_close((double)fminf(2.0f, 5.0f), fmin(2.0, 5.0), 1e-5) );
    test_pass("c1999:fminf");
    test_true( ccw_m_close((double)fminl(2.0L, 5.0L), fmin(2.0, 5.0), 1e-9) );
    test_pass("c1999:fminl");
    test_true( ccw_m_close((double)fmodf(7.0f, 3.0f), fmod(7.0, 3.0), 1e-5) );
    test_pass("c1999:fmodf");
    test_true( ccw_m_close((double)fmodl(7.0L, 3.0L), fmod(7.0, 3.0), 1e-9) );
    test_pass("c1999:fmodl");
    { int e_ = 0; long double m_ = frexpl(8.0L, &e_); test_true( ccw_m_close((double)m_, 0.5, 1e-9) && e_ == 4 ); }
    test_pass("c1999:frexpl");
    test_true( ccw_m_close((double)hypotl(3.0L, 4.0L), hypot(3.0, 4.0), 1e-9) );
    test_pass("c1999:hypotl");
    test_eq( ilogbl(8.0L), ilogb(8.0) );
    test_pass("c1999:ilogbl");
    test_true( ccw_m_close((double)ldexpl(0.5L, 4), 8.0, 1e-9) );
    test_pass("c1999:ldexpl");
    test_true( ccw_m_close((double)lgammaf(3.0f), lgamma(3.0), 1e-5) );
    test_pass("c1999:lgammaf");
    test_true( ccw_m_close((double)lgammal(3.0L), lgamma(3.0), 1e-9) );
    test_pass("c1999:lgammal");
   test_true( llrintf(2.0f) == llrint(2.0) );
    test_pass("c1999:llrintf");
}

TEST_CASE(math, float_long_double_c_c1999) {
   test_true( llrintl(2.0L) == llrint(2.0) );
    test_pass("c1999:llrintl");
  test_true( llroundl(2.5L) == llround(2.5) );
    test_pass("c1999:llroundl");
    test_true( ccw_m_close((double)log10f(1000.0f), log10(1000.0), 1e-5) );
    test_pass("c1999:log10f");
    test_true( ccw_m_close((double)log10l(1000.0L), log10(1000.0), 1e-9) );
    test_pass("c1999:log10l");
    test_true( ccw_m_close((double)log1pf(0.5f), log1p(0.5), 1e-5) );
    test_pass("c1999:log1pf");
    test_true( ccw_m_close((double)log1pl(0.5L), log1p(0.5), 1e-9) );
    test_pass("c1999:log1pl");
    test_true( ccw_m_close((double)log2f(8.0f), log2(8.0), 1e-5) );
    test_pass("c1999:log2f");
    test_true( ccw_m_close((double)logbf(8.0f), logb(8.0), 1e-5) );
    test_pass("c1999:logbf");
    test_true( ccw_m_close((double)logbl(8.0L), logb(8.0), 1e-9) );
    test_pass("c1999:logbl");
    test_true( ccw_m_close((double)logf(2.0f), log(2.0), 1e-5) );
    test_pass("c1999:logf");
    test_true( ccw_m_close((double)logl(2.0L), log(2.0), 1e-9) );
    test_pass("c1999:logl");
    test_true( lrintl(2.0L) == lrint(2.0) );
    test_pass("c1999:lrintl");
    { long double v_ = nanl(""); test_true( isnan((double)v_) ); }
    test_pass("c1999:nanl");
    test_true( ccw_m_close((double)nearbyintf(2.4f), nearbyint(2.4), 1e-5) );
    test_pass("c1999:nearbyintf");
    test_true( ccw_m_close((double)nearbyintl(2.4L), nearbyint(2.4), 1e-9) );
    test_pass("c1999:nearbyintl");
    test_true( ccw_m_close((double)nextafterf(1.0f, 2.0f), nextafter(1.0, 2.0), 1e-5) );
    test_pass("c1999:nextafterf");
    test_true( ccw_m_close((double)nextafterl(1.0L, 2.0L), nextafter(1.0, 2.0), 1e-9) );
    test_pass("c1999:nextafterl");
    test_true( nexttowardl(1.0L, 2.0L) > 1.0L );
    test_pass("c1999:nexttowardl");
    test_true( ccw_m_close((double)powl(2.0L, 10.0L), pow(2.0, 10.0), 1e-9) );
    test_pass("c1999:powl");
    test_true( ccw_m_close((double)remainderf(7.0f, 3.0f), remainder(7.0, 3.0), 1e-5) );
    test_pass("c1999:remainderf");
    test_true( ccw_m_close((double)remainderl(7.0L, 3.0L), remainder(7.0, 3.0), 1e-9) );
    test_pass("c1999:remainderl");
   { int q_ = 0; long double r_ = remquol(7.0L, 3.0L, &q_); test_true( ccw_m_close((double)r_, 1.0, 1e-9) && q_ != 0 ); }
    test_pass("c1999:remquol");
    test_true( ccw_m_close((double)rintf(2.6f), rint(2.6), 1e-5) );
    test_pass("c1999:rintf");
    test_true( ccw_m_close((double)rintl(2.6L), rint(2.6), 1e-9) );
    test_pass("c1999:rintl");
}

TEST_CASE(math, float_long_double_d_c1999) {
    test_true( ccw_m_close((double)roundf(2.5f), round(2.5), 1e-5) );
    test_pass("c1999:roundf");
    test_true( ccw_m_close((double)roundl(2.5L), round(2.5), 1e-9) );
    test_pass("c1999:roundl");
  test_true( ccw_m_close((double)scalblnl(1.0L, 3L), 8.0, 1e-9) );
    test_pass("c1999:scalblnl");
   test_true( ccw_m_close((double)scalbnf(1.0f, 3), 8.0, 1e-5) );
    test_pass("c1999:scalbnf");
    test_true( ccw_m_close((double)sinhf(0.5f), sinh(0.5), 1e-5) );
    test_pass("c1999:sinhf");
    test_true( ccw_m_close((double)sinhl(0.5L), sinh(0.5), 1e-9) );
    test_pass("c1999:sinhl");
    test_true( ccw_m_close((double)sinl(0.5L), sin(0.5), 1e-9) );
    test_pass("c1999:sinl");
    test_true( ccw_m_close((double)tanf(0.5f), tan(0.5), 1e-5) );
    test_pass("c1999:tanf");
    test_true( ccw_m_close((double)tanhf(0.5f), tanh(0.5), 1e-5) );
    test_pass("c1999:tanhf");
    test_true( ccw_m_close((double)tanhl(0.5L), tanh(0.5), 1e-9) );
    test_pass("c1999:tanhl");
    test_true( ccw_m_close((double)tanl(0.5L), tan(0.5), 1e-9) );
    test_pass("c1999:tanl");
    test_true( ccw_m_close((double)tgammaf(4.0f), tgamma(4.0), 1e-5) );
    test_pass("c1999:tgammaf");
    test_true( ccw_m_close((double)tgammal(4.0L), tgamma(4.0), 1e-9) );
    test_pass("c1999:tgammal");
    test_true( ccw_m_close((double)truncf(2.9f), trunc(2.9), 1e-5) );
    test_pass("c1999:truncf");
    test_true( ccw_m_close((double)truncl(2.9L), trunc(2.9), 1e-9) );
    test_pass("c1999:truncl");
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fdivl(6.0L, 3.0L), fdiv(6.0, 3.0), 1e-9) );
    test_pass("c2023:fdivl");
#else
    test_skip("c2023:fdivl");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fmaximum_magf(-5.0f, 3.0f), fmaximum_mag(-5.0, 3.0), 1e-5) );
    test_pass("c2023:fmaximum_magf");
#else
    test_skip("c2023:fmaximum_magf");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fmaximum_magl(-5.0L, 3.0L), fmaximum_mag(-5.0, 3.0), 1e-9) );
    test_pass("c2023:fmaximum_magl");
#else
    test_skip("c2023:fmaximum_magl");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fmaximum_numf(3.0f, 5.0f), fmaximum_num(3.0, 5.0), 1e-5) );
    test_pass("c2023:fmaximum_numf");
#else
    test_skip("c2023:fmaximum_numf");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fmaximum_numl(3.0L, 5.0L), fmaximum_num(3.0, 5.0), 1e-9) );
    test_pass("c2023:fmaximum_numl");
#else
    test_skip("c2023:fmaximum_numl");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fmaximuml(3.0L, 5.0L), fmaximum(3.0, 5.0), 1e-9) );
    test_pass("c2023:fmaximuml");
#else
    test_skip("c2023:fmaximuml");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fminimum_magf(-5.0f, 3.0f), fminimum_mag(-5.0, 3.0), 1e-5) );
    test_pass("c2023:fminimum_magf");
#else
    test_skip("c2023:fminimum_magf");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fminimum_magl(-5.0L, 3.0L), fminimum_mag(-5.0, 3.0), 1e-9) );
    test_pass("c2023:fminimum_magl");
#else
    test_skip("c2023:fminimum_magl");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fminimum_numf(3.0f, 5.0f), fminimum_num(3.0, 5.0), 1e-5) );
    test_pass("c2023:fminimum_numf");
#else
    test_skip("c2023:fminimum_numf");
#endif
}

TEST_CASE(math, float_long_double_e_c1999) {
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fminimum_numl(3.0L, 5.0L), fminimum_num(3.0, 5.0), 1e-9) );
    test_pass("c2023:fminimum_numl");
#else
    test_skip("c2023:fminimum_numl");
#endif
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)fminimumf(3.0f, 5.0f), fminimum(3.0, 5.0), 1e-5) );
    test_pass("c2023:fminimumf");
#else
    test_skip("c2023:fminimumf");
#endif
}

TEST_CASE(math, c23_additions_a_c2023) {
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)acospi(1.0), 0.0, 1e-9) );
    test_pass("c2023:acospi");
    test_true( ccw_m_close((double)acospif(1.0f), 0.0, 1e-5) );
    test_pass("c2023:acospif");
    test_true( ccw_m_close((double)acospil(1.0L), 0.0, 1e-9) );
    test_pass("c2023:acospil");
    test_true( ccw_m_close((double)asinpi(0.0), 0.0, 1e-9) );
    test_pass("c2023:asinpi");
    test_true( ccw_m_close((double)asinpif(0.0f), 0.0, 1e-5) );
    test_pass("c2023:asinpif");
    test_true( ccw_m_close((double)asinpil(0.0L), 0.0, 1e-9) );
    test_pass("c2023:asinpil");
    test_true( ccw_m_close((double)atan2pi(0.0, 1.0), 0.0, 1e-9) );
    test_pass("c2023:atan2pi");
    test_true( ccw_m_close((double)atan2pif(0.0f, 1.0f), 0.0, 1e-5) );
    test_pass("c2023:atan2pif");
    test_true( ccw_m_close((double)atan2pil(0.0L, 1.0L), 0.0, 1e-9) );
    test_pass("c2023:atan2pil");
    test_true( ccw_m_close((double)atanpi(0.0), 0.0, 1e-9) );
    test_pass("c2023:atanpi");
    test_true( ccw_m_close((double)atanpif(0.0f), 0.0, 1e-5) );
    test_pass("c2023:atanpif");
    test_true( ccw_m_close((double)atanpil(0.0L), 0.0, 1e-9) );
    test_pass("c2023:atanpil");
    { double in_ = 1.5, out_ = 0; test_eq( canonicalize(&out_, &in_), 0 ); test_true( ccw_m_close((double)out_, 1.5, 1e-9) ); }
    test_pass("c2023:canonicalize");
    { float in_ = 1.5f, out_ = 0; test_eq( canonicalizef(&out_, &in_), 0 ); test_true( ccw_m_close((double)out_, 1.5, 1e-5) ); }
    test_pass("c2023:canonicalizef");
    { long double in_ = 1.5L, out_ = 0; test_eq( canonicalizel(&out_, &in_), 0 ); test_true( ccw_m_close((double)out_, 1.5, 1e-9) ); }
    test_pass("c2023:canonicalizel");
    test_true( ccw_m_close((double)compoundn(1.0, 2), 4.0, 1e-9) );
    test_pass("c2023:compoundn");
    test_true( ccw_m_close((double)compoundnf(1.0f, 2), 4.0, 1e-5) );
    test_pass("c2023:compoundnf");
    test_true( ccw_m_close((double)compoundnl(1.0L, 2), 4.0, 1e-9) );
    test_pass("c2023:compoundnl");
    test_true( ccw_m_close((double)cospi(0.0), 1.0, 1e-9) );
    test_pass("c2023:cospi");
    test_true( ccw_m_close((double)cospif(0.0f), 1.0, 1e-5) );
    test_pass("c2023:cospif");
#else
    TEST_NOTE("the C23 math additions are not provided by this library");
    test_skip("c2023:acospi");
    test_skip("c2023:acospif");
    test_skip("c2023:acospil");
    test_skip("c2023:asinpi");
    test_skip("c2023:asinpif");
    test_skip("c2023:asinpil");
    test_skip("c2023:atan2pi");
    test_skip("c2023:atan2pif");
    test_skip("c2023:atan2pil");
    test_skip("c2023:atanpi");
    test_skip("c2023:atanpif");
    test_skip("c2023:atanpil");
    test_skip("c2023:canonicalize");
    test_skip("c2023:canonicalizef");
    test_skip("c2023:canonicalizel");
    test_skip("c2023:compoundn");
    test_skip("c2023:compoundnf");
    test_skip("c2023:compoundnl");
    test_skip("c2023:cospi");
    test_skip("c2023:cospif");
#endif
}

TEST_CASE(math, c23_additions_b_c2023) {
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)cospil(0.0L), 1.0, 1e-9) );
    test_pass("c2023:cospil");
    test_true( ccw_m_close((double)exp10(2.0), 100.0, 1e-9) );
    test_pass("c2023:exp10");
    test_true( ccw_m_close((double)exp10f(2.0f), 100.0, 1e-5) );
    test_pass("c2023:exp10f");
    test_true( ccw_m_close((double)exp10l(2.0L), 100.0, 1e-9) );
    test_pass("c2023:exp10l");
    test_true( ccw_m_close((double)exp10m1(2.0), 99.0, 1e-9) );
    test_pass("c2023:exp10m1");
    test_true( ccw_m_close((double)exp10m1f(2.0f), 99.0, 1e-5) );
    test_pass("c2023:exp10m1f");
    test_true( ccw_m_close((double)exp10m1l(2.0L), 99.0, 1e-9) );
    test_pass("c2023:exp10m1l");
    test_true( ccw_m_close((double)exp2m1(3.0), 7.0, 1e-9) );
    test_pass("c2023:exp2m1");
    test_true( ccw_m_close((double)exp2m1f(3.0f), 7.0, 1e-5) );
    test_pass("c2023:exp2m1f");
    test_true( ccw_m_close((double)exp2m1l(3.0L), 7.0, 1e-9) );
    test_pass("c2023:exp2m1l");
    test_true( (long long)fromfp(2.5, FP_INT_TOWARDZERO, 32) == (long long)2 );
    test_pass("c2023:fromfp");
    test_true( (long long)fromfpf(2.5f, FP_INT_TOWARDZERO, 32) == (long long)2 );
    test_pass("c2023:fromfpf");
    test_true( (long long)fromfpl(2.5L, FP_INT_TOWARDZERO, 32) == (long long)2 );
    test_pass("c2023:fromfpl");
    test_true( (long long)fromfpx(2.5, FP_INT_TOWARDZERO, 32) == (long long)2 );
    test_pass("c2023:fromfpx");
    test_true( (long long)fromfpxf(2.5f, FP_INT_TOWARDZERO, 32) == (long long)2 );
    test_pass("c2023:fromfpxf");
    test_true( (long long)fromfpxl(2.5L, FP_INT_TOWARDZERO, 32) == (long long)2 );
    test_pass("c2023:fromfpxl");
    test_true( llogb(8.0) == (long)ilogb(8.0) );
    test_pass("c2023:llogb");
    test_true( llogbf(8.0f) == (long)ilogb(8.0) );
    test_pass("c2023:llogbf");
    test_true( llogbl(8.0L) == (long)ilogb(8.0) );
    test_pass("c2023:llogbl");
    test_true( ccw_m_close((double)log10p1(9.0), 1.0, 1e-9) );
    test_pass("c2023:log10p1");
#else
    TEST_NOTE("the C23 math additions are not provided by this library");
    test_skip("c2023:cospil");
    test_skip("c2023:exp10");
    test_skip("c2023:exp10f");
    test_skip("c2023:exp10l");
    test_skip("c2023:exp10m1");
    test_skip("c2023:exp10m1f");
    test_skip("c2023:exp10m1l");
    test_skip("c2023:exp2m1");
    test_skip("c2023:exp2m1f");
    test_skip("c2023:exp2m1l");
    test_skip("c2023:fromfp");
    test_skip("c2023:fromfpf");
    test_skip("c2023:fromfpl");
    test_skip("c2023:fromfpx");
    test_skip("c2023:fromfpxf");
    test_skip("c2023:fromfpxl");
    test_skip("c2023:llogb");
    test_skip("c2023:llogbf");
    test_skip("c2023:llogbl");
    test_skip("c2023:log10p1");
#endif
}

TEST_CASE(math, c23_additions_c_c2023) {
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)log10p1f(9.0f), 1.0, 1e-5) );
    test_pass("c2023:log10p1f");
    test_true( ccw_m_close((double)log10p1l(9.0L), 1.0, 1e-9) );
    test_pass("c2023:log10p1l");
    test_true( ccw_m_close((double)log2p1(7.0), 3.0, 1e-9) );
    test_pass("c2023:log2p1");
    test_true( ccw_m_close((double)log2p1f(7.0f), 3.0, 1e-5) );
    test_pass("c2023:log2p1f");
    test_true( ccw_m_close((double)log2p1l(7.0L), 3.0, 1e-9) );
    test_pass("c2023:log2p1l");
    test_true( ccw_m_close((double)logp1(0.0), 0.0, 1e-9) );
    test_pass("c2023:logp1");
    test_true( ccw_m_close((double)logp1f(0.0f), 0.0, 1e-5) );
    test_pass("c2023:logp1f");
    test_true( ccw_m_close((double)logp1l(0.0L), 0.0, 1e-9) );
    test_pass("c2023:logp1l");
    test_true( ccw_m_close((double)pown(2.0, 10), 1024.0, 1e-9) );
    test_pass("c2023:pown");
    test_true( ccw_m_close((double)pownf(2.0f, 10), 1024.0, 1e-5) );
    test_pass("c2023:pownf");
    test_true( ccw_m_close((double)pownl(2.0L, 10), 1024.0, 1e-9) );
    test_pass("c2023:pownl");
    test_true( ccw_m_close((double)powr(2.0, 10.0), 1024.0, 1e-9) );
    test_pass("c2023:powr");
    test_true( ccw_m_close((double)powrf(2.0f, 10.0f), 1024.0, 1e-5) );
    test_pass("c2023:powrf");
    test_true( ccw_m_close((double)powrl(2.0L, 10.0L), 1024.0, 1e-9) );
    test_pass("c2023:powrl");
    test_true( ccw_m_close((double)rootn(8.0, 3), 2.0, 1e-9) );
    test_pass("c2023:rootn");
    test_true( ccw_m_close((double)rootnf(8.0f, 3), 2.0, 1e-5) );
    test_pass("c2023:rootnf");
    test_true( ccw_m_close((double)rootnl(8.0L, 3), 2.0, 1e-9) );
    test_pass("c2023:rootnl");
    test_true( ccw_m_close((double)rsqrt(4.0), 0.5, 1e-9) );
    test_pass("c2023:rsqrt");
    test_true( ccw_m_close((double)rsqrtf(4.0f), 0.5, 1e-5) );
    test_pass("c2023:rsqrtf");
    test_true( ccw_m_close((double)rsqrtl(4.0L), 0.5, 1e-9) );
    test_pass("c2023:rsqrtl");
#else
    TEST_NOTE("the C23 math additions are not provided by this library");
    test_skip("c2023:log10p1f");
    test_skip("c2023:log10p1l");
    test_skip("c2023:log2p1");
    test_skip("c2023:log2p1f");
    test_skip("c2023:log2p1l");
    test_skip("c2023:logp1");
    test_skip("c2023:logp1f");
    test_skip("c2023:logp1l");
    test_skip("c2023:pown");
    test_skip("c2023:pownf");
    test_skip("c2023:pownl");
    test_skip("c2023:powr");
    test_skip("c2023:powrf");
    test_skip("c2023:powrl");
    test_skip("c2023:rootn");
    test_skip("c2023:rootnf");
    test_skip("c2023:rootnl");
    test_skip("c2023:rsqrt");
    test_skip("c2023:rsqrtf");
    test_skip("c2023:rsqrtl");
#endif
}

TEST_CASE(math, c23_additions_d_c2023) {
#if defined(__STDC_VERSION_MATH_H__)
    test_true( ccw_m_close((double)sinpi(0.0), 0.0, 1e-9) );
    test_pass("c2023:sinpi");
    test_true( ccw_m_close((double)sinpif(0.0f), 0.0, 1e-5) );
    test_pass("c2023:sinpif");
    test_true( ccw_m_close((double)sinpil(0.0L), 0.0, 1e-9) );
    test_pass("c2023:sinpil");
    test_true( ccw_m_close((double)tanpi(0.0), 0.0, 1e-9) );
    test_pass("c2023:tanpi");
    test_true( ccw_m_close((double)tanpif(0.0f), 0.0, 1e-5) );
    test_pass("c2023:tanpif");
    test_true( ccw_m_close((double)tanpil(0.0L), 0.0, 1e-9) );
    test_pass("c2023:tanpil");
    test_true( (unsigned long long)ufromfp(2.5, FP_INT_TOWARDZERO, 32) == (unsigned long long)2 );
    test_pass("c2023:ufromfp");
    test_true( (unsigned long long)ufromfpf(2.5f, FP_INT_TOWARDZERO, 32) == (unsigned long long)2 );
    test_pass("c2023:ufromfpf");
    test_true( (unsigned long long)ufromfpl(2.5L, FP_INT_TOWARDZERO, 32) == (unsigned long long)2 );
    test_pass("c2023:ufromfpl");
    test_true( (unsigned long long)ufromfpx(2.5, FP_INT_TOWARDZERO, 32) == (unsigned long long)2 );
    test_pass("c2023:ufromfpx");
    test_true( (unsigned long long)ufromfpxf(2.5f, FP_INT_TOWARDZERO, 32) == (unsigned long long)2 );
    test_pass("c2023:ufromfpxf");
    test_true( (unsigned long long)ufromfpxl(2.5L, FP_INT_TOWARDZERO, 32) == (unsigned long long)2 );
    test_pass("c2023:ufromfpxl");
#else
    TEST_NOTE("the C23 math additions are not provided by this library");
    test_skip("c2023:sinpi");
    test_skip("c2023:sinpif");
    test_skip("c2023:sinpil");
    test_skip("c2023:tanpi");
    test_skip("c2023:tanpif");
    test_skip("c2023:tanpil");
    test_skip("c2023:ufromfp");
    test_skip("c2023:ufromfpf");
    test_skip("c2023:ufromfpl");
    test_skip("c2023:ufromfpx");
    test_skip("c2023:ufromfpxf");
    test_skip("c2023:ufromfpxl");
#endif
}

TEST_CASE(math, decimal_floating_point_a_c2023) {
    TEST_NOTE("decimal floating point (__STDC_IEC_60559_DFP__) is absent here");
    test_skip("c2023:opt:DEC_INFINITY");
    test_skip("c2023:opt:DEC_NAN");
    test_skip("c2023:opt:FP_FAST_D32ADDD128");
    test_skip("c2023:opt:FP_FAST_D32ADDD64");
    test_skip("c2023:opt:FP_FAST_D32DIVD128");
    test_skip("c2023:opt:FP_FAST_D32DIVD64");
    test_skip("c2023:opt:FP_FAST_D32FMAD128");
    test_skip("c2023:opt:FP_FAST_D32FMAD64");
    test_skip("c2023:opt:FP_FAST_D32MULD128");
    test_skip("c2023:opt:FP_FAST_D32MULD64");
    test_skip("c2023:opt:FP_FAST_D32SQRTD128");
    test_skip("c2023:opt:FP_FAST_D32SQRTD64");
    test_skip("c2023:opt:FP_FAST_D32SUBD128");
    test_skip("c2023:opt:FP_FAST_D32SUBD64");
    test_skip("c2023:opt:FP_FAST_D64ADDD128");
    test_skip("c2023:opt:FP_FAST_D64DIVD128");
    test_skip("c2023:opt:FP_FAST_D64FMAD128");
    test_skip("c2023:opt:FP_FAST_D64MULD128");
    test_skip("c2023:opt:FP_FAST_D64SQRTD128");
    test_skip("c2023:opt:FP_FAST_D64SUBD128");
    test_skip("c2023:opt:FP_FAST_DADDL");
    test_skip("c2023:opt:FP_FAST_DDIVL");
    test_skip("c2023:opt:FP_FAST_DFMAL");
    test_skip("c2023:opt:FP_FAST_DMULL");
    test_skip("c2023:opt:FP_FAST_DSQRTL");
    test_skip("c2023:opt:FP_FAST_DSUBL");
    test_skip("c2023:opt:FP_FAST_FADD");
    test_skip("c2023:opt:FP_FAST_FADDL");
    test_skip("c2023:opt:FP_FAST_FDIV");
    test_skip("c2023:opt:FP_FAST_FDIVL");
    test_skip("c2023:opt:FP_FAST_FFMA");
    test_skip("c2023:opt:FP_FAST_FFMAL");
    test_skip("c2023:opt:FP_FAST_FMAD128");
    test_skip("c2023:opt:FP_FAST_FMAD32");
    test_skip("c2023:opt:FP_FAST_FMAD64");
    test_skip("c2023:opt:FP_FAST_FMUL");
    test_skip("c2023:opt:FP_FAST_FMULL");
    test_skip("c2023:opt:FP_FAST_FSQRT");
    test_skip("c2023:opt:FP_FAST_FSQRTL");
    test_skip("c2023:opt:FP_FAST_FSUB");
}

TEST_CASE(math, decimal_floating_point_b_c2023) {
    test_skip("c2023:opt:FP_FAST_FSUBL");
    test_skip("c2023:opt:HUGE_VAL_D128");
    test_skip("c2023:opt:HUGE_VAL_D32");
    test_skip("c2023:opt:HUGE_VAL_D64");
    test_skip("c2023:opt:_Decimal32_t");
    test_skip("c2023:opt:_Decimal64_t");
    test_skip("c2023:opt:acosd128");
    test_skip("c2023:opt:acosd32");
    test_skip("c2023:opt:acosd64");
    test_skip("c2023:opt:acoshd128");
    test_skip("c2023:opt:acoshd32");
    test_skip("c2023:opt:acoshd64");
    test_skip("c2023:opt:acospid128");
    test_skip("c2023:opt:acospid32");
    test_skip("c2023:opt:acospid64");
    test_skip("c2023:opt:asind128");
    test_skip("c2023:opt:asind32");
    test_skip("c2023:opt:asind64");
    test_skip("c2023:opt:asinhd128");
    test_skip("c2023:opt:asinhd32");
    test_skip("c2023:opt:asinhd64");
    test_skip("c2023:opt:asinpid128");
    test_skip("c2023:opt:asinpid32");
    test_skip("c2023:opt:asinpid64");
    test_skip("c2023:opt:atan2d128");
    test_skip("c2023:opt:atan2d32");
    test_skip("c2023:opt:atan2d64");
    test_skip("c2023:opt:atan2pid128");
    test_skip("c2023:opt:atan2pid32");
    test_skip("c2023:opt:atan2pid64");
    test_skip("c2023:opt:atand128");
    test_skip("c2023:opt:atand32");
    test_skip("c2023:opt:atand64");
    test_skip("c2023:opt:atanhd128");
    test_skip("c2023:opt:atanhd32");
    test_skip("c2023:opt:atanhd64");
    test_skip("c2023:opt:atanpid128");
    test_skip("c2023:opt:atanpid32");
    test_skip("c2023:opt:atanpid64");
    test_skip("c2023:opt:canonicalized128");
}

TEST_CASE(math, decimal_floating_point_c_c2023) {
    test_skip("c2023:opt:canonicalized32");
    test_skip("c2023:opt:canonicalized64");
    test_skip("c2023:opt:cbrtd128");
    test_skip("c2023:opt:cbrtd32");
    test_skip("c2023:opt:cbrtd64");
    test_skip("c2023:opt:ceild128");
    test_skip("c2023:opt:ceild32");
    test_skip("c2023:opt:ceild64");
    test_skip("c2023:opt:compoundnd128");
    test_skip("c2023:opt:compoundnd32");
    test_skip("c2023:opt:compoundnd64");
    test_skip("c2023:opt:copysignd128");
    test_skip("c2023:opt:copysignd32");
    test_skip("c2023:opt:copysignd64");
    test_skip("c2023:opt:cosd128");
    test_skip("c2023:opt:cosd32");
    test_skip("c2023:opt:cosd64");
    test_skip("c2023:opt:coshd128");
    test_skip("c2023:opt:coshd32");
    test_skip("c2023:opt:coshd64");
    test_skip("c2023:opt:cospid128");
    test_skip("c2023:opt:cospid32");
    test_skip("c2023:opt:cospid64");
    test_skip("c2023:opt:d32addd128");
    test_skip("c2023:opt:d32addd64");
    test_skip("c2023:opt:d32divd128");
    test_skip("c2023:opt:d32divd64");
    test_skip("c2023:opt:d32fmad128");
    test_skip("c2023:opt:d32fmad64");
    test_skip("c2023:opt:d32muld128");
    test_skip("c2023:opt:d32muld64");
    test_skip("c2023:opt:d32sqrtd128");
    test_skip("c2023:opt:d32sqrtd64");
    test_skip("c2023:opt:d32subd128");
    test_skip("c2023:opt:d32subd64");
    test_skip("c2023:opt:d64addd128");
    test_skip("c2023:opt:d64divd128");
    test_skip("c2023:opt:d64fmad128");
    test_skip("c2023:opt:d64muld128");
    test_skip("c2023:opt:d64sqrtd128");
}

TEST_CASE(math, decimal_floating_point_d_c2023) {
    test_skip("c2023:opt:d64subd128");
    test_skip("c2023:opt:decodebind128");
    test_skip("c2023:opt:decodebind32");
    test_skip("c2023:opt:decodebind64");
    test_skip("c2023:opt:decodedecd128");
    test_skip("c2023:opt:decodedecd32");
    test_skip("c2023:opt:decodedecd64");
    test_skip("c2023:opt:encodebind128");
    test_skip("c2023:opt:encodebind32");
    test_skip("c2023:opt:encodebind64");
    test_skip("c2023:opt:encodedecd128");
    test_skip("c2023:opt:encodedecd32");
    test_skip("c2023:opt:encodedecd64");
    test_skip("c2023:opt:erfcd128");
    test_skip("c2023:opt:erfcd32");
    test_skip("c2023:opt:erfcd64");
    test_skip("c2023:opt:erfd128");
    test_skip("c2023:opt:erfd32");
    test_skip("c2023:opt:erfd64");
    test_skip("c2023:opt:exp10d128");
    test_skip("c2023:opt:exp10d32");
    test_skip("c2023:opt:exp10d64");
    test_skip("c2023:opt:exp10m1d128");
    test_skip("c2023:opt:exp10m1d32");
    test_skip("c2023:opt:exp10m1d64");
    test_skip("c2023:opt:exp2d128");
    test_skip("c2023:opt:exp2d32");
    test_skip("c2023:opt:exp2d64");
    test_skip("c2023:opt:exp2m1d128");
    test_skip("c2023:opt:exp2m1d32");
    test_skip("c2023:opt:exp2m1d64");
    test_skip("c2023:opt:expd128");
    test_skip("c2023:opt:expd32");
    test_skip("c2023:opt:expd64");
    test_skip("c2023:opt:expm1d128");
    test_skip("c2023:opt:expm1d32");
    test_skip("c2023:opt:expm1d64");
    test_skip("c2023:opt:fabsd128");
    test_skip("c2023:opt:fabsd32");
    test_skip("c2023:opt:fabsd64");
}

TEST_CASE(math, decimal_floating_point_e_c2023) {
    test_skip("c2023:opt:fdimd128");
    test_skip("c2023:opt:fdimd32");
    test_skip("c2023:opt:fdimd64");
    test_skip("c2023:opt:floord128");
    test_skip("c2023:opt:floord32");
    test_skip("c2023:opt:floord64");
    test_skip("c2023:opt:fmad128");
    test_skip("c2023:opt:fmad32");
    test_skip("c2023:opt:fmad64");
    test_skip("c2023:opt:fmaxd128");
    test_skip("c2023:opt:fmaxd32");
    test_skip("c2023:opt:fmaxd64");
    test_skip("c2023:opt:fmaximum_mag_numd128");
    test_skip("c2023:opt:fmaximum_mag_numd32");
    test_skip("c2023:opt:fmaximum_mag_numd64");
    test_skip("c2023:opt:fmaximum_magd128");
    test_skip("c2023:opt:fmaximum_magd32");
    test_skip("c2023:opt:fmaximum_magd64");
    test_skip("c2023:opt:fmaximum_numd128");
    test_skip("c2023:opt:fmaximum_numd32");
    test_skip("c2023:opt:fmaximum_numd64");
    test_skip("c2023:opt:fmaximumd128");
    test_skip("c2023:opt:fmaximumd32");
    test_skip("c2023:opt:fmaximumd64");
    test_skip("c2023:opt:fmind128");
    test_skip("c2023:opt:fmind32");
    test_skip("c2023:opt:fmind64");
    test_skip("c2023:opt:fminimum_mag_numd128");
    test_skip("c2023:opt:fminimum_mag_numd32");
    test_skip("c2023:opt:fminimum_mag_numd64");
    test_skip("c2023:opt:fminimum_magd128");
    test_skip("c2023:opt:fminimum_magd32");
    test_skip("c2023:opt:fminimum_magd64");
    test_skip("c2023:opt:fminimum_numd128");
    test_skip("c2023:opt:fminimum_numd32");
    test_skip("c2023:opt:fminimum_numd64");
    test_skip("c2023:opt:fminimumd128");
    test_skip("c2023:opt:fminimumd32");
    test_skip("c2023:opt:fminimumd64");
    test_skip("c2023:opt:fmodd128");
}

TEST_CASE(math, decimal_floating_point_f_c2023) {
    test_skip("c2023:opt:fmodd32");
    test_skip("c2023:opt:fmodd64");
    test_skip("c2023:opt:frexpd128");
    test_skip("c2023:opt:frexpd32");
    test_skip("c2023:opt:frexpd64");
    test_skip("c2023:opt:fromfpd128");
    test_skip("c2023:opt:fromfpd32");
    test_skip("c2023:opt:fromfpd64");
    test_skip("c2023:opt:fromfpxd128");
    test_skip("c2023:opt:fromfpxd32");
    test_skip("c2023:opt:fromfpxd64");
    test_skip("c2023:opt:getpayload");
    test_skip("c2023:opt:getpayloadd128");
    test_skip("c2023:opt:getpayloadd32");
    test_skip("c2023:opt:getpayloadd64");
    test_skip("c2023:opt:getpayloadf");
    test_skip("c2023:opt:getpayloadl");
    test_skip("c2023:opt:hypotd128");
    test_skip("c2023:opt:hypotd32");
    test_skip("c2023:opt:hypotd64");
    test_skip("c2023:opt:ilogbd128");
    test_skip("c2023:opt:ilogbd32");
    test_skip("c2023:opt:ilogbd64");
    test_skip("c2023:opt:ldexpd128");
    test_skip("c2023:opt:ldexpd32");
    test_skip("c2023:opt:ldexpd64");
    test_skip("c2023:opt:lgammad128");
    test_skip("c2023:opt:lgammad32");
    test_skip("c2023:opt:lgammad64");
    test_skip("c2023:opt:llogbd128");
    test_skip("c2023:opt:llogbd32");
    test_skip("c2023:opt:llogbd64");
    test_skip("c2023:opt:llquantexpd128");
    test_skip("c2023:opt:llquantexpd32");
    test_skip("c2023:opt:llquantexpd64");
    test_skip("c2023:opt:llrintd128");
    test_skip("c2023:opt:llrintd32");
    test_skip("c2023:opt:llrintd64");
    test_skip("c2023:opt:llroundd128");
    test_skip("c2023:opt:llroundd32");
}

TEST_CASE(math, decimal_floating_point_g_c2023) {
    test_skip("c2023:opt:llroundd64");
    test_skip("c2023:opt:log10d128");
    test_skip("c2023:opt:log10d32");
    test_skip("c2023:opt:log10d64");
    test_skip("c2023:opt:log10p1d128");
    test_skip("c2023:opt:log10p1d32");
    test_skip("c2023:opt:log10p1d64");
    test_skip("c2023:opt:log1pd128");
    test_skip("c2023:opt:log1pd32");
    test_skip("c2023:opt:log1pd64");
    test_skip("c2023:opt:log2d128");
    test_skip("c2023:opt:log2d32");
    test_skip("c2023:opt:log2d64");
    test_skip("c2023:opt:log2p1d128");
    test_skip("c2023:opt:log2p1d32");
    test_skip("c2023:opt:log2p1d64");
    test_skip("c2023:opt:logbd128");
    test_skip("c2023:opt:logbd32");
    test_skip("c2023:opt:logbd64");
    test_skip("c2023:opt:logd128");
    test_skip("c2023:opt:logd32");
    test_skip("c2023:opt:logd64");
    test_skip("c2023:opt:logp1d128");
    test_skip("c2023:opt:logp1d32");
    test_skip("c2023:opt:logp1d64");
    test_skip("c2023:opt:lrintd128");
    test_skip("c2023:opt:lrintd32");
    test_skip("c2023:opt:lrintd64");
    test_skip("c2023:opt:lroundd128");
    test_skip("c2023:opt:lroundd32");
    test_skip("c2023:opt:lroundd64");
    test_skip("c2023:opt:modfd128");
    test_skip("c2023:opt:modfd32");
    test_skip("c2023:opt:modfd64");
    test_skip("c2023:opt:nand128");
    test_skip("c2023:opt:nand32");
    test_skip("c2023:opt:nand64");
    test_skip("c2023:opt:nearbyintd128");
    test_skip("c2023:opt:nearbyintd32");
    test_skip("c2023:opt:nearbyintd64");
}

TEST_CASE(math, decimal_floating_point_h_c2023) {
    test_skip("c2023:opt:nextafterd128");
    test_skip("c2023:opt:nextafterd32");
    test_skip("c2023:opt:nextafterd64");
    test_skip("c2023:opt:nextdownd128");
    test_skip("c2023:opt:nextdownd32");
    test_skip("c2023:opt:nextdownd64");
    test_skip("c2023:opt:nexttowardd128");
    test_skip("c2023:opt:nexttowardd32");
    test_skip("c2023:opt:nexttowardd64");
    test_skip("c2023:opt:nextupd128");
    test_skip("c2023:opt:nextupd32");
    test_skip("c2023:opt:nextupd64");
    test_skip("c2023:opt:powd128");
    test_skip("c2023:opt:powd32");
    test_skip("c2023:opt:powd64");
    test_skip("c2023:opt:pownd128");
    test_skip("c2023:opt:pownd32");
    test_skip("c2023:opt:pownd64");
    test_skip("c2023:opt:powrd128");
    test_skip("c2023:opt:powrd32");
    test_skip("c2023:opt:powrd64");
    test_skip("c2023:opt:quantized128");
    test_skip("c2023:opt:quantized32");
    test_skip("c2023:opt:quantized64");
    test_skip("c2023:opt:quantumd128");
    test_skip("c2023:opt:quantumd32");
    test_skip("c2023:opt:quantumd64");
    test_skip("c2023:opt:remainderd128");
    test_skip("c2023:opt:remainderd32");
    test_skip("c2023:opt:remainderd64");
    test_skip("c2023:opt:rintd128");
    test_skip("c2023:opt:rintd32");
    test_skip("c2023:opt:rintd64");
    test_skip("c2023:opt:rootnd128");
    test_skip("c2023:opt:rootnd32");
    test_skip("c2023:opt:rootnd64");
    test_skip("c2023:opt:roundd128");
    test_skip("c2023:opt:roundd32");
    test_skip("c2023:opt:roundd64");
    test_skip("c2023:opt:roundevend128");
}

TEST_CASE(math, decimal_floating_point_i_c2023) {
    test_skip("c2023:opt:roundevend32");
    test_skip("c2023:opt:roundevend64");
    test_skip("c2023:opt:rsqrtd128");
    test_skip("c2023:opt:rsqrtd32");
    test_skip("c2023:opt:rsqrtd64");
    test_skip("c2023:opt:samequantumd128");
    test_skip("c2023:opt:samequantumd32");
    test_skip("c2023:opt:samequantumd64");
    test_skip("c2023:opt:scalblnd128");
    test_skip("c2023:opt:scalblnd32");
    test_skip("c2023:opt:scalblnd64");
    test_skip("c2023:opt:scalbnd128");
    test_skip("c2023:opt:scalbnd32");
    test_skip("c2023:opt:scalbnd64");
    test_skip("c2023:opt:setpayload");
    test_skip("c2023:opt:setpayloadd128");
    test_skip("c2023:opt:setpayloadd32");
    test_skip("c2023:opt:setpayloadd64");
    test_skip("c2023:opt:setpayloadf");
    test_skip("c2023:opt:setpayloadl");
    test_skip("c2023:opt:setpayloadsig");
    test_skip("c2023:opt:setpayloadsigd128");
    test_skip("c2023:opt:setpayloadsigd32");
    test_skip("c2023:opt:setpayloadsigd64");
    test_skip("c2023:opt:setpayloadsigf");
    test_skip("c2023:opt:setpayloadsigl");
    test_skip("c2023:opt:sind128");
    test_skip("c2023:opt:sind32");
    test_skip("c2023:opt:sind64");
    test_skip("c2023:opt:sinhd128");
    test_skip("c2023:opt:sinhd32");
    test_skip("c2023:opt:sinhd64");
    test_skip("c2023:opt:sinpid128");
    test_skip("c2023:opt:sinpid32");
    test_skip("c2023:opt:sinpid64");
    test_skip("c2023:opt:sqrtd128");
    test_skip("c2023:opt:sqrtd32");
    test_skip("c2023:opt:sqrtd64");
    test_skip("c2023:opt:tand128");
    test_skip("c2023:opt:tand32");
}

TEST_CASE(math, decimal_floating_point_j_c2023) {
    test_skip("c2023:opt:tand64");
    test_skip("c2023:opt:tanhd128");
    test_skip("c2023:opt:tanhd32");
    test_skip("c2023:opt:tanhd64");
    test_skip("c2023:opt:tanpid128");
    test_skip("c2023:opt:tanpid32");
    test_skip("c2023:opt:tanpid64");
    test_skip("c2023:opt:tgammad128");
    test_skip("c2023:opt:tgammad32");
    test_skip("c2023:opt:tgammad64");
    test_skip("c2023:opt:totalorder");
    test_skip("c2023:opt:totalorderd128");
    test_skip("c2023:opt:totalorderd32");
    test_skip("c2023:opt:totalorderd64");
    test_skip("c2023:opt:totalorderf");
    test_skip("c2023:opt:totalorderl");
    test_skip("c2023:opt:totalordermag");
    test_skip("c2023:opt:totalordermagd128");
    test_skip("c2023:opt:totalordermagd32");
    test_skip("c2023:opt:totalordermagd64");
    test_skip("c2023:opt:totalordermagf");
    test_skip("c2023:opt:totalordermagl");
    test_skip("c2023:opt:truncd128");
    test_skip("c2023:opt:truncd32");
    test_skip("c2023:opt:truncd64");
    test_skip("c2023:opt:ufromfpd128");
    test_skip("c2023:opt:ufromfpd32");
    test_skip("c2023:opt:ufromfpd64");
    test_skip("c2023:opt:ufromfpxd128");
    test_skip("c2023:opt:ufromfpxd32");
    test_skip("c2023:opt:ufromfpxd64");
}


TEST_CASE(math, macros_and_c23_rest) {
    test_true( HUGE_VAL > 0.0 );
    TEST_SKIP_PCC("pcc folds HUGE_VAL * 2.0 inconsistently with HUGE_VAL");
    test_true( HUGE_VAL * 2.0 == HUGE_VAL );
    test_pass("c1990:HUGE_VAL");

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    {
#pragma STDC FP_CONTRACT OFF
        test_true( 1 );
    }
    {
#pragma STDC FP_CONTRACT DEFAULT
        test_true( 1 );
    }
    test_pass("c1999:#pragma STDC FP_CONTRACT ON/OFF/DEFAULT");
#else
    TEST_NOTE("the STDC pragmas need C99");
    test_skip("c1999:#pragma STDC FP_CONTRACT ON/OFF/DEFAULT");
#endif

#if defined(__STDC_VERSION_MATH_H__)
    test_true( __STDC_VERSION_MATH_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_MATH_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_MATH_H__");
#endif

#if defined(FP_INT_UPWARD)
    test_true( FP_INT_UPWARD != FP_INT_DOWNWARD );
    test_true( FP_INT_TOWARDZERO != FP_INT_TONEAREST );
    test_true( (long long)fromfp(2.5, FP_INT_UPWARD, 32) == 3LL );
    test_pass("c2023:FP_INT_UPWARD");
    test_true( (long long)fromfp(2.5, FP_INT_DOWNWARD, 32) == 2LL );
    test_pass("c2023:FP_INT_DOWNWARD");
    test_true( (long long)fromfp(-2.5, FP_INT_TOWARDZERO, 32) == -2LL );
    test_pass("c2023:FP_INT_TOWARDZERO");
    test_true( (long long)fromfp(2.5, FP_INT_TONEAREST, 32) == 2LL );
    test_pass("c2023:FP_INT_TONEAREST");
    test_true( (long long)fromfp(2.5, FP_INT_TONEARESTFROMZERO, 32) == 3LL );
    test_pass("c2023:FP_INT_TONEARESTFROMZERO");
#else
    TEST_NOTE("the FP_INT_* directions are C23");
    test_skip("c2023:FP_INT_UPWARD");
    test_skip("c2023:FP_INT_DOWNWARD");
    test_skip("c2023:FP_INT_TOWARDZERO");
    test_skip("c2023:FP_INT_TONEAREST");
    test_skip("c2023:FP_INT_TONEARESTFROMZERO");
#endif

#if defined(FP_LLOGB0)
    test_true( FP_LLOGB0 == LLONG_MIN || FP_LLOGB0 < 0 );
    test_pass("c2023:FP_LLOGB0");
    test_true( FP_LLOGBNAN == LLONG_MIN || FP_LLOGBNAN == LLONG_MAX );
    test_pass("c2023:FP_LLOGBNAN");
#else
    test_skip("c2023:FP_LLOGB0");
    test_skip("c2023:FP_LLOGBNAN");
#endif

#if defined(iszero)
    test_true( iszero(0.0) );
    test_true( !iszero(1.0) );
    test_pass("c2023:iszero");
    test_true( !issubnormal(1.0) );
    test_true( issubnormal(DBL_TRUE_MIN) );
    test_pass("c2023:issubnormal");
    test_true( iseqsig(1.0, 1.0) );
    test_true( !iseqsig(1.0, 2.0) );
    test_pass("c2023:iseqsig");
#else
    TEST_NOTE("iszero / issubnormal / iseqsig are C23");
    test_skip("c2023:iszero");
    test_skip("c2023:issubnormal");
    test_skip("c2023:iseqsig");
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L && defined(_CCW_TARGET_C)
    test_true( nextup(1.0) > 1.0 );
    test_true( nextdown(1.0) < 1.0 );
    test_true( nextdown(nextup(1.0)) == 1.0 );
    test_pass("c2023:nextup");
    test_pass("c2023:nextdown");
    test_true( nextupf(1.0f) > 1.0f );
    test_pass("c2023:nextupf");
    test_true( nextdownf(1.0f) < 1.0f );
    test_pass("c2023:nextdownf");
    test_true( nextupl(1.0L) > 1.0L );
    test_pass("c2023:nextupl");
    test_true( nextdownl(1.0L) < 1.0L );
    test_pass("c2023:nextdownl");

    test_true( fadd(1.0, 2.0) == 3.0f );
    test_pass("c2023:fadd");
    test_true( fsub(3.0, 1.0) == 2.0f );
    test_pass("c2023:fsub");
    test_true( fmul(2.0, 3.0) == 6.0f );
    test_pass("c2023:fmul");
    test_true( fdiv(6.0, 3.0) == 2.0f );
    test_pass("c2023:fdiv");
    test_true( fsqrt(16.0) == 4.0f );
    test_pass("c2023:fsqrt");
    test_true( ffma(2.0, 3.0, 4.0) == 10.0f );
    test_pass("c2023:ffma");
    test_true( faddl(1.0L, 2.0L) == 3.0f );
    test_pass("c2023:faddl");
    test_true( fsubl(3.0L, 1.0L) == 2.0f );
    test_pass("c2023:fsubl");
    test_true( fmull(2.0L, 3.0L) == 6.0f );
    test_pass("c2023:fmull");
    test_true( fsqrtl(16.0L) == 4.0f );
    test_pass("c2023:fsqrtl");
    test_true( ffmal(2.0L, 3.0L, 4.0L) == 10.0f );
    test_pass("c2023:ffmal");
    test_true( daddl(1.0L, 2.0L) == 3.0 );
    test_pass("c2023:daddl");
    test_true( dsubl(3.0L, 1.0L) == 2.0 );
    test_pass("c2023:dsubl");
    test_true( dmull(2.0L, 3.0L) == 6.0 );
    test_pass("c2023:dmull");
    test_true( ddivl(6.0L, 3.0L) == 2.0 );
    test_pass("c2023:ddivl");
    test_true( dsqrtl(16.0L) == 4.0 );
    test_pass("c2023:dsqrtl");
    test_true( dfmal(2.0L, 3.0L, 4.0L) == 10.0 );
    test_pass("c2023:dfmal");

    test_true( fmaximum_mag_numf(-5.0f, 3.0f) == -5.0f );
    test_pass("c2023:fmaximum_mag_numf");
    test_true( fmaximum_mag_numl(-5.0L, 3.0L) == -5.0L );
    test_pass("c2023:fmaximum_mag_numl");
    test_true( fminimum_mag_numf(-5.0f, 3.0f) == 3.0f );
    test_pass("c2023:fminimum_mag_numf");
    test_true( fminimum_mag_numl(-5.0L, 3.0L) == 3.0L );
    test_pass("c2023:fminimum_mag_numl");
#else
    TEST_NOTE("the C23 narrowing arithmetic is not available in this configuration");
    test_skip("c2023:nextup");     test_skip("c2023:nextdown");
    test_skip("c2023:nextupf");    test_skip("c2023:nextdownf");
    test_skip("c2023:nextupl");    test_skip("c2023:nextdownl");
    test_skip("c2023:fadd");       test_skip("c2023:fsub");
    test_skip("c2023:fmul");       test_skip("c2023:fdiv");
    test_skip("c2023:fsqrt");      test_skip("c2023:ffma");
    test_skip("c2023:faddl");      test_skip("c2023:fsubl");
    test_skip("c2023:fmull");      test_skip("c2023:fsqrtl");
    test_skip("c2023:ffmal");      test_skip("c2023:daddl");
    test_skip("c2023:dsubl");      test_skip("c2023:dmull");
    test_skip("c2023:ddivl");      test_skip("c2023:dsqrtl");
    test_skip("c2023:dfmal");
    test_skip("c2023:fmaximum_mag_numf");  test_skip("c2023:fmaximum_mag_numl");
    test_skip("c2023:fminimum_mag_numf");  test_skip("c2023:fminimum_mag_numl");
#endif
}

TEST_SUITE(math)
    TEST_ADD(c89_basic)
    TEST_ADD(c89_trig)
    TEST_ADD(c99_round)
    TEST_ADD(c99_exp_pow)
    TEST_ADD(c99_remainder)
    TEST_ADD(c99_classify)
    TEST_ADD(c99_compare_macros)
    TEST_ADD(c99_float_variants)
    TEST_ADD(c99_macros)
    TEST_ADD(c99_eval_types)
    TEST_ADD(c23_roundeven_extrema)
    TEST_ADD(float_long_double_a_c1999)
    TEST_ADD(float_long_double_b_c1999)
    TEST_ADD(float_long_double_c_c1999)
    TEST_ADD(float_long_double_d_c1999)
    TEST_ADD(float_long_double_e_c1999)
    TEST_ADD(c23_additions_a_c2023)
    TEST_ADD(c23_additions_b_c2023)
    TEST_ADD(c23_additions_c_c2023)
    TEST_ADD(c23_additions_d_c2023)
    TEST_ADD(decimal_floating_point_a_c2023)
    TEST_ADD(decimal_floating_point_b_c2023)
    TEST_ADD(decimal_floating_point_c_c2023)
    TEST_ADD(decimal_floating_point_d_c2023)
    TEST_ADD(decimal_floating_point_e_c2023)
    TEST_ADD(decimal_floating_point_f_c2023)
    TEST_ADD(decimal_floating_point_g_c2023)
    TEST_ADD(decimal_floating_point_h_c2023)
    TEST_ADD(decimal_floating_point_i_c2023)
    TEST_ADD(decimal_floating_point_j_c2023)
    TEST_ADD(macros_and_c23_rest)
TEST_SUITE_END(math)
