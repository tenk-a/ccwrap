//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom. type_identity (C++20) is a plain typedef box.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_TYPE_IDENTITY_H
#define _CCW_LIBCPP___TYPE_TRAITS_TYPE_IDENTITY_H
#include "../__config"
#include "__ccw_native_traits.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct type_identity { typedef _Tp type; };
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
