//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_NEXT_PERMUTATION_H
#define _CCW_LIBCPP___ALGORITHM_NEXT_PERMUTATION_H
#include "../__config"
#include "../__algorithm/iter_swap.h"
#include "../__algorithm/reverse.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Bi, class _Compare>
_CCW_LIBCPP_HIDE_FROM_ABI bool next_permutation(_Bi __first, _Bi __last, _Compare __c) {
    if (__first == __last) return false;
    _Bi __i = __last; if (__first == --__i) return false;
    for (;;) {
        _Bi __i1 = __i;
        if (__c(*--__i, *__i1)) {
            _Bi __j = __last;
            while (!__c(*__i, *--__j)) {}
            _CCW_STD::iter_swap(__i, __j);
            _CCW_STD::reverse(__i1, __last);
            return true;
        }
        if (__i == __first) { _CCW_STD::reverse(__first, __last); return false; }
    }
}
template <class _Bi>
_CCW_LIBCPP_HIDE_FROM_ABI bool next_permutation(_Bi __first, _Bi __last) {
    if (__first == __last) return false;
    _Bi __i = __last; if (__first == --__i) return false;
    for (;;) {
        _Bi __i1 = __i;
        if (*--__i < *__i1) {
            _Bi __j = __last;
            while (!(*__i < *--__j)) {}
            _CCW_STD::iter_swap(__i, __j);
            _CCW_STD::reverse(__i1, __last);
            return true;
        }
        if (__i == __first) { _CCW_STD::reverse(__first, __last); return false; }
    }
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
