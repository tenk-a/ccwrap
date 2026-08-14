/*
 * ccwrap: libc++ layout. classic C++03 body for Open Watcom.
 * is_object == not(function|reference|void),
 * which is exact and needs no intrinsic (given the intrinsic-free is_function).
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_OBJECT_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_OBJECT_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_function.h"
#include "../__type_traits/is_reference.h"
#include "../__type_traits/is_void.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_object
    : public integral_constant<bool,
        !is_function<_Tp>::value && !is_reference<_Tp>::value && !is_void<_Tp>::value> {};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
