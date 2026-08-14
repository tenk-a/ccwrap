// bugchk-id:     A39
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   a conversion operator to a SIBLING specialisation is treated as a self-conversion
//
// A class template may convert to a different specialisation of itself; the standard
// library relies on it (ranges::in_out_result & friends are aggregates that convert
// between element types). Open Watcom treats every specialisation of the enclosing
// template as "its own class" and drops the operator:
//
//   - member template form  -> silently absent. Using it gives
//     `E400: cannot convert right expression for initialization`, and even the explicit
//     `r.operator res<const int*, long>()` gives
//     `E375: user-defined conversion has not been declared`.
//   - non-template form     -> `W446: user-defined conversion cannot convert to its own
//     class or base class`, which names the misconception outright.
//
// Converting to an UNRELATED type works, so the defect is the sibling-specialisation
// identity, not conversion operators as such (see the second half of main).
//
// Workaround: a converting CONSTRUCTOR works -- but it costs aggregate-ness, and for the
// ranges result types the aggregate half is the one worth keeping (`return {i, o};`).
//
// Raw compiler only -- no ccwrap headers.
template <class A, class B>
struct res {
    A in;
    B fun;
    template <class A2, class B2>
    operator res<A2, B2>() const { res<A2, B2> r = { in, fun }; return r; }
};

/* An unrelated target: this one is fine, which is the control for the experiment. */
struct plain { const int* in; long fun; };
template <class A, class B>
struct res2 {
    A in; B fun;
    operator plain() const { plain p = { in, fun }; return p; }
};

int main()
{
    int a[3] = {1, 2, 3};

    res<int*, int> f1 = { a, 7 };
    res<const int*, long> f2 = f1;          /* E400 here */

    res2<int*, int> g1 = { a, 7 };
    plain g2 = g1;                          /* this one compiles */

    return (int)(f2.fun + g2.fun);
}
