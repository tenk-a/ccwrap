// ccwrap <ctype.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(ctype.h)

#if defined(_MSC_VER) && _MSC_VER < 1800

#ifndef _CCW_CTYPE_ISBLANK_DEFINED
#define _CCW_CTYPE_ISBLANK_DEFINED
#ifdef __cplusplus
inline int isblank(int __c) { return __c == ' ' || __c == '\t'; }
#else
static __inline int isblank(int __c) { return __c == ' ' || __c == '\t'; }
#endif
#endif  /* _CCW_CTYPE_ISBLANK_DEFINED */

#if defined(_CCW_C_CONFORM) && !defined(__cplusplus)
#undef isprint
#define isprint(c) (isgraph(c) || (c) == ' ')
#endif

#endif
