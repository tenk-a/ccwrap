// ccwrap vc/ccwstd: thin wrapper -> vc/std/float.h. Only vc/ccwstd/ is on the include path, so
// the C layer still has to be reachable; it stays in std (the C library is never
// relocated -- see ccwstd/cstddef for the using-bridge that is the seam).
#include "../std/float.h"
