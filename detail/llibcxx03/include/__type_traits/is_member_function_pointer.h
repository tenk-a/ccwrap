//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_MEMBER_FUNCTION_POINTER_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_MEMBER_FUNCTION_POINTER_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct __ccw_libcpp_is_member_function_pointer : public false_type {};

template <class _Rp, class _Cp> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)()> : public true_type {};
template <class _Rp, class _Cp> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)() const> : public true_type {};
template <class _Rp, class _Cp> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)() volatile> : public true_type {};
template <class _Rp, class _Cp> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)() const volatile> : public true_type {};

template <class _Rp, class _Cp, class _A0> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0)> : public true_type {};
template <class _Rp, class _Cp, class _A0> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0) const> : public true_type {};
template <class _Rp, class _Cp, class _A0> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0) volatile> : public true_type {};
template <class _Rp, class _Cp, class _A0> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0) const volatile> : public true_type {};

template <class _Rp, class _Cp, class _A0, class _A1> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1)> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1) const> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1) volatile> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1) const volatile> : public true_type {};

template <class _Rp, class _Cp, class _A0, class _A1, class _A2> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2)> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2) const> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2) volatile> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2) const volatile> : public true_type {};

template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3)> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3) const> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3) volatile> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3) const volatile> : public true_type {};

template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4)> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4) const> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4) volatile> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4) const volatile> : public true_type {};

template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5)> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5) const> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5) volatile> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5) const volatile> : public true_type {};

template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5, _A6)> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5, _A6) const> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5, _A6) volatile> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5, _A6) const volatile> : public true_type {};

template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6, class _A7> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5, _A6, _A7)> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6, class _A7> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5, _A6, _A7) const> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6, class _A7> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5, _A6, _A7) volatile> : public true_type {};
template <class _Rp, class _Cp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6, class _A7> struct __ccw_libcpp_is_member_function_pointer<_Rp (_Cp::*)(_A0, _A1, _A2, _A3, _A4, _A5, _A6, _A7) const volatile> : public true_type {};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_member_function_pointer                     : public __ccw_libcpp_is_member_function_pointer<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_member_function_pointer<const _Tp>          : public __ccw_libcpp_is_member_function_pointer<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_member_function_pointer<volatile _Tp>       : public __ccw_libcpp_is_member_function_pointer<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_member_function_pointer<const volatile _Tp> : public __ccw_libcpp_is_member_function_pointer<_Tp> {};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
