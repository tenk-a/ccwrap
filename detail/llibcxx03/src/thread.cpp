//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: Win32 implementation of <thread> (see llibcxx03/include/thread).
//  Windows only; the whole file is empty on Open Watcom 1.9 (< 1300), where the
//  llibcxx03 tree is not used.
#if defined(__WATCOMC__) && __WATCOMC__ >= 1300

#include <thread>
#include <cstdlib>   // getenv / atoi for hardware_concurrency

extern "C" {
__declspec(dllimport) void*         __stdcall CreateThread(void*, unsigned long, unsigned long (__stdcall*)(void*), void*, unsigned long, unsigned long*);
__declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void*, unsigned long);
__declspec(dllimport) int           __stdcall CloseHandle(void*);
__declspec(dllimport) void          __stdcall Sleep(unsigned long);
__declspec(dllimport) unsigned long __stdcall GetCurrentThreadId(void);
__declspec(dllimport) void*         __stdcall GetCurrentProcess(void);
__declspec(dllimport) int           __stdcall TerminateProcess(void*, unsigned int);
}

namespace {
unsigned long __stdcall __ccw_thread_tramp(void* __p) {
    _CCW_STD::__thread_detail::__callable_base* __c = static_cast<_CCW_STD::__thread_detail::__callable_base*>(__p);
    __c->__run();
    delete __c;
    return 0;
}
}   // namespace

void _CCW_STD::thread::__start(_CCW_STD::__thread_detail::__callable_base* __c) {
    unsigned long __id = 0;
    void* __h = CreateThread(0, 0, &__ccw_thread_tramp, __c, 0, &__id);
    if (!__h) { delete __c; return; }
    __h_ = __h;
    __id_ = __id;
}

_CCW_STD::thread::~thread() {
    if (__h_) TerminateProcess(GetCurrentProcess(), 3);
}

void _CCW_STD::thread::join() {
    if (__h_) {
        WaitForSingleObject(__h_, 0xFFFFFFFFUL);   // INFINITE
        CloseHandle(__h_);
        __h_ = 0; __id_ = 0;
    }
}

void _CCW_STD::thread::detach() {
    if (__h_) { CloseHandle(__h_); __h_ = 0; __id_ = 0; }
}

unsigned _CCW_STD::thread::hardware_concurrency() {
    const char* __e = _CCW_STD::getenv("NUMBER_OF_PROCESSORS");
    int __n = __e ? _CCW_STD::atoi(__e) : 0;
    return __n > 0 ? (unsigned)__n : 1u;
}

void _CCW_STD::this_thread::__ccw_sleep_ms(unsigned long __ms) { Sleep(__ms); }
void _CCW_STD::this_thread::yield() { Sleep(0); }
_CCW_STD::thread::id _CCW_STD::this_thread::get_id() { return _CCW_STD::thread::id((unsigned long)GetCurrentThreadId()); }

#endif  // Watcom 2.0 or MSVC
