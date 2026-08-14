/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_REMOVE_COPY_H
#define _CCW_LIBCPP___ALGORITHM_REMOVE_COPY_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Out, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Out remove_copy(_It __f, _It __l, _Out __d, const _Tp& __v) { for (; __f != __l; ++__f) if (!(*__f == __v)) { *__d = *__f; ++__d; } return __d; }
template <class _It, class _Out, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI _Out remove_copy_if(_It __f, _It __l, _Out __d, _Pred __p) { for (; __f != __l; ++__f) if (!__p(*__f)) { *__d = *__f; ++__d; } return __d; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
