// ccwrap <stdarg.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_C_HEADER_PATH(stdarg.h)

#ifndef va_copy
 #define va_copy(dst, src)      ((dst) = (src))
#endif
#ifndef __va_copy
 #define __va_copy(dst, src)    va_copy((dst), (src))
#endif
