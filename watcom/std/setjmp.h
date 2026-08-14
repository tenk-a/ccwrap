// ccwrap watcom: <setjmp.h>. Thin forwarder to Open Watcom's native <setjmp.h> (via
// ../h on the INCLUDE path), guaranteeing ccwrap_common.h is pulled in first.
#pragma once

#include <ccwrap_common.h>
#ifndef setjmp
#include _CCW_NATIVE_C_HEADER_PATH(setjmp.h)
#endif
