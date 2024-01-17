#include "cxx_lang_test.hpp"

template<unsigned N>
struct foo {
	alignas(N) char buf[256];
};

int main() {
	foo<8>();
	return 0;
}
