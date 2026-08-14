#include <initializer_list>

#if defined(REJ_EXPLICIT_SPECIALIZATION) || defined(REJ_PARTIAL_SPECIALIZATION)
namespace std {
#ifdef REJ_EXPLICIT_SPECIALIZATION
template <> class initializer_list<int> { public: int fake; };
#endif
#ifdef REJ_PARTIAL_SPECIALIZATION
template <class T> class initializer_list<T*> { public: T* fake; };
#endif
}
#endif

int main() {
    std::initializer_list<int> ok = { 1, 2, 3 };
    std::initializer_list<double> widen = { 1, 2 };
    std::initializer_list<int> from_const = { 300 };
    int n = (int)ok.size() + (int)widen.size() + *from_const.begin();
    (void)n;

#ifdef REJ_NARROWING_DOUBLE_TO_INT
    std::initializer_list<int> bad = { 1.5 };
    (void)bad.size();
#endif

#ifdef REJ_NARROWING_CONSTANT_OUT_OF_RANGE
    std::initializer_list<char> bad2 = { 300 };
    (void)bad2.size();
#endif

    return 0;
}
