//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_REMOVE_ALL_EXTENTS_H
#define _CCW_LIBCPP___TYPE_TRAITS_REMOVE_ALL_EXTENTS_H
#include "../__config"
#include "__ccw_native_traits.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>          struct _CCW_LIBCPP_TEMPLATE_VIS remove_all_extents           { typedef _Tp type; };
template <class _Tp>          struct _CCW_LIBCPP_TEMPLATE_VIS remove_all_extents<_Tp[]>
    { typedef typename remove_all_extents<_Tp>::type type; };
template <class _Tp, int _Np> struct _CCW_LIBCPP_TEMPLATE_VIS remove_all_extents<_Tp[_Np]>
    { typedef typename remove_all_extents<_Tp>::type type; };
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
