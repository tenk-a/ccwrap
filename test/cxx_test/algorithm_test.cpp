#include "test_cxx.hpp"

#include <algorithm>
#include <vector>
#include <functional>
#include <string>

struct IsEven { bool operator()(int x) const { return (x % 2) == 0; } };
struct IsOdd  { bool operator()(int x) const { return (x % 2) != 0; } };
struct Neg    { int  operator()(int x) const { return -x; } };
struct Eq3    { bool operator()(int x) const { return x == 3; } };
struct Eq2    { bool operator()(int a, int b) const { return a == b; } };

static int g_alg_live = 0;

struct AlgVal {
    STD::string s_;
    AlgVal() { ++g_alg_live; }
    AlgVal(const char* s) : s_(s) { ++g_alg_live; }
    AlgVal(const AlgVal& o) : s_(o.s_) { ++g_alg_live; }
    AlgVal& operator=(const AlgVal& o) { s_ = o.s_; return *this; }
    ~AlgVal() { --g_alg_live; }
    bool operator< (const AlgVal& o) const { return s_ <  o.s_; }
    bool operator==(const AlgVal& o) const { return s_ == o.s_; }
};

struct AlgIsB { bool operator()(const AlgVal& v) const { return !v.s_.empty() && v.s_[0] == 'b'; } };
struct AlgUp  { AlgVal operator()(const AlgVal& v) const { AlgVal r(v); r.s_ += "!"; return r; } };

static STD::vector<AlgVal> alg_make(const char* cs) {
    STD::vector<AlgVal> v;
    for (const char* p = cs; *p; ++p) {
        STD::string t(1, *p);
        t += "_long_enough_to_heap";
        v.push_back(AlgVal(t.c_str()));
    }
    return v;
}

static STD::string alg_head(const STD::vector<AlgVal>& v, STD::size_t i) {
    return v[i].s_.substr(0, 1);
}

static STD::string alg_join(const STD::vector<AlgVal>& v, STD::size_t n) {
    STD::string r;
    for (STD::size_t i = 0; i < n && i < v.size(); ++i) r += alg_head(v, i);
    return r;
}

TEST_CASE(algorithm, cxx03_search_sort) {
    int a[] = { 5, 3, 1, 4, 2 };
    const int n = 5;

    STD::sort(a, a + n);
    test_eq( a[0], 1 ); test_eq( a[4], 5 );
    test_pass("cxx03:sort");
    test_true( STD::binary_search(a, a + n, 3) );
    test_pass("cxx03:binary_search");
    test_true( STD::lower_bound(a, a + n, 3) == a + 2 );
    test_pass("cxx03:lower_bound");
    test_true( STD::upper_bound(a, a + n, 3) == a + 3 );
    test_pass("cxx03:upper_bound");
    test_eq( *STD::find(a, a + n, 4), 4 );
    test_pass("cxx03:find");
    test_eq( *STD::find_if(a, a + n, Eq3()), 3 );
    test_pass("cxx03:find_if");
#if _tst_cplusplus >= 201103L
    struct Eq3Local { bool operator()(int x) const { return x == 3; } };
    test_eq( *STD::find_if(a, a + n, Eq3Local()), 3 );
#else
    TEST_SKIP1();
#endif
    test_eq( (int)STD::count(a, a + n, 3), 1 );
    test_pass("cxx03:count");
    test_eq( (int)STD::count_if(a, a + n, IsEven()), 2 );
    test_pass("cxx03:count_if");
    test_eq( *STD::min_element(a, a + n), 1 );
    test_pass("cxx03:min_element");
    test_eq( *STD::max_element(a, a + n), 5 );
    test_pass("cxx03:max_element");
    test_eq( STD::min(7, 3), 3 );
    test_pass("cxx03:min");
    test_eq( STD::max(7, 3), 7 );
    test_pass("cxx03:max");
}

TEST_CASE(algorithm, class_elements_cxx03) {
    g_alg_live = 0;
    {
        STD::vector<AlgVal> v = alg_make("ecadb");
        STD::sort(v.begin(), v.end());
        test_eq( alg_join(v, 5), STD::string("abcde") );

        AlgVal key = v[2];
        test_true( STD::binary_search(v.begin(), v.end(), key) );
        test_true( STD::lower_bound(v.begin(), v.end(), key) == v.begin() + 2 );
        test_true( STD::upper_bound(v.begin(), v.end(), key) == v.begin() + 3 );
        test_true( STD::find(v.begin(), v.end(), key) == v.begin() + 2 );
        test_eq( (int)STD::count(v.begin(), v.end(), key), 1 );
        test_true( STD::min_element(v.begin(), v.end()) == v.begin() );
        test_true( STD::max_element(v.begin(), v.end()) == v.begin() + 4 );
        test_eq( (int)STD::count_if(v.begin(), v.end(), AlgIsB()), 1 );

        STD::reverse(v.begin(), v.end());
        test_eq( alg_join(v, 5), STD::string("edcba") );

        STD::rotate(v.begin(), v.begin() + 2, v.end());
        test_eq( alg_join(v, 5), STD::string("cbaed") );

        STD::vector<AlgVal> out(5);
        STD::copy(v.begin(), v.end(), out.begin());
        test_eq( alg_join(out, 5), STD::string("cbaed") );
        STD::copy_backward(v.begin(), v.end(), out.end());
        test_eq( alg_join(out, 5), STD::string("cbaed") );

        STD::transform(v.begin(), v.end(), out.begin(), AlgUp());
        test_eq( out[0].s_.substr(out[0].s_.size() - 1), STD::string("!") );

        STD::replace(v.begin(), v.end(), v[0], AlgVal("z_long_enough_to_heap"));
        test_eq( alg_join(v, 5), STD::string("zbaed") );

        STD::vector<AlgVal> d = alg_make("aabcc");
        STD::vector<AlgVal>::iterator ue = STD::unique(d.begin(), d.end());
        test_eq( (int)(ue - d.begin()), 3 );
        test_eq( alg_join(d, 3), STD::string("abc") );

        STD::vector<AlgVal> w = alg_make("xy");
        STD::swap_ranges(w.begin(), w.end(), v.begin());
        test_eq( alg_join(v, 2), STD::string("xy") );

        STD::fill(v.begin(), v.end(), AlgVal("f_long_enough_to_heap"));
        test_eq( alg_join(v, 5), STD::string("fffff") );

        STD::vector<AlgVal> p = alg_make("badce");
        STD::stable_sort(p.begin(), p.end());
        test_eq( alg_join(p, 5), STD::string("abcde") );
        test_true( STD::equal(p.begin(), p.end(), p.begin()) );
    }
    test_eq( g_alg_live, 0 );
}

TEST_CASE(algorithm, cxx03_modify) {
    int a[] = { 1, 2, 3, 4, 5 };
    STD::vector<int> v(a, a + 5);

    STD::reverse(v.begin(), v.end());
    test_eq( v[0], 5 );
    test_pass("cxx03:reverse");
    STD::replace(v.begin(), v.end(), 3, 30);
    test_eq( v[2], 30 );
    test_pass("cxx03:replace");

    int out[5];
    STD::copy(a, a + 5, out);
    test_eq( out[4], 5 );
    test_pass("cxx03:copy");
    STD::transform(a, a + 5, out, Neg());
    test_eq( out[0], -1 );
    test_pass("cxx03:transform");

    STD::fill(v.begin(), v.end(), 0);
    test_eq( v[0], 0 );
    test_pass("cxx03:fill");
    int fn[3];
    STD::fill_n(fn, 3, 9);
    test_eq( fn[2], 9 );
    test_pass("cxx03:fill_n");

    int u[] = { 1, 1, 2, 3, 3 };
    int* ue = STD::unique(u, u + 5);
    test_eq( (int)(ue - u), 3 );
    test_pass("cxx03:unique");
    test_true( STD::equal(u, u + 3, a) );
    test_pass("cxx03:equal");
    STD::rotate(a, a + 2, a + 5);
    test_eq( a[0], 3 );
    test_pass("cxx03:rotate");
}

TEST_CASE(algorithm, cxx11) {
    int e[] = { 2, 4, 6, 8 };
    test_true(  STD::all_of(e, e + 4, IsEven()) );
    test_pass("cxx11:all_of");
    test_true(  STD::any_of(e, e + 4, IsEven()) );
    test_pass("cxx11:any_of");
    test_true( !STD::none_of(e, e + 4, IsEven()) );
    test_pass("cxx11:none_of");
    test_true(  STD::is_sorted(e, e + 4) );
    test_true(  STD::is_sorted(e, e + 4, STD::less<int>()) );
    test_pass("cxx11:is_sorted");
    test_eq( *STD::find_if_not(e, e + 4, IsOdd()), 2 );
    test_pass("cxx11:find_if_not");

    int out[4] = { 0, 0, 0, 0 };
    STD::copy_n(e, 4, out);
    test_eq( out[3], 8 );
    test_pass("cxx11:copy_n");

    STD::pair<int, int> mm = STD::minmax(9, 4);
    test_eq( mm.first, 4 ); test_eq( mm.second, 9 );
    test_pass("cxx11:minmax");
    STD::pair<const int*, const int*> me = STD::minmax_element(e, e + 4);
    test_eq( *me.first, 2 ); test_eq( *me.second, 8 );
    test_pass("cxx11:minmax_element");

    int s[] = { 2, 4, 6, 5 };
    test_true( STD::is_sorted_until(s, s + 4) == s + 3 );
    test_true( STD::is_sorted_until(s, s + 4, STD::less<int>()) == s + 3 );
    test_pass("cxx11:is_sorted_until");

    int part[] = { 2, 4, 1, 3 };
    test_true( STD::is_partitioned(part, part + 4, IsEven()) );
    test_pass("cxx11:is_partitioned");
    test_true( STD::partition_point(part, part + 4, IsEven()) == part + 2 );
    test_pass("cxx11:partition_point");
}

TEST_CASE(algorithm, cxx11_more) {
    int a[] = { 1, 2, 3, 4, 5, 6 };
    int out[6];
    int* e = STD::copy_if(a, a + 6, out, IsEven());
    test_eq( (int)(e - out), 3 );
    test_eq( out[0], 2 ); test_eq( out[2], 6 );
    test_pass("cxx11:copy_if");

    int t[3], f[3];
    STD::pair<int*, int*> pr = STD::partition_copy(a, a + 6, t, f, IsEven());
    test_eq( (int)(pr.first - t), 3 );
    test_eq( (int)(pr.second - f), 3 );
    test_eq( t[0], 2 ); test_eq( f[0], 1 );
    test_pass("cxx11:partition_copy");

    int p1[] = { 1, 2, 3, 4 };
    int p2[] = { 4, 3, 2, 1 };
    int p3[] = { 4, 3, 2, 9 };
    test_true(  STD::is_permutation(p1, p1 + 4, p2) );
    test_true( !STD::is_permutation(p1, p1 + 4, p3) );
    test_true(  STD::is_permutation(p1, p1 + 4, p2, Eq2()) );
    test_pass("cxx11:is_permutation");
#if _tst_cplusplus >= 201103L
    struct Eq { bool operator()(int a, int b) const { return a == b; } };
    test_true(  STD::is_permutation(p1, p1 + 4, p2, Eq()) );
#else
    TEST_SKIP1();
#endif

    int heap[] = { 9, 7, 8, 3, 4, 5 };
    test_true( STD::is_heap(heap, heap + 6) );
    test_true( STD::is_heap(heap, heap + 6, STD::less<int>()) );
    test_pass("cxx11:is_heap");
    test_true( STD::is_heap_until(heap, heap + 6) == heap + 6 );
    int bad[] = { 9, 7, 8, 3, 40, 5 };
    test_true( STD::is_heap_until(bad, bad + 6) == bad + 4 );
    test_pass("cxx11:is_heap_until");
    test_true( !STD::is_heap(bad, bad + 6) );
}

#if TEST_TARGET_CXX >= 2017
struct AddTo { int* s; void operator()(int x) const { *s += x; } };
TEST_CASE(algorithm, cxx17) {
    test_eq( STD::clamp(5, 0, 10), 5 );
    test_eq( STD::clamp(-3, 0, 10), 0 );
    test_eq( STD::clamp(42, 0, 10), 10 );
    test_eq( STD::clamp(5, 0, 10, STD::less<int>()), 5 );
    test_pass("cxx17:clamp");

    int a[] = { 1, 2, 3, 4, 5 };
    int sum = 0;
    AddTo adder = { &sum };
    STD::for_each_n(a, 3, adder);
    test_eq( sum, 6 );
    test_pass("cxx17:for_each_n");
}
#endif

#if TEST_TARGET_CXX >= 2020
TEST_CASE(algorithm, cxx20) {
    int a[] = { 1, 2, 3, 4, 5 };
    int* e = STD::shift_left(a, a + 5, 2);
    test_eq( a[0], 3 );
    test_true( e == a + 3 );
    test_pass("cxx20:shift_left");

    int b[] = { 1, 2, 3, 4, 5 };
    int* r = STD::shift_right(b, b + 5, 2);
    test_true( r == b + 2 );
    test_eq( b[2], 1 );
    test_pass("cxx20:shift_right");
}

#if TEST_TARGET_CXX >= 2011
TEST_CASE(algorithm, move_range) {

    int src[] = { 1, 2, 3, 4, 5 };
    int dst[5] = { 0, 0, 0, 0, 0 };
    int* e = STD::move(src, src + 5, dst);
    test_true( e == dst + 5 );
    test_eq( dst[0], 1 );
    test_eq( dst[4], 5 );
    test_pass("cxx11:move");

    int src2[] = { 10, 20, 30 };
    int dst2[3] = { 0, 0, 0 };
    int* b = STD::move_backward(src2, src2 + 3, dst2 + 3);
    test_true( b == dst2 );
    test_eq( dst2[0], 10 );
    test_eq( dst2[2], 30 );
    test_pass("cxx11:move_backward");

    int buf[5] = { 1, 2, 3, 0, 0 };
    STD::move_backward(buf, buf + 3, buf + 5);
    test_eq( buf[4], 3 );
    test_eq( buf[2], 1 );

    STD::vector<STD::string> vs;
    vs.push_back(STD::string("aa"));
    vs.push_back(STD::string("bb"));
    STD::vector<STD::string> vd(2);
    STD::move(vs.begin(), vs.end(), vd.begin());
    test_eq( vd[0], STD::string("aa") );
    test_eq( vd[1], STD::string("bb") );
}
#endif
#endif


TEST_CASE(algorithm, random_shuffle_until_cxx14) {
#if TEST_TARGET_CXX <= 2014 && !defined(__cpp_lib_erase_if)
    int a[5] = {1, 2, 3, 4, 5};
    STD::random_shuffle(a, a + 5);
    {
        int sum = 0, i;
        for (i = 0; i < 5; ++i) sum += a[i];
        test_eq( sum, 15 );
    }
    test_pass("cxx03:random_shuffle");
#else
    TEST_NOTE("random_shuffle was removed in C++17");
    test_skip("cxx03:random_shuffle");
#endif
}

TEST_CASE(algorithm, truncated_results_cxx26) {
#if defined(__cpp_lib_ranges_reverse_copy_truncated)
    {
        int src[3] = {1, 2, 3};
        int dst[2] = {0, 0};
        STD::ranges::reverse_copy_truncated_result<int*, int*> r =
            STD::ranges::reverse_copy_truncated(src, src + 3, dst, dst + 2);
        test_true( r.out == dst + 2 );
        test_eq( dst[0], 3 );
    }
    test_pass("cxx26:reverse_copy_truncated_result");
    {
        int src[3] = {1, 2, 3};
        int dst[2] = {0, 0};
        STD::ranges::rotate_copy_truncated_result<int*, int*> r =
            STD::ranges::rotate_copy_truncated(src, src + 1, src + 3, dst, dst + 2);
        test_true( r.out == dst + 2 );
    }
    test_pass("cxx26:rotate_copy_truncated_result");
#else
    TEST_NOTE("the truncated copy results are C++26 (P2905); not in this library");
    test_skip("cxx26:reverse_copy_truncated_result");
    test_skip("cxx26:rotate_copy_truncated_result");
#endif
}
