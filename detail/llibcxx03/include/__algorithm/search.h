/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_SEARCH_H
#define _CCW_LIBCPP___ALGORITHM_SEARCH_H
#include "../__config"

#if !_CCW_LIBCPP_NATIVE_STD

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2>
_CCW_LIBCPP_HIDE_FROM_ABI _It1 search(_It1 __f, _It1 __l, _It2 __sf, _It2 __sl) {
    for (;; ++__f) {
        _It1 __i = __f; _It2 __j = __sf;
        for (;; ++__i, ++__j) { if (__j == __sl) return __f; if (__i == __l) return __l; if (!(*__i == *__j)) break; }
    }
}
template <class _It1, class _It2, class _BinaryPred>
_CCW_LIBCPP_HIDE_FROM_ABI _It1 search(_It1 __f, _It1 __l, _It2 __sf, _It2 __sl, _BinaryPred __p) {
    for (;; ++__f) {
        _It1 __i = __f; _It2 __j = __sf;
        for (;; ++__i, ++__j) { if (__j == __sl) return __f; if (__i == __l) return __l; if (!__p(*__i, *__j)) break; }
    }
}
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_NATIVE_STD
#endif
