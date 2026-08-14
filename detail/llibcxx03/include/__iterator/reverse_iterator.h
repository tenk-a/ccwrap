//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_REVERSE_ITERATOR_H
#define _CCW_LIBCPP___ITERATOR_REVERSE_ITERATOR_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#if _CCW_LIBCPP_HAS_NATIVE_ITERATOR_TRAITS
#  include <iterator>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Iter>
class _CCW_LIBCPP_TEMPLATE_VIS reverse_iterator {
protected:
    _Iter current;
public:
    typedef _Iter                                                iterator_type;
    typedef typename iterator_traits<_Iter>::value_type          value_type;
    typedef typename iterator_traits<_Iter>::difference_type     difference_type;
    typedef typename iterator_traits<_Iter>::pointer             pointer;
    typedef typename iterator_traits<_Iter>::reference           reference;
    typedef typename iterator_traits<_Iter>::iterator_category   iterator_category;

    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator() : current() {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit reverse_iterator(_Iter __x) : current(__x) {}
    template <class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator(const reverse_iterator<_Up>& __u) : current(__u.base()) {}

    _CCW_LIBCPP_HIDE_FROM_ABI _Iter base() const { return current; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference operator*() const { _Iter __t = current; return *--__t; }
    _CCW_LIBCPP_HIDE_FROM_ABI pointer   operator->() const { return &(operator*()); }

    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator& operator++()    { --current; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator  operator++(int) { reverse_iterator __t = *this; --current; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator& operator--()    { ++current; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator  operator--(int) { reverse_iterator __t = *this; ++current; return __t; }

    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator  operator+(difference_type __n) const { return reverse_iterator(current - __n); }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator& operator+=(difference_type __n) { current -= __n; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator  operator-(difference_type __n) const { return reverse_iterator(current + __n); }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator& operator-=(difference_type __n) { current += __n; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference operator[](difference_type __n) const { return *(*this + __n); }
};

template <class _I1, class _I2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) { return __x.base() == __y.base(); }
template <class _I1, class _I2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) { return __x.base() != __y.base(); }
template <class _I1, class _I2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator< (const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) { return __x.base() >  __y.base(); }
template <class _I1, class _I2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator> (const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) { return __x.base() <  __y.base(); }
template <class _I1, class _I2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator<=(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) { return __x.base() >= __y.base(); }
template <class _I1, class _I2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator>=(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) { return __x.base() <= __y.base(); }

template <class _I1, class _I2>
_CCW_LIBCPP_HIDE_FROM_ABI typename reverse_iterator<_I1>::difference_type
operator-(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
    return __y.base() - __x.base();
}

template <class _Iter>
_CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator<_Iter>
operator+(typename reverse_iterator<_Iter>::difference_type __n, const reverse_iterator<_Iter>& __x) {
    return reverse_iterator<_Iter>(__x.base() - __n);
}

template <class _Iter> _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator<_Iter> make_reverse_iterator(_Iter __i) { return reverse_iterator<_Iter>(__i); }

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_HAS_NATIVE_ITERATOR_TRAITS
#endif
