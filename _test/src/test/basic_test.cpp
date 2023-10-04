#define __STDC_LIMIT_MACROS
/*
// mingw で stdio.h より先に stddef.h をincludeすると,
// __need_NULL,__need_size_t,__need_wchar_t,__need_wint_t が未定義でwint_tがないなどのエラーにばけてしまう...
*/

#if 0
#include <stdio.h>
#include <stddef.h>
#include <iso646.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>
#endif

#include <cstddef>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <ctime>
#include <cwchar>
#include <cwctype>

#ifdef getc
//#error defined getc
#endif

#include <_ccwrap/ccwrap_test.hpp>


CCWRAP_TEST_SUITE(basic) {

bool   b1 = false;
bool   b2 = true;

#ifndef __CCWRAP_NO_THREAD_LOCAL
thread_local int   th_int_val = 0;
#endif

char    ao1 = alignof(bool);
int     ao2 = alignof(char);
int     ao3 = alignof(int);
struct StHasOpAmp {
    bool operator&() const { return false; }
    void* amp;
};
int     ao4 = alignof(StHasOpAmp);


#ifndef __CCWRAP_NO_ALIGNAS
alignas(8)  int     align8_int_val1 = false;
alignas(8)  int     align8_int_val2 = false;
#endif

struct C_Base {
    virtual ~C_Base() {}
    virtual void membFunc() = 0;
    virtual void membFunc2() = 0;
};


struct C1 : public C_Base {
    virtual ~C1() {}
    virtual void membFunc() override { std::printf("C1 override\n"); }
    virtual void membFunc2() final { std::printf("C1 final\n"); }
};
struct C2 : public C1 {
    virtual ~C2() {}
    virtual void membFunc() override { std::printf("C2 override\n"); }
};


CCWRAP_TEST(check) {
    static_assert(sizeof(C2) > 0, "");
    static_assert(alignof(std::uint64_t) == 8, "");
    static_assert(alignof(std::max_align_t) >= 8, "");
    static_assert(alignof(std::intmax_t) <= sizeof(std::intmax_t), "");

    ccwrap_test_eq(ao1, 1);
    ccwrap_test_eq(ao2, 1);
    ccwrap_test_eq(ao3, 4);
    ccwrap_test_eq(ao4, sizeof(void*));
    ccwrap_test_eq(alignof(std::uint64_t), 8);

  #ifndef __CCWRAP_NO_THREAD_LOCAL
    th_int_val = 50;
    ccwrap_test_eq(th_int_val, 50);
  #endif

    char    buf[1024] = {0};
    char*   p         = new char[10];
    ccwrap_test_ptr(p);
    std::strcpy(p, "hello");
    std::snprintf(buf, sizeof buf, "%s world! (%d)", p, int(std::strlen(p)));
 #if __CCWRAP__ > 0
    ccwrap_test(strcasecmp(p, "HELLO") == 0);
 #else
    ccwrap_test(strcmp(p, "hello") == 0);
#endif
    delete[] p;

 #ifdef PRId64
    std::snprintf(buf, sizeof buf, "# %" PRId64 "  %" PRIx64 "\n", INT64_C(1234567890123456789), INT64_C(0x123456789abcdef0) );
    ccwrap_test(std::strcmp(buf, "# 1234567890123456789  123456789abcdef0\n") == 0);
 #endif

 #ifndef __CCWRAP_NO_ALIGNAS
    ccwrap_test_eq(uintptr_t(&align8_int_val1) & 7, 0);
    ccwrap_test_eq(uintptr_t(&align8_int_val2) & 7, 0);
 #endif
}

}
