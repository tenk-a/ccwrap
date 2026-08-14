//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_GENERATE_H
#define _CCW_LIBCPP___ALGORITHM_GENERATE_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Gen>
_CCW_LIBCPP_HIDE_FROM_ABI void generate(_It __f, _It __l, _Gen __g) { for (; __f != __l; ++__f) *__f = __g(); }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
