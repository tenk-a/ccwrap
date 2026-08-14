#include "test_cxx.hpp"

#if _TST_TEST_HAS(<version>)
#  include <version>
#endif

#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_ranges)

#include <ranges>
#include <vector>
#include <list>
#include <forward_list>
#include <string>
#include <sstream>
#include <utility>
#include <type_traits>
#include <limits>

namespace {
    struct RgBorrowed {
        int* b_;
        int* e_;
        int* begin() const { return b_; }
        int* end() const { return e_; }
    };
}
template <>
inline constexpr bool STD::ranges::enable_borrowed_range<RgBorrowed> = true;

namespace {
    struct RgSpanView : STD::ranges::view_interface<RgSpanView> {
        int* b_;
        int* e_;
        RgSpanView() : b_(0), e_(0) {}
        RgSpanView(int* b, int* e) : b_(b), e_(e) {}
        int* begin() const { return b_; }
        int* end() const { return e_; }
    };

    bool rg_is_odd(int x) { return x % 2 != 0; }
    int  rg_twice(int x) { return x * 2; }

#if defined(__cpp_lib_ranges_as_rvalue)
    struct RgAddOne : STD::ranges::range_adaptor_closure<RgAddOne> {
        template <class R>
        auto operator()(R&& r) const {
            return STD::views::transform(STD::forward<R>(r), [](int x) { return x + 1; });
        }
    };
#endif
}

namespace rg = STD::ranges;
namespace vw = STD::views;

TEST_CASE(ranges, concepts_cxx20) {
    typedef STD::vector<int> Vec;
    typedef STD::list<int> Lst;
    typedef STD::forward_list<int> Fwd;

    test_true( rg::range<Vec> );
    test_true( !rg::range<int> );
    test_pass("cxx20:ranges::range");

    test_true( rg::input_range<Vec> );
    test_true( !rg::input_range<int> );
    test_pass("cxx20:ranges::input_range");

    test_true( (rg::output_range<Vec, int>) );
    test_true( !(rg::output_range<const Vec&, int>) );
    test_pass("cxx20:ranges::output_range");

    test_true( rg::forward_range<Fwd> );
    test_true(( !rg::forward_range<rg::basic_istream_view<int, char> > ));
    test_pass("cxx20:ranges::forward_range");

    test_true( rg::bidirectional_range<Lst> );
    test_true( !rg::bidirectional_range<Fwd> );
    test_pass("cxx20:ranges::bidirectional_range");

    test_true( rg::random_access_range<Vec> );
    test_true( !rg::random_access_range<Lst> );
    test_pass("cxx20:ranges::random_access_range");

    test_true( rg::contiguous_range<Vec> );
    test_true( !rg::contiguous_range<Lst> );
    test_pass("cxx20:ranges::contiguous_range");

    test_true( rg::sized_range<Vec> );
    test_true( !rg::sized_range<Fwd> );
    test_pass("cxx20:ranges::sized_range");

    test_true( rg::common_range<Vec> );
    test_true(( !rg::common_range<rg::basic_istream_view<int, char> > ));
    test_pass("cxx20:ranges::common_range");

    test_true( rg::borrowed_range<RgBorrowed> );
    test_true( !rg::borrowed_range<Vec> );
    test_true( rg::borrowed_range<Vec&> );
    test_pass("cxx20:ranges::borrowed_range");

    test_true( rg::view<rg::empty_view<int> > );
    test_true( !rg::view<Vec> );
    test_pass("cxx20:ranges::view");

    test_true( rg::viewable_range<Vec&> );
    test_true( !rg::viewable_range<int> );
    test_pass("cxx20:ranges::viewable_range");
}

TEST_CASE(ranges, type_aliases_cxx20) {
    typedef STD::vector<int> Vec;

    test_true(( STD::is_same<rg::iterator_t<Vec>, Vec::iterator>::value ));
    test_true(( STD::is_same<rg::iterator_t<const Vec>, Vec::const_iterator>::value ));
    test_pass("cxx20:ranges::iterator_t");

    test_true(( STD::is_same<rg::sentinel_t<Vec>, Vec::iterator>::value ));
    test_true( (rg::common_range<Vec>) ==
               (STD::is_same<rg::sentinel_t<Vec>, rg::iterator_t<Vec> >::value) );
    test_pass("cxx20:ranges::sentinel_t");

    test_true(( STD::is_same<rg::range_value_t<Vec>, int>::value ));
    test_true(( STD::is_same<rg::range_value_t<STD::vector<STD::string> >, STD::string>::value ));
    test_pass("cxx20:ranges::range_value_t");

    test_true(( STD::is_same<rg::range_reference_t<Vec>, int&>::value ));
    test_true(( STD::is_same<rg::range_reference_t<const Vec>, const int&>::value ));
    test_pass("cxx20:ranges::range_reference_t");

    test_true(( STD::is_same<rg::range_rvalue_reference_t<Vec>, int&&>::value ));
    test_true(( STD::is_same<rg::range_rvalue_reference_t<const Vec>, const int&&>::value ));
    test_pass("cxx20:ranges::range_rvalue_reference_t");

    test_true(( STD::is_same<rg::range_difference_t<Vec>, Vec::difference_type>::value ));
    test_true( STD::numeric_limits<rg::range_difference_t<Vec> >::is_signed );
    test_pass("cxx20:ranges::range_difference_t");

    test_true(( STD::is_same<rg::range_size_t<Vec>, Vec::size_type>::value ));
    test_true( !STD::numeric_limits<rg::range_size_t<Vec> >::is_signed );
    test_pass("cxx20:ranges::range_size_t");

    test_true(( STD::is_same<rg::borrowed_iterator_t<Vec&>, Vec::iterator>::value ));
    test_true(( STD::is_same<rg::borrowed_iterator_t<Vec>, rg::dangling>::value ));
    test_pass("cxx20:ranges::borrowed_iterator_t");

    test_true(( STD::is_same<rg::borrowed_subrange_t<Vec&>,
                             rg::subrange<Vec::iterator, Vec::iterator> >::value ));
    test_true(( STD::is_same<rg::borrowed_subrange_t<Vec>, rg::dangling>::value ));
    test_pass("cxx20:ranges::borrowed_subrange_t");

    test_true(( STD::is_same<vw::all_t<Vec&>, rg::ref_view<Vec> >::value ));
    test_true(( STD::is_same<vw::all_t<Vec>, rg::owning_view<Vec> >::value ));
    test_pass("cxx20:all_t");
}

TEST_CASE(ranges, customisation_points_cxx20) {
    typedef STD::vector<int> Vec;

    test_true( rg::enable_borrowed_range<RgBorrowed> );
    test_true( !rg::enable_borrowed_range<Vec> );
    test_pass("cxx20:ranges::enable_borrowed_range");

    test_true( rg::enable_view<rg::empty_view<int> > );
    test_true( !rg::enable_view<Vec> );
    test_pass("cxx20:ranges::enable_view");

    test_true( !rg::disable_sized_range<Vec> );
    test_true( !rg::disable_sized_range<STD::forward_list<int> > );
    test_pass("cxx20:disable_sized_range");
}

TEST_CASE(ranges, reverse_access_cxx20) {
    STD::vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i * 10);

    test_true( rg::rend(v) == v.rend() );
    test_eq( *(rg::rend(v) - 1), 0 );
    test_pass("cxx20:ranges::rend");

    test_true( rg::crend(v) == v.crend() );
    test_eq( *(rg::crend(v) - 1), 0 );
    test_pass("cxx20:ranges::crend");
}

TEST_CASE(ranges, view_types_a_cxx20) {
    int arr[5] = { 1, 2, 3, 4, 5 };
    STD::vector<int> v(arr, arr + 5);

    rg::empty_view<int> ev;
    test_true( ev.empty() );
    test_eq( (long)rg::size(ev), 0L );
    test_pass("cxx20:ranges::empty_view");

    rg::single_view<int> sv(7);
    test_eq( (long)rg::size(sv), 1L );
    test_eq( *sv.begin(), 7 );
    test_pass("cxx20:ranges::single_view");

    rg::iota_view<int, int> iv(3, 7);
    test_eq( (long)rg::size(iv), 4L );
    test_eq( *iv.begin(), 3 );
    test_pass("cxx20:ranges::iota_view");

    rg::ref_view<STD::vector<int> > rv(v);
    test_eq( (long)rg::size(rv), 5L );
    test_true( &*rv.begin() == &v[0] );
    test_pass("cxx20:ranges::ref_view");

    STD::vector<int> tmp(v);
    rg::owning_view<STD::vector<int> > ov(STD::move(tmp));
    test_eq( (long)rg::size(ov), 5L );
    test_true( &*ov.begin() != &v[0] );
    test_pass("cxx20:ranges::owning_view");

    rg::filter_view<vw::all_t<STD::vector<int>&>, bool (*)(int)> fv(v, rg_is_odd);
    int fsum = 0;
    for (int x : fv) fsum += x;
    test_eq( fsum, 1 + 3 + 5 );
    test_pass("cxx20:ranges::filter_view");

    rg::transform_view<vw::all_t<STD::vector<int>&>, int (*)(int)> tv(v, rg_twice);
    test_eq( *tv.begin(), 2 );
    test_eq( (long)rg::size(tv), 5L );
    test_pass("cxx20:ranges::transform_view");

    rg::take_view<vw::all_t<STD::vector<int>&> > tkv(v, 2);
    test_eq( (long)rg::size(tkv), 2L );
    test_eq( *tkv.begin(), 1 );
    test_pass("cxx20:ranges::take_view");

    rg::take_while_view<vw::all_t<STD::vector<int>&>, bool (*)(int)> twv(v, rg_is_odd);
    int twn = 0;
    for (int x : twv) { (void)x; ++twn; }
    test_eq( twn, 1 );
    test_pass("cxx20:ranges::take_while_view");

    rg::drop_view<vw::all_t<STD::vector<int>&> > dv(v, 3);
    test_eq( (long)rg::size(dv), 2L );
    test_eq( *dv.begin(), 4 );
    test_pass("cxx20:ranges::drop_view");

    rg::drop_while_view<vw::all_t<STD::vector<int>&>, bool (*)(int)> dwv(v, rg_is_odd);
    test_eq( *dwv.begin(), 2 );
    test_pass("cxx20:ranges::drop_while_view");

    rg::reverse_view<vw::all_t<STD::vector<int>&> > rev(v);
    test_eq( *rev.begin(), 5 );
    test_eq( (long)rg::size(rev), 5L );
    test_pass("cxx20:ranges::reverse_view");
}

TEST_CASE(ranges, view_types_b_cxx20) {
    STD::vector<STD::vector<int> > nested;
    nested.push_back(STD::vector<int>(2, 1));
    nested.push_back(STD::vector<int>(3, 2));

    rg::join_view<vw::all_t<STD::vector<STD::vector<int> >&> > jv(nested);
    int jn = 0, jsum = 0;
    for (int x : jv) { ++jn; jsum += x; }
    test_eq( jn, 5 );
    test_eq( jsum, 1 * 2 + 2 * 3 );
    test_pass("cxx20:ranges::join_view");

    STD::string text("a,bb,c");
    STD::string sep(",");
    rg::lazy_split_view<vw::all_t<STD::string&>, vw::all_t<STD::string&> > lsv(text, sep);
    int lsn = 0;
    for (auto part : lsv) { (void)part; ++lsn; }
    test_eq( lsn, 3 );
    test_pass("cxx20:ranges::lazy_split_view");

    rg::split_view<vw::all_t<STD::string&>, vw::all_t<STD::string&> > spv(text, sep);
    int spn = 0;
    STD::string first;
    for (auto part : spv) {
        if (spn == 0) first.assign(part.begin(), part.end());
        ++spn;
    }
    test_eq( spn, 3 );
    test_eq( first, STD::string("a") );
    test_pass("cxx20:ranges::split_view");

    rg::take_while_view<vw::all_t<STD::string&>, bool (*)(char)> tw(text, +[](char c) { return c != ','; });
    test_true( !rg::common_range<decltype(tw)> );
    rg::common_view<decltype(tw)> cv(tw);
    test_true( rg::common_range<decltype(cv)> );
    test_pass("cxx20:ranges::common_view");

    STD::vector<STD::pair<int, STD::string> > pairs;
    pairs.push_back(STD::pair<int, STD::string>(1, "one"));
    pairs.push_back(STD::pair<int, STD::string>(2, "two"));

    rg::elements_view<vw::all_t<STD::vector<STD::pair<int, STD::string> >&>, 0> el(pairs);
    test_eq( *el.begin(), 1 );
    test_eq( (long)rg::size(el), 2L );
    test_pass("cxx20:ranges::elements_view");

    rg::keys_view<vw::all_t<STD::vector<STD::pair<int, STD::string> >&> > kv(pairs);
    test_eq( *kv.begin(), 1 );
    test_pass("cxx20:ranges::keys_view");

    rg::values_view<vw::all_t<STD::vector<STD::pair<int, STD::string> >&> > vv(pairs);
    test_eq( *vv.begin(), STD::string("one") );
    test_pass("cxx20:ranges::values_view");
}

TEST_CASE(ranges, subrange_and_view_interface_cxx20) {
    STD::vector<int> v;
    for (int i = 1; i <= 5; ++i) v.push_back(i);

    rg::subrange<STD::vector<int>::iterator> sr(v.begin() + 1, v.end() - 1);
    test_eq( (long)sr.size(), 3L );
    test_eq( *sr.begin(), 2 );
    test_true( !sr.empty() );
    test_pass("cxx20:ranges::subrange");

    test_true(( STD::is_same<decltype(sr),
                             rg::subrange<STD::vector<int>::iterator,
                                          STD::vector<int>::iterator,
                                          rg::subrange_kind::sized> >::value ));
    test_true( rg::subrange_kind::unsized != rg::subrange_kind::sized );
    test_pass("cxx20:ranges::subrange_kind");

    test_eq( (int)STD::tuple_size<decltype(sr)>::value, 2 );
    test_pass("cxx20:tuple_size");

    test_true(( STD::is_same<STD::tuple_element<0, decltype(sr)>::type,
                             STD::vector<int>::iterator>::value ));
    test_true(( STD::is_same<STD::tuple_element<1, decltype(sr)>::type,
                             STD::vector<int>::iterator>::value ));
    test_pass("cxx20:tuple_element");

    {
        STD::vector<int>::iterator b = STD::get<0>(sr);
        STD::vector<int>::iterator e = STD::get<1>(sr);
        test_true( b == sr.begin() );
        test_true( e == sr.end() );
    }
    test_pass("cxx23:get");

    RgSpanView sv(&v[0], &v[0] + v.size());
    test_true( !sv.empty() );
    test_eq( sv.front(), 1 );
    test_eq( sv.back(), 5 );
    test_eq( sv[2], 3 );
    test_eq( (long)sv.size(), 5L );
    test_true( (bool)sv );
    test_pass("cxx20:ranges::view_interface");
}

TEST_CASE(ranges, istream_views_cxx20) {
    {
        STD::istringstream is("1 2 3");
        rg::basic_istream_view<int, char> bv(is);
        int n = 0, sum = 0;
        for (int x : bv) { ++n; sum += x; }
        test_eq( n, 3 );
        test_eq( sum, 6 );
    }
    test_pass("cxx20:ranges::basic_istream_view");

    {
        STD::istringstream is("4 5");
        int sum = 0;
        for (int x : rg::istream_view<int>(is)) sum += x;
        test_eq( sum, 9 );
        test_true(( STD::is_same<rg::istream_view<int>,
                                 rg::basic_istream_view<int, char> >::value ));
    }
    test_pass("cxx20:istream_view");

    {
        STD::wistringstream is(L"6 7");
        int sum = 0;
        for (int x : rg::wistream_view<int>(is)) sum += x;
        test_eq( sum, 13 );
        test_true(( STD::is_same<rg::wistream_view<int>,
                                 rg::basic_istream_view<int, wchar_t> >::value ));
    }
    test_pass("cxx20:ranges::wistream_view");

    {
        STD::istringstream is("8 9");
        int sum = 0;
        for (int x : vw::istream<int>(is)) sum += x;
        test_eq( sum, 17 );
        test_true( !rg::forward_range<decltype(vw::istream<int>(is))> );
    }
    test_pass("cxx20:ranges::views::istream");
}

TEST_CASE(ranges, view_objects_a_cxx20) {
    int arr[5] = { 1, 2, 3, 4, 5 };
    STD::vector<int> v(arr, arr + 5);

    {
        auto a = vw::all(v);
        test_true(( STD::is_same<decltype(a), rg::ref_view<STD::vector<int> > >::value ));
        test_eq( (long)rg::size(a), 5L );
    }
    test_pass("cxx20:ranges::views::all");

    {
        auto c = vw::counted(v.begin() + 1, 3);
        test_eq( (long)rg::size(c), 3L );
        test_eq( *c.begin(), 2 );
    }
    test_pass("cxx20:ranges::views::counted");

    {
        auto e = vw::empty<int>;
        test_true( rg::empty(e) );
        test_eq( (long)rg::size(e), 0L );
    }
    test_pass("cxx20:ranges::views::empty");

    {
        auto s = vw::single(42);
        test_eq( (long)rg::size(s), 1L );
        test_eq( *s.begin(), 42 );
    }
    test_pass("cxx20:ranges::views::single");

    {
        auto i = vw::iota(1, 5);
        int sum = 0;
        for (int x : i) sum += x;
        test_eq( sum, 1 + 2 + 3 + 4 );
    }
    test_pass("cxx20:ranges::views::iota");

    {
        auto f = v | vw::filter(rg_is_odd);
        int sum = 0;
        for (int x : f) sum += x;
        test_eq( sum, 9 );
        test_true( rg::view<decltype(f)> );
    }
    test_pass("cxx20:ranges::views::filter");

    {
        auto t = v | vw::transform(rg_twice);
        test_eq( *t.begin(), 2 );
        test_eq( (long)rg::size(t), 5L );
    }
    test_pass("cxx20:ranges::views::transform");

    {
        auto t = v | vw::take(2);
        test_eq( (long)rg::size(t), 2L );
        test_eq( *(t.begin() + 1), 2 );
        auto over = v | vw::take(99);
        test_eq( (long)rg::size(over), 5L );
    }
    test_pass("cxx20:ranges::views::take");

    {
        auto t = v | vw::take_while(rg_is_odd);
        int n = 0;
        for (int x : t) { (void)x; ++n; }
        test_eq( n, 1 );
    }
    test_pass("cxx20:ranges::views::take_while");

    {
        auto d = v | vw::drop(3);
        test_eq( (long)rg::size(d), 2L );
        test_eq( *d.begin(), 4 );
    }
    test_pass("cxx20:ranges::views::drop");

    {
        auto d = v | vw::drop_while(rg_is_odd);
        test_eq( *d.begin(), 2 );
        test_eq( (long)rg::size(d), 4L );
    }
    test_pass("cxx20:ranges::views::drop_while");

    {
        auto r = v | vw::reverse;
        test_eq( *r.begin(), 5 );
        auto twice = v | vw::reverse | vw::reverse;
        test_eq( *twice.begin(), 1 );
    }
    test_pass("cxx20:ranges::views::reverse");
}

TEST_CASE(ranges, view_objects_b_cxx20) {
    STD::vector<STD::vector<int> > nested;
    nested.push_back(STD::vector<int>(2, 1));
    nested.push_back(STD::vector<int>(3, 2));

    {
        auto j = nested | vw::join;
        int n = 0, sum = 0;
        for (int x : j) { ++n; sum += x; }
        test_eq( n, 5 );
        test_eq( sum, 8 );
    }
    test_pass("cxx20:ranges::views::join");

    STD::string text("a,bb,c");
    {
        auto s = text | vw::split(',');
        int n = 0;
        STD::string last;
        for (auto part : s) { last.assign(part.begin(), part.end()); ++n; }
        test_eq( n, 3 );
        test_eq( last, STD::string("c") );
    }
    test_pass("cxx20:ranges::views::split");

    {
        auto s = text | vw::lazy_split(',');
        int n = 0;
        for (auto part : s) { (void)part; ++n; }
        test_eq( n, 3 );
        test_true( !rg::random_access_range<decltype(s)> );
    }
    test_pass("cxx20:ranges::views::lazy_split");

    {
        auto tw = text | vw::take_while([](char c) { return c != ','; });
        auto c = tw | vw::common;
        test_true( rg::common_range<decltype(c)> );
        test_true( !rg::common_range<decltype(tw)> );
    }
    test_pass("cxx20:ranges::views::common");

    STD::vector<STD::pair<int, STD::string> > pairs;
    pairs.push_back(STD::pair<int, STD::string>(1, "one"));
    pairs.push_back(STD::pair<int, STD::string>(2, "two"));

    {
        auto e = pairs | vw::elements<1>;
        test_eq( *e.begin(), STD::string("one") );
        test_eq( (long)rg::size(e), 2L );
    }
    test_pass("cxx20:ranges::views::elements");

    {
        auto k = pairs | vw::keys;
        int sum = 0;
        for (int x : k) sum += x;
        test_eq( sum, 3 );
    }
    test_pass("cxx20:ranges::views::keys");

    {
        auto vals = pairs | vw::values;
        test_eq( *vals.begin(), STD::string("one") );
    }
    test_pass("cxx20:ranges::views::values");
}

TEST_CASE(ranges, views_cxx23_a) {
    STD::vector<int> v;
    for (int i = 1; i <= 5; ++i) v.push_back(i);

#if defined(__cpp_lib_ranges_as_const)
    {
        auto c = v | vw::as_const;
        test_true(( STD::is_same<rg::range_reference_t<decltype(c)>, const int&>::value ));
        test_eq( *c.begin(), 1 );
    }
    test_pass("cxx23:ranges::views::as_const");
    {
        rg::as_const_view<vw::all_t<STD::vector<int>&> > acv(v);
        test_eq( (long)rg::size(acv), 5L );
        test_true(( STD::is_same<rg::range_reference_t<decltype(acv)>, const int&>::value ));
    }
    test_pass("cxx23:ranges::as_const_view");
    {
        test_true( rg::constant_range<rg::as_const_view<vw::all_t<STD::vector<int>&> > > );
        test_true( !rg::constant_range<STD::vector<int>&> );
    }
    test_pass("cxx23:ranges::constant_range");
    {
        test_true(( STD::is_same<rg::const_iterator_t<STD::vector<int> >,
                                 STD::vector<int>::const_iterator>::value ));
    }
    test_pass("cxx23:ranges::const_iterator_t");
    {
        test_true(( STD::is_same<rg::const_sentinel_t<STD::vector<int> >,
                                 STD::vector<int>::const_iterator>::value ));
    }
    test_pass("cxx23:ranges::const_sentinel_t");
    {
        test_true(( STD::is_same<rg::range_const_reference_t<STD::vector<int> >,
                                 const int&>::value ));
    }
    test_pass("cxx23:ranges::range_const_reference_t");
#else
    test_skip("cxx23:ranges::views::as_const");
    test_skip("cxx23:ranges::as_const_view");
    test_skip("cxx23:ranges::constant_range");
    test_skip("cxx23:ranges::const_iterator_t");
    test_skip("cxx23:ranges::const_sentinel_t");
    test_skip("cxx23:ranges::range_const_reference_t");
#endif

#if defined(__cpp_lib_ranges_as_rvalue)
    {
        STD::vector<STD::string> s;
        s.push_back("aa");
        s.push_back("bb");
        auto r = s | vw::as_rvalue;
        test_true(( STD::is_same<rg::range_reference_t<decltype(r)>, STD::string&&>::value ));
        STD::vector<STD::string> moved(r.begin(), r.end());
        test_eq( moved[0], STD::string("aa") );
        test_true( s[0].empty() );
    }
    test_pass("cxx23:ranges::views::as_rvalue");
    {
        rg::as_rvalue_view<vw::all_t<STD::vector<int>&> > arv(v);
        test_true(( STD::is_same<rg::range_reference_t<decltype(arv)>, int&&>::value ));
        test_eq( (long)rg::size(arv), 5L );
    }
    test_pass("cxx23:ranges::as_rvalue_view");
#else
    test_skip("cxx23:ranges::views::as_rvalue");
    test_skip("cxx23:ranges::as_rvalue_view");
#endif

#if defined(__cpp_lib_ranges_enumerate)
    {
        auto e = v | vw::enumerate;
        int lastIdx = -1, lastVal = -1;
        for (auto&& pair : e) { lastIdx = (int)STD::get<0>(pair); lastVal = STD::get<1>(pair); }
        test_eq( lastIdx, 4 );
        test_eq( lastVal, 5 );
    }
    test_pass("cxx23:ranges::views::enumerate");
    {
        rg::enumerate_view<vw::all_t<STD::vector<int>&> > ev(v);
        test_eq( (long)rg::size(ev), 5L );
        test_eq( (int)STD::get<0>(*ev.begin()), 0 );
    }
    test_pass("cxx23:ranges::enumerate_view");
#else
    test_skip("cxx23:ranges::views::enumerate");
    test_skip("cxx23:ranges::enumerate_view");
#endif

#if defined(__cpp_lib_ranges_repeat)
    {
        auto r = vw::repeat(7, 3);
        int n = 0, sum = 0;
        for (int x : r) { ++n; sum += x; }
        test_eq( n, 3 );
        test_eq( sum, 21 );
    }
    test_pass("cxx23:ranges::views::repeat");
    {
        rg::repeat_view<int, int> rv(9, 2);
        test_eq( (long)rg::size(rv), 2L );
        test_eq( *rv.begin(), 9 );
    }
    test_pass("cxx23:ranges::repeat_view");
#else
    test_skip("cxx23:ranges::views::repeat");
    test_skip("cxx23:ranges::repeat_view");
#endif

#if defined(__cpp_lib_ranges_to_container)
    {
        STD::list<int> l = v | rg::to<STD::list<int> >();
        test_eq( (long)l.size(), 5L );
        test_eq( l.front(), 1 );
        STD::vector<int> back = rg::to<STD::vector<int> >(l);
        test_eq( back[4], 5 );
    }
    test_pass("cxx23:ranges::to");
#else
    test_skip("cxx23:ranges::to");
#endif

#if defined(__cpp_lib_ranges_as_rvalue)
    {
        auto r = v | RgAddOne();
        test_eq( *r.begin(), 2 );
        auto chained = v | RgAddOne() | vw::take(2);
        test_eq( (long)rg::size(chained), 2L );
    }
    test_pass("cxx23:range_adaptor_closure");
#else
    test_skip("cxx23:range_adaptor_closure");
#endif
}

TEST_CASE(ranges, views_cxx23_b) {
    STD::vector<int> v;
    for (int i = 1; i <= 5; ++i) v.push_back(i);

#if defined(__cpp_lib_ranges_zip)
    {
        auto z = vw::zip(v, v);
        test_eq( (long)rg::size(z), 5L );
        test_eq( STD::get<0>(*z.begin()) + STD::get<1>(*z.begin()), 2 );
    }
    test_pass("cxx23:ranges::views::zip");
    {
        rg::zip_view<vw::all_t<STD::vector<int>&>, vw::all_t<STD::vector<int>&> > zv(v, v);
        test_eq( (long)rg::size(zv), 5L );
    }
    test_pass("cxx23:ranges::zip_view");
    {
        auto zt = vw::zip_transform(STD::plus<int>(), v, v);
        test_eq( *zt.begin(), 2 );
        test_eq( (long)rg::size(zt), 5L );
    }
    test_pass("cxx23:ranges::views::zip_transform");
    {
        rg::zip_transform_view<STD::plus<int>, vw::all_t<STD::vector<int>&>,
                               vw::all_t<STD::vector<int>&> > ztv(STD::plus<int>(), v, v);
        test_eq( *ztv.begin(), 2 );
    }
    test_pass("cxx23:ranges::zip_transform_view");
    {
        auto a = v | vw::adjacent<2>;
        test_eq( (long)rg::size(a), 4L );
        test_eq( STD::get<1>(*a.begin()), 2 );
    }
    test_pass("cxx23:ranges::views::adjacent");
    {
        rg::adjacent_view<vw::all_t<STD::vector<int>&>, 3> av(v);
        test_eq( (long)rg::size(av), 3L );
    }
    test_pass("cxx23:ranges::adjacent_view");
    {
        auto p = v | vw::pairwise;
        test_eq( (long)rg::size(p), 4L );
        test_eq( STD::get<0>(*p.begin()), 1 );
    }
    test_pass("cxx23:ranges::views::pairwise");
    {
        auto at = v | vw::adjacent_transform<2>(STD::plus<int>());
        test_eq( *at.begin(), 3 );
        test_eq( (long)rg::size(at), 4L );
    }
    test_pass("cxx23:ranges::views::adjacent_transform");
    {
        rg::adjacent_transform_view<vw::all_t<STD::vector<int>&>, STD::plus<int>, 2>
            atv(v, STD::plus<int>());
        test_eq( *atv.begin(), 3 );
    }
    test_pass("cxx23:ranges::adjacent_transform_view");
    {
        auto pt = v | vw::pairwise_transform(STD::plus<int>());
        test_eq( *pt.begin(), 3 );
        test_eq( (long)rg::size(pt), 4L );
    }
    test_pass("cxx23:ranges::views::pairwise_transform");
#else
    test_skip("cxx23:ranges::views::zip");
    test_skip("cxx23:ranges::zip_view");
    test_skip("cxx23:ranges::views::zip_transform");
    test_skip("cxx23:ranges::zip_transform_view");
    test_skip("cxx23:ranges::views::adjacent");
    test_skip("cxx23:ranges::adjacent_view");
    test_skip("cxx23:ranges::views::pairwise");
    test_skip("cxx23:ranges::views::adjacent_transform");
    test_skip("cxx23:ranges::adjacent_transform_view");
    test_skip("cxx23:ranges::views::pairwise_transform");
#endif

#if defined(__cpp_lib_ranges_chunk)
    {
        auto c = v | vw::chunk(2);
        test_eq( (long)rg::size(c), 3L );
        test_eq( (long)rg::size(*c.begin()), 2L );
    }
    test_pass("cxx23:ranges::views::chunk");
    {
        rg::chunk_view<vw::all_t<STD::vector<int>&> > cv(v, 3);
        test_eq( (long)rg::size(cv), 2L );
    }
    test_pass("cxx23:ranges::chunk_view");
#else
    test_skip("cxx23:ranges::views::chunk");
    test_skip("cxx23:ranges::chunk_view");
#endif

#if defined(__cpp_lib_ranges_chunk_by)
    {
        int data[6] = { 1, 2, 3, 1, 2, 3 };
        STD::vector<int> w(data, data + 6);
        auto c = w | vw::chunk_by(STD::less<int>());
        int n = 0;
        for (auto part : c) { (void)part; ++n; }
        test_eq( n, 2 );
        test_eq( (long)rg::size(*c.begin()), 3L );
    }
    test_pass("cxx23:ranges::views::chunk_by");
    {
        rg::chunk_by_view<vw::all_t<STD::vector<int>&>, STD::less<int> >
            cbv(v, STD::less<int>());
        test_eq( (long)rg::size(*cbv.begin()), 5L );
    }
    test_pass("cxx23:ranges::chunk_by_view");
#else
    test_skip("cxx23:ranges::views::chunk_by");
    test_skip("cxx23:ranges::chunk_by_view");
#endif

#if defined(__cpp_lib_ranges_slide)
    {
        auto s = v | vw::slide(3);
        test_eq( (long)rg::size(s), 3L );
        test_eq( (long)rg::size(*s.begin()), 3L );
    }
    test_pass("cxx23:ranges::views::slide");
    {
        rg::slide_view<vw::all_t<STD::vector<int>&> > sv(v, 2);
        test_eq( (long)rg::size(sv), 4L );
    }
    test_pass("cxx23:ranges::slide_view");
#else
    test_skip("cxx23:ranges::views::slide");
    test_skip("cxx23:ranges::slide_view");
#endif

#if defined(__cpp_lib_ranges_stride)
    {
        auto s = v | vw::stride(2);
        test_eq( (long)rg::size(s), 3L );
        test_eq( *s.begin(), 1 );
    }
    test_pass("cxx23:ranges::views::stride");
    {
        rg::stride_view<vw::all_t<STD::vector<int>&> > sv(v, 3);
        test_eq( (long)rg::size(sv), 2L );
    }
    test_pass("cxx23:ranges::stride_view");
#else
    test_skip("cxx23:ranges::views::stride");
    test_skip("cxx23:ranges::stride_view");
#endif

#if defined(__cpp_lib_ranges_join_with)
    {
        STD::vector<STD::string> parts;
        parts.push_back("ab");
        parts.push_back("cd");
        auto j = parts | vw::join_with('-');
        STD::string out(j.begin(), j.end());
        test_eq( out, STD::string("ab-cd") );
    }
    test_pass("cxx23:ranges::views::join_with");
    {
        STD::vector<STD::string> parts;
        parts.push_back("x");
        parts.push_back("y");
        rg::join_with_view<vw::all_t<STD::vector<STD::string>&>, rg::single_view<char> >
            jv(parts, rg::single_view<char>(','));
        STD::string out(jv.begin(), jv.end());
        test_eq( out, STD::string("x,y") );
    }
    test_pass("cxx23:ranges::join_with_view");
#else
    test_skip("cxx23:ranges::views::join_with");
    test_skip("cxx23:ranges::join_with_view");
#endif

#if defined(__cpp_lib_ranges_cartesian_product)
    {
        int small[2] = { 1, 2 };
        STD::vector<int> a(small, small + 2);
        auto c = vw::cartesian_product(a, a);
        test_eq( (long)rg::size(c), 4L );
        test_eq( STD::get<0>(*c.begin()) + STD::get<1>(*c.begin()), 2 );
    }
    test_pass("cxx23:ranges::views::cartesian_product");
    {
        rg::cartesian_product_view<vw::all_t<STD::vector<int>&>,
                                   vw::all_t<STD::vector<int>&> > cv(v, v);
        test_eq( (long)rg::size(cv), 25L );
    }
    test_pass("cxx23:ranges::cartesian_product_view");
#else
    test_skip("cxx23:ranges::views::cartesian_product");
    test_skip("cxx23:ranges::cartesian_product_view");
#endif

#if defined(__cpp_lib_ranges_zip)
    {
        test_true(( STD::is_same<rg::range_common_reference_t<STD::vector<int> >,
                                 STD::iter_common_reference_t<rg::iterator_t<STD::vector<int> > >
                                >::value ));
        test_true(( STD::is_same<rg::range_common_reference_t<STD::vector<int> >, int&>::value ));
    }
    test_pass("cxx23:ranges::range_common_reference_t");
#else
    test_skip("cxx23:ranges::range_common_reference_t");
#endif
}

TEST_CASE(ranges, views_cxx26) {
    STD::vector<int> v;
    for (int i = 1; i <= 3; ++i) v.push_back(i);

#if defined(__cpp_lib_ranges_concat)
    {
        auto c = vw::concat(v, v);
        test_eq( (long)rg::size(c), 6L );
        int sum = 0;
        for (int x : c) sum += x;
        test_eq( sum, 12 );
    }
    test_pass("cxx26:ranges::views::concat");
    {
        rg::concat_view<vw::all_t<STD::vector<int>&>, vw::all_t<STD::vector<int>&> > cv(v, v);
        test_eq( (long)rg::size(cv), 6L );
    }
    test_pass("cxx26:ranges::concat_view");
#else
    test_skip("cxx26:ranges::views::concat");
    test_skip("cxx26:ranges::concat_view");
#endif

#if defined(__cpp_lib_ranges_cache_latest)
    {
        int calls = 0;
        auto t = v | vw::transform([&calls](int x) { ++calls; return x * 2; })
                   | vw::cache_latest;
        auto it = t.begin();
        int a = *it;
        int b = *it;
        test_eq( a, b );
        test_eq( calls, 1 );
    }
    test_pass("cxx26:ranges::views::cache_latest");
    {
        rg::cache_latest_view<vw::all_t<STD::vector<int>&> > cv(v);
        test_eq( *cv.begin(), 1 );
    }
    test_pass("cxx26:ranges::cache_latest_view");
#else
    test_skip("cxx26:ranges::views::cache_latest");
    test_skip("cxx26:ranges::cache_latest_view");
#endif

#if defined(__cpp_lib_ranges_to_input)
    {
        auto t = v | vw::to_input;
        test_true( rg::input_range<decltype(t)> );
        test_true( !rg::forward_range<decltype(t)> );
        int sum = 0;
        for (int x : t) sum += x;
        test_eq( sum, 6 );
    }
    test_pass("cxx26:ranges::views::to_input");
    {
        rg::to_input_view<vw::all_t<STD::vector<int>&> > tv(v);
        test_true( !rg::forward_range<decltype(tv)> );
    }
    test_pass("cxx26:ranges::to_input_view");
#else
    test_skip("cxx26:ranges::views::to_input");
    test_skip("cxx26:ranges::to_input_view");
#endif

#if defined(__cpp_lib_ranges_as_input)
    {
        rg::as_input_view<vw::all_t<STD::vector<int>&> > av(v);
        test_true( !rg::forward_range<decltype(av)> );
        test_true( rg::input_range<decltype(av)> );
    }
    test_pass("cxx26:as_input_view");
#else
    TEST_NOTE("libstdc++ 15.2 ships to_input_view but not the as_input_view spelling");
    test_skip("cxx26:as_input_view");
#endif

#if defined(__cpp_lib_ranges_indices)
    {
        auto i = vw::indices(4);
        test_eq( (long)rg::size(i), 4L );
        test_eq( (long)*i.begin(), 0L );
    }
    test_pass("cxx26:ranges::views::indices");
#else
    TEST_NOTE("libstdc++ 15.2 has no views::indices (P2214)");
    test_skip("cxx26:ranges::views::indices");
#endif

#if defined(__cpp_lib_ranges_reserve_hint)
    {
        test_true( rg::approximately_sized_range<STD::vector<int> > );
        test_eq( (long)rg::reserve_hint(v), 3L );
    }
    test_pass("cxx26:approximately_sized_range");
    test_pass("cxx26:reserve_hint");
#else
    TEST_NOTE("libstdc++ 15.2 has no reserve_hint / approximately_sized_range (P2846)");
    test_skip("cxx26:approximately_sized_range");
    test_skip("cxx26:reserve_hint");
#endif
}

#else   /* no C++20 <ranges> here */
TEST_CASE_SKIP(ranges, concepts_cxx20)
TEST_CASE_SKIP(ranges, type_aliases_cxx20)
TEST_CASE_SKIP(ranges, customisation_points_cxx20)
TEST_CASE_SKIP(ranges, reverse_access_cxx20)
TEST_CASE_SKIP(ranges, view_types_a_cxx20)
TEST_CASE_SKIP(ranges, view_types_b_cxx20)
TEST_CASE_SKIP(ranges, subrange_and_view_interface_cxx20)
TEST_CASE_SKIP(ranges, istream_views_cxx20)
TEST_CASE_SKIP(ranges, view_objects_a_cxx20)
TEST_CASE_SKIP(ranges, view_objects_b_cxx20)
TEST_CASE_SKIP(ranges, views_cxx23_a)
TEST_CASE_SKIP(ranges, views_cxx23_b)
TEST_CASE_SKIP(ranges, views_cxx26)
#endif
