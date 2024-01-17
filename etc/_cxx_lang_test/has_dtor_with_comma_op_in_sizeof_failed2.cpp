#include "cxx_lang_test.hpp"

template<unsigned N>
struct ok1fail2 {
	char dummy[N == 1 ? 1 : 2];
};

template <class T>
class has_member_foo {
	template<class U>
	struct bar {
		U	u;
	};
	template<class U>
	static ok1fail2<sizeof((reinterpret_cast< bar<U>* >(0)->~bar<U>(), char()))> test(int);
	template<class U>
    static ok1fail2<2> test(...);
public:
	enum { value = sizeof(test<T>(0)) == 1 };
};

struct baz {
private:
	~baz() {}
};

int main() {
	return has_member_foo<baz>::value != 0;
}
