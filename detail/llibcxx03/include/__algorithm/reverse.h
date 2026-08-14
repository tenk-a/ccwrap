//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_REVERSE_H
#define _CCW_LIBCPP___ALGORITHM_REVERSE_H
#include "../__config"
#include "../__utility/swap.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _BiDirIter> _CCW_LIBCPP_HIDE_FROM_ABI void reverse(_BiDirIter __first, _BiDirIter __last) {
  for (; __first != __last && __first != --__last; ++__first) _CCW_STD::swap(*__first, *__last);
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
