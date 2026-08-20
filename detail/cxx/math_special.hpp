/**
 *  @file   detail/math_special.hpp
 *  @brief  Those C++17 special math functions that a closed recurrence covers (the polynomial family) plus beta.
 *  @license Boost Software License Version 1.0
 *  @note   Active only for `_CCW_TARGET_CXX >= 2017`. The namespace is `_CCW_STD`, so in a std build
 */
#ifndef _CCW_DETAIL_MATH_SPECIAL_HPP
#define _CCW_DETAIL_MATH_SPECIAL_HPP
#include <ccwrap_common.h>

#if _CCW_TARGET_CXX >= 2017

#if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION >= 190000 && defined(__cpp_lib_math_special_functions) == 0
# define _CCW_SF_NATIVE_HERMITE 1
#endif

namespace _CCW_STD {

#ifndef _CCW_SF_NATIVE_HERMITE
inline double hermite(unsigned __n, double __x) {
    if (__n == 0u) return 1.0;
    double __h0 = 1.0, __h1 = 2.0 * __x;
    for (unsigned __k = 1u; __k < __n; ++__k) {
        double __h2 = 2.0 * __x * __h1 - 2.0 * (double)__k * __h0;
        __h0 = __h1; __h1 = __h2;
    }
    return __h1;
}
#endif // !_CCW_SF_NATIVE_HERMITE

inline double laguerre(unsigned __n, double __x) {
    if (__n == 0u) return 1.0;
    double __l0 = 1.0, __l1 = 1.0 - __x;
    for (unsigned __k = 1u; __k < __n; ++__k) {
        double __l2 = ((2.0 * (double)__k + 1.0 - __x) * __l1 - (double)__k * __l0) / ((double)__k + 1.0);
        __l0 = __l1; __l1 = __l2;
    }
    return __l1;
}

inline double legendre(unsigned __l, double __x) {
    if (__l == 0u) return 1.0;
    double __p0 = 1.0, __p1 = __x;
    for (unsigned __k = 1u; __k < __l; ++__k) {
        double __p2 = ((2.0 * (double)__k + 1.0) * __x * __p1 - (double)__k * __p0) / ((double)__k + 1.0);
        __p0 = __p1; __p1 = __p2;
    }
    return __p1;
}

inline double assoc_laguerre(unsigned __n, unsigned __m, double __x) {
    if (__n == 0u) return 1.0;
    double __m_d = (double)__m;
    double __l0 = 1.0, __l1 = 1.0 + __m_d - __x;
    for (unsigned __k = 1u; __k < __n; ++__k) {
        double __k_d = (double)__k;
        double __l2 = ((2.0 * __k_d + __m_d + 1.0 - __x) * __l1 - (__k_d + __m_d) * __l0) / (__k_d + 1.0);
        __l0 = __l1; __l1 = __l2;
    }
    return __l1;
}

inline double assoc_legendre(unsigned __l, unsigned __m, double __x) {
    if (__m > __l) return 0.0;
    double __pmm = 1.0;
    if (__m > 0u) {
        double __somx2 = ::std::sqrt((1.0 - __x) * (1.0 + __x));   /* (1-x^2)^{1/2} */
        double __fact = 1.0;
        for (unsigned __i = 1u; __i <= __m; ++__i) { __pmm *= __fact * __somx2; __fact += 2.0; }
    }
    if (__l == __m) return __pmm;
    double __pmmp1 = __x * (2.0 * (double)__m + 1.0) * __pmm;   /* P_{m+1}^m */
    if (__l == __m + 1u) return __pmmp1;
    double __pll = 0.0;
    for (unsigned __ll = __m + 2u; __ll <= __l; ++__ll) {
        double __ll_d = (double)__ll;
        __pll = ((2.0 * __ll_d - 1.0) * __x * __pmmp1 - (__ll_d - 1.0 + (double)__m) * __pmm) / (__ll_d - (double)__m);
        __pmm = __pmmp1; __pmmp1 = __pll;
    }
    return __pll;
}

inline double beta(double __a, double __b) {
    return ::std::exp(::std::lgamma(__a) + ::std::lgamma(__b) - ::std::lgamma(__a + __b));
}

inline double __ccw_ell_max3(double __a, double __b, double __c) {
    double __m = __a > __b ? __a : __b; return __m > __c ? __m : __c;
}

inline double __ccw_rc(double __x, double __y) {
    const double C1 = 0.3, C2 = 1.0/7.0, C3 = 0.375, C4 = 9.0/22.0, ERRTOL = 0.0012;
    double __xt, __yt, __w;
    if (__y > 0.0) { __xt = __x; __yt = __y; __w = 1.0; }
    else { __xt = __x - __y; __yt = -__y; __w = ::std::sqrt(__x) / ::std::sqrt(__xt); }
    double __ave, __s;
    do {
        double __alamb = 2.0 * ::std::sqrt(__xt) * ::std::sqrt(__yt) + __yt;
        __xt = 0.25 * (__xt + __alamb); __yt = 0.25 * (__yt + __alamb);
        __ave = (__xt + __yt + __yt) / 3.0;
        __s = (__yt - __ave) / __ave;
    } while (::std::fabs(__s) > ERRTOL);
    return __w * (1.0 + __s * __s * (C1 + __s * (C2 + __s * (C3 + __s * C4)))) / ::std::sqrt(__ave);
}

inline double __ccw_rf(double __x, double __y, double __z) {
    const double ERRTOL = 0.0025, C1 = 1.0/24.0, C2 = 0.1, C3 = 3.0/44.0, C4 = 1.0/14.0;
    double __xt = __x, __yt = __y, __zt = __z, __ave, __dx, __dy, __dz;
    do {
        double __sx = ::std::sqrt(__xt), __sy = ::std::sqrt(__yt), __sz = ::std::sqrt(__zt);
        double __alamb = __sx * (__sy + __sz) + __sy * __sz;
        __xt = 0.25 * (__xt + __alamb); __yt = 0.25 * (__yt + __alamb); __zt = 0.25 * (__zt + __alamb);
        __ave = (__xt + __yt + __zt) / 3.0;
        __dx = (__ave - __xt) / __ave; __dy = (__ave - __yt) / __ave; __dz = (__ave - __zt) / __ave;
    } while (__ccw_ell_max3(::std::fabs(__dx), ::std::fabs(__dy), ::std::fabs(__dz)) > ERRTOL);
    double __e2 = __dx * __dy - __dz * __dz, __e3 = __dx * __dy * __dz;
    return (1.0 + (C1 * __e2 - C2 - C3 * __e3) * __e2 + C4 * __e3) / ::std::sqrt(__ave);
}

inline double __ccw_rd(double __x, double __y, double __z) {
    const double ERRTOL = 0.0015, C1 = 3.0/14.0, C2 = 1.0/6.0, C3 = 9.0/22.0,
                 C4 = 3.0/26.0, C5 = 0.25 * C3, C6 = 1.5 * C4;
    double __xt = __x, __yt = __y, __zt = __z, __sum = 0.0, __fac = 1.0, __ave, __dx, __dy, __dz;
    do {
        double __sx = ::std::sqrt(__xt), __sy = ::std::sqrt(__yt), __sz = ::std::sqrt(__zt);
        double __alamb = __sx * (__sy + __sz) + __sy * __sz;
        __sum += __fac / (__sz * (__zt + __alamb));
        __fac *= 0.25;
        __xt = 0.25 * (__xt + __alamb); __yt = 0.25 * (__yt + __alamb); __zt = 0.25 * (__zt + __alamb);
        __ave = 0.2 * (__xt + __yt + 3.0 * __zt);
        __dx = (__ave - __xt) / __ave; __dy = (__ave - __yt) / __ave; __dz = (__ave - __zt) / __ave;
    } while (__ccw_ell_max3(::std::fabs(__dx), ::std::fabs(__dy), ::std::fabs(__dz)) > ERRTOL);
    double __ea = __dx * __dy, __eb = __dz * __dz, __ec = __ea - __eb, __ed = __ea - 6.0 * __eb, __ee = __ed + __ec + __ec;
    return 3.0 * __sum + __fac * (1.0 + __ed * (-C1 + C5 * __ed - C6 * __dz * __ee)
        + __dz * (C2 * __ee + __dz * (-C3 * __ec + __dz * C4 * __ea))) / (__ave * ::std::sqrt(__ave));
}

inline double __ccw_rj(double __x, double __y, double __z, double __p) {
    const double ERRTOL = 0.0015, C1 = 3.0/14.0, C2 = 1.0/3.0, C3 = 3.0/22.0,
                 C4 = 3.0/26.0, C5 = 0.75 * C3, C6 = 1.5 * C4, C7 = 0.5 * C2, C8 = C3 + C3;
    double __xt = __x, __yt = __y, __zt = __z, __pt = __p, __sum = 0.0, __fac = 1.0, __ave, __dx, __dy, __dz, __dp;
    do {
        double __sx = ::std::sqrt(__xt), __sy = ::std::sqrt(__yt), __sz = ::std::sqrt(__zt);
        double __alamb = __sx * (__sy + __sz) + __sy * __sz;
        double __alpha = __pt * (__sx + __sy + __sz) + __sx * __sy * __sz; __alpha *= __alpha;
        double __beta = __pt * (__pt + __alamb) * (__pt + __alamb);
        __sum += __fac * __ccw_rc(__alpha, __beta);
        __fac *= 0.25;
        __xt = 0.25 * (__xt + __alamb); __yt = 0.25 * (__yt + __alamb);
        __zt = 0.25 * (__zt + __alamb); __pt = 0.25 * (__pt + __alamb);
        __ave = 0.2 * (__xt + __yt + __zt + __pt + __pt);
        __dx = (__ave - __xt) / __ave; __dy = (__ave - __yt) / __ave;
        __dz = (__ave - __zt) / __ave; __dp = (__ave - __pt) / __ave;
    } while (__ccw_ell_max3(::std::fabs(__dx), ::std::fabs(__dy),
             __ccw_ell_max3(::std::fabs(__dz), ::std::fabs(__dp), 0.0)) > ERRTOL);
    double __ea = __dx * (__dy + __dz) + __dy * __dz;
    double __eb = __dx * __dy * __dz;
    double __ec = __dp * __dp;
    double __ed = __ea - 3.0 * __ec;
    double __ee = __eb + 2.0 * __dp * (__ea - __ec);
    return 3.0 * __sum + __fac * (1.0 + __ed * (-C1 + C5 * __ed - C6 * __ee)
        + __eb * (C7 + __dp * (-C8 + __dp * C4)) + __dp * __ea * (C2 - __dp * C3)
        - C2 * __dp * __ec) / (__ave * ::std::sqrt(__ave));
}

inline double comp_ellint_1(double __k) {
    double __m = 1.0 - __k * __k;
    return __ccw_rf(0.0, __m, 1.0);
}
inline double comp_ellint_2(double __k) {
    double __m = 1.0 - __k * __k;
    if (__m <= 0.0) return 1.0;
    return __ccw_rf(0.0, __m, 1.0) - (__k * __k / 3.0) * __ccw_rd(0.0, __m, 1.0);
}
inline double comp_ellint_3(double __k, double __nu) {
    double __m = 1.0 - __k * __k;
    double __r = __ccw_rf(0.0, __m, 1.0);
    if (__nu != 0.0) __r += (__nu / 3.0) * __ccw_rj(0.0, __m, 1.0, 1.0 - __nu);
    return __r;
}
inline double ellint_1(double __k, double __phi) {
    double __s = ::std::sin(__phi), __c = ::std::cos(__phi);
    double __q = 1.0 - __k * __k * __s * __s;
    return __s * __ccw_rf(__c * __c, __q, 1.0);
}
inline double ellint_2(double __k, double __phi) {
    double __s = ::std::sin(__phi), __c = ::std::cos(__phi);
    double __q = 1.0 - __k * __k * __s * __s;
    return __s * __ccw_rf(__c * __c, __q, 1.0)
         - (__k * __k / 3.0) * __s * __s * __s * __ccw_rd(__c * __c, __q, 1.0);
}
inline double ellint_3(double __k, double __nu, double __phi) {
    double __s = ::std::sin(__phi), __c = ::std::cos(__phi);
    double __q = 1.0 - __k * __k * __s * __s;
    double __r = __s * __ccw_rf(__c * __c, __q, 1.0);
    if (__nu != 0.0)
        __r += (__nu / 3.0) * __s * __s * __s * __ccw_rj(__c * __c, __q, 1.0, 1.0 - __nu * __s * __s);
    return __r;
}

inline double __ccw_digamma_int(unsigned __n) {   // psi(n) = -gamma + sum_{j=1}^{n-1} 1/j
    double __r = -0.5772156649015328606;
    for (unsigned __j = 1u; __j < __n; ++__j) __r += 1.0 / (double)__j;
    return __r;
}

inline double cyl_bessel_j(double __nu, double __x) {
    if (__x == 0.0) return __nu == 0.0 ? 1.0 : 0.0;
    double __xh = 0.5 * __x;
    double __term = ::std::pow(__xh, __nu) / ::std::exp(::std::lgamma(__nu + 1.0));
    double __sum = __term;
    for (int __m = 1; __m < 400; ++__m) {
        __term *= -(__xh * __xh) / ((double)__m * (__nu + (double)__m));
        __sum += __term;
        if (::std::fabs(__term) < 1e-18 * ::std::fabs(__sum)) break;
    }
    return __sum;
}
inline double cyl_bessel_i(double __nu, double __x) {
    if (__x == 0.0) return __nu == 0.0 ? 1.0 : 0.0;
    double __xh = 0.5 * __x;
    double __term = ::std::pow(__xh, __nu) / ::std::exp(::std::lgamma(__nu + 1.0));
    double __sum = __term;
    for (int __m = 1; __m < 400; ++__m) {
        __term *= (__xh * __xh) / ((double)__m * (__nu + (double)__m));
        __sum += __term;
        if (::std::fabs(__term) < 1e-18 * ::std::fabs(__sum)) break;
    }
    return __sum;
}

inline double __ccw_neumann_int(unsigned __n, double __x) {
    double __xh = 0.5 * __x;
    double __lx = ::std::log(__xh);
    double __jn = cyl_bessel_j((double)__n, __x);
    const double __invpi = 1.0 / 3.14159265358979323846;
    double __y = 2.0 * __invpi * __lx * __jn;
    if (__n > 0u) {
        double __s1 = 0.0;
        double __xhn = ::std::pow(__xh, -(double)__n);   // (x/2)^{-n}
        double __fk = 1.0;                                // k!
        double __fnk1 = ::std::exp(::std::lgamma((double)__n)); // (n-1)!
        double __xh2k = 1.0;                              // (x/2)^{2k}
        for (unsigned __k = 0u; __k < __n; ++__k) {
            if (__k > 0u) { __fk *= (double)__k; __fnk1 /= (double)(__n - __k); __xh2k *= __xh * __xh; }
            __s1 += (__fnk1 / __fk) * __xhn * __xh2k;
        }
        __y -= __invpi * __s1;
    }
    double __s2 = 0.0;
    double __coef = ::std::pow(__xh, (double)__n) / ::std::exp(::std::lgamma((double)__n + 1.0)); // (x/2)^n / n!
    double __sign = 1.0;
    for (int __k = 0; __k < 400; ++__k) {
        double __psi = __ccw_digamma_int((unsigned)__k + 1u) + __ccw_digamma_int(__n + (unsigned)__k + 1u);
        double __t = __sign * __psi * __coef;
        __s2 += __t;
        if (__k > 2 && ::std::fabs(__t) < 1e-18 * ::std::fabs(__s2)) break;
        __coef *= (__xh * __xh) / ((double)(__k + 1) * (double)(__n + (unsigned)__k + 1u));
        __sign = -__sign;
    }
    __y -= __invpi * __s2;
    return __y;
}
inline double cyl_neumann(double __nu, double __x) {
    double __rn = ::std::floor(__nu + 0.5);
    if (__nu >= 0.0 && ::std::fabs(__nu - __rn) < 1e-9)
        return __ccw_neumann_int((unsigned)(__rn + 0.5), __x);
    const double __pi = 3.14159265358979323846;
    return (cyl_bessel_j(__nu, __x) * ::std::cos(__nu * __pi) - cyl_bessel_j(-__nu, __x)) / ::std::sin(__nu * __pi);
}

inline double __ccw_bessel_k_int(unsigned __n, double __x) {
    double __xh = 0.5 * __x;
    double __lx = ::std::log(__xh);
    double __iv = cyl_bessel_i((double)__n, __x);
    double __k = 0.0;
    if (__n > 0u) {
        double __s1 = 0.0;
        double __xhn = ::std::pow(__xh, -(double)__n);
        double __fk = 1.0, __fnk1 = ::std::exp(::std::lgamma((double)__n)), __xh2k = 1.0, __sign = 1.0;
        for (unsigned __i = 0u; __i < __n; ++__i) {
            if (__i > 0u) { __fk *= (double)__i; __fnk1 /= (double)(__n - __i); __xh2k *= __xh * __xh; __sign = -__sign; }
            __s1 += __sign * (__fnk1 / __fk) * __xhn * __xh2k;
        }
        __k += 0.5 * __s1;
    }
    __k += ((__n & 1u) ? 1.0 : -1.0) * __lx * __iv;
    double __s2 = 0.0;
    double __coef = ::std::pow(__xh, (double)__n) / ::std::exp(::std::lgamma((double)__n + 1.0));
    for (int __i = 0; __i < 400; ++__i) {
        double __psi = __ccw_digamma_int((unsigned)__i + 1u) + __ccw_digamma_int(__n + (unsigned)__i + 1u);
        double __t = __psi * __coef;
        __s2 += __t;
        if (__i > 2 && ::std::fabs(__t) < 1e-18 * ::std::fabs(__s2)) break;
        __coef *= (__xh * __xh) / ((double)(__i + 1) * (double)(__n + (unsigned)__i + 1u));
    }
    __k += ((__n & 1u) ? -0.5 : 0.5) * __s2;
    return __k;
}
inline double cyl_bessel_k(double __nu, double __x) {
    double __rn = ::std::floor(__nu + 0.5);
    if (__nu >= 0.0 && ::std::fabs(__nu - __rn) < 1e-9)
        return __ccw_bessel_k_int((unsigned)(__rn + 0.5), __x);
    const double __pi = 3.14159265358979323846;
    return 0.5 * __pi * (cyl_bessel_i(-__nu, __x) - cyl_bessel_i(__nu, __x)) / ::std::sin(__nu * __pi);
}

inline double sph_bessel(unsigned __n, double __x) {
    if (__x == 0.0) return __n == 0u ? 1.0 : 0.0;
    double __j0 = ::std::sin(__x) / __x;
    if (__n == 0u) return __j0;
    double __j1 = __j0 / __x - ::std::cos(__x) / __x;
    if (__n == 1u) return __j1;
    double __jm = __j0, __jc = __j1, __jp;
    for (unsigned __l = 1u; __l < __n; ++__l) {
        __jp = (2.0 * (double)__l + 1.0) / __x * __jc - __jm;
        __jm = __jc; __jc = __jp;
    }
    return __jc;
}
inline double sph_neumann(unsigned __n, double __x) {
    double __n0 = -::std::cos(__x) / __x;
    if (__n == 0u) return __n0;
    double __n1 = __n0 / __x - ::std::sin(__x) / __x;
    if (__n == 1u) return __n1;
    double __nm = __n0, __nc = __n1, __np;
    for (unsigned __l = 1u; __l < __n; ++__l) {
        __np = (2.0 * (double)__l + 1.0) / __x * __nc - __nm;
        __nm = __nc; __nc = __np;
    }
    return __nc;
}

inline double expint(double __x) {
    double __sum = 0.0, __t = 1.0;
    for (int __k = 1; __k < 500; ++__k) {
        __t *= __x / (double)__k;
        double __add = __t / (double)__k;
        __sum += __add;
        if (__k > 2 && ::std::fabs(__add) < 1e-18 * ::std::fabs(__sum)) break;
    }
    return 0.5772156649015328606 + ::std::log(::std::fabs(__x)) + __sum;
}

inline double riemann_zeta(double __s) {
    if (__s == 1.0) return ::std::exp(1000.0);          // pole -> +inf
    const int __n = 30;
    double __d[31];
    double __term = 1.0 / (double)__n, __acc = __term;
    __d[0] = (double)__n * __acc;
    for (int __j = 1; __j <= __n; ++__j) {
        __term *= 4.0 * (double)(__n + __j - 1) * (double)(__n - __j + 1) / ((double)(2 * __j) * (double)(2 * __j - 1));
        __acc += __term;
        __d[__j] = (double)__n * __acc;
    }
    double __sum = 0.0, __sign = 1.0;
    for (int __k = 0; __k < __n; ++__k) {
        __sum += __sign * (__d[__k] - __d[__n]) / ::std::pow((double)(__k + 1), __s);
        __sign = -__sign;
    }
    return -__sum / (__d[__n] * (1.0 - ::std::pow(2.0, 1.0 - __s)));
}

inline double sph_legendre(unsigned __l, unsigned __m, double __theta) {
    if (__m > __l) return 0.0;
    const double __pi = 3.14159265358979323846;
    double __x = ::std::cos(__theta);
    double __plm = assoc_legendre(__l, __m, __x);
    double __lnnorm = 0.5 * (::std::log((2.0 * (double)__l + 1.0) / (4.0 * __pi))
                     + ::std::lgamma((double)(__l - __m) + 1.0) - ::std::lgamma((double)(__l + __m) + 1.0));
    double __cs = (__m & 1u) ? -1.0 : 1.0;
    return __cs * ::std::exp(__lnnorm) * __plm;
}

#ifndef _CCW_SF_NATIVE_HERMITE
inline float  hermitef(unsigned __n, float __x)        { return (float)hermite(__n, (double)__x); }
inline long double hermitel(unsigned __n, long double __x) { return (long double)hermite(__n, (double)__x); }
#endif // !_CCW_SF_NATIVE_HERMITE
inline float  laguerref(unsigned __n, float __x)       { return (float)laguerre(__n, (double)__x); }
inline long double laguerrel(unsigned __n, long double __x){ return (long double)laguerre(__n, (double)__x); }
inline float  legendref(unsigned __l, float __x)       { return (float)legendre(__l, (double)__x); }
inline long double legendrel(unsigned __l, long double __x){ return (long double)legendre(__l, (double)__x); }
inline float  assoc_laguerref(unsigned __n, unsigned __m, float __x)        { return (float)assoc_laguerre(__n, __m, (double)__x); }
inline long double assoc_laguerrel(unsigned __n, unsigned __m, long double __x) { return (long double)assoc_laguerre(__n, __m, (double)__x); }
inline float  assoc_legendref(unsigned __l, unsigned __m, float __x)        { return (float)assoc_legendre(__l, __m, (double)__x); }
inline long double assoc_legendrel(unsigned __l, unsigned __m, long double __x) { return (long double)assoc_legendre(__l, __m, (double)__x); }
inline float  betaf(float __a, float __b)              { return (float)beta((double)__a, (double)__b); }
inline long double betal(long double __a, long double __b) { return (long double)beta((double)__a, (double)__b); }

inline float  comp_ellint_1f(float __k)             { return (float)comp_ellint_1((double)__k); }
inline long double comp_ellint_1l(long double __k)  { return (long double)comp_ellint_1((double)__k); }
inline float  comp_ellint_2f(float __k)             { return (float)comp_ellint_2((double)__k); }
inline long double comp_ellint_2l(long double __k)  { return (long double)comp_ellint_2((double)__k); }
inline float  comp_ellint_3f(float __k, float __nu)             { return (float)comp_ellint_3((double)__k, (double)__nu); }
inline long double comp_ellint_3l(long double __k, long double __nu) { return (long double)comp_ellint_3((double)__k, (double)__nu); }
inline float  ellint_1f(float __k, float __phi)                { return (float)ellint_1((double)__k, (double)__phi); }
inline long double ellint_1l(long double __k, long double __phi) { return (long double)ellint_1((double)__k, (double)__phi); }
inline float  ellint_2f(float __k, float __phi)                { return (float)ellint_2((double)__k, (double)__phi); }
inline long double ellint_2l(long double __k, long double __phi) { return (long double)ellint_2((double)__k, (double)__phi); }
inline float  ellint_3f(float __k, float __nu, float __phi)                    { return (float)ellint_3((double)__k, (double)__nu, (double)__phi); }
inline long double ellint_3l(long double __k, long double __nu, long double __phi) { return (long double)ellint_3((double)__k, (double)__nu, (double)__phi); }

inline float  cyl_bessel_if(float __nu, float __x)              { return (float)cyl_bessel_i((double)__nu, (double)__x); }
inline long double cyl_bessel_il(long double __nu, long double __x) { return (long double)cyl_bessel_i((double)__nu, (double)__x); }
inline float  cyl_bessel_jf(float __nu, float __x)              { return (float)cyl_bessel_j((double)__nu, (double)__x); }
inline long double cyl_bessel_jl(long double __nu, long double __x) { return (long double)cyl_bessel_j((double)__nu, (double)__x); }
inline float  cyl_bessel_kf(float __nu, float __x)              { return (float)cyl_bessel_k((double)__nu, (double)__x); }
inline long double cyl_bessel_kl(long double __nu, long double __x) { return (long double)cyl_bessel_k((double)__nu, (double)__x); }
inline float  cyl_neumannf(float __nu, float __x)              { return (float)cyl_neumann((double)__nu, (double)__x); }
inline long double cyl_neumannl(long double __nu, long double __x) { return (long double)cyl_neumann((double)__nu, (double)__x); }
inline float  sph_besself(unsigned __n, float __x)            { return (float)sph_bessel(__n, (double)__x); }
inline long double sph_bessell(unsigned __n, long double __x) { return (long double)sph_bessel(__n, (double)__x); }
inline float  sph_neumannf(unsigned __n, float __x)            { return (float)sph_neumann(__n, (double)__x); }
inline long double sph_neumannl(unsigned __n, long double __x) { return (long double)sph_neumann(__n, (double)__x); }

inline float  expintf(float __x)             { return (float)expint((double)__x); }
inline long double expintl(long double __x)  { return (long double)expint((double)__x); }
inline float  riemann_zetaf(float __s)             { return (float)riemann_zeta((double)__s); }
inline long double riemann_zetal(long double __s)  { return (long double)riemann_zeta((double)__s); }
inline float  sph_legendref(unsigned __l, unsigned __m, float __theta)            { return (float)sph_legendre(__l, __m, (double)__theta); }
inline long double sph_legendrel(unsigned __l, unsigned __m, long double __theta) { return (long double)sph_legendre(__l, __m, (double)__theta); }

} // namespace _CCW_STD

#include "math_special_ovl.hpp"

#endif  /* _CCW_TARGET_CXX >= 2017 */
#endif  /* _CCW_DETAIL_MATH_SPECIAL_HPP */
