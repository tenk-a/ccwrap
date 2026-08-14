#include "test_cxx.hpp"

TEST_CASE(hazard_pointer, functions_cxx26) {
    test_skip("cxx26:hazard_pointer");
    test_skip("cxx26:hazard_pointer_obj_base");
    test_skip("cxx26:make_hazard_pointer");
    test_skip("cxx26:swap(hazard_pointer&, hazard_pointer&)");
}

TEST_CASE(hazard_pointer, hazard_pointer_cxx26) {
    test_skip("cxx26:hazard_pointer::empty");
    test_skip("cxx26:hazard_pointer::hazard_pointer (default ctor)");
    test_skip("cxx26:hazard_pointer::hazard_pointer (move ctor)");
    test_skip("cxx26:hazard_pointer::operator= (move)");
    test_skip("cxx26:hazard_pointer::protect");
    test_skip("cxx26:hazard_pointer::reset_protection");
    test_skip("cxx26:hazard_pointer::swap (member)");
    test_skip("cxx26:hazard_pointer::try_protect");
    test_skip("cxx26:hazard_pointer::~hazard_pointer");
}

TEST_CASE(hazard_pointer, hazard_pointer_obj_base_cxx26) {
    test_skip("cxx26:hazard_pointer_obj_base::retire");
}

TEST_CASE(hazard_pointer, feature_test_macros) {
#if defined(__cpp_lib_hazard_pointer)
    test_true( __cpp_lib_hazard_pointer > 0L );
    test_pass("cxx26:__cpp_lib_hazard_pointer");
#else
    test_skip("cxx26:__cpp_lib_hazard_pointer");
#endif
}


TEST_CASE(hazard_pointer, ownership_and_reset_cxx26) {
    TEST_NOTE("<hazard_pointer> (P2530) is in no library here");
    test_skip("cxx26:hazard_pointer::hazard_pointer(const hazard_pointer&) = delete");
    test_skip("cxx26:hazard_pointer::operator=(const hazard_pointer&) = delete");
    test_skip("cxx26:hazard_pointer::reset_protection(pointer)");
    test_skip("cxx26:hazard_pointer::reset_protection(nullptr)");
}
