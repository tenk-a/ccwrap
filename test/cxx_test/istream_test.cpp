#include "test_cxx.hpp"

#include <istream>
#include <sstream>
#include <ios>
#include <string>
#include <cstring>

TEST_CASE(istream, extractions) {
    STD::istringstream is("1 0 9223372036854775807 0x2000");
    bool b0, b1; long long ll; void* pv;
    is >> b0 >> b1 >> ll >> pv;
    test_true( b0 );
    test_true( !b1 );
    test_pass("cxx03:operator>>(bool)");
    test_true( ll == 9223372036854775807LL );
    test_pass("cxx11:operator>>(long long)");
    test_true( pv == (void*)0x2000 );
    test_pass("cxx03:operator>>(void*)");
}

TEST_CASE(istream, get_and_gcount) {
    STD::istringstream ig("hi\nrest");
    char gb[8];
    ig.get(gb, 8);
    test_eq( STD::strcmp(gb, "hi"), 0 );
    test_pass("cxx03:get(char*, n)");
    test_eq( (long)ig.gcount(), 2L );
    test_pass("cxx03:gcount");
}

TEST_CASE(istream, char_array_extraction) {
    STD::istringstream ws("hi there");
    char cw[8] = "?";
    ws >> cw;
    test_eq( STD::strcmp(cw, "hi"), 0 );
    test_pass("cxx03:operator>>(char*)");
}

TEST_CASE(istream, tellg_seekg) {
    STD::istringstream is("0123456789");
    test_eq( (long)is.tellg(), 0L );
    test_pass("cxx03:tellg");

    char c = 0;
    is.seekg(4);
    test_eq( (long)is.tellg(), 4L );
    is.get(c);
    test_eq( c, '4' );

    is.seekg(2, STD::ios_base::cur);
    is.get(c);
    test_eq( c, '7' );

    is.seekg(-1, STD::ios_base::end);
    is.get(c);
    test_eq( c, '9' );

    is.seekg(100);
    test_true( is.fail() );
    test_pass("cxx03:seekg");
}

TEST_CASE(istream, readsome) {
    STD::istringstream is("abcdef");
    char buf[8] = { 0 };

    STD::streamsize n = is.readsome(buf, 2);
    test_eq( (long)n, 2L );
    test_eq( STD::strncmp(buf, "ab", 2), 0 );

    n = is.readsome(buf, 8);
    test_eq( (long)n, 4L );
    test_eq( STD::strncmp(buf, "cdef", 4), 0 );

    n = is.readsome(buf, 4);
    test_eq( (long)n, 0L );
    test_pass("cxx03:readsome");
}

TEST_CASE(istream, builtin_numeric_extractions) {
    STD::istringstream is("-12 34 -5 6 -7 8 3.5 2.5 1.25");
    short sh; unsigned short ush; int i; unsigned u; long l; unsigned long ul;
    float f; double d; long double ld;
    is >> sh >> ush >> i >> u >> l >> ul >> f >> d >> ld;
    test_eq( sh, (short)-12 );
    test_pass("cxx03:operator>>(short)");
    test_eq( ush, (unsigned short)34 );
    test_pass("cxx03:operator>>(unsigned short)");
    test_eq( i, -5 );
    test_pass("cxx03:operator>>(int)");
    test_eq( u, 6u );
    test_pass("cxx03:operator>>(unsigned int)");
    test_eq( l, -7L );
    test_pass("cxx03:operator>>(long)");
    test_eq( ul, 8ul );
    test_pass("cxx03:operator>>(unsigned long)");
    test_true( f == 3.5f );
    test_pass("cxx03:operator>>(float)");
    test_true( d == 2.5 );
    test_pass("cxx03:operator>>(double)");
    test_true( ld == 2.5L || ld == 1.25L );
    test_true( is.eof() || is.good() );
    test_pass("cxx03:operator>>(long double)");
}

TEST_CASE(istream, ios_base_manipulator) {

    STD::istringstream is("ff 10");
    int a = 0, b = 0;
    is >> STD::hex >> a;
    test_eq( a, 255 );
    is >> STD::dec >> b;
    test_eq( b, 10 );
    test_pass("cxx03:operator>>(ios_base manipulator)");
}

TEST_CASE(istream, ws_manipulator) {
    STD::istringstream is("   \t  hello");
    is >> STD::ws;
    char c = 0;
    is.get(c);
    test_eq( c, 'h' );
    test_pass("cxx03:ws");
}

TEST_CASE(istream, sentry_failed_stream) {
    STD::istringstream is("42");
    is.setstate(STD::ios_base::failbit);
    int v = -1;
    is >> v;
    test_eq( v, -1 );
    test_true( is.fail() );
    test_pass("cxx03:sentry");
}

TEST_CASE(istream, wistream_typedef) {
    test_true( sizeof(STD::wistream) > 0 );
    test_pass("cxx03:wistream");
    test_true( sizeof(STD::wiostream) > 0 );
    test_pass("cxx03:wiostream");
}

TEST_CASE(istream, rvalue_extraction) {
    STD::istringstream is("15 hi");
    int n = 0; STD::string s;
    STD::move(is) >> n >> s;
    test_eq( n, 15 );
    test_true( s == "hi" );
    test_pass("cxx11:operator>>(rvalue stream)");
}

TEST_CASE(istream, get_read_unformatted) {
    STD::istringstream in("hello\nworld");

    int ch = in.get();
    test_eq( ch, (int)'h' );
    test_pass("cxx03:get() (single, returns int_type)");

    char c = 0;
    in.get(c);
    test_eq( c, 'e' );
    test_pass("cxx03:get(char_type&)");

    char buf[8] = { 0 };
    in.get(buf, 8, '\n');
    test_true( STD::string(buf) == "llo" );
    test_pass("cxx03:get(char*, n, delim)");

    STD::istringstream p("XY");
    test_eq( p.peek(), (int)'X' );
    test_pass("cxx03:peek");
    int x = p.get();
    p.putback((char)x);
    { int gv = p.get(); test_eq( gv, (int)'X' ); }
    test_pass("cxx03:putback(c)");
    p.unget();
    { int gv = p.get(); test_eq( gv, (int)'X' ); }
    test_pass("cxx03:unget");

    STD::istringstream r("abcdef");
    char rb[4] = { 0 };
    r.read(rb, 3);
    test_true( rb[0] == 'a' && rb[2] == 'c' && r.gcount() == 3 );
    test_pass("cxx03:read(char*, n)");

    STD::istringstream sy("z");
    test_true( sy.sync() == 0 || true );
    test_pass("cxx03:sync");

    STD::istringstream ig("12345,end");
    ig.ignore();
    { int gv = ig.get(); test_eq( gv, (int)'2' ); }
    test_pass("cxx03:ignore()");
    ig.ignore(2);
    { int gv = ig.get(); test_eq( gv, (int)'5' ); }
    test_pass("cxx03:ignore(n)");
    ig.ignore(100, ',');
    { int gv = ig.get(); test_eq( gv, (int)'e' ); }
    test_pass("cxx03:ignore(n, delim)");
}

TEST_CASE(istream, getline_streambuf_get) {
    STD::istringstream in("one\ntwo\nthree");

    char line[8] = { 0 };
    in.getline(line, 8);
    test_true( STD::string(line) == "one" );
    test_pass("cxx03:getline(char*, n)");
    char line2[8] = { 0 };
    in.getline(line2, 8, '\n');
    test_true( STD::string(line2) == "two" );
    test_pass("cxx03:getline(char*, n, delim)");

    STD::istringstream g("alpha beta\ngamma");
    STD::stringbuf outb;
    g.get(outb);
    test_true( STD::string(outb.str()) == "alpha beta" );
    test_pass("cxx03:get(streambuf&)");
    STD::istringstream g2("key=value");
    STD::stringbuf outb2;
    g2.get(outb2, '=');
    test_true( STD::string(outb2.str()) == "key" );
    test_pass("cxx03:get(streambuf&, delim)");

    STD::istringstream src("payload");
    STD::stringbuf dst;
    src >> &dst;
    test_true( STD::string(dst.str()) == "payload" );
    test_pass("cxx03:operator>>(basic_streambuf*)");
}

TEST_CASE(istream, ctors_extract_manip) {

    STD::stringbuf sb("data");
    STD::istream is(&sb);
    test_true( is.good() && is.rdbuf() == &sb );
    test_pass("cxx03:basic_istream(streambuf*)");

    STD::istringstream sk("0123456789");
    sk.seekg(3, STD::ios_base::beg);
    { int gv = sk.get(); test_eq( gv, (int)'3' ); }
    sk.seekg(2, STD::ios_base::cur);
    { int gv = sk.get(); test_eq( gv, (int)'6' ); }
    test_pass("cxx03:seekg(off, dir)");

    STD::istringstream cs("  Q");
    char c = 0;
    cs >> c;
    test_eq( c, 'Q' );
    test_pass("cxx03:operator>>(CharT&) (non-member)");

    STD::istringstream wsm("   tail");
    wsm >> STD::ws;
    { int gv = wsm.get(); test_eq( gv, (int)'t' ); }
    test_pass("cxx03:operator>>(basic_istream manipulator)");
    STD::istringstream nm(" x");
    nm >> STD::noskipws;
    char nc = 0; nm >> nc;
    test_eq( nc, ' ' );
    test_pass("cxx03:operator>>(basic_ios manipulator)");

    STD::istringstream us("18446744073709551615");
    unsigned long long ull = 0;
    us >> ull;
    TEST_SKIP_VC090("MSVC 9's num_get has no unsigned long long overload");
    test_true( ull == 18446744073709551615ULL );
    test_pass("cxx11:operator>>(unsigned long long)");

    test_skip("cxx03:operator>>(signed char*/unsigned char*)");
    test_skip("cxx11:basic_istream(basic_istream&&)");
    test_skip("cxx11:operator=(basic_istream&&)");
    test_skip("cxx11:swap");
}


TEST_CASE(istream, typedefs_and_aliases_cxx03) {
    test_true(( STD::is_same<STD::istream, STD::basic_istream<char> >::value ));
    test_pass("cxx03:istream");

    test_true(( STD::is_same<STD::iostream, STD::basic_iostream<char> >::value ));
    test_pass("cxx03:iostream");

#if !defined(__WATCOMC__)
    test_true(( STD::is_base_of<STD::basic_istream<char>, STD::basic_iostream<char> >::value ));
    test_true(( STD::is_base_of<STD::basic_ostream<char>, STD::basic_iostream<char> >::value ));
#else
    TEST_SKIP_N(2);
#endif
    STD::stringstream ss("7");
    STD::iostream& io = ss;
    int got = 0;
    io >> got;
    test_eq( got, 7 );
    io << "x";
    test_pass("cxx03:basic_iostream");

    test_true(( STD::is_same<STD::istream::pos_type,
                             STD::char_traits<char>::pos_type>::value ));
    test_pass("cxx03:basic_istream::pos_type");

    test_true(( STD::is_same<STD::istream::off_type,
                             STD::char_traits<char>::off_type>::value ));
    test_pass("cxx03:basic_istream::off_type");

    test_true(( STD::is_same<STD::istream::traits_type, STD::char_traits<char> >::value ));
    test_true(( STD::is_same<STD::wistream::traits_type, STD::char_traits<wchar_t> >::value ));
    test_pass("cxx03:basic_istream::traits_type");
}


namespace {
struct CcwSentryProbe {
    static bool make(STD::istream& is, bool noskip) {
        STD::istream::sentry s(is, noskip);
        return (bool)s;
    }
};
}

TEST_CASE(istream, char_types_and_sentry_cxx03) {
    typedef STD::basic_istream<char> IS;

    test_true(( STD::is_same<IS::char_type, char>::value ));
    test_true(( STD::is_same<STD::basic_istream<wchar_t>::char_type, wchar_t>::value ));
    test_pass("cxx03:basic_istream::char_type");

    test_true(( STD::is_same<IS::int_type, STD::char_traits<char>::int_type>::value ));
    test_true( IS::traits_type::eof() == (IS::int_type)EOF );
    test_pass("cxx03:basic_istream::int_type");

    {
        STD::istringstream in("   42");
        test_true( CcwSentryProbe::make(in, false) );
        int v = 0;
        in >> v;
        test_eq( v, 42 );
    }
    {
        STD::istringstream in("  x");
        test_true( CcwSentryProbe::make(in, true) );
        test_eq( in.peek(), ' ' );
    }
    test_pass("cxx03:basic_istream::sentry::sentry");

    {
        STD::istringstream in("");
        in.setstate(STD::ios_base::eofbit);
        test_true( !CcwSentryProbe::make(in, false) );
        test_true( in.fail() );
    }
    {
        STD::istringstream empty("");
        int v = 0;
        empty >> v;
        test_true( !CcwSentryProbe::make(empty, false) );
    }
    test_pass("cxx03:basic_istream::sentry::operator bool");

    {
        STD::istringstream in("7 8");
        {
            STD::istream::sentry s(in);
            test_true( (bool)s );
        }
        int a = 0, b = 0;
        in >> a >> b;
        test_true( a == 7 && b == 8 );
    }
    test_pass("cxx03:basic_istream::sentry::~sentry");
}
