#include "test_cxx.hpp"

#if _tst_cplusplus >= 201103L
#include <chrono>
#include <compare>
#include <format>
#include <limits>
#include <ratio>
#include <sstream>
#include <string>
#include <type_traits>

using _ccw::string_view;

namespace {

typedef STD::chrono::duration<int, STD::milli>      ms_i;
typedef STD::chrono::duration<double, STD::ratio<1> > sec_d;
typedef STD::chrono::nanoseconds                    ns_t;

}

TEST_CASE(chrono, duration_member_aliases_and_ctors) {

    test_true( (STD::is_same<ms_i::rep, int>::value) );
    test_true( (STD::is_same<sec_d::rep, double>::value) );
    test_true( STD::numeric_limits<ns_t::rep>::digits >= 63 );
    test_pass("cxx11:duration::rep");

    test_true( (STD::is_same<ms_i::period, STD::milli>::value) );
    test_true( (STD::is_same<sec_d::period, STD::ratio<1> >::value) );
    test_true( (STD::is_same<ns_t::period, STD::nano>::value) );
    test_pass("cxx11:duration::period");

    ms_i d0 = ms_i();
    test_eq( d0.count(), 0 );
    sec_d f0 = sec_d();
    test_eq( f0.count(), 0.0 );
    test_pass("cxx11:duration default ctor");

    ms_i a(1500);
    test_eq( a.count(), 1500 );
    sec_d f(2.5);
    test_eq( f.count(), 2.5 );
    ns_t n(1234567890123LL);
    test_eq( n.count(), 1234567890123LL );
    test_pass("cxx11:duration ctor from rep");

    ns_t from_us(STD::chrono::microseconds(2));
    test_eq( from_us.count(), 2000LL );
    sec_d from_ms(a);
    test_eq( from_ms.count(), 1.5 );
    ms_i from_s(STD::chrono::seconds(3));
    test_eq( from_s.count(), 3000 );
    test_pass("cxx11:duration converting ctor");
}

TEST_CASE(chrono, duration_member_operators) {
    ms_i a(100);
    test_eq( (+a).count(), 100 );
    test_pass("cxx11:duration unary +");
    test_eq( (-a).count(), -100 );
    test_pass("cxx11:duration unary -");

    ms_i b(5);
    ++b;
    test_eq( b.count(), 6 );
    ms_i c = b++;
    test_eq( c.count(), 6 );
    test_eq( b.count(), 7 );
    test_pass("cxx11:duration increment");
    --b;
    test_eq( b.count(), 6 );
    ms_i e = b--;
    test_eq( e.count(), 6 );
    test_eq( b.count(), 5 );
    test_pass("cxx11:duration decrement");

    b += ms_i(10);
    test_eq( b.count(), 15 );
    b -= ms_i(4);
    test_eq( b.count(), 11 );
    b *= 3;
    test_eq( b.count(), 33 );
    b /= 2;
    test_eq( b.count(), 16 );
    b %= 5;
    test_eq( b.count(), 1 );
    ms_i g(17);
    g %= ms_i(5);
    test_eq( g.count(), 2 );
    test_pass("cxx11:duration compound assignment");

    sec_d f(2.5);
    f += sec_d(0.25);
    test_eq( f.count(), 2.75 );
    f -= sec_d(0.75);
    test_eq( f.count(), 2.0 );
    f *= 2.0;
    test_eq( f.count(), 4.0 );
    f /= 8.0;
    test_eq( f.count(), 0.5 );
    test_eq( (-f).count(), -0.5 );
    test_pass("cxx11:duration ops (floating rep)");

    ns_t n(1000);
    n += ns_t(500);
    test_eq( n.count(), 1500LL );
    ++n;
    test_eq( n.count(), 1501LL );
    test_pass("cxx11:duration ops (64-bit rep)");
}

TEST_CASE(chrono, duration_static_members) {
    test_eq( ms_i::zero().count(), 0 );
    test_eq( sec_d::zero().count(), 0.0 );
    test_eq( ns_t::zero().count(), 0LL );
    test_pass("cxx11:duration::zero");

    test_true( ms_i::min().count() == (STD::numeric_limits<int>::lowest)() );
    test_true( ns_t::min() < ns_t::zero() );
    test_true( sec_d::min().count() == (STD::numeric_limits<double>::lowest)() );
    test_pass("cxx11:duration::min");
    test_true( ms_i::max().count() == (STD::numeric_limits<int>::max)() );
    test_true( ns_t::max() > ns_t::zero() );
    test_true( sec_d::max().count() == (STD::numeric_limits<double>::max)() );
    test_pass("cxx11:duration::max");
}

TEST_CASE(chrono, duration_nonmember_arithmetic) {
    using namespace STD::chrono;

    milliseconds sum = milliseconds(700) + milliseconds(300);
    test_eq( sum.count(), 1000LL );
    microseconds mixed = milliseconds(1) + microseconds(1);
    test_eq( mixed.count(), 1001LL );
    test_true( (STD::is_same<decltype(mixed), microseconds>::value) );
    test_pass("cxx11:duration operator+");

    milliseconds diff = seconds(2) - milliseconds(500);
    test_eq( diff.count(), 1500LL );
    test_pass("cxx11:duration operator-");

    ms_i a(21);
    test_eq( (a * 2).count(), 42 );
    test_eq( (2 * a).count(), 42 );
    sec_d f(1.5);
    test_eq( (f * 2.0).count(), 3.0 );
    test_eq( (3.0 * f).count(), 4.5 );
    test_pass("cxx11:duration operator*");

    test_eq( (ms_i(42) / 2).count(), 21 );
    test_eq( (sec_d(3.0) / 4.0).count(), 0.75 );
    test_pass("cxx11:duration operator/(scalar)");

    test_eq( seconds(10) / seconds(3), 3LL );
    test_eq( minutes(1) / seconds(15), 4LL );
    double ratio = sec_d(3.0) / sec_d(2.0);
    test_eq( ratio, 1.5 );
    test_pass("cxx11:duration operator/(duration)");

    test_eq( (ms_i(17) % 5).count(), 2 );
    test_eq( (seconds(10) % seconds(3)).count(), 1LL );
    milliseconds rem = seconds(1) % milliseconds(300);
    test_eq( rem.count(), 100LL );
    test_pass("cxx11:duration operator%");

    test_eq( (seconds(-10) / seconds(3)), -3LL );
    test_eq( (seconds(-10) % seconds(3)).count(), -1LL );
    test_pass("cxx11:duration truncation (negative)");
}

TEST_CASE(chrono, duration_comparisons) {
    using namespace STD::chrono;

    test_true( seconds(1) == milliseconds(1000) );
    test_true( seconds(1) != milliseconds(999) );
    test_true( milliseconds(999) <  seconds(1) );
    test_true( milliseconds(1000) <= seconds(1) );
    test_true( seconds(1) >  milliseconds(999) );
    test_true( seconds(1) >= milliseconds(1000) );
    test_pass("cxx11:duration comparisons");

    test_true( sec_d(1.5) == ms_i(1500) );
    test_true( sec_d(1.5) >  ms_i(1499) );
    test_pass("cxx11:duration comparison (mixed rep)");

#if _tst_cplusplus >= 202002L
    STD::strong_ordering o1 = seconds(1) <=> milliseconds(999);
    test_true( o1 == STD::strong_ordering::greater );
    STD::strong_ordering o2 = seconds(1) <=> milliseconds(1000);
    test_true( o2 == STD::strong_ordering::equal );
    STD::strong_ordering o3 = milliseconds(1) <=> seconds(1);
    test_true( o3 == STD::strong_ordering::less );
    test_true( (seconds(2) <=> seconds(1)) > 0 );
    test_pass("cxx20:duration operator<=>");
#else
    TEST_NOTE("operator<=> needs a C++20 front end");
    test_skip("cxx20:duration operator<=>");
#endif
}

TEST_CASE(chrono, duration_cast_and_rounding) {
    using namespace STD::chrono;

    test_eq( duration_cast<seconds>(milliseconds(1999)).count(), 1LL );
    test_eq( duration_cast<seconds>(milliseconds(-1999)).count(), -1LL );
    test_eq( duration_cast<milliseconds>(seconds(2)).count(), 2000LL );
    test_eq( duration_cast<ns_t>(microseconds(3)).count(), 3000LL );

    test_eq( duration_cast<sec_d>(milliseconds(1500)).count(), 1.5 );
    test_eq( duration_cast<ms_i>(sec_d(0.25)).count(), 250 );
    test_pass("cxx11:duration_cast");
#if TEST_TARGET_CXX >= 2017

    test_eq( floor<seconds>(milliseconds(1999)).count(), 1LL );
    test_eq( floor<seconds>(milliseconds(-1001)).count(), -2LL );
    test_eq( floor<seconds>(milliseconds(2000)).count(), 2LL );
    test_pass("cxx17:chrono floor(duration)");

    test_eq( ceil<seconds>(milliseconds(1001)).count(), 2LL );
    test_eq( ceil<seconds>(milliseconds(-1999)).count(), -1LL );
    test_eq( ceil<seconds>(milliseconds(2000)).count(), 2LL );
    test_pass("cxx17:chrono ceil(duration)");

    test_eq( round<seconds>(milliseconds(1499)).count(), 1LL );
    test_eq( round<seconds>(milliseconds(1501)).count(), 2LL );
    test_eq( round<seconds>(milliseconds(1500)).count(), 2LL );
    test_eq( round<seconds>(milliseconds(2500)).count(), 2LL );
    test_eq( round<seconds>(milliseconds(-1500)).count(), -2LL );
    test_pass("cxx17:chrono round(duration)");

    test_eq( abs(seconds(-3)).count(), 3LL );
    test_eq( abs(seconds(3)).count(), 3LL );
    test_eq( abs(ms_i(-7)).count(), 7 );
    test_eq( abs(sec_d(-1.25)).count(), 1.25 );
#else
    TEST_NOTE("chrono floor / ceil / round / abs are C++17");
    test_skip("cxx17:chrono floor(duration)");
    test_skip("cxx17:chrono ceil(duration)");
    test_skip("cxx17:chrono round(duration)");
#endif
    test_pass("cxx17:chrono abs(duration)");
}

TEST_CASE(chrono, duration_traits_and_common_type) {

    test_true( !STD::chrono::treat_as_floating_point<int>::value );
    test_true( !STD::chrono::treat_as_floating_point<long long>::value );
    test_true(  STD::chrono::treat_as_floating_point<double>::value );
#if TEST_TARGET_CXX >= 2017
    test_true( !STD::chrono::treat_as_floating_point_v<int> );
    test_true(  STD::chrono::treat_as_floating_point_v<float> );
    test_true(  STD::chrono::treat_as_floating_point_v<long double> );
    test_pass("cxx11:treat_as_floating_point");
#else
    TEST_SKIP_N(3);
    test_skip("cxx11:treat_as_floating_point");
#endif

    test_eq( STD::chrono::duration_values<int>::zero(), 0 );
    test_true( STD::chrono::duration_values<int>::min() == (STD::numeric_limits<int>::lowest)() );
    test_true( STD::chrono::duration_values<int>::max() == (STD::numeric_limits<int>::max)() );
    test_eq( STD::chrono::duration_values<double>::zero(), 0.0 );
    test_true( STD::chrono::duration_values<double>::max() == (STD::numeric_limits<double>::max)() );
    test_pass("cxx11:duration_values");

    typedef STD::common_type<STD::chrono::milliseconds, STD::chrono::microseconds>::type C1;
    test_true( (STD::is_same<C1, STD::chrono::microseconds>::value) );
    typedef STD::common_type<ms_i, sec_d>::type C2;
    test_true( (STD::is_same<C2, STD::chrono::duration<double, STD::milli> >::value) );
    typedef STD::common_type<STD::chrono::hours, STD::chrono::minutes>::type C3;
    test_true( (STD::is_same<C3, STD::chrono::minutes>::value) );

    typedef STD::common_type<ns_t, ns_t>::type C4;
    test_true( (STD::is_same<C4, ns_t>::value) );
    test_pass("cxx11:common_type<duration>");
}

TEST_CASE(chrono, duration_standard_aliases) {
    using namespace STD::chrono;

    test_true( nanoseconds(1000) == microseconds(1) );
    test_true( microseconds(1000) == milliseconds(1) );
    test_true( milliseconds(1000) == seconds(1) );
    test_true( seconds(60) == minutes(1) );
    test_true( minutes(60) == hours(1) );
    test_true( (STD::is_same<nanoseconds::period,  STD::nano>::value) );
    test_true( (STD::is_same<microseconds::period, STD::micro>::value) );
    test_true( (STD::is_same<milliseconds::period, STD::milli>::value) );
    test_true( (STD::is_same<seconds::period, STD::ratio<1> >::value) );
    test_true( (STD::is_same<minutes::period, STD::ratio<60> >::value) );
    test_true( (STD::is_same<hours::period, STD::ratio<3600> >::value) );
    test_pass("cxx11:duration aliases (ns..hours)");

#if TEST_TARGET_CXX >= 2020
    test_true( hours(24) == days(1) );
    test_true( days(7) == weeks(1) );
    test_true( (STD::is_same<days::period,  STD::ratio<86400> >::value) );
    test_true( (STD::is_same<weeks::period, STD::ratio<604800> >::value) );

    test_true( (STD::is_same<months::period, STD::ratio<2629746> >::value) );
    test_true( (STD::is_same<years::period,  STD::ratio<31556952> >::value) );
    test_true( years(1) == months(12) );
    test_eq( duration_cast<days>(years(1)).count(), 365LL );
    test_eq( duration_cast<hours>(days(2)).count(), 48LL );
    test_pass("cxx20:duration aliases (days..years)");
#else
    TEST_NOTE("days / weeks / months / years are C++20");
    test_skip("cxx20:duration aliases (days..years)");
#endif
}

#if TEST_TARGET_CXX >= 2020
TEST_CASE(chrono, duration_io_and_format) {
    using namespace STD::chrono;

    STD::ostringstream os1;  os1 << milliseconds(42);
    test_str_eq( os1.str(), "42ms" );
    STD::ostringstream os2;  os2 << seconds(3);
    test_str_eq( os2.str(), "3s" );
    STD::ostringstream os3;  os3 << hours(2);
    test_str_eq( os3.str(), "2h" );
    STD::ostringstream os4;  os4 << nanoseconds(7);
    test_str_eq( os4.str(), "7ns" );
    STD::ostringstream os5;  os5 << minutes(5);
    test_str_eq( os5.str(), "5min" );
    STD::ostringstream os6;  os6 << duration<int, STD::ratio<3, 7> >(2);
    test_str_eq( os6.str(), "2[3/7]s" );
    test_pass("cxx20:duration operator<<");

    test_str_eq( STD::format("{}", milliseconds(42)), "42ms" );
    test_str_eq( STD::format("{}", hours(2)), "2h" );
    test_str_eq( STD::format("{:>8}", seconds(3)), "      3s" );
    test_str_eq( STD::format("{:%H:%M:%S}", seconds(3671)), "01:01:11" );
    test_str_eq( STD::format("{:%Q %q}", milliseconds(250)), "250 ms" );
    test_str_eq( STD::format("{:%j}", hours(49)), "2" );
    test_pass("cxx20:format duration");

#if !defined(__GNUC__)
    STD::istringstream is1("42");
    minutes m{};
    from_stream(is1, "%M", m);
    test_true( !is1.fail() );
    test_eq( m.count(), 42 );

    STD::istringstream is2("07");
    seconds s{};
    from_stream(is2, "%S", s);
    test_true( !is2.fail() );
    test_eq( s.count(), 7LL );

    STD::istringstream is3("xx");
    seconds bad(99);
    from_stream(is3, "%S", bad);
    test_true( is3.fail() );
    test_pass("cxx20:from_stream duration");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
}
#else
TEST_CASE(chrono, duration_io_and_format) {
    TEST_NOTE("duration stream output and chrono formatting are C++20");
    test_skip("cxx20:duration operator<<");
    test_skip("cxx20:format duration");
    test_skip("cxx20:from_stream duration");
}
#endif
#if TEST_TARGET_CXX >= 2014

TEST_CASE(chrono, chrono_literals) {
    using namespace STD::chrono;
    using namespace STD::chrono_literals;

    test_true( 1h   == hours(1) );
    test_true( 1min == minutes(1) );
    test_true( 1s   == seconds(1) );
    test_true( 1ms  == milliseconds(1) );
    test_true( 1us  == microseconds(1) );
    test_true( 1ns  == nanoseconds(1) );
    test_true( (STD::is_same<decltype(1h),   hours>::value) );
    test_true( (STD::is_same<decltype(1min), minutes>::value) );
    test_true( (STD::is_same<decltype(1s),   seconds>::value) );
    test_true( (STD::is_same<decltype(1ms),  milliseconds>::value) );
    test_true( (STD::is_same<decltype(1us),  microseconds>::value) );
    test_true( (STD::is_same<decltype(1ns),  nanoseconds>::value) );
    test_true( 90min == 1h + 30min );
    test_pass("cxx14:chrono_literals (integer)");

    auto fh   = 1.5h;
    auto fmin = 1.5min;
    auto fs   = 1.5s;
    auto fms  = 1.5ms;
    auto fus  = 1.5us;
    auto fns  = 1.5ns;
    test_eq( fh.count(),   1.5 );
    test_eq( fmin.count(), 1.5 );
    test_eq( fs.count(),   1.5 );
    test_eq( fms.count(),  1.5 );
    test_eq( fus.count(),  1.5 );
    test_eq( fns.count(),  1.5 );
    TEST_SKIP_GCC("libstdc++ floating chrono-literal uses a different representation");
    test_true( (STD::is_same<decltype(fh), duration<double, STD::ratio<3600> > >::value) );
    TEST_SKIP_GCC("libstdc++ floating chrono-literal uses a different representation");
    test_true( (STD::is_same<decltype(fns), duration<double, STD::nano> >::value) );
    test_true( fh == 90min );
    test_eq( duration_cast<minutes>(fh).count(), 90 );
    test_pass("cxx14:chrono_literals (floating)");

#if TEST_TARGET_CXX >= 2020
    day d = 29d;
    test_eq( (unsigned)d, 29u );
    test_true( (STD::is_same<decltype(29d), day>::value) );
    year y = 2024y;
    test_eq( (int)y, 2024 );
    test_true( (STD::is_same<decltype(2024y), year>::value) );
    test_pass("cxx20:chrono_literals (d/y)");
#else
    TEST_NOTE("the calendar literals (29d / 2024y) are C++20");
    test_skip("cxx20:chrono_literals (d/y)");
#endif
}
#else
TEST_CASE(chrono, chrono_literals) {
    TEST_NOTE("the chrono literals are C++14");
    test_skip("cxx14:chrono_literals (integer)");
    test_skip("cxx14:chrono_literals (floating)");
    test_skip("cxx20:chrono_literals (d/y)");
}
#endif

TEST_CASE(chrono, duration_compound_assignment) {
    using namespace STD::chrono;

    seconds s(10);
    seconds& r1 = (s += seconds(5));
    test_eq( (long long)s.count(), 15LL );
    test_true( &r1 == &s );
    test_pass("cxx11:duration::operator+=");

    seconds& r2 = (s -= seconds(3));
    test_eq( (long long)s.count(), 12LL );
    test_true( &r2 == &s );
    test_pass("cxx11:duration::operator-=");

    seconds& r3 = (s *= 3);
    test_eq( (long long)s.count(), 36LL );
    test_true( &r3 == &s );
    test_pass("cxx11:duration::operator*=");

    seconds& r4 = (s /= 4);
    test_eq( (long long)s.count(), 9LL );
    test_true( &r4 == &s );
    test_pass("cxx11:duration::operator/=");

    seconds m(17);
    seconds& r5 = (m %= 5);
    test_eq( (long long)m.count(), 2LL );
    m = seconds(17);
    m %= seconds(5);
    test_eq( (long long)m.count(), 2LL );
    test_true( &r5 == &m );
    test_pass("cxx11:duration::operator%=");

    seconds d(5);
    seconds& r6 = (--d);
    test_eq( (long long)d.count(), 4LL );
    test_true( &r6 == &d );
    seconds post = d--;
    test_eq( (long long)post.count(), 4LL );
    test_eq( (long long)d.count(), 3LL );
    test_pass("cxx11:duration::operator--");
}

TEST_CASE(chrono, duration_alias_periods) {
    using namespace STD::chrono;

    test_true( (STD::is_same<nanoseconds::period, STD::nano>::value) );
    test_eq( (long long)duration_cast<nanoseconds>(microseconds(1)).count(), 1000LL );
    test_pass("cxx11:nanoseconds");

    test_true( (STD::is_same<microseconds::period, STD::micro>::value) );
    test_eq( (long long)duration_cast<microseconds>(milliseconds(1)).count(), 1000LL );
    test_pass("cxx11:microseconds");

    test_true( (STD::is_same<milliseconds::period, STD::milli>::value) );
    test_eq( (long long)duration_cast<milliseconds>(seconds(1)).count(), 1000LL );
    test_pass("cxx11:milliseconds");

    test_true( (STD::is_same<seconds::period, STD::ratio<1> >::value) );
    test_eq( (long long)duration_cast<seconds>(minutes(1)).count(), 60LL );
    test_pass("cxx11:seconds");

    test_true( (STD::is_same<minutes::period, STD::ratio<60> >::value) );
    test_eq( (long long)duration_cast<minutes>(hours(1)).count(), 60LL );
    test_pass("cxx11:minutes");

    test_true( (STD::is_same<hours::period, STD::ratio<3600> >::value) );
    test_eq( (long long)duration_cast<hours>(minutes(120)).count(), 2LL );
    test_pass("cxx11:hours");

#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_chrono) && (__cpp_lib_chrono) >= 201907L
    test_true( (STD::is_same<days::period, STD::ratio<86400> >::value) );
    test_eq( (long long)duration_cast<hours>(days(1)).count(), 24LL );
    test_pass("cxx20:days");

    test_true( (STD::is_same<weeks::period, STD::ratio<604800> >::value) );
    test_eq( (long long)duration_cast<days>(weeks(1)).count(), 7LL );
    test_pass("cxx20:weeks");

    test_true( (STD::is_same<years::period, STD::ratio<31556952> >::value) );
    test_eq( (long long)duration_cast<days>(years(1)).count(), 365LL );
    test_pass("cxx20:years");

    test_true( (STD::is_same<months::period, STD::ratio<2629746> >::value) );
    test_eq( (long long)duration_cast<days>(months(12)).count(), 365LL );
    test_pass("cxx20:months");
#else
    test_skip("cxx20:days");
    test_skip("cxx20:weeks");
    test_skip("cxx20:years");
    test_skip("cxx20:months");
#endif

#if _TST_HAS_VAR_TEMPLATE && TEST_TARGET_CXX >= 2017
    {
        const bool v = STD::chrono::treat_as_floating_point_v<double>;
        const bool c = STD::chrono::treat_as_floating_point<double>::value;
        const bool f = STD::chrono::treat_as_floating_point_v<int>;
        test_true( v == c );
        test_true( v && !f );
    }
    test_pass("cxx17:treat_as_floating_point_v");
#else
    test_skip("cxx17:treat_as_floating_point_v");
#endif
}
#else
TEST_CASE_SKIP(chrono, duration_member_aliases_and_ctors)
TEST_CASE_SKIP(chrono, duration_member_operators)
TEST_CASE_SKIP(chrono, duration_static_members)
TEST_CASE_SKIP(chrono, duration_nonmember_arithmetic)
TEST_CASE_SKIP(chrono, duration_comparisons)
TEST_CASE_SKIP(chrono, duration_cast_and_rounding)
TEST_CASE_SKIP(chrono, duration_traits_and_common_type)
TEST_CASE_SKIP(chrono, duration_standard_aliases)
TEST_CASE_SKIP(chrono, duration_io_and_format)
TEST_CASE_SKIP(chrono, chrono_literals)
TEST_CASE_SKIP(chrono, duration_compound_assignment)
TEST_CASE_SKIP(chrono, duration_alias_periods)
#endif
