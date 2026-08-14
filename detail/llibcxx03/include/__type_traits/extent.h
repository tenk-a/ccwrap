//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_EXTENT_H
#define _CCW_LIBCPP___TYPE_TRAITS_EXTENT_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp, unsigned _Ip = 0>          struct _CCW_LIBCPP_TEMPLATE_VIS extent            : public integral_constant<unsigned long, 0> {};
template <class _Tp>          struct _CCW_LIBCPP_TEMPLATE_VIS extent<_Tp[], 0>    : public integral_constant<unsigned long, 0> {};
template <class _Tp, unsigned _Ip> struct _CCW_LIBCPP_TEMPLATE_VIS extent<_Tp[], _Ip> : public extent<_Tp, _Ip - 1> {};
template <class _Tp, int _Np>          struct _CCW_LIBCPP_TEMPLATE_VIS extent<_Tp[_Np], 0>    : public integral_constant<unsigned long, _Np> {};
template <class _Tp, int _Np, unsigned _Ip> struct _CCW_LIBCPP_TEMPLATE_VIS extent<_Tp[_Np], _Ip> : public extent<_Tp, _Ip - 1> {};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
