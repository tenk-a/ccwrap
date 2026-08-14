#include "test_cxx.hpp"

#include <ctime>
#include <cstring>

TEST_CASE(ctime, now) {
    STD_NS time_t t = STD_NS time(0);
    test_true( t > 0 );
    test_pass("cxx03:time");
    STD_NS clock_t c = STD_NS clock();
    test_true( c >= 0 );
    test_pass("cxx03:clock");
    test_true( CLOCKS_PER_SEC > 0 );
    test_pass("cxx03:CLOCKS_PER_SEC");
}

TEST_CASE(ctime, gmtime_fixed) {
    STD_NS time_t epoch = 0;
    STD_NS tm* g = STD_NS gmtime(&epoch);
    test_ptr( g );
    char buf[64];
    STD_NS strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", g);
    test_eq( STD::strcmp(buf, "1970-01-01 00:00:00"), 0 );
    test_eq( g->tm_year, 70 );
    test_eq( g->tm_mon, 0 );
    test_eq( g->tm_mday, 1 );
    test_pass("cxx03:gmtime + strftime (epoch)");
}

TEST_CASE(ctime, mktime_roundtrip) {
    STD_NS tm t;
    STD::memset(&t, 0, sizeof t);
    t.tm_year = 2000 - 1900; t.tm_mon = 0; t.tm_mday = 2;
    t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5; t.tm_isdst = -1;

    STD_NS time_t tt = STD_NS mktime(&t);
    test_true( tt != (STD_NS time_t)-1 );
    STD_NS tm* lt = STD_NS localtime(&tt);
    test_ptr( lt );
    char buf[64];
    STD_NS strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", lt);
    test_eq( STD::strcmp(buf, "2000-01-02 03:04:05"), 0 );
    test_eq( lt->tm_wday, 0 );
    test_pass("cxx03:mktime");
}

#if TEST_TARGET_CXX >= 2026 && _TST_HAS_STD_C23_LIB
TEST_CASE(ctime, timegm) {
    STD_NS tm t;
    STD::memset(&t, 0, sizeof t);
    t.tm_year = 70; t.tm_mon = 0; t.tm_mday = 1;
    t.tm_isdst = 0;
    test_eq( STD_NS timegm(&t), (STD_NS time_t)0 );

    STD::memset(&t, 0, sizeof t);
    t.tm_year = 2000 - 1900; t.tm_mon = 0; t.tm_mday = 2;
    t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5; t.tm_isdst = 0;
    STD_NS time_t tt = STD_NS timegm(&t);
    test_true( tt != (STD_NS time_t)-1 );

    STD_NS tm* g = STD_NS gmtime(&tt);
    test_ptr( g );
    char buf[64];
    STD_NS strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", g);
    test_eq( STD::strcmp(buf, "2000-01-02 03:04:05"), 0 );

    STD::memset(&t, 0, sizeof t);
    t.tm_year = 2001 - 1900; t.tm_mon = 8; t.tm_mday = 9;
    t.tm_hour = 1; t.tm_min = 46; t.tm_sec = 40; t.tm_isdst = 0;
    test_eq( STD_NS timegm(&t), (STD_NS time_t)1000000000 );

    STD_NS tm a, b;
    STD::memset(&a, 0, sizeof a); STD::memset(&b, 0, sizeof b);
    a.tm_year = b.tm_year = 2000 - 1900;
    a.tm_mon = b.tm_mon = 6;
    a.tm_mday = 1; b.tm_mday = 2;
    a.tm_isdst = b.tm_isdst = 0;
    test_eq( (long)(STD_NS timegm(&b) - STD_NS timegm(&a)), 86400L );
    test_pass("cxx26:timegm");
}
#else
TEST_CASE_SKIP(ctime, timegm)
#endif

TEST_CASE(ctime, difftime) {
    test_true( STD_NS difftime((STD_NS time_t)120, (STD_NS time_t)60) == 60.0 );
    test_true( STD_NS difftime((STD_NS time_t)0, (STD_NS time_t)0) == 0.0 );
    test_pass("cxx03:difftime");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(ctime, timespec_members) {
    STD_NS timespec ts;
    ts.tv_sec  = (STD_NS time_t)123;
    ts.tv_nsec = 456789L;
    test_true( ts.tv_sec == (STD_NS time_t)123 );
    test_eq( ts.tv_nsec, 456789L );

    STD_NS time_t epoch = 0;
    STD_NS tm* g = STD_NS gmtime(&epoch);
    test_ptr( g );
    test_eq( g->tm_yday, 0 );
    test_eq( g->tm_hour, 0 );
    test_eq( g->tm_min, 0 );
    test_eq( g->tm_sec, 0 );
    test_pass("cxx17:struct timespec");
}
#else
TEST_CASE(ctime, timespec_members) {
    TEST_NOTE("std::timespec is C++17");
    test_skip("cxx17:struct timespec");
}
#endif

#if TEST_TARGET_CXX >= 2011
#include <ctime>
#include <cstddef>
#include <cstring>
#include <string>

using _ccw::string_view;

static STD_NS tm make_y2k() {
    STD_NS tm t;
    STD::memset(&t, 0, sizeof t);
    t.tm_year  = 100;
    t.tm_mon   = 0;
    t.tm_mday  = 1;
    t.tm_hour  = 0;
    t.tm_min   = 0;
    t.tm_sec   = 0;
    t.tm_isdst = -1;
    return t;
}

TEST_CASE(ctime, types_and_macros) {

    STD_NS time_t* np = NULL;
    test_true( np == NULL );
    test_pass("cxx03:NULL");

    STD_NS size_t sz = sizeof(STD_NS tm);
    test_gt( sz, STD_NS size_t(0) );
    test_pass("cxx03:tm");

    test_true( STD_NS size_t(0) - 1 > 0 );
    test_pass("cxx03:size_t");

    STD_NS clock_t c = STD_NS clock_t(0);
    test_eq( (double)c, 0.0 );
    test_pass("cxx03:clock_t");

    STD_NS time_t  t = STD_NS time_t(0);
    test_eq( (double)t, 0.0 );
    test_true( sizeof(STD_NS time_t) >= 4 );
    test_pass("cxx03:time_t");

    test_gt( (double)CLOCKS_PER_SEC, 0.0 );
    test_pass("cxx03:CLOCKS_PER_SEC");

    test_ne( (int)TIME_UTC, 0 );
    test_pass("cxx17:TIME_UTC");
}

TEST_CASE(ctime, tm_members) {
    STD_NS tm t = make_y2k();

    t.tm_sec  = 59; t.tm_min = 58; t.tm_hour = 23;
    t.tm_mday = 31; t.tm_mon = 11; t.tm_year = 99;
    t.tm_wday = 5;  t.tm_yday = 364; t.tm_isdst = 0;
    test_eq( t.tm_sec,  59 );
    test_eq( t.tm_min,  58 );
    test_eq( t.tm_hour, 23 );
    test_eq( t.tm_mday, 31 );
    test_eq( t.tm_mon,  11 );
    test_eq( t.tm_year, 99 );
    test_eq( t.tm_wday, 5 );
    test_eq( t.tm_yday, 364 );
    test_eq( t.tm_isdst, 0 );
    test_pass("cxx03:struct tm members (tm_sec..tm_isdst)");
}

TEST_CASE(ctime, mktime_gmtime_localtime) {
    STD_NS tm t = make_y2k();
    STD_NS time_t tt = STD_NS mktime(&t);
    test_ne( tt, STD_NS time_t(-1) );

    test_eq( t.tm_wday, 6 );
    test_eq( t.tm_yday, 0 );
    test_eq( t.tm_year, 100 );
    test_pass("cxx03:mktime");

    STD_NS tm* lt = STD_NS localtime(&tt);
    test_ptr( lt );
    test_eq( lt->tm_year, 100 );
    test_eq( lt->tm_mon,  0 );
    test_eq( lt->tm_mday, 1 );
    test_eq( lt->tm_hour, 0 );
    test_eq( lt->tm_wday, 6 );
    test_pass("cxx03:localtime");

    STD_NS time_t utc = STD_NS time_t(946684800);
    STD_NS tm* gt = STD_NS gmtime(&utc);
    test_ptr( gt );
    test_eq( gt->tm_year, 100 );
    test_eq( gt->tm_mon,  0 );
    test_eq( gt->tm_mday, 1 );
    test_eq( gt->tm_hour, 0 );
    test_eq( gt->tm_min,  0 );
    test_eq( gt->tm_sec,  0 );
    test_eq( gt->tm_wday, 6 );
    test_eq( gt->tm_yday, 0 );
    test_eq( gt->tm_isdst, 0 );

    STD_NS time_t utc2 = utc + 24 * 60 * 60;
    STD_NS tm g2 = *STD_NS gmtime(&utc2);
    test_eq( g2.tm_mday, 2 );
    test_eq( g2.tm_wday, 0 );
    test_eq( g2.tm_yday, 1 );
    test_pass("cxx03:gmtime");
}

TEST_CASE(ctime, difftime_chk) {
    STD_NS time_t a = STD_NS time_t(946684800);
    STD_NS time_t b = a + 90;
    test_eq( STD_NS difftime(b, a),  90.0 );
    test_eq( STD_NS difftime(a, b), -90.0 );
    test_eq( STD_NS difftime(a, a),   0.0 );

    double d = STD_NS difftime(b, a);
    test_eq( d, 90.0 );
    test_pass("cxx03:difftime");
}

TEST_CASE(ctime, time_and_clock) {

    STD_NS time_t now = STD_NS time(NULL);
    test_ne( now, STD_NS time_t(-1) );
    test_gt( (double)now, 1577836800.0 );

    STD_NS time_t via_ptr = 0;
    STD_NS time_t ret = STD_NS time(&via_ptr);
    test_eq( (double)ret, (double)via_ptr );
    test_ge( STD_NS difftime(via_ptr, now), 0.0 );
    test_pass("cxx03:time");

    STD_NS clock_t c0 = STD_NS clock();
    test_ne( c0, STD_NS clock_t(-1) );
    volatile double spin = 0;
    for (int i = 0; i < 200000; ++i) spin += i;
    STD_NS clock_t c1 = STD_NS clock();
    test_ge( (double)c1, (double)c0 );

    double secs = double(c1 - c0) / CLOCKS_PER_SEC;
    test_lim( secs, 0.0, 60.0 );
    test_pass("cxx03:clock");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(ctime, timespec_get) {
    STD_NS timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 0;
    test_eq( (double)ts.tv_sec,  0.0 );
    test_eq( (double)ts.tv_nsec, 0.0 );

    int base = STD_NS timespec_get(&ts, TIME_UTC);
    test_gt( (double)ts.tv_sec, 1577836800.0 );
    test_lim( (double)ts.tv_nsec, 0.0, 999999999.0 );

    int bad = STD_NS timespec_get(&ts, 0);
    test_eq( bad, 0 );
    test_pass("cxx17:timespec_get");

    test_eq( base, (int)TIME_UTC );
    test_pass("cxx17:TIME_UTC");
}
#else
TEST_CASE(ctime, timespec_get) {
    TEST_NOTE("std::timespec / timespec_get are C++17");
    test_skip("cxx17:timespec_get");
    test_skip("cxx17:TIME_UTC");
}
#endif

TEST_CASE(ctime, asctime_ctime) {

    STD_NS time_t utc = STD_NS time_t(946684800);
    STD_NS tm g = *STD_NS gmtime(&utc);
    const char* a = STD_NS asctime(&g);
    test_ptr( a );
    test_eq( STD::strlen(a), STD_NS size_t(25) );
    test_str_eq( a, "Sat Jan  1 00:00:00 2000\n" );
    test_pass("cxx03:asctime");

    STD_NS time_t now = STD_NS time(NULL);
    STD_NS tm ln = *STD_NS localtime(&now);
    STD::string via_ctime(STD_NS ctime(&now));
    STD::string via_asctime(STD_NS asctime(&ln));
    test_str_eq( via_ctime.c_str(), via_asctime.c_str() );
    test_eq( via_ctime.size(), STD_NS size_t(25) );
    test_pass("cxx03:ctime");
}

TEST_CASE(ctime, strftime) {
    STD_NS time_t utc = STD_NS time_t(946684800);
    STD_NS tm g = *STD_NS gmtime(&utc);

    char buf[64];
    STD_NS size_t n = STD_NS strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", &g);
    test_eq( n, STD_NS size_t(19) );
    test_str_eq( buf, "2000-01-01 00:00:00" );

    n = STD_NS strftime(buf, sizeof buf, "%a %b %j", &g);
    test_eq( n, STD_NS size_t(11) );
    test_str_eq( buf, "Sat Jan 001" );

    n = STD_NS strftime(buf, sizeof buf, "100%%", &g);
    test_eq( n, STD_NS size_t(4) );
    test_str_eq( buf, "100%" );

    char small[4];
    STD_NS size_t z = STD_NS strftime(small, sizeof small, "%Y-%m-%d", &g);
    test_eq( z, STD_NS size_t(0) );

    buf[0] = 'x';
    STD_NS size_t e = STD_NS strftime(buf, sizeof buf, "", &g);
    test_eq( e, STD_NS size_t(0) );
    test_eq( (int)buf[0], 0 );
    test_pass("cxx03:strftime conversions");
}
#else
TEST_CASE_SKIP(ctime, types_and_macros)
TEST_CASE_SKIP(ctime, tm_members)
TEST_CASE_SKIP(ctime, mktime_gmtime_localtime)
TEST_CASE_SKIP(ctime, difftime_chk)
TEST_CASE_SKIP(ctime, time_and_clock)
TEST_CASE_SKIP(ctime, timespec_get)
TEST_CASE_SKIP(ctime, asctime_ctime)
TEST_CASE_SKIP(ctime, strftime)
#endif


TEST_CASE(ctime, c23_additions_cxx26) {
    TEST_NOTE("C++26/C23 <ctime> additions; no library here puts these in std");
    test_skip("cxx26:gmtime_r");
    test_skip("cxx26:localtime_r");
    test_skip("cxx26:timespec_getres");

#if defined(TIME_MONOTONIC)
    test_true( TIME_MONOTONIC != TIME_UTC );
    test_pass("cxx26:opt:TIME_MONOTONIC");
#else
    test_skip("cxx26:opt:TIME_MONOTONIC");
#endif
#if defined(TIME_ACTIVE)
    test_true( TIME_ACTIVE != TIME_UTC );
    test_pass("cxx26:opt:TIME_ACTIVE");
#else
    test_skip("cxx26:opt:TIME_ACTIVE");
#endif
#if defined(TIME_THREAD_ACTIVE)
    test_true( TIME_THREAD_ACTIVE != TIME_UTC );
    test_pass("cxx26:opt:TIME_THREAD_ACTIVE");
#else
    test_skip("cxx26:opt:TIME_THREAD_ACTIVE");
#endif
}


TEST_CASE(ctime, tm_members_cxx03) {
    STD_NS time_t t = (STD_NS time_t)946782245;
    STD_NS tm* g = STD_NS gmtime(&t);
    test_ptr( g );

    test_eq( g->tm_sec, 5 );
    test_pass("cxx03:tm::tm_sec");
    test_eq( g->tm_min, 4 );
    test_pass("cxx03:tm::tm_min");
    test_eq( g->tm_hour, 3 );
    test_pass("cxx03:tm::tm_hour");

    test_eq( g->tm_mday, 2 );
    test_pass("cxx03:tm::tm_mday");
    test_eq( g->tm_mon, 0 );
    test_pass("cxx03:tm::tm_mon");
    test_eq( g->tm_year, 100 );
    test_pass("cxx03:tm::tm_year");

    test_eq( g->tm_wday, 0 );
    test_pass("cxx03:tm::tm_wday");
    test_eq( g->tm_yday, 1 );
    test_pass("cxx03:tm::tm_yday");

    test_eq( g->tm_isdst, 0 );
    test_pass("cxx03:tm::tm_isdst");
}
