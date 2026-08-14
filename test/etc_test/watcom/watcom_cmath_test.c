#include <ccwrap_common.h>

#include <math.h>
#include <stdio.h>

static int failures = 0;

static void ck(int cond, const char *what) {
    if (!cond) { printf("[watcom_cmath] FAIL: %s\n", what); ++failures; }
}

int main(void) {

    ck(acosf(1.0f) == 0.0f, "acosf");
    ck(acosl(1.0L) == 0.0L, "acosl");
    ck(asinf(0.0f) == 0.0f, "asinf");
    ck(asinl(0.0L) == 0.0L, "asinl");
    ck(atanf(0.0f) == 0.0f, "atanf");
    ck(atanl(0.0L) == 0.0L, "atanl");
    ck(cosf(0.0f) == 1.0f, "cosf");
    ck(cosl(0.0L) == 1.0L, "cosl");
    ck(sinf(0.0f) == 0.0f, "sinf");
    ck(sinl(0.0L) == 0.0L, "sinl");
    ck(tanf(0.0f) == 0.0f, "tanf");
    ck(tanl(0.0L) == 0.0L, "tanl");

    ck(acoshf(1.0f) == 0.0f, "acoshf");
    ck(acoshl(1.0L) == 0.0L, "acoshl");
    ck(asinhf(0.0f) == 0.0f, "asinhf");
    ck(asinhl(0.0L) == 0.0L, "asinhl");
    ck(atanhf(0.0f) == 0.0f, "atanhf");
    ck(atanhl(0.0L) == 0.0L, "atanhl");
    ck(coshf(0.0f) == 1.0f, "coshf");
    ck(coshl(0.0L) == 1.0L, "coshl");
    ck(sinhf(0.0f) == 0.0f, "sinhf");
    ck(sinhl(0.0L) == 0.0L, "sinhl");
    ck(tanhf(0.0f) == 0.0f, "tanhf");
    ck(tanhl(0.0L) == 0.0L, "tanhl");

    ck(expf(0.0f) == 1.0f, "expf");
    ck(expl(0.0L) == 1.0L, "expl");
    ck(exp2f(3.0f) == 8.0f, "exp2f");
    ck(exp2l(3.0L) == 8.0L, "exp2l");
    ck(expm1f(0.0f) == 0.0f, "expm1f");
    ck(expm1l(0.0L) == 0.0L, "expm1l");
    ck(logf(1.0f) == 0.0f, "logf");
    ck(logl(1.0L) == 0.0L, "logl");
    ck(log10f(100.0f) == 2.0f, "log10f");
    ck(log10l(100.0L) == 2.0L, "log10l");
    ck(log1pf(0.0f) == 0.0f, "log1pf");
    ck(log1pl(0.0L) == 0.0L, "log1pl");
    ck(log2f(8.0f) == 3.0f, "log2f");
    ck(log2l(8.0L) == 3.0L, "log2l");
    ck(logbf(8.0f) == 3.0f, "logbf");
    ck(logbl(8.0L) == 3.0L, "logbl");

    ck(sqrtf(4.0f) == 2.0f, "sqrtf");
    ck(sqrtl(9.0L) == 3.0L, "sqrtl");
    ck(cbrtf(8.0f) == 2.0f, "cbrtf");
    ck(cbrtl(27.0L) == 3.0L, "cbrtl");
    ck(fabsf(-2.5f) == 2.5f, "fabsf");
    ck(fabsl(-3.0L) == 3.0L, "fabsl");
    ck(ceilf(2.1f) == 3.0f, "ceilf");
    ck(ceill(2.1L) == 3.0L, "ceill");
    ck(floorf(2.9f) == 2.0f, "floorf");
    ck(floorl(2.9L) == 2.0L, "floorl");
    ck(truncf(2.7f) == 2.0f, "truncf");
    ck(truncl(-2.7L) == -2.0L, "truncl");
    ck(roundf(2.5f) == 3.0f, "roundf");
    ck(roundl(-2.5L) == -3.0L, "roundl");
    ck(nearbyintf(2.0f) == 2.0f, "nearbyintf");
    ck(nearbyintl(2.0L) == 2.0L, "nearbyintl");
    ck(rintf(2.0f) == 2.0f, "rintf");
    ck(rintl(2.0L) == 2.0L, "rintl");

    ck(erff(0.0f) == 0.0f, "erff");
    ck(erfl(0.0L) == 0.0L, "erfl");
    ck(erfcf(0.0f) == 1.0f, "erfcf");
    ck(erfcl(0.0L) == 1.0L, "erfcl");
    ck(tgammaf(1.0f) == 1.0f, "tgammaf");
    ck(tgammal(1.0L) == 1.0L, "tgammal");
    ck(lgammaf(1.0f) == 0.0f, "lgammaf");
    ck(lgammal(1.0L) == 0.0L, "lgammal");

    ck(atan2f(0.0f, 1.0f) == 0.0f, "atan2f");
    ck(atan2l(0.0L, 1.0L) == 0.0L, "atan2l");
    ck(powf(2.0f, 3.0f) == 8.0f, "powf");
    ck(powl(2.0L, 3.0L) == 8.0L, "powl");
    ck(fmodf(7.0f, 4.0f) == 3.0f, "fmodf");
    ck(fmodl(7.0L, 4.0L) == 3.0L, "fmodl");
    ck(hypotf(3.0f, 4.0f) == 5.0f, "hypotf");
    ck(hypotl(3.0L, 4.0L) == 5.0L, "hypotl");
    ck(copysignf(2.0f, -1.0f) == -2.0f, "copysignf");
    ck(copysignl(2.0L, -1.0L) == -2.0L, "copysignl");
    ck(fdimf(5.0f, 3.0f) == 2.0f, "fdimf");
    ck(fdiml(3.0L, 5.0L) == 0.0L, "fdiml");
    ck(fmaxf(1.0f, 2.0f) == 2.0f, "fmaxf");
    ck(fmaxl(1.0L, 2.0L) == 2.0L, "fmaxl");
    ck(fminf(1.0f, 2.0f) == 1.0f, "fminf");
    ck(fminl(1.0L, 2.0L) == 1.0L, "fminl");
    ck(remainderf(7.0f, 4.0f) == (float)remainder(7.0, 4.0), "remainderf forwards");
    ck(remainderl(7.0L, 4.0L) == remainder(7.0, 4.0), "remainderl forwards");
    ck(remainderf(5.0f, 4.0f) == 1.0f, "remainderf (a case the CRT gets right)");
    ck(remainderl(5.0L, 4.0L) == 1.0L, "remainderl (a case the CRT gets right)");
    ck(nextafterf(1.0f, 2.0f) > 1.0f, "nextafterf");
    ck(nextafterl(1.0L, 2.0L) > 1.0L, "nextafterl");

    ck(fmaf(2.0f, 3.0f, 4.0f) == 10.0f, "fmaf");
    ck(fmal(2.0L, 3.0L, 4.0L) == 10.0L, "fmal");
    ck(ldexpf(1.0f, 3) == 8.0f, "ldexpf");
    ck(ldexpl(1.0L, 3) == 8.0L, "ldexpl");
    ck(scalbnf(1.0f, 4) == 16.0f, "scalbnf");
    ck(scalbnl(1.0L, 4) == 16.0L, "scalbnl");
    ck(scalblnf(1.0f, 4L) == 16.0f, "scalblnf");
    ck(scalblnl(1.0L, 4L) == 16.0L, "scalblnl");
    ck(ilogbf(8.0f) == 3, "ilogbf");
    ck(ilogbl(8.0L) == 3, "ilogbl");
    ck(lrintf(2.6f) == 3L, "lrintf");
    ck(lrintl(2.6L) == 3L, "lrintl");

    {
        int e = 0;
        float mf = frexpf(8.0f, &e);
        ck(mf == 0.5f && e == 4, "frexpf");
        e = 0;
        {
            long double ml = frexpl(8.0L, &e);
            ck(ml == 0.5L && e == 4, "frexpl");
        }
    }

    ck(lround(2.5) == 3L, "lround");
    ck(lroundf(2.5f) == 3L, "lroundf");
    ck(lroundl(2.5L) == 3L, "lroundl");
    ck(llround(-2.5) == -3, "llround");
    ck(llroundf(-2.5f) == -3, "llroundf");
    ck(llroundl(-2.5L) == -3, "llroundl");

    ck(lroundf(-2.5f) == -3L, "lroundf negative");
    ck(lround(-0.5) == -1L, "lround(-0.5)");
    ck(lround(0.5) == 1L, "lround(0.5)");

    {
        float fi = 0.0f;
        long double li = 0.0L;
        ck(modff(2.5f, &fi) == 0.5f && fi == 2.0f, "modff");
        ck(modfl(2.5L, &li) == 0.5L && li == 2.0L, "modfl");
        ck(modff(-2.5f, &fi) == -0.5f && fi == -2.0f, "modff negative");
    }

    {
        int q = 0;
        ck(remquof(5.0f, 4.0f, &q) == 1.0f, "remquof");
        ck(remquol(5.0L, 4.0L, &q) == 1.0L, "remquol");
    }
    ck(llrintf(2.6f) == 3, "llrintf");
    ck(llrintl(2.6L) == 3, "llrintl");
    ck(scalbln(1.0, 4L) == 16.0, "scalbln");
    ck(nexttoward(1.0, 2.0L) > 1.0, "nexttoward");
    ck(nexttowardf(1.0f, 2.0L) > 1.0f, "nexttowardf");
    ck(nexttowardl(1.0L, 2.0L) > 1.0L, "nexttowardl");

    ck(HUGE_VALF > 0.0f, "HUGE_VALF");
    ck(HUGE_VALL > 0.0L, "HUGE_VALL");
    ck(DECIMAL_DIG >= 15, "DECIMAL_DIG");
    ck(FLT_EVAL_METHOD >= 0, "FLT_EVAL_METHOD");
    {
        float_t a = 1.0f;
        double_t b = 2.0;
        ck(a + b == 3.0, "float_t / double_t");
    }

    ck(isgreater(2.0, 1.0) && !isgreater(1.0, 2.0) && !isgreater(2.0, 2.0), "isgreater");
    ck(isgreaterequal(2.0, 2.0) && !isgreaterequal(1.0, 2.0), "isgreaterequal");
    ck(isless(1.0, 2.0) && !isless(2.0, 1.0) && !isless(2.0, 2.0), "isless");
    ck(islessequal(2.0, 2.0) && !islessequal(2.0, 1.0), "islessequal");
    ck(islessgreater(1.0, 2.0) && !islessgreater(2.0, 2.0), "islessgreater");
    ck(!isunordered(1.0, 2.0), "isunordered on ordinary values");
    {
        double nn = NAN;
        ck(isunordered(nn, 1.0), "isunordered(NaN, x)");
        ck(isunordered(1.0, nn), "isunordered(x, NaN)");
        ck(!isgreater(nn, 1.0), "isgreater(NaN, x) is false");
        ck(!isless(nn, 1.0), "isless(NaN, x) is false");
        ck(!isgreaterequal(nn, 1.0), "isgreaterequal(NaN, x) is false");
        ck(!islessequal(nn, 1.0), "islessequal(NaN, x) is false");
        ck(!islessgreater(nn, 1.0), "islessgreater(NaN, x) is false");
    }

    if (failures == 0)
        printf("[watcom_cmath] all checks passed\n");
    return failures;
}
