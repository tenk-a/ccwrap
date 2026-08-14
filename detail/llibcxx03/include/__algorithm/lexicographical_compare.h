//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_LEXICOGRAPHICAL_COMPARE_H
#define _CCW_LIBCPP___ALGORITHM_LEXICOGRAPHICAL_COMPARE_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2>
_CCW_LIBCPP_HIDE_FROM_ABI bool lexicographical_compare(_It1 __f1, _It1 __l1, _It2 __f2, _It2 __l2) {
    for (; __f2 != __l2; ++__f1, ++__f2) {
        if (__f1 == __l1 || *__f1 < *__f2) return true;
        if (*__f2 < *__f1) return false;
    }
    return false;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
