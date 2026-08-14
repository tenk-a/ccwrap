// ccwrap watcom: <limits.h>. Forward to Open Watcom's native <limits.h> (via ../h on
// the INCLUDE path), then add the C23 *_WIDTH macros and the other C23 additions that
// it predates.
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(limits.h)

#include "../../detail/c/limits_c23.h"
