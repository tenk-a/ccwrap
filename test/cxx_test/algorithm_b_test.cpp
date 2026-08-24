#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <list>
#include <random>
#include <string>
#include <vector>

using _ccw::string_view;

namespace {

STD::string joinv(const STD::vector<int>& v) {
    STD::string s;
    for (STD::size_t i = 0; i < v.size(); ++i) {
        if (i) s += ',';
        s += STD::to_string(v[i]);
    }
    return s;
}

STD::string joinl(const STD::list<int>& v) {
    STD::string s;
    for (STD::list<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin()) s += ',';
        s += STD::to_string(*it);
    }
    return s;
}

STD::string joina(const int* p, STD::size_t n) {
    STD::string s;
    for (STD::size_t i = 0; i < n; ++i) {
        if (i) s += ',';
        s += STD::to_string(p[i]);
    }
    return s;
}

struct CountUp {
    int* c;
    explicit CountUp(int& r) : c(&r) {}
    int operator()() const { return ++(*c); }
};
struct CountPost {
    int* c;
    explicit CountPost(int& r) : c(&r) {}
    int operator()() const { return (*c)++; }
};
struct CountBy2 {
    int* c;
    explicit CountBy2(int& r) : c(&r) {}
    int operator()() const { return *c += 2; }
};
struct Square      { int  operator()(int x) const { return x * x; } };
struct StrSize     { int  operator()(const STD::string& s) const { return int(s.size()); } };
struct ToUpperChar { char operator()(char c) const { return char(c - 32); } };
struct SameParity  { bool operator()(int a, int b) const { return (a % 2) == (b % 2); } };
struct SameLength  { bool operator()(const STD::string& a, const STD::string& b) const {
                         return a.size() == b.size(); } };
struct IsOneChar   { bool operator()(const STD::string& x) const { return x.size() == 1; } };
struct LeOf {
    const STD::vector<int>* v;
    explicit LeOf(const STD::vector<int>& r) : v(&r) {}
    bool operator()(int x) const { return x <= (*v)[2]; }
};
struct GeOf {
    const STD::vector<int>* v;
    explicit GeOf(const STD::vector<int>& r) : v(&r) {}
    bool operator()(int x) const { return x >= (*v)[2]; }
};

bool is_even(int x) { return (x % 2) == 0; }
bool by_len(const STD::string& a, const STD::string& b) { return a.size() < b.size(); }

}

TEST_CASE(algorithm, copy_and_move) {
    int __src_a[] = {1, 2, 3, 4, 5};
    STD::vector<int> src(__src_a, __src_a + sizeof __src_a / sizeof *__src_a);

    STD::vector<int> dst(5, 0);
    STD::vector<int>::iterator e = STD::copy(src.begin(), src.end(), dst.begin());
    test_true( e == dst.end() );
    STD::string sd = joinv(dst);
    test_str_eq( sd, "1,2,3,4,5" );

    int __ls_a[] = {7, 8, 9};
    STD::list<int> ls(__ls_a, __ls_a + sizeof __ls_a / sizeof *__ls_a);
    int            arr[3] = {0, 0, 0};
    STD::copy(ls.begin(), ls.end(), arr);
    STD::string sa = joina(arr, 3);
    test_str_eq( sa, "7,8,9" );
    test_pass("cxx03:copy");

    int __ov_a[] = {1, 2, 3, 4, 5};
    STD::vector<int> ov(__ov_a, __ov_a + sizeof __ov_a / sizeof *__ov_a);
    STD::vector<int>::iterator cb =
        STD::copy_backward(ov.begin(), ov.begin() + 3, ov.end());
    test_true( cb == ov.begin() + 2 );
    STD::string so = joinv(ov);
    test_str_eq( so, "1,2,1,2,3" );
    test_pass("cxx03:copy_backward");

    STD::vector<int> ev;
    STD::copy_if(src.begin(), src.end(), STD::back_inserter(ev), is_even);
    STD::string se = joinv(ev);
    test_str_eq( se, "2,4" );
    test_pass("cxx11:copy_if");

    STD::vector<int> n3(3, 0);
    STD::vector<int>::iterator cn = STD::copy_n(src.begin(), 3, n3.begin());
    test_true( cn == n3.end() );
    STD::string sn = joinv(n3);
    test_str_eq( sn, "1,2,3" );
    STD::vector<int> zero(1, 42);
    STD::copy_n(src.begin(), 0, zero.begin());
    test_eq( zero[0], 42 );
    test_pass("cxx11:copy_n");

    STD::string __ms_a[] = {"alpha", "beta"};
    STD::vector<STD::string> ms(__ms_a, __ms_a + sizeof __ms_a / sizeof *__ms_a);
    STD::vector<STD::string> md(2);
    STD::vector<STD::string>::iterator me = STD::move(ms.begin(), ms.end(), md.begin());
    test_true( me == md.end() );
    test_str_eq( md[0], "alpha" );
    test_str_eq( md[1], "beta" );
    test_pass("cxx11:move");

    STD::string __mb_a[] = {"one", "two", "three"};
    STD::vector<STD::string> mb(__mb_a, __mb_a + sizeof __mb_a / sizeof *__mb_a);
    STD::vector<STD::string> mdst(3);
    STD::vector<STD::string>::iterator mbe =
        STD::move_backward(mb.begin(), mb.end(), mdst.end());
    test_true( mbe == mdst.begin() );
    test_str_eq( mdst[2], "three" );
    test_str_eq( mdst[0], "one" );
    test_pass("cxx11:move_backward");
}

TEST_CASE(algorithm, fill_and_generate) {
    STD::vector<int> v(4);
    STD::fill(v.begin(), v.end(), 7);
    STD::string s1 = joinv(v);
    test_str_eq( s1, "7,7,7,7" );

    int a[3];
    STD::fill(a, a + 3, -1);
    STD::string s2 = joina(a, 3);
    test_str_eq( s2, "-1,-1,-1" );

    STD::list<STD::string> ls(2);
    STD::fill(ls.begin(), ls.end(), STD::string("x"));
    test_str_eq( ls.front(), "x" );
    test_str_eq( ls.back(), "x" );
    test_pass("cxx03:fill");

    STD::vector<int> fn(5, 0);
#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::vector<int>::iterator fe = STD::fill_n(fn.begin(), 3, 9);
    test_true( fe == fn.begin() + 3 );
#else
    STD::fill_n(fn.begin(), 3, 9);
#endif
    STD::string s3 = joinv(fn);
    test_str_eq( s3, "9,9,9,0,0" );
    test_pass("cxx03:fill_n");

    int              counter = 0;
    STD::vector<int> g(4);
    STD::generate(g.begin(), g.end(), CountUp(counter));
    STD::string s4 = joinv(g);
    test_str_eq( s4, "1,2,3,4" );
    test_pass("cxx03:generate");

    counter = 10;
    STD::vector<int> gn(4, 0);
#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::vector<int>::iterator ge =
        STD::generate_n(gn.begin(), 2, CountPost(counter));
    test_true( ge == gn.begin() + 2 );
#else
    STD::generate_n(gn.begin(), 2, CountPost(counter));
#endif
    STD::string s5 = joinv(gn);
    test_str_eq( s5, "10,11,0,0" );
    test_pass("cxx03:generate_n");

    STD::list<int> gl(3);
    counter = 0;
    STD::generate(gl.begin(), gl.end(), CountBy2(counter));
    STD::string s6 = joinl(gl);
    test_str_eq( s6, "2,4,6" );
}

TEST_CASE(algorithm, transform_and_replace) {
    int __v_a[] = {1, 2, 3};
    STD::vector<int> v(__v_a, __v_a + sizeof __v_a / sizeof *__v_a);

    STD::vector<int> sq(3);
    STD::vector<int>::iterator te =
        STD::transform(v.begin(), v.end(), sq.begin(), Square());
    test_true( te == sq.end() );
    STD::string s1 = joinv(sq);
    test_str_eq( s1, "1,4,9" );

    int __w_a[] = {10, 20, 30};
    STD::vector<int> w(__w_a, __w_a + sizeof __w_a / sizeof *__w_a);
    STD::vector<int> sum(3);
    STD::transform(v.begin(), v.end(), w.begin(), sum.begin(), STD::plus<int>());
    STD::string s2 = joinv(sum);
    test_str_eq( s2, "11,22,33" );

    STD::string __names_a[] = {"a", "bbb", "cc"};
    STD::vector<STD::string> names(__names_a, __names_a + sizeof __names_a / sizeof *__names_a);
    STD::vector<int>         lens(3);
    STD::transform(names.begin(), names.end(), lens.begin(),
                   StrSize());
    STD::string s3 = joinv(lens);
    test_str_eq( s3, "1,3,2" );

    STD::string up = "abc";
    STD::transform(up.begin(), up.end(), up.begin(),
                   ToUpperChar());
    test_str_eq( up, "ABC" );
    test_pass("cxx03:transform");

    int __r_a[] = {1, 2, 1, 3};
    STD::vector<int> r(__r_a, __r_a + sizeof __r_a / sizeof *__r_a);
    STD::replace(r.begin(), r.end(), 1, 8);
    STD::string s4 = joinv(r);
    test_str_eq( s4, "8,2,8,3" );
    test_pass("cxx03:replace");

    int __rl_a[] = {1, 2, 3, 4};
    STD::list<int> rl(__rl_a, __rl_a + sizeof __rl_a / sizeof *__rl_a);
    STD::replace_if(rl.begin(), rl.end(), is_even, 0);
    STD::string s5 = joinl(rl);
    test_str_eq( s5, "1,0,3,0" );
    test_pass("cxx03:replace_if");

    STD::vector<int> rc(4);
    STD::vector<int>::iterator rce =
        STD::replace_copy(v.begin(), v.end(), rc.begin(), 2, 99);
    test_true( rce == rc.begin() + 3 );
    rc.pop_back();
    STD::string s6 = joinv(rc);
    test_str_eq( s6, "1,99,3" );
    test_pass("cxx03:replace_copy");

    STD::vector<int> rci;
    STD::replace_copy_if(v.begin(), v.end(), STD::back_inserter(rci), is_even, -1);
    STD::string s7 = joinv(rci);
    test_str_eq( s7, "1,-1,3" );
    test_pass("cxx03:replace_copy_if");
}

TEST_CASE(algorithm, remove_and_unique) {
    int __v_a[] = {1, 2, 1, 3, 1};
    STD::vector<int> v(__v_a, __v_a + sizeof __v_a / sizeof *__v_a);
    STD::vector<int>::iterator re = STD::remove(v.begin(), v.end(), 1);
    test_eq( re - v.begin(), 2 );
    v.erase(re, v.end());
    STD::string s1 = joinv(v);
    test_str_eq( s1, "2,3" );
    test_pass("cxx03:remove");

    int __l_a[] = {1, 2, 3, 4, 5, 6};
    STD::list<int> l(__l_a, __l_a + sizeof __l_a / sizeof *__l_a);
    STD::list<int>::iterator ri = STD::remove_if(l.begin(), l.end(), is_even);
    l.erase(ri, l.end());
    STD::string s2 = joinl(l);
    test_str_eq( s2, "1,3,5" );
    test_pass("cxx03:remove_if");

    STD::string __names_a[] = {"a", "skip", "b"};
    STD::vector<STD::string> names(__names_a, __names_a + sizeof __names_a / sizeof *__names_a);
    STD::vector<STD::string>::iterator rs =
        STD::remove(names.begin(), names.end(), STD::string("skip"));
    names.erase(rs, names.end());
    test_eq( int(names.size()), 2 );
    test_str_eq( names[1], "b" );

    int              src[] = {1, 0, 2, 0, 3};
    STD::vector<int> rc;
    STD::remove_copy(src, src + 5, STD::back_inserter(rc), 0);
    STD::string s3 = joinv(rc);
    test_str_eq( s3, "1,2,3" );
    test_pass("cxx03:remove_copy");

    STD::vector<int> rci;
    STD::remove_copy_if(src, src + 5, STD::back_inserter(rci), is_even);
    STD::string s4 = joinv(rci);
    test_str_eq( s4, "1,3" );
    test_pass("cxx03:remove_copy_if");

    int __u_a[] = {1, 1, 2, 2, 2, 3, 1};
    STD::vector<int> u(__u_a, __u_a + sizeof __u_a / sizeof *__u_a);
    STD::vector<int>::iterator ue = STD::unique(u.begin(), u.end());
    u.erase(ue, u.end());
    STD::string s5 = joinv(u);
    test_str_eq( s5, "1,2,3,1" );

    int __up_a[] = {1, 3, 2, 4, 5};
    STD::list<int> up(__up_a, __up_a + sizeof __up_a / sizeof *__up_a);
    STD::list<int>::iterator upe =
        STD::unique(up.begin(), up.end(),
                    SameParity());
    up.erase(upe, up.end());
    STD::string s6 = joinl(up);
    test_str_eq( s6, "1,2,5" );
    test_pass("cxx03:unique");

    int              dup[] = {5, 5, 6, 7, 7, 7};
    STD::vector<int> uc;
    STD::unique_copy(dup, dup + 6, STD::back_inserter(uc));
    STD::string s7 = joinv(uc);
    test_str_eq( s7, "5,6,7" );

    STD::string __words_a[] = {"aa", "bb", "c", "dd"};
    STD::vector<STD::string> words(__words_a, __words_a + sizeof __words_a / sizeof *__words_a);
    STD::vector<STD::string> uw;
    STD::unique_copy(words.begin(), words.end(), STD::back_inserter(uw),
                     SameLength());
    test_eq( int(uw.size()), 3 );
    test_str_eq( uw[0], "aa" );
    test_str_eq( uw[1], "c" );
    test_str_eq( uw[2], "dd" );
    test_pass("cxx03:unique_copy");
}

TEST_CASE(algorithm, reverse_rotate_shift) {
    int __v_a[] = {1, 2, 3, 4};
    STD::vector<int> v(__v_a, __v_a + sizeof __v_a / sizeof *__v_a);
    STD::reverse(v.begin(), v.end());
    STD::string s1 = joinv(v);
    test_str_eq( s1, "4,3,2,1" );

    STD::string __ls_a[] = {"a", "b", "c"};
    STD::list<STD::string> ls(__ls_a, __ls_a + sizeof __ls_a / sizeof *__ls_a);
    STD::reverse(ls.begin(), ls.end());
    test_str_eq( ls.front(), "c" );
    test_str_eq( ls.back(), "a" );
    test_pass("cxx03:reverse");

    int              a[] = {1, 2, 3};
    STD::vector<int> rc(3);
    STD::vector<int>::iterator rce = STD::reverse_copy(a, a + 3, rc.begin());
    test_true( rce == rc.end() );
    STD::string s2 = joinv(rc);
    test_str_eq( s2, "3,2,1" );
    test_pass("cxx03:reverse_copy");

    int __r_a[] = {1, 2, 3, 4, 5};
    STD::vector<int> r(__r_a, __r_a + sizeof __r_a / sizeof *__r_a);
#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::vector<int>::iterator rr = STD::rotate(r.begin(), r.begin() + 2, r.end());
    test_true( rr == r.begin() + 3 );
#else
    STD::rotate(r.begin(), r.begin() + 2, r.end());
#endif
    STD::string s3 = joinv(r);
    test_str_eq( s3, "3,4,5,1,2" );

    int __rl_a[] = {1, 2, 3};
    STD::list<int> rl(__rl_a, __rl_a + sizeof __rl_a / sizeof *__rl_a);
    STD::rotate(rl.begin(), ++rl.begin(), rl.end());
    STD::string s4 = joinl(rl);
    test_str_eq( s4, "2,3,1" );
    test_pass("cxx03:rotate");

    STD::vector<int> roc(5);
    int              b[] = {1, 2, 3, 4, 5};
    STD::vector<int>::iterator roe = STD::rotate_copy(b, b + 3, b + 5, roc.begin());
    test_true( roe == roc.end() );
    STD::string s5 = joinv(roc);
    test_str_eq( s5, "4,5,1,2,3" );
    test_pass("cxx03:rotate_copy");

#if TEST_TARGET_CXX >= 2020
    int __sl_a[] = {1, 2, 3, 4, 5};
    STD::vector<int> sl(__sl_a, __sl_a + sizeof __sl_a / sizeof *__sl_a);
    STD::vector<int>::iterator sle = STD::shift_left(sl.begin(), sl.end(), 2);
    test_true( sle == sl.begin() + 3 );
    test_eq( sl[0], 3 );
    test_eq( sl[1], 4 );
    test_eq( sl[2], 5 );
    int __sl0_a[] = {1, 2};
    STD::vector<int> sl0(__sl0_a, __sl0_a + sizeof __sl0_a / sizeof *__sl0_a);
    STD::vector<int>::iterator sl0e = STD::shift_left(sl0.begin(), sl0.end(), 0);
    test_true( sl0e == sl0.end() );
    STD::vector<int>::iterator slbig = STD::shift_left(sl0.begin(), sl0.end(), 9);
    test_true( slbig == sl0.begin() );
    test_pass("cxx20:shift_left");

    int __sr_a[] = {1, 2, 3, 4, 5};
    STD::vector<int> sr(__sr_a, __sr_a + sizeof __sr_a / sizeof *__sr_a);
    STD::vector<int>::iterator sre = STD::shift_right(sr.begin(), sr.end(), 2);
    test_true( sre == sr.begin() + 2 );
    test_eq( sr[2], 1 );
    test_eq( sr[3], 2 );
    test_eq( sr[4], 3 );
    test_pass("cxx20:shift_right");
#else
    TEST_NOTE("shift_left / shift_right are C++20");
    test_skip("cxx20:shift_left");
    test_skip("cxx20:shift_right");
#endif
}

TEST_CASE(algorithm, swap_family) {
    int a = 1, b = 2;
    STD::swap(a, b);
    test_eq( a, 2 );
    test_eq( b, 1 );

    STD::string s1 = "left", s2 = "right";
    STD::swap(s1, s2);
    test_str_eq( s1, "right" );
    test_str_eq( s2, "left" );

    int x[3] = {1, 2, 3};
    int y[3] = {7, 8, 9};
#if !defined(__WATCOMC__)
    STD::swap(x, y);
    STD::string sx = joina(x, 3);
    STD::string sy = joina(y, 3);
    test_str_eq( sx, "7,8,9" );
    test_str_eq( sy, "1,2,3" );

    STD::array<int, 2> ar1 = {1, 2};
    STD::array<int, 2> ar2 = {3, 4};
    STD::swap(ar1, ar2);
    test_eq( ar1[0], 3 );
    test_eq( ar2[0], 1 );
#else
    (void)x; (void)y;
    TEST_NOTE("swap(T (&)[N]) and array<T,N> need a bounded-array parameter, which "
              "Open Watcom cannot match (A12)");
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
    test_pass("cxx03:swap");

    int __v1_a[] = {1, 2, 3};
    STD::vector<int> v1(__v1_a, __v1_a + sizeof __v1_a / sizeof *__v1_a);
    int __l2_a[] = {7, 8, 9};
    STD::list<int> l2(__l2_a, __l2_a + sizeof __l2_a / sizeof *__l2_a);
    STD::list<int>::iterator sre = STD::swap_ranges(v1.begin(), v1.end(), l2.begin());
    test_true( sre == l2.end() );
    STD::string sv = joinv(v1);
    STD::string sl = joinl(l2);
    test_str_eq( sv, "7,8,9" );
    test_str_eq( sl, "1,2,3" );
    test_pass("cxx03:swap_ranges");

    STD::string __w_a[] = {"first", "second"};
    STD::vector<STD::string> w(__w_a, __w_a + sizeof __w_a / sizeof *__w_a);
    STD::iter_swap(w.begin(), w.begin() + 1);
    test_str_eq( w[0], "second" );
    test_str_eq( w[1], "first" );
    test_pass("cxx03:iter_swap");
}

TEST_CASE(algorithm, partition_family) {
    int __v_a[] = {1, 2, 3, 4, 5, 6};
    STD::vector<int> v(__v_a, __v_a + sizeof __v_a / sizeof *__v_a);
    STD::vector<int>::iterator p = STD::partition(v.begin(), v.end(), is_even);
    test_eq( p - v.begin(), 3 );
    test_true( STD::all_of(v.begin(), p, is_even) );
    test_true( STD::none_of(p, v.end(), is_even) );
    test_pass("cxx03:partition");

    int __s_a[] = {1, 2, 3, 4, 5, 6};
    STD::vector<int> s(__s_a, __s_a + sizeof __s_a / sizeof *__s_a);
    STD::vector<int>::iterator sp = STD::stable_partition(s.begin(), s.end(), is_even);
    test_eq( sp - s.begin(), 3 );
    STD::string ss = joinv(s);
    test_str_eq( ss, "2,4,6,1,3,5" );

    STD::string __ls_a[] = {"a", "bbb", "cc", "d"};
    STD::list<STD::string> ls(__ls_a, __ls_a + sizeof __ls_a / sizeof *__ls_a);
    STD::list<STD::string>::iterator lp =
        STD::stable_partition(ls.begin(), ls.end(),
                              IsOneChar());
    test_str_eq( ls.front(), "a" );
    test_str_eq( *lp, "bbb" );
    test_pass("cxx03:stable_partition");

    STD::vector<int> yes, no;
    STD::pair<STD::back_insert_iterator<STD::vector<int> >,
              STD::back_insert_iterator<STD::vector<int> > > pc =
        STD::partition_copy(v.begin(), v.end(),
                            STD::back_inserter(yes), STD::back_inserter(no), is_even);
    (void)pc;
    test_eq( int(yes.size()), 3 );
    test_eq( int(no.size()), 3 );
    test_true( STD::all_of(yes.begin(), yes.end(), is_even) );
    test_true( STD::none_of(no.begin(), no.end(), is_even) );
    test_pass("cxx11:partition_copy");
}

TEST_CASE(algorithm, sort_family) {
    int __v_a[] = {5, 1, 4, 2, 3};
    STD::vector<int> v(__v_a, __v_a + sizeof __v_a / sizeof *__v_a);
    STD::sort(v.begin(), v.end());
    STD::string s1 = joinv(v);
    test_str_eq( s1, "1,2,3,4,5" );

    STD::sort(v.begin(), v.end(), STD::greater<int>());
    STD::string s2 = joinv(v);
    test_str_eq( s2, "5,4,3,2,1" );

    int a[] = {3, 1, 2};
    STD::sort(a, a + 3);
    STD::string s3 = joina(a, 3);
    test_str_eq( s3, "1,2,3" );
    test_pass("cxx03:sort");

    STD::string __w_a[] = {"bb", "a", "cc", "d"};
    STD::vector<STD::string> w(__w_a, __w_a + sizeof __w_a / sizeof *__w_a);
    STD::stable_sort(w.begin(), w.end(), by_len);
    test_str_eq( w[0], "a" );
    test_str_eq( w[1], "d" );
    test_str_eq( w[2], "bb" );
    test_str_eq( w[3], "cc" );

    int __sv_a[] = {3, 1, 2};
    STD::vector<int> sv(__sv_a, __sv_a + sizeof __sv_a / sizeof *__sv_a);
    STD::stable_sort(sv.begin(), sv.end());
    STD::string s4 = joinv(sv);
    test_str_eq( s4, "1,2,3" );
    test_pass("cxx03:stable_sort");

    int __ps_a[] = {5, 1, 4, 2, 3};
    STD::vector<int> ps(__ps_a, __ps_a + sizeof __ps_a / sizeof *__ps_a);
    STD::partial_sort(ps.begin(), ps.begin() + 3, ps.end());
    test_eq( ps[0], 1 );
    test_eq( ps[1], 2 );
    test_eq( ps[2], 3 );

    int __psc_a[] = {5, 1, 4, 2, 3};
    STD::vector<int> psc(__psc_a, __psc_a + sizeof __psc_a / sizeof *__psc_a);
    STD::partial_sort(psc.begin(), psc.begin() + 2, psc.end(), STD::greater<int>());
    test_eq( psc[0], 5 );
    test_eq( psc[1], 4 );
    test_pass("cxx03:partial_sort");

    int __src_a[] = {9, 3, 7, 1, 5};
    STD::list<int> src(__src_a, __src_a + sizeof __src_a / sizeof *__src_a);
    STD::vector<int> out(3);
    STD::vector<int>::iterator pe =
        STD::partial_sort_copy(src.begin(), src.end(), out.begin(), out.end());
    test_true( pe == out.end() );
    STD::string s5 = joinv(out);
    test_str_eq( s5, "1,3,5" );

    STD::vector<int> outg(2);
    STD::partial_sort_copy(src.begin(), src.end(), outg.begin(), outg.end(),
                           STD::greater<int>());
    STD::string s6 = joinv(outg);
    test_str_eq( s6, "9,7" );
    test_pass("cxx03:partial_sort_copy");

    int __ne_a[] = {7, 3, 9, 1, 5};
    STD::vector<int> ne(__ne_a, __ne_a + sizeof __ne_a / sizeof *__ne_a);
    STD::nth_element(ne.begin(), ne.begin() + 2, ne.end());
    test_eq( ne[2], 5 );
    test_true( STD::all_of(ne.begin(), ne.begin() + 2,
                           LeOf(ne)) );
    test_true( STD::all_of(ne.begin() + 3, ne.end(),
                           GeOf(ne)) );

    int __neg_a[] = {7, 3, 9, 1, 5};
    STD::vector<int> neg(__neg_a, __neg_a + sizeof __neg_a / sizeof *__neg_a);
    STD::nth_element(neg.begin(), neg.begin() + 1, neg.end(), STD::greater<int>());
    test_eq( neg[1], 7 );
    test_pass("cxx03:nth_element");
}

TEST_CASE(algorithm, merge_family) {
    int __a_a[] = {1, 3, 5};
    STD::vector<int> a(__a_a, __a_a + sizeof __a_a / sizeof *__a_a);
    int __b_a[] = {2, 4, 6};
    STD::vector<int> b(__b_a, __b_a + sizeof __b_a / sizeof *__b_a);
    STD::vector<int> m(6);
    STD::vector<int>::iterator me =
        STD::merge(a.begin(), a.end(), b.begin(), b.end(), m.begin());
    test_true( me == m.end() );
    STD::string s1 = joinv(m);
    test_str_eq( s1, "1,2,3,4,5,6" );

    int __da_a[] = {5, 3, 1};
    STD::vector<int> da(__da_a, __da_a + sizeof __da_a / sizeof *__da_a);
    int __db_a[] = {6, 2};
    STD::vector<int> db(__db_a, __db_a + sizeof __db_a / sizeof *__db_a);
    STD::vector<int> dm;
    STD::merge(da.begin(), da.end(), db.begin(), db.end(), STD::back_inserter(dm),
               STD::greater<int>());
    STD::string s2 = joinv(dm);
    test_str_eq( s2, "6,5,3,2,1" );

    STD::string __la_a[] = {"a", "ccc"};
    STD::list<STD::string> la(__la_a, __la_a + sizeof __la_a / sizeof *__la_a);
    STD::string __lb_a[] = {"bb"};
    STD::list<STD::string> lb(__lb_a, __lb_a + sizeof __lb_a / sizeof *__lb_a);
    STD::vector<STD::string> lm;
    STD::merge(la.begin(), la.end(), lb.begin(), lb.end(), STD::back_inserter(lm), by_len);
    test_eq( int(lm.size()), 3 );
    test_str_eq( lm[0], "a" );
    test_str_eq( lm[1], "bb" );
    test_str_eq( lm[2], "ccc" );
    test_pass("cxx03:merge");

    int __im_a[] = {1, 4, 7, 2, 3, 9};
    STD::vector<int> im(__im_a, __im_a + sizeof __im_a / sizeof *__im_a);
    STD::inplace_merge(im.begin(), im.begin() + 3, im.end());
    STD::string s3 = joinv(im);
    test_str_eq( s3, "1,2,3,4,7,9" );

    int __il_a[] = {9, 5, 8, 2};
    STD::list<int> il(__il_a, __il_a + sizeof __il_a / sizeof *__il_a);
    STD::list<int>::iterator mid = il.begin();
    STD::advance(mid, 2);
    STD::inplace_merge(il.begin(), mid, il.end(), STD::greater<int>());
    STD::string s4 = joinl(il);
    test_str_eq( s4, "9,8,5,2" );
    test_pass("cxx03:inplace_merge");
}

TEST_CASE(algorithm, set_operations) {
    int __a_a[] = {1, 2, 3, 4};
    STD::vector<int> a(__a_a, __a_a + sizeof __a_a / sizeof *__a_a);
    int __b_a[] = {3, 4, 5};
    STD::vector<int> b(__b_a, __b_a + sizeof __b_a / sizeof *__b_a);

    STD::vector<int> u;
    STD::set_union(a.begin(), a.end(), b.begin(), b.end(), STD::back_inserter(u));
    STD::string s1 = joinv(u);
    test_str_eq( s1, "1,2,3,4,5" );

    STD::vector<int> i;
    STD::set_intersection(a.begin(), a.end(), b.begin(), b.end(), STD::back_inserter(i));
    STD::string s2 = joinv(i);
    test_str_eq( s2, "3,4" );

    STD::vector<int> d;
    STD::set_difference(a.begin(), a.end(), b.begin(), b.end(), STD::back_inserter(d));
    STD::string s3 = joinv(d);
    test_str_eq( s3, "1,2" );

    STD::vector<int> sd;
    STD::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                                  STD::back_inserter(sd));
    STD::string s4 = joinv(sd);
    test_str_eq( s4, "1,2,5" );

    int __da_a[] = {4, 3, 2, 1};
    STD::vector<int> da(__da_a, __da_a + sizeof __da_a / sizeof *__da_a);
    int __db_a[] = {5, 4, 3};
    STD::vector<int> db(__db_a, __db_a + sizeof __db_a / sizeof *__db_a);
    STD::vector<int> du, di, dd, dsd;
    STD::set_union(da.begin(), da.end(), db.begin(), db.end(),
                   STD::back_inserter(du), STD::greater<int>());
    STD::string s5 = joinv(du);
    test_str_eq( s5, "5,4,3,2,1" );
    STD::set_intersection(da.begin(), da.end(), db.begin(), db.end(),
                          STD::back_inserter(di), STD::greater<int>());
    STD::string s6 = joinv(di);
    test_str_eq( s6, "4,3" );
    test_pass("cxx03:set_intersection");
    STD::set_difference(da.begin(), da.end(), db.begin(), db.end(),
                        STD::back_inserter(dd), STD::greater<int>());
    STD::string s7 = joinv(dd);
    test_str_eq( s7, "2,1" );
    test_pass("cxx03:set_difference");
    STD::set_symmetric_difference(da.begin(), da.end(), db.begin(), db.end(),
                                  STD::back_inserter(dsd), STD::greater<int>());
    STD::string s8 = joinv(dsd);
    test_str_eq( s8, "5,2,1" );
    test_pass("cxx03:set_symmetric_difference");

    STD::string __la_a[] = {"apple", "pear"};
    STD::list<STD::string> la(__la_a, __la_a + sizeof __la_a / sizeof *__la_a);
    STD::string __lb_a[] = {"pear", "plum"};
    STD::list<STD::string> lb(__lb_a, __lb_a + sizeof __lb_a / sizeof *__lb_a);
    STD::vector<STD::string> lu;
    STD::set_union(la.begin(), la.end(), lb.begin(), lb.end(), STD::back_inserter(lu));
    test_eq( int(lu.size()), 3 );
    test_str_eq( lu[0], "apple" );
    test_str_eq( lu[2], "plum" );
    test_pass("cxx03:set_union");
}

TEST_CASE(algorithm, heap_operations) {
    int __v_a[] = {3, 1, 4, 1, 5, 9};
    STD::vector<int> v(__v_a, __v_a + sizeof __v_a / sizeof *__v_a);
    STD::make_heap(v.begin(), v.end());
    test_true( STD::is_heap(v.begin(), v.end()) );
    test_eq( v.front(), 9 );
    test_pass("cxx03:make_heap");

    v.push_back(11);
    STD::push_heap(v.begin(), v.end());
    test_true( STD::is_heap(v.begin(), v.end()) );
    test_eq( v.front(), 11 );
    test_pass("cxx03:push_heap");

    STD::pop_heap(v.begin(), v.end());
    test_eq( v.back(), 11 );
    v.pop_back();
    test_true( STD::is_heap(v.begin(), v.end()) );
    test_eq( v.front(), 9 );
    test_pass("cxx03:pop_heap");

    STD::sort_heap(v.begin(), v.end());
    STD::string s1 = joinv(v);
    test_str_eq( s1, "1,1,3,4,5,9" );
    test_pass("cxx03:sort_heap");

    int __m_a[] = {3, 1, 4, 1, 5};
    STD::vector<int> m(__m_a, __m_a + sizeof __m_a / sizeof *__m_a);
    STD::make_heap(m.begin(), m.end(), STD::greater<int>());
    test_true( STD::is_heap(m.begin(), m.end(), STD::greater<int>()) );
    test_eq( m.front(), 1 );

    m.push_back(0);
    STD::push_heap(m.begin(), m.end(), STD::greater<int>());
    test_eq( m.front(), 0 );
    test_true( STD::is_heap(m.begin(), m.end(), STD::greater<int>()) );

    STD::pop_heap(m.begin(), m.end(), STD::greater<int>());
    test_eq( m.back(), 0 );
    m.pop_back();
    test_eq( m.front(), 1 );

    STD::sort_heap(m.begin(), m.end(), STD::greater<int>());
    STD::string s2 = joinv(m);
    test_str_eq( s2, "5,4,3,1,1" );

    STD::string __w_a[] = {"a", "ccc", "bb"};
    STD::vector<STD::string> w(__w_a, __w_a + sizeof __w_a / sizeof *__w_a);
    STD::make_heap(w.begin(), w.end(), by_len);
    test_str_eq( w.front(), "ccc" );
    test_true( STD::is_heap(w.begin(), w.end(), by_len) );
    test_pass("cxx11:is_heap");
}

TEST_CASE(algorithm, permutations) {
    int __v_a[] = {1, 2, 3};
    STD::vector<int> v(__v_a, __v_a + sizeof __v_a / sizeof *__v_a);
    bool             ok = STD::next_permutation(v.begin(), v.end());
    test_true( ok );
    STD::string s1 = joinv(v);
    test_str_eq( s1, "1,3,2" );

    int __last_a[] = {3, 2, 1};
    STD::vector<int> last(__last_a, __last_a + sizeof __last_a / sizeof *__last_a);
    bool             wrapped = STD::next_permutation(last.begin(), last.end());
    test_true( !wrapped );
    STD::string s2 = joinv(last);
    test_str_eq( s2, "1,2,3" );

    int __c_a[] = {1, 2, 3};
    STD::vector<int> c(__c_a, __c_a + sizeof __c_a / sizeof *__c_a);
    int              n = 1;
    while (STD::next_permutation(c.begin(), c.end()))
        ++n;
    test_eq( n, 6 );
    test_pass("cxx03:next_permutation");

    int __p_a[] = {1, 2, 3};
    STD::vector<int> p(__p_a, __p_a + sizeof __p_a / sizeof *__p_a);
    bool             pok = STD::prev_permutation(p.begin(), p.end());
    test_true( !pok );
    STD::string s3 = joinv(p);
    test_str_eq( s3, "3,2,1" );

    int __p2_a[] = {2, 1, 3};
    STD::vector<int> p2(__p2_a, __p2_a + sizeof __p2_a / sizeof *__p2_a);
    bool             p2ok = STD::prev_permutation(p2.begin(), p2.end());
    test_true( p2ok );
    STD::string s4 = joinv(p2);
    test_str_eq( s4, "1,3,2" );
    test_pass("cxx03:prev_permutation");

    int __g_a[] = {3, 2, 1};
    STD::vector<int> g(__g_a, __g_a + sizeof __g_a / sizeof *__g_a);
    bool             gok = STD::next_permutation(g.begin(), g.end(), STD::greater<int>());
    test_true( gok );
    STD::string s5 = joinv(g);
    test_str_eq( s5, "3,1,2" );

    int __gp_a[] = {3, 1, 2};
    STD::vector<int> gp(__gp_a, __gp_a + sizeof __gp_a / sizeof *__gp_a);
    bool             gpok = STD::prev_permutation(gp.begin(), gp.end(), STD::greater<int>());
    test_true( gpok );
    STD::string s6 = joinv(gp);
    test_str_eq( s6, "3,2,1" );

    STD::string __ls_a[] = {"a", "b"};
    STD::list<STD::string> ls(__ls_a, __ls_a + sizeof __ls_a / sizeof *__ls_a);
    bool                   lok = STD::next_permutation(ls.begin(), ls.end());
    test_true( lok );
    test_str_eq( ls.front(), "b" );
}

TEST_CASE(algorithm, shuffle_and_sample) {
    int __orig_a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    STD::vector<int> orig(__orig_a, __orig_a + sizeof __orig_a / sizeof *__orig_a);
    STD::vector<int> v = orig;
    STD::mt19937     gen(12345u);

    STD::shuffle(v.begin(), v.end(), gen);
    test_eq( int(v.size()), 8 );
    test_pass("cxx11:shuffle");
#if _TST_HAS_CXX14_LIB_MEMBERS && !defined(__WATCOMC__)
    test_true( STD::is_permutation(v.begin(), v.end(), orig.begin(), orig.end()) );
    test_pass("cxx11:is_permutation");
#else
    TEST_SKIP1();
#endif

    STD::vector<int> out;
#if TEST_TARGET_CXX >= 2017
    STD::sample(orig.begin(), orig.end(), STD::back_inserter(out), 3, gen);
    test_eq( int(out.size()), 3 );
#else
    out.push_back(2); out.push_back(4); out.push_back(6);
    TEST_SKIP1();
#endif
    STD::sort(out.begin(), out.end());
    STD::vector<int>::iterator ue = STD::unique(out.begin(), out.end());
    test_true( ue == out.end() );
    test_true( STD::includes(orig.begin(), orig.end(), out.begin(), out.end()) );
    test_pass("cxx03:includes");

#if TEST_TARGET_CXX >= 2017
    STD::vector<int> all;
    STD::sample(orig.begin(), orig.end(), STD::back_inserter(all), 99, gen);
    test_eq( int(all.size()), 8 );
    STD::string sa = joinv(all);
    test_str_eq( sa, "1,2,3,4,5,6,7,8" );
    test_pass("cxx17:sample");

    int __ls_a[] = {10, 20, 30, 40};
    STD::list<int> ls(__ls_a, __ls_a + sizeof __ls_a / sizeof *__ls_a);
    STD::vector<int> lout(2);
    STD::vector<int>::iterator le =
        STD::sample(ls.begin(), ls.end(), lout.begin(), 2, gen);
    test_true( le == lout.end() );
    test_true( STD::includes(ls.begin(), ls.end(), lout.begin(), lout.end()) );
#else
    TEST_NOTE("sample is C++17");
    test_skip("cxx17:sample");
#endif
}
static int g_ab_live = 0;

struct AbLive {
    char* p_;
    AbLive() : p_(new char[16]) { p_[0] = 0; ++g_ab_live; }
    AbLive(char c) : p_(new char[16]) { p_[0] = c; p_[1] = 0; ++g_ab_live; }
    AbLive(const AbLive& o) : p_(new char[16]) {
        for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        ++g_ab_live;
    }
    AbLive& operator=(const AbLive& o) {
        if (this != &o) for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~AbLive() { delete[] p_; --g_ab_live; }
    char tag() const { return p_[0]; }
    bool operator< (const AbLive& o) const { return p_[0] <  o.p_[0]; }
    bool operator==(const AbLive& o) const { return p_[0] == o.p_[0]; }
};

struct AbIsB { bool operator()(const AbLive& v) const { return v.tag() == 'b'; } };

static STD::vector<AbLive> ab_make(const char* cs) {
    STD::vector<AbLive> v;
    for (const char* p = cs; *p; ++p) v.push_back(AbLive(*p));
    return v;
}

static STD::string ab_join(const STD::vector<AbLive>& v, STD::size_t n) {
    STD::string r;
    for (STD::size_t i = 0; i < n && i < v.size(); ++i) r += v[i].tag();
    return r;
}

TEST_CASE(algorithm, class_elements_mutating) {
    g_ab_live = 0;
    {
        STD::vector<AbLive> v = ab_make("abcbd");
        test_eq( g_ab_live, 5 );

        STD::vector<AbLive>::iterator re = STD::remove(v.begin(), v.end(), AbLive('b'));
        test_eq( (int)(re - v.begin()), 3 );
        test_eq( ab_join(v, 3), STD::string("acd") );
        test_eq( g_ab_live, 5 );
        v.erase(re, v.end());
        test_eq( g_ab_live, 3 );

        STD::vector<AbLive> w = ab_make("abcbd");
        STD::vector<AbLive>::iterator ri = STD::remove_if(w.begin(), w.end(), AbIsB());
        test_eq( (int)(ri - w.begin()), 3 );
        w.erase(ri, w.end());
        test_eq( ab_join(w, 3), STD::string("acd") );
        test_eq( g_ab_live, 6 );

        STD::vector<AbLive> d = ab_make("aabbc");
        STD::vector<AbLive>::iterator ue = STD::unique(d.begin(), d.end());
        test_eq( (int)(ue - d.begin()), 3 );
        test_eq( ab_join(d, 3), STD::string("abc") );
        test_eq( g_ab_live, 11 );

        STD::vector<AbLive> p = ab_make("badce");
        STD::vector<AbLive>::iterator pe = STD::stable_partition(p.begin(), p.end(), AbIsB());
        test_eq( (int)(pe - p.begin()), 1 );
        test_eq( p[0].tag(), 'b' );
        test_eq( ab_join(p, 5), STD::string("badce") );
        test_eq( g_ab_live, 16 );

        STD::vector<AbLive> s = ab_make("edcba");
        STD::sort(s.begin(), s.end());
        test_eq( ab_join(s, 5), STD::string("abcde") );
        STD::partial_sort(s.begin(), s.begin() + 2, s.end());
        test_eq( ab_join(s, 2), STD::string("ab") );
        STD::nth_element(s.begin(), s.begin() + 2, s.end());
        test_eq( s[2].tag(), 'c' );
        test_eq( g_ab_live, 21 );

        STD::vector<AbLive> m1 = ab_make("ace");
        STD::vector<AbLive> m2 = ab_make("bdf");
        STD::vector<AbLive> mo(6);
        STD::merge(m1.begin(), m1.end(), m2.begin(), m2.end(), mo.begin());
        test_eq( ab_join(mo, 6), STD::string("abcdef") );

        STD::vector<AbLive> uo(6);
        STD::vector<AbLive>::iterator ue2 =
            STD::set_union(m1.begin(), m1.end(), m2.begin(), m2.end(), uo.begin());
        test_eq( (int)(ue2 - uo.begin()), 6 );
        test_eq( ab_join(uo, 6), STD::string("abcdef") );

        STD::vector<AbLive> h = ab_make("cadbe");
        STD::make_heap(h.begin(), h.end());
        test_eq( h.front().tag(), 'e' );
        STD::pop_heap(h.begin(), h.end());
        test_eq( h.back().tag(), 'e' );
        h.pop_back();
        STD::push_heap(h.begin(), h.end());
        STD::sort_heap(h.begin(), h.end());
        test_eq( ab_join(h, 4), STD::string("abcd") );

        STD::vector<AbLive> pm = ab_make("abc");
        test_true( STD::next_permutation(pm.begin(), pm.end()) );
        test_eq( ab_join(pm, 3), STD::string("acb") );
        test_true( STD::prev_permutation(pm.begin(), pm.end()) );
        test_eq( ab_join(pm, 3), STD::string("abc") );

        STD::vector<AbLive> a1 = ab_make("xy");
        STD::vector<AbLive> a2 = ab_make("pq");
        STD::swap_ranges(a1.begin(), a1.end(), a2.begin());
        test_eq( ab_join(a1, 2), STD::string("pq") );
        test_eq( ab_join(a2, 2), STD::string("xy") );
        a1.swap(a2);
        test_eq( ab_join(a1, 2), STD::string("xy") );
    }
    test_eq( g_ab_live, 0 );
}

#else
TEST_CASE_SKIP(algorithm, copy_and_move)
TEST_CASE_SKIP(algorithm, fill_and_generate)
TEST_CASE_SKIP(algorithm, transform_and_replace)
TEST_CASE_SKIP(algorithm, remove_and_unique)
TEST_CASE_SKIP(algorithm, reverse_rotate_shift)
TEST_CASE_SKIP(algorithm, swap_family)
TEST_CASE_SKIP(algorithm, partition_family)
TEST_CASE_SKIP(algorithm, sort_family)
TEST_CASE_SKIP(algorithm, merge_family)
TEST_CASE_SKIP(algorithm, set_operations)
TEST_CASE_SKIP(algorithm, heap_operations)
TEST_CASE_SKIP(algorithm, permutations)
TEST_CASE_SKIP(algorithm, shuffle_and_sample)
TEST_CASE_SKIP(algorithm, class_elements_mutating)
#endif
