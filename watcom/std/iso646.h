// ccwrap watcom: <iso646.h>. Thin forwarder to Open Watcom's native <iso646.h> (via
// ../h on the INCLUDE path), guaranteeing ccwrap_common.h is pulled in first. In C it
// defines the alternative operator macros (and, or, not, ...); in C++ those are
// keywords and the native header is a no-op.
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(iso646.h)
