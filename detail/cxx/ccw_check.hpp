/**
 *  @file   detail/cxx/ccw_check.hpp
 *  @brief  __ccw_chk_fail: report a broken container/string invariant and abort.
 *  @license Boost Software License Version 1.0
 *  @note   Only compiled when _CCW_CFG_CHECK is on.
 */
#ifndef _CCW_DETAIL_CCW_CHECK_HPP
#define _CCW_DETAIL_CCW_CHECK_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#if defined(__cplusplus) && defined(_CCW_CFG_CHECK) && _CCW_CFG_CHECK
#include <cstdio>
#include <cstdlib>

#if _CCW_CFG_CHECK >= 2
inline void __ccw_chk_scan(void)
{
    unsigned long __lo = (unsigned long)(void*)&__ccw_chk_scan;
    unsigned long __hi = __lo;
    unsigned long* __sp = (unsigned long*)(void*)&__hi;
    int __n = 0;
    __lo = (__lo > 0x00200000ul) ? __lo - 0x00200000ul : 0x00001000ul;
    __hi += 0x00200000ul;
    for (int __i = 0; __i < 4096 && __n < 24; ++__i) {
        unsigned long __v = __sp[__i];
        if (__v >= __lo && __v <= __hi && (__v & 3ul) != 3ul) {
            std::fprintf(stderr, "  [%2d] +%04x  %08lx\n", __n, (unsigned)(__i * 4), __v);
            ++__n;
        }
    }
}
#endif

inline void __ccw_chk_fail(const char* __what, const void* __obj,
                           unsigned long __a, unsigned long __b, unsigned long __c,
                           const char* __file, int __line)
{
    std::fprintf(stderr, "ccwrap CHECK: %s  obj=%p  %08lx %08lx %08lx  at %s:%d\n",
                 __what, __obj, __a, __b, __c, __file, __line);
#if _CCW_CFG_CHECK >= 2
    __ccw_chk_scan();
#endif
    std::fflush(stderr);
    std::abort();
}

#define _CCW_CHK(__cond, __what, __obj, __a, __b, __c) \
    do { if (!(__cond)) __ccw_chk_fail((__what), (const void*)(__obj), \
                                       (unsigned long)(__a), (unsigned long)(__b), \
                                       (unsigned long)(__c), __FILE__, __LINE__); } while (0)

#define _CCW_CHK_INIT(__v)  unsigned long __v = 0
#define _CCW_CHK_STEP(__v, __lim, __what, __obj, __p) \
    do { if (++(__v) > (unsigned long)(__lim) + 64u) \
             __ccw_chk_fail((__what), (const void*)(__obj), (unsigned long)(__v), \
                            (unsigned long)(__lim), (unsigned long)(__p), \
                            __FILE__, __LINE__); } while (0)
#else
#define _CCW_CHK(__cond, __what, __obj, __a, __b, __c)  ((void)0)
#define _CCW_CHK_INIT(__v)                              ((void)0)
#define _CCW_CHK_STEP(__v, __lim, __what, __obj, __p)   ((void)0)
#endif

#endif  // _CCW_DETAIL_CCW_CHECK_HPP
