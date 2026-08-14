#include "test_cxx.hpp"

TEST_CASE(linalg, functions_a_cxx26) {
    test_skip("cxx26:ExecutionPolicy-parameterized overloads");
    test_skip("cxx26:add");
    test_skip("cxx26:apply_givens_rotation");
    test_skip("cxx26:column_major");
    test_skip("cxx26:column_major_t");
    test_skip("cxx26:conjugate_transposed");
    test_skip("cxx26:conjugated");
    test_skip("cxx26:conjugated_accessor");
    test_skip("cxx26:copy");
    test_skip("cxx26:dot");
    test_skip("cxx26:dotc");
    test_skip("cxx26:explicit_diagonal");
    test_skip("cxx26:explicit_diagonal_t");
    test_skip("cxx26:hermitian_matrix_product");
    test_skip("cxx26:hermitian_matrix_rank_1_update");
    test_skip("cxx26:hermitian_matrix_rank_2_update");
    test_skip("cxx26:hermitian_matrix_rank_2k_update");
    test_skip("cxx26:hermitian_matrix_rank_k_update");
    test_skip("cxx26:hermitian_matrix_vector_product");
    test_skip("cxx26:implicit_unit_diagonal");
    test_skip("cxx26:implicit_unit_diagonal_t");
    test_skip("cxx26:layout_blas_packed");
    test_skip("cxx26:layout_transpose");
    test_skip("cxx26:lower_triangle");
    test_skip("cxx26:lower_triangle_t");
    test_skip("cxx26:matrix_frob_norm");
    test_skip("cxx26:matrix_inf_norm");
    test_skip("cxx26:matrix_one_norm");
    test_skip("cxx26:matrix_product");
    test_skip("cxx26:matrix_rank_1_update");
}

TEST_CASE(linalg, functions_b_cxx26) {
    test_skip("cxx26:matrix_rank_1_update_c");
    test_skip("cxx26:matrix_vector_product");
    test_skip("cxx26:row_major");
    test_skip("cxx26:row_major_t");
    test_skip("cxx26:scale");
    test_skip("cxx26:scaled");
    test_skip("cxx26:scaled_accessor");
    test_skip("cxx26:setup_givens_rotation");
    test_skip("cxx26:setup_givens_rotation_result");
    test_skip("cxx26:swap_elements");
    test_skip("cxx26:symmetric_matrix_product");
    test_skip("cxx26:symmetric_matrix_rank_1_update");
    test_skip("cxx26:symmetric_matrix_rank_2_update");
    test_skip("cxx26:symmetric_matrix_rank_2k_update");
    test_skip("cxx26:symmetric_matrix_rank_k_update");
    test_skip("cxx26:symmetric_matrix_vector_product");
    test_skip("cxx26:transposed");
    test_skip("cxx26:triangular_matrix_left_product");
    test_skip("cxx26:triangular_matrix_matrix_left_solve");
    test_skip("cxx26:triangular_matrix_matrix_right_solve");
    test_skip("cxx26:triangular_matrix_product");
    test_skip("cxx26:triangular_matrix_right_product");
    test_skip("cxx26:triangular_matrix_vector_product");
    test_skip("cxx26:triangular_matrix_vector_solve");
    test_skip("cxx26:upper_triangle");
    test_skip("cxx26:upper_triangle_t");
    test_skip("cxx26:vector_abs_sum");
    test_skip("cxx26:vector_idx_abs_max");
    test_skip("cxx26:vector_two_norm");
}

TEST_CASE(linalg, layout_blas_packed_cxx26) {
    test_skip("cxx26:layout_blas_packed::mapping");
}

TEST_CASE(linalg, feature_test_macros) {
#if defined(__cpp_lib_linalg)
    test_true( __cpp_lib_linalg > 0L );
    test_pass("cxx26:__cpp_lib_linalg");
#else
    test_skip("cxx26:__cpp_lib_linalg");
#endif
}
