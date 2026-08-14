#include "test_cxx.hpp"

TEST_CASE(contracts, functions_cxx26) {
    test_skip("cxx26:contract_assert (core-language statement)");
    test_skip("cxx26:handle_contract_violation (user-replaceable handler)");
    test_skip("cxx26:__cpp_contracts");
    test_skip("cxx26:post contract attribute (core language)");
    test_skip("cxx26:pre contract attribute (core language)");
}

TEST_CASE(contracts, contracts_cxx26) {
    test_skip("cxx26:contracts::assertion_kind::pre");
    test_skip("cxx26:contracts::assertion_kind::post");
    test_skip("cxx26:contracts::assertion_kind::assert");
    test_skip("cxx26:contracts::contract_violation");
    test_skip("cxx26:contracts::contract_violation::comment");
    test_skip("cxx26:contracts::contract_violation::detection_mode");
    test_skip("cxx26:contracts::contract_violation::kind");
    test_skip("cxx26:contracts::contract_violation::location");
    test_skip("cxx26:contracts::contract_violation::semantic");
    test_skip("cxx26:contracts::detection_mode::predicate_false");
    test_skip("cxx26:contracts::detection_mode::evaluation_exception");
    test_skip("cxx26:contracts::evaluation_semantic::ignore");
    test_skip("cxx26:contracts::evaluation_semantic::observe");
    test_skip("cxx26:contracts::evaluation_semantic::enforce");
    test_skip("cxx26:contracts::evaluation_semantic::quick_enforce");
    test_skip("cxx26:contracts::invoke_default_contract_violation_handler");
    test_skip("cxx26:contracts::contract_violation::is_terminating");
    test_skip("cxx26:contracts::contract_violation copy constructor is deleted");
    test_skip("cxx26:contracts::contract_violation copy assignment is deleted");
    test_skip("cxx26:contracts::contract_violation::~contract_violation");
}
