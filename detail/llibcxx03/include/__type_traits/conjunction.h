/*
 * ccwrap: libc++ layout. classic C++03 body.
 * conjunction (C++17) is a variadic logical AND that inherits from the first argument
 * whose ::value is false, or from the last argument otherwise.
 *
 * Open Watcom has no variadic templates, so this is a FIXED-ARITY emulation (up to 5
 * operands, padded with true_type). Two known deviations from the standard:
 *   * at most 5 operands;
 *   * no short-circuit INSTANTIATION -- every operand's ::value is read, so an operand
 *     that is ill-formed after an earlier false one is not shielded (fine for the usual
 *     use where all operands are already-formed bool constants);
 *   * when fewer than 5 operands are given, the trailing slots are true_type padding, so
 *     an all-true conjunction inherits from the padding rather than the caller's last
 *     operand -- the bool ::value is always correct, but a non-bool value only passes
 *     through when all 5 slots are explicit. (disjunction is unaffected: it selects the
 *     first TRUE operand, which is always a real one.)
 * `::value != false` is used instead of a bool() cast to dodge Open Watcom's function-
 * style-cast-with-:: misparse.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_CONJUNCTION_H
#define _CCW_LIBCPP___TYPE_TRAITS_CONJUNCTION_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/conditional.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS || !defined(__cpp_lib_logical_traits)
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _B1, class _B2, class _B3, class _B4, class _B5>
struct __ccw_conjunction
    : public conditional< _B1::value != false,
          typename conditional< _B2::value != false,
              typename conditional< _B3::value != false,
                  typename conditional< _B4::value != false, _B5, _B4>::type,
              _B3>::type,
          _B2>::type,
      _B1>::type {};

template <class _B1 = true_type, class _B2 = true_type, class _B3 = true_type,
          class _B4 = true_type, class _B5 = true_type>
struct _CCW_LIBCPP_TEMPLATE_VIS conjunction : public __ccw_conjunction<_B1, _B2, _B3, _B4, _B5> {};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // native <type_traits> lacks __cpp_lib_logical_traits
#endif
