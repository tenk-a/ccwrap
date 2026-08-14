//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  ranges::in_found_result -- an algorithm result aggregate.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_IN_FOUND_RESULT_H
#define _CCW_LIBCPP___ALGORITHM_IN_FOUND_RESULT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _InIter1>
struct in_found_result {
    _InIter1 in;
    bool found;

#if !defined(__WATCOMC__)   // Watcom drops this operator (W446 / E375)
    template <class _InIter1_2>
    _CCW_LIBCPP_HIDE_FROM_ABI operator in_found_result<_InIter1_2>() const {
        in_found_result<_InIter1_2> __r = { in, found };
        return __r;
    }
#endif
};

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
