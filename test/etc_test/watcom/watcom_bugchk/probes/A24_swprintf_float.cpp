// bugchk-id:     A24
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   wide swprintf %f / %Lf drops the digits before the decimal point (to_wstring)
#include <stdio.h>
#include <wchar.h>

static int wide_is(const wchar_t* w, const char* want) {
    int i = 0;
    for (; want[i]; ++i)
        if (w[i] != (wchar_t)want[i]) return 0;
    return w[i] == 0;
}

int main() {
    wchar_t buf[64];
    int ok = 1;

    int n = swprintf(buf, 64, L"%f", 1.5);
    if (!wide_is(buf, "1.500000")) { ok = 0; printf("A24 swprintf %%f  n=%d first=%d -> BROKEN\n", n, (int)buf[0]); }

    n = swprintf(buf, 64, L"%Lf", (long double)2.25);
    if (!wide_is(buf, "2.250000")) { ok = 0; printf("A24 swprintf %%Lf n=%d first=%d -> BROKEN\n", n, (int)buf[0]); }

    n = swprintf(buf, 64, L"%d", 1234);              // integer conversions were always fine
    if (!wide_is(buf, "1234")) { ok = 0; printf("A24 swprintf %%d -> BROKEN\n"); }

    printf("A24 wide float formatting -> %s\n", ok ? "OK" : "BROKEN");
    return ok ? 0 : 1;
}
