// ccwrap watcom: <threads.h> (C11 threads).
//
// Open Watcom ships no <threads.h> at all. The backport in ../detail/win/threads.h is
// written to be toolchain-neutral -- threads go through the CRT _beginthreadex
// (<process.h>, which Open Watcom has) and the mutex / condition-variable / TLS calls
// are self-declared Win32 prototypes over void* handles -- so vc and watcom can share it.
#pragma once

#include <ccwrap_common.h>

#if !defined(_CCW_OS_WIN)
# error "ccwrap: <threads.h> is Win32-only on Open Watcom -- the C11 thread API rests on the CRT _beginthreadex plus Win32 sync objects."
#endif
#include "../../detail/c/win/threads.h"
