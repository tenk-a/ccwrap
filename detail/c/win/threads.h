/**
 *  @file   detail/win/threads.h
 *  @brief  C11 <threads.h> backport over the CRT _beginthreadex + Win32.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_WIN_THREADS_H
#define _CCW_DETAIL_WIN_THREADS_H
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <intrin.h>
#include "win32_fwd.h"

#if defined(_MSC_VER) && _MSC_VER < 1500
# define _CCW_CNDVAR_DYNAMIC 1
#endif

#ifdef __cplusplus
extern "C" {
#endif
_CCW_WINAPI(void*) GetCurrentThread(void);
_CCW_WINAPI(unsigned long) GetCurrentThreadId(void);
_CCW_WINAPI(unsigned long) WaitForSingleObject(void*, unsigned long);
_CCW_WINAPI(int) GetExitCodeThread(void*, unsigned long*);
_CCW_WINAPI(int) CloseHandle(void*);
_CCW_WINAPI(void) Sleep(unsigned long);
_CCW_WINAPI(int) SwitchToThread(void);
_CCW_WINAPI(void) InitializeCriticalSection(__ccw_pcritical_section);
_CCW_WINAPI(void) EnterCriticalSection(__ccw_pcritical_section);
_CCW_WINAPI(int) TryEnterCriticalSection(__ccw_pcritical_section);
_CCW_WINAPI(void) LeaveCriticalSection(__ccw_pcritical_section);
_CCW_WINAPI(void) DeleteCriticalSection(__ccw_pcritical_section);
#if !defined(_CCW_CNDVAR_DYNAMIC)
_CCW_WINAPI(void) InitializeConditionVariable(__ccw_pcondition_variable);
_CCW_WINAPI(int) SleepConditionVariableCS(__ccw_pcondition_variable, __ccw_pcritical_section, unsigned long);
_CCW_WINAPI(void) WakeConditionVariable(__ccw_pcondition_variable);
_CCW_WINAPI(void) WakeAllConditionVariable(__ccw_pcondition_variable);
#endif
_CCW_WINAPI(unsigned long) TlsAlloc(void);
_CCW_WINAPI(void*) TlsGetValue(unsigned long);
_CCW_WINAPI(int) TlsSetValue(unsigned long, void*);
_CCW_WINAPI(int) TlsFree(unsigned long);
#if defined(_CCW_CNDVAR_DYNAMIC)
_CCW_WINAPI(__ccw_hmodule) GetModuleHandleA(const char*);
_CCW_WINAPI(__ccw_farproc_t) GetProcAddress(__ccw_hmodule, const char*);
#endif
#ifdef __cplusplus
}
#endif

#if defined(_CCW_CNDVAR_DYNAMIC)
typedef void (_ccw_stdcall* __ccw_cv_void_t)(__ccw_pcondition_variable);
typedef int  (_ccw_stdcall* __ccw_cv_sleep_t)(__ccw_pcondition_variable, __ccw_pcritical_section, unsigned long);

struct __ccw_cv_fns {
    __ccw_cv_void_t  _init;
    __ccw_cv_void_t  _wake;
    __ccw_cv_void_t  _wake_all;
    __ccw_cv_sleep_t _sleep;
    int              _done;
};

static inline struct __ccw_cv_fns* __ccw_cv(void) {
    static struct __ccw_cv_fns f;
    if (!f._done) {
        __ccw_hmodule k = GetModuleHandleA("kernel32.dll");
        if (k) {
            f._init     = (__ccw_cv_void_t)GetProcAddress(k, "InitializeConditionVariable");
            f._wake     = (__ccw_cv_void_t)GetProcAddress(k, "WakeConditionVariable");
            f._wake_all = (__ccw_cv_void_t)GetProcAddress(k, "WakeAllConditionVariable");
            f._sleep    = (__ccw_cv_sleep_t)GetProcAddress(k, "SleepConditionVariableCS");
        }
        f._done = 1;
    }
    return &f;
}
#endif  /* _CCW_CNDVAR_DYNAMIC */

enum {
    thrd_success = 0, thrd_nomem = 1, thrd_timedout = 2, thrd_busy = 3, thrd_error = 4
};
enum { mtx_plain = 0, mtx_recursive = 1, mtx_timed = 2 };

typedef int (*thrd_start_t)(void*);
typedef void (*tss_dtor_t)(void*);

typedef struct { void* _h; unsigned long _id; } thrd_t;
typedef struct { void* _opaque[8]; } mtx_t;      /* holds a CRITICAL_SECTION */
typedef struct { void* _p; }        cnd_t;       /* holds a CONDITION_VARIABLE */
typedef unsigned long               tss_t;       /* TLS index */
typedef struct { long _s; }         once_flag;
#define ONCE_FLAG_INIT              { 0 }
#define TSS_DTOR_ITERATIONS         1

struct __ccw_thrd_start { thrd_start_t fn; void* arg; };
static inline unsigned _ccw_stdcall __ccw_thrd_trampoline(void* p) {
    struct __ccw_thrd_start s = *(struct __ccw_thrd_start*)p;
    free(p);
    return (unsigned)s.fn(s.arg);
}

static inline int thrd_create(thrd_t* thr, thrd_start_t fn, void* arg) {
    struct __ccw_thrd_start* s = (struct __ccw_thrd_start*)malloc(sizeof(*s));
    unsigned id = 0;
    void* h;
    if (!s) return thrd_nomem;
    s->fn = fn; s->arg = arg;
    h = (void*)_beginthreadex(0, 0, __ccw_thrd_trampoline, s, 0, &id);
    if (!h) { free(s); return thrd_error; }
    thr->_h = h; thr->_id = id;
    return thrd_success;
}
static inline int thrd_join(thrd_t thr, int* res) {
    unsigned long code = 0;
    WaitForSingleObject(thr._h, 0xFFFFFFFFul);
    GetExitCodeThread(thr._h, &code);
    if (res) *res = (int)code;
    CloseHandle(thr._h);
    return thrd_success;
}
static inline int thrd_detach(thrd_t thr) { CloseHandle(thr._h); return thrd_success; }
static inline thrd_t thrd_current(void) {
    thrd_t t; t._h = GetCurrentThread(); t._id = GetCurrentThreadId(); return t;
}
static inline int  thrd_equal(thrd_t a, thrd_t b) { return a._id == b._id; }
static inline void thrd_yield(void) { SwitchToThread(); }
static inline void thrd_exit(int res) { _endthreadex((unsigned)res); }
static inline int  thrd_sleep(const struct timespec* dur, struct timespec* rem) {
    unsigned long ms = (unsigned long)(dur->tv_sec * 1000 + dur->tv_nsec / 1000000);
    (void)rem;
    Sleep(ms);
    return 0;
}

static inline int mtx_init(mtx_t* m, int type) { (void)type; InitializeCriticalSection((__ccw_pcritical_section)m); return thrd_success; }
static inline int mtx_lock(mtx_t* m)     { EnterCriticalSection((__ccw_pcritical_section)m); return thrd_success; }
static inline int mtx_trylock(mtx_t* m)  { return TryEnterCriticalSection((__ccw_pcritical_section)m) ? thrd_success : thrd_busy; }
static inline int mtx_unlock(mtx_t* m)   { LeaveCriticalSection((__ccw_pcritical_section)m); return thrd_success; }
static inline void mtx_destroy(mtx_t* m) { DeleteCriticalSection((__ccw_pcritical_section)m); }

#if defined(_CCW_CNDVAR_DYNAMIC)
static inline int cnd_init(cnd_t* c) {
    struct __ccw_cv_fns* f = __ccw_cv();
    if (!f->_init) return thrd_error;
    f->_init((__ccw_pcondition_variable)&c->_p);
    return thrd_success;
}
static inline int cnd_signal(cnd_t* c) {
    struct __ccw_cv_fns* f = __ccw_cv();
    if (!f->_wake) return thrd_error;
    f->_wake((__ccw_pcondition_variable)&c->_p);
    return thrd_success;
}
static inline int cnd_broadcast(cnd_t* c) {
    struct __ccw_cv_fns* f = __ccw_cv();
    if (!f->_wake_all) return thrd_error;
    f->_wake_all((__ccw_pcondition_variable)&c->_p);
    return thrd_success;
}
static inline int cnd_wait(cnd_t* c, mtx_t* m) {
    struct __ccw_cv_fns* f = __ccw_cv();
    if (!f->_sleep) return thrd_error;
    return f->_sleep((__ccw_pcondition_variable)&c->_p, (__ccw_pcritical_section)m, 0xFFFFFFFFul)
        ? thrd_success : thrd_error;
}
#else
static inline int cnd_init(cnd_t* c)      { InitializeConditionVariable((__ccw_pcondition_variable)&c->_p); return thrd_success; }
static inline int cnd_signal(cnd_t* c)    { WakeConditionVariable((__ccw_pcondition_variable)&c->_p); return thrd_success; }
static inline int cnd_broadcast(cnd_t* c) { WakeAllConditionVariable((__ccw_pcondition_variable)&c->_p); return thrd_success; }
static inline int cnd_wait(cnd_t* c, mtx_t* m) {
    return SleepConditionVariableCS((__ccw_pcondition_variable)&c->_p, (__ccw_pcritical_section)m, 0xFFFFFFFFul) ? thrd_success : thrd_error;
}
#endif  // _CCW_CNDVAR_DYNAMIC
static inline void cnd_destroy(cnd_t* c) { (void)c; }

static inline int  tss_create(tss_t* key, tss_dtor_t dtor) { (void)dtor; *key = TlsAlloc(); return (*key == 0xFFFFFFFFul) ? thrd_error : thrd_success; }
static inline void* tss_get(tss_t key)                     { return TlsGetValue(key); }
static inline int  tss_set(tss_t key, void* val)          { return TlsSetValue(key, val) ? thrd_success : thrd_error; }
static inline void tss_delete(tss_t key)                  { TlsFree(key); }

static inline void call_once(once_flag* flag, void (*func)(void)) {
    for (;;) {
        if (flag->_s == 2) return;
        if (flag->_s == 0 && _InterlockedCompareExchange(&flag->_s, 1, 0) == 0) {
            func();
            flag->_s = 2;
            return;
        }
        Sleep(0);   /* another thread is running func; spin-yield */
    }
}

#endif  // _CCW_DETAIL_WIN_THREADS_H
