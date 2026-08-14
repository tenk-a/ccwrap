//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_ALL_OF_H
#define _CCW_LIBCPP___ALGORITHM_ALL_OF_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI bool all_of(_It __f, _It __l, _Pred __p) { for (; __f != __l; ++__f) if (!__p(*__f)) return false; return true; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
