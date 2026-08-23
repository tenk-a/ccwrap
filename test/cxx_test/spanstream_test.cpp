#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2023 && _TST_TEST_HAS(<spanstream>)
#include <spanstream>
#include <span>
#include <string>
#include <cstring>
#include <type_traits>
#include <utility>

TEST_CASE(spanstream, ospanstream) {
    char sbuf[32];
    STD::span<char> sp(sbuf, 32);
    STD::ospanstream oss(sp);
    oss << "n=" << 99;
    STD::span<char> written = oss.span();
    test_eq( written.size(), (STD::size_t)4 );
    test_eq( STD::strncmp(written.data(), "n=99", 4), 0 );
    test_pass("cxx23:ospanstream");
}

TEST_CASE(spanstream, ispanstream) {
    char ibuf[] = "7 abc";
    STD::span<char> isp(ibuf, STD::strlen(ibuf));
    STD::ispanstream iss(isp);
    int k; STD::string w;
    iss >> k >> w;
    test_eq( k, 7 );
    test_eq( w, STD::string("abc") );
    test_pass("cxx23:ispanstream");
}

TEST_CASE(spanstream, seek) {
    char ibuf[] = "0123456789";
    STD::span<char> isp(ibuf, 10);
    STD::ispanstream is(isp);

    char c = 0;
    test_eq( (long)is.tellg(), 0L );
    is.seekg(3);
    is.get(c);
    test_eq( c, '3' );
    is.seekg(-2, STD::ios_base::end);
    is.get(c);
    test_eq( c, '8' );
    is.seekg(99);
    test_true( is.fail() );

    char obuf[8];
    STD::span<char> osp(obuf, 8);
    STD::ospanstream os(osp);
    os << "abcd";
    test_eq( (long)os.tellp(), 4L );
    os.seekp(1);
    os << "XY";
    test_eq( (long)os.tellp(), 3L );

    STD::span<char> w = os.span();
    test_eq( w.size(), (STD::size_t)3 );
    test_eq( STD::strncmp(w.data(), "aXY", 3), 0 );
    test_eq( obuf[3], 'd' );
    test_pass("cxx23:seek");
}

#else
TEST_CASE_SKIP(spanstream, ospanstream)
TEST_CASE_SKIP(spanstream, ispanstream)
TEST_CASE_SKIP(spanstream, seek)
#endif

#if TEST_TARGET_CXX >= 2023 && _TST_TEST_HAS(<spanstream>)
TEST_CASE(spanstream, member_types_cxx23) {
    typedef STD::char_traits<char> Tr;

    typedef STD::basic_spanbuf<char> SB;
    char raw[8];
    STD::spanbuf sb_obj(STD::span<char>(raw, 8));
    test_true(( STD::is_same<SB, STD::spanbuf>::value ));
    test_true( sb_obj.span().data() == raw );
    test_pass("cxx23:basic_spanbuf");
    test_true(( STD::is_same<SB::char_type, char>::value ));
    test_pass("cxx23:basic_spanbuf::char_type");
    test_true(( STD::is_same<SB::traits_type, Tr>::value ));
    test_pass("cxx23:basic_spanbuf::traits_type");
    test_true(( STD::is_same<SB::int_type, Tr::int_type>::value ));
    test_pass("cxx23:basic_spanbuf::int_type");
    test_true(( STD::is_same<SB::pos_type, Tr::pos_type>::value ));
    test_pass("cxx23:basic_spanbuf::pos_type");
    test_true(( STD::is_same<SB::off_type, Tr::off_type>::value ));
    test_pass("cxx23:basic_spanbuf::off_type");

    typedef STD::basic_ispanstream<char> IS;
    test_true(( STD::is_same<IS, STD::ispanstream>::value ));
    test_pass("cxx23:basic_ispanstream");
    test_true(( STD::is_same<IS::char_type, char>::value ));
    test_pass("cxx23:basic_ispanstream::char_type");
    test_true(( STD::is_same<IS::traits_type, Tr>::value ));
    test_pass("cxx23:basic_ispanstream::traits_type");
    test_true(( STD::is_same<IS::int_type, Tr::int_type>::value ));
    test_pass("cxx23:basic_ispanstream::int_type");
    test_true(( STD::is_same<IS::pos_type, Tr::pos_type>::value ));
    test_pass("cxx23:basic_ispanstream::pos_type");
    test_true(( STD::is_same<IS::off_type, Tr::off_type>::value ));
    test_pass("cxx23:basic_ispanstream::off_type");

    typedef STD::basic_ospanstream<char> OS;
    test_true(( STD::is_same<OS, STD::ospanstream>::value ));
    test_pass("cxx23:basic_ospanstream");
    test_true(( STD::is_same<OS::char_type, char>::value ));
    test_pass("cxx23:basic_ospanstream::char_type");
    test_true(( STD::is_same<OS::traits_type, Tr>::value ));
    test_pass("cxx23:basic_ospanstream::traits_type");
    test_true(( STD::is_same<OS::int_type, Tr::int_type>::value ));
    test_pass("cxx23:basic_ospanstream::int_type");
    test_true(( STD::is_same<OS::pos_type, Tr::pos_type>::value ));
    test_pass("cxx23:basic_ospanstream::pos_type");
    test_true(( STD::is_same<OS::off_type, Tr::off_type>::value ));
    test_pass("cxx23:basic_ospanstream::off_type");

    typedef STD::basic_spanstream<char> SS;
    char both[16];
    STD::spanstream ss_obj((STD::span<char>(both, 16)));
    ss_obj << 21;
    int back = 0;
    ss_obj >> back;
    test_eq( back, 21 );
    test_true(( STD::is_same<SS, STD::spanstream>::value ));
    test_pass("cxx23:basic_spanstream");
    test_true(( STD::is_same<SS::char_type, char>::value ));
    test_pass("cxx23:basic_spanstream::char_type");
    test_true(( STD::is_same<SS::traits_type, Tr>::value ));
    test_pass("cxx23:basic_spanstream::traits_type");
    test_true(( STD::is_same<SS::int_type, Tr::int_type>::value ));
    test_pass("cxx23:basic_spanstream::int_type");
    test_true(( STD::is_same<SS::pos_type, Tr::pos_type>::value ));
    test_pass("cxx23:basic_spanstream::pos_type");
    test_true(( STD::is_same<SS::off_type, Tr::off_type>::value ));
    test_pass("cxx23:basic_spanstream::off_type");

    test_true(( STD::is_same<STD::wspanbuf, STD::basic_spanbuf<wchar_t> >::value ));
    test_pass("cxx23:wspanbuf");
    test_true(( STD::is_same<STD::wispanstream, STD::basic_ispanstream<wchar_t> >::value ));
    test_pass("cxx23:wispanstream");
    test_true(( STD::is_same<STD::wospanstream, STD::basic_ospanstream<wchar_t> >::value ));
    test_pass("cxx23:wospanstream");
    test_true(( STD::is_same<STD::wspanstream, STD::basic_spanstream<wchar_t> >::value ));
    test_pass("cxx23:wspanstream");
}

TEST_CASE(spanstream, span_access_cxx23) {
    char a[16], b[16];
    STD::spanbuf sb((STD::span<char>(a, 16)));
    test_true( sb.span().data() == a );
    sb.span(STD::span<char>(b, 16));
    test_true( sb.span().data() == b );
    test_pass("cxx23:spanbuf::span (get/set)");

    char obuf[16];
    STD::ospanstream os((STD::span<char>(obuf, 16)));
    os << "ab";
    test_eq( os.span().size(), (STD::size_t)2 );
    test_true( os.span().data() == obuf );
    test_pass("cxx23:ospanstream::span");

    char ibuf[] = "xyz";
    STD::ispanstream is(STD::span<char>(ibuf, 3));
    test_eq( is.span().size(), (STD::size_t)3 );
    char c = 0;
    is.get(c);
    test_eq( c, 'x' );
    test_eq( is.span().size(), (STD::size_t)3 );
    test_pass("cxx23:ispanstream::span");

    STD::spanstream ss((STD::span<char>(a, 16)));
    test_true( ss.rdbuf() != 0 );
    test_true( tst_type_is<STD::spanbuf*>(ss.rdbuf()) );
    ss << "q";
    test_eq( ss.rdbuf()->span().size(), (STD::size_t)1 );
    test_pass("cxx23:spanstream::rdbuf");
}

#if defined(__WATCOMC__)
TEST_CASE_SKIP(spanstream, swap_and_assign_cxx23)
#else
TEST_CASE(spanstream, swap_and_assign_cxx23) {
    char a[16] = { 0 }, b[16] = { 0 };

    STD::spanbuf sa((STD::span<char>(a, 16)));
    STD::spanbuf sbb((STD::span<char>(b, 16)));
    sa.swap(sbb);
    test_true( sa.span().data() == b );
    test_true( sbb.span().data() == a );
    test_pass("cxx23:spanbuf::swap");

    STD::spanstream x((STD::span<char>(a, 16)));
    STD::spanstream y((STD::span<char>(b, 16)));
    x << "1";
    y << "22";
    x.swap(y);
    test_eq( x.span().size(), (STD::size_t)2 );
    test_eq( y.span().size(), (STD::size_t)1 );
    test_pass("cxx23:spanstream::swap");

#if _TST_HAS_RVALUE_REF
    STD::spanbuf ma((STD::span<char>(a, 16)));
    STD::spanbuf mb;
    mb = STD::move(ma);
    test_true( mb.span().data() == a );
    TEST_SKIP_VC120("MSVC 12 is_copy_assignable answers true even for a DELETED copy assignment (measured on plain cl); vc bugs A26");
    test_true( !STD::is_copy_assignable<STD::spanbuf>::value );
    test_pass("cxx23:basic_spanbuf::operator=");
#else
    TEST_NOTE("no rvalue references here: move assignment falls back to the copy, which is deleted");
    TEST_SKIP_N(2);
    test_skip("cxx23:basic_spanbuf::operator=");
#endif

#if _TST_HAS_RVALUE_REF
    char src[] = "5 6";
    STD::ispanstream ia(STD::span<char>(src, 3));
    STD::ispanstream ib((STD::span<char>()));
    ib = STD::move(ia);
    int got = 0;
    ib >> got;
    test_eq( got, 5 );
    TEST_SKIP_VC120("MSVC 12 is_copy_assignable answers true even for a DELETED copy assignment (measured on plain cl); vc bugs A26");
    test_true( !STD::is_copy_assignable<STD::ispanstream>::value );
    test_pass("cxx23:basic_ispanstream::operator=");
#else
    TEST_NOTE("no rvalue references here: move assignment falls back to the copy, which is deleted");
    TEST_SKIP_N(2);
    test_skip("cxx23:basic_ispanstream::operator=");
#endif

#if _TST_HAS_RVALUE_REF
    STD::ospanstream oa((STD::span<char>(b, 16)));
    STD::ospanstream ob((STD::span<char>()));
    ob = STD::move(oa);
    ob << "z";
    test_eq( ob.span().size(), (STD::size_t)1 );
    TEST_SKIP_VC120("MSVC 12 is_copy_assignable answers true even for a DELETED copy assignment (measured on plain cl); vc bugs A26");
    test_true( !STD::is_copy_assignable<STD::ospanstream>::value );
    test_pass("cxx23:basic_ospanstream::operator=");
#else
    TEST_NOTE("no rvalue references here: move assignment falls back to the copy, which is deleted");
    TEST_SKIP_N(2);
    test_skip("cxx23:basic_ospanstream::operator=");
#endif

#if _TST_HAS_RVALUE_REF
    STD::spanstream sa2((STD::span<char>(a, 16)));
    STD::spanstream sb2((STD::span<char>()));
    sb2 = STD::move(sa2);
    sb2 << "w";
    test_eq( sb2.span().size(), (STD::size_t)1 );
    TEST_SKIP_VC120("MSVC 12 is_copy_assignable answers true even for a DELETED copy assignment (measured on plain cl); vc bugs A26");
    test_true( !STD::is_copy_assignable<STD::spanstream>::value );
    test_pass("cxx23:basic_spanstream::operator=");
#else
    TEST_NOTE("no rvalue references here: move assignment falls back to the copy, which is deleted");
    TEST_SKIP_N(2);
    test_skip("cxx23:basic_spanstream::operator=");
#endif
}
#endif

namespace {
struct CcwSpanProbe : public STD::spanbuf {
    CcwSpanProbe(STD::span<char> s, STD::ios_base::openmode m) : STD::spanbuf(s, m) {}
    pos_type call_seekoff(off_type o, STD::ios_base::seekdir d, STD::ios_base::openmode m)
                                        { return this->seekoff(o, d, m); }
    pos_type call_seekpos(pos_type p, STD::ios_base::openmode m)
                                        { return this->seekpos(p, m); }
    STD::streambuf* call_setbuf(char* s, STD::streamsize n) { return this->setbuf(s, n); }
};
}

TEST_CASE(spanstream, spanbuf_virtuals_cxx23) {
    char data[] = "abcdef";

    {
        CcwSpanProbe b(STD::span<char>(data, 6), STD::ios_base::in);
        STD::spanbuf::pos_type p = b.call_seekoff(2, STD::ios_base::beg, STD::ios_base::in);
        test_eq( (long)(STD::streamoff)p, 2L );
        test_eq( (int)b.sgetc(), (int)'c' );
        STD::spanbuf::pos_type e = b.call_seekoff(0, STD::ios_base::end, STD::ios_base::in);
        test_eq( (long)(STD::streamoff)e, 6L );
        STD::spanbuf::pos_type bad =
            b.call_seekoff(100, STD::ios_base::beg, STD::ios_base::in);
        test_eq( (long)(STD::streamoff)bad, -1L );
        test_pass("cxx23:basic_spanbuf::seekoff");
    }
    {
        CcwSpanProbe b(STD::span<char>(data, 6), STD::ios_base::in);
        STD::spanbuf::pos_type p = b.call_seekpos(STD::spanbuf::pos_type(4),
                                                  STD::ios_base::in);
        test_eq( (long)(STD::streamoff)p, 4L );
        test_eq( (int)b.sgetc(), (int)'e' );
        test_pass("cxx23:basic_spanbuf::seekpos");
    }
    {
        CcwSpanProbe b(STD::span<char>(data, 6), STD::ios_base::in);
        char other[] = "zz";
        STD::streambuf* r = b.call_setbuf(other, 2);
        test_true( r == &b );
        test_eq( (int)b.sgetc(), (int)'z' );
        test_pass("cxx23:basic_spanbuf::setbuf");
    }
}
#else
TEST_CASE(spanstream, member_types_cxx23) {
    test_skip("cxx23:basic_spanbuf");
    test_skip("cxx23:basic_spanbuf::char_type");
    test_skip("cxx23:basic_spanbuf::traits_type");
    test_skip("cxx23:basic_spanbuf::int_type");
    test_skip("cxx23:basic_spanbuf::pos_type");
    test_skip("cxx23:basic_spanbuf::off_type");
    test_skip("cxx23:basic_ispanstream");
    test_skip("cxx23:basic_ispanstream::char_type");
    test_skip("cxx23:basic_ispanstream::traits_type");
    test_skip("cxx23:basic_ispanstream::int_type");
    test_skip("cxx23:basic_ispanstream::pos_type");
    test_skip("cxx23:basic_ispanstream::off_type");
    test_skip("cxx23:basic_ospanstream");
    test_skip("cxx23:basic_ospanstream::char_type");
    test_skip("cxx23:basic_ospanstream::traits_type");
    test_skip("cxx23:basic_ospanstream::int_type");
    test_skip("cxx23:basic_ospanstream::pos_type");
    test_skip("cxx23:basic_ospanstream::off_type");
    test_skip("cxx23:basic_spanstream");
    test_skip("cxx23:basic_spanstream::char_type");
    test_skip("cxx23:basic_spanstream::traits_type");
    test_skip("cxx23:basic_spanstream::int_type");
    test_skip("cxx23:basic_spanstream::pos_type");
    test_skip("cxx23:basic_spanstream::off_type");
    test_skip("cxx23:wspanbuf");
    test_skip("cxx23:wispanstream");
    test_skip("cxx23:wospanstream");
    test_skip("cxx23:wspanstream");
}
TEST_CASE(spanstream, span_access_cxx23) {
    test_skip("cxx23:spanbuf::span (get/set)");
    test_skip("cxx23:ospanstream::span");
    test_skip("cxx23:ispanstream::span");
    test_skip("cxx23:spanstream::rdbuf");
}
TEST_CASE(spanstream, swap_and_assign_cxx23) {
    test_skip("cxx23:spanbuf::swap");
    test_skip("cxx23:spanstream::swap");
    test_skip("cxx23:basic_spanbuf::operator=");
    test_skip("cxx23:basic_ispanstream::operator=");
    test_skip("cxx23:basic_ospanstream::operator=");
    test_skip("cxx23:basic_spanstream::operator=");
}
TEST_CASE(spanstream, spanbuf_virtuals_cxx23) {
    test_skip("cxx23:basic_spanbuf::seekoff");
    test_skip("cxx23:basic_spanbuf::seekpos");
    test_skip("cxx23:basic_spanbuf::setbuf");
}
#endif

TEST_CASE(spanstream, feature_test_macros) {
#if defined(__cpp_lib_spanstream) && (__cpp_lib_spanstream) >= 202106L
    test_true( (__cpp_lib_spanstream) >= 202106L );
    test_pass("cxx23:__cpp_lib_spanstream=202106L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_spanstream=202106L");
#endif
}
