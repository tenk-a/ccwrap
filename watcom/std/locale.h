// ccwrap <locale.h>
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(locale.h)

#ifndef __cplusplus
# ifndef _CCW_LOCALECONV_FIXED
#  define _CCW_LOCALECONV_FIXED
#  include "../../detail/c/lconv_c.h"
#  define localeconv _ccw_localeconv
# endif
#endif
