#include <../ccwrap/ccwrap_misc/test.hpp>
#define __CCWRAP_NS ccwrap
#include <../ccwrap/ccwrap_misc/bit.hpp>
#undef min
#undef max

#include <bit>
#include <limits>


TEST_SUITE(bit) {
	using namespace std;


	template <typename T>
	void test_bit_functions(T value) {

	    // Test bit_cast
	    T cast_result_std = std::bit_cast<T>(value);
	    T cast_result_my  = ccwrap::bit_cast<T>(value);
	    test_eq(cast_result_std, cast_result_my);

	    // Test byteswap
	    T swap_result_std = std::byteswap(value);
	    T swap_result_my  = ccwrap::byteswap(value);
	    test_eq(swap_result_std, swap_result_my);

	    // Test has_single_bit
	    bool single_bit_std = std::has_single_bit(value);
	    bool single_bit_my  = ccwrap::has_single_bit(value);
	    test_eq(single_bit_std, single_bit_my);

	    // Test rotl
	    T rotl_result_std = std::rotl(value, 1);
	    T rotl_result_my  = ccwrap::rotl(value, 1);
	    test_eq(rotl_result_std, rotl_result_my);

	    // Test rotr
	    T rotr_result_std = std::rotr(value, 1);
	    T rotr_result_my  = ccwrap::rotr(value, 1);
	    test_eq(rotr_result_std, rotr_result_my);

	    // Test countr_zero
	    int crz_result_std = std::countr_zero(value);
	    int crz_result_my  = ccwrap::countr_zero(value);
	    test_eq(crz_result_std, crz_result_my);

	    // Test countr_one
	    int cro_result_std = std::countr_one(value);
	    int cro_result_my  = ccwrap::countr_one(value);
	    test_eq(cro_result_std, cro_result_my);

	    // Test countl_zero
	    int clz_result_std = std::countl_zero(value);
	    int clz_result_my  = ccwrap::countl_zero(value);
	    test_eq(clz_result_std, clz_result_my);

	    // Test countl_one
	    int clo_result_std = std::countl_one(value);
	    int clo_result_my  = ccwrap::countl_one(value);
	    test_eq(clo_result_std, clo_result_my);

	    // Test bit_floor
	    T floor_result_std = std::bit_floor(value);
	    T floor_result_my  = ccwrap::bit_floor(value);
	    test_eq(floor_result_std, floor_result_my);

	    // Test bit_width
	    int width_result_std = std::bit_width(value);
	    int width_result_my  = ccwrap::bit_width(value);
	    test_eq(width_result_std, width_result_my);

	    int popcount_std = std::popcount(value);
	    int popcount_my  = ccwrap::popcount(value);
	    test_eq(popcount_std, popcount_my);
	}

	template <typename T>
	void run_tests_for_type(char const* /*name*/) {

		unsigned byt = sizeof(T);
		size_t   len = (byt == 2) ? 255 : (byt == 2) ? 0xffff : 0xfffff;
		for (unsigned i = 0; i <= len; ++i) {
	        test_bit_functions(T(i));
		}
		if (byt > 2) {
			T mi = std::numeric_limits<T>::min();
			T ma = std::numeric_limits<T>::max();
			for (T i = ma; i >= ma - len; --i) {
		        test_bit_functions(T(i));
			}
			for (T i = T((mi+ma)/2 - len); i <= T((mi+ma)/2 + len); ++i) {
		        test_bit_functions(T(i));
			}
		}
	}

	TEST_CASE(bit, check) {
		ccwrap::endian le = ccwrap::endian::little;
		ccwrap::endian be = ccwrap::endian::big;
		ccwrap::endian ne = ccwrap::endian::native;
		ccwrap::endian wk = ne;
		float f1 = 1.0f;
		std::uint32_t u1 = 0x3f800000;
		double d2 = 1.0f;
		std::uint64_t u2 = std::uint64_t(0x3ff0000000000000ULL);

		test_eq(u1, ccwrap::bit_cast<uint32_t>(f1));
		test_eq(u2, ccwrap::bit_cast<uint64_t>(d2));

	    run_tests_for_type<uint8_t>("uint8_t");
	    run_tests_for_type<uint16_t>("uint16_t");
	    run_tests_for_type<uint32_t>("uint32_t");
	    run_tests_for_type<uint64_t>("uint64_t");
	}

}
