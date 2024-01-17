#include "cxx_lang_test.hpp"

template <class T>
struct is_array { enum { v = 0 }; };

template <class T>
struct is_array<T[]> { enum { v = 1 }; };

template <class T>
struct is_array<T*> { enum { v = 3 }; };


int main() {
	unsigned n1 = is_array<char[]>::v;
	unsigned n3 = is_array<char*>::v;
	return (n1 == 1 && n3 == 3) ? 0 : 1;
}
