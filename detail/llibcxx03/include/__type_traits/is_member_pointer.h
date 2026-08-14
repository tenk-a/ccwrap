/*
 * ccwrap: libc++ layout. classic C++03 body for Open Watcom.
 * (partial specialisation, no builtins needed)
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_MEMBER_POINTER_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_MEMBER_POINTER_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>            struct __ccw_libcpp_is_member_pointer             : public false_type {};
template <class _Tp, class _Cp> struct __ccw_libcpp_is_member_pointer<_Tp _Cp::*> : public true_type  {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_member_pointer : public __ccw_libcpp_is_member_pointer<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_member_pointer<const _Tp> : public __ccw_libcpp_is_member_pointer<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_member_pointer<volatile _Tp> : public __ccw_libcpp_is_member_pointer<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_member_pointer<const volatile _Tp> : public __ccw_libcpp_is_member_pointer<_Tp> {};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
