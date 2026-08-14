//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_UNARY_FUNCTION_H
#define _CCW_LIBCPP___FUNCTIONAL_UNARY_FUNCTION_H
#include "../__config"
#if _CCW_LIBCPP_NATIVE_STD
#  include <functional>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Arg, class _Result>
struct _CCW_LIBCPP_TEMPLATE_VIS unary_function {
    typedef _Arg    argument_type;
    typedef _Result result_type;
};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
