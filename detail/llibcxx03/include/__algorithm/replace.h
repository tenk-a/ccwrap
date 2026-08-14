/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_REPLACE_H
#define _CCW_LIBCPP___ALGORITHM_REPLACE_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIter, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI void replace(_FwdIter __first, _FwdIter __last, const _Tp& __old, const _Tp& __new) {
  for (; __first != __last; ++__first) if (*__first == __old) *__first = __new;
}
template <class _FwdIter, class _Pred, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI void replace_if(_FwdIter __first, _FwdIter __last, _Pred __p, const _Tp& __new) {
  for (; __first != __last; ++__first) if (__p(*__first)) *__first = __new;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
