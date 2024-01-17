#include "cxx_lang_test.hpp"

template <class T>
struct is_array { enum { v = 0 }; };

template <class T>
struct is_array<T[]> { enum { v = 1 }; };

extern char buf[];

int main() {
	unsigned n = is_array<char[]>::v;
	unsigned n2 = is_array<char[10]>::v;
	return n == 0 && n2 == 0;
}
