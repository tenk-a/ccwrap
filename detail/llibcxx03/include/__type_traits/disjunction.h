/*
 * ccwrap: libc++ layout. classic C++03 body.
 *
 * disjunction (C++17) is a variadic logical OR that inherits from the first argument
 * whose ::value is true, or from the last argument otherwise.
 *
 * Fixed-arity emulation (up to 5 operands, padded with false_type); same two deviations
 * as conjunction (arity cap, no short-circuit instantiation). See conjunction.h.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_DISJUNCTION_H
#define _CCW_LIBCPP___TYPE_TRAITS_DISJUNCTION_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/conditional.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS || !defined(__cpp_lib_logical_traits)
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _B1, class _B2, class _B3, class _B4, class _B5>
struct __ccw_disjunction
    : public conditional< _B1::value != false, _B1,
          typename conditional< _B2::value != false, _B2,
              typename conditional< _B3::value != false, _B3,
                  typename conditional< _B4::value != false, _B4, _B5>::type
              >::type
          >::type
      >::type {};

template <class _B1 = false_type, class _B2 = false_type, class _B3 = false_type,
          class _B4 = false_type, class _B5 = false_type>
struct _CCW_LIBCPP_TEMPLATE_VIS disjunction : public __ccw_disjunction<_B1, _B2, _B3, _B4, _B5> {};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // native <type_traits> lacks __cpp_lib_logical_traits
#endif
