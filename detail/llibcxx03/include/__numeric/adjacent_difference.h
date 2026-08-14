//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_ADJACENT_DIFFERENCE_H
#define _CCW_LIBCPP___NUMERIC_ADJACENT_DIFFERENCE_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InIter, class _OutIter> _CCW_LIBCPP_HIDE_FROM_ABI _OutIter adjacent_difference(_InIter __first, _InIter __last, _OutIter __d) {
  if (__first == __last) return __d;
  typename iterator_traits<_InIter>::value_type __prev = *__first;
  *__d = __prev; ++__d; ++__first;
  for (; __first != __last; ++__first, ++__d) { typename iterator_traits<_InIter>::value_type __cur = *__first; *__d = __cur - __prev; __prev = __cur; }
  return __d;
}
template <class _InIter, class _OutIter, class _BinOp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutIter adjacent_difference(_InIter __first, _InIter __last, _OutIter __d, _BinOp __op) {
  if (__first == __last) return __d;
  typename iterator_traits<_InIter>::value_type __prev = *__first;
  *__d = __prev; ++__d; ++__first;
  for (; __first != __last; ++__first, ++__d) {
    typename iterator_traits<_InIter>::value_type __cur = *__first;
    *__d = __op(__cur, __prev);
    __prev = __cur;
  }
  return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
