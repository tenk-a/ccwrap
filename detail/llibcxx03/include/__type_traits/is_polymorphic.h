// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: is_polymorphic without a compiler intrinsic (Open Watcom).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_POLYMORPHIC_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_POLYMORPHIC_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_class.h"
#include "../__type_traits/remove_cv.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && defined(__WATCOMC__)
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp> struct __ccw_ipoly_raw {
    struct __d1 : public _Tp { __d1(); ~__d1(); char __pad_[256]; };
    struct __d2 : public _Tp { __d2(); virtual ~__d2(); char __pad_[256]; };
    enum { value = sizeof(__d1) == sizeof(__d2) };
};

template <bool _IsCls, class _Tp> struct __ccw_ipoly_gate { enum { value = 0 }; };
template <class _Tp> struct __ccw_ipoly_gate<true, _Tp> {
    enum { value = __ccw_ipoly_raw<typename remove_cv<_Tp>::type>::value };
};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_polymorphic
    : public integral_constant<bool, (bool)__ccw_ipoly_gate<(bool)is_class<_Tp>::value, _Tp>::value> {};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && __WATCOMC__
#endif // _CCW_LIBCPP___TYPE_TRAITS_IS_POLYMORPHIC_H
