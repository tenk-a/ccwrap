#include "test_cxx.hpp"

TEST_CASE(rcu, functions_cxx26) {
    test_skip("cxx26:rcu_barrier");
    test_skip("cxx26:rcu_default_domain");
    test_skip("cxx26:rcu_domain");
    test_skip("cxx26:rcu_obj_base");
    test_skip("cxx26:rcu_retire");
    test_skip("cxx26:rcu_synchronize");
}

TEST_CASE(rcu, rcu_domain_cxx26) {
    test_skip("cxx26:rcu_domain::lock");
    test_skip("cxx26:rcu_domain::operator=");
    test_skip("cxx26:rcu_domain::try_lock");
    test_skip("cxx26:rcu_domain::unlock");
}

TEST_CASE(rcu, rcu_obj_base_cxx26) {
    test_skip("cxx26:rcu_obj_base::operator=");
    test_skip("cxx26:rcu_obj_base::retire");
}

TEST_CASE(rcu, feature_test_macros) {
#if defined(__cpp_lib_rcu) && (__cpp_lib_rcu) >= 202306L
    test_true( (__cpp_lib_rcu) >= 202306L );
    test_pass("cxx26:__cpp_lib_rcu=202306L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_rcu=202306L");
#endif
}
