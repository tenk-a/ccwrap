//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_PARTIAL_SUM_H
#define _CCW_LIBCPP___NUMERIC_PARTIAL_SUM_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InIter, class _OutIter> _CCW_LIBCPP_HIDE_FROM_ABI _OutIter partial_sum(_InIter __first, _InIter __last, _OutIter __d) {
  if (__first == __last) return __d;
  typename iterator_traits<_InIter>::value_type __acc = *__first;
  *__d = __acc; ++__d; ++__first;
  for (; __first != __last; ++__first, ++__d) { __acc = __acc + *__first; *__d = __acc; }
  return __d;
}
template <class _InIter, class _OutIter, class _BinOp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutIter partial_sum(_InIter __first, _InIter __last, _OutIter __d, _BinOp __op) {
  if (__first == __last) return __d;
  typename iterator_traits<_InIter>::value_type __acc = *__first;
  *__d = __acc; ++__d; ++__first;
  for (; __first != __last; ++__first, ++__d) { __acc = __op(__acc, *__first); *__d = __acc; }
  return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
