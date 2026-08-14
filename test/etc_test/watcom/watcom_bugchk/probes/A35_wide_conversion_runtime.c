/* bugchk-id:     A35
 * bugchk-kind:   run
 * bugchk-expect: bug
 * bugchk-desc:   wctob(WEOF) returns '?' instead of EOF, and mbsrtowcs does not null-terminate
 *
 * Two Open Watcom C runtime bugs in the wide-character conversions. Raw compiler and raw
 * CRT -- no ccwrap headers, so the wrapper is not what is broken.
 *
 * 1. wctob(WEOF)
 *    C99 7.24.6.1.2: "returns EOF if c does not correspond to a single-byte character in
 *    the initial shift state" -- and WEOF never does. Open Watcom returns 63, i.e. '?',
 *    the same value it uses for unconvertible characters, so a caller cannot tell
 *    "unconvertible" from "the character '?'" either.
 *
 * 2. mbsrtowcs(dst, &src, len, &st)
 *    C99 7.24.6.4.1: when conversion stops because the terminating null was reached, a
 *    null wide character is stored in dst. Open Watcom converts the characters and sets
 *    *src to NULL (so it claims it consumed the terminator) but leaves dst[n] untouched
 *    -- the destination is not a valid wide string.
 *
 * Found because test/cxx_test/cwchar_test.cpp's "merged" block had been silently disabled
 * on Watcom by an `#if __cplusplus >= 201103L` gate (Watcom's __cplusplus is 1, bug B6);
 * enabling it surfaced both at once.
 */
#include <stdio.h>
#include <wchar.h>
#include <string.h>

int main(void)
{
    mbstate_t st;
    const char *src = "wide";
    const char *p;
    wchar_t wbuf[16];
    size_t n;
    int bad = 0;
    int r;

    r = wctob(WEOF);
    printf("wctob(WEOF) = %d (want %d)\n", r, EOF);
    if (r != EOF) { puts("BROKEN: wctob(WEOF) is not EOF"); bad = 1; }

    memset(&st, 0, sizeof st);
    memset(wbuf, 0x5A, sizeof wbuf);        /* poison, so a missing null shows up */
    p = src;
    n = mbsrtowcs(wbuf, &p, 16, &st);
    printf("mbsrtowcs -> n=%u, src consumed=%s, wbuf[4]=%d (want 0)\n",
           (unsigned)n, (p == NULL ? "yes" : "no"), (int)wbuf[4]);
    if (n == 4 && p == NULL && wbuf[4] != 0) {
        puts("BROKEN: mbsrtowcs consumed the terminator but stored no null wide character");
        bad = 1;
    }

    if (!bad) puts("ok: both conversions behave as C99 requires");
    return bad;
}
