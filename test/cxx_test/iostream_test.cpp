#include "test_cxx.hpp"
#include <iostream>
#include <sstream>
#include <string>

TEST_CASE(iostream, cout_is_an_ostream) {
    STD::stringbuf buf;
    STD::streambuf* old = STD::cout.rdbuf(&buf);
    STD::cout << "n=" << 42 << ' ' << STD::hex << 255;
    STD::cout.rdbuf(old);
    test_eq( buf.str(), STD::string("n=42 ff") );
    test_pass("cxx03:cout is an ostream");
}

TEST_CASE(iostream, cerr_and_clog) {

    STD::stringbuf eb, lb;
    STD::streambuf* oe = STD::cerr.rdbuf(&eb);
    STD::streambuf* ol = STD::clog.rdbuf(&lb);
    STD::cerr << "err" << 1;
    STD::clog << "log" << 2;
    STD::cerr.rdbuf(oe);
    STD::clog.rdbuf(ol);
    test_eq( eb.str(), STD::string("err1") );
    test_eq( lb.str(), STD::string("log2") );

    test_true( STD::cerr.good() );
    test_pass("cxx03:cerr and clog");
}

TEST_CASE(iostream, cin_is_an_istream) {
    STD::stringbuf in(STD::string("123 hello 4.5"), STD::ios_base::in);
    STD::streambuf* old = STD::cin.rdbuf(&in);

    int i = 0; STD::string w; double d = 0;
    STD::cin >> i >> w >> d;
    STD::cin.rdbuf(old);

    test_eq( i, 123 );
    test_eq( w, STD::string("hello") );
    test_eq( d, 4.5 );
    test_pass("cxx03:cin is an istream");
}

TEST_CASE(iostream, tie) {

    test_true( STD::cin.tie() == &STD::cout );
#if !defined(__WATCOMC__)
    test_true( sizeof(STD::wcout) > 0 );
    test_true( sizeof(STD::wcin) > 0 );
    test_pass("cxx03:tie");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx03:tie");
#endif
}

TEST_CASE(iostream, sync_with_stdio) {

    bool prev = STD::ios_base::sync_with_stdio(false);
    (void)prev;
    STD::stringbuf buf;
    STD::streambuf* old = STD::cout.rdbuf(&buf);
    STD::cout << "ok";
    STD::cout.rdbuf(old);
    STD::ios_base::sync_with_stdio(true);
    test_eq( buf.str(), STD::string("ok") );
    test_pass("cxx03:sync with stdio");
}

TEST_CASE(iostream, init_and_wide_streams) {

    {
        STD::ios_base::Init init_guard;
        (void)init_guard;
        test_true( STD::cout.rdbuf() != 0 );
    }
    test_true( STD::cout.rdbuf() != 0 );
    test_pass("cxx03:ios_base::Init (auto init of standard streams)");

#if 1

    test_true( STD::wcout.rdbuf() != 0 );
    test_pass("cxx03:wcout");
    test_true( STD::wcin.rdbuf() != 0 );
    test_pass("cxx03:wcin");
    test_true( STD::wcerr.rdbuf() != 0 );
    test_pass("cxx03:wcerr");
    test_true( STD::wclog.rdbuf() != 0 );
    test_pass("cxx03:wclog");
#else
    TEST_SKIP1(); test_skip("cxx03:wcout");
    TEST_SKIP1(); test_skip("cxx03:wcin");
    TEST_SKIP1(); test_skip("cxx03:wcerr");
    TEST_SKIP1(); test_skip("cxx03:wclog");
#endif
}


TEST_CASE(iostream, cerr_and_clog_cxx03) {
    test_true( STD::cerr.rdbuf() != 0 );
    test_true( (STD::cerr.flags() & STD::ios_base::unitbuf) != 0 );
    test_true( STD::cerr.good() );
    test_pass("cxx03:cerr");

    test_true( STD::clog.rdbuf() != 0 );
    TEST_NOTE("sharing one streambuf with cerr is not required; only libstdc++ does it");
    TEST_SKIP1();
    test_true( STD::clog.rdbuf() == STD::cerr.rdbuf() );
    test_true( STD::clog.good() );
    test_pass("cxx03:clog");
}
