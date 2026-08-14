//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//
//  ccwrap-only header (libc++ splits these one per file and writes them against its own
//  __has_builtin machinery). These are the traits that CANNOT be written in the language
//  -- each needs a compiler intrinsic, and every compiler this tree supports has them
//  EXCEPT Open Watcom (no __is_enum / __is_union / __is_empty /
//  __is_polymorphic / __is_abstract / __is_base_of / __underlying_type, and no
//  substitute was found for any of them). is_class is the exception -- it IS
//  expressible without an intrinsic, and llibcxx03 does that in is_class.h.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_CCW_INTRINSIC_TRAITS_H
#define _CCW_LIBCPP___TYPE_TRAITS_CCW_INTRINSIC_TRAITS_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_scalar.h"
#include "../__type_traits/remove_extent.h"

#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && !defined(__WATCOMC__)

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
struct __ccw_scalar_core : public integral_constant<bool,
    is_scalar<typename remove_all_extents<_Tp>::type>::value> {};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_enum  : public integral_constant<bool, __is_enum(_Tp)> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_union : public integral_constant<bool, __is_union(_Tp)> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_empty : public integral_constant<bool, __is_empty(_Tp)> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_abstract : public integral_constant<bool, __is_abstract(_Tp)> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_polymorphic : public integral_constant<bool, __is_polymorphic(_Tp)> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_pod : public integral_constant<bool, __is_pod(_Tp) || __ccw_scalar_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS has_virtual_destructor : public integral_constant<bool, __has_virtual_destructor(_Tp)> {};

#if !defined(_MSC_VER) || _MSC_VER >= 1600
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_standard_layout : public integral_constant<bool, __is_standard_layout(_Tp) || __ccw_scalar_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivial : public integral_constant<bool, __is_trivial(_Tp) || __ccw_scalar_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_copyable : public integral_constant<bool, __has_trivial_copy(_Tp) || __ccw_scalar_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_literal_type : public integral_constant<bool, __is_literal_type(_Tp) || __ccw_scalar_core<_Tp>::value> {};
#else
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_standard_layout : public integral_constant<bool, __is_pod(_Tp) || __ccw_scalar_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivial : public integral_constant<bool, __is_pod(_Tp) || __ccw_scalar_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_copyable : public integral_constant<bool, __is_pod(_Tp) || __ccw_scalar_core<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_literal_type : public integral_constant<bool, __is_pod(_Tp) || __ccw_scalar_core<_Tp>::value> {};
#endif

#if !defined(_MSC_VER) || _MSC_VER >= 1700
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_final : public integral_constant<bool, __is_final(_Tp)> {};
#else
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_final : public integral_constant<bool, false> {};
#endif

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && !__WATCOMC__
#endif // _CCW_LIBCPP___TYPE_TRAITS_CCW_INTRINSIC_TRAITS_H
