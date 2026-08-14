/*
 * ccwrap: libc++ layout. classic C++03 body for Open Watcom.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_SIGNED_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_SIGNED_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct __ccw_libcpp_is_signed             : public false_type {};
template <> struct __ccw_libcpp_is_signed<signed char>         : public true_type  {};
template <> struct __ccw_libcpp_is_signed<short>               : public true_type  {};
template <> struct __ccw_libcpp_is_signed<int>                 : public true_type  {};
template <> struct __ccw_libcpp_is_signed<long>                : public true_type  {};
template <> struct __ccw_libcpp_is_signed<long long>           : public true_type  {};
template <> struct __ccw_libcpp_is_signed<float>               : public true_type  {};
template <> struct __ccw_libcpp_is_signed<double>              : public true_type  {};
template <> struct __ccw_libcpp_is_signed<long double>         : public true_type  {};
#if !_CCW_IS_CHAR_UNSIGNED
template <> struct __ccw_libcpp_is_signed<char>                : public true_type  {};
#endif
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_signed : public __ccw_libcpp_is_signed<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_signed<const _Tp> : public __ccw_libcpp_is_signed<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_signed<volatile _Tp> : public __ccw_libcpp_is_signed<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_signed<const volatile _Tp> : public __ccw_libcpp_is_signed<_Tp> {};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
