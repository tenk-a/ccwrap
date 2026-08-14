//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  ranges::min_max_result -- an algorithm result aggregate.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_MIN_MAX_RESULT_H
#define _CCW_LIBCPP___ALGORITHM_MIN_MAX_RESULT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _Tp>
struct min_max_result {
    _Tp min;
    _Tp max;

#if !defined(__WATCOMC__)   // Watcom drops this operator (W446 / E375)
    template <class _Tp_2>
    _CCW_LIBCPP_HIDE_FROM_ABI operator min_max_result<_Tp_2>() const {
        min_max_result<_Tp_2> __r = { min, max };
        return __r;
    }
#endif
};

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
