#include "cxx_lang_test.hpp"

template<typename T>
class Bar {
public:
    template<typename U> void test(T* t, U bgn, U end);         // dmc error.
    //template<typename U> void test(T* dst, U start, U last);  // dmc ok.
};

template<typename T>
template<typename U>    // watcom error.
void Bar<T>::test(T* dst, U start, U last) {
    while (start != last) *dst++ = *start++;
}

int main() {
    char buf[10] = {0}, src[] = "hello";
    Bar<char>().test(buf, src, src + sizeof(src) + 1);
    return 0;
}
