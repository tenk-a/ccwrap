//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#ifndef _CCW_LIBCPP___TYPE_TRAITS_ENABLE_IF_H
#define _CCW_LIBCPP___TYPE_TRAITS_ENABLE_IF_H

#include "../__config"
#include "__ccw_native_traits.h"

#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <bool, class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS enable_if {};
template <class _Tp>              struct _CCW_LIBCPP_TEMPLATE_VIS enable_if<true, _Tp> { typedef _Tp type; };

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

#endif // _CCW_LIBCPP___TYPE_TRAITS_ENABLE_IF_H
