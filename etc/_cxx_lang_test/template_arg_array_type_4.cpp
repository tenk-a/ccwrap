#include "cxx_lang_test.hpp"

template <class T>
struct is_array { enum { v = 0 }; };

template <class T>
struct is_array<T[]> { enum { v = 1 }; };

template <class T, unsigned N>
struct is_array<T[N]> { enum { v = 2 }; };

int main() {
	unsigned n1 = is_array<char[]>::v;
	unsigned n2 = is_array<char[20]>::v;
	return (n1 == 1 && n2 == 2) ? 0 : 1;
}
