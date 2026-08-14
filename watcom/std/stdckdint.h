// ccwrap watcom forwarder: pull the genuine C++ <stdckdint.h> from detail/llibcxx03/include so that
// putting only watcom/ on the include path overrides Open Watcom's native header.
//
// C++ gets the libcxx03 function templates (namespace std); C gets the C23 macros from
// ../../detail/c/stdckdint_c.h, shared with vc. The two cannot coexist in one
// translation unit: a function-like macro named ckd_add would rewrite
// `std::ckd_add(...)` too.
#pragma once

#include <ccwrap_common.h>

#ifdef __cplusplus
#include _CCW_NATIVE_STL_HEADER_PATH(stdckdint.h)
#else
#include "../../detail/c/stdckdint_c.h"
#endif
