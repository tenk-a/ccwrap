//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_ITER_SWAP_H
#define _CCW_LIBCPP___ALGORITHM_ITER_SWAP_H
#include "../__config"
#include "../__utility/swap.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2>
_CCW_LIBCPP_HIDE_FROM_ABI void iter_swap(_It1 __a, _It2 __b) { _CCW_STD::swap(*__a, *__b); }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
