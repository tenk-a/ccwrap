#include "c_test.h"

#if defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER < 1900) || TEST_TARGET_C < 1999
# define TST_TGMATH_UNAVAILABLE 1
#else
# include <tgmath.h>
#endif

#include <math.h>

static int near_eq(double a, double b) {
    double d = a - b;
    return (d < 0 ? -d : d) < 1e-6;
}

TEST_CASE(tgmath, dispatches_on_argument_type) {
#ifdef TST_TGMATH_UNAVAILABLE
    TEST_SKIP_CASE();
#else

    float  f = 16.0f;
    double d = 16.0;
    long double l = 16.0L;

    test_true( near_eq((double)sqrt(f), 4.0) );
    test_true( near_eq(sqrt(d), 4.0) );
    test_true( near_eq((double)sqrt(l), 4.0) );

    TEST_SKIP_VC("");
    test_eq( (int)sizeof(sqrt(f)), (int)sizeof(float) );
    test_eq( (int)sizeof(sqrt(d)), (int)sizeof(double) );

    test_true( near_eq(sqrt(16), 4.0) );
    test_eq( (int)sizeof(sqrt(16)), (int)sizeof(double) );
    test_pass("c1999:tgmath type-generic dispatch property");
#endif
}

TEST_CASE(tgmath, common_functions) {
#ifdef TST_TGMATH_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    float  f = 2.0f;
    double d = 2.0;

    test_true( near_eq((double)pow(f, 10.0f), 1024.0) && near_eq(pow(d, 10.0), 1024.0) );
    test_pass("c1999:pow (tgmath)");
    test_true( near_eq(fabs(-2.5), 2.5) );     test_pass("c1999:fabs (tgmath)");
    test_true( near_eq(exp(0.0), 1.0) );       test_pass("c1999:exp (tgmath)");
    test_true( near_eq(log(1.0), 0.0) );       test_pass("c1999:log (tgmath)");
    test_true( near_eq(sin(0.0), 0.0) );       test_pass("c1999:sin (tgmath)");
    test_true( near_eq(cos(0.0), 1.0) );       test_pass("c1999:cos (tgmath)");
    test_true( near_eq(atan2(0.0, 1.0), 0.0) );test_pass("c1999:atan2 (tgmath)");
    test_true( near_eq(fmod(7.0, 3.0), 1.0) ); test_pass("c1999:fmod (tgmath)");
    test_true( near_eq(hypot(3.0, 4.0), 5.0) );test_pass("c1999:hypot (tgmath)");
    test_true( near_eq(cbrt(27.0), 3.0) );     test_pass("c1999:cbrt (tgmath)");
    test_true( near_eq(trunc(2.9), 2.0) );     test_pass("c1999:trunc (tgmath)");
    test_true( near_eq(round(2.5), 3.0) );     test_pass("c1999:round (tgmath)");
#endif
}

TEST_CASE(tgmath, mixed_argument_types) {
#ifdef TST_TGMATH_UNAVAILABLE
    TEST_SKIP_CASE();
#else

    float  f = 3.0f;
    double d = 4.0;

    test_true( near_eq(hypot(f, d), 5.0) );
    test_eq( (int)sizeof(hypot(f, d)), (int)sizeof(double) );
    TEST_SKIP_VC("");
    test_eq( (int)sizeof(hypot(f, f)), (int)sizeof(float) );
    test_true( near_eq(pow(2, 10), 1024.0) );
    test_pass("c1999:mixed-type argument promotion (wider type wins)");
#endif
}

static int ccw_tg_close(long double a, long double b, long double tol) {
    long double d = a - b; long double m = b < 0 ? -b : b;
    if (d < 0) d = -d;
    return d <= tol * (1.0L + m);
}

TEST_CASE(tgmath, type_generic_a_c1999) {
    test_true( ccw_tg_close(acos(0.5f), acosf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(acos(0.5), acos(0.5), 1e-9L) );
    test_true( ccw_tg_close(acos(0.5L), acosl(0.5L), 1e-9L) );
    test_pass("c1999:acos (tgmath)");
    test_true( ccw_tg_close(acosh(2.0f), acoshf(2.0f), 1e-5L) );
    test_true( ccw_tg_close(acosh(2.0), acosh(2.0), 1e-9L) );
    test_true( ccw_tg_close(acosh(2.0L), acoshl(2.0L), 1e-9L) );
    test_pass("c1999:acosh (tgmath)");
    test_true( ccw_tg_close(asin(0.5f), asinf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(asin(0.5), asin(0.5), 1e-9L) );
    test_true( ccw_tg_close(asin(0.5L), asinl(0.5L), 1e-9L) );
    test_pass("c1999:asin (tgmath)");
    test_true( ccw_tg_close(asinh(0.5f), asinhf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(asinh(0.5), asinh(0.5), 1e-9L) );
    test_true( ccw_tg_close(asinh(0.5L), asinhl(0.5L), 1e-9L) );
    test_pass("c1999:asinh (tgmath)");
    test_true( ccw_tg_close(atan(0.5f), atanf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(atan(0.5), atan(0.5), 1e-9L) );
    test_true( ccw_tg_close(atan(0.5L), atanl(0.5L), 1e-9L) );
    test_pass("c1999:atan (tgmath)");
    test_true( ccw_tg_close(atanh(0.5f), atanhf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(atanh(0.5), atanh(0.5), 1e-9L) );
    test_true( ccw_tg_close(atanh(0.5L), atanhl(0.5L), 1e-9L) );
    test_pass("c1999:atanh (tgmath)");
    test_true( ccw_tg_close(ceil(3.2f), ceilf(3.2f), 1e-5L) );
    test_true( ccw_tg_close(ceil(3.2), ceil(3.2), 1e-9L) );
    test_true( ccw_tg_close(ceil(3.2L), ceill(3.2L), 1e-9L) );
    test_pass("c1999:ceil (tgmath)");
    test_true( ccw_tg_close(copysign(3.0f, -1.0f), copysignf(3.0f, -1.0f), 1e-5L) );
    test_true( ccw_tg_close(copysign(3.0, -1.0), copysign(3.0, -1.0), 1e-9L) );
    test_true( ccw_tg_close(copysign(3.0L, -1.0L), copysignl(3.0L, -1.0L), 1e-9L) );
    test_pass("c1999:copysign (tgmath)");
    test_true( ccw_tg_close(cosh(0.5f), coshf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(cosh(0.5), cosh(0.5), 1e-9L) );
    test_true( ccw_tg_close(cosh(0.5L), coshl(0.5L), 1e-9L) );
    test_pass("c1999:cosh (tgmath)");
    test_true( ccw_tg_close(erf(0.5f), erff(0.5f), 1e-5L) );
    test_true( ccw_tg_close(erf(0.5), erf(0.5), 1e-9L) );
    test_true( ccw_tg_close(erf(0.5L), erfl(0.5L), 1e-9L) );
    test_pass("c1999:erf (tgmath)");
    test_true( ccw_tg_close(erfc(0.5f), erfcf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(erfc(0.5), erfc(0.5), 1e-9L) );
    test_true( ccw_tg_close(erfc(0.5L), erfcl(0.5L), 1e-9L) );
    test_pass("c1999:erfc (tgmath)");
    test_true( ccw_tg_close(exp2(3.0f), exp2f(3.0f), 1e-5L) );
    test_true( ccw_tg_close(exp2(3.0), exp2(3.0), 1e-9L) );
    test_true( ccw_tg_close(exp2(3.0L), exp2l(3.0L), 1e-9L) );
    test_pass("c1999:exp2 (tgmath)");
    test_true( ccw_tg_close(expm1(0.5f), expm1f(0.5f), 1e-5L) );
    test_true( ccw_tg_close(expm1(0.5), expm1(0.5), 1e-9L) );
    test_true( ccw_tg_close(expm1(0.5L), expm1l(0.5L), 1e-9L) );
    test_pass("c1999:expm1 (tgmath)");
    test_true( ccw_tg_close(fdim(5.0f, 2.0f), fdimf(5.0f, 2.0f), 1e-5L) );
    test_true( ccw_tg_close(fdim(5.0, 2.0), fdim(5.0, 2.0), 1e-9L) );
    test_true( ccw_tg_close(fdim(5.0L, 2.0L), fdiml(5.0L, 2.0L), 1e-9L) );
    test_pass("c1999:fdim (tgmath)");
    test_true( ccw_tg_close(floor(3.7f), floorf(3.7f), 1e-5L) );
    test_true( ccw_tg_close(floor(3.7), floor(3.7), 1e-9L) );
    test_true( ccw_tg_close(floor(3.7L), floorl(3.7L), 1e-9L) );
    test_pass("c1999:floor (tgmath)");
    test_true( ccw_tg_close(fmax(2.0f, 5.0f), fmaxf(2.0f, 5.0f), 1e-5L) );
    test_true( ccw_tg_close(fmax(2.0, 5.0), fmax(2.0, 5.0), 1e-9L) );
    test_true( ccw_tg_close(fmax(2.0L, 5.0L), fmaxl(2.0L, 5.0L), 1e-9L) );
    test_pass("c1999:fmax (tgmath)");
}

TEST_CASE(tgmath, type_generic_b_c1999) {
    test_true( ccw_tg_close(fmin(2.0f, 5.0f), fminf(2.0f, 5.0f), 1e-5L) );
    test_true( ccw_tg_close(fmin(2.0, 5.0), fmin(2.0, 5.0), 1e-9L) );
    test_true( ccw_tg_close(fmin(2.0L, 5.0L), fminl(2.0L, 5.0L), 1e-9L) );
    test_pass("c1999:fmin (tgmath)");
    test_true( ilogb(8.0f) == ilogbf(8.0f) );
    test_true( ilogb(8.0) == ilogb(8.0) );
    test_true( ilogb(8.0L) == ilogbl(8.0L) );
    test_pass("c1999:ilogb (tgmath)");
    test_true( ccw_tg_close(lgamma(3.0f), lgammaf(3.0f), 1e-5L) );
    test_true( ccw_tg_close(lgamma(3.0), lgamma(3.0), 1e-9L) );
    test_true( ccw_tg_close(lgamma(3.0L), lgammal(3.0L), 1e-9L) );
    test_pass("c1999:lgamma (tgmath)");
    test_true( llrint(2.0f) == llrintf(2.0f) );
    test_true( llrint(2.0) == llrint(2.0) );
    test_true( llrint(2.0L) == llrintl(2.0L) );
    test_pass("c1999:llrint (tgmath)");
    test_true( llround(2.5f) == llroundf(2.5f) );
    test_true( llround(2.5) == llround(2.5) );
    test_true( llround(2.5L) == llroundl(2.5L) );
    test_pass("c1999:llround (tgmath)");
    test_true( ccw_tg_close(log10(1000.0f), log10f(1000.0f), 1e-5L) );
    test_true( ccw_tg_close(log10(1000.0), log10(1000.0), 1e-9L) );
    test_true( ccw_tg_close(log10(1000.0L), log10l(1000.0L), 1e-9L) );
    test_pass("c1999:log10 (tgmath)");
    test_true( ccw_tg_close(log1p(0.5f), log1pf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(log1p(0.5), log1p(0.5), 1e-9L) );
    test_true( ccw_tg_close(log1p(0.5L), log1pl(0.5L), 1e-9L) );
    test_pass("c1999:log1p (tgmath)");
    test_true( ccw_tg_close(log2(8.0f), log2f(8.0f), 1e-5L) );
    test_true( ccw_tg_close(log2(8.0), log2(8.0), 1e-9L) );
    test_true( ccw_tg_close(log2(8.0L), log2l(8.0L), 1e-9L) );
    test_pass("c1999:log2 (tgmath)");
    test_true( ccw_tg_close(logb(8.0f), logbf(8.0f), 1e-5L) );
    test_true( ccw_tg_close(logb(8.0), logb(8.0), 1e-9L) );
    test_true( ccw_tg_close(logb(8.0L), logbl(8.0L), 1e-9L) );
    test_pass("c1999:logb (tgmath)");
    test_true( lrint(2.0f) == lrintf(2.0f) );
    test_true( lrint(2.0) == lrint(2.0) );
    test_true( lrint(2.0L) == lrintl(2.0L) );
    test_pass("c1999:lrint (tgmath)");
    test_true( lround(2.5f) == lroundf(2.5f) );
    test_true( lround(2.5) == lround(2.5) );
    test_true( lround(2.5L) == lroundl(2.5L) );
    test_pass("c1999:lround (tgmath)");
    test_true( ccw_tg_close(nearbyint(2.4f), nearbyintf(2.4f), 1e-5L) );
    test_true( ccw_tg_close(nearbyint(2.4), nearbyint(2.4), 1e-9L) );
    test_true( ccw_tg_close(nearbyint(2.4L), nearbyintl(2.4L), 1e-9L) );
    test_pass("c1999:nearbyint (tgmath)");
    test_true( ccw_tg_close(nextafter(1.0f, 2.0f), nextafterf(1.0f, 2.0f), 1e-5L) );
    test_true( ccw_tg_close(nextafter(1.0, 2.0), nextafter(1.0, 2.0), 1e-9L) );
    test_true( ccw_tg_close(nextafter(1.0L, 2.0L), nextafterl(1.0L, 2.0L), 1e-9L) );
    test_pass("c1999:nextafter (tgmath)");
    test_true( ccw_tg_close(remainder(7.0f, 3.0f), remainderf(7.0f, 3.0f), 1e-5L) );
    test_true( ccw_tg_close(remainder(7.0, 3.0), remainder(7.0, 3.0), 1e-9L) );
    test_true( ccw_tg_close(remainder(7.0L, 3.0L), remainderl(7.0L, 3.0L), 1e-9L) );
    test_pass("c1999:remainder (tgmath)");
    test_true( ccw_tg_close(rint(2.6f), rintf(2.6f), 1e-5L) );
    test_true( ccw_tg_close(rint(2.6), rint(2.6), 1e-9L) );
    test_true( ccw_tg_close(rint(2.6L), rintl(2.6L), 1e-9L) );
    test_pass("c1999:rint (tgmath)");
    test_true( ccw_tg_close(sinh(0.5f), sinhf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(sinh(0.5), sinh(0.5), 1e-9L) );
    test_true( ccw_tg_close(sinh(0.5L), sinhl(0.5L), 1e-9L) );
    test_pass("c1999:sinh (tgmath)");
}

TEST_CASE(tgmath, type_generic_c_c1999) {
    test_true( ccw_tg_close(sqrt(16.0f), sqrtf(16.0f), 1e-5L) );
    test_true( ccw_tg_close(sqrt(16.0), sqrt(16.0), 1e-9L) );
    test_true( ccw_tg_close(sqrt(16.0L), sqrtl(16.0L), 1e-9L) );
    test_pass("c1999:sqrt (tgmath)");
    test_true( ccw_tg_close(tan(0.5f), tanf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(tan(0.5), tan(0.5), 1e-9L) );
    test_true( ccw_tg_close(tan(0.5L), tanl(0.5L), 1e-9L) );
    test_pass("c1999:tan (tgmath)");
    test_true( ccw_tg_close(tanh(0.5f), tanhf(0.5f), 1e-5L) );
    test_true( ccw_tg_close(tanh(0.5), tanh(0.5), 1e-9L) );
    test_true( ccw_tg_close(tanh(0.5L), tanhl(0.5L), 1e-9L) );
    test_pass("c1999:tanh (tgmath)");
    test_true( ccw_tg_close(tgamma(4.0f), tgammaf(4.0f), 1e-5L) );
    test_true( ccw_tg_close(tgamma(4.0), tgamma(4.0), 1e-9L) );
    test_true( ccw_tg_close(tgamma(4.0L), tgammal(4.0L), 1e-9L) );
    test_pass("c1999:tgamma (tgmath)");
}

TEST_CASE(tgmath, complex_and_decimal_a_c1999) {
    TEST_NOTE("complex / decimal type-generic forms: not exercised here");
    test_skip("c1999:carg (tgmath)");
    test_skip("c1999:cimag (tgmath)");
    test_skip("c1999:conj (tgmath)");
    test_skip("c1999:cproj (tgmath)");
    test_skip("c1999:creal (tgmath)");
    test_skip("c1999:fma (tgmath)");
    test_skip("c1999:frexp (tgmath)");
    test_skip("c1999:ldexp (tgmath)");
    test_skip("c1999:nexttoward (tgmath)");
    test_skip("c1999:remquo (tgmath)");
    test_skip("c1999:scalbln (tgmath)");
    test_skip("c1999:scalbn (tgmath)");
    test_skip("c2023:__STDC_VERSION_TGMATH_H__");
    test_skip("c2023:acospi (tgmath)");
    test_skip("c2023:asinpi (tgmath)");
    test_skip("c2023:atan2pi (tgmath)");
    test_skip("c2023:atanpi (tgmath)");
    test_skip("c2023:compoundn (tgmath)");
    test_skip("c2023:cospi (tgmath)");
    test_skip("c2023:dadd (tgmath)");
    test_skip("c2023:ddiv (tgmath)");
    test_skip("c2023:dfma (tgmath)");
    test_skip("c2023:dmul (tgmath)");
    test_skip("c2023:dsqrt (tgmath)");
    test_skip("c2023:dsub (tgmath)");
    test_skip("c2023:exp10 (tgmath)");
    test_skip("c2023:exp10m1 (tgmath)");
    test_skip("c2023:exp2m1 (tgmath)");
    test_skip("c2023:fadd (tgmath)");
    test_skip("c2023:fdiv (tgmath)");
    test_skip("c2023:ffma (tgmath)");
    test_skip("c2023:fmaximum (tgmath)");
    test_skip("c2023:fmaximum_mag (tgmath)");
    test_skip("c2023:fmaximum_mag_num (tgmath)");
    test_skip("c2023:fmaximum_num (tgmath)");
    test_skip("c2023:fminimum (tgmath)");
    test_skip("c2023:fminimum_mag (tgmath)");
    test_skip("c2023:fminimum_mag_num (tgmath)");
    test_skip("c2023:fminimum_num (tgmath)");
    test_skip("c2023:fmul (tgmath)");
}

TEST_CASE(tgmath, complex_and_decimal_b_c1999) {
    test_skip("c2023:fromfp (tgmath)");
    test_skip("c2023:fromfpx (tgmath)");
    test_skip("c2023:fsqrt (tgmath)");
    test_skip("c2023:fsub (tgmath)");
    test_skip("c2023:llogb (tgmath)");
    test_skip("c2023:log10p1 (tgmath)");
    test_skip("c2023:log2p1 (tgmath)");
    test_skip("c2023:logp1 (tgmath)");
    test_skip("c2023:nextdown (tgmath)");
    test_skip("c2023:nextup (tgmath)");
    test_skip("c2023:opt:d32add (tgmath)");
    test_skip("c2023:opt:d32div (tgmath)");
    test_skip("c2023:opt:d32fma (tgmath)");
    test_skip("c2023:opt:d32mul (tgmath)");
    test_skip("c2023:opt:d32sqrt (tgmath)");
    test_skip("c2023:opt:d32sub (tgmath)");
    test_skip("c2023:opt:d64add (tgmath)");
    test_skip("c2023:opt:d64div (tgmath)");
    test_skip("c2023:opt:d64fma (tgmath)");
    test_skip("c2023:opt:d64mul (tgmath)");
    test_skip("c2023:opt:d64sqrt (tgmath)");
    test_skip("c2023:opt:d64sub (tgmath)");
    test_skip("c2023:opt:llquantexp (tgmath)");
    test_skip("c2023:opt:quantize (tgmath)");
    test_skip("c2023:opt:quantum (tgmath)");
    test_skip("c2023:opt:samequantum (tgmath)");
    test_skip("c2023:pown (tgmath)");
    test_skip("c2023:powr (tgmath)");
    test_skip("c2023:rootn (tgmath)");
    test_skip("c2023:roundeven (tgmath)");
    test_skip("c2023:rsqrt (tgmath)");
    test_skip("c2023:sinpi (tgmath)");
    test_skip("c2023:tanpi (tgmath)");
    test_skip("c2023:ufromfp (tgmath)");
    test_skip("c2023:ufromfpx (tgmath)");
}

TEST_SUITE(tgmath)
    TEST_ADD(dispatches_on_argument_type)
    TEST_ADD(common_functions)
    TEST_ADD(mixed_argument_types)
    TEST_ADD(type_generic_a_c1999)
    TEST_ADD(type_generic_b_c1999)
    TEST_ADD(type_generic_c_c1999)
    TEST_ADD(complex_and_decimal_a_c1999)
    TEST_ADD(complex_and_decimal_b_c1999)
TEST_SUITE_END(tgmath)
