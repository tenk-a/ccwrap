#include "test_cxx.hpp"
#include <cwchar>

TEST_CASE(cwchar, header_compiles) { test_true( true );  test_pass("cxx03:<cwchar> header compiles"); }

#if TEST_TARGET_CXX >= 2011
#include <cwchar>
#include <climits>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstddef>
#include <clocale>
#include <ctime>
#include <string>
#include <type_traits>

using _ccw::string_view;

static int vfw(STD::FILE* f, const wchar_t* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int n = STD_NS vfwprintf(f, fmt, ap);
    va_end(ap);
    return n;
}
static int vsw(wchar_t* buf, STD_NS size_t sz, const wchar_t* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int n = STD_NS vswprintf(buf, sz, fmt, ap);
    va_end(ap);
    return n;
}
static int vfs(STD::FILE* f, const wchar_t* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int n = STD_NS vfwscanf(f, fmt, ap);
    va_end(ap);
    return n;
}
static int vss(const wchar_t* s, const wchar_t* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int n = STD_NS vswscanf(s, fmt, ap);
    va_end(ap);
    return n;
}

TEST_CASE(cwchar, types_and_macros) {

    STD_NS size_t n = sizeof(wchar_t);
    test_gt( n, STD_NS size_t(0) );
    test_true( STD::is_unsigned<STD_NS size_t>::value );
    test_pass("cxx03:wchar_t");

    STD_NS mbstate_t st;
    STD::memset(&st, 0, sizeof st);
    test_gt( sizeof(STD_NS mbstate_t), STD_NS size_t(0) );
    test_pass("cxx03:mbstate_t");

    STD_NS wint_t wi = STD_NS wint_t(L'a');
    test_eq( (long)wi, (long)L'a' );
    test_ge( sizeof(STD_NS wint_t), sizeof(wchar_t) );
    test_pass("cxx03:wint_t");

    STD_NS tm* tp = NULL;
    test_true( tp == NULL );
    test_pass("cxx03:struct tm");

    wchar_t* wp = NULL;
    test_true( wp == NULL );
    test_pass("cxx03:NULL");

    STD_NS wint_t eof = WEOF;
    test_ne( (long)eof, (long)L'a' );
    test_ne( (long)eof, 0L );
    test_pass("cxx03:WEOF");

    test_le( (long)WCHAR_MIN, 0L );
    test_pass("cxx03:WCHAR_MIN");
    test_ge( (long)WCHAR_MAX, 0xFFFFL );
    test_lim( (long)L'A', (long)WCHAR_MIN, (long)WCHAR_MAX );
    test_eq( (long)WCHAR_MAX, (long)(wchar_t)WCHAR_MAX );
    test_pass("cxx03:WCHAR_MAX");
}

TEST_CASE(cwchar, wcslen_copy_cat) {
    test_eq( STD_NS wcslen(L""), STD_NS size_t(0) );
    test_eq( STD_NS wcslen(L"hello"), STD_NS size_t(5) );
    test_pass("cxx03:wcslen");

    wchar_t buf[32];
    wchar_t* r = STD_NS wcscpy(buf, L"abc");
    test_true( r == buf );
    test_true( STD_NS wcscmp(buf, L"abc") == 0 );
    test_eq( STD_NS wcslen(buf), STD_NS size_t(3) );
    test_pass("cxx03:wcscpy");

    wchar_t pad[6];
    STD_NS wmemset(pad, L'#', 6);
    wchar_t* p = STD_NS wcsncpy(pad, L"ab", 5);
    test_true( p == pad );
    test_true( STD_NS wcscmp(pad, L"ab") == 0 );
    test_eq( (int)pad[2], 0 );
    test_eq( (int)pad[4], 0 );
    test_eq( (int)pad[5], (int)L'#' );
    test_pass("cxx03:wcsncpy");

    STD_NS wcscpy(buf, L"ab");
    wchar_t* c = STD_NS wcscat(buf, L"cd");
    test_true( c == buf );
    test_true( STD_NS wcscmp(buf, L"abcd") == 0 );
    test_pass("cxx03:wcscat");

    wchar_t* c2 = STD_NS wcsncat(buf, L"efgh", 2);
    test_true( c2 == buf );
    test_true( STD_NS wcscmp(buf, L"abcdef") == 0 );
    test_eq( STD_NS wcslen(buf), STD_NS size_t(6) );
    test_pass("cxx03:wcsncat");
}

TEST_CASE(cwchar, wcscmp_and_friends) {
    test_eq( STD_NS wcscmp(L"abc", L"abc"), 0 );
    test_lt( STD_NS wcscmp(L"abc", L"abd"), 0 );
    test_gt( STD_NS wcscmp(L"abd", L"abc"), 0 );
    test_lt( STD_NS wcscmp(L"ab",  L"abc"), 0 );
    test_pass("cxx03:wcscmp");

    test_eq( STD_NS wcsncmp(L"abcX", L"abcY", 3), 0 );
    test_lt( STD_NS wcsncmp(L"abcX", L"abcY", 4), 0 );
    test_eq( STD_NS wcsncmp(L"abc",  L"abc",  0), 0 );
    test_pass("cxx03:wcsncmp");

    STD::string saved(STD::setlocale(LC_ALL, NULL));
    STD::setlocale(LC_ALL, "C");
    test_eq( STD_NS wcscoll(L"abc", L"abc"), 0 );
    test_lt( STD_NS wcscoll(L"abc", L"abd"), 0 );
    test_gt( STD_NS wcscoll(L"b",   L"a"),   0 );
    test_pass("cxx03:wcscoll");

    STD_NS size_t need = STD_NS wcsxfrm(NULL, L"abc", 0);
    test_ge( need, STD_NS size_t(3) );
    wchar_t xa[64], xb[64];
    STD_NS size_t na = STD_NS wcsxfrm(xa, L"abc", 64);
    STD_NS size_t nb = STD_NS wcsxfrm(xb, L"abd", 64);
    test_lt( na, STD_NS size_t(64) );
    test_lt( nb, STD_NS size_t(64) );
    test_lt( STD_NS wcscmp(xa, xb), 0 );
    STD::setlocale(LC_ALL, saved.c_str());
    test_pass("cxx03:wcsxfrm");
}

TEST_CASE(cwchar, search) {
    wchar_t buf[] = L"hello, world";
    const wchar_t* cbuf = buf;

    wchar_t* h = STD_NS wcschr(buf, L'o');
    test_true( h == buf + 4 );
    const wchar_t* ch = STD_NS wcschr(cbuf, L'o');
    test_true( ch == cbuf + 4 );
    test_true( STD_NS wcschr(cbuf, L'z') == NULL );
    test_true( STD_NS wcschr(cbuf, L'\0') == cbuf + 12 );
    test_pass("cxx03:wcschr");

    wchar_t* r = STD_NS wcsrchr(buf, L'o');
    test_true( r == buf + 8 );
    const wchar_t* cr = STD_NS wcsrchr(cbuf, L'l');
    test_true( cr == cbuf + 10 );
    test_true( STD_NS wcsrchr(cbuf, L'z') == NULL );
    test_pass("cxx03:wcsrchr");

    wchar_t* s = STD_NS wcsstr(buf, L"world");
    test_true( s == buf + 7 );
    const wchar_t* cs = STD_NS wcsstr(cbuf, L"lo,");
    test_true( cs == cbuf + 3 );
    test_true( STD_NS wcsstr(cbuf, L"") == cbuf );
    test_true( STD_NS wcsstr(cbuf, L"nope") == NULL );
    test_pass("cxx03:wcsstr");

    wchar_t* pb = STD_NS wcspbrk(buf, L",w");
    test_true( pb == buf + 5 );
    const wchar_t* cpb = STD_NS wcspbrk(cbuf, L"w");
    test_true( cpb == cbuf + 7 );
    test_true( STD_NS wcspbrk(cbuf, L"xyz") == NULL );
    test_pass("cxx03:wcspbrk");

    test_eq( STD_NS wcsspn(L"abcde", L"abc"), STD_NS size_t(3) );
    test_eq( STD_NS wcsspn(L"abcde", L"xyz"), STD_NS size_t(0) );
    test_pass("cxx03:wcsspn");
    test_eq( STD_NS wcscspn(L"abcde", L"dc"), STD_NS size_t(2) );
    test_eq( STD_NS wcscspn(L"abcde", L"xyz"), STD_NS size_t(5) );
    test_pass("cxx03:wcscspn");
}

TEST_CASE(cwchar, wcstok) {
    wchar_t src[] = L"a,bb;;ccc";
    wchar_t* ctx = NULL;
    wchar_t* t1 = STD_NS wcstok(src, L",;", &ctx);
    test_ptr( t1 );
    test_true( STD_NS wcscmp(t1, L"a") == 0 );

    wchar_t* t2 = STD_NS wcstok(NULL, L",;", &ctx);
    test_ptr( t2 );
    test_true( STD_NS wcscmp(t2, L"bb") == 0 );

    wchar_t* t3 = STD_NS wcstok(NULL, L",;", &ctx);
    test_ptr( t3 );
    test_true( STD_NS wcscmp(t3, L"ccc") == 0 );

    wchar_t* t4 = STD_NS wcstok(NULL, L",;", &ctx);
    test_true( t4 == NULL );
    test_pass("cxx03:wcstok");
}

TEST_CASE(cwchar, wmem) {
    wchar_t a[8] = { L'a', L'b', L'c', L'd', 0, 0, 0, 0 };
    wchar_t b[8] = { 0 };

    wchar_t* r = STD_NS wmemcpy(b, a, 4);
    test_true( r == b );
    test_eq( STD_NS wmemcmp(a, b, 4), 0 );
    test_pass("cxx03:wmemcpy");

    wchar_t ov[6] = { L'1', L'2', L'3', L'4', L'5', 0 };
    wchar_t* m = STD_NS wmemmove(ov + 1, ov, 4);
    test_true( m == ov + 1 );
    test_eq( (int)ov[0], (int)L'1' );
    test_eq( (int)ov[1], (int)L'1' );
    test_eq( (int)ov[4], (int)L'4' );
    test_pass("cxx03:wmemmove");

    test_eq( STD_NS wmemcmp(L"abc", L"abc", 3), 0 );
    test_lt( STD_NS wmemcmp(L"abc", L"abd", 3), 0 );
    test_gt( STD_NS wmemcmp(L"abd", L"abc", 3), 0 );
    test_eq( STD_NS wmemcmp(L"abc", L"xyz", 0), 0 );
    test_pass("cxx03:wmemcmp");

    wchar_t* f = STD_NS wmemchr(a, L'c', 4);
    test_true( f == a + 2 );
    const wchar_t* ca = a;
    const wchar_t* cf = STD_NS wmemchr(ca, L'd', 4);
    test_true( cf == ca + 3 );
    test_true( STD_NS wmemchr(ca, L'z', 4) == NULL );
    test_true( STD_NS wmemchr(ca, L'a', 0) == NULL );
    test_pass("cxx03:wmemchr");

    wchar_t s[5];
    wchar_t* sr = STD_NS wmemset(s, L'*', 5);
    test_true( sr == s );
    for (int i = 0; i < 5; ++i)
        test_eq( (int)s[i], (int)L'*' );
    test_pass("cxx03:wmemset");
}

TEST_CASE(cwchar, wcsto_numeric) {

    wchar_t* end = NULL;

    TEST_NOTE("wcstol/wcstoul/wcstod are C++03; wcstoll/wcstoull/wcstof/wcstold are C++11");

    long l10 = STD_NS wcstol(L"  -42xyz", &end, 10);
    test_eq( l10, -42L );
    test_true( end != NULL && *end == L'x' );
    long l16 = STD_NS wcstol(L"7f", &end, 16);
    test_eq( l16, 127L );
    long lpx = STD_NS wcstol(L"0x10", &end, 0);
    test_eq( lpx, 16L );
    long loc = STD_NS wcstol(L"010", &end, 0);
    test_eq( loc, 8L );
    long lno = STD_NS wcstol(L"zzz", &end, 10);
    test_eq( lno, 0L );
    test_true( end != NULL );
    test_pass("cxx11:wcstol");

    unsigned long umax = STD_NS wcstoul(L"4294967295", &end, 10);
    test_eq( umax, 4294967295UL );
    unsigned long uhex = STD_NS wcstoul(L"ff", &end, 16);
    test_eq( uhex, 255UL );
    test_pass("cxx11:wcstoul");

    long long ll = STD_NS wcstoll(L"-9007199254740993", &end, 10);
    test_eq( ll, -9007199254740993LL );
    test_pass("cxx11:wcstoll");
    unsigned long long ull = STD_NS wcstoull(L"18446744073709551615", &end, 10);
    test_eq( ull, 18446744073709551615ULL );
    test_pass("cxx11:wcstoull");

    double d = STD_NS wcstod(L"3.5e2rest", &end);
    test_eq( d, 350.0 );
    test_true( end != NULL && *end == L'r' );
    double dnull = STD_NS wcstod(L"-0.25", NULL);
    test_eq( dnull, -0.25 );

    const wchar_t* txt = L"abc";
    double z = STD_NS wcstod(txt, &end);
    test_eq( z, 0.0 );
    test_true( end == txt );
    test_pass("cxx11:wcstod");

    float f = STD_NS wcstof(L"1.5", &end);
    test_eq( (double)f, 1.5 );
    test_pass("cxx11:wcstof");
    long double ld = STD_NS wcstold(L"2.25", &end);
    test_eq( (double)ld, 2.25 );
    test_pass("cxx11:wcstold");
}

TEST_CASE(cwchar, swprintf_swscanf) {
    wchar_t buf[64];

    int n = STD_NS swprintf(buf, 64, L"%d-%ls-%.2f", 7, L"ab", 1.5);
    test_eq( n, 9 );
    test_true( STD_NS wcscmp(buf, L"7-ab-1.50") == 0 );
    test_eq( n, (int)STD_NS wcslen(buf) );
    test_pass("cxx03:swprintf");

    wchar_t vb[64];
    int vn = vsw(vb, 64, L"%d-%ls-%.2f", 7, L"ab", 1.5);
    test_eq( vn, n );
    test_true( STD_NS wcscmp(vb, buf) == 0 );
    test_pass("cxx03:vswprintf");

    int i = 0; double dv = 0; wchar_t ws[16] = {0};
    int got = STD_NS swscanf(L"7-ab-1.50", L"%d-%15[^-]-%lf", &i, ws, &dv);
    test_eq( got, 3 );
    test_eq( i, 7 );
    TEST_SKIP_GCC();
    test_true( STD_NS wcscmp(ws, L"ab") == 0 );
    test_eq( dv, 1.5 );

    int i3 = 0;
    int got3 = STD_NS swscanf(L"xyz", L"%d", &i3);
    test_eq( got3, 0 );
    test_pass("cxx03:swscanf");

    int i2 = 0;
    int got2 = vss(L"123 456", L"%d", &i2);
    test_eq( got2, 1 );
    test_eq( i2, 123 );
    test_pass("cxx03:vswscanf");
}

TEST_CASE(cwchar, wide_file_streams) {
    const char* path = "ccwwctmp.txt";
    STD::FILE* fp = STD::fopen(path, "w+");
    test_ptr( fp );
    if (!fp)
        return;

    int orient_set   = STD_NS fwide(fp, 1);
    int orient_query = STD_NS fwide(fp, 0);
    test_gt( orient_set, 0 );
    test_ge( orient_query, 0 );
    test_pass("cxx03:fwide");

    STD_NS wint_t pa = STD_NS fputwc(L'A', fp);
    test_eq( (long)pa, (long)L'A' );
    test_pass("cxx03:fputwc");
    int pbc = STD_NS fputws(L"BC\n", fp);
    test_ge( pbc, 0 );
    test_pass("cxx03:fputws");
    int pf = STD_NS fwprintf(fp, L"%d %ls\n", 42, L"xy");
    test_eq( pf, 6 );
    test_pass("cxx03:fwprintf");
    STD_NS wint_t pz = STD_NS putwc(L'Z', fp);
    test_eq( (long)pz, (long)L'Z' );
    test_pass("cxx03:putwc");
    int pv = vfw(fp, L"[%d]\n", 7);
    test_eq( pv, 4 );
    test_pass("cxx03:vfwprintf");

    STD::rewind(fp);

    STD_NS wint_t c = STD_NS fgetwc(fp);
    test_eq( (long)c, (long)L'A' );
    test_pass("cxx03:fgetwc");
    STD_NS wint_t back = STD_NS ungetwc(c, fp);
    test_eq( (long)back, (long)L'A' );
    STD_NS wint_t again = STD_NS fgetwc(fp);
    test_eq( (long)again, (long)L'A' );
    test_pass("cxx03:ungetwc");

    wchar_t line[32];
    wchar_t* g = STD_NS fgetws(line, 32, fp);
    test_true( g == line );
    test_true( STD_NS wcscmp(line, L"BC\n") == 0 );
    test_pass("cxx03:fgetws");

    int d = 0;
    wchar_t w[8] = {0};
    int nscan = STD_NS fwscanf(fp, L"%d %7ls", &d, w);
    test_eq( nscan, 2 );
    test_eq( d, 42 );
    test_true( STD_NS wcscmp(w, L"xy") == 0 );
    test_pass("cxx03:fwscanf");

    STD_NS wint_t nl = STD_NS getwc(fp);
    test_eq( (long)nl, (long)L'\n' );
    STD_NS wint_t z = STD_NS getwc(fp);
    test_eq( (long)z, (long)L'Z' );
    test_pass("cxx03:getwc");

    int v = 0;
    int nv = vfs(fp, L"[%d]", &v);
    test_eq( nv, 1 );
    test_eq( v, 7 );
    test_pass("cxx03:vfwscanf");

    STD_NS fgetwc(fp);
    STD_NS wint_t eof = STD_NS fgetwc(fp);
    test_eq( (long)eof, (long)WEOF );
    test_true( STD::feof(fp) != 0 );
    test_pass("cxx03:feof");

    int closed = STD::fclose(fp);
    test_eq( closed, 0 );
    test_pass("cxx03:fclose");
    int removed = STD::remove(path);
    test_eq( removed, 0 );
    test_pass("cxx03:remove");
}

TEST_CASE(cwchar, stdio_bound_functions) {

    STD::va_list ap;

    test_eq( sizeof(STD_NS wprintf(L"%d", 1)),        sizeof(int) );
    test_pass("cxx03:wprintf");
    test_eq( sizeof(STD_NS wscanf(L"%d", (int*)0)),   sizeof(int) );
    test_pass("cxx03:wscanf");
    test_eq( sizeof(STD_NS vwprintf(L"%d", ap)),      sizeof(int) );
    test_pass("cxx03:vwprintf");
    test_eq( sizeof(STD_NS vwscanf(L"%d", ap)),       sizeof(int) );
    test_pass("cxx03:vwscanf");
    test_eq( sizeof(STD_NS getwchar()),               sizeof(STD_NS wint_t) );
    test_pass("cxx03:getwchar");
    test_eq( sizeof(STD_NS putwchar(L'x')),           sizeof(STD_NS wint_t) );
    test_pass("cxx03:putwchar");
}

TEST_CASE(cwchar, wcsftime) {
    STD::string saved(STD::setlocale(LC_ALL, NULL));
    STD::setlocale(LC_ALL, "C");

    STD::time_t utc = STD::time_t(946684800);
    STD_NS tm g = *STD::gmtime(&utc);

    wchar_t buf[64];
    STD_NS size_t n = STD_NS wcsftime(buf, 64, L"%Y-%m-%d %H:%M:%S", &g);
    test_eq( n, STD_NS size_t(19) );
    test_true( STD_NS wcscmp(buf, L"2000-01-01 00:00:00") == 0 );

    n = STD_NS wcsftime(buf, 64, L"%a %b", &g);
    test_eq( n, STD_NS size_t(7) );
    test_true( STD_NS wcscmp(buf, L"Sat Jan") == 0 );

    wchar_t small[4];
    STD_NS size_t z = STD_NS wcsftime(small, 4, L"%Y-%m-%d", &g);
    test_eq( z, STD_NS size_t(0) );

    STD::setlocale(LC_ALL, saved.c_str());
    test_pass("cxx03:wcsftime");
}

TEST_CASE(cwchar, btowc_wctob_mbsinit) {
    STD::string saved(STD::setlocale(LC_ALL, NULL));
    STD::setlocale(LC_ALL, "C");

    test_eq( (long)STD_NS btowc('A'), (long)L'A' );
    test_eq( (long)STD_NS btowc(0),   0L );
    test_eq( (long)STD_NS btowc(EOF), (long)WEOF );
    test_pass("cxx03:btowc");

    test_eq( STD_NS wctob(L'A'), (int)'A' );
    TEST_SKIP_WAT();
    test_eq( STD_NS wctob(WEOF), EOF );
    for (int c = 'a'; c <= 'z'; ++c)
        test_eq( STD_NS wctob(STD_NS btowc(c)), c );
    test_pass("cxx03:wctob");

    STD_NS mbstate_t st;
    STD::memset(&st, 0, sizeof st);
    test_ne( STD_NS mbsinit(&st), 0 );
    test_ne( STD_NS mbsinit(NULL), 0 );
    test_pass("cxx03:mbsinit");

    STD::setlocale(LC_ALL, saved.c_str());
}

TEST_CASE(cwchar, mbrtowc_wcrtomb_mbrlen) {
    STD::string saved(STD::setlocale(LC_ALL, NULL));
    STD::setlocale(LC_ALL, "C");

    STD_NS mbstate_t st;
    STD::memset(&st, 0, sizeof st);

    STD_NS size_t l1 = STD_NS mbrlen("a", 1, &st);
    test_eq( l1, STD_NS size_t(1) );
    STD::memset(&st, 0, sizeof st);
    STD_NS size_t l0 = STD_NS mbrlen("", 1, &st);
    test_eq( l0, STD_NS size_t(0) );
    STD::memset(&st, 0, sizeof st);
    STD_NS size_t linc = STD_NS mbrlen("a", 0, &st);
    test_true( linc == STD_NS size_t(-2) || linc == STD_NS size_t(0) );
    test_pass("cxx03:mbrlen");

    STD::memset(&st, 0, sizeof st);
    wchar_t wc = 0;
    STD_NS size_t r1 = STD_NS mbrtowc(&wc, "Q", 1, &st);
    test_eq( r1, STD_NS size_t(1) );
    test_eq( (long)wc, (long)L'Q' );

    STD::memset(&st, 0, sizeof st);
    wc = L'#';
    STD_NS size_t r0 = STD_NS mbrtowc(&wc, "", 1, &st);
    test_eq( r0, STD_NS size_t(0) );
    test_eq( (long)wc, 0L );

    STD::memset(&st, 0, sizeof st);
    STD_NS size_t rn = STD_NS mbrtowc(NULL, "z", 1, &st);
    test_eq( rn, STD_NS size_t(1) );
    test_pass("cxx03:mbrtowc");

    STD::memset(&st, 0, sizeof st);
    char mb[MB_LEN_MAX + 1];
    STD::memset(mb, 0, sizeof mb);
    STD_NS size_t w1 = STD_NS wcrtomb(mb, L'Q', &st);
    test_eq( w1, STD_NS size_t(1) );
    test_eq( (int)mb[0], (int)'Q' );

    STD::memset(&st, 0, sizeof st);
    STD_NS size_t w0 = STD_NS wcrtomb(mb, L'\0', &st);
    test_eq( w0, STD_NS size_t(1) );
    test_eq( (int)mb[0], 0 );
    test_pass("cxx03:wcrtomb");

    STD::setlocale(LC_ALL, saved.c_str());
}

TEST_CASE(cwchar, mbsrtowcs_wcsrtombs) {
    STD::string saved(STD::setlocale(LC_ALL, NULL));
    STD::setlocale(LC_ALL, "C");

    STD_NS mbstate_t st;
    STD::memset(&st, 0, sizeof st);

    const char* src = "wide";
    const char* p = src;
    STD_NS size_t cnt = STD_NS mbsrtowcs(NULL, &p, 0, &st);
    test_eq( cnt, STD_NS size_t(4) );
    test_true( p == src );

    STD::memset(&st, 0, sizeof st);
    wchar_t wbuf[16];
    STD_NS wmemset(wbuf, L'.', 16);
    p = src;
    STD_NS size_t n = STD_NS mbsrtowcs(wbuf, &p, 16, &st);
    test_eq( n, STD_NS size_t(4) );
    test_true( p == NULL );
    test_eq( (long)wbuf[0], (long)L'w' );
    test_eq( (long)wbuf[3], (long)L'e' );
    TEST_SKIP_WAT();
    test_true( STD_NS wcscmp(wbuf, L"wide") == 0 );

    STD::memset(&st, 0, sizeof st);
    STD_NS wmemset(wbuf, L'.', 16);
    p = src;
    STD_NS size_t n2 = STD_NS mbsrtowcs(wbuf, &p, 2, &st);
    test_eq( n2, STD_NS size_t(2) );
    test_true( p == src + 2 );
    test_eq( (long)wbuf[0], (long)L'w' );
    test_eq( (long)wbuf[1], (long)L'i' );
    test_pass("cxx03:mbsrtowcs");

    STD::memset(&st, 0, sizeof st);
    const wchar_t* wsrc = L"wide";
    const wchar_t* wp = wsrc;
    STD_NS size_t wcnt = STD_NS wcsrtombs(NULL, &wp, 0, &st);
    test_eq( wcnt, STD_NS size_t(4) );
    test_true( wp == wsrc );

    STD::memset(&st, 0, sizeof st);
    char nbuf[16];
    STD::memset(nbuf, 0, sizeof nbuf);
    wp = wsrc;
    STD_NS size_t m = STD_NS wcsrtombs(nbuf, &wp, 16, &st);
    test_eq( m, STD_NS size_t(4) );
    test_true( wp == NULL );
    test_str_eq( nbuf, "wide" );
    test_pass("cxx03:wcsrtombs");

    STD::setlocale(LC_ALL, saved.c_str());
}
#else
TEST_CASE_SKIP(cwchar, types_and_macros)
TEST_CASE_SKIP(cwchar, wcslen_copy_cat)
TEST_CASE_SKIP(cwchar, wcscmp_and_friends)
TEST_CASE_SKIP(cwchar, search)
TEST_CASE_SKIP(cwchar, wcstok)
TEST_CASE_SKIP(cwchar, wmem)
TEST_CASE_SKIP(cwchar, wcsto_numeric)
TEST_CASE_SKIP(cwchar, swprintf_swscanf)
TEST_CASE_SKIP(cwchar, wide_file_streams)
TEST_CASE_SKIP(cwchar, stdio_bound_functions)
TEST_CASE_SKIP(cwchar, wcsftime)
TEST_CASE_SKIP(cwchar, btowc_wctob_mbsinit)
TEST_CASE_SKIP(cwchar, mbrtowc_wcrtomb_mbrlen)
TEST_CASE_SKIP(cwchar, mbsrtowcs_wcsrtombs)
#endif

TEST_CASE(cwchar, size_t_type) {
    STD_NS size_t n = sizeof(wchar_t);
    test_true( n >= 1u );
    bool is_unsigned = (STD_NS size_t)-1 > (STD_NS size_t)0;
    test_true( is_unsigned );
    test_pass("cxx03:size_t");
}

TEST_CASE(cwchar, wchar_width_cxx26) {
#if defined(WCHAR_WIDTH)
    test_eq( (int)WCHAR_WIDTH, (int)(sizeof(wchar_t) * 8) );
    test_true( WCHAR_WIDTH >= 8 );
    test_pass("cxx26:WCHAR_WIDTH");
#else
    TEST_NOTE("WCHAR_WIDTH is C23/C++26; this library does not define it yet");
    test_skip("cxx26:WCHAR_WIDTH");
#endif
}
