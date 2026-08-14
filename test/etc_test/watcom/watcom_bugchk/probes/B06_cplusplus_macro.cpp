// bugchk-id:     B06
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   __cplusplus is 1, not 199711L, so it cannot be used for version detection (use __WATCOMC__)
#include <stdio.h>

int main() {
    long v = (long)__cplusplus;
    int ok = (v >= 199711L);
    printf("B06 __cplusplus = %ld (C++03 requires 199711) -> %s\n", v, ok ? "OK" : "BROKEN");
    return ok ? 0 : 1;
}
