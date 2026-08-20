/* The genuine Win32 declarations, standing in for <windows.h>: that header cannot
   be preprocessed under ccwrap's forced config, so the
   coexistence tests declare by hand exactly what it declares. The struct tags are
   the ones Open Watcom's own h/nt, the Microsoft SDK and MinGW all use. */
#ifndef CCW_TEST_WATCOM_WIN32_DECLS_H
#define CCW_TEST_WATCOM_WIN32_DECLS_H

typedef struct _FILETIME {
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
} FILETIME, *LPFILETIME;

typedef struct _RTL_CRITICAL_SECTION { void* Reserved[6]; }
    CRITICAL_SECTION, *LPCRITICAL_SECTION, *PCRITICAL_SECTION;
typedef struct _RTL_CONDITION_VARIABLE { void* Ptr; }
    CONDITION_VARIABLE, *PCONDITION_VARIABLE;
typedef struct _RTL_SRWLOCK { void* Ptr; } SRWLOCK, *PSRWLOCK;
typedef struct _SECURITY_ATTRIBUTES { unsigned long nLength; void* lpDescriptor; int bInherit; }
    SECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
typedef struct HINSTANCE__ { int unused; } *HMODULE;
typedef int (__stdcall *FARPROC)(void);
typedef void* HANDLE;

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllimport) void __stdcall GetSystemTimeAsFileTime(LPFILETIME);

__declspec(dllimport) void __stdcall InitializeCriticalSection(LPCRITICAL_SECTION);
__declspec(dllimport) void __stdcall EnterCriticalSection(LPCRITICAL_SECTION);
__declspec(dllimport) int  __stdcall TryEnterCriticalSection(LPCRITICAL_SECTION);
__declspec(dllimport) void __stdcall LeaveCriticalSection(LPCRITICAL_SECTION);
__declspec(dllimport) void __stdcall DeleteCriticalSection(LPCRITICAL_SECTION);

__declspec(dllimport) void __stdcall InitializeConditionVariable(PCONDITION_VARIABLE);
__declspec(dllimport) int  __stdcall SleepConditionVariableCS(PCONDITION_VARIABLE, PCRITICAL_SECTION, unsigned long);
__declspec(dllimport) void __stdcall WakeConditionVariable(PCONDITION_VARIABLE);
__declspec(dllimport) void __stdcall WakeAllConditionVariable(PCONDITION_VARIABLE);

__declspec(dllimport) void __stdcall InitializeSRWLock(PSRWLOCK);
__declspec(dllimport) void __stdcall AcquireSRWLockExclusive(PSRWLOCK);
__declspec(dllimport) void __stdcall ReleaseSRWLockExclusive(PSRWLOCK);
__declspec(dllimport) void __stdcall AcquireSRWLockShared(PSRWLOCK);
__declspec(dllimport) void __stdcall ReleaseSRWLockShared(PSRWLOCK);
__declspec(dllimport) unsigned char __stdcall TryAcquireSRWLockExclusive(PSRWLOCK);
__declspec(dllimport) unsigned char __stdcall TryAcquireSRWLockShared(PSRWLOCK);

__declspec(dllimport) HANDLE  __stdcall CreateSemaphoreA(LPSECURITY_ATTRIBUTES, long, long, const char*);
__declspec(dllimport) int     __stdcall ReleaseSemaphore(HANDLE, long, long*);
__declspec(dllimport) HANDLE  __stdcall CreateMutexA(LPSECURITY_ATTRIBUTES, int, const char*);
__declspec(dllimport) int     __stdcall ReleaseMutex(HANDLE);
__declspec(dllimport) HANDLE  __stdcall GetCurrentThread(void);
__declspec(dllimport) unsigned long __stdcall GetCurrentThreadId(void);
__declspec(dllimport) unsigned long __stdcall WaitForSingleObject(HANDLE, unsigned long);
__declspec(dllimport) int     __stdcall GetExitCodeThread(HANDLE, unsigned long*);
__declspec(dllimport) int     __stdcall CloseHandle(HANDLE);
__declspec(dllimport) void    __stdcall Sleep(unsigned long);
__declspec(dllimport) int     __stdcall SwitchToThread(void);
__declspec(dllimport) unsigned long __stdcall TlsAlloc(void);
__declspec(dllimport) void*   __stdcall TlsGetValue(unsigned long);
__declspec(dllimport) int     __stdcall TlsSetValue(unsigned long, void*);
__declspec(dllimport) int     __stdcall TlsFree(unsigned long);

__declspec(dllimport) HMODULE __stdcall GetModuleHandleA(const char*);
__declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE, const char*);
__declspec(dllimport) unsigned short __stdcall RtlCaptureStackBackTrace(unsigned long, unsigned long, void**, unsigned long*);
__declspec(dllimport) int     __stdcall IsDebuggerPresent(void);
__declspec(dllimport) void    __stdcall DebugBreak(void);

#ifdef __cplusplus
}
#endif

#endif /* CCW_TEST_WATCOM_WIN32_DECLS_H */
