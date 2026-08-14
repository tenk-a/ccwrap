//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_FIND_FIRST_OF_H
#define _CCW_LIBCPP___ALGORITHM_FIND_FIRST_OF_H
#include "../__config"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2>
_CCW_LIBCPP_HIDE_FROM_ABI _It1 find_first_of(_It1 __f, _It1 __l, _It2 __sf, _It2 __sl) {
    for (; __f != __l; ++__f) for (_It2 __j = __sf; __j != __sl; ++__j) if (*__f == *__j) return __f;
    return __l;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
