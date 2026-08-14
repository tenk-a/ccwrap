//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_GENERATE_N_H
#define _CCW_LIBCPP___ALGORITHM_GENERATE_N_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Size, class _Gen>
_CCW_LIBCPP_HIDE_FROM_ABI _It generate_n(_It __f, _Size __n, _Gen __g) { for (; __n > 0; --__n, ++__f) *__f = __g(); return __f; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
