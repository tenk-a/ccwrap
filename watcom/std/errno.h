// ccwrap watcom: <errno.h>. Forward to Open Watcom's native <errno.h> (via ../h on the
// INCLUDE path), then add the C11 / POSIX error numbers it lacks.
//
// Open Watcom stops at EILSEQ (40) and has none of the network/stream family that C11
// requires (EADDRINUSE, ECONNRESET, ETIMEDOUT, ...). The supplements are macro-only
// and shared with <cerrno>, so they live in ../../detail/c/errno_c11.h.
#pragma once

#include <ccwrap_common.h>
#ifndef _ERRNO_H_INCLUDED
# ifdef errno
#  undef errno
#  undef _ERRNO_DEFINED
# endif
# include _CCW_NATIVE_C_HEADER_PATH(errno.h)
#endif

#include "../../detail/c/errno_c11.h"
