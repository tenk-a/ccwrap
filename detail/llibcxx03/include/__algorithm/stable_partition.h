//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_STABLE_PARTITION_H
#define _CCW_LIBCPP___ALGORITHM_STABLE_PARTITION_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include <vector>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _BidIt, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI _BidIt stable_partition(_BidIt __first, _BidIt __last, _Pred __pred) {
    typedef typename iterator_traits<_BidIt>::value_type _Vp;
    _CCW_STD::vector<_Vp> __falses;
    _BidIt __result = __first;
    for (_BidIt __it = __first; __it != __last; ++__it) {
        if (__pred(*__it)) { *__result = *__it; ++__result; }
        else __falses.push_back(*__it);
    }
    _BidIt __point = __result;
    for (_CCW_STD::size_t __k = 0; __k < __falses.size(); ++__k, ++__result) *__result = __falses[__k];
    return __point;
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
