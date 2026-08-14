#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2017
#include <chrono>
#include <sstream>
#include <string>

using _ccw::string_view;

#if defined(__WATCOMC__) || _tst_cplusplus < 201103L || (defined(_MSC_VER) && _MSC_VER < 1600)
#  define _TST_F_LLIBCXX03 1
#else
#  define _TST_F_LLIBCXX03 0
#endif

namespace {

template <class _T>
STD::string ccw_f_str(const _T& v) {
    using namespace STD::chrono;
    STD::ostringstream os;
    os << v;
    return os.str();
}

}

TEST_CASE(chrono, time_point_floor_ceil_round) {
    using namespace STD::chrono;
    typedef time_point<system_clock, milliseconds> tp_ms;

    test_eq( floor<seconds>(tp_ms(milliseconds(1500))).time_since_epoch().count(), 1LL );
    test_eq( ceil <seconds>(tp_ms(milliseconds(1500))).time_since_epoch().count(), 2LL );
    test_eq( floor<seconds>(tp_ms(milliseconds(2000))).time_since_epoch().count(), 2LL );
    test_eq( ceil <seconds>(tp_ms(milliseconds(2000))).time_since_epoch().count(), 2LL );
    test_pass("cxx17:constexpr floor(time_point)");
    test_pass("cxx17:constexpr ceil(time_point)");

    test_eq( floor<seconds>(tp_ms(milliseconds(-1500))).time_since_epoch().count(), -2LL );
    test_eq( ceil <seconds>(tp_ms(milliseconds(-1500))).time_since_epoch().count(), -1LL );
    test_pass("cxx17:constexpr floor(time_point) (negative)");
    test_pass("cxx17:constexpr ceil(time_point) (negative)");

    test_eq( round<seconds>(tp_ms(milliseconds(1400))).time_since_epoch().count(), 1LL );
    test_eq( round<seconds>(tp_ms(milliseconds(1600))).time_since_epoch().count(), 2LL );
    test_eq( round<seconds>(tp_ms(milliseconds(2500))).time_since_epoch().count(), 2LL );
    test_eq( round<seconds>(tp_ms(milliseconds(3500))).time_since_epoch().count(), 4LL );
    test_eq( round<seconds>(tp_ms(milliseconds(-1500))).time_since_epoch().count(), -2LL );
    test_pass("cxx17:constexpr round(time_point)");

    time_point<steady_clock, milliseconds> st(milliseconds(1500));
    test_eq( floor<seconds>(st).time_since_epoch().count(), 1LL );
    test_eq( floor<milliseconds>(time_point<system_clock, seconds>(seconds(2))).time_since_epoch().count(), 2000LL );
    test_pass("cxx17:constexpr floor(time_point) (keeps clock)");
}

#if TEST_TARGET_CXX >= 2020 && _TST_HAS_CHRONO_CXX20

TEST_CASE(chrono, duration_stream_suffixes) {
    using namespace STD::chrono;

    test_str_eq( ccw_f_str(nanoseconds(11)),  "11ns" );
    test_str_eq( ccw_f_str(milliseconds(7)),  "7ms" );
    test_str_eq( ccw_f_str(seconds(3)),       "3s" );
    test_str_eq( ccw_f_str(minutes(25)),      "25min" );
    test_str_eq( ccw_f_str(hours(2)),         "2h" );
    test_str_eq( ccw_f_str(days(5)),          "5d" );
    test_pass("cxx20:operator<<(duration) common units");

    STD::string us = ccw_f_str(microseconds(9));
    test_true( us == "9us" || us == "9\xC2\xB5s" );
    test_pass("cxx20:operator<<(duration) micro");

    test_str_eq( ccw_f_str(duration<int, STD::atto>(4)),  "4as" );
    test_str_eq( ccw_f_str(duration<int, STD::femto>(4)), "4fs" );
    test_str_eq( ccw_f_str(duration<int, STD::pico>(4)),  "4ps" );
    test_str_eq( ccw_f_str(duration<int, STD::centi>(4)), "4cs" );
    test_str_eq( ccw_f_str(duration<int, STD::deci>(4)),  "4ds" );
    test_pass("cxx20:operator<<(duration) sub-second units");

    test_str_eq( ccw_f_str(duration<int, STD::deca>(4)),  "4das" );
    test_str_eq( ccw_f_str(duration<int, STD::hecto>(4)), "4hs" );
    test_str_eq( ccw_f_str(duration<int, STD::kilo>(4)),  "4ks" );
    test_str_eq( ccw_f_str(duration<int, STD::mega>(4)),  "4Ms" );
    test_str_eq( ccw_f_str(duration<int, STD::giga>(4)),  "4Gs" );
    test_str_eq( ccw_f_str(duration<int, STD::tera>(4)),  "4Ts" );
    test_str_eq( ccw_f_str(duration<int, STD::peta>(4)),  "4Ps" );
    test_str_eq( ccw_f_str(duration<int, STD::exa>(4)),   "4Es" );
    test_pass("cxx20:operator<<(duration) super-second units");

    test_str_eq( ccw_f_str(weeks(2)),                        "2[604800]s" );
    test_str_eq( ccw_f_str(months(1)),                       "1[2629746]s" );
    test_str_eq( ccw_f_str(years(1)),                        "1[31556952]s" );
    test_str_eq( ccw_f_str(duration<int, STD::ratio<5, 1> >(4)), "4[5]s" );
    test_str_eq( ccw_f_str(duration<int, STD::ratio<3, 7> >(4)), "4[3/7]s" );
    test_pass("cxx20:operator<<(duration) unnamed period");
}

TEST_CASE(chrono, duration_stream_values) {
    using namespace STD::chrono;

    test_str_eq( ccw_f_str(seconds(0)),  "0s" );
    test_str_eq( ccw_f_str(seconds(-7)), "-7s" );
    test_str_eq( ccw_f_str(hours(-1)),   "-1h" );
    test_pass("cxx20:operator<<(duration) sign");

    test_str_eq( ccw_f_str(duration<double, STD::ratio<1> >(1.5)), "1.5s" );
    test_pass("cxx20:operator<<(duration) floating rep");

    STD::ostringstream os;
    test_true( &(os << seconds(1)) == &os );
    test_pass("cxx20:operator<<(duration) returns the stream");
}

TEST_CASE(chrono, time_point_stream) {
    using namespace STD::chrono;

    test_str_eq( ccw_f_str(sys_days(days(0))),  "1970-01-01" );
    test_str_eq( ccw_f_str(sys_days(days(-1))), "1969-12-31" );
    test_pass("cxx20:operator<<(sys_days)");

    test_str_eq( ccw_f_str(sys_seconds(seconds(90061))), "1970-01-02 01:01:01" );
    test_str_eq( ccw_f_str(sys_seconds(seconds(-1))),    "1969-12-31 23:59:59" );
    test_pass("cxx20:operator<<(sys_time)");

    typedef time_point<system_clock, milliseconds> tp_ms;
    typedef time_point<system_clock, microseconds> tp_us;
    test_str_eq( ccw_f_str(tp_ms(milliseconds(1500))),  "1970-01-01 00:00:01.500" );
    test_str_eq( ccw_f_str(tp_ms(milliseconds(-1500))), "1969-12-31 23:59:58.500" );
    test_str_eq( ccw_f_str(tp_us(microseconds(1))),     "1970-01-01 00:00:00.000001" );
    test_pass("cxx20:operator<<(sys_time) subseconds");

    test_str_eq( ccw_f_str(local_days(days(0))),         "1970-01-01" );
    test_str_eq( ccw_f_str(local_seconds(seconds(3661))), "1970-01-01 01:01:01" );
    test_pass("cxx20:operator<<(local_time)");

    test_str_eq( ccw_f_str(tai_seconds(seconds(0))),     "1958-01-01 00:00:00" );
    test_str_eq( ccw_f_str(tai_seconds(seconds(86400))), "1958-01-02 00:00:00" );
    test_str_eq( ccw_f_str(gps_seconds(seconds(0))),     "1980-01-06 00:00:00" );
    test_str_eq( ccw_f_str(gps_seconds(seconds(-1))),    "1980-01-05 23:59:59" );
    test_pass("cxx20:operator<<(tai_time/gps_time)");

    sys_seconds inst(seconds(981173106));
    time_point<file_clock, seconds> f
        = time_point_cast<seconds>(clock_cast<file_clock>(inst));
    test_str_eq( ccw_f_str(f), ccw_f_str(inst) );
    test_str_eq( ccw_f_str(inst), "2001-02-03 04:05:06" );
    test_pass("cxx20:operator<<(file_time)");

    STD::ostringstream os;
    test_true( &(os << sys_days(days(0))) == &os );
    test_pass("cxx20:operator<<(time_point) returns the stream");
}

TEST_CASE(chrono, utc_stream_leap_second) {
    using namespace STD::chrono;

    sys_seconds ins(seconds(78796800));
    utc_seconds u = utc_clock::from_sys(ins);

    test_str_eq( ccw_f_str(ins), "1972-07-01 00:00:00" );
    test_str_eq( ccw_f_str(u),   "1972-07-01 00:00:00" );
    test_pass("cxx20:operator<<(utc_time)");

    utc_seconds leap = u - seconds(1);
    test_true( get_leap_second_info(leap).is_leap_second );
    test_str_eq( ccw_f_str(leap), "1972-06-30 23:59:60" );
    test_pass("cxx20:operator<<(utc_time) leap second");

    utc_seconds before = u - seconds(2);
    test_true( !get_leap_second_info(before).is_leap_second );
    test_str_eq( ccw_f_str(before), "1972-06-30 23:59:59" );
    test_pass("cxx20:operator<<(utc_time) second before the leap");
}

TEST_CASE(chrono, duration_from_stream) {
    using namespace STD::chrono;

    {
        milliseconds d(0);
        STD::istringstream is("01:02:03.456");
        test_true( &from_stream(is, "%T", d) == &is );
        test_true( !is.fail() );
        test_eq( d.count(), 3723456LL );
    }
    test_pass("cxx20:from_stream(duration) %T");

    {
        minutes d(0);
        STD::istringstream is("12:34");
        from_stream(is, "%R", d);
        test_true( !is.fail() );
        test_eq( d.count(), 754LL );
    }
    test_pass("cxx20:from_stream(duration) %R");

    {
        seconds d(0);
        STD::istringstream is("100");
        from_stream(is, "%j", d);
        test_true( !is.fail() );
        test_eq( d.count(), 8640000LL );
    }
    test_pass("cxx20:from_stream(duration) %j");

    {
        seconds d(0);
        STD::istringstream is("3600");
        from_stream(is, "%S", d);
        test_true( !is.fail() );
        test_eq( d.count(), 36LL );
    }
    test_pass("cxx20:from_stream(duration) %S field width");

    {
        seconds d(0);
        STD::istringstream is("00:00:01.750");
        from_stream(is, "%T", d);
        test_true( !is.fail() );
        test_eq( d.count(), 1LL );
    }
    test_pass("cxx20:from_stream(duration) period conversion");

    {
        seconds d(99);
        STD::istringstream is("nonsense");
        from_stream(is, "%T", d);
        test_true( is.fail() );
        test_eq( d.count(), 99LL );
    }
    test_pass("cxx20:from_stream(duration) failure");
}

TEST_CASE(chrono, time_point_from_stream_clocks) {
    using namespace STD::chrono;
    const long long fixed = 981173106LL;

    {
        sys_seconds tp;
        STD::istringstream is("2001-02-03 04:05:06");
        test_true( &from_stream(is, "%F %T", tp) == &is );
        test_true( !is.fail() );
        test_eq( tp.time_since_epoch().count(), fixed );
        test_str_eq( ccw_f_str(tp), "2001-02-03 04:05:06" );
    }
    test_pass("cxx20:from_stream(sys_time) %F %T");

    {
        sys_seconds tp;
        STD::istringstream is("Feb 3 2001");
        from_stream(is, "%b %d %Y", tp);
        test_true( !is.fail() );
        test_eq( tp.time_since_epoch().count(), 981158400LL );
    }
    test_pass("cxx20:from_stream(sys_time) month name");

    {
        sys_seconds tp;
        STD::istringstream is("03/04/05");
        from_stream(is, "%D", tp);
        test_true( !is.fail() );
        test_eq( tp.time_since_epoch().count(), 1109894400LL );
    }
    test_pass("cxx20:from_stream(sys_time) %D");

    {
        sys_seconds tp;
        STD::istringstream is("2001-02-03 07:05:06 PM");
        from_stream(is, "%F %I:%M:%S %p", tp);
        test_true( !is.fail() );
        test_eq( tp.time_since_epoch().count(), 981227106LL );
    }
    test_pass("cxx20:from_stream(sys_time) %I %p");

    {
        sys_seconds tp;
        STD::istringstream is("2001-02-03 04:05:06 +0900");
        from_stream(is, "%F %T %z", tp);
        test_true( !is.fail() );
        test_eq( tp.time_since_epoch().count(), fixed - 32400LL );
    }
    test_pass("cxx20:from_stream(sys_time) %z");

    {
        sys_seconds tp;
        STD::string  abbrev("unset");
        minutes      offset(0);
        STD::istringstream is("2001-02-03 04:05:06 JST +0900");
        from_stream(is, "%F %T %Z %z", tp, &abbrev, &offset);
        test_true( !is.fail() );
        test_str_eq( abbrev, "JST" );
        test_eq( offset.count(), 540LL );
        test_eq( tp.time_since_epoch().count(), fixed - 32400LL );
    }
    test_pass("cxx20:from_stream(sys_time) %Z and the offset argument");

    {
        time_point<system_clock, milliseconds> tp;
        STD::istringstream is("2001-02-03 04:05:06.250");
        from_stream(is, "%F %T", tp);
        test_true( !is.fail() );
        test_eq( tp.time_since_epoch().count(), fixed * 1000LL + 250LL );
    }
    test_pass("cxx20:from_stream(sys_time) subseconds");

    {
        local_seconds lt;
        STD::istringstream is("2001-02-03 04:05:06");
        from_stream(is, "%F %T", lt);
        test_true( !is.fail() );
        test_eq( lt.time_since_epoch().count(), fixed );
    }
    {
        tai_seconds tt;
        STD::istringstream is("1958-01-02 00:00:00");
        from_stream(is, "%F %T", tt);
        test_true( !is.fail() );
        test_eq( tt.time_since_epoch().count(), 86400LL );
        test_str_eq( ccw_f_str(tt), "1958-01-02 00:00:00" );
    }
    {
        gps_seconds gt;
        STD::istringstream is("1980-01-06 00:00:00");
        from_stream(is, "%F %T", gt);
        test_true( !is.fail() );
        test_eq( gt.time_since_epoch().count(), 0LL );
    }
    {
        utc_seconds ut;
        STD::istringstream is("2001-02-03 04:05:06");
        from_stream(is, "%F %T", ut);
        test_true( !is.fail() );
        test_str_eq( ccw_f_str(ut), "2001-02-03 04:05:06" );
    }
    {
        time_point<file_clock, seconds> ft;
        STD::istringstream is("2001-02-03 04:05:06");
        from_stream(is, "%F %T", ft);
        test_true( !is.fail() );
        test_str_eq( ccw_f_str(ft), "2001-02-03 04:05:06" );
    }
    test_pass("cxx20:from_stream(local/tai/gps/utc/file time) round-trip");
}

TEST_CASE(chrono, from_stream_failures) {
    using namespace STD::chrono;

    {
        sys_seconds tp(seconds(42));
        STD::istringstream is("2001-02-30");
        from_stream(is, "%F", tp);
        test_true( is.fail() );
        test_eq( tp.time_since_epoch().count(), 42LL );
    }
    test_pass("cxx20:from_stream(istream&, const charT*, Parsable&) (rejects an impossible date)");

    {
        sys_seconds tp;
        STD::istringstream is("2001");
        from_stream(is, "%Y", tp);
        test_true( is.fail() );
    }
    test_pass("cxx20:from_stream(istream&, const charT*, Parsable&) (rejects an incomplete date)");

    {
        sys_seconds tp;
        STD::istringstream is("2001/02/03");
        from_stream(is, "%F", tp);
        test_true( is.fail() );
    }
    test_pass("cxx20:from_stream(istream&, const charT*, Parsable&) (rejects a literal mismatch)");

    {
        seconds d(0);
        STD::istringstream is("");
        from_stream(is, "%T", d);
        test_true( is.fail() );
    }
    test_pass("cxx20:from_stream(istream&, const charT*, Parsable&) (rejects empty input)");
}

#if _TST_F_LLIBCXX03
TEST_CASE(chrono, duration_from_stream_unit_extension) {
    using namespace STD::chrono;

    {
        seconds d(0);
        STD::istringstream is("42");
        from_stream(is, "%Q", d);
        test_true( !is.fail() );
        test_eq( d.count(), 42LL );
    }
    {
        seconds d(0);
        STD::istringstream is("1000ms");
        from_stream(is, "%Q%q", d);
        test_true( !is.fail() );
        test_eq( d.count(), 1LL );
    }
    {
        milliseconds d(0);
        STD::istringstream is("2s");
        from_stream(is, "%Q%q", d);
        test_true( !is.fail() );
        test_eq( d.count(), 2000LL );
    }
    {
        seconds d(0);
        STD::istringstream is("3min");
        from_stream(is, "%Q%q", d);
        test_true( !is.fail() );
        test_eq( d.count(), 180LL );
    }
    {
        STD::ostringstream os;
        os << hours(7);
        STD::istringstream is(os.str());
        hours d(0);
        from_stream(is, "%Q%q", d);
        test_true( !is.fail() );
        test_eq( d.count(), 7LL );
    }
    {
        seconds d(5);
        STD::istringstream is("12zz");
        from_stream(is, "%Q%q", d);
        test_true( is.fail() );
        test_eq( d.count(), 5LL );
    }
    test_pass("ccwrap:from_stream(duration) %Q and %q");
}
#else
TEST_CASE_SKIP(chrono, duration_from_stream_unit_extension)
#endif

#else   // TEST_TARGET_CXX >= 2020 && _TST_HAS_CHRONO_CXX20

TEST_CASE_SKIP(chrono, duration_stream_suffixes)
TEST_CASE_SKIP(chrono, duration_stream_values)
TEST_CASE_SKIP(chrono, time_point_stream)
TEST_CASE_SKIP(chrono, utc_stream_leap_second)
TEST_CASE_SKIP(chrono, duration_from_stream)
TEST_CASE_SKIP(chrono, time_point_from_stream_clocks)
TEST_CASE_SKIP(chrono, from_stream_failures)
TEST_CASE_SKIP(chrono, duration_from_stream_unit_extension)

#endif

#else   // TEST_TARGET_CXX >= 2017

TEST_CASE_SKIP(chrono, time_point_floor_ceil_round)
TEST_CASE_SKIP(chrono, duration_stream_suffixes)
TEST_CASE_SKIP(chrono, duration_stream_values)
TEST_CASE_SKIP(chrono, time_point_stream)
TEST_CASE_SKIP(chrono, utc_stream_leap_second)
TEST_CASE_SKIP(chrono, duration_from_stream)
TEST_CASE_SKIP(chrono, time_point_from_stream_clocks)
TEST_CASE_SKIP(chrono, from_stream_failures)
TEST_CASE_SKIP(chrono, duration_from_stream_unit_extension)

#endif
