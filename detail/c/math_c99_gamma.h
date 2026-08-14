/**
 *  @file   detail/math_c99_gamma.h
 *  @brief  Implementation of the C99 gamma / error functions: lgamma / tgamma / erf / erfc.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_MATH_C99_GAMMA_H___
#define _CCW_DETAIL_MATH_C99_GAMMA_H___

#include <ccwrap_common.h>

#define _CCW_GAMMA_PI 3.14159265358979323846

static _ccw_forceinline double __ccw_lgamma(double __x) {
    static const double __g = 7.0;
    static const double __c[9] = {
        0.99999999999980993, 676.5203681218851, -1259.1392167224028,
        771.32342877765313, -176.61502916214059, 12.507343278686905,
        -0.13857109526572012, 9.9843695780195716e-6, 1.5056327351493116e-7
    };
    double __a, __t;
    int __i;
    if (__x < 0.5) {
        return log(_CCW_GAMMA_PI / fabs(sin(_CCW_GAMMA_PI * __x))) - __ccw_lgamma(1.0 - __x);
    }
    __x -= 1.0;
    __a = __c[0];
    __t = __x + __g + 0.5;
    for (__i = 1; __i < 9; ++__i) __a += __c[__i] / (__x + (double)__i);
    return 0.5 * log(2.0 * _CCW_GAMMA_PI) + (__x + 0.5) * log(__t) - __t + log(__a);
}

static _ccw_forceinline double __ccw_tgamma(double __x) {
    static const double __g = 7.0;
    static const double __c[9] = {
        0.99999999999980993, 676.5203681218851, -1259.1392167224028,
        771.32342877765313, -176.61502916214059, 12.507343278686905,
        -0.13857109526572012, 9.9843695780195716e-6, 1.5056327351493116e-7
    };
    double __a, __t;
    int __i;
    if (__x < 0.5) {
        return _CCW_GAMMA_PI / (sin(_CCW_GAMMA_PI * __x) * __ccw_tgamma(1.0 - __x));
    }
    __x -= 1.0;
    __a = __c[0];
    __t = __x + __g + 0.5;
    for (__i = 1; __i < 9; ++__i) __a += __c[__i] / (__x + (double)__i);
    return sqrt(2.0 * _CCW_GAMMA_PI) * pow(__t, __x + 0.5) * exp(-__t) * __a;
}

static _ccw_forceinline double __ccw_erf(double __x);

static _ccw_forceinline double __ccw_erfc_nr(double __x) {
    double __z = fabs(__x);
    double __t = 1.0 / (1.0 + 0.5 * __z);
    double __ans = __t * exp(-__z * __z - 1.26551223 + __t * (1.00002368 + __t * (0.37409196
        + __t * (0.09678418 + __t * (-0.18628806 + __t * (0.27886807 + __t * (-1.13520398
        + __t * (1.48851587 + __t * (-0.82215223 + __t * 0.17087277)))))))));
    return __x >= 0.0 ? __ans : 2.0 - __ans;
}

static _ccw_forceinline double __ccw_erfc(double __x) {
    if (__x > 2.0 || __x < -2.0) return __ccw_erfc_nr(__x);
    return 1.0 - __ccw_erf(__x);
}

static _ccw_forceinline double __ccw_erf(double __x) {
    if (__x > 2.0)  return 1.0 - __ccw_erfc_nr(__x);
    if (__x < -2.0) return __ccw_erfc_nr(-__x) - 1.0;
    {
        double __x2 = __x * __x;
        double __p = __x, __sum = __x;   /* n=0 term = x */
        int __n;
        for (__n = 1; __n < 300; ++__n) {
            double __add;
            __p *= -__x2 / (double)__n;             /* (-1)^n x^(2n+1)/n! */
            __add = __p / (double)(2 * __n + 1);
            __sum += __add;
            if (fabs(__add) < 1e-18 * (fabs(__sum) + 1e-300)) break;
        }
        return 1.12837916709551257390 * __sum;      /* 2/sqrt(pi) */
    }
}

static _ccw_forceinline double lgamma(double __x) { return __ccw_lgamma(__x); }
static _ccw_forceinline double tgamma(double __x) { return __ccw_tgamma(__x); }
static _ccw_forceinline double erfc(double __x)   { return __ccw_erfc(__x); }
static _ccw_forceinline double erf(double __x)    { return __ccw_erf(__x); }

#undef _CCW_GAMMA_PI

#endif /* _CCW_DETAIL_MATH_C99_GAMMA_H___ */
