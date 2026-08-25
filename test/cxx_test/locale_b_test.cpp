#include "test_cxx.hpp"
#include <clocale>
#include <stdexcept>
#include <type_traits>

#include <locale>
#include <string>
#include <sstream>
#include <iterator>
#include <ctime>
#include <cstring>
#include <type_traits>

#if defined(_MSC_VER) && _MSC_VER < 1700 && !defined(_CCW_STD_RELOCATED)
#  define _TST_CVT_LENGTH_STATE const STD::mbstate_t&
#else
#  define _TST_CVT_LENGTH_STATE STD::mbstate_t&
#endif

namespace {

struct LbNumPunct : STD::numpunct<char> {
    char do_decimal_point() const { return '#'; }
    char do_thousands_sep() const { return '@'; }
    STD::string do_grouping() const { return STD::string("\3\2"); }
    STD::string do_truename() const { return STD::string("yes"); }
    STD::string do_falsename() const { return STD::string("no"); }
};

struct LbCollate : STD::collate<char> {
    mutable int compares_;
    LbCollate() : compares_(0) {}
    int do_compare(const char*, const char*, const char*, const char*) const {
        ++compares_;
        return -1;
    }
    STD::string do_transform(const char* lo, const char* hi) const {
        return STD::string("K") + STD::string(lo, hi);
    }
    long do_hash(const char*, const char*) const { return 4242L; }
};

struct LbCtypeChar : STD::ctype<char> {
    char do_toupper(char) const { return 'U'; }
    const char* do_toupper(char* lo, const char* hi) const {
        for (; lo != hi; ++lo) *lo = 'U';
        return hi;
    }
    char do_tolower(char) const { return 'l'; }
    const char* do_tolower(char* lo, const char* hi) const {
        for (; lo != hi; ++lo) *lo = 'l';
        return hi;
    }
    char do_widen(char) const { return 'W'; }
    const char* do_widen(const char* lo, const char* hi, char* to) const {
        for (; lo != hi; ++lo, ++to) *to = 'W';
        return hi;
    }
    char do_narrow(char, char) const { return 'N'; }
    const char* do_narrow(const char* lo, const char* hi, char, char* to) const {
        for (; lo != hi; ++lo, ++to) *to = 'N';
        return hi;
    }
};

struct LbCtypeWide : STD::ctype<wchar_t> {
    bool do_is(mask m, wchar_t) const { return m == space; }
    const wchar_t* do_is(const wchar_t* lo, const wchar_t*, mask* vec) const {
        *vec = space;
        return lo;
    }
    const wchar_t* do_scan_is(mask, const wchar_t* lo, const wchar_t*) const { return lo; }
    const wchar_t* do_scan_not(mask, const wchar_t*, const wchar_t* hi) const { return hi; }
};

struct LbCodecvt : STD::codecvt<char, char, STD::mbstate_t> {
    typedef STD::codecvt<char, char, STD::mbstate_t> base;
    int  do_encoding() const TEST_NOTHROW { return 7; }
    bool do_always_noconv() const TEST_NOTHROW { return false; }
    int  do_max_length() const TEST_NOTHROW { return 9; }
    int  do_length(_TST_CVT_LENGTH_STATE, const char*, const char*, STD::size_t) const { return 3; }
    base::result do_in(STD::mbstate_t&, const char* from, const char*, const char*& from_next,
                       char* to, char*, char*& to_next) const {
        from_next = from; to_next = to;
        return base::error;
    }
    base::result do_out(STD::mbstate_t&, const char* from, const char*, const char*& from_next,
                        char* to, char*, char*& to_next) const {
        from_next = from; to_next = to;
        return base::partial;
    }
    base::result do_unshift(STD::mbstate_t&, char* to, char*, char*& to_next) const {
        to_next = to;
        return base::noconv;
    }
};

struct LbMoneyPunct : STD::moneypunct<char> {
    char do_decimal_point() const { return ','; }
    char do_thousands_sep() const { return '.'; }
    STD::string do_grouping() const { return STD::string("\3"); }
    STD::string do_curr_symbol() const { return STD::string("Cr$"); }
    STD::string do_positive_sign() const { return STD::string("+"); }
    STD::string do_negative_sign() const { return STD::string("-"); }
    int do_frac_digits() const { return 3; }
    pattern do_pos_format() const {
        pattern p;
        p.field[0] = (char)symbol; p.field[1] = (char)sign;
        p.field[2] = (char)none;   p.field[3] = (char)value;
        return p;
    }
    pattern do_neg_format() const {
        pattern p;
        p.field[0] = (char)sign;  p.field[1] = (char)symbol;
        p.field[2] = (char)none;  p.field[3] = (char)value;
        return p;
    }
};

struct LbNumGet : STD::num_get<char> {
    typedef STD::num_get<char> base;
    mutable int calls_;
    LbNumGet() : calls_(0) {}
    base::iter_type do_get(base::iter_type in, base::iter_type, STD::ios_base&,
                           STD::ios_base::iostate& err, long& v) const {
        ++calls_;
        v = 123;
        err = STD::ios_base::goodbit;
        return in;
    }
};

struct LbNumPut : STD::num_put<char> {
    typedef STD::num_put<char> base;
    mutable int calls_;
    LbNumPut() : calls_(0) {}
    base::iter_type do_put(base::iter_type out, STD::ios_base&, char, long) const {
        ++calls_;
        *out = 'L'; ++out;
        return out;
    }
};

struct LbMoneyGet : STD::money_get<char> {
    typedef STD::money_get<char> base;
    mutable int calls_;
    LbMoneyGet() : calls_(0) {}
    base::iter_type do_get(base::iter_type in, base::iter_type, bool, STD::ios_base&,
                           STD::ios_base::iostate& err, long double& v) const {
        ++calls_;
        v = 42.0L;
        err = STD::ios_base::goodbit;
        return in;
    }
    base::iter_type do_get(base::iter_type in, base::iter_type, bool, STD::ios_base&,
                           STD::ios_base::iostate& err, base::string_type& v) const {
        ++calls_;
        v = "42";
        err = STD::ios_base::goodbit;
        return in;
    }
};

struct LbMoneyPut : STD::money_put<char> {
    typedef STD::money_put<char> base;
    mutable int calls_;
    LbMoneyPut() : calls_(0) {}
    base::iter_type do_put(base::iter_type out, bool, STD::ios_base&, char, long double) const {
        ++calls_;
        *out = 'M'; ++out;
        return out;
    }
    base::iter_type do_put(base::iter_type out, bool, STD::ios_base&, char,
                           const base::string_type&) const {
        ++calls_;
        *out = 'S'; ++out;
        return out;
    }
};

struct LbTimeGet : STD::time_get<char> {
    typedef STD::time_get<char> base;
    mutable int calls_;
    LbTimeGet() : calls_(0) {}
    base::dateorder do_date_order() const { return base::ydm; }
    base::iter_type do_get_time(base::iter_type in, base::iter_type, STD::ios_base&,
                                STD::ios_base::iostate& err, STD::tm* t) const {
        ++calls_; t->tm_hour = 1; err = STD::ios_base::goodbit; return in;
    }
    base::iter_type do_get_date(base::iter_type in, base::iter_type, STD::ios_base&,
                                STD::ios_base::iostate& err, STD::tm* t) const {
        ++calls_; t->tm_mday = 2; err = STD::ios_base::goodbit; return in;
    }
    base::iter_type do_get_weekday(base::iter_type in, base::iter_type, STD::ios_base&,
                                   STD::ios_base::iostate& err, STD::tm* t) const {
        ++calls_; t->tm_wday = 3; err = STD::ios_base::goodbit; return in;
    }
    base::iter_type do_get_monthname(base::iter_type in, base::iter_type, STD::ios_base&,
                                     STD::ios_base::iostate& err, STD::tm* t) const {
        ++calls_; t->tm_mon = 4; err = STD::ios_base::goodbit; return in;
    }
    base::iter_type do_get_year(base::iter_type in, base::iter_type, STD::ios_base&,
                                STD::ios_base::iostate& err, STD::tm* t) const {
        ++calls_; t->tm_year = 5; err = STD::ios_base::goodbit; return in;
    }
};

struct LbTimePut : STD::time_put<char> {
    typedef STD::time_put<char> base;
    mutable int calls_;
    LbTimePut() : calls_(0) {}
    base::iter_type do_put(base::iter_type out, STD::ios_base&, char, const STD::tm*,
                           char, char) const {
        ++calls_;
        *out = 'T'; ++out;
        return out;
    }
};

struct LbMessages : STD::messages<char> {
    typedef STD::messages<char> base;
    mutable int opens_;
    mutable int closes_;
    LbMessages() : opens_(0), closes_(0) {}
    base::catalog do_open(const STD::string&, const STD::locale&) const {
        ++opens_;
        return (base::catalog)7;
    }
    base::string_type do_get(base::catalog, int, int, const base::string_type&) const {
        return base::string_type("translated");
    }
    void do_close(base::catalog) const { ++closes_; }
};

}

TEST_CASE(locale, facet_member_types_char_cxx03) {
    test_true(( STD::is_same<STD::ctype<char>::char_type, char>::value ));
    test_true(( STD::is_same<STD::ctype<wchar_t>::char_type, wchar_t>::value ));
    test_pass("cxx03:ctype::char_type");

    test_true(( STD::is_same<STD::collate<char>::char_type, char>::value ));
    test_pass("cxx03:collate::char_type");
    test_true(( STD::is_same<STD::collate<char>::string_type, STD::string>::value ));
    {
        STD::collate<char>::string_type s("ab");
        test_eq( s, STD::string("ab") );
    }
    test_pass("cxx03:collate::string_type");

    test_true(( STD::is_same<STD::numpunct<char>::char_type, char>::value ));
    test_pass("cxx03:numpunct::char_type");
    test_true(( STD::is_same<STD::numpunct<char>::string_type, STD::string>::value ));
    test_pass("cxx03:numpunct::string_type");

    test_true(( STD::is_same<STD::moneypunct<char>::char_type, char>::value ));
    test_pass("cxx03:moneypunct::char_type");
    test_true(( STD::is_same<STD::moneypunct<char>::string_type, STD::string>::value ));
    test_pass("cxx03:moneypunct::string_type");

    test_true(( STD::is_same<STD::messages<char>::char_type, char>::value ));
    test_pass("cxx03:messages::char_type");
    test_true(( STD::is_same<STD::messages<char>::string_type, STD::string>::value ));
    test_pass("cxx03:messages::string_type");
}

TEST_CASE(locale, facet_member_types_io_cxx03) {
    typedef STD::istreambuf_iterator<char> InIt;
    typedef STD::ostreambuf_iterator<char> OutIt;

    test_true(( STD::is_same<STD::num_get<char>::char_type, char>::value ));
    test_pass("cxx03:num_get::char_type");
    test_true(( STD::is_same<STD::num_get<char>::iter_type, InIt>::value ));
    test_pass("cxx03:num_get::iter_type");

    test_true(( STD::is_same<STD::num_put<char>::char_type, char>::value ));
    test_pass("cxx03:num_put::char_type");
    test_true(( STD::is_same<STD::num_put<char>::iter_type, OutIt>::value ));
    test_pass("cxx03:num_put::iter_type");

    test_true(( STD::is_same<STD::money_get<char>::char_type, char>::value ));
    test_pass("cxx03:money_get::char_type");
    test_true(( STD::is_same<STD::money_get<char>::iter_type, InIt>::value ));
    test_pass("cxx03:money_get::iter_type");
    test_true(( STD::is_same<STD::money_get<char>::string_type, STD::string>::value ));
    test_pass("cxx03:money_get::string_type");

    test_true(( STD::is_same<STD::money_put<char>::char_type, char>::value ));
    test_pass("cxx03:money_put::char_type");
    test_true(( STD::is_same<STD::money_put<char>::iter_type, OutIt>::value ));
    test_pass("cxx03:money_put::iter_type");
    test_true(( STD::is_same<STD::money_put<char>::string_type, STD::string>::value ));
    test_pass("cxx03:money_put::string_type");

    test_true(( STD::is_same<STD::time_get<char>::char_type, char>::value ));
    test_pass("cxx03:time_get::char_type");
    test_true(( STD::is_same<STD::time_get<char>::iter_type, InIt>::value ));
    test_pass("cxx03:time_get::iter_type");

    test_true(( STD::is_same<STD::time_put<char>::char_type, char>::value ));
    test_pass("cxx03:time_put::char_type");
    test_true(( STD::is_same<STD::time_put<char>::iter_type, OutIt>::value ));
    test_pass("cxx03:time_put::iter_type");

    typedef STD::codecvt<char, char, STD::mbstate_t> Cvt;
    test_true(( STD::is_same<Cvt::intern_type, char>::value ));
    test_pass("cxx03:codecvt::intern_type");
    test_true(( STD::is_same<Cvt::extern_type, char>::value ));
    test_pass("cxx03:codecvt::extern_type");
    test_true(( STD::is_same<Cvt::state_type, STD::mbstate_t>::value ));
    {
        Cvt::state_type st;
        STD::memset(&st, 0, sizeof(st));
        test_true( sizeof(st) > 0 );
    }
    test_pass("cxx03:codecvt::state_type");
}

TEST_CASE(locale, base_classes_cxx03) {
    const STD::ctype<char>& ct = STD::use_facet<STD::ctype<char> >(STD::locale::classic());
    STD::ctype_base* cb = 0;
    (void)cb;
    test_true( sizeof(STD::ctype_base::mask) > 0 );
    test_true( (STD::is_base_of<STD::ctype_base, STD::ctype<char> >::value) );
    test_pass("cxx03:ctype_base");

    test_true( ct.is(STD::ctype_base::space, ' ') );
    test_true( !ct.is(STD::ctype_base::space, 'a') );
    test_pass("cxx03:ctype_base::space");
    test_true( ct.is(STD::ctype_base::print, 'a') );
    test_true( !ct.is(STD::ctype_base::print, '\n') );
    test_pass("cxx03:ctype_base::print");
    test_true( ct.is(STD::ctype_base::cntrl, '\n') );
    test_true( !ct.is(STD::ctype_base::cntrl, 'a') );
    test_pass("cxx03:ctype_base::cntrl");
    test_true( ct.is(STD::ctype_base::upper, 'A') );
    test_true( !ct.is(STD::ctype_base::upper, 'a') );
    test_pass("cxx03:ctype_base::upper");
    test_true( ct.is(STD::ctype_base::lower, 'a') );
    test_true( !ct.is(STD::ctype_base::lower, 'A') );
    test_pass("cxx03:ctype_base::lower");
    test_true( ct.is(STD::ctype_base::alpha, 'a') );
    test_true( !ct.is(STD::ctype_base::alpha, '1') );
    test_pass("cxx03:ctype_base::alpha");
    test_true( ct.is(STD::ctype_base::digit, '7') );
    test_true( !ct.is(STD::ctype_base::digit, 'a') );
    test_pass("cxx03:ctype_base::digit");
    test_true( ct.is(STD::ctype_base::punct, ',') );
    test_true( !ct.is(STD::ctype_base::punct, 'a') );
    test_pass("cxx03:ctype_base::punct");
    test_true( ct.is(STD::ctype_base::xdigit, 'f') );
    test_true( !ct.is(STD::ctype_base::xdigit, 'g') );
    test_pass("cxx03:ctype_base::xdigit");
    test_true( ct.is(STD::ctype_base::alnum, '1') );
    test_true( ct.is(STD::ctype_base::alnum, 'a') );
    test_true( !ct.is(STD::ctype_base::alnum, ' ') );
    test_pass("cxx03:ctype_base::alnum");
    test_true( ct.is(STD::ctype_base::graph, 'a') );
    test_true( !ct.is(STD::ctype_base::graph, ' ') );
    test_pass("cxx03:ctype_base::graph");

    test_true( (STD::is_base_of<STD::codecvt_base,
                                STD::codecvt<char, char, STD::mbstate_t> >::value) );
    test_pass("cxx03:codecvt_base");
    {
        STD::codecvt_base::result r = STD::codecvt_base::ok;
        test_true( r == STD::codecvt_base::ok );
        test_true( r != STD::codecvt_base::error );
        test_pass("cxx03:codecvt_base::ok");
        r = STD::codecvt_base::partial;
        test_true( r == STD::codecvt_base::partial );
        test_true( r != STD::codecvt_base::ok );
        test_pass("cxx03:codecvt_base::partial");
        r = STD::codecvt_base::error;
        test_true( r == STD::codecvt_base::error );
        test_true( r != STD::codecvt_base::noconv );
        test_pass("cxx03:codecvt_base::error");
        r = STD::codecvt_base::noconv;
        test_true( r == STD::codecvt_base::noconv );
        test_true( r != STD::codecvt_base::partial );
        test_pass("cxx03:codecvt_base::noconv");
    }

    test_true( (STD::is_base_of<STD::messages_base, STD::messages<char> >::value) );
    test_pass("cxx03:messages_base");
    test_true(( STD::is_same<STD::messages<char>::catalog,
                             STD::messages_base::catalog>::value ));
    {
        STD::messages_base::catalog c = (STD::messages_base::catalog)3;
        test_true( c == (STD::messages_base::catalog)3 );
    }
    test_pass("cxx03:messages_base::catalog");

    test_true( (STD::is_base_of<STD::time_base, STD::time_get<char> >::value) );
    test_pass("cxx03:time_base");
    {
        STD::time_base::dateorder d = STD::time_base::no_order;
        test_true( d == STD::time_base::no_order );
        test_true( d != STD::time_base::dmy );
        test_pass("cxx03:time_base::no_order");
        d = STD::time_base::dmy;
        test_true( d == STD::time_base::dmy && d != STD::time_base::mdy );
        test_pass("cxx03:time_base::dmy");
        d = STD::time_base::mdy;
        test_true( d == STD::time_base::mdy && d != STD::time_base::ymd );
        test_pass("cxx03:time_base::mdy");
        d = STD::time_base::ymd;
        test_true( d == STD::time_base::ymd && d != STD::time_base::ydm );
        test_pass("cxx03:time_base::ymd");
        d = STD::time_base::ydm;
        test_true( d == STD::time_base::ydm && d != STD::time_base::no_order );
        test_pass("cxx03:time_base::ydm");
    }
}

TEST_CASE(locale, money_base_and_locale_categories_cxx03) {
    test_true( (STD::is_base_of<STD::money_base, STD::moneypunct<char> >::value) );
    test_pass("cxx03:money_base");
    {
        STD::money_base::part p = STD::money_base::none;
        test_true( p == STD::money_base::none && p != STD::money_base::space );
        test_pass("cxx03:money_base::none");
        p = STD::money_base::space;
        test_true( p == STD::money_base::space && p != STD::money_base::symbol );
        test_pass("cxx03:money_base::space");
        p = STD::money_base::symbol;
        test_true( p == STD::money_base::symbol && p != STD::money_base::sign );
        test_pass("cxx03:money_base::symbol");
        p = STD::money_base::sign;
        test_true( p == STD::money_base::sign && p != STD::money_base::value );
        test_pass("cxx03:money_base::sign");
        p = STD::money_base::value;
        test_true( p == STD::money_base::value && p != STD::money_base::none );
        test_pass("cxx03:money_base::value");
    }
    {
        STD::money_base::pattern pat =
            STD::use_facet<STD::moneypunct<char> >(STD::locale::classic()).pos_format();
        test_true( sizeof(pat.field) == 4 );
        test_pass("cxx03:money_base::pattern");
        bool has_value = false;
        for (int i = 0; i < 4; ++i)
            if (pat.field[i] == (char)STD::money_base::value) has_value = true;
        test_true( has_value );
        test_true( sizeof(pat.field[0]) == 1 );
        test_pass("cxx03:money_base::pattern::field");
    }

    STD::locale::category cat = STD::locale::all;
    test_true( sizeof(cat) >= sizeof(int) );
    test_true( (STD::locale::all & STD::locale::ctype) == STD::locale::ctype );
    test_pass("cxx03:locale::category");

    test_true( STD::locale::none == 0 );
    test_true( (STD::locale::all & STD::locale::none) == 0 );
    test_pass("cxx03:locale::none");
    test_true( STD::locale::collate != STD::locale::none );
    test_true( (STD::locale::all & STD::locale::collate) == STD::locale::collate );
    test_pass("cxx03:locale::collate");
    test_true( STD::locale::ctype != STD::locale::collate );
    test_true( (STD::locale::all & STD::locale::ctype) == STD::locale::ctype );
    test_pass("cxx03:locale::ctype");
    test_true( STD::locale::monetary != STD::locale::ctype );
    test_true( (STD::locale::all & STD::locale::monetary) == STD::locale::monetary );
    test_pass("cxx03:locale::monetary");
    test_true( STD::locale::numeric != STD::locale::monetary );
    test_true( (STD::locale::all & STD::locale::numeric) == STD::locale::numeric );
    test_pass("cxx03:locale::numeric");
    test_true( STD::locale::time != STD::locale::numeric );
    test_true( (STD::locale::all & STD::locale::time) == STD::locale::time );
    test_pass("cxx03:locale::time");
    test_true( STD::locale::messages != STD::locale::time );
    test_true( (STD::locale::all & STD::locale::messages) == STD::locale::messages );
    test_pass("cxx03:locale::messages");
    test_true( STD::locale::all == (STD::locale::collate | STD::locale::ctype |
                                    STD::locale::monetary | STD::locale::numeric |
                                    STD::locale::time | STD::locale::messages) );
    test_pass("cxx03:locale::all");

    test_true( &STD::ctype<char>::id != 0 );
    test_true( (void*)&STD::ctype<char>::id != (void*)&STD::numpunct<char>::id );
    test_true( sizeof(STD::locale::id) > 0 );
    test_pass("cxx03:locale::id");
}

TEST_CASE(locale, do_virtuals_numpunct_collate_cxx03) {
    LbNumPunct np;
    test_eq( np.decimal_point(), '#' );
    test_pass("cxx03:numpunct::do_decimal_point");
    test_eq( np.thousands_sep(), '@' );
    test_pass("cxx03:numpunct::do_thousands_sep");
    test_eq( np.grouping(), STD::string("\3\2") );
    test_pass("cxx03:numpunct::do_grouping");
    test_eq( np.truename(), STD::string("yes") );
    test_pass("cxx03:numpunct::do_truename");
    test_eq( np.falsename(), STD::string("no") );
    test_pass("cxx03:numpunct::do_falsename");

    LbCollate col;
    const char a[] = "aaa", b[] = "bbb";
    int c1 = col.compare(a, a + 3, b, b + 3);
    int c2 = col.compare(b, b + 3, a, a + 3);
    test_eq( c1, -1 );
    test_eq( c2, -1 );
    test_eq( col.compares_, 2 );
    test_pass("cxx03:collate::do_compare");

    STD::string tr = col.transform(a, a + 3);
    test_eq( tr, STD::string("Kaaa") );
    test_pass("cxx03:collate::do_transform");

    long ha = col.hash(a, a + 3);
    long hb = col.hash(b, b + 3);
    test_eq( ha, 4242L );
    test_eq( hb, 4242L );
    test_pass("cxx03:collate::do_hash");
}

TEST_CASE(locale, do_virtuals_ctype_cxx03) {
    LbCtypeChar ct;

    test_eq( ct.toupper('a'), 'U' );
    {
        char buf[3] = { 'a', 'b', 'c' };
        ct.toupper(buf, buf + 3);
        test_eq( buf[2], 'U' );
    }
    test_pass("cxx03:ctype::do_toupper");

    test_eq( ct.tolower('A'), 'l' );
    {
        char buf[3] = { 'A', 'B', 'C' };
        ct.tolower(buf, buf + 3);
        test_eq( buf[0], 'l' );
    }
    test_pass("cxx03:ctype::do_tolower");

    test_eq( ct.widen('a'), 'W' );
    {
        const char src[3] = { 'a', 'b', 'c' };
        char dst[3] = { 0, 0, 0 };
        ct.widen(src, src + 3, dst);
        test_eq( dst[1], 'W' );
    }
    test_pass("cxx03:ctype::do_widen");

    test_eq( ct.narrow('a', '?'), 'N' );
    {
        const char src[3] = { 'a', 'b', 'c' };
        char dst[3] = { 0, 0, 0 };
        ct.narrow(src, src + 3, '?', dst);
        test_eq( dst[1], 'N' );
    }
    test_pass("cxx03:ctype::do_narrow");

    LbCtypeWide cw;
    test_true( cw.is(STD::ctype_base::space, L'x') );
    test_true( !cw.is(STD::ctype_base::alpha, L'x') );
    test_pass("cxx03:ctype::do_is");

    {
        const wchar_t s[3] = { L'a', L'b', L'c' };
        test_true( cw.scan_is(STD::ctype_base::alpha, s, s + 3) == s );
        test_pass("cxx03:ctype::do_scan_is");
        test_true( cw.scan_not(STD::ctype_base::alpha, s, s + 3) == s + 3 );
        test_pass("cxx03:ctype::do_scan_not");
    }
}

TEST_CASE(locale, do_virtuals_codecvt_cxx03) {
    LbCodecvt cv;

    test_eq( cv.encoding(), 7 );
    test_pass("cxx03:codecvt::do_encoding");
    test_true( !cv.always_noconv() );
    test_pass("cxx03:codecvt::do_always_noconv");
    test_eq( cv.max_length(), 9 );
    test_pass("cxx03:codecvt::do_max_length");

    STD::mbstate_t st;
    STD::memset(&st, 0, sizeof(st));
    const char src[4] = { 'a', 'b', 'c', 'd' };
    test_eq( cv.length(st, src, src + 4, 4), 3 );
    test_pass("cxx03:codecvt::do_length");

    char dst[4];
    const char* fnext = 0;
    char* tnext = 0;
    STD::codecvt_base::result r = cv.in(st, src, src + 4, fnext, dst, dst + 4, tnext);
    test_true( r == STD::codecvt_base::error );
    test_true( fnext == src && tnext == dst );
    test_pass("cxx03:codecvt::do_in");
    test_pass("cxx03:codecvt::in");

    r = cv.out(st, src, src + 4, fnext, dst, dst + 4, tnext);
    test_true( r == STD::codecvt_base::partial );
    test_true( fnext == src );
    test_pass("cxx03:codecvt::do_out");

    r = cv.unshift(st, dst, dst + 4, tnext);
    test_true( r == STD::codecvt_base::noconv );
    test_true( tnext == dst );
    test_pass("cxx03:codecvt::do_unshift");
    test_pass("cxx03:codecvt::unshift");

    {
        STD::locale loc(STD::locale::classic(),
                        new STD::codecvt_byname<char, char, STD::mbstate_t>("C"));
        test_true(( STD::has_facet<STD::codecvt<char, char, STD::mbstate_t> >(loc) ));
        const STD::codecvt<char, char, STD::mbstate_t>& c =
            STD::use_facet<STD::codecvt<char, char, STD::mbstate_t> >(loc);
        test_true( c.encoding() == 1 || c.encoding() == 0 || c.encoding() == -1 );
    }
    test_pass("cxx03:codecvt_byname");
}

TEST_CASE(locale, do_virtuals_moneypunct_cxx03) {
    LbMoneyPunct mp;

    test_eq( mp.decimal_point(), ',' );
    test_pass("cxx03:moneypunct::do_decimal_point");
    test_eq( mp.thousands_sep(), '.' );
    test_pass("cxx03:moneypunct::do_thousands_sep");
    test_eq( mp.grouping(), STD::string("\3") );
    test_pass("cxx03:moneypunct::do_grouping");
    test_eq( mp.curr_symbol(), STD::string("Cr$") );
    test_pass("cxx03:moneypunct::do_curr_symbol");
    test_eq( mp.positive_sign(), STD::string("+") );
    test_pass("cxx03:moneypunct::do_positive_sign");
    test_eq( mp.negative_sign(), STD::string("-") );
    test_pass("cxx03:moneypunct::do_negative_sign");
    test_eq( mp.frac_digits(), 3 );
    test_pass("cxx03:moneypunct::do_frac_digits");
    {
        STD::money_base::pattern p = mp.pos_format();
        test_eq( (int)p.field[0], (int)STD::money_base::symbol );
        test_eq( (int)p.field[3], (int)STD::money_base::value );
    }
    test_pass("cxx03:moneypunct::do_pos_format");
    {
        STD::money_base::pattern p = mp.neg_format();
        test_eq( (int)p.field[0], (int)STD::money_base::sign );
        test_eq( (int)p.field[1], (int)STD::money_base::symbol );
    }
    test_pass("cxx03:moneypunct::do_neg_format");

    const STD::moneypunct<char>& c =
        STD::use_facet<STD::moneypunct<char> >(STD::locale::classic());
    {
        char dp = c.decimal_point();
        test_true( dp == dp );
        test_true( sizeof(dp) == 1 );
    }
    test_pass("cxx03:moneypunct::decimal_point");
    {
        char sep = c.thousands_sep();
        test_true( sep == sep );
        test_true( sizeof(sep) == 1 );
    }
    test_pass("cxx03:moneypunct::thousands_sep");
    {
        STD::string g = c.grouping();
        test_true( g.c_str() != 0 );
        test_true( g.empty() || g[0] != 0 );
    }
    test_pass("cxx03:moneypunct::grouping");
    test_eq( c.frac_digits(), 0 );
    test_pass("cxx03:moneypunct::frac_digits");
}

TEST_CASE(locale, do_virtuals_num_money_cxx03) {
    STD::istringstream in("0");
    STD::ostringstream out;
    STD::ios_base::iostate err = STD::ios_base::goodbit;

    {
        LbNumGet ng;
        long v = 0;
        ng.get(STD::istreambuf_iterator<char>(in), STD::istreambuf_iterator<char>(),
               in, err, v);
        test_eq( v, 123L );
        test_eq( ng.calls_, 1 );
    }
    test_pass("cxx03:num_get::do_get");

    {
        LbNumPut np;
        np.put(STD::ostreambuf_iterator<char>(out), out, ' ', 5L);
        test_eq( np.calls_, 1 );
        test_eq( out.str(), STD::string("L") );
    }
    test_pass("cxx03:num_put::do_put");

    {
        LbMoneyGet mg;
        long double amount = 0;
        err = STD::ios_base::goodbit;
        mg.get(STD::istreambuf_iterator<char>(in), STD::istreambuf_iterator<char>(),
               false, in, err, amount);
        test_true( amount == 42.0L );
        test_eq( mg.calls_, 1 );
    }
    test_pass("cxx03:money_get::do_get");

    {
        STD::ostringstream mout;
        LbMoneyPut mp;
        mp.put(STD::ostreambuf_iterator<char>(mout), false, mout, ' ', 1.0L);
        test_eq( mp.calls_, 1 );
        test_eq( mout.str(), STD::string("M") );
    }
    test_pass("cxx03:money_put::do_put");
}

TEST_CASE(locale, do_virtuals_time_messages_cxx03) {
    STD::istringstream in("12:00:00");
    STD::ios_base::iostate err = STD::ios_base::goodbit;
    STD::tm tmv;
    STD::memset(&tmv, 0, sizeof(tmv));

    LbTimeGet tg;
    test_true( tg.date_order() == STD::time_base::ydm );
    test_pass("cxx03:time_get::do_date_order");

    tg.get_time(STD::istreambuf_iterator<char>(in), STD::istreambuf_iterator<char>(),
                in, err, &tmv);
    test_eq( tmv.tm_hour, 1 );
    test_pass("cxx03:time_get::do_get_time");

    tg.get_date(STD::istreambuf_iterator<char>(in), STD::istreambuf_iterator<char>(),
                in, err, &tmv);
    test_eq( tmv.tm_mday, 2 );
    test_pass("cxx03:time_get::do_get_date");

    tg.get_weekday(STD::istreambuf_iterator<char>(in), STD::istreambuf_iterator<char>(),
                   in, err, &tmv);
    test_eq( tmv.tm_wday, 3 );
    test_pass("cxx03:time_get::do_get_weekday");

    tg.get_monthname(STD::istreambuf_iterator<char>(in), STD::istreambuf_iterator<char>(),
                     in, err, &tmv);
    test_eq( tmv.tm_mon, 4 );
    test_pass("cxx03:time_get::do_get_monthname");

    tg.get_year(STD::istreambuf_iterator<char>(in), STD::istreambuf_iterator<char>(),
                in, err, &tmv);
    test_eq( tmv.tm_year, 5 );
    test_pass("cxx03:time_get::do_get_year");

#if _TST_HAS_CXX11_LIB_MEMBERS
    {
        STD::istringstream in2("1");
        STD::tm tm2;
        STD::memset(&tm2, 0, sizeof(tm2));
        err = STD::ios_base::goodbit;
        const STD::time_get<char>& classic =
            STD::use_facet<STD::time_get<char> >(STD::locale::classic());
        classic.get(STD::istreambuf_iterator<char>(in2), STD::istreambuf_iterator<char>(),
                    in2, err, &tm2, 'H', 0);
        test_eq( tm2.tm_hour, 1 );
        test_true( err != STD::ios_base::failbit );
    }
    test_pass("cxx03:time_get::do_get");
#else
    test_skip("cxx03:time_get::do_get");
#endif

    {
        STD::ostringstream out;
        LbTimePut tp;
        tp.put(STD::ostreambuf_iterator<char>(out), out, ' ', &tmv, 'x', 0);
        test_eq( tp.calls_, 1 );
        test_eq( out.str(), STD::string("T") );
    }
    test_pass("cxx03:time_put::do_put");

    LbMessages ms;
    STD::messages<char>::catalog cat = ms.open("cat", STD::locale::classic());
    test_true( cat == (STD::messages<char>::catalog)7 );
    test_eq( ms.opens_, 1 );
    test_pass("cxx03:messages::do_open");

    test_eq( ms.get(cat, 0, 0, STD::string("fallback")), STD::string("translated") );
    test_pass("cxx03:messages::do_get");

    ms.close(cat);
    test_eq( ms.closes_, 1 );
    test_pass("cxx03:messages::do_close");
}

TEST_CASE(locale, classification_free_functions_cxx03) {
    STD::locale c = STD::locale::classic();
    test_true(  STD::isalpha('a', c) );
    test_true( !STD::isalpha('1', c) );
    test_true(  STD::isalpha(L'a', c) );
    test_pass("cxx03:isalpha(loc)");
}

#if TEST_TARGET_CXX >= 2011 || defined(__GLIBCXX__) || defined(_LIBCPP_VERSION)
#define _TST_NG_FAIL_VAL(zero, old) (zero)
#else
#define _TST_NG_FAIL_VAL(zero, old) (old)
#endif

namespace {
typedef STD::istreambuf_iterator<char> LbIn;
typedef STD::ostreambuf_iterator<char> LbOut;

template <class T>
static bool lb_read(const char* text, T& v, STD::ios_base::fmtflags fl = STD::ios_base::fmtflags(0),
                    STD::ios_base::fmtflags msk = STD::ios_base::fmtflags(0)) {
    STD::istringstream in(text);
    if (msk != STD::ios_base::fmtflags(0)) in.setf(fl, msk);
    else if (fl != STD::ios_base::fmtflags(0)) in.setf(fl);
    STD::ios_base::iostate err = STD::ios_base::goodbit;
    STD::use_facet<STD::num_get<char> >(in.getloc())
        .get(LbIn(in), LbIn(), in, err, v);
    return (err & STD::ios_base::failbit) != STD::ios_base::failbit;
}

template <class T>
static STD::string lb_write(T v, STD::ios_base::fmtflags fl = STD::ios_base::fmtflags(0),
                            STD::ios_base::fmtflags msk = STD::ios_base::fmtflags(0)) {
    STD::ostringstream out;
    if (msk != STD::ios_base::fmtflags(0)) out.setf(fl, msk);
    else if (fl != STD::ios_base::fmtflags(0)) out.setf(fl);
    STD::use_facet<STD::num_put<char> >(out.getloc())
        .put(LbOut(out), out, ' ', v);
    return out.str();
}
}

TEST_CASE(locale, num_get_value_types_cxx03) {
    {   bool b = true;
        test_true( lb_read("0", b) );      test_true( !b );
        test_true( lb_read("1", b) );      test_true( b );
        b = false;
        test_true( lb_read("true", b, STD::ios_base::boolalpha) );  test_true( b );
        test_true( lb_read("false", b, STD::ios_base::boolalpha) ); test_true( !b );
        b = true;
        test_true( !lb_read("2", b) );     test_true( b );
        test_true( !lb_read("yes", b, STD::ios_base::boolalpha) ); }
    test_pass("cxx03:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, bool&)");

    {   long l = 0;
        test_true( lb_read("-123", l) );   test_eq( l, -123L );
        test_true( lb_read("+7", l) );     test_eq( l, 7L );
        test_true( lb_read("ff", l, STD::ios_base::hex, STD::ios_base::basefield) );
        test_eq( l, 255L );
        test_true( lb_read("17", l, STD::ios_base::oct, STD::ios_base::basefield) );
        test_eq( l, 15L );
        l = 5;
        test_true( !lb_read("zz", l) );    test_eq( l, _TST_NG_FAIL_VAL(0L, 5L) ); }
    test_pass("cxx03:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, long&)");

    {   unsigned short us = 1;
        test_true( lb_read("65535", us) ); test_eq( (long)us, 65535L );
        test_true( lb_read("0", us) );     test_eq( (long)us, 0L );
        us = 3;
        test_true( !lb_read("q", us) );    test_eq( (long)us, _TST_NG_FAIL_VAL(0L, 3L) ); }
    test_pass("cxx03:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, unsigned short&)");

    {   unsigned int ui = 1;
        test_true( lb_read("4000", ui) );  test_eq( (long)ui, 4000L );
        test_true( lb_read("1f", ui, STD::ios_base::hex, STD::ios_base::basefield) );
        test_eq( (long)ui, 31L );
        ui = 9;
        test_true( !lb_read("-", ui) );    test_eq( (long)ui, _TST_NG_FAIL_VAL(0L, 9L) ); }
    test_pass("cxx03:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, unsigned int&)");

    {   unsigned long ul = 1;
        test_true( lb_read("70000", ul) ); test_eq( (long)ul, 70000L );
        test_true( lb_read("10", ul, STD::ios_base::oct, STD::ios_base::basefield) );
        test_eq( (long)ul, 8L );
        ul = 2;
        test_true( !lb_read("x", ul) );    test_eq( (long)ul, _TST_NG_FAIL_VAL(0L, 2L) ); }
    test_pass("cxx03:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, unsigned long&)");

    {   float f = 0.0f;
        test_true( lb_read("1.5", f) );    test_eq( f, 1.5f );
        test_true( lb_read("-2.5", f) );   test_eq( f, -2.5f );
        test_true( lb_read("1e2", f) );    test_eq( f, 100.0f );
        f = 8.0f;
        test_true( !lb_read("abc", f) );   test_eq( f, _TST_NG_FAIL_VAL(0.0f, 8.0f) ); }
    test_pass("cxx03:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, float&)");

    {   double d = 0.0;
        test_true( lb_read("2.25", d) );   test_eq( d, 2.25 );
        test_true( lb_read("-0.5", d) );   test_eq( d, -0.5 );
        test_true( lb_read("3e-2", d) );   test_true( d > 0.029 && d < 0.031 );
        d = 6.0;
        test_true( !lb_read("zz", d) );    test_eq( d, _TST_NG_FAIL_VAL(0.0, 6.0) ); }
    test_pass("cxx03:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, double&)");

    {   long double ld = 0.0;
        test_true( lb_read("3.5", ld) );   test_true( ld == (long double)3.5 );
        test_true( lb_read("-1.25", ld) ); test_true( ld == (long double)-1.25 );
        ld = (long double)4.0;
        test_true( !lb_read("qq", ld) );
        test_true( ld == (long double)_TST_NG_FAIL_VAL(0.0, 4.0) ); }
    test_pass("cxx03:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, long double&)");

    {   void* pv = 0;
        test_true( lb_read("0x1a", pv) );  test_true( pv == (void*)0x1a );
        void* pv2 = (void*)0x99;
        test_true( !lb_read("zz", pv2) );
        TEST_SKIP_LIBCXX("libc++ leaves val alone when %p fails; the standard does not say");
        test_true( pv2 == _TST_NG_FAIL_VAL((void*)0, (void*)0x99) ); }
    test_pass("cxx03:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, void*&)");
}

TEST_CASE(locale, num_get_value_types_cxx11) {
    {   long long ll = 0;
        test_true( lb_read("-9000000000", ll) );
        test_true( ll == (long long)-9000000000LL );
        test_true( lb_read("ff", ll, STD::ios_base::hex, STD::ios_base::basefield) );
        test_true( ll == (long long)255 );
        ll = 3;
        test_true( !lb_read("zz", ll) );
        test_true( ll == (long long)_TST_NG_FAIL_VAL(0, 3) ); }
    test_pass("cxx11:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, long long&)");

    {   unsigned long long ull = 0;
        test_true( lb_read("18000000000", ull) );
        test_true( ull == (unsigned long long)18000000000ULL );
        test_true( lb_read("10", ull, STD::ios_base::oct, STD::ios_base::basefield) );
        test_true( ull == (unsigned long long)8 );
        ull = 4;
        test_true( !lb_read("q", ull) );
        test_true( ull == (unsigned long long)_TST_NG_FAIL_VAL(0, 4) ); }
    test_pass("cxx11:num_get::do_get(iter_type, iter_type, ios_base&, iostate&, unsigned long long&)");
}

TEST_CASE(locale, num_put_value_types_cxx03) {
    test_eq( lb_write(true), STD::string("1") );
    test_eq( lb_write(false), STD::string("0") );
    test_eq( lb_write(true, STD::ios_base::boolalpha), STD::string("true") );
    test_eq( lb_write(false, STD::ios_base::boolalpha), STD::string("false") );
    test_pass("cxx03:num_put::do_put(iter_type, ios_base&, char_type, bool)");

    test_eq( lb_write((long)-123), STD::string("-123") );
    test_eq( lb_write((long)0), STD::string("0") );
    test_eq( lb_write((long)255, STD::ios_base::hex, STD::ios_base::basefield),
             STD::string("ff") );
    test_eq( lb_write((long)8, STD::ios_base::oct, STD::ios_base::basefield),
             STD::string("10") );
    test_pass("cxx03:num_put::do_put(iter_type, ios_base&, char_type, long)");

    test_eq( lb_write((unsigned long)70000), STD::string("70000") );
    test_eq( lb_write((unsigned long)0), STD::string("0") );
    test_eq( lb_write((unsigned long)255, STD::ios_base::hex, STD::ios_base::basefield),
             STD::string("ff") );
    test_pass("cxx03:num_put::do_put(iter_type, ios_base&, char_type, unsigned long)");

    test_eq( lb_write((double)2.25), STD::string("2.25") );
    test_eq( lb_write((double)-2.25), STD::string("-2.25") );
    {   STD::ostringstream o;
        o.setf(STD::ios_base::fixed, STD::ios_base::floatfield);
        o.precision(2);
        STD::use_facet<STD::num_put<char> >(o.getloc()).put(LbOut(o), o, ' ', (double)1.5);
        test_eq( o.str(), STD::string("1.50") ); }
    test_pass("cxx03:num_put::do_put(iter_type, ios_base&, char_type, double)");

    test_eq( lb_write((long double)3.5), STD::string("3.5") );
    test_eq( lb_write((long double)-3.5), STD::string("-3.5") );
    test_pass("cxx03:num_put::do_put(iter_type, ios_base&, char_type, long double)");

    {   int obj = 0;
        STD::string ps = lb_write((const void*)&obj);
        test_true( ps.size() > 0 );
        STD::string zs = lb_write((const void*)0);
        test_true( zs.size() > 0 ); }
    test_pass("cxx03:num_put::do_put(iter_type, ios_base&, char_type, const void*)");
}

TEST_CASE(locale, num_put_value_types_cxx11) {
    test_eq( lb_write((long long)-9000000000LL), STD::string("-9000000000") );
    test_eq( lb_write((long long)0), STD::string("0") );
    test_eq( lb_write((long long)255, STD::ios_base::hex, STD::ios_base::basefield),
             STD::string("ff") );
    test_pass("cxx11:num_put::do_put(iter_type, ios_base&, char_type, long long)");

    test_eq( lb_write((unsigned long long)18000000000ULL), STD::string("18000000000") );
    test_eq( lb_write((unsigned long long)0), STD::string("0") );
    test_eq( lb_write((unsigned long long)8, STD::ios_base::oct, STD::ios_base::basefield),
             STD::string("10") );
    test_pass("cxx11:num_put::do_put(iter_type, ios_base&, char_type, unsigned long long)");
}

TEST_CASE(locale, ctype_range_forms_cxx03) {
    const STD::ctype<char>& ct = STD::use_facet<STD::ctype<char> >(STD::locale::classic());

    {   char b[6] = { 'a', 'B', '3', 'z', '-', 'q' };
        const char* end = ct.toupper(b, b + 6);
        test_true( end == b + 6 );
        test_eq( b[0], 'A' );
        test_eq( b[1], 'B' );
        test_eq( b[2], '3' );
        test_eq( b[3], 'Z' );
        test_eq( b[4], '-' );
        char one[1] = { 'x' };
        ct.toupper(one, one + 1);
        test_eq( one[0], 'X' );
        char none[1] = { 'y' };
        ct.toupper(none, none);
        test_eq( none[0], 'y' ); }
    test_pass("cxx03:ctype::do_toupper(charT* low, const charT* high)");

    {   char b[6] = { 'A', 'b', '3', 'Z', '-', 'Q' };
        const char* end = ct.tolower(b, b + 6);
        test_true( end == b + 6 );
        test_eq( b[0], 'a' );
        test_eq( b[1], 'b' );
        test_eq( b[2], '3' );
        test_eq( b[3], 'z' );
        char none[1] = { 'Y' };
        ct.tolower(none, none);
        test_eq( none[0], 'Y' ); }
    test_pass("cxx03:ctype::do_tolower(charT* low, const charT* high)");

    {   const char src[4] = { 'a', 'B', '9', ' ' };
        char to[4] = { 0, 0, 0, 0 };
        const char* end = ct.widen(src, src + 4, to);
        test_true( end == src + 4 );
        test_eq( to[0], 'a' );
        test_eq( to[1], 'B' );
        test_eq( to[3], ' ' );
        char none[1] = { 0 };
        ct.widen(src, src, none);
        test_eq( none[0], 0 ); }
    test_pass("cxx03:ctype::do_widen(const char* low, const char* high, charT* to)");

    {   const char src[4] = { 'a', 'B', '9', ' ' };
        char to[4] = { 0, 0, 0, 0 };
        const char* end = ct.narrow(src, src + 4, '?', to);
        test_true( end == src + 4 );
        test_eq( to[0], 'a' );
        test_eq( to[2], '9' );
        char none[1] = { 0 };
        ct.narrow(src, src, '?', none);
        test_eq( none[0], 0 ); }
    test_pass("cxx03:ctype::do_narrow(const charT* low, const charT* high, char dfault, char* to)");

    {   const char src[4] = { 'a', '1', ' ', 'Z' };
        STD::ctype<char>::mask m[4];
        for (int i = 0; i < 4; ++i) m[i] = STD::ctype<char>::mask(0);
        const char* end = ct.is(src, src + 4, m);
        test_true( end == src + 4 );
        test_true( (m[0] & STD::ctype<char>::alpha) != 0 );
        test_true( (m[1] & STD::ctype<char>::digit) != 0 );
        test_true( (m[2] & STD::ctype<char>::space) != 0 );
        test_true( (m[3] & STD::ctype<char>::upper) != 0 );
        test_true( (m[1] & STD::ctype<char>::alpha) == 0 ); }
    test_pass("cxx03:ctype::do_is(const charT* low, const charT* high, mask* vec)");
}

TEST_CASE(locale, money_string_forms_cxx03) {
    {   STD::ostringstream out;
        const STD::money_put<char>& mp = STD::use_facet<STD::money_put<char> >(out.getloc());
        mp.put(LbOut(out), false, out, ' ', STD::string("1234"));
        test_true( out.str().size() > 0 );
        test_true( out.str().find('1') != STD::string::npos );

        STD::ostringstream out2;
        mp.put(LbOut(out2), false, out2, ' ', STD::string(""));
        test_true( out2.str().size() == 0 || out2.str().size() > 0 ); }
    test_pass("cxx03:money_put::do_put(iter_type, bool, ios_base&, char_type, const string_type&)");

    {   STD::istringstream in("1234");
        const STD::money_get<char>& mg = STD::use_facet<STD::money_get<char> >(in.getloc());
        STD::ios_base::iostate err = STD::ios_base::goodbit;
        STD::string digits;
        mg.get(LbIn(in), LbIn(), false, in, err, digits);
        test_true( (err & STD::ios_base::failbit) != STD::ios_base::failbit );
        test_eq( digits, STD::string("1234") );

        STD::istringstream bad("zz");
        STD::ios_base::iostate err2 = STD::ios_base::goodbit;
        STD::string none;
        mg.get(LbIn(bad), LbIn(), false, bad, err2, none);
        test_true( (err2 & STD::ios_base::failbit) == STD::ios_base::failbit ); }
    test_pass("cxx03:money_get::do_get(iter_type, iter_type, bool, ios_base&, iostate&, string_type&)");
}

TEST_CASE(locale, time_get_format_range_cxx11) {
#if _TST_HAS_CXX11_LIB_MEMBERS
    const STD::time_get<char>& tg =
        STD::use_facet<STD::time_get<char> >(STD::locale::classic());
    {   STD::istringstream in("2020-03-04");
        STD::ios_base::iostate err = STD::ios_base::goodbit;
        STD::tm t;
        STD::memset(&t, 0, sizeof t);
        const char fmt[] = "%Y-%m-%d";
        tg.get(LbIn(in), LbIn(), in, err, &t, fmt, fmt + 8);
        test_true( (err & STD::ios_base::failbit) != STD::ios_base::failbit );
        test_eq( t.tm_year, 120 );
        test_eq( t.tm_mon, 2 );
        test_eq( t.tm_mday, 4 ); }

    {   STD::istringstream bad("zz");
        STD::ios_base::iostate err = STD::ios_base::goodbit;
        STD::tm t;
        STD::memset(&t, 0, sizeof t);
        const char fmt[] = "%Y";
        tg.get(LbIn(bad), LbIn(), bad, err, &t, fmt, fmt + 2);
        test_true( (err & STD::ios_base::failbit) == STD::ios_base::failbit ); }
    test_pass("cxx11:time_get::get(iter_type, iter_type, ios_base&, iostate&, tm*, const char_type*, const char_type*)");
#else
    TEST_NOTE("the format-range form of time_get::get is C++11");
    TEST_SKIP_N(5);
    test_skip("cxx11:time_get::get(iter_type, iter_type, ios_base&, iostate&, tm*, const char_type*, const char_type*)");
#endif
}


#if TEST_TARGET_CXX >= 2011
struct WscCvt : STD::codecvt<wchar_t, char, STD_NS mbstate_t> { };  // public dtor (libstdc++ rejects the facet itself)
typedef STD::wstring_convert<WscCvt>                  WscConv;

static bool wsc_utf8_locale() {
    static const char* const names[] = { ".UTF-8", ".utf8", "C.UTF-8", "en_US.UTF-8", ".65001" };
    for (unsigned i = 0; i < sizeof names / sizeof names[0]; ++i)
        if (STD::setlocale(LC_ALL, names[i]) != 0) return true;
    return false;
}

static bool wsc_facet_decodes_utf8() {
    if (!wsc_utf8_locale()) return false;
    STD::string u8;
    u8 += (char)0xE3; u8 += (char)0x81; u8 += (char)0x82;
#if TEST_HAS_EH
    try {
        WscConv c;
        return c.from_bytes(u8).size() == 1;
    } catch (...) {
        return false;
    }
#else
    WscConv c(STD::string("!"), STD::wstring(L"!"));
    return c.from_bytes(u8).size() == 1;
#endif
}
#endif

TEST_CASE(locale, wstring_convert_cxx11) {
#if TEST_TARGET_CXX >= 2011
    STD::string saved(STD::setlocale(LC_ALL, NULL));
    const bool  utf8 = wsc_facet_decodes_utf8();

    STD::string u8;
    u8 += (char)0xE3; u8 += (char)0x81; u8 += (char)0x82; u8 += 'A';

    {   WscConv c;
        test_true( sizeof(WscConv) > 0 );
        test_pass("cxx11:wstring_convert");
        test_true(( STD::is_same<WscConv::byte_string, STD::string>::value ));
        test_pass("cxx11:wstring_convert::byte_string");
        test_true(( STD::is_same<WscConv::wide_string, STD::wstring>::value ));
        test_pass("cxx11:wstring_convert::wide_string");
        test_true(( STD::is_same<WscConv::state_type, WscCvt::state_type>::value ));
        test_pass("cxx11:wstring_convert::state_type");
        test_true( sizeof(WscConv::int_type) > 0 );
        test_pass("cxx11:wstring_convert::int_type");
        test_pass("cxx11:wstring_convert default construction");
        test_pass("cxx11:wstring_convert::~wstring_convert"); }

    {   WscConv c(new WscCvt);
        test_eq( (long)c.converted(), 0L );
        test_pass("cxx11:wstring_convert::wstring_convert(Codecvt* pcvt)"); }

    {   WscCvt::state_type st;
        STD::memset(&st, 0, sizeof st);
        WscConv c(new WscCvt, st);
        test_eq( (long)c.converted(), 0L );
        test_pass("cxx11:wstring_convert::wstring_convert(Codecvt* pcvt, state_type state)"); }

    {   WscConv c(STD::string("<B>"), STD::wstring(L"<W>"));
        test_eq( (long)c.converted(), 0L );
        test_pass("cxx11:wstring_convert::wstring_convert(const byte_string& byte_err, const wide_string& wide_err)"); }

    {   WscConv c;
        STD::wstring w = c.from_bytes('Z');
        test_eq( (long)w.size(), 1L );
        test_true( w[0] == (wchar_t)'Z' );
        test_pass("cxx11:wstring_convert::from_bytes(char)"); }

    {   WscConv c;
        STD::wstring w = c.from_bytes("ab");
        test_eq( (long)w.size(), 2L );
        test_true( w[0] == (wchar_t)'a' && w[1] == (wchar_t)'b' );
        test_pass("cxx11:wstring_convert::from_bytes(const char*)"); }

    {   WscConv c;
        STD::wstring w = c.from_bytes(STD::string("abc"));
        test_eq( (long)w.size(), 3L );
        test_pass("cxx11:wstring_convert::from_bytes(const byte_string&)"); }

    {   WscConv c;
        const char*  b = "abcd";
        STD::wstring w = c.from_bytes(b, b + 2);
        test_eq( (long)w.size(), 2L );
        test_eq( (long)c.converted(), 2L );
        test_pass("cxx11:wstring_convert::from_bytes(const char* first, const char* last)"); }

    if (utf8) {
        WscConv     c;
        STD::string bad;
        bad += (char)0xFF;
        bool threw = false;
        try { c.from_bytes(bad); } catch (const STD::range_error&) { threw = true; } catch (...) { }
        test_true( threw );
    } else {
        TEST_NOTE("this locale accepts any byte, so from_bytes cannot fail");
        TEST_SKIP1();
    }
    test_pass("cxx11:wstring_convert::from_bytes (conversion error without wide_err -> range_error)");

    if (utf8) {
        WscConv      c(STD::string("<B>"), STD::wstring(L"<W>"));
        STD::string  bad;
        bad += (char)0xFF;
        STD::wstring w = c.from_bytes(bad);
        test_eq( (long)w.size(), 3L );
        test_true( w[0] == (wchar_t)'<' );
    } else {
        TEST_NOTE("this locale accepts any byte, so from_bytes cannot fail");
        TEST_SKIP_N(2);
    }
    test_pass("cxx11:wstring_convert::from_bytes (conversion error with wide_err -> wide_err)");

    {   WscConv     c;
        STD::string b = c.to_bytes((wchar_t)'Q');
        test_eq( (long)b.size(), 1L );
        test_true( b[0] == 'Q' );
        test_pass("cxx11:wstring_convert::to_bytes(Elem)"); }

    {   WscConv     c;
        STD::string b = c.to_bytes(L"xy");
        test_eq( (long)b.size(), 2L );
        test_pass("cxx11:wstring_convert::to_bytes(const Elem*)"); }

    {   WscConv     c;
        STD::string b = c.to_bytes(STD::wstring(L"xyz"));
        test_eq( (long)b.size(), 3L );
        test_pass("cxx11:wstring_convert::to_bytes(const wide_string&)"); }

    {   WscConv        c;
        const wchar_t* w = L"wxyz";
        STD::string    b = c.to_bytes(w, w + 2);
        test_eq( (long)b.size(), 2L );
        test_eq( (long)c.converted(), 2L );
        test_pass("cxx11:wstring_convert::to_bytes(const Elem* first, const Elem* last)"); }

    if (utf8) {
        WscConv      c;
        STD::wstring bad;
        bad.push_back((wchar_t)0xD800);
        bool threw = false;
        try { c.to_bytes(bad); } catch (const STD::range_error&) { threw = true; } catch (...) { }
        test_true( threw );
    } else {
        TEST_NOTE("this locale maps every wide unit, so to_bytes cannot fail");
        TEST_SKIP1();
    }
    test_pass("cxx11:wstring_convert::to_bytes (conversion error without byte_err -> byte_err)");

    if (utf8) {
        WscConv      c(STD::string("<B>"), STD::wstring(L"<W>"));
        STD::wstring bad;
        bad.push_back((wchar_t)0xD800);
        STD::string  b = c.to_bytes(bad);
        test_eq( (long)b.size(), 3L );
        test_true( b[0] == '<' );
    } else {
        TEST_NOTE("this locale maps every wide unit, so to_bytes cannot fail");
        TEST_SKIP_N(2);
    }
    test_pass("cxx11:wstring_convert::to_bytes (conversion error with byte_err -> byte_err)");

    {   WscConv     c;
        STD::string src = utf8 ? u8 : STD::string("ab");
        c.from_bytes(src);
        test_eq( (long)c.converted(), (long)src.size() );
        test_pass("cxx11:wstring_convert::converted"); }

    {   WscConv          c;
        WscConv::state_type st = c.state();
        test_true( sizeof(st) > 0 );
        test_pass("cxx11:wstring_convert::state"); }

    STD::setlocale(LC_ALL, saved.c_str());
#else
    TEST_NOTE("wstring_convert is C++11");
    TEST_SKIP_N(29);
    test_skip("cxx11:wstring_convert");
    test_skip("cxx11:wstring_convert::byte_string");
    test_skip("cxx11:wstring_convert::wide_string");
    test_skip("cxx11:wstring_convert::state_type");
    test_skip("cxx11:wstring_convert::int_type");
    test_skip("cxx11:wstring_convert default construction");
    test_skip("cxx11:wstring_convert::~wstring_convert");
    test_skip("cxx11:wstring_convert::wstring_convert(Codecvt* pcvt)");
    test_skip("cxx11:wstring_convert::wstring_convert(Codecvt* pcvt, state_type state)");
    test_skip("cxx11:wstring_convert::wstring_convert(const byte_string& byte_err, const wide_string& wide_err)");
    test_skip("cxx11:wstring_convert::from_bytes(char)");
    test_skip("cxx11:wstring_convert::from_bytes(const char*)");
    test_skip("cxx11:wstring_convert::from_bytes(const byte_string&)");
    test_skip("cxx11:wstring_convert::from_bytes(const char* first, const char* last)");
    test_skip("cxx11:wstring_convert::from_bytes (conversion error without wide_err -> range_error)");
    test_skip("cxx11:wstring_convert::from_bytes (conversion error with wide_err -> wide_err)");
    test_skip("cxx11:wstring_convert::to_bytes(Elem)");
    test_skip("cxx11:wstring_convert::to_bytes(const Elem*)");
    test_skip("cxx11:wstring_convert::to_bytes(const wide_string&)");
    test_skip("cxx11:wstring_convert::to_bytes(const Elem* first, const Elem* last)");
    test_skip("cxx11:wstring_convert::to_bytes (conversion error without byte_err -> byte_err)");
    test_skip("cxx11:wstring_convert::to_bytes (conversion error with byte_err -> byte_err)");
    test_skip("cxx11:wstring_convert::converted");
    test_skip("cxx11:wstring_convert::state");
#endif
}

TEST_CASE(locale, wstring_convert_cxx14) {
#if TEST_TARGET_CXX >= 2014
    test_true(( !STD::is_convertible<WscCvt*, WscConv>::value ));
    test_pass("cxx14:wstring_convert::wstring_convert(Codecvt* pcvt) is explicit");
    test_true(( !STD::is_convertible<STD::string, WscConv>::value ));
    test_pass("cxx14:wstring_convert::wstring_convert(const byte_string& byte_err, const wide_string& wide_err) is explicit");
    {   WscConv c;
        test_eq( (long)c.converted(), 0L );
        test_pass("cxx14:wstring_convert::converted() const noexcept"); }
    TEST_SKIP_WAT("Open Watcom cannot see a private copy constructor through is_copy_constructible (A47)");
    test_true( !STD::is_copy_constructible<WscConv>::value );
    test_pass("cxx14:wstring_convert::wstring_convert(const wstring_convert&) = delete");
    test_true( !STD::is_copy_assignable<WscConv>::value );
    test_pass("cxx14:wstring_convert::operator=(const wstring_convert&) = delete");
#else
    TEST_NOTE("these refinements are C++14 (LWG 2176)");
    TEST_SKIP_N(5);
    test_skip("cxx14:wstring_convert::wstring_convert(Codecvt* pcvt) is explicit");
    test_skip("cxx14:wstring_convert::wstring_convert(const byte_string& byte_err, const wide_string& wide_err) is explicit");
    test_skip("cxx14:wstring_convert::converted() const noexcept");
    test_skip("cxx14:wstring_convert::wstring_convert(const wstring_convert&) = delete");
    test_skip("cxx14:wstring_convert::operator=(const wstring_convert&) = delete");
#endif
}

TEST_CASE(locale, wstring_convert_cxx20) {
#if TEST_TARGET_CXX >= 2020
    WscConv c;
    test_eq( (long)c.converted(), 0L );
    test_pass("cxx20:wstring_convert::wstring_convert() (separate delegating constructor)");
#else
    TEST_NOTE("the defaulted constructor was split out by P0935R0 (C++20)");
    TEST_SKIP1();
    test_skip("cxx20:wstring_convert::wstring_convert() (separate delegating constructor)");
#endif
}

TEST_CASE(locale, wbuffer_convert_cxx11) {
#if TEST_TARGET_CXX >= 2011
    STD::string saved(STD::setlocale(LC_ALL, NULL));
    (void)wsc_utf8_locale();
    typedef STD::wbuffer_convert<WscCvt> WbConv;

    {   WbConv b;
        test_true( b.rdbuf() == 0 );
        test_pass("cxx11:wbuffer_convert");
        test_pass("cxx11:wbuffer_convert default construction");
        test_pass("cxx11:wbuffer_convert::~wbuffer_convert"); }

    {   STD::stringstream ss;
        WbConv            b(ss.rdbuf());
        STD::basic_streambuf<wchar_t>* sb = &b;
        test_true( sb != 0 );
        test_pass("cxx11:wbuffer_convert is a basic_streambuf<Elem, Tr>");
        test_true(( STD::is_same<WbConv::state_type, WscCvt::state_type>::value ));
        test_pass("cxx11:wbuffer_convert::state_type");
        test_true( b.rdbuf() == ss.rdbuf() );
        test_pass("cxx11:wbuffer_convert::rdbuf"); }

    {   STD::stringstream ss1, ss2;
        WbConv            b(ss1.rdbuf());
        STD_NS streambuf* old = b.rdbuf(ss2.rdbuf());
        test_true( old == ss1.rdbuf() );
        test_true( b.rdbuf() == ss2.rdbuf() );
        test_pass("cxx11:wbuffer_convert::rdbuf(streambuf* bytebuf)"); }

    {   WscCvt::state_type st;
        STD::memset(&st, 0, sizeof st);
        STD::stringstream ss;
        WbConv            b(ss.rdbuf(), new WscCvt, st);
        test_true( b.rdbuf() == ss.rdbuf() );
        test_pass("cxx11:wbuffer_convert::wbuffer_convert(streambuf* bytebuf, Codecvt* pcvt, state_type state)");
        WbConv::state_type s2 = b.state();
        test_true( sizeof(s2) > 0 );
        test_pass("cxx11:wbuffer_convert::state"); }

    {   STD::stringstream ss;
        WbConv            b(ss.rdbuf());
        STD::wostream     wos(&b);
        wos << (wchar_t)'h' << (wchar_t)'i';
        wos.flush();
        test_eq( ss.str(), STD::string("hi") );
        test_pass("cxx11:wbuffer_convert wide output via byte streambuf"); }

    {   STD::stringstream ss("hi");
        WbConv            b(ss.rdbuf());
        STD::wistream     wis(&b);
        wchar_t           w[4];
        wis.read(w, 2);
        test_eq( (long)wis.gcount(), 2L );
        test_true( w[0] == (wchar_t)'h' && w[1] == (wchar_t)'i' );
        test_pass("cxx11:wbuffer_convert wide input via byte streambuf"); }

    STD::setlocale(LC_ALL, saved.c_str());
#else
    TEST_NOTE("wbuffer_convert is C++11");
    TEST_SKIP_N(11);
    test_skip("cxx11:wbuffer_convert");
    test_skip("cxx11:wbuffer_convert is a basic_streambuf<Elem, Tr>");
    test_skip("cxx11:wbuffer_convert::state_type");
    test_skip("cxx11:wbuffer_convert::wbuffer_convert(streambuf* bytebuf, Codecvt* pcvt, state_type state)");
    test_skip("cxx11:wbuffer_convert default construction");
    test_skip("cxx11:wbuffer_convert::~wbuffer_convert");
    test_skip("cxx11:wbuffer_convert::rdbuf");
    test_skip("cxx11:wbuffer_convert::rdbuf(streambuf* bytebuf)");
    test_skip("cxx11:wbuffer_convert::state");
    test_skip("cxx11:wbuffer_convert wide input via byte streambuf");
    test_skip("cxx11:wbuffer_convert wide output via byte streambuf");
#endif
}

TEST_CASE(locale, wbuffer_convert_cxx14_cxx20) {
#if TEST_TARGET_CXX >= 2014
    typedef STD::wbuffer_convert<WscCvt> WbConv;
    test_true(( !STD::is_convertible<STD_NS streambuf*, WbConv>::value ));
    test_pass("cxx14:wbuffer_convert::wbuffer_convert(streambuf* bytebuf, Codecvt* pcvt, state_type state) is explicit");
    TEST_SKIP_WAT("Open Watcom cannot see a private copy constructor through is_copy_constructible (A47)");
    test_true( !STD::is_copy_constructible<WbConv>::value );
    test_pass("cxx14:wbuffer_convert::wbuffer_convert(const wbuffer_convert&) = delete");
    test_true( !STD::is_copy_assignable<WbConv>::value );
    test_pass("cxx14:wbuffer_convert::operator=(const wbuffer_convert&) = delete");
#else
    TEST_NOTE("these refinements are C++14 (LWG 2176)");
    TEST_SKIP_N(3);
    test_skip("cxx14:wbuffer_convert::wbuffer_convert(streambuf* bytebuf, Codecvt* pcvt, state_type state) is explicit");
    test_skip("cxx14:wbuffer_convert::wbuffer_convert(const wbuffer_convert&) = delete");
    test_skip("cxx14:wbuffer_convert::operator=(const wbuffer_convert&) = delete");
#endif
#if TEST_TARGET_CXX >= 2020
    {   STD::wbuffer_convert<WscCvt> b;
        test_true( b.rdbuf() == 0 );
        test_pass("cxx20:wbuffer_convert::wbuffer_convert() (separate delegating constructor)"); }
#else
    TEST_NOTE("the defaulted constructor was split out by P0935R0 (C++20)");
    TEST_SKIP1();
    test_skip("cxx20:wbuffer_convert::wbuffer_convert() (separate delegating constructor)");
#endif
}
