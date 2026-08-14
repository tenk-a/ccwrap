//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_EQUAL_RANGE_H
#define _CCW_LIBCPP___ALGORITHM_EQUAL_RANGE_H
#include "../__config"
#include "../__algorithm/lower_bound.h"
#include "../__algorithm/upper_bound.h"
#include "../__utility/pair.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _FwdIt, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI pair<_FwdIt, _FwdIt> equal_range(_FwdIt __first, _FwdIt __last, const _Tp& __v) {
    return pair<_FwdIt, _FwdIt>(_CCW_STD::lower_bound(__first, __last, __v), _CCW_STD::upper_bound(__first, __last, __v));
}
template <class _FwdIt, class _Tp, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI pair<_FwdIt, _FwdIt> equal_range(_FwdIt __first, _FwdIt __last, const _Tp& __v, _Comp __comp) {
    return pair<_FwdIt, _FwdIt>(_CCW_STD::lower_bound(__first, __last, __v, __comp), _CCW_STD::upper_bound(__first, __last, __v, __comp));
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
