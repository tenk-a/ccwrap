//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_INCREMENTABLE_TRAITS_H
#define _CCW_LIBCPP___ITERATOR_INCREMENTABLE_TRAITS_H
#include "../__config"
#include <cstddef>   // ptrdiff_t
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Ip> struct incrementable_traits { typedef typename _Ip::difference_type difference_type; };
template <class _Tp> struct incrementable_traits<_Tp*> { typedef _CCW_STD::ptrdiff_t difference_type; };
template <class _Ip> struct incrementable_traits<const _Ip> : incrementable_traits<_Ip> {};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___ITERATOR_INCREMENTABLE_TRAITS_H
