#include "test_cxx.hpp"

#include <locale>
#include <string>
#include <sstream>
#include <iterator>
#include <ctime>
#include <cstring>
#include <cstdio>

TEST_CASE(locale, classic) {
    STD::locale loc = STD::locale::classic();
    test_eq( loc.name(), STD::string("C") );
    test_pass("cxx03:locale::classic");
    test_true( STD::locale() == STD::locale::classic() );
    test_pass("cxx03:locale default==classic");
}

TEST_CASE(locale, construct_and_combine) {
    STD::locale c = STD::locale::classic();

    STD::locale a(c, "C", STD::locale::all);
    test_true( STD::has_facet<STD::ctype<char> >(a) );
    test_pass("cxx03:locale(loc,name,cat) ctor");
#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::locale b(c, STD::string("C"), STD::locale::ctype);
    test_true( STD::has_facet<STD::numpunct<char> >(b) );
    test_pass("cxx03:locale(loc,string,cat) ctor");
#else
    test_skip("cxx03:locale(loc,string,cat) ctor");
#endif
    STD::locale d(c, c, STD::locale::numeric);
    test_true( STD::has_facet<STD::num_put<char> >(d) );
    test_pass("cxx03:locale(loc,loc,cat) ctor");

    STD::locale e(c, new STD::numpunct<char>());
    test_true( STD::has_facet<STD::numpunct<char> >(e) );
    const STD::numpunct<char>& np = STD::use_facet<STD::numpunct<char> >(e);
    test_eq( np.decimal_point(), '.' );

    test_true( STD::has_facet<STD::ctype<char> >(e) );
    test_pass("cxx03:locale(loc,facet*) ctor");

    STD::locale f = e.combine<STD::ctype<char> >(c);
    test_true( STD::has_facet<STD::ctype<char> >(f) );
    const STD::ctype<char>& ctf = STD::use_facet<STD::ctype<char> >(f);
    test_true( ctf.is(STD::ctype_base::alpha, 'z') );
    test_pass("cxx03:locale::combine");
}

TEST_CASE(locale, ctype_facet) {
    STD::locale loc = STD::locale::classic();
    const STD::ctype<char>& ct = STD::use_facet<STD::ctype<char> >(loc);

    test_true(  ct.is(STD::ctype_base::alpha, 'x') );
    test_true( !ct.is(STD::ctype_base::alpha, '3') );
    test_true(  ct.is(STD::ctype_base::digit, '7') );
    test_true(  ct.is(STD::ctype_base::space, ' ') );
    test_pass("cxx03:ctype<char>::is");
    test_eq( ct.toupper('a'), 'A' );
    test_pass("cxx03:ctype<char>::toupper");
    test_eq( ct.tolower('Z'), 'z' );
    test_pass("cxx03:ctype<char>::tolower");

    test_true(  STD::isalpha('q', loc) );
    test_true( !STD::isalpha('4', loc) );
    test_true(  STD::isdigit('4', loc) );
    test_true(  STD::isalnum('q', loc) );
    test_pass("cxx03:isalpha(loc) family");
    test_eq( STD::toupper('m', loc), 'M' );
    test_pass("cxx03:toupper(loc)");
    test_eq( STD::tolower('M', loc), 'm' );
    test_pass("cxx03:tolower(loc)");
    test_true(  STD::isblank(' ', loc) );
    test_true(  STD::isblank('\t', loc) );
    test_true( !STD::isblank('x', loc) );
    test_pass("cxx11:isblank(loc)");

    test_eq( (long)STD::ctype<char>::table_size, 256L );
    test_pass("cxx03:ctype<char>::table_size");
#if _TST_HAS_CXX11_LIB_MEMBERS
    const STD::ctype_base::mask* tab = ct.table();
    test_ptr( (void*)tab );
    test_true( (tab[(unsigned char)'x'] & STD::ctype_base::alpha) != 0 );
    test_true( (tab[(unsigned char)'5'] & STD::ctype_base::digit) != 0 );
    test_pass("cxx03:ctype<char>::table");
    const STD::ctype_base::mask* ctab = STD::ctype<char>::classic_table();
    test_ptr( (void*)ctab );
    test_true( (ctab[(unsigned char)'x'] & STD::ctype_base::alpha) != 0 );
    test_pass("cxx03:ctype<char>::classic_table");
#else
    test_skip("cxx03:ctype<char>::table");
    test_skip("cxx03:ctype<char>::classic_table");
#endif

    const char* s = "a1 ";
    STD::ctype_base::mask m[3];
    ct.is(s, s + 3, m);
    test_true( (m[0] & STD::ctype_base::alpha) != 0 );
    test_true( (m[1] & STD::ctype_base::digit) != 0 );
    test_true( (m[2] & STD::ctype_base::space) != 0 );
    test_pass("cxx03:ctype<char>::is(range)");

    const char* txt = "  abc";
    test_eq( *ct.scan_is(STD::ctype_base::alpha, txt, txt + 5), 'a' );
    test_pass("cxx03:ctype<char>::scan_is");
    test_eq( *ct.scan_not(STD::ctype_base::space, txt, txt + 5), 'a' );
    test_pass("cxx03:ctype<char>::scan_not");
}

TEST_CASE(locale, numpunct_and_collate) {
    STD::locale loc = STD::locale::classic();

    test_true( STD::has_facet<STD::numpunct<char> >(loc) );
    test_pass("cxx03:has_facet");
    const STD::numpunct<char>& np = STD::use_facet<STD::numpunct<char> >(loc);
    test_eq( np.decimal_point(), '.' );
    test_eq( np.thousands_sep(), ',' );
    test_eq( np.truename(), STD::string("true") );
    test_pass("cxx03:numpunct<char>");

    const STD::collate<char>& col = STD::use_facet<STD::collate<char> >(loc);
    const char* a1 = "abc";
    const char* a2 = "abd";
    test_lt( col.compare(a1, a1 + 3, a2, a2 + 3), 0 );
    test_eq( col.compare(a1, a1 + 3, a1, a1 + 3), 0 );
    test_pass("cxx03:collate<char>");
}

TEST_CASE(locale, num_put_facet) {
    const STD::num_put<char>& np = STD::use_facet<STD::num_put<char> >(STD::locale::classic());

    STD::ostringstream a;
    np.put(STD::ostreambuf_iterator<char>(a), a, a.fill(), (long)42);
    test_eq( a.str(), STD::string("42") );
    test_pass("cxx03:num_put::put(long)");

    STD::ostringstream n; np.put(STD::ostreambuf_iterator<char>(n), n, n.fill(), (long)-7);
    test_eq( n.str(), STD::string("-7") );
    test_pass("cxx03:num_put::put negative");

    STD::ostringstream x; x << STD::hex; np.put(STD::ostreambuf_iterator<char>(x), x, x.fill(), (unsigned long)255);
    test_eq( x.str(), STD::string("ff") );
    test_pass("cxx03:num_put::put hex");

    STD::ostringstream h; h << STD::hex << STD::showbase << STD::uppercase;
    np.put(STD::ostreambuf_iterator<char>(h), h, h.fill(), (unsigned long)255);
    test_eq( h.str(), STD::string("0XFF") );
    test_pass("cxx03:num_put::put showbase");

    STD::ostringstream o; o << STD::oct; np.put(STD::ostreambuf_iterator<char>(o), o, o.fill(), (long)9);
    test_eq( o.str(), STD::string("11") );
    test_pass("cxx03:num_put::put oct");

    STD::ostringstream sp; sp << STD::showpos; np.put(STD::ostreambuf_iterator<char>(sp), sp, sp.fill(), (long)5);
    test_eq( sp.str(), STD::string("+5") );
    test_pass("cxx03:num_put::put showpos");

    STD::ostringstream r; r.width(5); r.fill('*'); np.put(STD::ostreambuf_iterator<char>(r), r, r.fill(), (long)42);
    test_eq( r.str(), STD::string("***42") );
    test_pass("cxx03:num_put::put width");
    test_true( 1 );
    test_pass("cxx03:num_put::put fill");
    STD::ostringstream l; l << STD::left; l.width(5); l.fill('*'); np.put(STD::ostreambuf_iterator<char>(l), l, l.fill(), (long)42);
    test_eq( l.str(), STD::string("42***") );
    test_pass("cxx03:num_put::put left");
    STD::ostringstream in; in << STD::internal << STD::showpos; in.width(5); in.fill('0');
    np.put(STD::ostreambuf_iterator<char>(in), in, in.fill(), (long)42);
    test_eq( in.str(), STD::string("+0042") );
    test_pass("cxx03:num_put::put internal");

    STD::ostringstream f; f << STD::fixed; f.precision(2); np.put(STD::ostreambuf_iterator<char>(f), f, f.fill(), 3.14159);
    test_eq( f.str(), STD::string("3.14") );
    test_pass("cxx03:num_put::put fixed");

    STD::ostringstream ba; ba << STD::boolalpha; np.put(STD::ostreambuf_iterator<char>(ba), ba, ba.fill(), true);
    test_eq( ba.str(), STD::string("true") );
    test_pass("cxx03:num_put::put boolalpha");
    STD::ostringstream nb; np.put(STD::ostreambuf_iterator<char>(nb), nb, nb.fill(), false);
    test_eq( nb.str(), STD::string("0") );
    test_pass("cxx03:num_put::put bool");
}

TEST_CASE(locale, num_get_facet) {
    const STD::num_get<char>& ng = STD::use_facet<STD::num_get<char> >(STD::locale::classic());
    STD::istreambuf_iterator<char> e;
    STD::ios_base::iostate err;

    {   STD::istringstream is("42");   long v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_eq( v, 42L );  test_true( (err & STD::ios_base::failbit) == 0 ); }
    test_pass("cxx03:num_get::get(long)");

    {   STD::istringstream is("-7");   long v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_eq( v, -7L ); }
    test_pass("cxx03:num_get::get negative");

    {   STD::istringstream is("ff");   is >> STD::hex; unsigned long v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_eq( (long)v, 255L ); }
    test_pass("cxx03:num_get::get hex");

    {   STD::istringstream is("3.5");  double v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_true( v == 3.5 ); }
    test_pass("cxx03:num_get::get(double)");

    {   STD::istringstream is("xyz");  long v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_true( (err & STD::ios_base::failbit) != 0 ); }
    test_pass("cxx03:num_get::get failbit");

    {   STD::istringstream is("true"); is >> STD::boolalpha; bool v = false; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_true( v ); }
    test_pass("cxx03:num_get::get(bool)");
}

TEST_CASE(locale, time_put_facet) {
    const STD::time_put<char>& tp = STD::use_facet<STD::time_put<char> >(STD::locale::classic());
    STD::tm t; STD::memset(&t, 0, sizeof t);
    t.tm_year = 121; t.tm_mon = 6; t.tm_mday = 13; t.tm_hour = 9; t.tm_min = 5; t.tm_sec = 30; t.tm_wday = 2;

    STD::ostringstream y;
    tp.put(STD::ostreambuf_iterator<char>(y), y, y.fill(), &t, 'Y');
    test_eq( y.str(), STD::string("2021") );
    test_pass("cxx03:time_put::put spec");

    const char* pat = "%Y-%m-%d";
    STD::ostringstream d;
    tp.put(STD::ostreambuf_iterator<char>(d), d, d.fill(), &t, pat, pat + STD::strlen(pat));
    test_eq( d.str(), STD::string("2021-07-13") );
    test_pass("cxx03:time_put::put pattern");

    const char* tf = "%H:%M:%S";
    STD::ostringstream tm;
    tp.put(STD::ostreambuf_iterator<char>(tm), tm, tm.fill(), &t, tf, tf + STD::strlen(tf));
    test_eq( tm.str(), STD::string("09:05:30") );
    test_pass("cxx03:time_put::put time pattern");
}

TEST_CASE(locale, time_get_facet) {
    const STD::time_get<char>& tg = STD::use_facet<STD::time_get<char> >(STD::locale::classic());
    STD::istreambuf_iterator<char> e;
    STD::ios_base::iostate err;

    {   STD::istringstream is("09:05:30"); STD::tm t; STD::memset(&t, 0, sizeof t); err = STD::ios_base::goodbit;
        tg.get_time(STD::istreambuf_iterator<char>(is), e, is, err, &t);
        test_true( (err & STD::ios_base::failbit) == 0 );
        test_eq( t.tm_hour, 9 ); test_eq( t.tm_min, 5 ); test_eq( t.tm_sec, 30 ); }
    test_pass("cxx03:time_get::get_time");

    {   STD::time_base::dateorder ord = tg.date_order();
        const char* ds = (ord == STD::time_base::dmy) ? "13/07/2021"
                       : (ord == STD::time_base::ymd) ? "2021/07/13"
                       : (ord == STD::time_base::ydm) ? "2021/13/07"
                       :                                "07/13/2021";
        STD::istringstream is(ds); STD::tm t; STD::memset(&t, 0, sizeof t); err = STD::ios_base::goodbit;
        tg.get_date(STD::istreambuf_iterator<char>(is), e, is, err, &t);
        test_eq( t.tm_mon, 6 ); test_eq( t.tm_mday, 13 ); test_eq( t.tm_year, 121 ); }
    test_pass("cxx03:time_get::get_date");

    {   STD::istringstream is("July"); STD::tm t; STD::memset(&t, 0, sizeof t); err = STD::ios_base::goodbit;
        tg.get_monthname(STD::istreambuf_iterator<char>(is), e, is, err, &t);
        test_eq( t.tm_mon, 6 ); }
    test_pass("cxx03:time_get::get_monthname");
    {   STD::istringstream is("Tue"); STD::tm t; STD::memset(&t, 0, sizeof t); err = STD::ios_base::goodbit;
        tg.get_weekday(STD::istreambuf_iterator<char>(is), e, is, err, &t);
        test_eq( t.tm_wday, 2 ); }
    test_pass("cxx03:time_get::get_weekday");

    {   STD::istringstream is("2021"); STD::tm t; STD::memset(&t, 0, sizeof t); err = STD::ios_base::goodbit;
        tg.get_year(STD::istreambuf_iterator<char>(is), e, is, err, &t);
        test_eq( t.tm_year, 121 ); }
    test_pass("cxx03:time_get::get_year");

    {   STD::time_base::dateorder ord = tg.date_order();
        test_true( ord == STD::time_base::no_order || ord == STD::time_base::dmy
                || ord == STD::time_base::mdy      || ord == STD::time_base::ymd
                || ord == STD::time_base::ydm ); }
    test_pass("cxx03:time_get::date_order");

#if _TST_HAS_CXX11_LIB_MEMBERS
    {   STD::istringstream is("2021"); STD::tm t; STD::memset(&t, 0, sizeof t); err = STD::ios_base::goodbit;
        tg.get(STD::istreambuf_iterator<char>(is), e, is, err, &t, 'Y');
        test_eq( t.tm_year, 121 ); }
    test_pass("cxx03:time_get::get");
#else
    test_skip("cxx03:time_get::get");
#endif
}

typedef STD::moneypunct<char, false> ccw_mpf;
typedef STD::moneypunct<char, true>  ccw_mpt;

TEST_CASE(locale, moneypunct_facet) {
    STD::locale loc = STD::locale::classic();
    test_true( STD::has_facet<ccw_mpf>(loc) );
    const ccw_mpf& mp = STD::use_facet<ccw_mpf>(loc);

    (void)mp.decimal_point();
    (void)mp.thousands_sep();
    (void)mp.negative_sign();
    test_eq( mp.frac_digits(), 0 );
    test_true( mp.curr_symbol().empty() );
    test_true( mp.positive_sign().empty() );
    test_pass("cxx03:moneypunct fields");
    bool intl_f = ccw_mpf::intl; bool intl_t = ccw_mpt::intl;
    test_true( !intl_f );
    test_true( intl_t );
    test_pass("cxx03:moneypunct::intl");

    STD::money_base::pattern p = mp.pos_format();
    test_true( p.field[3] == (char)STD::money_base::value ||
               p.field[0] == (char)STD::money_base::value ||
               p.field[1] == (char)STD::money_base::value ||
               p.field[2] == (char)STD::money_base::value );
    test_pass("cxx03:moneypunct::pos_format");
}

TEST_CASE(locale, money_put_get_facet) {
    STD::locale loc = STD::locale::classic();
    const STD::money_put<char>& mput = STD::use_facet<STD::money_put<char> >(loc);
    const STD::money_get<char>& mget = STD::use_facet<STD::money_get<char> >(loc);

    {   STD::ostringstream os;
        mput.put(STD::ostreambuf_iterator<char>(os), false, os, os.fill(), (long double)12345);
        test_eq( os.str(), STD::string("12345") ); }
    test_pass("cxx03:money_put::put(long double)");

    {   STD::ostringstream os;
        mput.put(STD::ostreambuf_iterator<char>(os), false, os, os.fill(), STD::string("6789"));
        test_eq( os.str(), STD::string("6789") ); }
    test_pass("cxx03:money_put::put(string)");

    {   STD::ostringstream os;
        mput.put(STD::ostreambuf_iterator<char>(os), false, os, os.fill(), (long double)-42);
        TEST_SKIP_GCC();
        test_eq( os.str(), STD::string("-42") ); }
    test_pass("cxx03:money_put::put negative");

    {   STD::istringstream is("12345"); STD::istreambuf_iterator<char> e; STD::ios_base::iostate err = STD::ios_base::goodbit;
        long double v = 0;
        mget.get(STD::istreambuf_iterator<char>(is), e, false, is, err, v);
        test_true( (err & STD::ios_base::failbit) == 0 );
        test_true( v == (long double)12345 ); }
    test_pass("cxx03:money_get::get(long double)");

    {   STD::istringstream is("6789"); STD::istreambuf_iterator<char> e; STD::ios_base::iostate err = STD::ios_base::goodbit;
        STD::string digs;
        mget.get(STD::istreambuf_iterator<char>(is), e, false, is, err, digs);
        test_eq( digs, STD::string("6789") ); }
    test_pass("cxx03:money_get::get(string)");
}

TEST_CASE(locale, codecvt_facet) {
    STD::locale loc = STD::locale::classic();
    typedef STD::codecvt<char, char, STD::mbstate_t> cvt;
    test_true( STD::has_facet<cvt>(loc) );
    test_pass("cxx03:has_facet<codecvt>");
    const cvt& cv = STD::use_facet<cvt>(loc);

    test_true( cv.always_noconv() );
    test_pass("cxx03:codecvt always_noconv");
    test_eq( cv.encoding(), 1 );
    test_pass("cxx03:codecvt encoding");
    test_eq( cv.max_length(), 1 );
    test_pass("cxx03:codecvt max_length");

    STD::mbstate_t st; STD::memset(&st, 0, sizeof st);
    const char* src = "hello"; const char* fn; char buf[8]; char* tn;
    cvt::result r = cv.out(st, src, src + 5, fn, buf, buf + 8, tn);
    test_true( r == cvt::noconv );
    test_pass("cxx03:codecvt::out");

    STD::mbstate_t st2; STD::memset(&st2, 0, sizeof st2);
    test_eq( cv.length(st2, src, src + 5, 3), 3 );
    test_eq( cv.length(st2, src, src + 5, 10), 5 );
    test_pass("cxx03:codecvt::length");
}

TEST_CASE(locale, messages_facet) {
    STD::locale loc = STD::locale::classic();
    test_true( STD::has_facet<STD::messages<char> >(loc) );
    test_pass("cxx03:has_facet<messages>");
    const STD::messages<char>& ms = STD::use_facet<STD::messages<char> >(loc);

    STD::messages<char>::catalog cat = ms.open(STD::string("ccw-no-such-catalog"), loc);
    TEST_SKIP_GCC();
    test_true( cat == (STD::messages<char>::catalog)-1 );
    test_pass("cxx03:messages::open");
    STD::string def = "default-text";
    test_eq( ms.get(cat, 0, 0, def), def );
    test_pass("cxx03:messages::get");
    ms.close(cat);
    test_true( true );
    test_pass("cxx03:messages::close");

#if defined(_CCW_HAS_MESSAGE_CATALOG)
    {
        const char* mp = "ccw_msgtest.msg";
        STD::FILE*  fp = STD::fopen(mp, "wb");
        if (fp) {
            STD::fputs("$set 1\n1 Hello\n2 Two\\nlines\n$set 5\n1 five-one\n", fp);
            STD::fclose(fp);
            STD::messages<char>::catalog c2 = ms.open(STD::string(mp), loc);
            if (c2 >= 0) {
                test_eq( ms.get(c2, 1, 1, def), STD::string("Hello") );
                test_eq( ms.get(c2, 1, 2, def), STD::string("Two\nlines") );
                test_eq( ms.get(c2, 5, 1, def), STD::string("five-one") );
                test_eq( ms.get(c2, 9, 9, def), def );
                ms.close(c2);
            } else {
                TEST_NOTE("messages::open does not read a gencat source catalog here");
            }
            STD::remove(mp);
        }
    }
#else
    TEST_NOTE("the catalog format is unspecified; only the ccwrap reader takes gencat source");
#endif
}

typedef STD::moneypunct_byname<char, false> ccw_mpf_byname;

TEST_CASE(locale, byname_facets) {

    STD::ctype_byname<char>* ct = new STD::ctype_byname<char>("C");
    test_true(  ct->is(STD::ctype_base::alpha, 'x') );
    test_true( !ct->is(STD::ctype_base::alpha, '3') );
    test_pass("cxx03:ctype_byname");

    STD::numpunct_byname<char>* np = new STD::numpunct_byname<char>("C");
    test_eq( np->decimal_point(), '.' );
    test_pass("cxx03:numpunct_byname");

    STD::collate_byname<char>* co = new STD::collate_byname<char>("C");
    const char* a = "abc"; const char* b = "abd";
    test_lt( co->compare(a, a + 3, b, b + 3), 0 );
    test_pass("cxx03:collate_byname");

    ccw_mpf_byname* mp = new ccw_mpf_byname("C");
    test_eq( mp->frac_digits(), 0 );
    test_pass("cxx03:moneypunct_byname");

    STD::messages_byname<char>* ms = new STD::messages_byname<char>("C");
    STD::string d = "x";
    test_eq( ms->get((STD::messages<char>::catalog)-1, 0, 0, d), d );
    test_pass("cxx03:messages_byname");

    STD::time_put_byname<char>* tp = new STD::time_put_byname<char>("C");
    STD::tm t; STD::memset(&t, 0, sizeof t); t.tm_year = 121;
    STD::ostringstream os;
    tp->put(STD::ostreambuf_iterator<char>(os), os, os.fill(), &t, 'Y');
    test_eq( os.str(), STD::string("2021") );
    test_pass("cxx03:time_put_byname");

#if TEST_HAS_EH
    {
        static const char* const cand[] = {
            "en-US", "en_US.UTF-8", "en_US.utf8", "English_United States.1252", "en_US", 0
        };
        bool got = false;
        for (int i = 0; cand[i] != 0 && !got; ++i) {
            try {
                STD::locale l(cand[i]);
                const STD::numpunct<char>&        n  = STD::use_facet<STD::numpunct<char> >(l);
                const STD::moneypunct<char, false>& m  = STD::use_facet<STD::moneypunct<char, false> >(l);
                const STD::moneypunct<char, true>&  mi = STD::use_facet<STD::moneypunct<char, true> >(l);
                got = true;
                test_true( !l.name().empty() );
                test_eq( n.decimal_point(), '.' );
                test_eq( n.thousands_sep(), ',' );
                test_true( !m.curr_symbol().empty() );
                test_eq( m.frac_digits(), 2 );
                test_true( !mi.curr_symbol().empty() );

                STD::tm lt; STD::memset(&lt, 0, sizeof lt);
                lt.tm_wday = 2; lt.tm_mon = 6; lt.tm_mday = 13; lt.tm_year = 121;
                STD::ostringstream lo; lo.imbue(l);
                const STD::time_put<char>& ltp = STD::use_facet<STD::time_put<char> >(l);
                const char lf[] = "%A";
                ltp.put(STD::ostreambuf_iterator<char>(lo), lo, ' ', &lt, lf, lf + 2);
                STD::string lday = lo.str();
                test_true( !lday.empty() );

                STD::istringstream li(lday); li.imbue(l);
                STD::tm lt2; STD::memset(&lt2, 0, sizeof lt2);
                STD::ios_base::iostate lerr = STD::ios_base::goodbit;
                STD::use_facet<STD::time_get<char> >(l).get_weekday(
                    STD::istreambuf_iterator<char>(li), STD::istreambuf_iterator<char>(),
                    li, lerr, &lt2);
                test_eq( lt2.tm_wday, 2 );

                const STD::collate<char>& lco = STD::use_facet<STD::collate<char> >(l);
                const char* ca = "apple"; const char* cb = "Banana";
                int cr = lco.compare(ca, ca + 5, cb, cb + 6);
                test_true( cr != 0 );
                test_eq( lco.transform(ca, ca + 5) < lco.transform(cb, cb + 6), cr < 0 );

                const STD::ctype<char>& lct = STD::use_facet<STD::ctype<char> >(l);
                test_eq( lct.toupper('a'), 'A' );
                test_true( lct.is(STD::ctype_base::alpha, 'x') );
            } catch (...) {
            }
        }
        if (!got)
            TEST_NOTE("no en-US locale is installed here; the byname facets were only checked against \"C\"");
    }
#endif
}

TEST_CASE(locale, locale_object_operations) {
    STD::locale c = STD::locale::classic();

    const STD::ctype<char>& ct = STD::use_facet<STD::ctype<char> >(c);
    test_true( ct.is(STD::ctype_base::alpha, 'a') );
    test_pass("cxx03:use_facet");

    test_eq( c.name(), STD::string("C") );
    test_pass("cxx03:locale::name");

    STD::locale cc("C");
    test_eq( cc.name(), STD::string("C") );
    test_pass("cxx03:locale(name) ctor");

    test_true( c == STD::locale::classic() );
    test_pass("cxx03:locale::operator==");
    STD::locale c2 = STD::locale::classic();
    test_true( !(c2 != STD::locale::classic()) );
    test_pass("cxx03:locale::operator!=");

    STD::locale prev = STD::locale::global(STD::locale::classic());
    STD::locale::global(prev);
    test_pass("cxx03:locale::global");

    test_true(  c(STD::string("a"), STD::string("b")) );
    test_true( !c(STD::string("b"), STD::string("a")) );
    test_pass("cxx03:locale::operator() (facet string compare)");
}

TEST_CASE(locale, ctype_members_and_classification) {
    STD::locale c = STD::locale::classic();
    const STD::ctype<char>& ct = STD::use_facet<STD::ctype<char> >(c);

    test_eq( ct.widen('A'), 'A' );
    test_pass("cxx03:ctype<char>::widen");
    test_eq( ct.narrow('A', '?'), 'A' );
    test_pass("cxx03:ctype<char>::narrow");

    const STD::ctype<wchar_t>& wct = STD::use_facet<STD::ctype<wchar_t> >(c);
    test_true(  wct.is(STD::ctype_base::alpha, L'a') );
    test_true( !wct.is(STD::ctype_base::digit, L'a') );
    test_pass("cxx03:ctype<wchar_t>");

    test_true(  STD::isalnum('a', c) ); test_true( !STD::isalnum(' ', c) );
    test_pass("cxx03:isalnum(loc)");
    test_true(  STD::isdigit('7', c) ); test_true( !STD::isdigit('x', c) );
    test_pass("cxx03:isdigit(loc)");
    test_true(  STD::isxdigit('f', c) ); test_true( !STD::isxdigit('g', c) );
    test_pass("cxx03:isxdigit(loc)");
    test_true(  STD::islower('a', c) ); test_true( !STD::islower('A', c) );
    test_pass("cxx03:islower(loc)");
    test_true(  STD::isupper('A', c) ); test_true( !STD::isupper('a', c) );
    test_pass("cxx03:isupper(loc)");
    test_true(  STD::isspace(' ', c) ); test_true( !STD::isspace('a', c) );
    test_pass("cxx03:isspace(loc)");
    test_true(  STD::ispunct('!', c) ); test_true( !STD::ispunct('a', c) );
    test_pass("cxx03:ispunct(loc)");
    test_true(  STD::iscntrl('\n', c) ); test_true( !STD::iscntrl('a', c) );
    test_pass("cxx03:iscntrl(loc)");
    test_true(  STD::isprint('a', c) ); test_true( !STD::isprint('\n', c) );
    test_pass("cxx03:isprint(loc)");
    test_true(  STD::isgraph('a', c) ); test_true( !STD::isgraph(' ', c) );
    test_pass("cxx03:isgraph(loc)");
}

TEST_CASE(locale, collate_and_numpunct_members) {
    STD::locale c = STD::locale::classic();

    const STD::collate<char>& col = STD::use_facet<STD::collate<char> >(c);
    const char* a = "abc"; const char* b = "abd";
    test_true( col.compare(a, a + 3, b, b + 3) < 0 );
    test_true( col.compare(a, a + 3, a, a + 3) == 0 );
    test_pass("cxx03:collate<char>::compare");
    STD::string tr = col.transform(a, a + 3);
    test_true( !tr.empty() );
    test_pass("cxx03:collate<char>::transform");
    long h1 = col.hash(a, a + 3);
    long h2 = col.hash(a, a + 3);
    test_eq( h1, h2 );
    test_pass("cxx03:collate<char>::hash");

    const STD::numpunct<char>& np = STD::use_facet<STD::numpunct<char> >(c);
    test_eq( np.decimal_point(), '.' );
    test_pass("cxx03:numpunct decimal_point");
    char ts = np.thousands_sep(); (void)ts;
    test_pass("cxx03:numpunct thousands_sep");
    test_true( np.grouping().empty() );
    test_pass("cxx03:numpunct grouping");
    test_eq( np.truename(), STD::string("true") );
    test_pass("cxx03:numpunct truename");
    test_eq( np.falsename(), STD::string("false") );
    test_pass("cxx03:numpunct falsename");
}

TEST_CASE(locale, num_put_overloads) {
    const STD::num_put<char>& np = STD::use_facet<STD::num_put<char> >(STD::locale::classic());

    STD::ostringstream d; d.precision(6);
    np.put(STD::ostreambuf_iterator<char>(d), d, d.fill(), 3.5);
    test_eq( d.str(), STD::string("3.5") );
    test_pass("cxx03:num_put::put(double)");

    STD::ostringstream ld;
    np.put(STD::ostreambuf_iterator<char>(ld), ld, ld.fill(), (long double)2.5);
    test_eq( ld.str(), STD::string("2.5") );
    test_pass("cxx03:num_put::put(long double)");

    STD::ostringstream ul;
    np.put(STD::ostreambuf_iterator<char>(ul), ul, ul.fill(), (unsigned long)123);
    test_eq( ul.str(), STD::string("123") );
    test_pass("cxx03:num_put::put(unsigned long)");

    STD::ostringstream pv; int obj = 0;
    np.put(STD::ostreambuf_iterator<char>(pv), pv, pv.fill(), (void*)&obj);
    test_true( !pv.str().empty() );
    test_pass("cxx03:num_put::put(void*)");

    STD::ostringstream ll;
    np.put(STD::ostreambuf_iterator<char>(ll), ll, ll.fill(), (long long)9000000000LL);
    test_eq( ll.str(), STD::string("9000000000") );
    STD::ostringstream ull;
    np.put(STD::ostreambuf_iterator<char>(ull), ull, ull.fill(), (unsigned long long)18000000000ULL);
    test_eq( ull.str(), STD::string("18000000000") );
    test_pass("cxx11:num_put::put(long long/unsigned long long)");
}

TEST_CASE(locale, num_get_overloads) {
    const STD::num_get<char>& ng = STD::use_facet<STD::num_get<char> >(STD::locale::classic());
    STD::istreambuf_iterator<char> e;
    STD::ios_base::iostate err;

    {   STD::istringstream is("3.5"); float v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_true( v == 3.5f ); }
    test_pass("cxx03:num_get::get(float)");

    {   STD::istringstream is("2.5"); long double v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_true( v == (long double)2.5 ); }
    test_pass("cxx03:num_get::get(long double)");

    {   STD::istringstream is("123"); unsigned long v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_eq( (long)v, 123L ); }
    test_pass("cxx03:num_get::get(unsigned long)");

    {   int obj = 0;
        const STD::num_put<char>& np = STD::use_facet<STD::num_put<char> >(STD::locale::classic());
        STD::ostringstream fmt;
        np.put(STD::ostreambuf_iterator<char>(fmt), fmt, fmt.fill(), (void*)&obj);
        STD::istringstream is(fmt.str()); void* v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_true( v == (void*)&obj ); }
    test_pass("cxx03:num_get::get(void*)");

    {   STD::istringstream is("9000000000"); long long v = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is), e, is, err, v);
        test_true( v == 9000000000LL );
        STD::istringstream is2("18000000000"); unsigned long long u = 0; err = STD::ios_base::goodbit;
        ng.get(STD::istreambuf_iterator<char>(is2), e, is2, err, u);
        test_true( u == 18000000000ULL ); }
    test_pass("cxx11:num_get::get(long long/unsigned long long)");
}

TEST_CASE(locale, moneypunct_members_and_time_byname) {
    STD::locale c = STD::locale::classic();
    const STD::moneypunct<char>& mp = STD::use_facet<STD::moneypunct<char> >(c);

    STD::string cs = mp.curr_symbol();
    test_true( cs.c_str() != 0 );
    test_pass("cxx03:moneypunct::curr_symbol");
    STD::string ps = mp.positive_sign();
    test_true( ps.c_str() != 0 );
    test_pass("cxx03:moneypunct::positive_sign");
    STD::string ns = mp.negative_sign();
    test_true( ns.c_str() != 0 );
    test_pass("cxx03:moneypunct::negative_sign");
    STD::money_base::pattern nf = mp.neg_format();
    bool has_value = nf.field[0] == STD::money_base::value || nf.field[1] == STD::money_base::value
                  || nf.field[2] == STD::money_base::value || nf.field[3] == STD::money_base::value;
    test_true( has_value );
    test_pass("cxx03:moneypunct::neg_format");

    STD::locale loc(c, new STD::time_get_byname<char>("C"));
    test_true( STD::has_facet<STD::time_get<char> >(loc) );
    test_pass("cxx03:time_get_byname");
}
