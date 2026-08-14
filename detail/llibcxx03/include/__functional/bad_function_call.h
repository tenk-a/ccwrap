//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_BAD_FUNCTION_CALL_H
#define _CCW_LIBCPP___FUNCTIONAL_BAD_FUNCTION_CALL_H
#include "../__config"
#include <exception>
#include "../__exception/eh_anchor.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

class bad_function_call : public exception {
public:
    _CCW_LIBCPP_HIDE_FROM_ABI bad_function_call() _CCW_LIBCPP_NOEXCEPT {}
    virtual ~bad_function_call() _CCW_LIBCPP_NOEXCEPT {}
    virtual const char* what() const _CCW_LIBCPP_NOEXCEPT { return "std::bad_function_call"; }
};

_CCW_LIBCPP_END_NAMESPACE_STD
_CCW_EH_ANCHOR(ebfc, bad_function_call, bad_function_call)

#endif // _CCW_LIBCPP___FUNCTIONAL_BAD_FUNCTION_CALL_H
