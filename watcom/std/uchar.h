// ccwrap <uchar.h>
#pragma once

#include <ccwrap_common.h>

#include <stddef.h>      /* size_t */

#ifndef _CCW_16_32_DEFINED
 #define _CCW_16_32_DEFINED
 typedef _ccw_char16        char16_t;
 typedef _ccw_char32        char32_t;
#endif
#ifndef __CCW_HAS_CHAR8_T
 #define __CCW_HAS_CHAR8_T  1
 typedef _ccw_char8         char8_t;
#endif

#include "../../detail/c/uchar_impl.h"
