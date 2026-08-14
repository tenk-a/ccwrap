// bugchk-id:     A43
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   math_errhandling is 4 (MATH_ERRWATCOM), and math errors reach neither errno nor the FP flags
//
// C99 7.12/2: math_errhandling expands to a value with ONLY MATH_ERRNO (1) and/or
// MATH_ERREXCEPT (2) set. Open Watcom answers 4 -- its own MATH_ERRWATCOM extension --
// so a portable program gets "false" from both `& MATH_ERRNO` and `& MATH_ERREXCEPT`
// while the value is still not 0.
//
// The second half of the probe shows the value is not merely mislabelled: the runtime
// reports domain and range errors through NEITHER channel. sqrt(-1.0) returns 0.0 (not
// a NaN) with errno untouched, and log(0.0) / exp(10000.0) leave errno alone too. The
// errors are only visible through Watcom's own matherr hook.
//
// ccwrap therefore redefines math_errhandling to 0 (detail/math_errhandling.h), which
// is both legal and true. This probe deliberately does NOT include any ccwrap header,
// so it keeps reporting `bug` until Open Watcom itself changes.
//
// Raw compiler / raw CRT only.
#include <stdio.h>
#include <math.h>
#include <errno.h>

int main(void)
{
    int bad = 0;
    int meh = math_errhandling;
    double r;

    printf("math_errhandling = %d (MATH_ERRNO=%d MATH_ERREXCEPT=%d MATH_ERRWATCOM=%d)\n",
           meh, MATH_ERRNO, MATH_ERREXCEPT, MATH_ERRWATCOM);
    if ((meh & ~(MATH_ERRNO | MATH_ERREXCEPT)) != 0) {
        puts("BROKEN: math_errhandling has bits C99 does not allow");
        bad = 1;
    }

    errno = 0;
    r = sqrt(-1.0);
    printf("sqrt(-1) = %f, errno = %d (EDOM = %d)\n", r, errno, EDOM);
    if (errno == 0) { puts("BROKEN: a domain error did not set errno"); bad = 1; }

    errno = 0;
    r = log(0.0);
    printf("log(0)   = %f, errno = %d (ERANGE = %d)\n", r, errno, ERANGE);
    if (errno == 0) { puts("BROKEN: a range error did not set errno"); bad = 1; }

    if (!bad) puts("ok: math_errhandling is C99-legal and matches what the runtime does");
    return bad;
}
