//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_ARITH_CONSTRAINT_H
#define _CCW_LIBCPP___NUMERIC_ARITH_CONSTRAINT_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
struct __ccw_not_bool {
    typedef _Tp type;
};

template <>
struct __ccw_not_bool<bool> {
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___NUMERIC_ARITH_CONSTRAINT_H
