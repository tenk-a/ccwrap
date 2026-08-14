/* bugchk-id:     A34
 * bugchk-kind:   run
 * bugchk-expect: bug
 * bugchk-desc:   a u64 sum of two compile-time-constant negatives passed into an inlined callee becomes 0
 *
 * Sibling of A26 (64-bit constant folding). Here the VALUE is destroyed, not a zero
 * test: computing `(unsigned long long)a + (unsigned long long)b` inside a small static
 * function, where BOTH a and b are compile-time constants and the mathematical sum is
 * NEGATIVE (so the u64 has its high bits set), then handing that value to another
 * static function, delivers 0 instead.
 *
 *   const operands, negative sum -> BROKEN (0)
 *   const operands, positive sum -> fine
 *   any runtime operand          -> fine
 *
 * Found in detail/stdckdint_c.h: ckd_add(&sc, -128, -1) stored 0 instead of the wrapped
 * 127, while ckd_add(&sc, 127, 1) stored the correct -128, and the same call with a
 * runtime operand was correct too.
 *
 * Raw compiler only -- no ccwrap headers.
 */
#include <stdio.h>

typedef long long          i64;
typedef unsigned long long u64;

/* The trigger is `inline` on the function that does the 64-bit wrap, NOT the calling
   convention: measured, `inline` reproduces under __watcall / __cdecl / the default
   alike, and a plain non-inline `static` is correct under all three. __watcall is kept
   here only because that is the shape ccwrap's _CCW_F_DEF emits in header-only C mode. */
static __inline i64 min_for_size(unsigned int size)
{
    switch (size) {
    case 1:  return -128;
    case 2:  return -32768;
    case 4:  return (-2147483647 - 1);
    default: return (-9223372036854775807LL - 1LL);
    }
}
static __inline i64 max_for_size(unsigned int size)
{
    switch (size) {
    case 1:  return 127;
    case 2:  return 32767;
    case 4:  return 2147483647;
    default: return 9223372036854775807LL;
    }
}

static inline void __watcall store(void *result, unsigned int size, u64 value)
{
    unsigned int i;
    unsigned char *p = (unsigned char *)result;
    for (i = 0; i < size; ++i) { p[i] = (unsigned char)(value & 0xff); value >>= 8; }
}

static inline int __watcall wrap_add(void *result, unsigned int size, i64 a, i64 b)
{
    i64 minv = min_for_size(size);
    i64 maxv = max_for_size(size);
    int overflow = (b > 0 && a > maxv - b) || (b < 0 && a < minv - b);
    u64 wrapped = (u64)a + (u64)b;

    store(result, size, wrapped);
    return overflow;
}

static volatile int rt_m128 = -128;
static volatile int rt_m1   = -1;

int main(void)
{
    signed char c_neg = 0, c_pos = 0, c_run = 0;
    int bad = 0;

    wrap_add(&c_neg, 1, -128, -1);                     /* -129 -> wraps to 127 */
    wrap_add(&c_pos, 1, 127, 1);                       /*  128 -> wraps to -128 */
    wrap_add(&c_run, 1, (i64)rt_m128, (i64)rt_m1);     /* same as c_neg, at run time */

    printf("const negative sum: %d (want 127)\n", (int)c_neg);
    printf("const positive sum: %d (want -128)\n", (int)c_pos);
    printf("runtime operands  : %d (want 127)\n", (int)c_run);

    if ((int)c_neg != 127) {
        puts("BROKEN: constant-folded u64 sum with a negative result delivered the wrong value");
        bad = 1;
    }
    if ((int)c_pos != -128) { puts("BROKEN: constant-folded positive sum is wrong too"); bad = 1; }
    if ((int)c_run != 127)  { puts("BROKEN: even the runtime path is wrong"); bad = 1; }
    if (!bad) puts("ok: constant folding preserves the value");
    return bad;
}
