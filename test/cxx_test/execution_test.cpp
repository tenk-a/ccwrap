#include "test_cxx.hpp"
#if _TST_TEST_HAS(<version>)
#  include <version>
#endif
#if TEST_TARGET_CXX >= 2017
#include <execution>
#endif

TEST_CASE(execution, header_compiles) {
#if TEST_TARGET_CXX >= 2017
    test_true( true );
    test_pass("cxx17:header compiles");
#else
    TEST_NOTE("<execution> is C++17");
    test_skip("cxx17:header compiles");
#endif
}

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_EXECUTION_POLICY
#include <execution>
#include <algorithm>
#include <numeric>
#include <vector>
#include <type_traits>

namespace {
template <class _Want, class _Got>
bool has_policy_type(const _Got&) { return STD::is_same<_Want, _Got>::value; }

int  times2(int x)  { return x * 2; }
int  squarei(int x) { return x * x; }
int  negate_i(int x) { return -x; }
bool over90(int x)  { return x > 90; }

struct count_even {
    int* p_;
    explicit count_even(int* p) : p_(p) {}
    void operator()(int x) const { if (x % 2 == 0) ++*p_; }
};
}

TEST_CASE(execution, policy_types) {

    typedef STD::execution::sequenced_policy seq_t;
    typedef STD::execution::parallel_policy par_t;
    typedef STD::execution::parallel_unsequenced_policy par_unseq_t;
    typedef STD::execution::unsequenced_policy unseq_t;

    test_true( STD::is_execution_policy<seq_t>::value );
    test_true( STD::is_copy_constructible<seq_t>::value );
    test_true( !(STD::is_same<seq_t, par_t>::value) );
    test_true( !(STD::is_same<seq_t, unseq_t>::value) );
    seq_t s2 = STD::execution::seq;
    (void)s2;
    test_pass("cxx17:sequenced_policy");

    test_true( STD::is_execution_policy<par_t>::value );
    test_true( STD::is_copy_constructible<par_t>::value );
    test_true( !(STD::is_same<par_t, par_unseq_t>::value) );
    par_t p2 = STD::execution::par;
    (void)p2;
    test_pass("cxx17:parallel_policy");

    test_true( STD::is_execution_policy<par_unseq_t>::value );
    test_true( STD::is_copy_constructible<par_unseq_t>::value );
    test_true( !(STD::is_same<par_unseq_t, unseq_t>::value) );
    par_unseq_t u2 = STD::execution::par_unseq;
    (void)u2;
    test_pass("cxx17:parallel_unsequenced_policy");
}

TEST_CASE(execution, policy_objects) {
    test_true( has_policy_type<STD::execution::sequenced_policy>(STD::execution::seq) );
#if _tst_cplusplus >= 201103L
    test_true( STD::is_const<decltype(STD::execution::seq)>::value );
    test_pass("cxx17:execution::seq");
#else
    TEST_SKIP1();
    test_skip("cxx17:execution::seq");
#endif

    test_true( has_policy_type<STD::execution::parallel_policy>(STD::execution::par) );
#if _tst_cplusplus >= 201103L
    test_true( STD::is_const<decltype(STD::execution::par)>::value );
    test_pass("cxx17:execution::par");
#else
    TEST_SKIP1();
    test_skip("cxx17:execution::par");
#endif

    test_true( has_policy_type<STD::execution::parallel_unsequenced_policy>(STD::execution::par_unseq) );
#if _tst_cplusplus >= 201103L
    test_true( STD::is_const<decltype(STD::execution::par_unseq)>::value );
    test_pass("cxx17:execution::par_unseq");
#else
    TEST_SKIP1();
    test_skip("cxx17:execution::par_unseq");
#endif
}

TEST_CASE(execution, unsequenced) {
    typedef STD::execution::unsequenced_policy unseq_t;
    test_true( STD::is_execution_policy<unseq_t>::value );
    test_pass("cxx20:unsequenced_policy");

    unseq_t u = STD::execution::unseq;
    (void)u;
    test_true( has_policy_type<unseq_t>(STD::execution::unseq) );
    test_pass("cxx20:execution::unseq");
}

TEST_CASE(execution, is_execution_policy_trait) {

    test_true( STD::is_execution_policy<STD::execution::sequenced_policy>::value );
    test_true( STD::is_execution_policy<STD::execution::parallel_policy>::value );
    test_true( STD::is_execution_policy<STD::execution::parallel_unsequenced_policy>::value );
    test_true( STD::is_execution_policy<STD::execution::unsequenced_policy>::value );

    test_true( !STD::is_execution_policy<int>::value );
    test_true( !STD::is_execution_policy<void>::value );
    test_true( !STD::is_execution_policy<STD::vector<int> >::value );

    test_true( (STD::is_same<STD::is_execution_policy<int>::value_type, bool>::value) );
    test_pass("cxx17:is execution policy trait");

#if _TST_HAS_VAR_TEMPLATE && _tst_cplusplus >= 201402L
    test_true(  STD::is_execution_policy_v<STD::execution::parallel_policy> );
    test_true(  STD::is_execution_policy_v<STD::execution::unsequenced_policy> );
    test_true( !STD::is_execution_policy_v<double> );
    test_eq( STD::is_execution_policy_v<STD::execution::sequenced_policy>,
             STD::is_execution_policy<STD::execution::sequenced_policy>::value );
    test_pass("cxx17:is_execution_policy_v");
#else
    test_skip("cxx17:is_execution_policy_v");
#endif
}

TEST_CASE(execution, algorithms_accept_the_policies) {

    STD::vector<int> v;
    for (int i = 1; i <= 100; ++i)
        v.push_back(i);

    int seq_count = 0;
    STD::for_each(STD::execution::seq, v.begin(), v.end(),
                  count_even(&seq_count));
    test_eq( seq_count, 50 );

    STD::vector<int> doubled(v.size());
    STD::transform(STD::execution::par, v.begin(), v.end(), doubled.begin(),
                   times2);
    test_eq( doubled.front(), 2 );
    test_eq( doubled.back(), 200 );

    STD::vector<int> squared(v.size());
    STD::transform(STD::execution::par_unseq, v.begin(), v.end(), squared.begin(),
                   squarei);
    test_eq( squared.front(), 1 );
    test_eq( squared.back(), 10000 );

    STD::vector<int> negated(v.size());
    STD::transform(STD::execution::unseq, v.begin(), v.end(), negated.begin(),
                   negate_i);
    test_eq( negated.front(), -1 );
    test_eq( negated.back(), -100 );

    int total = STD::reduce(STD::execution::par, v.begin(), v.end(), 0);
    test_eq( total, 5050 );

    STD::vector<int> w(v.rbegin(), v.rend());
    STD::sort(STD::execution::par, w.begin(), w.end());
    test_true( w == v );
    STD::ptrdiff_t n = STD::count_if(STD::execution::par, v.begin(), v.end(),
                                     over90);
    test_eq( (int)n, 10 );
    test_pass("cxx17:algorithms accept the policies");
}
#else
TEST_CASE(execution, policy_types) {
    TEST_NOTE("no execution policies here (libc++ keeps its PSTL behind _LIBCPP_ENABLE_EXPERIMENTAL) or a pre-C++11 front end");
    test_skip("cxx17:sequenced_policy");
    test_skip("cxx17:parallel_policy");
    test_skip("cxx17:parallel_unsequenced_policy");
}
TEST_CASE(execution, policy_objects) {
    TEST_NOTE("no execution policies here (libc++ keeps its PSTL behind _LIBCPP_ENABLE_EXPERIMENTAL) or a pre-C++11 front end");
    test_skip("cxx17:execution::seq");
    test_skip("cxx17:execution::par");
    test_skip("cxx17:execution::par_unseq");
}
TEST_CASE(execution, unsequenced) {
    TEST_NOTE("no execution policies here (libc++ keeps its PSTL behind _LIBCPP_ENABLE_EXPERIMENTAL) or a pre-C++11 front end");
    test_skip("cxx20:unsequenced_policy");
    test_skip("cxx20:execution::unseq");
}
TEST_CASE(execution, is_execution_policy_trait) {
    TEST_NOTE("no execution policies here (libc++ keeps its PSTL behind _LIBCPP_ENABLE_EXPERIMENTAL) or a pre-C++11 front end");
    test_skip("cxx17:is execution policy trait");
    test_skip("cxx17:is_execution_policy_v");
}
TEST_CASE(execution, algorithms_accept_the_policies) {
    TEST_NOTE("no execution policies here (libc++ keeps its PSTL behind _LIBCPP_ENABLE_EXPERIMENTAL) or a pre-C++11 front end");
    test_skip("cxx17:algorithms accept the policies");
}
#endif

TEST_CASE(execution, feature_test_macros) {
#if defined(__cpp_lib_execution) && (__cpp_lib_execution) >= 201603L
    test_true( (__cpp_lib_execution) >= 201603L );
    test_pass("cxx17:__cpp_lib_execution");
#else
    test_skip("cxx17:__cpp_lib_execution");
#endif
#if defined(__cpp_lib_execution) && (__cpp_lib_execution) >= 201902L
    test_true( (__cpp_lib_execution) >= 201902L );
    test_pass("cxx20:__cpp_lib_execution");
#else
    test_skip("cxx20:__cpp_lib_execution");
#endif
#if defined(__cpp_lib_freestanding_execution)
    test_true( __cpp_lib_freestanding_execution > 0L );
    test_pass("cxx26:__cpp_lib_freestanding_execution");
#else
    test_skip("cxx26:__cpp_lib_freestanding_execution");
#endif
#if defined(__cpp_lib_senders)
    test_true( __cpp_lib_senders > 0L );
    test_pass("cxx26:__cpp_lib_senders");
#else
    test_skip("cxx26:__cpp_lib_senders");
#endif
#if defined(__cpp_lib_parallel_scheduler)
    test_true( __cpp_lib_parallel_scheduler > 0L );
    test_pass("cxx26:__cpp_lib_parallel_scheduler");
#else
    test_skip("cxx26:__cpp_lib_parallel_scheduler");
#endif
#if defined(__cpp_lib_counting_scope)
    test_true( __cpp_lib_counting_scope > 0L );
    test_pass("cxx26:__cpp_lib_counting_scope");
#else
    test_skip("cxx26:__cpp_lib_counting_scope");
#endif
}

TEST_CASE(execution, senders_a_cxx26) {
    TEST_NOTE("std::execution senders (P2300) are in no library in reach");
    test_skip("cxx26:affine_t");
    test_skip("cxx26:apply_sender");
    test_skip("cxx26:as_awaitable_t");
    test_skip("cxx26:associate_t");
    test_skip("cxx26:bulk_chunked_t");
    test_skip("cxx26:bulk_item_receiver_proxy");
    test_skip("cxx26:bulk_t");
    test_skip("cxx26:bulk_unchunked_t");
    test_skip("cxx26:completion_signatures");
    test_skip("cxx26:completion_signatures_of_t");
    test_skip("cxx26:connect");
    test_skip("cxx26:connect_result_t");
    test_skip("cxx26:connect_t");
    test_skip("cxx26:continues_on_t");
    test_skip("cxx26:counting_scope");
    test_skip("cxx26:default_domain");
    test_skip("cxx26:dependent_sender");
    test_skip("cxx26:dependent_sender_error");
    test_skip("cxx26:enable_sender");
    test_skip("cxx26:env");
    test_skip("cxx26:env_of_t");
    test_skip("cxx26:error_types_of_t");
    test_skip("cxx26:get_allocator_t");
    test_skip("cxx26:get_await_completion_adaptor_t");
}

TEST_CASE(execution, senders_b_cxx26) {
    TEST_NOTE("std::execution senders (P2300) are in no library in reach");
    test_skip("cxx26:get_completion_domain_t");
    test_skip("cxx26:get_completion_scheduler_t");
    test_skip("cxx26:get_completion_signatures");
    test_skip("cxx26:get_delegation_scheduler_t");
    test_skip("cxx26:get_domain_t");
    test_skip("cxx26:get_env_t");
    test_skip("cxx26:get_forward_progress_guarantee_t");
    test_skip("cxx26:get_parallel_scheduler");
    test_skip("cxx26:get_scheduler_t");
    test_skip("cxx26:get_start_scheduler_t");
    test_skip("cxx26:get_stop_token_t");
    test_skip("cxx26:indeterminate_domain");
    test_skip("cxx26:inlinable_receiver");
    test_skip("cxx26:into_variant_t");
    test_skip("cxx26:just_error_t");
    test_skip("cxx26:just_stopped_t");
    test_skip("cxx26:just_t");
    test_skip("cxx26:let_error_t");
    test_skip("cxx26:let_stopped_t");
    test_skip("cxx26:let_value_t");
    test_skip("cxx26:on_t");
    test_skip("cxx26:operation_state");
    test_skip("cxx26:operation_state_tag");
    test_skip("cxx26:parallel_scheduler");
}

TEST_CASE(execution, senders_c_cxx26) {
    TEST_NOTE("std::execution senders (P2300) are in no library in reach");
    test_skip("cxx26:parallel_scheduler_backend");
    test_skip("cxx26:prop");
    test_skip("cxx26:query_parallel_scheduler_backend");
    test_skip("cxx26:receiver");
    test_skip("cxx26:receiver_proxy");
    test_skip("cxx26:receiver_tag");
    test_skip("cxx26:run_loop");
    test_skip("cxx26:schedule_from_t");
    test_skip("cxx26:schedule_result_t");
    test_skip("cxx26:schedule_t");
    test_skip("cxx26:scheduler");
    test_skip("cxx26:scheduler_tag");
    test_skip("cxx26:scope_association");
    test_skip("cxx26:scope_token");
    test_skip("cxx26:sender");
    test_skip("cxx26:sender_adaptor_closure");
    test_skip("cxx26:sender_in");
    test_skip("cxx26:sender_tag");
    test_skip("cxx26:sends_stopped");
    test_skip("cxx26:set_error_t");
    test_skip("cxx26:set_stopped_t");
    test_skip("cxx26:set_value_t");
    test_skip("cxx26:simple_counting_scope");
    test_skip("cxx26:spawn_future_t");
}

TEST_CASE(execution, senders_d_cxx26) {
    TEST_NOTE("std::execution senders (P2300) are in no library in reach");
    test_skip("cxx26:spawn_t");
    test_skip("cxx26:start_t");
    test_skip("cxx26:starts_on_t");
    test_skip("cxx26:stop_token_of_t");
    test_skip("cxx26:stopped_as_error_t");
    test_skip("cxx26:stopped_as_optional_t");
    test_skip("cxx26:sync_wait_t");
    test_skip("cxx26:sync_wait_with_variant_t");
    test_skip("cxx26:tag_of_t");
    test_skip("cxx26:task");
    test_skip("cxx26:task_scheduler");
    test_skip("cxx26:then_t");
    test_skip("cxx26:transform_sender");
    test_skip("cxx26:upon_error_t");
    test_skip("cxx26:upon_stopped_t");
    test_skip("cxx26:value_types_of_t");
    test_skip("cxx26:variant");
    test_skip("cxx26:when_all_t");
    test_skip("cxx26:when_all_with_variant_t");
    test_skip("cxx26:with_awaitable_senders");
    test_skip("cxx26:with_error");
}
