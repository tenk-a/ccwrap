// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: is_enum without a compiler intrinsic (Open Watcom).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_ENUM_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_ENUM_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_arithmetic.h"
#include "../__type_traits/is_class.h"
#include "../__type_traits/is_reference.h"
#include "../__type_traits/is_void.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && defined(__WATCOMC__)
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp> struct __ccw_ienum_conv {
    static _Tp& __mk();
    static __ccw_ic_yes __chk(int);
    static __ccw_ic_no  __chk(...);
    enum { value = sizeof(__chk(__mk())) == sizeof(__ccw_ic_yes) };
};

template <bool _Excl, class _Tp> struct __ccw_ienum_gate { enum { value = 0 }; };
template <class _Tp> struct __ccw_ienum_gate<false, _Tp> { enum { value = __ccw_ienum_conv<_Tp>::value }; };

template <class _Tp> struct __ccw_is_enum_raw {
    enum { __cls = is_class<_Tp>::value };
    enum { __ari = is_arithmetic<_Tp>::value };
    enum { __ref = is_reference<_Tp>::value };
    enum { __vod = is_void<_Tp>::value };
    enum { __excl = __cls || __ari || __ref || __vod };
    enum { value = __ccw_ienum_gate<(bool)__excl, _Tp>::value };
};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_enum
    : public integral_constant<bool, (bool)__ccw_is_enum_raw<_Tp>::value> {};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && __WATCOMC__
#endif // _CCW_LIBCPP___TYPE_TRAITS_IS_ENUM_H
