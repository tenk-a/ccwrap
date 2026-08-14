//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom (no trait builtins, no SFINAE, no rvalue refs).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_FUNDAMENTAL_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_FUNDAMENTAL_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_arithmetic.h"
#include "../__type_traits/is_void.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_fundamental
    : public integral_constant<bool, is_arithmetic<_Tp>::value || is_void<_Tp>::value> {};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
