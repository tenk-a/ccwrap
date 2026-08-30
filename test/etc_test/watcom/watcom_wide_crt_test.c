#include <ccwrap_common.h>

#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int failures = 0;

static void ck(int cond, const char *what) {
    if (!cond) { printf("[watcom_wide_crt] FAIL: %s\n", what); ++failures; }
}

static long long ident_ll(long long v) { return v; }

int main(void) {
    wchar_t buf[80];
    wchar_t src[32];
    wchar_t *tok;
    wchar_t *end = 0;

    ck(_wtoi64(L"1234567890123") == 1234567890123LL, "_wtoi64 large");
    ck(_wtoi64(L"-42") == -42LL, "_wtoi64 negative");
    ck(_wtoi64(L"0") == 0LL, "_wtoi64 zero");

    ck(_wcstoi64(L"7fffffffffffffff", &end, 16) == 0x7fffffffffffffffLL, "_wcstoi64 hex max");
    ck(end != 0 && *end == 0, "_wcstoi64 endptr");
    ck(_wcstoi64(L"-100", (wchar_t**)0, 10) == -100LL, "_wcstoi64 dec negative");
    ck(_wcstoui64(L"ffffffffffffffff", (wchar_t**)0, 16) == 0xffffffffffffffffULL, "_wcstoui64 max");
    ck(_wcstoui64(L"777", (wchar_t**)0, 8) == 511ULL, "_wcstoui64 octal");

    _i64tow(1234567890123LL, buf, 10);
    ck(wcscmp(buf, L"1234567890123") == 0, "_i64tow dec");
    _i64tow(-255LL, buf, 10);
    ck(wcscmp(buf, L"-255") == 0, "_i64tow negative dec");
    _i64tow(ident_ll(-1LL), buf, 16);
    ck(wcscmp(buf, L"ffffffffffffffff") == 0, "_i64tow radix16 treats value as unsigned");
    _i64tow(0LL, buf, 10);
    ck(wcscmp(buf, L"0") == 0, "_i64tow zero");
    _i64tow(-9223372036854775807LL - 1LL, buf, 10);
    ck(wcscmp(buf, L"-9223372036854775808") == 0, "_i64tow INT64_MIN");
    _ui64tow(0xffffffffffffffffULL, buf, 16);
    ck(wcscmp(buf, L"ffffffffffffffff") == 0, "_ui64tow hex max");
    _ui64tow(255ULL, buf, 2);
    ck(wcscmp(buf, L"11111111") == 0, "_ui64tow base2");
    _ui64tow(35ULL, buf, 36);
    ck(wcscmp(buf, L"z") == 0, "_ui64tow base36");

    wcscpy(src, L"a,b;c");
    tok = _wcstok(src, L",;");
    ck(tok != 0 && wcscmp(tok, L"a") == 0, "_wcstok first");
    tok = _wcstok((wchar_t*)0, L",;");
    ck(tok != 0 && wcscmp(tok, L"b") == 0, "_wcstok second");
    tok = _wcstok((wchar_t*)0, L",;");
    ck(tok != 0 && wcscmp(tok, L"c") == 0, "_wcstok third");
    tok = _wcstok((wchar_t*)0, L",;");
    ck(tok == 0, "_wcstok exhausted");

    {
        FILE* fp = _wfopen(L"ccw_wcrt_tmp.txt", L"wb");
        ck(fp != 0, "_wfopen write");
        if (fp) { fputs("wide", fp); fclose(fp); }
        fp = _wfopen(L"ccw_wcrt_tmp.txt", L"rb");
        ck(fp != 0, "_wfopen read");
        if (fp) {
            char rb[8];
            size_t n = fread(rb, 1, 4, fp);
            ck(n == 4 && rb[0] == 'w' && rb[3] == 'e', "_wfopen roundtrip");
            fclose(fp);
        }
        ck(_wremove(L"ccw_wcrt_tmp.txt") == 0, "_wremove");
    }

    if (failures == 0) printf("[watcom_wide_crt] all ok\n");
    return failures;
}
