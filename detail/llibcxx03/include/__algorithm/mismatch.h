/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_MISMATCH_H
#define _CCW_LIBCPP___ALGORITHM_MISMATCH_H
#include "../__config"
#include "../__utility/pair.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2>
_CCW_LIBCPP_HIDE_FROM_ABI pair<_It1, _It2> mismatch(_It1 __f1, _It1 __l1, _It2 __f2) {
    for (; __f1 != __l1 && *__f1 == *__f2; ++__f1, ++__f2) {}
    return pair<_It1, _It2>(__f1, __f2);
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
