// bugchk-id:     A26
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   with optimisation off, a zero test on a constant-folded 64-bit & lies (detail/bit_impl.h)
#include <stdio.h>

typedef unsigned long long u64;

int main() {
    u64 v = (u64)1 << 40;                 /* 0x0000010000000000 */
    u64 m = 0xffffffff00000000ULL;
    /* v & m == 0x0000010000000000, which is plainly not zero. */

    int a = ((v & m) == 0);               /* want 0 */
    int b = ((v & m) != 0);               /* want 1 */
    int c = (!(v & m));                   /* want 0 */
    int d = 0; if (v & m) d = 1;          /* want 1 */

    u64 t = v & m; int via_var = (t == 0);            /* the documented workaround: want 0 */
    int nonzero_cmp = ((v & m) == 0x0000010000000000ULL); /* want 1: only the ZERO test is wrong */
    int gt = ((v & m) > 0);                           /* want 1 */

    int ok = (a == 0) && (b == 1) && (c == 0) && (d == 1);
    if (a != 0) printf("A26 (v&m)==0 : %d want 0 -> BROKEN\n", a);
    if (b != 1) printf("A26 (v&m)!=0 : %d want 1 -> BROKEN\n", b);
    if (c != 0) printf("A26 !(v&m)   : %d want 0 -> BROKEN\n", c);
    if (d != 1) printf("A26 if(v&m)  : %d want 1 -> BROKEN\n", d);
    printf("A26 (workaround) via variable: %d want 0 -> %s\n", via_var, via_var == 0 ? "OK" : "ALSO BROKEN");
    printf("A26 (context) nonzero cmp: %d, >0: %d (both want 1)\n", nonzero_cmp, gt);
    printf("A26 64-bit & zero test -> %s\n", ok ? "OK" : "BROKEN");
    return ok ? 0 : 1;
}
