// bugchk-id:     A22
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   typename T::type in a default template argument is rejected (E912) -- boyer_moore_searcher's Hash
template <class It> struct iterator_traits { typedef char value_type; };
template <class T>  struct iterator_traits<T*> { typedef T value_type; };
template <class T>  struct hash { unsigned operator()(T) const { return 0; } };

// The standard default for boyer_moore_searcher's Hash.
template <class It, class Hash = hash<typename iterator_traits<It>::value_type> >
struct boyer_moore_searcher {
    Hash h;
};

int main() {
    boyer_moore_searcher<char*> s;
    (void)s;
    return 0;
}
