// ccwrap <stdio.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(stdio.h)

#if _MSC_VER < 1900

#include <stdarg.h>

#pragma warning(push)
#pragma warning(disable:4996)

#ifdef __cplusplus
extern "C" {
#endif

#if _MSC_VER < 1400
static __forceinline int vsnprintf(char buf[], size_t bufsz, char const* fmt, va_list a) {
    return _vsnprintf(buf, bufsz, fmt, a);
}
#endif

#if defined(_CCW_C_CONFORM)
static inline int snprintf(char buf[], size_t bufsz, char const* fmt, ...) {
    int     len;
    va_list a;
    va_start(a, fmt);
    len = _vscprintf(fmt, a);           // chars the full conversion needs (excl. NUL)
    va_end(a);
    if (buf != 0 && bufsz > 0) {
        va_start(a, fmt);
        _vsnprintf(buf, bufsz - 1, fmt, a);   // write at most bufsz-1, may leave no NUL
        va_end(a);
        buf[bufsz - 1] = '\0';                 // guarantee termination in the last slot
    }
    return len;
}
#else
static inline int snprintf(char buf[], size_t bufsz, char const* fmt, ...) {
    int     n;
    va_list a;
    va_start(a, fmt);
    n = vsnprintf(buf, bufsz, fmt, a);
    va_end(a);
    return n;
}
#endif

static __forceinline int vsnwprintf(wchar_t buf[], size_t bufsz, wchar_t const* fmt, va_list a) {
    return _vsnwprintf(buf, bufsz, fmt, a);
}

static inline int snwprintf(wchar_t buf[], size_t bufsz, wchar_t const* fmt, ...) {
    int     n;
    va_list a;
    va_start(a, fmt);
    n = vsnwprintf(buf, bufsz, fmt, a);
    va_end(a);
    return n;
}

#if _MSC_VER < 1800
#include <ctype.h>
#include <string.h>
static inline int _ccw_vsscanf(const char* __buf, const char* __fmt, va_list __ap) {
    const char* __b = __buf;
    const char* __f = __fmt;
    char        __spec[64];
    int         __total = 0;
    while (*__f) {
        if (isspace((unsigned char)*__f)) {           /* whitespace: skip run in input */
            while (isspace((unsigned char)*__b)) ++__b;
            ++__f;
            continue;
        }
        if (*__f != '%') {                            /* literal char must match */
            if (*__b != *__f) break;
            ++__b; ++__f;
            continue;
        }
        {
            const char* __start = __f++;              /* at the char after '%' */
            const char* __end;
            int __suppress = 0, __consumed = -1, __r, __len;
            if (*__f == '%') {                        /* literal %% */
                if (*__b != '%') break;
                ++__b; ++__f;
                continue;
            }
            if (*__f == '*') __suppress = 1;
            while (*__f && !strchr("diouxXeEfgGaAcsp[n", *__f)) ++__f;
            if (*__f == 0) break;
            __end = __f + 1;
            if (*__f == '[') {                        /* scan set: [ ... ] */
                const char* __p = __f + 1;
                if (*__p == '^') ++__p;
                if (*__p == ']') ++__p;
                while (*__p && *__p != ']') ++__p;
                if (*__p == ']') ++__p;
                __end = __p;
            }
            __len = (int)(__end - __start);
            if (__len < 0 || __len > 60) break;
            memcpy(__spec, __start, (size_t)__len);
            __spec[__len] = '%'; __spec[__len + 1] = 'n'; __spec[__len + 2] = 0;
            if (__suppress) {
                __r = sscanf(__b, __spec, &__consumed);
            } else {
                void* __arg = va_arg(__ap, void*);
                __r = sscanf(__b, __spec, __arg, &__consumed);
            }
            if (__consumed < 0) break;                /* conversion made no progress */
            __b += __consumed;
            __f = __end;
            if (!__suppress) {
                if (__r < 1) break;
                ++__total;
            }
        }
    }
    return __total;
}
static inline int vsscanf(const char* __buf, const char* __fmt, va_list __ap) {
    return _ccw_vsscanf(__buf, __fmt, __ap);
}

static inline int _ccw_vfscanf(FILE* __fp, const char* __fmt, va_list __ap) {
    const char* __f = __fmt;
    char        __spec[64];
    int         __total = 0;
    while (*__f) {
        if (isspace((unsigned char)*__f)) {           /* whitespace: skip a run */
            int __c;
            while ((__c = fgetc(__fp)) != EOF && isspace(__c)) {}
            if (__c != EOF) ungetc(__c, __fp);
            ++__f;
            continue;
        }
        if (*__f != '%') {                            /* literal char must match */
            int __c = fgetc(__fp);
            if (__c != (unsigned char)*__f) { if (__c != EOF) ungetc(__c, __fp); break; }
            ++__f;
            continue;
        }
        {
            const char* __start = __f++;
            const char* __end;
            int __suppress = 0, __r, __len;
            if (*__f == '%') {                        /* literal %% */
                int __c = fgetc(__fp);
                if (__c != '%') { if (__c != EOF) ungetc(__c, __fp); break; }
                ++__f;
                continue;
            }
            if (*__f == '*') __suppress = 1;
            while (*__f && !strchr("diouxXeEfgGaAcsp[n", *__f)) ++__f;
            if (*__f == 0) break;
            __end = __f + 1;
            if (*__f == '[') {                        /* scan set: [ ... ] */
                const char* __p = __f + 1;
                if (*__p == '^') ++__p;
                if (*__p == ']') ++__p;
                while (*__p && *__p != ']') ++__p;
                if (*__p == ']') ++__p;
                __end = __p;
            }
            __len = (int)(__end - __start);
            if (__len < 0 || __len > 62) break;
            memcpy(__spec, __start, (size_t)__len);
            __spec[__len] = 0;
            if (__suppress) {
                __r = fscanf(__fp, __spec);
            } else {
                void* __arg = va_arg(__ap, void*);
                __r = fscanf(__fp, __spec, __arg);
            }
            __f = __end;
            if (!__suppress) {
                if (__r < 1) break;
                ++__total;
            }
        }
    }
    return __total;
}
static inline int vfscanf(FILE* __fp, const char* __fmt, va_list __ap) {
    return _ccw_vfscanf(__fp, __fmt, __ap);
}
static inline int vscanf(const char* __fmt, va_list __ap) {
    return _ccw_vfscanf(stdin, __fmt, __ap);
}
#endif  /* _MSC_VER < 1800 */

#ifdef __cplusplus
}
#endif

#pragma warning(pop)

#endif
