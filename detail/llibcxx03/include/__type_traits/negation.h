//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom. negation (C++17) is a single-argument logical
// NOT over a BoolConstant; no variadic templates involved, so it ports as-is.
// (conjunction / disjunction are provided as fixed-arity emulations; see their headers.)
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_NEGATION_H
#define _CCW_LIBCPP___TYPE_TRAITS_NEGATION_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS || !defined(__cpp_lib_logical_traits)

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Bp> struct _CCW_LIBCPP_TEMPLATE_VIS negation
    : public integral_constant<bool, !_Bp::value> {};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // native <type_traits> lacks __cpp_lib_logical_traits
#endif
