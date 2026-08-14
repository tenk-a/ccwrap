// ccwrap vc/ccwstd: thin wrapper -> vc/std/yvals_core.h. vc/ccwstd/ replaces vc/std/ on the include path,
// so every ccwrap vc header needs a door here or the native one would win instead
// (yvals.h / yvals_core.h especially: that is where the ccwrap prelude enters MSVC).
#include "../std/yvals_core.h"
