/*
 * ccwrap: libc++ layout. classic C++03 body.
 *
 * No __is_destructible intrinsic and no decltype here, so this uses the classic
 * sizeof-expression SFINAE on a pseudo-destructor call (probe-verified on Open Watcom).
 * Special cases per [meta.unary.prop]: references are destructible; void and function
 * types are not. Arrays are not distinguished from their element (a known Watcom
 * array-trait limitation) -- an array of a destructible type may report false here.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_DESTRUCTIBLE_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_DESTRUCTIBLE_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#if !_CCW_LIBCPP_HAS_NATIVE_CTOR_TRAITS

#if defined(__has_builtin)
#  if __has_builtin(__is_destructible)
#    define _CCW_LIBCPP_IS_DESTRUCTIBLE_BUILTIN 1
#  endif
#endif

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Up> _Up& __ccw_destr_lval();
struct __ccw_destr_yes { char __c; };
struct __ccw_destr_no  { char __c[2]; };

template <class _Tp>
struct __ccw_is_destructible_test {
#if defined(_CCW_LIBCPP_IS_DESTRUCTIBLE_BUILTIN)
    static const bool value = __is_destructible(_Tp);
#elif defined(_MSC_VER) && _MSC_VER < 1600
    static const bool value = true;
#else
# if defined(_MSC_VER)
    template <class _Up> static __ccw_destr_yes __t(decltype(__ccw_destr_lval<_Up>().~_Up(), 0));
# else
    template <class _Up> static __ccw_destr_yes __t(char(*)[sizeof( (__ccw_destr_lval<_Up>().~_Up(), 0) )]);
# endif
    template <class _Up> static __ccw_destr_no  __t(...);
    static const bool value = sizeof(__t<_Tp>(0)) == sizeof(__ccw_destr_yes);
#endif
};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_destructible
    : public integral_constant<bool, __ccw_is_destructible_test<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_destructible<_Tp&>          : public true_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_destructible<void>          : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_destructible<const void>    : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_destructible<volatile void> : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_destructible<const volatile void> : public false_type {};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_nothrow_destructible
    : public is_destructible<_Tp> {};

#if defined(__WATCOMC__)
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_destructible
    : public is_destructible<_Tp> {};
#else
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_destructible
    : public integral_constant<bool, __has_trivial_destructor(_Tp)> {};
#endif

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_CTOR_TRAITS
#endif
