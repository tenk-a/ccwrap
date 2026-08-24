// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: is_abstract without a compiler intrinsic (Open Watcom).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_ABSTRACT_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_ABSTRACT_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_class.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && defined(__WATCOMC__)
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Vp> __ccw_ic_no  __ccw_iabs_test(void (*)(_Vp));
template <class _Vp> __ccw_ic_yes __ccw_iabs_test(...);

template <class _Tp> struct __ccw_iabs_raw {
    enum { value = sizeof(__ccw_iabs_test<_Tp>(0)) == sizeof(__ccw_ic_yes) };
};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_abstract
    : public integral_constant<bool, (bool)__ccw_iabs_raw<_Tp>::value> {};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && __WATCOMC__
#endif // _CCW_LIBCPP___TYPE_TRAITS_IS_ABSTRACT_H
