//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_ALLOCATE_AT_LEAST_H
#define _CCW_LIBCPP___MEMORY_ALLOCATE_AT_LEAST_H
#include "../__config"
#include <cstddef>
#if !_CCW_LIBCPP_NATIVE_STD

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Pointer, class _SizeType = _CCW_STD::size_t>
struct allocation_result {
    _Pointer  ptr;
    _SizeType count;
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // !_CCW_LIBCPP_NATIVE_STD

#endif // _CCW_LIBCPP___MEMORY_ALLOCATE_AT_LEAST_H
