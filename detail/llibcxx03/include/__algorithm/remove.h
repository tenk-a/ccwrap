//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_REMOVE_H
#define _CCW_LIBCPP___ALGORITHM_REMOVE_H
#include "../__config"
#include "../__algorithm/find.h"
#include "../__algorithm/find_if.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIter, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter remove(_FwdIter __first, _FwdIter __last, const _Tp& __v) {
  __first = _CCW_STD::find(__first, __last, __v);
  if (__first != __last) { _FwdIter __i = __first; while (++__i != __last) if (!(*__i == __v)) { *__first = *__i; ++__first; } }
  return __first;
}
template <class _FwdIter, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter remove_if(_FwdIter __first, _FwdIter __last, _Pred __p) {
  __first = _CCW_STD::find_if(__first, __last, __p);
  if (__first != __last) { _FwdIter __i = __first; while (++__i != __last) if (!__p(*__i)) { *__first = *__i; ++__first; } }
  return __first;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
