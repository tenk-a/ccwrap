// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: is_base_of without a compiler intrinsic.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_BASE_OF_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_BASE_OF_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_class.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

typedef char __ccw_bo_yes;
struct __ccw_bo_no { char __pad_[2]; };

template <class _Bp, class _Dp>
struct __ccw_ptr_converts {
    static __ccw_bo_yes __test(const volatile _Bp*);
    static __ccw_bo_no  __test(const volatile void*);
    enum { value = sizeof(__test((_Dp*)0)) == sizeof(__ccw_bo_yes) };
};

template <class _Bp, class _Dp>
struct __ccw_is_base_of_raw {
    enum { __bc = __ccw_is_class_cv<_Bp>::value };
    enum { __dc = __ccw_is_class_cv<_Dp>::value };
    enum { __cv = __ccw_ptr_converts<_Bp, _Dp>::value };
    enum { value = __bc && __dc && __cv };
};

template <class _Bp, class _Dp>
struct _CCW_LIBCPP_TEMPLATE_VIS is_base_of
    : public integral_constant<bool, (bool)__ccw_is_base_of_raw<_Bp, _Dp>::value> {};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif // _CCW_LIBCPP___TYPE_TRAITS_IS_BASE_OF_H
