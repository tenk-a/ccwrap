/**
 *  @file   watcom/std/ccw_eh_anchor.h
 *  @brief  Include this from exactly one .cpp when the per-header anchors are
 *          turned off with _CCW_EH_NO_ANCHOR (Open Watcom bug C10).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_EH_ANCHOR_H
#define _CCW_EH_ANCHOR_H
#include <ccwrap_common.h>
#include "../../detail/llibcxx03/include/__exception/eh_anchor_all.h"
#endif
