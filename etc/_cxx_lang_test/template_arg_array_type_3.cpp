#include "cxx_lang_test.hpp"

template <class T>
struct is_array { enum { v = 0 }; };

template <class T, unsigned N>
struct is_array<T[N]> { enum { v = 1 }; };

extern char buf[];

int main() {
	unsigned n = is_array<char[20]>::v;
	return n == 1 ? 0 : 1;
}
