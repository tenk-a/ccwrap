//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_REMOVE_CV_H
#define _CCW_LIBCPP___TYPE_TRAITS_REMOVE_CV_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/remove_const.h"
#include "../__type_traits/remove_volatile.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cv                     { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cv<const _Tp>          { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cv<volatile _Tp>       { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cv<const volatile _Tp> { typedef _Tp type; };
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
