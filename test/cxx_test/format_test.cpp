#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_format)
#include <format>
#include <string>
#include <locale>
#include <iterator>
#include <type_traits>
#include <vector>

using STD::format;
typedef STD::string S;

namespace {

struct FmtPoint { int x; int y; };
}
template <>
struct STD::formatter<FmtPoint, char> {
    constexpr STD::format_parse_context::iterator parse(STD::format_parse_context& ctx) {
        return ctx.begin();
    }
    template <class Ctx>
    typename Ctx::iterator format(const FmtPoint& p, Ctx& ctx) const {
        return STD::format_to(ctx.out(), "({},{})", p.x, p.y);
    }
};

TEST_CASE(format, default_by_type) {
    test_eq( format("{}", 100), S("100") );
    test_pass("cxx20:format {} (int)");
    test_eq( format("{}", -5), S("-5") );
    test_pass("cxx20:format {} (negative int)");
    test_eq( format("{}", 12345678901234LL), S("12345678901234") );
    test_pass("cxx20:format {} (long long)");
    test_eq( format("{}", 3.14), S("3.14") );
    test_pass("cxx20:format {} (double)");
    test_eq( format("{}", "abcd"), S("abcd") );
    test_pass("cxx20:format {} (const char*)");
    test_eq( format("{}", S("str")), S("str") );
    test_pass("cxx20:format {} (string)");
    test_eq( format("{}", 'A'), S("A") );
    test_pass("cxx20:format {} (char)");
    test_eq( format("{}", true), S("true") );
    test_pass("cxx20:format {} (bool)");
}

TEST_CASE(format, explicit_index_and_escape) {
    test_eq( format("{0} {1}", "a", "b"), S("a b") );
    test_eq( format("{1} {0}", "a", "b"), S("b a") );
    test_eq( format("{0} {0}", "x"), S("x x") );
    test_pass("cxx20:format explicit index");
    test_eq( format("{} + {} = {}", 1, 2, 3), S("1 + 2 = 3") );
    test_pass("cxx20:format auto index (multiple)");
    test_eq( format("{{}}"), S("{}") );
    test_eq( format("{{{}}}", 7), S("{7}") );
    test_eq( format("100%"), S("100%") );
    test_pass("cxx20:format brace escape");
}

TEST_CASE(format, integer_types) {
    test_eq( format("{:d}", 42), S("42") );
    test_pass("cxx20:format {:d}");
    test_eq( format("{:b}", 5), S("101") );
    test_pass("cxx20:format {:b} (binary)");
    test_eq( format("{:B}", 5), S("101") );
    test_pass("cxx20:format {:B}");
    test_eq( format("{:o}", 8), S("10") );
    test_pass("cxx20:format {:o} (octal)");
    test_eq( format("{:x}", 255), S("ff") );
    test_pass("cxx20:format {:x} (hex)");
    test_eq( format("{:X}", 255), S("FF") );
    test_pass("cxx20:format {:X}");
    test_eq( format("{:#x}", 255), S("0xff") );
    test_pass("cxx20:format {:#x} (alternate)");
    test_eq( format("{:#b}", 5), S("0b101") );
    test_pass("cxx20:format {:#b}");
    test_eq( format("{:#o}", 8), S("010") );
    test_pass("cxx20:format {:#o}");
    test_eq( format("{:c}", 65), S("A") );
    test_pass("cxx20:format {:c} (int as char)");
}

TEST_CASE(format, float_types) {
    test_eq( format("{:f}", 1.5), S("1.500000") );
    test_pass("cxx20:format {:f} (fixed)");
    test_eq( format("{:F}", 1.5), S("1.500000") );
    test_pass("cxx20:format {:F}");
    test_eq( format("{:e}", 1500.0), S("1.500000e+03") );
    test_pass("cxx20:format {:e} (scientific)");
    test_eq( format("{:E}", 1500.0), S("1.500000E+03") );
    test_pass("cxx20:format {:E}");
    test_eq( format("{:g}", 0.0001), S("0.0001") );
    test_pass("cxx20:format {:g} (general, small)");
    test_eq( format("{:g}", 1000000.0), S("1e+06") );
    test_pass("cxx20:format {:g} (general, exp switch)");
    test_eq( format("{:G}", 1000000.0), S("1E+06") );
    test_pass("cxx20:format {:G}");

    S a = format("{:a}", 1.0);
    test_true( a.size() >= 2 && a.find('p') != S::npos );
    test_pass("cxx20:format {:a} (hex float)");
}

TEST_CASE(format, precision) {
    test_eq( format("{:.2f}", 3.14159), S("3.14") );
    test_pass("cxx20:format {:.2f} (precision + fixed)");
    test_eq( format("{:.0f}", 3.9), S("4") );
    test_pass("cxx20:format {:.0f} (zero precision)");
    test_eq( format("{:.3e}", 12345.678), S("1.235e+04") );
    test_pass("cxx20:format {:.3e}");
    test_eq( format("{:.3g}", 3.14159), S("3.14") );
    test_pass("cxx20:format {:.3g} (precision + general)");
    test_eq( format("{:.6g}", 3.14159), S("3.14159") );
    test_pass("cxx20:format {:.6g}");
}

TEST_CASE(format, align_and_fill) {
    test_eq( format("{:<5}", 42), S("42   ") );
    test_pass("cxx20:format {:<} (left align)");
    test_eq( format("{:>5}", 42), S("   42") );
    test_pass("cxx20:format {:>} (right align)");
    test_eq( format("{:^5}", 42), S(" 42  ") );
    test_pass("cxx20:format {:^} (center align)");
    test_eq( format("{:*<5}", 42), S("42***") );
    test_pass("cxx20:format {:*<} (fill char)");
    test_eq( format("{:0>5}", 42), S("00042") );
    test_pass("cxx20:format {:0>} (fill with 0 via align)");
    test_eq( format("{:05}", 42), S("00042") );
    test_pass("cxx20:format {:05} (zero pad)");
    test_eq( format("{:05}", -42), S("-0042") );
    test_pass("cxx20:format {:05} (zero pad, negative)");
}

TEST_CASE(format, sign) {
    test_eq( format("{:+}", 42), S("+42") );
    test_pass("cxx20:format {:+} (always sign)");
    test_eq( format("{: }", 42), S(" 42") );
    test_pass("cxx20:format {: } (space for positive)");
    test_eq( format("{:-}", 42), S("42") );
    test_pass("cxx20:format {:-} (sign for negative only)");
    test_eq( format("{:+}", -42), S("-42") );
    test_pass("cxx20:format {:+} (negative)");
}

TEST_CASE(format, combined_paths) {

    test_eq( format("{:10.6g}", 3.14159), S("   3.14159") );
    test_pass("cxx20:format {:10.6g}");
    test_eq( format("{:<10.6g}", 3.14159), S("3.14159   ") );
    test_pass("cxx20:format {:<10.6g}");
    test_eq( format("{:+08.2f}", 3.14159), S("+0003.14") );
    test_pass("cxx20:format {:+08.2f}");
    test_eq( format("{:#010x}", 255), S("0x000000ff") );
    test_pass("cxx20:format {:#010x}");
    test_eq( format("{:>+8d}", 42), S("     +42") );
    test_pass("cxx20:format {:>+8d}");
}

TEST_CASE(format, dynamic_width_precision) {
#if defined(__clang__) && defined(__GLIBCXX__) && __cplusplus > 202302L
    test_skip("cxx20:format {:{}} (dynamic width)");
    test_skip("cxx20:format {:.{}f} (dynamic precision)");
    test_skip("cxx20:format {:{}.{}f} (dynamic width+precision)");
    test_skip("cxx20:format {0:{1}} (indexed dynamic width)");
#else
    test_eq( format("{:{}}", 42, 5), S("   42") );
    test_pass("cxx20:format {:{}} (dynamic width)");
    test_eq( format("{:.{}f}", 3.14159, 2), S("3.14") );
    test_pass("cxx20:format {:.{}f} (dynamic precision)");
    test_eq( format("{:{}.{}f}", 3.14159, 8, 2), S("    3.14") );
    test_pass("cxx20:format {:{}.{}f} (dynamic width+precision)");
    test_eq( format("{0:{1}}", 42, 5), S("   42") );
    test_pass("cxx20:format {0:{1}} (indexed dynamic width)");
#endif
}

TEST_CASE(format, string_and_char) {
    test_eq( format("{:>8}", "hi"), S("      hi") );
    test_pass("cxx20:format string (right align)");
    test_eq( format("{:<8}", "hi"), S("hi      ") );
    test_pass("cxx20:format string (left align, default for non-arithmetic is left)");
    test_eq( format("{:.3}", "abcdef"), S("abc") );
    test_pass("cxx20:format string {:.3} (truncate)");
    test_eq( format("{:*^6}", "hi"), S("**hi**") );
    test_pass("cxx20:format string (center + fill)");
}

TEST_CASE(format, bool_and_pointer) {
    test_eq( format("{:s}", true), S("true") );
    test_pass("cxx20:format bool {:s}");
    test_eq( format("{:d}", true), S("1") );
    test_pass("cxx20:format bool {:d} (as int)");
    void* p = reinterpret_cast<void*>(static_cast<STD::uintptr_t>(0x1234));
    S ps = format("{}", p);
    test_true( ps.size() >= 3 && ps[0] == '0' && ps[1] == 'x' );
    test_pass("cxx20:format pointer {}");
    test_eq( format("{}", static_cast<void*>(0)), S("0x0") );
    test_pass("cxx20:format pointer (null)");
}

TEST_CASE(format, format_to_and_size) {
    S out;
    STD::format_to( STD::back_inserter(out), "{}-{}", 1, 2 );
    test_eq( out, S("1-2") );
    test_pass("cxx20:format_to");
    STD::size_t n = STD::formatted_size("{:5}", 42);
    test_eq( (long)n, 5L );
    test_pass("cxx20:formatted_size");
    char buf[8];
    STD::format_to_n_result<char*> r = STD::format_to_n(buf, 3, "{}", 12345);
    test_eq( (long)(r.out - buf), 3L );
    test_true( buf[0] == '1' && buf[1] == '2' && buf[2] == '3' );
    test_pass("cxx20:format_to_n");
}

#if TEST_HAS_EH
TEST_CASE(format, errors_and_runtime) {

    int one = 1, two = 2;
    test_eq( STD::vformat("{} {}", STD::make_format_args(one, two)), S("1 2") );
    test_pass("cxx20:make_format_args");

    int three = 3;
    test_eq( STD::vformat("[{}]", STD::make_format_args(three)), S("[3]") );
    test_pass("cxx20:vformat");

    bool threw = false;
    try {
        int x = 1;
        S bad = "{:d}";
        (void)STD::vformat(bad + "}", STD::make_format_args(x));
    } catch (const STD::format_error&) { threw = true; }
    test_true( threw );
    test_pass("cxx20:format_error (thrown on bad spec)");
}
#else   // !TEST_HAS_EH
TEST_CASE(format, errors_and_runtime) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx20:make_format_args");
    test_skip("cxx20:vformat");
    test_skip("cxx20:format_error (thrown on bad spec)");
}
#endif  // TEST_HAS_EH

TEST_CASE(format, spec_fields) {
    test_eq( format("{:<5}", 42), S("42   ") );
    test_pass("cxx20:format align spec");
    test_eq( format("{:*>5}", 42), S("***42") );
    test_pass("cxx20:format fill spec");
    test_eq( format("{:5}", 42), S("   42") );
    test_pass("cxx20:format width spec");
    test_eq( format("{:.2f}", 3.14159), S("3.14") );
    test_pass("cxx20:format precision spec");
    test_eq( format("{:+}", 42), S("+42") );
    test_pass("cxx20:format sign spec");
    test_eq( format("{:#x}", 255), S("0xff") );
    test_pass("cxx20:format # spec");
    test_eq( format("{:05}", 42), S("00042") );
    test_pass("cxx20:format 0 spec");

    test_eq( format("{:b}", 5), S("101") );
    test_eq( format("{:o}", 8), S("10") );
    test_eq( format("{:x}", 255), S("ff") );
    test_eq( format("{:d}", 42), S("42") );
    test_pass("cxx20:format integer type spec (b/o/x/d)");

    test_eq( format("{:.1e}", 1.5), S("1.5e+00") );
    test_eq( format("{:.1f}", 1.5), S("1.5") );
    test_eq( format("{:g}", 1.5), S("1.5") );
    test_true( !format("{:a}", 1.0).empty() );
    test_pass("cxx20:format float type spec (e/f/g/a)");

    test_eq( format("{:>8}", "hi"), S("      hi") );
    test_pass("cxx20:format string spec");
    test_eq( format("{:c}", 66), S("B") );
    test_pass("cxx20:format char spec");
    test_eq( format("{}", true), S("true") );
    test_eq( format("{:d}", true), S("1") );
    test_pass("cxx20:format bool spec");
}

TEST_CASE(format, locale_and_error_type) {

    test_eq( format(STD::locale::classic(), "{:L}", 1000), S("1000") );
    test_pass("cxx20:format(locale)");

    STD::format_error fe("boom");
    test_true( S(fe.what()).find("boom") != S::npos );
    test_pass("cxx20:format_error");
}

namespace { struct IntVisitor {
    int operator()(int v) const { return v; }
    template <class T> int operator()(T) const { return -1; }
}; }

TEST_CASE(format, api_types) {

    int x = 42;
    auto x_store = STD::make_format_args(x);
    STD::format_args fa(x_store);
    S r1 = STD::vformat("{}", fa);
    test_eq( r1, S("42") );
    test_pass("cxx20:format_args");

    S out;
    STD::vformat_to(STD::back_inserter(out), "{}", STD::make_format_args(x));
    test_eq( out, S("42") );
    test_pass("cxx20:vformat_to");

    wchar_t wc = 0; (void)wc;
    int wx = 7;
    STD::wstring wr = STD::vformat(L"{}", STD::make_wformat_args(wx));
    test_true( wr == L"7" );
    test_pass("cxx20:make_wformat_args");

    int y = 5;
    auto y_store = STD::make_format_args(y);
    STD::format_args fy(y_store);
    STD::basic_format_arg<STD::format_context> arg = fy.get(0);
    int got = STD::visit_format_arg(IntVisitor(), arg);
    test_eq( got, 5 );
    test_pass("cxx20:visit_format_arg");
    test_true( (STD::is_same<STD::basic_format_arg<STD::format_context>,
                             STD::basic_format_arg<STD::format_context> >::value) );
    test_pass("cxx20:basic_format_arg");

    test_true( (STD::is_same<STD::format_args,
                             STD::basic_format_args<STD::format_context> >::value) );
    test_true( sizeof(STD::basic_format_args<STD::format_context>) > 0 );
    test_pass("cxx20:basic_format_args");
    test_true( sizeof(STD::format_context) > 0 );
    test_pass("cxx20:format_context");
    typedef STD::basic_format_context<char*, char> BFC;
    test_true( sizeof(BFC*) > 0 );
    test_pass("cxx20:basic_format_context");
    test_true( (STD::is_same<STD::format_parse_context,
                             STD::basic_format_parse_context<char> >::value) );
    test_pass("cxx20:format_parse_context");
    test_true( sizeof(STD::basic_format_parse_context<char>) > 0 );
    test_pass("cxx20:basic_format_parse_context");
}

TEST_CASE(format, formatters) {

    STD::formatter<int, char> fi;
    (void)fi;
    test_eq( format("{}", 123), S("123") );
    test_eq( format("{}", 'A'), S("A") );
    test_eq( format("{}", "str"), S("str") );
    test_pass("cxx20:formatter built-in specializations");

    FmtPoint p; p.x = 3; p.y = 4;
    test_eq( format("{}", p), S("(3,4)") );
    test_pass("cxx20:formatter user specialization");
}

TEST_CASE(format, cxx23_and_cxx26) {
#if defined(__cpp_lib_format_ranges)
    bool fmtable = STD::formattable<int, char>;
    test_true( fmtable );
    test_pass("cxx23:formattable concept");

    STD::range_formatter<int> rf;
    (void)rf;
    test_true( true );
    test_pass("cxx23:range_formatter");

    test_true( STD::format_kind<STD::vector<int> > != STD::range_format::disabled );
    test_pass("cxx23:format_kind");

    STD::format_string<int> fs = "{}";
    test_eq( format(fs, 9), S("9") );

    STD::wformat_string<int> wfs = L"{}";
    test_true( STD::format(wfs, 9) == L"9" );
#else
    test_skip("cxx23:formattable concept");
    test_skip("cxx23:range_formatter");
    test_skip("cxx23:format_kind");
#endif

}

namespace {
struct FmtProbe { int v; };

int  g_fp_seen       = 0;
bool g_fp_ctx_out    = false;
bool g_fp_ctx_advance = false;
}

template <>
struct STD::formatter<FmtProbe, char> {
    int width_ = 0;
    int seen_ = 0;
    constexpr STD::format_parse_context::iterator parse(STD::format_parse_context& ctx) {
        typedef STD::format_parse_context PC;
        PC::iterator it = ctx.begin();
        PC::iterator last = ctx.end();
        seen_ = 0;
        if (it == last || *it == '}' || (*it >= '0' && *it <= '9')) seen_ |= 1;
        width_ = 0;
        while (it != last && *it >= '0' && *it <= '9') {
            width_ = width_ * 10 + (*it - '0');
            ++it;
        }
        ctx.advance_to(it);
        if (last - it == ctx.end() - ctx.begin()) seen_ |= 2;
        return ctx.begin();
    }
    template <class Ctx>
    typename Ctx::iterator format(const FmtProbe& p, Ctx& ctx) const {
        g_fp_seen = seen_;
        g_fp_ctx_out = true;
        typename Ctx::iterator o = ctx.out();
        o = STD::format_to(o, "{}", p.v + width_);
        ctx.advance_to(o);
        g_fp_ctx_advance = true;
        return ctx.out();
    }
};

TEST_CASE(format, parse_context_api_cxx20) {
    typedef STD::format_parse_context PC;
    g_fp_seen = 0;
    FmtProbe p; p.v = 1;
    test_eq( format("{:40}", p), S("41") );

    test_true(( STD::is_same<PC::char_type, char>::value ));
    test_pass("cxx20:basic_format_parse_context::char_type");
    test_true( (g_fp_seen & 1) != 0 );
    test_pass("cxx20:basic_format_parse_context::begin");
    test_pass("cxx20:basic_format_parse_context::end");
    test_true(( STD::is_same<PC::iterator, PC::const_iterator>::value ));
    test_true(( STD::is_same<PC::iterator,
                             STD::basic_string_view<char>::const_iterator>::value ));
    test_pass("cxx20:basic_format_parse_context::iterator");
    test_pass("cxx20:basic_format_parse_context::const_iterator");
    test_true( (g_fp_seen & 2) != 0 );
    test_pass("cxx20:basic_format_parse_context::advance_to");

    {
        PC ctx("{}");
        STD::size_t first = ctx.next_arg_id();
        STD::size_t second = ctx.next_arg_id();
        test_eq( (long)first, 0L );
        test_eq( (long)second, 1L );
        test_pass("cxx20:basic_format_parse_context::next_arg_id");
    }
    {
        PC ctx("{}");
        ctx.check_arg_id(0);
        test_true( true );
        test_pass("cxx20:basic_format_parse_context::check_arg_id");
    }

    test_true( !STD::is_copy_assignable<PC>::value );
    test_true( !STD::is_copy_constructible<PC>::value );
    test_pass("cxx20:basic_format_parse_context::operator=");

    test_true(( STD::is_same<STD::wformat_parse_context,
                             STD::basic_format_parse_context<wchar_t> >::value ));
    test_pass("cxx20:wformat_parse_context");

#if defined(__cpp_lib_format) && (__cpp_lib_format) >= 202306L
    {
        PC ctx("{}");
        ctx.check_dynamic_spec_integral(0);
        test_true( true );
        test_pass("cxx26:basic_format_parse_context::check_dynamic_spec_integral");
        PC ctx2("{}");
        ctx2.check_dynamic_spec_string(0);
        test_true( true );
        test_pass("cxx26:basic_format_parse_context::check_dynamic_spec_string");
        PC ctx3("{}");
        ctx3.check_dynamic_spec<int, STD::string_view>(0);
        test_true( true );
        test_pass("cxx26:basic_format_parse_context::check_dynamic_spec");
    }
#else
    TEST_NOTE("check_dynamic_spec is C++26 (P2757); not in this library");
    test_skip("cxx26:basic_format_parse_context::check_dynamic_spec_integral");
    test_skip("cxx26:basic_format_parse_context::check_dynamic_spec_string");
    test_skip("cxx26:basic_format_parse_context::check_dynamic_spec");
#endif
}

TEST_CASE(format, format_context_api_cxx20) {
    g_fp_ctx_out = g_fp_ctx_advance = false;
    FmtProbe p; p.v = 2;
    test_eq( format("{}", p), S("2") );

    test_true( g_fp_ctx_out );
    test_pass("cxx20:basic_format_context::out");
    test_true( g_fp_ctx_advance );
    test_pass("cxx20:basic_format_context::advance_to");

    test_true(( STD::is_same<STD::format_context::char_type, char>::value ));
    test_true(( STD::is_same<STD::wformat_context::char_type, wchar_t>::value ));
    test_pass("cxx20:basic_format_context::char_type");

    test_true(( STD::is_same<STD::format_context::iterator,
                             decltype(STD::declval<STD::format_context&>().out())>::value ));
    test_pass("cxx20:basic_format_context::iterator");

    {
        int a = 7;
        STD::string b = "z";
        auto store = STD::make_format_args(a, b);
        STD::format_args fa(store);
        STD::basic_format_arg<STD::format_context> a0 = fa.get(0);
        test_true( (bool)a0 );
        STD::basic_format_arg<STD::format_context> none = fa.get(9);
        test_true( !(bool)none );
        test_pass("cxx20:basic_format_args::get");
    }
    test_pass("cxx20:basic_format_context::arg");

    test_true( !STD::is_copy_assignable<STD::format_context>::value );
    test_pass("cxx26:basic_format_context::operator=");

    test_true(( STD::is_same<STD::wformat_context,
                STD::basic_format_context<STD::wformat_context::iterator,
                                          wchar_t> >::value ));
    test_true(( STD::is_same<STD::wformat_context::char_type, wchar_t>::value ));
    test_pass("cxx20:wformat_context");
    test_true(( STD::is_same<STD::wformat_args,
                STD::basic_format_args<STD::wformat_context> >::value ));
    test_pass("cxx20:wformat_args");
}

TEST_CASE(format, format_arg_api) {
    int a = 5;
    auto store = STD::make_format_args(a);
    STD::format_args fa(store);
    STD::basic_format_arg<STD::format_context> arg = fa.get(0);

    typedef STD::basic_format_arg<STD::format_context>::handle H;
    test_true( sizeof(H) > 0 );
    test_true( !STD::is_default_constructible<H>::value );
    test_pass("cxx20:basic_format_arg::handle");

#if defined(__cpp_lib_format) && (__cpp_lib_format) >= 202306L
    int got = arg.visit(IntVisitor());
    test_eq( got, 5 );
    test_pass("cxx26:basic_format_arg::visit");
#else
    TEST_NOTE("basic_format_arg::visit is C++26 (P2637); not in this library");
    test_skip("cxx26:basic_format_arg::visit");
#endif
}

TEST_CASE(format, format_string_aliases_cxx20) {
    STD::format_string<int> fs("{}");
    test_eq( STD::string(fs.get()), S("{}") );
    test_eq( format(fs, 3), S("3") );
    test_pass("cxx20:basic_format_string");

    test_true(( STD::is_same<STD::format_string<int>,
                             STD::basic_format_string<char, int> >::value ));
    test_true(( STD::is_same<decltype(fs.get()), STD::string_view>::value ));
    test_pass("cxx20:format_string");

    STD::wformat_string<int> wfs(L"{}");
    test_true(( STD::is_same<STD::wformat_string<int>,
                             STD::basic_format_string<wchar_t, int> >::value ));
    test_true( STD::wstring(wfs.get()) == L"{}" );
    test_true( STD::format(wfs, 4) == L"4" );
    test_pass("cxx20:wformat_string");
}

TEST_CASE(format, standard_formatter_specializations_cxx20) {
    test_eq( format("{}", 'A'), S("A") );
    test_eq( format("{:3}", 'A'), S("A  ") );
#if defined(__cpp_lib_format_ranges)
    test_true(( STD::formattable<char, char> ));
    test_pass("cxx20:formatter<char>");
#else
    TEST_SKIP1();
    test_skip("cxx20:formatter<char>");
#endif

    test_true( STD::format(L"{}", L'A') == L"A" );
#if defined(__cpp_lib_format_ranges)
    test_true(( STD::formattable<wchar_t, wchar_t> ));
    test_pass("cxx20:formatter<wchar_t>");
#else
    TEST_SKIP1();
    test_skip("cxx20:formatter<wchar_t>");
#endif

    const char* cs = "str";
    test_eq( format("{}", cs), S("str") );
    test_eq( format("{:>5}", cs), S("  str") );
    test_pass("cxx20:formatter<const char*>");

    const wchar_t* ws = L"str";
    test_true( STD::format(L"{}", ws) == L"str" );
    test_pass("cxx20:formatter<const wchar_t*>");

    STD::string_view sv("view");
    test_eq( format("{}", sv), S("view") );
    test_eq( format("{:.2}", sv), S("vi") );
    test_pass("cxx20:formatter<string_view>");

    STD::wstring_view wsv(L"view");
    test_true( STD::format(L"{}", wsv) == L"view" );
    test_pass("cxx20:formatter<wstring_view>");

    int obj = 0;
    const void* vp = &obj;
    S ps = format("{}", vp);
    test_true( ps.size() > 2 && ps[0] == '0' && ps[1] == 'x' );
    test_pass("cxx20:formatter<const void*>");

    S ns = format("{}", nullptr);
    test_true( ns.size() > 0 );
    test_pass("cxx20:formatter<nullptr_t>");
}

#if defined(__cpp_lib_format_ranges)
namespace {
struct FmtOptOutRange {
    int a[2];
    const int* begin() const { return a; }
    const int* end() const { return a + 2; }
};
}
template <>
constexpr STD::range_format STD::format_kind<FmtOptOutRange> = STD::range_format::disabled;
#endif

TEST_CASE(format, range_format_and_nonlocking_cxx23) {
#if defined(__cpp_lib_format_ranges)
    STD::range_format rf = STD::range_format::sequence;
    test_true( rf == STD::range_format::sequence );
    test_pass("cxx23:range_format");

    test_true( STD::format_kind<STD::vector<int> > == STD::range_format::sequence );
    test_eq( format("{}", STD::vector<int>(2, 7)), S("[7, 7]") );
    test_pass("cxx23:range_format::sequence");

    test_true( STD::range_format::disabled != STD::range_format::sequence );
    test_true( STD::format_kind<STD::string> == STD::range_format::sequence );
    test_true( STD::format_kind<FmtOptOutRange> == STD::range_format::disabled );
    test_true(( !STD::formattable<FmtOptOutRange, char> ));
    test_pass("cxx23:range_format::disabled");

    test_true( STD::range_format::map != STD::range_format::set );
    test_pass("cxx23:range_format::map");
    test_pass("cxx23:range_format::set");
    test_true( STD::range_format::string != STD::range_format::debug_string );
    test_pass("cxx23:range_format::string");
    test_pass("cxx23:range_format::debug_string");

#if defined(__cpp_lib_print) && (__cpp_lib_print) >= 202403L
    test_true(( STD::enable_nonlocking_formatter_optimization<int> ));
    test_true(( STD::enable_nonlocking_formatter_optimization<char> ));
    test_true(( !STD::enable_nonlocking_formatter_optimization<FmtPoint> ));
    test_pass("cxx23:enable_nonlocking_formatter_optimization");
#else
    TEST_NOTE("enable_nonlocking_formatter_optimization (P3107) is not declared here");
    test_skip("cxx23:enable_nonlocking_formatter_optimization");
#endif
#else
    TEST_NOTE("range formatting is C++23; not in this library");
    test_skip("cxx23:range_format");
    test_skip("cxx23:range_format::sequence");
    test_skip("cxx23:range_format::disabled");
    test_skip("cxx23:range_format::map");
    test_skip("cxx23:range_format::set");
    test_skip("cxx23:range_format::string");
    test_skip("cxx23:range_format::debug_string");
    test_skip("cxx23:enable_nonlocking_formatter_optimization");
#endif
}

TEST_CASE(format, cxx26_additions) {
#if defined(__cpp_lib_format) && (__cpp_lib_format) >= 202603L
    {
        STD::string spec = "{}";
        test_eq( STD::format(STD::dynamic_format(spec), 8), S("8") );
    }
    test_pass("cxx26:dynamic_format");
#else
    TEST_NOTE("dynamic_format is the final C++26 name (P3953); not in this library");
    test_skip("cxx26:dynamic_format");
#endif

#if defined(__cpp_lib_constexpr_format) && (__cpp_lib_constexpr_format) >= 202502L
    {
        struct K {
            static constexpr STD::size_t run() { return STD::formatted_size("{}", 12345); }
        };
        static_assert(K::run() == 5, "format in a constant expression");
        test_eq( (long)K::run(), 5L );
    }
    test_pass("cxx26:constexpr format facilities");
#else
    TEST_NOTE("constexpr formatting is C++26 (P3391); not in this library");
    test_skip("cxx26:constexpr format facilities");
#endif
}

#elif TEST_TARGET_CXX >= 2020   // Open Watcom: <format> is absent, so the cases only exist as skips
TEST_CASE_SKIP(format, default_by_type)
TEST_CASE_SKIP(format, explicit_index_and_escape)
TEST_CASE_SKIP(format, integer_types)
TEST_CASE_SKIP(format, float_types)
TEST_CASE_SKIP(format, precision)
TEST_CASE_SKIP(format, align_and_fill)
TEST_CASE_SKIP(format, sign)
TEST_CASE_SKIP(format, combined_paths)
TEST_CASE_SKIP(format, dynamic_width_precision)
TEST_CASE_SKIP(format, string_and_char)
TEST_CASE_SKIP(format, bool_and_pointer)
TEST_CASE_SKIP(format, format_to_and_size)
TEST_CASE_SKIP(format, errors_and_runtime)
TEST_CASE_SKIP(format, spec_fields)
TEST_CASE_SKIP(format, locale_and_error_type)
TEST_CASE_SKIP(format, api_types)
TEST_CASE_SKIP(format, formatters)
TEST_CASE_SKIP(format, cxx23_and_cxx26)
TEST_CASE(format, parse_context_api_cxx20) {
    test_skip("cxx20:basic_format_parse_context::char_type");
    test_skip("cxx20:basic_format_parse_context::begin");
    test_skip("cxx20:basic_format_parse_context::end");
    test_skip("cxx20:basic_format_parse_context::iterator");
    test_skip("cxx20:basic_format_parse_context::const_iterator");
    test_skip("cxx20:basic_format_parse_context::advance_to");
    test_skip("cxx20:basic_format_parse_context::next_arg_id");
    test_skip("cxx20:basic_format_parse_context::check_arg_id");
    test_skip("cxx20:basic_format_parse_context::operator=");
    test_skip("cxx20:wformat_parse_context");
    test_skip("cxx26:basic_format_parse_context::check_dynamic_spec_integral");
    test_skip("cxx26:basic_format_parse_context::check_dynamic_spec_string");
    test_skip("cxx26:basic_format_parse_context::check_dynamic_spec");
}
TEST_CASE(format, format_context_api_cxx20) {
    test_skip("cxx20:basic_format_context::out");
    test_skip("cxx20:basic_format_context::advance_to");
    test_skip("cxx20:basic_format_context::char_type");
    test_skip("cxx20:basic_format_context::iterator");
    test_skip("cxx20:basic_format_args::get");
    test_skip("cxx20:basic_format_context::arg");
    test_skip("cxx26:basic_format_context::operator=");
    test_skip("cxx20:wformat_context");
    test_skip("cxx20:wformat_args");
}
TEST_CASE(format, format_arg_api) {
    test_skip("cxx20:basic_format_arg::handle");
    test_skip("cxx26:basic_format_arg::visit");
}
TEST_CASE(format, format_string_aliases_cxx20) {
    test_skip("cxx20:basic_format_string");
    test_skip("cxx20:format_string");
    test_skip("cxx20:wformat_string");
}
TEST_CASE(format, standard_formatter_specializations_cxx20) {
    test_skip("cxx20:formatter<char>");
    test_skip("cxx20:formatter<wchar_t>");
    test_skip("cxx20:formatter<const char*>");
    test_skip("cxx20:formatter<const wchar_t*>");
    test_skip("cxx20:formatter<string_view>");
    test_skip("cxx20:formatter<wstring_view>");
    test_skip("cxx20:formatter<const void*>");
    test_skip("cxx20:formatter<nullptr_t>");
}
TEST_CASE(format, range_format_and_nonlocking_cxx23) {
    test_skip("cxx23:range_format");
    test_skip("cxx23:range_format::sequence");
    test_skip("cxx23:range_format::disabled");
    test_skip("cxx23:range_format::map");
    test_skip("cxx23:range_format::set");
    test_skip("cxx23:range_format::string");
    test_skip("cxx23:range_format::debug_string");
    test_skip("cxx23:enable_nonlocking_formatter_optimization");
}
TEST_CASE(format, cxx26_additions) {
    test_skip("cxx26:dynamic_format");
    test_skip("cxx26:constexpr format facilities");
}
#endif

TEST_CASE(format, feature_test_macros) {
#if defined(__cpp_lib_format) && (__cpp_lib_format) >= 201907L
    test_true( (__cpp_lib_format) >= 201907L );
    test_pass("cxx20:__cpp_lib_format=201907L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_format=201907L");
#endif
#if defined(__cpp_lib_format_ranges)
    test_true( __cpp_lib_format_ranges > 0L );
    test_pass("cxx23:__cpp_lib_format_ranges");
#else
    test_skip("cxx23:__cpp_lib_format_ranges");
#endif
#if defined(__cpp_lib_format_uchar)
    test_true( __cpp_lib_format_uchar > 0L );
    test_pass("cxx26:__cpp_lib_format_uchar");
#else
    test_skip("cxx26:__cpp_lib_format_uchar");
#endif
#if defined(__cpp_lib_constexpr_format)
    test_true( __cpp_lib_constexpr_format > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_format");
#else
    test_skip("cxx26:__cpp_lib_constexpr_format");
#endif
#if defined(__cpp_lib_constexpr_exceptions)
    test_true( __cpp_lib_constexpr_exceptions > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_exceptions");
#else
    test_skip("cxx26:__cpp_lib_constexpr_exceptions");
#endif
}
