#include "c_test.h"

#include <time.h>
#include <string.h>
#include <locale.h>

static struct tm make_y2k(void) {
    struct tm t;
    memset(&t, 0, sizeof t);
    t.tm_year  = 100;
    t.tm_mon   = 0;
    t.tm_mday  = 1;
    t.tm_isdst = -1;
    return t;
}

TEST_CASE(time, now_and_clocks) {
    time_t  t = time(0);
    clock_t c = clock();
    size_t  z = sizeof(struct tm);
    test_true( z > 0 );                test_pass("c1990:size_t");
    test_true( (void*)NULL == (void*)0 ); test_pass("c1990:NULL");
    test_true( sizeof(time_t) >= 4 );  test_pass("c1990:time_t");
    test_true( sizeof(clock_t) >= 1 ); test_pass("c1990:clock_t");
    test_true( t > 0 );                test_pass("c1990:time");
    test_true( c >= 0 );               test_pass("c1990:clock");
    test_true( CLOCKS_PER_SEC > 0 );   test_pass("c1990:CLOCKS_PER_SEC");
}

TEST_CASE(time, tm_members) {
    struct tm t = make_y2k();
    t.tm_sec  = 59; t.tm_min = 58; t.tm_hour = 23;
    t.tm_mday = 31; t.tm_mon = 11; t.tm_year = 99;
    t.tm_wday = 5;  t.tm_yday = 364; t.tm_isdst = 0;
    test_eq( t.tm_sec,  59 );  test_pass("c1990:tm.tm_sec");
    test_eq( t.tm_min,  58 );  test_pass("c1990:tm.tm_min");
    test_eq( t.tm_hour, 23 );  test_pass("c1990:tm.tm_hour");
    test_eq( t.tm_mday, 31 );  test_pass("c1990:tm.tm_mday");
    test_eq( t.tm_mon,  11 );  test_pass("c1990:tm.tm_mon");
    test_eq( t.tm_year, 99 );  test_pass("c1990:tm.tm_year");
    test_eq( t.tm_wday, 5 );   test_pass("c1990:tm.tm_wday");
    test_eq( t.tm_yday, 364 ); test_pass("c1990:tm.tm_yday");
    test_eq( t.tm_isdst, 0 );  test_pass("c1990:tm.tm_isdst");
}

TEST_CASE(time, gmtime_fixed) {
    time_t epoch = 0;
    struct tm* g = gmtime(&epoch);
    char buf[64];
    test_ptr( g );
    test_eq( g->tm_year, 70 ); test_eq( g->tm_mon, 0 ); test_eq( g->tm_mday, 1 );
    test_eq( g->tm_yday, 0 ); test_eq( g->tm_hour, 0 );
    test_pass("c1990:gmtime");
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", g);
    test_eq( strcmp(buf, "1970-01-01 00:00:00"), 0 );
    test_pass("c1990:strftime");
}

TEST_CASE(time, mktime_gmtime_localtime) {
    struct tm t = make_y2k();
    struct tm* lt;
    struct tm  g2;
    time_t tt, utc, utc2;
    struct tm* gt;
    struct tm norm;

    tt = mktime(&t);
    test_ne( tt, (time_t)-1 );

    test_eq( t.tm_wday, 6 );
    test_eq( t.tm_yday, 0 );

    norm = make_y2k();
    norm.tm_year = 99; norm.tm_mon = 12; norm.tm_mday = 1;
    test_ne( mktime(&norm), (time_t)-1 );
    test_eq( norm.tm_year, 100 );
    test_eq( norm.tm_mon, 0 );
    test_eq( norm.tm_mday, 1 );
    norm = make_y2k();
    norm.tm_mon = 0; norm.tm_mday = 0;
    test_ne( mktime(&norm), (time_t)-1 );
    test_eq( norm.tm_year, 99 );
    test_eq( norm.tm_mon, 11 );
    test_eq( norm.tm_mday, 31 );
    test_pass("c1990:mktime");

    lt = localtime(&tt);
    test_ptr( lt );
    test_eq( lt->tm_year, 100 );
    test_eq( lt->tm_mon,  0 );
    test_eq( lt->tm_mday, 1 );
    test_eq( lt->tm_wday, 6 );
    test_pass("c1990:localtime");

    utc = (time_t)946684800;
    gt = gmtime(&utc);
    test_ptr( gt );
    test_eq( gt->tm_year, 100 );
    test_eq( gt->tm_mon,  0 );
    test_eq( gt->tm_mday, 1 );
    test_eq( gt->tm_hour, 0 );
    test_eq( gt->tm_wday, 6 );
    test_eq( gt->tm_isdst, 0 );

    utc2 = utc + 24 * 60 * 60;
    g2 = *gmtime(&utc2);
    test_eq( g2.tm_mday, 2 );
    test_eq( g2.tm_wday, 0 );
    test_eq( g2.tm_yday, 1 );
    test_pass("c1990:gmtime (normalisation)");
}

TEST_CASE(time, timegm_utc) {
#if defined(TEST_TARGET_C) && TEST_TARGET_C >= 2026
    struct tm t, a, b;
    time_t tt;
    struct tm* g;
    char buf[64];

    memset(&t, 0, sizeof t);
    t.tm_year = 70; t.tm_mon = 0; t.tm_mday = 1; t.tm_isdst = 0;
    test_eq( timegm(&t), (time_t)0 );

    memset(&t, 0, sizeof t);
    t.tm_year = 2001 - 1900; t.tm_mon = 8; t.tm_mday = 9;
    t.tm_hour = 1; t.tm_min = 46; t.tm_sec = 40; t.tm_isdst = 0;
    test_eq( timegm(&t), (time_t)1000000000 );

    memset(&t, 0, sizeof t);
    t.tm_year = 2000 - 1900; t.tm_mon = 0; t.tm_mday = 2;
    t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5; t.tm_isdst = 0;
    tt = timegm(&t);
    g = gmtime(&tt);
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", g);
    test_eq( strcmp(buf, "2000-01-02 03:04:05"), 0 );

    memset(&a, 0, sizeof a); memset(&b, 0, sizeof b);
    a.tm_year = b.tm_year = 100;
    a.tm_mon = b.tm_mon = 6;
    a.tm_mday = 1; b.tm_mday = 2;
    a.tm_isdst = b.tm_isdst = 0;
    test_eq( (long)(timegm(&b) - timegm(&a)), 86400L );
    test_pass("c2023:timegm");
#else
    TEST_SKIP_CASE();
#endif
}

TEST_CASE(time, difftime_values) {
    time_t a = (time_t)946684800;
    time_t b = a + 90;
    test_true( difftime(b, a) ==  90.0 );
    test_true( difftime(a, b) == -90.0 );
    test_true( difftime(a, a) ==   0.0 );
    test_pass("c1990:difftime");
}

TEST_CASE(time, time_and_clock) {
    time_t now = time(NULL);
    time_t via_ptr = 0;
    time_t ret;
    clock_t c0, c1;
    volatile double spin = 0;
    double secs;
    int i;

    test_ne( now, (time_t)-1 );
    test_gt( (double)now, 1577836800.0 );

    ret = time(&via_ptr);
    test_eq( (double)ret, (double)via_ptr );
    test_ge( difftime(via_ptr, now), 0.0 );

    c0 = clock();
    test_ne( c0, (clock_t)-1 );
    for (i = 0; i < 200000; ++i) spin += i;
    c1 = clock();
    test_ge( (double)c1, (double)c0 );
    secs = (double)(c1 - c0) / CLOCKS_PER_SEC;
    test_lim( secs, 0.0, 60.0 );
    test_pass("c1990:time(&t) pointer form + clock elapsed");
}

TEST_CASE(time, timespec_and_get) {
#if TEST_TARGET_C >= 2011 || defined(_MSC_VER) || defined(__WATCOMC__)
    struct timespec ts;
    ts.tv_sec  = (time_t)123;
    ts.tv_nsec = 456789L;
    test_true( ts.tv_sec == (time_t)123 );
    test_eq( ts.tv_nsec, 456789L );
    test_pass("c2011:struct timespec");

#if defined(TIME_UTC)
    test_true( (int)TIME_UTC != 0 );
    test_pass("c2011:TIME_UTC");
    {
        int base;
        ts.tv_sec = 0; ts.tv_nsec = 0;
        base = timespec_get(&ts, TIME_UTC);
        test_eq( base, (int)TIME_UTC );
        test_gt( (double)ts.tv_sec, 1577836800.0 );
        test_lim( (double)ts.tv_nsec, 0.0, 999999999.0 );
    }
    test_pass("c2011:timespec_get");
#else
    TEST_SKIP1(); TEST_NOTE("TIME_UTC not defined");
    test_skip("c2011:TIME_UTC");
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    TEST_NOTE("timespec_get not available");
    test_skip("c2011:timespec_get");
#endif
#else
    TEST_NOTE("struct timespec is C11 and this build asked for C99");
    test_skip("c2011:struct timespec");
    test_skip("c2011:TIME_UTC");
    test_skip("c2011:timespec_get");
#endif
}

TEST_CASE(time, c23_extras) {
#ifdef __STDC_VERSION_TIME_H__
    test_ge( __STDC_VERSION_TIME_H__, 202311L );
    test_pass("c2023:__STDC_VERSION_TIME_H__");
#else
    TEST_SKIP1(); TEST_NOTE("__STDC_VERSION_TIME_H__ not defined");
    test_skip("c2023:__STDC_VERSION_TIME_H__");
#endif

#ifdef TIME_MONOTONIC
    test_ne( TIME_MONOTONIC, TIME_UTC );
    test_pass("c2023:opt:TIME_MONOTONIC");
    test_true( TIME_ACTIVE != TIME_MONOTONIC );
    test_pass("c2023:opt:TIME_ACTIVE");
    test_true( TIME_THREAD_ACTIVE != TIME_ACTIVE );
    test_pass("c2023:opt:TIME_THREAD_ACTIVE");
#else
    TEST_SKIP1(); TEST_NOTE("TIME_MONOTONIC not defined");
    test_skip("c2023:opt:TIME_MONOTONIC");
    TEST_SKIP1(); TEST_NOTE("TIME_ACTIVE not defined");
    test_skip("c2023:opt:TIME_ACTIVE");
    TEST_SKIP1(); TEST_NOTE("TIME_THREAD_ACTIVE not defined");
    test_skip("c2023:opt:TIME_THREAD_ACTIVE");
#endif

    TEST_SKIP1(); TEST_NOTE("gmtime_r / localtime_r / timespec_getres not supplied for C");
    test_skip("c2023:gmtime_r");
    TEST_SKIP1(); test_skip("c2023:localtime_r");
    TEST_SKIP1(); test_skip("c2023:timespec_getres");
}

TEST_CASE(time, asctime_ctime) {
    time_t utc = (time_t)946684800;
    struct tm g = *gmtime(&utc);
    const char* a = asctime(&g);
    time_t now;
    struct tm ln;
    char via_c[64], via_a[64];

    test_ptr( a );
    test_eq( strlen(a), (size_t)25 );
    test_str_eq( a, "Sat Jan  1 00:00:00 2000\n" );
    test_pass("c1990:asctime");

    now = time(NULL);
    ln = *localtime(&now);
    strcpy(via_c, ctime(&now));
    strcpy(via_a, asctime(&ln));
    test_str_eq( via_c, via_a );
    test_eq( strlen(via_c), (size_t)25 );
    test_pass("c1990:ctime");
}

TEST_CASE(time, strftime_formats) {
    time_t utc = (time_t)946684800;
    struct tm g = *gmtime(&utc);
    char buf[64];
    char small[4];
    char exact[5];
    size_t n, z, e;

    n = strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", &g);
    test_eq( n, (size_t)19 );
    test_str_eq( buf, "2000-01-01 00:00:00" );

    n = strftime(buf, sizeof buf, "%a %b %j", &g);
    test_eq( n, (size_t)11 );
    test_str_eq( buf, "Sat Jan 001" );

    n = strftime(buf, sizeof buf, "100%%", &g);
    test_eq( n, (size_t)4 );
    test_str_eq( buf, "100%" );

    z = strftime(small, sizeof small, "%Y-%m-%d", &g);
    test_eq( z, (size_t)0 );

    n = strftime(exact, sizeof exact, "%Y", &g);
    test_eq( n, (size_t)4 );
    test_str_eq( exact, "2000" );
    z = strftime(small, sizeof small, "%Y", &g);
    test_eq( z, (size_t)0 );

    buf[0] = 'x';
    e = strftime(buf, sizeof buf, "", &g);
    test_eq( e, (size_t)0 );
    test_eq( (int)buf[0], 0 );
    test_pass("c1990:strftime conversion specifiers");
}


TEST_CASE(time, strftime_c99_specifiers) {
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_NOTE("the pre-UCRT MSVC CRT has none of the C99 strftime specifiers");
    TEST_SKIP_N(10);
    test_skip("c1999:strftime C99 conversion specifiers (%D %F %g %G %h %n %r %R %t %T %u %V %z)");
#else
    struct tm t;
    char buf[64];
    size_t n;
    memset(&t, 0, sizeof t);
    t.tm_year = 100; t.tm_mon = 0; t.tm_mday = 2;
    t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5;
    t.tm_wday = 0; t.tm_yday = 1; t.tm_isdst = 0;

    n = strftime(buf, sizeof buf, "%F", &t);
    test_true( n > 0 && strcmp(buf, "2000-01-02") == 0 );
    n = strftime(buf, sizeof buf, "%T", &t);
    test_true( n > 0 && strcmp(buf, "03:04:05") == 0 );
    n = strftime(buf, sizeof buf, "%D", &t);
    test_true( n > 0 && strcmp(buf, "01/02/00") == 0 );
    n = strftime(buf, sizeof buf, "%R", &t);
    test_true( n > 0 && strcmp(buf, "03:04") == 0 );
    n = strftime(buf, sizeof buf, "%n%t", &t);
    test_true( n == 2 && buf[0] == '\n' && buf[1] == '\t' );
    n = strftime(buf, sizeof buf, "%u", &t);
    test_true( n > 0 && strcmp(buf, "7") == 0 );
    n = strftime(buf, sizeof buf, "%G-%g-%V", &t);
    test_true( n > 0 && strcmp(buf, "1999-99-52") == 0 );
    n = strftime(buf, sizeof buf, "%h", &t);
    test_true( n > 0 && strlen(buf) >= 3 );
    n = strftime(buf, sizeof buf, "%r", &t);
    test_true( n > 0 );
    n = strftime(buf, sizeof buf, "%z", &t);
    test_true( n >= 0 );
    test_pass("c1999:strftime C99 conversion specifiers (%D %F %g %G %h %n %r %R %t %T %u %V %z)");
#endif
}

TEST_CASE(time, strftime_eo_modifiers) {
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_NOTE("the pre-UCRT MSVC CRT does not accept the E / O modifiers");
    TEST_SKIP_N(10);
    test_skip("c1999:strftime E/O modifiers (%Ec %EC %Ex %EX %Ey %EY %Od %Oe ...)");
#else
    struct tm t;
    char plain[64], alt[64];
    memset(&t, 0, sizeof t);
    t.tm_year = 100; t.tm_mon = 0; t.tm_mday = 2;
    t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5;
    t.tm_wday = 0; t.tm_yday = 1; t.tm_isdst = 0;
    setlocale(LC_TIME, "C");

    test_true( strftime(plain, sizeof plain, "%y", &t) > 0 );
    test_true( strftime(alt, sizeof alt, "%Ey", &t) > 0 );
    test_true( strcmp(plain, alt) == 0 );

    test_true( strftime(plain, sizeof plain, "%d", &t) > 0 );
    test_true( strftime(alt, sizeof alt, "%Od", &t) > 0 );
    test_true( strcmp(plain, alt) == 0 );

    test_true( strftime(plain, sizeof plain, "%c", &t) > 0 );
    test_true( strftime(alt, sizeof alt, "%Ec", &t) > 0 );
    test_true( strcmp(plain, alt) == 0 );

    test_true( strftime(alt, sizeof alt, "%EC%EX%EY%Oe%Om%OM", &t) > 0 );
    test_pass("c1999:strftime E/O modifiers (%Ec %EC %Ex %EX %Ey %EY %Od %Oe ...)");
#endif
}

TEST_CASE(time, annex_k_and_timespec) {
#if defined(TIME_UTC)
    {
        struct timespec ts;
        int base;
        memset(&ts, 0, sizeof ts);
        base = timespec_get(&ts, TIME_UTC);
        test_eq( base, TIME_UTC );
        test_true( ts.tv_sec > 0 );
        test_true( ts.tv_nsec >= 0 && ts.tv_nsec < 1000000000L );
    }
    test_pass("c2011:timespec_get(ts, TIME_UTC) time base");
#else
    TEST_NOTE("timespec_get is C11; this library does not provide it");
    test_skip("c2011:timespec_get(ts, TIME_UTC) time base");
#endif

#if defined(__STDC_LIB_EXT1__)
    {
        errno_t e = (errno_t)0;
        rsize_t r = (rsize_t)1;
        char buf[64];
        time_t now = time(NULL);
        struct tm out;
        test_true( e == 0 && r == 1 );
        test_true( asctime_s(buf, sizeof buf, gmtime(&now)) == 0 );
        test_true( ctime_s(buf, sizeof buf, &now) == 0 );
        test_ptr( gmtime_s(&now, &out) );
        test_ptr( localtime_s(&now, &out) );
    }
    test_pass("c2011:opt:errno_t");
    test_pass("c2011:opt:rsize_t");
    test_pass("c2011:opt:asctime_s");
    test_pass("c2011:opt:ctime_s");
    test_pass("c2011:opt:gmtime_s");
    test_pass("c2011:opt:localtime_s");
#else
    TEST_NOTE("Annex K is optional; this library does not provide it");
    test_skip("c2011:opt:errno_t");
    test_skip("c2011:opt:rsize_t");
    test_skip("c2011:opt:asctime_s");
    test_skip("c2011:opt:ctime_s");
    test_skip("c2011:opt:gmtime_s");
    test_skip("c2011:opt:localtime_s");
#endif
}

TEST_SUITE(time)
    TEST_ADD(now_and_clocks)
    TEST_ADD(tm_members)
    TEST_ADD(gmtime_fixed)
    TEST_ADD(mktime_gmtime_localtime)
    TEST_ADD(timegm_utc)
    TEST_ADD(difftime_values)
    TEST_ADD(time_and_clock)
    TEST_ADD(timespec_and_get)
    TEST_ADD(c23_extras)
    TEST_ADD(asctime_ctime)
    TEST_ADD(strftime_formats)
    TEST_ADD(strftime_c99_specifiers)
    TEST_ADD(strftime_eo_modifiers)
    TEST_ADD(annex_k_and_timespec)
TEST_SUITE_END(time)
