#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbit.h>
#include <stdckdint.h>
#include <stdatomic.h>
#include <threads.h>
#include <uchar.h>
#include <stdnoreturn.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>
#include "vc_nodlg.h"

#if defined(snprintf) || defined(vsnprintf)
#error snprintf and vsnprintf must not be macros.
#endif

static mtx_t g_mtx;
static int   g_counter;
static int   g_once_calls;

static void once_fn(void) { g_once_calls++; }

static int worker(void* arg)
{
    int i;
    int n = *(int*)arg;
    for (i = 0; i < n; ++i) {
        mtx_lock(&g_mtx);
        ++g_counter;
        mtx_unlock(&g_mtx);
    }
    return 42;
}

static int test_va_copy_impl(char const* fmt, ...)
{
    int n1;
    int n2;
    char s1[32];
    char s2[32];
    va_list a;
    va_list b;

    va_start(a, fmt);
    va_copy(b, a);
    n1 = vsnprintf(s1, sizeof(s1), fmt, a);
    n2 = vsnprintf(s2, sizeof(s2), fmt, b);
    va_end(b);
    va_end(a);

    return (n1 == n2 && strcmp(s1, s2) == 0) ? n1 : -1;
}

static int test_headers(void)
{
    alignas(8) static char aligned_storage[8];
    char buf[32];
    bool b = true and not false;
    intmax_t im = INTMAX_C(123);
    uintmax_t um = UINTMAX_C(456);

    _Static_assert(sizeof(int) >= 2, int_is_too_small);
    (void)aligned_storage;

    if (!b)
        return 1;
    if (snprintf(buf, sizeof(buf), "%d", 1234) != 4)
        return 2;
    if (strcmp(buf, "1234") != 0)
        return 3;
    if (snprintf(buf, sizeof(buf), "%s", "abcd") != 4)
        return 4;
    if (strcmp(buf, "abcd") != 0)
        return 5;
    if (test_va_copy_impl("%d %s", 7, "abc") != 5)
        return 6;
    if (im != 123 || um != 456)
        return 7;
    if (snprintf(buf, sizeof(buf), "%" PRIdMAX, im) <= 0)
        return 8;

    {
        unsigned int bits = 0x28u;
        int checked = 0;
        char8_t c8 = (char8_t)'A';
        char16_t c16 = (char16_t)0x3042u;
        char32_t c32 = (char32_t)0x1f600u;

        if (stdc_count_ones(bits) != 2)
            return 9;
        if (stdc_leading_zeros_uc((unsigned char)0x10u) != 3)
            return 10;
        if (stdc_trailing_zeros_ui(bits) != 3)
            return 11;
        if (!stdc_has_single_bit_ui(0x80u))
            return 12;
        if (stdc_bit_width_ui(bits) != 6)
            return 13;
        if (stdc_bit_floor_ui(bits) != 0x20u)
            return 14;
        if (stdc_bit_ceil_ui(bits) != 0x40u)
            return 15;
        if (ckd_add(&checked, 100, 23))
            return 16;
        if (checked != 123)
            return 17;
        if (!ckd_add(&checked, INT32_MAX, 1))
            return 18;
        if (sizeof(c8) != 1 || sizeof(c16) < 2 || sizeof(c32) < 4)
            return 19;
        if ((unsigned char)c8 != 'A')
            return 20;
    }

    {
        struct timespec ts;
        char *dup;

        if (round(2.5) != 3.0 || round(-2.5) != -3.0)
            return 21;
        if (trunc(2.7) != 2.0 || trunc(-2.7) != -2.0)
            return 22;
        if (fmin(2.0, 3.0) != 2.0 || fmax(2.0, 3.0) != 3.0)
            return 23;
        if (fdim(5.0, 2.0) != 3.0 || fdim(2.0, 5.0) != 0.0)
            return 24;
        if (copysign(3.0, -1.0) != -3.0 || !signbit(-2.0) || signbit(2.0))
            return 25;
        if (exp2(3.0) != 8.0)
            return 26;
        if (fabs(log2(8.0) - 3.0) > 1e-9 || fabs(cbrt(27.0) - 3.0) > 1e-9)
            return 27;
        if (fabs(hypot(3.0, 4.0) - 5.0) > 1e-9 || fabs(hypotf(3.0f, 4.0f) - 5.0f) > 1e-6f)
            return 28;
        if (lround(2.5) != 2 && lround(2.5) != 3)
            return 29;
        if (!isnan(nan("")) || isnan(1.0))
            return 30;
        if (!isinf(INFINITY) || isinf(1.0) || !isfinite(1.0))
            return 31;
        if (fpclassify(1.0) != FP_NORMAL || fpclassify(0.0) != FP_ZERO)
            return 32;
        if (ilogb(8.0) != 3 || scalbn(1.0, 4) != 16.0)
            return 33;

        if (timespec_get(&ts, TIME_UTC) != TIME_UTC || ts.tv_sec <= 0)
            return 34;

        if (DECIMAL_DIG < 10)
            return 35;

        if (INT_WIDTH != 32 || ULLONG_WIDTH != 64)
            return 36;
        if (UINT64_WIDTH != 64 || SIZE_WIDTH < 32)
            return 37;
        dup = strndup("abcdef", 3);
        if (!dup || strcmp(dup, "abc") != 0)
            return 38;
        free(dup);
        if (0)
            unreachable();
    }

    {
        if (stdc_count_zeros_ui(0x28u) != 30)
            return 39;
        if (stdc_count_ones_ull(0xFFull) != 8)
            return 40;
        if (stdc_first_trailing_one_ui(0x28u) != 4)
            return 41;
        if (stdc_leading_ones_uc((unsigned char)0xF0u) != 4)
            return 42;
    }

    {
        int r;
        if (ckd_sub(&r, 5, 3) || r != 2)
            return 43;
        if (!ckd_sub(&r, INT_MIN, 1))
            return 44;
        if (ckd_mul(&r, 6, 7) || r != 42)
            return 45;
        if (!ckd_mul(&r, INT_MAX, 2))
            return 46;
    }

    {
        if (nearbyint(2.4) != 2.0 || rint(2.6) != 3.0)
            return 47;
        if (fabs(remainder(5.0, 3.0) - (-1.0)) > 1e-9)
            return 48;
        if (nextafter(1.0, 2.0) <= 1.0)
            return 49;
        if (fminf(2.0f, 3.0f) != 2.0f || fmaxf(2.0f, 3.0f) != 3.0f)
            return 50;
        if (fabs(acosh(1.0)) > 1e-9 || fabs(asinh(0.0)) > 1e-9 || fabs(atanh(0.0)) > 1e-9)
            return 51;
        if (llround(2.5) < 2)
            return 52;
    }

    {
        int r0 = fegetround();
        fenv_t env;

        if (fesetround(FE_UPWARD) != 0 || fegetround() != FE_UPWARD)
            return 53;
        if (fesetround(FE_TONEAREST) != 0 || fegetround() != FE_TONEAREST)
            return 54;
        fesetround(r0);

        feclearexcept(FE_ALL_EXCEPT);
        if (fetestexcept(FE_ALL_EXCEPT) != 0)
            return 55;
        if (fegetenv(&env) != 0 || fesetenv(&env) != 0)
            return 56;
    }

    {
        atomic_int a;
        atomic_flag f = ATOMIC_FLAG_INIT;
        int exp;

        atomic_init(&a, 5);
        if (atomic_load(&a) != 5)
            return 57;
        atomic_store(&a, 10);
        if (atomic_load(&a) != 10)
            return 58;
        if (atomic_fetch_add(&a, 3) != 10 || atomic_load(&a) != 13)
            return 59;
        if (atomic_fetch_sub(&a, 1) != 13 || atomic_load(&a) != 12)
            return 60;
        if (atomic_exchange(&a, 100) != 12 || atomic_load(&a) != 100)
            return 61;
        exp = 100;
        if (!atomic_compare_exchange_strong(&a, &exp, 7) || atomic_load(&a) != 7)
            return 62;
        exp = 99;
        if (atomic_compare_exchange_strong(&a, &exp, 0) || exp != 7)
            return 63;

        atomic_store(&a, 0x0F);
        if (atomic_fetch_or(&a, 0xF0) != 0x0F || atomic_load(&a) != 0xFF)
            return 64;
        if (atomic_fetch_and(&a, 0x0F) != 0xFF || atomic_load(&a) != 0x0F)
            return 65;
        if (atomic_fetch_xor(&a, 0xFF) != 0x0F || atomic_load(&a) != 0xF0)
            return 66;

        if (atomic_flag_test_and_set(&f))
            return 67;
        if (!atomic_flag_test_and_set(&f))
            return 68;
        atomic_flag_clear(&f);
        atomic_thread_fence(memory_order_seq_cst);
    }

    {
        thrd_t t1, t2;
        int r1 = 0, r2 = 0;
        int n = 1000;
        once_flag of = ONCE_FLAG_INIT;
        tss_t key;

        g_counter = 0;
        g_once_calls = 0;
        mtx_init(&g_mtx, mtx_plain);
        if (thrd_create(&t1, worker, &n) != thrd_success)
            return 70;
        if (thrd_create(&t2, worker, &n) != thrd_success)
            return 71;
        thrd_join(t1, &r1);
        thrd_join(t2, &r2);
        if (r1 != 42 || r2 != 42)
            return 72;
        if (g_counter != 2 * n)
            return 73;
        mtx_destroy(&g_mtx);

        call_once(&of, once_fn);
        call_once(&of, once_fn);
        if (g_once_calls != 1)
            return 74;

        if (tss_create(&key, 0) != thrd_success)
            return 75;
        tss_set(key, (void*)0x1234);
        if (tss_get(key) != (void*)0x1234)
            return 76;
        tss_delete(key);

        if (!thrd_equal(thrd_current(), thrd_current()))
            return 77;
    }

    return 0;
}

int main(void)
{
    ccw_no_crash_dialogs();
    return test_headers();
}
