#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011
#include <cmath>
#include <climits>
#include <limits>
#include <type_traits>

#if _TST_HAS_DECLTYPE

static bool near_eq(double a, double b, double eps = 1e-9) {
    return STD_NS fabs(a - b) <= eps * (STD_NS fabs(b) > 1.0 ? STD_NS fabs(b) : 1.0);
}

static const double kEpsF = 1e-6;

TEST_CASE(cmath, ceil_floor_trunc) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS ceil(1.0f)), float>, "ceil(float)");
    static_assert(STD::is_same_v<decltype(STD_NS ceil(1.0)), double>, "ceil(double)");
    static_assert(STD::is_same_v<decltype(STD_NS ceil(1.0L)), long double>, "ceil(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS ceilf(1.0f)), float>, "ceilf");
    static_assert(STD::is_same_v<decltype(STD_NS ceill(1.0L)), long double>, "ceill");
    static_assert(STD::is_same_v<decltype(STD_NS floor(1.0f)), float>, "floor(float)");
    static_assert(STD::is_same_v<decltype(STD_NS floor(1.0)), double>, "floor(double)");
    static_assert(STD::is_same_v<decltype(STD_NS floor(1.0L)), long double>, "floor(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS floorf(1.0f)), float>, "floorf");
    static_assert(STD::is_same_v<decltype(STD_NS floorl(1.0L)), long double>, "floorl");
    static_assert(STD::is_same_v<decltype(STD_NS trunc(1.0f)), float>, "trunc(float)");
    static_assert(STD::is_same_v<decltype(STD_NS trunc(1.0)), double>, "trunc(double)");
    static_assert(STD::is_same_v<decltype(STD_NS trunc(1.0L)), long double>, "trunc(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS truncf(1.0f)), float>, "truncf");
    static_assert(STD::is_same_v<decltype(STD_NS truncl(1.0L)), long double>, "truncl");
#endif

    test_true( STD_NS ceil(2.1) == 3.0 );
    test_true( STD_NS ceil(-2.1) == -2.0 );
    test_true( STD_NS ceil(2.0) == 2.0 );
    test_true( STD_NS ceil(2.1f) == 3.0f );
    test_true( STD_NS ceil(2.1L) == 3.0L );
    test_true( STD_NS ceilf(2.1f) == 3.0f );
    test_true( STD_NS ceill(2.1L) == 3.0L );
    test_pass("cxx03:ceil");

    test_true( STD_NS floor(2.9) == 2.0 );
    test_true( STD_NS floor(-2.1) == -3.0 );
    test_true( STD_NS floor(2.9f) == 2.0f );
    test_true( STD_NS floor(-2.1L) == -3.0L );
    test_true( STD_NS floorf(2.9f) == 2.0f );
    test_true( STD_NS floorl(-2.1L) == -3.0L );
    test_pass("cxx03:floor");

    test_true( STD_NS trunc(2.9) == 2.0 );
    test_true( STD_NS trunc(-2.9) == -2.0 );
    test_true( STD_NS trunc(2.9f) == 2.0f );
    test_true( STD_NS trunc(-2.9L) == -2.0L );
    test_true( STD_NS truncf(-2.9f) == -2.0f );
    test_true( STD_NS truncl(2.9L) == 2.0L );
    test_pass("cxx11:trunc");
}

TEST_CASE(cmath, fabs_abs) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS fabs(1.0f)), float>, "fabs(float)");
    static_assert(STD::is_same_v<decltype(STD_NS fabs(1.0)), double>, "fabs(double)");
    static_assert(STD::is_same_v<decltype(STD_NS fabs(1.0L)), long double>, "fabs(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS fabsf(1.0f)), float>, "fabsf");
    static_assert(STD::is_same_v<decltype(STD_NS fabsl(1.0L)), long double>, "fabsl");
#endif

    test_true( STD_NS fabs(-2.5) == 2.5 );
    test_true( STD_NS fabs(2.5) == 2.5 );
    test_true( STD_NS fabs(-0.0) == 0.0 );
    test_true( STD_NS fabs(-2.5f) == 2.5f );
    test_true( STD_NS fabs(-2.5L) == 2.5L );
    test_true( STD_NS fabsf(-2.5f) == 2.5f );
    test_true( STD_NS fabsl(-2.5L) == 2.5L );
    test_pass("cxx03:fabs");

#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS abs(-5)), int>, "abs(int)");
    static_assert(STD::is_same_v<decltype(STD_NS abs(-5L)), long>, "abs(long)");
    static_assert(STD::is_same_v<decltype(STD_NS abs(-5LL)), long long>, "abs(long long)");
    static_assert(STD::is_same_v<decltype(STD_NS abs(-2.5f)), float>, "abs(float)");
    static_assert(STD::is_same_v<decltype(STD_NS abs(-2.5)), double>, "abs(double)");
    static_assert(STD::is_same_v<decltype(STD_NS abs(-2.5L)), long double>, "abs(long double)");
#endif

    test_eq( STD_NS abs(-5), 5 );
    test_eq( STD_NS abs(5), 5 );
    test_eq( STD_NS abs(-5L), 5L );
    test_eq( STD_NS abs(-5LL), 5LL );
    test_true( STD_NS abs(-2.5f) == 2.5f );
    test_true( STD_NS abs(-2.5) == 2.5 );
    test_true( STD_NS abs(-2.5L) == 2.5L );
    test_pass("cxx03:abs");
}

TEST_CASE(cmath, fmod) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS fmod(5.0f, 2.0f)), float>, "fmod(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS fmod(5.0, 2.0)), double>, "fmod(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS fmod(5.0L, 2.0L)), long double>, "fmod(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS fmodf(5.0f, 2.0f)), float>, "fmodf");
    static_assert(STD::is_same_v<decltype(STD_NS fmodl(5.0L, 2.0L)), long double>, "fmodl");
#endif

    test_true( STD_NS fmod(5.0, 2.0) == 1.0 );
    test_true( STD_NS fmod(-5.0, 2.0) == -1.0 );
    test_true( STD_NS fmod(5.0, -2.0) == 1.0 );
    test_true( STD_NS fmod(4.0, 2.0) == 0.0 );
    test_true( STD_NS fmod(5.5, 2.0) == 1.5 );
    test_true( STD_NS fmod(5.0f, 2.0f) == 1.0f );
    test_true( STD_NS fmod(5.0L, 2.0L) == 1.0L );
    test_true( STD_NS fmodf(5.0f, 2.0f) == 1.0f );
    test_true( STD_NS fmodl(-5.0L, 2.0L) == -1.0L );
    test_pass("cxx03:fmod");
}

TEST_CASE(cmath, frexp_ldexp) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS frexp(8.0f, (int*)nullptr)), float>, "frexp(float,int*)");
    static_assert(STD::is_same_v<decltype(STD_NS frexp(8.0, (int*)nullptr)), double>, "frexp(double,int*)");
    static_assert(STD::is_same_v<decltype(STD_NS frexp(8.0L, (int*)nullptr)), long double>, "frexp(ld,int*)");
    static_assert(STD::is_same_v<decltype(STD_NS frexpf(8.0f, (int*)nullptr)), float>, "frexpf");
    static_assert(STD::is_same_v<decltype(STD_NS frexpl(8.0L, (int*)nullptr)), long double>, "frexpl");
    static_assert(STD::is_same_v<decltype(STD_NS ldexp(0.5f, 4)), float>, "ldexp(float,int)");
    static_assert(STD::is_same_v<decltype(STD_NS ldexp(0.5, 4)), double>, "ldexp(double,int)");
    static_assert(STD::is_same_v<decltype(STD_NS ldexp(0.5L, 4)), long double>, "ldexp(ld,int)");
    static_assert(STD::is_same_v<decltype(STD_NS ldexpf(0.5f, 4)), float>, "ldexpf");
    static_assert(STD::is_same_v<decltype(STD_NS ldexpl(0.5L, 4)), long double>, "ldexpl");
#endif

    int e = 0;
    double m = STD_NS frexp(8.0, &e);
    test_true( m == 0.5 );
    test_eq( e, 4 );

    e = 0;
    m = STD_NS frexp(0.0, &e);
    test_true( m == 0.0 );
    test_eq( e, 0 );

    int ef = 0;
    float mf = STD_NS frexp(8.0f, &ef);
    test_true( mf == 0.5f );
    test_eq( ef, 4 );

    int el = 0;
    long double ml = STD_NS frexp(8.0L, &el);
    test_true( ml == 0.5L );
    test_eq( el, 4 );

    ef = 0;
    mf = STD_NS frexpf(-8.0f, &ef);
    test_true( mf == -0.5f );
    test_eq( ef, 4 );

    el = 0;
    ml = STD_NS frexpl(8.0L, &el);
    test_true( ml == 0.5L );
    test_eq( el, 4 );
    test_pass("cxx03:frexp");

    test_true( STD_NS ldexp(0.5, 4) == 8.0 );
    test_true( STD_NS ldexp(1.0, 10) == 1024.0 );
    test_true( STD_NS ldexp(1.0, -1) == 0.5 );
    test_true( STD_NS ldexp(0.5f, 4) == 8.0f );
    test_true( STD_NS ldexp(0.5L, 4) == 8.0L );
    test_true( STD_NS ldexpf(0.5f, 4) == 8.0f );
    test_true( STD_NS ldexpl(0.5L, 4) == 8.0L );
    test_pass("cxx03:ldexp");
}

TEST_CASE(cmath, modf) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS modf(3.5f, (float*)nullptr)), float>, "modf(float,float*)");
    static_assert(STD::is_same_v<decltype(STD_NS modf(3.5, (double*)nullptr)), double>, "modf(double,double*)");
    static_assert(STD::is_same_v<decltype(STD_NS modf(3.5L, (long double*)nullptr)), long double>, "modf(ld,ld*)");
    static_assert(STD::is_same_v<decltype(STD_NS modff(3.5f, (float*)nullptr)), float>, "modff");
    static_assert(STD::is_same_v<decltype(STD_NS modfl(3.5L, (long double*)nullptr)), long double>, "modfl");
#endif

    double ip = 0;
    double fp = STD_NS modf(3.5, &ip);
    test_true( ip == 3.0 );
    test_true( fp == 0.5 );

    fp = STD_NS modf(-3.5, &ip);
    test_true( ip == -3.0 );
    test_true( fp == -0.5 );

    float ipf = 0;
    float fpf = STD_NS modf(3.5f, &ipf);
    test_true( ipf == 3.0f );
    test_true( fpf == 0.5f );

    long double ipl = 0;
    long double fpl = STD_NS modf(3.5L, &ipl);
    test_true( ipl == 3.0L );
    test_true( fpl == 0.5L );

    ipf = 0;
    fpf = STD_NS modff(2.25f, &ipf);
    test_true( ipf == 2.0f );
    test_true( fpf == 0.25f );

    ipl = 0;
    fpl = STD_NS modfl(2.25L, &ipl);
    test_true( ipl == 2.0L );
    test_true( fpl == 0.25L );
    test_pass("cxx03:modf");
}

TEST_CASE(cmath, ilogb_logb) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS ilogb(8.0f)), int>, "ilogb(float)");
    static_assert(STD::is_same_v<decltype(STD_NS ilogb(8.0)), int>, "ilogb(double)");
    static_assert(STD::is_same_v<decltype(STD_NS ilogb(8.0L)), int>, "ilogb(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS ilogbf(8.0f)), int>, "ilogbf");
    static_assert(STD::is_same_v<decltype(STD_NS ilogbl(8.0L)), int>, "ilogbl");
    static_assert(STD::is_same_v<decltype(STD_NS logb(8.0f)), float>, "logb(float)");
    static_assert(STD::is_same_v<decltype(STD_NS logb(8.0)), double>, "logb(double)");
    static_assert(STD::is_same_v<decltype(STD_NS logb(8.0L)), long double>, "logb(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS logbf(8.0f)), float>, "logbf");
    static_assert(STD::is_same_v<decltype(STD_NS logbl(8.0L)), long double>, "logbl");
#endif

    test_eq( STD_NS ilogb(8.0), 3 );
    test_eq( STD_NS ilogb(1.0), 0 );
    test_eq( STD_NS ilogb(0.5), -1 );
    test_eq( STD_NS ilogb(-8.0), 3 );
    test_eq( STD_NS ilogb(8.0f), 3 );
    test_eq( STD_NS ilogb(8.0L), 3 );
    test_eq( STD_NS ilogbf(8.0f), 3 );
    test_eq( STD_NS ilogbl(8.0L), 3 );
    test_eq( STD_NS ilogb(0.0), FP_ILOGB0 );
    test_eq( STD_NS ilogb(STD::numeric_limits<double>::quiet_NaN()), FP_ILOGBNAN );
    test_pass("cxx11:ilogb");

    test_true( STD_NS logb(8.0) == 3.0 );
    test_true( STD_NS logb(1.0) == 0.0 );
    test_true( STD_NS logb(0.5) == -1.0 );
    test_true( STD_NS logb(8.0f) == 3.0f );
    test_true( STD_NS logb(8.0L) == 3.0L );
    test_true( STD_NS logbf(8.0f) == 3.0f );
    test_true( STD_NS logbl(8.0L) == 3.0L );
    test_pass("cxx11:logb");
}

TEST_CASE(cmath, scalbn_scalbln) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS scalbn(1.0f, 4)), float>, "scalbn(float,int)");
    static_assert(STD::is_same_v<decltype(STD_NS scalbn(1.0, 4)), double>, "scalbn(double,int)");
    static_assert(STD::is_same_v<decltype(STD_NS scalbn(1.0L, 4)), long double>, "scalbn(ld,int)");
    static_assert(STD::is_same_v<decltype(STD_NS scalbnf(1.0f, 4)), float>, "scalbnf");
    static_assert(STD::is_same_v<decltype(STD_NS scalbnl(1.0L, 4)), long double>, "scalbnl");
    static_assert(STD::is_same_v<decltype(STD_NS scalbln(1.0f, 4L)), float>, "scalbln(float,long)");
    static_assert(STD::is_same_v<decltype(STD_NS scalbln(1.0, 4L)), double>, "scalbln(double,long)");
    static_assert(STD::is_same_v<decltype(STD_NS scalbln(1.0L, 4L)), long double>, "scalbln(ld,long)");
    static_assert(STD::is_same_v<decltype(STD_NS scalblnf(1.0f, 4L)), float>, "scalblnf");
    static_assert(STD::is_same_v<decltype(STD_NS scalblnl(1.0L, 4L)), long double>, "scalblnl");
#endif

    test_true( STD_NS scalbn(1.0, 10) == 1024.0 );
    test_true( STD_NS scalbn(3.0, -1) == 1.5 );
    test_true( STD_NS scalbn(1.0, 0) == 1.0 );
    test_true( STD_NS scalbn(1.0f, 10) == 1024.0f );
    test_true( STD_NS scalbn(1.0L, 10) == 1024.0L );
    test_true( STD_NS scalbnf(1.0f, 10) == 1024.0f );
    test_true( STD_NS scalbnl(1.0L, 10) == 1024.0L );
    test_pass("cxx11:scalbn");

    test_true( STD_NS scalbln(1.0, 10L) == 1024.0 );
    test_true( STD_NS scalbln(3.0, -1L) == 1.5 );
    test_true( STD_NS scalbln(1.0f, 10L) == 1024.0f );
    test_true( STD_NS scalbln(1.0L, 10L) == 1024.0L );
    test_true( STD_NS scalblnf(1.0f, 10L) == 1024.0f );
    test_true( STD_NS scalblnl(1.0L, 10L) == 1024.0L );
    test_pass("cxx11:scalbln");
}

TEST_CASE(cmath, nearbyint_rint) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS nearbyint(2.5f)), float>, "nearbyint(float)");
    static_assert(STD::is_same_v<decltype(STD_NS nearbyint(2.5)), double>, "nearbyint(double)");
    static_assert(STD::is_same_v<decltype(STD_NS nearbyint(2.5L)), long double>, "nearbyint(ld)");
    static_assert(STD::is_same_v<decltype(STD_NS nearbyintf(2.5f)), float>, "nearbyintf");
    static_assert(STD::is_same_v<decltype(STD_NS nearbyintl(2.5L)), long double>, "nearbyintl");
    static_assert(STD::is_same_v<decltype(STD_NS rint(2.5f)), float>, "rint(float)");
    static_assert(STD::is_same_v<decltype(STD_NS rint(2.5)), double>, "rint(double)");
    static_assert(STD::is_same_v<decltype(STD_NS rint(2.5L)), long double>, "rint(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS rintf(2.5f)), float>, "rintf");
    static_assert(STD::is_same_v<decltype(STD_NS rintl(2.5L)), long double>, "rintl");
#endif

    test_true( STD_NS nearbyint(2.3) == 2.0 );
    TEST_SKIP_WAT();
    test_true( STD_NS nearbyint(2.5) == 2.0 );
    test_true( STD_NS nearbyint(3.5) == 4.0 );
    TEST_SKIP_WAT();
    test_true( STD_NS nearbyint(-2.5) == -2.0 );
    TEST_SKIP_WAT();
    test_true( STD_NS nearbyint(2.5f) == 2.0f );
    test_true( STD_NS nearbyint(3.5L) == 4.0L );
    test_true( STD_NS nearbyintf(3.5f) == 4.0f );
    TEST_SKIP_WAT();
    test_true( STD_NS nearbyintl(2.5L) == 2.0L );
    test_pass("cxx11:nearbyint");

    test_true( STD_NS rint(2.3) == 2.0 );
    TEST_SKIP_WAT();
    test_true( STD_NS rint(2.5) == 2.0 );
    test_true( STD_NS rint(3.5) == 4.0 );
    test_true( STD_NS rint(-2.7) == -3.0 );
    TEST_SKIP_WAT();
    test_true( STD_NS rint(2.5f) == 2.0f );
    test_true( STD_NS rint(3.5L) == 4.0L );
    test_true( STD_NS rintf(3.5f) == 4.0f );
    TEST_SKIP_WAT();
    test_true( STD_NS rintl(2.5L) == 2.0L );
    test_pass("cxx11:rint");
}

TEST_CASE(cmath, lrint_llrint) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS lrint(2.5f)), long>, "lrint(float)");
    static_assert(STD::is_same_v<decltype(STD_NS lrint(2.5)), long>, "lrint(double)");
    static_assert(STD::is_same_v<decltype(STD_NS lrint(2.5L)), long>, "lrint(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS lrintf(2.5f)), long>, "lrintf");
    static_assert(STD::is_same_v<decltype(STD_NS lrintl(2.5L)), long>, "lrintl");
    static_assert(STD::is_same_v<decltype(STD_NS llrint(2.5f)), long long>, "llrint(float)");
    static_assert(STD::is_same_v<decltype(STD_NS llrint(2.5)), long long>, "llrint(double)");
    static_assert(STD::is_same_v<decltype(STD_NS llrint(2.5L)), long long>, "llrint(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS llrintf(2.5f)), long long>, "llrintf");
    static_assert(STD::is_same_v<decltype(STD_NS llrintl(2.5L)), long long>, "llrintl");
#endif

    test_eq( STD_NS lrint(2.3), 2L );
    TEST_SKIP_WAT();
    test_eq( STD_NS lrint(2.5), 2L );
    test_eq( STD_NS lrint(3.5), 4L );
    test_eq( STD_NS lrint(-2.7), -3L );
    TEST_SKIP_WAT();
    test_eq( STD_NS lrint(2.5f), 2L );
    test_eq( STD_NS lrint(3.5L), 4L );
    test_eq( STD_NS lrintf(3.5f), 4L );
    TEST_SKIP_WAT();
    test_eq( STD_NS lrintl(2.5L), 2L );
    test_pass("cxx11:lrint");

    test_eq( STD_NS llrint(2.3), 2LL );
    test_eq( STD_NS llrint(3.5), 4LL );
    test_eq( STD_NS llrint(-2.7), -3LL );
    TEST_SKIP_WAT();
    test_eq( STD_NS llrint(2.5f), 2LL );
    test_eq( STD_NS llrint(3.5L), 4LL );
    test_eq( STD_NS llrintf(3.5f), 4LL );
    TEST_SKIP_WAT();
    test_eq( STD_NS llrintl(2.5L), 2LL );
    test_pass("cxx11:llrint");
}

TEST_CASE(cmath, round_lround_llround) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS round(2.5f)), float>, "round(float)");
    static_assert(STD::is_same_v<decltype(STD_NS round(2.5)), double>, "round(double)");
    static_assert(STD::is_same_v<decltype(STD_NS round(2.5L)), long double>, "round(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS roundf(2.5f)), float>, "roundf");
    static_assert(STD::is_same_v<decltype(STD_NS roundl(2.5L)), long double>, "roundl");
    static_assert(STD::is_same_v<decltype(STD_NS lround(2.5f)), long>, "lround(float)");
    static_assert(STD::is_same_v<decltype(STD_NS lround(2.5)), long>, "lround(double)");
    static_assert(STD::is_same_v<decltype(STD_NS lround(2.5L)), long>, "lround(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS lroundf(2.5f)), long>, "lroundf");
    static_assert(STD::is_same_v<decltype(STD_NS lroundl(2.5L)), long>, "lroundl");
    static_assert(STD::is_same_v<decltype(STD_NS llround(2.5f)), long long>, "llround(float)");
    static_assert(STD::is_same_v<decltype(STD_NS llround(2.5)), long long>, "llround(double)");
    static_assert(STD::is_same_v<decltype(STD_NS llround(2.5L)), long long>, "llround(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS llroundf(2.5f)), long long>, "llroundf");
    static_assert(STD::is_same_v<decltype(STD_NS llroundl(2.5L)), long long>, "llroundl");
#endif

    test_true( STD_NS round(2.5) == 3.0 );
    test_true( STD_NS round(-2.5) == -3.0 );
    test_true( STD_NS round(2.4) == 2.0 );
    test_true( STD_NS round(2.5f) == 3.0f );
    test_true( STD_NS round(-2.5L) == -3.0L );
    test_true( STD_NS roundf(2.5f) == 3.0f );
    test_true( STD_NS roundl(-2.5L) == -3.0L );
    test_pass("cxx11:round");

    test_eq( STD_NS lround(2.5), 3L );
    test_eq( STD_NS lround(-2.5), -3L );
    test_eq( STD_NS lround(2.5f), 3L );
    test_eq( STD_NS lround(2.5L), 3L );
    test_eq( STD_NS lroundf(2.5f), 3L );
    test_eq( STD_NS lroundl(-2.5L), -3L );
    test_pass("cxx11:lround");

    test_eq( STD_NS llround(2.5), 3LL );
    test_eq( STD_NS llround(-2.5), -3LL );
    test_eq( STD_NS llround(2.5f), 3LL );
    test_eq( STD_NS llround(2.5L), 3LL );
    test_eq( STD_NS llroundf(2.5f), 3LL );
    test_eq( STD_NS llroundl(-2.5L), -3LL );
    test_pass("cxx11:llround");
}

TEST_CASE(cmath, remainder_remquo) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS remainder(5.0f, 2.0f)), float>, "remainder(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS remainder(5.0, 2.0)), double>, "remainder(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS remainder(5.0L, 2.0L)), long double>, "remainder(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS remainderf(5.0f, 2.0f)), float>, "remainderf");
    static_assert(STD::is_same_v<decltype(STD_NS remainderl(5.0L, 2.0L)), long double>, "remainderl");
    static_assert(STD::is_same_v<decltype(STD_NS remquo(5.0f, 2.0f, (int*)nullptr)), float>, "remquo(f,f,int*)");
    static_assert(STD::is_same_v<decltype(STD_NS remquo(5.0, 2.0, (int*)nullptr)), double>, "remquo(d,d,int*)");
    static_assert(STD::is_same_v<decltype(STD_NS remquo(5.0L, 2.0L, (int*)nullptr)), long double>, "remquo(ld,ld,int*)");
    static_assert(STD::is_same_v<decltype(STD_NS remquof(5.0f, 2.0f, (int*)nullptr)), float>, "remquof");
    static_assert(STD::is_same_v<decltype(STD_NS remquol(5.0L, 2.0L, (int*)nullptr)), long double>, "remquol");
#endif

    test_true( STD_NS remainder(5.0, 2.0) == 1.0 );
    TEST_SKIP_WAT();
    test_true( STD_NS remainder(7.0, 2.0) == -1.0 );
    test_true( STD_NS fmod(7.0, 2.0) == 1.0 );
    test_true( STD_NS remainder(4.0, 2.0) == 0.0 );
    test_true( STD_NS remainder(5.0f, 2.0f) == 1.0f );
    test_true( STD_NS remainder(5.0L, 2.0L) == 1.0L );
    TEST_SKIP_WAT();
    test_true( STD_NS remainderf(7.0f, 2.0f) == -1.0f );
    TEST_SKIP_WAT();
    test_true( STD_NS remainderl(7.0L, 2.0L) == -1.0L );
    test_pass("cxx11:remainder");

    int q = 0;
    double r = STD_NS remquo(5.0, 2.0, &q);
    test_true( r == 1.0 );
    TEST_SKIP_WAT();
    test_eq( q, 2 );

    q = 0;
    r = STD_NS remquo(-5.0, 2.0, &q);
    test_true( r == -1.0 );
    TEST_SKIP_WAT();
    test_eq( q, -2 );

    int qf = 0;
    float rf = STD_NS remquo(5.0f, 2.0f, &qf);
    test_true( rf == 1.0f );
    TEST_SKIP_WAT();
    test_eq( qf, 2 );

    int ql = 0;
    long double rl = STD_NS remquo(5.0L, 2.0L, &ql);
    test_true( rl == 1.0L );
    TEST_SKIP_WAT();
    test_eq( ql, 2 );

    qf = 0;
    rf = STD_NS remquof(5.0f, 2.0f, &qf);
    test_true( rf == 1.0f );
    TEST_SKIP_WAT();
    test_eq( qf, 2 );

    ql = 0;
    rl = STD_NS remquol(5.0L, 2.0L, &ql);
    test_true( rl == 1.0L );
    TEST_SKIP_WAT();
    test_eq( ql, 2 );
    test_pass("cxx11:remquo");
}

TEST_CASE(cmath, copysign) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS copysign(3.0f, -1.0f)), float>, "copysign(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS copysign(3.0, -1.0)), double>, "copysign(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS copysign(3.0L, -1.0L)), long double>, "copysign(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS copysignf(3.0f, -1.0f)), float>, "copysignf");
    static_assert(STD::is_same_v<decltype(STD_NS copysignl(3.0L, -1.0L)), long double>, "copysignl");
#endif

    test_true( STD_NS copysign(3.0, -1.0) == -3.0 );
    test_true( STD_NS copysign(-3.0, 1.0) == 3.0 );
    test_true( STD_NS copysign(3.0, 1.0) == 3.0 );

    TEST_SKIP_WAT();
    test_true( STD_NS signbit(STD_NS copysign(1.0, -0.0)) );
    test_true( STD_NS copysign(3.0f, -1.0f) == -3.0f );
    test_true( STD_NS copysign(3.0L, -1.0L) == -3.0L );
    test_true( STD_NS copysignf(3.0f, -1.0f) == -3.0f );
    test_true( STD_NS copysignl(3.0L, -1.0L) == -3.0L );
    test_pass("cxx11:copysign");
}

TEST_CASE(cmath, nextafter_nexttoward) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS nextafter(1.0f, 2.0f)), float>, "nextafter(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS nextafter(1.0, 2.0)), double>, "nextafter(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS nextafter(1.0L, 2.0L)), long double>, "nextafter(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS nextafterf(1.0f, 2.0f)), float>, "nextafterf");
    static_assert(STD::is_same_v<decltype(STD_NS nextafterl(1.0L, 2.0L)), long double>, "nextafterl");
    static_assert(STD::is_same_v<decltype(STD_NS nexttoward(1.0f, 2.0L)), float>, "nexttoward(float,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS nexttoward(1.0, 2.0L)), double>, "nexttoward(double,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS nexttoward(1.0L, 2.0L)), long double>, "nexttoward(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS nexttowardf(1.0f, 2.0L)), float>, "nexttowardf");
    static_assert(STD::is_same_v<decltype(STD_NS nexttowardl(1.0L, 2.0L)), long double>, "nexttowardl");
#endif

    double up = STD_NS nextafter(1.0, 2.0);
    test_true( up > 1.0 );
    test_true( up == 1.0 + STD::numeric_limits<double>::epsilon() );
    double dn = STD_NS nextafter(1.0, 0.0);
    test_true( dn < 1.0 );
    test_true( STD_NS nextafter(1.0, 1.0) == 1.0 );
    float upf = STD_NS nextafter(1.0f, 2.0f);
    test_true( upf > 1.0f );
    test_true( upf == 1.0f + STD::numeric_limits<float>::epsilon() );
    test_true( STD_NS nextafter(1.0L, 2.0L) > 1.0L );
    test_true( STD_NS nextafterf(1.0f, 2.0f) > 1.0f );
    test_true( STD_NS nextafterl(1.0L, 0.0L) < 1.0L );
    test_pass("cxx11:nextafter");

    test_true( STD_NS nexttoward(1.0, 2.0L) > 1.0 );
    test_true( STD_NS nexttoward(1.0, 0.0L) < 1.0 );
    test_true( STD_NS nexttoward(1.0f, 2.0L) > 1.0f );
    test_true( STD_NS nexttoward(1.0L, 2.0L) > 1.0L );
    test_true( STD_NS nexttowardf(1.0f, 2.0L) > 1.0f );
    test_true( STD_NS nexttowardl(1.0L, 0.0L) < 1.0L );
    test_pass("cxx11:nexttoward");
}

TEST_CASE(cmath, fdim_fmax_fmin) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS fdim(5.0f, 3.0f)), float>, "fdim(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS fdim(5.0, 3.0)), double>, "fdim(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS fdim(5.0L, 3.0L)), long double>, "fdim(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS fdimf(5.0f, 3.0f)), float>, "fdimf");
    static_assert(STD::is_same_v<decltype(STD_NS fdiml(5.0L, 3.0L)), long double>, "fdiml");
    static_assert(STD::is_same_v<decltype(STD_NS fmax(5.0f, 3.0f)), float>, "fmax(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS fmax(5.0, 3.0)), double>, "fmax(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS fmax(5.0L, 3.0L)), long double>, "fmax(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS fmaxf(5.0f, 3.0f)), float>, "fmaxf");
    static_assert(STD::is_same_v<decltype(STD_NS fmaxl(5.0L, 3.0L)), long double>, "fmaxl");
    static_assert(STD::is_same_v<decltype(STD_NS fmin(5.0f, 3.0f)), float>, "fmin(float,float)");
    static_assert(STD::is_same_v<decltype(STD_NS fmin(5.0, 3.0)), double>, "fmin(double,double)");
    static_assert(STD::is_same_v<decltype(STD_NS fmin(5.0L, 3.0L)), long double>, "fmin(ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS fminf(5.0f, 3.0f)), float>, "fminf");
    static_assert(STD::is_same_v<decltype(STD_NS fminl(5.0L, 3.0L)), long double>, "fminl");
#endif

    test_true( STD_NS fdim(5.0, 3.0) == 2.0 );
    test_true( STD_NS fdim(3.0, 5.0) == 0.0 );
    test_true( STD_NS fdim(5.0f, 3.0f) == 2.0f );
    test_true( STD_NS fdim(5.0L, 3.0L) == 2.0L );
    test_true( STD_NS fdimf(3.0f, 5.0f) == 0.0f );
    test_true( STD_NS fdiml(5.0L, 3.0L) == 2.0L );
    test_pass("cxx11:fdim");

    test_true( STD_NS fmax(5.0, 3.0) == 5.0 );
    test_true( STD_NS fmax(-5.0, -3.0) == -3.0 );

    double qn = STD::numeric_limits<double>::quiet_NaN();
    test_true( STD_NS fmax(qn, 3.0) == 3.0 );
    test_true( STD_NS fmin(qn, 3.0) == 3.0 );
    test_true( STD_NS fmax(5.0f, 3.0f) == 5.0f );
    test_true( STD_NS fmax(5.0L, 3.0L) == 5.0L );
    test_true( STD_NS fmaxf(5.0f, 3.0f) == 5.0f );
    test_true( STD_NS fmaxl(5.0L, 3.0L) == 5.0L );
    test_pass("cxx11:fmax");

    test_true( STD_NS fmin(5.0, 3.0) == 3.0 );
    test_true( STD_NS fmin(-5.0, -3.0) == -5.0 );
    test_true( STD_NS fmin(5.0f, 3.0f) == 3.0f );
    test_true( STD_NS fmin(5.0L, 3.0L) == 3.0L );
    test_true( STD_NS fminf(5.0f, 3.0f) == 3.0f );
    test_true( STD_NS fminl(5.0L, 3.0L) == 3.0L );
    test_pass("cxx11:fmin");
}

TEST_CASE(cmath, fma) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS fma(2.0f, 3.0f, 4.0f)), float>, "fma(f,f,f)");
    static_assert(STD::is_same_v<decltype(STD_NS fma(2.0, 3.0, 4.0)), double>, "fma(d,d,d)");
    static_assert(STD::is_same_v<decltype(STD_NS fma(2.0L, 3.0L, 4.0L)), long double>, "fma(ld,ld,ld)");
    static_assert(STD::is_same_v<decltype(STD_NS fmaf(2.0f, 3.0f, 4.0f)), float>, "fmaf");
    static_assert(STD::is_same_v<decltype(STD_NS fmal(2.0L, 3.0L, 4.0L)), long double>, "fmal");
#endif

    test_true( STD_NS fma(2.0, 3.0, 4.0) == 10.0 );
    test_true( STD_NS fma(0.0, 5.0, 1.0) == 1.0 );
    test_true( STD_NS fma(-2.0, 3.0, 4.0) == -2.0 );

    double e2 = STD::numeric_limits<double>::epsilon() / 2;
    double f = STD_NS fma(e2, e2, 1.0);
    test_true( f >= 1.0 );
    test_true( STD_NS fma(2.0f, 3.0f, 4.0f) == 10.0f );
    test_true( STD_NS fma(2.0L, 3.0L, 4.0L) == 10.0L );
    test_true( STD_NS fmaf(2.0f, 3.0f, 4.0f) == 10.0f );
    test_true( STD_NS fmal(2.0L, 3.0L, 4.0L) == 10.0L );
    test_pass("cxx11:fma");
}

TEST_CASE(cmath, nan_builders) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS nan("")), double>, "nan(const char*)");
    static_assert(STD::is_same_v<decltype(STD_NS nanf("")), float>, "nanf(const char*)");
    static_assert(STD::is_same_v<decltype(STD_NS nanl("")), long double>, "nanl(const char*)");
#endif

    double d = STD_NS nan("");
    test_true( STD_NS isnan(d) );
    TEST_SKIP_WAT();
    test_true( d != d );

    float f = STD_NS nanf("");
    test_true( STD_NS isnan(f) );

    long double l = STD_NS nanl("");
    test_true( STD_NS isnan(l) );

    double d2 = STD_NS nan("1");
    test_true( STD_NS isnan(d2) );
    test_pass("cxx11:nan");
}

TEST_CASE(cmath, erf_erfc) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS erf(1.0f)), float>, "erf(float)");
    static_assert(STD::is_same_v<decltype(STD_NS erf(1.0)), double>, "erf(double)");
    static_assert(STD::is_same_v<decltype(STD_NS erf(1.0L)), long double>, "erf(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS erff(1.0f)), float>, "erff");
    static_assert(STD::is_same_v<decltype(STD_NS erfl(1.0L)), long double>, "erfl");
    static_assert(STD::is_same_v<decltype(STD_NS erfc(1.0f)), float>, "erfc(float)");
    static_assert(STD::is_same_v<decltype(STD_NS erfc(1.0)), double>, "erfc(double)");
    static_assert(STD::is_same_v<decltype(STD_NS erfc(1.0L)), long double>, "erfc(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS erfcf(1.0f)), float>, "erfcf");
    static_assert(STD::is_same_v<decltype(STD_NS erfcl(1.0L)), long double>, "erfcl");
#endif

    test_true( near_eq(STD_NS erf(0.0), 0.0) );
    test_true( near_eq(STD_NS erf(1.0), 0.842700792949715, 1e-12) );
    test_true( near_eq(STD_NS erf(-1.0), -0.842700792949715, 1e-12) );
    test_true( near_eq(STD_NS erf(1.0f), 0.842700792949715, kEpsF) );
    test_true( near_eq((double)STD_NS erf(1.0L), 0.842700792949715, 1e-12) );
    test_true( near_eq(STD_NS erff(1.0f), 0.842700792949715, kEpsF) );
    test_true( near_eq((double)STD_NS erfl(1.0L), 0.842700792949715, 1e-12) );
    test_pass("cxx11:erf");

#if defined(_MSC_VER) && _MSC_VER < 1800
    TEST_SKIP_VC("ccwrap's erf/erfc for vc8..11 is a 1.2e-7 approximation");
#endif
    test_true( near_eq(STD_NS erfc(0.0), 1.0) );
#if defined(_MSC_VER) && _MSC_VER < 1800
    TEST_SKIP_VC("ccwrap's erf/erfc for vc8..11 is a 1.2e-7 approximation");
#endif
    test_true( near_eq(STD_NS erfc(1.0), 1.0 - 0.842700792949715, 1e-12) );
#if defined(_MSC_VER) && _MSC_VER < 1800
    TEST_SKIP_VC("ccwrap's erf/erfc for vc8..11 is a 1.2e-7 approximation");
#endif
    test_true( near_eq(STD_NS erf(0.5) + STD_NS erfc(0.5), 1.0) );
    test_true( near_eq(STD_NS erfc(0.0f), 1.0, kEpsF) );
#if defined(_MSC_VER) && _MSC_VER < 1800
    TEST_SKIP_VC("ccwrap's erf/erfc for vc8..11 is a 1.2e-7 approximation");
#endif
    test_true( near_eq((double)STD_NS erfc(0.0L), 1.0) );
    test_true( near_eq(STD_NS erfcf(0.0f), 1.0, kEpsF) );
#if defined(_MSC_VER) && _MSC_VER < 1800
    TEST_SKIP_VC("ccwrap's erf/erfc for vc8..11 is a 1.2e-7 approximation");
#endif
    test_true( near_eq((double)STD_NS erfcl(0.0L), 1.0) );
    test_pass("cxx11:erfc");
}

TEST_CASE(cmath, lgamma_tgamma) {
#if _TST_HAS_VAR_TEMPLATE
    static_assert(STD::is_same_v<decltype(STD_NS lgamma(5.0f)), float>, "lgamma(float)");
    static_assert(STD::is_same_v<decltype(STD_NS lgamma(5.0)), double>, "lgamma(double)");
    static_assert(STD::is_same_v<decltype(STD_NS lgamma(5.0L)), long double>, "lgamma(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS lgammaf(5.0f)), float>, "lgammaf");
    static_assert(STD::is_same_v<decltype(STD_NS lgammal(5.0L)), long double>, "lgammal");
    static_assert(STD::is_same_v<decltype(STD_NS tgamma(5.0f)), float>, "tgamma(float)");
    static_assert(STD::is_same_v<decltype(STD_NS tgamma(5.0)), double>, "tgamma(double)");
    static_assert(STD::is_same_v<decltype(STD_NS tgamma(5.0L)), long double>, "tgamma(long double)");
    static_assert(STD::is_same_v<decltype(STD_NS tgammaf(5.0f)), float>, "tgammaf");
    static_assert(STD::is_same_v<decltype(STD_NS tgammal(5.0L)), long double>, "tgammal");
#endif

    test_true( near_eq(STD_NS tgamma(1.0), 1.0, 1e-12) );
    test_true( near_eq(STD_NS tgamma(5.0), 24.0, 1e-12) );
    test_true( near_eq(STD_NS tgamma(0.5), 1.7724538509055159, 1e-12) );
    test_true( near_eq(STD_NS tgamma(5.0f), 24.0, kEpsF) );
    test_true( near_eq((double)STD_NS tgamma(5.0L), 24.0, 1e-12) );
    test_true( near_eq(STD_NS tgammaf(5.0f), 24.0, kEpsF) );
    test_true( near_eq((double)STD_NS tgammal(5.0L), 24.0, 1e-12) );
    test_pass("cxx11:tgamma");

    test_true( near_eq(STD_NS lgamma(1.0), 0.0, 1e-12) );
    test_true( near_eq(STD_NS lgamma(2.0), 0.0, 1e-12) );
    test_true( near_eq(STD_NS lgamma(5.0), STD_NS log(24.0), 1e-12) );
    test_true( near_eq(STD_NS lgamma(5.0f), STD_NS log(24.0), kEpsF) );
    test_true( near_eq((double)STD_NS lgamma(5.0L), STD_NS log(24.0), 1e-12) );
    test_true( near_eq(STD_NS lgammaf(5.0f), STD_NS log(24.0), kEpsF) );
    test_true( near_eq((double)STD_NS lgammal(5.0L), STD_NS log(24.0), 1e-12) );
    test_pass("cxx11:lgamma");
}
#else
TEST_CASE_SKIP(cmath, ceil_floor_trunc)
TEST_CASE_SKIP(cmath, fabs_abs)
TEST_CASE_SKIP(cmath, fmod)
TEST_CASE_SKIP(cmath, frexp_ldexp)
TEST_CASE_SKIP(cmath, modf)
TEST_CASE_SKIP(cmath, ilogb_logb)
TEST_CASE_SKIP(cmath, scalbn_scalbln)
TEST_CASE_SKIP(cmath, nearbyint_rint)
TEST_CASE_SKIP(cmath, lrint_llrint)
TEST_CASE_SKIP(cmath, round_lround_llround)
TEST_CASE_SKIP(cmath, remainder_remquo)
TEST_CASE_SKIP(cmath, copysign)
TEST_CASE_SKIP(cmath, nextafter_nexttoward)
TEST_CASE_SKIP(cmath, fdim_fmax_fmin)
TEST_CASE_SKIP(cmath, fma)
TEST_CASE_SKIP(cmath, nan_builders)
TEST_CASE_SKIP(cmath, erf_erfc)
TEST_CASE_SKIP(cmath, lgamma_tgamma)
#endif

#else
TEST_CASE_SKIP(cmath, ceil_floor_trunc)
TEST_CASE_SKIP(cmath, fabs_abs)
TEST_CASE_SKIP(cmath, fmod)
TEST_CASE_SKIP(cmath, frexp_ldexp)
TEST_CASE_SKIP(cmath, modf)
TEST_CASE_SKIP(cmath, ilogb_logb)
TEST_CASE_SKIP(cmath, scalbn_scalbln)
TEST_CASE_SKIP(cmath, nearbyint_rint)
TEST_CASE_SKIP(cmath, lrint_llrint)
TEST_CASE_SKIP(cmath, round_lround_llround)
TEST_CASE_SKIP(cmath, remainder_remquo)
TEST_CASE_SKIP(cmath, copysign)
TEST_CASE_SKIP(cmath, nextafter_nexttoward)
TEST_CASE_SKIP(cmath, fdim_fmax_fmin)
TEST_CASE_SKIP(cmath, fma)
TEST_CASE_SKIP(cmath, nan_builders)
TEST_CASE_SKIP(cmath, erf_erfc)
TEST_CASE_SKIP(cmath, lgamma_tgamma)
#endif
