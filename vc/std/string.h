// ccwrap <string.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(string.h)

#if _CCW_TARGET_C >= 2023
#include <stdlib.h>

_CCW_F_DEF(FUNC, char *, strndup,(const char *s, size_t n), (s, n)) {
    size_t len = 0;
    char  *p;
    while (len < n && s[len])
        ++len;
    p = (char *)malloc(len + 1);
    if (p) {
        memcpy(p, s, len);
        p[len] = '\0';
    }
    return p;
}

#endif
