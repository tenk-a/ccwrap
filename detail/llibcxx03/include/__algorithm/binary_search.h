//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_BINARY_SEARCH_H
#define _CCW_LIBCPP___ALGORITHM_BINARY_SEARCH_H
#include "../__config"
#include "../__algorithm/lower_bound.h"
#include "../__functional/operations.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIter, class _Tp, class _Compare>
_CCW_LIBCPP_HIDE_FROM_ABI bool binary_search(_FwdIter __first, _FwdIter __last, const _Tp& __v, _Compare __comp) {
  __first = _CCW_STD::lower_bound(__first, __last, __v, __comp);
  return __first != __last && !__comp(__v, *__first);
}
template <class _FwdIter, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI bool binary_search(_FwdIter __first, _FwdIter __last, const _Tp& __v) {
  return _CCW_STD::binary_search(__first, __last, __v, less<_Tp>());
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
