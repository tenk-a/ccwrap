#include "cxx_lang_test.hpp"

template<__ccwrap_llong N>
struct ll_t {
	enum { value = int(N) };
    //static const __ccwrap_llong value = N;

};

int main() {
	return ll_t<123456>::value != 123456;
}
