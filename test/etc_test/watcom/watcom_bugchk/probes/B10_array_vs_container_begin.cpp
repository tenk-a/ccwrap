// bugchk-id:     B10
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   begin(T(&)[N]) and begin(C&) cannot be ranked, so std::begin/end/size cannot serve both
struct Cont {
    int a[3];
    int* begin() { return a; }
};

template <class T, unsigned long N> T* my_begin(T (&arr)[N]) { return arr; }
template <class C> int* my_begin(C& c) { return c.begin(); }

int main() {
    int arr[3] = { 1, 2, 3 };
    Cont c;
    c.a[0] = 5;
    return (*my_begin(arr) == 1 && *my_begin(c) == 5) ? 0 : 1;
}
