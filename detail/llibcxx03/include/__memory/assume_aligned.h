//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_ASSUME_ALIGNED_H
#define _CCW_LIBCPP___MEMORY_ASSUME_ALIGNED_H
#include "../__config"
#include <cstddef>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <_CCW_STD::size_t _Np, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp* assume_aligned(_Tp* __p) { return __p; }

template <_CCW_STD::size_t _Alignment, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI bool is_sufficiently_aligned(_Tp* __p) {
    return (reinterpret_cast<_CCW_STD::size_t>(__p) % _Alignment) == 0;
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___MEMORY_ASSUME_ALIGNED_H
