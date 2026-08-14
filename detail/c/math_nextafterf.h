/**
 *  @file   detail/math_nextafterf.h
 *  @brief  A correct float nextafter (language-neutral helper).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_MATH_NEXTAFTERF_H___
#define _CCW_DETAIL_MATH_NEXTAFTERF_H___
#include <ccwrap_common.h>

static _ccw_forceinline float __ccw_nextafterf(float __x, float __y) {
    union { float __f; _ccw_uint32 __u; } __v, __vy;
    _ccw_uint32 __ux;
    __v.__f = __x;
    __vy.__f = __y;
    __ux = __v.__u;
    if ((__ux & 0x7FFFFFFFu) > 0x7F800000u) return __x;          /* x is NaN */
    if ((__vy.__u & 0x7FFFFFFFu) > 0x7F800000u) return __y;      /* y is NaN */
    if (__x == __y) return __y;                                  /* incl. +0 == -0 -> y */
    if ((__ux & 0x7FFFFFFFu) == 0u) {                            /* x == 0: smallest subnormal toward y */
        __v.__u = 1u | (__vy.__u & 0x80000000u);
        return __v.__f;
    }
    if ((__x < __y) == (__x > 0.0f)) __v.__u = __ux + 1u;        /* move away from zero */
    else                             __v.__u = __ux - 1u;        /* move toward zero */
    return __v.__f;
}

#endif /* _CCW_DETAIL_MATH_NEXTAFTERF_H___ */
