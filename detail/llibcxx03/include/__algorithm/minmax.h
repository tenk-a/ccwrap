//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_MINMAX_H
#define _CCW_LIBCPP___ALGORITHM_MINMAX_H
#include "../__config"
#include "../__utility/pair.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI pair<_Tp, _Tp> minmax(const _Tp& __a, const _Tp& __b) {
    return __b < __a ? pair<_Tp, _Tp>(__b, __a) : pair<_Tp, _Tp>(__a, __b);
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
