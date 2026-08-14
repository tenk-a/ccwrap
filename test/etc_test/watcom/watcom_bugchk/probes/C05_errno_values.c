/* bugchk-id:     C05
 * bugchk-kind:   run
 * bugchk-expect: bug
 * bugchk-desc:   errno numbers are Watcom's own, not POSIX/MSVC (ENOENT=1, EPERM=28, EINVAL=9 ...)
 *
 * Not a defect the compiler will "fix" -- it is an ABI choice, and it is recorded here so a
 * change would be noticed. The lesson it encodes: never hard-code an errno value. std::errc
 * enumerators must be spelled with the E* macros, or every errc silently means another error.
 * (A test that asserted errc::invalid_argument == 22 did not catch that; it agreed with the
 * implementation's identical mistake.)
 */
#include <stdio.h>
#include <errno.h>

int main(void) {
    struct { const char* name; int got; int posix; } t[] = {
        { "ENOENT", ENOENT,  2 },
        { "EPERM",  EPERM,   1 },
        { "ESRCH",  ESRCH,   3 },
        { "EINVAL", EINVAL, 22 },
        { "EFAULT", EFAULT, 14 },
        { "EACCES", EACCES, 13 }
    };
    int i, bad = 0;
    for (i = 0; i < 6; ++i) {
        if (t[i].got != t[i].posix) {
            ++bad;
            printf("C05 %-7s = %2d, POSIX/MSVC says %2d -> BROKEN\n", t[i].name, t[i].got, t[i].posix);
        }
    }
    printf("C05 errno numbering: %d/6 differ from POSIX -> %s\n", bad, bad == 0 ? "OK" : "BROKEN");
    return bad == 0 ? 0 : 1;
}
