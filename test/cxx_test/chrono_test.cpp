#include "test_cxx.hpp"

#include <chrono>

TEST_CASE(chrono, duration_ops) {
    STD::chrono::seconds s(5);
    test_eq( (long long)s.count(), 5 );

    STD::chrono::seconds a(3), b(4);
    test_eq( (long long)(a + b).count(), 7 );
    test_eq( (long long)(b - a).count(), 1 );
    test_eq( (long long)(a * 2).count(), 6 );
    test_eq( (long long)(b / 2).count(), 2 );
    test_eq( (long long)(-a).count(), -3 );
    test_eq( (long long)(+a).count(), 3 );

    STD::chrono::seconds c(10);
    c += STD::chrono::seconds(5);  test_eq( (long long)c.count(), 15 );
    c -= STD::chrono::seconds(3);  test_eq( (long long)c.count(), 12 );
    c *= 2;                        test_eq( (long long)c.count(), 24 );
    c /= 4;                        test_eq( (long long)c.count(), 6 );
    ++c;                           test_eq( (long long)c.count(), 7 );
    --c;                           test_eq( (long long)c.count(), 6 );

    test_true( STD::chrono::seconds(1) == STD::chrono::seconds(1) );
    test_true( STD::chrono::seconds(1) != STD::chrono::seconds(2) );
    test_true( STD::chrono::seconds(1) <  STD::chrono::seconds(2) );
    test_true( STD::chrono::seconds(2) >  STD::chrono::seconds(1) );
    test_true( STD::chrono::seconds(1) <= STD::chrono::seconds(1) );
    test_true( STD::chrono::seconds(1) >= STD::chrono::seconds(1) );
    test_pass("cxx11:duration ops");
}

TEST_CASE(chrono, cast_and_types) {
    STD::chrono::milliseconds ms = STD::chrono::duration_cast<STD::chrono::milliseconds>(STD::chrono::seconds(2));
    test_eq( (long long)ms.count(), 2000 );
    STD::chrono::seconds sec = STD::chrono::duration_cast<STD::chrono::seconds>(STD::chrono::milliseconds(1500));
    test_eq( (long long)sec.count(), 1 );

    STD::chrono::hours        h(1);
    STD::chrono::minutes      mn = STD::chrono::duration_cast<STD::chrono::minutes>(h);
    test_eq( (long long)mn.count(), 60 );
    STD::chrono::nanoseconds  ns = STD::chrono::duration_cast<STD::chrono::nanoseconds>(STD::chrono::microseconds(1));
    test_eq( (long long)ns.count(), 1000 );
    STD::chrono::microseconds us = STD::chrono::duration_cast<STD::chrono::microseconds>(STD::chrono::milliseconds(1));
    test_eq( (long long)us.count(), 1000 );
    test_pass("cxx11:cast and types");
}

TEST_CASE(chrono, time_point_clocks) {
    typedef STD::chrono::steady_clock clock;
    clock::time_point t0 = clock::now();
    clock::time_point t1 = t0 + clock::duration(1000);
    test_true( t1 > t0 );
    test_true( t0 < t1 );
    test_true( t0 != t1 );
    test_eq( (long long)(t1 - t0).count(), 1000 );
    clock::time_point t2 = t1 - clock::duration(1000);
    test_true( t2 == t0 );

    t2 += clock::duration(5);
    test_true( t2 > t0 );
    t2 -= clock::duration(5);
    test_true( t2 == t0 );

    STD::chrono::system_clock::time_point sc = STD::chrono::system_clock::now();
    test_true( sc.time_since_epoch().count() != 0 );
    clock::time_point st = clock::now();
    test_true( st >= t0 );
    STD::chrono::high_resolution_clock::time_point hr = STD::chrono::high_resolution_clock::now();
    (void)hr.time_since_epoch();

    STD::chrono::milliseconds d = STD::chrono::duration_cast<STD::chrono::milliseconds>(clock::now() - t0);
    test_true( d.count() >= 0 );
    test_pass("cxx11:time point clocks");
}

TEST_CASE(chrono, count_and_is_steady) {
    STD::chrono::milliseconds d(250);
    test_eq( (long)d.count(), 250L );
    STD::chrono::seconds s(3);
    test_eq( (long)s.count(), 3L );
    test_pass("cxx11:duration::count");

    test_true( STD::chrono::steady_clock::is_steady );
    test_pass("cxx11:steady_clock::is_steady");

    bool sys = STD::chrono::system_clock::is_steady;
    (void)sys;
    test_true( sys == true || sys == false );
    test_pass("cxx11:system_clock::is_steady");

    bool hr = STD::chrono::high_resolution_clock::is_steady;
    (void)hr;
    test_true( hr == true || hr == false );
    test_pass("cxx11:high_resolution_clock::is_steady");
}

#if TEST_TARGET_CXX >= 2020 && _TST_HAS_CHRONO_CXX20
TEST_CASE(chrono, is_clock) {

    test_true( STD::chrono::is_clock<STD::chrono::system_clock>::value );
    test_true( STD::chrono::is_clock<STD::chrono::steady_clock>::value );
    test_true( STD::chrono::is_clock<STD::chrono::high_resolution_clock>::value );

    test_true( !STD::chrono::is_clock<STD::chrono::seconds>::value );
    test_true( !STD::chrono::is_clock<STD::chrono::system_clock::time_point>::value );

    test_true( !STD::chrono::is_clock<int>::value );
    test_true( !STD::chrono::is_clock<double>::value );
    test_true( !STD::chrono::is_clock<STD::chrono::system_clock*>::value );
    test_pass("cxx11:is clock");
}
#else
TEST_CASE_SKIP(chrono, is_clock)
#endif
