/**
 *  @file   detail/limits_c23.h
 *  @brief  The C23 / C++20 additions to <limits.h> (for Open Watcom).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_LIMITS_C23_H___
#define _CCW_DETAIL_LIMITS_C23_H___

#ifndef __STDC_VERSION_LIMITS_H__
#define __STDC_VERSION_LIMITS_H__ 202311L
#endif

#ifndef BOOL_WIDTH
#define BOOL_WIDTH   1
#endif
#ifndef CHAR_WIDTH
#define CHAR_WIDTH   CHAR_BIT
#endif
#ifndef SCHAR_WIDTH
#define SCHAR_WIDTH  CHAR_BIT
#endif
#ifndef UCHAR_WIDTH
#define UCHAR_WIDTH  CHAR_BIT
#endif
#ifndef SHRT_WIDTH
#define SHRT_WIDTH   16
#endif
#ifndef USHRT_WIDTH
#define USHRT_WIDTH  16
#endif
#ifndef INT_WIDTH
#if INT_MAX == 32767
#define INT_WIDTH    16
#else
#define INT_WIDTH    32
#endif
#endif
#ifndef UINT_WIDTH
#define UINT_WIDTH   INT_WIDTH
#endif
#ifndef LONG_WIDTH
#define LONG_WIDTH   32
#endif
#ifndef ULONG_WIDTH
#define ULONG_WIDTH  32
#endif
#ifndef LLONG_WIDTH
#define LLONG_WIDTH  64
#endif
#ifndef ULLONG_WIDTH
#define ULLONG_WIDTH 64
#endif

#ifndef WCHAR_MIN
#define WCHAR_MIN 0
#endif
#ifndef WCHAR_MAX
#define WCHAR_MAX 65535U
#endif
#ifndef WCHAR_WIDTH
#define WCHAR_WIDTH 16
#endif
#ifndef WINT_MIN
#define WINT_MIN WCHAR_MIN
#endif
#ifndef WINT_MAX
#define WINT_MAX WCHAR_MAX
#endif
#ifndef WINT_WIDTH
#define WINT_WIDTH WCHAR_WIDTH
#endif

#endif /* _CCW_DETAIL_LIMITS_C23_H___ */
