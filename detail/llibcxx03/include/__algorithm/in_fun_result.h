//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  ranges::in_fun_result -- an algorithm result aggregate.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_IN_FUN_RESULT_H
#define _CCW_LIBCPP___ALGORITHM_IN_FUN_RESULT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _InIter1, class _Func1>
struct in_fun_result {
    _InIter1 in;
    _Func1 fun;

#if !defined(__WATCOMC__)   // Watcom drops this operator (W446 / E375)
    template <class _InIter1_2, class _Func1_2>
    _CCW_LIBCPP_HIDE_FROM_ABI operator in_fun_result<_InIter1_2, _Func1_2>() const {
        in_fun_result<_InIter1_2, _Func1_2> __r = { in, fun };
        return __r;
    }
#endif
};

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
