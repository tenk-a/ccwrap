#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020 && _tst_cplusplus >= 201103L
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <list>
#include <random>
#include <string>
#include <vector>

using _ccw::string_view;

namespace rng = STD::ranges;

namespace {

struct Rec {
    int         id;
    STD::string name;
};

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

bool is_even(int x) { return (x % 2) == 0; }

STD::vector<Rec> recs() {
    return STD::vector<Rec>{ {3, "cee"}, {1, "a"}, {2, "bb"} };
}

}

TEST_CASE(algorithm, ranges_all_any_none_for_each) {
    STD::vector<int> v{2, 4, 6};
    STD::list<int>   l{1, 2, 3};
    STD::vector<Rec> r = recs();

    test_true(  rng::all_of(v, is_even) );
    test_true(  rng::all_of(v.begin(), v.end(), is_even) );
    test_true( !rng::all_of(l, is_even) );
    test_true(  rng::all_of(r, [](int id) { return id > 0; }, &Rec::id) );
    test_pass("cxx20:ranges::all_of");

    test_true(  rng::any_of(l, is_even) );
    test_true( !rng::any_of(l.begin(), l.end(), [](int x) { return x > 9; }) );
    test_true(  rng::any_of(r, [](const STD::string& s) { return s.size() == 3; }, &Rec::name) );
    test_pass("cxx20:ranges::any_of");

    test_true(  rng::none_of(v, [](int x) { return x % 2; }) );
    test_true( !rng::none_of(l, is_even) );
    test_true(  rng::none_of(r, [](int id) { return id > 9; }, &Rec::id) );
    test_pass("cxx20:ranges::none_of");

    int sum = 0;
    rng::for_each_result<STD::vector<int>::iterator, STD::function<void(int)> > fr =
        rng::for_each(v, STD::function<void(int)>([&sum](int x) { sum += x; }));
    test_eq( sum, 12 );
    test_true( fr.in == v.end() );

    int idsum = 0;
    auto fr2 = rng::for_each(r, [&idsum](int id) { idsum += id; }, &Rec::id);
    test_eq( idsum, 6 );
    test_true( fr2.in == r.end() );

    int isum = 0;
    auto fr3 = rng::for_each(l.begin(), l.end(), [&isum](int x) { isum += x; });
    test_eq( isum, 6 );
    test_true( fr3.in == l.end() );
    test_pass("cxx20:ranges::for_each");

    int n2 = 0;
    auto fn = rng::for_each_n(v.begin(), 2, [&n2](int x) { n2 += x; });
    test_eq( n2, 6 );
    test_true( fn.in == v.begin() + 2 );

    int pn = 0;
    auto fn2 = rng::for_each_n(r.begin(), 2, [&pn](int id) { pn += id; }, &Rec::id);
    test_eq( pn, 4 );
    test_true( fn2.in == r.begin() + 2 );
    test_pass("cxx20:ranges::for_each_n");
}

TEST_CASE(algorithm, ranges_find_family) {
    STD::vector<int> v{1, 2, 3, 2, 5};
    STD::list<int>   l{4, 5, 6};
    STD::vector<Rec> r = recs();

    test_true( rng::find(v, 3) == v.begin() + 2 );
    test_true( rng::find(v.begin(), v.end(), 9) == v.end() );
    test_true( rng::find(l, 5) == ++l.begin() );
    test_true( rng::find(r, 2, &Rec::id) == r.begin() + 2 );
    test_pass("cxx20:ranges::find");

    test_true( rng::find_if(v, is_even) == v.begin() + 1 );
    test_true( rng::find_if(r, [](const STD::string& s) { return s.size() == 1; },
                            &Rec::name) == r.begin() + 1 );
    test_pass("cxx20:ranges::find_if");
    test_true( rng::find_if_not(v, [](int x) { return x < 3; }) == v.begin() + 2 );
    test_true( rng::find_if_not(l.begin(), l.end(), [](int) { return true; }) == l.end() );
    test_pass("cxx20:ranges::find_if_not");

    STD::vector<int> a{1, 1, 2, 3};
    test_true( rng::adjacent_find(a) == a.begin() );
    test_true( rng::adjacent_find(v) == v.end() );
    test_true( rng::adjacent_find(v, [](int x, int y) { return y == x + 1; })
               == v.begin() );
    test_true( rng::adjacent_find(v.begin(), v.end(),
                                  [](int x, int y) { return x == y; }) == v.end() );
    test_pass("cxx20:ranges::adjacent_find");

    STD::vector<int> hay{1, 2, 3, 1, 2, 3};
    STD::vector<int> pat{1, 2};
    rng::subrange<STD::vector<int>::iterator> fe = rng::find_end(hay, pat);
    test_true( fe.begin() == hay.begin() + 3 );
    test_eq( fe.end() - fe.begin(), 2 );
    auto fe2 = rng::find_end(hay.begin(), hay.end(), pat.begin(), pat.end(),
                             STD::equal_to<int>());
    test_true( fe2.begin() == hay.begin() + 3 );
    test_pass("cxx20:ranges::find_end");

    STD::vector<int> any{5, 3};
    test_true( rng::find_first_of(hay, any) == hay.begin() + 2 );
    test_true( rng::find_first_of(hay.begin(), hay.end(), any.begin(), any.end(),
                                  STD::equal_to<int>()) == hay.begin() + 2 );
    STD::vector<int> ids{2};
    test_true( rng::find_first_of(r, ids, STD::equal_to<int>(), &Rec::id)
               == r.begin() + 2 );
    test_pass("cxx20:ranges::find_first_of");

#if !defined(__GNUC__)
    rng::subrange<STD::vector<int>::iterator> fl = rng::find_last(v, 2);
    test_true( fl.begin() == v.begin() + 3 );
    test_eq( fl.end() - fl.begin(), 2 );
    auto fl2 = rng::find_last(v.begin(), v.end(), 9);
    test_true( fl2.begin() == v.end() );
    auto flp = rng::find_last(r, 1, &Rec::id);
    test_true( flp.begin() == r.begin() + 1 );
    test_pass("cxx23:ranges::find_last");
    auto fli = rng::find_last_if(v, is_even);
    test_true( fli.begin() == v.begin() + 3 );
    test_pass("cxx23:ranges::find_last_if");
    auto flin = rng::find_last_if_not(v, is_even);
    test_true( flin.begin() == v.begin() + 4 );
    test_pass("cxx23:ranges::find_last_if_not");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
}

TEST_CASE(algorithm, ranges_count_mismatch_equal) {
    STD::vector<int> v{1, 2, 2, 3};
    STD::list<int>   l{2, 2, 2};
    STD::vector<Rec> r = recs();

    test_eq( rng::count(v, 2), 2 );
    test_eq( rng::count(v.begin(), v.end(), 9), 0 );
    test_eq( rng::count(l, 2), 3 );
    test_eq( rng::count(r, 2, &Rec::id), 1 );
    test_pass("cxx20:ranges::count");

    test_eq( rng::count_if(v, is_even), 2 );
    test_eq( rng::count_if(v.begin(), v.end(), [](int x) { return x > 1; }), 3 );
    test_eq( rng::count_if(r, [](const STD::string& s) { return s.size() > 1; },
                           &Rec::name), 2 );
    test_pass("cxx20:ranges::count_if");

    STD::vector<int> a{1, 2, 3};
    STD::vector<int> b{1, 2, 9};
    rng::mismatch_result<STD::vector<int>::iterator, STD::vector<int>::iterator> mr =
        rng::mismatch(a, b);
    test_true( mr.in1 == a.begin() + 2 );
    test_true( mr.in2 == b.begin() + 2 );
    auto mr2 = rng::mismatch(a.begin(), a.end(), b.begin(), b.end(),
                             STD::equal_to<int>());
    test_true( mr2.in1 == a.begin() + 2 );
    STD::vector<int> mods{1, 2, 3};
    auto mr3 = rng::mismatch(a, mods, STD::equal_to<int>(),
                             STD::identity(), STD::identity());
    test_true( mr3.in1 == a.end() );
    test_pass("cxx20:ranges::mismatch");

    test_true(  rng::equal(a, a) );
    test_true( !rng::equal(a, b) );
    test_true(  rng::equal(a.begin(), a.end(), a.begin(), a.end(), STD::equal_to<int>()) );
    STD::vector<int> want{3, 1, 2};
    test_true(  rng::equal(r, want, STD::equal_to<int>(), &Rec::id) );
    STD::vector<int> shorter{1, 2};
    test_true( !rng::equal(a, shorter) );
    test_pass("cxx20:ranges::equal");

    STD::vector<int> p{3, 2, 1};
    test_true(  rng::is_permutation(a, p) );
    test_true( !rng::is_permutation(a, b) );
    test_true(  rng::is_permutation(a.begin(), a.end(), p.begin(), p.end(),
                                    STD::equal_to<int>()) );
    STD::vector<int> pids{1, 2, 3};
    test_true(  rng::is_permutation(r, pids, STD::equal_to<int>(), &Rec::id) );
    test_pass("cxx20:ranges::is_permutation");
}

TEST_CASE(algorithm, ranges_search_family) {
    STD::vector<int> hay{1, 2, 3, 4, 5};
    STD::vector<int> pat{3, 4};

    rng::subrange<STD::vector<int>::iterator> s = rng::search(hay, pat);
    test_true( s.begin() == hay.begin() + 2 );
    test_eq( s.end() - s.begin(), 2 );
    auto s2 = rng::search(hay.begin(), hay.end(), pat.begin(), pat.end(),
                          STD::equal_to<int>());
    test_true( s2.begin() == hay.begin() + 2 );
    STD::vector<int> nope{4, 3};
    auto s3 = rng::search(hay, nope);
    test_true( s3.begin() == hay.end() );

    STD::vector<Rec> r = recs();
    STD::vector<int> ids{1, 2};
    auto sp = rng::search(r, ids, STD::equal_to<int>(), &Rec::id);
    test_true( sp.begin() == r.begin() + 1 );
    test_pass("cxx20:ranges::search");

    STD::vector<int> rep{1, 7, 7, 7, 2};
    rng::subrange<STD::vector<int>::iterator> sn = rng::search_n(rep, 3, 7);
    test_true( sn.begin() == rep.begin() + 1 );
    test_eq( sn.end() - sn.begin(), 3 );
    auto sn2 = rng::search_n(rep.begin(), rep.end(), 4, 7);
    test_true( sn2.begin() == rep.end() );
    auto sn3 = rng::search_n(rep, 2, 7, STD::equal_to<int>());
    test_true( sn3.begin() == rep.begin() + 1 );
    test_pass("cxx20:ranges::search_n");

#if !defined(__GNUC__)
    test_true(  rng::contains(hay, 4) );
    test_true( !rng::contains(hay, 9) );
    test_true(  rng::contains(hay.begin(), hay.end(), 1) );
    test_true(  rng::contains(r, 2, &Rec::id) );
    test_pass("cxx23:ranges::contains");

    test_true(  rng::contains_subrange(hay, pat) );
    test_true( !rng::contains_subrange(hay, nope) );
    test_true(  rng::contains_subrange(hay.begin(), hay.end(), pat.begin(), pat.end()) );
    test_pass("cxx23:ranges::contains_subrange");

    STD::vector<int> pre{1, 2};
    STD::vector<int> suf{4, 5};
    test_true(  rng::starts_with(hay, pre) );
    test_true( !rng::starts_with(hay, suf) );
    test_true(  rng::starts_with(hay.begin(), hay.end(), pre.begin(), pre.end()) );
    test_pass("cxx23:ranges::starts_with");
    test_true(  rng::ends_with(hay, suf) );
    test_true( !rng::ends_with(hay, pre) );
    test_true(  rng::ends_with(hay.begin(), hay.end(), suf.begin(), suf.end()) );
    test_pass("cxx23:ranges::ends_with");
#else
    for (int __i = 0; __i < 13; ++__i) TEST_SKIP1();
#endif
}

TEST_CASE(algorithm, ranges_copy_move_swap) {
    STD::vector<int> src{1, 2, 3, 4};

    STD::vector<int> dst(4);
    rng::copy_result<STD::vector<int>::iterator, STD::vector<int>::iterator> cr =
        rng::copy(src, dst.begin());
    test_true( cr.in == src.end() );
    test_true( cr.out == dst.end() );
    STD::string s1 = joinv(dst);
    test_str_eq( s1, "1,2,3,4" );

    STD::list<int> ldst(4);
    auto cr2 = rng::copy(src.begin(), src.end(), ldst.begin());
    test_true( cr2.out == ldst.end() );
    STD::string s2 = joinl(ldst);
    test_str_eq( s2, "1,2,3,4" );
    test_pass("cxx20:ranges::copy");

    STD::vector<int> ev;
    auto ci = rng::copy_if(src, STD::back_inserter(ev), is_even);
    test_true( ci.in == src.end() );
    STD::string s3 = joinv(ev);
    test_str_eq( s3, "2,4" );

    STD::vector<Rec> r = recs();
    STD::vector<Rec> big;
    auto cip = rng::copy_if(r, STD::back_inserter(big),
                            [](int id) { return id >= 2; }, &Rec::id);
    test_true( cip.in == r.end() );
    test_eq( int(big.size()), 2 );
    test_pass("cxx20:ranges::copy_if");

    STD::vector<int> n2(2);
    auto cn = rng::copy_n(src.begin(), 2, n2.begin());
    test_true( cn.out == n2.end() );
    STD::string s4 = joinv(n2);
    test_str_eq( s4, "1,2" );
    test_pass("cxx20:ranges::copy_n");

    STD::vector<int> cb(6, 0);
    rng::copy_backward_result<STD::vector<int>::iterator, STD::vector<int>::iterator> cbr =
        rng::copy_backward(src, cb.end());
    test_true( cbr.out == cb.begin() + 2 );
    STD::string s5 = joinv(cb);
    test_str_eq( s5, "0,0,1,2,3,4" );
    auto cbr2 = rng::copy_backward(src.begin(), src.end(), cb.end());
    test_true( cbr2.in == src.end() );
    test_pass("cxx20:ranges::copy_backward");

    STD::vector<STD::string> ms{"aa", "bb"};
    STD::vector<STD::string> md(2);
    rng::move_result<STD::vector<STD::string>::iterator,
                     STD::vector<STD::string>::iterator> mr = rng::move(ms, md.begin());
    test_true( mr.out == md.end() );
    test_str_eq( md[0], "aa" );
    test_pass("cxx20:ranges::move");

    STD::vector<STD::string> mb{"x", "y"};
    STD::vector<STD::string> mbd(3);
    rng::move_backward_result<STD::vector<STD::string>::iterator,
                              STD::vector<STD::string>::iterator> mbr =
        rng::move_backward(mb, mbd.end());
    test_true( mbr.out == mbd.begin() + 1 );
    test_str_eq( mbd[2], "y" );
    test_str_eq( mbd[1], "x" );
    test_pass("cxx20:ranges::move_backward");

    STD::vector<int> a{1, 2, 3};
    STD::list<int>   b{7, 8, 9};
    auto sr = rng::swap_ranges(a, b);
    test_true( sr.in1 == a.end() );
    test_true( sr.in2 == b.end() );
    STD::string s6 = joinv(a);
    STD::string s7 = joinl(b);
    test_str_eq( s6, "7,8,9" );
    test_str_eq( s7, "1,2,3" );
    auto sr2 = rng::swap_ranges(a.begin(), a.end(), b.begin(), b.end());
    test_true( sr2.in1 == a.end() );
    STD::string s8 = joinv(a);
    test_str_eq( s8, "1,2,3" );
    test_pass("cxx20:ranges::swap_ranges");
}

TEST_CASE(algorithm, ranges_transform_replace_fill) {
    STD::vector<int> v{1, 2, 3};

    STD::vector<int> sq(3);
    rng::unary_transform_result<STD::vector<int>::iterator, STD::vector<int>::iterator> ut =
        rng::transform(v, sq.begin(), [](int x) { return x * x; });
    test_true( ut.in == v.end() );
    test_true( ut.out == sq.end() );
    STD::string s1 = joinv(sq);
    test_str_eq( s1, "1,4,9" );

    STD::vector<Rec> r = recs();
    STD::vector<int> dbl(3);
    auto utp = rng::transform(r, dbl.begin(), [](int id) { return id * 10; }, &Rec::id);
    test_true( utp.out == dbl.end() );
    STD::string s2 = joinv(dbl);
    test_str_eq( s2, "30,10,20" );

    STD::vector<int> w{10, 20, 30};
    STD::vector<int> sum(3);
    rng::binary_transform_result<STD::vector<int>::iterator, STD::vector<int>::iterator,
                                 STD::vector<int>::iterator> bt =
        rng::transform(v, w, sum.begin(), STD::plus<int>());
    test_true( bt.in1 == v.end() );
    test_true( bt.in2 == w.end() );
    STD::string s3 = joinv(sum);
    test_str_eq( s3, "11,22,33" );
    STD::vector<int> sum2(3);
    auto bt2 = rng::transform(v.begin(), v.end(), w.begin(), w.end(), sum2.begin(),
                              STD::plus<int>());
    test_true( bt2.out == sum2.end() );
    test_pass("cxx20:ranges::transform");

    STD::vector<int> rp{1, 2, 1};
    STD::vector<int>::iterator re = rng::replace(rp, 1, 8);
    test_true( re == rp.end() );
    STD::string s4 = joinv(rp);
    test_str_eq( s4, "8,2,8" );
    test_pass("cxx20:ranges::replace");

    STD::list<int> rl{1, 2, 3, 4};
    rng::replace_if(rl, is_even, 0);
    STD::string s5 = joinl(rl);
    test_str_eq( s5, "1,0,3,0" );

    STD::vector<Rec> rr = recs();
    rng::replace_if(rr, [](int id) { return id == 1; }, Rec{9, "nine"}, &Rec::id);
    test_eq( rr[1].id, 9 );
    test_pass("cxx20:ranges::replace_if");

    STD::vector<int> rc(3);
    rng::replace_copy_result<STD::vector<int>::iterator, STD::vector<int>::iterator> rcr =
        rng::replace_copy(v, rc.begin(), 2, 99);
    test_true( rcr.in == v.end() );
    STD::string s6 = joinv(rc);
    test_str_eq( s6, "1,99,3" );
    test_pass("cxx20:ranges::replace_copy");

    STD::vector<int> rci(3);
    rng::replace_copy_if_result<STD::vector<int>::iterator, STD::vector<int>::iterator> rcir =
        rng::replace_copy_if(v.begin(), v.end(), rci.begin(), is_even, -1);
    test_true( rcir.out == rci.end() );
    STD::string s7 = joinv(rci);
    test_str_eq( s7, "1,-1,3" );
    test_pass("cxx20:ranges::replace_copy_if");

    STD::vector<int> f(3);
    STD::vector<int>::iterator fe = rng::fill(f, 7);
    test_true( fe == f.end() );
    STD::string s8 = joinv(f);
    test_str_eq( s8, "7,7,7" );
    rng::fill(f.begin(), f.end(), 5);
    STD::string s9 = joinv(f);
    test_str_eq( s9, "5,5,5" );
    test_pass("cxx20:ranges::fill");

    STD::vector<int> fn(4, 0);
    STD::vector<int>::iterator fne = rng::fill_n(fn.begin(), 2, 3);
    test_true( fne == fn.begin() + 2 );
    STD::string s10 = joinv(fn);
    test_str_eq( s10, "3,3,0,0" );
    test_pass("cxx20:ranges::fill_n");

    int              c = 0;
    STD::vector<int> g(3);
    STD::vector<int>::iterator ge = rng::generate(g, [&c]() { return ++c; });
    test_true( ge == g.end() );
    STD::string s11 = joinv(g);
    test_str_eq( s11, "1,2,3" );
    c = 0;
    STD::list<int> gl(3);
    rng::generate(gl.begin(), gl.end(), [&c]() { return c += 2; });
    STD::string s12 = joinl(gl);
    test_str_eq( s12, "2,4,6" );
    test_pass("cxx20:ranges::generate");

    c = 10;
    STD::vector<int> gn(3, 0);
    STD::vector<int>::iterator gne = rng::generate_n(gn.begin(), 2, [&c]() { return c++; });
    test_true( gne == gn.begin() + 2 );
    STD::string s13 = joinv(gn);
    test_str_eq( s13, "10,11,0" );
    test_pass("cxx20:ranges::generate_n");
}

TEST_CASE(algorithm, ranges_remove_unique_reverse_rotate) {

    STD::vector<int> v{1, 2, 1, 3};
    rng::subrange<STD::vector<int>::iterator> rem = rng::remove(v, 1);
    test_eq( rem.begin() - v.begin(), 2 );
    v.erase(rem.begin(), rem.end());
    STD::string s1 = joinv(v);
    test_str_eq( s1, "2,3" );
    test_pass("cxx20:ranges::remove");

    STD::list<int> l{1, 2, 3, 4};
    auto reml = rng::remove_if(l, is_even);
    l.erase(reml.begin(), reml.end());
    STD::string s2 = joinl(l);
    test_str_eq( s2, "1,3" );

    STD::vector<Rec> r = recs();
    auto remr = rng::remove_if(r, [](int id) { return id == 1; }, &Rec::id);
    r.erase(remr.begin(), remr.end());
    test_eq( int(r.size()), 2 );
    test_pass("cxx20:ranges::remove_if");

    STD::vector<int> src{1, 0, 2, 0};
    STD::vector<int> rc;
    rng::remove_copy_result<STD::vector<int>::iterator,
                            STD::back_insert_iterator<STD::vector<int> > > rcr =
        rng::remove_copy(src, STD::back_inserter(rc), 0);
    test_true( rcr.in == src.end() );
    STD::string s3 = joinv(rc);
    test_str_eq( s3, "1,2" );
    test_pass("cxx20:ranges::remove_copy");

    STD::vector<int> rci;
    auto rcir = rng::remove_copy_if(src.begin(), src.end(), STD::back_inserter(rci),
                                    is_even);
    test_true( rcir.in == src.end() );
    STD::string s4 = joinv(rci);
    test_str_eq( s4, "1" );
    test_pass("cxx20:ranges::remove_copy_if");

    STD::vector<int> u{1, 1, 2, 2, 3};
    auto ur = rng::unique(u);
    u.erase(ur.begin(), ur.end());
    STD::string s5 = joinv(u);
    test_str_eq( s5, "1,2,3" );

    STD::vector<int> up{1, 3, 2, 4};
    auto upr = rng::unique(up.begin(), up.end(),
                           [](int a, int b) { return (a % 2) == (b % 2); });
    up.erase(upr.begin(), upr.end());
    STD::string s6 = joinv(up);
    test_str_eq( s6, "1,2" );
    test_pass("cxx20:ranges::unique");

    STD::vector<int> dup{5, 5, 6};
    STD::vector<int> uc;
    rng::unique_copy_result<STD::vector<int>::iterator,
                            STD::back_insert_iterator<STD::vector<int> > > ucr =
        rng::unique_copy(dup, STD::back_inserter(uc));
    test_true( ucr.in == dup.end() );
    STD::string s7 = joinv(uc);
    test_str_eq( s7, "5,6" );
    test_pass("cxx20:ranges::unique_copy");

    STD::vector<int> rv{1, 2, 3};
    STD::vector<int>::iterator rve = rng::reverse(rv);
    test_true( rve == rv.end() );
    STD::string s8 = joinv(rv);
    test_str_eq( s8, "3,2,1" );
    rng::reverse(rv.begin(), rv.end());
    STD::string s9 = joinv(rv);
    test_str_eq( s9, "1,2,3" );
    test_pass("cxx20:ranges::reverse");

    STD::vector<int> rcp(3);
    rng::reverse_copy_result<STD::vector<int>::iterator, STD::vector<int>::iterator> rcpr =
        rng::reverse_copy(rv, rcp.begin());
    test_true( rcpr.out == rcp.end() );
    STD::string s10 = joinv(rcp);
    test_str_eq( s10, "3,2,1" );
    test_pass("cxx20:ranges::reverse_copy");

    STD::vector<int> ro{1, 2, 3, 4, 5};
    rng::subrange<STD::vector<int>::iterator> ror = rng::rotate(ro, ro.begin() + 2);
    test_true( ror.begin() == ro.begin() + 3 );
    STD::string s11 = joinv(ro);
    test_str_eq( s11, "3,4,5,1,2" );

    STD::list<int> rl{1, 2, 3};
    rng::rotate(rl.begin(), ++rl.begin(), rl.end());
    STD::string s12 = joinl(rl);
    test_str_eq( s12, "2,3,1" );
    test_pass("cxx20:ranges::rotate");

    STD::vector<int> ri{1, 2, 3, 4};
    STD::vector<int> rout(4);
    rng::rotate_copy_result<STD::vector<int>::iterator, STD::vector<int>::iterator> rcr2 =
        rng::rotate_copy(ri, ri.begin() + 2, rout.begin());
    test_true( rcr2.out == rout.end() );
    STD::string s13 = joinv(rout);
    test_str_eq( s13, "3,4,1,2" );
    test_pass("cxx20:ranges::rotate_copy");
}

TEST_CASE(algorithm, ranges_partition_family) {
    STD::vector<int> v{1, 2, 3, 4, 5, 6};
    test_true( !rng::is_partitioned(v, is_even) );

    rng::subrange<STD::vector<int>::iterator> p = rng::partition(v, is_even);
    test_eq( p.begin() - v.begin(), 3 );
    test_pass("cxx20:ranges::partition");
    test_true( rng::is_partitioned(v, is_even) );
    test_true( rng::all_of(v.begin(), p.begin(), is_even) );

    STD::vector<int> s{1, 2, 3, 4, 5, 6};
    auto sp = rng::stable_partition(s.begin(), s.end(), is_even);
    test_eq( sp.begin() - s.begin(), 3 );
    STD::string s1 = joinv(s);
    test_str_eq( s1, "2,4,6,1,3,5" );
    test_pass("cxx20:ranges::stable_partition");

    STD::vector<Rec> r = recs();
    auto rp = rng::stable_partition(r, [](int id) { return id > 1; }, &Rec::id);
    test_eq( rp.begin() - r.begin(), 2 );
    test_eq( r[0].id, 3 );
    test_eq( r[1].id, 2 );
    test_eq( r[2].id, 1 );
    test_true( rng::is_partitioned(r, [](int id) { return id > 1; }, &Rec::id) );
    test_pass("cxx20:ranges::is_partitioned");

    STD::vector<int> yes, no;
    STD::vector<int> src{1, 2, 3, 4};
    rng::partition_copy_result<STD::vector<int>::iterator,
                               STD::back_insert_iterator<STD::vector<int> >,
                               STD::back_insert_iterator<STD::vector<int> > > pc =
        rng::partition_copy(src, STD::back_inserter(yes), STD::back_inserter(no), is_even);
    test_true( pc.in == src.end() );
    STD::string s2 = joinv(yes);
    STD::string s3 = joinv(no);
    test_str_eq( s2, "2,4" );
    test_str_eq( s3, "1,3" );
    test_pass("cxx20:ranges::partition_copy");

    STD::vector<int> pp{2, 4, 1, 3};
    test_true( rng::partition_point(pp, is_even) == pp.begin() + 2 );
    test_true( rng::partition_point(pp.begin(), pp.end(), is_even) == pp.begin() + 2 );
    STD::vector<Rec> pr = recs();
    rng::stable_partition(pr, [](int id) { return id > 1; }, &Rec::id);
    test_true( rng::partition_point(pr, [](int id) { return id > 1; }, &Rec::id)
               == pr.begin() + 2 );
    test_pass("cxx20:ranges::partition_point");
}

TEST_CASE(algorithm, ranges_sort_family) {
    STD::vector<int> v{5, 1, 4, 2, 3};
    STD::vector<int>::iterator se = rng::sort(v);
    test_true( se == v.end() );
    STD::string s1 = joinv(v);
    test_str_eq( s1, "1,2,3,4,5" );

    rng::sort(v.begin(), v.end(), rng::greater());
    STD::string s2 = joinv(v);
    test_str_eq( s2, "5,4,3,2,1" );

    STD::vector<Rec> r = recs();
    rng::sort(r, rng::less(), &Rec::id);
    test_eq( r[0].id, 1 );
    test_eq( r[2].id, 3 );
    test_pass("cxx20:ranges::sort");

    test_true(  rng::is_sorted(r, rng::less(), &Rec::id) );
    test_true( !rng::is_sorted(v) );
    test_true(  rng::is_sorted(v, rng::greater()) );
    test_true(  rng::is_sorted(v.begin(), v.end(), rng::greater()) );
    test_pass("cxx20:ranges::is_sorted");

    STD::vector<int> u{1, 3, 2};
    test_true( rng::is_sorted_until(u) == u.begin() + 2 );
    test_true( rng::is_sorted_until(u.begin(), u.end()) == u.begin() + 2 );
    test_true( rng::is_sorted_until(v, rng::greater()) == v.end() );
    test_true( rng::is_sorted_until(r, rng::less(), &Rec::id) == r.end() );
    test_pass("cxx20:ranges::is_sorted_until");

    STD::vector<Rec> sr = recs();
    rng::stable_sort(sr, rng::less(), [](const Rec& x) { return x.name.size(); });
    test_str_eq( sr[0].name, "a" );
    test_str_eq( sr[1].name, "bb" );
    test_str_eq( sr[2].name, "cee" );
    STD::vector<int> ss{3, 1, 2};
    rng::stable_sort(ss.begin(), ss.end());
    STD::string s3 = joinv(ss);
    test_str_eq( s3, "1,2,3" );
    test_pass("cxx20:ranges::stable_sort");

    STD::vector<int> ps{5, 1, 4, 2, 3};
    STD::vector<int>::iterator pse = rng::partial_sort(ps, ps.begin() + 3);
    test_true( pse == ps.end() );
    test_eq( ps[0], 1 );
    test_eq( ps[2], 3 );
    STD::vector<int> pg{5, 1, 4};
    rng::partial_sort(pg.begin(), pg.begin() + 2, pg.end(), rng::greater());
    test_eq( pg[0], 5 );
    test_eq( pg[1], 4 );
    test_pass("cxx20:ranges::partial_sort");

    STD::list<int>   src{9, 3, 7, 1};
    STD::vector<int> out(2);
    rng::partial_sort_copy_result<STD::list<int>::iterator, STD::vector<int>::iterator> pc =
        rng::partial_sort_copy(src, out);
    test_true( pc.in == src.end() );
    test_true( pc.out == out.end() );
    STD::string s4 = joinv(out);
    test_str_eq( s4, "1,3" );
    STD::vector<int> outg(2);
    auto pc2 = rng::partial_sort_copy(src.begin(), src.end(), outg.begin(), outg.end(),
                                      rng::greater());
    test_true( pc2.out == outg.end() );
    STD::string s5 = joinv(outg);
    test_str_eq( s5, "9,7" );
    test_pass("cxx20:ranges::partial_sort_copy");

    STD::vector<int> ne{7, 3, 9, 1, 5};
    STD::vector<int>::iterator nee = rng::nth_element(ne, ne.begin() + 2);
    test_true( nee == ne.end() );
    test_eq( ne[2], 5 );
    STD::vector<int> ng{7, 3, 9, 1, 5};
    rng::nth_element(ng.begin(), ng.begin() + 1, ng.end(), rng::greater());
    test_eq( ng[1], 7 );
    test_pass("cxx20:ranges::nth_element");
}

TEST_CASE(algorithm, ranges_binary_search_family) {
    STD::vector<int> v{1, 3, 3, 5};
    STD::list<int>   l{1, 2, 3};

    test_true( rng::lower_bound(v, 3) == v.begin() + 1 );
    test_true( rng::upper_bound(v, 3) == v.begin() + 3 );
    test_true( rng::lower_bound(v.begin(), v.end(), 4) == v.begin() + 3 );
    test_true( rng::upper_bound(l, 2) == ++(++l.begin()) );

    STD::vector<int> d{9, 5, 1};
    test_true( rng::lower_bound(d, 5, rng::greater()) == d.begin() + 1 );
    test_true( rng::upper_bound(d, 5, rng::greater()) == d.begin() + 2 );

    STD::vector<Rec> r = recs();
    rng::sort(r, rng::less(), &Rec::id);
    test_true( rng::lower_bound(r, 2, rng::less(), &Rec::id) == r.begin() + 1 );
    test_pass("cxx20:ranges::lower_bound");
    test_true( rng::upper_bound(r, 2, rng::less(), &Rec::id) == r.begin() + 2 );
    test_pass("cxx20:ranges::upper_bound");

    rng::subrange<STD::vector<int>::iterator> er = rng::equal_range(v, 3);
    test_true( er.begin() == v.begin() + 1 );
    test_eq( er.end() - er.begin(), 2 );
    auto er0 = rng::equal_range(v.begin(), v.end(), 4);
    test_true( er0.begin() == er0.end() );
    auto erp = rng::equal_range(r, 3, rng::less(), &Rec::id);
    test_eq( erp.end() - erp.begin(), 1 );
    test_pass("cxx20:ranges::equal_range");

    test_true(  rng::binary_search(v, 5) );
    test_true( !rng::binary_search(v, 4) );
    test_true(  rng::binary_search(v.begin(), v.end(), 1) );
    test_true(  rng::binary_search(d, 9, rng::greater()) );
    test_true(  rng::binary_search(r, 2, rng::less(), &Rec::id) );
    test_pass("cxx20:ranges::binary_search");
}

TEST_CASE(algorithm, ranges_merge_and_set_ops) {
    STD::vector<int> a{1, 3, 5};
    STD::vector<int> b{2, 4};

    STD::vector<int> m(5);
    rng::merge_result<STD::vector<int>::iterator, STD::vector<int>::iterator,
                      STD::vector<int>::iterator> mr = rng::merge(a, b, m.begin());
    test_true( mr.in1 == a.end() );
    test_true( mr.in2 == b.end() );
    test_true( mr.out == m.end() );
    STD::string s1 = joinv(m);
    test_str_eq( s1, "1,2,3,4,5" );

    STD::vector<int> m2;
    auto mr2 = rng::merge(a.begin(), a.end(), b.begin(), b.end(),
                          STD::back_inserter(m2), rng::less());
    test_true( mr2.in1 == a.end() );
    STD::string s2 = joinv(m2);
    test_str_eq( s2, "1,2,3,4,5" );
    test_pass("cxx20:ranges::merge");

    STD::vector<int> im{1, 4, 7, 2, 3};
    STD::vector<int>::iterator ime = rng::inplace_merge(im, im.begin() + 3);
    test_true( ime == im.end() );
    STD::string s3 = joinv(im);
    test_str_eq( s3, "1,2,3,4,7" );
    STD::vector<int> dm{9, 5, 8, 2};
    rng::inplace_merge(dm.begin(), dm.begin() + 2, dm.end(), rng::greater());
    STD::string s4 = joinv(dm);
    test_str_eq( s4, "9,8,5,2" );
    test_pass("cxx20:ranges::inplace_merge");

    STD::vector<int> super{1, 2, 3, 4, 5};
    STD::vector<int> sub{2, 4};
    test_true(  rng::includes(super, sub) );
    test_true( !rng::includes(super, STD::vector<int>{2, 9}) );
    test_true(  rng::includes(super.begin(), super.end(), sub.begin(), sub.end(),
                              rng::less()) );
    STD::vector<Rec> r = recs();
    rng::sort(r, rng::less(), &Rec::id);
    STD::vector<int> ids{1, 3};
    test_true( rng::includes(r, ids, rng::less(), &Rec::id) );
    test_pass("cxx20:ranges::includes");

    STD::vector<int> x{1, 2, 3, 4};
    STD::vector<int> y{3, 4, 5};

    STD::vector<int> u;
    rng::set_union_result<STD::vector<int>::iterator, STD::vector<int>::iterator,
                          STD::back_insert_iterator<STD::vector<int> > > ur =
        rng::set_union(x, y, STD::back_inserter(u));
    test_true( ur.in1 == x.end() );
    STD::string s5 = joinv(u);
    test_str_eq( s5, "1,2,3,4,5" );

    STD::vector<int> i;
    rng::set_intersection_result<STD::vector<int>::iterator, STD::vector<int>::iterator,
                                 STD::back_insert_iterator<STD::vector<int> > > ir =
        rng::set_intersection(x, y, STD::back_inserter(i));
    test_true( ir.in2 == y.end() );
    STD::string s6 = joinv(i);
    test_str_eq( s6, "3,4" );

    STD::vector<int> df;
    rng::set_difference_result<STD::vector<int>::iterator,
                               STD::back_insert_iterator<STD::vector<int> > > dr =
        rng::set_difference(x, y, STD::back_inserter(df));
    test_true( dr.in == x.end() );
    STD::string s7 = joinv(df);
    test_str_eq( s7, "1,2" );

    STD::vector<int> sd;
    rng::set_symmetric_difference_result<STD::vector<int>::iterator,
                                         STD::vector<int>::iterator,
                                         STD::back_insert_iterator<STD::vector<int> > > sdr =
        rng::set_symmetric_difference(x, y, STD::back_inserter(sd));
    test_true( sdr.in1 == x.end() );
    STD::string s8 = joinv(sd);
    test_str_eq( s8, "1,2,5" );

    STD::vector<int> dx{4, 3, 2};
    STD::vector<int> dy{5, 3};
    STD::vector<int> du, di, dd, dsd;
    rng::set_union(dx.begin(), dx.end(), dy.begin(), dy.end(),
                   STD::back_inserter(du), rng::greater());
    STD::string s9 = joinv(du);
    test_str_eq( s9, "5,4,3,2" );
    test_pass("cxx20:ranges::set_union");
    rng::set_intersection(dx.begin(), dx.end(), dy.begin(), dy.end(),
                          STD::back_inserter(di), rng::greater());
    STD::string s10 = joinv(di);
    test_str_eq( s10, "3" );
    test_pass("cxx20:ranges::set_intersection");
    rng::set_difference(dx.begin(), dx.end(), dy.begin(), dy.end(),
                        STD::back_inserter(dd), rng::greater());
    STD::string s11 = joinv(dd);
    test_str_eq( s11, "4,2" );
    test_pass("cxx20:ranges::set_difference");
    rng::set_symmetric_difference(dx.begin(), dx.end(), dy.begin(), dy.end(),
                                  STD::back_inserter(dsd), rng::greater());
    STD::string s12 = joinv(dsd);
    test_str_eq( s12, "5,4,2" );
    test_pass("cxx20:ranges::set_symmetric_difference");
}

TEST_CASE(algorithm, ranges_heap_operations) {
    STD::vector<int> v{3, 1, 4, 1, 5};
    STD::vector<int>::iterator mh = rng::make_heap(v);
    test_true( mh == v.end() );
    test_pass("cxx20:ranges::make_heap");
    test_true( rng::is_heap(v) );
    test_eq( v.front(), 5 );

    v.push_back(9);
    STD::vector<int>::iterator ph = rng::push_heap(v);
    test_true( ph == v.end() );
    test_eq( v.front(), 9 );
    test_pass("cxx20:ranges::push_heap");
    test_true( rng::is_heap(v.begin(), v.end()) );

    rng::pop_heap(v);
    test_eq( v.back(), 9 );
    test_pass("cxx20:ranges::pop_heap");
    v.pop_back();
    test_true( rng::is_heap_until(v) == v.end() );

    rng::sort_heap(v);
    STD::string s1 = joinv(v);
    test_str_eq( s1, "1,1,3,4,5" );
    test_pass("cxx20:ranges::sort_heap");

    STD::vector<int> m{3, 1, 4};
    rng::make_heap(m.begin(), m.end(), rng::greater());
    test_true( rng::is_heap(m, rng::greater()) );
    test_pass("cxx20:ranges::is_heap");
    test_eq( m.front(), 1 );
    m.push_back(0);
    rng::push_heap(m.begin(), m.end(), rng::greater());
    test_eq( m.front(), 0 );
    rng::pop_heap(m.begin(), m.end(), rng::greater());
    test_eq( m.back(), 0 );
    m.pop_back();
    rng::sort_heap(m, rng::greater());
    STD::string s2 = joinv(m);
    test_str_eq( s2, "4,3,1" );

    STD::vector<int> nh{1, 2, 3};
    test_true( !rng::is_heap(nh) );
    test_true( rng::is_heap_until(nh) == nh.begin() + 1 );
    test_true( rng::is_heap_until(nh.begin(), nh.end()) == nh.begin() + 1 );
    test_pass("cxx20:ranges::is_heap_until");

    STD::vector<Rec> r = recs();
    auto proj = [](const Rec& x) { return x.name.size(); };
    rng::make_heap(r, rng::less(), proj);
    test_str_eq( r.front().name, "cee" );
    test_true( rng::is_heap(r, rng::less(), proj) );
    test_true( rng::is_heap_until(r, rng::less(), proj) == r.end() );
}

TEST_CASE(algorithm, ranges_minmax_clamp_lexcmp) {
    STD::vector<int> v{4, 1, 9, 3};
    STD::vector<Rec> r = recs();

    test_eq( rng::min(3, 7), 3 );
    test_eq( rng::max(3, 7), 7 );
    test_eq( rng::min(3, 7, rng::greater()), 7 );
    test_eq( rng::max(3, 7, rng::greater()), 3 );
    test_eq( rng::min({4, 1, 7}), 1 );
    test_eq( rng::max({4, 1, 7}), 7 );
    test_eq( rng::min(v), 1 );
    test_eq( rng::max(v), 9 );
    test_eq( rng::min(v, rng::greater()), 9 );
    Rec lo = rng::min(r, rng::less(), &Rec::id);
    test_eq( lo.id, 1 );
    test_pass("cxx20:ranges::min");
    Rec hi = rng::max(r, rng::less(), &Rec::id);
    test_eq( hi.id, 3 );
    test_pass("cxx20:ranges::max");

    int nine = 9, two = 2;
    rng::minmax_result<const int&> mm = rng::minmax(nine, two);
    test_eq( mm.min, 2 );
    test_eq( mm.max, 9 );
    rng::minmax_result<int> mml = rng::minmax({5, 3, 8});
    test_eq( mml.min, 3 );
    test_eq( mml.max, 8 );
    rng::minmax_result<int> mmr = rng::minmax(v);
    test_eq( mmr.min, 1 );
    test_eq( mmr.max, 9 );
    auto mmc = rng::minmax(v, rng::greater());
    test_eq( mmc.min, 9 );
    test_eq( mmc.max, 1 );
    auto mmp = rng::minmax(r, rng::less(), &Rec::id);
    test_eq( mmp.min.id, 1 );
    test_eq( mmp.max.id, 3 );
    test_pass("cxx20:ranges::minmax");

    test_true( rng::min_element(v) == v.begin() + 1 );
    test_true( rng::max_element(v) == v.begin() + 2 );
    test_true( rng::min_element(v.begin(), v.end(), rng::greater()) == v.begin() + 2 );
    test_pass("cxx20:ranges::min_element");
    test_true( rng::max_element(r, rng::less(), &Rec::id) == r.begin() );
    test_pass("cxx20:ranges::max_element");

    rng::minmax_element_result<STD::vector<int>::iterator> mme = rng::minmax_element(v);
    test_true( mme.min == v.begin() + 1 );
    test_true( mme.max == v.begin() + 2 );
    auto mmep = rng::minmax_element(r, rng::less(), &Rec::id);
    test_eq( mmep.min->id, 1 );
    test_eq( mmep.max->id, 3 );
    test_pass("cxx20:ranges::minmax_element");

    test_eq( rng::clamp(5, 1, 10), 5 );
    test_eq( rng::clamp(-1, 1, 10), 1 );
    test_eq( rng::clamp(99, 1, 10), 10 );
    test_eq( rng::clamp(5, 10, 1, rng::greater()), 5 );
    Rec c = rng::clamp(Rec{9, "x"}, Rec{1, "lo"}, Rec{4, "hi"}, rng::less(), &Rec::id);
    test_eq( c.id, 4 );
    test_pass("cxx20:ranges::clamp");

    STD::vector<int> a{1, 2, 3};
    STD::vector<int> b{1, 2, 4};
    test_true(  rng::lexicographical_compare(a, b) );
    test_true( !rng::lexicographical_compare(b, a) );
    test_true(  rng::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(),
                                             rng::less()) );
    test_true(  rng::lexicographical_compare(b, a, rng::greater()) );
    STD::vector<int> ids{3, 1, 3};
    test_true( rng::lexicographical_compare(r, ids, rng::less(), &Rec::id) );
    test_pass("cxx20:ranges::lexicographical_compare");
}

TEST_CASE(algorithm, ranges_permutation_shuffle_sample) {

    STD::vector<int> v{1, 2, 3};
    rng::next_permutation_result<STD::vector<int>::iterator> np = rng::next_permutation(v);
    test_true( np.found );
    test_true( np.in == v.end() );
    STD::string s1 = joinv(v);
    test_str_eq( s1, "1,3,2" );

    STD::vector<int> last{3, 2, 1};
    auto np2 = rng::next_permutation(last.begin(), last.end());
    test_true( !np2.found );
    STD::string s2 = joinv(last);
    test_str_eq( s2, "1,2,3" );

    STD::vector<int> g{3, 2, 1};
    auto npg = rng::next_permutation(g, rng::greater());
    test_true( npg.found );
    STD::string s3 = joinv(g);
    test_str_eq( s3, "3,1,2" );
    test_pass("cxx20:ranges::next_permutation");

    rng::prev_permutation_result<STD::vector<int>::iterator> pp =
        rng::prev_permutation(g, rng::greater());
    test_true( pp.found );
    STD::string s4 = joinv(g);
    test_str_eq( s4, "3,2,1" );

    STD::vector<Rec> r = recs();
    auto ppr = rng::prev_permutation(r, rng::less(), &Rec::id);
    test_true( ppr.found );
    test_eq( r[0].id, 2 );
    test_eq( r[1].id, 3 );
    test_pass("cxx20:ranges::prev_permutation");

    STD::vector<int> orig{1, 2, 3, 4, 5, 6};
    STD::vector<int> sh = orig;
    STD::mt19937     gen(4242u);
    STD::vector<int>::iterator she = rng::shuffle(sh, gen);
    test_true( she == sh.end() );
    test_true( rng::is_permutation(sh, orig) );
    rng::shuffle(sh.begin(), sh.end(), gen);
    test_true( rng::is_permutation(sh, orig) );
    test_pass("cxx20:ranges::shuffle");

    STD::vector<int> out;
    rng::sample(orig, STD::back_inserter(out), 3, gen);
    test_eq( int(out.size()), 3 );
    rng::sort(out);
    test_true( rng::includes(orig, out) );
    STD::vector<int> out2(2);
    STD::vector<int>::iterator se =
        rng::sample(orig.begin(), orig.end(), out2.begin(), 2, gen);
    test_true( se == out2.end() );
    test_true( rng::includes(orig, out2) );
    test_pass("cxx20:ranges::sample");
}

#if !defined(__GNUC__)
TEST_CASE(algorithm, ranges_fold_and_iota) {
    STD::vector<int> v{1, 2, 3, 4};
    STD::list<int>   l{10, 20};

    test_eq( rng::fold_left(v, 0, STD::plus<int>()), 10 );
    test_eq( rng::fold_left(v.begin(), v.end(), 100, STD::plus<int>()), 110 );
    test_eq( rng::fold_left(l, 0, STD::plus<int>()), 30 );
    test_eq( rng::fold_right(v, 0, STD::plus<int>()), 10 );
    test_eq( rng::fold_right(v.begin(), v.end(), 1, STD::multiplies<int>()), 24 );

    test_eq( rng::fold_left(v, 0, STD::minus<int>()), -10 );
    test_pass("cxx23:ranges::fold_left");
    test_eq( rng::fold_right(v, 0, STD::minus<int>()), -2 );
    test_pass("cxx23:ranges::fold_right");

    STD::optional<int> flf = rng::fold_left_first(v, STD::plus<int>());
    test_true( flf.has_value() );
    test_eq( flf.value(), 10 );
    STD::vector<int> empty;
    STD::optional<int> fle = rng::fold_left_first(empty, STD::plus<int>());
    test_true( !fle.has_value() );
    test_pass("cxx23:ranges::fold_left_first");
    STD::optional<int> frl = rng::fold_right_last(v, STD::minus<int>());
    test_true( frl.has_value() );
    test_eq( frl.value(), -2 );
    test_pass("cxx23:ranges::fold_right_last");
    STD::optional<int> flfi = rng::fold_left_first(v.begin(), v.end(), STD::plus<int>());
    test_eq( flfi.value(), 10 );

    rng::fold_left_with_iter_result<STD::vector<int>::iterator, int> fw =
        rng::fold_left_with_iter(v, 0, STD::plus<int>());
    test_true( fw.in == v.end() );
    test_eq( fw.value, 10 );
    auto fw2 = rng::fold_left_with_iter(v.begin(), v.end(), 5, STD::plus<int>());
    test_true( fw2.in == v.end() );
    test_eq( fw2.value, 15 );
    test_pass("cxx23:ranges::fold_left_with_iter");

    rng::fold_left_first_with_iter_result<STD::vector<int>::iterator, STD::optional<int> > ff =
        rng::fold_left_first_with_iter(v, STD::plus<int>());
    test_true( ff.in == v.end() );
    test_true( ff.value.has_value() );
    test_eq( ff.value.value(), 10 );
    auto ffe = rng::fold_left_first_with_iter(empty, STD::plus<int>());
    test_true( !ffe.value.has_value() );
    test_pass("cxx23:ranges::fold_left_first_with_iter");

    STD::vector<STD::string> w{"a", "b", "c"};
    STD::string cat = rng::fold_left(w, STD::string(""),
                                     [](STD::string acc, const STD::string& s) {
                                         return acc + s;
                                     });
    test_str_eq( cat, "abc" );

    STD::vector<int> io(4);
    rng::iota_result<STD::vector<int>::iterator, int> ir = rng::iota(io, 5);
    test_true( ir.out == io.end() );
    test_eq( ir.value, 9 );
    STD::string s1 = joinv(io);
    test_str_eq( s1, "5,6,7,8" );

    STD::list<int> il(3);
    auto ir2 = rng::iota(il.begin(), il.end(), 0);
    test_true( ir2.out == il.end() );
    test_eq( ir2.value, 3 );
    STD::string s2 = joinl(il);
    test_str_eq( s2, "0,1,2" );
    test_pass("cxx23:ranges::iota");
}
#else
TEST_CASE_SKIP(algorithm, ranges_fold_and_iota)
#endif


namespace {
int g_ac_live = 0;

struct AcLive {
    char* p_;
    AcLive() : p_(new char[16]) { p_[0] = 0; ++g_ac_live; }
    AcLive(char c) : p_(new char[16]) { p_[0] = c; p_[1] = 0; ++g_ac_live; }
    AcLive(const AcLive& o) : p_(new char[16]) {
        for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        ++g_ac_live;
    }
    AcLive& operator=(const AcLive& o) {
        if (this != &o) for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~AcLive() { delete[] p_; --g_ac_live; }
    char tag() const { return p_[0]; }
    bool operator< (const AcLive& o) const { return p_[0] <  o.p_[0]; }
    bool operator> (const AcLive& o) const { return p_[0] >  o.p_[0]; }
    bool operator<=(const AcLive& o) const { return p_[0] <= o.p_[0]; }
    bool operator>=(const AcLive& o) const { return p_[0] >= o.p_[0]; }
    bool operator==(const AcLive& o) const { return p_[0] == o.p_[0]; }
    bool operator!=(const AcLive& o) const { return p_[0] != o.p_[0]; }
};

STD::vector<AcLive> ac_make(const char* cs) {
    STD::vector<AcLive> v;
    for (const char* p = cs; *p; ++p) v.push_back(AcLive(*p));
    return v;
}

STD::string ac_join(const STD::vector<AcLive>& v, STD::size_t n) {
    STD::string r;
    for (STD::size_t i = 0; i < n && i < v.size(); ++i) r += v[i].tag();
    return r;
}

struct AcIsB { bool operator()(const AcLive& v) const { return v.tag() == 'b'; } };
struct AcTag { char operator()(const AcLive& v) const { return v.tag(); } };
}

TEST_CASE(algorithm, ranges_class_elements) {
    g_ac_live = 0;
    {
        STD::vector<AcLive> v = ac_make("ecabd");
        test_eq( g_ac_live, 5 );

        test_true( rng::all_of(v, [](const AcLive& x) { return x.tag() >= 'a'; }) );
        test_eq( (int)rng::count_if(v, AcIsB()), 1 );
        test_true( rng::find(v, AcLive('a')) == v.begin() + 2 );
        test_true( rng::find_if(v, AcIsB()) == v.begin() + 3 );
        test_eq( g_ac_live, 5 );

        test_true( rng::min_element(v)->tag() == 'a' );
        test_true( rng::max_element(v)->tag() == 'e' );

        rng::sort(v);
        test_eq( ac_join(v, 5), STD::string("abcde") );
        test_eq( g_ac_live, 5 );
        test_true( rng::is_sorted(v) );
        test_true( rng::binary_search(v, AcLive('c')) );
        test_true( rng::lower_bound(v, AcLive('c')) == v.begin() + 2 );

        STD::vector<AcLive> out(5);
        test_eq( g_ac_live, 10 );
        rng::copy(v, out.begin());
        test_eq( ac_join(out, 5), STD::string("abcde") );
        test_eq( g_ac_live, 10 );

        rng::reverse(out);
        test_eq( ac_join(out, 5), STD::string("edcba") );
        rng::rotate(out, out.begin() + 2);
        test_eq( ac_join(out, 5), STD::string("cbaed") );
        test_eq( g_ac_live, 10 );

        rng::fill(out, AcLive('z'));
        test_eq( ac_join(out, 5), STD::string("zzzzz") );
        test_eq( g_ac_live, 10 );

        STD::vector<AcLive> d = ac_make("aabbc");
        test_eq( g_ac_live, 15 );
        rng::subrange<STD::vector<AcLive>::iterator> sub = rng::unique(d);
        test_eq( (int)(sub.begin() - d.begin()), 3 );
        test_eq( ac_join(d, 3), STD::string("abc") );
        test_eq( g_ac_live, 15 );

        STD::vector<AcLive> r = ac_make("abcbd");
        rng::subrange<STD::vector<AcLive>::iterator> rem = rng::remove(r, AcLive('b'));
        test_eq( (int)(rem.begin() - r.begin()), 3 );
        test_eq( ac_join(r, 3), STD::string("acd") );
        test_eq( g_ac_live, 20 );
        r.erase(rem.begin(), r.end());
        test_eq( g_ac_live, 18 );

        STD::vector<char> tags(5);
        rng::transform(v, tags.begin(), AcTag());
        test_eq( tags[0], 'a' );
        test_eq( tags[4], 'e' );
        test_eq( g_ac_live, 18 );
    }
    test_eq( g_ac_live, 0 );
}

#else
TEST_CASE_SKIP(algorithm, ranges_all_any_none_for_each)
TEST_CASE_SKIP(algorithm, ranges_find_family)
TEST_CASE_SKIP(algorithm, ranges_count_mismatch_equal)
TEST_CASE_SKIP(algorithm, ranges_search_family)
TEST_CASE_SKIP(algorithm, ranges_copy_move_swap)
TEST_CASE_SKIP(algorithm, ranges_transform_replace_fill)
TEST_CASE_SKIP(algorithm, ranges_remove_unique_reverse_rotate)
TEST_CASE_SKIP(algorithm, ranges_partition_family)
TEST_CASE_SKIP(algorithm, ranges_sort_family)
TEST_CASE_SKIP(algorithm, ranges_binary_search_family)
TEST_CASE_SKIP(algorithm, ranges_merge_and_set_ops)
TEST_CASE_SKIP(algorithm, ranges_heap_operations)
TEST_CASE_SKIP(algorithm, ranges_minmax_clamp_lexcmp)
TEST_CASE_SKIP(algorithm, ranges_permutation_shuffle_sample)
TEST_CASE_SKIP(algorithm, ranges_fold_and_iota)
TEST_CASE_SKIP(algorithm, ranges_class_elements)
#endif
