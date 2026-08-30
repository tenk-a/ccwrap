#include "test_cxx.hpp"

#include <fstream>
#include <string>
#include <cstdio>
#if !defined(__WATCOMC__) && TEST_TARGET_CXX >= 2017
#include <filesystem>
#endif

TEST_CASE(fstream, write_read_roundtrip) {
    const char* fn = "ccwfstst.txt";

    {
        STD::ofstream ofs(fn);
        test_true( ofs.is_open() );
        test_pass("cxx03:ofstream");
        ofs << 7 << ' ' << 2 << ' ' << 3.5 << '\n' << "hello world" << STD::endl;
    }
    {
        STD::ifstream ifs(fn);
        test_true( ifs.is_open() );
        test_pass("cxx03:ifstream");
        int a = 0, b = 0; double d = 0;
        ifs >> a >> b >> d;
        test_eq( a, 7 );
        test_eq( b, 2 );
        test_eq( d, 3.5 );
        test_pass("cxx03:ofstream operator<<");
        test_true( !ifs.fail() );
        test_pass("cxx03:ifstream operator>>");

        STD::string w1, w2;
        ifs >> w1 >> w2;
        test_eq( w1, STD::string("hello") );
        test_eq( w2, STD::string("world") );
        test_pass("cxx03:operator>>(string)");
    }
    {
        STD::ifstream ifs(fn);
        STD::string line;
        STD::getline(ifs, line);
        test_eq( line, STD::string("7 2 3.5") );
        test_pass("cxx03:getline(ifstream)");
    }

    STD::remove(fn);
}

TEST_CASE(fstream, seek) {
    const char* fn = "ccwfssk.txt";

    { STD::ofstream ofs(fn); ofs << "0123456789"; }

    STD::ifstream ifs(fn);
    test_eq( (long)ifs.tellg(), 0L );
    test_pass("cxx03:tellg");

    char c = 0;
    ifs.get(c);
    test_eq( c, '0' );
    test_eq( (long)ifs.tellg(), 1L );

    ifs.seekg(5);
    test_eq( (long)ifs.tellg(), 5L );
    ifs.get(c);
    test_eq( c, '5' );
    test_pass("cxx03:seekg");

    ifs.seekg(0, STD::ios_base::end);
    test_eq( (long)ifs.tellg(), 10L );
    ifs.seekg(-3, STD::ios_base::cur);
    ifs.get(c);
    test_eq( c, '7' );
    test_pass("cxx03:seekg(off,dir)");

    ifs.seekg(0, STD::ios_base::beg);
    ifs.get(c);
    test_eq( c, '0' );
    ifs.close();

    {
        STD::fstream fs(fn, STD::ios_base::in | STD::ios_base::out);
        test_true( fs.is_open() );
        test_pass("cxx03:fstream");
        fs.seekp(3);
        test_eq( (long)fs.tellp(), 3L );
        test_pass("cxx03:tellp");
        fs << "XY";
    }
    {
        STD::ifstream chk(fn);
        STD::string line;
        STD::getline(chk, line);
        test_eq( line, STD::string("012XY56789") );
        test_pass("cxx03:seekp");
    }

    STD::remove(fn);
}

TEST_CASE(fstream, open_close_reopen) {
    const char* fn = "ccwfsop.txt";

    STD::ofstream ofs;
    test_true( !ofs.is_open() );
    test_pass("cxx03:ofstream()");
    ofs.open(fn);
    test_true( ofs.is_open() );
    test_pass("cxx03:open");
    ofs << "first";
    ofs.close();
    test_true( !ofs.is_open() );
    test_pass("cxx03:close");

    const char* fn2 = "ccwfsop2.txt";
    ofs.open(fn2);
    test_true( ofs.is_open() );
    ofs << "second";
    ofs.close();

    STD::ifstream ifs;
    ifs.open(fn);
    test_true( ifs.is_open() );
    test_pass("cxx03:is_open");
    STD::string s; ifs >> s;
    test_eq( s, STD::string("first") );
    ifs.close();
    ifs.clear();
    ifs.open(fn2);
    ifs >> s;
    test_eq( s, STD::string("second") );
    test_pass("cxx03:reopen");
    ifs.close();

    STD::remove(fn);
    STD::remove(fn2);
}

TEST_CASE(fstream, open_modes_and_failure) {
    const char* fn = "ccwfsmod.txt";

    STD::ifstream miss("ccwnosf2.txt");
    test_true( !miss.is_open() );
    test_true( miss.fail() );
    test_pass("cxx03:open failure");

    { STD::ofstream a(fn); a << "AAA"; }
    { STD::ofstream ap(fn, STD::ios_base::app); ap << "BBB"; }
    {
        STD::ifstream in(fn);
        STD::string s; in >> s;
        test_eq( s, STD::string("AAABBB") );
    }
    test_pass("cxx03:ios_base::app");

    { STD::ofstream tr(fn, STD::ios_base::out | STD::ios_base::trunc); tr << "X"; }
    {
        STD::ifstream in(fn);
        STD::string s; in >> s;
        test_eq( s, STD::string("X") );
    }
    test_pass("cxx03:ios_base::trunc");

    STD::remove(fn);
}

TEST_CASE(fstream, filebuf_direct) {
    const char* fn = "ccwfbuf.txt";

    STD::filebuf wb;
    test_true( wb.open(fn, STD::ios_base::out) != 0 );
    test_true( wb.is_open() );
    test_pass("cxx03:filebuf::open");
    wb.sputn("hello", 5);
    wb.close();
    test_true( !wb.is_open() );
    test_pass("cxx03:filebuf::close");

    STD::filebuf rb;
    test_true( rb.open(fn, STD::ios_base::in) != 0 );
    int c1 = rb.sbumpc();
    int c2 = rb.sbumpc();
    test_eq( c1, (int)'h' );
    test_eq( c2, (int)'e' );
    test_pass("cxx03:basic_filebuf");
    rb.close();

    STD::ifstream ifs(fn);
    test_ptr( ifs.rdbuf() );
    test_pass("cxx03:ifstream::rdbuf");
    test_true( ifs.rdbuf()->is_open() );
    test_pass("cxx03:filebuf::is_open");
    ifs.close();

    STD::remove(fn);
}

#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(fstream, ofstream_move) {
    const char* fn = "ccwfsmv.txt";
    {
        STD::ofstream a(fn);
        test_true( a.is_open() );
        a << "AB";
        STD::ofstream b(STD::move(a));
        test_true( b.is_open() );
        test_pass("cxx11:ofstream move ctor");
        b << "CD";
        b.close();
    }
    {
        STD::ifstream in(fn);
        STD::ifstream in2(STD::move(in));
        STD::string s; in2 >> s;
        test_eq( s, STD::string("ABCD") );
        test_pass("cxx11:ifstream move ctor");
    }

    {
        STD::ofstream a(fn); a << "Z";
        STD::ofstream c; c = STD::move(a);
        c << "Q"; c.close();
        STD::ifstream in(fn); STD::string s; in >> s;
        test_eq( s, STD::string("ZQ") );
        test_pass("cxx11:ofstream move assign");
    }
    STD::remove(fn);
}
#else
TEST_CASE_SKIP(fstream, ofstream_move)
#endif


#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(fstream, fstream_move_and_swap) {
    const char* fn = "ccwfsmv2.txt";
    {
        STD::fstream f(fn, STD::ios_base::in | STD::ios_base::out | STD::ios_base::trunc);
        f << "hi";
        STD::fstream g(STD::move(f));
        g.seekg(0);
        STD::string s; g >> s;
        test_eq( s, STD::string("hi") );
        test_pass("cxx11:fstream move ctor");
        g.close();
    }
    STD::remove(fn);
}
#else
TEST_CASE_SKIP(fstream, fstream_move_and_swap)
#endif


TEST_CASE(fstream, wide_typedefs) {
    STD::wfilebuf*  a = 0;
    STD::wifstream* b = 0;
    STD::wofstream* c = 0;
    STD::wfstream*  d = 0;
    test_true( !a && !b && !c && !d );
    test_pass("cxx03:wide fstream typedefs");
    test_true( sizeof(STD::wfstream) > 0 );
    test_pass("cxx03:wfstream");
    test_true( sizeof(STD::wfilebuf) > 0 );
    test_pass("cxx03:wfilebuf");
}

TEST_CASE(fstream, ifstream_open_close) {
    const char* fn = "ccwfsic.txt";
    { STD::ofstream o(fn); o << "content"; }

    STD::ifstream ifs;
    test_true( !ifs.is_open() );
    ifs.open(fn);
    test_true( ifs.is_open() );
    test_pass("cxx03:ifstream::open");

    STD::string s; ifs >> s;
    test_eq( s, STD::string("content") );
    test_pass("cxx03:ifstream::is_open");

    ifs.close();
    test_true( !ifs.is_open() );
    test_pass("cxx03:ifstream::close");

    STD::remove(fn);
}

TEST_CASE(fstream, rdbuf_accessors) {
    const char* fn = "ccwfsrb.txt";
    { STD::ofstream o(fn); o << "x"; }

    STD::ofstream ofs(fn);
    test_true( ofs.rdbuf() != 0 );
    test_pass("cxx03:ofstream::rdbuf");
    ofs.close();

    STD::fstream fs(fn, STD::ios_base::in | STD::ios_base::out);
    test_true( fs.rdbuf() != 0 );
    test_pass("cxx03:fstream::rdbuf");
    fs.close();

    STD::remove(fn);
}

#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(fstream, move_assign_and_swap) {
    const char* fn = "ccwfsms.txt";

    {
        { STD::ofstream o(fn); o << "MV"; }
        STD::ifstream a(fn);
        STD::ifstream b;
        b = STD::move(a);
        STD::string s; b >> s;
        test_eq( s, STD::string("MV") );
        test_pass("cxx11:ifstream move assign");
    }
    {
        STD::fstream f(fn, STD::ios_base::in | STD::ios_base::out | STD::ios_base::trunc);
        f << "fa";
        STD::fstream g;
        g = STD::move(f);
        g.seekg(0);
        STD::string s; g >> s;
        test_eq( s, STD::string("fa") );
        test_pass("cxx11:fstream move assign");
    }
    {
        const char* f1 = "ccwfss1.txt";
        const char* f2 = "ccwfss2.txt";
        STD::ofstream a(f1); STD::ofstream b(f2);
        a.swap(b);
        a << "toF2"; b << "toF1";
        a.close(); b.close();

        {   STD::ifstream i1(f1), i2(f2); STD::string s1, s2; i1 >> s1; i2 >> s2;
            test_eq( s1, STD::string("toF1") );
            test_eq( s2, STD::string("toF2") );
        }
        test_pass("cxx11:ofstream::swap");
        STD::remove(f1); STD::remove(f2);
    }
    {
        const char* f1 = "ccwfss3.txt";
        const char* f2 = "ccwfss4.txt";
        { STD::ofstream o(f1); o << "AAA"; }
        { STD::ofstream o(f2); o << "BBB"; }
        {   STD::ifstream a(f1), b(f2);
            a.swap(b);
            STD::string s1, s2; a >> s1; b >> s2;
            test_eq( s1, STD::string("BBB") );
            test_eq( s2, STD::string("AAA") );
        }
        test_pass("cxx11:ifstream::swap");
        STD::remove(f1); STD::remove(f2);
    }
    {
        const char* f1 = "ccwfss5.txt";
        const char* f2 = "ccwfss6.txt";
        STD::fstream a(f1, STD::ios_base::in | STD::ios_base::out | STD::ios_base::trunc);
        STD::fstream b(f2, STD::ios_base::in | STD::ios_base::out | STD::ios_base::trunc);
        a << "P"; b << "Q";
        a.swap(b);
        a << "1"; b << "2";
        test_pass("cxx11:fstream::swap");
        STD::swap(a, b);
        a << "!"; b << "?";
        a.close(); b.close();
        {   STD::ifstream i1(f1), i2(f2); STD::string s1, s2; i1 >> s1; i2 >> s2;
            test_eq( s1, STD::string("P2!") );
            test_eq( s2, STD::string("Q1?") );
        }
        test_pass("cxx11:swap(fstream) (non-member)");
        STD::remove(f1); STD::remove(f2);
    }

    STD::remove(fn);
}
#else
TEST_CASE_SKIP(fstream, move_assign_and_swap)
#endif


#if _TST_HAS_CXX11_LIB_MEMBERS && _TST_HAS_RVALUE_REF
TEST_CASE(fstream, filebuf_swap) {
    const char* f1 = "ccwfsfb1.txt";
    const char* f2 = "ccwfsfb2.txt";
    { STD::ofstream o(f1); o << "L"; }
    { STD::ofstream o(f2); o << "R"; }

    STD::filebuf a, b;
    a.open(f1, STD::ios_base::in);
    b.open(f2, STD::ios_base::in);
    a.swap(b);
    test_true( a.is_open() && b.is_open() );
    int ca = a.sbumpc();
    int cb = b.sbumpc();
    test_eq( ca, (int)'R' );
    test_eq( cb, (int)'L' );
    test_pass("cxx11:filebuf::swap");

    a.close(); b.close();
    STD::remove(f1); STD::remove(f2);
}
#else
TEST_CASE_SKIP(fstream, filebuf_swap)
#endif


TEST_CASE(fstream, filesystem_path_openers) {
#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
    STD::filesystem::path p("ccwfsfsp.txt");
    {
        STD::ofstream o(p);
        test_true( o.is_open() );
        o << "fsp";
        test_pass("cxx17:ofstream(filesystem::path)");
    }
    {
        STD::ifstream i(p);
        test_true( i.is_open() );
        STD::string s; i >> s;
        test_eq( s, STD::string("fsp") );
        test_pass("cxx17:ifstream(filesystem::path)");
    }
    {
        STD::fstream f(p, STD::ios_base::in | STD::ios_base::out);
        test_true( f.is_open() );
        test_pass("cxx17:fstream(filesystem::path)");
    }
    {
        STD::filebuf fb;
        test_true( fb.open(p, STD::ios_base::in) != 0 );
        test_true( fb.is_open() );
        fb.close();
        test_pass("cxx17:filebuf::open(filesystem::path)");
    }
    STD::filesystem::remove(p);
#else
    test_skip("cxx17:ofstream(filesystem::path)");
    test_skip("cxx17:ifstream(filesystem::path)");
    test_skip("cxx17:fstream(filesystem::path)");
    test_skip("cxx17:filebuf::open(filesystem::path)");
#endif
}

TEST_CASE(fstream, filebuf_native_handle) {
#if !defined(__WATCOMC__) && TEST_TARGET_CXX >= 2026
    const char* fn = "ccwfsnh.txt";
    { STD::ofstream o(fn); o << "n"; }
    STD::filebuf fb;
    fb.open(fn, STD::ios_base::in);
    STD::filebuf::native_handle_type h = fb.native_handle();
    (void)h;
    test_true( fb.is_open() );
    test_pass("cxx26:filebuf::native_handle");
    fb.close();
    STD::remove(fn);
#else
    test_skip("cxx26:filebuf::native_handle");
#endif
}

TEST_CASE(fstream, feature_test_macros) {
#if defined(__cpp_lib_fstream_native_handle)
    test_true( __cpp_lib_fstream_native_handle > 0L );
    test_pass("cxx26:__cpp_lib_fstream_native_handle == 202306L");
#else
    test_skip("cxx26:__cpp_lib_fstream_native_handle == 202306L");
#endif
}
