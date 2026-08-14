//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_REVERSE_COPY_H
#define _CCW_LIBCPP___ALGORITHM_REVERSE_COPY_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _BidIt, class _OutIt>
_CCW_LIBCPP_HIDE_FROM_ABI _OutIt reverse_copy(_BidIt __f, _BidIt __l, _OutIt __d) { for (; __f != __l; ) { --__l; *__d = *__l; ++__d; } return __d; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
