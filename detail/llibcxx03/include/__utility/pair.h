//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom (no rvalue refs / move semantics).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___UTILITY_PAIR_H
#define _CCW_LIBCPP___UTILITY_PAIR_H
#include "../__config"
#include "../__utility/move.h"
#include "../__utility/piecewise_construct.h"
#include "../__utility/swap.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_same.h"
#include <cstddef>
#if _CCW_LIBCPP_NATIVE_STD
#  include <utility>
#else

#  define _CCW_HAS_PAIR_PIECEWISE 1

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>                       struct tuple_size;
template <_CCW_STD::size_t _Np, class _Tp> struct __ccw_tup_ctor;

template <class _T1, class _T2>
struct _CCW_LIBCPP_TEMPLATE_VIS pair {
    typedef _T1 first_type;
    typedef _T2 second_type;

    _T1 first;
    _T2 second;

    _CCW_LIBCPP_HIDE_FROM_ABI pair() : first(), second() {}
    _CCW_LIBCPP_HIDE_FROM_ABI pair(const _T1& __x, const _T2& __y) : first(__x), second(__y) {}
    template <class _U1, class _U2>
    _CCW_LIBCPP_HIDE_FROM_ABI pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
    template <class _Tup1, class _Tup2>
    _CCW_LIBCPP_HIDE_FROM_ABI pair(piecewise_construct_t, const _Tup1& __t1, const _Tup2& __t2)
        : first(__ccw_tup_ctor<(_CCW_STD::size_t)tuple_size<_Tup1>::value, _T1>::make(__t1)),
          second(__ccw_tup_ctor<(_CCW_STD::size_t)tuple_size<_Tup2>::value, _T2>::make(__t2)) {}
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI pair(_CCW_RV_REF(pair) __r)
        : first(::_ccw::move(_CCW_MOVED_OBJ(__r).first)),
          second(::_ccw::move(_CCW_MOVED_OBJ(__r).second)) {}
    template <class _U1, class _U2>
    _CCW_LIBCPP_HIDE_FROM_ABI pair(::_ccw::rv<_U1>& __x, ::_ccw::rv<_U2>& __y) : first(__x), second(__y) {}
    _CCW_LIBCPP_HIDE_FROM_ABI pair& operator=(_CCW_RV_REF(pair) __r) {
        pair& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) {
            first  = ::_ccw::move(__o.first);
            second = ::_ccw::move(__o.second);
        }
        return *this;
    }
#endif

    _CCW_LIBCPP_HIDE_FROM_ABI void swap(pair& __p) {
        _CCW_STD::swap(first,  __p.first);
        _CCW_STD::swap(second, __p.second);
    }
};

template <class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ return __x.first == __y.first && __x.second == __y.second; }

template <class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ return !(__x == __y); }

template <class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second); }

template <class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)  { return __y < __x; }
template <class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) { return !(__y < __x); }
template <class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) { return !(__x < __y); }

#if _CCW_CFG_MAKE_PAIR_BYREF
template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI pair<_T1,  _T2 > make_pair(const _T1& __x, const _T2& __y) { return pair<_T1,  _T2 >(__x, __y); }
template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI pair<_T1*, _T2 > make_pair(_T1* __x,       const _T2& __y) { return pair<_T1*, _T2 >(__x, __y); }
template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI pair<_T1,  _T2*> make_pair(const _T1& __x, _T2* __y)       { return pair<_T1,  _T2*>(__x, __y); }
template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI pair<_T1*, _T2*> make_pair(_T1* __x,       _T2* __y)       { return pair<_T1*, _T2*>(__x, __y); }
#else
template <class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI pair<_T1, _T2> make_pair(_T1 __x, _T2 __y) { return pair<_T1, _T2>(__x, __y); }
#endif

template <class _Tp>                   struct tuple_size;
template <_CCW_STD::size_t _Ip, class _Tp>  struct tuple_element;

template <class _T1, class _T2>
struct tuple_size<pair<_T1, _T2> > : integral_constant<_CCW_STD::size_t, 2> {};
template <class _T1, class _T2> struct tuple_element<0, pair<_T1, _T2> > { typedef _T1 type; };
template <class _T1, class _T2> struct tuple_element<1, pair<_T1, _T2> > { typedef _T2 type; };

template <_CCW_STD::size_t _Ip> struct __ccw_pair_get;
template <> struct __ccw_pair_get<0> {
    template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI static _T1&       __get(pair<_T1, _T2>& __p)       { return __p.first; }
    template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI static const _T1& __get(const pair<_T1, _T2>& __p) { return __p.first; }
};
template <> struct __ccw_pair_get<1> {
    template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI static _T2&       __get(pair<_T1, _T2>& __p)       { return __p.second; }
    template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI static const _T2& __get(const pair<_T1, _T2>& __p) { return __p.second; }
};
template <_CCW_STD::size_t _Ip, class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI typename tuple_element<_Ip, pair<_T1, _T2> >::type& get(pair<_T1, _T2>& __p) { return __ccw_pair_get<_Ip>::__get(__p); }
template <_CCW_STD::size_t _Ip, class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI const typename tuple_element<_Ip, pair<_T1, _T2> >::type& get(const pair<_T1, _T2>& __p) { return __ccw_pair_get<_Ip>::__get(__p); }

template <bool _First> struct __ccw_pair_get_t;
template <> struct __ccw_pair_get_t<true> {
    template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI static _T1&       __get(pair<_T1, _T2>& __p)       { return __p.first; }
    template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI static const _T1& __get(const pair<_T1, _T2>& __p) { return __p.first; }
};
template <> struct __ccw_pair_get_t<false> {
    template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI static _T2&       __get(pair<_T1, _T2>& __p)       { return __p.second; }
    template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI static const _T2& __get(const pair<_T1, _T2>& __p) { return __p.second; }
};
template <class _Tp, class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp& get(pair<_T1, _T2>& __p) { return __ccw_pair_get_t< is_same<_Tp, _T1>::value >::__get(__p); }
template <class _Tp, class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI const _Tp& get(const pair<_T1, _T2>& __p) { return __ccw_pair_get_t< is_same<_Tp, _T1>::value >::__get(__p); }

#if !defined(__WATCOMC__)
template <class _T1, class _T2>
_CCW_LIBCPP_HIDE_FROM_ABI void swap(pair<_T1, _T2>& __x, pair<_T1, _T2>& __y) { __x.swap(__y); }
#endif

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
