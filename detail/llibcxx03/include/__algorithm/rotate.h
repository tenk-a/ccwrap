//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom. Reverse-based rotate (bidirectional+).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_ROTATE_H
#define _CCW_LIBCPP___ALGORITHM_ROTATE_H
#include "../__config"
#include "../__algorithm/reverse.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _BidIt>
_CCW_LIBCPP_HIDE_FROM_ABI _BidIt rotate(_BidIt __first, _BidIt __middle, _BidIt __last) {
    _CCW_STD::reverse(__first, __middle);
    _CCW_STD::reverse(__middle, __last);
    _CCW_STD::reverse(__first, __last);
    _BidIt __r = __first;
    for (_BidIt __m = __middle; __m != __last; ++__m) ++__r;   // r = first + (last - middle)
    return __r;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
