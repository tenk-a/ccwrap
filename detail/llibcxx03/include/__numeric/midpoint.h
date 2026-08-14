//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//
// C++20 midpoint. The standard requires no overflow for the integer form and that
// the result be rounded *towards a*; the naive `a + (b - a) / 2` satisfies neither
// (`b - a` overflows for far-apart signed values, and wraps for unsigned when a > b).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_MIDPOINT_H
#define _CCW_LIBCPP___NUMERIC_MIDPOINT_H
#include "../__config"
#include "../__numeric/arith_constraint.h"
#include <limits>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp __midpoint_int(_Tp __a, _Tp __b) {
    unsigned long long __ua, __ub;
    if (numeric_limits<_Tp>::is_signed) {
        __ua = (unsigned long long)(long long)__a;
        __ub = (unsigned long long)(long long)__b;
    } else {
        __ua = (unsigned long long)__a;
        __ub = (unsigned long long)__b;
    }
    if (__b >= __a) {
        _Tp __half = (_Tp)((__ub - __ua) / 2);
        return (_Tp)(__a + __half);
    }
    _Tp __half = (_Tp)((__ua - __ub) / 2);
    return (_Tp)(__a - __half);
}

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp __midpoint_fp(_Tp __a, _Tp __b) {
    const _Tp __lo = numeric_limits<_Tp>::min() * 2;
    const _Tp __hi = numeric_limits<_Tp>::max() / 2;
    const _Tp __aa = __a < 0 ? (_Tp)-__a : __a;
    const _Tp __bb = __b < 0 ? (_Tp)-__b : __b;
    if (__aa <= __hi && __bb <= __hi) return (_Tp)((__a + __b) / 2);  // a + b cannot overflow
    if (__aa < __lo) return (_Tp)(__a + __b / 2);                     // not safe to halve a
    if (__bb < __lo) return (_Tp)(__a / 2 + __b);                     // not safe to halve b
    return (_Tp)(__a / 2 + __b / 2);
}

template <class _Tp>
struct __midpoint_impl {
    _CCW_LIBCPP_HIDE_FROM_ABI static _Tp __do(_Tp __a, _Tp __b) {
        if (numeric_limits<_Tp>::is_integer) return __midpoint_int(__a, __b);
        return __midpoint_fp(__a, __b);
    }
};

template <class _Tp>
struct __midpoint_impl<_Tp*> {
    _CCW_LIBCPP_HIDE_FROM_ABI static _Tp* __do(_Tp* __a, _Tp* __b) { return __a + (__b - __a) / 2; }
};

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_not_bool<_Tp>::type midpoint(_Tp __a, _Tp __b) {
    return __midpoint_impl<_Tp>::__do(__a, __b);
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
