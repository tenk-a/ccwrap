/*
 * ccwrap: libc++ layout. classic C++03 body for Open Watcom.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_UNSIGNED_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_UNSIGNED_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct __ccw_libcpp_is_unsigned              : public false_type {};
template <> struct __ccw_libcpp_is_unsigned<bool>                 : public true_type  {};
template <> struct __ccw_libcpp_is_unsigned<unsigned char>        : public true_type  {};
template <> struct __ccw_libcpp_is_unsigned<unsigned short>       : public true_type  {};
template <> struct __ccw_libcpp_is_unsigned<unsigned int>         : public true_type  {};
template <> struct __ccw_libcpp_is_unsigned<unsigned long>        : public true_type  {};
template <> struct __ccw_libcpp_is_unsigned<unsigned long long>   : public true_type  {};
#if _CCW_IS_CHAR_UNSIGNED
template <> struct __ccw_libcpp_is_unsigned<char>                 : public true_type  {};
#endif
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_unsigned : public __ccw_libcpp_is_unsigned<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_unsigned<const _Tp> : public __ccw_libcpp_is_unsigned<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_unsigned<volatile _Tp> : public __ccw_libcpp_is_unsigned<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_unsigned<const volatile _Tp> : public __ccw_libcpp_is_unsigned<_Tp> {};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
