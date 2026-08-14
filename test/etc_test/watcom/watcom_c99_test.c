#include <ccwrap_common.h>

#include <fenv.h>
#include <stddef.h>
#include <stdio.h>

static int failures = 0;

static void ck(int cond, const char *what) {
    if (!cond) { printf("[watcom_c99] FAIL: %s\n", what); ++failures; }
}

int main(void) {
    fenv_t env;
    fexcept_t flags;

    ck(__STDC_VERSION__ >= 199901L, "compiled in C99 mode");

    ck(FE_DIVBYZERO != 0, "FE_DIVBYZERO");
    ck(FE_INEXACT != 0, "FE_INEXACT");
    ck(FE_INVALID != 0, "FE_INVALID");
    ck(FE_OVERFLOW != 0, "FE_OVERFLOW");
    ck(FE_UNDERFLOW != 0, "FE_UNDERFLOW");
    ck((FE_ALL_EXCEPT & FE_DIVBYZERO) != 0, "FE_ALL_EXCEPT covers FE_DIVBYZERO");
    ck((FE_ALL_EXCEPT & FE_INEXACT) != 0, "FE_ALL_EXCEPT covers FE_INEXACT");

    ck(FE_TONEAREST != FE_DOWNWARD, "FE_TONEAREST != FE_DOWNWARD");
    ck(FE_UPWARD != FE_TOWARDZERO, "FE_UPWARD != FE_TOWARDZERO");

    ck(feclearexcept(FE_ALL_EXCEPT) == 0, "feclearexcept");
    ck(fetestexcept(FE_ALL_EXCEPT) == 0, "no exceptions pending after clear");
    ck(feraiseexcept(FE_INVALID) == 0, "feraiseexcept");
    ck((fetestexcept(FE_INVALID) & FE_INVALID) != 0, "FE_INVALID observed after raise");
    ck(feclearexcept(FE_ALL_EXCEPT) == 0, "feclearexcept again");
    ck(fetestexcept(FE_INVALID) == 0, "FE_INVALID cleared");

    ck(fegetexceptflag(&flags, FE_ALL_EXCEPT) == 0, "fegetexceptflag");
    ck(fesetexceptflag(&flags, FE_ALL_EXCEPT) == 0, "fesetexceptflag");

    ck(fegetenv(&env) == 0, "fegetenv");
    ck(fesetenv(&env) == 0, "fesetenv");
    ck(feholdexcept(&env) == 0, "feholdexcept");
    ck(feupdateenv(&env) == 0, "feupdateenv");

    {
        int old = fegetround();
        ck(fesetround(FE_TOWARDZERO) == 0, "fesetround(FE_TOWARDZERO)");
        ck(fegetround() == FE_TOWARDZERO, "fegetround sees FE_TOWARDZERO");
        ck(fesetround(FE_TONEAREST) == 0, "fesetround(FE_TONEAREST)");
        ck(fegetround() == FE_TONEAREST, "fegetround sees FE_TONEAREST");
        fesetround(old);
    }

    ck(FE_DFL_ENV != NULL, "FE_DFL_ENV");
    ck(fesetenv(FE_DFL_ENV) == 0, "fesetenv(FE_DFL_ENV)");

    if (failures == 0)
        printf("[watcom_c99] all checks passed\n");
    return failures;
}
