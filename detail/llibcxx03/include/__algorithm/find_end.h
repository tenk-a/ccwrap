/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_FIND_END_H
#define _CCW_LIBCPP___ALGORITHM_FIND_END_H
#include "../__config"

#include "../__algorithm/search.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2>
_CCW_LIBCPP_HIDE_FROM_ABI _It1 find_end(_It1 __f, _It1 __l, _It2 __sf, _It2 __sl) {
    if (__sf == __sl) return __l;
    _It1 __result = __l;
    for (;;) { _It1 __new = _CCW_STD::search(__f, __l, __sf, __sl); if (__new == __l) break; __result = __new; __f = __new; ++__f; }
    return __result;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
