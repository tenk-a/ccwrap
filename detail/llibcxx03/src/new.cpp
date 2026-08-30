//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: replaceable global allocation functions for Open Watcom.
//  The native runtime's operator new returns NULL on failure and never calls
//  the new_handler; these definitions (searched before the native library, the
//  same way the locale layer overrides localeconv) restore the standard loop:
//  try malloc, call the installed new_handler, else throw bad_alloc.
//===----------------------------------------------------------------------===//
#if defined(__WATCOMC__) && __WATCOMC__ >= 1300

#include <new>
#include <cstdlib>

_CCW_STD::new_handler& _CCW_STD::__ccw_new_handler_slot() {
    static _CCW_STD::new_handler __h = 0;
    return __h;
}

static void* __ccw_new_core(_CCW_STD::size_t __n, int __can_return_null) {
    if (__n == 0) __n = 1;
    for (;;) {
        void* __p = _CCW_STD::malloc(__n);
        if (__p) return __p;
        _CCW_STD::new_handler __h = _CCW_STD::__ccw_new_handler_slot();
        if (!__h) {
            if (__can_return_null) return 0;
            _CCW_THROW(_CCW_STD::bad_alloc());
        }
        __h();
    }
}

void* _CCW_STD::__ccw_new_nothrow(_CCW_STD::size_t __n) _CCW_LIBCPP_NOEXCEPT {
    return __ccw_new_core(__n, 1);
}

void _CCW_STD::__ccw_throw_bad_array_new_length() {
    _CCW_THROW(_CCW_STD::bad_array_new_length());
}

#ifndef _CCW_NEW_NO_REPLACE
void* operator new(_CCW_STD::size_t __n)   { return __ccw_new_core(__n, 0); }
void* operator new[](_CCW_STD::size_t __n) { return __ccw_new_core(__n, 0); }
void  operator delete(void* __p)   { if (__p) _CCW_STD::free(__p); }
void  operator delete[](void* __p) { if (__p) _CCW_STD::free(__p); }
#endif

#endif  // __WATCOMC__ >= 1300
