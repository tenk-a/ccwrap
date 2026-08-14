// bugchk-id:     A23
// bugchk-kind:   compile
// bugchk-expect: ok
// bugchk-desc:   dependent local typedef in a member fn template -- GUARD: catalogued E912 does not reproduce
template <class It> struct iterator_traits { typedef long difference_type; };
template <class T>  struct iterator_traits<T*> { typedef long difference_type; };

template <class RA, class Hash>
struct boyer_moore_searcher {
    RA first_;
    template <class It2>
    It2 operator()(It2 f, It2 l) const {
        typedef typename iterator_traits<It2>::difference_type _Diff;   // E912 on Watcom
        _Diff n = l - f;
        return f + n;
    }
};

int main() {
    boyer_moore_searcher<char*, int> s;
    char buf[4] = { 0, 0, 0, 0 };
    return s(buf, buf + 3) == buf + 3 ? 0 : 1;
}
