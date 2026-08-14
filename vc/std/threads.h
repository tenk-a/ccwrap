// ccwrap <threads.h>
#pragma once

#include <ccwrap_common.h>

#if !defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L \
    && !defined(__STDC_NO_THREADS__) && defined(_MSC_VER) && _MSC_VER >= 1944
 #include _CCW_NATIVE_UC_HEADER_PATH(threads.h)
#else
 #include <../../detail/c/win/threads.h>
#endif
