#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>

static int g_fail = 0;

static void check(int cond, const char *what)
{
    if (!cond) { printf("locale_utf8: %s failed\n", what); g_fail = 1; }
}

static const char k_a[]     = { (char)0xE3, (char)0x81, (char)0x82, 0 };
static const char k_emoji[] = { (char)0xF0, (char)0x9F, (char)0x98, (char)0x80, 0 };

static void test_default(void)
{
    check(setlocale(LC_ALL, "C") != NULL, "setlocale(C)");
    check(strcmp(setlocale(LC_ALL, NULL), "C") == 0, "query returns C");
    check(_ccw_locale_is_utf8() == 0, "C locale is not utf8");
    check(_ccw_locale_codepage() == _ccw_locale_default_codepage(), "C locale uses the default code page");
    check(setlocale(LC_ALL, "no-such-locale-@@@") == NULL, "unknown language rejected");
    check(setlocale(LC_ALL, ".no-such-codeset") == NULL, "unknown codeset rejected");
    check(setlocale(LC_ALL, ".99999") == NULL, "invalid code page rejected");
    check(strcmp(setlocale(LC_ALL, NULL), "C") == 0, "a rejected request leaves the locale alone");
}

static void test_select_utf8(void)
{
    check(setlocale(LC_ALL, ".UTF-8") != NULL, "setlocale(.UTF-8)");
    check(_ccw_locale_codepage() == 65001u, ".UTF-8 selects code page 65001");
    check(setlocale(LC_ALL, "C") != NULL, "back to C");
    check(setlocale(LC_ALL, ".utf8") != NULL, "setlocale(.utf8)");
    check(_ccw_locale_is_utf8(), ".utf8 selects utf8");
    check(setlocale(LC_ALL, "C") != NULL, "back to C");
    check(setlocale(LC_ALL, ".65001") != NULL, "setlocale(.65001)");
    check(_ccw_locale_is_utf8(), ".65001 selects utf8");
    check(setlocale(LC_ALL, "C") != NULL, "back to C");
    check(setlocale(LC_CTYPE, "ja_JP.UTF-8") != NULL, "a language part is ignored when a codeset is given");
    check(_ccw_locale_is_utf8(), "ja_JP.UTF-8 selects utf8");
    check(strcmp(setlocale(LC_ALL, NULL), "C.UTF-8") == 0, "query reports C.UTF-8");
    check(MB_CUR_MAX == 4, "MB_CUR_MAX is 4 for utf8");
}

static void test_utf8_conversions(void)
{
    wchar_t   w[8];
    char      b[16];
    mbstate_t st;
    size_t    r;
    wchar_t   wc = 0;
    const char *p;

    check(setlocale(LC_ALL, ".UTF-8") != NULL, "setlocale(.UTF-8)");

    check(mbtowc(&wc, k_a, 4) == 3 && wc == 0x3042, "mbtowc decodes a 3-byte sequence");
    check(mblen(k_a, 4) == 3, "mblen counts a 3-byte sequence");
    check(mbtowc(&wc, k_a, 2) == -1, "mbtowc rejects a truncated sequence");

    r = mbstowcs(w, k_a, 8);
    check(r == 1 && w[0] == 0x3042 && w[1] == 0, "mbstowcs converts and terminates");

    r = mbstowcs(w, k_emoji, 8);
    check(r == 2 && w[0] == 0xD83D && w[1] == 0xDE00, "mbstowcs makes a surrogate pair");

    memset(b, 0x5A, sizeof b);
    r = wcstombs(b, w, sizeof b);
    check(r == 4 && memcmp(b, k_emoji, 4) == 0 && b[4] == 0, "wcstombs encodes a surrogate pair");

    memset(&st, 0, sizeof st);
    r = mbrtowc(&wc, k_a, 1, &st);
    check(r == (size_t)-2, "mbrtowc reports an incomplete sequence");
    check(!mbsinit(&st), "the partial sequence is kept in mbstate_t");
    r = mbrtowc(&wc, k_a + 1, 3, &st);
    check(r == 2 && wc == 0x3042, "mbrtowc resumes from the kept state");
    check(mbsinit(&st), "mbstate_t is back to the initial state");

    memset(&st, 0, sizeof st);
    memset(w, 0x5A, sizeof w);
    p = k_a;
    r = mbsrtowcs(w, &p, 8, &st);
    check(r == 1 && w[0] == 0x3042 && w[1] == 0, "mbsrtowcs stores the terminating null");
    check(p == NULL, "mbsrtowcs clears src when it reaches the end");

    check(wctob(WEOF) == EOF, "wctob(WEOF) is EOF");
    check(wctob((wint_t)'A') == 'A', "wctob passes ASCII through");
    check(btowc('A') == (wint_t)'A', "btowc passes ASCII through");
    check(btowc(0xE3) == WEOF, "btowc rejects a lead byte");

    memset(&st, 0, sizeof st);
    b[0] = 0;
    r = wcrtomb(b, (wchar_t)0xD83D, &st);
    check(r == 0 && !mbsinit(&st), "wcrtomb holds a high surrogate");
    r = wcrtomb(b, (wchar_t)0xDE00, &st);
    check(r == 4 && memcmp(b, k_emoji, 4) == 0, "wcrtomb emits the pair as 4 bytes");
    check(mbsinit(&st), "mbstate_t is back to the initial state");
}

static void test_select_dbcs(void)
{
    wchar_t w[8];
    char    b[16];
    size_t  r;
    static const char sjis_a[] = { (char)0x82, (char)0xA0, 0 };

    if (setlocale(LC_ALL, ".932") == NULL) {
        printf("locale_utf8: code page 932 not installed, skipping the dbcs part\n");
        return;
    }
    check(_ccw_locale_codepage() == 932u, ".932 selects code page 932");
    check(mbtowc(w, sjis_a, 2) == 2 && w[0] == 0x3042, "dbcs mbtowc decodes a lead+trail pair");
    r = mbstowcs(w, sjis_a, 8);
    check(r == 1 && w[0] == 0x3042, "dbcs mbstowcs converts");
    r = wcstombs(b, w, sizeof b);
    check(r == 2 && memcmp(b, sjis_a, 2) == 0, "dbcs wcstombs converts back");
}

int main(void)
{
    test_default();
    test_select_utf8();
    test_utf8_conversions();
    test_select_dbcs();
    setlocale(LC_ALL, "C");
    if (g_fail) return 1;
    printf("locale_utf8: ok\n");
    return 0;
}
