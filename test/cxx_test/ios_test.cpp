#include "test_cxx.hpp"

#include <ios>
#include <locale>
#include <sstream>
#include <string>
#include <system_error>
#include <type_traits>

namespace {
    int g_ios_ev = 0;
    void ios_ev_cb(STD::ios_base::event, STD::ios_base&, int) { ++g_ios_ev; }
}

static int ios_imbue_callback_hits = 0;

static void ios_imbue_callback(STD::ios_base::event ev, STD::ios_base&, int idx) {
    if (ev == STD::ios_base::imbue_event && idx == 17)
        ++ios_imbue_callback_hits;
}

TEST_CASE(ios, format_manipulators) {
    STD::ostringstream m1;
    m1 << STD::showpoint << 1.0;
    test_eq( m1.str(), STD::string("1.00000") );
    test_pass("cxx03:showpoint");

    STD::ostringstream mf;
    mf << STD::fixed << 1.5;
    test_true( (mf.flags() & STD::ios_base::floatfield) == STD::ios_base::fixed );
    test_pass("cxx03:fixed");

    STD::ostringstream m2;
    m2 << STD::hexfloat << 1.0;
    test_true( (m2.flags() & STD::ios_base::floatfield)
               == (STD::ios_base::fixed | STD::ios_base::scientific) );
    test_pass("cxx11:hexfloat");

    STD::ostringstream m3;
    m3 << STD::scientific;
    test_true( (m3.flags() & STD::ios_base::floatfield) == STD::ios_base::scientific );
    test_pass("cxx03:scientific");
    m3 << STD::defaultfloat;
    test_true( (m3.flags() & STD::ios_base::floatfield) == 0 );
    test_pass("cxx11:defaultfloat");

    STD::ostringstream m4;
    m4 << STD::internal;
    test_true( (m4.flags() & STD::ios_base::adjustfield) == STD::ios_base::internal );
    test_pass("cxx03:internal");

    STD::ostringstream m5;
    test_true( (m5 << STD::unitbuf).flags() & STD::ios_base::unitbuf );
    test_pass("cxx03:unitbuf");
}

TEST_CASE(ios, state_bits) {
    STD::istringstream s("42");

    test_true( s.good() );
    test_pass("cxx03:good");
    test_true( !s.eof() );
    test_pass("cxx03:eof");
    test_true( !s.fail() );
    test_pass("cxx03:fail");
    test_true( !s.bad() );
    test_pass("cxx03:bad");
    test_eq( s.rdstate(), STD::ios_base::goodbit );
    test_pass("cxx03:rdstate");
    test_true( (bool)s );
    test_true( !!s );
    test_true( !(!s) );
    test_pass("cxx03:operator!");

    s.setstate(STD::ios_base::failbit);
    test_true( s.fail() );
    test_true( !s.good() );
    test_true( !s );
    test_pass("cxx03:setstate");
    s.clear();
    test_true( s.good() );
    test_pass("cxx03:clear");

    int v = 0;
    s >> v;
    test_eq( v, 42 );
    int w = 0;
    s >> w;
    test_true( s.eof() || s.fail() );
    test_pass("cxx03:eofbit");

    s.clear(STD::ios_base::eofbit);
    test_true( s.eof() );
    test_true( !s.bad() );
    test_pass("cxx03:clear(state)");
}

TEST_CASE(ios, format_state) {
    STD::ostringstream os;

    test_eq( (long)os.width(), 0L );
    os.width(6);
    test_eq( (long)os.width(), 6L );
    test_pass("cxx03:width");
    test_eq( (long)os.precision(), 6L );
    STD::streamsize oldp = os.precision(3);
    test_eq( (long)oldp, 6L );
    test_eq( (long)os.precision(), 3L );
    test_pass("cxx03:precision");
    test_eq( os.fill(), ' ' );
    char oldf = os.fill('*');
    test_eq( oldf, ' ' );
    test_eq( os.fill(), '*' );
    test_pass("cxx03:fill");

    STD::ios_base::fmtflags saved = os.flags();
    os.setf(STD::ios_base::showpos);
    test_true( os.flags() & STD::ios_base::showpos );
    test_pass("cxx03:setf");
    os.unsetf(STD::ios_base::showpos);
    test_true( !(os.flags() & STD::ios_base::showpos) );
    test_pass("cxx03:unsetf");

    os.setf(STD::ios_base::hex, STD::ios_base::basefield);
    test_true( (os.flags() & STD::ios_base::basefield) == STD::ios_base::hex );
    test_pass("cxx03:setf(mask)");

    os.flags(saved);
    test_true( (os.flags() & STD::ios_base::basefield) != STD::ios_base::hex );
    test_pass("cxx03:flags");
}

TEST_CASE(ios, more_manipulators) {
    STD::ostringstream b;
    b << STD::boolalpha << true << ' ' << false;
    test_eq( b.str(), STD::string("true false") );
    test_pass("cxx03:boolalpha");

    STD::ostringstream nb;
    nb << STD::noboolalpha << true;
    test_eq( nb.str(), STD::string("1") );
    test_pass("cxx03:noboolalpha");

    STD::ostringstream sp;
    sp << STD::showpos << 5;
    test_eq( sp.str(), STD::string("+5") );
    test_pass("cxx03:showpos");

    STD::ostringstream dc;
    dc << STD::dec << 255;
    test_eq( dc.str(), STD::string("255") );
    test_pass("cxx03:dec");

    STD::ostringstream hx;
    hx << STD::hex << 255;
    test_eq( hx.str(), STD::string("ff") );
    test_pass("cxx03:hex");

    STD::ostringstream up;
    up << STD::uppercase << STD::hex << 255;
    test_eq( up.str(), STD::string("FF") );
    test_pass("cxx03:uppercase");

    STD::ostringstream oc;
    oc << STD::oct << 9;
    test_eq( oc.str(), STD::string("11") );
    test_pass("cxx03:oct");

    STD::ostringstream lf;
    lf << STD::left;
    lf.width(4);
    lf.fill('.');
    lf << 7;
    test_eq( lf.str(), STD::string("7...") );
    test_pass("cxx03:left");

    STD::ostringstream rf;
    rf << STD::right;
    rf.width(4);
    rf.fill('.');
    rf << 7;
    test_eq( rf.str(), STD::string("...7") );
    test_pass("cxx03:right");

    STD::istringstream ws("   9");
    int v = 0;
    ws >> STD::skipws >> v;
    test_eq( v, 9 );
    test_pass("cxx03:skipws");
    STD::istringstream nws(" x");
    char c = 'Z';
    nws >> STD::noskipws >> c;
    test_eq( c, ' ' );
    test_pass("cxx03:noskipws");
}

TEST_CASE(ios, fpos_streamoff) {
    STD::streampos p0(10);
    STD::streampos p1 = p0 + STD::streamoff(5);
    test_true( STD::streamoff(p1) == 15 );
    test_pass("cxx03:streamoff");
    test_true( (p1 - p0) == 5 );
    test_pass("cxx03:fpos arithmetic");
    test_true( p0 == STD::streampos(10) );
    test_true( p0 != p1 );
    test_pass("cxx03:streampos");
}

#if TEST_HAS_EH
TEST_CASE(ios, exceptions_mask) {
    STD::istringstream es("x");
    es.exceptions(STD::ios_base::failbit);
    bool threw = false;
    try { int z; es >> z; } catch (STD::ios_base::failure&) { threw = true; }
    test_true( threw );
    test_pass("cxx03:ios_base::failure");
    test_true( es.exceptions() == STD::ios_base::failbit );
    test_pass("cxx03:exceptions");
}
#else   // !TEST_HAS_EH
TEST_CASE(ios, exceptions_mask) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx03:ios_base::failure");
    test_skip("cxx03:exceptions");
}
#endif  // TEST_HAS_EH

TEST_CASE(ios, widen_narrow_copyfmt) {
    STD::ostringstream wn;
    test_eq( wn.widen('A'), 'A' );
    test_pass("cxx03:widen");
    test_eq( wn.narrow('B', '?'), 'B' );
    test_pass("cxx03:narrow");

    STD::ostringstream sb;
    sb << STD::showbase << STD::hex << 255;
    test_eq( sb.str(), STD::string("0xff") );
    test_pass("cxx03:showbase");

    STD::ostringstream cf1;
    cf1 << STD::hex << STD::showbase;
    STD::ostringstream cf2;
    cf2.copyfmt(cf1);
    cf2 << 255;
    test_eq( cf2.str(), STD::string("0xff") );
    test_pass("cxx03:copyfmt");
}

TEST_CASE(ios, xalloc_iword_pword) {
    int idx = STD::ios_base::xalloc();
    test_true( idx >= 0 );
    test_pass("cxx03:xalloc");
    STD::ostringstream xw;
    xw.iword(idx) = 42;
    xw.pword(idx) = &idx;
    test_eq( (long)xw.iword(idx), 42L );
    test_pass("cxx03:iword");
    test_true( xw.pword(idx) == &idx );
    test_pass("cxx03:pword");

    test_true( STD::ios_base::sync_with_stdio(true) == true );
    test_pass("cxx03:sync_with_stdio");
    { STD::ios_base::Init init_guard; (void)init_guard; }
    test_true( true );
    test_pass("cxx03:ios_base::Init");
}

TEST_CASE(ios, imbue_getloc) {
    STD::ostringstream os;
    test_true( os.getloc() == STD::locale::classic() );
    test_pass("cxx03:getloc");

    os.register_callback(&ios_imbue_callback, 17);
    ios_imbue_callback_hits = 0;

    STD::locale old = os.imbue(STD::locale("C"));
    test_true( old == STD::locale::classic() );
    test_true( os.getloc() == STD::locale::classic() );
    test_pass("cxx03:imbue");
    test_eq( ios_imbue_callback_hits, 1 );
    test_pass("cxx03:register_callback");

    test_true( os.rdbuf()->getloc() == STD::locale::classic() );
    test_pass("cxx03:rdbuf");
    test_true( os.rdbuf()->pubimbue(STD::locale()) == STD::locale::classic() );
    test_pass("cxx03:imbue forwards to rdbuf");

    STD::ostringstream os2;
    os2.copyfmt(os);
    test_true( os2.getloc() == os.getloc() );
    test_pass("cxx03:copyfmt carries locale");
}

TEST_CASE(ios, tie_rdbuf) {
    STD::ostringstream a, b;
    a.tie(&b);
    test_true( a.tie() != 0 );
    a.tie(0);
    test_true( a.tie() == 0 );
    test_pass("cxx03:tie");

    STD::streambuf* sbp = a.rdbuf();
    test_ptr( sbp );

    STD::ostream c(0);
    c.rdbuf(sbp);
    test_true( c.rdbuf() == sbp );
    c.rdbuf(0);
    test_pass("cxx03:rdbuf(set)");
}

TEST_CASE(ios, flag_constants) {
    typedef STD::ios_base B;

    B::fmtflags f = B::dec | B::hex | B::oct | B::left | B::right | B::internal
                  | B::scientific | B::fixed | B::boolalpha | B::showbase | B::showpoint
                  | B::showpos | B::skipws | B::unitbuf | B::uppercase;
    (void)f;
    test_true( B::dec != B::hex && (B::adjustfield & B::left) != 0
               && (B::basefield & B::dec) != 0 && (B::floatfield & B::fixed) != 0 );
    test_pass("cxx03:fmtflags constants");

    B::iostate st = B::goodbit | B::badbit | B::eofbit | B::failbit;
    (void)st;
    test_true( B::badbit != B::eofbit && B::failbit != B::goodbit );
    test_pass("cxx03:iostate constants");

    B::openmode om = B::in | B::out | B::binary | B::trunc | B::app | B::ate;
    (void)om;
    test_true( B::in != B::out && B::binary != B::trunc && B::app != B::ate );
    test_pass("cxx03:openmode constants (in/out/binary/trunc/app/ate)");

    test_true( B::beg != B::cur && B::cur != B::end && B::beg != B::end );
    test_pass("cxx03:seekdir constants (beg/cur/end)");
}

TEST_CASE(ios, negative_manipulators) {
    STD::ostringstream os;
    os << STD::showbase << STD::noshowbase;
    test_true( (os.flags() & STD::ios_base::showbase) == 0 );
    test_pass("cxx03:noshowbase");
    os << STD::showpoint << STD::noshowpoint;
    test_true( (os.flags() & STD::ios_base::showpoint) == 0 );
    test_pass("cxx03:noshowpoint");
    os << STD::showpos << STD::noshowpos;
    test_true( (os.flags() & STD::ios_base::showpos) == 0 );
    test_pass("cxx03:noshowpos");
    os << STD::unitbuf << STD::nounitbuf;
    test_true( (os.flags() & STD::ios_base::unitbuf) == 0 );
    test_pass("cxx03:nounitbuf");
    os << STD::uppercase << STD::nouppercase;
    test_true( (os.flags() & STD::ios_base::uppercase) == 0 );
    test_pass("cxx03:nouppercase");
}

TEST_CASE(ios, fill_fpos_events) {
    STD::ostringstream os;
    os.fill('*');
    test_eq( os.fill(), '*' );
    test_pass("cxx03:fill (set)");

    STD::fpos<STD::mbstate_t> p(5);
    STD::mbstate_t ms = p.state();
    (void)ms;
    test_true( true );
    test_pass("cxx03:fpos::state");

    g_ios_ev = 0;
    STD::stringstream s, s2;
    s.register_callback(ios_ev_cb, 0);
    s.copyfmt(s2);
    test_true( g_ios_ev > 0 );
    test_pass("cxx03:ios_base::event");
    test_pass("cxx03:ios_base::event_callback");

    test_skip("cxx03:init (protected)");
    test_skip("cxx11:basic_ios::move");
    test_skip("cxx11:basic_ios::swap");
}

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
typedef STD::io_errc::io_errc _tst_io_errc;
#else
typedef STD::io_errc _tst_io_errc;
#endif

TEST_CASE(ios, io_error_category) {

    test_true( STD::is_error_code_enum<_tst_io_errc>::value );
    test_pass("cxx11:is_error_code_enum<io_errc>");

    STD::error_code ec = STD::make_error_code(_tst_io_errc(STD::io_errc::stream));
    test_eq( ec.value(), 1 );
    test_pass("cxx11:io_errc");
    test_true( ec.category() == STD::iostream_category() );
    test_true( STD::string(ec.category().name()) == "iostream" );
    test_pass("cxx11:iostream_category");
    test_pass("cxx11:make_error_code(io_errc)");

    STD::error_condition cond = STD::make_error_condition(_tst_io_errc(STD::io_errc::stream));
    test_eq( cond.value(), 1 );
    test_pass("cxx11:make_error_condition(io_errc)");
}


namespace {
#if _TST_HAS_CXX11_LIB_MEMBERS
struct CcwIosProbe : public STD::basic_ios<char> {
    CcwIosProbe(STD::streambuf* sb) { this->init(sb); }
    void swap_buf(STD::streambuf* sb) { this->set_rdbuf(sb); }
};
#endif
}

TEST_CASE(ios, member_types_cxx03) {
    typedef STD::basic_ios<char> IOS;

    test_true(( STD::is_same<IOS::char_type, char>::value ));
    test_true(( STD::is_same<STD::basic_ios<wchar_t>::char_type, wchar_t>::value ));
    test_pass("cxx03:basic_ios::char_type");

    test_true(( STD::is_same<IOS::int_type, STD::char_traits<char>::int_type>::value ));
    test_pass("cxx03:basic_ios::int_type");

    test_true(( STD::is_same<IOS::pos_type, STD::char_traits<char>::pos_type>::value ));
    test_pass("cxx03:basic_ios::pos_type");

    test_true(( STD::is_same<IOS::off_type, STD::char_traits<char>::off_type>::value ));
    test_pass("cxx03:basic_ios::off_type");

    test_true(( STD::is_same<IOS::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx11:basic_ios::traits_type");

    test_true( STD::is_signed<STD::streamsize>::value );
    test_true( sizeof(STD::streamsize) >= sizeof(int) );
    STD::streamsize n = -1;
    test_true( n < 0 );
    test_pass("cxx03:streamsize");
}

TEST_CASE(ios, assignment_and_set_rdbuf_cxx03) {
#if !defined(__WATCOMC__)
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible/deleted copy assignment as assignable (vc bugs A26)");
    TEST_SKIP_CXX03_ACCESS("the C++03 is_copy_assignable cannot see access control");
    test_true( !STD::is_copy_assignable<STD::basic_ios<char> >::value );
    test_pass("cxx03:basic_ios::operator=");

    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible/deleted copy assignment as assignable (vc bugs A26)");
    TEST_SKIP_CXX03_ACCESS("the C++03 is_copy_assignable cannot see access control");
    test_true( !STD::is_copy_assignable<STD::ios_base>::value );
    test_pass("cxx03:ios_base::operator=");
#else
    TEST_NOTE("is_copy_assignable ODR-uses operator= on Open Watcom (bugs A47)");
    test_skip("cxx03:basic_ios::operator=");
    TEST_NOTE("is_copy_assignable ODR-uses operator= on Open Watcom (bugs A47)");
    test_skip("cxx03:ios_base::operator=");
#endif

#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::stringbuf a("one"), b("two");
    CcwIosProbe s(&a);
    test_true( s.rdbuf() == &a );
    s.setstate(STD::ios_base::failbit);
    s.swap_buf(&b);
    test_true( s.rdbuf() == &b );
    test_true( s.fail() );
    test_pass("cxx11:basic_ios::set_rdbuf");
#else
    TEST_NOTE("basic_ios::set_rdbuf is C++11; this library predates it");
    TEST_SKIP_N(3);
    test_skip("cxx11:basic_ios::set_rdbuf");
#endif
}

TEST_CASE(ios, fmtflags_constants_cxx03) {
    typedef STD::ios_base B;
    STD::ostringstream os;

    B::fmtflags saved = os.flags();
    os.flags(B::dec | B::showbase);
    test_true( (os.flags() & B::showbase) != 0 );
    os.flags(saved);
    test_true( tst_type_is<B::fmtflags>(os.flags()) );
    test_pass("cxx03:ios_base::fmtflags");

    test_true( B::dec != B::oct && B::dec != B::hex && B::oct != B::hex );
    test_true( (B::basefield & B::dec) == B::dec );
    os.setf(B::dec, B::basefield);
    os << 255;
    test_eq( os.str(), STD::string("255") );
    test_pass("cxx03:ios_base::dec");

    { STD::ostringstream o; o.setf(B::oct, B::basefield); o << 8;
      test_eq( o.str(), STD::string("10") ); }
    test_true( (B::basefield & B::oct) == B::oct );
    test_pass("cxx03:ios_base::oct");

    { STD::ostringstream o; o.setf(B::hex, B::basefield); o << 255;
      test_eq( o.str(), STD::string("ff") ); }
    test_true( (B::basefield & B::hex) == B::hex );
    test_pass("cxx03:ios_base::hex");

    test_true( B::basefield == (B::dec | B::oct | B::hex) );
    test_true( (B::basefield & B::left) == 0 );
    test_pass("cxx03:ios_base::basefield");

    { STD::ostringstream o; o.width(4); o.fill('.'); o.setf(B::left, B::adjustfield);
      o << 'x'; test_eq( o.str(), STD::string("x...") ); }
    test_pass("cxx03:ios_base::left");
    { STD::ostringstream o; o.width(4); o.fill('.'); o.setf(B::right, B::adjustfield);
      o << 'x'; test_eq( o.str(), STD::string("...x") ); }
    test_pass("cxx03:ios_base::right");
    { STD::ostringstream o; o.width(5); o.fill('.'); o.setf(B::internal, B::adjustfield);
      o << -1; test_eq( o.str(), STD::string("-...1") ); }
    test_pass("cxx03:ios_base::internal");

    test_true( B::adjustfield == (B::left | B::right | B::internal) );
    test_true( (B::adjustfield & B::dec) == 0 );
    test_pass("cxx03:ios_base::adjustfield");

    { STD::ostringstream o; o.setf(B::scientific, B::floatfield); o.precision(1);
      o << 1234.0; test_true( o.str().find('e') != STD::string::npos ); }
    test_pass("cxx03:ios_base::scientific");
    { STD::ostringstream o; o.setf(B::fixed, B::floatfield); o.precision(2);
      o << 1.5; test_eq( o.str(), STD::string("1.50") ); }
    test_pass("cxx03:ios_base::fixed");

    test_true( (B::floatfield & B::fixed) == B::fixed );
    test_true( (B::floatfield & B::scientific) == B::scientific );
    test_true( (B::floatfield & B::dec) == 0 );
    test_pass("cxx03:ios_base::floatfield");

    { STD::ostringstream o; o.setf(B::boolalpha); o << true;
      test_eq( o.str(), STD::string("true") ); }
    test_pass("cxx03:ios_base::boolalpha");
    { STD::ostringstream o; o.setf(B::showbase); o.setf(B::hex, B::basefield); o << 255;
      test_eq( o.str(), STD::string("0xff") ); }
    test_pass("cxx03:ios_base::showbase");
    { STD::ostringstream o; o.setf(B::showpoint); o << 1.0;
      test_true( o.str().find('.') != STD::string::npos ); }
    test_pass("cxx03:ios_base::showpoint");
    { STD::ostringstream o; o.setf(B::showpos); o << 5;
      test_eq( o.str(), STD::string("+5") ); }
    test_pass("cxx03:ios_base::showpos");
    {
        STD::istringstream i("  7");
        int v = 0; i >> v;
        test_eq( v, 7 );
        STD::istringstream j("  7");
        j.unsetf(B::skipws);
        int w = 0; j >> w;
        test_true( j.fail() );
    }
    test_pass("cxx03:ios_base::skipws");
    { STD::ostringstream o; o.setf(B::unitbuf);
      test_true( (o.flags() & B::unitbuf) != 0 ); o << "x";
      test_eq( o.str(), STD::string("x") ); }
    test_pass("cxx03:ios_base::unitbuf");
    { STD::ostringstream o; o.setf(B::uppercase); o.setf(B::hex, B::basefield); o << 255;
      test_eq( o.str(), STD::string("FF") ); }
    test_pass("cxx03:ios_base::uppercase");
}

TEST_CASE(ios, state_and_mode_constants_cxx03) {
    typedef STD::ios_base B;

    STD::istringstream is("1");
    test_true( tst_type_is<B::iostate>(is.rdstate()) );
    is.setstate(B::badbit);
    test_true( (is.rdstate() & B::badbit) != 0 );
    is.clear();
    test_pass("cxx03:ios_base::iostate");

    test_eq( (int)B::goodbit, 0 );
    test_true( is.rdstate() == B::goodbit && is.good() );
    test_pass("cxx03:ios_base::goodbit");

    { STD::istringstream i("x"); i.setstate(B::badbit);
      test_true( i.bad() && i.fail() && !i.good() ); }
    test_true( B::badbit != B::failbit && B::badbit != B::eofbit );
    test_pass("cxx03:ios_base::badbit");

    { STD::istringstream i("x"); int v; i >> v;
      test_true( i.fail() && !i.bad() ); }
    test_true( B::failbit != B::eofbit );
    test_pass("cxx03:ios_base::failbit");

    { STD::istringstream i("1"); int v; i >> v; char c; i.get(c);
      test_true( i.eof() ); }
    test_pass("cxx03:ios_base::eofbit");

    STD::ios_base::openmode om = B::in;
    test_true(( STD::is_same<B::openmode, STD::ios_base::openmode>::value ));
    om |= B::out;
    test_true( (om & B::in) != 0 && (om & B::out) != 0 );
    test_pass("cxx03:ios_base::openmode");

    test_true( B::app != B::binary && B::app != B::ate && B::app != B::trunc );
    { STD::stringbuf b(STD::string("ab"), B::out | B::app);
      test_true( b.sputc('c') == 'c' ); }
    test_pass("cxx03:ios_base::app");
    test_true( (B::binary & (B::in | B::out)) == 0 );
    test_pass("cxx03:ios_base::binary");
    { STD::stringbuf b(STD::string("ab"), B::in);
      test_eq( (int)b.sgetc(), (int)'a' );
      test_true( b.sputc('z') == STD::char_traits<char>::eof() );
    }
    test_pass("cxx03:ios_base::in");
    { STD::stringbuf b(STD::string(""), B::out);
      test_true( b.sputc('z') == 'z' );
      test_true( b.sgetc() == STD::char_traits<char>::eof() );
    }
    test_pass("cxx03:ios_base::out");
    test_true( B::trunc != B::in && B::trunc != B::out );
    test_pass("cxx03:ios_base::trunc");
    { STD::stringbuf b(STD::string("abc"), B::in | B::out | B::ate);
      test_true( b.sputc('d') == 'd' );
      test_eq( b.str(), STD::string("abcd") );
    }
    test_pass("cxx03:ios_base::ate");

    STD::ios_base::seekdir sd = B::beg;
    (void)sd;
    test_true( B::beg != B::cur && B::cur != B::end && B::beg != B::end );
    test_pass("cxx03:ios_base::seekdir");

    STD::istringstream sk("0123456789");
    sk.seekg(2, B::beg);
    test_eq( (long)sk.tellg(), 2L );
    test_pass("cxx03:ios_base::beg");
    sk.seekg(3, B::cur);
    test_eq( (long)sk.tellg(), 5L );
    test_pass("cxx03:ios_base::cur");
    sk.seekg(-1, B::end);
    test_eq( (long)sk.tellg(), 9L );
    test_pass("cxx03:ios_base::end");

#if defined(__cpp_lib_ios_noreplace) && (__cpp_lib_ios_noreplace) >= 202207L
    test_true( B::noreplace != B::trunc && B::noreplace != B::app );
    STD::ios_base::openmode nr = B::out | B::noreplace;
    test_true( (nr & B::noreplace) != 0 );
    test_pass("cxx23:ios_base::noreplace");
#else
    TEST_NOTE("ios_base::noreplace is C++23 (P2467); not in this library");
    test_skip("cxx23:ios_base::noreplace");
#endif
}

namespace {
int g_ev_erase = 0, g_ev_imbue = 0, g_ev_copyfmt = 0;
void ios_ev_count(STD::ios_base::event e, STD::ios_base&, int) {
    if (e == STD::ios_base::erase_event)        ++g_ev_erase;
    else if (e == STD::ios_base::imbue_event)   ++g_ev_imbue;
    else if (e == STD::ios_base::copyfmt_event) ++g_ev_copyfmt;
}
}

TEST_CASE(ios, event_enumerators_cxx03) {
    test_true( STD::ios_base::erase_event != STD::ios_base::imbue_event );
    test_true( STD::ios_base::imbue_event != STD::ios_base::copyfmt_event );
    test_true( STD::ios_base::erase_event != STD::ios_base::copyfmt_event );

    g_ev_erase = g_ev_imbue = g_ev_copyfmt = 0;
    {
        STD::ostringstream os;
        os.register_callback(ios_ev_count, 0);

        os.imbue(STD::locale::classic());
        test_true( g_ev_imbue >= 1 );
        test_pass("cxx03:ios_base::imbue_event");

        STD::ostringstream other;
        other.register_callback(ios_ev_count, 0);
        os.copyfmt(other);
        test_true( g_ev_erase >= 1 );
        test_pass("cxx03:ios_base::erase_event");
        test_true( g_ev_copyfmt >= 1 );
        test_pass("cxx03:ios_base::copyfmt_event");
    }
}

TEST_CASE(ios, io_errc_stream_cxx11) {
    STD::error_code ec = STD::make_error_code(STD::io_errc::stream);
    test_true( ec.category() == STD::iostream_category() );
    const int io_stream_value = static_cast<int>(STD::io_errc::stream);
    test_eq( ec.value(), io_stream_value );
#if !defined(__WATCOMC__) && _TST_HAS_SCOPED_ENUM
    test_true( ec == STD::io_errc::stream );
#else
    TEST_SKIP1();
#endif
    test_true( io_stream_value != 0 );
    STD::error_condition eco = STD::make_error_condition(STD::io_errc::stream);
    test_true( eco.category() == STD::iostream_category() );
    test_pass("cxx11:io_errc::stream");
}

TEST_CASE(ios, state_conversion_operators) {
    STD::istringstream good("1");
    STD::istringstream bad("x");
    int v = 0;
    bad >> v;

#if _TST_HAS_CXX11_LIB_MEMBERS
    test_true( (bool)good );
    test_true( !(bool)bad );
    if (good) test_true( true ); else test_true( false );
#if !defined(__WATCOMC__)
    test_true(( !STD::is_convertible<STD::istringstream&, bool>::value ));
    test_true(( !STD::is_convertible<STD::istringstream&, void*>::value ));
    test_pass("cxx11:basic_ios::operator bool");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:basic_ios::operator bool");
#endif
    TEST_NOTE("C++11 replaced operator void* with the explicit operator bool");
    test_skip("cxx03:basic_ios::operator void*");
#else
    test_true( good != 0 );
    test_true( bad == 0 );
    void* p = good;
    test_ptr( p );
    test_pass("cxx03:basic_ios::operator void*");
    test_skip("cxx11:basic_ios::operator bool");
#endif
    test_true( !good.fail() );
    test_true( bad.fail() );
}

TEST_CASE(ios, feature_test_macros) {
#if defined(__cpp_lib_ios_noreplace)
    test_true( __cpp_lib_ios_noreplace > 0L );
    test_pass("cxx23:__cpp_lib_ios_noreplace");
#else
    test_skip("cxx23:__cpp_lib_ios_noreplace");
#endif
}
