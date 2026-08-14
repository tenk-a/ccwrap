//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap: classic C++03 is_convertible for Open Watcom via the two-overload sizeof
//  trick (no compiler builtin -- ordinary overload resolution between test(_To) and
//  test(...)).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_CONVERTIBLE_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_CONVERTIBLE_H

#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_class.h"
#include "../__type_traits/is_base_of.h"
#include "../__type_traits/is_same.h"

#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <bool _ClassInvolved, class _From, class _To>
struct __ccw_is_convertible_impl;

template <class _From, class _To>
struct __ccw_is_convertible_impl<false, _From, _To> {
    typedef char __yes;
    struct __no { char __c[2]; };
    _CCW_LIBCPP_HIDE_FROM_ABI static __yes __test(_To);
    _CCW_LIBCPP_HIDE_FROM_ABI static __no  __test(...);
    _CCW_LIBCPP_HIDE_FROM_ABI static _From __make();
    enum { value = sizeof(__ccw_is_convertible_impl::__test(__ccw_is_convertible_impl::__make())) == sizeof(__yes) };
};

template <class _From, class _To>
struct __ccw_is_convertible_impl<true, _From, _To> {
    enum { __same = is_same<_From, _To>::value };
    enum { __base = __ccw_is_base_of_raw<_To, _From>::value };   // _To is a base of _From
    enum { value = __same || __base };
};

template <class _From, class _To>
struct __ccw_is_convertible_class_gate {
    enum { __f = __ccw_is_class_cv<_From>::value };
    enum { __t = __ccw_is_class_cv<_To>::value };
    enum { value = __f || __t };
};

template <class _From, class _To>
struct _CCW_LIBCPP_TEMPLATE_VIS is_convertible
    : public integral_constant<bool,
        (bool)__ccw_is_convertible_impl<(bool)__ccw_is_convertible_class_gate<_From, _To>::value,
                                        _From, _To>::value> {};

template <class _To>   struct _CCW_LIBCPP_TEMPLATE_VIS is_convertible<void, _To>   : public false_type {};
template <class _From> struct _CCW_LIBCPP_TEMPLATE_VIS is_convertible<_From, void> : public false_type {};
template <>            struct _CCW_LIBCPP_TEMPLATE_VIS is_convertible<void, void>  : public true_type  {};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

#endif // _CCW_LIBCPP___TYPE_TRAITS_IS_CONVERTIBLE_H
