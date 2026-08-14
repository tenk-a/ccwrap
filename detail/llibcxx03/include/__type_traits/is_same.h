//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_SAME_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_SAME_H

#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"

#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp, class _Up> struct _CCW_LIBCPP_TEMPLATE_VIS is_same          : public false_type {};
template <class _Tp>            struct _CCW_LIBCPP_TEMPLATE_VIS is_same<_Tp, _Tp> : public true_type  {};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

#endif // _CCW_LIBCPP___TYPE_TRAITS_IS_SAME_H
