/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_SEARCH_N_H
#define _CCW_LIBCPP___ALGORITHM_SEARCH_N_H
#include "../__config"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Size, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _It search_n(_It __f, _It __l, _Size __n, const _Tp& __v) {
    if (__n <= 0) return __f;
    for (; __f != __l; ++__f) {
        if (!(*__f == __v)) continue;
        _It __i = __f; _Size __c = 0;
        while (__i != __l && *__i == __v) { ++__c; if (__c == __n) return __f; ++__i; }
        if (__i == __l) break;
        __f = __i;   // skip the scanned run (loop's ++ moves past the mismatch)
    }
    return __l;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
