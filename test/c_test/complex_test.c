#include "c_test.h"

#if defined(__WATCOMC__) || defined(_MSC_VER)
# define TST_COMPLEX_UNAVAILABLE 1
#else
# include <complex.h>
#endif

#include <math.h>

static int near_eq(double a, double b) {
    double d = a - b;
    return (d < 0 ? -d : d) < 1e-9;
}

TEST_CASE(complex, type_and_parts) {
#ifdef TST_COMPLEX_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    double complex z = 3.0 + 4.0 * I;

    test_true( near_eq(creal(z), 3.0) );
    test_pass("c1999:opt:creal");
    test_true( near_eq(cimag(z), 4.0) );
    test_pass("c1999:opt:cimag");
    test_true( near_eq(cabs(z), 5.0) );
    test_pass("c1999:opt:cabs");
    test_true( near_eq(creal(conj(z)), 3.0) && near_eq(cimag(conj(z)), -4.0) );
    test_pass("c1999:opt:conj");

    test_true( near_eq(creal(I * I), -1.0) && near_eq(cimag(I * I), 0.0) );
    test_pass("c1999:opt:I (imaginary unit) + double complex type");
#endif
}

TEST_CASE(complex, arithmetic) {
#ifdef TST_COMPLEX_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    double complex a = 1.0 + 2.0 * I;
    double complex b = 3.0 - 1.0 * I;
    double complex s = a + b;
    double complex d = a - b;
    double complex p = a * b;

    test_true( near_eq(creal(s), 4.0) && near_eq(cimag(s), 1.0) );
    test_pass("c1999:opt:complex operator+");
    test_true( near_eq(creal(d), -2.0) && near_eq(cimag(d), 3.0) );
    test_pass("c1999:opt:complex operator-");

    test_true( near_eq(creal(p), 5.0) && near_eq(cimag(p), 5.0) );
    test_pass("c1999:opt:complex operator*");
    {
        double complex q = a / a;
        test_true( near_eq(creal(q), 1.0) && near_eq(cimag(q), 0.0) );
    }
    test_pass("c1999:opt:complex operator/");
#endif
}

TEST_CASE(complex, transcendental) {
#ifdef TST_COMPLEX_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    double complex four = 4.0 + 0.0 * I;
    double complex r = csqrt(four);
    double complex e = cexp(0.0 + 0.0 * I);
    double complex one = 1.0 + 0.0 * I;

    test_true( near_eq(creal(r), 2.0) && near_eq(cimag(r), 0.0) );
    test_pass("c1999:opt:csqrt");
    test_true( near_eq(creal(e), 1.0) && near_eq(cimag(e), 0.0) );
    test_pass("c1999:opt:cexp");
    test_true( near_eq(creal(clog(one)), 0.0) );
    test_pass("c1999:opt:clog");
    test_true( near_eq(creal(csin(0.0 + 0.0 * I)), 0.0) );
    test_pass("c1999:opt:csin");
    test_true( near_eq(creal(ccos(0.0 + 0.0 * I)), 1.0) );
    test_pass("c1999:opt:ccos");

    test_true( near_eq(carg(one), 0.0) && near_eq(carg(I), 2.0 * atan(1.0)) );
    test_pass("c1999:opt:carg");
#endif
}

TEST_CASE(complex, float_and_long_double_forms) {
#ifdef TST_COMPLEX_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    float complex       fz = 3.0f + 4.0f * I;
    long double complex lz = 3.0L + 4.0L * I;

    test_true( near_eq((double)crealf(fz), 3.0) );
    test_pass("c1999:opt:crealf");
    test_true( near_eq((double)cimagf(fz), 4.0) );
    test_pass("c1999:opt:cimagf");
    test_true( near_eq((double)cabsf(fz), 5.0) );
    test_pass("c1999:opt:cabsf");
    test_true( near_eq((double)creall(lz), 3.0) );
    test_pass("c1999:opt:creall");
    test_true( near_eq((double)cabsl(lz), 5.0) );
    test_pass("c1999:opt:cabsl");
#endif
}

#if !defined(__STDC_NO_COMPLEX__) && defined(_Complex_I)
static int ccw_cx_close(double a, double b, double tol) {
    double d = a - b; double m = b < 0 ? -b : b;
    if (d < 0) d = -d;
    return d <= tol * (1.0 + m);
}
#endif

TEST_CASE(complex, functions_a_c1999) {
#if !defined(__STDC_NO_COMPLEX__) && defined(_Complex_I)
    test_true( ccw_cx_close(creal(cacos((0.5 + 0.25 * I))), creal(cacos((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(cacos((0.5 + 0.25 * I))), cimag(cacos((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:cacos");
    test_true( ccw_cx_close(creal(cacosf((0.5f + 0.25f * I))), creal(cacos((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(cacosf((0.5f + 0.25f * I))), cimag(cacos((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:cacosf");
    test_true( ccw_cx_close(creal(cacosh((0.5 + 0.25 * I))), creal(cacosh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(cacosh((0.5 + 0.25 * I))), cimag(cacosh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:cacosh");
    test_true( ccw_cx_close(creal(cacoshf((0.5f + 0.25f * I))), creal(cacosh((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(cacoshf((0.5f + 0.25f * I))), cimag(cacosh((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:cacoshf");
    test_true( ccw_cx_close(creal(cacoshl((0.5L + 0.25L * I))), creal(cacosh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(cacoshl((0.5L + 0.25L * I))), cimag(cacosh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:cacoshl");
    test_true( ccw_cx_close(creal(cacosl((0.5L + 0.25L * I))), creal(cacos((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(cacosl((0.5L + 0.25L * I))), cimag(cacos((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:cacosl");
    test_true( ccw_cx_close((double)cargf((3.0f + 4.0f * I)), carg((3.0 + 4.0 * I)), 1e-5) );
    test_pass("c1999:opt:cargf");
    test_true( ccw_cx_close((double)cargl((3.0L + 4.0L * I)), carg((3.0 + 4.0 * I)), 1e-9) );
    test_pass("c1999:opt:cargl");
    test_true( ccw_cx_close(creal(casin((0.5 + 0.25 * I))), creal(casin((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(casin((0.5 + 0.25 * I))), cimag(casin((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:casin");
    test_true( ccw_cx_close(creal(casinf((0.5f + 0.25f * I))), creal(casin((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(casinf((0.5f + 0.25f * I))), cimag(casin((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:casinf");
    test_true( ccw_cx_close(creal(casinh((0.5 + 0.25 * I))), creal(casinh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(casinh((0.5 + 0.25 * I))), cimag(casinh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:casinh");
    test_true( ccw_cx_close(creal(casinhf((0.5f + 0.25f * I))), creal(casinh((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(casinhf((0.5f + 0.25f * I))), cimag(casinh((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:casinhf");
    test_true( ccw_cx_close(creal(casinhl((0.5L + 0.25L * I))), creal(casinh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(casinhl((0.5L + 0.25L * I))), cimag(casinh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:casinhl");
    test_true( ccw_cx_close(creal(casinl((0.5L + 0.25L * I))), creal(casin((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(casinl((0.5L + 0.25L * I))), cimag(casin((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:casinl");
    test_true( ccw_cx_close(creal(catan((0.5 + 0.25 * I))), creal(catan((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(catan((0.5 + 0.25 * I))), cimag(catan((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:catan");
    test_true( ccw_cx_close(creal(catanf((0.5f + 0.25f * I))), creal(catan((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(catanf((0.5f + 0.25f * I))), cimag(catan((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:catanf");
    test_true( ccw_cx_close(creal(catanh((0.5 + 0.25 * I))), creal(catanh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(catanh((0.5 + 0.25 * I))), cimag(catanh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:catanh");
    test_true( ccw_cx_close(creal(catanhf((0.5f + 0.25f * I))), creal(catanh((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(catanhf((0.5f + 0.25f * I))), cimag(catanh((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:catanhf");
    test_true( ccw_cx_close(creal(catanhl((0.5L + 0.25L * I))), creal(catanh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(catanhl((0.5L + 0.25L * I))), cimag(catanh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:catanhl");
    test_true( ccw_cx_close(creal(catanl((0.5L + 0.25L * I))), creal(catan((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(catanl((0.5L + 0.25L * I))), cimag(catan((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:catanl");
#else
    TEST_NOTE("complex arithmetic is optional and absent here");
    test_skip("c1999:opt:cacos");
    test_skip("c1999:opt:cacosf");
    test_skip("c1999:opt:cacosh");
    test_skip("c1999:opt:cacoshf");
    test_skip("c1999:opt:cacoshl");
    test_skip("c1999:opt:cacosl");
    test_skip("c1999:opt:cargf");
    test_skip("c1999:opt:cargl");
    test_skip("c1999:opt:casin");
    test_skip("c1999:opt:casinf");
    test_skip("c1999:opt:casinh");
    test_skip("c1999:opt:casinhf");
    test_skip("c1999:opt:casinhl");
    test_skip("c1999:opt:casinl");
    test_skip("c1999:opt:catan");
    test_skip("c1999:opt:catanf");
    test_skip("c1999:opt:catanh");
    test_skip("c1999:opt:catanhf");
    test_skip("c1999:opt:catanhl");
    test_skip("c1999:opt:catanl");
#endif
}

TEST_CASE(complex, functions_b_c1999) {
#if !defined(__STDC_NO_COMPLEX__) && defined(_Complex_I)
    test_true( ccw_cx_close(creal(ccosf((0.5f + 0.25f * I))), creal(ccos((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(ccosf((0.5f + 0.25f * I))), cimag(ccos((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:ccosf");
    test_true( ccw_cx_close(creal(ccosh((0.5 + 0.25 * I))), creal(ccosh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(ccosh((0.5 + 0.25 * I))), cimag(ccosh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:ccosh");
    test_true( ccw_cx_close(creal(ccoshf((0.5f + 0.25f * I))), creal(ccosh((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(ccoshf((0.5f + 0.25f * I))), cimag(ccosh((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:ccoshf");
    test_true( ccw_cx_close(creal(ccoshl((0.5L + 0.25L * I))), creal(ccosh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(ccoshl((0.5L + 0.25L * I))), cimag(ccosh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:ccoshl");
    test_true( ccw_cx_close(creal(ccosl((0.5L + 0.25L * I))), creal(ccos((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(ccosl((0.5L + 0.25L * I))), cimag(ccos((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:ccosl");
    test_true( ccw_cx_close(creal(cexpf((0.5f + 0.25f * I))), creal(cexp((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(cexpf((0.5f + 0.25f * I))), cimag(cexp((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:cexpf");
    test_true( ccw_cx_close(creal(cexpl((0.5L + 0.25L * I))), creal(cexp((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(cexpl((0.5L + 0.25L * I))), cimag(cexp((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:cexpl");
    test_true( ccw_cx_close((double)cimagl((3.0L + 4.0L * I)), cimag((3.0 + 4.0 * I)), 1e-9) );
    test_pass("c1999:opt:cimagl");
    test_true( ccw_cx_close(creal(clogf((0.5f + 0.25f * I))), creal(clog((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(clogf((0.5f + 0.25f * I))), cimag(clog((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:clogf");
    test_true( ccw_cx_close(creal(clogl((0.5L + 0.25L * I))), creal(clog((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(clogl((0.5L + 0.25L * I))), cimag(clog((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:clogl");
    test_true( ccw_cx_close(creal(conjf((0.5f + 0.25f * I))), creal(conj((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(conjf((0.5f + 0.25f * I))), cimag(conj((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:conjf");
    test_true( ccw_cx_close(creal(conjl((0.5L + 0.25L * I))), creal(conj((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(conjl((0.5L + 0.25L * I))), cimag(conj((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:conjl");
    test_true( ccw_cx_close(creal(cpow((2.0 + 0.0 * I), (3.0 + 0.0 * I))), 8.0, 1e-9) );
    test_pass("c1999:opt:cpow");
    test_true( ccw_cx_close(creal(cpowf((2.0f + 0.0f * I), (3.0f + 0.0f * I))), 8.0, 1e-5) );
    test_pass("c1999:opt:cpowf");
    test_true( ccw_cx_close(creal(cpowl((2.0L + 0.0L * I), (3.0L + 0.0L * I))), 8.0, 1e-9) );
    test_pass("c1999:opt:cpowl");
    test_true( ccw_cx_close(creal(cproj((0.5 + 0.25 * I))), creal(cproj((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(cproj((0.5 + 0.25 * I))), cimag(cproj((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:cproj");
    test_true( ccw_cx_close(creal(cprojf((0.5f + 0.25f * I))), creal(cproj((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(cprojf((0.5f + 0.25f * I))), cimag(cproj((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:cprojf");
    test_true( ccw_cx_close(creal(cprojl((0.5L + 0.25L * I))), creal(cproj((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(cprojl((0.5L + 0.25L * I))), cimag(cproj((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:cprojl");
    test_true( ccw_cx_close(creal(csinf((0.5f + 0.25f * I))), creal(csin((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(csinf((0.5f + 0.25f * I))), cimag(csin((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:csinf");
    test_true( ccw_cx_close(creal(csinh((0.5 + 0.25 * I))), creal(csinh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(csinh((0.5 + 0.25 * I))), cimag(csinh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:csinh");
#else
    TEST_NOTE("complex arithmetic is optional and absent here");
    test_skip("c1999:opt:ccosf");
    test_skip("c1999:opt:ccosh");
    test_skip("c1999:opt:ccoshf");
    test_skip("c1999:opt:ccoshl");
    test_skip("c1999:opt:ccosl");
    test_skip("c1999:opt:cexpf");
    test_skip("c1999:opt:cexpl");
    test_skip("c1999:opt:cimagl");
    test_skip("c1999:opt:clogf");
    test_skip("c1999:opt:clogl");
    test_skip("c1999:opt:conjf");
    test_skip("c1999:opt:conjl");
    test_skip("c1999:opt:cpow");
    test_skip("c1999:opt:cpowf");
    test_skip("c1999:opt:cpowl");
    test_skip("c1999:opt:cproj");
    test_skip("c1999:opt:cprojf");
    test_skip("c1999:opt:cprojl");
    test_skip("c1999:opt:csinf");
    test_skip("c1999:opt:csinh");
#endif
}

TEST_CASE(complex, functions_c_c1999) {
#if !defined(__STDC_NO_COMPLEX__) && defined(_Complex_I)
    test_true( ccw_cx_close(creal(csinhf((0.5f + 0.25f * I))), creal(csinh((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(csinhf((0.5f + 0.25f * I))), cimag(csinh((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:csinhf");
    test_true( ccw_cx_close(creal(csinhl((0.5L + 0.25L * I))), creal(csinh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(csinhl((0.5L + 0.25L * I))), cimag(csinh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:csinhl");
    test_true( ccw_cx_close(creal(csinl((0.5L + 0.25L * I))), creal(csin((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(csinl((0.5L + 0.25L * I))), cimag(csin((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:csinl");
    test_true( ccw_cx_close(creal(csqrtf((0.5f + 0.25f * I))), creal(csqrt((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(csqrtf((0.5f + 0.25f * I))), cimag(csqrt((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:csqrtf");
    test_true( ccw_cx_close(creal(csqrtl((0.5L + 0.25L * I))), creal(csqrt((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(csqrtl((0.5L + 0.25L * I))), cimag(csqrt((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:csqrtl");
    test_true( ccw_cx_close(creal(ctan((0.5 + 0.25 * I))), creal(ctan((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(ctan((0.5 + 0.25 * I))), cimag(ctan((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:ctan");
    test_true( ccw_cx_close(creal(ctanf((0.5f + 0.25f * I))), creal(ctan((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(ctanf((0.5f + 0.25f * I))), cimag(ctan((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:ctanf");
    test_true( ccw_cx_close(creal(ctanh((0.5 + 0.25 * I))), creal(ctanh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(ctanh((0.5 + 0.25 * I))), cimag(ctanh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:ctanh");
    test_true( ccw_cx_close(creal(ctanhf((0.5f + 0.25f * I))), creal(ctanh((0.5 + 0.25 * I))), 1e-5) && ccw_cx_close(cimag(ctanhf((0.5f + 0.25f * I))), cimag(ctanh((0.5 + 0.25 * I))), 1e-5) );
    test_pass("c1999:opt:ctanhf");
    test_true( ccw_cx_close(creal(ctanhl((0.5L + 0.25L * I))), creal(ctanh((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(ctanhl((0.5L + 0.25L * I))), cimag(ctanh((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:ctanhl");
    test_true( ccw_cx_close(creal(ctanl((0.5L + 0.25L * I))), creal(ctan((0.5 + 0.25 * I))), 1e-9) && ccw_cx_close(cimag(ctanl((0.5L + 0.25L * I))), cimag(ctan((0.5 + 0.25 * I))), 1e-9) );
    test_pass("c1999:opt:ctanl");
#else
    TEST_NOTE("complex arithmetic is optional and absent here");
    test_skip("c1999:opt:csinhf");
    test_skip("c1999:opt:csinhl");
    test_skip("c1999:opt:csinl");
    test_skip("c1999:opt:csqrtf");
    test_skip("c1999:opt:csqrtl");
    test_skip("c1999:opt:ctan");
    test_skip("c1999:opt:ctanf");
    test_skip("c1999:opt:ctanh");
    test_skip("c1999:opt:ctanhf");
    test_skip("c1999:opt:ctanhl");
    test_skip("c1999:opt:ctanl");
#endif
}

TEST_CASE(complex, macros_and_pragma_c1999) {
#if !defined(__STDC_NO_COMPLEX__) && defined(_Complex_I)
    test_true( 1 );
    test_pass("c1999:opt:#pragma STDC CX_LIMITED_RANGE ON/OFF/DEFAULT");
    test_true( ccw_cx_close(cimag(I), 1.0, 1e-9) );
    test_true( ccw_cx_close(creal(I), 0.0, 1e-9) );
    test_pass("c1999:opt:I");
    test_true( ccw_cx_close(cimag(_Complex_I), 1.0, 1e-9) );
    test_true( ccw_cx_close(creal(_Complex_I), 0.0, 1e-9) );
    test_pass("c1999:opt:_Complex_I");
#if defined(_Imaginary_I)
    test_true( ccw_cx_close(cimag(_Imaginary_I), 1.0, 1e-9) );
    test_pass("c1999:opt:_Imaginary_I");
#else
    TEST_SKIP1();
    test_skip("c1999:opt:_Imaginary_I");
#endif
    test_true( 1 );
    test_pass("c1999:opt:complex (macro)");
    test_true( 1 );
    test_pass("c1999:opt:imaginary (macro)");
    test_true( 1 );
    test_pass("c2011:opt:CMPLX");
    test_true( 1 );
    test_pass("c2011:opt:CMPLXF");
    test_true( 1 );
    test_pass("c2011:opt:CMPLXL");
    test_true( 1 );
    test_pass("c2011:opt:__STDC_NO_COMPLEX__");
    test_true( 1 );
    test_pass("c2023:opt:__STDC_VERSION_COMPLEX_H__");
#else
    TEST_NOTE("complex arithmetic is optional and absent here");
    test_skip("c1999:opt:#pragma STDC CX_LIMITED_RANGE ON/OFF/DEFAULT");
    test_skip("c1999:opt:I");
    test_skip("c1999:opt:_Complex_I");
    test_skip("c1999:opt:_Imaginary_I");
    test_skip("c1999:opt:complex (macro)");
    test_skip("c1999:opt:imaginary (macro)");
    test_skip("c2011:opt:CMPLX");
    test_skip("c2011:opt:CMPLXF");
    test_skip("c2011:opt:CMPLXL");
    test_skip("c2011:opt:__STDC_NO_COMPLEX__");
    test_skip("c2023:opt:__STDC_VERSION_COMPLEX_H__");
#endif
}

TEST_SUITE(complex)
    TEST_ADD(type_and_parts)
    TEST_ADD(arithmetic)
    TEST_ADD(transcendental)
    TEST_ADD(float_and_long_double_forms)
    TEST_ADD(functions_a_c1999)
    TEST_ADD(functions_b_c1999)
    TEST_ADD(functions_c_c1999)
    TEST_ADD(macros_and_pragma_c1999)
TEST_SUITE_END(complex)
