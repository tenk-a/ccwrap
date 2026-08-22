// ccwrap watcom: <wchar.h>. Forward to Open Watcom's native <wchar.h>, then add the
// global-namespace names [depr.c.headers] requires. Native's C++ branch is a
// using-list drawn from <cwchar> and it omits the restartable-conversion entries.
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(wchar.h)

#ifdef __cplusplus

#include <cwchar>
#ifndef _CCW_WCHAR_CXX_USING
#define _CCW_WCHAR_CXX_USING
using std::btowc; using std::mbsinit; using std::fwide;
#endif

#endif  /* __cplusplus */

#include "../../detail/c/mbconv_wide_impl.h"

#if defined(_CCW_MB_HAS_CONV) && !defined(_CCW_MB_WCHAR_TAKEOVER)
#define _CCW_MB_WCHAR_TAKEOVER
#ifdef __cplusplus
namespace std {
using ::_ccw_mbsinit;   using ::_ccw_mbrlen;    using ::_ccw_mbrtowc; using ::_ccw_wcrtomb;
using ::_ccw_mbsrtowcs; using ::_ccw_wcsrtombs; using ::_ccw_btowc;   using ::_ccw_wctob;
}
#endif
#define mbsinit     _ccw_mbsinit
#define mbrlen      _ccw_mbrlen
#define mbrtowc     _ccw_mbrtowc
#define wcrtomb     _ccw_wcrtomb
#define mbsrtowcs   _ccw_mbsrtowcs
#define wcsrtombs   _ccw_wcsrtombs
#define btowc       _ccw_btowc
#define wctob       _ccw_wctob
#endif
