#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011
#include <algorithm>
#include <execution>
#include <numeric>
#include <functional>
#include <iterator>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

using _ccw::string_view;

#if TEST_TARGET_CXX >= 2020
namespace rng = STD::ranges;
#endif

namespace {

bool is_even(int x) { return (x % 2) == 0; }
struct Seven { int operator()() const { return 7; } };

bool is_positive(int x) { return x > 0; }
bool is_lt5(int x)      { return x < 5; }
int  square(int x)      { return x * x; }

struct add_into {
    int* __p;
    explicit add_into(int* __q) : __p(__q) {}
    void operator()(int x) const { *__p += x; }
};

}

#if TEST_TARGET_CXX >= 2020
TEST_CASE(algorithm, result_types_members) {
    int a[3] = {1, 2, 3};

    rng::in_fun_result<int*, int> r1 = {a, 7};
    test_true( r1.in == a );
    test_eq( r1.fun, 7 );
    test_pass("cxx20:in_fun_result");

    rng::in_in_result<int*, int*> r2 = {a, a + 1};
    test_true( r2.in1 == a );
    test_true( r2.in2 == a + 1 );
    test_pass("cxx20:in_in_result");

    rng::in_out_result<int*, int*> r3 = {a, a + 2};
    test_true( r3.in == a );
    test_true( r3.out == a + 2 );
    test_pass("cxx20:in_out_result");

    rng::in_in_out_result<int*, int*, int*> r4 = {a, a + 1, a + 2};
    test_true( r4.in1 == a );
    test_true( r4.in2 == a + 1 );
    test_true( r4.out == a + 2 );
    test_pass("cxx20:in_in_out_result");

    rng::in_out_out_result<int*, int*, int*> r5 = {a, a + 1, a + 2};
    test_true( r5.in == a );
    test_true( r5.out1 == a + 1 );
    test_true( r5.out2 == a + 2 );
    test_pass("cxx20:in_out_out_result");

    rng::min_max_result<int> r6 = {2, 9};
    test_eq( r6.min, 2 );
    test_eq( r6.max, 9 );
    test_pass("cxx20:min_max_result");

    rng::in_found_result<int*> r7 = {a + 1, true};
    test_true( r7.in == a + 1 );
    test_true( r7.found );
    test_pass("cxx20:in_found_result");

#if !defined(__GNUC__)
    rng::in_value_result<int*, int> r8 = {a + 3, 42};
    test_true( r8.in == a + 3 );
    test_eq( r8.value, 42 );
    test_pass("cxx23:in_value_result");

    rng::out_value_result<int*, int> r9 = {a + 3, 5};
    test_true( r9.out == a + 3 );
    test_eq( r9.value, 5 );
    test_pass("cxx23:out_value_result");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
}

#if defined(__WATCOMC__)
TEST_CASE_SKIP(algorithm, result_type_conversions)
#else
TEST_CASE(algorithm, result_type_conversions) {
    int a[3] = {1, 2, 3};

    rng::in_fun_result<int*, int> f1 = {a, 7};
    rng::in_fun_result<const int*, long> f2 = f1;
    test_true( f2.in == a );
    test_eq( f2.fun, 7L );
    rng::in_fun_result<const int*, long> f3 = STD::move(f1);
    test_true( f3.in == a );
    test_pass("cxx20:in_fun_result (convert)");

    rng::in_in_result<int*, int*> i1 = {a, a + 1};
    rng::in_in_result<const int*, const int*> i2 = i1;
    test_true( i2.in1 == a );
    test_true( i2.in2 == a + 1 );
    rng::in_in_result<const int*, const int*> i3 = STD::move(i1);
    test_true( i3.in2 == a + 1 );
    test_pass("cxx20:in_in_result (convert)");

    rng::in_out_result<int*, int*> o1 = {a, a + 2};
    rng::in_out_result<const int*, const int*> o2 = o1;
    test_true( o2.in == a );
    test_true( o2.out == a + 2 );
    rng::in_out_result<const int*, const int*> o3 = STD::move(o1);
    test_true( o3.out == a + 2 );
    test_pass("cxx20:in_out_result (convert)");

    rng::in_in_out_result<int*, int*, int*> q1 = {a, a + 1, a + 2};
    rng::in_in_out_result<const int*, const int*, const int*> q2 = q1;
    test_true( q2.in1 == a );
    test_true( q2.out == a + 2 );
    rng::in_in_out_result<const int*, const int*, const int*> q3 = STD::move(q1);
    test_true( q3.in2 == a + 1 );
    test_pass("cxx20:in_in_out_result (convert)");

    rng::in_out_out_result<int*, int*, int*> t1 = {a, a + 1, a + 2};
    rng::in_out_out_result<const int*, const int*, const int*> t2 = t1;
    test_true( t2.in == a );
    test_true( t2.out1 == a + 1 );
    test_true( t2.out2 == a + 2 );
    rng::in_out_out_result<const int*, const int*, const int*> t3 = STD::move(t1);
    test_true( t3.out2 == a + 2 );
    test_pass("cxx20:in_out_out_result (convert)");

    rng::min_max_result<int> m1 = {2, 9};
    rng::min_max_result<long> m2 = m1;
    test_eq( m2.min, 2L );
    test_eq( m2.max, 9L );
    rng::min_max_result<long> m3 = STD::move(m1);
    test_eq( m3.max, 9L );
    test_pass("cxx20:min_max_result (convert)");

    rng::in_found_result<int*> d1 = {a + 1, true};
    rng::in_found_result<const int*> d2 = d1;
    test_true( d2.in == a + 1 );
    test_true( d2.found );
    rng::in_found_result<const int*> d3 = STD::move(d1);
    test_true( d3.found );
    test_pass("cxx20:in_found_result (convert)");

#if !defined(__GNUC__)
    rng::in_value_result<int*, int> v1 = {a + 3, 42};
    rng::in_value_result<const int*, long> v2 = v1;
    test_true( v2.in == a + 3 );
    test_eq( v2.value, 42L );
    rng::in_value_result<const int*, long> v3 = STD::move(v1);
    test_eq( v3.value, 42L );
    test_pass("cxx23:in_value_result (convert)");

    rng::out_value_result<int*, int> w1 = {a + 3, 5};
    rng::out_value_result<const int*, long> w2 = w1;
    test_true( w2.out == a + 3 );
    test_eq( w2.value, 5L );
    rng::out_value_result<const int*, long> w3 = STD::move(w1);
    test_eq( w3.value, 5L );
    test_pass("cxx23:out_value_result (convert)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
}

#endif
#else
TEST_CASE(algorithm, result_types_members) {
    TEST_NOTE("the ranges result types are C++20");
    test_skip("cxx20:in_fun_result");
    test_skip("cxx20:in_in_result");
    test_skip("cxx20:in_out_result");
    test_skip("cxx20:in_in_out_result");
    test_skip("cxx20:in_out_out_result");
    test_skip("cxx20:min_max_result");
    test_skip("cxx20:in_found_result");
    test_skip("cxx23:in_value_result");
    test_skip("cxx23:out_value_result");
}
TEST_CASE(algorithm, result_type_conversions) {
    TEST_NOTE("the ranges result types are C++20");
    test_skip("cxx20:in_fun_result (convert)");
    test_skip("cxx20:in_in_result (convert)");
    test_skip("cxx20:in_out_result (convert)");
    test_skip("cxx20:in_in_out_result (convert)");
    test_skip("cxx20:in_out_out_result (convert)");
    test_skip("cxx20:min_max_result (convert)");
    test_skip("cxx20:in_found_result (convert)");
    test_skip("cxx23:in_value_result (convert)");
    test_skip("cxx23:out_value_result (convert)");
}
#endif

#if !defined(__cpp_lib_ranges) || !_TST_HAS_VAR_TEMPLATE

TEST_CASE_SKIP(algorithm, ranges_result_aliases)
#else
TEST_CASE(algorithm, ranges_result_aliases) {
    typedef int*                                     I;
    typedef const int*                               I2;
    typedef int*                                     O;
    typedef STD::back_insert_iterator<STD::vector<int> > BO;

    test_true( (STD::is_same_v< rng::for_each_result<I, int>,
                                rng::in_fun_result<I, int> >) );
    test_pass("cxx20:for_each_result");
    test_true( (STD::is_same_v< rng::for_each_n_result<I, int>,
                                rng::in_fun_result<I, int> >) );
    test_pass("cxx20:for_each_n_result");

    test_true( (STD::is_same_v< rng::mismatch_result<I, I2>,
                                rng::in_in_result<I, I2> >) );
    test_pass("cxx20:mismatch_result");
    test_true( (STD::is_same_v< rng::swap_ranges_result<I, I2>,
                                rng::in_in_result<I, I2> >) );
    test_pass("cxx20:swap_ranges_result");

    test_true( (STD::is_same_v< rng::copy_result<I, O>,        rng::in_out_result<I, O> >) );
    test_pass("cxx20:copy_result");
    test_true( (STD::is_same_v< rng::copy_n_result<I, O>,      rng::in_out_result<I, O> >) );
    test_pass("cxx20:copy_n_result");
    test_true( (STD::is_same_v< rng::copy_if_result<I, BO>,    rng::in_out_result<I, BO> >) );
    test_pass("cxx20:copy_if_result");
    test_true( (STD::is_same_v< rng::copy_backward_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:copy_backward_result");
    test_true( (STD::is_same_v< rng::move_result<I, O>,        rng::in_out_result<I, O> >) );
    test_pass("cxx20:move_result");
    test_true( (STD::is_same_v< rng::move_backward_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:move_backward_result");
    test_true( (STD::is_same_v< rng::unary_transform_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:unary_transform_result");
    test_true( (STD::is_same_v< rng::replace_copy_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:replace_copy_result");
    test_true( (STD::is_same_v< rng::replace_copy_if_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:replace_copy_if_result");
    test_true( (STD::is_same_v< rng::remove_copy_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:remove_copy_result");
    test_true( (STD::is_same_v< rng::remove_copy_if_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:remove_copy_if_result");
    test_true( (STD::is_same_v< rng::reverse_copy_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:reverse_copy_result");
    test_true( (STD::is_same_v< rng::rotate_copy_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:rotate_copy_result");
    test_true( (STD::is_same_v< rng::unique_copy_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:unique_copy_result");
    test_true( (STD::is_same_v< rng::partial_sort_copy_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:partial_sort_copy_result");

    test_true( (STD::is_same_v< rng::binary_transform_result<I, I2, O>,
                                rng::in_in_out_result<I, I2, O> >) );
    test_pass("cxx20:binary_transform_result");
    test_true( (STD::is_same_v< rng::merge_result<I, I2, O>,
                                rng::in_in_out_result<I, I2, O> >) );
    test_pass("cxx20:merge_result");
    test_true( (STD::is_same_v< rng::set_union_result<I, I2, O>,
                                rng::in_in_out_result<I, I2, O> >) );
    test_pass("cxx20:set_union_result");
    test_true( (STD::is_same_v< rng::set_intersection_result<I, I2, O>,
                                rng::in_in_out_result<I, I2, O> >) );
    test_pass("cxx20:set_intersection_result");
    test_true( (STD::is_same_v< rng::set_symmetric_difference_result<I, I2, O>,
                                rng::in_in_out_result<I, I2, O> >) );
    test_pass("cxx20:set_symmetric_difference_result");

    test_true( (STD::is_same_v< rng::set_difference_result<I, O>,
                                rng::in_out_result<I, O> >) );
    test_pass("cxx20:set_difference_result");

    test_true( (STD::is_same_v< rng::partition_copy_result<I, O, BO>,
                                rng::in_out_out_result<I, O, BO> >) );
    test_pass("cxx20:partition_copy_result");

    test_true( (STD::is_same_v< rng::minmax_result<int>,   rng::min_max_result<int> >) );
    test_pass("cxx20:minmax_result");
    test_true( (STD::is_same_v< rng::minmax_element_result<I>, rng::min_max_result<I> >) );
    test_pass("cxx20:minmax_element_result");

    test_true( (STD::is_same_v< rng::next_permutation_result<I>,
                                rng::in_found_result<I> >) );
    test_pass("cxx20:next_permutation_result");
    test_true( (STD::is_same_v< rng::prev_permutation_result<I>,
                                rng::in_found_result<I> >) );
    test_pass("cxx20:prev_permutation_result");

#if !defined(__GNUC__)
    test_true( (STD::is_same_v< rng::fold_left_with_iter_result<I, int>,
                                rng::in_value_result<I, int> >) );
    test_pass("cxx23:fold_left_with_iter_result");
    test_true( (STD::is_same_v< rng::fold_left_first_with_iter_result<I, STD::optional<int> >,
                                rng::in_value_result<I, STD::optional<int> > >) );
    test_pass("cxx23:fold_left_first_with_iter_result");
    test_true( (STD::is_same_v< rng::iota_result<O, int>,
                                rng::out_value_result<O, int> >) );
    test_pass("cxx23:iota_result");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif

    static const int src_init[] = {1, 2, 3};
    STD::vector<int> src(src_init, src_init + 3);
    STD::vector<int> dst(3);
    test_true( (STD::is_same_v< decltype(rng::copy(src, dst.begin())),
                                rng::copy_result<STD::vector<int>::iterator,
                                                 STD::vector<int>::iterator> >) );
    test_true( (STD::is_same_v< decltype(rng::minmax_element(src)),
                                rng::minmax_element_result<STD::vector<int>::iterator> >) );
#if !defined(__GNUC__)
    test_true( (STD::is_same_v< decltype(rng::iota(dst, 0)),
                                rng::iota_result<STD::vector<int>::iterator, int> >) );
#else
    TEST_SKIP1();
#endif
}

#endif

#if _TST_HAS_EXECUTION_POLICY
TEST_CASE(algorithm, exec_policy_non_modifying) {
    static const int v_init[] = {1, 2, 3, 4, 5, 6};
    STD::vector<int> v(v_init, v_init + 6);
    int              a[] = {2, 4, 6};

    test_true(  STD::all_of(STD::execution::seq, v.begin(), v.end(),
                            is_positive) );
    test_true(  STD::all_of(STD::execution::par, a, a + 3, is_even) );
    test_pass("cxx17:all_of (par)");
    test_true(  STD::any_of(STD::execution::par_unseq, v.begin(), v.end(), is_even) );
    test_pass("cxx17:any_of (par)");
    test_true( !STD::none_of(STD::execution::unseq, v.begin(), v.end(), is_even) );
    test_pass("cxx17:none_of (par)");

    test_eq( STD::count(STD::execution::par, v.begin(), v.end(), 3), 1 );
    test_pass("cxx17:count (par)");
    test_eq( STD::count_if(STD::execution::par_unseq, v.begin(), v.end(), is_even), 3 );
    test_pass("cxx17:count_if (par)");

    test_true( STD::find(STD::execution::par, v.begin(), v.end(), 4) == v.begin() + 3 );
    test_pass("cxx17:find (par)");
    test_true( STD::find_if(STD::execution::seq, v.begin(), v.end(), is_even)
               == v.begin() + 1 );
    test_pass("cxx17:find_if (par)");
    test_true( STD::find_if_not(STD::execution::par, v.begin(), v.end(),
                                is_lt5) == v.begin() + 4 );
    test_pass("cxx17:find_if_not (par)");

    int sum = 0;
    STD::for_each(STD::execution::seq, v.begin(), v.end(), add_into(&sum));
    test_eq( sum, 21 );
    test_pass("cxx17:for_each (par)");
    int n2 = 0;
    STD::for_each_n(STD::execution::seq, v.begin(), 2, add_into(&n2));
    test_eq( n2, 3 );
    test_pass("cxx17:for_each_n (par)");

    STD::vector<int> same = v;
    test_true( STD::equal(STD::execution::par, v.begin(), v.end(), same.begin()) );
    test_true( STD::equal(STD::execution::par, v.begin(), v.end(),
                          same.begin(), same.end()) );
    test_pass("cxx17:equal (par)");

    test_true( STD::is_sorted(STD::execution::par, v.begin(), v.end()) );
    test_pass("cxx17:is_sorted (par)");
    test_true( STD::is_sorted_until(STD::execution::par, v.begin(), v.end()) == v.end() );
    test_pass("cxx17:is_sorted_until (par)");
    test_true( STD::is_partitioned(STD::execution::par, a, a + 3, is_even) );
    test_pass("cxx17:is_partitioned (par)");

    test_true( STD::min_element(STD::execution::par, v.begin(), v.end()) == v.begin() );
    test_pass("cxx17:min_element (par)");
    test_true( STD::max_element(STD::execution::par, v.begin(), v.end()) == v.end() - 1 );
    test_pass("cxx17:max_element (par)");
    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> mm =
        STD::minmax_element(STD::execution::par, v.begin(), v.end());
    test_eq( *mm.first, 1 );
    test_eq( *mm.second, 6 );
    test_pass("cxx17:minmax_element (par)");

    static const int pat_init[] = {3, 4};
    STD::vector<int> pat(pat_init, pat_init + 2);
    test_true( STD::search(STD::execution::par, v.begin(), v.end(),
                           pat.begin(), pat.end()) == v.begin() + 2 );
    test_pass("cxx17:search (par)");
    test_true( STD::adjacent_find(STD::execution::par, v.begin(), v.end()) == v.end() );
    test_pass("cxx17:adjacent_find (par)");
}

TEST_CASE(algorithm, exec_policy_modifying) {
    static const int src_init[] = {1, 2, 3, 4, 5};
    STD::vector<int> src(src_init, src_init + 5);

    STD::vector<int> dst(5);
    STD::vector<int>::iterator ce =
        STD::copy(STD::execution::par, src.begin(), src.end(), dst.begin());
    test_true( ce == dst.end() );
    test_eq( dst[4], 5 );
    test_pass("cxx17:copy (par)");

    STD::vector<int> sq(5);
    STD::transform(STD::execution::par_unseq, src.begin(), src.end(), sq.begin(),
                   square);
    test_eq( sq[4], 25 );

    STD::vector<int> bt(5);
    STD::transform(STD::execution::par, src.begin(), src.end(), sq.begin(), bt.begin(),
                   STD::plus<int>());
    test_eq( bt[0], 2 );
    test_eq( bt[4], 30 );
    test_pass("cxx17:transform (par)");

    STD::vector<int> f(4);
    STD::fill(STD::execution::par, f.begin(), f.end(), 8);
    test_eq( f[3], 8 );
    test_pass("cxx17:fill (par)");
    STD::fill_n(STD::execution::unseq, f.begin(), 2, 1);
    test_eq( f[0], 1 );
    test_eq( f[2], 8 );
    test_pass("cxx17:fill_n (par)");

    static const int r_init[] = {1, 2, 1};
    STD::vector<int> r(r_init, r_init + 3);
    STD::replace(STD::execution::par, r.begin(), r.end(), 1, 9);
    test_eq( r[0], 9 );
    test_eq( r[2], 9 );
    test_pass("cxx17:replace (par)");
    STD::replace_if(STD::execution::par, r.begin(), r.end(), is_even, 0);
    test_eq( r[1], 0 );
    test_pass("cxx17:replace_if (par)");

    static const int rm_init[] = {1, 0, 2, 0};
    STD::vector<int> rm(rm_init, rm_init + 4);
    STD::vector<int>::iterator re =
        STD::remove(STD::execution::par, rm.begin(), rm.end(), 0);
    test_eq( re - rm.begin(), 2 );
    test_pass("cxx17:remove (par)");

    static const int rv_init[] = {1, 2, 3};
    STD::vector<int> rv(rv_init, rv_init + 3);
    STD::reverse(STD::execution::par, rv.begin(), rv.end());
    test_eq( rv[0], 3 );
    test_pass("cxx17:reverse (par)");

    static const int sv_init[] = {5, 1, 4, 2, 3};
    STD::vector<int> sv(sv_init, sv_init + 5);
    STD::sort(STD::execution::par, sv.begin(), sv.end());
    test_eq( sv[0], 1 );
    test_eq( sv[4], 5 );
    STD::sort(STD::execution::par_unseq, sv.begin(), sv.end(), STD::greater<int>());
    test_eq( sv[0], 5 );
    test_pass("cxx17:sort (par)");

    static const int st_init[] = {3, 1, 2};
    STD::vector<int> st(st_init, st_init + 3);
    STD::stable_sort(STD::execution::par, st.begin(), st.end());
    test_eq( st[0], 1 );
    test_eq( st[2], 3 );
    test_pass("cxx17:stable_sort (par)");

    static const int ps_init[] = {5, 1, 4, 2, 3};
    STD::vector<int> ps(ps_init, ps_init + 5);
    STD::partial_sort(STD::execution::par, ps.begin(), ps.begin() + 2, ps.end());
    test_eq( ps[0], 1 );
    test_eq( ps[1], 2 );
    test_pass("cxx17:partial_sort (par)");

    static const int ne_init[] = {7, 3, 9, 1, 5};
    STD::vector<int> ne(ne_init, ne_init + 5);
    STD::nth_element(STD::execution::par, ne.begin(), ne.begin() + 2, ne.end());
    test_eq( ne[2], 5 );
    test_pass("cxx17:nth_element (par)");

    static const int pv_init[] = {1, 2, 3, 4};
    STD::vector<int> pv(pv_init, pv_init + 4);
    STD::vector<int>::iterator pe =
        STD::partition(STD::execution::par, pv.begin(), pv.end(), is_even);
    test_eq( pe - pv.begin(), 2 );
    test_pass("cxx17:partition (par)");

    static const int a_init[] = {1, 3, 5};
    STD::vector<int> a(a_init, a_init + 3);
    static const int b_init[] = {2, 4};
    STD::vector<int> b(b_init, b_init + 2);
    STD::vector<int> m(5);
    STD::merge(STD::execution::par, a.begin(), a.end(), b.begin(), b.end(), m.begin());
    test_eq( m[0], 1 );
    test_eq( m[4], 5 );
    test_pass("cxx17:merge (par)");

    STD::vector<int> u(5);
    STD::vector<int>::iterator ue =
        STD::set_union(STD::execution::par, a.begin(), a.end(), b.begin(), b.end(),
                       u.begin());
    test_eq( ue - u.begin(), 5 );
    test_pass("cxx17:set_union (par)");

    static const int uq_init[] = {1, 1, 2};
    STD::vector<int> uq(uq_init, uq_init + 3);
    STD::vector<int>::iterator uqe = STD::unique(STD::execution::par, uq.begin(), uq.end());
    test_eq( uqe - uq.begin(), 2 );
    test_pass("cxx17:unique (par)");
}

TEST_CASE(algorithm, exec_policy_fill) {
    static const int s6[] = {1, 2, 3, 4, 5, 6};
    namespace ex = STD::execution;

    STD::vector<int> v(s6, s6 + 6), o(6, 0);
    STD::vector<int>::iterator e = STD::copy_if(ex::par, v.begin(), v.end(), o.begin(), is_even);
    test_eq( e - o.begin(), 3 );
    test_pass("cxx17:copy_if (par)");
    STD::vector<int> o2(3, 0);
    STD::copy_n(ex::par, v.begin(), 3, o2.begin());
    test_eq( o2[2], 3 );
    test_pass("cxx17:copy_n (par)");
    STD::vector<int> o3(6, 0);
    STD::move(ex::par, v.begin(), v.end(), o3.begin());
    test_eq( o3[5], 6 );
    test_pass("cxx17:move (par)");

    STD::vector<int> aa(3, 1), bb(3, 2);
    STD::swap_ranges(ex::par, aa.begin(), aa.end(), bb.begin());
    test_eq( aa[0], 2 );
    test_eq( bb[0], 1 );
    test_pass("cxx17:swap_ranges (par)");

    STD::vector<int> g(4, 0);
    STD::generate(ex::par, g.begin(), g.end(), Seven());
    test_eq( g[3], 7 );
    test_pass("cxx17:generate (par)");
    STD::generate_n(ex::par, g.begin(), 2, Seven());
    test_eq( g[1], 7 );
    test_pass("cxx17:generate_n (par)");

    static const int sub2[] = {3, 4};
    test_true( STD::find_end(ex::par, v.begin(), v.end(), sub2, sub2 + 2) == v.begin() + 2 );
    test_pass("cxx17:find_end (par)");
    static const int fs[] = {5, 4};
    test_true( STD::find_first_of(ex::par, v.begin(), v.end(), fs, fs + 2) == v.begin() + 3 );
    test_pass("cxx17:find_first_of (par)");
    static const int rep2[] = {7, 7, 7, 1};
    STD::vector<int> sn(rep2, rep2 + 4);
    test_true( STD::search_n(ex::par, sn.begin(), sn.end(), 2, 7) == sn.begin() );
    test_pass("cxx17:search_n (par)");

    STD::vector<int> mm(s6, s6 + 6); mm[3] = 40;
    STD::pair<STD::vector<int>::iterator, STD::vector<int>::iterator> mp =
        STD::mismatch(ex::par, v.begin(), v.end(), mm.begin());
    test_eq( mp.first - v.begin(), 3 );
    test_pass("cxx17:mismatch (par)");
    static const int lc[] = {1, 2, 3, 4, 5, 7};
    test_true( STD::lexicographical_compare(ex::par, v.begin(), v.end(), lc, lc + 6) );
    test_pass("cxx17:lexicographical_compare (par)");
    static const int inc[] = {2, 4};
    test_true( STD::includes(ex::par, v.begin(), v.end(), inc, inc + 2) );
    test_pass("cxx17:includes (par)");

    static const int hp[] = {6, 5, 4, 3, 2, 1};
    STD::vector<int> h(hp, hp + 6);
    test_true( STD::is_heap(ex::par, h.begin(), h.end()) );
    test_pass("cxx17:is_heap (par)");
    test_true( STD::is_heap_until(ex::par, h.begin(), h.end()) == h.end() );
    test_pass("cxx17:is_heap_until (par)");

    static const int im[] = {1, 3, 5, 2, 4, 6};
    STD::vector<int> mv(im, im + 6);
    STD::inplace_merge(ex::par, mv.begin(), mv.begin() + 3, mv.end());
    test_true( mv[0] == 1 && mv[5] == 6 );
    test_pass("cxx17:inplace_merge (par)");

    static const int pc[] = {5, 1, 4, 2, 3};
    STD::vector<int> src(pc, pc + 5), out(3, 0);
    STD::partial_sort_copy(ex::par, src.begin(), src.end(), out.begin(), out.end());
    test_true( out[0] == 1 && out[2] == 3 );
    test_pass("cxx17:partial_sort_copy (par)");

    STD::vector<int> tt(3, 0), ff(3, 0);
    STD::partition_copy(ex::par, v.begin(), v.end(), tt.begin(), ff.begin(), is_even);
    test_eq( tt[0], 2 );
    test_eq( ff[0], 1 );
    test_pass("cxx17:partition_copy (par)");
    STD::vector<int> sp2(s6, s6 + 6);
    STD::stable_partition(ex::par, sp2.begin(), sp2.end(), is_even);
    test_true( is_even(sp2[0]) );
    test_pass("cxx17:stable_partition (par)");

    STD::vector<int> ri(s6, s6 + 6);
    STD::vector<int>::iterator re = STD::remove_if(ex::par, ri.begin(), ri.end(), is_even);
    test_eq( re - ri.begin(), 3 );
    test_pass("cxx17:remove_if (par)");
    STD::vector<int> rc(6, 0);
    STD::vector<int>::iterator rce = STD::remove_copy(ex::par, v.begin(), v.end(), rc.begin(), 3);
    test_eq( rce - rc.begin(), 5 );
    test_pass("cxx17:remove_copy (par)");
    STD::vector<int> rci(6, 0);
    STD::vector<int>::iterator rcie = STD::remove_copy_if(ex::par, v.begin(), v.end(), rci.begin(), is_even);
    test_eq( rcie - rci.begin(), 3 );
    test_pass("cxx17:remove_copy_if (par)");

    STD::vector<int> rp(6, 0);
    STD::replace_copy(ex::par, v.begin(), v.end(), rp.begin(), 3, 30);
    test_eq( rp[2], 30 );
    test_pass("cxx17:replace_copy (par)");
    STD::vector<int> rpi(6, 0);
    STD::replace_copy_if(ex::par, v.begin(), v.end(), rpi.begin(), is_even, 0);
    test_eq( rpi[1], 0 );
    test_pass("cxx17:replace_copy_if (par)");

    STD::vector<int> rv(6, 0);
    STD::reverse_copy(ex::par, v.begin(), v.end(), rv.begin());
    test_eq( rv[0], 6 );
    test_pass("cxx17:reverse_copy (par)");
    STD::vector<int> rt(s6, s6 + 6);
    STD::rotate(ex::par, rt.begin(), rt.begin() + 2, rt.end());
    test_eq( rt[0], 3 );
    test_pass("cxx17:rotate (par)");
    STD::vector<int> rtc(6, 0);
    STD::rotate_copy(ex::par, v.begin(), v.begin() + 2, v.end(), rtc.begin());
    test_eq( rtc[0], 3 );
    test_pass("cxx17:rotate_copy (par)");

    static const int uq2[] = {1, 1, 2, 2, 3};
    STD::vector<int> uv(uq2, uq2 + 5), uo(5, 0);
    STD::vector<int>::iterator ue = STD::unique_copy(ex::par, uv.begin(), uv.end(), uo.begin());
    test_eq( ue - uo.begin(), 3 );
    test_pass("cxx17:unique_copy (par)");

    static const int sa[] = {1, 2, 3, 4}, sb[] = {2, 4, 6};
    STD::vector<int> so(6, 0);
    STD::vector<int>::iterator sde = STD::set_difference(ex::par, sa, sa + 4, sb, sb + 3, so.begin());
    test_eq( sde - so.begin(), 2 );
    test_pass("cxx17:set_difference (par)");
    STD::vector<int> si(6, 0);
    STD::vector<int>::iterator sie = STD::set_intersection(ex::par, sa, sa + 4, sb, sb + 3, si.begin());
    test_eq( sie - si.begin(), 2 );
    test_pass("cxx17:set_intersection (par)");
    STD::vector<int> ss(6, 0);
    STD::vector<int>::iterator sse = STD::set_symmetric_difference(ex::par, sa, sa + 4, sb, sb + 3, ss.begin());
    test_eq( sse - ss.begin(), 3 );
    test_pass("cxx17:set_symmetric_difference (par)");
}


namespace {
int g_ad_live = 0;

struct AdLive {
    char* p_;
    AdLive() : p_(new char[16]) { p_[0] = 0; ++g_ad_live; }
    AdLive(char c) : p_(new char[16]) { p_[0] = c; p_[1] = 0; ++g_ad_live; }
    AdLive(const AdLive& o) : p_(new char[16]) {
        for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        ++g_ad_live;
    }
    AdLive& operator=(const AdLive& o) {
        if (this != &o) for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~AdLive() { delete[] p_; --g_ad_live; }
    char tag() const { return p_[0]; }
    bool operator< (const AdLive& o) const { return p_[0] <  o.p_[0]; }
    bool operator==(const AdLive& o) const { return p_[0] == o.p_[0]; }
};

struct AdIsB { bool operator()(const AdLive& v) const { return v.tag() == 'b'; } };

STD::vector<AdLive> ad_make(const char* cs) {
    STD::vector<AdLive> v;
    for (const char* p = cs; *p; ++p) v.push_back(AdLive(*p));
    return v;
}

STD::string ad_join(const STD::vector<AdLive>& v, STD::size_t n) {
    STD::string r;
    for (STD::size_t i = 0; i < n && i < v.size(); ++i) r += v[i].tag();
    return r;
}
}

TEST_CASE(algorithm, exec_policy_class_elements) {
    g_ad_live = 0;
    {
        STD::vector<AdLive> v = ad_make("ecabd");
        test_eq( g_ad_live, 5 );

        test_eq( (int)STD::count_if(STD::execution::par, v.begin(), v.end(), AdIsB()), 1 );
        test_true( STD::find(STD::execution::par, v.begin(), v.end(), AdLive('a')) == v.begin() + 2 );
        test_eq( g_ad_live, 5 );

        STD::sort(STD::execution::par, v.begin(), v.end());
        test_eq( ad_join(v, 5), STD::string("abcde") );
        test_eq( g_ad_live, 5 );

        STD::vector<AdLive> out(5);
        STD::copy(STD::execution::par, v.begin(), v.end(), out.begin());
        test_eq( ad_join(out, 5), STD::string("abcde") );
        test_eq( g_ad_live, 10 );

        STD::reverse(STD::execution::par, out.begin(), out.end());
        test_eq( ad_join(out, 5), STD::string("edcba") );
        test_eq( g_ad_live, 10 );

        STD::fill(STD::execution::par, out.begin(), out.end(), AdLive('z'));
        test_eq( ad_join(out, 5), STD::string("zzzzz") );
        test_eq( g_ad_live, 10 );

        STD::vector<AdLive>::iterator ue =
            STD::unique(STD::execution::par, out.begin(), out.end());
        test_eq( (int)(ue - out.begin()), 1 );
        test_eq( g_ad_live, 10 );
    }
    test_eq( g_ad_live, 0 );
}

#else
TEST_CASE(algorithm, exec_policy_non_modifying) {
    TEST_NOTE("no execution policies here (libc++ keeps its PSTL behind _LIBCPP_ENABLE_EXPERIMENTAL)");
    test_skip("cxx17:all_of (par)");
    test_skip("cxx17:any_of (par)");
    test_skip("cxx17:none_of (par)");
    test_skip("cxx17:count (par)");
    test_skip("cxx17:count_if (par)");
    test_skip("cxx17:find (par)");
    test_skip("cxx17:find_if (par)");
    test_skip("cxx17:find_if_not (par)");
    test_skip("cxx17:for_each (par)");
    test_skip("cxx17:for_each_n (par)");
    test_skip("cxx17:equal (par)");
    test_skip("cxx17:is_sorted (par)");
    test_skip("cxx17:is_sorted_until (par)");
    test_skip("cxx17:is_partitioned (par)");
    test_skip("cxx17:min_element (par)");
    test_skip("cxx17:max_element (par)");
    test_skip("cxx17:minmax_element (par)");
    test_skip("cxx17:search (par)");
    test_skip("cxx17:adjacent_find (par)");
}
TEST_CASE(algorithm, exec_policy_modifying) {
    TEST_NOTE("no execution policies here (libc++ keeps its PSTL behind _LIBCPP_ENABLE_EXPERIMENTAL)");
    test_skip("cxx17:copy (par)");
    test_skip("cxx17:transform (par)");
    test_skip("cxx17:fill (par)");
    test_skip("cxx17:fill_n (par)");
    test_skip("cxx17:replace (par)");
    test_skip("cxx17:replace_if (par)");
    test_skip("cxx17:remove (par)");
    test_skip("cxx17:reverse (par)");
    test_skip("cxx17:sort (par)");
    test_skip("cxx17:stable_sort (par)");
    test_skip("cxx17:partial_sort (par)");
    test_skip("cxx17:nth_element (par)");
    test_skip("cxx17:partition (par)");
    test_skip("cxx17:merge (par)");
    test_skip("cxx17:set_union (par)");
    test_skip("cxx17:unique (par)");
}
TEST_CASE(algorithm, exec_policy_fill) {
    TEST_NOTE("no execution policies here (libc++ keeps its PSTL behind _LIBCPP_ENABLE_EXPERIMENTAL)");
    test_skip("cxx17:copy_if (par)");
    test_skip("cxx17:copy_n (par)");
    test_skip("cxx17:move (par)");
    test_skip("cxx17:swap_ranges (par)");
    test_skip("cxx17:generate (par)");
    test_skip("cxx17:generate_n (par)");
    test_skip("cxx17:find_end (par)");
    test_skip("cxx17:find_first_of (par)");
    test_skip("cxx17:search_n (par)");
    test_skip("cxx17:mismatch (par)");
    test_skip("cxx17:lexicographical_compare (par)");
    test_skip("cxx17:includes (par)");
    test_skip("cxx17:is_heap (par)");
    test_skip("cxx17:is_heap_until (par)");
    test_skip("cxx17:inplace_merge (par)");
    test_skip("cxx17:partial_sort_copy (par)");
    test_skip("cxx17:partition_copy (par)");
    test_skip("cxx17:stable_partition (par)");
    test_skip("cxx17:remove_if (par)");
    test_skip("cxx17:remove_copy (par)");
    test_skip("cxx17:remove_copy_if (par)");
    test_skip("cxx17:replace_copy (par)");
    test_skip("cxx17:replace_copy_if (par)");
    test_skip("cxx17:reverse_copy (par)");
    test_skip("cxx17:rotate (par)");
    test_skip("cxx17:rotate_copy (par)");
    test_skip("cxx17:unique_copy (par)");
    test_skip("cxx17:set_difference (par)");
    test_skip("cxx17:set_intersection (par)");
    test_skip("cxx17:set_symmetric_difference (par)");
}


TEST_CASE(algorithm, exec_policy_class_elements) {
    TEST_NOTE("no execution policies here");
    TEST_SKIP_N(15);
}
#endif
TEST_CASE(algorithm, cxx20_algorithm_extra) {
#if defined(__cpp_lib_shift) && !defined(__GNUC__)
    namespace ex = STD::execution;
    static const int s5[] = {1, 2, 3, 4, 5};
    STD::vector<int> sl(s5, s5 + 5);
    STD::shift_left(ex::par, sl.begin(), sl.end(), 2);
    test_true( sl[0] == 3 && sl[1] == 4 && sl[2] == 5 );
    test_pass("cxx20:shift_left (par)");
    STD::vector<int> sr(s5, s5 + 5);
    STD::shift_right(ex::par, sr.begin(), sr.end(), 2);
    test_true( sr[2] == 1 && sr[3] == 2 && sr[4] == 3 );
    test_pass("cxx20:shift_right (par)");
#else
    test_skip("cxx20:shift_left (par)");
    test_skip("cxx20:shift_right (par)");
#endif

#if defined(__cpp_lib_three_way_comparison)
    static const int la[] = {1, 2, 3}, lb[] = {1, 2, 4};
    test_true( STD::lexicographical_compare_three_way(la, la + 3, lb, lb + 3) < 0 );
    test_true( STD::lexicographical_compare_three_way(la, la + 3, la, la + 3) == 0 );
    test_pass("cxx20:lexicographical_compare_three_way");
#else
    test_skip("cxx20:lexicographical_compare_three_way");
#endif
}
#else
TEST_CASE_SKIP(algorithm, result_types_members)
TEST_CASE_SKIP(algorithm, result_type_conversions)
TEST_CASE_SKIP(algorithm, ranges_result_aliases)
TEST_CASE_SKIP(algorithm, exec_policy_non_modifying)
TEST_CASE_SKIP(algorithm, exec_policy_modifying)
TEST_CASE_SKIP(algorithm, exec_policy_fill)
TEST_CASE_SKIP(algorithm, cxx20_algorithm_extra)
#endif
