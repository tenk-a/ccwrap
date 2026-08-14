// bugchk-id:     A21
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   a template friend defined inside a class is rejected (E296) -- hidden friends
struct unreachable_sentinel_t {
    template <class I>
    friend bool operator==(unreachable_sentinel_t, const I&) { return false; }
    template <class I>
    friend bool operator!=(unreachable_sentinel_t, const I&) { return true; }
};

int main() {
    int i = 0;
    return (unreachable_sentinel_t() != i) ? 0 : 1;
}
