//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  Forward declaration of _CCW_STD::allocator (single source, so <iosfwd> and the real
//  headers agree without repeating default template arguments).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_MEMORY_H
#define _CCW_LIBCPP___FWD_MEMORY_H
#include "../__config"
#if _CCW_LIBCPP_NATIVE_STD
#  include <memory>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp> class allocator;
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
