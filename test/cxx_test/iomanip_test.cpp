#include "test_cxx.hpp"

#include <iomanip>
#include <sstream>
#include <string>
#if TEST_TARGET_CXX >= 2017
#include <string_view>
#endif
#include <ctime>
#include <cstring>

TEST_CASE(iomanip, width_fill_base_precision) {
    STD::ostringstream im;
    im << STD::setw(5) << STD::setfill('*') << 42;
    test_eq( im.str(), STD::string("***42") );
    test_pass("cxx03:setw");

    STD::ostringstream im2;
    im2 << STD::setbase(16) << 255 << ' ' << STD::setbase(8) << 9;
    test_eq( im2.str(), STD::string("ff 11") );
    test_pass("cxx03:setbase");

    STD::ostringstream im3;
    im3 << STD::setprecision(3) << STD::fixed << 3.14159;
    test_eq( im3.str(), STD::string("3.142") );
    test_pass("cxx03:setprecision");

    STD::ostringstream im4;
    im4 << STD::setiosflags(STD::ios_base::showpos) << 7
        << STD::resetiosflags(STD::ios_base::showpos) << 8;
    test_eq( im4.str(), STD::string("+78") );
    test_pass("cxx03:setiosflags");
}

TEST_CASE(iomanip, width_semantics) {

    STD::ostringstream a;
    a << STD::setw(4) << 1 << 2;
    test_eq( a.str(), STD::string("   12") );

    STD::ostringstream b;
    b << STD::setfill('0') << STD::setw(3) << 7 << STD::setw(3) << 8;
    test_eq( b.str(), STD::string("007008") );

    STD::ostringstream l;
    l << STD::left << STD::setfill('.') << STD::setw(5) << 42;
    test_eq( l.str(), STD::string("42...") );
    STD::ostringstream r;
    r << STD::right << STD::setfill('.') << STD::setw(5) << 42;
    test_eq( r.str(), STD::string("...42") );

    STD::ostringstream p;
    p << STD::setprecision(2) << 3.14159;
    test_eq( p.str(), STD::string("3.1") );

    STD::istringstream in("abcdefgh");
    STD::string s;
    in >> STD::setw(3) >> s;
    test_eq( s, STD::string("abc") );
    test_pass("cxx03:width sticky semantics");
}

TEST_CASE(iomanip, setfill_and_resetiosflags) {
    STD::ostringstream os;
    os << STD::setfill('*') << STD::setw(4) << 7;
    test_eq( os.str(), STD::string("***7") );
    test_pass("cxx03:setfill");

    STD::ostringstream r;
    r << STD::setiosflags(STD::ios_base::showpos) << 5
      << ' ' << STD::resetiosflags(STD::ios_base::showpos) << 6;
    test_eq( r.str(), STD::string("+5 6") );
    test_pass("cxx03:resetiosflags");
}

#if TEST_TARGET_CXX >= 2011
TEST_CASE(iomanip, get_time) {
    STD::tm tmv;
    STD::memset(&tmv, 0, sizeof(tmv));
    STD::istringstream is("2021-07-13");
    is >> STD::get_time(&tmv, "%Y-%m-%d");
    test_true( !is.fail() );
    test_eq( tmv.tm_year, 121 );
    test_eq( tmv.tm_mon, 6 );
    test_eq( tmv.tm_mday, 13 );
    test_pass("cxx11:get_time");
}

TEST_CASE(iomanip, money) {

    STD::ostringstream os;
    os << STD::put_money((long double)12345);
    test_pass("cxx11:put_money");

    STD::istringstream is(os.str());
    long double got = 0;
    is >> STD::get_money(got);
    TEST_SKIP_VC090("MSVC 9's money_get does not read back its own money_put");
    test_true( !is.fail() );
    TEST_SKIP_VC090("MSVC 9's money_get does not read back its own money_put");
    test_eq( (long)got, 12345L );
    test_pass("cxx11:get_money");
}

TEST_CASE(iomanip, put_time) {
    STD::tm tmv;
    STD::memset(&tmv, 0, sizeof(tmv));
    tmv.tm_year = 121; tmv.tm_mon = 6; tmv.tm_mday = 13;
    STD::ostringstream pt;
    pt << STD::put_time(&tmv, "%Y-%m-%d");
    test_eq( pt.str(), STD::string("2021-07-13") );
    test_pass("cxx11:put_time");
}

#if TEST_TARGET_CXX >= 2014
TEST_CASE(iomanip, quoted) {
    STD::ostringstream q;
    q << STD::quoted("a\"b\\c");
    test_eq( q.str(), STD::string("\"a\\\"b\\\\c\"") );

    STD::string qs("hi there");
    STD::ostringstream q2;
    q2 << STD::quoted(qs);
    test_eq( q2.str(), STD::string("\"hi there\"") );

    STD::istringstream qi("\"one two\" rest");
    STD::string got;
    qi >> STD::quoted(got);
    test_eq( got, STD::string("one two") );

    STD::istringstream qi2("\"esc\\\"q\"");
    STD::string got2;
    qi2 >> STD::quoted(got2);
    test_eq( got2, STD::string("esc\"q") );
    test_pass("cxx14:quoted");
}
#else
TEST_CASE(iomanip, quoted) {
    TEST_NOTE("std::quoted is C++14");
    test_skip("cxx14:quoted");
}
#endif
#endif

TEST_CASE(iomanip, feature_test_macros) {
#if defined(__cpp_lib_quoted_string_io)
    test_true( __cpp_lib_quoted_string_io > 0L );
    test_pass("cxx14:__cpp_lib_quoted_string_io");
#else
    test_skip("cxx14:__cpp_lib_quoted_string_io");
#endif
}

TEST_CASE(iomanip, quoted_forms_cxx14) {
#if defined(__cpp_lib_quoted_string_io)
    {   STD::ostringstream o;
        o << STD::quoted("say \"hi\"");
        test_eq( o.str(), STD::string("\"say \\\"hi\\\"\"") );
        STD::ostringstream o2;
        o2 << STD::quoted("plain");
        test_eq( o2.str(), STD::string("\"plain\"") );
        STD::ostringstream o3;
        o3 << STD::quoted("a|b", '|', '\\');
        test_true( o3.str().size() > 3 ); }
    test_pass("cxx14:quoted(const charT* s, charT delim, charT escape)");

    {   STD::istringstream i("\"round trip\" rest");
        STD::string got;
        i >> STD::quoted(got);
        test_eq( got, STD::string("round trip") );
        STD::string rest;
        i >> rest;
        test_eq( rest, STD::string("rest") );

        STD::istringstream i2("\"a\\\"b\"");
        STD::string got2;
        i2 >> STD::quoted(got2);
        test_eq( got2, STD::string("a\"b") ); }
    test_pass("cxx14:quoted(basic_string&, charT delim, charT escape) (extraction)");

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
    {   STD::string src("view me");
        STD::string_view sv(src);
        STD::ostringstream o;
        o << STD::quoted(sv);
        test_eq( o.str(), STD::string("\"view me\"") );
        STD::ostringstream o2;
        o2 << STD::quoted(STD::string_view(""));
        test_eq( o2.str(), STD::string("\"\"") ); }
    test_pass("cxx17:quoted(basic_string_view, charT delim, charT escape)");
#else
    TEST_NOTE("the string_view form of quoted is C++17");
    TEST_SKIP_N(2);
    test_skip("cxx17:quoted(basic_string_view, charT delim, charT escape)");
#endif
#else
    TEST_NOTE("quoted needs __cpp_lib_quoted_string_io");
    TEST_SKIP_N(8);
    test_skip("cxx14:quoted(const charT* s, charT delim, charT escape)");
    test_skip("cxx14:quoted(basic_string&, charT delim, charT escape) (extraction)");
    test_skip("cxx17:quoted(basic_string_view, charT delim, charT escape)");
#endif
}
