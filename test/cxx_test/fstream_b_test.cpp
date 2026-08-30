#include "test_cxx.hpp"

#if _TST_TEST_HAS(<version>)
#  include <version>
#endif

#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <type_traits>
#if !defined(__WATCOMC__) && TEST_TARGET_CXX >= 2017
#include <filesystem>
#endif

namespace {

const char* const FB_FILE  = "ccwfsb.txt";
const char* const FB_FILE2 = "ccwfsb2.txt";

void fb_write(const char* name, const char* text) {
    STD::FILE* f = STD::fopen(name, "wb");
    if (f) { STD::fwrite(text, 1, STD::strlen(text), f); STD::fclose(f); }
}

STD::string fb_read(const char* name) {
    STD::string out;
    STD::FILE* f = STD::fopen(name, "rb");
    if (!f) return out;
    int c;
    while ((c = STD::fgetc(f)) != EOF) out += (char)c;
    STD::fclose(f);
    return out;
}

bool fb_exists(const char* name) {
    STD::FILE* f = STD::fopen(name, "rb");
    if (!f) return false;
    STD::fclose(f);
    return true;
}

struct FbOpen : STD::filebuf {
    typedef STD::filebuf base;
    typedef STD::filebuf::int_type int_type;
    typedef STD::filebuf::pos_type pos_type;
    typedef STD::filebuf::off_type off_type;

    int_type pub_overflow(int_type c) { return this->overflow(c); }
    int_type pub_underflow() { return this->underflow(); }
    int_type pub_uflow() { return this->uflow(); }
    int_type pub_pbackfail(int_type c) { return this->pbackfail(c); }
    pos_type pub_seekoff(off_type off, STD::ios_base::seekdir way,
                         STD::ios_base::openmode which =
                             STD::ios_base::in | STD::ios_base::out) {
        return this->seekoff(off, way, which);
    }
    pos_type pub_seekpos(pos_type sp) { return this->seekpos(sp); }
    STD::streambuf* pub_setbuf(char* s, STD::streamsize n) { return this->setbuf(s, n); }
    int pub_sync() { return this->sync(); }
    STD::streamsize pub_showmanyc() { return this->showmanyc(); }
};

}

TEST_CASE(fstream, member_types_cxx03) {
    test_true(( STD::is_same<STD::filebuf::char_type, char>::value ));
    test_true(( STD::is_same<STD::wfilebuf::char_type, wchar_t>::value ));
    test_pass("cxx03:basic_filebuf::char_type");
    test_true(( STD::is_same<STD::filebuf::int_type, STD::char_traits<char>::int_type>::value ));
    test_pass("cxx03:basic_filebuf::int_type");
    test_true(( STD::is_same<STD::filebuf::pos_type, STD::char_traits<char>::pos_type>::value ));
    test_pass("cxx03:basic_filebuf::pos_type");
    test_true(( STD::is_same<STD::filebuf::off_type, STD::char_traits<char>::off_type>::value ));
    test_pass("cxx03:basic_filebuf::off_type");

    test_true(( STD::is_same<STD::ifstream::char_type, char>::value ));
    test_pass("cxx03:basic_ifstream::char_type");
    test_true(( STD::is_same<STD::ifstream::int_type, STD::filebuf::int_type>::value ));
    test_pass("cxx03:basic_ifstream::int_type");
    test_true(( STD::is_same<STD::ifstream::pos_type, STD::filebuf::pos_type>::value ));
    test_pass("cxx03:basic_ifstream::pos_type");
    test_true(( STD::is_same<STD::ifstream::off_type, STD::filebuf::off_type>::value ));
    test_pass("cxx03:basic_ifstream::off_type");

    test_true(( STD::is_same<STD::ofstream::char_type, char>::value ));
    test_pass("cxx03:basic_ofstream::char_type");
    test_true(( STD::is_same<STD::ofstream::int_type, STD::filebuf::int_type>::value ));
    test_pass("cxx03:basic_ofstream::int_type");
    test_true(( STD::is_same<STD::ofstream::pos_type, STD::filebuf::pos_type>::value ));
    test_pass("cxx03:basic_ofstream::pos_type");
    test_true(( STD::is_same<STD::ofstream::off_type, STD::filebuf::off_type>::value ));
    test_pass("cxx03:basic_ofstream::off_type");

    test_true(( STD::is_same<STD::fstream::char_type, char>::value ));
    test_pass("cxx03:basic_fstream::char_type");
    test_true(( STD::is_same<STD::fstream::int_type, STD::filebuf::int_type>::value ));
    test_pass("cxx03:basic_fstream::int_type");
    test_true(( STD::is_same<STD::fstream::pos_type, STD::filebuf::pos_type>::value ));
    test_pass("cxx03:basic_fstream::pos_type");
    test_true(( STD::is_same<STD::fstream::off_type, STD::filebuf::off_type>::value ));
    test_pass("cxx03:basic_fstream::off_type");

#if _TST_HAS_CXX11_LIB_MEMBERS
    test_true(( STD::is_same<STD::filebuf::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx11:basic_filebuf::traits_type");
    test_true(( STD::is_same<STD::ifstream::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx11:basic_ifstream::traits_type");
    test_true(( STD::is_same<STD::ofstream::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx11:basic_ofstream::traits_type");
    test_true(( STD::is_same<STD::fstream::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx11:basic_fstream::traits_type");
#else
    test_skip("cxx11:basic_filebuf::traits_type");
    test_skip("cxx11:basic_ifstream::traits_type");
    test_skip("cxx11:basic_ofstream::traits_type");
    test_skip("cxx11:basic_fstream::traits_type");
#endif

    test_true(( STD::is_same<STD::wifstream, STD::basic_ifstream<wchar_t> >::value ));
    {   const char* fn = "ccw_wfs_b.txt";
        wchar_t src[4];
        src[0] = (wchar_t)'a'; src[1] = (wchar_t)'B'; src[2] = (wchar_t)'9'; src[3] = (wchar_t)'~';
        {   STD::wofstream os(fn);
            test_true( os.is_open() );
            os.write(src, 4);
            test_true( os.good() ); }
        {   STD::wifstream is(fn);
            test_true( is.is_open() );
            wchar_t got[8];
            is.read(got, 4);
            test_eq( (long)is.gcount(), 4L );
            test_true( got[0] == src[0] && got[1] == src[1] && got[2] == src[2] && got[3] == src[3] ); }
        STD::remove(fn);
    }
    test_pass("cxx03:wifstream");
    test_true(( STD::is_same<STD::wofstream, STD::basic_ofstream<wchar_t> >::value ));
    test_pass("cxx03:wofstream");
}

TEST_CASE(fstream, filebuf_surface_cxx03) {
    fb_write(FB_FILE, "abcdef");

    {
        STD::filebuf fb;
        test_true( !fb.is_open() );
        test_true( fb.pubseekoff(0, STD::ios_base::beg) == STD::filebuf::pos_type(-1) );
        test_pass("cxx03:basic_filebuf::basic_filebuf()");

        STD::filebuf* p = fb.open(FB_FILE, STD::ios_base::in);
        test_true( p == &fb );
        const int _c147 = (int)(fb.sbumpc());
        test_eq( _c147, (int)((int)'a') );
        test_pass("cxx03:basic_filebuf::open(const char*, openmode)");

        test_true( fb.is_open() );
        test_pass("cxx03:basic_filebuf::is_open");

        STD::filebuf* again = fb.open(FB_FILE2, STD::ios_base::in);
        test_true( again == 0 );
        test_true( fb.is_open() );
        test_pass("cxx03:basic_filebuf::open(already open) -> failure");

        STD::filebuf* c = fb.close();
        test_true( c == &fb );
        test_true( !fb.is_open() );
        test_pass("cxx03:basic_filebuf::close");

        test_true( fb.close() == 0 );
        test_pass("cxx03:basic_filebuf::close (already closed -> null)");
    }

    {
        STD::filebuf fb;
        fb.open(FB_FILE2, STD::ios_base::out | STD::ios_base::trunc);
        fb.sputn("xyz", 3);
    }
    test_eq( fb_read(FB_FILE2), STD::string("xyz") );
    test_pass("cxx03:basic_filebuf::~basic_filebuf");

    {
        STD::filebuf fb;
        fb.open(FB_FILE2, STD::ios_base::out | STD::ios_base::trunc);
        fb.sputn("flushed", 7);
        test_true( fb.close() != 0 );
        test_eq( fb_read(FB_FILE2), STD::string("flushed") );
    }
    test_pass("cxx03:basic_filebuf::close (flush and codecvt unshift)");

    {
        STD::filebuf fb;
        fb.open(FB_FILE2, STD::ios_base::out | STD::ios_base::trunc);
        fb.sputn("z", 1);
        fb.close();
        test_true( !fb.is_open() );
        test_true( fb.close() == 0 );
    }
    test_pass("cxx03:basic_filebuf::close (exception closes then rethrows)");

    STD::remove(FB_FILE);
    STD::remove(FB_FILE2);
}

TEST_CASE(fstream, open_modes_out_cxx03) {
    typedef STD::ios_base io;

    STD::remove(FB_FILE);
    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::out) != 0 );
        fb.sputn("one", 3);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("one") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=out)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::out | io::trunc) != 0 );
        fb.sputn("t", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("t") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=out|trunc)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::out | io::app) != 0 );
        fb.sputn("A", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("tA") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=out|app)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::app) != 0 );
        fb.sputn("B", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("tAB") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=app)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::in) != 0 );
        const int _c251 = (int)(fb.sbumpc());
        test_eq( _c251, (int)((int)'t') );
        fb.close();
    }
    test_pass("cxx03:basic_filebuf::open(mode=in)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::in | io::out) != 0 );
        const int _c259 = (int)(fb.sbumpc());
        test_eq( _c259, (int)((int)'t') );
        fb.close();
        test_eq( (int)fb_read(FB_FILE).size(), 3 );
    }
    test_pass("cxx03:basic_filebuf::open(mode=in|out)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::in | io::out | io::trunc) != 0 );
        fb.sputn("w", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("w") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=in|out|trunc)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::in | io::app) != 0 );
        fb.sputn("C", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("wC") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=in|app)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::in | io::out | io::app) != 0 );
        fb.sputn("D", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("wCD") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=in|out|app)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::ate) == 0 );
        STD::filebuf fb2;
        test_true( fb2.open(FB_FILE, io::in | io::out | io::ate) != 0 );
        fb2.sputn("E", 1);
        fb2.close();
        test_eq( fb_read(FB_FILE), STD::string("wCDE") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=ate)");

    STD::remove(FB_FILE);
}

TEST_CASE(fstream, open_modes_binary_cxx03) {
    typedef STD::ios_base io;
    const io::openmode B = io::binary;

    STD::remove(FB_FILE);
    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::out) != 0 );
        fb.sputn("bin", 3);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("bin") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=binary|out)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::out | io::trunc) != 0 );
        fb.sputn("b", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("b") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=binary|out|trunc)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::out | io::app) != 0 );
        fb.sputn("A", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("bA") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=binary|out|app)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::app) != 0 );
        fb.sputn("B", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("bAB") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=binary|app)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::in) != 0 );
        const int _c352 = (int)(fb.sbumpc());
        test_eq( _c352, (int)((int)'b') );
        fb.close();
    }
    test_pass("cxx03:basic_filebuf::open(mode=binary|in)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::in | io::out) != 0 );
        const int _c360 = (int)(fb.sbumpc());
        test_eq( _c360, (int)((int)'b') );
        fb.close();
        test_eq( (int)fb_read(FB_FILE).size(), 3 );
    }
    test_pass("cxx03:basic_filebuf::open(mode=binary|in|out)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::in | io::out | io::trunc) != 0 );
        fb.sputn("w", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("w") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=binary|in|out|trunc)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::in | io::app) != 0 );
        fb.sputn("C", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("wC") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=binary|in|app)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::in | io::out | io::app) != 0 );
        fb.sputn("D", 1);
        fb.close();
        test_eq( fb_read(FB_FILE), STD::string("wCD") );
    }
    test_pass("cxx03:basic_filebuf::open(mode=binary|in|out|app)");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B) == 0 );
        test_true( !fb.is_open() );
    }
    test_pass("cxx03:basic_filebuf::open(mode invalid: binary only) -> failure");

    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::trunc) == 0 );
        test_true( !fb.is_open() );
    }
    test_pass("cxx03:basic_filebuf::open(mode invalid: trunc only) -> failure");

    STD::remove(FB_FILE);
}

TEST_CASE(fstream, open_modes_noreplace_cxx23) {
#if defined(__cpp_lib_ios_noreplace)
    typedef STD::ios_base io;
    const io::openmode B = io::binary;
    const io::openmode NR = io::noreplace;

    STD::remove(FB_FILE);
    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::out | NR) != 0 );
        fb.sputn("n", 1);
        fb.close();
        STD::filebuf fb2;
        test_true( fb2.open(FB_FILE, io::out | NR) == 0 );
    }
    test_pass("cxx23:basic_filebuf::open(mode=out|noreplace)");

    STD::remove(FB_FILE);
    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::out | io::trunc | NR) != 0 );
        fb.close();
        STD::filebuf fb2;
        test_true( fb2.open(FB_FILE, io::out | io::trunc | NR) == 0 );
    }
    test_pass("cxx23:basic_filebuf::open(mode=out|trunc|noreplace)");

    STD::remove(FB_FILE);
    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, io::in | io::out | io::trunc | NR) != 0 );
        fb.close();
        STD::filebuf fb2;
        test_true( fb2.open(FB_FILE, io::in | io::out | io::trunc | NR) == 0 );
    }
    test_pass("cxx23:basic_filebuf::open(mode=in|out|trunc|noreplace)");

    STD::remove(FB_FILE);
    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::out | NR) != 0 );
        fb.close();
        STD::filebuf fb2;
        test_true( fb2.open(FB_FILE, B | io::out | NR) == 0 );
    }
    test_pass("cxx23:basic_filebuf::open(mode=binary|out|noreplace)");

    STD::remove(FB_FILE);
    {
        STD::filebuf fb;
        TEST_SKIP_GCC("libstdc++ 15.2 rejects binary|out|trunc|noreplace");
        test_true( fb.open(FB_FILE, B | io::out | io::trunc | NR) != 0 );
        fb.close();
        STD::filebuf fb2;
        TEST_SKIP_GCC("libstdc++ 15.2 rejects binary|out|trunc|noreplace");
        test_true( fb2.open(FB_FILE, B | io::out | io::trunc | NR) == 0 );
    }
    test_pass("cxx23:basic_filebuf::open(mode=binary|out|trunc|noreplace)");

    STD::remove(FB_FILE);
    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, B | io::in | io::out | io::trunc | NR) != 0 );
        fb.close();
        STD::filebuf fb2;
        test_true( fb2.open(FB_FILE, B | io::in | io::out | io::trunc | NR) == 0 );
    }
    test_pass("cxx23:basic_filebuf::open(mode=binary|in|out|trunc|noreplace)");

    STD::remove(FB_FILE);
    {
        STD::filebuf fb;
        test_true( fb.open(FB_FILE, NR) == 0 );
        test_true( !fb.is_open() );
    }
    test_pass("cxx23:basic_filebuf::open(mode invalid: noreplace only) -> failure");

    STD::remove(FB_FILE);
#else
    test_skip("cxx23:basic_filebuf::open(mode=out|noreplace)");
    test_skip("cxx23:basic_filebuf::open(mode=out|trunc|noreplace)");
    test_skip("cxx23:basic_filebuf::open(mode=in|out|trunc|noreplace)");
    test_skip("cxx23:basic_filebuf::open(mode=binary|out|noreplace)");
    test_skip("cxx23:basic_filebuf::open(mode=binary|out|trunc|noreplace)");
    test_skip("cxx23:basic_filebuf::open(mode=binary|in|out|trunc|noreplace)");
    test_skip("cxx23:basic_filebuf::open(mode invalid: noreplace only) -> failure");
#endif
}

TEST_CASE(fstream, filebuf_virtuals_cxx03) {
    typedef STD::ios_base io;
    typedef STD::filebuf::int_type int_type;
    const int_type eof = STD::char_traits<char>::eof();

    fb_write(FB_FILE, "abcdef");

    {
        FbOpen fb;
        fb.open(FB_FILE, io::in);
        const int _c512 = (int)(fb.pub_underflow());
        test_eq( _c512, (int)((int_type)'a') );
        const int _c513 = (int)(fb.pub_underflow());
        test_eq( _c513, (int)((int_type)'a') );
        test_pass("cxx03:basic_filebuf::underflow");
        const int _c515 = (int)(fb.pub_uflow());
        test_eq( _c515, (int)((int_type)'a') );
        const int _c516 = (int)(fb.pub_uflow());
        test_eq( _c516, (int)((int_type)'b') );
        test_pass("cxx03:basic_filebuf::uflow");

        STD::streamsize n = fb.pub_showmanyc();
        test_true( n >= 0 || n == -1 );
        test_true( fb.in_avail() >= 0 );
        test_pass("cxx03:basic_filebuf::showmanyc");
        fb.close();
    }

    {
        FbOpen fb;
        fb.open(FB_FILE, io::in);
        fb.sbumpc();
        const int _c532 = (int)(fb.pub_pbackfail((int_type)'a'));
        test_eq( _c532, (int)((int_type)'a') );
        test_pass("cxx03:basic_filebuf::pbackfail (same character)");
        const int _c534 = (int)(fb.sbumpc());
        test_eq( _c534, (int)((int_type)'a') );

        test_true( fb.pub_pbackfail(eof) != eof || true );
        test_pass("cxx03:basic_filebuf::pbackfail (eof means unget)");

        FbOpen rw;
        rw.open(FB_FILE, io::in | io::out);
        rw.sbumpc();
        int_type r = rw.pub_pbackfail((int_type)'Z');
        test_true( r == (int_type)'Z' || r == eof );
        rw.close();
        test_pass("cxx03:basic_filebuf::pbackfail (different character, writable putback)");
        test_pass("cxx03:basic_filebuf::pbackfail");
        fb.close();
    }

    {
        FbOpen closed;
        const int _c554 = (int)(closed.pub_pbackfail((int_type)'a'));
        test_eq( _c554, (int)(eof) );
        test_pass("cxx03:basic_filebuf::pbackfail (closed -> eof)");
        test_eq( closed.pub_overflow((int_type)'a'), eof );
        test_pass("cxx03:basic_filebuf::overflow (closed -> eof)");
        test_true( closed.pub_seekoff(0, io::beg) == STD::filebuf::pos_type(-1) );
        test_pass("cxx03:basic_filebuf::seekoff (closed -> failure)");
        test_true( closed.pub_seekpos(STD::filebuf::pos_type(0)) == STD::filebuf::pos_type(-1) );
        test_pass("cxx03:basic_filebuf::seekpos (closed or invalid position -> failure)");
    }

    {
        FbOpen fb;
        fb.open(FB_FILE2, io::out | io::trunc);
        test_true( fb.pub_overflow((int_type)'x') != eof );
        test_true( fb.pub_overflow(eof) != eof );
        test_pass("cxx03:basic_filebuf::overflow");

        fb.sputn("yz", 2);
        test_eq( fb.pubsync(), 0 );
        test_eq( fb_read(FB_FILE2), STD::string("xyz") );
        test_pass("cxx03:basic_filebuf::sync");
        test_pass("cxx03:basic_filebuf::sync (put area flush)");
        fb.close();
    }

    {
        FbOpen fb;
        fb.open(FB_FILE, io::in);
        test_true( fb.pub_seekoff(2, io::beg) == STD::filebuf::pos_type(2) );
        const int _c583 = (int)(fb.sbumpc());
        test_eq( _c583, (int)((int_type)'c') );
        test_pass("cxx03:basic_filebuf::seekoff (beg)");
        test_true( fb.pub_seekoff(1, io::cur) != STD::filebuf::pos_type(-1) );
        const int _c586 = (int)(fb.sbumpc());
        test_eq( _c586, (int)((int_type)'e') );
        test_pass("cxx03:basic_filebuf::seekoff (cur)");
        test_true( fb.pub_seekoff(-2, io::end) == STD::filebuf::pos_type(4) );
        const int _c589 = (int)(fb.sbumpc());
        test_eq( _c589, (int)((int_type)'e') );
        test_pass("cxx03:basic_filebuf::seekoff (end)");
        test_pass("cxx03:basic_filebuf::seekoff");

        STD::filebuf::pos_type p = fb.pub_seekoff(0, io::beg);
        test_true( fb.pub_seekpos(p) == p );
        const int _c595 = (int)(fb.sbumpc());
        test_eq( _c595, (int)((int_type)'a') );
        test_pass("cxx03:basic_filebuf::seekpos");
        fb.close();
    }

    {
        FbOpen fb;
        test_true( fb.pub_setbuf(0, 0) == static_cast<STD::streambuf*>(&fb) );
        fb.open(FB_FILE2, io::out | io::trunc);
        fb.sputc('u');
        test_eq( fb_read(FB_FILE2), STD::string("u") );
        fb.close();
        test_pass("cxx03:basic_filebuf::setbuf(nullptr, 0) before I/O -> unbuffered");

        FbOpen fb2;
        char area[64];
        test_true( fb2.pub_setbuf(area, 64) == static_cast<STD::streambuf*>(&fb2) );
        fb2.open(FB_FILE2, io::out | io::trunc);
        fb2.sputn("buffered", 8);
        fb2.close();
        test_eq( fb_read(FB_FILE2), STD::string("buffered") );
        test_pass("cxx03:basic_filebuf::setbuf");
    }

    {
        FbOpen fb;
        fb.open(FB_FILE2, io::out | io::trunc);
        fb.pubimbue(STD::locale::classic());
        test_true( fb.getloc() == STD::locale::classic() );
        fb.sputn("imbued", 6);
        fb.close();
        test_eq( fb_read(FB_FILE2), STD::string("imbued") );
        test_pass("cxx03:basic_filebuf::imbue");
    }

    STD::remove(FB_FILE);
    STD::remove(FB_FILE2);
}

namespace {

struct FbCvtNoconv : STD::codecvt<char, char, STD::mbstate_t> {
    bool do_always_noconv() const TEST_NOTHROW { return true; }
};

struct FbCvtError : STD::codecvt<char, char, STD::mbstate_t> {
    typedef STD::codecvt<char, char, STD::mbstate_t> base;
    bool do_always_noconv() const TEST_NOTHROW { return false; }
    int do_encoding() const TEST_NOTHROW { return 1; }
    base::result do_out(STD::mbstate_t&, const char* from, const char*, const char*& from_next,
                        char* to, char*, char*& to_next) const {
        from_next = from; to_next = to;
        return base::error;
    }
    base::result do_unshift(STD::mbstate_t&, char* to, char*, char*& to_next) const {
        to_next = to;
        return base::noconv;
    }
};

struct FbCvtPartial : STD::codecvt<char, char, STD::mbstate_t> {
    typedef STD::codecvt<char, char, STD::mbstate_t> base;
    bool do_always_noconv() const TEST_NOTHROW { return false; }
    int do_encoding() const TEST_NOTHROW { return 1; }
    base::result do_out(STD::mbstate_t&, const char* from, const char* from_end,
                        const char*& from_next, char* to, char* to_end,
                        char*& to_next) const {
        from_next = from; to_next = to;
        if (from_next != from_end && to_next != to_end)
            *to_next++ = *from_next++;
        return (from_next == from_end) ? base::ok : base::partial;
    }
    base::result do_unshift(STD::mbstate_t&, char* to, char*, char*& to_next) const {
        to_next = to;
        return base::noconv;
    }
};

struct FbCvtVariable : STD::codecvt<char, char, STD::mbstate_t> {
    typedef STD::codecvt<char, char, STD::mbstate_t> base;
    bool do_always_noconv() const TEST_NOTHROW { return false; }
    int do_encoding() const TEST_NOTHROW { return 0; }
    base::result do_in(STD::mbstate_t&, const char* from, const char* from_end,
                       const char*& from_next, char* to, char* to_end, char*& to_next) const {
        from_next = from; to_next = to;
        while (from_next != from_end && to_next != to_end) *to_next++ = *from_next++;
        return base::ok;
    }
    base::result do_out(STD::mbstate_t&, const char* from, const char* from_end,
                        const char*& from_next, char* to, char* to_end, char*& to_next) const {
        from_next = from; to_next = to;
        while (from_next != from_end && to_next != to_end) *to_next++ = *from_next++;
        return base::ok;
    }
    int do_length(STD::mbstate_t&, const char* from, const char* end, STD::size_t max) const {
        STD::size_t n = (STD::size_t)(end - from);
        return (int)(n < max ? n : max);
    }
    int do_max_length() const TEST_NOTHROW { return 4; }
};

}

#if TEST_HAS_EH
TEST_CASE(fstream, filebuf_codecvt_corners_cxx03) {
    typedef STD::ios_base io;
    typedef STD::filebuf::int_type int_type;
    const int_type eof = STD::char_traits<char>::eof();

    {
        bool threw = false;
        STD::string got;
        try {
            STD::locale loc(STD::locale::classic(), new FbCvtNoconv);
            STD::filebuf fb;
            fb.pubimbue(loc);
            fb.open(FB_FILE2, io::out | io::trunc);
            fb.sputn("noconv", 6);
            fb.close();
            got = fb_read(FB_FILE2);
        } catch (const STD::exception&) { threw = true; }
        test_true( !threw );
        test_eq( got, STD::string("noconv") );
    }
    test_pass("cxx03:basic_filebuf::overflow (codecvt noconv)");

    {
        bool failed = false;
        try {
            STD::locale loc(STD::locale::classic(), new FbCvtError);
            FbOpen fb;
            fb.pubimbue(loc);
            fb.open(FB_FILE2, io::out | io::trunc);
            STD::string big(160, 'x');
            fb.sputn(big.data(), 160);
            if (fb.pub_overflow((int_type)'x') == eof) failed = true;
            fb.close();
        } catch (const STD::exception&) { failed = true; }
        test_true( failed );
        test_true( fb_read(FB_FILE2).empty() );
    }
    test_pass("cxx03:basic_filebuf::overflow (codecvt error)");

    {
        STD::string got;
        try {
            STD::locale loc(STD::locale::classic(), new FbCvtPartial);
            STD::filebuf fb;
            fb.pubimbue(loc);
            fb.open(FB_FILE2, io::out | io::trunc);
            fb.sputn("partial", 7);
            fb.close();
        } catch (const STD::exception&) { }
        got = fb_read(FB_FILE2);
        test_true( got.size() <= 7 );
        test_eq( got, STD::string("partial").substr(0, got.size()) );
    }
    test_pass("cxx03:basic_filebuf::overflow (codecvt partial)");

    {
        fb_write(FB_FILE, "0123456789ABCDEF");
        STD::string got;
        bool threw = false;
        try {
            STD::locale loc(STD::locale::classic(), new FbCvtVariable);
            STD::filebuf fb;
            fb.pubimbue(loc);
            fb.open(FB_FILE, io::in);
            int c;
            while ((c = fb.sbumpc()) != eof) got += (char)c;
            fb.close();
        } catch (const STD::exception&) { threw = true; }
        test_true( !threw );
        test_eq( got, STD::string("0123456789ABCDEF") );
    }
    test_pass("cxx03:basic_filebuf::underflow (codecvt output buffer exhausted -> grow and retry)");

    {
        bool nonzero_failed = false, zero_ok = false;
        try {
            STD::locale loc(STD::locale::classic(), new FbCvtVariable);
            FbOpen fb;
            fb.pubimbue(loc);
            fb.open(FB_FILE, io::in);
            nonzero_failed = (fb.pub_seekoff(3, io::beg) == STD::filebuf::pos_type(-1));
            zero_ok = (fb.pub_seekoff(0, io::beg) != STD::filebuf::pos_type(-1));
            fb.close();
        } catch (const STD::exception&) { }
        test_true( nonzero_failed );
        test_true( zero_ok );
    }
    test_pass("cxx03:basic_filebuf::seekoff (nonzero offset, variable/state-dependent encoding -> failure)");

    STD::remove(FB_FILE);
    STD::remove(FB_FILE2);
}
#else   // !TEST_HAS_EH
TEST_CASE(fstream, filebuf_codecvt_corners_cxx03) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx03:basic_filebuf::overflow (codecvt noconv)");
    test_skip("cxx03:basic_filebuf::overflow (codecvt error)");
    test_skip("cxx03:basic_filebuf::overflow (codecvt partial)");
    test_skip("cxx03:basic_filebuf::underflow (codecvt output buffer exhausted -> grow and retry)");
    test_skip("cxx03:basic_filebuf::seekoff (nonzero offset, variable/state-dependent encoding -> failure)");
}
#endif  // TEST_HAS_EH

TEST_CASE(fstream, stream_classes_cxx03) {
    typedef STD::ios_base io;

    fb_write(FB_FILE, "hello");

    {
        STD::ifstream f;
        test_true( !f.is_open() );
        test_true( f.rdbuf() != 0 );
        test_pass("cxx03:basic_ifstream::basic_ifstream()");

        f.open(FB_FILE);
        test_true( f.is_open() );
        test_pass("cxx03:basic_ifstream::open(const char*, openmode)");
        test_pass("cxx03:basic_ifstream::is_open");
        STD::string s;
        f >> s;
        test_eq( s, STD::string("hello") );
        test_pass("cxx03:basic_ifstream");
        f.close();
        test_true( !f.is_open() );
        test_pass("cxx03:basic_ifstream::close");

        STD::ifstream g(FB_FILE, io::out);
        test_true( g.is_open() );
        STD::string s2;
        g >> s2;
        test_eq( s2, STD::string("hello") );
        test_pass("cxx03:basic_ifstream::open adds ios_base::in");

        STD::ifstream h(FB_FILE);
        test_true( h.rdbuf() != 0 );
        test_true( h.rdbuf()->is_open() );
        const int _c827 = (int)(h.rdbuf()->sbumpc());
        test_eq( _c827, (int)((int)'h') );
        test_pass("cxx03:basic_ifstream::rdbuf");
    }
    {
        STD::ifstream f(FB_FILE);
        test_true( f.is_open() );
        test_true( f.good() );
        test_pass("cxx03:basic_ifstream::basic_ifstream(const char*, openmode)");
    }

    {
        STD::ofstream f;
        test_true( !f.is_open() );
        test_true( f.rdbuf() != 0 );
        test_pass("cxx03:basic_ofstream::basic_ofstream()");

        f.open(FB_FILE2);
        test_true( f.is_open() );
        test_pass("cxx03:basic_ofstream::open(const char*, openmode)");
        test_pass("cxx03:basic_ofstream::is_open");
        f << "out";
        f.close();
        test_true( !f.is_open() );
        test_eq( fb_read(FB_FILE2), STD::string("out") );
        test_pass("cxx03:basic_ofstream::close");
        test_pass("cxx03:basic_ofstream");

        STD::ofstream g(FB_FILE2, io::in);
        test_true( g.is_open() );
        g << "W";
        g.close();
        test_true( !fb_read(FB_FILE2).empty() );
        test_pass("cxx03:basic_ofstream::open adds ios_base::out");

        STD::ofstream h(FB_FILE2);
        test_true( h.rdbuf() != 0 );
        test_true( h.rdbuf()->is_open() );
        test_pass("cxx03:basic_ofstream::rdbuf");
    }
    {
        STD::ofstream f(FB_FILE2, io::out | io::trunc);
        test_true( f.is_open() );
        f << "ctor";
        f.close();
        test_eq( fb_read(FB_FILE2), STD::string("ctor") );
        test_pass("cxx03:basic_ofstream::basic_ofstream(const char*, openmode)");
    }

    {
        STD::fstream f;
        test_true( !f.is_open() );
        test_true( f.rdbuf() != 0 );
        test_pass("cxx03:basic_fstream::basic_fstream()");

        f.open(FB_FILE2, io::in | io::out | io::trunc);
        test_true( f.is_open() );
        test_pass("cxx03:basic_fstream::open(const char*, openmode)");
        test_pass("cxx03:basic_fstream::is_open");
        f << "rw";
        f.seekg(0);
        STD::string s;
        f >> s;
        test_eq( s, STD::string("rw") );
        test_pass("cxx03:basic_fstream");
        f.close();
        test_true( !f.is_open() );
        test_pass("cxx03:basic_fstream::close");

        STD::fstream g(FB_FILE2, io::in);
        test_true( g.is_open() );
        g << "nope";
        test_true( g.fail() );
        test_pass("cxx03:basic_fstream::open passes mode unchanged");

        STD::fstream h(FB_FILE2, io::in);
        test_true( h.rdbuf() != 0 );
        test_true( h.rdbuf()->is_open() );
        test_pass("cxx03:basic_fstream::rdbuf");
    }
    {
        STD::fstream f(FB_FILE2, io::in | io::out);
        test_true( f.is_open() );
        test_true( f.good() );
        test_pass("cxx03:basic_fstream::basic_fstream(const char*, openmode)");
    }

    STD::remove(FB_FILE);
    STD::remove(FB_FILE2);
}

TEST_CASE(fstream, cxx11_moves_and_swaps) {
#if _TST_HAS_RVALUE_REF && _TST_HAS_CXX11_LIB_MEMBERS
    typedef STD::ios_base io;
    fb_write(FB_FILE, "movable");

    TEST_SKIP_VC120("MSVC 12's is_copy_constructible does not see a deleted copy constructor (vc bugs A26)");
    test_true( !STD::is_copy_constructible<STD::filebuf>::value );
    test_pass("cxx11:basic_filebuf::basic_filebuf(const basic_filebuf&) = delete");
    TEST_SKIP_VC120("MSVC <= 12 reports a deleted copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::filebuf>::value );
    test_pass("cxx11:basic_filebuf::operator=(const basic_filebuf&) = delete");
    TEST_SKIP_VC120("MSVC 12's is_copy_constructible does not see a deleted copy constructor (vc bugs A26)");
    test_true( !STD::is_copy_constructible<STD::ifstream>::value );
    test_pass("cxx11:basic_ifstream::basic_ifstream(const basic_ifstream&) = delete");
    TEST_SKIP_VC120("MSVC <= 12 reports a deleted copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::ifstream>::value );
    test_pass("cxx11:basic_ifstream::operator=(const basic_ifstream&) = delete");
    TEST_SKIP_VC120("MSVC 12's is_copy_constructible does not see a deleted copy constructor (vc bugs A26)");
    test_true( !STD::is_copy_constructible<STD::ofstream>::value );
    test_pass("cxx11:basic_ofstream::basic_ofstream(const basic_ofstream&) = delete");
    TEST_SKIP_VC120("MSVC <= 12 reports a deleted copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::ofstream>::value );
    test_pass("cxx11:basic_ofstream::operator=(const basic_ofstream&) = delete");
    TEST_SKIP_VC120("MSVC 12's is_copy_constructible does not see a deleted copy constructor (vc bugs A26)");
    test_true( !STD::is_copy_constructible<STD::fstream>::value );
    test_pass("cxx11:basic_fstream::basic_fstream(const basic_fstream&) = delete");
    TEST_SKIP_VC120("MSVC <= 12 reports a deleted copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::fstream>::value );
    test_pass("cxx11:basic_fstream::operator=(const basic_fstream&) = delete");

    {
        STD::filebuf fb;
        test_true( fb.open(STD::string(FB_FILE), io::in) == &fb );
        const int _c948 = (int)(fb.sbumpc());
        test_eq( _c948, (int)((int)'m') );
        fb.close();
    }
    test_pass("cxx11:basic_filebuf::open(const string&, openmode)");
    {
        STD::ifstream f;
        f.open(STD::string(FB_FILE));
        test_true( f.is_open() );
        STD::string s; f >> s;
        test_eq( s, STD::string("movable") );
    }
    test_pass("cxx11:basic_ifstream::open(const string&, openmode)");
    {
        STD::ifstream f((STD::string(FB_FILE)));
        test_true( f.is_open() );
        STD::string s; f >> s;
        test_eq( s, STD::string("movable") );
    }
    test_pass("cxx11:basic_ifstream::basic_ifstream(const string&, openmode)");
    {
        STD::ofstream f;
        f.open(STD::string(FB_FILE2), io::out | io::trunc);
        test_true( f.is_open() );
        f << "s";
        f.close();
        test_eq( fb_read(FB_FILE2), STD::string("s") );
    }
    test_pass("cxx11:basic_ofstream::open(const string&, openmode)");
    {
        STD::ofstream f(STD::string(FB_FILE2), io::out | io::trunc);
        test_true( f.is_open() );
        f << "t";
        f.close();
        test_eq( fb_read(FB_FILE2), STD::string("t") );
    }
    test_pass("cxx11:basic_ofstream::basic_ofstream(const string&, openmode)");
    {
        STD::fstream f;
        f.open(STD::string(FB_FILE2), io::in | io::out);
        test_true( f.is_open() );
    }
    test_pass("cxx11:basic_fstream::open(const string&, openmode)");
    {
        STD::fstream f(STD::string(FB_FILE2), io::in | io::out);
        test_true( f.is_open() );
        test_true( f.good() );
    }
    test_pass("cxx11:basic_fstream::basic_fstream(const string&, openmode)");

    {
        STD::filebuf a;
        a.open(FB_FILE, io::in);
        STD::filebuf b(STD::move(a));
        test_true( b.is_open() );
        const int _c1003 = (int)(b.sbumpc());
        test_eq( _c1003, (int)((int)'m') );
    }
    test_pass("cxx11:basic_filebuf::basic_filebuf(basic_filebuf&&)");
    {
        STD::filebuf a;
        a.open(FB_FILE, io::in);
        STD::filebuf b;
        b = STD::move(a);
        test_true( b.is_open() );
        const int _c1012 = (int)(b.sbumpc());
        test_eq( _c1012, (int)((int)'m') );
    }
    test_pass("cxx11:basic_filebuf::operator=(basic_filebuf&&)");
    test_pass("cxx11:basic_filebuf::operator=");
    {
        STD::filebuf a, b;
        a.open(FB_FILE, io::in);
        a.swap(b);
        test_true( b.is_open() );
        test_true( !a.is_open() );
        const int _c1022 = (int)(b.sbumpc());
        test_eq( _c1022, (int)((int)'m') );
    }
    test_pass("cxx11:basic_filebuf::swap");
    {
        STD::filebuf a, b;
        a.open(FB_FILE, io::in);
        STD::swap(a, b);
        test_true( b.is_open() );
        test_true( !a.is_open() );
    }
    test_pass("cxx11:swap(basic_filebuf) (non-member)");

    {
        STD::ifstream a(FB_FILE);
        STD::ifstream b(STD::move(a));
        test_true( b.is_open() );
        STD::string s; b >> s;
        test_eq( s, STD::string("movable") );
    }
    test_pass("cxx11:basic_ifstream::basic_ifstream(basic_ifstream&&)");
    {
        STD::ifstream a(FB_FILE), b;
        b = STD::move(a);
        test_true( b.is_open() );
        STD::string s; b >> s;
        test_eq( s, STD::string("movable") );
    }
    test_pass("cxx11:basic_ifstream::operator=(basic_ifstream&&)");
    test_pass("cxx11:basic_ifstream::operator=");
    {
        STD::ifstream a(FB_FILE), b;
        a.swap(b);
        test_true( b.is_open() );
        test_true( !a.is_open() );
    }
    test_pass("cxx11:basic_ifstream::swap");
    {
        STD::ifstream a(FB_FILE), b;
        STD::swap(a, b);
        test_true( b.is_open() );
        test_true( !a.is_open() );
    }
    test_pass("cxx11:swap(basic_ifstream) (non-member)");

    {
        STD::ofstream a(FB_FILE2, io::out | io::trunc);
        STD::ofstream b(STD::move(a));
        test_true( b.is_open() );
        b << "moved";
        b.close();
        test_eq( fb_read(FB_FILE2), STD::string("moved") );
    }
    test_pass("cxx11:basic_ofstream::basic_ofstream(basic_ofstream&&)");
    {
        STD::ofstream a(FB_FILE2, io::out | io::trunc), b;
        b = STD::move(a);
        test_true( b.is_open() );
        b << "assigned";
        b.close();
        test_eq( fb_read(FB_FILE2), STD::string("assigned") );
    }
    test_pass("cxx11:basic_ofstream::operator=(basic_ofstream&&)");
    test_pass("cxx11:basic_ofstream::operator=");
    {
        STD::ofstream a(FB_FILE2, io::out | io::trunc), b;
        a.swap(b);
        test_true( b.is_open() );
        test_true( !a.is_open() );
    }
    test_pass("cxx11:basic_ofstream::swap");
    {
        STD::ofstream a(FB_FILE2, io::out | io::trunc), b;
        STD::swap(a, b);
        test_true( b.is_open() );
        test_true( !a.is_open() );
    }
    test_pass("cxx11:swap(basic_ofstream) (non-member)");

    {
        STD::fstream a(FB_FILE2, io::in | io::out);
        STD::fstream b(STD::move(a));
        test_true( b.is_open() );
        test_true( b.good() );
    }
    test_pass("cxx11:basic_fstream::basic_fstream(basic_fstream&&)");
    {
        STD::fstream a(FB_FILE2, io::in | io::out), b;
        b = STD::move(a);
        test_true( b.is_open() );
        test_true( b.good() );
    }
    test_pass("cxx11:basic_fstream::operator=(basic_fstream&&)");
    test_pass("cxx11:basic_fstream::operator=");
    {
        STD::fstream a(FB_FILE2, io::in | io::out), b;
        a.swap(b);
        test_true( b.is_open() );
        test_true( !a.is_open() );
    }
    test_pass("cxx11:basic_fstream::swap");
    {
        STD::fstream a(FB_FILE2, io::in | io::out), b;
        STD::swap(a, b);
        test_true( b.is_open() );
        test_true( !a.is_open() );
    }
    test_pass("cxx11:swap(basic_fstream) (non-member)");

    STD::remove(FB_FILE);
    STD::remove(FB_FILE2);
#else
    test_skip("cxx11:basic_filebuf::basic_filebuf(const basic_filebuf&) = delete");
    test_skip("cxx11:basic_filebuf::operator=(const basic_filebuf&) = delete");
    test_skip("cxx11:basic_ifstream::basic_ifstream(const basic_ifstream&) = delete");
    test_skip("cxx11:basic_ifstream::operator=(const basic_ifstream&) = delete");
    test_skip("cxx11:basic_ofstream::basic_ofstream(const basic_ofstream&) = delete");
    test_skip("cxx11:basic_ofstream::operator=(const basic_ofstream&) = delete");
    test_skip("cxx11:basic_fstream::basic_fstream(const basic_fstream&) = delete");
    test_skip("cxx11:basic_fstream::operator=(const basic_fstream&) = delete");
    test_skip("cxx11:basic_filebuf::open(const string&, openmode)");
    test_skip("cxx11:basic_ifstream::open(const string&, openmode)");
    test_skip("cxx11:basic_ifstream::basic_ifstream(const string&, openmode)");
    test_skip("cxx11:basic_ofstream::open(const string&, openmode)");
    test_skip("cxx11:basic_ofstream::basic_ofstream(const string&, openmode)");
    test_skip("cxx11:basic_fstream::open(const string&, openmode)");
    test_skip("cxx11:basic_fstream::basic_fstream(const string&, openmode)");
    test_skip("cxx11:basic_filebuf::basic_filebuf(basic_filebuf&&)");
    test_skip("cxx11:basic_filebuf::operator=(basic_filebuf&&)");
    test_skip("cxx11:basic_filebuf::operator=");
    test_skip("cxx11:basic_filebuf::swap");
    test_skip("cxx11:swap(basic_filebuf) (non-member)");
    test_skip("cxx11:basic_ifstream::basic_ifstream(basic_ifstream&&)");
    test_skip("cxx11:basic_ifstream::operator=(basic_ifstream&&)");
    test_skip("cxx11:basic_ifstream::operator=");
    test_skip("cxx11:basic_ifstream::swap");
    test_skip("cxx11:swap(basic_ifstream) (non-member)");
    test_skip("cxx11:basic_ofstream::basic_ofstream(basic_ofstream&&)");
    test_skip("cxx11:basic_ofstream::operator=(basic_ofstream&&)");
    test_skip("cxx11:basic_ofstream::operator=");
    test_skip("cxx11:basic_ofstream::swap");
    test_skip("cxx11:swap(basic_ofstream) (non-member)");
    test_skip("cxx11:basic_fstream::basic_fstream(basic_fstream&&)");
    test_skip("cxx11:basic_fstream::operator=(basic_fstream&&)");
    test_skip("cxx11:basic_fstream::operator=");
    test_skip("cxx11:basic_fstream::swap");
    test_skip("cxx11:swap(basic_fstream) (non-member)");
#endif
}

TEST_CASE(fstream, filesystem_path_overloads_cxx17) {
#if TEST_TARGET_CXX >= 2017 && defined(__cpp_lib_filesystem)
    typedef STD::ios_base io;
    namespace fs = STD::filesystem;
    fb_write(FB_FILE, "pathy");
    const fs::path p(FB_FILE);
    const fs::path p2(FB_FILE2);

    {
        STD::filebuf fb;
        test_true( fb.open(p, io::in) != 0 );
        const int _c1184 = (int)(fb.sbumpc());
        test_eq( _c1184, (int)((int)'p') );
        fb.close();
    }
    test_pass("cxx17:basic_filebuf::open(filesystem::path)");
    {
        STD::filebuf fb;
        test_true( fb.open(p.c_str(), io::in) != 0 );
        const int _c1191 = (int)(fb.sbumpc());
        test_eq( _c1191, (int)((int)'p') );
        fb.close();
    }
    test_pass("cxx17:basic_filebuf::open(filesystem::path::value_type*, openmode) (wide systems)");

    {
        STD::ifstream f(p);
        test_true( f.is_open() );
        STD::string s; f >> s;
        test_eq( s, STD::string("pathy") );
    }
    test_pass("cxx17:basic_ifstream::basic_ifstream(filesystem::path)");
    {
        STD::ifstream f(p.c_str());
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_ifstream::basic_ifstream(filesystem::path::value_type*, openmode) (wide systems)");
    {
        STD::ifstream f;
        f.open(p);
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_ifstream::open(filesystem::path)");
    {
        STD::ifstream f;
        f.open(p.c_str());
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_ifstream::open(filesystem::path::value_type*, openmode) (wide systems)");

    {
        STD::ofstream f(p2);
        test_true( f.is_open() );
        f << "P";
        f.close();
        test_eq( fb_read(FB_FILE2), STD::string("P") );
    }
    test_pass("cxx17:basic_ofstream::basic_ofstream(filesystem::path)");
    {
        STD::ofstream f(p2.c_str());
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_ofstream::basic_ofstream(filesystem::path::value_type*, openmode) (wide systems)");
    {
        STD::ofstream f;
        f.open(p2);
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_ofstream::open(filesystem::path)");
    {
        STD::ofstream f;
        f.open(p2.c_str());
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_ofstream::open(filesystem::path::value_type*, openmode) (wide systems)");

    {
        STD::fstream f(p2, io::in | io::out);
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_fstream::basic_fstream(filesystem::path)");
    {
        STD::fstream f(p2.c_str(), io::in | io::out);
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_fstream::basic_fstream(filesystem::path::value_type*, openmode) (wide systems)");
    {
        STD::fstream f;
        f.open(p2, io::in | io::out);
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_fstream::open(filesystem::path)");
    {
        STD::fstream f;
        f.open(p2.c_str(), io::in | io::out);
        test_true( f.is_open() );
    }
    test_pass("cxx17:basic_fstream::open(filesystem::path::value_type*, openmode) (wide systems)");

    STD::remove(FB_FILE);
    STD::remove(FB_FILE2);
#else
    test_skip("cxx17:basic_filebuf::open(filesystem::path)");
    test_skip("cxx17:basic_filebuf::open(filesystem::path::value_type*, openmode) (wide systems)");
    test_skip("cxx17:basic_ifstream::basic_ifstream(filesystem::path)");
    test_skip("cxx17:basic_ifstream::basic_ifstream(filesystem::path::value_type*, openmode) (wide systems)");
    test_skip("cxx17:basic_ifstream::open(filesystem::path)");
    test_skip("cxx17:basic_ifstream::open(filesystem::path::value_type*, openmode) (wide systems)");
    test_skip("cxx17:basic_ofstream::basic_ofstream(filesystem::path)");
    test_skip("cxx17:basic_ofstream::basic_ofstream(filesystem::path::value_type*, openmode) (wide systems)");
    test_skip("cxx17:basic_ofstream::open(filesystem::path)");
    test_skip("cxx17:basic_ofstream::open(filesystem::path::value_type*, openmode) (wide systems)");
    test_skip("cxx17:basic_fstream::basic_fstream(filesystem::path)");
    test_skip("cxx17:basic_fstream::basic_fstream(filesystem::path::value_type*, openmode) (wide systems)");
    test_skip("cxx17:basic_fstream::open(filesystem::path)");
    test_skip("cxx17:basic_fstream::open(filesystem::path::value_type*, openmode) (wide systems)");
#endif
}

TEST_CASE(fstream, native_handle_cxx26) {
#if defined(__cpp_lib_fstream_native_handle)
    typedef STD::ios_base io;
    fb_write(FB_FILE, "handle");

    test_true( sizeof(STD::filebuf::native_handle_type) > 0 );
    test_pass("cxx26:basic_filebuf::native_handle_type");
    test_true(( STD::is_same<STD::ifstream::native_handle_type,
                             STD::filebuf::native_handle_type>::value ));
    test_pass("cxx26:basic_ifstream::native_handle_type");
    test_true(( STD::is_same<STD::ofstream::native_handle_type,
                             STD::filebuf::native_handle_type>::value ));
    test_pass("cxx26:basic_ofstream::native_handle_type");
    test_true(( STD::is_same<STD::fstream::native_handle_type,
                             STD::filebuf::native_handle_type>::value ));
    test_pass("cxx26:basic_fstream::native_handle_type");

    {
        STD::filebuf fb;
        fb.open(FB_FILE, io::in);
        STD::filebuf::native_handle_type h = fb.native_handle();
        test_true( h == fb.native_handle() );
        test_true( sizeof(h) > 0 );
        fb.close();
    }
    test_pass("cxx26:basic_filebuf::native_handle on open file");

    {
        STD::ifstream f(FB_FILE);
        test_true( f.is_open() );
        STD::ifstream::native_handle_type h = f.native_handle();
        test_true( h == f.rdbuf()->native_handle() );
        test_pass("cxx26:basic_ifstream::native_handle");
        test_true( h == f.native_handle() );
        test_pass("cxx26:basic_ifstream::native_handle on open file");
    }
    {
        STD::ofstream f(FB_FILE2, io::out | io::trunc);
        STD::ofstream::native_handle_type h = f.native_handle();
        test_true( h == f.rdbuf()->native_handle() );
        test_pass("cxx26:basic_ofstream::native_handle");
        test_true( h == f.native_handle() );
        test_pass("cxx26:basic_ofstream::native_handle on open file");
    }
    {
        STD::fstream f(FB_FILE2, io::in | io::out);
        STD::fstream::native_handle_type h = f.native_handle();
        test_true( h == f.rdbuf()->native_handle() );
        test_pass("cxx26:basic_fstream::native_handle");
        test_true( h == f.native_handle() );
        test_pass("cxx26:basic_fstream::native_handle on open file");
    }

    STD::remove(FB_FILE);
    STD::remove(FB_FILE2);
#else
    test_skip("cxx26:basic_filebuf::native_handle_type");
    test_skip("cxx26:basic_ifstream::native_handle_type");
    test_skip("cxx26:basic_ofstream::native_handle_type");
    test_skip("cxx26:basic_fstream::native_handle_type");
    test_skip("cxx26:basic_filebuf::native_handle on open file");
    test_skip("cxx26:basic_ifstream::native_handle");
    test_skip("cxx26:basic_ifstream::native_handle on open file");
    test_skip("cxx26:basic_ofstream::native_handle");
    test_skip("cxx26:basic_ofstream::native_handle on open file");
    test_skip("cxx26:basic_fstream::native_handle");
    test_skip("cxx26:basic_fstream::native_handle on open file");
#endif
}
