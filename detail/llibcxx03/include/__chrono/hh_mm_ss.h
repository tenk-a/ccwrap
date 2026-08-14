//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom. [time.hms] hh_mm_ss and the 12/24-hour helpers.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___CHRONO_HH_MM_SS_H
#define _CCW_LIBCPP___CHRONO_HH_MM_SS_H
#include "../__config"
#include "../__type_traits/common_type.h"
#include <ratio>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace chrono {

template <unsigned _Np> struct __hms_pow10 {
    static const long long value = 10 * __hms_pow10<_Np - 1>::value;
};
template <> struct __hms_pow10<0> { static const long long value = 1; };

template <long long _Vp> struct __hms_val { static const long long value = _Vp; };

template <class _Np, class _Dp, int _Wp> struct __hms_go_on {
    enum { __two    = (_Np::value >= 2) };
    enum { __left   = (_Dp::value != 0) };
    enum { __room   = (_Wp < 19) };
    enum { value    = __two && __left && __room };
};

template <bool _GoOn, class _Np, class _Dp, int _Wp> struct __hms_width_impl;

template <class _Np, class _Dp, int _Wp>
struct __hms_width_impl<false, _Np, _Dp, _Wp> { enum { value = 0 }; };

template <class _Np, class _Dp, int _Wp>
struct __hms_width_impl<true, _Np, _Dp, _Wp> {
    typedef __hms_val<_Dp::value % _Np::value * 10> __next;
    enum { value = 1 + __hms_width_impl<(bool)__hms_go_on<_Np, __next, _Wp + 1>::value,
                                        _Np, __next, _Wp + 1>::value };
};

template <class _Period> struct __hms_width {
    typedef __hms_val<_Period::den + 0> __den;
    typedef __hms_val<10>               __ten;
    enum { __raw = __hms_width_impl<(bool)__hms_go_on<__den, __ten, 0>::value, __den, __ten, 0>::value };
    enum { value = __raw < 19 ? (int)__raw : 6 };
};

template <class _Duration>
class hh_mm_ss {
    typedef typename common_type<_Duration, chrono::seconds>::type __ccw_common;

public:
    enum { __ccw_fw = __hms_width<typename __ccw_common::period>::value };
    static const unsigned fractional_width = (unsigned)__ccw_fw;
    typedef duration<typename __ccw_common::rep,
                     ratio<1, __hms_pow10<(unsigned)__ccw_fw>::value + 0> > precision;

    _CCW_LIBCPP_HIDE_FROM_ABI hh_mm_ss() { __split(_Duration()); }
    _CCW_LIBCPP_HIDE_FROM_ABI explicit hh_mm_ss(_Duration __d) { __split(__d); }

    _CCW_LIBCPP_HIDE_FROM_ABI bool            is_negative() const { return __is_neg_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::hours   hours() const       { return __h_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::minutes minutes() const     { return __m_; }
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::seconds seconds() const     { return __s_; }
    _CCW_LIBCPP_HIDE_FROM_ABI precision       subseconds() const  { return __f_; }

    _CCW_LIBCPP_HIDE_FROM_ABI precision to_duration() const {
        precision __r = duration_cast<precision>(__h_) + duration_cast<precision>(__m_);
        __r = __r + duration_cast<precision>(__s_) + __f_;
        return __is_neg_ ? -__r : __r;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI operator precision() const { return to_duration(); }

private:
    _CCW_LIBCPP_HIDE_FROM_ABI void __split(_Duration __d) {
        __is_neg_ = __d < _Duration(0);
        __ccw_common __rest = duration_cast<__ccw_common>(chrono::abs(__d));
        __h_ = duration_cast<chrono::hours>(__rest);
        __rest = __rest - duration_cast<__ccw_common>(__h_);
        __m_ = duration_cast<chrono::minutes>(__rest);
        __rest = __rest - duration_cast<__ccw_common>(__m_);
        __s_ = duration_cast<chrono::seconds>(__rest);
        __rest = __rest - duration_cast<__ccw_common>(__s_);
        __f_ = duration_cast<precision>(__rest);
    }

    bool            __is_neg_;
    chrono::hours   __h_;
    chrono::minutes __m_;
    chrono::seconds __s_;
    precision       __f_;
};

template <class _Duration> const unsigned hh_mm_ss<_Duration>::fractional_width;

_CCW_LIBCPP_HIDE_FROM_ABI inline bool is_am(const hours& __h) { return __h >= hours(0)  && __h < hours(12); }
_CCW_LIBCPP_HIDE_FROM_ABI inline bool is_pm(const hours& __h) { return __h >= hours(12) && __h < hours(24); }

_CCW_LIBCPP_HIDE_FROM_ABI inline hours make12(const hours& __h) {
    if (__h == hours(0))  return hours(12);
    if (__h <= hours(12)) return __h;
    return __h - hours(12);
}

_CCW_LIBCPP_HIDE_FROM_ABI inline hours make24(const hours& __h, bool __is_pm) {
    if (__is_pm) return __h == hours(12) ? __h : __h + hours(12);
    return __h == hours(12) ? hours(0) : __h;
}

}   // namespace chrono
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___CHRONO_HH_MM_SS_H
