#include <ccwrap_common.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
#include <stdnoreturn.h>
#include <uchar.h>
#include <assert.h>
#include <math.h>
#include <string.h>

_Static_assert(sizeof(int) * 8 == _CCW_INT_BIT, "int width matches _CCW_INT_BIT");
static_assert(sizeof(char16_t) == 2, "char16_t is 2 bytes");
static_assert(sizeof(char32_t) == 4, "char32_t is 4 bytes");

static int failures = 0;
#define CHECK(cond)  do { if (!(cond)) { printf("  FAIL: %s\n", #cond); ++failures; } } while (0)

int main(void)
{
    uint32_t a  = UINT32_C(0x12345678);
    uint64_t b  = UINT64_C(0x1122334455667788);
    int32_t  s  = INT32_C(-2000000000);
    char16_t u  = 0x41;
    char32_t w  = 0x42;
    bool     ok = true;

    printf("[watcom_ctest] compiler=%s\n", _CCW_COMPILER);

    CHECK(a == 0x12345678u);
    CHECK(b == 0x1122334455667788ull);
    CHECK(s == -2000000000);
    CHECK(INT32_MAX == 2147483647);
    CHECK(u == 0x41 && w == 0x42);
    CHECK(ok == true);
    CHECK(alignof(double) >= 1u && alignof(double) <= 8u);
    CHECK(sizeof(_ccw_int64) == 8);

    CHECK(lround(2.5) == 3);
    CHECK(lround(-2.5) == -3);
    CHECK(llround(1e9 + 0.5) == 1000000001);
    CHECK(strnlen("hello", 3) == 3);
    CHECK(strnlen("hi", 8) == 2);

    CHECK(round(2.4) == 2.0 && round(2.6) == 3.0);
    CHECK(trunc(2.9) == 2.0 && trunc(-2.9) == -2.0);
    CHECK(fmin(1.0, 2.0) == 1.0 && fmax(1.0, 2.0) == 2.0);
    CHECK(copysign(3.0, -1.0) == -3.0);
    CHECK(cbrt(27.0) > 2.999 && cbrt(27.0) < 3.001);
    CHECK(exp2(10.0) == 1024.0);

    if (failures == 0) printf("[watcom_ctest] all checks passed\n");
    else               printf("[watcom_ctest] %d failure(s)\n", failures);
    return failures ? 1 : 0;
}
