/* bugchk-id:     A36
 * bugchk-kind:   compile
 * bugchk-expect: bug
 * bugchk-desc:   `A a = {};` (empty-brace aggregate init) is rejected, though C++03 allows it
 *
 * C++03 8.5.1/7: an aggregate may be initialised with an empty initializer-list, and every
 * member is then value-initialised. Open Watcom rejects the empty list outright:
 *
 *     agg.cpp(2): Error! E121: col(20) syntax error
 *
 * A non-empty list (`A b = {1,2,3,4}`) is accepted, so it is specifically the EMPTY one.
 *
 * Consequence: the idiomatic "zero this aggregate" spelling is unavailable, which matters
 * for std::array -- `std::array<int,4> a = {};` is how one writes a zeroed array without
 * naming the element count. Work around it by listing the initialisers, by leaving the
 * object uninitialised where the code fills it anyway, or by memset for POD.
 *
 * Raw compiler only -- no ccwrap headers, no library.
 */

struct A { int e[4]; };

A make_nonempty() { A b = {1, 2, 3, 4}; return b; }   /* accepted */

A make_empty()    { A a = {};           return a; }   /* REJECTED: this is the bug */

int main() { return make_nonempty().e[0] + make_empty().e[0]; }
