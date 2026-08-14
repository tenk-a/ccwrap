//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_ARRAY_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_ARRAY_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>              struct _CCW_LIBCPP_TEMPLATE_VIS is_array           : public false_type {};
template <class _Tp>              struct _CCW_LIBCPP_TEMPLATE_VIS is_array<_Tp[]>    : public true_type  {};
template <class _Tp, int _Np>     struct _CCW_LIBCPP_TEMPLATE_VIS is_array<_Tp[_Np]> : public true_type  {};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
