// ccwrap <locale.h>
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(locale.h)

// C++ goes through <clocale>, which the native <locale.h> includes for us.
#ifndef __cplusplus
# ifndef _CCW_LOCALECONV_FIXED
#  define _CCW_LOCALECONV_FIXED
#  include "../../detail/c/lconv_c.h"
#  define localeconv _ccw_localeconv
# endif
#endif
