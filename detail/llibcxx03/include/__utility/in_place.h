//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___UTILITY_IN_PLACE_H
#define _CCW_LIBCPP___UTILITY_IN_PLACE_H
#include "../__config"
#include <cstddef>
#if _CCW_LIBCPP_HAS_NATIVE_CXX17_LIB
#  include <utility>
#  define _CCW_LIBCPP___UTILITY_IN_PLACE_DEFINED
#  define _CCW_LIBCPP___UTILITY_IN_PLACE_TYPE_DEFINED
#  define _CCW_LIBCPP___UTILITY_IN_PLACE_INDEX_DEFINED
#else

#if _CCW_TARGET_CXX >= 2017 && !defined(_CCW_STD_RELOCATED)
#  define _CCW_LIBCPP___UTILITY_IN_PLACE_SHARES_STD 1
#else
#  define _CCW_LIBCPP___UTILITY_IN_PLACE_SHARES_STD 0
#endif

#if _CCW_LIBCPP___UTILITY_IN_PLACE_SHARES_STD
#  ifdef _CCW_STD_IN_PLACE_DEFINED
#    define _CCW_LIBCPP___UTILITY_IN_PLACE_DEFINED
#  endif
#  ifdef _CCW_STD_IN_PLACE_TYPE_DEFINED
#    define _CCW_LIBCPP___UTILITY_IN_PLACE_TYPE_DEFINED
#  endif
#  ifdef _CCW_STD_IN_PLACE_INDEX_DEFINED
#    define _CCW_LIBCPP___UTILITY_IN_PLACE_INDEX_DEFINED
#  endif
#endif

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#ifndef _CCW_LIBCPP___UTILITY_IN_PLACE_DEFINED
#define _CCW_LIBCPP___UTILITY_IN_PLACE_DEFINED
#if _CCW_LIBCPP___UTILITY_IN_PLACE_SHARES_STD
#  define _CCW_STD_IN_PLACE_DEFINED
#endif
struct in_place_t { _CCW_LIBCPP_HIDE_FROM_ABI explicit in_place_t() {} };
_ccw_inline_const in_place_t in_place = in_place_t();
#endif

#ifndef _CCW_LIBCPP___UTILITY_IN_PLACE_TYPE_DEFINED
#define _CCW_LIBCPP___UTILITY_IN_PLACE_TYPE_DEFINED
#if _CCW_LIBCPP___UTILITY_IN_PLACE_SHARES_STD
#  define _CCW_STD_IN_PLACE_TYPE_DEFINED
#endif
template <class _Tp>
struct in_place_type_t { _CCW_LIBCPP_HIDE_FROM_ABI explicit in_place_type_t() {} };
#if _CCW_HAS_VAR_TEMPLATE
template <class _Tp>
const in_place_type_t<_Tp> in_place_type = in_place_type_t<_Tp>();
#endif
#endif

#ifndef _CCW_LIBCPP___UTILITY_IN_PLACE_INDEX_DEFINED
#define _CCW_LIBCPP___UTILITY_IN_PLACE_INDEX_DEFINED
#if _CCW_LIBCPP___UTILITY_IN_PLACE_SHARES_STD
#  define _CCW_STD_IN_PLACE_INDEX_DEFINED
#endif
template <_CCW_STD::size_t _Ip>
struct in_place_index_t { _CCW_LIBCPP_HIDE_FROM_ABI explicit in_place_index_t() {} };
#if _CCW_HAS_VAR_TEMPLATE
template <_CCW_STD::size_t _Ip>
const in_place_index_t<_Ip> in_place_index = in_place_index_t<_Ip>();
#endif
#endif

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif // _CCW_LIBCPP___UTILITY_IN_PLACE_H
