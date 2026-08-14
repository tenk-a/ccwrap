/*
 * ccwrap: libc++ layout. classic C++03 body for Open Watcom.
 * remove_cvref (C++20) strips reference then cv.
 *
 * The reference and cv forms are enumerated directly rather than composed as
 * remove_cv<remove_reference<T>::type>::type -- that feeds a dependent nested typedef to
 * a template, which Open Watcom resolves unreliably (see remove_cv.h / is_member_pointer.h).
 * Open Watcom has no rvalue references, so only lvalue-reference forms appear.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_REMOVE_CVREF_H
#define _CCW_LIBCPP___TYPE_TRAITS_REMOVE_CVREF_H
#include "../__config"
#include "__ccw_native_traits.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS || !defined(__cpp_lib_remove_cvref)

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cvref                       { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cvref<const _Tp>            { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cvref<volatile _Tp>         { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cvref<const volatile _Tp>   { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cvref<_Tp&>                 { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cvref<const _Tp&>           { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cvref<volatile _Tp&>        { typedef _Tp type; };
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS remove_cvref<const volatile _Tp&>  { typedef _Tp type; };
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // native <type_traits> lacks __cpp_lib_remove_cvref
#endif
