//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_NEXT_PREV_H
#define _CCW_LIBCPP___ITERATOR_NEXT_PREV_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include "../__iterator/advance.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI _It next(_It __i, typename iterator_traits<_It>::difference_type __n) { _CCW_STD::advance(__i, __n); return __i; }
template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI _It next(_It __i) { ++__i; return __i; }
template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI _It prev(_It __i, typename iterator_traits<_It>::difference_type __n) { _CCW_STD::advance(__i, -__n); return __i; }
template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI _It prev(_It __i) { --__i; return __i; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
