// ccwrap watcom: <fenv.h> (C99 floating-point environment).
//
// Open Watcom ships a perfectly good native <fenv.h>, but it hides its ENTIRE body
// behind a C99 check:
#pragma once

#include <ccwrap_common.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L
#error "ccwrap: <fenv.h> needs C99 mode on Open Watcom -- compile with -za99. The native <fenv.h> gates its whole body on __STDC_VERSION__ >= 199901L, and the default C dialect is 199409L, so without -za99 the header is silently empty. (C++ is fine; use <cfenv>.)"
#endif

#include _CCW_NATIVE_C_HEADER_PATH(fenv.h)
