/**
 *  @file   watcom/std/ccw_autolink.h
 *  @brief  Names the prebuilt ccw library that matches the compiler options,
 *          via #pragma library. Included from <ccwrap_common.h> in C++ mode.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_CCW_AUTOLINK_H
#define _CCW_CCW_AUTOLINK_H
#pragma once

#include "ccwrap_header.h"

#if defined(__cplusplus) && !defined(_CCW_NO_AUTO_LINK) && !defined(_CCW_LIB_NAME)

#if !defined(_CPPUNWIND) || defined(__SW_XD) || defined(__SW_XDT) || defined(__SW_XDS)
# if defined(_CPPRTTI)
#  define _CCW_LIB_EH_ID        2       // -xr
# else
#  define _CCW_LIB_EH_ID        1       // -ne
# endif
#elif defined(__SW_XST)
# if defined(_CPPRTTI)
#  define _CCW_LIB_EH_ID        0       // -eh
# else
#  define _CCW_LIB_EH_ID        3       // -xst
# endif
#elif defined(__SW_XS) || defined(__SW_XSS)
# if defined(_CPPRTTI)
#  define _CCW_LIB_EH_ID        5       // -xrxs
# else
#  define _CCW_LIB_EH_ID        4       // -xs
# endif
#else
# if defined(_CPPRTTI)
#  define _CCW_LIB_EH_ID        0       // -eh
# else
#  define _CCW_LIB_EH_ID        3       // -xst
# endif
#endif

#if defined(__SW_3S) || defined(__SW_4S) || defined(__SW_5S) || defined(__SW_6S)
# define _CCW_LIB_ABI_ID        1       // 3s
#else
# define _CCW_LIB_ABI_ID        0       // 3r
#endif

#define _CCW_LIB_ID             (_CCW_LIB_EH_ID * 2 + _CCW_LIB_ABI_ID)

#if defined(_CCW_OS_DOS) && defined(__386__)
# if defined(__WATCOM_LFN__)
#  define _CCW_LIB_MOD_ID       1       // -lfn
# else
#  define _CCW_LIB_MOD_ID       0
# endif
#elif defined(_CCW_OS_WIN) && defined(__386__)
# if defined(_CCW_FS_WCHAR) && !_CCW_FS_WCHAR
#  define _CCW_LIB_MOD_ID       2       // -fsc
# else
#  define _CCW_LIB_MOD_ID       0
# endif
#endif

#if defined(_CCW_LIB_MOD_ID) && !defined(_CCW_NS_CCWSTD)
# if  _CCW_LIB_MOD_ID == 0
#  if   _CCW_LIB_ID == 0
#   define _CCW_LIB_NAME    "ccw-eh3r"
#  elif _CCW_LIB_ID == 1
#   define _CCW_LIB_NAME    "ccw-eh3s"
#  elif _CCW_LIB_ID == 2
#   define _CCW_LIB_NAME    "ccw-ne3r"
#  elif _CCW_LIB_ID == 3
#   define _CCW_LIB_NAME    "ccw-ne3s"
#  elif _CCW_LIB_ID == 4
#   define _CCW_LIB_NAME    "ccw-xr3r"
#  elif _CCW_LIB_ID == 5
#   define _CCW_LIB_NAME    "ccw-xr3s"
#  elif _CCW_LIB_ID == 6
#   define _CCW_LIB_NAME    "ccw-xst3r"
#  elif _CCW_LIB_ID == 7
#   define _CCW_LIB_NAME    "ccw-xst3s"
#  elif _CCW_LIB_ID == 8
#   define _CCW_LIB_NAME    "ccw-xs3r"
#  elif _CCW_LIB_ID == 9
#   define _CCW_LIB_NAME    "ccw-xs3s"
#  elif _CCW_LIB_ID == 10
#   define _CCW_LIB_NAME    "ccw-xrxs3r"
#  elif _CCW_LIB_ID == 11
#   define _CCW_LIB_NAME    "ccw-xrxs3s"
#  endif
# elif _CCW_LIB_MOD_ID == 1
#  if   _CCW_LIB_ID == 0
#   define _CCW_LIB_NAME    "ccw-lfn-eh3r"
#  elif _CCW_LIB_ID == 1
#   define _CCW_LIB_NAME    "ccw-lfn-eh3s"
#  elif _CCW_LIB_ID == 2
#   define _CCW_LIB_NAME    "ccw-lfn-ne3r"
#  elif _CCW_LIB_ID == 3
#   define _CCW_LIB_NAME    "ccw-lfn-ne3s"
#  elif _CCW_LIB_ID == 4
#   define _CCW_LIB_NAME    "ccw-lfn-xr3r"
#  elif _CCW_LIB_ID == 5
#   define _CCW_LIB_NAME    "ccw-lfn-xr3s"
#  elif _CCW_LIB_ID == 6
#   define _CCW_LIB_NAME    "ccw-lfn-xst3r"
#  elif _CCW_LIB_ID == 7
#   define _CCW_LIB_NAME    "ccw-lfn-xst3s"
#  elif _CCW_LIB_ID == 8
#   define _CCW_LIB_NAME    "ccw-lfn-xs3r"
#  elif _CCW_LIB_ID == 9
#   define _CCW_LIB_NAME    "ccw-lfn-xs3s"
#  elif _CCW_LIB_ID == 10
#   define _CCW_LIB_NAME    "ccw-lfn-xrxs3r"
#  elif _CCW_LIB_ID == 11
#   define _CCW_LIB_NAME    "ccw-lfn-xrxs3s"
#  endif
# elif _CCW_LIB_MOD_ID == 2
#  if   _CCW_LIB_ID == 0
#   define _CCW_LIB_NAME    "ccw-fsc-eh3r"
#  elif _CCW_LIB_ID == 1
#   define _CCW_LIB_NAME    "ccw-fsc-eh3s"
#  elif _CCW_LIB_ID == 2
#   define _CCW_LIB_NAME    "ccw-fsc-ne3r"
#  elif _CCW_LIB_ID == 3
#   define _CCW_LIB_NAME    "ccw-fsc-ne3s"
#  elif _CCW_LIB_ID == 4
#   define _CCW_LIB_NAME    "ccw-fsc-xr3r"
#  elif _CCW_LIB_ID == 5
#   define _CCW_LIB_NAME    "ccw-fsc-xr3s"
#  elif _CCW_LIB_ID == 6
#   define _CCW_LIB_NAME    "ccw-fsc-xst3r"
#  elif _CCW_LIB_ID == 7
#   define _CCW_LIB_NAME    "ccw-fsc-xst3s"
#  elif _CCW_LIB_ID == 8
#   define _CCW_LIB_NAME    "ccw-fsc-xs3r"
#  elif _CCW_LIB_ID == 9
#   define _CCW_LIB_NAME    "ccw-fsc-xs3s"
#  elif _CCW_LIB_ID == 10
#   define _CCW_LIB_NAME    "ccw-fsc-xrxs3r"
#  elif _CCW_LIB_ID == 11
#   define _CCW_LIB_NAME    "ccw-fsc-xrxs3s"
#  endif
# endif
#elif defined(_CCW_LIB_MOD_ID) && defined(_CCW_NS_CCWSTD)
# if  _CCW_LIB_MOD_ID == 0
#  if   _CCW_LIB_ID == 0
#   define _CCW_LIB_NAME    "ccw_ccwstd-eh3r"
#  elif _CCW_LIB_ID == 1
#   define _CCW_LIB_NAME    "ccw_ccwstd-eh3s"
#  elif _CCW_LIB_ID == 2
#   define _CCW_LIB_NAME    "ccw_ccwstd-ne3r"
#  elif _CCW_LIB_ID == 3
#   define _CCW_LIB_NAME    "ccw_ccwstd-ne3s"
#  elif _CCW_LIB_ID == 4
#   define _CCW_LIB_NAME    "ccw_ccwstd-xr3r"
#  elif _CCW_LIB_ID == 5
#   define _CCW_LIB_NAME    "ccw_ccwstd-xr3s"
#  elif _CCW_LIB_ID == 6
#   define _CCW_LIB_NAME    "ccw_ccwstd-xst3r"
#  elif _CCW_LIB_ID == 7
#   define _CCW_LIB_NAME    "ccw_ccwstd-xst3s"
#  elif _CCW_LIB_ID == 8
#   define _CCW_LIB_NAME    "ccw_ccwstd-xs3r"
#  elif _CCW_LIB_ID == 9
#   define _CCW_LIB_NAME    "ccw_ccwstd-xs3s"
#  elif _CCW_LIB_ID == 10
#   define _CCW_LIB_NAME    "ccw_ccwstd-xrxs3r"
#  elif _CCW_LIB_ID == 11
#   define _CCW_LIB_NAME    "ccw_ccwstd-xrxs3s"
#  endif
# elif _CCW_LIB_MOD_ID == 1
#  if   _CCW_LIB_ID == 0
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-eh3r"
#  elif _CCW_LIB_ID == 1
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-eh3s"
#  elif _CCW_LIB_ID == 2
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-ne3r"
#  elif _CCW_LIB_ID == 3
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-ne3s"
#  elif _CCW_LIB_ID == 4
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-xr3r"
#  elif _CCW_LIB_ID == 5
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-xr3s"
#  elif _CCW_LIB_ID == 6
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-xst3r"
#  elif _CCW_LIB_ID == 7
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-xst3s"
#  elif _CCW_LIB_ID == 8
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-xs3r"
#  elif _CCW_LIB_ID == 9
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-xs3s"
#  elif _CCW_LIB_ID == 10
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-xrxs3r"
#  elif _CCW_LIB_ID == 11
#   define _CCW_LIB_NAME    "ccw_ccwstd-lfn-xrxs3s"
#  endif
# elif _CCW_LIB_MOD_ID == 2
#  if   _CCW_LIB_ID == 0
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-eh3r"
#  elif _CCW_LIB_ID == 1
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-eh3s"
#  elif _CCW_LIB_ID == 2
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-ne3r"
#  elif _CCW_LIB_ID == 3
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-ne3s"
#  elif _CCW_LIB_ID == 4
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-xr3r"
#  elif _CCW_LIB_ID == 5
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-xr3s"
#  elif _CCW_LIB_ID == 6
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-xst3r"
#  elif _CCW_LIB_ID == 7
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-xst3s"
#  elif _CCW_LIB_ID == 8
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-xs3r"
#  elif _CCW_LIB_ID == 9
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-xs3s"
#  elif _CCW_LIB_ID == 10
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-xrxs3r"
#  elif _CCW_LIB_ID == 11
#   define _CCW_LIB_NAME    "ccw_ccwstd-fsc-xrxs3s"
#  endif
# endif
#endif

#endif  // __cplusplus && !_CCW_NO_AUTO_LINK && !_CCW_LIB_NAME

#if defined(__cplusplus) && !defined(_CCW_NO_AUTO_LINK) && defined(_CCW_LIB_NAME)
#pragma library (_CCW_LIB_NAME)
#endif

#endif  // _CCW_CCW_AUTOLINK_H
