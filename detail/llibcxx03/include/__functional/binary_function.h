//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_BINARY_FUNCTION_H
#define _CCW_LIBCPP___FUNCTIONAL_BINARY_FUNCTION_H
#include "../__config"
#if _CCW_LIBCPP_NATIVE_STD
#  include <functional>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Arg1, class _Arg2, class _Result>
struct _CCW_LIBCPP_TEMPLATE_VIS binary_function {
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
