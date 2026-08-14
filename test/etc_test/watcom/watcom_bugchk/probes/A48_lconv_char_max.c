// bugchk-id:     A48
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   localeconv() fills struct lconv with 127 although CHAR_MAX is 255
//
// C99 7.11.2.1: in the "C" locale, every member of struct lconv that is not available
// has the value CHAR_MAX. Open Watcom's `char` is UNSIGNED by default, so CHAR_MAX is
// 255 in ordinary code -- but the CRT was built with signed char and stores 127. A
// program that follows the standard and compares against CHAR_MAX therefore never
// matches, with no way to tell "unavailable" from a real setting.
//
// Raw compiler / raw CRT only.
#include <stdio.h>
#include <locale.h>
#include <limits.h>

int main(void)
{
    struct lconv* lc;
    int bad = 0;

    setlocale(LC_ALL, "C");
    lc = localeconv();
    printf("CHAR_MAX = %d, frac_digits = %d, p_cs_precedes = %d\n",
           (int)CHAR_MAX, (int)lc->frac_digits, (int)lc->p_cs_precedes);

    if ((int)lc->frac_digits != (int)CHAR_MAX) {
        puts("BROKEN: frac_digits is not CHAR_MAX in the C locale");
        bad = 1;
    }
    if ((int)lc->p_cs_precedes != (int)CHAR_MAX) {
        puts("BROKEN: p_cs_precedes is not CHAR_MAX in the C locale");
        bad = 1;
    }

    if (!bad) puts("ok: the unavailable lconv members really are CHAR_MAX");
    return bad;
}
