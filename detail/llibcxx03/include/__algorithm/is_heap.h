/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_IS_HEAP_H
#define _CCW_LIBCPP___ALGORITHM_IS_HEAP_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI _RandIt is_heap_until(_RandIt __first, _RandIt __last, _Comp __comp) {
    typename iterator_traits<_RandIt>::difference_type __n = __last - __first;
    for (typename iterator_traits<_RandIt>::difference_type __c = 1; __c < __n; ++__c)
        if (__comp(__first[(__c - 1) / 2], __first[__c])) return __first + __c;
    return __last;
}
template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI _RandIt is_heap_until(_RandIt __first, _RandIt __last) {
    typename iterator_traits<_RandIt>::difference_type __n = __last - __first;
    for (typename iterator_traits<_RandIt>::difference_type __c = 1; __c < __n; ++__c)
        if (__first[(__c - 1) / 2] < __first[__c]) return __first + __c;
    return __last;
}
template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI bool is_heap(_RandIt __first, _RandIt __last, _Comp __comp) { return _CCW_STD::is_heap_until(__first, __last, __comp) == __last; }
template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI bool is_heap(_RandIt __first, _RandIt __last) { return _CCW_STD::is_heap_until(__first, __last) == __last; }

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
