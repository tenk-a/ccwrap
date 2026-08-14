// ccwrap watcom: <float.h>. Forward to Open Watcom's own <float.h> (via ../h on the
// INCLUDE path), then add the C11 / C23 floating-point characteristic macros it
// predates: FLT_DECIMAL_DIG, FLT_EVAL_METHOD, *_HAS_SUBNORM, *_NORM_MAX and the
// *_IS_IEC_60559 flags.
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(float.h)

#include "../../detail/c/float_c11.h"
