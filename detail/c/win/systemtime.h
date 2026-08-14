/**
 *  @file   detail/c/win/systemtime.h
 *  @brief  Win32 wall clock in 100 ns units, shared by the timespec_get backports.
 *  @license Boost Software License Version 1.0
 */
#ifndef CCW_DETAIL_WIN_SYSTEMTIME_H_INCLUDED__
#define CCW_DETAIL_WIN_SYSTEMTIME_H_INCLUDED__

#include "win32_fwd.h"

#ifdef __cplusplus
namespace __ccw { extern "C" {
#endif
_CCW_WINAPI(void) GetSystemTimeAsFileTime(__ccw_pfiletime);
_CCW_WINAPI(__ccw_hmodule) GetModuleHandleA(const char*);
_CCW_WINAPI(__ccw_farproc_t) GetProcAddress(__ccw_hmodule, const char*);
#ifdef __cplusplus
} }
#endif

#if defined(__cplusplus)
#  define _CCW_ST_NS  ::__ccw::
#else
#  define _CCW_ST_NS
#endif

typedef void (_ccw_stdcall* __ccw_systemtime_ft_t)(__ccw_pfiletime);

#if defined(__cplusplus)
#  define _CCW_ST_FN  inline
#else
#  define _CCW_ST_FN  static _ccw_forceinline
#endif

_CCW_ST_FN void __ccw_win_systemtime_100ns(unsigned long __ft[2])
{
    static __ccw_systemtime_ft_t __fn;
    static int __resolved;
    if (!__resolved) {
        __ccw_hmodule __k = _CCW_ST_NS GetModuleHandleA("kernel32.dll");
        if (__k)
            __fn = (__ccw_systemtime_ft_t)
                   _CCW_ST_NS GetProcAddress(__k, "GetSystemTimePreciseAsFileTime");
        __resolved = 1;
    }
    if (__fn)
        __fn((__ccw_pfiletime)__ft);
    else
        _CCW_ST_NS GetSystemTimeAsFileTime((__ccw_pfiletime)__ft);
}

#undef _CCW_ST_FN
#undef _CCW_ST_NS

#endif /* CCW_DETAIL_WIN_SYSTEMTIME_H_INCLUDED__ */
