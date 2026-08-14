// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: move_iterator for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_MOVE_ITERATOR_H
#define _CCW_LIBCPP___ITERATOR_MOVE_ITERATOR_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include "../__utility/move.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Iter>
class move_iterator {
    _Iter __i_;
public:
    typedef _Iter                                              iterator_type;
    typedef typename iterator_traits<_Iter>::iterator_category iterator_category;
    typedef typename iterator_traits<_Iter>::value_type        value_type;
    typedef typename iterator_traits<_Iter>::difference_type   difference_type;
    typedef _Iter                                              pointer;
#if _CCW_HAS_REAL_RVALUE_REF
    typedef value_type&&                                       reference;
#else
    typedef typename ::_ccw::__ccw_move_ret<value_type>::type  reference;
#endif

    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator() : __i_() {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit move_iterator(_Iter __x) : __i_(__x) {}
    template <class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator(const move_iterator<_Up>& __u) : __i_(__u.base()) {}
    template <class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator& operator=(const move_iterator<_Up>& __u) { __i_ = __u.base(); return *this; }

    _CCW_LIBCPP_HIDE_FROM_ABI _Iter base() const { return __i_; }

    _CCW_LIBCPP_HIDE_FROM_ABI reference operator*() const { return ::_ccw::move(*__i_); }
    _CCW_LIBCPP_HIDE_FROM_ABI pointer   operator->() const { return __i_; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference operator[](difference_type __n) const { return ::_ccw::move(__i_[__n]); }

    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator& operator++()    { ++__i_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator  operator++(int) { move_iterator __t = *this; ++__i_; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator& operator--()    { --__i_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator  operator--(int) { move_iterator __t = *this; --__i_; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator& operator+=(difference_type __n) { __i_ += __n; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator& operator-=(difference_type __n) { __i_ -= __n; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator  operator+(difference_type __n) const { return move_iterator(__i_ + __n); }
    _CCW_LIBCPP_HIDE_FROM_ABI move_iterator  operator-(difference_type __n) const { return move_iterator(__i_ - __n); }
};

template <class _I1, class _I2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const move_iterator<_I1>& __x, const move_iterator<_I2>& __y) { return __x.base() == __y.base(); }
template <class _I1, class _I2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const move_iterator<_I1>& __x, const move_iterator<_I2>& __y) { return __x.base() != __y.base(); }
template <class _I1, class _I2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator<(const move_iterator<_I1>& __x, const move_iterator<_I2>& __y) { return __x.base() < __y.base(); }
template <class _I1, class _I2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator>(const move_iterator<_I1>& __x, const move_iterator<_I2>& __y) { return __y.base() < __x.base(); }
template <class _I1, class _I2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator<=(const move_iterator<_I1>& __x, const move_iterator<_I2>& __y) { return !(__y.base() < __x.base()); }
template <class _I1, class _I2>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator>=(const move_iterator<_I1>& __x, const move_iterator<_I2>& __y) { return !(__x.base() < __y.base()); }

template <class _I1, class _I2>
_CCW_LIBCPP_HIDE_FROM_ABI typename move_iterator<_I1>::difference_type
operator-(const move_iterator<_I1>& __x, const move_iterator<_I2>& __y) { return __x.base() - __y.base(); }

template <class _Iter>
_CCW_LIBCPP_HIDE_FROM_ABI move_iterator<_Iter>
operator+(typename move_iterator<_Iter>::difference_type __n, const move_iterator<_Iter>& __x) { return __x + __n; }

template <class _Iter>
_CCW_LIBCPP_HIDE_FROM_ABI move_iterator<_Iter> make_move_iterator(_Iter __i) { return move_iterator<_Iter>(__i); }

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___ITERATOR_MOVE_ITERATOR_H
