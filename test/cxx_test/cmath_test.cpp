#include "test_cxx.hpp"

#include <cmath>

namespace { inline bool near_eq(double a, double b) { double d = a - b; return (d < 0 ? -d : d) < 1e-9; } }

TEST_CASE(cmath, c89_basic) {
    test_true( near_eq(STD_NS sqrt(16.0), 4.0) );
    test_pass("cxx03:sqrt");
    test_true( near_eq(STD_NS pow(2.0, 10.0), 1024.0) );
    test_pass("cxx03:pow");
    test_true( near_eq(STD_NS fabs(-2.5), 2.5) );
    test_pass("cxx03:fabs");
    test_true( near_eq(STD_NS floor(3.7), 3.0) );
    test_pass("cxx03:floor");
    test_true( near_eq(STD_NS ceil(3.2), 4.0) );
    test_pass("cxx03:ceil");
    test_true( near_eq(STD_NS fmod(7.0, 3.0), 1.0) );
    test_pass("cxx03:fmod");
    test_true( near_eq(STD_NS exp(0.0), 1.0) );
    test_pass("cxx03:exp");
    test_true( near_eq(STD_NS log(1.0), 0.0) );
    test_pass("cxx03:log");
    test_true( near_eq(STD_NS log10(1000.0), 3.0) );
    test_pass("cxx03:log10");

    double ip = 0;
    double fr = STD_NS modf(3.25, &ip);
    test_true( near_eq(ip, 3.0) && near_eq(fr, 0.25) );
    test_pass("cxx03:modf");
    int ex = 0;
    double mant = STD_NS frexp(8.0, &ex);
    test_true( near_eq(mant, 0.5) && ex == 4 );
    test_pass("cxx03:frexp");
    test_true( near_eq(STD_NS ldexp(0.5, 4), 8.0) );
    test_pass("cxx03:ldexp");
}

TEST_CASE(cmath, c89_trig) {
    test_true( near_eq(STD_NS sin(0.0), 0.0) );
    test_pass("cxx03:sin");
    test_true( near_eq(STD_NS cos(0.0), 1.0) );
    test_pass("cxx03:cos");
    test_true( near_eq(STD_NS tan(0.0), 0.0) );
    test_pass("cxx03:tan");
    test_true( near_eq(STD_NS asin(0.0), 0.0) );
    test_pass("cxx03:asin");
    test_true( near_eq(STD_NS acos(1.0), 0.0) );
    test_pass("cxx03:acos");
    test_true( near_eq(STD_NS atan(0.0), 0.0) );
    test_pass("cxx03:atan");
    test_true( near_eq(STD_NS atan2(0.0, 1.0), 0.0) );
    test_pass("cxx03:atan2");
    test_true( near_eq(STD_NS sinh(0.0), 0.0) );
    test_pass("cxx03:sinh");
    test_true( near_eq(STD_NS cosh(0.0), 1.0) );
    test_pass("cxx03:cosh");
    test_true( near_eq(STD_NS tanh(0.0), 0.0) );
    test_pass("cxx03:tanh");
}

TEST_CASE(cmath, c99_round) {
    test_true( near_eq(STD_NS round(2.5), 3.0) );
    test_true( near_eq(STD_NS round(-2.5), -3.0) );
    test_pass("cxx11:round");
    test_true( near_eq(STD_NS trunc(2.9), 2.0) );
    test_true( near_eq(STD_NS trunc(-2.9), -2.0) );
    test_pass("cxx11:trunc");
    test_eq( (long)STD_NS lround(2.5), 3L );
    test_pass("cxx11:lround");
    test_eq( (long)STD_NS lrint(2.0), 2L );
    test_pass("cxx11:lrint");
    test_true( near_eq(STD_NS nearbyint(2.4), 2.0) );
    test_pass("cxx11:nearbyint");
    test_true( near_eq(STD_NS rint(2.6), 3.0) );
    test_pass("cxx11:rint");
    test_true( near_eq(STD_NS fdim(5.0, 2.0), 3.0) );
    test_true( near_eq(STD_NS fdim(2.0, 5.0), 0.0) );
    test_pass("cxx11:fdim");
}

TEST_CASE(cmath, c99_exp_pow) {
    test_true( near_eq(STD_NS cbrt(27.0), 3.0) );
    test_pass("cxx11:cbrt");
    test_true( near_eq(STD_NS log2(8.0), 3.0) );
    test_pass("cxx11:log2");
    test_true( near_eq(STD_NS exp2(4.0), 16.0) );
    test_pass("cxx11:exp2");
    test_true( near_eq(STD_NS expm1(0.0), 0.0) );
    test_pass("cxx11:expm1");
    test_true( near_eq(STD_NS log1p(0.0), 0.0) );
    test_pass("cxx11:log1p");
    test_true( near_eq(STD_NS hypot(3.0, 4.0), 5.0) );
    test_pass("cxx11:hypot");
    test_true( near_eq(STD_NS fma(2.0, 3.0, 4.0), 10.0) );
    test_pass("cxx11:fma");
    test_true( near_eq(STD_NS fmax(2.0, 5.0), 5.0) );
    test_pass("cxx11:fmax");
    test_true( near_eq(STD_NS fmin(2.0, 5.0), 2.0) );
    test_pass("cxx11:fmin");
    test_true( near_eq(STD_NS copysign(3.0, -1.0), -3.0) );
    test_pass("cxx11:copysign");
    test_true( near_eq(STD_NS scalbn(1.0, 3), 8.0) );
    test_pass("cxx11:scalbn");
    test_true( near_eq(STD_NS acosh(1.0), 0.0) );
    test_pass("cxx11:acosh");
    test_true( near_eq(STD_NS asinh(0.0), 0.0) );
    test_pass("cxx11:asinh");
    test_true( near_eq(STD_NS atanh(0.0), 0.0) );
    test_pass("cxx11:atanh");
    test_true( near_eq(STD_NS remainder(7.0, 3.0), 1.0) );
    test_pass("cxx11:remainder");
}

TEST_CASE(cmath, c99_classify) {
    test_true(  STD_NS isnan(STD_NS nan("")) );
    test_pass("cxx11:isnan");
    test_true(  STD_NS isinf(HUGE_VAL) );
    test_true( !STD_NS isinf(1.0) );
    test_pass("cxx11:isinf");
    test_true(  STD_NS isfinite(1.0) );
    test_pass("cxx11:isfinite");
    test_true(  STD_NS isnormal(1.0) );
    test_pass("cxx11:isnormal");
    test_true( !STD_NS signbit(1.0) );
    test_true(  STD_NS signbit(-1.0) );
    test_pass("cxx11:signbit");
    test_true(  STD_NS isgreater(2.0, 1.0) );
    test_pass("cxx11:isgreater");
    test_true(  STD_NS isless(1.0, 2.0) );
    test_pass("cxx11:isless");
    test_eq( (int)STD_NS ilogb(8.0), 3 );
    test_pass("cxx11:ilogb");
}

TEST_CASE(cmath, classification_macros) {

    test_eq( STD_NS fpclassify(0.0), FP_ZERO );
    test_pass("cxx11:FP_ZERO");
    test_eq( STD_NS fpclassify(1.0), FP_NORMAL );
    test_eq( STD_NS fpclassify(HUGE_VAL), FP_INFINITE );
    test_pass("cxx11:FP_INFINITE");

    int nan_cat = FP_NAN, sub_cat = FP_SUBNORMAL;
    test_true( nan_cat != FP_ZERO && nan_cat != FP_NORMAL && nan_cat != FP_INFINITE );
    test_pass("cxx11:FP_NAN");
    test_true( sub_cat != FP_ZERO && sub_cat != FP_NORMAL && sub_cat != FP_NAN );
    test_pass("cxx11:FP_SUBNORMAL");

    int ilbn = FP_ILOGBNAN;
    (void)ilbn;
    test_true( ilbn == FP_ILOGBNAN );
    test_pass("cxx11:FP_ILOGBNAN");

#ifdef FP_FAST_FMAF
    test_eq( FP_FAST_FMAF, 1 );
    test_pass("cxx11:opt:FP_FAST_FMAF");
#else
    TEST_SKIP1();
    test_skip("cxx11:opt:FP_FAST_FMAF");
#endif
#ifdef FP_FAST_FMAL
    test_eq( FP_FAST_FMAL, 1 );
    test_pass("cxx11:opt:FP_FAST_FMAL");
#else
    TEST_SKIP1();
    test_skip("cxx11:opt:FP_FAST_FMAL");
#endif
}

#if TEST_TARGET_CXX >= 2020
TEST_CASE(cmath, lerp) {
    test_true( near_eq(STD::lerp(0.0, 10.0, 0.5), 5.0) );
    test_true( near_eq(STD::lerp(0.0, 10.0, 0.0), 0.0) );
    test_true( near_eq(STD::lerp(0.0, 10.0, 1.0), 10.0) );
    test_true( near_eq(STD::lerp(2.0, 2.0, 0.5), 2.0) );
    test_pass("cxx20:lerp");
}
#endif

#if TEST_TARGET_CXX >= 2026 && defined(_CCW_DETAIL_MATH_C23_H)
TEST_CASE(cmath, c23_extrema_roundeven) {
    double __nan = STD_NS nan("");
    double __nz = STD_NS copysign(0.0, -1.0);

    test_true( STD_NS roundeven(2.5) == 2.0 );
    test_true( STD_NS roundeven(3.5) == 4.0 );
    test_true( STD_NS roundeven(-2.5) == -2.0 );
    test_true( STD_NS roundeven(2.4) == 2.0 );
    test_true( STD_NS roundeven(2.6) == 3.0 );
    test_true( STD_NS roundevenf(1.5f) == 2.0f );
    test_pass("cxx26:roundeven");

    test_true( STD_NS fmaximum(3.0, 5.0) == 5.0 );
    test_true( STD_NS isnan(STD_NS fmaximum(__nan, 5.0)) );
    test_true( STD_NS signbit(__nz) );
    test_true( !STD_NS signbit(STD_NS fmaximum(__nz, 0.0)) );
    test_true( STD_NS fmaximumf(1.0f, 2.0f) == 2.0f );
    test_pass("cxx26:fmaximum");

    test_true( STD_NS fminimum(3.0, 5.0) == 3.0 );
    test_true( STD_NS isnan(STD_NS fminimum(3.0, __nan)) );
    test_true( STD_NS signbit(STD_NS fminimum(0.0, __nz)) );
    test_pass("cxx26:fminimum");

    test_true( STD_NS fmaximum_num(__nan, 5.0) == 5.0 );
    test_true( STD_NS isnan(STD_NS fmaximum_num(__nan, __nan)) );
    test_pass("cxx26:fmaximum_num");

    test_true( STD_NS fminimum_num(__nan, 5.0) == 5.0 );
    test_pass("cxx26:fminimum_num");

    test_true( STD_NS fmaximum_mag(-5.0, 3.0) == -5.0 );
    test_pass("cxx26:fmaximum_mag");

    test_true( STD_NS fminimum_mag(-5.0, 3.0) == 3.0 );
    test_pass("cxx26:fminimum_mag");

    test_true( !STD_NS issignaling(5.0) );
    test_true( !STD_NS issignaling(__nan) );
    test_pass("cxx26:issignaling");

    test_true( STD_NS iscanonical(5.0) );
    test_pass("cxx26:iscanonical");
}
#else
TEST_CASE_SKIP(cmath, c23_extrema_roundeven)
#endif

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_SPECIAL_MATH
TEST_CASE(cmath, special_polynomials_and_beta) {

    test_true( near_eq(STD::hermite(0, 5.0), 1.0) );
    test_true( near_eq(STD::hermite(1, 3.0), 6.0) );
    test_true( near_eq(STD::hermite(2, 1.0), 2.0) );
    test_true( near_eq(STD::hermite(3, 1.0), -4.0) );
    test_true( near_eq((double)STD::hermitef(2, 1.0f), 2.0) );
    test_pass("cxx17:hermite");

    test_true( near_eq(STD::laguerre(0, 7.0), 1.0) );
    test_true( near_eq(STD::laguerre(1, 2.0), -1.0) );
    test_true( near_eq(STD::laguerre(2, 1.0), -0.5) );
    test_pass("cxx17:laguerre");

    test_true( near_eq(STD::legendre(1, 0.7), 0.7) );
    test_true( near_eq(STD::legendre(2, 1.0), 1.0) );
    test_true( near_eq(STD::legendre(2, 0.5), -0.125) );
    test_true( near_eq(STD::legendre(3, 0.5), -0.4375) );
    test_true( near_eq((double)STD::legendrel(2, 0.5L), -0.125) );
    test_pass("cxx17:legendre");

    test_true( near_eq(STD::assoc_laguerre(0, 2, 3.0), 1.0) );
    test_true( near_eq(STD::assoc_laguerre(1, 0, 0.25), 0.75) );
    test_true( near_eq(STD::assoc_laguerre(1, 1, 1.0), 1.0) );
    test_pass("cxx17:assoc_laguerre");

    test_true( near_eq(STD::assoc_legendre(1, 0, 0.5), 0.5) );
    test_true( near_eq(STD::assoc_legendre(2, 0, 0.5), -0.125) );
    test_true( near_eq(STD::assoc_legendre(1, 1, 0.0), 1.0) );
    test_true( near_eq(STD::assoc_legendre(1, 1, 0.5), STD_NS sqrt(0.75)) );
    test_pass("cxx17:assoc_legendre");

    test_true( near_eq(STD::beta(1.0, 1.0), 1.0) );
    test_true( near_eq(STD::beta(2.0, 3.0), 1.0 / 12.0) );
    test_pass("cxx17:beta");
}
#else
TEST_CASE_SKIP(cmath, special_polynomials_and_beta)
#endif

namespace {
inline bool ccw_cm_close(double a, double b, double tol) {
    double d = a - b; if (d < 0) d = -d;
    double m = b < 0 ? -b : b;
    return d <= tol * (1.0 + m);
}
}

TEST_CASE(cmath, float_long_double_overloads_a_cxx11) {
    test_true( ccw_cm_close((double)STD_NS acosf(0.5f), STD_NS acos(0.5), 1e-5) );
    test_pass("cxx11:acosf");
    test_true( ccw_cm_close((double)STD_NS acosl(0.5L), STD_NS acos(0.5), 1e-9) );
    test_pass("cxx11:acosl");
    test_true( ccw_cm_close((double)STD_NS asinf(0.5f), STD_NS asin(0.5), 1e-5) );
    test_pass("cxx11:asinf");
    test_true( ccw_cm_close((double)STD_NS asinl(0.5L), STD_NS asin(0.5), 1e-9) );
    test_pass("cxx11:asinl");
    test_true( ccw_cm_close((double)STD_NS atan2f(1.0f, 2.0f), STD_NS atan2(1.0, 2.0), 1e-5) );
    test_pass("cxx11:atan2f");
    test_true( ccw_cm_close((double)STD_NS atan2l(1.0L, 2.0L), STD_NS atan2(1.0, 2.0), 1e-9) );
    test_pass("cxx11:atan2l");
    test_true( ccw_cm_close((double)STD_NS atanf(0.5f), STD_NS atan(0.5), 1e-5) );
    test_pass("cxx11:atanf");
    test_true( ccw_cm_close((double)STD_NS atanl(0.5L), STD_NS atan(0.5), 1e-9) );
    test_pass("cxx11:atanl");
    test_true( ccw_cm_close((double)STD_NS cbrtf(27.0f), STD_NS cbrt(27.0), 1e-5) );
    test_pass("cxx11:cbrtf");
    test_true( ccw_cm_close((double)STD_NS cbrtl(27.0L), STD_NS cbrt(27.0), 1e-9) );
    test_pass("cxx11:cbrtl");
    test_true( ccw_cm_close((double)STD_NS ceilf(3.2f), STD_NS ceil(3.2), 1e-5) );
    test_pass("cxx11:ceilf");
    test_true( ccw_cm_close((double)STD_NS ceill(3.2L), STD_NS ceil(3.2), 1e-9) );
    test_pass("cxx11:ceill");
    test_true( ccw_cm_close((double)STD_NS copysignf(3.0f, -1.0f), STD_NS copysign(3.0, -1.0), 1e-5) );
    test_pass("cxx11:copysignf");
    test_true( ccw_cm_close((double)STD_NS copysignl(3.0L, -1.0L), STD_NS copysign(3.0, -1.0), 1e-9) );
    test_pass("cxx11:copysignl");
    test_true( ccw_cm_close((double)STD_NS cosf(0.5f), STD_NS cos(0.5), 1e-5) );
    test_pass("cxx11:cosf");
    test_true( ccw_cm_close((double)STD_NS coshf(0.5f), STD_NS cosh(0.5), 1e-5) );
    test_pass("cxx11:coshf");
    test_true( ccw_cm_close((double)STD_NS coshl(0.5L), STD_NS cosh(0.5), 1e-9) );
    test_pass("cxx11:coshl");
    test_true( ccw_cm_close((double)STD_NS cosl(0.5L), STD_NS cos(0.5), 1e-9) );
    test_pass("cxx11:cosl");
    test_true( ccw_cm_close((double)STD_NS erfcf(0.5f), STD_NS erfc(0.5), 1e-5) );
    test_pass("cxx11:erfcf");
    test_true( ccw_cm_close((double)STD_NS erfcl(0.5L), STD_NS erfc(0.5), 1e-9) );
    test_pass("cxx11:erfcl");
    test_true( ccw_cm_close((double)STD_NS erff(0.5f), STD_NS erf(0.5), 1e-5) );
    test_pass("cxx11:erff");
    test_true( ccw_cm_close((double)STD_NS erfl(0.5L), STD_NS erf(0.5), 1e-9) );
    test_pass("cxx11:erfl");
}

TEST_CASE(cmath, float_long_double_overloads_b_cxx11) {
    test_true( ccw_cm_close((double)STD_NS exp2f(3.0f), STD_NS exp2(3.0), 1e-5) );
    test_pass("cxx11:exp2f");
    test_true( ccw_cm_close((double)STD_NS exp2l(3.0L), STD_NS exp2(3.0), 1e-9) );
    test_pass("cxx11:exp2l");
    test_true( ccw_cm_close((double)STD_NS expf(1.0f), STD_NS exp(1.0), 1e-5) );
    test_pass("cxx11:expf");
    test_true( ccw_cm_close((double)STD_NS expl(1.0L), STD_NS exp(1.0), 1e-9) );
    test_pass("cxx11:expl");
    test_true( ccw_cm_close((double)STD_NS expm1f(0.5f), STD_NS expm1(0.5), 1e-5) );
    test_pass("cxx11:expm1f");
    test_true( ccw_cm_close((double)STD_NS expm1l(0.5L), STD_NS expm1(0.5), 1e-9) );
    test_pass("cxx11:expm1l");
    test_true( ccw_cm_close((double)STD_NS fabsf(-2.5f), STD_NS fabs(-2.5), 1e-5) );
    test_pass("cxx11:fabsf");
    test_true( ccw_cm_close((double)STD_NS fabsl(-2.5L), STD_NS fabs(-2.5), 1e-9) );
    test_pass("cxx11:fabsl");
    test_true( ccw_cm_close((double)STD_NS fdimf(5.0f, 2.0f), STD_NS fdim(5.0, 2.0), 1e-5) );
    test_pass("cxx11:fdimf");
    test_true( ccw_cm_close((double)STD_NS fdiml(5.0L, 2.0L), STD_NS fdim(5.0, 2.0), 1e-9) );
    test_pass("cxx11:fdiml");
    test_true( ccw_cm_close((double)STD_NS floorf(3.7f), STD_NS floor(3.7), 1e-5) );
    test_pass("cxx11:floorf");
    test_true( ccw_cm_close((double)STD_NS floorl(3.7L), STD_NS floor(3.7), 1e-9) );
    test_pass("cxx11:floorl");
    test_true( ccw_cm_close((double)STD_NS fmaf(2.0f, 3.0f, 4.0f), STD_NS fma(2.0, 3.0, 4.0), 1e-5) );
    test_pass("cxx11:fmaf");
    test_true( ccw_cm_close((double)STD_NS fmal(2.0L, 3.0L, 4.0L), STD_NS fma(2.0, 3.0, 4.0), 1e-9) );
    test_pass("cxx11:fmal");
    test_true( ccw_cm_close((double)STD_NS fmaxf(2.0f, 5.0f), STD_NS fmax(2.0, 5.0), 1e-5) );
    test_pass("cxx11:fmaxf");
    test_true( ccw_cm_close((double)STD_NS fmaxl(2.0L, 5.0L), STD_NS fmax(2.0, 5.0), 1e-9) );
    test_pass("cxx11:fmaxl");
    test_true( ccw_cm_close((double)STD_NS fminf(2.0f, 5.0f), STD_NS fmin(2.0, 5.0), 1e-5) );
    test_pass("cxx11:fminf");
    test_true( ccw_cm_close((double)STD_NS fminl(2.0L, 5.0L), STD_NS fmin(2.0, 5.0), 1e-9) );
    test_pass("cxx11:fminl");
    test_true( ccw_cm_close((double)STD_NS fmodf(7.0f, 3.0f), STD_NS fmod(7.0, 3.0), 1e-5) );
    test_pass("cxx11:fmodf");
    test_true( ccw_cm_close((double)STD_NS fmodl(7.0L, 3.0L), STD_NS fmod(7.0, 3.0), 1e-9) );
    test_pass("cxx11:fmodl");
    { int e = 0; float m = STD_NS frexpf(8.0f, &e); test_true( ccw_cm_close((double)m, 0.5, 1e-5) && e == 4 ); }
    test_pass("cxx11:frexpf");
    { int e = 0; long double m = STD_NS frexpl(8.0L, &e); test_true( ccw_cm_close((double)m, 0.5, 1e-9) && e == 4 ); }
    test_pass("cxx11:frexpl");
}

TEST_CASE(cmath, float_long_double_overloads_c_cxx11) {
    test_true( ccw_cm_close((double)STD_NS hypotf(3.0f, 4.0f), STD_NS hypot(3.0, 4.0), 1e-5) );
    test_pass("cxx11:hypotf");
    test_true( ccw_cm_close((double)STD_NS hypotl(3.0L, 4.0L), STD_NS hypot(3.0, 4.0), 1e-9) );
    test_pass("cxx11:hypotl");
    test_eq( STD_NS ilogbf(8.0f), STD_NS ilogb(8.0) );
    test_pass("cxx11:ilogbf");
    test_eq( STD_NS ilogbl(8.0L), STD_NS ilogb(8.0) );
    test_pass("cxx11:ilogbl");
    test_true( ccw_cm_close((double)STD_NS ldexpf(0.5f, 4), 8.0, 1e-5) );
    test_pass("cxx11:ldexpf");
    test_true( ccw_cm_close((double)STD_NS ldexpl(0.5L, 4), 8.0, 1e-9) );
    test_pass("cxx11:ldexpl");
    test_true( ccw_cm_close((double)STD_NS lgammaf(3.0f), STD_NS lgamma(3.0), 1e-5) );
    test_pass("cxx11:lgammaf");
    test_true( ccw_cm_close((double)STD_NS lgammal(3.0L), STD_NS lgamma(3.0), 1e-9) );
    test_pass("cxx11:lgammal");
    test_true( STD_NS llrintf(2.0f) == STD_NS llrint(2.0) );
    test_pass("cxx11:llrintf");
    test_true( STD_NS llrintl(2.0L) == STD_NS llrint(2.0) );
    test_pass("cxx11:llrintl");
    test_true( STD_NS llroundf(2.5f) == STD_NS llround(2.5) );
    test_pass("cxx11:llroundf");
    test_true( STD_NS llroundl(2.5L) == STD_NS llround(2.5) );
    test_pass("cxx11:llroundl");
    test_true( ccw_cm_close((double)STD_NS log10f(1000.0f), STD_NS log10(1000.0), 1e-5) );
    test_pass("cxx11:log10f");
    test_true( ccw_cm_close((double)STD_NS log10l(1000.0L), STD_NS log10(1000.0), 1e-9) );
    test_pass("cxx11:log10l");
    test_true( ccw_cm_close((double)STD_NS log1pf(0.5f), STD_NS log1p(0.5), 1e-5) );
    test_pass("cxx11:log1pf");
    test_true( ccw_cm_close((double)STD_NS log1pl(0.5L), STD_NS log1p(0.5), 1e-9) );
    test_pass("cxx11:log1pl");
    test_true( ccw_cm_close((double)STD_NS log2f(8.0f), STD_NS log2(8.0), 1e-5) );
    test_pass("cxx11:log2f");
    test_true( ccw_cm_close((double)STD_NS log2l(8.0L), STD_NS log2(8.0), 1e-9) );
    test_pass("cxx11:log2l");
    test_true( ccw_cm_close((double)STD_NS logbf(8.0f), STD_NS logb(8.0), 1e-5) );
    test_pass("cxx11:logbf");
    test_true( ccw_cm_close((double)STD_NS logbl(8.0L), STD_NS logb(8.0), 1e-9) );
    test_pass("cxx11:logbl");
    test_true( ccw_cm_close((double)STD_NS logf(2.0f), STD_NS log(2.0), 1e-5) );
    test_pass("cxx11:logf");
    test_true( ccw_cm_close((double)STD_NS logl(2.0L), STD_NS log(2.0), 1e-9) );
    test_pass("cxx11:logl");
}

TEST_CASE(cmath, float_long_double_overloads_d_cxx11) {
    test_true( STD_NS lrintf(2.0f) == STD_NS lrint(2.0) );
    test_pass("cxx11:lrintf");
    test_true( STD_NS lrintl(2.0L) == STD_NS lrint(2.0) );
    test_pass("cxx11:lrintl");
    test_true( STD_NS lroundf(2.5f) == STD_NS lround(2.5) );
    test_pass("cxx11:lroundf");
    test_true( STD_NS lroundl(2.5L) == STD_NS lround(2.5) );
    test_pass("cxx11:lroundl");
    { float ip = 0; float fr = STD_NS modff(3.25f, &ip); test_true( ccw_cm_close((double)ip, 3.0, 1e-5) && ccw_cm_close((double)fr, 0.25, 1e-5) ); }
    test_pass("cxx11:modff");
    { long double ip = 0; long double fr = STD_NS modfl(3.25L, &ip); test_true( ccw_cm_close((double)ip, 3.0, 1e-9) && ccw_cm_close((double)fr, 0.25, 1e-9) ); }
    test_pass("cxx11:modfl");
    { float v = STD_NS nanf(""); test_true( STD_NS isnan(v) ); }
    test_pass("cxx11:nanf");
    { long double v = STD_NS nanl(""); test_true( STD_NS isnan(v) ); }
    test_pass("cxx11:nanl");
    test_true( ccw_cm_close((double)STD_NS nearbyintf(2.4f), STD_NS nearbyint(2.4), 1e-5) );
    test_pass("cxx11:nearbyintf");
    test_true( ccw_cm_close((double)STD_NS nearbyintl(2.4L), STD_NS nearbyint(2.4), 1e-9) );
    test_pass("cxx11:nearbyintl");
    test_true( ccw_cm_close((double)STD_NS nextafterf(1.0f, 2.0f), STD_NS nextafter(1.0, 2.0), 1e-5) );
    test_pass("cxx11:nextafterf");
    test_true( ccw_cm_close((double)STD_NS nextafterl(1.0L, 2.0L), STD_NS nextafter(1.0, 2.0), 1e-9) );
    test_pass("cxx11:nextafterl");
    test_true( STD_NS nexttowardf(1.0f, 2.0L) > 1.0f );
    test_pass("cxx11:nexttowardf");
    test_true( STD_NS nexttowardl(1.0L, 2.0L) > 1.0L );
    test_pass("cxx11:nexttowardl");
    test_true( ccw_cm_close((double)STD_NS powf(2.0f, 10.0f), STD_NS pow(2.0, 10.0), 1e-5) );
    test_pass("cxx11:powf");
    test_true( ccw_cm_close((double)STD_NS powl(2.0L, 10.0L), STD_NS pow(2.0, 10.0), 1e-9) );
    test_pass("cxx11:powl");
    test_true( ccw_cm_close((double)STD_NS remainderf(7.0f, 3.0f), STD_NS remainder(7.0, 3.0), 1e-5) );
    test_pass("cxx11:remainderf");
    test_true( ccw_cm_close((double)STD_NS remainderl(7.0L, 3.0L), STD_NS remainder(7.0, 3.0), 1e-9) );
    test_pass("cxx11:remainderl");
    { int q = 0; float r = STD_NS remquof(7.0f, 3.0f, &q); test_true( ccw_cm_close((double)r, 1.0, 1e-5) && q != 0 ); }
    test_pass("cxx11:remquof");
    { int q = 0; long double r = STD_NS remquol(7.0L, 3.0L, &q); test_true( ccw_cm_close((double)r, 1.0, 1e-9) && q != 0 ); }
    test_pass("cxx11:remquol");
    test_true( ccw_cm_close((double)STD_NS rintf(2.6f), STD_NS rint(2.6), 1e-5) );
    test_pass("cxx11:rintf");
    test_true( ccw_cm_close((double)STD_NS rintl(2.6L), STD_NS rint(2.6), 1e-9) );
    test_pass("cxx11:rintl");
}

TEST_CASE(cmath, float_long_double_overloads_e_cxx11) {
    test_true( ccw_cm_close((double)STD_NS roundf(2.5f), STD_NS round(2.5), 1e-5) );
    test_pass("cxx11:roundf");
    test_true( ccw_cm_close((double)STD_NS roundl(2.5L), STD_NS round(2.5), 1e-9) );
    test_pass("cxx11:roundl");
    test_true( ccw_cm_close((double)STD_NS scalblnf(1.0f, 3L), 8.0, 1e-5) );
    test_pass("cxx11:scalblnf");
    test_true( ccw_cm_close((double)STD_NS scalblnl(1.0L, 3L), 8.0, 1e-9) );
    test_pass("cxx11:scalblnl");
    test_true( ccw_cm_close((double)STD_NS scalbnf(1.0f, 3), 8.0, 1e-5) );
    test_pass("cxx11:scalbnf");
    test_true( ccw_cm_close((double)STD_NS scalbnl(1.0L, 3), 8.0, 1e-9) );
    test_pass("cxx11:scalbnl");
    test_true( ccw_cm_close((double)STD_NS sinf(0.5f), STD_NS sin(0.5), 1e-5) );
    test_pass("cxx11:sinf");
    test_true( ccw_cm_close((double)STD_NS sinhf(0.5f), STD_NS sinh(0.5), 1e-5) );
    test_pass("cxx11:sinhf");
    test_true( ccw_cm_close((double)STD_NS sinhl(0.5L), STD_NS sinh(0.5), 1e-9) );
    test_pass("cxx11:sinhl");
    test_true( ccw_cm_close((double)STD_NS sinl(0.5L), STD_NS sin(0.5), 1e-9) );
    test_pass("cxx11:sinl");
    test_true( ccw_cm_close((double)STD_NS sqrtf(16.0f), STD_NS sqrt(16.0), 1e-5) );
    test_pass("cxx11:sqrtf");
    test_true( ccw_cm_close((double)STD_NS sqrtl(16.0L), STD_NS sqrt(16.0), 1e-9) );
    test_pass("cxx11:sqrtl");
    test_true( ccw_cm_close((double)STD_NS tanf(0.5f), STD_NS tan(0.5), 1e-5) );
    test_pass("cxx11:tanf");
    test_true( ccw_cm_close((double)STD_NS tanhf(0.5f), STD_NS tanh(0.5), 1e-5) );
    test_pass("cxx11:tanhf");
    test_true( ccw_cm_close((double)STD_NS tanhl(0.5L), STD_NS tanh(0.5), 1e-9) );
    test_pass("cxx11:tanhl");
    test_true( ccw_cm_close((double)STD_NS tanl(0.5L), STD_NS tan(0.5), 1e-9) );
    test_pass("cxx11:tanl");
    test_true( ccw_cm_close((double)STD_NS tgammaf(4.0f), STD_NS tgamma(4.0), 1e-5) );
    test_pass("cxx11:tgammaf");
    test_true( ccw_cm_close((double)STD_NS tgammal(4.0L), STD_NS tgamma(4.0), 1e-9) );
    test_pass("cxx11:tgammal");
    test_true( ccw_cm_close((double)STD_NS truncf(2.9f), STD_NS trunc(2.9), 1e-5) );
    test_pass("cxx11:truncf");
    test_true( ccw_cm_close((double)STD_NS truncl(2.9L), STD_NS trunc(2.9), 1e-9) );
    test_pass("cxx11:truncl");
}

TEST_CASE(cmath, inverse_hyperbolic_overloads_cxx11) {
    test_true( ccw_cm_close((double)STD_NS acoshf(2.0f), STD_NS acosh(2.0), 1e-5) );
    test_pass("cxx11:acoshf");
    test_true( ccw_cm_close((double)STD_NS acoshl(2.0L), STD_NS acosh(2.0), 1e-9) );
    test_pass("cxx11:acoshl");
    test_true( ccw_cm_close((double)STD_NS asinhf(0.5f), STD_NS asinh(0.5), 1e-5) );
    test_pass("cxx11:asinhf");
    test_true( ccw_cm_close((double)STD_NS asinhl(0.5L), STD_NS asinh(0.5), 1e-9) );
    test_pass("cxx11:asinhl");
    test_true( ccw_cm_close((double)STD_NS atanhf(0.5f), STD_NS atanh(0.5), 1e-5) );
    test_pass("cxx11:atanhf");
    test_true( ccw_cm_close((double)STD_NS atanhl(0.5L), STD_NS atanh(0.5), 1e-9) );
    test_pass("cxx11:atanhl");
}

#if TEST_TARGET_CXX >= 2017 && defined(__cpp_lib_math_special_functions)
TEST_CASE(cmath, special_function_overloads_a_cxx17) {
    test_true( ccw_cm_close((double)STD::assoc_laguerref(1u, 0u, 0.5f),
                            STD::assoc_laguerre(1u, 0u, 0.5), 1e-5) );
    test_pass("cxx17:assoc_laguerref");
    test_true( ccw_cm_close((double)STD::assoc_laguerrel(1u, 0u, 0.5L),
                            STD::assoc_laguerre(1u, 0u, 0.5), 1e-9) );
    test_pass("cxx17:assoc_laguerrel");
    test_true( ccw_cm_close((double)STD::assoc_legendref(1u, 0u, 0.5f),
                            STD::assoc_legendre(1u, 0u, 0.5), 1e-5) );
    test_pass("cxx17:assoc_legendref");
    test_true( ccw_cm_close((double)STD::assoc_legendrel(1u, 0u, 0.5L),
                            STD::assoc_legendre(1u, 0u, 0.5), 1e-9) );
    test_pass("cxx17:assoc_legendrel");
    test_true( ccw_cm_close((double)STD::betaf(2.0f, 3.0f), STD::beta(2.0, 3.0), 1e-5) );
    test_pass("cxx17:betaf");
    test_true( ccw_cm_close((double)STD::betal(2.0L, 3.0L), STD::beta(2.0, 3.0), 1e-9) );
    test_pass("cxx17:betal");
    test_true( ccw_cm_close((double)STD::comp_ellint_1f(0.5f),
                            STD::comp_ellint_1(0.5), 1e-5) );
    test_pass("cxx17:comp_ellint_1f");
    test_true( ccw_cm_close((double)STD::comp_ellint_1l(0.5L),
                            STD::comp_ellint_1(0.5), 1e-9) );
    test_pass("cxx17:comp_ellint_1l");
    test_true( ccw_cm_close((double)STD::comp_ellint_2f(0.5f),
                            STD::comp_ellint_2(0.5), 1e-5) );
    test_pass("cxx17:comp_ellint_2f");
    test_true( ccw_cm_close((double)STD::comp_ellint_2l(0.5L),
                            STD::comp_ellint_2(0.5), 1e-9) );
    test_pass("cxx17:comp_ellint_2l");
    test_true( ccw_cm_close((double)STD::comp_ellint_3f(0.5f, 0.25f),
                            STD::comp_ellint_3(0.5, 0.25), 1e-5) );
    test_pass("cxx17:comp_ellint_3f");
    test_true( ccw_cm_close((double)STD::comp_ellint_3l(0.5L, 0.25L),
                            STD::comp_ellint_3(0.5, 0.25), 1e-9) );
    test_pass("cxx17:comp_ellint_3l");
    test_true( ccw_cm_close((double)STD::cyl_bessel_if(0.0f, 1.0f),
                            STD::cyl_bessel_i(0.0, 1.0), 1e-5) );
    test_pass("cxx17:cyl_bessel_if");
    test_true( ccw_cm_close((double)STD::cyl_bessel_il(0.0L, 1.0L),
                            STD::cyl_bessel_i(0.0, 1.0), 1e-9) );
    test_pass("cxx17:cyl_bessel_il");
    test_true( ccw_cm_close((double)STD::cyl_bessel_jf(0.0f, 1.0f),
                            STD::cyl_bessel_j(0.0, 1.0), 1e-5) );
    test_pass("cxx17:cyl_bessel_jf");
    test_true( ccw_cm_close((double)STD::cyl_bessel_jl(0.0L, 1.0L),
                            STD::cyl_bessel_j(0.0, 1.0), 1e-9) );
    test_pass("cxx17:cyl_bessel_jl");
    test_true( ccw_cm_close((double)STD::cyl_bessel_kf(0.0f, 1.0f),
                            STD::cyl_bessel_k(0.0, 1.0), 1e-5) );
    test_pass("cxx17:cyl_bessel_kf");
    test_true( ccw_cm_close((double)STD::cyl_bessel_kl(0.0L, 1.0L),
                            STD::cyl_bessel_k(0.0, 1.0), 1e-9) );
    test_pass("cxx17:cyl_bessel_kl");
    test_true( ccw_cm_close((double)STD::cyl_neumannf(0.0f, 1.0f),
                            STD::cyl_neumann(0.0, 1.0), 1e-5) );
    test_pass("cxx17:cyl_neumannf");
    test_true( ccw_cm_close((double)STD::cyl_neumannl(0.0L, 1.0L),
                            STD::cyl_neumann(0.0, 1.0), 1e-9) );
    test_pass("cxx17:cyl_neumannl");
}

TEST_CASE(cmath, special_function_overloads_b_cxx17) {
    test_true( ccw_cm_close((double)STD::ellint_1f(0.5f, 0.25f),
                            STD::ellint_1(0.5, 0.25), 1e-5) );
    test_pass("cxx17:ellint_1f");
    test_true( ccw_cm_close((double)STD::ellint_1l(0.5L, 0.25L),
                            STD::ellint_1(0.5, 0.25), 1e-9) );
    test_pass("cxx17:ellint_1l");
    test_true( ccw_cm_close((double)STD::ellint_2f(0.5f, 0.25f),
                            STD::ellint_2(0.5, 0.25), 1e-5) );
    test_pass("cxx17:ellint_2f");
    test_true( ccw_cm_close((double)STD::ellint_2l(0.5L, 0.25L),
                            STD::ellint_2(0.5, 0.25), 1e-9) );
    test_pass("cxx17:ellint_2l");
    test_true( ccw_cm_close((double)STD::ellint_3f(0.5f, 0.25f, 0.5f),
                            STD::ellint_3(0.5, 0.25, 0.5), 1e-5) );
    test_pass("cxx17:ellint_3f");
    test_true( ccw_cm_close((double)STD::ellint_3l(0.5L, 0.25L, 0.5L),
                            STD::ellint_3(0.5, 0.25, 0.5), 1e-9) );
    test_pass("cxx17:ellint_3l");
    test_true( ccw_cm_close((double)STD::expintf(1.0f), STD::expint(1.0), 1e-5) );
    test_pass("cxx17:expintf");
    test_true( ccw_cm_close((double)STD::expintl(1.0L), STD::expint(1.0), 1e-9) );
    test_pass("cxx17:expintl");
    test_true( ccw_cm_close((double)STD::hermitef(2u, 1.0f), STD::hermite(2u, 1.0), 1e-5) );
    test_pass("cxx17:hermitef");
    test_true( ccw_cm_close((double)STD::hermitel(2u, 1.0L), STD::hermite(2u, 1.0), 1e-9) );
    test_pass("cxx17:hermitel");
    test_true( ccw_cm_close((double)STD::laguerref(1u, 0.5f), STD::laguerre(1u, 0.5), 1e-5) );
    test_pass("cxx17:laguerref");
    test_true( ccw_cm_close((double)STD::laguerrel(1u, 0.5L), STD::laguerre(1u, 0.5), 1e-9) );
    test_pass("cxx17:laguerrel");
    test_true( ccw_cm_close((double)STD::legendref(1u, 0.5f), STD::legendre(1u, 0.5), 1e-5) );
    test_pass("cxx17:legendref");
    test_true( ccw_cm_close((double)STD::legendrel(1u, 0.5L), STD::legendre(1u, 0.5), 1e-9) );
    test_pass("cxx17:legendrel");
    test_true( ccw_cm_close((double)STD::riemann_zetaf(2.0f),
                            STD::riemann_zeta(2.0), 1e-5) );
    test_pass("cxx17:riemann_zetaf");
    test_true( ccw_cm_close((double)STD::riemann_zetal(2.0L),
                            STD::riemann_zeta(2.0), 1e-9) );
    test_pass("cxx17:riemann_zetal");
    test_true( ccw_cm_close((double)STD::sph_besself(0u, 1.0f),
                            STD::sph_bessel(0u, 1.0), 1e-5) );
    test_pass("cxx17:sph_besself");
    test_true( ccw_cm_close((double)STD::sph_bessell(0u, 1.0L),
                            STD::sph_bessel(0u, 1.0), 1e-9) );
    test_pass("cxx17:sph_bessell");
    test_true( ccw_cm_close((double)STD::sph_legendref(1u, 0u, 0.5f),
                            STD::sph_legendre(1u, 0u, 0.5), 1e-5) );
    test_pass("cxx17:sph_legendref");
    test_true( ccw_cm_close((double)STD::sph_legendrel(1u, 0u, 0.5L),
                            STD::sph_legendre(1u, 0u, 0.5), 1e-9) );
    test_pass("cxx17:sph_legendrel");
    test_true( ccw_cm_close((double)STD::sph_neumannf(0u, 1.0f),
                            STD::sph_neumann(0u, 1.0), 1e-5) );
    test_pass("cxx17:sph_neumannf");
    test_true( ccw_cm_close((double)STD::sph_neumannl(0u, 1.0L),
                            STD::sph_neumann(0u, 1.0), 1e-9) );
    test_pass("cxx17:sph_neumannl");
}
#else
TEST_CASE(cmath, special_function_overloads_a_cxx17) {
    test_skip("cxx17:assoc_laguerref"); test_skip("cxx17:assoc_laguerrel");
    test_skip("cxx17:assoc_legendref"); test_skip("cxx17:assoc_legendrel");
    test_skip("cxx17:betaf");           test_skip("cxx17:betal");
    test_skip("cxx17:comp_ellint_1f");  test_skip("cxx17:comp_ellint_1l");
    test_skip("cxx17:comp_ellint_2f");  test_skip("cxx17:comp_ellint_2l");
    test_skip("cxx17:comp_ellint_3f");  test_skip("cxx17:comp_ellint_3l");
    test_skip("cxx17:cyl_bessel_if");   test_skip("cxx17:cyl_bessel_il");
    test_skip("cxx17:cyl_bessel_jf");   test_skip("cxx17:cyl_bessel_jl");
    test_skip("cxx17:cyl_bessel_kf");   test_skip("cxx17:cyl_bessel_kl");
    test_skip("cxx17:cyl_neumannf");    test_skip("cxx17:cyl_neumannl");
}
TEST_CASE(cmath, special_function_overloads_b_cxx17) {
    test_skip("cxx17:ellint_1f");     test_skip("cxx17:ellint_1l");
    test_skip("cxx17:ellint_2f");     test_skip("cxx17:ellint_2l");
    test_skip("cxx17:ellint_3f");     test_skip("cxx17:ellint_3l");
    test_skip("cxx17:expintf");       test_skip("cxx17:expintl");
    test_skip("cxx17:hermitef");      test_skip("cxx17:hermitel");
    test_skip("cxx17:laguerref");     test_skip("cxx17:laguerrel");
    test_skip("cxx17:legendref");     test_skip("cxx17:legendrel");
    test_skip("cxx17:riemann_zetaf"); test_skip("cxx17:riemann_zetal");
    test_skip("cxx17:sph_besself");   test_skip("cxx17:sph_bessell");
    test_skip("cxx17:sph_legendref"); test_skip("cxx17:sph_legendrel");
    test_skip("cxx17:sph_neumannf");  test_skip("cxx17:sph_neumannl");
}
#endif

TEST_CASE(cmath, feature_test_macros) {
#if defined(__cpp_lib_constexpr_cmath)
    test_true( __cpp_lib_constexpr_cmath > 0L );
    test_pass("cxx23:__cpp_lib_constexpr_cmath");
#else
    test_skip("cxx23:__cpp_lib_constexpr_cmath");
#endif
#if defined(__cpp_lib_constexpr_cmath)
    test_true( __cpp_lib_constexpr_cmath > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_cmath");
#else
    test_skip("cxx26:__cpp_lib_constexpr_cmath");
#endif
}

TEST_CASE(cmath, constexpr_evaluation) {
#if defined(__cpp_lib_constexpr_cmath) && (__cpp_lib_constexpr_cmath) >= 202202L
    {
        struct K {
            static constexpr double run() {
                return STD_NS fabs(-2.0) + STD_NS fmin(1.0, 5.0) + STD_NS ceil(0.25)
                     + STD_NS trunc(3.75) + STD_NS fmax(0.0, 1.0);
            }
        };
        static_assert(K::run() == 8.0, "P0533R9 cmath in a constant expression");
        test_true( K::run() == 8.0 );
    }
    test_pass("cxx23:constexpr cmath and cstdlib");
#else
    TEST_NOTE("__cpp_lib_constexpr_cmath is absent, or older than P0533R9 (202202L)");
    test_skip("cxx23:constexpr cmath and cstdlib");
#endif

#if defined(__cpp_lib_constexpr_cmath) && (__cpp_lib_constexpr_cmath) >= 202306L
    {
        struct K {
            static constexpr double run() { return STD_NS sqrt(16.0) + STD_NS exp(0.0); }
        };
        static_assert(K::run() == 5.0, "P1383R2 cmath in a constant expression");
        test_true( K::run() == 5.0 );
    }
    test_pass("cxx26:more constexpr cmath");
#else
    TEST_NOTE("__cpp_lib_constexpr_cmath is absent, or older than P1383R2 (202306L)");
    test_skip("cxx26:more constexpr cmath");
#endif
}

TEST_CASE(cmath, next_up_down_cxx26) {
#if defined(_CCW_HAS_C23_NEXTUP)
    test_true( STD_NS nextupf(1.0f) > 1.0f );
    test_true( STD_NS nextdownf(STD_NS nextupf(1.0f)) == 1.0f );
    test_pass("cxx26:nextupf");
    test_true( STD_NS nextdownf(1.0f) < 1.0f );
    test_pass("cxx26:nextdownf");
    test_true( STD_NS nextupl(1.0L) > 1.0L );
    test_true( STD_NS nextdownl(STD_NS nextupl(1.0L)) == 1.0L );
    test_pass("cxx26:nextupl");
    test_true( STD_NS nextdownl(1.0L) < 1.0L );
    test_pass("cxx26:nextdownl");
#else
    TEST_NOTE("no std::nextup / nextdown in this library, and ccwrap does not add them");
    test_skip("cxx26:nextupf");
    test_skip("cxx26:nextdownf");
    test_skip("cxx26:nextupl");
    test_skip("cxx26:nextdownl");
#endif
}
