//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//  C++20 safe integer comparisons: cmp_equal / cmp_less / ... and in_range. Each
//  branch compares like-signed values so a negative signed operand never wraps.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___UTILITY_CMP_H
#define _CCW_LIBCPP___UTILITY_CMP_H
#include "../__config"
#include "../__type_traits/is_signed.h"
#include "../__type_traits/make_unsigned.h"
#include <limits>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <bool _TS, bool _US> struct __ccw_cmp_less;
template <> struct __ccw_cmp_less<true, true>   { template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI static bool __f(_Tp __t, _Up __u) { return __t < __u; } };
template <> struct __ccw_cmp_less<false, false> { template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI static bool __f(_Tp __t, _Up __u) { return __t < __u; } };
template <> struct __ccw_cmp_less<true, false>  { template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI static bool __f(_Tp __t, _Up __u) { return __t < 0 ? true  : static_cast<typename make_unsigned<_Tp>::type>(__t) < __u; } };
template <> struct __ccw_cmp_less<false, true>  { template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI static bool __f(_Tp __t, _Up __u) { return __u < 0 ? false : __t < static_cast<typename make_unsigned<_Up>::type>(__u); } };

template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool cmp_less(_Tp __t, _Up __u) {
    return __ccw_cmp_less<is_signed<_Tp>::value, is_signed<_Up>::value>::__f(__t, __u);
}
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool cmp_greater(_Tp __t, _Up __u)       { return cmp_less(__u, __t); }
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool cmp_less_equal(_Tp __t, _Up __u)    { return !cmp_less(__u, __t); }
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool cmp_greater_equal(_Tp __t, _Up __u) { return !cmp_less(__t, __u); }
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool cmp_equal(_Tp __t, _Up __u)         { return !cmp_less(__t, __u) && !cmp_less(__u, __t); }
template <class _Tp, class _Up> _CCW_LIBCPP_HIDE_FROM_ABI bool cmp_not_equal(_Tp __t, _Up __u)     { return !cmp_equal(__t, __u); }

template <class _Rp, class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI bool in_range(_Tp __t) {
    return cmp_greater_equal(__t, _CCW_STD::numeric_limits<_Rp>::min())
        && cmp_less_equal(__t, _CCW_STD::numeric_limits<_Rp>::max());
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___UTILITY_CMP_H
