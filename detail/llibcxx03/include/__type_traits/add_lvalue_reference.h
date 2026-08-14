/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_ADD_LVALUE_REFERENCE_H
#define _CCW_LIBCPP___TYPE_TRAITS_ADD_LVALUE_REFERENCE_H
#include "../__config"
#include "__ccw_native_traits.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS add_lvalue_reference               { typedef _Tp&       type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS add_lvalue_reference<_Tp&>         { typedef _Tp&       type; };
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS add_lvalue_reference<void>               { typedef void       type; };
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS add_lvalue_reference<const void>         { typedef const void type; };
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS add_lvalue_reference<volatile void>      { typedef volatile void type; };
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS add_lvalue_reference<const volatile void>{ typedef const volatile void type; };
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
