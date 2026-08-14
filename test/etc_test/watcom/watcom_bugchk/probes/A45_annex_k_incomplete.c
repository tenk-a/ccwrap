// bugchk-id:     A45
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   __STDC_LIB_EXT1__ is advertised but getenv_s always fails (and memset_s does not link)
//
// Open Watcom's <_preincl.h> defines __STDC_LIB_EXT1__ as 200509L, which tells a
// portable program that Annex K is present. Two things are wrong with that claim:
//
//   1. getenv_s returns a non-zero value even when the variable exists and was copied.
//      K.3.6.2.1 says it returns zero on success. The length is written to *len, so the
//      call clearly did the work -- only the status is wrong. There is no way for a
//      caller to tell success from failure.
//
//   2. memset_s is declared by <string.h> but has no definition in the runtime, so a
//      program that calls it fails at LINK time (E2028). That half cannot be probed
//      here without breaking the build, so it is only recorded in the catalogue.
//
// Note the declarations need __STDC_WANT_LIB_EXT1__ before any include (K.3.1.1);
// without it the names are hidden even though the macro above says they exist.

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int bad = 0;

#if !defined(__STDC_LIB_EXT1__)
    puts("ok: this library does not claim Annex K");
#else
    {
        char buf[64];
        size_t got = 12345;
        int r;

        printf("__STDC_LIB_EXT1__ = %ldL\n", (long)__STDC_LIB_EXT1__);

        r = (int)getenv_s(&got, buf, sizeof buf, "PATH");
        printf("getenv_s(\"PATH\")        -> r=%d  len=%lu\n", r, (unsigned long)got);
        if (r != 0 && got != 0) {
            puts("BROKEN: getenv_s reported failure although it produced a length");
            bad = 1;
        }

        got = 12345;
        r = (int)getenv_s(&got, buf, sizeof buf, "NO_SUCH_VARIABLE_XYZ");
        printf("getenv_s(missing)       -> r=%d  len=%lu\n", r, (unsigned long)got);
        if (got != 0) {
            puts("BROKEN: a missing variable must set *len to zero");
            bad = 1;
        }
    }
    if (!bad) puts("ok: the Annex K entry points behave as K.3 specifies");
#endif
    return bad;
}
