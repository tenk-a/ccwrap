// ccwrap watcom: <stddef.h>. Thin forwarder to Open Watcom's native <stddef.h> (via
// ../h on the INCLUDE path -- size_t, ptrdiff_t, NULL, offsetof), guaranteeing
// ccwrap_common.h is pulled in first.
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(stddef.h)

#ifdef __cplusplus

#include <cstddef>
#ifndef _CCW_STDDEF_CXX_USING
#define _CCW_STDDEF_CXX_USING
using std::max_align_t;
#endif

#else  /* C */

#include "../../detail/c/max_align_t.h"

#endif  /* __cplusplus */
