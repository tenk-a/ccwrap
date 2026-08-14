//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_COPY_BACKWARD_H
#define _CCW_LIBCPP___ALGORITHM_COPY_BACKWARD_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _BidIt1, class _BidIt2>
_CCW_LIBCPP_HIDE_FROM_ABI _BidIt2 copy_backward(_BidIt1 __f, _BidIt1 __l, _BidIt2 __dl) { while (__f != __l) { --__l; --__dl; *__dl = *__l; } return __dl; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
