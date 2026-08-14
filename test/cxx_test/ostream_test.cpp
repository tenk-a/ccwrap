#include "test_cxx.hpp"

#include <ostream>
#include <sstream>
#include <string>
#include <ios>

TEST_CASE(ostream, formatted_numeric) {
    {
        STD::ostringstream os;
        os << 255;
        test_eq( os.str(), STD::string("255") );
        test_pass("cxx03:operator<<(int)");
    }
    {
        STD::ostringstream os;
        os << STD::hex << 255;
        test_eq( os.str(), STD::string("ff") );
        test_pass("cxx03:operator<<(int) hex");
    }
    {
        STD::ostringstream os;
        os << STD::oct << 8;
        test_eq( os.str(), STD::string("10") );
        test_pass("cxx03:operator<<(int) oct");
    }
    {
        STD::ostringstream os;
        os << STD::showpos << 5;
        test_eq( os.str(), STD::string("+5") );
        test_pass("cxx03:operator<<(int) showpos");
    }
    {
        STD::ostringstream os;
        os << STD::boolalpha << true << ' ' << false;
        test_eq( os.str(), STD::string("true false") );
        test_pass("cxx03:operator<<(bool)");
    }
    {
        STD::ostringstream os;
        os.width(5); os.fill('*');
        os << 42;
        test_eq( os.str(), STD::string("***42") );
        test_pass("cxx03:operator<<(int) width+fill");
    }
    {
        STD::ostringstream os;
        os << STD::left; os.width(5); os.fill('.');
        os << 42;
        test_eq( os.str(), STD::string("42...") );
        test_pass("cxx03:operator<<(int) left");
    }
}

TEST_CASE(ostream, float_and_pointer) {
    STD::ostringstream os;
    os.precision(2);
    os << STD::fixed << 3.14159;
    test_eq( os.str(), STD::string("3.14") );
    test_pass("cxx03:operator<<(double)");

    STD::ostringstream os2;
    int x = 0;
    os2 << (const void*)&x;
    test_true( os2.str().size() > 0 );
    test_pass("cxx03:operator<<(void*)");

#if TEST_TARGET_CXX >= 2023
    STD::ostringstream os3;
    os3 << (const volatile void*)&x;
    TEST_SKIP_WAT("the overload is omitted; it would make `os << (int*)p` ambiguous");
    test_eq( os3.str(), os2.str() );
    test_pass("cxx23:operator<<(const volatile void*)");
#else
    test_skip("cxx23:operator<<(const volatile void*)");
#endif
}

TEST_CASE(ostream, char_inserters) {
    {
        STD::ostringstream os;
        os << 'A' << 'b';
        test_eq( os.str(), STD::string("Ab") );
        test_pass("cxx03:operator<<(char)");
    }
    {
        STD::ostringstream os;
        signed char sc = 'S';
        os << sc;
        test_eq( os.str(), STD::string("S") );
        test_pass("cxx03:operator<<(signed char)");
        unsigned char uc = 'U';
        os << uc;
        test_eq( os.str(), STD::string("SU") );
        test_pass("cxx03:operator<<(unsigned char)");
    }
    {
        STD::ostringstream os;
        const unsigned char* us = (const unsigned char*)"hello";
        os << us;
        test_eq( os.str(), STD::string("hello") );
        test_pass("cxx03:operator<<(const unsigned char*)");
        const signed char*  ss = (const signed char*)"!";
        os << ss;
        test_eq( os.str(), STD::string("hello!") );
        test_pass("cxx03:operator<<(const signed char*)");
    }
    {
        STD::ostringstream os;
        os << "cstr" << STD::string(" and str");
        test_eq( os.str(), STD::string("cstr and str") );
        test_pass("cxx03:operator<<(const char*)");
    }
    {
        STD::ostringstream os;
        os.width(3); os.fill('-');
        os << 'x';
        os << 'y';
        test_eq( os.str(), STD::string("--xy") );
        test_pass("cxx03:operator<<(char) width");
    }
}

TEST_CASE(ostream, unformatted_put_write_flush) {
    STD::ostringstream os;
    os.put('H');
    test_eq( os.str(), STD::string("H") );
    test_pass("cxx03:put");
    os.write("ello", 4);
    test_eq( os.str(), STD::string("Hello") );
    test_pass("cxx03:write");
    os.flush();
    test_true( os.good() );
    test_true( &os.flush() == &os );
    test_pass("cxx03:basic_ostream::flush");
}

TEST_CASE(ostream, tellp_seekp) {
    STD::ostringstream os;
    os << "abcdef";
    STD::ostream::pos_type p = os.tellp();
    test_eq( (long)p, 6L );
    test_pass("cxx03:tellp");

    os.seekp(0);
    os << "XYZ";
    test_eq( os.str(), STD::string("XYZdef") );
    test_pass("cxx03:seekp");

    os.seekp(0, STD::ios_base::end);
    os << "!";
    test_eq( os.str(), STD::string("XYZdef!") );
    test_pass("cxx03:seekp(offset, dir)");
}

TEST_CASE(ostream, streambuf_inserter) {
    STD::istringstream in("piped through");
    STD::ostringstream os;
    os << in.rdbuf();
    test_eq( os.str(), STD::string("piped through") );
    test_pass("cxx03:operator<<(basic_streambuf*)");
}

TEST_CASE(ostream, sentry_and_state) {
    {
        STD::ostringstream os;
        STD::ostream::sentry s(os);
        test_true( (bool)s );
        test_pass("cxx03:sentry");
    }
    {
        STD::ostringstream os;
        os.setstate(STD::ios_base::failbit);
        STD::ostream::sentry s(os);
        test_true( !s );
        os << 123;
        test_eq( os.str(), STD::string("") );
        test_pass("cxx03:sentry (failed stream)");
    }
}

TEST_CASE(ostream, manipulators) {
    {
        STD::ostringstream os;
        os << "line" << STD::endl;
        test_eq( os.str(), STD::string("line\n") );
        test_pass("cxx03:endl");
    }
    {
        STD::ostringstream os;
        os << "a" << STD::flush << "b";
        test_eq( os.str(), STD::string("ab") );
        test_pass("cxx03:flush (manipulator)");
    }
    {
        STD::ostringstream os;
#if defined(_MSC_VER) || !_TST_HAS_SYNCSTREAM || TEST_TARGET_CXX < 2020
        (void)os;
        TEST_SKIP1();
#else
        os << STD::emit_on_flush << "x" << STD::noemit_on_flush << "y" << STD::flush_emit;
        test_eq( os.str(), STD::string("xy") );
#endif
        test_pass("cxx20:emit_on_flush");
    }
}

TEST_CASE(ostream, wostream_typedef) {
    test_true( sizeof(STD::wostream) > 0 );
    test_pass("cxx03:wostream");
}

TEST_CASE(ostream, rvalue_insertion) {
    STD::ostringstream os;
    STD::move(os) << "n=" << 12 << '!';
    test_eq( os.str(), STD::string("n=12!") );
    test_pass("cxx11:operator<<(rvalue stream)");
}

#include <iomanip>
TEST_CASE(ostream, streambuf_inserter_and_padding) {

    STD::istringstream src("payload");
    STD::ostringstream dst;
    dst << src.rdbuf();
    test_eq( dst.str(), STD::string("payload") );
    test_pass("cxx03:operator<<(basic_streambuf*) copy");

    STD::ostringstream nb;
    nb << (STD::streambuf*)0;
    test_true( nb.bad() );
    test_pass("cxx03:operator<<(basic_streambuf*) null->badbit");

    STD::istringstream empty("");
    STD::ostringstream es;
    es << empty.rdbuf();
    test_true( es.fail() );
    test_pass("cxx03:operator<<(basic_streambuf*) empty->failbit");

    STD::ostringstream r;
    r << STD::setw(5) << STD::setfill('*') << 'x';
    test_eq( r.str(), STD::string("****x") );
    test_pass("cxx03:operator<<(char) right-pad");
    STD::ostringstream l;
    l << STD::left << STD::setw(5) << STD::setfill('-') << "ab";
    test_eq( l.str(), STD::string("ab---") );
    test_pass("cxx03:operator<<(const char*) left-pad");
}

static STD::ios& ccw_set_showpos(STD::ios& s) { s.setf(STD::ios_base::showpos); return s; }

static STD::ostream& ccw_bang(STD::ostream& s) { return s << '!'; }

TEST_CASE(ostream, construct_from_streambuf) {
    STD::stringbuf sb;
    STD::ostream os(&sb);
    os << "direct";
    test_eq( sb.str(), STD::string("direct") );
    test_true( os.rdbuf() == &sb );
    test_pass("cxx03:basic_ostream(streambuf*)");
}

TEST_CASE(ostream, integral_inserters) {
    { STD::ostringstream os; os << (short)-7;              test_eq( os.str(), STD::string("-7") );
      test_pass("cxx03:operator<<(short)"); }
    { STD::ostringstream os; os << (unsigned short)7u;     test_eq( os.str(), STD::string("7") );
      test_pass("cxx03:operator<<(unsigned short)"); }
    { STD::ostringstream os; os << (unsigned int)42u;      test_eq( os.str(), STD::string("42") );
      test_pass("cxx03:operator<<(unsigned int)"); }
    { STD::ostringstream os; os << 100000L;                test_eq( os.str(), STD::string("100000") );
      test_pass("cxx03:operator<<(long)"); }
    { STD::ostringstream os; os << 4000000000UL;           test_eq( os.str(), STD::string("4000000000") );
      test_pass("cxx03:operator<<(unsigned long)"); }
    { STD::ostringstream os; os << 9000000000LL;           test_eq( os.str(), STD::string("9000000000") );
      test_pass("cxx11:operator<<(long long)"); }
    { STD::ostringstream os; os << 18000000000ULL;         test_eq( os.str(), STD::string("18000000000") );
      test_pass("cxx11:operator<<(unsigned long long)"); }
}

TEST_CASE(ostream, floating_inserters) {
    { STD::ostringstream os; os << 1.5f;   test_eq( os.str(), STD::string("1.5") );
      test_pass("cxx03:operator<<(float)"); }
    { STD::ostringstream os; os << 2.5L;   test_eq( os.str(), STD::string("2.5") );
      test_pass("cxx03:operator<<(long double)"); }
}

TEST_CASE(ostream, ends_and_manipulator_overloads) {
    {
        STD::ostringstream os;
        os << 'a' << STD::ends;
        STD::string s = os.str();
        test_eq( s.size(), (STD::string::size_type)2 );
        test_true( s[1] == '\0' );
        test_pass("cxx03:ends");
    }
    {
        STD::ostringstream os;
        STD::ostream& r = (os << STD::hex);
        test_true( &r == &os );
        os << 255;
        test_eq( os.str(), STD::string("ff") );
        test_pass("cxx03:operator<<(ios_base manipulator)");
    }
    {
        STD::ostringstream os;
        os << ccw_set_showpos << 5;
        test_eq( os.str(), STD::string("+5") );
        test_pass("cxx03:operator<<(basic_ios manipulator)");
    }
    {
        STD::ostringstream os;
        os << "x" << ccw_bang;
        test_eq( os.str(), STD::string("x!") );
        test_pass("cxx03:operator<<(basic_ostream manipulator)");
    }
}

TEST_CASE(ostream, nullptr_inserter) {
#if _TST_HAS_NULLPTR && TEST_TARGET_CXX >= 2017
    STD::ostringstream os;
    os << nullptr;
    test_true( !os.fail() );
    test_pass("cxx17:operator<<(nullptr_t)");
#else
    test_skip("cxx17:operator<<(nullptr_t)");
#endif
}

TEST_CASE(ostream, cxx20_sync_manipulators) {
    {
        STD::ostringstream os;
#if defined(_MSC_VER) || !_TST_HAS_SYNCSTREAM || TEST_TARGET_CXX < 2020
        (void)os; test_skip("cxx20:flush_emit");
#else
        os << "a" << STD::flush_emit;
        test_eq( os.str(), STD::string("a") );
        test_pass("cxx20:flush_emit");
#endif
    }
    {
        STD::ostringstream os;
#if defined(_MSC_VER) || !_TST_HAS_SYNCSTREAM || TEST_TARGET_CXX < 2020
        (void)os; test_skip("cxx20:noemit_on_flush");
#else
        os << STD::noemit_on_flush << "b" << STD::flush;
        test_eq( os.str(), STD::string("b") );
        test_pass("cxx20:noemit_on_flush");
#endif
    }
}

TEST_CASE(ostream, protected_move_swap) {
    test_skip("cxx11:basic_ostream(basic_ostream&&)");
    test_skip("cxx11:operator=(basic_ostream&&)");
    test_skip("cxx11:swap");
}


TEST_CASE(ostream, member_types_cxx03) {
    typedef STD::basic_ostream<char> OS;

    test_true(( STD::is_same<STD::ostream, OS>::value ));
    STD::ostringstream oss;
    STD::ostream& os = oss;
    os << 1;
    test_true( oss.str() == "1" );
    test_pass("cxx03:ostream");

    test_true(( STD::is_same<OS::char_type, char>::value ));
    test_true(( STD::is_same<STD::basic_ostream<wchar_t>::char_type, wchar_t>::value ));
    test_pass("cxx03:basic_ostream::char_type");

    test_true(( STD::is_same<OS::int_type, STD::char_traits<char>::int_type>::value ));
    test_pass("cxx03:basic_ostream::int_type");

    test_true(( STD::is_same<OS::pos_type, STD::char_traits<char>::pos_type>::value ));
    test_pass("cxx03:basic_ostream::pos_type");

    test_true(( STD::is_same<OS::off_type, STD::char_traits<char>::off_type>::value ));
    test_pass("cxx03:basic_ostream::off_type");

    test_true(( STD::is_same<OS::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx03:basic_ostream::traits_type");
}

#if _TST_HAS_PRINT
TEST_CASE(ostream, print_overloads_cxx23) {
    STD::ostringstream os;
    STD::print(os, "{}-{}", 1, 2);
    test_true( os.str() == "1-2" );
    test_pass("cxx23:print");

    STD::ostringstream os2;
    STD::println(os2, "{}", 3);
    test_true( os2.str() == "3\n" );
    test_pass("cxx23:println");

#if defined(__cpp_lib_print) && (__cpp_lib_print) >= 202403L
    STD::ostringstream os2b;
    STD::println(os2b);
    test_true( os2b.str() == "\n" );
    test_pass("cxx23:println(ostream&)");
#else
    TEST_NOTE("println(ostream&) needs __cpp_lib_print >= 202403L (P3142R0)");
    test_skip("cxx23:println(ostream&)");
#endif

    int v = 4;
    STD::ostringstream os3;
    STD::vprint_unicode(os3, "u{}", STD::make_format_args(v));
    test_true( os3.str() == "u4" );
    test_pass("cxx23:vprint_unicode");

    STD::ostringstream os4;
    STD::vprint_nonunicode(os4, "n{}", STD::make_format_args(v));
    test_true( os4.str() == "n4" );
    test_pass("cxx23:vprint_nonunicode");
}
#else
TEST_CASE(ostream, print_overloads_cxx23) {
    test_skip("cxx23:print");
    test_skip("cxx23:println");
    test_skip("cxx23:println(ostream&)");
    test_skip("cxx23:vprint_unicode");
    test_skip("cxx23:vprint_nonunicode");
}
#endif

TEST_CASE(ostream, feature_test_macros) {
#if defined(__cpp_lib_syncbuf) && (__cpp_lib_syncbuf) >= 201803L
    test_true( (__cpp_lib_syncbuf) >= 201803L );
    test_pass("cxx20:__cpp_lib_syncbuf=201803L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_syncbuf=201803L");
#endif
#if defined(__cpp_lib_print) && (__cpp_lib_print) >= 202207L
    test_true( (__cpp_lib_print) >= 202207L );
    test_pass("cxx23:__cpp_lib_print=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_print=202207L");
#endif
}
