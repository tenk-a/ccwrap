//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_NTH_ELEMENT_H
#define _CCW_LIBCPP___ALGORITHM_NTH_ELEMENT_H
#include "../__config"
#include "../__algorithm/sort.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _RandIt, class _Compare>
_CCW_LIBCPP_HIDE_FROM_ABI void nth_element(_RandIt __first, _RandIt, _RandIt __last, _Compare __c) { _CCW_STD::sort(__first, __last, __c); }

template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI void nth_element(_RandIt __first, _RandIt, _RandIt __last) { _CCW_STD::sort(__first, __last); }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
