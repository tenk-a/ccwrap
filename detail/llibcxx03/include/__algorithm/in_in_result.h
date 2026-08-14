//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  ranges::in_in_result -- an algorithm result aggregate.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_IN_IN_RESULT_H
#define _CCW_LIBCPP___ALGORITHM_IN_IN_RESULT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _InIter1, class _InIter2>
struct in_in_result {
    _InIter1 in1;
    _InIter2 in2;

#if !defined(__WATCOMC__)   // Watcom drops this operator (W446 / E375)
    template <class _InIter1_2, class _InIter2_2>
    _CCW_LIBCPP_HIDE_FROM_ABI operator in_in_result<_InIter1_2, _InIter2_2>() const {
        in_in_result<_InIter1_2, _InIter2_2> __r = { in1, in2 };
        return __r;
    }
#endif
};

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
