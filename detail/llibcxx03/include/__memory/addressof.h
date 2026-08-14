//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_ADDRESSOF_H
#define _CCW_LIBCPP___MEMORY_ADDRESSOF_H
#include "../__config"

#if _CCW_LIBCPP_NATIVE_STD
#  include <memory>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp* addressof(_Tp& __x) {
    return reinterpret_cast<_Tp*>(
        &const_cast<char&>(reinterpret_cast<const volatile char&>(__x)));
}
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
