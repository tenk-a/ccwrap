/**
 *  @file   detail/c/ccwrap_config.h
 *  @brief  Build-time configuration knobs for ccwrap, gathered per compiler.
 *  @license Boost Software License Version 1.0
 *  @note   Definitions only -- this file must not include anything.
 */
#ifndef _CCW_DETAIL_CCWRAP_CONFIG_H
#define _CCW_DETAIL_CCWRAP_CONFIG_H
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif


#ifndef _CCW_CFG_MAKE_PAIR_BYREF
# if defined(__WATCOMC__)
#  define _CCW_CFG_MAKE_PAIR_BYREF  1
# else
#  define _CCW_CFG_MAKE_PAIR_BYREF  0
# endif
#endif

#ifndef _CCW_CFG_FNCTMPL_ARG_BYREF
# if defined(__WATCOMC__)
#  define _CCW_CFG_FNCTMPL_ARG_BYREF    1
# else
#  define _CCW_CFG_FNCTMPL_ARG_BYREF    0
# endif
#endif


#ifndef _CCW_CFG_CHECK
# define _CCW_CFG_CHECK 0
#endif



#endif  /* _CCW_DETAIL_CCWRAP_CONFIG_H */
