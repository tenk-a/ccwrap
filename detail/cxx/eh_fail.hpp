/**
 *  @file   detail/cxx/eh_fail.hpp
 *  @brief  __ccw_eh_fail: what _CCW_THROW does when exceptions are off.
 *  @license Boost Software License Version 1.0
 *  @note   Reports and aborts. It must stop even under NDEBUG, so it does not
 *          rely on assert() alone.
 */
#ifndef _CCW_DETAIL_EH_FAIL_HPP
#define _CCW_DETAIL_EH_FAIL_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#if defined(__cplusplus) && !_CCW_HAS_EXCEPTIONS
#include <cassert>
#include <cstdio>
#include <cstdlib>

_CCW_F_INL(PROC, void, __ccw_eh_fail, (const char* __what, const char* __file, int __line),
           (__what, __file, __line))
{
    std::fprintf(stderr, "ccwrap: would throw %s at %s:%d (built without exceptions)\n",
                 __what, __file, __line);
    std::fflush(stderr);
    _CCW_ASSERT(!"ccwrap: would throw, but the build has no exceptions");
    std::abort();
}
#endif

#endif  // _CCW_DETAIL_EH_FAIL_HPP
