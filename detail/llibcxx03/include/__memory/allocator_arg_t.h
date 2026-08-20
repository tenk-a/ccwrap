//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_ALLOCATOR_ARG_T_H
#define _CCW_LIBCPP___MEMORY_ALLOCATOR_ARG_T_H
#include "../__config"
#if _CCW_LIBCPP_HAS_NATIVE_MEMORY_TRAITS
#  include <memory>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct allocator_arg_t { _CCW_LIBCPP_HIDE_FROM_ABI explicit allocator_arg_t() {} };
_ccw_inline_const allocator_arg_t allocator_arg = allocator_arg_t();

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_HAS_NATIVE_MEMORY_TRAITS
#endif // _CCW_LIBCPP___MEMORY_ALLOCATOR_ARG_T_H
