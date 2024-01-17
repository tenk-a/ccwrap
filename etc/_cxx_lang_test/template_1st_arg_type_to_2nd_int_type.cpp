#include "cxx_lang_test.hpp"

template <class T, T V>
struct integral_constant {
    typedef integral_constant   type;
    typedef T                   value_type;
    static const T              value = V;
};

int main() {
	return integral_constant<long, 10>::value != 10;
}
