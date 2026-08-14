//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#ifndef _CCW_LIBCPP___TYPE_TRAITS_INTEGRAL_CONSTANT_H
#define _CCW_LIBCPP___TYPE_TRAITS_INTEGRAL_CONSTANT_H

#include "../__config"
#include "__ccw_native_traits.h"

#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#if defined(__WATCOMC__)
template <class _Tp, long __v>
#else
template <class _Tp, _Tp __v>
#endif
struct _CCW_LIBCPP_TEMPLATE_VIS integral_constant {
  static const _Tp value = (_Tp)__v;
  typedef _Tp value_type;
  typedef integral_constant type;
  _CCW_LIBCPP_HIDE_FROM_ABI operator value_type() const _CCW_LIBCPP_NOEXCEPT { return value; }
};

#if !defined(__WATCOMC__)
template <class _Tp, _Tp __v>
const _Tp integral_constant<_Tp, __v>::value;
#endif

typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;

template <bool _Bp> struct bool_constant : public integral_constant<bool, _Bp> {};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

#endif // _CCW_LIBCPP___TYPE_TRAITS_INTEGRAL_CONSTANT_H
