#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020 && _tst_cplusplus >= 201103L
#include <chrono>
#include <compare>
#include <ctime>
#include <format>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>

using _ccw::string_view;

namespace {

typedef STD::chrono::time_point<STD::chrono::system_clock, STD::chrono::milliseconds> sys_ms;
typedef STD::chrono::time_point<STD::chrono::steady_clock, STD::chrono::nanoseconds>  stdy_ns;

STD::chrono::sys_seconds fixed_instant() {
    using namespace STD::chrono;
    return sys_seconds(sys_days(2024y / February / 29d)) + hours(12) + minutes(34) + seconds(56);
}

}

TEST_CASE(chrono, time_point_members) {
    using namespace STD::chrono;

    test_true( (STD::is_same<sys_ms::clock, system_clock>::value) );
    test_true( (STD::is_same<sys_ms::duration, milliseconds>::value) );
    test_true( (STD::is_same<sys_ms::rep, milliseconds::rep>::value) );
    test_true( (STD::is_same<sys_ms::period, STD::milli>::value) );
    test_true( (STD::is_same<stdy_ns::clock, steady_clock>::value) );
    test_true( (STD::is_same<stdy_ns::period, STD::nano>::value) );
    test_pass("cxx11:time_point member types");

    sys_ms t0;
    test_eq( t0.time_since_epoch().count(), 0LL );
    test_true( t0.time_since_epoch() == milliseconds::zero() );
    test_pass("cxx11:time_point::time_since_epoch");

    sys_ms t1(milliseconds(1500));
    test_eq( t1.time_since_epoch().count(), 1500LL );
    test_pass("cxx11:time_point ctor from duration");

    sys_seconds ts(seconds(7));
    sys_ms t2(ts);
    test_eq( t2.time_since_epoch().count(), 7000LL );
    time_point<system_clock, nanoseconds> t3(t1);
    test_eq( t3.time_since_epoch().count(), 1500000000LL );
    test_pass("cxx11:time_point converting ctor");

    sys_ms t4(milliseconds(1000));
    t4 += milliseconds(500);
    test_eq( t4.time_since_epoch().count(), 1500LL );
    t4 -= milliseconds(200);
    test_eq( t4.time_since_epoch().count(), 1300LL );
    test_pass("cxx11:time_point compound assignment");

    test_true( sys_ms::min() < sys_ms() );
    test_true( sys_ms::min().time_since_epoch() == milliseconds::min() );
    test_pass("cxx11:time_point::min");
    test_true( sys_ms::max() > sys_ms() );
    test_true( sys_ms::max().time_since_epoch() == milliseconds::max() );
    test_pass("cxx11:time_point::max");

    typedef STD::common_type<sys_ms, sys_seconds>::type CT;
    test_true( (STD::is_same<CT, sys_ms>::value) );
    typedef STD::common_type<time_point<system_clock, seconds>,
                             time_point<system_clock, microseconds> >::type CT2;
    test_true( (STD::is_same<CT2, time_point<system_clock, microseconds> >::value) );
    test_pass("cxx11:common_type<time_point>");
}

TEST_CASE(chrono, time_point_nonmember_ops) {
    using namespace STD::chrono;

    sys_seconds base(seconds(1000));

    sys_seconds a = base + seconds(30);
    test_eq( a.time_since_epoch().count(), 1030LL );
    sys_seconds b = seconds(30) + base;
    test_eq( b.time_since_epoch().count(), 1030LL );

    sys_ms c = base + milliseconds(250);
    test_eq( c.time_since_epoch().count(), 1000250LL );
    test_pass("cxx11:time_point operator+");

    sys_seconds d = base - seconds(100);
    test_eq( d.time_since_epoch().count(), 900LL );
    test_pass("cxx11:time_point operator-(duration)");

    seconds diff = a - base;
    test_eq( diff.count(), 30LL );
    milliseconds diff2 = c - base;
    test_eq( diff2.count(), 250LL );
    test_true( (base - a) == seconds(-30) );
    test_pass("cxx11:time_point operator-(time_point)");

    test_true( a == b );
    test_true( a != base );
    test_true( base <  a );
    test_true( base <= a );
    test_true( a >  base );
    test_true( a >= b );
    test_true( sys_seconds(seconds(1)) == sys_ms(milliseconds(1000)) );
    test_true( sys_ms(milliseconds(999)) < sys_seconds(seconds(1)) );
    test_pass("cxx11:time_point comparisons");

#if defined(__cpp_impl_three_way_comparison)
    STD::strong_ordering o1 = a <=> base;
    test_true( o1 == STD::strong_ordering::greater );
    STD::strong_ordering o2 = a <=> b;
    test_true( o2 == STD::strong_ordering::equal );
    STD::strong_ordering o3 = base <=> a;
    test_true( o3 == STD::strong_ordering::less );
    test_pass("cxx20:time_point operator<=>");
#else
    TEST_NOTE("operator<=> is a C++20 language feature");
    TEST_SKIP_N(3);
    test_skip("cxx20:time_point operator<=>");
#endif

    sys_ms t(milliseconds(1999));
    test_eq( time_point_cast<seconds>(t).time_since_epoch().count(), 1LL );
    test_pass("cxx11:time_point_cast");
    test_eq( floor<seconds>(t).time_since_epoch().count(), 1LL );
    test_pass("cxx17:chrono floor(time_point)");
    test_eq( ceil<seconds>(t).time_since_epoch().count(), 2LL );
    test_pass("cxx17:chrono ceil(time_point)");
    test_eq( round<seconds>(t).time_since_epoch().count(), 2LL );
    sys_ms t2(milliseconds(1499));
    test_eq( round<seconds>(t2).time_since_epoch().count(), 1LL );
    test_pass("cxx17:chrono round(time_point)");

    sys_ms tn(milliseconds(-1500));
    test_eq( time_point_cast<seconds>(tn).time_since_epoch().count(), -1LL );
    test_eq( floor<seconds>(tn).time_since_epoch().count(), -2LL );
    test_eq( ceil<seconds>(tn).time_since_epoch().count(), -1LL );

    test_eq( time_point_cast<milliseconds>(sys_seconds(seconds(2))).time_since_epoch().count(), 2000LL );
    test_pass("cxx11:time_point_cast (negative/finer)");
}

TEST_CASE(chrono, clock_common_members) {
    using namespace STD::chrono;

    test_true( (STD::is_same<system_clock::duration,
                             duration<system_clock::rep, system_clock::period> >::value) );
    test_true( (STD::is_same<system_clock::time_point,
                             time_point<system_clock, system_clock::duration> >::value) );
    test_true( STD::numeric_limits<system_clock::rep>::is_signed );
    test_true( system_clock::is_steady == false );
    test_pass("cxx11:system_clock");

    test_true( (STD::is_same<steady_clock::duration,
                             duration<steady_clock::rep, steady_clock::period> >::value) );
    test_true( (STD::is_same<steady_clock::time_point,
                             time_point<steady_clock, steady_clock::duration> >::value) );
    test_true( steady_clock::is_steady == true );
    test_pass("cxx11:steady_clock");

    test_true( (STD::is_same<high_resolution_clock::time_point,
                 time_point<high_resolution_clock, high_resolution_clock::duration> >::value) );
    test_true( high_resolution_clock::period::den >= 1000 );
    (void)high_resolution_clock::is_steady;
    test_pass("cxx11:high_resolution_clock");

#if _TST_HAS_CHRONO_CXX20
    test_true(  STD::chrono::is_clock<system_clock>::value );
    test_true(  STD::chrono::is_clock<steady_clock>::value );
    test_true(  STD::chrono::is_clock_v<high_resolution_clock> );
    test_true(  STD::chrono::is_clock_v<utc_clock> );
    test_true(  STD::chrono::is_clock_v<tai_clock> );
    test_true(  STD::chrono::is_clock_v<gps_clock> );
    test_true(  STD::chrono::is_clock_v<file_clock> );
    test_true( !STD::chrono::is_clock_v<int> );
    test_true( !STD::chrono::is_clock_v<seconds> );
    test_pass("cxx20:is_clock");
#else
    TEST_NOTE("this library has no C++20 chrono clocks (is_clock / utc / tai / gps / file)");
    test_skip("cxx20:is_clock");
#endif

    system_clock::time_point s1 = system_clock::now();
    system_clock::time_point s2 = system_clock::now();
    test_true( s1 <= s2 );
    test_true( s1 > sys_seconds(sys_days(2020y / January / 1d)) );
    test_pass("cxx11:system_clock::now");

    steady_clock::time_point k1 = steady_clock::now();
    steady_clock::time_point k2 = steady_clock::now();
    test_true( k1 <= k2 );
    test_true( (k2 - k1) >= steady_clock::duration::zero() );
    test_pass("cxx11:steady_clock::now");

    high_resolution_clock::time_point h1 = high_resolution_clock::now();
    high_resolution_clock::time_point h2 = high_resolution_clock::now();
    test_true( h1 <= h2 );
    test_pass("cxx11:high_resolution_clock::now");

#if _TST_HAS_CHRONO_CXX20
    utc_clock::time_point u1 = utc_clock::now();
    test_true( u1.time_since_epoch() > seconds(0) );
    test_pass("cxx20:utc_clock::now");
    tai_clock::time_point ta1 = tai_clock::now();
    test_true( ta1.time_since_epoch() > seconds(0) );
    test_pass("cxx20:tai_clock::now");
    gps_clock::time_point g1 = gps_clock::now();
    test_true( g1.time_since_epoch() > seconds(0) );
    test_pass("cxx20:gps_clock::now");
    file_clock::time_point fc1 = file_clock::now();
    test_true( fc1.time_since_epoch() != file_clock::duration::zero() );
    test_pass("cxx20:file_clock::now");
#else
    TEST_NOTE("this library has no C++20 chrono clocks (is_clock / utc / tai / gps / file)");
    test_skip("cxx20:utc_clock::now");
    test_skip("cxx20:tai_clock::now");
    test_skip("cxx20:gps_clock::now");
    test_skip("cxx20:file_clock::now");
#endif
}

TEST_CASE(chrono, system_clock_time_t) {
    using namespace STD::chrono;

    sys_seconds inst = fixed_instant();
    STD::time_t tt = system_clock::to_time_t(system_clock::time_point(inst));

    test_eq( (long long)tt, 1709210096LL );
    test_pass("cxx11:system_clock::to_time_t");

    system_clock::time_point back = system_clock::from_time_t(tt);
    test_true( floor<seconds>(back) == inst );

    STD::time_t zero = system_clock::to_time_t(system_clock::from_time_t(0));
    test_eq( (long long)zero, 0LL );
    test_pass("cxx11:system_clock::from_time_t");
}

TEST_CASE(chrono, clock_aliases) {
    using namespace STD::chrono;

    test_true( (STD::is_same<sys_time<seconds>, time_point<system_clock, seconds> >::value) );
    test_true( (STD::is_same<sys_seconds, sys_time<seconds> >::value) );
    test_true( (STD::is_same<sys_days, sys_time<days> >::value) );
    test_pass("cxx20:sys_time aliases");

#if _TST_HAS_CHRONO_CXX20
    test_true( (STD::is_same<utc_time<seconds>, time_point<utc_clock, seconds> >::value) );
    test_true( (STD::is_same<utc_seconds, utc_time<seconds> >::value) );
    test_pass("cxx20:utc_time aliases");

    test_true( (STD::is_same<tai_time<seconds>, time_point<tai_clock, seconds> >::value) );
    test_true( (STD::is_same<tai_seconds, tai_time<seconds> >::value) );
    test_pass("cxx20:tai_time aliases");

    test_true( (STD::is_same<gps_time<seconds>, time_point<gps_clock, seconds> >::value) );
    test_true( (STD::is_same<gps_seconds, gps_time<seconds> >::value) );
    test_pass("cxx20:gps_time aliases");

    test_true( (STD::is_same<file_time<seconds>, time_point<file_clock, seconds> >::value) );
    test_pass("cxx20:file_time aliases");
#else
    TEST_NOTE("this library has no C++20 chrono clocks (utc / tai / gps)");
    test_skip("cxx20:utc_time aliases");
    test_skip("cxx20:tai_time aliases");
    test_skip("cxx20:gps_time aliases");
    test_skip("cxx20:file_time aliases");
#endif

    test_true( (STD::is_same<local_time<seconds>, time_point<local_t, seconds> >::value) );
    test_true( (STD::is_same<local_seconds, local_time<seconds> >::value) );
    test_true( (STD::is_same<local_days, local_time<days> >::value) );
#if _TST_HAS_CHRONO_CXX20
    test_true( !STD::chrono::is_clock_v<local_t> );
#else
    TEST_SKIP1();
#endif

    local_days ld(sys_days(2024y / February / 29d).time_since_epoch());
    local_seconds ls = ld + hours(12);
    test_eq( ls.time_since_epoch().count(), 1709208000LL + 0LL );
    test_true( (STD::is_same<decltype(ls), local_seconds>::value) );
    test_pass("cxx20:local_time aliases");
}

TEST_CASE(chrono, time_point_stream_output) {
    using namespace STD::chrono;

    sys_seconds inst = fixed_instant();

    STD::ostringstream os1;  os1 << inst;
    test_str_eq( os1.str(), "2024-02-29 12:34:56" );
    test_pass("cxx20:operator<<(sys_time)");

    STD::ostringstream os2;  os2 << floor<days>(inst);
    test_str_eq( os2.str(), "2024-02-29" );
    test_pass("cxx20:operator<<(sys_days)");

#if _TST_HAS_CHRONO_CXX20
    STD::ostringstream os3;  os3 << clock_cast<utc_clock>(inst);
    test_str_eq( os3.str(), "2024-02-29 12:34:56" );
    test_pass("cxx20:operator<<(utc_time)");
    STD::ostringstream os4;  os4 << clock_cast<tai_clock>(inst);
    test_str_eq( os4.str(), "2024-02-29 12:35:33" );
    test_pass("cxx20:operator<<(tai_time)");
    STD::ostringstream os5;  os5 << clock_cast<gps_clock>(inst);
    test_str_eq( os5.str(), "2024-02-29 12:35:14" );
    test_pass("cxx20:operator<<(gps_time)");
    STD::ostringstream os6;  os6 << clock_cast<file_clock>(inst);
    test_str_eq( os6.str(), "2024-02-29 12:34:56" );
    test_pass("cxx20:operator<<(file_time)");
#else
    TEST_NOTE("this library has no utc / tai / gps / file clocks, so clock_cast has no targets");
    test_skip("cxx20:operator<<(utc_time)");
    test_skip("cxx20:operator<<(tai_time)");
    test_skip("cxx20:operator<<(gps_time)");
    test_skip("cxx20:operator<<(file_time)");
#endif
    local_seconds ls(inst.time_since_epoch());
    STD::ostringstream os7;  os7 << ls;
    test_str_eq( os7.str(), "2024-02-29 12:34:56" );
    test_pass("cxx20:operator<<(local_time)");

    test_str_eq( STD::format("{}", inst), "2024-02-29 12:34:56" );
    test_str_eq( STD::format("{:%F %T}", inst), "2024-02-29 12:34:56" );
    test_str_eq( STD::format("{:%Y/%m/%d}", inst), "2024/02/29" );
    test_str_eq( STD::format("{:%H%M%S}", inst), "123456" );
#if _TST_HAS_CHRONO_CXX20
    test_str_eq( STD::format("{}", clock_cast<utc_clock>(inst)), "2024-02-29 12:34:56" );
    test_str_eq( STD::format("{:%F}", clock_cast<tai_clock>(inst)), "2024-02-29" );
    test_str_eq( STD::format("{:%F}", clock_cast<gps_clock>(inst)), "2024-02-29" );
    test_str_eq( STD::format("{:%F}", clock_cast<file_clock>(inst)), "2024-02-29" );
#else
    TEST_SKIP_N(4);
#endif
    test_str_eq( STD::format("{:%F %T}", ls), "2024-02-29 12:34:56" );

    test_str_eq( STD::format("{:%Z}", inst), "UTC" );
    test_pass("cxx20:format sys_time/etc");
}

#if !defined(__GNUC__)
TEST_CASE(chrono, time_point_from_stream) {
    using namespace STD::chrono;

    sys_seconds inst = fixed_instant();

    STD::istringstream is1("2024-02-29 12:34:56");
    sys_seconds s{};
    from_stream(is1, "%F %T", s);
    test_true( !is1.fail() );
    test_true( s == inst );
    test_pass("cxx20:from_stream(sys_time)");

    STD::istringstream is2("2024-02-29");
    sys_days sd{};
    from_stream(is2, "%F", sd);
    test_true( !is2.fail() );
    test_true( sd == floor<days>(inst) );
    test_pass("cxx20:from_stream(sys_days)");

    STD::istringstream is3("2024-02-29 12:34:56");
    utc_seconds u{};
    from_stream(is3, "%F %T", u);
    test_true( !is3.fail() );
    test_true( u == clock_cast<utc_clock>(inst) );
    test_pass("cxx20:from_stream(utc_time)");

    STD::istringstream is4("2024-02-29 12:35:33");
    tai_seconds t{};
    from_stream(is4, "%F %T", t);
    test_true( !is4.fail() );
    test_true( t == clock_cast<tai_clock>(inst) );
    test_pass("cxx20:from_stream(tai_time)");

    STD::istringstream is5("2024-02-29 12:35:14");
    gps_seconds g{};
    from_stream(is5, "%F %T", g);
    test_true( !is5.fail() );
    test_true( g == clock_cast<gps_clock>(inst) );
    test_pass("cxx20:from_stream(gps_time)");

    STD::istringstream is6("2024-02-29 12:34:56");
    file_time<seconds> f{};
    from_stream(is6, "%F %T", f);
    test_true( !is6.fail() );
    test_true( f == clock_cast<file_clock>(inst) );
    test_pass("cxx20:from_stream(file_time)");

    STD::istringstream is7("2024-02-29 12:34:56");
    local_seconds ls{};
    from_stream(is7, "%F %T", ls);
    test_true( !is7.fail() );
    test_eq( ls.time_since_epoch().count(), inst.time_since_epoch().count() );
    test_pass("cxx20:from_stream(local_time)");

    STD::istringstream is8("not-a-date");
    sys_seconds bad{};
    from_stream(is8, "%F %T", bad);
    test_true( is8.fail() );
    test_pass("cxx20:from_stream failbit");
}
#else
TEST_CASE_SKIP(chrono, time_point_from_stream)
#endif


#define _TST_CLOCK_MEMBERS(C, TAG)                                                     \
    do {                                                                               \
        test_true( (STD::is_same<C::rep, C::duration::rep>::value) );                  \
        test_true( STD::numeric_limits<C::rep>::is_specialized );                       \
        test_pass(TAG "::rep");                                                        \
        test_true( (STD::is_same<C::period, C::duration::period>::value) );            \
        test_true( C::period::num > 0 && C::period::den > 0 );                         \
        test_pass(TAG "::period");                                                     \
        test_true( (STD::is_same<C::duration,                                          \
                                 STD::chrono::duration<C::rep, C::period> >::value) ); \
        test_true( (STD::is_same<C::duration, C::time_point::duration>::value) );      \
        test_pass(TAG "::duration");                                                   \
        test_true( (STD::is_same<C::time_point,                                        \
                                 STD::chrono::time_point<C, C::duration> >::value) );  \
        test_true( C::time_point() == C::time_point(C::duration::zero()) );            \
        test_pass(TAG "::time_point");                                                 \
    } while (0)

TEST_CASE(chrono, clock_member_types_cxx11) {
    using namespace STD::chrono;
    _TST_CLOCK_MEMBERS(system_clock, "cxx11:system_clock");
    _TST_CLOCK_MEMBERS(steady_clock, "cxx11:steady_clock");
    _TST_CLOCK_MEMBERS(high_resolution_clock, "cxx11:high_resolution_clock");
}

TEST_CASE(chrono, clock_member_types_cxx20) {
#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_chrono) && (__cpp_lib_chrono) >= 201907L
    using namespace STD::chrono;
    _TST_CLOCK_MEMBERS(file_clock, "cxx20:file_clock");
    _TST_CLOCK_MEMBERS(gps_clock, "cxx20:gps_clock");
    _TST_CLOCK_MEMBERS(tai_clock, "cxx20:tai_clock");
    _TST_CLOCK_MEMBERS(utc_clock, "cxx20:utc_clock");

    { const bool b = file_clock::is_steady; test_true( b == true || b == false );
      test_true( (STD::is_same<const bool, decltype(file_clock::is_steady)>::value) ); }
    test_pass("cxx20:file_clock::is_steady");
    { const bool b = gps_clock::is_steady; test_true( b == true || b == false );
      test_true( (STD::is_same<const bool, decltype(gps_clock::is_steady)>::value) ); }
    test_pass("cxx20:gps_clock::is_steady");
    { const bool b = tai_clock::is_steady; test_true( b == true || b == false );
      test_true( (STD::is_same<const bool, decltype(tai_clock::is_steady)>::value) ); }
    test_pass("cxx20:tai_clock::is_steady");
    { const bool b = utc_clock::is_steady; test_true( b == true || b == false );
      test_true( (STD::is_same<const bool, decltype(utc_clock::is_steady)>::value) ); }
    test_pass("cxx20:utc_clock::is_steady");

    test_true( STD::chrono::is_clock_v<file_clock> && STD::chrono::is_clock_v<gps_clock> );
    test_true( STD::chrono::is_clock_v<tai_clock> && STD::chrono::is_clock_v<utc_clock> );
    test_pass("cxx20:file_clock");
    test_pass("cxx20:gps_clock");
    test_pass("cxx20:tai_clock");
    test_pass("cxx20:utc_clock");
#else
    test_skip("cxx20:file_clock::rep");
    test_skip("cxx20:file_clock::period");
    test_skip("cxx20:file_clock::duration");
    test_skip("cxx20:file_clock::time_point");
    test_skip("cxx20:gps_clock::rep");
    test_skip("cxx20:gps_clock::period");
    test_skip("cxx20:gps_clock::duration");
    test_skip("cxx20:gps_clock::time_point");
    test_skip("cxx20:tai_clock::rep");
    test_skip("cxx20:tai_clock::period");
    test_skip("cxx20:tai_clock::duration");
    test_skip("cxx20:tai_clock::time_point");
    test_skip("cxx20:utc_clock::rep");
    test_skip("cxx20:utc_clock::period");
    test_skip("cxx20:utc_clock::duration");
    test_skip("cxx20:utc_clock::time_point");
    test_skip("cxx20:file_clock::is_steady");
    test_skip("cxx20:gps_clock::is_steady");
    test_skip("cxx20:tai_clock::is_steady");
    test_skip("cxx20:utc_clock::is_steady");
    test_skip("cxx20:file_clock");
    test_skip("cxx20:gps_clock");
    test_skip("cxx20:tai_clock");
    test_skip("cxx20:utc_clock");
#endif
}

TEST_CASE(chrono, time_point_member_types_and_ops) {
    using namespace STD::chrono;
    typedef time_point<system_clock, milliseconds> tp;

    test_true( (STD::is_same<tp::clock, system_clock>::value) );
    test_true( (STD::is_same<tp::clock::time_point::clock, system_clock>::value) );
    test_pass("cxx11:time_point::clock");

    test_true( (STD::is_same<tp::duration, milliseconds>::value) );
    tp t0(tp::duration(3));
    test_eq( (long long)t0.time_since_epoch().count(), 3LL );
    test_pass("cxx11:time_point::duration");

    test_true( (STD::is_same<tp::rep, milliseconds::rep>::value) );
    test_true( (STD::is_same<tp::rep, tp::duration::rep>::value) );
    test_pass("cxx11:time_point::rep");

    test_true( (STD::is_same<tp::period, STD::milli>::value) );
    test_true( (STD::is_same<tp::period, tp::duration::period>::value) );
    test_pass("cxx11:time_point::period");

    tp t(milliseconds(1000));
    tp& r1 = (t += milliseconds(500));
    test_eq( (long long)t.time_since_epoch().count(), 1500LL );
    test_true( &r1 == &t );
    test_pass("cxx11:time_point::operator+=");

    tp& r2 = (t -= milliseconds(200));
    test_eq( (long long)t.time_since_epoch().count(), 1300LL );
    test_true( &r2 == &t );
    test_pass("cxx11:time_point::operator-=");

#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_chrono) && (__cpp_lib_chrono) >= 201907L
    tp u(milliseconds(5));
    tp& r3 = (++u);
    test_eq( (long long)u.time_since_epoch().count(), 6LL );
    test_true( &r3 == &u );
    tp post = u++;
    test_eq( (long long)post.time_since_epoch().count(), 6LL );
    test_eq( (long long)u.time_since_epoch().count(), 7LL );
    test_pass("cxx20:time_point::operator++");

    tp& r4 = (--u);
    test_eq( (long long)u.time_since_epoch().count(), 6LL );
    test_true( &r4 == &u );
    tp pre = u--;
    test_eq( (long long)pre.time_since_epoch().count(), 6LL );
    test_eq( (long long)u.time_since_epoch().count(), 5LL );
    test_pass("cxx20:time_point::operator--");
#else
    test_skip("cxx20:time_point::operator++");
    test_skip("cxx20:time_point::operator--");
#endif
}

TEST_CASE(chrono, clock_time_point_aliases_cxx20) {
#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_chrono) && (__cpp_lib_chrono) >= 201907L
    using namespace STD::chrono;

    test_true(( STD::is_same<sys_time<seconds>, time_point<system_clock, seconds> >::value ));
    test_pass("cxx20:sys_time");
    test_true(( STD::is_same<sys_seconds, sys_time<seconds> >::value ));
    test_pass("cxx20:sys_seconds");
    test_true(( STD::is_same<sys_days, sys_time<days> >::value ));
    {
        sys_days epoch{};
        test_eq( (long long)epoch.time_since_epoch().count(), 0LL );
    }
    test_pass("cxx20:sys_days");

    test_true(( STD::is_same<local_time<seconds>, time_point<local_t, seconds> >::value ));
    test_pass("cxx20:local_t");
    test_true(( STD::is_same<local_time<milliseconds>,
                             time_point<local_t, milliseconds> >::value ));
    test_pass("cxx20:local_time");
    test_true(( STD::is_same<local_seconds, local_time<seconds> >::value ));
    test_pass("cxx20:local_seconds");
    test_true(( STD::is_same<local_days, local_time<days> >::value ));
    test_pass("cxx20:local_days");

    test_true(( STD::is_same<file_time<seconds>, time_point<file_clock, seconds> >::value ));
    test_pass("cxx20:file_time");

    test_true(( STD::is_same<gps_time<seconds>, time_point<gps_clock, seconds> >::value ));
    test_pass("cxx20:gps_time");
    test_true(( STD::is_same<gps_seconds, gps_time<seconds> >::value ));
    test_pass("cxx20:gps_seconds");

    test_true(( STD::is_same<tai_time<seconds>, time_point<tai_clock, seconds> >::value ));
    test_pass("cxx20:tai_time");
    test_true(( STD::is_same<tai_seconds, tai_time<seconds> >::value ));
    test_pass("cxx20:tai_seconds");

    test_true(( STD::is_same<utc_time<seconds>, time_point<utc_clock, seconds> >::value ));
    test_pass("cxx20:utc_time");
    test_true(( STD::is_same<utc_seconds, utc_time<seconds> >::value ));
    test_pass("cxx20:utc_seconds");

# if defined(_TST_HAS_CHRONO_FILE_SECONDS)
    test_true(( STD::is_same<file_seconds, file_time<seconds> >::value ));
    test_pass("cxx20:file_seconds");
# else
    TEST_NOTE("no [time.syn] declares file_seconds; the file clock has only file_time");
    test_skip("cxx20:file_seconds");
# endif
#else
    test_skip("cxx20:sys_time");
    test_skip("cxx20:sys_seconds");
    test_skip("cxx20:sys_days");
    test_skip("cxx20:local_t");
    test_skip("cxx20:local_time");
    test_skip("cxx20:local_seconds");
    test_skip("cxx20:local_days");
    test_skip("cxx20:file_time");
    test_skip("cxx20:gps_time");
    test_skip("cxx20:gps_seconds");
    test_skip("cxx20:tai_time");
    test_skip("cxx20:tai_seconds");
    test_skip("cxx20:utc_time");
    test_skip("cxx20:utc_seconds");
    test_skip("cxx20:file_seconds");
#endif
}
#else
TEST_CASE_SKIP(chrono, time_point_members)
TEST_CASE_SKIP(chrono, time_point_nonmember_ops)
TEST_CASE_SKIP(chrono, clock_common_members)
TEST_CASE_SKIP(chrono, system_clock_time_t)
TEST_CASE_SKIP(chrono, clock_aliases)
TEST_CASE_SKIP(chrono, time_point_stream_output)
TEST_CASE_SKIP(chrono, time_point_from_stream)
TEST_CASE_SKIP(chrono, clock_member_types_cxx11)
TEST_CASE_SKIP(chrono, clock_member_types_cxx20)
TEST_CASE_SKIP(chrono, time_point_member_types_and_ops)
TEST_CASE_SKIP(chrono, clock_time_point_aliases_cxx20)
#endif
