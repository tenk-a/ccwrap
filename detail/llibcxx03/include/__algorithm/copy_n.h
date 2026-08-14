//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_COPY_N_H
#define _CCW_LIBCPP___ALGORITHM_COPY_N_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIt, class _Size, class _OutputIt>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt copy_n(_InputIt __f, _Size __n, _OutputIt __d) {
    for (; __n > 0; --__n, ++__f, ++__d) *__d = *__f;
    return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
