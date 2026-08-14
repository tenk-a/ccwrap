/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_UNIQUE_COPY_H
#define _CCW_LIBCPP___ALGORITHM_UNIQUE_COPY_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Out, class _BinPred>
_CCW_LIBCPP_HIDE_FROM_ABI _Out unique_copy(_It __f, _It __l, _Out __d, _BinPred __pred) {
    if (__f == __l) return __d;
    typename iterator_traits<_It>::value_type __prev = *__f; *__d = __prev; ++__d; ++__f;
    for (; __f != __l; ++__f) if (!__pred(*__f, __prev)) { __prev = *__f; *__d = __prev; ++__d; }
    return __d;
}

template <class _It, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _Out unique_copy(_It __f, _It __l, _Out __d) {
    if (__f == __l) return __d;
    typename iterator_traits<_It>::value_type __prev = *__f; *__d = __prev; ++__d; ++__f;
    for (; __f != __l; ++__f) if (!(*__f == __prev)) { __prev = *__f; *__d = __prev; ++__d; }
    return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
