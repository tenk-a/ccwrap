/**
 *  @file   detail/c/ccwrap_config.h
 *  @brief  Build-time configuration knobs for ccwrap, gathered per compiler.
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @license Boost Software License Version 1.0
 *  @note   Definitions only -- this file must not include anything.
 */
#ifndef _CCW_DETAIL_CCWRAP_CONFIG_H
#define _CCW_DETAIL_CCWRAP_CONFIG_H
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

/* --- Open Watcom --- */

/* make_pair takes its arguments by reference (overload set) instead of by value. */
#ifndef _CCW_CFG_MAKE_PAIR_BYREF
# if defined(__WATCOMC__)
#  define _CCW_CFG_MAKE_PAIR_BYREF  1
# else
#  define _CCW_CFG_MAKE_PAIR_BYREF  0
# endif
#endif

/* fnctmpl.hpp (emplace / make_unique / ...) takes its arguments by reference. */
#ifndef _CCW_CFG_FNCTMPL_ARG_BYREF
# if defined(__WATCOMC__)
#  define _CCW_CFG_FNCTMPL_ARG_BYREF    1
# else
#  define _CCW_CFG_FNCTMPL_ARG_BYREF    0
# endif
#endif

/* --- all compilers --- */

/* Container/string self-checks: report the broken invariant and abort. Off by default. */
#ifndef _CCW_CFG_CHECK
# define _CCW_CFG_CHECK 0
#endif

/* --- MSVC --- */

/* --- gcc / clang --- */

#endif  /* _CCW_DETAIL_CCWRAP_CONFIG_H */
