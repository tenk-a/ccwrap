//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_FLOATING_POINT_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_FLOATING_POINT_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct __ccw_libcpp_is_floating_point              : public false_type {};
template <>          struct __ccw_libcpp_is_floating_point<float>       : public true_type  {};
template <>          struct __ccw_libcpp_is_floating_point<double>      : public true_type  {};
template <>          struct __ccw_libcpp_is_floating_point<long double> : public true_type  {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_floating_point : public __ccw_libcpp_is_floating_point<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_floating_point<const _Tp> : public __ccw_libcpp_is_floating_point<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_floating_point<volatile _Tp> : public __ccw_libcpp_is_floating_point<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_floating_point<const volatile _Tp> : public __ccw_libcpp_is_floating_point<_Tp> {};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
