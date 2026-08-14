//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  ranges::in_in_out_result -- an algorithm result aggregate.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_IN_IN_OUT_RESULT_H
#define _CCW_LIBCPP___ALGORITHM_IN_IN_OUT_RESULT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _InIter1, class _InIter2, class _OutIter1>
struct in_in_out_result {
    _InIter1 in1;
    _InIter2 in2;
    _OutIter1 out;

#if !defined(__WATCOMC__)   // Watcom drops this operator (W446 / E375)
    template <class _InIter1_2, class _InIter2_2, class _OutIter1_2>
    _CCW_LIBCPP_HIDE_FROM_ABI operator in_in_out_result<_InIter1_2, _InIter2_2, _OutIter1_2>() const {
        in_in_out_result<_InIter1_2, _InIter2_2, _OutIter1_2> __r = { in1, in2, out };
        return __r;
    }
#endif
};

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
