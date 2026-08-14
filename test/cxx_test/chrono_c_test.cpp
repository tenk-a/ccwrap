#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020
#include <chrono>
#include <compare>
#include <sstream>
#include <string>
#include <type_traits>

using _ccw::string_view;

TEST_CASE(chrono, cal_day) {
    using namespace STD::chrono;

    day d0;
    d0 = day(1);
    day d(29);
    test_true( 1 );
    test_pass("cxx20:day ctor");
    test_eq( (unsigned)d, 29u );
    test_pass("cxx20:day operator unsigned");

    day e(5);
    ++e;                     test_eq( (unsigned)e, 6u );
    day p = e++;             test_eq( (unsigned)p, 6u );
                             test_eq( (unsigned)e, 7u );
    --e;                     test_eq( (unsigned)e, 6u );
    day q = e--;             test_eq( (unsigned)q, 6u );
                             test_eq( (unsigned)e, 5u );
    e += days(10);           test_eq( (unsigned)e, 15u );
    e -= days(4);            test_eq( (unsigned)e, 11u );
    test_pass("cxx20:day arithmetic members");

    test_true( !day(0).ok() );
    test_true(  day(1).ok() );
    test_true(  day(31).ok() );
    test_true( !day(32).ok() );
    test_true( !day(255).ok() );
    test_pass("cxx20:day::ok");
}

TEST_CASE(chrono, cal_month) {
    using namespace STD::chrono;

    month m0;
    m0 = month(1);
    month m(2);
    test_true( 1 );
    test_pass("cxx20:month ctor");
    test_eq( (unsigned)m, 2u );
    test_pass("cxx20:month operator unsigned");

    month c(11);
    ++c;                     test_eq( (unsigned)c, 12u );
    ++c;                     test_eq( (unsigned)c, 1u );
    month p = c++;           test_eq( (unsigned)p, 1u );
                             test_eq( (unsigned)c, 2u );
    --c;                     test_eq( (unsigned)c, 1u );
    month q = c--;           test_eq( (unsigned)q, 1u );
                             test_eq( (unsigned)c, 12u );
    c += months(2);          test_eq( (unsigned)c, 2u );
    c -= months(3);          test_eq( (unsigned)c, 11u );
    test_pass("cxx20:month arithmetic members");

    test_true( !month(0).ok() );
    test_true(  month(1).ok() );
    test_true(  month(12).ok() );
    test_true( !month(13).ok() );
    test_pass("cxx20:month::ok");
}

TEST_CASE(chrono, cal_year) {
    using namespace STD::chrono;

    year y0;
    y0 = year(1);
    year y(2024);
    test_true( 1 );
    test_pass("cxx20:year ctor");
    test_eq( (int)y, 2024 );
    test_pass("cxx20:year operator int");

    year c(2000);
    ++c;                     test_eq( (int)c, 2001 );
    year p = c++;            test_eq( (int)p, 2001 );
                             test_eq( (int)c, 2002 );
    --c;                     test_eq( (int)c, 2001 );
    year q = c--;            test_eq( (int)q, 2001 );
                             test_eq( (int)c, 2000 );
    c += years(24);          test_eq( (int)c, 2024 );
    c -= years(4);           test_eq( (int)c, 2020 );
    test_pass("cxx20:year arithmetic members");

    test_eq( (int)(+year(2024)), 2024 );
    test_pass("cxx20:year unary +");

    test_eq( (int)(-year(2024)), -2024 );
    test_pass("cxx20:year unary -");

    test_true(  year(2024).is_leap() );
    test_true( !year(2023).is_leap() );
    test_true(  year(2000).is_leap() );
    test_true( !year(1900).is_leap() );
    test_true(  year(1996).is_leap() );
    test_pass("cxx20:year::is_leap");

    test_true( year(2024).ok() );
    test_true( !year(-32768).ok() );
    test_pass("cxx20:year::ok");

    test_true( year::min().ok() );
    test_eq( (int)year::min(), -32767 );
    test_pass("cxx20:year::min");

    test_true( year::max().ok() );
    test_eq( (int)year::max(),  32767 );
    test_pass("cxx20:year::max");
}

TEST_CASE(chrono, cal_weekday) {
    using namespace STD::chrono;

    weekday w0;
    w0 = weekday(0);
    weekday w(4);
    test_true( w == Thursday );
    test_pass("cxx20:weekday ctor");

    weekday ws(sys_days(year(2024) / February / day(29)));
    test_true( ws == Thursday );
    weekday ws2(sys_days(year(2000) / January / day(1)));
    test_true( ws2 == Saturday );
    test_pass("cxx20:weekday from sys_days");

    weekday wl(local_days(sys_days(year(2024) / February / day(29)).time_since_epoch()));
    test_true( wl == Thursday );
    test_pass("cxx20:weekday from local_days");

    weekday c(Saturday);
    ++c;                     test_true( c == Sunday );
    weekday p = c++;         test_true( p == Sunday );
                             test_true( c == Monday );
    --c;                     test_true( c == Sunday );
    weekday q = c--;         test_true( q == Sunday );
                             test_true( c == Saturday );
    c += days(2);            test_true( c == Monday );
    c -= days(3);            test_true( c == Friday );
    test_pass("cxx20:weekday arithmetic members");

    test_eq( Sunday.c_encoding(),   0u );
    test_eq( Thursday.c_encoding(), 4u );
    test_eq( Saturday.c_encoding(), 6u );
    test_pass("cxx20:weekday::c_encoding");

    test_eq( Monday.iso_encoding(), 1u );
    test_eq( Sunday.iso_encoding(), 7u );
    test_eq( Saturday.iso_encoding(), 6u );

    test_true( weekday(7) == Sunday );
    test_pass("cxx20:weekday::iso_encoding");

    test_true(  Sunday.ok() );
    test_true(  weekday(6).ok() );
    test_true(  weekday(7).ok() );
    test_true( !weekday(8).ok() );
    test_pass("cxx20:weekday::ok");

    weekday_indexed wi = Thursday[2];
    test_true( wi.weekday() == Thursday );
    test_eq( wi.index(), 2u );
    weekday_last wlst = Thursday[last];
    test_true( wlst.weekday() == Thursday );
    test_pass("cxx20:weekday::operator[]");
}

TEST_CASE(chrono, cal_weekday_indexed_and_last) {
    using namespace STD::chrono;

    weekday_indexed i0;
    i0 = weekday_indexed(Monday, 1);
    weekday_indexed wi(Thursday, 3);
    test_true( wi.weekday() == Thursday );
    test_eq( wi.index(), 3u );
    test_true( wi.ok() );
    test_true( !weekday_indexed(Thursday, 0).ok() );
    test_true(  weekday_indexed(Thursday, 5).ok() );
    test_true( !weekday_indexed(Thursday, 6).ok() );
    test_true( !weekday_indexed(weekday(9), 1).ok() );
    test_pass("cxx20:weekday_indexed");

    test_true( wi == Thursday[3] );
    test_true( wi != Thursday[2] );
    test_true( wi != Friday[3] );
    test_pass("cxx20:weekday_indexed operator==");

    weekday_last wl(Thursday);
    test_true( wl.weekday() == Thursday );
    test_true( wl.ok() );
    test_true( !weekday_last(weekday(9)).ok() );

    test_true( wl == Thursday[last] );
    test_true( wl != Friday[last] );
    test_pass("cxx20:weekday_last");

    #if _TST_HAS_DECLTYPE
    test_true( (STD::is_same<const last_spec, decltype(last)>::value) );
    test_pass("cxx20:last_spec type");
    #else
    TEST_SKIP1();
    test_skip("cxx20:last_spec type");
    #endif
}

TEST_CASE(chrono, cal_month_day_and_last) {
    using namespace STD::chrono;

    month_day md0;
    md0 = month_day(January, day(1));
    month_day md(February, day(29));
    test_true( md.month() == February );
    test_true( md.day() == day(29) );
    test_true( md.ok() );

    test_true(  month_day(February, day(29)).ok() );
    test_true( !month_day(February, day(30)).ok() );
    test_true(  month_day(April, day(30)).ok() );
    test_true( !month_day(April, day(31)).ok() );
    test_true(  month_day(December, day(31)).ok() );
    test_true( !month_day(month(0), day(1)).ok() );
    test_true( !month_day(January, day(0)).ok() );
    test_pass("cxx20:month_day");

    month_day_last mdl(February);
    test_true( mdl.month() == February );
    test_true( mdl.ok() );
    test_true( !month_day_last(month(13)).ok() );
    test_true( mdl != month_day_last(March) );
    test_true( mdl < month_day_last(March) );
    test_pass("cxx20:month_day_last");

    #if !defined(__WATCOMC__)

    test_true( mdl == month_day_last(February) );
    test_pass("cxx20:month_day_last operator==");
    #else
    TEST_SKIP1();
    test_skip("cxx20:month_day_last operator==");
    #endif
    #if defined(__cpp_impl_three_way_comparison)
    test_true( (month_day_last(March) <=> mdl) == STD::strong_ordering::greater );
    test_pass("cxx20:month_day_last operator<=>");
    #else
    TEST_SKIP1();
    test_skip("cxx20:month_day_last operator<=>");
    #endif
}

TEST_CASE(chrono, cal_month_weekday_and_last) {
    using namespace STD::chrono;

    month_weekday mw(February, Thursday[3]);
    test_true( mw.month() == February );
    test_true( mw.weekday_indexed() == Thursday[3] );
    test_true( mw.ok() );
    test_true( !month_weekday(month(0), Thursday[3]).ok() );
    test_true( !month_weekday(February, Thursday[0]).ok() );

    test_true( mw == month_weekday(February, Thursday[3]) );
    test_true( mw != month_weekday(February, Thursday[2]) );
    test_true( mw != month_weekday(March, Thursday[3]) );
    test_pass("cxx20:month_weekday");

    month_weekday_last mwl(February, Thursday[last]);
    test_true( mwl.month() == February );
    test_true( mwl.weekday_last() == Thursday[last] );
    test_true( mwl.ok() );
    test_true( !month_weekday_last(month(13), Thursday[last]).ok() );

    test_true( mwl == month_weekday_last(February, Thursday[last]) );
    test_true( mwl != month_weekday_last(February, Friday[last]) );
    test_true( mwl != month_weekday_last(March, Thursday[last]) );
    test_pass("cxx20:month_weekday_last");
}

TEST_CASE(chrono, cal_year_month) {
    using namespace STD::chrono;

    year_month ym0;
    ym0 = year_month(year(2000), January);
    year_month ym(year(2024), February);
    test_true( ym.year() == year(2024) );
    test_true( ym.month() == February );
    test_true( ym.ok() );
    test_true( !year_month(year(2024), month(0)).ok() );
    test_true( !year_month(year(-32768), February).ok() );
    test_pass("cxx20:year_month");

    year_month c(year(2024), November);
    c += months(3);
    test_true( c == year_month(year(2025), February) );
    c -= months(14);
    test_true( c == year_month(year(2023), December) );
    test_pass("cxx20:year_month arithmetic members");
}

TEST_CASE(chrono, cal_year_month_day) {
    using namespace STD::chrono;

    year_month_day ymd0;
    ymd0 = year_month_day(year(2000), January, day(1));
    year_month_day ymd(year(2024), February, day(29));
    test_true( ymd.year() == year(2024) );
    test_true( ymd.month() == February );
    test_true( ymd.day() == day(29) );
    test_true( ymd.ok() );
    test_pass("cxx20:year_month_day");

    year_month_day from_last(year(2024) / February / last);
    test_true( from_last == ymd );
    year_month_day from_last23(year(2023) / February / last);
    test_true( from_last23.day() == day(28) );
    test_pass("cxx20:year_month_day from ymd_last");

    sys_days sd = sys_days(ymd);
    year_month_day round(sd);
    test_true( round == ymd );
    test_eq( sd.time_since_epoch().count(), 19782LL );
    test_true( sys_days(year(1970) / January / day(1)).time_since_epoch() == days(0) );
    test_true( year_month_day(sys_days(days(0))) == year_month_day(year(1970), January, day(1)) );
    test_true( sys_days(year(1969) / December / day(31)).time_since_epoch() == days(-1) );
    test_pass("cxx20:year_month_day sys_days conversion");

    local_days ld = local_days(ymd);
    test_eq( ld.time_since_epoch().count(), 19782LL );
    year_month_day from_local(ld);
    test_true( from_local == ymd );
    test_pass("cxx20:year_month_day local_days conversion");

    year_month_day c(year(2024), January, day(31));
    c += months(1);
    test_true( c == year_month_day(year(2024), February, day(31)) );
    test_true( !c.ok() );
    c -= months(1);
    test_true( c == year_month_day(year(2024), January, day(31)) );
    test_true( c.ok() );
    test_pass("cxx20:year_month_day arithmetic members");

    test_true(  year_month_day(year(2024), February, day(29)).ok() );
    test_true( !year_month_day(year(2023), February, day(29)).ok() );
    test_true( !year_month_day(year(2024), month(13), day(1)).ok() );
    test_true( !year_month_day(year(2024), April, day(31)).ok() );
    test_pass("cxx20:year_month_day::ok");
}

TEST_CASE(chrono, cal_year_month_day_last) {
    using namespace STD::chrono;

    year_month_day_last ymdl(year(2024), month_day_last(February));
    test_true( ymdl.year() == year(2024) );
    test_true( ymdl.month() == February );
    test_true( ymdl.month_day_last() == month_day_last(February) );
    test_true( ymdl.day() == day(29) );
    test_true( ymdl.ok() );
    test_true( year_month_day_last(year(2023), month_day_last(February)).day() == day(28) );
    test_true( year_month_day_last(year(2024), month_day_last(April)).day() == day(30) );
    test_true( !year_month_day_last(year(2024), month_day_last(month(0))).ok() );
    test_pass("cxx20:year_month_day_last");

    sys_days sd = sys_days(ymdl);
    test_true( year_month_day(sd) == year_month_day(year(2024), February, day(29)) );
    local_days ld = local_days(ymdl);
    test_eq( ld.time_since_epoch().count(), sd.time_since_epoch().count() );
    test_pass("cxx20:year_month_day_last conversions");

    year_month_day_last c(year(2024), month_day_last(January));
    c += months(1);
    test_true( c == year_month_day_last(year(2024), month_day_last(February)) );
    test_true( c.day() == day(29) );
    c -= months(2);
    test_true( c == year_month_day_last(year(2023), month_day_last(December)) );
    test_true( c.day() == day(31) );
    test_pass("cxx20:year_month_day_last arithmetic members");
}

TEST_CASE(chrono, cal_year_month_weekday) {
    using namespace STD::chrono;

    year_month_weekday ymw0;
    ymw0 = year_month_weekday(year(2000), January, Monday[1]);

    year_month_weekday ymw(year(2024), February, Thursday[5]);
    test_true( ymw.year() == year(2024) );
    test_true( ymw.month() == February );
    test_true( ymw.weekday() == Thursday );
    test_eq( ymw.index(), 5u );
    test_true( ymw.weekday_indexed() == Thursday[5] );
    test_true( ymw.ok() );
    test_pass("cxx20:year_month_weekday");

    sys_days sd = sys_days(ymw);
    test_true( year_month_day(sd) == year_month_day(year(2024), February, day(29)) );
    local_days ld = local_days(ymw);
    test_eq( ld.time_since_epoch().count(), sd.time_since_epoch().count() );
    test_pass("cxx20:year_month_weekday conversions");

    year_month_weekday back(sd);
    test_true( back == ymw );
    year_month_weekday backl(ld);
    test_true( backl == ymw );

    test_true( year_month_day(sys_days(year(2024) / February / Thursday[1]))
               == year_month_day(year(2024), February, day(1)) );
    test_pass("cxx20:year_month_weekday from sys_days");

    year_month_weekday c(year(2024), January, Thursday[1]);
    c += months(1);
    test_true( c == year_month_weekday(year(2024), February, Thursday[1]) );
    c -= months(2);
    test_true( c == year_month_weekday(year(2023), December, Thursday[1]) );
    test_pass("cxx20:year_month_weekday arithmetic members");

    test_true(  year_month_weekday(year(2024), February, Thursday[5]).ok() );
    test_true( !year_month_weekday(year(2024), February, Thursday[6]).ok() );
    test_true( !year_month_weekday(year(2024), February, Friday[5]).ok() );
    test_true( !year_month_weekday(year(2024), month(0), Thursday[1]).ok() );
    test_pass("cxx20:year_month_weekday::ok");
}

TEST_CASE(chrono, cal_year_month_weekday_last) {
    using namespace STD::chrono;

    year_month_weekday_last ymwl(year(2024), February, Thursday[last]);
    test_true( ymwl.year() == year(2024) );
    test_true( ymwl.month() == February );
    test_true( ymwl.weekday() == Thursday );
    test_true( ymwl.weekday_last() == Thursday[last] );
    test_true( ymwl.ok() );
    test_true( !year_month_weekday_last(year(2024), month(13), Thursday[last]).ok() );
    test_true( !year_month_weekday_last(year(2024), February, weekday(9)[last]).ok() );
    test_pass("cxx20:year_month_weekday_last");

    sys_days sd = sys_days(ymwl);
    test_true( year_month_day(sd) == year_month_day(year(2024), February, day(29)) );
    local_days ld = local_days(ymwl);
    test_eq( ld.time_since_epoch().count(), sd.time_since_epoch().count() );

    test_true( year_month_day(sys_days(year(2024) / February / Friday[last]))
               == year_month_day(year(2024), February, day(23)) );
    test_pass("cxx20:year_month_weekday_last conversions");

    year_month_weekday_last c(year(2024), January, Thursday[last]);
    c += months(1);
    test_true( c == year_month_weekday_last(year(2024), February, Thursday[last]) );
    c -= months(2);
    test_true( c == year_month_weekday_last(year(2023), December, Thursday[last]) );
    test_pass("cxx20:year_month_weekday_last arithmetic members");
}

TEST_CASE(chrono, cal_comparisons) {
    using namespace STD::chrono;

    test_true( day(1) == day(1) );
    test_true( day(1) != day(2) );
    test_true( day(1) <  day(2) );
    test_pass("cxx20:day comparisons");

    test_true( January == month(1) );
    test_true( January != February );
    test_true( January <  February );
    test_pass("cxx20:month comparisons");

    test_true( year(2024) == year(2024) );
    test_true( year(2024) != year(2023) );
    test_true( year(2023) <  year(2024) );
    test_pass("cxx20:year comparisons");

    test_true( Sunday == weekday(0) );
    test_true( Sunday != Monday );
    test_pass("cxx20:weekday comparisons");

    test_true( (February / day(29)) == month_day(February, day(29)) );
    test_true( (February / day(29)) != (February / day(28)) );
    test_true( (January / day(31)) < (February / day(1)) );
    test_pass("cxx20:month_day comparisons");

    test_true( (year(2024) / February) == year_month(year(2024), February) );
    test_true( (year(2024) / February) != (year(2024) / March) );
    test_true( (year(2023) / December) < (year(2024) / January) );
    test_pass("cxx20:year_month comparisons");

    year_month_day a(year(2024), February, day(29));
    year_month_day b(year(2024), March, day(1));
    test_true( a == year_month_day(year(2024), February, day(29)) );
    test_true( a != b );
    test_true( a <  b );
    test_pass("cxx20:year_month_day comparisons");

    test_true( (year(2024) / February / last) == year_month_day_last(year(2024), month_day_last(February)) );
    test_true( (year(2024) / February / last) != (year(2024) / March / last) );
    test_true( (year(2024) / February / last) < (year(2024) / March / last) );
    test_pass("cxx20:year_month_day_last comparisons");

    test_true( (year(2024) / February / Thursday[1]) == year_month_weekday(year(2024), February, Thursday[1]) );
    test_true( (year(2024) / February / Thursday[1]) != (year(2024) / February / Thursday[2]) );
    test_true( (year(2024) / February / Thursday[last])
               == year_month_weekday_last(year(2024), February, Thursday[last]) );
    test_true( (year(2024) / February / Thursday[last]) != (year(2024) / February / Friday[last]) );
    test_pass("cxx20:year_month_weekday comparisons");

    #if defined(__cpp_impl_three_way_comparison)
    test_true( (day(2) <=> day(1)) == STD::strong_ordering::greater );
    #else
    TEST_SKIP1();
    #endif
    #if defined(__cpp_impl_three_way_comparison)
    test_true( (December <=> January) == STD::strong_ordering::greater );
    #else
    TEST_SKIP1();
    #endif
    #if defined(__cpp_impl_three_way_comparison)
    test_true( (year(2024) <=> year(2024)) == STD::strong_ordering::equal );
    #else
    TEST_SKIP1();
    #endif
    #if defined(__cpp_impl_three_way_comparison)
    test_true( ((March / day(1)) <=> (February / day(1))) == STD::strong_ordering::greater );
    #else
    TEST_SKIP1();
    #endif
    #if defined(__cpp_impl_three_way_comparison)
    test_true( ((year(2024) / March) <=> (year(2024) / February)) == STD::strong_ordering::greater );
    #else
    TEST_SKIP1();
    #endif
    #if defined(__cpp_impl_three_way_comparison)
    test_true( (b <=> a) == STD::strong_ordering::greater );
    #else
    TEST_SKIP1();
    #endif
    #if defined(__cpp_impl_three_way_comparison)
    test_true( ((year(2024) / March / last) <=> (year(2024) / February / last))
               == STD::strong_ordering::greater );
    test_pass("cxx20:calendar operator<=>");
    #else
    TEST_SKIP1();
    test_skip("cxx20:calendar operator<=>");
    #endif
}

TEST_CASE(chrono, cal_arithmetic) {
    using namespace STD::chrono;

    test_true( (day(10) + days(5)) == day(15) );
    test_true( (days(5) + day(10)) == day(15) );
    test_true( (day(10) - days(5)) == day(5) );
    test_true( (day(10) - day(4)) == days(6) );
    test_pass("cxx20:day arithmetic operators");

    test_true( (November + months(3)) == February );
    test_true( (months(3) + November) == February );
    test_true( (February - months(3)) == November );
    test_true( (March - January) == months(2) );
    test_true( (January - March) == months(10) );
    test_pass("cxx20:month arithmetic operators");

    test_true( (year(2024) + years(1)) == year(2025) );
    test_true( (years(1) + year(2024)) == year(2025) );
    test_true( (year(2024) - years(4)) == year(2020) );
    test_true( (year(2024) - year(2000)) == years(24) );
    test_pass("cxx20:year arithmetic operators");

    test_true( (Saturday + days(1)) == Sunday );
    test_true( (days(1) + Saturday) == Sunday );
    test_true( (Sunday - days(1)) == Saturday );
    test_true( (Thursday - Monday) == days(3) );
    test_true( (Monday - Thursday) == days(4) );
    test_pass("cxx20:weekday arithmetic operators");

    year_month ym(year(2024), November);
    test_true( (ym + months(3)) == year_month(year(2025), February) );
    test_true( (months(3) + ym) == year_month(year(2025), February) );
    test_true( (ym - months(11)) == year_month(year(2023), December) );
    test_true( (ym + years(1)) == year_month(year(2025), November) );
    test_true( (years(1) + ym) == year_month(year(2025), November) );
    test_true( (ym - years(24)) == year_month(year(2000), November) );

    test_true( (year_month(year(2024), March) - year_month(year(2024), January)) == months(2) );
    test_pass("cxx20:year_month arithmetic operators");

    test_true( ((year(2024) / January / day(31)) + months(1)) == (year(2024) / February / day(31)) );
    test_true( ((year(2024) / February / day(29)) + years(1)) == (year(2025) / February / day(29)) );
    test_true( ((year(2024) / February / last) + months(1)) == (year(2024) / March / last) );
    test_true( ((year(2024) / February / Thursday[1]) + months(1)) == (year(2024) / March / Thursday[1]) );
    test_true( ((year(2024) / February / Thursday[last]) + years(1))
               == (year(2025) / February / Thursday[last]) );
    test_pass("cxx20:calendar type arithmetic operators");
}

TEST_CASE(chrono, cal_slash_operators) {
    using namespace STD::chrono;

    test_true( (year(2024) / February) == year_month(year(2024), February) );
    test_true( (year(2024) / 2) == year_month(year(2024), February) );
    test_pass("cxx20:operator/ (year_month)");

    test_true( (February / day(29)) == month_day(February, day(29)) );
    test_true( (February / 29) == month_day(February, day(29)) );
    test_true( (2 / day(29)) == month_day(February, day(29)) );
    test_pass("cxx20:operator/ (month_day)");

    test_true( (February / last) == month_day_last(February) );
    test_true( (last / February) == month_day_last(February) );
    test_true( (last / 2) == month_day_last(February) );
    test_pass("cxx20:operator/ (month_day_last)");

    test_true( (Thursday[3]) == weekday_indexed(Thursday, 3) );
    test_pass("cxx20:weekday_indexed via operator[]");

    test_true( (Thursday[last]) == weekday_last(Thursday) );
    test_pass("cxx20:weekday_last via operator[]");

    test_true( (February / Thursday[3]) == month_weekday(February, Thursday[3]) );
    test_true( (Thursday[3] / February) == month_weekday(February, Thursday[3]) );
    test_true( (Thursday[3] / 2) == month_weekday(February, Thursday[3]) );
    test_pass("cxx20:operator/ (month_weekday)");

    test_true( (February / Thursday[last]) == month_weekday_last(February, Thursday[last]) );
    test_true( (Thursday[last] / February) == month_weekday_last(February, Thursday[last]) );
    test_true( (Thursday[last] / 2) == month_weekday_last(February, Thursday[last]) );
    test_pass("cxx20:operator/ (month_weekday_last)");

    year_month ym(year(2024), February);
    test_true( (ym / day(29)) == year_month_day(year(2024), February, day(29)) );
    test_true( (ym / 29) == year_month_day(year(2024), February, day(29)) );
    test_pass("cxx20:operator/ (year_month_day)");

    test_true( (ym / last) == year_month_day_last(year(2024), month_day_last(February)) );
    test_pass("cxx20:operator/ (year_month_day_last)");

    test_true( (ym / Thursday[1]) == year_month_weekday(year(2024), February, Thursday[1]) );
    test_pass("cxx20:operator/ (year_month_weekday)");

    test_true( (ym / Thursday[last]) == year_month_weekday_last(year(2024), February, Thursday[last]) );
    test_pass("cxx20:operator/ (year_month_weekday_last)");

    test_true( sys_days(year(2024) / February / day(29)) == sys_days(year(2024) / February / last) );
    test_true( sys_days(year(2024) / February / day(29)) == sys_days(year(2024) / February / Thursday[last]) );
    test_true( sys_days(year(2024) / February / day(29)) == sys_days(year(2024) / February / Thursday[5]) );
    test_pass("cxx20:operator/ chained forms");
}

TEST_CASE(chrono, cal_constants) {
    using namespace STD::chrono;

    test_true( (year(2024) / February / last).day() == day(29) );
    test_pass("cxx20:last constant");

    test_eq( Sunday.c_encoding(),    0u );
    test_eq( Monday.c_encoding(),    1u );
    test_eq( Tuesday.c_encoding(),   2u );
    test_eq( Wednesday.c_encoding(), 3u );
    test_eq( Thursday.c_encoding(),  4u );
    test_eq( Friday.c_encoding(),    5u );
    test_eq( Saturday.c_encoding(),  6u );
    test_pass("cxx20:weekday constants");

    test_eq( (unsigned)January,   1u );
    test_eq( (unsigned)February,  2u );
    test_eq( (unsigned)March,     3u );
    test_eq( (unsigned)April,     4u );
    test_eq( (unsigned)May,       5u );
    test_eq( (unsigned)June,      6u );
    test_eq( (unsigned)July,      7u );
    test_eq( (unsigned)August,    8u );
    test_eq( (unsigned)September, 9u );
    test_eq( (unsigned)October,  10u );
    test_eq( (unsigned)November, 11u );
    test_eq( (unsigned)December, 12u );
    test_pass("cxx20:month constants");

    #if _TST_HAS_DECLTYPE
    test_true( (STD::is_same<const month, decltype(January)>::value) );
    test_true( (STD::is_same<const weekday, decltype(Sunday)>::value) );
    test_pass("cxx20:calendar constant types");
    #else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:calendar constant types");
    #endif
}

TEST_CASE(chrono, cal_stream_output) {
    using namespace STD::chrono;

    STD::ostringstream o1;  o1 << day(29);
    test_str_eq( o1.str(), "29" );
    STD::ostringstream o2;  o2 << January;
    test_str_eq( o2.str(), "Jan" );
    STD::ostringstream o3;  o3 << year(2024);
    test_str_eq( o3.str(), "2024" );
    STD::ostringstream o4;  o4 << Thursday;
    test_str_eq( o4.str(), "Thu" );
    STD::ostringstream o5;  o5 << (February / day(29));
    test_str_eq( o5.str(), "Feb/29" );
    STD::ostringstream o6;  o6 << (year(2024) / February);
    test_str_eq( o6.str(), "2024/Feb" );
    STD::ostringstream o7;  o7 << (year(2024) / February / day(29));
    test_str_eq( o7.str(), "2024-02-29" );
    test_pass("cxx20:operator<< calendar types");

    STD::ostringstream o8;  o8 << day(0);
    test_true( o8.str().find("not a valid day") != STD::string::npos );
    test_pass("cxx20:operator<< invalid value");

    #if defined(__cpp_lib_format)

    test_str_eq( STD::format("{}", day(29)), "29" );
    test_str_eq( STD::format("{}", February), "Feb" );
    test_str_eq( STD::format("{}", year(2024)), "2024" );
    test_str_eq( STD::format("{}", Thursday), "Thu" );
    test_str_eq( STD::format("{}", year(2024) / February / day(29)), "2024-02-29" );
    test_str_eq( STD::format("{:%Y-%m-%d}", year(2024) / February / day(29)), "2024-02-29" );
    test_str_eq( STD::format("{:%F}", year(2024) / February / day(29)), "2024-02-29" );
    test_str_eq( STD::format("{:%b %d, %Y}", year(2024) / February / day(29)), "Feb 29, 2024" );
    test_str_eq( STD::format("{:%A}", Thursday), "Thursday" );
    test_str_eq( STD::format("{:%B}", February), "February" );
    test_str_eq( STD::format("{}", year(2024) / February / last), "2024/Feb/last" );
    test_str_eq( STD::format("{}", Thursday[2]), "Thu[2]" );
    test_str_eq( STD::format("{}", Thursday[last]), "Thu[last]" );
    test_str_eq( STD::format("{}", February / Thursday[2]), "Feb/Thu[2]" );
    test_str_eq( STD::format("{}", February / last), "Feb/last" );
    test_str_eq( STD::format("{}", year(2024) / February / Thursday[2]), "2024/Feb/Thu[2]" );
    test_str_eq( STD::format("{}", year(2024) / February / Thursday[last]), "2024/Feb/Thu[last]" );
    test_pass("cxx20:format calendar types");
    #else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:format calendar types");
    #endif
}

#if !defined(__GNUC__)
TEST_CASE(chrono, cal_from_stream) {
    using namespace STD::chrono;

    STD::istringstream i1("29");
    day d;
    from_stream(i1, "%d", d);
    test_true( !i1.fail() );
    test_true( d == day(29) );
    test_pass("cxx20:from_stream(day)");

    STD::istringstream i2("02");
    month m;
    from_stream(i2, "%m", m);
    test_true( !i2.fail() );
    test_true( m == February );
    test_pass("cxx20:from_stream(month)");

    STD::istringstream i3("2024");
    year y;
    from_stream(i3, "%Y", y);
    test_true( !i3.fail() );
    test_true( y == year(2024) );
    test_pass("cxx20:from_stream(year)");

    STD::istringstream i4("Thu");
    weekday w;
    from_stream(i4, "%a", w);
    test_true( !i4.fail() );
    test_true( w == Thursday );
    test_pass("cxx20:from_stream(weekday)");

    STD::istringstream i5("02/29");
    month_day md;
    from_stream(i5, "%m/%d", md);
    test_true( !i5.fail() );
    test_true( md == (February / day(29)) );
    test_pass("cxx20:from_stream(month_day)");

    STD::istringstream i6("2024/02");
    year_month ym;
    from_stream(i6, "%Y/%m", ym);
    test_true( !i6.fail() );
    test_true( ym == (year(2024) / February) );
    test_pass("cxx20:from_stream(year_month)");

    STD::istringstream i7("2024-02-29");
    year_month_day ymd;
    from_stream(i7, "%F", ymd);
    test_true( !i7.fail() );
    test_true( ymd == (year(2024) / February / day(29)) );
    test_pass("cxx20:from_stream(year_month_day)");

    STD::istringstream i8("zz");
    day bad;
    from_stream(i8, "%d", bad);
    test_true( i8.fail() );
    test_pass("cxx20:from_stream failbit (calendar)");
}
#else
TEST_CASE_SKIP(chrono, cal_from_stream)
#endif


namespace {
template <class _Want, class _Got>
bool cal_has_type(const _Got&) { return STD::is_same<_Want, _Got>::value; }
}

TEST_CASE(chrono, cal_month_constants_each) {
    using namespace STD::chrono;
#define _TST_MONTH_CONST(NAME, N)                                                  \
    do {                                                                           \
        test_true( cal_has_type<month>(NAME) );           \
        test_eq( (unsigned)NAME, (unsigned)(N) );                                  \
        test_true( NAME == month(N) && NAME.ok() );                                \
        test_pass("cxx20:" #NAME);                                                 \
    } while (0)
    _TST_MONTH_CONST(January, 1);
    _TST_MONTH_CONST(February, 2);
    _TST_MONTH_CONST(March, 3);
    _TST_MONTH_CONST(April, 4);
    _TST_MONTH_CONST(May, 5);
    _TST_MONTH_CONST(June, 6);
    _TST_MONTH_CONST(July, 7);
    _TST_MONTH_CONST(August, 8);
    _TST_MONTH_CONST(September, 9);
    _TST_MONTH_CONST(October, 10);
    _TST_MONTH_CONST(November, 11);
    _TST_MONTH_CONST(December, 12);
#undef _TST_MONTH_CONST
}

TEST_CASE(chrono, cal_weekday_constants_each) {
    using namespace STD::chrono;
#define _TST_WDAY_CONST(NAME, N)                                                   \
    do {                                                                           \
        test_true( cal_has_type<weekday>(NAME) );         \
        test_eq( NAME.c_encoding(), (unsigned)(N) );                               \
        test_true( NAME == weekday(N) && NAME.ok() );                              \
        test_pass("cxx20:" #NAME);                                                 \
    } while (0)
    _TST_WDAY_CONST(Sunday, 0);
    _TST_WDAY_CONST(Monday, 1);
    _TST_WDAY_CONST(Tuesday, 2);
    _TST_WDAY_CONST(Wednesday, 3);
    _TST_WDAY_CONST(Thursday, 4);
    _TST_WDAY_CONST(Friday, 5);
    _TST_WDAY_CONST(Saturday, 6);
#undef _TST_WDAY_CONST
}

TEST_CASE(chrono, cal_member_operators_each) {
    using namespace STD::chrono;

    day d(5);
    day& dr = (++d);
    test_eq( (unsigned)d, 6u );
    test_true( &dr == &d );
    day dpost = d++;
    test_eq( (unsigned)dpost, 6u );
    test_eq( (unsigned)d, 7u );
    test_pass("cxx20:day::operator++");

    day& dr2 = (--d);
    test_eq( (unsigned)d, 6u );
    test_true( &dr2 == &d );
    day dpre = d--;
    test_eq( (unsigned)dpre, 6u );
    test_eq( (unsigned)d, 5u );
    test_pass("cxx20:day::operator--");

    day& dr3 = (d += days(10));
    test_eq( (unsigned)d, 15u );
    test_true( &dr3 == &d );
    test_pass("cxx20:day::operator+=");

    day& dr4 = (d -= days(4));
    test_eq( (unsigned)d, 11u );
    test_true( &dr4 == &d );
    test_pass("cxx20:day::operator-=");

    month m(11);
    month& mr = (++m);
    test_eq( (unsigned)m, 12u );
    test_true( &mr == &m );
    month mpost = m++;
    test_eq( (unsigned)mpost, 12u );
    test_eq( (unsigned)m, 1u );
    test_pass("cxx20:month::operator++");

    month& mr2 = (--m);
    test_eq( (unsigned)m, 12u );
    test_true( &mr2 == &m );
    month mpre = m--;
    test_eq( (unsigned)mpre, 12u );
    test_eq( (unsigned)m, 11u );
    test_pass("cxx20:month::operator--");

    month& mr3 = (m += months(2));
    test_eq( (unsigned)m, 1u );
    test_true( &mr3 == &m );
    test_pass("cxx20:month::operator+=");

    month& mr4 = (m -= months(2));
    test_eq( (unsigned)m, 11u );
    test_true( &mr4 == &m );
    test_pass("cxx20:month::operator-=");

    weekday w(6);
    weekday& wr = (++w);
    test_eq( w.c_encoding(), 0u );
    test_true( &wr == &w );
    weekday wpost = w++;
    test_eq( wpost.c_encoding(), 0u );
    test_eq( w.c_encoding(), 1u );
    test_pass("cxx20:weekday::operator++");

    weekday& wr2 = (--w);
    test_eq( w.c_encoding(), 0u );
    test_true( &wr2 == &w );
    weekday wpre = w--;
    test_eq( wpre.c_encoding(), 0u );
    test_eq( w.c_encoding(), 6u );
    test_pass("cxx20:weekday::operator--");

    weekday& wr3 = (w += days(2));
    test_eq( w.c_encoding(), 1u );
    test_true( &wr3 == &w );
    test_pass("cxx20:weekday::operator+=");

    weekday& wr4 = (w -= days(2));
    test_eq( w.c_encoding(), 6u );
    test_true( &wr4 == &w );
    test_pass("cxx20:weekday::operator-=");

    year y(2024);
    year& yr = (++y);
    test_eq( (int)y, 2025 );
    test_true( &yr == &y );
    year ypost = y++;
    test_eq( (int)ypost, 2025 );
    test_eq( (int)y, 2026 );
    test_pass("cxx20:year::operator++");

    year& yr2 = (--y);
    test_eq( (int)y, 2025 );
    test_true( &yr2 == &y );
    year ypre = y--;
    test_eq( (int)ypre, 2025 );
    test_eq( (int)y, 2024 );
    test_pass("cxx20:year::operator--");

    year& yr3 = (y += years(3));
    test_eq( (int)y, 2027 );
    test_true( &yr3 == &y );
    test_pass("cxx20:year::operator+=");

    year& yr4 = (y -= years(3));
    test_eq( (int)y, 2024 );
    test_true( &yr4 == &y );
    test_pass("cxx20:year::operator-=");

    year_month_day ymd(year(2024) / January / 31);
    year_month_day& r1 = (ymd += months(1));
    test_eq( (int)ymd.year(), 2024 );
    test_eq( (unsigned)ymd.month(), 2u );
    test_eq( (unsigned)ymd.day(), 31u );
    test_true( !ymd.ok() );
    test_true( &r1 == &ymd );
    test_pass("cxx20:year_month_day::operator+=");

    year_month_day& r2 = (ymd -= months(1));
    test_eq( (unsigned)ymd.month(), 1u );
    test_true( ymd.ok() );
    test_true( &r2 == &ymd );
    year_month_day ymd2(year(2024) / March / 1);
    ymd2 += years(1);
    test_eq( (int)ymd2.year(), 2025 );
    ymd2 -= years(1);
    test_eq( (int)ymd2.year(), 2024 );
    test_pass("cxx20:year_month_day::operator-=");
}
#else
TEST_CASE_SKIP(chrono, cal_day)
TEST_CASE_SKIP(chrono, cal_month)
TEST_CASE_SKIP(chrono, cal_year)
TEST_CASE_SKIP(chrono, cal_weekday)
TEST_CASE_SKIP(chrono, cal_weekday_indexed_and_last)
TEST_CASE_SKIP(chrono, cal_month_day_and_last)
TEST_CASE_SKIP(chrono, cal_month_weekday_and_last)
TEST_CASE_SKIP(chrono, cal_year_month)
TEST_CASE_SKIP(chrono, cal_year_month_day)
TEST_CASE_SKIP(chrono, cal_year_month_day_last)
TEST_CASE_SKIP(chrono, cal_year_month_weekday)
TEST_CASE_SKIP(chrono, cal_year_month_weekday_last)
TEST_CASE_SKIP(chrono, cal_comparisons)
TEST_CASE_SKIP(chrono, cal_arithmetic)
TEST_CASE_SKIP(chrono, cal_slash_operators)
TEST_CASE_SKIP(chrono, cal_constants)
TEST_CASE_SKIP(chrono, cal_stream_output)
TEST_CASE_SKIP(chrono, cal_from_stream)
TEST_CASE_SKIP(chrono, cal_month_constants_each)
TEST_CASE_SKIP(chrono, cal_weekday_constants_each)
TEST_CASE_SKIP(chrono, cal_member_operators_each)
#endif
