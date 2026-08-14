#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011
#include <chrono>
#include <functional>
#include <sstream>
#include <string>
#include <type_traits>

using _ccw::string_view;

#if _tst_cplusplus >= 202002L
#  define _TST_E_HAS_FORMAT 1
#  include <format>
#else
#  define _TST_E_HAS_FORMAT 0
#endif

#if TEST_TARGET_CXX >= 2020

TEST_CASE(chrono, am_pm_helpers) {
    using namespace STD::chrono;

    test_true(  is_am(hours(0)) );
    test_true(  is_am(hours(11)) );
    test_true( !is_am(hours(12)) );
    test_true( !is_am(hours(23)) );
    test_pass("cxx20:is_am");

    test_true( !is_pm(hours(0)) );
    test_true( !is_pm(hours(11)) );
    test_true(  is_pm(hours(12)) );
    test_true(  is_pm(hours(23)) );
    test_pass("cxx20:is_pm");

    test_eq( make12(hours(0)).count(),  12LL );
    test_eq( make12(hours(1)).count(),   1LL );
    test_eq( make12(hours(11)).count(), 11LL );
    test_eq( make12(hours(12)).count(), 12LL );
    test_eq( make12(hours(13)).count(),  1LL );
    test_eq( make12(hours(23)).count(), 11LL );
    test_pass("cxx20:make12");

    test_eq( make24(hours(12), false).count(),  0LL );
    test_eq( make24(hours(1),  false).count(),  1LL );
    test_eq( make24(hours(11), false).count(), 11LL );
    test_eq( make24(hours(12), true ).count(), 12LL );
    test_eq( make24(hours(1),  true ).count(), 13LL );
    test_eq( make24(hours(11), true ).count(), 23LL );
    test_pass("cxx20:make24");

    for (int h = 0; h < 24; ++h) {
        hours hh(h);
        test_eq( make24(make12(hh), is_pm(hh)).count(), (long long)h );
    }
    test_pass("cxx20:make12/make24 round-trip");
}

TEST_CASE(chrono, hh_mm_ss_members) {
    using namespace STD::chrono;

    test_eq( (int)hh_mm_ss<seconds>::fractional_width,      0 );
    test_eq( (int)hh_mm_ss<milliseconds>::fractional_width, 3 );
    test_eq( (int)hh_mm_ss<microseconds>::fractional_width, 6 );
    test_eq( (int)hh_mm_ss<nanoseconds>::fractional_width,  9 );
    test_pass("cxx20:hh_mm_ss::fractional_width");

    test_true( (STD::is_same<hh_mm_ss<milliseconds>::precision::period, STD::milli>::value) );
    test_true( (STD::is_same<hh_mm_ss<seconds>::precision::period, STD::ratio<1> >::value) );
    test_pass("cxx20:hh_mm_ss::precision");

    hh_mm_ss<milliseconds> z;
    test_true( !z.is_negative() );
    test_eq( z.hours().count(), 0LL );
    test_eq( z.to_duration().count(), 0LL );
    test_pass("cxx20:hh_mm_ss default ctor");

    milliseconds t = hours(1) + minutes(2) + seconds(3) + milliseconds(456);
    hh_mm_ss<milliseconds> h(t);
    test_true( !h.is_negative() );
    test_eq( h.hours().count(),   1LL );
    test_eq( h.minutes().count(), 2LL );
    test_eq( h.seconds().count(), 3LL );
    test_eq( h.subseconds().count(), 456LL );
    test_eq( h.to_duration().count(), t.count() );

    milliseconds p = static_cast<hh_mm_ss<milliseconds>::precision>(h);
    test_eq( p.count(), t.count() );
    test_pass("cxx20:hh_mm_ss(Duration) accessors");

    hh_mm_ss<milliseconds> n(-t);
    test_true( n.is_negative() );
    test_eq( n.hours().count(),   1LL );
    test_eq( n.minutes().count(), 2LL );
    test_eq( n.seconds().count(), 3LL );
    test_eq( n.subseconds().count(), 456LL );
    test_eq( n.to_duration().count(), (-t).count() );
    test_pass("cxx20:hh_mm_ss::is_negative");

    hh_mm_ss<seconds> big(hours(30) + minutes(15));
    test_eq( big.hours().count(), 30LL );
    test_eq( big.minutes().count(), 15LL );
    test_eq( big.seconds().count(), 0LL );
    test_eq( big.subseconds().count(), 0LL );
    test_pass("cxx20:hh_mm_ss (>24h)");

    hh_mm_ss<seconds> s(seconds(3671));
    test_eq( s.hours().count(),   1LL );
    test_eq( s.minutes().count(), 1LL );
    test_eq( s.seconds().count(), 11LL );
    test_pass("cxx20:hh_mm_ss (seconds)");

    STD::ostringstream os;  os << h;
    test_str_eq( os.str(), "01:02:03.456" );
    STD::ostringstream os2; os2 << n;
    test_str_eq( os2.str(), "-01:02:03.456" );
    STD::ostringstream os3; os3 << s;
    test_str_eq( os3.str(), "01:01:11" );
    test_pass("cxx20:hh_mm_ss operator<<");

#if _TST_E_HAS_FORMAT
    test_str_eq( STD::format("{}", h), "01:02:03.456" );
    test_str_eq( STD::format("{}", n), "-01:02:03.456" );
    test_str_eq( STD::format("{}", s), "01:01:11" );
    test_str_eq( STD::format("{:%H:%M}", h), "01:02" );
    test_pass("cxx20:hh_mm_ss format");
#else
    TEST_NOTE("no <format> in this environment");
    TEST_SKIP_N(4);
    test_skip("cxx20:hh_mm_ss format");
#endif
}

TEST_CASE(chrono, hh_mm_ss_minutes_and_to_duration) {
#if _TST_HAS_CHRONO_CXX20
    using namespace STD::chrono;

    hh_mm_ss<seconds> h(hours(3) + minutes(25) + seconds(45));
    test_eq( (long long)h.minutes().count(), 25LL );
    test_true( tst_type_is<STD::chrono::minutes>(h.minutes()) );
    test_pass("cxx20:hh_mm_ss::minutes");

    test_eq( (long long)h.to_duration().count(), 3LL * 3600 + 25 * 60 + 45 );
    test_true( h.to_duration() == hours(3) + STD::chrono::minutes(25) + seconds(45) );
    hh_mm_ss<seconds> n(-(hours(1) + seconds(2)));
    test_true( n.is_negative() );
    test_eq( (long long)n.to_duration().count(), -3602LL );
    test_pass("cxx20:hh_mm_ss::to_duration");
#else
    TEST_SKIP_N(6);
    test_skip("cxx20:hh_mm_ss::minutes");
    test_skip("cxx20:hh_mm_ss::to_duration");
#endif
}

#else
TEST_CASE_SKIP(chrono, am_pm_helpers)
TEST_CASE_SKIP(chrono, hh_mm_ss_members)
TEST_CASE_SKIP(chrono, hh_mm_ss_minutes_and_to_duration)
#endif  // TEST_TARGET_CXX >= 2020

TEST_CASE(chrono, duration_mixed_period_ops) {
    using namespace STD::chrono;

    typedef STD::common_type<milliseconds, seconds>::type c1;
    test_true( (STD::is_same<c1, milliseconds>::value) );
    typedef STD::common_type<hours, minutes>::type c2;
    test_true( (STD::is_same<c2, minutes>::value) );
    typedef STD::common_type<milliseconds, microseconds>::type c3;
    test_true( (STD::is_same<c3, microseconds>::value) );
    typedef STD::common_type<seconds, seconds>::type c4;
    test_true( (STD::is_same<c4, seconds>::value) );
    typedef duration<long long, STD::ratio<2,3> > two_thirds;
    typedef duration<long long, STD::ratio<3,4> > three_quarters;
    typedef STD::common_type<two_thirds, three_quarters>::type c5;
    test_eq( (long long)c5::period::num, 1LL );
    test_eq( (long long)c5::period::den, 12LL );
    test_pass("cxx11:common_type<duration,duration>");

    typedef time_point<system_clock, seconds>      tp_s;
    typedef time_point<system_clock, milliseconds> tp_ms;
    typedef STD::common_type<tp_s, tp_ms>::type    ctp;
    test_true( (STD::is_same<ctp, tp_ms>::value) );
    test_true( (STD::is_same<STD::common_type<tp_s, tp_s>::type, tp_s>::value) );
    test_pass("cxx11:common_type<time_point,time_point>");

    test_eq( (hours(3) + minutes(25)).count(), 205LL );
    test_eq( (minutes(25) + hours(3)).count(), 205LL );
    test_eq( (seconds(1) - milliseconds(250)).count(), 750LL );
    test_eq( (hours(1) + minutes(2) + seconds(3) + milliseconds(456)).count(), 3723456LL );
    test_pass("cxx11:duration mixed-period + and -");

    test_true( milliseconds(1000) == seconds(1) );
    test_true( milliseconds(999)  <  seconds(1) );
    test_true( seconds(1) > milliseconds(999) );
    test_true( milliseconds(1000) <= seconds(1) );
    test_true( milliseconds(1000) >= seconds(1) );
    test_true( milliseconds(1001) != seconds(1) );
    test_pass("cxx11:duration mixed-period comparison");

    long long q = minutes(2) / seconds(30);
    test_eq( q, 4LL );
    test_eq( (long long)(seconds(90) / seconds(30)), 3LL );
    seconds r = minutes(2) % seconds(35);
    test_eq( r.count(), 15LL );
    test_eq( (seconds(10) / 2).count(), 5LL );
    test_eq( (seconds(10) % 3).count(), 1LL );
    test_pass("cxx11:duration mixed-period / and %");

    tp_s  t0((seconds(60)));
    tp_ms t1 = t0 + milliseconds(500);
    test_eq( t1.time_since_epoch().count(), 60500LL );
    test_eq( (t0 - milliseconds(500)).time_since_epoch().count(), 59500LL );
    test_eq( (milliseconds(500) + t0).time_since_epoch().count(), 60500LL );
    test_eq( (t1 - t0).count(), 500LL );
    test_true( t1 > t0 );
    test_true( tp_ms(milliseconds(60000)) == t0 );
    test_pass("cxx11:time_point mixed-period arithmetic");
}

#if _TST_HAS_CHRONO_CXX20 && !defined(__GNUC__)

namespace {
STD::chrono::sys_seconds ccw_fixed_instant() {
    using namespace STD::chrono;
    year_month_day ymd(year(2024), February, day(29));
    return sys_seconds(sys_days(ymd)) + hours(12) + minutes(34) + seconds(56);
}
}

TEST_CASE(chrono, utc_tai_gps_file_conversions) {
    using namespace STD::chrono;

    sys_seconds inst = ccw_fixed_instant();

    utc_seconds u = utc_clock::from_sys(inst);
    seconds leaps = u.time_since_epoch() - inst.time_since_epoch();
    test_eq( leaps.count(), 27LL );
    test_pass("cxx20:utc_clock::from_sys");

    sys_seconds back = utc_clock::to_sys(u);
    test_true( back == inst );
    test_pass("cxx20:utc_clock::to_sys");

    tai_seconds t = tai_clock::from_utc(u);
    test_eq( (t.time_since_epoch() - u.time_since_epoch()).count(), 378691210LL );
    test_pass("cxx20:tai_clock::from_utc");

    utc_seconds u_back = tai_clock::to_utc(t);
    test_true( u_back == u );
    test_pass("cxx20:tai_clock::to_utc");

    gps_seconds g = gps_clock::from_utc(u);
    test_true( g.time_since_epoch() < t.time_since_epoch() );
    test_pass("cxx20:gps_clock::from_utc");

    utc_seconds ug = gps_clock::to_utc(g);
    test_true( ug == u );
    test_pass("cxx20:gps_clock::to_utc");

    time_point<file_clock, seconds> f = file_clock::from_utc(u);
    test_true( 1 );
    test_pass("cxx20:file_clock::from_utc");

    utc_seconds uf = file_clock::to_utc(f);
    test_true( uf == u );
    test_pass("cxx20:file_clock::to_utc");

    utc_seconds cu = clock_cast<utc_clock>(inst);
    test_true( cu == u );
    tai_seconds ct = clock_cast<tai_clock>(inst);
    test_true( ct == t );
    gps_seconds cg = clock_cast<gps_clock>(inst);
    test_true( cg == g );
    sys_seconds cs = clock_cast<system_clock>(ct);
    test_true( cs == inst );
    sys_seconds cs2 = clock_cast<system_clock>(cg);
    test_true( cs2 == inst );
    time_point<file_clock, seconds> cf = clock_cast<file_clock>(inst);
    test_true( clock_cast<system_clock>(cf) == inst );
    test_true( clock_cast<system_clock>(inst) == inst );
    test_pass("cxx20:clock_cast");

    clock_time_conversion<utc_clock, system_clock> conv;
    test_true( conv(inst) == u );
    clock_time_conversion<system_clock, system_clock> ident;
    test_true( ident(inst) == inst );
    clock_time_conversion<tai_clock, utc_clock> tu;
    test_true( tu(u) == t );
    test_pass("cxx20:clock_time_conversion");

    test_true(( STD::is_same<utc_clock::duration,
                             duration<utc_clock::rep, utc_clock::period> >::value ));
    test_true(( STD::is_same<utc_clock::time_point,
                             time_point<utc_clock, utc_clock::duration> >::value ));
    test_true( utc_clock::is_steady == false );
    test_true(( STD::is_same<tai_clock::duration,
                             duration<tai_clock::rep, tai_clock::period> >::value ));
    test_true(( STD::is_same<tai_clock::time_point,
                             time_point<tai_clock, tai_clock::duration> >::value ));
    test_true(( STD::is_same<gps_clock::duration,
                             duration<gps_clock::rep, gps_clock::period> >::value ));
    test_true(( STD::is_same<gps_clock::time_point,
                             time_point<gps_clock, gps_clock::duration> >::value ));
    test_pass("cxx20:utc/tai/gps clock member types");

    utc_seconds n1 = time_point_cast<seconds>(utc_clock::now());
    tai_seconds n2 = time_point_cast<seconds>(tai_clock::now());
    gps_seconds n3 = time_point_cast<seconds>(gps_clock::now());
    test_true( n1.time_since_epoch().count() > 0 );
    test_true( n2.time_since_epoch().count() > n1.time_since_epoch().count() );
    test_true( n3.time_since_epoch().count() > 0 );
    test_true( n3.time_since_epoch().count() < n1.time_since_epoch().count() );
    test_pass("cxx20:utc/tai/gps clock now()");
}

TEST_CASE(chrono, leap_second_info) {
    using namespace STD::chrono;

    utc_seconds u = clock_cast<utc_clock>(ccw_fixed_instant());
    leap_second_info li = get_leap_second_info(u);
    test_true( li.is_leap_second == false );
    test_eq( li.elapsed.count(), 27LL );
    test_pass("cxx20:get_leap_second_info");

    year_month_day y1971(year(1971), January, day(1));
    utc_seconds early = clock_cast<utc_clock>(sys_seconds(sys_days(y1971)));
    leap_second_info le = get_leap_second_info(early);
    test_true( le.is_leap_second == false );
    test_eq( le.elapsed.count(), 0LL );
    test_pass("cxx20:leap_second_info (pre-1972)");

    year_month_day y2017d(year(2017), January, day(1));
    utc_seconds y2017 = clock_cast<utc_clock>(sys_seconds(sys_days(y2017d)));
    leap_second_info l17 = get_leap_second_info(y2017 - seconds(1));
    test_true( l17.is_leap_second == true );
    test_eq( l17.elapsed.count(), 27LL );
    test_pass("cxx20:leap_second_info (is_leap_second)");
}

#else
TEST_CASE_SKIP(chrono, utc_tai_gps_file_conversions)
TEST_CASE(chrono, leap_second_info) {
    TEST_NOTE("this library has no utc_clock / leap_second_info");
    test_skip("cxx20:get_leap_second_info");
    test_skip("cxx20:leap_second_info (pre-1972)");
    test_skip("cxx20:leap_second_info (is_leap_second)");
}
#endif

TEST_CASE(chrono, hash_value_classes) {
#if _TST_HAS_CHRONO_HASH
    using namespace STD::chrono;

    STD::hash<milliseconds> hdur;
    test_eq( hdur(milliseconds(5)), hdur(milliseconds(5)) );
    test_true( hdur(milliseconds(5)) != hdur(milliseconds(6)) );
    STD::hash<time_point<system_clock, seconds> > htp;
    time_point<system_clock, seconds> tp1((seconds(1000))), tp2((seconds(2000)));
    test_eq( htp(tp1), htp(tp1) );
    test_true( htp(tp1) != htp(tp2) );
    test_pass("cxx26:hash<chrono::duration>");
    test_pass("cxx26:hash<chrono::time_point>");

    STD::hash<day> hd;
    test_eq( hd(day(9)), hd(day(9)) );
    test_true( hd(day(9)) != hd(day(10)) );
    test_pass("cxx26:hash<chrono::day>");

    STD::hash<month> hmo;
    test_eq( hmo(month(3)), hmo(month(3)) );
    test_true( hmo(month(3)) != hmo(month(4)) );
    test_pass("cxx26:hash<chrono::month>");

    STD::hash<year> hy;
    test_eq( hy(year(2026)), hy(year(2026)) );
    test_true( hy(year(2026)) != hy(year(2025)) );
    test_pass("cxx26:hash<chrono::year>");

    STD::hash<weekday> hw;
    test_eq( hw(weekday(3)), hw(weekday(3)) );
    test_true( hw(weekday(3)) != hw(weekday(4)) );
    test_pass("cxx26:hash<chrono::weekday>");

    STD::hash<weekday_indexed> hwi;
    test_eq( hwi(weekday(3)[2]), hwi(weekday(3)[2]) );
    test_true( hwi(weekday(3)[2]) != hwi(weekday(3)[3]) );
    test_true( hwi(weekday(3)[2]) != hwi(weekday(4)[2]) );
    test_pass("cxx26:hash<chrono::weekday_indexed>");

    STD::hash<weekday_last> hwl;
    test_eq( hwl(weekday(3)[last]), hwl(weekday(3)[last]) );
    test_true( hwl(weekday(3)[last]) != hwl(weekday(4)[last]) );
    test_pass("cxx26:hash<chrono::weekday_last>");

    STD::hash<month_day> hmd;
    test_eq( hmd(month_day(month(3), day(9))), hmd(month_day(month(3), day(9))) );
    test_true( hmd(month_day(month(3), day(9))) != hmd(month_day(month(3), day(10))) );
    test_true( hmd(month_day(month(3), day(9))) != hmd(month_day(month(4), day(9))) );
    test_pass("cxx26:hash<chrono::month_day>");

    STD::hash<month_day_last> hmdl;
    test_eq( hmdl(month_day_last(month(3))), hmdl(month_day_last(month(3))) );
    test_true( hmdl(month_day_last(month(3))) != hmdl(month_day_last(month(4))) );
    test_pass("cxx26:hash<chrono::month_day_last>");

    STD::hash<month_weekday> hmw;
    test_eq( hmw(month_weekday(month(3), weekday(2)[1])),
             hmw(month_weekday(month(3), weekday(2)[1])) );
    test_true( hmw(month_weekday(month(3), weekday(2)[1])) !=
               hmw(month_weekday(month(3), weekday(2)[2])) );
    test_pass("cxx26:hash<chrono::month_weekday>");

    STD::hash<month_weekday_last> hmwl;
    test_eq( hmwl(month_weekday_last(month(3), weekday(2)[last])),
             hmwl(month_weekday_last(month(3), weekday(2)[last])) );
    test_true( hmwl(month_weekday_last(month(3), weekday(2)[last])) !=
               hmwl(month_weekday_last(month(4), weekday(2)[last])) );
    test_pass("cxx26:hash<chrono::month_weekday_last>");

    STD::hash<year_month> hym;
    test_eq( hym(year_month(year(2026), month(8))), hym(year_month(year(2026), month(8))) );
    test_true( hym(year_month(year(2026), month(8))) != hym(year_month(year(2026), month(9))) );
    test_true( hym(year_month(year(2026), month(8))) != hym(year_month(year(2025), month(8))) );
    test_pass("cxx26:hash<chrono::year_month>");

    STD::hash<year_month_day> hymd;
    year_month_day d1(year(2026), month(8), day(3));
    year_month_day d2(year(2026), month(8), day(3));
    year_month_day d3(year(2026), month(8), day(4));
    test_eq( hymd(d1), hymd(d2) );
    test_true( hymd(d1) != hymd(d3) );
    test_true( hymd(d1) != hymd(year_month_day(year(2026), month(9), day(3))) );
    test_pass("cxx26:hash<chrono::year_month_day>");

    STD::hash<year_month_day_last> hymdl;
    year_month_day_last l1(year(2026), month_day_last(month(8)));
    test_eq( hymdl(l1), hymdl(year_month_day_last(year(2026), month_day_last(month(8)))) );
    test_true( hymdl(l1) != hymdl(year_month_day_last(year(2026), month_day_last(month(9)))) );
    test_pass("cxx26:hash<chrono::year_month_day_last>");

    STD::hash<year_month_weekday> hymw;
    year_month_weekday w1(year(2026), month(8), weekday(2)[1]);
    test_eq( hymw(w1), hymw(year_month_weekday(year(2026), month(8), weekday(2)[1])) );
    test_true( hymw(w1) != hymw(year_month_weekday(year(2026), month(8), weekday(2)[2])) );
    test_pass("cxx26:hash<chrono::year_month_weekday>");

    STD::hash<year_month_weekday_last> hymwl;
    year_month_weekday_last k1(year(2026), month(8), weekday(2)[last]);
    test_eq( hymwl(k1), hymwl(year_month_weekday_last(year(2026), month(8), weekday(2)[last])) );
    test_true( hymwl(k1) != hymwl(year_month_weekday_last(year(2026), month(9), weekday(2)[last])) );
    test_pass("cxx26:hash<chrono::year_month_weekday_last>");

    TEST_NOTE("zoned_time is not implemented (no tzdb)");
    test_skip("cxx26:hash<chrono::zoned_time>");
#else
    TEST_SKIP_N(38);
    test_skip("cxx26:hash<chrono::duration>");
    test_skip("cxx26:hash<chrono::time_point>");
    test_skip("cxx26:hash<chrono::day>");
    test_skip("cxx26:hash<chrono::month>");
    test_skip("cxx26:hash<chrono::year>");
    test_skip("cxx26:hash<chrono::weekday>");
    test_skip("cxx26:hash<chrono::weekday_indexed>");
    test_skip("cxx26:hash<chrono::weekday_last>");
    test_skip("cxx26:hash<chrono::month_day>");
    test_skip("cxx26:hash<chrono::month_day_last>");
    test_skip("cxx26:hash<chrono::month_weekday>");
    test_skip("cxx26:hash<chrono::month_weekday_last>");
    test_skip("cxx26:hash<chrono::year_month>");
    test_skip("cxx26:hash<chrono::year_month_day>");
    test_skip("cxx26:hash<chrono::year_month_day_last>");
    test_skip("cxx26:hash<chrono::year_month_weekday>");
    test_skip("cxx26:hash<chrono::year_month_weekday_last>");
    test_skip("cxx26:hash<chrono::zoned_time>");
#endif
}

#else
TEST_CASE_SKIP(chrono, am_pm_helpers)
TEST_CASE_SKIP(chrono, hh_mm_ss_members)
TEST_CASE_SKIP(chrono, hh_mm_ss_minutes_and_to_duration)
TEST_CASE_SKIP(chrono, duration_mixed_period_ops)
TEST_CASE_SKIP(chrono, utc_tai_gps_file_conversions)
TEST_CASE_SKIP(chrono, leap_second_info)
TEST_CASE_SKIP(chrono, hash_value_classes)
#endif  // TEST_TARGET_CXX >= 2011
