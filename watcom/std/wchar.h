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
