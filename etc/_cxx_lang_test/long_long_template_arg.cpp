#include "cxx_lang_test.hpp"

template<__CCWRAP_LLONG N>
struct ll_t {
	enum { value = int(N) };
    //static const __CCWRAP_LLONG value = N;

};

int main() {
	return ll_t<123456>::value != 123456;
}
