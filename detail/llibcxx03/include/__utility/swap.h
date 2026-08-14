//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom (no rvalue refs; move is emulated).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___UTILITY_SWAP_H
#define _CCW_LIBCPP___UTILITY_SWAP_H
#include "../__config"
#include "../__utility/move.h"
#if _CCW_LIBCPP_NATIVE_STD
#  include <utility>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI void swap(_Tp& __a, _Tp& __b) {
    _Tp __tmp = ::_ccw::move(__a);
    __a = ::_ccw::move(__b);
    __b = ::_ccw::move(__tmp);
}
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
