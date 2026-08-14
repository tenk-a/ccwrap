// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  __algorithm/ranges_util.h -- the pieces every ranges:: algorithm needs:
//  projection invocation, range access, and the comparison function objects.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_RANGES_UTIL_H
#define _CCW_LIBCPP___ALGORITHM_RANGES_UTIL_H

#include "../__config"

// vc12 has its own class template std::identity in <type_traits>, which collides
// with the ranges identity; the ranges algorithms therefore start at vc14.
#if _CCW_HAS_ALIAS_TEMPLATE && (!defined(_MSC_VER) || _MSC_VER >= 1900)

#include "../__functional/identity.h"
#include <cstddef>
#include <iterator>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _Fp, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI inline auto __ccw_proj(_Fp& __f, _Tp& __x) -> decltype(__f(__x)) {
    return __f(__x);
}
template <class _Mp, class _Cp, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI inline auto __ccw_proj(_Mp _Cp::*__pm, _Tp& __x) -> decltype(__x.*__pm) {
    return __x.*__pm;
}
template <class _Rp, class _Cp, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI inline auto __ccw_proj(_Rp (_Cp::*__pf)() const, _Tp& __x) -> decltype((__x.*__pf)()) {
    return (__x.*__pf)();
}

struct equal_to {
    typedef void is_transparent;
    template <class _Tp, class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __a, const _Up& __b) const { return __a == __b; }
};
struct not_equal_to {
    typedef void is_transparent;
    template <class _Tp, class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __a, const _Up& __b) const { return !(__a == __b); }
};
struct less {
    typedef void is_transparent;
    template <class _Tp, class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __a, const _Up& __b) const { return __a < __b; }
};
struct greater {
    typedef void is_transparent;
    template <class _Tp, class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __a, const _Up& __b) const { return __b < __a; }
};
struct less_equal {
    typedef void is_transparent;
    template <class _Tp, class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __a, const _Up& __b) const { return !(__b < __a); }
};
struct greater_equal {
    typedef void is_transparent;
    template <class _Tp, class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __a, const _Up& __b) const { return !(__a < __b); }
};

template <class _Rp>
struct __ccw_is_range {
    typedef typename _CCW_STD::remove_reference<_Rp>::type __c;
    template <class _Up> _CCW_LIBCPP_HIDE_FROM_ABI static char __t(int, decltype(((_Up*)0)->begin())* = 0);
    template <class _Up> _CCW_LIBCPP_HIDE_FROM_ABI static double __t(long);
    enum { value = sizeof(__t<__c>(0)) == sizeof(char) };
};
template <class _Tp, _CCW_STD::size_t _Np> struct __ccw_is_range<_Tp (&)[_Np]> { enum { value = 1 }; };
template <class _Tp, _CCW_STD::size_t _Np> struct __ccw_is_range<_Tp[_Np]>     { enum { value = 1 }; };

template <bool _Bp, class _Tp> struct __ccw_range_en           { };
template <class _Tp>           struct __ccw_range_en<true, _Tp> { typedef _Tp type; };

#define _CCW_RNG_IF(_Rng, ...) typename ranges::__ccw_range_en<ranges::__ccw_is_range<_Rng>::value, __VA_ARGS__>::type
#define _CCW_RNG_IFN(_Rng, ...) typename ranges::__ccw_range_en<!ranges::__ccw_is_range<_Rng>::value, __VA_ARGS__>::type
#define _CCW_RNG_IT(_Rng)       decltype(ranges::__ccw_begin(*(typename _CCW_STD::remove_reference<_Rng>::type*)0))
#define _CCW_RNG_IF1N(_R1, _R2, ...) typename ranges::__ccw_range_en<ranges::__ccw_is_range<_R1>::value && !ranges::__ccw_is_range<_R2>::value, __VA_ARGS__>::type

#define _CCW_RNG_C ,

#define _CCW_RNG_RANGE_A1(_Nm, ...)                                                                 template <class _Rp, class _A0, class _Proj = identity>                                         _CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, __VA_ARGS__)                                         _Nm(_Rp&& __r, _A0 __a0, _Proj __pr = _Proj()) {                                                    return ranges::_Nm(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __a0, __pr); }

#define _CCW_RNG_RANGE_A0(_Nm, _Dflt, ...)                                                          template <class _Rp, class _A0 = _Dflt, class _Proj = identity>                                 _CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, __VA_ARGS__)                                         _Nm(_Rp&& __r, _A0 __a0 = _A0(), _Proj __pr = _Proj()) {                                            return ranges::_Nm(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __a0, __pr); }

#define _CCW_RNG_RANGE_2R(_Nm, ...)                                                                 template <class _R1, class _R2, class _Pred = equal_to,                                                   class _P1 = identity, class _P2 = identity>                                           _CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_R1, __VA_ARGS__)                                         _Nm(_R1&& __r1, _R2&& __r2, _Pred __p = _Pred(),                                                    _P1 __p1 = _P1(), _P2 __p2 = _P2()) {                                                           return ranges::_Nm(ranges::__ccw_begin(__r1), ranges::__ccw_end(__r1),                                             ranges::__ccw_begin(__r2), ranges::__ccw_end(__r2), __p, __p1, __p2); }

template <class _Rp>
_CCW_LIBCPP_HIDE_FROM_ABI inline auto __ccw_begin(_Rp& __r) -> decltype(__r.begin()) { return __r.begin(); }
template <class _Rp>
_CCW_LIBCPP_HIDE_FROM_ABI inline auto __ccw_end(_Rp& __r) -> decltype(__r.end()) { return __r.end(); }
template <class _Tp, _CCW_STD::size_t _Np>
_CCW_LIBCPP_HIDE_FROM_ABI inline _Tp* __ccw_begin(_Tp (&__a)[_Np]) { return __a; }
template <class _Tp, _CCW_STD::size_t _Np>
_CCW_LIBCPP_HIDE_FROM_ABI inline _Tp* __ccw_end(_Tp (&__a)[_Np]) { return __a + _Np; }

template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI inline typename _CCW_STD::iterator_traits<_It>::difference_type
__ccw_dist(_It __f, _It __l) { return _CCW_STD::distance(__f, __l); }

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_HAS_ALIAS_TEMPLATE && vc14+
#endif // _CCW_LIBCPP___ALGORITHM_RANGES_UTIL_H
