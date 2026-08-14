//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_READABLE_TRAITS_H
#define _CCW_LIBCPP___ITERATOR_READABLE_TRAITS_H
#include "../__config"
#include "../__type_traits/remove_cv.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Ip> struct indirectly_readable_traits { typedef typename _Ip::value_type value_type; };
template <class _Tp> struct indirectly_readable_traits<_Tp*> { typedef typename remove_cv<_Tp>::type value_type; };
template <class _Ip> struct indirectly_readable_traits<const _Ip> : indirectly_readable_traits<_Ip> {};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___ITERATOR_READABLE_TRAITS_H
