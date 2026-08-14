// ccwrap watcom: <inttypes.h>. Open Watcom ships one, so this only forwards to it
// (via ../h on the INCLUDE path) and adds nothing of its own.
//
// A wrapper exists even though it supplements nothing, so that putting watcom/std on the
// include path covers every header the toolchain has -- a name that resolves here for
// some compilers and falls through to the native search for others is the kind of
// difference that only shows up on one target.
#pragma once

#include <ccwrap_common.h>
#include _CCW_NATIVE_C_HEADER_PATH(inttypes.h)
