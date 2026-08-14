#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011
#include <cmath>
#include <type_traits>

#if _TST_HAS_DECLTYPE

static bool near_eq(double a, double b, double eps = 1e-9) {
    return STD_NS fabs(a - b) <= eps * (STD_NS fabs(b) > 1.0 ? STD_NS fabs(b) : 1.0);
}

static const double kPi = 3.14159265358979323846;
static const double kE  = 2.71828182845904523536;

static const double kEpsF = 1e-6;

TEST_CASE(cmath, acos) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS acos(0.5f)), float>, "acos(float)");
    static_assert(STD::is_same_v<decltype(STD_NS acos(0.5)), double>, "acos(double)");
    static_assert(STD::is_same_v<decltype(STD_NS acos(0.5L)), long double>, "acos(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS acosf(0.5f)), float>, "acosf");
    static_assert(STD::is_same_v<decltype(STD_NS acosl(0.5L)), long double>, "acosl");
#endif

    test_true( near_eq(STD_NS acos(1.0), 0.0) );
    test_true( near_eq(STD_NS acos(0.0), kPi / 2) );
    test_true( near_eq(STD_NS acos(-1.0), kPi) );
    test_true( near_eq(STD_NS acos(1.0f), 0.0, kEpsF) );
    test_true( near_eq(STD_NS acos(0.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD_NS acos(-1.0L), kPi) );
    test_true( near_eq(STD_NS acosf(0.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD_NS acosl(-1.0L), kPi) );
    test_pass("cxx03:acos");
}

TEST_CASE(cmath, asin) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS asin(0.5f)), float>, "asin(float)");
    static_assert(STD::is_same_v<decltype(STD_NS asin(0.5)), double>, "asin(double)");
    static_assert(STD::is_same_v<decltype(STD_NS asin(0.5L)), long double>, "asin(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS asinf(0.5f)), float>, "asinf");
    static_assert(STD::is_same_v<decltype(STD_NS asinl(0.5L)), long double>, "asinl");
#endif

    test_true( near_eq(STD_NS asin(0.0), 0.0) );
    test_true( near_eq(STD_NS asin(1.0), kPi / 2) );
    test_true( near_eq(STD_NS asin(-1.0), -kPi / 2) );
    test_true( near_eq(STD_NS asin(0.0f), 0.0, kEpsF) );
    test_true( near_eq(STD_NS asin(1.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD_NS asin(1.0L), kPi / 2) );
    test_true( near_eq(STD_NS asinf(1.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD_NS asinl(1.0L), kPi / 2) );
    test_pass("cxx03:asin");
}

TEST_CASE(cmath, atan) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS atan(0.5f)), float>, "atan(float)");
    static_assert(STD::is_same_v<decltype(STD_NS atan(0.5)), double>, "atan(double)");
    static_assert(STD::is_same_v<decltype(STD_NS atan(0.5L)), long double>, "atan(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS atanf(0.5f)), float>, "atanf");
    static_assert(STD::is_same_v<decltype(STD_NS atanl(0.5L)), long double>, "atanl");
#endif

    test_true( near_eq(STD_NS atan(0.0), 0.0) );
    test_true( near_eq(STD_NS atan(1.0), kPi / 4) );
    test_true( near_eq(STD_NS atan(-1.0), -kPi / 4) );
    test_true( near_eq(STD_NS atan(1.0f), kPi / 4, kEpsF) );
    test_true( near_eq((double)STD_NS atan(1.0L), kPi / 4) );
    test_true( near_eq(STD_NS atanf(1.0f), kPi / 4, kEpsF) );
    test_true( near_eq((double)STD_NS atanl(1.0L), kPi / 4) );
    test_pass("cxx03:atan");
}

TEST_CASE(cmath, cos) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS cos(0.5f)), float>, "cos(float)");
    static_assert(STD::is_same_v<decltype(STD_NS cos(0.5)), double>, "cos(double)");
    static_assert(STD::is_same_v<decltype(STD_NS cos(0.5L)), long double>, "cos(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS cosf(0.5f)), float>, "cosf");
    static_assert(STD::is_same_v<decltype(STD_NS cosl(0.5L)), long double>, "cosl");
#endif

    test_true( near_eq(STD_NS cos(0.0), 1.0) );
    test_true( near_eq(STD_NS cos(kPi), -1.0) );
    test_true( near_eq(STD_NS cos(kPi / 3), 0.5) );
    test_true( near_eq(STD_NS cos(0.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD_NS cos(0.0L), 1.0) );
    test_true( near_eq(STD_NS cosf(0.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD_NS cosl(0.0L), 1.0) );
    test_true( STD_NS cos(1.0) == STD_NS cos(-1.0) );
    test_pass("cxx03:cos");
}

TEST_CASE(cmath, sin) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS sin(0.5f)), float>, "sin(float)");
    static_assert(STD::is_same_v<decltype(STD_NS sin(0.5)), double>, "sin(double)");
    static_assert(STD::is_same_v<decltype(STD_NS sin(0.5L)), long double>, "sin(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS sinf(0.5f)), float>, "sinf");
    static_assert(STD::is_same_v<decltype(STD_NS sinl(0.5L)), long double>, "sinl");
#endif

    test_true( near_eq(STD_NS sin(0.0), 0.0) );
    test_true( near_eq(STD_NS sin(kPi / 2), 1.0) );
    test_true( near_eq(STD_NS sin(kPi / 6), 0.5) );
    test_true( near_eq(STD_NS sin(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS sin(0.0L), 0.0) );
    test_true( near_eq(STD_NS sinf(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS sinl(0.0L), 0.0) );
    test_true( near_eq(STD_NS sin(0.7) * STD_NS sin(0.7) + STD_NS cos(0.7) * STD_NS cos(0.7), 1.0) );
    test_pass("cxx03:sin");
}

TEST_CASE(cmath, tan) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS tan(0.5f)), float>, "tan(float)");
    static_assert(STD::is_same_v<decltype(STD_NS tan(0.5)), double>, "tan(double)");
    static_assert(STD::is_same_v<decltype(STD_NS tan(0.5L)), long double>, "tan(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS tanf(0.5f)), float>, "tanf");
    static_assert(STD::is_same_v<decltype(STD_NS tanl(0.5L)), long double>, "tanl");
#endif

    test_true( near_eq(STD_NS tan(0.0), 0.0) );
    test_true( near_eq(STD_NS tan(kPi / 4), 1.0) );
    test_true( near_eq(STD_NS tan(0.3), STD_NS sin(0.3) / STD_NS cos(0.3)) );
    test_true( near_eq(STD_NS tan(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS tan(0.0L), 0.0) );
    test_true( near_eq(STD_NS tanf(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS tanl(0.0L), 0.0) );
    test_pass("cxx03:tan");
}

TEST_CASE(cmath, atan2) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS atan2(1.0f, 1.0f)), float>, "atan2(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS atan2(1.0, 1.0)), double>, "atan2(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS atan2(1.0L, 1.0L)), long double>, "atan2(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS atan2f(1.0f, 1.0f)), float>, "atan2f");
    static_assert(STD::is_same_v<decltype(STD_NS atan2l(1.0L, 1.0L)), long double>, "atan2l");
#endif

    test_true( near_eq(STD_NS atan2(0.0, 1.0), 0.0) );
    test_true( near_eq(STD_NS atan2(1.0, 1.0), kPi / 4) );
    test_true( near_eq(STD_NS atan2(1.0, 0.0), kPi / 2) );
    test_true( near_eq(STD_NS atan2(0.0, -1.0), kPi) );
    test_true( near_eq(STD_NS atan2(-1.0, -1.0), -3 * kPi / 4) );
    test_true( near_eq(STD_NS atan2(1.0f, 1.0f), kPi / 4, kEpsF) );
    test_true( near_eq((double)STD_NS atan2(1.0L, 1.0L), kPi / 4) );
    test_true( near_eq(STD_NS atan2f(1.0f, 0.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD_NS atan2l(1.0L, 0.0L), kPi / 2) );
    test_pass("cxx03:atan2");
}

TEST_CASE(cmath, cosh) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS cosh(0.5f)), float>, "cosh(float)");
    static_assert(STD::is_same_v<decltype(STD_NS cosh(0.5)), double>, "cosh(double)");
    static_assert(STD::is_same_v<decltype(STD_NS cosh(0.5L)), long double>, "cosh(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS coshf(0.5f)), float>, "coshf");
    static_assert(STD::is_same_v<decltype(STD_NS coshl(0.5L)), long double>, "coshl");
#endif

    test_true( near_eq(STD_NS cosh(0.0), 1.0) );
    test_true( near_eq(STD_NS cosh(1.0), (kE + 1.0 / kE) / 2) );
    test_true( near_eq(STD_NS cosh(2.0), STD_NS cosh(-2.0)) );
    test_true( near_eq(STD_NS cosh(0.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD_NS cosh(0.0L), 1.0) );
    test_true( near_eq(STD_NS coshf(0.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD_NS coshl(0.0L), 1.0) );
    test_pass("cxx03:cosh");
}

TEST_CASE(cmath, sinh) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS sinh(0.5f)), float>, "sinh(float)");
    static_assert(STD::is_same_v<decltype(STD_NS sinh(0.5)), double>, "sinh(double)");
    static_assert(STD::is_same_v<decltype(STD_NS sinh(0.5L)), long double>, "sinh(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS sinhf(0.5f)), float>, "sinhf");
    static_assert(STD::is_same_v<decltype(STD_NS sinhl(0.5L)), long double>, "sinhl");
#endif

    test_true( near_eq(STD_NS sinh(0.0), 0.0) );
    test_true( near_eq(STD_NS sinh(1.0), (kE - 1.0 / kE) / 2) );
    test_true( near_eq(STD_NS sinh(2.0), -STD_NS sinh(-2.0)) );

    test_true( near_eq(STD_NS cosh(1.3) * STD_NS cosh(1.3) - STD_NS sinh(1.3) * STD_NS sinh(1.3), 1.0) );
    test_true( near_eq(STD_NS sinh(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS sinh(0.0L), 0.0) );
    test_true( near_eq(STD_NS sinhf(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS sinhl(0.0L), 0.0) );
    test_pass("cxx03:sinh");
}

TEST_CASE(cmath, tanh) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS tanh(0.5f)), float>, "tanh(float)");
    static_assert(STD::is_same_v<decltype(STD_NS tanh(0.5)), double>, "tanh(double)");
    static_assert(STD::is_same_v<decltype(STD_NS tanh(0.5L)), long double>, "tanh(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS tanhf(0.5f)), float>, "tanhf");
    static_assert(STD::is_same_v<decltype(STD_NS tanhl(0.5L)), long double>, "tanhl");
#endif

    test_true( near_eq(STD_NS tanh(0.0), 0.0) );
    test_true( near_eq(STD_NS tanh(1.0), STD_NS sinh(1.0) / STD_NS cosh(1.0)) );
    test_true( STD_NS tanh(100.0) <= 1.0 && STD_NS tanh(100.0) > 0.999 );
    test_true( near_eq(STD_NS tanh(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS tanh(0.0L), 0.0) );
    test_true( near_eq(STD_NS tanhf(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS tanhl(0.0L), 0.0) );
    test_pass("cxx03:tanh");
}

TEST_CASE(cmath, acosh_asinh_atanh) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS acosh(2.0f)), float>, "acosh(float)");
    static_assert(STD::is_same_v<decltype(STD_NS acosh(2.0)), double>, "acosh(double)");
    static_assert(STD::is_same_v<decltype(STD_NS acosh(2.0L)), long double>, "acosh(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS acoshf(2.0f)), float>, "acoshf");
    static_assert(STD::is_same_v<decltype(STD_NS acoshl(2.0L)), long double>, "acoshl");
    static_assert(STD::is_same_v<decltype(STD_NS asinh(2.0f)), float>, "asinh(float)");
    static_assert(STD::is_same_v<decltype(STD_NS asinh(2.0)), double>, "asinh(double)");
    static_assert(STD::is_same_v<decltype(STD_NS asinh(2.0L)), long double>, "asinh(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS asinhf(2.0f)), float>, "asinhf");
    static_assert(STD::is_same_v<decltype(STD_NS asinhl(2.0L)), long double>, "asinhl");
    static_assert(STD::is_same_v<decltype(STD_NS atanh(0.5f)), float>, "atanh(float)");
    static_assert(STD::is_same_v<decltype(STD_NS atanh(0.5)), double>, "atanh(double)");
    static_assert(STD::is_same_v<decltype(STD_NS atanh(0.5L)), long double>, "atanh(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS atanhf(0.5f)), float>, "atanhf");
    static_assert(STD::is_same_v<decltype(STD_NS atanhl(0.5L)), long double>, "atanhl");
#endif

    test_true( near_eq(STD_NS acosh(1.0), 0.0) );
    test_true( near_eq(STD_NS acosh(STD_NS cosh(1.5)), 1.5) );
    test_true( near_eq(STD_NS acosh(1.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS acosh(1.0L), 0.0) );
    test_true( near_eq(STD_NS acoshf(1.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS acoshl(1.0L), 0.0) );
    test_pass("cxx11:acosh");

    test_true( near_eq(STD_NS asinh(0.0), 0.0) );
    test_true( near_eq(STD_NS asinh(STD_NS sinh(1.5)), 1.5) );
    test_true( near_eq(STD_NS asinh(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS asinh(0.0L), 0.0) );
    test_true( near_eq(STD_NS asinhf(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS asinhl(0.0L), 0.0) );
    test_pass("cxx11:asinh");

    test_true( near_eq(STD_NS atanh(0.0), 0.0) );
    test_true( near_eq(STD_NS atanh(STD_NS tanh(0.75)), 0.75) );
    test_true( near_eq(STD_NS atanh(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS atanh(0.0L), 0.0) );
    test_true( near_eq(STD_NS atanhf(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS atanhl(0.0L), 0.0) );
    test_pass("cxx11:atanh");
}

TEST_CASE(cmath, exp) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS exp(1.0f)), float>, "exp(float)");
    static_assert(STD::is_same_v<decltype(STD_NS exp(1.0)), double>, "exp(double)");
    static_assert(STD::is_same_v<decltype(STD_NS exp(1.0L)), long double>, "exp(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS expf(1.0f)), float>, "expf");
    static_assert(STD::is_same_v<decltype(STD_NS expl(1.0L)), long double>, "expl");
#endif

    test_true( near_eq(STD_NS exp(0.0), 1.0) );
    test_true( near_eq(STD_NS exp(1.0), kE) );
    test_true( near_eq(STD_NS exp(-1.0), 1.0 / kE) );
    test_true( near_eq(STD_NS exp(0.0f), 1.0, kEpsF) );
    test_true( near_eq(STD_NS exp(1.0f), kE, kEpsF) );
    test_true( near_eq((double)STD_NS exp(1.0L), kE) );
    test_true( near_eq(STD_NS expf(1.0f), kE, kEpsF) );
    test_true( near_eq((double)STD_NS expl(1.0L), kE) );
    test_pass("cxx03:exp");
}

TEST_CASE(cmath, exp2_expm1) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS exp2(1.0f)), float>, "exp2(float)");
    static_assert(STD::is_same_v<decltype(STD_NS exp2(1.0)), double>, "exp2(double)");
    static_assert(STD::is_same_v<decltype(STD_NS exp2(1.0L)), long double>, "exp2(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS exp2f(1.0f)), float>, "exp2f");
    static_assert(STD::is_same_v<decltype(STD_NS exp2l(1.0L)), long double>, "exp2l");
    static_assert(STD::is_same_v<decltype(STD_NS expm1(1.0f)), float>, "expm1(float)");
    static_assert(STD::is_same_v<decltype(STD_NS expm1(1.0)), double>, "expm1(double)");
    static_assert(STD::is_same_v<decltype(STD_NS expm1(1.0L)), long double>, "expm1(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS expm1f(1.0f)), float>, "expm1f");
    static_assert(STD::is_same_v<decltype(STD_NS expm1l(1.0L)), long double>, "expm1l");
#endif

    test_true( near_eq(STD_NS exp2(0.0), 1.0) );
    test_true( near_eq(STD_NS exp2(10.0), 1024.0) );
    test_true( near_eq(STD_NS exp2(-1.0), 0.5) );
    test_true( near_eq(STD_NS exp2(10.0f), 1024.0, kEpsF) );
    test_true( near_eq((double)STD_NS exp2(10.0L), 1024.0) );
    test_true( near_eq(STD_NS exp2f(10.0f), 1024.0, kEpsF) );
    test_true( near_eq((double)STD_NS exp2l(10.0L), 1024.0) );
    test_pass("cxx11:exp2");

    test_true( near_eq(STD_NS expm1(0.0), 0.0) );
    test_true( near_eq(STD_NS expm1(1.0), kE - 1.0) );

    test_true( near_eq(STD_NS expm1(1e-10) / 1e-10, 1.0, 1e-8) );
    test_true( near_eq(STD_NS expm1(1.0f), kE - 1.0, kEpsF) );
    test_true( near_eq((double)STD_NS expm1(1.0L), kE - 1.0) );
    test_true( near_eq(STD_NS expm1f(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS expm1l(1.0L), kE - 1.0) );
    test_pass("cxx11:expm1");
}

TEST_CASE(cmath, log) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS log(1.0f)), float>, "log(float)");
    static_assert(STD::is_same_v<decltype(STD_NS log(1.0)), double>, "log(double)");
    static_assert(STD::is_same_v<decltype(STD_NS log(1.0L)), long double>, "log(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS logf(1.0f)), float>, "logf");
    static_assert(STD::is_same_v<decltype(STD_NS logl(1.0L)), long double>, "logl");
#endif

    test_true( near_eq(STD_NS log(1.0), 0.0) );
    test_true( near_eq(STD_NS log(kE), 1.0) );
    test_true( near_eq(STD_NS log(STD_NS exp(3.5)), 3.5) );
    test_true( near_eq(STD_NS log(1.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS log(1.0L), 0.0) );
    test_true( near_eq(STD_NS logf(1.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS logl(1.0L), 0.0) );
    test_pass("cxx03:log");
}

TEST_CASE(cmath, log10_log2_log1p) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS log10(1.0f)), float>, "log10(float)");
    static_assert(STD::is_same_v<decltype(STD_NS log10(1.0)), double>, "log10(double)");
    static_assert(STD::is_same_v<decltype(STD_NS log10(1.0L)), long double>, "log10(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS log10f(1.0f)), float>, "log10f");
    static_assert(STD::is_same_v<decltype(STD_NS log10l(1.0L)), long double>, "log10l");
    static_assert(STD::is_same_v<decltype(STD_NS log2(1.0f)), float>, "log2(float)");
    static_assert(STD::is_same_v<decltype(STD_NS log2(1.0)), double>, "log2(double)");
    static_assert(STD::is_same_v<decltype(STD_NS log2(1.0L)), long double>, "log2(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS log2f(1.0f)), float>, "log2f");
    static_assert(STD::is_same_v<decltype(STD_NS log2l(1.0L)), long double>, "log2l");
    static_assert(STD::is_same_v<decltype(STD_NS log1p(1.0f)), float>, "log1p(float)");
    static_assert(STD::is_same_v<decltype(STD_NS log1p(1.0)), double>, "log1p(double)");
    static_assert(STD::is_same_v<decltype(STD_NS log1p(1.0L)), long double>, "log1p(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS log1pf(1.0f)), float>, "log1pf");
    static_assert(STD::is_same_v<decltype(STD_NS log1pl(1.0L)), long double>, "log1pl");
#endif

    test_true( near_eq(STD_NS log10(1.0), 0.0) );
    test_true( near_eq(STD_NS log10(100.0), 2.0) );
    test_true( near_eq(STD_NS log10(0.001), -3.0) );
    test_true( near_eq(STD_NS log10(100.0f), 2.0, kEpsF) );
    test_true( near_eq((double)STD_NS log10(100.0L), 2.0) );
    test_true( near_eq(STD_NS log10f(100.0f), 2.0, kEpsF) );
    test_true( near_eq((double)STD_NS log10l(100.0L), 2.0) );
    test_pass("cxx03:log10");

    test_true( near_eq(STD_NS log2(1.0), 0.0) );
    test_true( near_eq(STD_NS log2(8.0), 3.0) );
    test_true( near_eq(STD_NS log2(0.25), -2.0) );
    test_true( near_eq(STD_NS log2(8.0f), 3.0, kEpsF) );
    test_true( near_eq((double)STD_NS log2(8.0L), 3.0) );
    test_true( near_eq(STD_NS log2f(8.0f), 3.0, kEpsF) );
    test_true( near_eq((double)STD_NS log2l(8.0L), 3.0) );
    test_pass("cxx11:log2");

    test_true( near_eq(STD_NS log1p(0.0), 0.0) );
    test_true( near_eq(STD_NS log1p(kE - 1.0), 1.0) );
    test_true( near_eq(STD_NS log1p(1e-10) / 1e-10, 1.0, 1e-8) );
    test_true( near_eq(STD_NS log1p(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS log1p(0.0L), 0.0) );
    test_true( near_eq(STD_NS log1pf(0.0f), 0.0, kEpsF) );
    test_true( near_eq((double)STD_NS log1pl(0.0L), 0.0) );
    test_pass("cxx11:log1p");
}

TEST_CASE(cmath, sqrt_cbrt) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS sqrt(4.0f)), float>, "sqrt(float)");
    static_assert(STD::is_same_v<decltype(STD_NS sqrt(4.0)), double>, "sqrt(double)");
    static_assert(STD::is_same_v<decltype(STD_NS sqrt(4.0L)), long double>, "sqrt(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS sqrtf(4.0f)), float>, "sqrtf");
    static_assert(STD::is_same_v<decltype(STD_NS sqrtl(4.0L)), long double>, "sqrtl");
    static_assert(STD::is_same_v<decltype(STD_NS cbrt(8.0f)), float>, "cbrt(float)");
    static_assert(STD::is_same_v<decltype(STD_NS cbrt(8.0)), double>, "cbrt(double)");
    static_assert(STD::is_same_v<decltype(STD_NS cbrt(8.0L)), long double>, "cbrt(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS cbrtf(8.0f)), float>, "cbrtf");
    static_assert(STD::is_same_v<decltype(STD_NS cbrtl(8.0L)), long double>, "cbrtl");
#endif

    test_true( STD_NS sqrt(4.0) == 2.0 );
    test_true( STD_NS sqrt(0.0) == 0.0 );
    test_true( STD_NS sqrt(2.25) == 1.5 );
    test_true( STD_NS sqrt(4.0f) == 2.0f );
    test_true( STD_NS sqrt(4.0L) == 2.0L );
    test_true( STD_NS sqrtf(4.0f) == 2.0f );
    test_true( STD_NS sqrtl(4.0L) == 2.0L );
    test_pass("cxx03:sqrt");

    test_true( near_eq(STD_NS cbrt(27.0), 3.0) );
    test_true( near_eq(STD_NS cbrt(-8.0), -2.0) );
    test_true( near_eq(STD_NS cbrt(0.0), 0.0) );
    test_true( near_eq(STD_NS cbrt(27.0f), 3.0, kEpsF) );
    test_true( near_eq((double)STD_NS cbrt(27.0L), 3.0) );
    test_true( near_eq(STD_NS cbrtf(27.0f), 3.0, kEpsF) );
    test_true( near_eq((double)STD_NS cbrtl(-8.0L), -2.0) );
    test_pass("cxx11:cbrt");
}

TEST_CASE(cmath, pow) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS pow(2.0f, 3.0f)), float>, "pow(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS pow(2.0, 3.0)), double>, "pow(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS pow(2.0L, 3.0L)), long double>, "pow(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS powf(2.0f, 3.0f)), float>, "powf");
    static_assert(STD::is_same_v<decltype(STD_NS powl(2.0L, 3.0L)), long double>, "powl");
#endif

    test_true( STD_NS pow(2.0, 10.0) == 1024.0 );
    test_true( STD_NS pow(2.0, 0.0) == 1.0 );
    test_true( STD_NS pow(2.0, -1.0) == 0.5 );
    test_true( near_eq(STD_NS pow(9.0, 0.5), 3.0) );
    test_true( near_eq(STD_NS pow(-2.0, 3.0), -8.0) );
    test_true( STD_NS pow(2.0f, 10.0f) == 1024.0f );
    test_true( STD_NS pow(2.0L, 10.0L) == 1024.0L );
    test_true( STD_NS powf(2.0f, 10.0f) == 1024.0f );
    test_true( STD_NS powl(2.0L, 10.0L) == 1024.0L );

#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS pow(2.0, 10)), double>, "pow(double,int)");
#endif
    test_true( STD_NS pow(2.0, 10) == 1024.0 );
    test_pass("cxx03:pow");
}

TEST_CASE(cmath, hypot) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS hypot(3.0f, 4.0f)), float>, "hypot(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS hypot(3.0, 4.0)), double>, "hypot(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS hypot(3.0L, 4.0L)), long double>, "hypot(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS hypotf(3.0f, 4.0f)), float>, "hypotf");
    static_assert(STD::is_same_v<decltype(STD_NS hypotl(3.0L, 4.0L)), long double>, "hypotl");
#endif

    test_true( STD_NS hypot(3.0, 4.0) == 5.0 );
    test_true( STD_NS hypot(0.0, 0.0) == 0.0 );
    test_true( STD_NS hypot(-3.0, -4.0) == 5.0 );
    test_true( STD_NS hypot(3.0f, 4.0f) == 5.0f );
    test_true( STD_NS hypot(3.0L, 4.0L) == 5.0L );
    test_true( STD_NS hypotf(3.0f, 4.0f) == 5.0f );
    test_true( STD_NS hypotl(3.0L, 4.0L) == 5.0L );

#if _TST_HAS_VAR_TEMPLATE && !_TST_NO_GLOBAL_HYPOT3
    static_assert(STD::is_same_v<decltype(STD_NS hypot(1.0f, 2.0f, 2.0f)), float>, "hypot(f,f,f)");
    static_assert(STD::is_same_v<decltype(STD_NS hypot(1.0, 2.0, 2.0)), double>, "hypot(d,d,d)");
    static_assert(STD::is_same_v<decltype(STD_NS hypot(1.0L, 2.0L, 2.0L)), long double>, "hypot(ld,ld,ld)");
#endif
#if TEST_TARGET_CXX >= 2017 && !_TST_NO_GLOBAL_HYPOT3
    test_true( near_eq(STD_NS hypot(2.0, 3.0, 6.0), 7.0) );
    test_true( near_eq(STD_NS hypot(1.0, 2.0, 2.0), 3.0) );
    test_true( near_eq(STD_NS hypot(2.0f, 3.0f, 6.0f), 7.0, kEpsF) );
    test_true( near_eq((double)STD_NS hypot(2.0L, 3.0L, 6.0L), 7.0) );
    test_pass("cxx11:hypot");
#else
    TEST_SKIP_N(4);
    test_skip("cxx11:hypot");
#endif
}
#else
TEST_CASE_SKIP(cmath, acos)
TEST_CASE_SKIP(cmath, asin)
TEST_CASE_SKIP(cmath, atan)
TEST_CASE_SKIP(cmath, cos)
TEST_CASE_SKIP(cmath, sin)
TEST_CASE_SKIP(cmath, tan)
TEST_CASE_SKIP(cmath, atan2)
TEST_CASE_SKIP(cmath, cosh)
TEST_CASE_SKIP(cmath, sinh)
TEST_CASE_SKIP(cmath, tanh)
TEST_CASE_SKIP(cmath, acosh_asinh_atanh)
TEST_CASE_SKIP(cmath, exp)
TEST_CASE_SKIP(cmath, exp2_expm1)
TEST_CASE_SKIP(cmath, log)
TEST_CASE_SKIP(cmath, log10_log2_log1p)
TEST_CASE_SKIP(cmath, sqrt_cbrt)
TEST_CASE_SKIP(cmath, pow)
TEST_CASE_SKIP(cmath, hypot)
#endif

#else
TEST_CASE_SKIP(cmath, acos)
TEST_CASE_SKIP(cmath, asin)
TEST_CASE_SKIP(cmath, atan)
TEST_CASE_SKIP(cmath, cos)
TEST_CASE_SKIP(cmath, sin)
TEST_CASE_SKIP(cmath, tan)
TEST_CASE_SKIP(cmath, atan2)
TEST_CASE_SKIP(cmath, cosh)
TEST_CASE_SKIP(cmath, sinh)
TEST_CASE_SKIP(cmath, tanh)
TEST_CASE_SKIP(cmath, acosh_asinh_atanh)
TEST_CASE_SKIP(cmath, exp)
TEST_CASE_SKIP(cmath, exp2_expm1)
TEST_CASE_SKIP(cmath, log)
TEST_CASE_SKIP(cmath, log10_log2_log1p)
TEST_CASE_SKIP(cmath, sqrt_cbrt)
TEST_CASE_SKIP(cmath, pow)
TEST_CASE_SKIP(cmath, hypot)
#endif
