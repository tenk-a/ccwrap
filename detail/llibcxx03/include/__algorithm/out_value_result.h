//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  ranges::out_value_result -- an algorithm result aggregate.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_OUT_VALUE_RESULT_H
#define _CCW_LIBCPP___ALGORITHM_OUT_VALUE_RESULT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _OutIter1, class _ValT>
struct out_value_result {
    _OutIter1 out;
    _ValT value;

#if !defined(__WATCOMC__)   // Watcom drops this operator (W446 / E375)
    template <class _OutIter1_2, class _ValT_2>
    _CCW_LIBCPP_HIDE_FROM_ABI operator out_value_result<_OutIter1_2, _ValT_2>() const {
        out_value_result<_OutIter1_2, _ValT_2> __r = { out, value };
        return __r;
    }
#endif
};

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
