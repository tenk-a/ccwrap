#include "test_cxx.hpp"

#include <system_error>
#include <sstream>
#include <string>
#include <cstring>
#include <type_traits>
#include <cerrno>
#include <cstdio>
#include <ios>
#include <string>

#if defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800)
#  define _TST_ERRC_CONVERTS 0
#else
#  define _TST_ERRC_CONVERTS 1
#endif

TEST_CASE(system_error, errc_values) {

    STD::errc e = STD::errc();
    test_true( e == STD::errc() );
    test_true( !(STD::errc(STD::errc::io_error) == STD::errc()) );
    test_pass("cxx11:errc default-init");

    test_eq( int(STD::errc::invalid_argument), EINVAL );
    test_eq( int(STD::errc::result_out_of_range), ERANGE );
    test_eq( int(STD::errc::not_supported), ENOTSUP );
    test_eq( int(STD::errc::no_such_file_or_directory), ENOENT );
    test_eq( int(STD::errc::operation_not_permitted), EPERM );
    test_eq( int(STD::errc::no_such_process), ESRCH );
    test_eq( int(STD::errc::permission_denied), EACCES );
    test_eq( int(STD::errc::bad_address), EFAULT );
    test_eq( int(STD::errc::timed_out), ETIMEDOUT );
    test_eq( int(STD::errc::address_in_use), EADDRINUSE );
    test_eq( int(STD::errc::connection_reset), ECONNRESET );
    test_eq( int(STD::errc::value_too_large), EOVERFLOW );
    test_pass("cxx11:errc");

    test_true( int(STD::errc::invalid_argument) != int(STD::errc::no_such_file_or_directory) );
    test_true( int(STD::errc::operation_not_permitted) != int(STD::errc::no_such_file_or_directory) );
    test_true( int(STD::errc::address_in_use) != int(STD::errc::connection_reset) );
    test_pass("cxx11:errc enumerators distinct");

    TEST_SKIP_VC_RANGE(1600, 1800, "native is_error_condition_enum cannot be specialised for ccwrap's errc");
    test_true( STD::is_error_condition_enum<STD::errc>::value );
    test_pass("cxx11:is_error_condition_enum");
    test_true( !STD::is_error_code_enum<STD::errc>::value );
    test_pass("cxx11:is_error_code_enum");
}

TEST_CASE(system_error, error_code_basics) {
    STD::error_code ec;
    test_true( !ec );
    test_eq( ec.value(), 0 );
    test_pass("cxx11:error_code default ctor");

    ec = STD::make_error_code(STD::errc::invalid_argument);
    test_true( !!ec );
    test_pass("cxx11:make_error_code");
    test_eq( ec.value(), EINVAL );
    test_pass("cxx11:error_code::value");
    test_true( ec.category() == STD::generic_category() );
    test_pass("cxx11:generic_category");
    test_true( STD::string(ec.category().name()) == "generic" );
    test_pass("cxx11:error_category::name");
    test_true( !ec.message().empty() );
    test_pass("cxx11:error_code::message");

    STD::error_code ec2(EINVAL, STD::generic_category());
    test_true( ec == ec2 );
    test_true( !(ec != ec2) );
    test_pass("cxx11:error_code(value,category)");

    STD::error_code ec3(EINVAL, STD::system_category());
    test_true( ec != ec3 );
    test_pass("cxx11:system_category");

    STD::error_code lo(1, STD::generic_category());
    STD::error_code hi(2, STD::generic_category());
    test_true( lo < hi );
    test_true( !(hi < lo) );
#if TEST_TARGET_CXX >= 2020
    test_true( hi > lo );
    test_true( lo <= hi );  test_true( hi >= lo );
    test_true( lo <= lo );  test_true( lo >= lo );
    test_true( !(lo > hi) );
    test_pass("cxx11:error_code relational");
#else
    TEST_SKIP_N(6);
    test_skip("cxx11:error_code relational");
#endif

    STD::error_condition clo(1, STD::generic_category());
    STD::error_condition chi(2, STD::generic_category());
    test_true( clo < chi );
    test_true( !(chi < clo) );
#if TEST_TARGET_CXX >= 2020
    test_true( chi > clo );
    test_true( clo <= chi ); test_true( chi >= clo );
    test_pass("cxx11:error_condition relational");
#else
    TEST_SKIP_N(3);
    test_skip("cxx11:error_condition relational");
#endif

    ec.clear();
    test_true( !ec );
    test_pass("cxx11:error_code::clear");
}

TEST_CASE(system_error, error_condition_and_equivalent) {
    STD::error_code      ec   = STD::make_error_code(STD::errc::result_out_of_range);
    STD::error_condition cond = STD::make_error_condition(STD::errc::result_out_of_range);

    test_true( ec == cond );
    test_true( cond == ec );
    test_pass("cxx11:error_code == error_condition (equivalent)");
    test_true( ec.default_error_condition() == cond );
    test_pass("cxx11:error_code::default_error_condition");

    STD::error_condition other = STD::make_error_condition(STD::errc::io_error);
    test_true( ec != other );
    test_pass("cxx11:make_error_condition");

    test_true( !!cond );
    STD::error_condition empty_cond;
    test_true( !empty_cond );
    test_pass("cxx11:error_condition::operator bool");

    test_true( ec == STD::errc(STD::errc::result_out_of_range) );
    test_true( ec != STD::errc(STD::errc::io_error) );
    test_true( other == STD::errc(STD::errc::io_error) );
    test_pass("cxx11:errc comparison");
}

#if TEST_HAS_EH
TEST_CASE(system_error, system_error_exception) {
    bool threw = false;
    try {
        throw STD::system_error(STD::make_error_code(STD::errc::permission_denied), "opening");
    } catch (const STD::system_error& e) {
        threw = true;
        test_eq( e.code().value(), EACCES );
        test_true( e.code() == STD::make_error_code(STD::errc::permission_denied) );
        test_true( STD::string(e.what()).find("opening") != STD::string::npos );
    }
    test_true( threw );
    test_pass("cxx11:system_error(error_code)");

    threw = false;
    try { throw STD::system_error(22, STD::generic_category(), "bad"); }
    catch (const STD::exception&) { threw = true; }
    test_true( threw );
    test_pass("cxx11:system_error(int,category)");
}
#else   // !TEST_HAS_EH
TEST_CASE(system_error, system_error_exception) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:system_error(error_code)");
    test_skip("cxx11:system_error(int,category)");
}
#endif  // TEST_HAS_EH

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
typedef STD::io_errc::io_errc _tst_io_errc;
#else
typedef STD::io_errc _tst_io_errc;
#endif

#if TEST_HAS_EH
TEST_CASE(system_error, io_errc) {
    test_true( STD::is_error_code_enum<_tst_io_errc>::value );
    test_pass("cxx11:is_error_code_enum (io_errc)");

    STD::error_code ec = STD::make_error_code(_tst_io_errc(STD::io_errc::stream));
    test_eq( ec.value(), 1 );
    test_pass("cxx11:io_errc");
    test_true( ec.category() == STD::iostream_category() );
    test_true( STD::string(ec.category().name()) == "iostream" );
    test_pass("cxx11:iostream_category");
    test_true( ec != STD::make_error_code(STD::errc::operation_not_permitted) );
    test_pass("cxx11:error_code comparison (io_errc)");

#if _TST_HAS_CXX11_LIB_MEMBERS && !(defined(__GLIBCXX__) && __cplusplus < 201103L)
    bool threw = false;
    try {
        throw STD::ios_base::failure("boom");
    } catch (const STD::system_error& e) {
        threw = true;
        test_true( e.code() == STD::make_error_code(_tst_io_errc(STD::io_errc::stream)) );
        test_true( STD::string(e.what()).find("boom") != STD::string::npos );
    }
    test_true( threw );
    test_pass("cxx11:ios_base::failure");
#else
    TEST_SKIP_N(3);
    test_skip("cxx11:ios_base::failure");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(system_error, io_errc) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:is_error_code_enum (io_errc)");
    test_skip("cxx11:io_errc");
    test_skip("cxx11:iostream_category");
    test_skip("cxx11:error_code comparison (io_errc)");
    test_skip("cxx11:ios_base::failure");
}
#endif  // TEST_HAS_EH

TEST_CASE(system_error, error_category_members) {
    const STD::error_category& gen = STD::generic_category();
    const STD::error_category& sys = STD::system_category();

    test_true( !gen.message(EINVAL).empty() );
    test_pass("cxx11:error_category::message");

    STD::error_condition inval = STD::make_error_condition(STD::errc::invalid_argument);
    test_true( gen.equivalent(EINVAL, inval) );
    test_true( !gen.equivalent(ERANGE, inval) );
    test_pass("cxx11:error_category::equivalent(value)");

    STD::error_code ec = STD::make_error_code(STD::errc::invalid_argument);
    test_true( gen.equivalent(ec, EINVAL) );
    test_true( !gen.equivalent(ec, ERANGE) );
    test_pass("cxx11:error_category::equivalent(code)");

    STD::error_condition dec = gen.default_error_condition(EINVAL);
    test_eq( dec.value(), EINVAL );
    test_true( dec.category() == STD::generic_category() );
    test_pass("cxx11:error_category::default_error_condition");

    test_true( gen == STD::generic_category() );
    test_pass("cxx11:error_category::operator==");
    test_true( gen != sys );
    test_pass("cxx11:error_category::operator!=");

    test_true( (gen < sys) != (sys < gen) );
    test_pass("cxx11:error_category::operator<");

#if defined(__cpp_lib_three_way_comparison)

    test_true( (gen <=> gen) == 0 );
    bool a_lt = (gen <=> sys) < 0, b_lt = (sys <=> gen) < 0;
    test_true( a_lt != b_lt );
    test_pass("cxx20:error_category::operator<=>");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:error_category::operator<=>");
#endif
}

TEST_CASE(system_error, error_code_members) {

#if !defined(__WATCOMC__)
    STD::error_code ec = _tst_io_errc(STD::io_errc::stream);
    test_true( ec == STD::make_error_code(_tst_io_errc(STD::io_errc::stream)) );
    test_pass("cxx11:error_code(ErrorCodeEnum)");
#else
    STD::error_code ec = STD::make_error_code(_tst_io_errc(STD::io_errc::stream));
    TEST_NOTE("Open Watcom has no enum class, so io_errc / errc are structs; giving "
              "error_code an implicit conversion from them makes every comparison "
              "between two such values ambiguous (A11 / B4)");
    TEST_SKIP1();
    test_skip("cxx11:error_code(ErrorCodeEnum)");
#endif

    test_true( ec.category() == STD::iostream_category() );
    test_pass("cxx11:error_code::category");

    test_true( !!ec );
    STD::error_code z;
    test_true( !z );
    test_pass("cxx11:error_code::operator bool");

    ec.assign(EINVAL, STD::generic_category());
    test_eq( ec.value(), EINVAL );
    test_true( ec.category() == STD::generic_category() );
    test_pass("cxx11:error_code::assign");

#if !defined(__WATCOMC__)
    ec = _tst_io_errc(STD::io_errc::stream);
    test_true( ec.category() == STD::iostream_category() );
    test_eq( ec.value(), 1 );
    test_pass("cxx11:error_code::operator=(ErrorCodeEnum)");
#else
    TEST_NOTE("same as error_code(ErrorCodeEnum): the implicit conversion cannot be "
              "added without breaking io_errc / errc comparisons (A11 / B4)");
    TEST_SKIP_N(2);
    test_skip("cxx11:error_code::operator=(ErrorCodeEnum)");
#endif
}

TEST_CASE(system_error, error_condition_members) {

    STD::error_condition c;
    test_eq( c.value(), 0 );
    test_true( !c );
    test_pass("cxx11:error_condition default ctor");

    STD::error_condition c2(EINVAL, STD::generic_category());
    test_pass("cxx11:error_condition(value,category)");

    test_eq( c2.value(), EINVAL );
    test_pass("cxx11:error_condition::value");

    test_true( c2.category() == STD::generic_category() );
    test_pass("cxx11:error_condition::category");

#if _TST_ERRC_CONVERTS
    STD::error_condition c3 = STD::errc(STD::errc::io_error);
    test_true( c3 == STD::errc(STD::errc::io_error) );
    test_pass("cxx11:error_condition(ErrorConditionEnum)");
#else
    STD::error_condition c3 = STD::make_error_condition(STD::errc(STD::errc::io_error));
    TEST_NOTE("Open Watcom has no enum class, so errc is a struct; an implicit "
              "conversion to error_condition makes errc comparisons ambiguous (A11 / B4)");
    TEST_SKIP1();
    test_skip("cxx11:error_condition(ErrorConditionEnum)");
#endif

#if _TST_ERRC_CONVERTS
    c3 = STD::errc(STD::errc::invalid_argument);
    test_true( c3 == STD::errc(STD::errc::invalid_argument) );
    test_pass("cxx11:error_condition::operator=(ErrorConditionEnum)");
#else
    c3 = STD::make_error_condition(STD::errc(STD::errc::invalid_argument));
    TEST_NOTE("Open Watcom has no enum class, so errc is a struct; an implicit "
              "conversion to error_condition makes errc comparisons ambiguous (A11 / B4)");
    TEST_SKIP1();
    test_skip("cxx11:error_condition::operator=(ErrorConditionEnum)");
#endif

    c3.assign(ERANGE, STD::generic_category());
    test_eq( c3.value(), ERANGE );
    test_true( c3.category() == STD::generic_category() );
    test_pass("cxx11:error_condition::assign");

    test_true( !c3.message().empty() );
    test_pass("cxx11:error_condition::message");

    c3.clear();
    test_eq( c3.value(), 0 );
    test_true( c3.category() == STD::generic_category() );
    test_pass("cxx11:error_condition::clear");
}

TEST_CASE(system_error, system_error_members) {

    STD::error_code code = STD::make_error_code(STD::errc::permission_denied);
    STD::system_error e1(code, "ctx1");
    test_true( e1.code() == code );
    test_pass("cxx11:system_error(error_code,what)");

    test_eq( e1.code().value(), EACCES );
    test_pass("cxx11:system_error::code");

    test_true( STD::string(e1.what()).find("ctx1") != STD::string::npos );
    test_pass("cxx11:system_error::what");

    STD::system_error e2(EINVAL, STD::generic_category(), "ctx2");
    test_eq( e2.code().value(), EINVAL );
    test_true( e2.code().category() == STD::generic_category() );
    test_true( STD::string(e2.what()).find("ctx2") != STD::string::npos );
    test_pass("cxx11:system_error(int,category,what)");
}

TEST_CASE(system_error, io_errc_makers) {
    STD::error_code ec = STD::make_error_code(_tst_io_errc(STD::io_errc::stream));
    test_eq( ec.value(), 1 );
    test_true( ec.category() == STD::iostream_category() );
    test_pass("cxx11:make_error_code(io_errc)");

    STD::error_condition cond = STD::make_error_condition(_tst_io_errc(STD::io_errc::stream));
    test_eq( cond.value(), 1 );
    test_true( cond.category() == STD::iostream_category() );
    test_pass("cxx11:make_error_condition(io_errc)");
}

TEST_CASE(system_error, cxx17_traits_and_hash) {

#if defined(__cpp_variable_templates) && TEST_TARGET_CXX >= 2017
    test_true( STD::is_error_code_enum_v<_tst_io_errc> );
    test_true( !STD::is_error_code_enum_v<STD::errc> );
    test_pass("cxx17:is_error_code_enum_v");
    test_true( STD::is_error_condition_enum_v<STD::errc> );
    test_true( !STD::is_error_condition_enum_v<_tst_io_errc> );
    test_pass("cxx17:is_error_condition_enum_v");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx17:is_error_code_enum_v");
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx17:is_error_condition_enum_v");
#endif

#if 1
    STD::error_code a = STD::make_error_code(STD::errc::invalid_argument);
    STD::error_code b = STD::make_error_code(STD::errc::invalid_argument);
    STD::hash<STD::error_code> hc;
    test_true( hc(a) == hc(b) );
    test_pass("cxx17:hash<error_code>");

    STD::error_condition ca = STD::make_error_condition(STD::errc::invalid_argument);
    STD::error_condition cb = STD::make_error_condition(STD::errc::invalid_argument);
#if TEST_TARGET_CXX >= 2017
    STD::hash<STD::error_condition> hd;
    test_true( hd(ca) == hd(cb) );
    test_pass("cxx17:hash<error_condition>");
#else
    TEST_NOTE("hash<error_condition> is C++17");
    test_skip("cxx17:hash<error_condition>");
#endif
#else
    TEST_SKIP1(); test_skip("cxx17:hash<error_code>");
    TEST_SKIP1(); test_skip("cxx17:hash<error_condition>");
#endif
}

TEST_CASE(system_error, cxx20_spaceship) {
#if defined(__cpp_lib_three_way_comparison)
    STD::error_code lo(1, STD::generic_category());
    STD::error_code hi(2, STD::generic_category());
    test_true( (lo <=> hi) < 0 );
    test_true( (lo <=> lo) == 0 );
    test_pass("cxx20:operator<=>(error_code)");

    STD::error_condition clo(1, STD::generic_category());
    STD::error_condition chi(2, STD::generic_category());
    test_true( (clo <=> chi) < 0 );
    test_true( (clo <=> clo) == 0 );
    test_pass("cxx20:operator<=>(error_condition)");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:operator<=>(error_code)");
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:operator<=>(error_condition)");
#endif
}

TEST_CASE(system_error, errno_round_trip) {
    STD::FILE* f = STD::fopen("ccwnosf1.txt", "r");
    test_true( f == 0 );
    if (f) { STD::fclose(f); return; }

    const int e = errno;
    test_eq( e, ENOENT );

    const STD::error_code ec(e, STD::generic_category());
    test_true( ec == STD::errc(STD::errc::no_such_file_or_directory) );
    test_true( !(ec == STD::errc(STD::errc::operation_not_permitted)) );
    test_true( ec == STD::make_error_code(STD::errc::no_such_file_or_directory) );
    test_pass("cxx11:error_code maps errno to errc");
    test_true( !ec.message().empty() );
    test_pass("cxx11:error_code::message (round-trip)");

    const STD::error_condition cond = STD::generic_category().default_error_condition(e);
    test_eq( cond.value(), ENOENT );
    test_pass("cxx11:generic_category default_error_condition");
    test_true( cond == STD::errc(STD::errc::no_such_file_or_directory) );
    test_pass("cxx11:error_condition maps errno to errc");
}


#if TEST_TARGET_CXX >= 2011
#if TEST_HAS_EH
TEST_CASE(system_error, core_classes_cxx11) {
    const STD::error_category& g = STD::generic_category();
    const STD::error_category& s = STD::system_category();
    test_true( &g == &STD::generic_category() );
    test_true( g == STD::generic_category() );
    test_true( g != s );
    test_true( g.name() != 0 );
    TEST_SKIP_VC120("MSVC 12's is_copy_constructible does not see a deleted copy constructor (vc bugs A26)");
    test_true( !STD::is_copy_constructible<STD::error_category>::value );
    test_pass("cxx11:error_category");

    STD::error_code ec(int(STD::errc::invalid_argument), STD::generic_category());
    test_eq( ec.value(), (int)STD::errc::invalid_argument );
    test_true( ec.category() == STD::generic_category() );
    test_true( (bool)ec );
    test_true( !STD::error_code() );
    test_true( ec.message().size() > 0 );
    test_pass("cxx11:error_code");

    STD::error_condition cond = STD::make_error_condition(STD::errc::invalid_argument);
    test_true( ec == cond );
    test_true( cond.value() == (int)STD::errc::invalid_argument );
    test_true( cond.category() == STD::generic_category() );
    test_pass("cxx11:error_condition");

    bool caught = false;
    try {
        throw STD::system_error(ec, "while testing");
    } catch (const STD::system_error& e) {
        caught = true;
        test_true( e.code() == ec );
        test_true( STD::strstr(e.what(), "while testing") != 0 );
    }
    test_true( caught );
    test_pass("cxx11:system_error");

    STD::ostringstream os;
    os << ec;
    test_true( os.str().size() > 0 );
    test_true( os.str().find(':') != STD::string::npos );
    test_pass("cxx11:operator<<(error_code)");
}
#else   // !TEST_HAS_EH
TEST_CASE(system_error, core_classes_cxx11) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:error_category");
    test_skip("cxx11:error_code");
    test_skip("cxx11:error_condition");
    test_skip("cxx11:system_error");
    test_skip("cxx11:operator<<(error_code)");
}
#endif  // TEST_HAS_EH
#else
TEST_CASE(system_error, core_classes_cxx11) {
    test_skip("cxx11:error_category");
    test_skip("cxx11:error_code");
    test_skip("cxx11:error_condition");
    test_skip("cxx11:system_error");
    test_skip("cxx11:operator<<(error_code)");
}
#endif
