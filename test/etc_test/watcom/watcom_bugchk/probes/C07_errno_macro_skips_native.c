/* bugchk-id:     C07
 * bugchk-kind:   run
 * bugchk-expect: bug
 * bugchk-desc:   native <stdlib.h> defines the errno macro without any E* constant
 *
 * C7: "`errno` is a macro, therefore <errno.h> has been read" -- FALSE.
 *
 * Native <stdlib.h> (and anything pulling <_comdef.h>) defines the `errno` macro and
 * _ERRNO_DEFINED all by itself, but none of the E* constants. ccwrap's <errno.h> used to
 * key its native include off `#ifndef errno`, so this include order silently lost every
 * error number (EDOM/ERANGE/EILSEQ/...). Found via <cinttypes> -> <cstdlib> before <cuchar>.
 *
 * Raw compiler only, no ccwrap headers. `bug` here is the *premise* the old guard got
 * wrong, so it stays `bug` on a fixed toolchain too -- the probe guards that the premise
 * has not silently changed under us, and that the cure still works.
 */
#include <stdio.h>
#include <stdlib.h>     /* defines errno + _ERRNO_DEFINED, but no E* */

#if !defined(errno)
# define PREMISE "ok: <stdlib.h> no longer defines errno (premise gone)"
# define PREMISE_HOLDS 0
#elif defined(ERANGE)
# define PREMISE "ok: the errno macro now arrives with the E* constants"
# define PREMISE_HOLDS 0
#else
# define PREMISE "BROKEN: errno defined but ERANGE missing -- a guard keyed off `errno` loses them"
# define PREMISE_HOLDS 1
#endif

/* The cure ccwrap's watcom/errno.h uses: key off the native include guard, and hide
   errno/_ERRNO_DEFINED across the include (the computed include path would otherwise
   macro-expand `errno`; and without undef'ing _ERRNO_DEFINED the native header would
   not re-form the macro). */
#ifndef _ERRNO_H_INCLUDED
# ifdef errno
#  undef errno
#  undef _ERRNO_DEFINED
# endif
# include <errno.h>
#endif

/* Exit code is the verdict: non-zero = the defect is present (bugchk-expect: bug). */
int main(void)
{
    int cure_ok;
    puts(PREMISE);
#if defined(errno) && defined(ERANGE) && defined(EILSEQ)
    errno = 0;
    printf("cure ok: ERANGE=%d EILSEQ=%d, errno usable (%d)\n", ERANGE, EILSEQ, errno);
    cure_ok = 1;
#else
    puts("BROKEN: undef + re-include did not restore errno / the E* constants");
    cure_ok = 0;
#endif
    return (PREMISE_HOLDS || !cure_ok) ? 1 : 0;
}
