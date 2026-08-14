// ccwrap watcom: <stdint.h>. Forward to Open Watcom's native <stdint.h> (via ../h on
// the INCLUDE path), then add the C23 *_WIDTH macros / __STDC_VERSION_STDINT_H__ that
// it predates.
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(stdint.h)

#ifdef __cplusplus
#include <cstdint>
#endif

#include "../../detail/c/stdint_width.h"
