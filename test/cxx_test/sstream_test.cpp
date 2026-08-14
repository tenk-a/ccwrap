#include "test_cxx.hpp"

#include <sstream>
#include <locale>
#include <vector>
#include <string>
#include <iterator>
#include <complex>
#include <cstring>
#include <type_traits>
#include <memory>
#if TEST_TARGET_CXX >= 2017
#include <string_view>
#endif

TEST_CASE(sstream, ostringstream) {
    STD::ostringstream os;
    os << "x=" << 42 << " y=" << 3.5 << ' ' << true;
    test_eq( os.str(), STD::string("x=42 y=3.5 1") );
    test_pass("cxx03:ostringstream::operator<<");
    os << STD::endl;
    test_eq( os.str(), STD::string("x=42 y=3.5 1\n") );
    test_pass("cxx03:ostringstream::str");
}

TEST_CASE(sstream, istringstream) {
    STD::istringstream is("10 20 hello 2.5");
    int a = 0, b = 0; STD::string w; double d = 0;
    is >> a >> b >> w >> d;
    test_eq( a, 10 );
    test_eq( b, 20 );
    test_eq( w, STD::string("hello") );
    test_eq( d, 2.5 );
    test_true( !is.fail() );
    test_pass("cxx03:istringstream::operator>>");

    STD::istringstream nums("1 2 3 4");
    int n, sum = 0;
    while (nums >> n) sum += n;
    test_eq( sum, 10 );
    test_pass("cxx03:istringstream");
}

TEST_CASE(sstream, stringstream_roundtrip) {
    STD::stringstream ss;
    ss << 7 << ' ' << 8;
    int p = 0, q = 0;
    ss >> p >> q;
    test_eq( p, 7 );
    test_eq( q, 8 );
    test_pass("cxx03:stringstream");
}

TEST_CASE(sstream, streambuf_iterators) {
    STD::istringstream slurp("the quick brown fox");
    STD::string whole((STD::istreambuf_iterator<char>(slurp)), STD::istreambuf_iterator<char>());
    test_eq( whole, STD::string("the quick brown fox") );
    test_pass("cxx03:istreambuf_iterator");

    STD::ostringstream osb;
    STD::ostreambuf_iterator<char> obi(osb);
    STD::string src = "copied";
    for (STD::string::iterator it = src.begin(); it != src.end(); ++it) { *obi = *it; ++obi; }
    test_eq( osb.str(), STD::string("copied") );
    test_pass("cxx03:ostreambuf_iterator");
    test_true( !obi.failed() );
    test_pass("cxx03:ostreambuf_iterator::failed");
}

TEST_CASE(sstream, integer_and_float_output) {
    STD::ostringstream fs;
    fs << STD::hex << 255 << ' ' << STD::showpos << STD::dec << 5;
    test_eq( fs.str(), STD::string("ff +5") );
    test_pass("cxx03:operator<<(int) (hex/showpos/dec)");

    STD::ostringstream ll;  ll  << 9223372036854775807LL;
    test_eq( ll.str(), STD::string("9223372036854775807") );
    test_pass("cxx11:operator<<(long long)");
    STD::ostringstream ull; ull << 18446744073709551615ULL;
    test_eq( ull.str(), STD::string("18446744073709551615") );
    test_pass("cxx11:operator<<(unsigned long long)");
    STD::ostringstream lh;  lh  << STD::hex << (unsigned long long)255;
    test_eq( lh.str(), STD::string("ff") );
    test_pass("cxx11:operator<<(unsigned long long) (hex)");
    STD::ostringstream ld;  ld  << 2.5L;
    test_eq( ld.str(), STD::string("2.5") );
    test_pass("cxx03:operator<<(long double)");
}

TEST_CASE(sstream, complex_io) {
    STD::ostringstream cos;
    cos << STD::complex<double>(1.0, 2.0);
    test_eq( cos.str(), STD::string("(1,2)") );
    test_pass("cxx03:operator<<(complex)");

    STD::istringstream cis("(3,4)");
    STD::complex<double> cz;
    cis >> cz;
    test_true( cz == STD::complex<double>(3.0, 4.0) );
    test_pass("cxx03:operator>>(complex)");
}

TEST_CASE(sstream, seek_put) {
    STD::ostringstream os;
    os << "hello world";
    test_eq( (long)os.tellp(), 11L );
    test_pass("cxx03:ostringstream::tellp");

    os.seekp(0);
    test_eq( (long)os.tellp(), 0L );
    os << "HELLO";
    test_eq( os.str(), STD::string("HELLO world") );
    test_eq( (long)os.tellp(), 5L );
    test_pass("cxx03:ostringstream::seekp");

    os.seekp(0, STD::ios_base::end);
    test_eq( (long)os.tellp(), 11L );
    os << "!";
    test_eq( os.str(), STD::string("HELLO world!") );

    os.seekp(-2, STD::ios_base::cur);
    test_eq( (long)os.tellp(), 10L );
    test_pass("cxx03:ostringstream::seekp (relative)");
}

TEST_CASE(sstream, seek_get) {
    STD::istringstream is("abcdef");
    test_eq( (long)is.tellg(), 0L );
    test_pass("cxx03:istringstream::tellg");

    char c = 0;
    is.get(c);
    test_eq( c, 'a' );
    test_eq( (long)is.tellg(), 1L );

    is.seekg(3);
    test_eq( (long)is.tellg(), 3L );
    is.get(c);
    test_eq( c, 'd' );

    is.seekg(0, STD::ios_base::end);
    test_eq( (long)is.tellg(), 6L );
    is.seekg(-2, STD::ios_base::cur);
    test_eq( (long)is.tellg(), 4L );
    is.get(c);
    test_eq( c, 'e' );
    test_pass("cxx03:istringstream::seekg");

    STD::string rest;
    while (is >> rest) {}
    test_true( is.eof() );
    test_true( is.fail() );

    is.seekg(0);
    TEST_SKIP_VC090("MSVC 9's seekg clears the failure state");
    test_true( is.fail() );

    is.clear();
    is.seekg(0);
    test_true( !is.eof() && !is.fail() );
    is.get(c);
    test_eq( c, 'a' );
    test_pass("cxx03:istringstream::seekg (failbit no-op)");
}

TEST_CASE(sstream, seek_out_of_range_fails) {
    STD::istringstream is("abc");
    is.seekg(99);
    test_true( is.fail() );
    test_pass("cxx03:seekg (out of range)");

    STD::ostringstream os;
    os << "abc";
    os.seekp(-1, STD::ios_base::beg);
    test_true( os.fail() );
    test_pass("cxx03:seekp (out of range)");
}

TEST_CASE(sstream, readsome_and_sync) {
    STD::istringstream is("abcdef");
    char buf[4] = { 0 };
    STD::streamsize n = is.readsome(buf, 3);
    test_eq( (long)n, 3L );
    test_eq( (long)is.gcount(), 3L );
    test_true( STD::strncmp(buf, "abc", 3) == 0 );
    test_true( !is.eof() );
    test_pass("cxx03:istringstream::readsome");

    test_eq( is.sync(), 0 );
    test_pass("cxx03:istringstream::sync");
}

#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(sstream, ostringstream_move_ctor) {
    STD::ostringstream a;
    a << "hello" << 42;
    STD::ostringstream b(STD::move(a));
    test_true( b.str() == "hello42" );
    b << '!';
    test_true( b.str() == "hello42!" );
    test_pass("cxx11:ostringstream(ostringstream&&)");
}
#else
TEST_CASE_SKIP(sstream, ostringstream_move_ctor)
#endif


#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(sstream, ostringstream_move_assign) {
    STD::ostringstream a; a << "src";
    STD::ostringstream b; b << "dst";
    b = STD::move(a);
    test_true( b.str() == "src" );
    test_pass("cxx11:ostringstream::operator=(ostringstream&&)");
}
#else
TEST_CASE_SKIP(sstream, ostringstream_move_assign)
#endif

#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(sstream, istringstream_move_then_read) {
    STD::istringstream a("7 8 9");
    int first = 0; a >> first;
    test_eq( first, 7 );
    STD::istringstream b(STD::move(a));
    int x = 0, y = 0; b >> x >> y;
    test_eq( x, 8 );
    test_eq( y, 9 );
    test_pass("cxx11:istringstream(istringstream&&)");
}
#else
TEST_CASE_SKIP(sstream, istringstream_move_then_read)
#endif


#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(sstream, stringstream_move_roundtrip) {
    STD::stringstream s; s << "abc " << 5;
    STD::stringstream t(STD::move(s));
    STD::string w; int n = 0;
    t >> w >> n;
    test_true( w == "abc" );
    test_eq( n, 5 );
    test_pass("cxx11:stringstream(stringstream&&)");
}
#else
TEST_CASE_SKIP(sstream, stringstream_move_roundtrip)
#endif


#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(sstream, stringstream_swap) {
    STD::ostringstream p; p << "PP";
    STD::ostringstream q; q << "QQ";
    p.swap(q);
    test_true( p.str() == "QQ" );
    test_true( q.str() == "PP" );
    p << 'x';
    test_true( p.str() == "QQx" );
    test_pass("cxx11:ostringstream::swap");
}
#else
TEST_CASE_SKIP(sstream, stringstream_swap)
#endif


TEST_CASE(sstream, wide_typedefs) {
    STD::wstringstream ss;
    ss << L"hello";
    test_true( ss.str() == L"hello" );
    test_eq( ss.str().size(), 5u );
    test_pass("cxx03:wstringstream");

    STD::wostringstream os;
    os << L"abc" << L"def";
    test_true( os.str() == L"abcdef" );
    test_pass("cxx03:wostringstream");

    STD::wistringstream is(L"seed");
    test_true( is.str() == L"seed" );
    test_pass("cxx03:wistringstream");

    STD::wstringbuf sb(L"buf");
    test_true( sb.str() == L"buf" );
    test_pass("cxx03:wstringbuf");
}

TEST_CASE(sstream, stringbuf_basic) {

    STD::stringbuf sb;
    test_true( sb.str().empty() );
    test_pass("cxx03:basic_stringbuf");

    STD::stringbuf sb2("seed");
    test_eq( sb2.str(), STD::string("seed") );
    test_pass("cxx03:stringbuf::str (get)");

    sb2.str("replaced");
    test_eq( sb2.str(), STD::string("replaced") );
    test_pass("cxx03:stringbuf::str (set)");
}

TEST_CASE(sstream, rdbuf_and_str_observers) {
    STD::ostringstream os;
    os << "abc";
    test_true( os.rdbuf() != 0 );
    test_eq( os.rdbuf()->str(), STD::string("abc") );
    test_pass("cxx03:ostringstream::rdbuf");
    test_eq( os.str(), STD::string("abc") );
    test_pass("cxx03:ostringstream");

    STD::istringstream is("payload");
    test_true( is.rdbuf() != 0 );
    test_pass("cxx03:istringstream::rdbuf");
    test_eq( is.str(), STD::string("payload") );
    test_pass("cxx03:istringstream::str");

    STD::istringstream is2("data", STD::ios_base::in);
    test_eq( is2.str(), STD::string("data") );
    test_pass("cxx03:istringstream(string, mode)");

    STD::stringstream ss;
    ss << "rw";
    test_true( ss.rdbuf() != 0 );
    test_pass("cxx03:stringstream::rdbuf");
    test_eq( ss.str(), STD::string("rw") );
    test_pass("cxx03:stringstream::str");
}

#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(sstream, move_assign_and_swap) {
    {
        STD::stringstream a; a << "aa";
        STD::stringstream b;
        b = STD::move(a);
        test_eq( b.str(), STD::string("aa") );
        test_pass("cxx11:stringstream::operator=(stringstream&&)");
    }
    {
        STD::istringstream a("src");
        STD::istringstream b;
        b = STD::move(a);
        test_eq( b.str(), STD::string("src") );
        test_pass("cxx11:istringstream::operator=(istringstream&&)");
    }
    {
        STD::stringstream a; a << "one";
        STD::stringstream b; b << "two";
        a.swap(b);
        test_eq( a.str(), STD::string("two") );
        test_eq( b.str(), STD::string("one") );
        test_pass("cxx11:stringstream::swap");
    }
    {
        STD::istringstream a("first");
        STD::istringstream b("second");
        a.swap(b);
        test_eq( a.str(), STD::string("second") );
        test_eq( b.str(), STD::string("first") );
        test_pass("cxx11:istringstream::swap");
    }
    {
        STD::stringbuf a("bufA");
        STD::stringbuf b("bufB");
        a.swap(b);
        test_eq( a.str(), STD::string("bufB") );
        test_eq( b.str(), STD::string("bufA") );
        test_pass("cxx11:stringbuf::swap");
    }
}
#else
TEST_CASE_SKIP(sstream, move_assign_and_swap)
#endif


TEST_CASE(sstream, cxx20_view_and_allocator) {
#if !defined(__WATCOMC__) && TEST_TARGET_CXX >= 2020 && _TST_HAS_CXX20_LIB_MEMBERS
    {
        STD::stringbuf sb("viewme");
        test_true( sb.view() == STD::string_view("viewme") );
        test_pass("cxx20:stringbuf::view");
    }
    {
        STD::ostringstream os; os << "outv";
        test_true( os.view() == STD::string_view("outv") );
        test_pass("cxx20:ostringstream::view");
    }
    {
        STD::istringstream is("inv");
        test_true( is.view() == STD::string_view("inv") );
        test_pass("cxx20:istringstream::view");
    }
    {
        STD::stringstream ss; ss << "rwv";
        test_true( ss.view() == STD::string_view("rwv") );
        test_pass("cxx20:stringstream::view");
    }
#else
    test_skip("cxx20:stringbuf::view");
    test_skip("cxx20:ostringstream::view");
    test_skip("cxx20:istringstream::view");
    test_skip("cxx20:stringstream::view");
#endif

#if !defined(__WATCOMC__) && TEST_TARGET_CXX >= 2020 && _TST_HAS_CXX20_LIB_MEMBERS
    {
        STD::stringbuf sb("g");
        STD::allocator<char> a = sb.get_allocator();
        (void)a;
        test_true( true );
        test_pass("cxx20:stringbuf::get_allocator");
    }
#else
    test_skip("cxx20:stringbuf::get_allocator");
#endif
}

TEST_CASE(sstream, feature_test_macros) {
#if defined(__cpp_lib_sstream_from_string_view) && (__cpp_lib_sstream_from_string_view) >= 202306L
    test_true( (__cpp_lib_sstream_from_string_view) >= 202306L );
    test_pass("cxx26:__cpp_lib_sstream_from_string_view=202306L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_sstream_from_string_view=202306L");
#endif
}

TEST_CASE(sstream, member_types_cxx03) {
    typedef STD::char_traits<char> Tr;

    typedef STD::basic_stringbuf<char> SB;
    test_true(( STD::is_same<SB::char_type, char>::value ));
    test_pass("cxx03:basic_stringbuf::char_type");
    test_true(( STD::is_same<SB::traits_type, Tr>::value ));
    test_pass("cxx03:basic_stringbuf::traits_type");
    test_true(( STD::is_same<SB::int_type, Tr::int_type>::value ));
    test_true( SB::traits_type::eq_int_type(SB::traits_type::eof(), Tr::eof()) );
    test_pass("cxx03:basic_stringbuf::int_type");
    test_true(( STD::is_same<SB::pos_type, Tr::pos_type>::value ));
    test_pass("cxx03:basic_stringbuf::pos_type");
    test_true(( STD::is_same<SB::off_type, Tr::off_type>::value ));
    test_true( STD::is_signed<STD::streamoff>::value );
    test_pass("cxx03:basic_stringbuf::off_type");
    test_true(( STD::is_same<SB::allocator_type, STD::allocator<char> >::value ));
    test_pass("cxx03:basic_stringbuf::allocator_type");

    typedef STD::basic_istringstream<char> IS;
    STD::istringstream is_obj("x");
    test_true(( STD::is_same<IS, STD::istringstream>::value ));
    test_true( is_obj.rdbuf() != 0 );
    test_pass("cxx03:basic_istringstream");
    test_true(( STD::is_same<IS::char_type, char>::value ));
    test_pass("cxx03:basic_istringstream::char_type");
    test_true(( STD::is_same<IS::traits_type, Tr>::value ));
    test_pass("cxx03:basic_istringstream::traits_type");
    test_true(( STD::is_same<IS::int_type, Tr::int_type>::value ));
    test_pass("cxx03:basic_istringstream::int_type");
    test_true(( STD::is_same<IS::pos_type, Tr::pos_type>::value ));
    test_pass("cxx03:basic_istringstream::pos_type");
    test_true(( STD::is_same<IS::off_type, Tr::off_type>::value ));
    test_pass("cxx03:basic_istringstream::off_type");
    test_true(( STD::is_same<IS::allocator_type, STD::allocator<char> >::value ));
    test_pass("cxx03:basic_istringstream::allocator_type");

    typedef STD::basic_ostringstream<char> OS;
    STD::ostringstream os_obj;
    test_true(( STD::is_same<OS, STD::ostringstream>::value ));
    test_true( os_obj.rdbuf() != 0 );
    test_pass("cxx03:basic_ostringstream");
    test_true(( STD::is_same<OS::char_type, char>::value ));
    test_pass("cxx03:basic_ostringstream::char_type");
    test_true(( STD::is_same<OS::traits_type, Tr>::value ));
    test_pass("cxx03:basic_ostringstream::traits_type");
    test_true(( STD::is_same<OS::int_type, Tr::int_type>::value ));
    test_pass("cxx03:basic_ostringstream::int_type");
    test_true(( STD::is_same<OS::pos_type, Tr::pos_type>::value ));
    test_pass("cxx03:basic_ostringstream::pos_type");
    test_true(( STD::is_same<OS::off_type, Tr::off_type>::value ));
    test_pass("cxx03:basic_ostringstream::off_type");
    test_true(( STD::is_same<OS::allocator_type, STD::allocator<char> >::value ));
    test_pass("cxx03:basic_ostringstream::allocator_type");

    typedef STD::basic_stringstream<char> SS;
    STD::stringstream ss_obj;
    test_true(( STD::is_same<SS, STD::stringstream>::value ));
    ss_obj << 12;
    int back = 0;
    ss_obj >> back;
    test_eq( back, 12 );
    test_pass("cxx03:basic_stringstream");
    test_true(( STD::is_same<SS::char_type, char>::value ));
    test_pass("cxx03:basic_stringstream::char_type");
    test_true(( STD::is_same<SS::traits_type, Tr>::value ));
    test_pass("cxx03:basic_stringstream::traits_type");
    test_true(( STD::is_same<SS::int_type, Tr::int_type>::value ));
    test_pass("cxx03:basic_stringstream::int_type");
    test_true(( STD::is_same<SS::pos_type, Tr::pos_type>::value ));
    test_pass("cxx03:basic_stringstream::pos_type");
    test_true(( STD::is_same<SS::off_type, Tr::off_type>::value ));
    test_pass("cxx03:basic_stringstream::off_type");
    test_true(( STD::is_same<SS::allocator_type, STD::allocator<char> >::value ));
    test_pass("cxx03:basic_stringstream::allocator_type");
}

namespace {
struct CcwProbeBuf : public STD::stringbuf {
    CcwProbeBuf() {}
    CcwProbeBuf(const STD::string& s, STD::ios_base::openmode m) : STD::stringbuf(s, m) {}
    int_type call_underflow()           { return this->underflow(); }
    int_type call_overflow(int_type c)  { return this->overflow(c); }
    int_type call_pbackfail(int_type c) { return this->pbackfail(c); }
    pos_type call_seekoff(off_type o, STD::ios_base::seekdir d, STD::ios_base::openmode m)
                                        { return this->seekoff(o, d, m); }
    pos_type call_seekpos(pos_type p, STD::ios_base::openmode m)
                                        { return this->seekpos(p, m); }
    STD::streambuf* call_setbuf(char* s, STD::streamsize n) { return this->setbuf(s, n); }
};
}

TEST_CASE(sstream, stringbuf_virtuals_cxx03) {
    typedef STD::stringbuf::traits_type Tr;

    {
        CcwProbeBuf b(STD::string("ab"), STD::ios_base::in);
        STD::stringbuf::int_type c = b.call_underflow();
        test_true( Tr::eq_int_type(c, Tr::to_int_type('a')) );
        test_eq( (int)b.sgetc(), (int)'a' );
        int taken = (int)b.sbumpc();
        test_eq( taken, (int)'a' );
        CcwProbeBuf empty(STD::string(""), STD::ios_base::in);
        test_true( Tr::eq_int_type(empty.call_underflow(), Tr::eof()) );
        test_pass("cxx03:basic_stringbuf::underflow");
    }
    {
        CcwProbeBuf b(STD::string(""), STD::ios_base::out);
        STD::stringbuf::int_type r = b.call_overflow(Tr::to_int_type('z'));
        test_true( !Tr::eq_int_type(r, Tr::eof()) );
        test_eq( b.str(), STD::string("z") );
        test_true( !Tr::eq_int_type(b.call_overflow(Tr::eof()), Tr::eof()) );
        test_pass("cxx03:basic_stringbuf::overflow");
    }
    {
        CcwProbeBuf b(STD::string("ab"), STD::ios_base::in | STD::ios_base::out);
        int taken = (int)b.sbumpc();
        test_eq( taken, (int)'a' );
        STD::stringbuf::int_type r = b.call_pbackfail(Tr::to_int_type('a'));
        test_true( Tr::eq_int_type(r, Tr::to_int_type('a')) || Tr::eq_int_type(r, Tr::eof()) );
        CcwProbeBuf fresh(STD::string("ab"), STD::ios_base::in);
        test_true( Tr::eq_int_type(fresh.call_pbackfail(Tr::to_int_type('x')), Tr::eof()) );
        test_pass("cxx03:basic_stringbuf::pbackfail");
    }
    {
        CcwProbeBuf b(STD::string("abcdef"), STD::ios_base::in);
        STD::stringbuf::pos_type p = b.call_seekoff(2, STD::ios_base::beg, STD::ios_base::in);
        test_eq( (long)(STD::streamoff)p, 2L );
        test_eq( (int)b.sgetc(), (int)'c' );
        STD::stringbuf::pos_type e = b.call_seekoff(0, STD::ios_base::end, STD::ios_base::in);
        test_eq( (long)(STD::streamoff)e, 6L );
        STD::stringbuf::pos_type bad =
            b.call_seekoff(100, STD::ios_base::beg, STD::ios_base::in);
        test_eq( (long)(STD::streamoff)bad, -1L );
        test_pass("cxx03:basic_stringbuf::seekoff");
    }
    {
        CcwProbeBuf b(STD::string("abcdef"), STD::ios_base::in);
        STD::stringbuf::pos_type p = b.call_seekpos(STD::stringbuf::pos_type(3),
                                                    STD::ios_base::in);
        test_eq( (long)(STD::streamoff)p, 3L );
        test_eq( (int)b.sgetc(), (int)'d' );
        test_pass("cxx03:basic_stringbuf::seekpos");
    }
    {
        CcwProbeBuf b(STD::string("abc"), STD::ios_base::in);
        STD::streambuf* none = b.call_setbuf(0, 0);
        test_true( none == &b );
        test_eq( (int)b.sgetc(), (int)'a' );
        char scratch[8];
        STD::streambuf* r = b.call_setbuf(scratch, 8);
        test_true( r == &b );
        test_pass("cxx03:basic_stringbuf::setbuf");
    }
}

TEST_CASE(sstream, str_from_string_view_cxx26) {
#if defined(__cpp_lib_sstream_from_string_view) && (__cpp_lib_sstream_from_string_view) >= 202306L
    STD::string_view sv("hello");

    STD::stringbuf sb;
    sb.str(sv);
    test_eq( sb.str(), STD::string("hello") );
    test_pass("cxx26:basic_stringbuf::str(string_view)");

    STD::istringstream is;
    is.str(sv);
    STD::string word;
    is >> word;
    test_eq( word, STD::string("hello") );
    test_pass("cxx26:basic_istringstream::str(string_view)");

    STD::ostringstream os;
    os.str(sv);
    test_eq( os.str(), STD::string("hello") );
    test_pass("cxx26:basic_ostringstream::str(string_view)");

    STD::stringstream ss;
    ss.str(sv);
    test_eq( ss.str(), STD::string("hello") );
    test_pass("cxx26:basic_stringstream::str(string_view)");
#else
    TEST_NOTE("str(string_view) needs __cpp_lib_sstream_from_string_view >= 202306L");
    test_skip("cxx26:basic_stringbuf::str(string_view)");
    test_skip("cxx26:basic_istringstream::str(string_view)");
    test_skip("cxx26:basic_ostringstream::str(string_view)");
    test_skip("cxx26:basic_stringstream::str(string_view)");
#endif

}

TEST_CASE(sstream, reuse_and_locale_lifetime) {
    STD::ostringstream os;
    for (int i = 0; i < 50; ++i) {
        os.str(STD::string());
        os.clear();
        os << "row" << i << ':' << (i * 1.5) << ' ' << STD::string("tail_long_enough_to_leave_sso");
        test_true( os.str().size() > 20 );
    }
    test_true( os.str().find("row49:") == 0 );

    STD::istringstream is;
    int sum = 0;
    for (int i = 0; i < 50; ++i) {
        STD::string src = "12 34 ";
        src += STD::string("ignored_long_enough_to_leave_sso");
        is.str(src);
        is.clear();
        int a = 0, b = 0;
        is >> a >> b;
        test_true( !is.fail() );
        sum += a + b;
    }
    test_eq( sum, 50 * 46 );

    STD::stringstream ss;
    for (int i = 0; i < 30; ++i) {
        ss.str(STD::string());
        ss.clear();
        ss << i << ' ' << STD::string("v_long_enough_to_leave_sso");
        int got = -1;
        STD::string word;
        ss >> got >> word;
        test_eq( got, i );
        test_true( word.size() > 20 );
    }

    {
        STD::locale keep = os.getloc();
        for (int i = 0; i < 20; ++i) {
            STD::ostringstream tmp;
            tmp.imbue(keep);
            tmp << i;
            test_true( !tmp.str().empty() );
        }
        os.imbue(keep);
        os.str(STD::string());
        os << 7;
        test_true( os.str() == STD::string("7") );
    }

    {
        STD::vector<STD::string> rows;
        for (int i = 0; i < 40; ++i) {
            STD::ostringstream one;
            one << "line" << i << '_' << STD::string("pad_long_enough_to_leave_sso");
            rows.push_back(one.str());
        }
        test_eq( (int)rows.size(), 40 );
        test_true( rows[0].find("line0_") == 0 );
        test_true( rows[39].size() > 25 );
        rows.clear();
        test_true( rows.empty() );
    }
}
