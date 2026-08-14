//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_ASSIGNABLE_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_ASSIGNABLE_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/is_const.h"
#include "../__type_traits/is_class.h"
#if !_CCW_LIBCPP_HAS_NATIVE_CTOR_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Up> _Up        __ccw_asg_declval();
template <class _Up> _Up&       __ccw_asg_lval();
template <class _Up> const _Up& __ccw_asg_clval();
struct __ccw_asg_yes { char __c; };
struct __ccw_asg_no  { char __c[2]; };

template <class _Tp, class _Up>
struct __ccw_is_assignable_test {
    template <class _T1, class _U1>
    static __ccw_asg_yes __t(char(*)[sizeof( (__ccw_asg_declval<_T1>() = __ccw_asg_declval<_U1>(), 0) )]);
    template <class _T1, class _U1>
    static __ccw_asg_no  __t(...);
    static const bool value = sizeof(__t<_Tp, _Up>(0)) == sizeof(__ccw_asg_yes);
};

#if defined(_MSC_VER) && _MSC_VER < 1600
template <class _Tp> struct __ccw_asg_target             { typedef _Tp type; enum { __ref = 0 }; };
template <class _Tp> struct __ccw_asg_target<_Tp&>       { typedef _Tp type; enum { __ref = 1 }; };

template <class _Tp, class _Up>
struct __ccw_is_assignable_rule {
    typedef typename __ccw_asg_target<_Tp>::type __tgt;
    enum { __c = is_const<__tgt>::value };
    enum { __ok = (int)__ccw_asg_target<_Tp>::__ref || (int)is_class<__tgt>::value };
    enum { value = !__c && __ok };
};
#endif

#if defined(_MSC_VER) && _MSC_VER < 1600
template <class _Tp, class _Up> struct _CCW_LIBCPP_TEMPLATE_VIS is_assignable
    : public integral_constant<bool, __ccw_is_assignable_rule<_Tp, _Up>::value> {};
#else
template <class _Tp, class _Up> struct _CCW_LIBCPP_TEMPLATE_VIS is_assignable
    : public integral_constant<bool, __ccw_is_assignable_test<_Tp, _Up>::value> {};
#endif
template <class _Up> struct _CCW_LIBCPP_TEMPLATE_VIS is_assignable<void, _Up>                : public false_type {};
template <class _Up> struct _CCW_LIBCPP_TEMPLATE_VIS is_assignable<const void, _Up>          : public false_type {};
template <class _Up> struct _CCW_LIBCPP_TEMPLATE_VIS is_assignable<volatile void, _Up>       : public false_type {};
template <class _Up> struct _CCW_LIBCPP_TEMPLATE_VIS is_assignable<const volatile void, _Up> : public false_type {};

template <class _Tp>
struct __ccw_is_copy_asgn_test {
    template <class _Up>
    static __ccw_asg_yes __t(char(*)[sizeof( (__ccw_asg_lval<_Up>() = __ccw_asg_clval<_Up>(), 0) )]);
    template <class _Up>
    static __ccw_asg_no  __t(...);
    static const bool value = sizeof(__t<_Tp>(0)) == sizeof(__ccw_asg_yes);
};

#if defined(_MSC_VER) && _MSC_VER < 1600
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_assignable
    : public integral_constant<bool, !is_const<_Tp>::value> {};
#else
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_assignable
    : public integral_constant<bool, __ccw_is_copy_asgn_test<_Tp>::value> {};
#endif
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_assignable<void>                : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_assignable<const void>          : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_assignable<volatile void>       : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_assignable<const volatile void> : public false_type {};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_move_assignable
    : public is_copy_assignable<_Tp> {};

template <class _Tp, class _Up> struct _CCW_LIBCPP_TEMPLATE_VIS is_nothrow_assignable
    : public is_assignable<_Tp, _Up> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_nothrow_copy_assignable
    : public is_copy_assignable<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_nothrow_move_assignable
    : public is_move_assignable<_Tp> {};

template <class _Tp, class _Up> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_assignable
    : public is_assignable<_Tp, _Up> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_copy_assignable
    : public is_copy_assignable<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_move_assignable
    : public is_move_assignable<_Tp> {};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_CTOR_TRAITS
#endif
