#include "test_cxx.hpp"
#include <streambuf>
#include <sstream>
#include <string>
#include <locale>
#include <cstring>
#include <type_traits>
#include <ostream>

TEST_CASE(streambuf, header_compiles) {
    test_true( true );
    test_pass("cxx03:basic_streambuf");
}

TEST_CASE(streambuf, put_area) {
    STD::stringbuf sb;
    int c = sb.sputc('H');
    test_eq( c, (int)'H' );
    test_pass("cxx03:sputc");
    STD::streamsize n1 = sb.sputn("ello", 4);
    test_eq( (long)n1, 4L );
    test_eq( sb.str(), STD::string("Hello") );

    STD::stringbuf sb2;
    STD::streamsize n = sb2.sputn("abcdef", 6);
    test_eq( (long)n, 6L );
    test_eq( sb2.str(), STD::string("abcdef") );
    test_pass("cxx03:sputn");
}

TEST_CASE(streambuf, get_area) {
    STD::stringbuf rb(STD::string("abc"), STD::ios_base::in);
    int c;

    test_eq( rb.sgetc(), (int)'a' );
    test_eq( rb.sgetc(), (int)'a' );
    test_pass("cxx03:sgetc");
    c = rb.sbumpc(); test_eq( c, (int)'a' );
    c = rb.sbumpc(); test_eq( c, (int)'b' );
    test_eq( rb.sgetc(), (int)'c' );
    c = rb.sbumpc(); test_eq( c, (int)'c' );
    test_pass("cxx03:sbumpc");
    test_eq( rb.sgetc(), EOF );
    c = rb.sbumpc(); test_eq( c, EOF );
    test_pass("cxx03:EOF at end");

    STD::stringbuf rb2(STD::string("abc"), STD::ios_base::in);
    int nc = rb2.snextc();
    test_eq( nc, (int)'b' );
    test_pass("cxx03:snextc");
}

TEST_CASE(streambuf, sgetn_block_read) {
    STD::stringbuf rb(STD::string("0123456789"), STD::ios_base::in);
    char buf[6];
    STD::memset(buf, 0, sizeof buf);
    STD::streamsize got = rb.sgetn(buf, 5);
    test_eq( (long)got, 5L );
    test_eq( STD::string(buf, 5), STD::string("01234") );
    got = rb.sgetn(buf, 5);
    test_eq( (long)got, 5L );
    test_eq( STD::string(buf, 5), STD::string("56789") );
    got = rb.sgetn(buf, 5);
    test_eq( (long)got, 0L );
    test_pass("cxx03:sgetn");
}

TEST_CASE(streambuf, putback_and_unget) {
    STD::stringbuf rb(STD::string("xy"), STD::ios_base::in);
    int c;
    c = rb.sbumpc();       test_eq( c, (int)'x' );
    c = rb.sungetc();      test_eq( c, (int)'x' );
    test_eq( rb.sgetc(), (int)'x' );
    c = rb.sbumpc();       test_eq( c, (int)'x' );
    test_pass("cxx03:sungetc");
    c = rb.sputbackc('x'); test_eq( c, (int)'x' );
    c = rb.sbumpc();       test_eq( c, (int)'x' );
    c = rb.sbumpc();       test_eq( c, (int)'y' );
    test_pass("cxx03:sputbackc");
}

TEST_CASE(streambuf, in_avail) {
    STD::stringbuf rb(STD::string("hello"), STD::ios_base::in);
    test_eq( (long)rb.in_avail(), 5L );
    rb.sbumpc();
    rb.sbumpc();
    test_eq( (long)rb.in_avail(), 3L );
    test_pass("cxx03:in_avail");
}

TEST_CASE(streambuf, seek) {
    STD::stringbuf ss(STD::string("0123456789"),
                      STD::ios_base::in | STD::ios_base::out);

    STD::streambuf::pos_type p = ss.pubseekoff(5, STD::ios_base::beg, STD::ios_base::in);
    test_eq( (long)p, 5L );
    test_eq( ss.sgetc(), (int)'5' );

    ss.pubseekoff(2, STD::ios_base::cur, STD::ios_base::in);
    test_eq( ss.sgetc(), (int)'7' );

    ss.pubseekoff(-1, STD::ios_base::end, STD::ios_base::in);
    test_eq( ss.sgetc(), (int)'9' );
    test_pass("cxx03:pubseekoff");

    ss.pubseekpos(3, STD::ios_base::out);
    ss.sputc('X');
    test_eq( ss.str(), STD::string("012X456789") );
    test_pass("cxx03:pubseekpos");
}

TEST_CASE(streambuf, sync_setbuf_locale) {
    STD::stringbuf sb;
    sb.sputn("hi", 2);
    test_eq( sb.pubsync(), 0 );
    test_pass("cxx03:pubsync");

    test_true( sb.pubsetbuf(0, 0) == &sb );
    test_pass("cxx03:pubsetbuf");

    test_true( sb.getloc() == STD::locale::classic() );
    test_pass("cxx03:getloc");
    STD::locale prev = sb.pubimbue(STD::locale());
    test_true( prev == STD::locale::classic() );
    test_pass("cxx03:pubimbue");
}

namespace {
class cstr_buf : public STD::streambuf {
public:
    explicit cstr_buf(const char* s) {
        char* b = const_cast<char*>(s);
        this->setg(b, b, b + STD::strlen(s));
    }
};
}

TEST_CASE(streambuf, custom_setg) {
    cstr_buf cb("world");
    test_eq( cb.sgetc(), (int)'w' );
    test_eq( (long)cb.in_avail(), 5L );

    char out[8];
    STD::memset(out, 0, sizeof out);
    STD::streamsize n = cb.sgetn(out, 8);
    test_eq( (long)n, 5L );
    test_eq( STD::string(out, 5), STD::string("world") );
    test_eq( cb.sbumpc(), EOF );
    test_pass("cxx03:setg");
}


namespace {
struct CcwProbeBuf : public STD::streambuf {
    char in_[8];
    char out_[8];
    CcwProbeBuf() {
        STD::memcpy(in_, "abcdefgh", 8);
        STD::memset(out_, 0, sizeof out_);
        setg(in_, in_ + 2, in_ + 8);
        setp(out_, out_ + 8);
    }
    char* p_eback() { return eback(); }
    char* p_gptr()  { return gptr(); }
    char* p_egptr() { return egptr(); }
    void  p_gbump(int n) { gbump(n); }
    char* p_pbase() { return pbase(); }
    char* p_pptr()  { return pptr(); }
    char* p_epptr() { return epptr(); }
    void  p_pbump(int n) { pbump(n); }
    void  p_setp(char* b, char* e) { setp(b, e); }

    int_type p_underflow()             { return underflow(); }
    int_type p_uflow()                 { return uflow(); }
    int_type p_overflow(int_type c)    { return overflow(c); }
    int_type p_pbackfail(int_type c)   { return pbackfail(c); }
    STD::streamsize p_showmanyc()      { return showmanyc(); }
    int p_sync()                       { return sync(); }
    STD::streambuf* p_setbuf(char* s, STD::streamsize n) { return setbuf(s, n); }
    pos_type p_seekoff(off_type o, STD::ios_base::seekdir d) {
        return seekoff(o, d, STD::ios_base::in);
    }
    pos_type p_seekpos(pos_type p)     { return seekpos(p, STD::ios_base::in); }
    void p_imbue(const STD::locale& l)  { imbue(l); }
    STD::streamsize p_xsgetn(char* s, STD::streamsize n) { return xsgetn(s, n); }
    STD::streamsize p_xsputn(const char* s, STD::streamsize n) { return xsputn(s, n); }
};

struct CcwEchoBuf : public STD::streambuf {
    char src_[4];
    char sink_[16];
    int  sunk_;
    int  overflowed_;
    int  synced_;
    CcwEchoBuf() : sunk_(0), overflowed_(0), synced_(0) {
        STD::memcpy(src_, "xy", 3);
        STD::memset(sink_, 0, sizeof sink_);
        setg(src_, src_, src_ + 2);
    }
protected:
    int_type overflow(int_type c) {
        ++overflowed_;
        if (!traits_type::eq_int_type(c, traits_type::eof()) && sunk_ < 15)
            sink_[sunk_++] = traits_type::to_char_type(c);
        return traits_type::not_eof(c);
    }
    int sync() { ++synced_; return 0; }
};
}

TEST_CASE(streambuf, member_types_cxx03) {
    typedef STD::basic_streambuf<char> SB;

    test_true(( STD::is_same<STD::streambuf, SB>::value ));
    test_pass("cxx03:streambuf");

    test_true(( STD::is_same<STD::wstreambuf, STD::basic_streambuf<wchar_t> >::value ));
    test_pass("cxx03:wstreambuf");

    test_true(( STD::is_same<SB::char_type, char>::value ));
    test_true(( STD::is_same<STD::wstreambuf::char_type, wchar_t>::value ));
    test_pass("cxx03:basic_streambuf::char_type");

    test_true(( STD::is_same<SB::int_type, STD::char_traits<char>::int_type>::value ));
    test_pass("cxx03:basic_streambuf::int_type");

    test_true(( STD::is_same<SB::pos_type, STD::char_traits<char>::pos_type>::value ));
    test_pass("cxx03:basic_streambuf::pos_type");

    test_true(( STD::is_same<SB::off_type, STD::char_traits<char>::off_type>::value ));
    test_pass("cxx03:basic_streambuf::off_type");

    test_true(( STD::is_same<SB::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx03:basic_streambuf::traits_type");

    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible/deleted copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::streambuf>::value );
    test_pass("cxx11:basic_streambuf::operator=");
}

TEST_CASE(streambuf, buffer_pointers_cxx03) {
    CcwProbeBuf b;

    test_true( b.p_eback() == b.in_ );
    test_pass("cxx03:basic_streambuf::eback");

    test_true( b.p_gptr() == b.in_ + 2 );
    test_eq( *b.p_gptr(), 'c' );
    test_pass("cxx03:basic_streambuf::gptr");

    test_true( b.p_egptr() == b.in_ + 8 );
    test_eq( (int)(b.p_egptr() - b.p_gptr()), 6 );
    test_pass("cxx03:basic_streambuf::egptr");

    b.p_gbump(2);
    test_true( b.p_gptr() == b.in_ + 4 );
    test_eq( *b.p_gptr(), 'e' );
    b.p_gbump(-1);
    test_true( b.p_gptr() == b.in_ + 3 );
    test_pass("cxx03:basic_streambuf::gbump");

    test_true( b.p_pbase() == b.out_ );
    test_pass("cxx03:basic_streambuf::pbase");

    test_true( b.p_pptr() == b.out_ );
    test_pass("cxx03:basic_streambuf::pptr");

    test_true( b.p_epptr() == b.out_ + 8 );
    test_pass("cxx03:basic_streambuf::epptr");

    *b.p_pptr() = 'Z';
    b.p_pbump(1);
    test_true( b.p_pptr() == b.out_ + 1 );
    test_eq( b.out_[0], 'Z' );
    test_pass("cxx03:basic_streambuf::pbump");

    b.p_setp(b.out_ + 2, b.out_ + 8);
    test_true( b.p_pbase() == b.out_ + 2 );
    test_true( b.p_pptr() == b.out_ + 2 );
    test_true( b.p_epptr() == b.out_ + 8 );
    test_pass("cxx03:basic_streambuf::setp");
}

TEST_CASE(streambuf, default_virtuals_cxx03) {
    CcwProbeBuf b;
    typedef STD::streambuf::traits_type T;

    test_true( T::eq_int_type(b.p_underflow(), T::eof()) );
    test_pass("cxx03:basic_streambuf::underflow");

    test_true( T::eq_int_type(b.p_uflow(), T::to_int_type('c')) ||
               T::eq_int_type(b.p_uflow(), T::eof()) );
    test_pass("cxx03:basic_streambuf::uflow");

    test_true( T::eq_int_type(b.p_overflow(T::to_int_type('x')), T::eof()) );
    test_pass("cxx03:basic_streambuf::overflow");

    test_true( T::eq_int_type(b.p_pbackfail(T::to_int_type('a')), T::eof()) );
    test_pass("cxx03:basic_streambuf::pbackfail");

    test_eq( (long)b.p_showmanyc(), 0L );
    test_pass("cxx03:basic_streambuf::showmanyc");

    test_eq( b.p_sync(), 0 );
    test_pass("cxx03:basic_streambuf::sync");

    char scratch[4];
    test_true( b.p_setbuf(scratch, 4) == &b );
    test_pass("cxx03:basic_streambuf::setbuf");

    STD::streambuf::pos_type bad = STD::streambuf::pos_type(STD::streambuf::off_type(-1));
    test_true( b.p_seekoff(0, STD::ios_base::beg) == bad );
    test_pass("cxx03:basic_streambuf::seekoff");
    test_true( b.p_seekpos(STD::streambuf::pos_type(0)) == bad );
    test_pass("cxx03:basic_streambuf::seekpos");

    STD::locale classic;
    b.p_imbue(classic);
    test_true( b.getloc() == classic );
    test_pass("cxx03:basic_streambuf::imbue");
}

TEST_CASE(streambuf, bulk_transfer_cxx03) {
    CcwProbeBuf b;
    char got[4];

    STD::streamsize n = b.p_xsgetn(got, 3);
    test_eq( (long)n, 3L );
    test_true( got[0] == 'c' && got[2] == 'e' );
    test_true( b.p_gptr() == b.in_ + 5 );
    test_pass("cxx03:basic_streambuf::xsgetn");

    STD::streamsize w = b.p_xsputn("PQ", 2);
    test_eq( (long)w, 2L );
    test_eq( b.out_[0], 'P' );
    test_true( b.p_pptr() == b.out_ + 2 );
    test_pass("cxx03:basic_streambuf::xsputn");

    CcwEchoBuf e;
    STD::ostream os(&e);
    os << "hi" << STD::flush;
    test_true( e.overflowed_ >= 2 );
    test_true( e.sunk_ >= 2 );
    test_true( e.sink_[0] == 'h' && e.sink_[1] == 'i' );
    test_true( e.synced_ >= 1 );
}

#if !_TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(streambuf, swap_cxx11) {
    TEST_NOTE("basic_streambuf::swap is C++11; this library predates it");
    test_skip("cxx11:basic_streambuf::swap");
}
#else
TEST_CASE(streambuf, swap_cxx11) {
    struct Swapper : public STD::streambuf {
        char buf_[4];
        Swapper(char c) { STD::memset(buf_, c, 4); setg(buf_, buf_, buf_ + 4); }
        void do_swap(Swapper& o) { this->swap(o); }
        char* g() { return gptr(); }
    };
    Swapper a('a'), b('b');
    test_eq( *a.g(), 'a' );
    a.do_swap(b);
    test_eq( *a.g(), 'b' );
    test_eq( *b.g(), 'a' );
    test_pass("cxx11:basic_streambuf::swap");
}
#endif
