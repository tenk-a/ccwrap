// bugchk-id:     A40
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   the literal -0.0 is folded to +0.0, and printf drops a negative zero's sign
//
// Two separate defects, both about negative zero:
//
//   1. CONSTANT FOLDING. `double a = -0.0;` produces a value whose sign bit is CLEAR.
//      A negative zero built at runtime (negating a volatile zero, multiplying by a
//      negative, underflowing) is correct, so it is the compile-time path alone.
//
//   2. printf. Handed a value whose sign bit IS set, "%f" prints "0.000000" rather
//      than "-0.000000".
//
// Hit through std::to_chars(buf, end, -0.0), which owes "-0". ccwrap's <charconv>
// works around (2) by reading the sign bit itself instead of trusting printf; (1) is
// not workable around -- the negative zero never reaches the library.
//
// Raw compiler only -- no ccwrap headers.
#include <stdio.h>
#include <string.h>

static int neg(double v)
{
    union { double d; unsigned char b[8]; } u;
    u.d = v;
    return (u.b[7] & 0x80) != 0;        /* little-endian x86 */
}

int main(void)
{
    int bad = 0;

    /* (1) the literal */
    double a = -0.0;
    printf("literal -0.0    : signbit=%d (want 1)\n", neg(a));
    if (!neg(a)) { puts("BROKEN: the literal -0.0 was folded to +0.0"); bad = 1; }

    /* control: a runtime negative zero must be fine */
    {
        volatile double z = 0.0;
        double c = -z;
        printf("runtime -z      : signbit=%d (want 1)\n", neg(c));
        if (!neg(c)) { puts("BROKEN: even a runtime negative zero is lost"); bad = 1; }
    }

    /* (2) printf, fed a genuine negative zero via its bit pattern */
    {
        unsigned char raw[8] = { 0, 0, 0, 0, 0, 0, 0, 0x80 };
        double g;
        char   out[64];
        memcpy(&g, raw, 8);
        sprintf(out, "%f", g);
        printf("printf(-0.0)    : \"%s\" (want \"-0.000000\")\n", out);
        if (out[0] != '-') { puts("BROKEN: printf dropped the negative zero's sign"); bad = 1; }
    }

    if (!bad) puts("ok: negative zero survives both folding and printf");
    return bad;
}
