#include "test_cxx.hpp"

#if _tst_cplusplus >= 201103L
#include <algorithm>
#include <functional>
#include <forward_list>
#include <list>
#include <string>
#include <vector>

using _ccw::string_view;

namespace {

bool is_even(int x) { return (x % 2) == 0; }

bool ci_eq(char a, char b) {
    char la = (a >= 'A' && a <= 'Z') ? char(a + 32) : a;
    char lb = (b >= 'A' && b <= 'Z') ? char(b + 32) : b;
    return la == lb;
}

bool by_len(const STD::string& a, const STD::string& b) { return a.size() < b.size(); }

}

TEST_CASE(algorithm, all_any_none_of) {
    STD::vector<int>    v{2, 4, 6, 8};
    int                 a[] = {1, 3, 5};
    STD::list<int>      l{2, 3, 4};
    STD::forward_list<int> f{10, 20};

    test_true(  STD::all_of(v.begin(), v.end(), is_even) );
    test_true( !STD::all_of(l.begin(), l.end(), is_even) );
    test_true(  STD::all_of(f.begin(), f.end(), is_even) );
    test_true(  STD::all_of(a, a, is_even) );
    test_pass("cxx11:all_of");

    test_true(  STD::any_of(l.begin(), l.end(), is_even) );
    test_true( !STD::any_of(a, a + 3, is_even) );
    test_true(  STD::any_of(v.begin(), v.end(), [](int x) { return x == 6; }) );
    test_pass("cxx11:any_of");

    test_true(  STD::none_of(a, a + 3, is_even) );
    test_true( !STD::none_of(v.begin(), v.end(), is_even) );
    test_true(  STD::none_of(f.begin(), f.end(), [](int x) { return x > 100; }) );
    test_pass("cxx11:none_of");
}

TEST_CASE(algorithm, for_each_and_count) {
    STD::vector<int> v{1, 2, 3, 4, 5};

    int sum = 0;
    STD::for_each(v.begin(), v.end(), [&sum](int x) { sum += x; });
    test_eq( sum, 15 );

    struct Acc {
        int total = 0;
        void operator()(int x) { total += x; }
    };
    Acc acc = STD::for_each(v.begin(), v.end(), Acc());
    test_eq( acc.total, 15 );
    test_pass("cxx03:for_each");

#if TEST_TARGET_CXX >= 2017
    int n3 = 0;
    STD::vector<int>::iterator it = STD::for_each_n(v.begin(), 3, [&n3](int x) { n3 += x; });
    test_eq( n3, 6 );
    test_true( it == v.begin() + 3 );
    test_pass("cxx17:for_each_n");
#else
    TEST_NOTE("for_each_n is C++17");
    TEST_SKIP_N(2);
    test_skip("cxx17:for_each_n");
#endif

    STD::list<STD::string> ls{"a", "bb", "a", "ccc"};
    STD::string cat;
    STD::for_each(ls.begin(), ls.end(), [&cat](const STD::string& s) { cat += s; });
    test_str_eq( cat, "abbaccc" );

    int arr[] = {1, 2, 2, 3, 2};
    test_eq( STD::count(arr, arr + 5, 2), 3 );
    test_eq( STD::count(v.begin(), v.end(), 9), 0 );
    test_eq( STD::count(ls.begin(), ls.end(), STD::string("a")), 2 );
    test_pass("cxx03:count");

    test_eq( STD::count_if(v.begin(), v.end(), is_even), 2 );
    test_eq( STD::count_if(arr, arr + 5, [](int x) { return x > 1; }), 4 );
    test_eq( STD::count_if(ls.begin(), ls.end(),
                           [](const STD::string& s) { return s.size() > 1; }), 2 );
    test_pass("cxx03:count_if");
}

TEST_CASE(algorithm, find_family) {
    STD::vector<int>    v{1, 2, 3, 4, 5};
    STD::list<STD::string> ls{"red", "green", "blue"};
    int                 a[] = {5, 5, 7, 7, 9};

    test_true( STD::find(v.begin(), v.end(), 3) == v.begin() + 2 );
    test_true( STD::find(v.begin(), v.end(), 99) == v.end() );
    test_true( STD::find(a, a + 5, 7) == a + 2 );
    test_true( STD::find(ls.begin(), ls.end(), STD::string("blue")) != ls.end() );
    test_pass("cxx03:find");

    test_true( STD::find_if(v.begin(), v.end(), is_even) == v.begin() + 1 );
    test_true( STD::find_if(ls.begin(), ls.end(),
                            [](const STD::string& s) { return s.size() == 4; })
               != ls.end() );
    test_pass("cxx03:find_if");

    test_true( STD::find_if_not(v.begin(), v.end(), [](int x) { return x < 4; })
               == v.begin() + 3 );
    test_true( STD::find_if_not(a, a + 5, [](int) { return true; }) == a + 5 );
    test_pass("cxx11:find_if_not");

    test_true( STD::adjacent_find(a, a + 5) == a + 0 );
    test_true( STD::adjacent_find(v.begin(), v.end()) == v.end() );
    test_true( STD::adjacent_find(v.begin(), v.end(),
                                  [](int x, int y) { return y == x + 1; }) == v.begin() );
    STD::list<int> l{1, 4, 4, 8};
    test_true( STD::adjacent_find(l.begin(), l.end()) == ++l.begin() );
    test_pass("cxx03:adjacent_find");

    int hay[] = {1, 2, 3, 1, 2, 3, 4};
    int need[] = {1, 2, 3};
    test_true( STD::find_end(hay, hay + 7, need, need + 3) == hay + 3 );
    STD::string s = "abcABCabc";
    STD::string sub = "ABC";
    STD::string::iterator fe =
        STD::find_end(s.begin(), s.end(), sub.begin(), sub.end(), ci_eq);
    test_true( fe == s.begin() + 6 );
    test_true( STD::find_end(hay, hay + 7, need, need) == hay + 7 );
    test_pass("cxx03:find_end");

    int any[] = {9, 3};
    test_true( STD::find_first_of(hay, hay + 7, any, any + 2) == hay + 2 );
    test_true( STD::find_first_of(v.begin(), v.end(), any, any + 2,
                                  [](int x, int y) { return x == y; }) == v.begin() + 2 );
    test_true( STD::find_first_of(v.begin(), v.end(), any, any) == v.end() );
    test_pass("cxx03:find_first_of");
}

TEST_CASE(algorithm, search_family) {
    STD::vector<int> hay{1, 2, 3, 4, 5, 6};
    STD::vector<int> need{3, 4};

    test_true( STD::search(hay.begin(), hay.end(), need.begin(), need.end())
               == hay.begin() + 2 );
    test_true( STD::search(hay.begin(), hay.end(), need.begin(), need.begin())
               == hay.begin() );
    STD::vector<int> nomatch{4, 3};
    test_true( STD::search(hay.begin(), hay.end(), nomatch.begin(), nomatch.end())
               == hay.end() );

    STD::list<char> text{'H', 'e', 'l', 'l', 'o'};
    STD::string     pat = "LL";
    test_true( STD::search(text.begin(), text.end(), pat.begin(), pat.end(), ci_eq)
               != text.end() );
    test_pass("cxx03:search");

#if TEST_TARGET_CXX >= 2017
    STD::string  hs = "the quick brown fox";
    STD::string  ps = "brown";
    typedef STD::string::iterator SIt;
    STD::string::iterator d =
        STD::search(hs.begin(), hs.end(), STD::default_searcher<SIt>(ps.begin(), ps.end()));
    test_true( d == hs.begin() + 10 );
    test_pass("cxx17:default_searcher");
    STD::string::iterator bm =
        STD::search(hs.begin(), hs.end(), STD::boyer_moore_searcher<SIt>(ps.begin(), ps.end()));
    test_true( bm == hs.begin() + 10 );
    test_pass("cxx17:boyer_moore_searcher");
    STD::string::iterator bmh =
        STD::search(hs.begin(), hs.end(),
                    STD::boyer_moore_horspool_searcher<SIt>(ps.begin(), ps.end()));
    test_true( bmh == hs.begin() + 10 );
    test_pass("cxx17:boyer_moore_horspool_searcher");
#else
    TEST_NOTE("the searchers are C++17");
    test_skip("cxx17:default_searcher");
    test_skip("cxx17:boyer_moore_searcher");
    test_skip("cxx17:boyer_moore_horspool_searcher");
#endif

    int a[] = {1, 7, 7, 7, 2};
    test_true( STD::search_n(a, a + 5, 3, 7) == a + 1 );
    test_true( STD::search_n(a, a + 5, 4, 7) == a + 5 );
    test_true( STD::search_n(a, a + 5, 0, 7) == a + 0 );
    STD::string letters = "aaBBc";
    test_true( STD::search_n(letters.begin(), letters.end(), 2, 'b', ci_eq)
               == letters.begin() + 2 );
    test_pass("cxx03:search_n");
}

TEST_CASE(algorithm, mismatch_and_equal) {
    STD::vector<int> a{1, 2, 3, 4};
    STD::vector<int> b{1, 2, 9, 4};
    int              c[] = {1, 2, 3, 4};

    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> m1 =
        STD::mismatch(a.begin(), a.end(), b.begin());
    test_true( m1.first == a.begin() + 2 );
    test_eq( *m1.second, 9 );

    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> m2 =
        STD::mismatch(a.begin(), a.end(), b.begin(),
                      [](int x, int y) { return (x % 3) == (y % 3); });
    test_true( m2.first == a.end() );

    STD::pair<STD::vector<int>::iterator, int*> m3 =
        STD::mismatch(a.begin(), a.end(), c, [](int x, int y) { return x == y; });
    test_true( m3.first == a.end() );

    STD::vector<int> shorter{1, 2};
#if _TST_HAS_CXX14_LIB_MEMBERS
    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> m4 =
        STD::mismatch(a.begin(), a.end(), shorter.begin(), shorter.end());
    test_true( m4.first == a.begin() + 2 );
    test_true( m4.second == shorter.end() );
#else
    TEST_SKIP_N(2);
#endif
#if _TST_HAS_CXX14_LIB_MEMBERS
    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> m5 =
        STD::mismatch(a.begin(), a.end(), b.begin(), b.end(),
                      [](int x, int y) { return x == y; });
    test_true( m5.first == a.begin() + 2 );
    test_pass("cxx03:mismatch");
#else
    TEST_SKIP1();
    test_skip("cxx03:mismatch");
#endif

    test_true(  STD::equal(a.begin(), a.end(), c) );
    test_true( !STD::equal(a.begin(), a.end(), b.begin()) );
    STD::string s1 = "abc", s2 = "ABC";
    test_true( STD::equal(s1.begin(), s1.end(), s2.begin(), ci_eq) );
#if _TST_HAS_CXX14_LIB_MEMBERS
    test_true( STD::equal(a.begin(), a.end(), c, c + 4) );
    test_true( !STD::equal(a.begin(), a.end(), shorter.begin(), shorter.end()) );
    test_true( STD::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), ci_eq) );
#else
    TEST_SKIP_N(3);
#endif

    STD::list<int> la{1, 2, 3, 4};
#if _TST_HAS_CXX14_LIB_MEMBERS
    test_true( STD::equal(la.begin(), la.end(), c, c + 4) );
    test_pass("cxx03:equal");
#else
    TEST_SKIP1();
    test_skip("cxx03:equal");
#endif
}

TEST_CASE(algorithm, permutation_compare_includes) {
    STD::vector<int> a{1, 2, 3};
    STD::vector<int> b{3, 1, 2};
    STD::vector<int> d{1, 2, 4};

    test_true(  STD::is_permutation(a.begin(), a.end(), b.begin()) );
    test_true( !STD::is_permutation(a.begin(), a.end(), d.begin()) );
    test_true(  STD::is_permutation(a.begin(), a.end(), b.begin(),
                                    [](int x, int y) { return x == y; }) );

    STD::vector<int> longer{3, 1, 2, 5};
#if _TST_HAS_CXX14_LIB_MEMBERS
    test_true(  STD::is_permutation(a.begin(), a.end(), b.begin(), b.end()) );
    test_true( !STD::is_permutation(a.begin(), a.end(), longer.begin(), longer.end()) );
    test_true(  STD::is_permutation(a.begin(), a.end(), b.begin(), b.end(),
                                    [](int x, int y) { return x == y; }) );
#else
    TEST_SKIP_N(3);
#endif

    STD::list<STD::string> ls1{"x", "yy"};
    STD::list<STD::string> ls2{"yy", "x"};
#if _TST_HAS_CXX14_LIB_MEMBERS
    test_true( STD::is_permutation(ls1.begin(), ls1.end(), ls2.begin(), ls2.end()) );
    test_pass("cxx11:is_permutation");
#else
    TEST_SKIP1();
    test_skip("cxx11:is_permutation");
#endif

    int p[] = {1, 2, 3};
    int q[] = {1, 2, 4};
    test_true(  STD::lexicographical_compare(p, p + 3, q, q + 3) );
    test_true( !STD::lexicographical_compare(q, q + 3, p, p + 3) );
    test_true(  STD::lexicographical_compare(p, p + 2, p, p + 3) );
    test_true( !STD::lexicographical_compare(p, p + 3, p, p + 3) );
    STD::string x = "ABC", y = "abd";
    test_true( STD::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end(),
                                            [](char l, char r) {
                                                char ll = (l >= 'A' && l <= 'Z') ? char(l + 32) : l;
                                                char rr = (r >= 'A' && r <= 'Z') ? char(r + 32) : r;
                                                return ll < rr;
                                            }) );
    STD::vector<STD::string> sv1{"aa", "b"};
    STD::vector<STD::string> sv2{"z", "b"};

    test_true(  STD::lexicographical_compare(sv2.begin(), sv2.end(), sv1.begin(), sv1.end(),
                                             by_len) );
    test_true( !STD::lexicographical_compare(sv1.begin(), sv1.end(), sv2.begin(), sv2.end(),
                                             by_len) );
    test_pass("cxx03:lexicographical_compare");

    STD::vector<int> super{1, 2, 3, 4, 5};
    STD::vector<int> sub{2, 4};
    test_true(  STD::includes(super.begin(), super.end(), sub.begin(), sub.end()) );
    STD::vector<int> nosub{2, 9};
    test_true( !STD::includes(super.begin(), super.end(), nosub.begin(), nosub.end()) );
    test_true(  STD::includes(super.begin(), super.end(), sub.begin(), sub.begin()) );

    STD::vector<int> dsuper{5, 4, 3, 2, 1};
    STD::vector<int> dsub{4, 2};
    test_true( STD::includes(dsuper.begin(), dsuper.end(), dsub.begin(), dsub.end(),
                             STD::greater<int>()) );
    test_pass("cxx03:includes");
}

TEST_CASE(algorithm, min_max_scalar) {
    test_eq( STD::min(3, 7), 3 );
    test_eq( STD::max(3, 7), 7 );
    test_eq( STD::min(STD::string("pear"), STD::string("apple")), STD::string("apple") );

    const STD::string lo = "aaa", hi = "z";
    test_str_eq( STD::min(lo, hi, by_len), "z" );
    test_str_eq( STD::max(lo, hi, by_len), "aaa" );
    test_eq( STD::min(3, 7, STD::greater<int>()), 7 );
    test_eq( STD::max(3, 7, STD::greater<int>()), 3 );

    test_eq( STD::min({4, 1, 7, 2}), 1 );
    test_eq( STD::max({4, 1, 7, 2}), 7 );
    test_eq( STD::min({4, 1, 7, 2}, STD::greater<int>()), 7 );
    test_pass("cxx03:min");
    test_eq( STD::max({4, 1, 7, 2}, STD::greater<int>()), 1 );
    test_pass("cxx03:max");

    STD::pair<int, int> mm = STD::minmax(9, 2);
    test_eq( mm.first, 2 );
    test_eq( mm.second, 9 );
    STD::pair<int, int> mmc = STD::minmax(9, 2, STD::greater<int>());
    test_eq( mmc.first, 9 );
    test_eq( mmc.second, 2 );
    STD::pair<int, int> mml = STD::minmax({5, 3, 8, 1});
    test_eq( mml.first, 1 );
    test_eq( mml.second, 8 );
    STD::pair<int, int> mmlc = STD::minmax({5, 3, 8, 1}, STD::greater<int>());
    test_eq( mmlc.first, 8 );
    test_eq( mmlc.second, 1 );
    test_pass("cxx11:minmax");

#if TEST_TARGET_CXX >= 2017
    test_eq( STD::clamp(5, 1, 10), 5 );
    test_eq( STD::clamp(-1, 1, 10), 1 );
    test_eq( STD::clamp(99, 1, 10), 10 );
    test_eq( STD::clamp(5, 10, 1, STD::greater<int>()), 5 );
    test_eq( STD::clamp(99, 10, 1, STD::greater<int>()), 10 );
    test_str_eq( STD::clamp(STD::string("mm"), STD::string("aa"), STD::string("zz")), "mm" );
    test_pass("cxx17:clamp");
#else
    TEST_NOTE("clamp is C++17");
    test_skip("cxx17:clamp");
#endif
}

TEST_CASE(algorithm, min_max_element) {
    STD::vector<int>       v{4, 1, 9, 3};
    STD::list<STD::string> ls{"pear", "fig", "banana"};
    int                    e[] = {1};

    test_true( STD::min_element(v.begin(), v.end()) == v.begin() + 1 );
    test_true( STD::max_element(v.begin(), v.end()) == v.begin() + 2 );
    test_true( STD::min_element(v.begin(), v.begin()) == v.begin() );
    test_true( STD::min_element(e, e + 1) == e );

    test_true( STD::min_element(v.begin(), v.end(), STD::greater<int>()) == v.begin() + 2 );
    test_true( STD::max_element(v.begin(), v.end(), STD::greater<int>()) == v.begin() + 1 );

    STD::list<STD::string>::iterator sm = STD::min_element(ls.begin(), ls.end(), by_len);
    test_str_eq( *sm, "fig" );
    STD::list<STD::string>::iterator sx = STD::max_element(ls.begin(), ls.end(), by_len);
    test_str_eq( *sx, "banana" );
    test_pass("cxx03:max_element");
    STD::list<STD::string>::iterator lex = STD::min_element(ls.begin(), ls.end());
    test_str_eq( *lex, "banana" );
    test_pass("cxx03:min_element");

    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> mm =
        STD::minmax_element(v.begin(), v.end());
    test_true( mm.first == v.begin() + 1 );
    test_true( mm.second == v.begin() + 2 );
    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> mmc =
        STD::minmax_element(v.begin(), v.end(), STD::greater<int>());
    test_true( mmc.first == v.begin() + 2 );
    test_true( mmc.second == v.begin() + 1 );
    STD::pair<STD::list<STD::string>::iterator, STD::list<STD::string>::iterator> mms =
        STD::minmax_element(ls.begin(), ls.end(), by_len);
    test_str_eq( *mms.first, "fig" );
    test_str_eq( *mms.second, "banana" );
    test_pass("cxx11:minmax_element");
}

TEST_CASE(algorithm, sorted_partition_heap_queries) {
    STD::vector<int> s{1, 2, 3, 4};
    STD::vector<int> u{1, 3, 2};
    STD::list<int>   ds{9, 5, 1};

    test_true(  STD::is_sorted(s.begin(), s.end()) );
    test_true( !STD::is_sorted(u.begin(), u.end()) );
    test_true(  STD::is_sorted(ds.begin(), ds.end(), STD::greater<int>()) );
    test_true( !STD::is_sorted(s.begin(), s.end(), STD::greater<int>()) );
    test_pass("cxx11:is_sorted");

    test_true( STD::is_sorted_until(s.begin(), s.end()) == s.end() );
    test_true( STD::is_sorted_until(u.begin(), u.end()) == u.begin() + 2 );
    test_true( STD::is_sorted_until(ds.begin(), ds.end(), STD::greater<int>()) == ds.end() );
    STD::vector<int> du{9, 5, 7};
    test_true( STD::is_sorted_until(du.begin(), du.end(), STD::greater<int>())
               == du.begin() + 2 );
    test_pass("cxx11:is_sorted_until");

    STD::vector<int> p{2, 4, 1, 3};
    test_true(  STD::is_partitioned(p.begin(), p.end(), is_even) );
    test_true( !STD::is_partitioned(u.begin(), u.end(), is_even) );
    STD::vector<int> np{2, 1, 4};
    test_true( !STD::is_partitioned(np.begin(), np.end(), is_even) );
    test_pass("cxx11:is_partitioned");
    test_true( STD::partition_point(p.begin(), p.end(), is_even) == p.begin() + 2 );
    STD::vector<int> lt{1, 2, 3, 9, 9};
    test_true( STD::partition_point(lt.begin(), lt.end(),
                                    [](int x) { return x < 5; }) == lt.begin() + 3 );
    test_pass("cxx11:partition_point");

    STD::vector<int> h{9, 5, 4, 1, 3};
    test_true( STD::is_heap(h.begin(), h.end()) );
    test_true( STD::is_heap_until(h.begin(), h.end()) == h.end() );
    STD::vector<int> nh{1, 2, 3};
    test_true( !STD::is_heap(nh.begin(), nh.end()) );
    test_true( STD::is_heap_until(nh.begin(), nh.end()) == nh.begin() + 1 );

    STD::vector<int> mh{1, 3, 4, 9};
    test_true( STD::is_heap(mh.begin(), mh.end(), STD::greater<int>()) );
    test_true( STD::is_heap_until(mh.begin(), mh.end(), STD::greater<int>()) == mh.end() );
    test_true( !STD::is_heap(h.begin(), h.end(), STD::greater<int>()) );
    test_pass("cxx11:is_heap");
    test_true( STD::is_heap_until(h.begin(), h.end(), STD::greater<int>()) == h.begin() + 1 );
    test_pass("cxx11:is_heap_until");
}

TEST_CASE(algorithm, binary_search_family) {
    STD::vector<int>       v{1, 3, 3, 3, 5, 7};
    STD::list<STD::string> ls{"a", "bb", "ccc"};
    int                    d[] = {9, 7, 5, 3, 1};

    test_true( STD::lower_bound(v.begin(), v.end(), 3) == v.begin() + 1 );
    test_true( STD::upper_bound(v.begin(), v.end(), 3) == v.begin() + 4 );
    test_true( STD::lower_bound(v.begin(), v.end(), 4) == v.begin() + 4 );
    test_true( STD::upper_bound(v.begin(), v.end(), 0) == v.begin() );

    test_true( STD::lower_bound(d, d + 5, 5, STD::greater<int>()) == d + 2 );
    test_true( STD::upper_bound(d, d + 5, 5, STD::greater<int>()) == d + 3 );

    STD::list<STD::string>::iterator lb =
        STD::lower_bound(ls.begin(), ls.end(), STD::string("xx"), by_len);
    test_str_eq( *lb, "bb" );
    test_pass("cxx03:lower_bound");
    STD::list<STD::string>::iterator ub =
        STD::upper_bound(ls.begin(), ls.end(), STD::string("xx"), by_len);
    test_str_eq( *ub, "ccc" );
    test_pass("cxx03:upper_bound");

    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> er =
        STD::equal_range(v.begin(), v.end(), 3);
    test_true( er.first == v.begin() + 1 );
    test_true( er.second == v.begin() + 4 );
    test_eq( er.second - er.first, 3 );
    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> er0 =
        STD::equal_range(v.begin(), v.end(), 4);
    test_true( er0.first == er0.second );
    STD::pair<int*, int*> erd = STD::equal_range(d, d + 5, 7, STD::greater<int>());
    test_eq( erd.second - erd.first, 1 );
    test_eq( *erd.first, 7 );
    test_pass("cxx03:equal_range");

    test_true(  STD::binary_search(v.begin(), v.end(), 5) );
    test_true( !STD::binary_search(v.begin(), v.end(), 4) );
    test_true(  STD::binary_search(d, d + 5, 9, STD::greater<int>()) );
    test_true( !STD::binary_search(d, d + 5, 8, STD::greater<int>()) );
    test_true(  STD::binary_search(ls.begin(), ls.end(), STD::string("zz"), by_len) );
    test_pass("cxx03:binary_search");
}
#else
TEST_CASE_SKIP(algorithm, all_any_none_of)
TEST_CASE_SKIP(algorithm, for_each_and_count)
TEST_CASE_SKIP(algorithm, find_family)
TEST_CASE_SKIP(algorithm, search_family)
TEST_CASE_SKIP(algorithm, mismatch_and_equal)
TEST_CASE_SKIP(algorithm, permutation_compare_includes)
TEST_CASE_SKIP(algorithm, min_max_scalar)
TEST_CASE_SKIP(algorithm, min_max_element)
TEST_CASE_SKIP(algorithm, sorted_partition_heap_queries)
TEST_CASE_SKIP(algorithm, binary_search_family)
#endif
