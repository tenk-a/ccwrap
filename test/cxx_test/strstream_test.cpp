#include "test_cxx.hpp"
#if _TST_HAS_REMOVED_CXX26
#include <strstream>
#include <cstring>

TEST_CASE(strstream, ostrstream) {
    STD::ostrstream os;
    os << "x=" << 42 << STD::ends;
    test_eq( STD::strcmp(os.str(), "x=42"), 0 );
    test_pass("cxx03:ostrstream");
    os.freeze(false);
    test_true( STD::strcmp(os.str(), "x=42") == 0 );
    test_pass("cxx03:freeze");
}

TEST_CASE(strstream, istrstream) {
    STD::istrstream is("7 ab 1.5");
    int a; char w[8]; double d;
    is >> a >> w >> d;
    test_eq( a, 7 );
    test_eq( STD::strcmp(w, "ab"), 0 );
    test_eq( d, 1.5 );
    test_pass("cxx03:istrstream");
}

TEST_CASE(strstream, ostrstream_pcount_and_fixed_buffer) {

    STD::ostrstream os;
    os << "hello";
    test_eq( os.pcount(), 5 );
    os << "!";
    test_eq( os.pcount(), 6 );
    test_pass("cxx03:pcount");
    os.freeze(false);

    char buf[16];
    STD::ostrstream fx(buf, sizeof buf);
    fx << "ab" << 12 << STD::ends;
    test_eq( STD::strcmp(buf, "ab12"), 0 );
    test_pass("cxx03:ostrstream(char*,n)");
    test_eq( fx.str(), buf );
    test_pass("cxx03:str");

    test_ptr( os.rdbuf() );
    test_pass("cxx03:strstreambuf");
}

TEST_CASE(strstream, bidirectional) {

    STD::strstream ss;
    ss << "42 abc";
    int n = 0;
    char w[8];
    ss >> n >> w;
    test_eq( n, 42 );
    test_eq( STD::strcmp(w, "abc"), 0 );
    test_pass("cxx03:strstream");

    char buf[16];
    STD::strstream fs(buf, sizeof buf, STD::ios_base::in | STD::ios_base::out);
    fs << "7 xy" << STD::ends;
    int a = 0;
    fs.seekg(0);
    fs >> a;
    test_eq( a, 7 );
    test_pass("cxx03:strstream(char*,n,mode)");
    test_ptr( fs.rdbuf() );
    test_pass("cxx03:strstream::rdbuf");
}

TEST_CASE(strstream, istrstream_with_length) {

    const char* data = "12345 rest-ignored";
    STD::istrstream is(data, 5);
    int v = 0;
    is >> v;
    test_eq( v, 12345 );
    test_pass("cxx03:istrstream(char*,n)");
    char c = 0;
    is.get(c);
    test_true( is.eof() || is.fail() );
    test_pass("cxx03:istrstream length window");
    test_ptr( is.rdbuf() );
    test_pass("cxx03:istrstream::rdbuf");
}

TEST_CASE(strstream, seek) {
    STD::istrstream is("0123456789");
    test_eq( (long)is.tellg(), 0L );
    test_pass("cxx03:tellg");

    char c = 0;
    is.seekg(4);
    test_eq( (long)is.tellg(), 4L );
    is.get(c);
    test_eq( c, '4' );
    test_pass("cxx03:seekg");

    is.seekg(2, STD::ios_base::cur);
    is.get(c);
    test_eq( c, '7' );

    is.seekg(-1, STD::ios_base::end);
    is.get(c);
    test_eq( c, '9' );
    test_pass("cxx03:seekg(off,dir)");

    is.seekg(99);
    test_true( is.fail() );
    test_pass("cxx03:seekg out of range");
}

TEST_CASE(strstream, explicit_members) {

    STD::ostrstream os;
    os << "n=" << 5 << STD::ends;
    char* p = os.str();
    test_true( STD::strcmp(p, "n=5") == 0 );
    test_pass("cxx03:ostrstream()");
    test_pass("cxx03:ostrstream::str");

    STD::strstreambuf* sb = os.rdbuf();
    test_ptr( sb );
    test_pass("cxx03:ostrstream::rdbuf");
    os.freeze(false);

    STD::istrstream is("12 34");
    int a = 0, b = 0;
    is >> a >> b;
    test_eq( a, 12 );
    test_eq( b, 34 );
    test_pass("cxx03:istrstream(const char*)");

    STD::strstream ss;
    ss << "hi" << STD::ends;
    char* q = ss.str();
    test_true( STD::strcmp(q, "hi") == 0 );
    test_pass("cxx03:strstream::str");
    ss.freeze(false);

    char buf[16];
    STD::strstreambuf usb(buf, sizeof buf, buf);
    STD::ostream ous(&usb);
    ous << "ab7" << STD::ends;
    test_true( STD::strcmp(buf, "ab7") == 0 );
    test_pass("cxx03:strstreambuf(char*,n,char*)");
}
#else
TEST_CASE(strstream, ostrstream) {
    TEST_NOTE("<strstream> was removed in C++26 and this library honours the removal");
    test_skip("cxx03:ostrstream");
    test_skip("cxx03:freeze");
}
TEST_CASE(strstream, istrstream) {
    TEST_NOTE("<strstream> was removed in C++26 and this library honours the removal");
    test_skip("cxx03:istrstream");
}
TEST_CASE(strstream, ostrstream_pcount_and_fixed_buffer) {
    TEST_NOTE("<strstream> was removed in C++26 and this library honours the removal");
    test_skip("cxx03:pcount");
    test_skip("cxx03:ostrstream(char*,n)");
    test_skip("cxx03:str");
    test_skip("cxx03:strstreambuf");
}
TEST_CASE(strstream, bidirectional) {
    TEST_NOTE("<strstream> was removed in C++26 and this library honours the removal");
    test_skip("cxx03:strstream");
    test_skip("cxx03:strstream(char*,n,mode)");
    test_skip("cxx03:strstream::rdbuf");
}
TEST_CASE(strstream, istrstream_with_length) {
    TEST_NOTE("<strstream> was removed in C++26 and this library honours the removal");
    test_skip("cxx03:istrstream(char*,n)");
    test_skip("cxx03:istrstream length window");
    test_skip("cxx03:istrstream::rdbuf");
}
TEST_CASE(strstream, seek) {
    TEST_NOTE("<strstream> was removed in C++26 and this library honours the removal");
    test_skip("cxx03:tellg");
    test_skip("cxx03:seekg");
    test_skip("cxx03:seekg(off,dir)");
    test_skip("cxx03:seekg out of range");
}
TEST_CASE(strstream, explicit_members) {
    TEST_NOTE("<strstream> was removed in C++26 and this library honours the removal");
    test_skip("cxx03:ostrstream()");
    test_skip("cxx03:ostrstream::str");
    test_skip("cxx03:ostrstream::rdbuf");
    test_skip("cxx03:istrstream(const char*)");
    test_skip("cxx03:strstream::str");
    test_skip("cxx03:strstreambuf(char*,n,char*)");
}
#endif
