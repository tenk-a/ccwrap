// ccwrap watcom: <wctype.h>. Forward to Open Watcom's native <wctype.h>, then add
// the global-namespace names [depr.c.headers] requires. Native's C++ branch is a
// using-list drawn from <cwctype> and it omits the wctrans entries.
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(wctype.h)

#ifdef __cplusplus

#include <cwctype>
#ifndef _CCW_WCTYPE_CXX_USING
#define _CCW_WCTYPE_CXX_USING
using std::wctrans_t; using std::wctrans; using std::towctrans;
#endif

#endif  /* __cplusplus */
