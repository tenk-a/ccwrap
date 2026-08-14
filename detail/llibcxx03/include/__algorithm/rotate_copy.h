/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_ROTATE_COPY_H
#define _CCW_LIBCPP___ALGORITHM_ROTATE_COPY_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIt, class _OutputIt>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt rotate_copy(_FwdIt __first, _FwdIt __mid, _FwdIt __last, _OutputIt __d) {
    for (_FwdIt __i = __mid; __i != __last; ++__i, ++__d) *__d = *__i;
    for (_FwdIt __i = __first; __i != __mid; ++__i, ++__d) *__d = *__i;
    return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
