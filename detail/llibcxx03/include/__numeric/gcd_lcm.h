//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_GCD_LCM_H
#define _CCW_LIBCPP___NUMERIC_GCD_LCM_H
#include "../__config"
#include "../__numeric/arith_constraint.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_not_bool<_Tp>::type gcd(_Tp __m, _Tp __n) {
    if (__m < 0) __m = -__m;
    if (__n < 0) __n = -__n;
    while (__n != 0) { _Tp __t = __m % __n; __m = __n; __n = __t; }
    return __m;
}
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_not_bool<_Tp>::type lcm(_Tp __m, _Tp __n) {
    if (__m == 0 || __n == 0) return 0;
    _Tp __g = gcd(__m, __n);
    _Tp __r = (__m / __g) * __n;
    return __r < 0 ? -__r : __r;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
