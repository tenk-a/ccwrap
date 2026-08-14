//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  ranges::in_out_out_result -- an algorithm result aggregate.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_IN_OUT_OUT_RESULT_H
#define _CCW_LIBCPP___ALGORITHM_IN_OUT_OUT_RESULT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _InIter1, class _OutIter1, class _OutIter2>
struct in_out_out_result {
    _InIter1 in;
    _OutIter1 out1;
    _OutIter2 out2;

#if !defined(__WATCOMC__)   // Watcom drops this operator (W446 / E375)
    template <class _InIter1_2, class _OutIter1_2, class _OutIter2_2>
    _CCW_LIBCPP_HIDE_FROM_ABI operator in_out_out_result<_InIter1_2, _OutIter1_2, _OutIter2_2>() const {
        in_out_out_result<_InIter1_2, _OutIter1_2, _OutIter2_2> __r = { in, out1, out2 };
        return __r;
    }
#endif
};

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
