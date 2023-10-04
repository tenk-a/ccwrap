/**
 *  @file   stdint.h
 *  @brief  stdint(typedef int??_t) for vc(<=15.??)
 *  @date   2000-2017
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @note
 *   -  public domain software
 */
#pragma once

#include <ccwrap_header.h>

#if (_MSC_VER >= 1600)  // vc10 or later

#include __CCWRAP_NATIVE_C_HEADER_PATH(stdint.h)

#else   // _MSC_VER     // vc9 or earlier

#include <stddef.h>

#include "../_ccwrap/stdint_impl.h"

#endif  // _MSC_VER
