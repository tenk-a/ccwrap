//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_IS_CONSTRUCTIBLE_H
#define _CCW_LIBCPP___TYPE_TRAITS_IS_CONSTRUCTIBLE_H
#include "../__config"
#include "is_class.h"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/__ccw_triviality.h"
#if !_CCW_LIBCPP_HAS_NATIVE_CTOR_TRAITS
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Up> _Up&       __ccw_ctor_lval();
template <class _Up> const _Up& __ccw_ctor_clval();
struct __ccw_ctor_yes { char __c; };
struct __ccw_ctor_no  { char __c[2]; };

template <class _Tp>
struct __ccw_is_dflt_ctible_test {
    template <class _Up> static __ccw_ctor_yes __t(char(*)[sizeof( (_Up(), 0) )]);
    template <class _Up> static __ccw_ctor_no  __t(...);
    static const bool value = sizeof(__t<_Tp>(0)) == sizeof(__ccw_ctor_yes);
};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_default_constructible
    : public integral_constant<bool, __ccw_is_dflt_ctible_test<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_default_constructible<_Tp&>  : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_default_constructible<void>               : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_default_constructible<const void>         : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_default_constructible<volatile void>      : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_default_constructible<const volatile void>: public false_type {};

template <class _Up> _Up __ccw_ctor_mk();

#if !defined(__WATCOMC__) && (defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 8))
#  define _CCW_LIBCPP_HAS_IS_CONSTRUCTIBLE_BUILTIN 1
#endif

#if defined(__WATCOMC__)
template <class _Tp>
struct __ccw_is_copy_ctible_test {
    template <class _Up> static __ccw_ctor_yes __t(char(*)[sizeof(__ccw_ctor_mk<_Up>())]);
    template <class _Up> static __ccw_ctor_no  __t(...);
    static const bool value = sizeof(__t<_Tp>(0)) == sizeof(__ccw_ctor_yes);
};
#elif defined(_CCW_LIBCPP_HAS_IS_CONSTRUCTIBLE_BUILTIN)
template <class _Tp>
struct __ccw_is_copy_ctible_test {
    static const bool value = __is_constructible(_Tp, const _Tp&);
};
#else
template <class _Tp>
struct __ccw_is_copy_ctible_test {
    template <class _Up> static __ccw_ctor_yes __t(char(*)[sizeof(_Up(__ccw_ctor_clval<_Up>()))]);
    template <class _Up> static __ccw_ctor_no  __t(...);
    static const bool value = sizeof(__t<_Tp>(0)) == sizeof(__ccw_ctor_yes);
};
#endif   // defined(__WATCOMC__)

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_constructible
    : public integral_constant<bool, __ccw_is_copy_ctible_test<_Tp>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_constructible<_Tp&>  : public true_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_constructible<void>               : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_constructible<const void>         : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_constructible<volatile void>      : public false_type {};
template <>          struct _CCW_LIBCPP_TEMPLATE_VIS is_copy_constructible<const volatile void>: public false_type {};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_move_constructible
    : public is_copy_constructible<_Tp> {};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_nothrow_default_constructible
    : public is_default_constructible<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_nothrow_copy_constructible
    : public is_copy_constructible<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_nothrow_move_constructible
    : public is_move_constructible<_Tp> {};

struct __ccw_ctor_none {};

template <class _Tp, class _A0>
struct __ccw_is_ctible1_test {
    template <class _Up> static __ccw_ctor_yes __t(char(*)[sizeof( (_Up(__ccw_ctor_clval<_A0>()), 0) )]);
    template <class _Up> static __ccw_ctor_no  __t(...);
    static const bool value = sizeof(__t<_Tp>(0)) == sizeof(__ccw_ctor_yes);
};
template <class _Tp, class _A0, class _A1>
struct __ccw_is_ctible2_test {
    template <class _Up> static __ccw_ctor_yes __t(char(*)[sizeof( (new _Up(__ccw_ctor_clval<_A0>(), __ccw_ctor_clval<_A1>()), 0) )]);
    template <class _Up> static __ccw_ctor_no  __t(...);
    static const bool value = sizeof(__t<_Tp>(0)) == sizeof(__ccw_ctor_yes);
};
template <class _Tp, class _A0, class _A1, class _A2>
struct __ccw_is_ctible3_test {
    template <class _Up> static __ccw_ctor_yes __t(char(*)[sizeof( (new _Up(__ccw_ctor_clval<_A0>(), __ccw_ctor_clval<_A1>(), __ccw_ctor_clval<_A2>()), 0) )]);
    template <class _Up> static __ccw_ctor_no  __t(...);
    static const bool value = sizeof(__t<_Tp>(0)) == sizeof(__ccw_ctor_yes);
};

template <bool _IsClass, class _Tp, class _A0, class _A1>
struct __ccw_ctible2_sel { enum { value = 0 }; };
template <class _Tp, class _A0, class _A1>
struct __ccw_ctible2_sel<true, _Tp, _A0, _A1> { enum { value = __ccw_is_ctible2_test<_Tp, _A0, _A1>::value }; };
template <bool _IsClass, class _Tp, class _A0, class _A1, class _A2>
struct __ccw_ctible3_sel { enum { value = 0 }; };
template <class _Tp, class _A0, class _A1, class _A2>
struct __ccw_ctible3_sel<true, _Tp, _A0, _A1, _A2> { enum { value = __ccw_is_ctible3_test<_Tp, _A0, _A1, _A2>::value }; };

template <class _Tp, class _A0 = __ccw_ctor_none, class _A1 = __ccw_ctor_none, class _A2 = __ccw_ctor_none>
struct _CCW_LIBCPP_TEMPLATE_VIS is_constructible
    : public integral_constant<bool, (bool)__ccw_ctible3_sel<is_class<_Tp>::value, _Tp, _A0, _A1, _A2>::value> {};
template <class _Tp, class _A0, class _A1>
struct _CCW_LIBCPP_TEMPLATE_VIS is_constructible<_Tp, _A0, _A1, __ccw_ctor_none>
    : public integral_constant<bool, (bool)__ccw_ctible2_sel<is_class<_Tp>::value, _Tp, _A0, _A1>::value> {};
template <class _Tp, class _A0>
struct _CCW_LIBCPP_TEMPLATE_VIS is_constructible<_Tp, _A0, __ccw_ctor_none, __ccw_ctor_none>
    : public integral_constant<bool, __ccw_is_ctible1_test<_Tp, _A0>::value> {};
template <class _Tp>
struct _CCW_LIBCPP_TEMPLATE_VIS is_constructible<_Tp, __ccw_ctor_none, __ccw_ctor_none, __ccw_ctor_none>
    : public is_default_constructible<_Tp> {};

template <class _Tp, class _A0 = __ccw_ctor_none, class _A1 = __ccw_ctor_none, class _A2 = __ccw_ctor_none>
struct _CCW_LIBCPP_TEMPLATE_VIS is_nothrow_constructible
    : public is_constructible<_Tp, _A0, _A1, _A2> {};
#if defined(__WATCOMC__)
template <class _Tp, class _A0 = __ccw_ctor_none, class _A1 = __ccw_ctor_none, class _A2 = __ccw_ctor_none>
struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_constructible
    : public integral_constant<bool, (bool)__ccw_triv_both<(bool)is_constructible<_Tp, _A0, _A1, _A2>::value,
                                                           (bool)__ccw_triv_core<_Tp>::value>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_default_constructible
    : public integral_constant<bool, (bool)__ccw_triv_both<(bool)is_default_constructible<_Tp>::value,
                                                           (bool)__ccw_triv_core<_Tp>::value>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_copy_constructible
    : public integral_constant<bool, (bool)__ccw_triv_both<(bool)is_copy_constructible<_Tp>::value,
                                                           (bool)__ccw_triv_core<_Tp>::value>::value> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_move_constructible
    : public integral_constant<bool, (bool)__ccw_triv_both<(bool)is_move_constructible<_Tp>::value,
                                                           (bool)__ccw_triv_core<_Tp>::value>::value> {};
#else
template <class _Tp, class _A0 = __ccw_ctor_none, class _A1 = __ccw_ctor_none, class _A2 = __ccw_ctor_none>
struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_constructible
    : public is_constructible<_Tp, _A0, _A1, _A2> {};

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_default_constructible
    : public is_default_constructible<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_copy_constructible
    : public is_copy_constructible<_Tp> {};
template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS is_trivially_move_constructible
    : public is_move_constructible<_Tp> {};
#endif // defined(__WATCOMC__)

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_HAS_NATIVE_CTOR_TRAITS
#endif
