//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_DEFAULT_SENTINEL_H
#define _CCW_LIBCPP___ITERATOR_DEFAULT_SENTINEL_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct default_sentinel_t {};
_ccw_inline_const default_sentinel_t default_sentinel = default_sentinel_t();

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___ITERATOR_DEFAULT_SENTINEL_H
