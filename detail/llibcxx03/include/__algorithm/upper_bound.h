//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_UPPER_BOUND_H
#define _CCW_LIBCPP___ALGORITHM_UPPER_BOUND_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include "../__iterator/advance.h"
#include "../__iterator/distance.h"
#include "../__functional/operations.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIter, class _Tp, class _Compare>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter upper_bound(_FwdIter __first, _FwdIter __last, const _Tp& __v, _Compare __comp) {
  typename iterator_traits<_FwdIter>::difference_type __len = _CCW_STD::distance(__first, __last);
  while (__len > 0) {
    typename iterator_traits<_FwdIter>::difference_type __half = __len / 2;
    _FwdIter __mid = __first; _CCW_STD::advance(__mid, __half);
    if (__comp(__v, *__mid)) __len = __half;
    else { __first = __mid; ++__first; __len = __len - __half - 1; }
  }
  return __first;
}
template <class _FwdIter, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter upper_bound(_FwdIter __first, _FwdIter __last, const _Tp& __v) {
  return _CCW_STD::upper_bound(__first, __last, __v, less<_Tp>());
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
