/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_REPLACE_COPY_H
#define _CCW_LIBCPP___ALGORITHM_REPLACE_COPY_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Out, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Out replace_copy(_It __f, _It __l, _Out __d, const _Tp& __old, const _Tp& __new) { for (; __f != __l; ++__f, ++__d) *__d = (*__f == __old) ? __new : *__f; return __d; }
template <class _It, class _Out, class _Pred, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Out replace_copy_if(_It __f, _It __l, _Out __d, _Pred __p, const _Tp& __new) { for (; __f != __l; ++__f, ++__d) *__d = __p(*__f) ? __new : *__f; return __d; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
