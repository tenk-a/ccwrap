// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//
//  ccwrap-only header (libc++ splits these one per file and writes them against
//  __has_builtin). Open Watcom has no triviality intrinsic and none can be written in
//  the language, so the whole family answers true only where the type category alone
//  guarantees triviality: scalars, arrays of scalars, and nothing else. A false
//  negative is possible for a class; a false positive never is.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_CCW_TRIVIALITY_H
#define _CCW_LIBCPP___TYPE_TRAITS_CCW_TRIVIALITY_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_scalar.h"
#include "../__type_traits/remove_all_extents.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && defined(__WATCOMC__)
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp> struct __ccw_triv_core
    : public integral_constant<bool, is_scalar<typename remove_all_extents<_Tp>::type>::value> {};

template <bool _A, bool _B> struct __ccw_triv_both { enum { value = 0 }; };
template <> struct __ccw_triv_both<true, true>     { enum { value = 1 }; };

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_pod
    : public integral_constant<bool, (bool)__ccw_triv_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_standard_layout
    : public integral_constant<bool, (bool)__ccw_triv_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivial
    : public integral_constant<bool, (bool)__ccw_triv_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_copyable
    : public integral_constant<bool, (bool)__ccw_triv_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_literal_type
    : public integral_constant<bool, (bool)__ccw_triv_core<_Tp>::value> {};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_final : public false_type {};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && __WATCOMC__
#endif // _CCW_LIBCPP___TYPE_TRAITS_CCW_TRIVIALITY_H
