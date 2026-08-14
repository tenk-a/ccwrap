// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: is_class / is_union / is_enum without compiler intrinsics.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_CLASS_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_CLASS_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_arithmetic.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

typedef char __ccw_ic_yes;
struct __ccw_ic_no { char __pad_[2]; };
template <class _Up> __ccw_ic_yes __ccw_ic_test(int, void (_Up::*)(void) = 0);
template <class _Up> __ccw_ic_no  __ccw_ic_test(long);

template <class _Tp> struct __ccw_is_class_raw {
    enum { value = sizeof(__ccw_ic_test<_Tp>(0)) == sizeof(__ccw_ic_yes) };
};

template <class _Tp> struct __ccw_is_class_cv                     : public __ccw_is_class_raw<_Tp> {};
template <class _Tp> struct __ccw_is_class_cv<const _Tp>          : public __ccw_is_class_raw<_Tp> {};
template <class _Tp> struct __ccw_is_class_cv<volatile _Tp>       : public __ccw_is_class_raw<_Tp> {};
template <class _Tp> struct __ccw_is_class_cv<const volatile _Tp> : public __ccw_is_class_raw<_Tp> {};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_class
    : public integral_constant<bool, (bool)__ccw_is_class_cv<_Tp>::value> {};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif // _CCW_LIBCPP___TYPE_TRAITS_IS_CLASS_H
