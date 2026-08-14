/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_PARTITION_H
#define _CCW_LIBCPP___ALGORITHM_PARTITION_H
#include "../__config"

#include "../__algorithm/iter_swap.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI _It partition(_It __f, _It __l, _Pred __p) {
    while (__f != __l) { while (__p(*__f)) { ++__f; if (__f == __l) return __f; } do { --__l; if (__f == __l) return __f; } while (!__p(*__l)); _CCW_STD::iter_swap(__f, __l); ++__f; }
    return __f;
}
template <class _It, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI bool is_partitioned(_It __f, _It __l, _Pred __p) { for (; __f != __l && __p(*__f); ++__f) {} for (; __f != __l; ++__f) if (__p(*__f)) return false; return true; }
template <class _It, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI _It partition_point(_It __f, _It __l, _Pred __p) { for (; __f != __l && __p(*__f); ++__f) {} return __f; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
