/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_PARTIAL_SORT_COPY_H
#define _CCW_LIBCPP___ALGORITHM_PARTIAL_SORT_COPY_H
#include "../__config"
#include "../__algorithm/sort.h"
#include "../__functional/operations.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIt, class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI _RandIt partial_sort_copy(_InputIt __f, _InputIt __l, _RandIt __df, _RandIt __dl, _Comp __comp) {
    _RandIt __d = __df;
    for (; __f != __l && __d != __dl; ++__f, ++__d) *__d = *__f;   // copy first (dl-df) elements
    _CCW_STD::sort(__df, __d, __comp);
    if (__d == __dl) {   // more input: keep the smallest (dl-df) via insertion against the max
        for (; __f != __l; ++__f) {
            if (__comp(*__f, *(__d - 1))) {
                _RandIt __pos = __d - 1;
                while (__pos != __df && __comp(*__f, *(__pos - 1))) { *__pos = *(__pos - 1); --__pos; }
                *__pos = *__f;
            }
        }
    }
    return __d;
}
template <class _InputIt, class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI _RandIt partial_sort_copy(_InputIt __f, _InputIt __l, _RandIt __df, _RandIt __dl) {
    return _CCW_STD::partial_sort_copy(__f, __l, __df, __dl, _CCW_STD::less<typename iterator_traits<_RandIt>::value_type>());
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
