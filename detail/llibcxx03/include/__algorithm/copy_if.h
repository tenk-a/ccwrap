//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_COPY_IF_H
#define _CCW_LIBCPP___ALGORITHM_COPY_IF_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Out, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI _Out copy_if(_It __f, _It __l, _Out __d, _Pred __p) { for (; __f != __l; ++__f) if (__p(*__f)) { *__d = *__f; ++__d; } return __d; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
