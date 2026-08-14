#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011 && _TST_HAS_SPECIAL_MATH
#include <cmath>
#include <type_traits>

#if _TST_HAS_DECLTYPE

static bool near_eq(double a, double b, double eps = 1e-9) {
    return STD_NS fabs(a - b) <= eps * (STD_NS fabs(b) > 1.0 ? STD_NS fabs(b) : 1.0);
}

static const double kPi = 3.14159265358979323846;
static const double kEps  = 1e-10;
static const double kEpsF = 1e-5;

TEST_CASE(cmath, assoc_laguerre) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::assoc_laguerre(1u, 1u, 0.5f)), float>, "assoc_laguerre(u,u,float)");
    static_assert(STD::is_same_v<decltype(STD::assoc_laguerre(1u, 1u, 0.5)), double>, "assoc_laguerre(u,u,double)");
    static_assert(STD::is_same_v<decltype(STD::assoc_laguerre(1u, 1u, 0.5L)), long double>, "assoc_laguerre(u,u,ld)");
    static_assert(STD::is_same_v<decltype(STD::assoc_laguerref(1u, 1u, 0.5f)), float>, "assoc_laguerref");
    static_assert(STD::is_same_v<decltype(STD::assoc_laguerrel(1u, 1u, 0.5L)), long double>, "assoc_laguerrel");
#endif

    test_true( near_eq(STD::assoc_laguerre(0u, 0u, 0.5), 1.0, kEps) );
    test_true( near_eq(STD::assoc_laguerre(0u, 2u, 3.0), 1.0, kEps) );
    test_true( near_eq(STD::assoc_laguerre(1u, 0u, 0.5), 0.5, kEps) );
    test_true( near_eq(STD::assoc_laguerre(1u, 1u, 0.5), 1.5, kEps) );
    test_true( near_eq(STD::assoc_laguerre(1u, 2u, 1.0), 2.0, kEps) );
    test_true( near_eq(STD::assoc_laguerre(1u, 1u, 0.5f), 1.5, kEpsF) );
    test_true( near_eq((double)STD::assoc_laguerre(1u, 1u, 0.5L), 1.5, kEps) );
    test_true( near_eq(STD::assoc_laguerref(1u, 1u, 0.5f), 1.5, kEpsF) );
    test_true( near_eq((double)STD::assoc_laguerrel(1u, 1u, 0.5L), 1.5, kEps) );
    test_pass("cxx17:assoc_laguerre");
}

TEST_CASE(cmath, assoc_legendre) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::assoc_legendre(1u, 1u, 0.5f)), float>, "assoc_legendre(u,u,float)");
    static_assert(STD::is_same_v<decltype(STD::assoc_legendre(1u, 1u, 0.5)), double>, "assoc_legendre(u,u,double)");
    static_assert(STD::is_same_v<decltype(STD::assoc_legendre(1u, 1u, 0.5L)), long double>, "assoc_legendre(u,u,ld)");
    static_assert(STD::is_same_v<decltype(STD::assoc_legendref(1u, 1u, 0.5f)), float>, "assoc_legendref");
    static_assert(STD::is_same_v<decltype(STD::assoc_legendrel(1u, 1u, 0.5L)), long double>, "assoc_legendrel");
#endif

    test_true( near_eq(STD::assoc_legendre(0u, 0u, 0.5), 1.0, kEps) );
    test_true( near_eq(STD::assoc_legendre(1u, 0u, 0.5), 0.5, kEps) );
    test_true( near_eq(STD::assoc_legendre(2u, 0u, 0.5), -0.125, kEps) );

    double p11 = STD::assoc_legendre(1u, 1u, 0.5);
    test_true( near_eq(STD_NS fabs(p11), STD_NS sqrt(0.75), kEps) );
    double p21 = STD::assoc_legendre(2u, 1u, 0.5);
    test_true( near_eq(STD_NS fabs(p21), 3 * 0.5 * STD_NS sqrt(0.75), kEps) );
    test_true( near_eq(STD::assoc_legendre(2u, 0u, 0.5f), -0.125, kEpsF) );
    test_true( near_eq((double)STD::assoc_legendre(2u, 0u, 0.5L), -0.125, kEps) );
    test_true( near_eq(STD::assoc_legendref(1u, 0u, 0.5f), 0.5, kEpsF) );
    test_true( near_eq((double)STD::assoc_legendrel(1u, 0u, 0.5L), 0.5, kEps) );
    test_pass("cxx17:assoc_legendre");
}

TEST_CASE(cmath, beta) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::beta(2.0f, 3.0f)), float>, "beta(float,float)");
    static_assert(STD::is_same_v<decltype(STD::beta(2.0, 3.0)), double>, "beta(double,double)");
    static_assert(STD::is_same_v<decltype(STD::beta(2.0L, 3.0L)), long double>, "beta(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD::betaf(2.0f, 3.0f)), float>, "betaf");
    static_assert(STD::is_same_v<decltype(STD::betal(2.0L, 3.0L)), long double>, "betal");
#endif

    test_true( near_eq(STD::beta(1.0, 1.0), 1.0, kEps) );
    test_true( near_eq(STD::beta(2.0, 3.0), 1.0 / 12, kEps) );
    test_true( near_eq(STD::beta(3.0, 2.0), STD::beta(2.0, 3.0), kEps) );
    test_true( near_eq(STD::beta(2.5, 1.5),
                       STD_NS tgamma(2.5) * STD_NS tgamma(1.5) / STD_NS tgamma(4.0), 1e-9) );
    test_true( near_eq(STD::beta(2.0f, 3.0f), 1.0 / 12, kEpsF) );
    test_true( near_eq((double)STD::beta(2.0L, 3.0L), 1.0 / 12, kEps) );
    test_true( near_eq(STD::betaf(2.0f, 3.0f), 1.0 / 12, kEpsF) );
    test_true( near_eq((double)STD::betal(2.0L, 3.0L), 1.0 / 12, kEps) );
    test_pass("cxx17:beta");
}

TEST_CASE(cmath, comp_ellint_1_2) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_1(0.5f)), float>, "comp_ellint_1(float)");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_1(0.5)), double>, "comp_ellint_1(double)");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_1(0.5L)), long double>, "comp_ellint_1(ld)");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_1f(0.5f)), float>, "comp_ellint_1f");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_1l(0.5L)), long double>, "comp_ellint_1l");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_2(0.5f)), float>, "comp_ellint_2(float)");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_2(0.5)), double>, "comp_ellint_2(double)");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_2(0.5L)), long double>, "comp_ellint_2(ld)");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_2f(0.5f)), float>, "comp_ellint_2f");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_2l(0.5L)), long double>, "comp_ellint_2l");
#endif

    test_true( near_eq(STD::comp_ellint_1(0.0), kPi / 2, kEps) );
    test_true( near_eq(STD::comp_ellint_1(0.5), 1.6857503548125961, 1e-9) );
    test_true( near_eq(STD::comp_ellint_1(0.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD::comp_ellint_1(0.0L), kPi / 2, kEps) );
    test_true( near_eq(STD::comp_ellint_1f(0.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD::comp_ellint_1l(0.0L), kPi / 2, kEps) );
    test_pass("cxx17:comp_ellint_1");

    test_true( near_eq(STD::comp_ellint_2(0.0), kPi / 2, kEps) );
    test_true( near_eq(STD::comp_ellint_2(1.0), 1.0, 1e-9) );
    test_true( near_eq(STD::comp_ellint_2(0.5), 1.4674622093394272, 1e-9) );
    test_true( near_eq(STD::comp_ellint_2(0.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD::comp_ellint_2(0.0L), kPi / 2, kEps) );
    test_true( near_eq(STD::comp_ellint_2f(0.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD::comp_ellint_2l(0.0L), kPi / 2, kEps) );
    test_pass("cxx17:comp_ellint_2");
}

TEST_CASE(cmath, comp_ellint_3) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_3(0.5f, 0.5f)), float>, "comp_ellint_3(float,float)");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_3(0.5, 0.5)), double>, "comp_ellint_3(double,double)");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_3(0.5L, 0.5L)), long double>, "comp_ellint_3(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_3f(0.5f, 0.5f)), float>, "comp_ellint_3f");
    static_assert(STD::is_same_v<decltype(STD::comp_ellint_3l(0.5L, 0.5L)), long double>, "comp_ellint_3l");
#endif

    test_true( near_eq(STD::comp_ellint_3(0.0, 0.0), kPi / 2, kEps) );
    test_true( near_eq(STD::comp_ellint_3(0.0, 0.5), kPi / (2 * STD_NS sqrt(0.5)), 1e-9) );
    test_true( near_eq(STD::comp_ellint_3(0.5, 0.0), STD::comp_ellint_1(0.5), 1e-9) );
    test_true( near_eq(STD::comp_ellint_3(0.0f, 0.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD::comp_ellint_3(0.0L, 0.0L), kPi / 2, kEps) );
    test_true( near_eq(STD::comp_ellint_3f(0.0f, 0.0f), kPi / 2, kEpsF) );
    test_true( near_eq((double)STD::comp_ellint_3l(0.0L, 0.0L), kPi / 2, kEps) );
    test_pass("cxx17:comp_ellint_3");
}

TEST_CASE(cmath, ellint_1_2) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::ellint_1(0.5f, 0.5f)), float>, "ellint_1(float,float)");
    static_assert(STD::is_same_v<decltype(STD::ellint_1(0.5, 0.5)), double>, "ellint_1(double,double)");
    static_assert(STD::is_same_v<decltype(STD::ellint_1(0.5L, 0.5L)), long double>, "ellint_1(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD::ellint_1f(0.5f, 0.5f)), float>, "ellint_1f");
    static_assert(STD::is_same_v<decltype(STD::ellint_1l(0.5L, 0.5L)), long double>, "ellint_1l");
    static_assert(STD::is_same_v<decltype(STD::ellint_2(0.5f, 0.5f)), float>, "ellint_2(float,float)");
    static_assert(STD::is_same_v<decltype(STD::ellint_2(0.5, 0.5)), double>, "ellint_2(double,double)");
    static_assert(STD::is_same_v<decltype(STD::ellint_2(0.5L, 0.5L)), long double>, "ellint_2(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD::ellint_2f(0.5f, 0.5f)), float>, "ellint_2f");
    static_assert(STD::is_same_v<decltype(STD::ellint_2l(0.5L, 0.5L)), long double>, "ellint_2l");
#endif

    test_true( near_eq(STD::ellint_1(0.0, kPi / 4), kPi / 4, kEps) );
    test_true( near_eq(STD::ellint_2(0.0, kPi / 4), kPi / 4, kEps) );

    test_true( near_eq(STD::ellint_1(0.5, kPi / 2), STD::comp_ellint_1(0.5), 1e-9) );
    test_true( near_eq(STD::ellint_2(0.5, kPi / 2), STD::comp_ellint_2(0.5), 1e-9) );
    test_true( near_eq(STD::ellint_1(0.0, 0.0), 0.0, kEps) );
    test_true( near_eq(STD::ellint_2(0.0, 0.0), 0.0, kEps) );

    test_true( near_eq(STD::ellint_1(0.0f, 1.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD::ellint_1(0.0L, 1.0L), 1.0, kEps) );
    test_true( near_eq(STD::ellint_1f(0.0f, 1.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD::ellint_1l(0.0L, 1.0L), 1.0, kEps) );
    test_pass("cxx17:ellint_1");
    test_true( near_eq(STD::ellint_2(0.0f, 1.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD::ellint_2(0.0L, 1.0L), 1.0, kEps) );
    test_true( near_eq(STD::ellint_2f(0.0f, 1.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD::ellint_2l(0.0L, 1.0L), 1.0, kEps) );
    test_pass("cxx17:ellint_2");
}

TEST_CASE(cmath, ellint_3) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::ellint_3(0.5f, 0.5f, 0.5f)), float>, "ellint_3(f,f,f)");
    static_assert(STD::is_same_v<decltype(STD::ellint_3(0.5, 0.5, 0.5)), double>, "ellint_3(d,d,d)");
    static_assert(STD::is_same_v<decltype(STD::ellint_3(0.5L, 0.5L, 0.5L)), long double>, "ellint_3(ld,ld,ld)");
    static_assert(STD::is_same_v<decltype(STD::ellint_3f(0.5f, 0.5f, 0.5f)), float>, "ellint_3f");
    static_assert(STD::is_same_v<decltype(STD::ellint_3l(0.5L, 0.5L, 0.5L)), long double>, "ellint_3l");
#endif

    test_true( near_eq(STD::ellint_3(0.0, 0.0, kPi / 4), kPi / 4, kEps) );
    test_true( near_eq(STD::ellint_3(0.0, 0.0, 1.0), 1.0, kEps) );

    test_true( near_eq(STD::ellint_3(0.5, 0.0, 1.0), STD::ellint_1(0.5, 1.0), 1e-9) );

    test_true( near_eq(STD::ellint_3(0.5, 0.25, kPi / 2), STD::comp_ellint_3(0.5, 0.25), 1e-9) );
    test_true( near_eq(STD::ellint_3(0.0f, 0.0f, 1.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD::ellint_3(0.0L, 0.0L, 1.0L), 1.0, kEps) );
    test_true( near_eq(STD::ellint_3f(0.0f, 0.0f, 1.0f), 1.0, kEpsF) );
    test_true( near_eq((double)STD::ellint_3l(0.0L, 0.0L, 1.0L), 1.0, kEps) );
    test_pass("cxx17:ellint_3");
}

TEST_CASE(cmath, cyl_bessel_i) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_i(0.0f, 1.0f)), float>, "cyl_bessel_i(float,float)");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_i(0.0, 1.0)), double>, "cyl_bessel_i(double,double)");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_i(0.0L, 1.0L)), long double>, "cyl_bessel_i(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_if(0.0f, 1.0f)), float>, "cyl_bessel_if");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_il(0.0L, 1.0L)), long double>, "cyl_bessel_il");
#endif

    test_true( near_eq(STD::cyl_bessel_i(0.0, 0.0), 1.0, kEps) );
    test_true( near_eq(STD::cyl_bessel_i(1.0, 0.0), 0.0, kEps) );
    test_true( near_eq(STD::cyl_bessel_i(0.0, 1.0), 1.2660658777520084, 1e-9) );
    test_true( near_eq(STD::cyl_bessel_i(1.0, 1.0), 0.5651591039924850, 1e-9) );
    test_true( near_eq(STD::cyl_bessel_i(0.0f, 1.0f), 1.2660658777520084, kEpsF) );
    test_true( near_eq((double)STD::cyl_bessel_i(0.0L, 1.0L), 1.2660658777520084, 1e-9) );
    test_true( near_eq(STD::cyl_bessel_if(0.0f, 1.0f), 1.2660658777520084, kEpsF) );
    test_true( near_eq((double)STD::cyl_bessel_il(0.0L, 1.0L), 1.2660658777520084, 1e-9) );
    test_pass("cxx17:cyl_bessel_i");
}

TEST_CASE(cmath, cyl_bessel_j) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_j(0.0f, 1.0f)), float>, "cyl_bessel_j(float,float)");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_j(0.0, 1.0)), double>, "cyl_bessel_j(double,double)");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_j(0.0L, 1.0L)), long double>, "cyl_bessel_j(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_jf(0.0f, 1.0f)), float>, "cyl_bessel_jf");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_jl(0.0L, 1.0L)), long double>, "cyl_bessel_jl");
#endif

    test_true( near_eq(STD::cyl_bessel_j(0.0, 0.0), 1.0, kEps) );
    test_true( near_eq(STD::cyl_bessel_j(1.0, 0.0), 0.0, kEps) );
    test_true( near_eq(STD::cyl_bessel_j(0.0, 1.0), 0.7651976865579666, 1e-9) );
    test_true( near_eq(STD::cyl_bessel_j(1.0, 1.0), 0.4400505857449335, 1e-9) );
    test_true( near_eq(STD::cyl_bessel_j(0.0f, 1.0f), 0.7651976865579666, kEpsF) );
    test_true( near_eq((double)STD::cyl_bessel_j(0.0L, 1.0L), 0.7651976865579666, 1e-9) );
    test_true( near_eq(STD::cyl_bessel_jf(0.0f, 1.0f), 0.7651976865579666, kEpsF) );
    test_true( near_eq((double)STD::cyl_bessel_jl(0.0L, 1.0L), 0.7651976865579666, 1e-9) );
    test_pass("cxx17:cyl_bessel_j");
}

TEST_CASE(cmath, cyl_bessel_k_neumann) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_k(0.0f, 1.0f)), float>, "cyl_bessel_k(float,float)");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_k(0.0, 1.0)), double>, "cyl_bessel_k(double,double)");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_k(0.0L, 1.0L)), long double>, "cyl_bessel_k(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_kf(0.0f, 1.0f)), float>, "cyl_bessel_kf");
    static_assert(STD::is_same_v<decltype(STD::cyl_bessel_kl(0.0L, 1.0L)), long double>, "cyl_bessel_kl");
    static_assert(STD::is_same_v<decltype(STD::cyl_neumann(0.0f, 1.0f)), float>, "cyl_neumann(float,float)");
    static_assert(STD::is_same_v<decltype(STD::cyl_neumann(0.0, 1.0)), double>, "cyl_neumann(double,double)");
    static_assert(STD::is_same_v<decltype(STD::cyl_neumann(0.0L, 1.0L)), long double>, "cyl_neumann(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD::cyl_neumannf(0.0f, 1.0f)), float>, "cyl_neumannf");
    static_assert(STD::is_same_v<decltype(STD::cyl_neumannl(0.0L, 1.0L)), long double>, "cyl_neumannl");
#endif

    test_true( near_eq(STD::cyl_bessel_k(0.0, 1.0), 0.4210244382407083, 1e-9) );
    test_true( near_eq(STD::cyl_bessel_k(1.0, 1.0), 0.6019072301972346, 1e-9) );
    test_true( STD::cyl_bessel_k(0.0, 2.0) < STD::cyl_bessel_k(0.0, 1.0) );
    test_true( near_eq(STD::cyl_bessel_k(0.0f, 1.0f), 0.4210244382407083, kEpsF) );
    test_true( near_eq((double)STD::cyl_bessel_k(0.0L, 1.0L), 0.4210244382407083, 1e-9) );
    test_true( near_eq(STD::cyl_bessel_kf(0.0f, 1.0f), 0.4210244382407083, kEpsF) );
    test_true( near_eq((double)STD::cyl_bessel_kl(0.0L, 1.0L), 0.4210244382407083, 1e-9) );
    test_pass("cxx17:cyl_bessel_k");

    test_true( near_eq(STD::cyl_neumann(0.0, 1.0), 0.0882569642156769, 1e-9) );
    test_true( near_eq(STD::cyl_neumann(1.0, 1.0), -0.7812128213002887, 1e-9) );
    test_true( near_eq(STD::cyl_neumann(0.0f, 1.0f), 0.0882569642156769, 1e-4) );
    test_true( near_eq((double)STD::cyl_neumann(0.0L, 1.0L), 0.0882569642156769, 1e-9) );
    test_true( near_eq(STD::cyl_neumannf(1.0f, 1.0f), -0.7812128213002887, kEpsF) );
    test_true( near_eq((double)STD::cyl_neumannl(1.0L, 1.0L), -0.7812128213002887, 1e-9) );
    test_pass("cxx17:cyl_neumann");
}

TEST_CASE(cmath, expint) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::expint(1.0f)), float>, "expint(float)");
    static_assert(STD::is_same_v<decltype(STD::expint(1.0)), double>, "expint(double)");
    static_assert(STD::is_same_v<decltype(STD::expint(1.0L)), long double>, "expint(long double)");
    static_assert(STD::is_same_v<decltype(STD::expintf(1.0f)), float>, "expintf");
    static_assert(STD::is_same_v<decltype(STD::expintl(1.0L)), long double>, "expintl");
#endif

    test_true( near_eq(STD::expint(1.0), 1.8951178163559368, 1e-9) );
    test_true( near_eq(STD::expint(-1.0), -0.2193839343955203, 1e-9) );
    test_true( near_eq(STD::expint(2.0), 4.9542343560018907, 1e-9) );
    test_true( near_eq(STD::expint(1.0f), 1.8951178163559368, kEpsF) );
    test_true( near_eq((double)STD::expint(1.0L), 1.8951178163559368, 1e-9) );
    test_true( near_eq(STD::expintf(1.0f), 1.8951178163559368, kEpsF) );
    test_true( near_eq((double)STD::expintl(1.0L), 1.8951178163559368, 1e-9) );
    test_pass("cxx17:expint");
}

TEST_CASE(cmath, hermite) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::hermite(2u, 0.5f)), float>, "hermite(unsigned,float)");
    static_assert(STD::is_same_v<decltype(STD::hermite(2u, 0.5)), double>, "hermite(unsigned,double)");
    static_assert(STD::is_same_v<decltype(STD::hermite(2u, 0.5L)), long double>, "hermite(unsigned,ld)");
    static_assert(STD::is_same_v<decltype(STD::hermitef(2u, 0.5f)), float>, "hermitef");
    static_assert(STD::is_same_v<decltype(STD::hermitel(2u, 0.5L)), long double>, "hermitel");
#endif

    test_true( near_eq(STD::hermite(0u, 0.5), 1.0, kEps) );
    test_true( near_eq(STD::hermite(1u, 0.5), 1.0, kEps) );
    test_true( near_eq(STD::hermite(2u, 0.5), -1.0, kEps) );
    test_true( near_eq(STD::hermite(3u, 0.5), 8 * 0.125 - 6.0, 1e-9) );
    test_true( near_eq(STD::hermite(2u, 0.0), -2.0, kEps) );
    test_true( near_eq(STD::hermite(2u, 0.5f), -1.0, kEpsF) );
    test_true( near_eq((double)STD::hermite(2u, 0.5L), -1.0, kEps) );
    test_true( near_eq(STD::hermitef(2u, 0.5f), -1.0, kEpsF) );
    test_true( near_eq((double)STD::hermitel(2u, 0.5L), -1.0, kEps) );
    test_pass("cxx17:hermite");
}

TEST_CASE(cmath, laguerre) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::laguerre(2u, 0.5f)), float>, "laguerre(unsigned,float)");
    static_assert(STD::is_same_v<decltype(STD::laguerre(2u, 0.5)), double>, "laguerre(unsigned,double)");
    static_assert(STD::is_same_v<decltype(STD::laguerre(2u, 0.5L)), long double>, "laguerre(unsigned,ld)");
    static_assert(STD::is_same_v<decltype(STD::laguerref(2u, 0.5f)), float>, "laguerref");
    static_assert(STD::is_same_v<decltype(STD::laguerrel(2u, 0.5L)), long double>, "laguerrel");
#endif

    test_true( near_eq(STD::laguerre(0u, 0.5), 1.0, kEps) );
    test_true( near_eq(STD::laguerre(1u, 0.5), 0.5, kEps) );
    test_true( near_eq(STD::laguerre(2u, 0.5), 1.0 - 1.0 + 0.125, 1e-9) );
    test_true( near_eq(STD::laguerre(1u, 0.0), 1.0, kEps) );
    test_true( near_eq(STD::laguerre(2u, 0.0), 1.0, kEps) );

    test_true( near_eq(STD::laguerre(2u, 0.5), STD::assoc_laguerre(2u, 0u, 0.5), 1e-9) );
    test_true( near_eq(STD::laguerre(1u, 0.5f), 0.5, kEpsF) );
    test_true( near_eq((double)STD::laguerre(1u, 0.5L), 0.5, kEps) );
    test_true( near_eq(STD::laguerref(1u, 0.5f), 0.5, kEpsF) );
    test_true( near_eq((double)STD::laguerrel(1u, 0.5L), 0.5, kEps) );
    test_pass("cxx17:laguerre");
}

TEST_CASE(cmath, legendre) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::legendre(2u, 0.5f)), float>, "legendre(unsigned,float)");
    static_assert(STD::is_same_v<decltype(STD::legendre(2u, 0.5)), double>, "legendre(unsigned,double)");
    static_assert(STD::is_same_v<decltype(STD::legendre(2u, 0.5L)), long double>, "legendre(unsigned,ld)");
    static_assert(STD::is_same_v<decltype(STD::legendref(2u, 0.5f)), float>, "legendref");
    static_assert(STD::is_same_v<decltype(STD::legendrel(2u, 0.5L)), long double>, "legendrel");
#endif

    test_true( near_eq(STD::legendre(0u, 0.5), 1.0, kEps) );
    test_true( near_eq(STD::legendre(1u, 0.5), 0.5, kEps) );
    test_true( near_eq(STD::legendre(2u, 0.5), -0.125, kEps) );
    test_true( near_eq(STD::legendre(3u, 0.5), -0.4375, 1e-9) );
    test_true( near_eq(STD::legendre(5u, 1.0), 1.0, 1e-9) );
    test_true( near_eq(STD::legendre(3u, -0.5), 0.4375, 1e-9) );
    test_true( near_eq(STD::legendre(2u, 0.5f), -0.125, kEpsF) );
    test_true( near_eq((double)STD::legendre(2u, 0.5L), -0.125, kEps) );
    test_true( near_eq(STD::legendref(2u, 0.5f), -0.125, kEpsF) );
    test_true( near_eq((double)STD::legendrel(2u, 0.5L), -0.125, kEps) );
    test_pass("cxx17:legendre");
}

TEST_CASE(cmath, riemann_zeta) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::riemann_zeta(2.0f)), float>, "riemann_zeta(float)");
    static_assert(STD::is_same_v<decltype(STD::riemann_zeta(2.0)), double>, "riemann_zeta(double)");
    static_assert(STD::is_same_v<decltype(STD::riemann_zeta(2.0L)), long double>, "riemann_zeta(ld)");
    static_assert(STD::is_same_v<decltype(STD::riemann_zetaf(2.0f)), float>, "riemann_zetaf");
    static_assert(STD::is_same_v<decltype(STD::riemann_zetal(2.0L)), long double>, "riemann_zetal");
#endif

    test_true( near_eq(STD::riemann_zeta(2.0), kPi * kPi / 6, 1e-9) );
    test_true( near_eq(STD::riemann_zeta(4.0), kPi * kPi * kPi * kPi / 90, 1e-9) );
    test_true( near_eq(STD::riemann_zeta(0.0), -0.5, 1e-9) );
    test_true( near_eq(STD::riemann_zeta(-1.0), -1.0 / 12, 1e-9) );
    test_true( STD::riemann_zeta(10.0) > 1.0 );
    test_true( near_eq(STD::riemann_zeta(2.0f), kPi * kPi / 6, kEpsF) );
    test_true( near_eq((double)STD::riemann_zeta(2.0L), kPi * kPi / 6, 1e-9) );
    test_true( near_eq(STD::riemann_zetaf(2.0f), kPi * kPi / 6, kEpsF) );
    test_true( near_eq((double)STD::riemann_zetal(2.0L), kPi * kPi / 6, 1e-9) );
    test_pass("cxx17:riemann_zeta");
}

TEST_CASE(cmath, sph_bessel_neumann) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::sph_bessel(0u, 1.0f)), float>, "sph_bessel(unsigned,float)");
    static_assert(STD::is_same_v<decltype(STD::sph_bessel(0u, 1.0)), double>, "sph_bessel(unsigned,double)");
    static_assert(STD::is_same_v<decltype(STD::sph_bessel(0u, 1.0L)), long double>, "sph_bessel(unsigned,ld)");
    static_assert(STD::is_same_v<decltype(STD::sph_besself(0u, 1.0f)), float>, "sph_besself");
    static_assert(STD::is_same_v<decltype(STD::sph_bessell(0u, 1.0L)), long double>, "sph_bessell");
    static_assert(STD::is_same_v<decltype(STD::sph_neumann(0u, 1.0f)), float>, "sph_neumann(unsigned,float)");
    static_assert(STD::is_same_v<decltype(STD::sph_neumann(0u, 1.0)), double>, "sph_neumann(unsigned,double)");
    static_assert(STD::is_same_v<decltype(STD::sph_neumann(0u, 1.0L)), long double>, "sph_neumann(unsigned,ld)");
    static_assert(STD::is_same_v<decltype(STD::sph_neumannf(0u, 1.0f)), float>, "sph_neumannf");
    static_assert(STD::is_same_v<decltype(STD::sph_neumannl(0u, 1.0L)), long double>, "sph_neumannl");
#endif

    test_true( near_eq(STD::sph_bessel(0u, 1.0), STD_NS sin(1.0) / 1.0, 1e-9) );
    test_true( near_eq(STD::sph_bessel(0u, 0.0), 1.0, kEps) );
    test_true( near_eq(STD::sph_bessel(1u, 0.0), 0.0, kEps) );
    test_true( near_eq(STD::sph_bessel(1u, 1.0), 0.3011686789397568, 1e-9) );
    test_true( near_eq(STD::sph_bessel(0u, 1.0f), STD_NS sin(1.0), kEpsF) );
    test_true( near_eq((double)STD::sph_bessel(0u, 1.0L), STD_NS sin(1.0), 1e-9) );
    test_true( near_eq(STD::sph_besself(0u, 1.0f), STD_NS sin(1.0), kEpsF) );
    test_true( near_eq((double)STD::sph_bessell(0u, 1.0L), STD_NS sin(1.0), 1e-9) );
    test_pass("cxx17:sph_bessel");

    test_true( near_eq(STD::sph_neumann(0u, 1.0), -STD_NS cos(1.0), 1e-9) );
    test_true( near_eq(STD::sph_neumann(1u, 1.0), -1.3817732906760363, 1e-9) );
    test_true( near_eq(STD::sph_neumann(0u, 1.0f), -STD_NS cos(1.0), kEpsF) );
    test_true( near_eq((double)STD::sph_neumann(0u, 1.0L), -STD_NS cos(1.0), 1e-9) );
    test_true( near_eq(STD::sph_neumannf(0u, 1.0f), -STD_NS cos(1.0), kEpsF) );
    test_true( near_eq((double)STD::sph_neumannl(0u, 1.0L), -STD_NS cos(1.0), 1e-9) );
    test_pass("cxx17:sph_neumann");
}

TEST_CASE(cmath, sph_legendre) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::sph_legendre(1u, 0u, 1.0f)), float>, "sph_legendre(u,u,float)");
    static_assert(STD::is_same_v<decltype(STD::sph_legendre(1u, 0u, 1.0)), double>, "sph_legendre(u,u,double)");
    static_assert(STD::is_same_v<decltype(STD::sph_legendre(1u, 0u, 1.0L)), long double>, "sph_legendre(u,u,ld)");
    static_assert(STD::is_same_v<decltype(STD::sph_legendref(1u, 0u, 1.0f)), float>, "sph_legendref");
    static_assert(STD::is_same_v<decltype(STD::sph_legendrel(1u, 0u, 1.0L)), long double>, "sph_legendrel");
#endif

    const double y00 = STD_NS sqrt(1.0 / (4 * kPi));
    test_true( near_eq(STD::sph_legendre(0u, 0u, 0.0), y00, 1e-9) );
    test_true( near_eq(STD::sph_legendre(0u, 0u, 1.0), y00, 1e-9) );

    const double y10 = STD_NS sqrt(3.0 / (4 * kPi));
    test_true( near_eq(STD::sph_legendre(1u, 0u, 0.0), y10, 1e-9) );
    test_true( near_eq(STD::sph_legendre(1u, 0u, kPi / 2), 0.0, 1e-9) );
    test_true( near_eq(STD::sph_legendre(1u, 0u, 1.0), y10 * STD_NS cos(1.0), 1e-9) );
    test_true( near_eq(STD::sph_legendre(0u, 0u, 1.0f), y00, kEpsF) );
    test_true( near_eq((double)STD::sph_legendre(0u, 0u, 1.0L), y00, 1e-9) );
    test_true( near_eq(STD::sph_legendref(0u, 0u, 1.0f), y00, kEpsF) );
    test_true( near_eq((double)STD::sph_legendrel(0u, 0u, 1.0L), y00, 1e-9) );
    test_pass("cxx17:sph_legendre");
}

#if TEST_TARGET_CXX >= 2020
TEST_CASE(cmath, lerp_chk) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD::lerp(0.0f, 1.0f, 0.5f)), float>, "lerp(f,f,f)");
    static_assert(STD::is_same_v<decltype(STD::lerp(0.0, 1.0, 0.5)), double>, "lerp(d,d,d)");
    static_assert(STD::is_same_v<decltype(STD::lerp(0.0L, 1.0L, 0.5L)), long double>, "lerp(ld,ld,ld)");
#endif

    test_true( STD::lerp(0.0, 10.0, 0.5) == 5.0 );
    test_true( STD::lerp(0.0, 10.0, 0.0) == 0.0 );
    test_true( STD::lerp(0.0, 10.0, 1.0) == 10.0 );
    test_true( STD::lerp(2.0, 4.0, 0.25) == 2.5 );
    test_true( STD::lerp(0.0, 10.0, 2.0) == 20.0 );
    test_true( STD::lerp(0.0, 10.0, -1.0) == -10.0 );
    test_true( STD::lerp(0.0f, 10.0f, 0.5f) == 5.0f );
    test_true( STD::lerp(0.0L, 10.0L, 0.5L) == 5.0L );

#if _TST_HAS_VAR_TEMPLATE
    constexpr double c = STD::lerp(0.0, 8.0, 0.25);
    static_assert(c == 2.0, "lerp is constexpr");
#else
    const double c = STD::lerp(0.0, 8.0, 0.25);
#endif
    test_true( c == 2.0 );
    test_pass("cxx20:lerp");
}
#else
TEST_CASE(cmath, lerp_chk) {
    TEST_NOTE("lerp is C++20");
    test_skip("cxx20:lerp");
}
#endif
#else
TEST_CASE_SKIP(cmath, assoc_laguerre)
TEST_CASE_SKIP(cmath, assoc_legendre)
TEST_CASE_SKIP(cmath, beta)
TEST_CASE_SKIP(cmath, comp_ellint_1_2)
TEST_CASE_SKIP(cmath, comp_ellint_3)
TEST_CASE_SKIP(cmath, ellint_1_2)
TEST_CASE_SKIP(cmath, ellint_3)
TEST_CASE_SKIP(cmath, cyl_bessel_i)
TEST_CASE_SKIP(cmath, cyl_bessel_j)
TEST_CASE_SKIP(cmath, cyl_bessel_k_neumann)
TEST_CASE_SKIP(cmath, expint)
TEST_CASE_SKIP(cmath, hermite)
TEST_CASE_SKIP(cmath, laguerre)
TEST_CASE_SKIP(cmath, legendre)
TEST_CASE_SKIP(cmath, riemann_zeta)
TEST_CASE_SKIP(cmath, sph_bessel_neumann)
TEST_CASE_SKIP(cmath, sph_legendre)
TEST_CASE_SKIP(cmath, lerp_chk)
#endif

#else
TEST_CASE_SKIP(cmath, assoc_laguerre)
TEST_CASE_SKIP(cmath, assoc_legendre)
TEST_CASE_SKIP(cmath, beta)
TEST_CASE_SKIP(cmath, comp_ellint_1_2)
TEST_CASE_SKIP(cmath, comp_ellint_3)
TEST_CASE_SKIP(cmath, ellint_1_2)
TEST_CASE_SKIP(cmath, ellint_3)
TEST_CASE_SKIP(cmath, cyl_bessel_i)
TEST_CASE_SKIP(cmath, cyl_bessel_j)
TEST_CASE_SKIP(cmath, cyl_bessel_k_neumann)
TEST_CASE_SKIP(cmath, expint)
TEST_CASE_SKIP(cmath, hermite)
TEST_CASE_SKIP(cmath, laguerre)
TEST_CASE_SKIP(cmath, legendre)
TEST_CASE_SKIP(cmath, riemann_zeta)
TEST_CASE_SKIP(cmath, sph_bessel_neumann)
TEST_CASE_SKIP(cmath, sph_legendre)
TEST_CASE_SKIP(cmath, lerp_chk)
#endif
