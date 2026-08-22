// ccwrap watcom: <stdlib.h>. Forward to Open Watcom's native <stdlib.h>, then
// supply what it leaves out: quick_exit / at_quick_exit for C, and for C++ the
// global-namespace names [depr.c.headers] requires (native's C++ branch is a
// using-list that omits the C99/C11 additions).
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(stdlib.h)

#ifdef __cplusplus

#include <cstdlib>
#ifndef _CCW_STDLIB_CXX_USING
#define _CCW_STDLIB_CXX_USING
using std::lldiv_t; using std::llabs; using std::lldiv;
#endif

#else  /* C */

#if _CCW_TARGET_C >= 2011
#include "../../detail/c/stdlib_c11.h"
#endif

#endif  /* __cplusplus */

#include "../../detail/c/mbconv_impl.h"

#if defined(_CCW_MB_HAS_CONV) && !defined(_CCW_MB_STDLIB_TAKEOVER)
#define _CCW_MB_STDLIB_TAKEOVER
#ifdef __cplusplus
namespace std {
using ::_ccw_mb_cur_max; using ::_ccw_mblen;    using ::_ccw_mbtowc;
using ::_ccw_wctomb;     using ::_ccw_mbstowcs; using ::_ccw_wcstombs;
}
#endif
#undef  MB_CUR_MAX
#define MB_CUR_MAX  ((size_t)_ccw_mb_cur_max())
#define mblen       _ccw_mblen
#define mbtowc      _ccw_mbtowc
#define wctomb      _ccw_wctomb
#define mbstowcs    _ccw_mbstowcs
#define wcstombs    _ccw_wcstombs
#endif
