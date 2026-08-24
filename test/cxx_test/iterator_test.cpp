#include "test_cxx.hpp"

#include <iterator>
#include <vector>
#include <list>
#include <sstream>
#include <type_traits>

namespace {
    struct ItFn   { int  operator()(int) const { return 0; } };
    struct ItPred { bool operator()(int) const { return true; } };
    struct ItRel  { bool operator()(int, int) const { return true; } };
}

TEST_CASE(iterator, cxx03) {
    STD::vector<int> v;
    v.push_back(1); v.push_back(2); v.push_back(3);

    test_eq( STD::distance(v.begin(), v.end()), 3 );
    test_pass("cxx03:distance");
    STD::vector<int>::iterator it = v.begin();
    STD::advance(it, 2);
    test_eq( *it, 3 );
    test_pass("cxx03:advance");
    test_eq( (int)STD::iterator_traits<STD::vector<int>::iterator>::difference_type(0), 0 );
    test_pass("cxx03:iterator_traits");

    STD::vector<int> out;
    STD::back_insert_iterator<STD::vector<int> > bi = STD::back_inserter(out);
    *bi = 9; ++bi;
    test_eq( out[0], 9 );
    test_pass("cxx03:back_inserter");
    STD::insert_iterator<STD::vector<int> > ii = STD::inserter(out, out.end());
    *ii = 8;
    test_eq( out[1], 8 );
    test_pass("cxx03:inserter");

    STD::list<int> l;
    STD::front_insert_iterator<STD::list<int> > fi = STD::front_inserter(l);
    *fi = 1; ++fi; *fi = 2;
    test_eq( l.front(), 2 );
    test_pass("cxx03:front_inserter");

    STD::vector<int>::reverse_iterator r = v.rbegin();
    test_eq( *r, 3 );
    test_true( v.rbegin() != v.rend() );
    test_pass("cxx03:reverse_iterator");
}

TEST_CASE(iterator, cxx11) {
    int a[] = { 10, 20, 30 };
#if !defined(__WATCOMC__)
    test_eq( *STD::begin(a), 10 );
    test_eq( *(STD::end(a) - 1), 30 );
#else
    (void)a;
    TEST_NOTE("Open Watcom cannot match a bounded array T[N] against a template "
              "parameter, so the array overloads cannot be provided (A12)");
    TEST_SKIP1(); TEST_SKIP1();
#endif

    STD::vector<int> v;
    v.push_back(1); v.push_back(2); v.push_back(3);
    test_eq( *STD::begin(v), 1 );
    test_pass("cxx11:begin (free)");
    test_eq( *(STD::end(v) - 1), 3 );
    test_pass("cxx11:end (free)");
    test_eq( *STD::next(v.begin()), 2 );
    test_eq( *STD::next(v.begin(), 2), 3 );
    test_pass("cxx11:next");
    test_eq( *STD::prev(v.end()), 3 );
    test_eq( *STD::prev(v.end(), 2), 2 );
    test_pass("cxx11:prev");
}

#if TEST_TARGET_CXX >= 2014
TEST_CASE(iterator, cxx14) {
    STD::vector<int> v;
    v.push_back(1); v.push_back(2); v.push_back(3);
    test_eq( *STD::cbegin(v), 1 );
    test_pass("cxx14:cbegin (free)");
    test_true( STD::cbegin(v) != STD::cend(v) );
    test_pass("cxx14:cend (free)");
    test_eq( *STD::rbegin(v), 3 );
    test_pass("cxx14:rbegin (free)");
    test_true( STD::rbegin(v) != STD::rend(v) );
    test_pass("cxx14:rend (free)");
    test_eq( *STD::crbegin(v), 3 );
    test_pass("cxx14:crbegin (free)");
    test_true( STD::crbegin(v) != STD::crend(v) );
    test_pass("cxx14:crend (free)");

#if !defined(__WATCOMC__)
    int a[] = { 7, 8, 9 };
    test_eq( *STD::cbegin(a), 7 );
    test_eq( *STD::rbegin(a), 9 );
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif
}
#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(iterator, cxx17) {
    STD::vector<int> v;
    v.push_back(1); v.push_back(2);
    test_eq( STD::size(v), 2u );
    test_pass("cxx17:size");
    test_true( !STD::empty(v) );
    test_pass("cxx17:empty");

#if !defined(__WATCOMC__)
    int a[5] = { 0 };
    test_eq( STD::size(a), 5u );
    test_true( !STD::empty(a) );
    int da[3] = { 7, 8, 9 };
    test_eq( *STD::data(da), 7 );
    test_pass("cxx17:data");
#else
    TEST_NOTE("Open Watcom cannot match a bounded array T[N] against a template "
              "parameter, so the array overloads cannot be provided (A12)");
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx17:data");
#endif
}
#endif

#if TEST_TARGET_CXX >= 2020
TEST_CASE(iterator, cxx20) {
    STD::vector<int> v;
    v.push_back(1); v.push_back(2); v.push_back(3);
    test_eq( (long long)STD::ssize(v), 3 );
#if !defined(__WATCOMC__)
    int a[4] = { 0 };
    test_eq( (long long)STD::ssize(a), 4 );
    test_pass("cxx20:ssize");
#else
    TEST_SKIP1();
    test_skip("cxx20:ssize");
#endif
}
#endif

#if TEST_TARGET_CXX >= 2020
TEST_CASE(iterator, tags_sentinels) {

    STD::contiguous_iterator_tag ct;
    STD::random_access_iterator_tag& r = ct;
    (void)r;
    test_true( sizeof(STD::contiguous_iterator_tag) >= 1 );
    test_pass("cxx20:contiguous_iterator_tag");

    STD::default_sentinel_t ds = STD::default_sentinel;
    (void)ds;
    test_true( sizeof(STD::default_sentinel_t) >= 1 );
    test_pass("cxx20:default_sentinel");

    int arr[3]; int* p = arr;
    test_true( STD::unreachable_sentinel != p );
    test_true( p != STD::unreachable_sentinel );
    test_true( !(STD::unreachable_sentinel == p) );
    test_pass("cxx20:unreachable_sentinel");
}
#endif

#if TEST_TARGET_CXX >= 2020
#include <type_traits>
TEST_CASE(iterator, incrementable_readable_traits_and_ranges_ops) {

    test_true( (STD::is_same<STD::incrementable_traits<int*>::difference_type, STD::ptrdiff_t>::value) );
    test_true( (STD::is_same<STD::incrementable_traits<const int*>::difference_type, STD::ptrdiff_t>::value) );
    test_true( (STD::is_same<STD::incrementable_traits<STD::vector<int>::iterator>::difference_type,
                             STD::vector<int>::difference_type>::value) );
    test_pass("cxx20:incrementable_traits");

    test_true( (STD::is_same<STD::indirectly_readable_traits<int*>::value_type, int>::value) );
    test_true( (STD::is_same<STD::indirectly_readable_traits<const int*>::value_type, int>::value) );
    test_true( (STD::is_same<STD::indirectly_readable_traits<STD::vector<int>::iterator>::value_type, int>::value) );
    test_pass("cxx20:indirectly_readable_traits");

    STD::vector<int> v; for (int i = 0; i < 5; ++i) v.push_back(i);
    STD::vector<int>::iterator it = v.begin();
    STD::ranges::advance(it, 2);
    test_eq( *it, 2 );
    test_pass("cxx20:ranges::advance");
    test_eq( STD::ranges::distance(v.begin(), v.end()), (STD::ptrdiff_t)5 );
    test_pass("cxx20:ranges::distance");
    test_eq( *STD::ranges::next(v.begin()), 1 );
    test_eq( *STD::ranges::next(v.begin(), 3), 3 );
    test_pass("cxx20:ranges::next");
    test_eq( *STD::ranges::prev(v.end(), 1), 4 );
    test_eq( *STD::ranges::prev(v.end(), 2), 3 );
    test_pass("cxx20:ranges::prev");

    STD::ranges::iter_swap(v.begin(), v.begin() + 4);
    test_eq( v[0], 4 );
    test_eq( v[4], 0 );
    test_pass("cxx20:ranges::iter_swap");
    int m = STD::ranges::iter_move(v.begin());
    test_eq( m, 4 );
    test_pass("cxx20:ranges::iter_move");
}
#endif

TEST_CASE(iterator, iterator_tags_and_streams) {

    test_true( sizeof(STD::input_iterator_tag)  >= 1 );
    test_pass("cxx03:input_iterator_tag");
    test_true( sizeof(STD::output_iterator_tag) >= 1 );
    test_pass("cxx03:output_iterator_tag");
    test_true(( STD::is_base_of<STD::input_iterator_tag, STD::forward_iterator_tag>::value ));
    test_pass("cxx03:forward_iterator_tag");
    test_true(( STD::is_base_of<STD::forward_iterator_tag, STD::bidirectional_iterator_tag>::value ));
    test_pass("cxx03:bidirectional_iterator_tag");
    test_true(( STD::is_base_of<STD::bidirectional_iterator_tag, STD::random_access_iterator_tag>::value ));
    test_pass("cxx03:random_access_iterator_tag");

    test_skip("cxx03:iterator (deprecated cxx17)");

    STD::istringstream is("10 20 30");
    STD::istream_iterator<int> ii(is), iend;
    test_eq( *ii, 10 );
    ++ii;
    test_eq( *ii, 20 );
    test_pass("cxx03:istream_iterator");

    STD::ostringstream os;
    STD::ostream_iterator<int> oi(os, ",");
    *oi = 1; ++oi; *oi = 2; ++oi;
    test_true( os.str() == "1,2," );
    test_pass("cxx03:ostream_iterator");

    STD::istringstream is2("abc");
    STD::istreambuf_iterator<char> bi(is2), bend;
    test_eq( *bi, 'a' );
    test_true( bi != bend );
    test_pass("cxx03:istreambuf_iterator");

    STD::ostringstream os2;
    STD::ostreambuf_iterator<char> bo(os2);
    *bo = 'X'; ++bo; *bo = 'Y';
    test_true( os2.str() == "XY" );
    test_pass("cxx03:ostreambuf_iterator");
}

#if TEST_TARGET_CXX >= 2011 && _TST_HAS_RVALUE_REF
TEST_CASE(iterator, move_and_reverse_iterators) {
    STD::vector<STD::string> v;
    v.push_back("a"); v.push_back("b"); v.push_back("c");

    STD::move_iterator<STD::vector<STD::string>::iterator> mi(v.begin());
    STD::string taken = *mi;
    test_true( taken == "a" );
    test_pass("cxx11:move_iterator");

    STD::vector<STD::string> dst(STD::make_move_iterator(v.begin() + 1),
                                 STD::make_move_iterator(v.end()));
    test_true( dst.size() == 2 && dst[0] == "b" && dst[1] == "c" );
    test_pass("cxx11:make_move_iterator");

#if TEST_TARGET_CXX >= 2014

    STD::vector<int> n; n.push_back(1); n.push_back(2); n.push_back(3);
    STD::reverse_iterator<STD::vector<int>::iterator> r = STD::make_reverse_iterator(n.end());
    test_eq( *r, 3 );
    test_pass("cxx14:make_reverse_iterator");
#else
    test_skip("cxx14:make_reverse_iterator");
#endif
}
#endif

#if defined(__cpp_lib_concepts)
TEST_CASE(iterator, cxx20_iterator_concepts) {
    test_true( STD::weakly_incrementable<int*> );          test_pass("cxx20:weakly_incrementable");
    test_true( STD::incrementable<int*> );                 test_pass("cxx20:incrementable");
    test_true( STD::input_or_output_iterator<int*> );      test_pass("cxx20:input_or_output_iterator");
    test_true( STD::indirectly_readable<int*> );           test_pass("cxx20:indirectly_readable");
    test_true( (STD::indirectly_writable<int*, int>) );    test_pass("cxx20:indirectly_writable");
    test_true( STD::input_iterator<int*> );                test_pass("cxx20:input_iterator");
    test_true( (STD::output_iterator<int*, int>) );        test_pass("cxx20:output_iterator");
    test_true( STD::forward_iterator<int*> );              test_pass("cxx20:forward_iterator");
    test_true( STD::bidirectional_iterator<int*> );        test_pass("cxx20:bidirectional_iterator");
    test_true( STD::random_access_iterator<int*> );        test_pass("cxx20:random_access_iterator");
    test_true( STD::contiguous_iterator<int*> );           test_pass("cxx20:contiguous_iterator");
    test_true( (STD::sentinel_for<int*, int*>) );          test_pass("cxx20:sentinel_for");
    test_true( (STD::sized_sentinel_for<int*, int*>) );    test_pass("cxx20:sized_sentinel_for");
    test_true( (STD::indirectly_movable<int*, int*>) );    test_pass("cxx20:indirectly_movable");
    test_true( (STD::indirectly_movable_storable<int*, int*>) ); test_pass("cxx20:indirectly_movable_storable");
    test_true( (STD::indirectly_copyable<int*, int*>) );   test_pass("cxx20:indirectly_copyable");
    test_true( (STD::indirectly_copyable_storable<int*, int*>) ); test_pass("cxx20:indirectly_copyable_storable");
    test_true( (STD::indirectly_swappable<int*, int*>) );  test_pass("cxx20:indirectly_swappable");
    test_true( (STD::indirectly_comparable<int*, int*, STD::ranges::equal_to>) ); test_pass("cxx20:indirectly_comparable");
    test_true( (STD::indirectly_unary_invocable<ItFn, int*>) ); test_pass("cxx20:indirectly_unary_invocable");
    test_true( (STD::indirectly_regular_unary_invocable<ItFn, int*>) ); test_pass("cxx20:indirectly_regular_unary_invocable");
    test_true( (STD::indirect_unary_predicate<ItPred, int*>) ); test_pass("cxx20:indirect_unary_predicate");
    test_true( (STD::indirect_binary_predicate<ItRel, int*, int*>) ); test_pass("cxx20:indirect_binary_predicate");
    test_true( (STD::indirect_equivalence_relation<ItRel, int*>) ); test_pass("cxx20:indirect_equivalence_relation");
    test_true( (STD::indirect_strict_weak_order<ItRel, int*>) ); test_pass("cxx20:indirect_strict_weak_order");
    test_true( (STD::mergeable<int*, int*, int*>) );       test_pass("cxx20:mergeable");
    test_true( STD::permutable<int*> );                    test_pass("cxx20:permutable");
    test_true( STD::sortable<int*> );                      test_pass("cxx20:sortable");
}

TEST_CASE(iterator, cxx20_iterator_aliases) {
    test_true(( STD::is_same<STD::iter_value_t<int*>, int>::value ));
    test_pass("cxx20:iter_value_t");
    test_true(( STD::is_same<STD::iter_reference_t<int*>, int&>::value ));
    test_pass("cxx20:iter_reference_t");
    test_true(( STD::is_same<STD::iter_difference_t<int*>, STD::ptrdiff_t>::value ));
    test_pass("cxx20:iter_difference_t");
    test_true(( STD::is_same<STD::iter_rvalue_reference_t<int*>, int&&>::value ));
    test_pass("cxx20:iter_rvalue_reference_t");
    test_true(( STD::is_reference<STD::iter_common_reference_t<int*> >::value ));
    test_pass("cxx20:iter_common_reference_t");
    test_true(( STD::is_same<STD::indirect_result_t<ItFn, int*>, int>::value ));
    test_pass("cxx20:indirect_result_t");

    test_true(( STD::is_same<STD::iter_value_t<STD::projected<int*, ItFn> >, int>::value ));
    test_pass("cxx20:projected");

    int arr[3] = { 5, 6, 7 };

    STD::counted_iterator<int*> cnt(arr, 3);
    test_eq( *cnt, 5 );
    test_eq( (long)cnt.count(), 3L );
    test_pass("cxx20:counted_iterator");

    STD::common_iterator<STD::counted_iterator<int*>, STD::default_sentinel_t> ci(cnt);
    test_eq( *ci, 5 );
    test_pass("cxx20:common_iterator");

    STD::move_sentinel<int*> ms(arr + 3);
    test_true( ms.base() == arr + 3 );
    test_pass("cxx20:move_sentinel");
}
#else
TEST_CASE(iterator, cxx20_iterator_concepts) {
    test_skip("cxx20:weakly_incrementable"); test_skip("cxx20:incrementable");
    test_skip("cxx20:input_or_output_iterator"); test_skip("cxx20:indirectly_readable");
    test_skip("cxx20:indirectly_writable"); test_skip("cxx20:input_iterator");
    test_skip("cxx20:output_iterator"); test_skip("cxx20:forward_iterator");
    test_skip("cxx20:bidirectional_iterator"); test_skip("cxx20:random_access_iterator");
    test_skip("cxx20:contiguous_iterator"); test_skip("cxx20:sentinel_for");
    test_skip("cxx20:sized_sentinel_for"); test_skip("cxx20:indirectly_movable");
    test_skip("cxx20:indirectly_movable_storable"); test_skip("cxx20:indirectly_copyable");
    test_skip("cxx20:indirectly_copyable_storable"); test_skip("cxx20:indirectly_swappable");
    test_skip("cxx20:indirectly_comparable"); test_skip("cxx20:indirectly_unary_invocable");
    test_skip("cxx20:indirectly_regular_unary_invocable"); test_skip("cxx20:indirect_unary_predicate");
    test_skip("cxx20:indirect_binary_predicate"); test_skip("cxx20:indirect_equivalence_relation");
    test_skip("cxx20:indirect_strict_weak_order"); test_skip("cxx20:mergeable");
    test_skip("cxx20:permutable"); test_skip("cxx20:sortable");
}
TEST_CASE(iterator, cxx20_iterator_aliases) {
    test_skip("cxx20:iter_value_t"); test_skip("cxx20:iter_reference_t");
    test_skip("cxx20:iter_difference_t"); test_skip("cxx20:iter_rvalue_reference_t");
    test_skip("cxx20:iter_common_reference_t"); test_skip("cxx20:indirect_result_t");
    test_skip("cxx20:projected"); test_skip("cxx20:common_iterator");
    test_skip("cxx20:counted_iterator"); test_skip("cxx20:move_sentinel");
}
#endif

TEST_CASE(iterator, feature_test_macros) {
#if defined(__cpp_lib_nonmember_container_access)
    test_true( __cpp_lib_nonmember_container_access > 0L );
    test_pass("cxx17:__cpp_lib_nonmember_container_access");
#else
    test_skip("cxx17:__cpp_lib_nonmember_container_access");
#endif
#if defined(__cpp_lib_constexpr_iterator)
    test_true( __cpp_lib_constexpr_iterator > 0L );
    test_pass("cxx20:__cpp_lib_constexpr_iterator");
#else
    test_skip("cxx20:__cpp_lib_constexpr_iterator");
#endif
#if defined(__cpp_lib_ssize)
    test_true( __cpp_lib_ssize > 0L );
    test_pass("cxx20:__cpp_lib_ssize");
#else
    test_skip("cxx20:__cpp_lib_ssize");
#endif
#if defined(__cpp_lib_ranges_as_const) && (__cpp_lib_ranges_as_const) >= 202207L
    test_true( (__cpp_lib_ranges_as_const) >= 202207L );
    test_pass("cxx23:__cpp_lib_ranges_as_const=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_as_const=202207L");
#endif
#if defined(__cpp_lib_ranges_as_const) && (__cpp_lib_ranges_as_const) >= 202311L
    test_true( (__cpp_lib_ranges_as_const) >= 202311L );
    test_pass("cxx23:__cpp_lib_ranges_as_const=202311L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_ranges_as_const=202311L");
#endif
#if defined(__cpp_lib_move_iterator_concept)
    test_true( __cpp_lib_move_iterator_concept > 0L );
    test_pass("cxx23:__cpp_lib_move_iterator_concept");
#else
    test_skip("cxx23:__cpp_lib_move_iterator_concept");
#endif
#if defined(__cpp_lib_freestanding_iterator)
    test_true( __cpp_lib_freestanding_iterator > 0L );
    test_pass("cxx26:__cpp_lib_freestanding_iterator");
#else
    test_skip("cxx26:__cpp_lib_freestanding_iterator");
#endif
}

static int g_it_live = 0;

struct ItLive {
    char* p_;
    ItLive() : p_(new char[32]) { p_[0] = 0; ++g_it_live; }
    ItLive(char c) : p_(new char[32]) { p_[0] = c; p_[1] = 0; ++g_it_live; }
    ItLive(const ItLive& o) : p_(new char[32]) {
        for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        ++g_it_live;
    }
    ItLive& operator=(const ItLive& o) {
        if (this != &o) for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~ItLive() { delete[] p_; --g_it_live; }
    char tag() const { return p_[0]; }
    bool operator==(const ItLive& o) const { return p_[0] == o.p_[0]; }
    bool operator< (const ItLive& o) const { return p_[0] <  o.p_[0]; }
};

TEST_CASE(iterator, class_elements_lifetime) {
    g_it_live = 0;
    {
        STD::vector<ItLive> v;
        STD::back_insert_iterator<STD::vector<ItLive> > bi(STD::back_inserter(v));
        for (int i = 0; i < 4; ++i) *bi++ = ItLive((char)('a' + i));
        test_eq( (int)v.size(), 4 );
        test_eq( g_it_live, 4 );
        test_eq( v[0].tag(), 'a' );
        test_eq( v[3].tag(), 'd' );

        STD::list<ItLive> l;
        STD::front_insert_iterator<STD::list<ItLive> > fi(STD::front_inserter(l));
        for (int i = 0; i < 3; ++i) *fi++ = ItLive((char)('x' + i));
        test_eq( (int)l.size(), 3 );
        test_eq( g_it_live, 7 );
        test_eq( l.front().tag(), 'z' );

        STD::insert_iterator<STD::vector<ItLive> > ii(STD::inserter(v, v.begin()));
        *ii++ = ItLive('0');
        test_eq( (int)v.size(), 5 );
        test_eq( g_it_live, 8 );
        test_eq( v[0].tag(), '0' );

        STD::vector<ItLive>::reverse_iterator r = v.rbegin();
        test_eq( r->tag(), 'd' );
        test_eq( (int)STD::distance(v.rbegin(), v.rend()), 5 );
        STD::vector<ItLive>::iterator a = v.begin();
        STD::advance(a, 2);
        test_eq( a->tag(), 'b' );

        for (STD::vector<ItLive>::iterator ci = v.begin(); ci != v.end(); ++ci) l.push_back(*ci);
        test_eq( (int)l.size(), 8 );
        test_eq( g_it_live, 13 );
    }
    test_eq( g_it_live, 0 );
}
