// bugchk-id:     A46
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   mbrtowc reports an incomplete sequence but leaves the conversion state initial
//
// C99 7.24.6.3.2: when the bytes examined form an incomplete but potentially valid
// multibyte character, mbrtowc returns (size_t)-2 and "the conversion state" describes
// the partial character, so a following call can finish it. 7.24.6.2.1 in turn says
// mbsinit is non-zero only for an INITIAL state.
//
// Open Watcom returns -2 correctly but keeps the state initial, so the partial byte is
// forgotten: feeding the remaining bytes afterwards restarts from scratch and the
// character is lost. Byte-at-a-time decoders (the usual reason to hold an mbstate_t)
// therefore cannot work.

#include <stdio.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
    mbstate_t st;
    wchar_t wc = 0;
    size_t k;
    int bad = 0;

    memset(&st, 0, sizeof st);
    printf("mbsinit(fresh state) = %d (want non-zero)\n", mbsinit(&st));

    k = mbrtowc(&wc, "\xE3", 1, &st);   /* first byte of a 3-byte UTF-8 character */
    printf("mbrtowc(1 of 3 bytes) -> %ld (want -2)   mbsinit = %d (want 0)\n",
           (long)k, mbsinit(&st));

    if (k == (size_t)-2 && mbsinit(&st) != 0) {
        puts("BROKEN: an incomplete sequence left the state initial");
        bad = 1;
    }
    if (!bad) puts("ok: the partial character is remembered in the state");
    return bad;
}
