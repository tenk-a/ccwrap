// bugchk-id:     A44b
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   the NARROW scanf has no %a/%A, mis-reads hex floats with %f, and %l[ is raw
//
// A44 records the wide side. These three are the same family but on the NARROW side, and
// they matter because the obvious repair for A44 is "narrow the format, call the narrow
// function, widen the result" -- a round trip that only works for what the narrow side
// gets right.
//
//   sscanf("0x1p+4", "%la", &v)   ->  0  (no conversion at all: even "16.0" returns 0)
//   sscanf("0x1p+4", "%lf", &v)   ->  1, v == 0   (C99 7.21.6.2 says %f takes strtod's
//                                      forms, which include hexadecimal since C99)
//   sscanf("abcx",   "%l[abc]", w)->  w[0] == 0x6261   (two bytes packed into a wchar_t)
//
// %ls and %lc in a narrow format ARE correct, and printf's %a is correct, which is what
// makes this easy to miss.

#include <stdio.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
    wchar_t w[32];
    char n[32];
    double v;
    int bad = 0, r;

    v = -1.0;
    r = sscanf("0x1p+4", "%la", &v);
    printf("sscanf \"%%la\", \"0x1p+4\"   -> r=%d  v = %g\n", r, v);
    if (r != 1 || v != 16.0) {
        puts("BROKEN: the narrow scanf has no hexadecimal-float conversion");
        bad = 1;
    }

    v = -1.0;
    r = sscanf("0x1p+4", "%lf", &v);
    printf("sscanf \"%%lf\", \"0x1p+4\"   -> r=%d  v = %g\n", r, v);
    if (r != 1 || v != 16.0) {
        puts("BROKEN: %f did not accept the hexadecimal form strtod takes");
        bad = 1;
    }

    memset(w, 0, sizeof w);
    r = sscanf("abcx", "%l[abc]", w);
    printf("sscanf \"%%l[abc]\"         -> r=%d  w = %u %u %u\n", r,
           (unsigned)w[0], (unsigned)w[1], (unsigned)w[2]);
    if (r != 1 || w[0] != L'a' || w[1] != L'b' || w[2] != L'c') {
        puts("BROKEN: a narrow scanset with l wrote raw bytes, not wide characters");
        bad = 1;
    }

    /* the parts that do work, kept as the control */
    memset(w, 0, sizeof w);
    r = sscanf("abc", "%ls", w);
    if (r != 1 || w[0] != L'a' || w[2] != L'c' || w[3] != 0) {
        puts("BROKEN: even %ls is wrong now");
        bad = 1;
    }
    memset(n, 0, sizeof n);
    r = sprintf(n, "%a", 16.0);
    if (r < 1 || n[0] != '0' || n[1] != 'x') {
        puts("BROKEN: even printf's %a is wrong now");
        bad = 1;
    }

    if (!bad) puts("ok: the narrow scanf handles %a, hex %f and %l[");
    return bad;
}
