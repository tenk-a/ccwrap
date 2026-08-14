//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_MINMAX_ELEMENT_H
#define _CCW_LIBCPP___ALGORITHM_MINMAX_ELEMENT_H
#include "../__config"
#include "../__utility/pair.h"
#include "../__functional/operations.h"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI pair<_It, _It> minmax_element(_It __f, _It __l, _Comp __comp) {
    _It __mn = __f, __mx = __f;
    if (__f == __l) return pair<_It, _It>(__mn, __mx);
    for (++__f; __f != __l; ++__f) {
        if (__comp(*__f, *__mn)) __mn = __f;
        if (!__comp(*__f, *__mx)) __mx = __f;   // last max on ties (per standard)
    }
    return pair<_It, _It>(__mn, __mx);
}
template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI pair<_It, _It> minmax_element(_It __f, _It __l) { return _CCW_STD::minmax_element(__f, __l, less<typename iterator_traits<_It>::value_type>()); }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
