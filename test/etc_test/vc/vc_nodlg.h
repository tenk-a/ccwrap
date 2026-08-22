#ifndef CCW_ETC_VC_NODLG_H
#define CCW_ETC_VC_NODLG_H

#if defined(_MSC_VER)

#include <crtdbg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C" __declspec(dllimport) unsigned long __stdcall SetErrorMode(unsigned long);
#else
__declspec(dllimport) unsigned long __stdcall SetErrorMode(unsigned long);
#endif

static void __cdecl ccw_nodlg_invalid_param(wchar_t const* expr, wchar_t const* func,
                                            wchar_t const* file, unsigned int line,
                                            uintptr_t reserved)
{
    (void)expr; (void)func; (void)file; (void)line; (void)reserved;
}

static void ccw_no_crash_dialogs(void)
{
    SetErrorMode(0x0001u | 0x0002u | 0x8000u);   /* FAILCRITICALERRORS | NOGPFAULTERRORBOX | NOOPENFILEERRORBOX */
    _set_invalid_parameter_handler(ccw_nodlg_invalid_param);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#if defined(_DEBUG)
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR,  _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR,  _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN,   _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN,   _CRTDBG_FILE_STDERR);
#endif
    setvbuf(stdout, (char*)0, _IONBF, 0);
}

#else

static void ccw_no_crash_dialogs(void) {}

#endif

#endif
