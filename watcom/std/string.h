// ccwrap <string.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_C_HEADER_PATH(string.h)

#ifdef __cplusplus

#include <cstring>
#ifndef _CCW_STRING_STRNLEN_USING
#define _CCW_STRING_STRNLEN_USING
using std::strnlen;
#endif

#else  /* C */

#ifndef _CCW_STRING_STRNLEN_DEFINED
#define _CCW_STRING_STRNLEN_DEFINED
static _ccw_forceinline size_t strnlen(const char* s, size_t n) {
    size_t i = 0;
    while (i < n && s[i]) ++i;
    return i;
}
#endif  /* _CCW_STRING_STRNLEN_DEFINED */

#endif  /* __cplusplus */

#define _CCW_WCRT_WANT_WCSTOK
#include "../../detail/c/win/wide_crt_vc.h"
