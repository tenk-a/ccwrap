// bugchk-id:     A44
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   wide formatted I/O does not convert between narrow and wide for %s/%c/%[...]
//
// C99 7.24.2 says the `s`, `c` and `[` conversions convert between multibyte characters
// and wide characters in BOTH directions: a narrow %s inside a wide format is widened,
// and a narrow %s inside a wide *scanf* is narrowed back. Open Watcom instead copies the
// raw storage units, so the destination ends up holding the wrong kind of character:
//
//   swprintf(w, n, L"%s", "bc")  ->  w[0] == 0x6362   (two bytes packed into one wchar_t)
//   swscanf(L"abcdefgh", L"%5s", n)  ->  n == "a"     (wide units written into a char[])
//   swscanf(L"abcx", L"%3l[abc]", w) ->  w[0] == 0x6261
//
// The opposite direction (a wide %ls inside a NARROW format) is correct, which is what
// makes this easy to miss: testing only sprintf("%ls", ...) says everything is fine.
//
// Also checked here: swscanf has no %a/%A hexadecimal-float conversion (C99 7.24.2.2).

#include <stdio.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
    wchar_t w[32];
    char n[32];
    double v;
    int bad = 0, r;

    memset(w, 0, sizeof w);
    r = swprintf(w, 32, L"%s", "bc");
    printf("swprintf L\"%%s\", \"bc\"     -> r=%d  w = %u %u\n", r,
           (unsigned)w[0], (unsigned)w[1]);
    if (r != 2 || w[0] != L'b' || w[1] != L'c') {
        puts("BROKEN: a narrow %s in a wide format was not widened");
        bad = 1;
    }

    memset(w, 0, sizeof w);
    r = swscanf(L"abcx", L"%3l[abc]", w);
    printf("swscanf L\"%%3l[abc]\"      -> r=%d  w = %u %u %u\n", r,
           (unsigned)w[0], (unsigned)w[1], (unsigned)w[2]);
    if (r != 1 || w[0] != L'a' || w[1] != L'b' || w[2] != L'c') {
        puts("BROKEN: the wide scanset did not widen its characters");
        bad = 1;
    }

    memset(n, 0, sizeof n);
    r = swscanf(L"abcdefgh", L"%5s", n);
    printf("swscanf L\"%%5s\"           -> r=%d  n = \"%s\"\n", r, n);
    if (r != 1 || strcmp(n, "abcde") != 0) {
        puts("BROKEN: a narrow %s in a wide scanf was not narrowed");
        bad = 1;
    }

    v = 0.0;
    r = swscanf(L"0x1p+4", L"%la", &v);
    printf("swscanf L\"%%la\"           -> r=%d  v = %g\n", r, v);
    if (r != 1 || v != 16.0) {
        puts("BROKEN: swscanf has no hexadecimal-float conversion");
        bad = 1;
    }

    /* the direction that does work, kept as the control */
    memset(n, 0, sizeof n);
    r = sprintf(n, "%ls", L"Abc");
    printf("sprintf \"%%ls\", L\"Abc\"    -> r=%d  n = \"%s\"\n", r, n);
    if (r != 3 || strcmp(n, "Abc") != 0) {
        puts("BROKEN: even the narrowing direction is wrong now");
        bad = 1;
    }

    if (!bad) puts("ok: wide formatted I/O converts in both directions");
    return bad;
}
