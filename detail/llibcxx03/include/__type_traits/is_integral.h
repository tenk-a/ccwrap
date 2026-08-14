//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_INTEGRAL_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_INTEGRAL_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct __ccw_libcpp_is_integral                     : public false_type {};
template <>          struct __ccw_libcpp_is_integral<bool>               : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<char>               : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<signed char>        : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<unsigned char>      : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<wchar_t>            : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<short>              : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<unsigned short>     : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<int>                : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<unsigned int>       : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<long>               : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<unsigned long>      : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<long long>          : public true_type  {};
template <>          struct __ccw_libcpp_is_integral<unsigned long long> : public true_type  {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_integral : public __ccw_libcpp_is_integral<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_integral<const _Tp> : public __ccw_libcpp_is_integral<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_integral<volatile _Tp> : public __ccw_libcpp_is_integral<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_integral<const volatile _Tp> : public __ccw_libcpp_is_integral<_Tp> {};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
