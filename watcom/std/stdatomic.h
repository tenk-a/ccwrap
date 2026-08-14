// ccwrap watcom forwarder: pull ccwrap's <stdatomic.h> from detail/llibcxx03/include so that
// putting only watcom/ on the include path overrides Open Watcom's native header.
//
// C++ only. The implementation in llibcxx03 is C++ (namespace std), so including this
// from C fails loudly and specifically here rather than spraying unrelated errors out
// of the native <cstddef>.
#pragma once
#ifndef __cplusplus
#error "ccwrap: watcom <stdatomic.h> is C++ only. C11 atomics need the _Atomic specifier, which Open Watcom lacks. Use <atomic> from C++."
#endif
#include <ccwrap_common.h>
#include _CCW_NATIVE_STL_HEADER_PATH(stdatomic.h)
