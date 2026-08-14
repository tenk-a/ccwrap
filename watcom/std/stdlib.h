// ccwrap watcom: <stdlib.h>. Forward to Open Watcom's native <stdlib.h>, then
// supply what it leaves out: quick_exit / at_quick_exit for C, and for C++ the
// global-namespace names [depr.c.headers] requires (native's C++ branch is a
// using-list that omits the C99/C11 additions).
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(stdlib.h)

#ifdef __cplusplus

#include <cstdlib>
#ifndef _CCW_STDLIB_CXX_USING
#define _CCW_STDLIB_CXX_USING
using std::lldiv_t; using std::llabs; using std::lldiv;
#endif

#else  /* C */

#if _CCW_TARGET_C >= 2011
#include "../../detail/c/stdlib_c11.h"
#endif

#endif  /* __cplusplus */
