//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_SWAP_RANGES_H
#define _CCW_LIBCPP___ALGORITHM_SWAP_RANGES_H
#include "../__config"
#include "../__algorithm/iter_swap.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2>
_CCW_LIBCPP_HIDE_FROM_ABI _It2 swap_ranges(_It1 __f1, _It1 __l1, _It2 __f2) { for (; __f1 != __l1; ++__f1, ++__f2) _CCW_STD::iter_swap(__f1, __f2); return __f2; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
