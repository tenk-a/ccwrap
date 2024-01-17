#include "cxx_lang_test.hpp"

template <class T> struct void_type1 { typedef void type; };

//template <class T, class U> struct void_type2 { typedef void type; };


template<bool V>
struct bool_constant {
    typedef bool_constant   type;
    typedef bool            value_type;
    enum { value = V };
};

typedef bool_constant<true>  true_type;
typedef bool_constant<false> false_type;


template<class T, class U = void>
struct has_value_type : false_type { };

template<class T>
struct has_value_type<T, typename void_type1<typename T::value_type>::type> : true_type { };

struct St {
//	typedef int value_type;
};

int main() {
	//return has_value_type<St>::value == false;
	return has_value_type<St>::value != false;
}
