// -*- C++ -*-
//===----------------------------------------------------------------------===//
//  ccwrap llibcxx03 internal helper: integral-ness *tag* for overload dispatch.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_INTEGRAL_DISPATCH_H
#define _CCW_LIBCPP___TYPE_TRAITS_INTEGRAL_DISPATCH_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct __ccw_int_true_tag {};
struct __ccw_int_false_tag {};

template <class _Up> struct __ccw_dispatch_is_int         { typedef __ccw_int_false_tag _Tag; };
template <> struct __ccw_dispatch_is_int<bool>            { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<char>           { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<signed char>    { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<unsigned char>  { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<wchar_t>        { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<short>          { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<unsigned short> { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<int>            { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<unsigned int>   { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<long>           { typedef __ccw_int_true_tag _Tag; };
template <> struct __ccw_dispatch_is_int<unsigned long>  { typedef __ccw_int_true_tag _Tag; };

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___TYPE_TRAITS_INTEGRAL_DISPATCH_H
