//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#ifndef _CCW_LIBCPP___TYPE_TRAITS_CONDITIONAL_H
#define _CCW_LIBCPP___TYPE_TRAITS_CONDITIONAL_H

#include "../__config"
#include "__ccw_native_traits.h"

#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <bool _Bp, class _If, class _Then> struct _CCW_LIBCPP_TEMPLATE_VIS conditional              { typedef _If   type; };
template <class _If, class _Then>           struct _CCW_LIBCPP_TEMPLATE_VIS conditional<false, _If, _Then> { typedef _Then type; };

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

#endif // _CCW_LIBCPP___TYPE_TRAITS_CONDITIONAL_H
