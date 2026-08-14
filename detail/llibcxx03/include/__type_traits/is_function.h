/*
 * ccwrap: libc++ layout. classic C++03 body for Open Watcom.
 * (no __is_function intrinsic needed).
 *
 * The pre-intrinsic "is_const<const T>" technique does NOT work on Open Watcom: it
 * (non-conformingly) accepts a cv-qualified function type, so is_const<const F> is true
 * for a function type F and the trick reports false. Instead we enumerate function
 * types by arity, Boost.TypeTraits style. Limitation: only plain (non-cv/ref-qualified,
 * non-variadic) function types of up to 8 parameters are recognised -- enough for the
 * standard's observable use (function vs object/reference/void). done (wat): partial.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_FUNCTION_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_FUNCTION_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_function : public false_type {};

template <class _Rp>
struct is_function<_Rp()> : public true_type {};
template <class _Rp, class _A0>
struct is_function<_Rp(_A0)> : public true_type {};
template <class _Rp, class _A0, class _A1>
struct is_function<_Rp(_A0, _A1)> : public true_type {};
template <class _Rp, class _A0, class _A1, class _A2>
struct is_function<_Rp(_A0, _A1, _A2)> : public true_type {};
template <class _Rp, class _A0, class _A1, class _A2, class _A3>
struct is_function<_Rp(_A0, _A1, _A2, _A3)> : public true_type {};
template <class _Rp, class _A0, class _A1, class _A2, class _A3, class _A4>
struct is_function<_Rp(_A0, _A1, _A2, _A3, _A4)> : public true_type {};
template <class _Rp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5>
struct is_function<_Rp(_A0, _A1, _A2, _A3, _A4, _A5)> : public true_type {};
template <class _Rp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6>
struct is_function<_Rp(_A0, _A1, _A2, _A3, _A4, _A5, _A6)> : public true_type {};
template <class _Rp, class _A0, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6, class _A7>
struct is_function<_Rp(_A0, _A1, _A2, _A3, _A4, _A5, _A6, _A7)> : public true_type {};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
