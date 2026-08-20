// bugchk-id:     A53
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   strtod moves endptr past leading whitespace even when no conversion happens
//
// C99 7.20.1.3p7: "If no conversion could be performed, the value of nptr is stored in
// the object pointed to by endptr."  That is the only way a caller can tell "there was
// no number here" from "the number was zero".
//
// Open Watcom stores a pointer PAST the whitespace (and past a lone sign) it examined,
// so `end != nptr` is true for a string that holds no number at all.  Code that uses the
// standard test then reads every blank line as the value 0.  strtol has the same shape
// but is not covered here.

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    static const char* t[] = { "  ", " x", "", "  12", "abc", " +", 0 };
    int i;
    int bad = 0;

    for (i = 0; t[i] != 0; ++i) {
        char*  e = (char*)0;
        double v = strtod(t[i], &e);
        long   off = (long)(e - t[i]);
        int    converts = (i == 3);
        printf("strtod(\"%s\") -> %g  endptr offset %ld%s\n",
               t[i], v, off, (!converts && off != 0) ? "   (want 0)" : "");
        if (!converts && off != 0)
            bad = 1;
    }
    if (bad)
        puts("BROKEN: endptr advanced although no conversion was performed");
    else
        puts("ok: endptr is left at nptr when there is no number");
    return bad;
}
