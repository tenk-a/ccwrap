/**
 *  @file   detail/old/debugging.hpp
 *  @brief  <debugging> (C++26) gap-fill: is_debugger_present / breakpoint /
 *          breakpoint_if_debugging.
 *  @license Boost Software License Version 1.0
 *  @note
 *      On Windows these map to the Win32 debugging APIs. Elsewhere
 *      is_debugger_present() is false and breakpoint() is a no-op (the standard
 *      allows breakpoint() to do nothing when no debugger is attached; a build
 *      without a way to trap simply has nothing to do).
 */
#ifndef _CCW_DETAIL_DEBUGGING_HPP
#define _CCW_DETAIL_DEBUGGING_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#ifndef _CCW_DEBUGGING_DEFINED
#define _CCW_DEBUGGING_DEFINED

#if defined(_WIN32)
extern "C" __declspec(dllimport) int  __stdcall IsDebuggerPresent(void);
extern "C" __declspec(dllimport) void __stdcall DebugBreak(void);
#endif

namespace std {

inline bool is_debugger_present() {
#if defined(_WIN32)
    return IsDebuggerPresent() != 0;
#else
    return false;
#endif
}

inline void breakpoint() {
#if defined(_WIN32)
    DebugBreak();
#endif
}

inline void breakpoint_if_debugging() {
    if (is_debugger_present())
        breakpoint();
}

}   // namespace std

#endif  // _CCW_DEBUGGING_DEFINED
#endif  // _CCW_DETAIL_DEBUGGING_HPP
