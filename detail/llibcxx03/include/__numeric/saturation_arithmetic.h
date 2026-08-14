/*
 * ccwrap: libc++ layout. classic C++03 body.
 *
 * C++26 saturation arithmetic: add_sat / sub_sat / mul_sat / div_sat / saturate_cast.
 * Open Watcom has no __builtin_*_overflow, no `if constexpr` and no SFINAE, so the
 * signed/unsigned split is decided at run time on numeric_limits<T>::is_signed (both
 * arms compile for every integer T) and overflow is detected by the classic
 * pre-multiply/limit comparisons (CERT INT30/INT32) rather than by wide arithmetic --
 * which matters because _Tp may be 64-bit and no wider type is available.
 *
 * Subset note: the standard constrains these to the signed/unsigned integer types
 * (excluding bool and the character types). The bool exclusion is emulated with
 * __ccw_not_bool in the return type (see <__numeric/arith_constraint.h>); the rest of
 * the constraint is not expressible without SFINAE/concepts, so passing a
 * character type is still the caller's responsibility.
 */
#ifndef _CCW_LIBCPP___NUMERIC_SATURATION_ARITHMETIC_H
#define _CCW_LIBCPP___NUMERIC_SATURATION_ARITHMETIC_H
#include "../__config"
#include "../__numeric/arith_constraint.h"
#include "../__utility/cmp.h"
#include <limits>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_not_bool<_Tp>::type add_sat(_Tp __x, _Tp __y) _CCW_LIBCPP_NOEXCEPT {
    const _Tp __mx = numeric_limits<_Tp>::max();
    const _Tp __mn = numeric_limits<_Tp>::min();
    if (!numeric_limits<_Tp>::is_signed) {
        _Tp __s = (_Tp)(__x + __y);         // modular
        return __s < __x ? __mx : __s;      // unsigned overflow iff it wrapped below x
    }
    if (__y > 0) {
        if (__x > (_Tp)(__mx - __y)) return __mx;
    } else {
        if (__x < (_Tp)(__mn - __y)) return __mn;
    }
    return (_Tp)(__x + __y);
}

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_not_bool<_Tp>::type sub_sat(_Tp __x, _Tp __y) _CCW_LIBCPP_NOEXCEPT {
    const _Tp __mx = numeric_limits<_Tp>::max();
    const _Tp __mn = numeric_limits<_Tp>::min();
    if (!numeric_limits<_Tp>::is_signed) {
        return __x < __y ? __mn : (_Tp)(__x - __y);   // __mn == 0
    }
    if (__y < 0) {
        if (__x > (_Tp)(__mx + __y)) return __mx;
    } else {
        if (__x < (_Tp)(__mn + __y)) return __mn;
    }
    return (_Tp)(__x - __y);
}

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_not_bool<_Tp>::type mul_sat(_Tp __x, _Tp __y) _CCW_LIBCPP_NOEXCEPT {
    const _Tp __mx = numeric_limits<_Tp>::max();
    const _Tp __mn = numeric_limits<_Tp>::min();
    if (__x == 0 || __y == 0) return (_Tp)0;
    if (!numeric_limits<_Tp>::is_signed) {
        return (__x > (_Tp)(__mx / __y)) ? __mx : (_Tp)(__x * __y);
    }
    if (__x > 0) {
        if (__y > 0) { if (__x > (_Tp)(__mx / __y)) return __mx; }
        else         { if (__y < (_Tp)(__mn / __x)) return __mn; }
    } else {
        if (__y > 0) { if (__x < (_Tp)(__mn / __y)) return __mn; }
        else         { if (__y < (_Tp)(__mx / __x)) return __mx; }
    }
    return (_Tp)(__x * __y);
}

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_not_bool<_Tp>::type div_sat(_Tp __x, _Tp __y) _CCW_LIBCPP_NOEXCEPT {
    if (numeric_limits<_Tp>::is_signed &&
        __x == numeric_limits<_Tp>::min() && __y == (_Tp)(-1))
        return numeric_limits<_Tp>::max();
    return (_Tp)(__x / __y);
}

template <class _Rp, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_not_bool<_Rp>::type saturate_cast(_Tp __x) _CCW_LIBCPP_NOEXCEPT {
    if (cmp_less(__x, numeric_limits<_Rp>::min()))
        return numeric_limits<_Rp>::min();
    if (cmp_greater(__x, numeric_limits<_Rp>::max()))
        return numeric_limits<_Rp>::max();
    return (_Rp)__x;
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___NUMERIC_SATURATION_ARITHMETIC_H
