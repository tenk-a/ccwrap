// bugchk-id:     A09
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   x != x and trichotomy-based NaN tests are folded away (total order assumed)
#include <stdio.h>

int main() {
    union { unsigned long long u; double d; } x;
    x.u = 0x7FF8000000000000ULL;              // quiet NaN, built by bit pattern
    double nan = x.d;

    int self_ne     = (nan != nan);                        // want 1
    int unordered   = (!(nan >= 0.0) && !(nan < 0.0));     // want 1
    int ok = self_ne && unordered;

    printf("A09 nan != nan          : %d (want 1) -> %s\n", self_ne,   self_ne   ? "OK" : "BROKEN");
    printf("A09 !(n>=0) && !(n<0)   : %d (want 1) -> %s\n", unordered, unordered ? "OK" : "BROKEN");
    return ok ? 0 : 1;
}
