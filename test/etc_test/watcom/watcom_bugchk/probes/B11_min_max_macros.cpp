// bugchk-id:     B11
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   __min / __max are function-like macros in the native headers, so they cannot be identifiers
#include <stdlib.h>

// libcxx03's regex had to rename these to __rmin / __rmax.
static int __min(int a, int b) { return a < b ? a : b; }
static int __max(int a, int b) { return a > b ? a : b; }

int main() { return (__min(1, 2) == 1 && __max(1, 2) == 2) ? 0 : 1; }
