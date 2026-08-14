/* win32_fwd.h - ccwrap detail: opaque Win32 tags, so that ccwrap's own Win32
 * declarations name the same types <windows.h> names.
 *
 * Declaring these APIs with void* where the real ones take a struct pointer makes
 * the two declarations disagree, and extern "C" functions cannot be overloaded --
 * so a program that includes both <windows.h> and, say, <mutex> fails to compile,
 * in either include order. The tag names below are identical in the Microsoft SDK,
 * MinGW and Open Watcom's own h/nt, and the tags stay incomplete here, so this
 * header composes with <windows.h> whichever comes first.
 */
#ifndef CCW_DETAIL_WIN_WIN32_FWD_H_INCLUDED__
#define CCW_DETAIL_WIN_WIN32_FWD_H_INCLUDED__

struct _RTL_CRITICAL_SECTION;
struct _RTL_CONDITION_VARIABLE;
struct _RTL_SRWLOCK;
struct _SECURITY_ATTRIBUTES;
struct _FILETIME;
struct HINSTANCE__;

typedef struct _RTL_CRITICAL_SECTION*   __ccw_pcritical_section;
typedef struct _RTL_CONDITION_VARIABLE* __ccw_pcondition_variable;
typedef struct _RTL_SRWLOCK*            __ccw_psrwlock;
typedef struct _SECURITY_ATTRIBUTES*    __ccw_psecurity_attributes;
typedef struct _FILETIME*               __ccw_pfiletime;
typedef struct HINSTANCE__*             __ccw_hmodule;

#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_ARM64)
typedef _ccw_llong (_ccw_stdcall* __ccw_farproc_t)();
#else
typedef int (_ccw_stdcall* __ccw_farproc_t)();
#endif

#endif /* CCW_DETAIL_WIN_WIN32_FWD_H_INCLUDED__ */
