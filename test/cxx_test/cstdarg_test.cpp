#include "test_cxx.hpp"
#include <cstdarg>

TEST_CASE(cstdarg, header_compiles) { test_true( true );     test_pass("cxx03:<cstdarg> header compiles");
}

#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace {

int sum_ints(int n, ...) {
    STD_NS va_list ap;
    va_start(ap, n);
    int s = 0;
    for (int i = 0; i < n; ++i)
        s += va_arg(ap, int);
    va_end(ap);
    return s;
}

double sum_mixed(int n, ...) {
    STD_NS va_list ap;
    va_start(ap, n);
    double s = 0.0;
    for (int i = 0; i < n; ++i) {
        int kind = va_arg(ap, int);
        switch (kind) {
        case 0:  s += va_arg(ap, int);              break;
        case 1:  s += va_arg(ap, double);           break;
        case 2:  s += (double)va_arg(ap, long);     break;
        case 3:  s += (double)va_arg(ap, unsigned); break;
        default:                                    break;
        }
    }
    va_end(ap);
    return s;
}

const char* pick_str(int idx, ...) {
    STD_NS va_list ap;
    va_start(ap, idx);
    const char* p = 0;
    for (int i = 0; i <= idx; ++i)
        p = va_arg(ap, const char*);
    va_end(ap);
    return p;
}

long long sum_and_max(int n, int* out_max, ...) {
    STD_NS va_list ap;
    STD_NS va_list ap2;
    va_start(ap, out_max);
    va_copy(ap2, ap);

    long long s = 0;
    for (int i = 0; i < n; ++i)
        s += va_arg(ap, int);
    va_end(ap);

    int mx = 0;
    for (int i = 0; i < n; ++i) {
        int v = va_arg(ap2, int);
        if (i == 0 || v > mx)
            mx = v;
    }
    va_end(ap2);

    *out_max = mx;
    return s;
}

int fmt(char* buf, STD::size_t sz, const char* f, ...) {
    STD_NS va_list ap;
    va_start(ap, f);
    int r = STD::vsnprintf(buf, sz, f, ap);
    va_end(ap);
    return r;
}

}

TEST_CASE(cstdarg, va_start_arg_end) {
    test_eq( sum_ints(0), 0 );
    test_eq( sum_ints(1, 42), 42 );
    test_pass("cxx03:va_start");
    test_eq( sum_ints(3, 1, 2, 3), 6 );
    test_pass("cxx03:va_arg");
    test_eq( sum_ints(5, 10, -20, 30, -40, 50), 30 );
    test_pass("cxx03:va_end");
}

TEST_CASE(cstdarg, va_arg_types) {
    double d = sum_mixed(4, 0, 1, 1, 2.5, 2, 10L, 3, 7u);
    test_true( d > 20.4 && d < 20.6 );

    const char* s = pick_str(2, "a", "b", "c");
    test_true( STD::strcmp(s, "c") == 0 );
    const char* s0 = pick_str(0, "z", "y");
    test_true( STD::strcmp(s0, "z") == 0 );
    test_pass("cxx03:va_arg (multiple types)");
}

TEST_CASE(cstdarg, va_copy) {
    int mx = 0;
    long long s = sum_and_max(4, &mx, 3, 9, 4, 1);
    test_eq( s, 17LL );
    test_eq( mx, 9 );

    mx = 0;
    s = sum_and_max(1, &mx, -5);
    test_eq( s, -5LL );
    test_eq( mx, -5 );
    test_pass("cxx11:va_copy");
}

TEST_CASE(cstdarg, va_list_passed_on) {

    char buf[64];
    int n = fmt(buf, sizeof buf, "%d-%s-%c", 12, "ab", 'z');
    test_eq( n, 7 );
    test_true( STD::strcmp(buf, "12-ab-z") == 0 );
    test_pass("cxx03:va_list passed to another function");
}
