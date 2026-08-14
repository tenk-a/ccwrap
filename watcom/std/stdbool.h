// ccwrap watcom: <stdbool.h>. Thin forwarder to Open Watcom's native <stdbool.h> (via
// ../h on the INCLUDE path), guaranteeing ccwrap_common.h is pulled in first. In C it
// defines bool/true/false and __bool_true_false_are_defined; in C++ those are
// keywords.
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(stdbool.h)
