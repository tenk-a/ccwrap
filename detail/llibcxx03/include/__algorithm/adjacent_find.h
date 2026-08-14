//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_ADJACENT_FIND_H
#define _CCW_LIBCPP___ALGORITHM_ADJACENT_FIND_H
#include "../__config"
#include "../__functional/operations.h"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI _It adjacent_find(_It __f, _It __l, _Pred __p) {
    if (__f == __l) return __l;
    _It __n = __f; ++__n;
    for (; __n != __l; ++__f, ++__n) if (__p(*__f, *__n)) return __f;
    return __l;
}
template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI _It adjacent_find(_It __f, _It __l) { return _CCW_STD::adjacent_find(__f, __l, equal_to<typename iterator_traits<_It>::value_type>()); }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
