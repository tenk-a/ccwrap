/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_SET_INTERSECTION_H
#define _CCW_LIBCPP___ALGORITHM_SET_INTERSECTION_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2, class _Out, class _Compare>
_CCW_LIBCPP_HIDE_FROM_ABI _Out set_intersection(_It1 __f1, _It1 __l1, _It2 __f2, _It2 __l2, _Out __d, _Compare __c) {
    while (__f1 != __l1 && __f2 != __l2) {
        if (__c(*__f1, *__f2)) ++__f1;
        else if (__c(*__f2, *__f1)) ++__f2;
        else { *__d = *__f1; ++__d; ++__f1; ++__f2; }
    }
    return __d;
}

template <class _It1, class _It2, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _Out set_intersection(_It1 __f1, _It1 __l1, _It2 __f2, _It2 __l2, _Out __d) {
    while (__f1 != __l1 && __f2 != __l2) {
        if (*__f1 < *__f2) ++__f1;
        else if (*__f2 < *__f1) ++__f2;
        else { *__d = *__f1; ++__d; ++__f1; ++__f2; }
    }
    return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
