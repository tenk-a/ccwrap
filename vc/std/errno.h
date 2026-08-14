// ccwrap <errno.h>
#pragma once

#include <ccwrap_common.h>

#ifdef errno
 #pragma push_macro("errno")
 #undef errno
 #include _CCW_NATIVE_UC_HEADER_PATH(errno.h)
 #pragma pop_macro("errno")
#else
 #include _CCW_NATIVE_UC_HEADER_PATH(errno.h)
#endif

#include <../../detail/c/errno_c11.h>
