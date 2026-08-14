// bugchk-id:     A41
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   sizeof(f(<class value>)) in an enum initialiser is rejected (E169)
//
// The standard detection idiom -- two overloads, one taking the target type and one
// taking ellipsis, ranked by sizeof inside an enum initialiser -- works when the
// ARGUMENT is a scalar but not when it is a class:
//
//     Error! E169: invalid storage class for a member
//
// Neither returning the value by reference, nor taking the parameter by const&, nor
// moving the sizeof out to a namespace-scope function template changes anything. The
// ellipsis is not at fault: the same shape with a scalar argument compiles (see the
// `scalar_ok` control below, which is instantiated first).
//
// The obvious alternative -- probing through a helper whose CONSTRUCTOR takes the
// target type -- compiles but always answers false, because a user-defined conversion
// is never selected here (A11 / A39).
//
// Consequence: is_convertible cannot be built for class types. ccwrap's version falls
// back on identity and derived-to-base when a class is involved, so a user-defined
// conversion reads as false; before that it did not compile at all.
//
// Raw compiler only -- no ccwrap headers.
typedef char yes_t;
struct no_t { char pad[2]; };

struct It  { int* p; It() : p(0) {} };
struct CIt { const int* p; CIt() : p(0) {} CIt(const It& o) : p(o.p) {} };

template <class From, class To>
struct conv {
    static yes_t test(To);
    static no_t  test(...);
    static From& make();
    enum { value = sizeof(conv::test(conv::make())) == sizeof(yes_t) };
};

/* control: the very same shape with a scalar argument is accepted */
template <class To>
struct scalar_ok {
    static yes_t test(To);
    static no_t  test(...);
    enum { value = sizeof(test(0)) == sizeof(yes_t) };
};

int main()
{
    int a = (int)scalar_ok<long>::value;      /* fine */
    int b = (int)conv<It, CIt>::value;        /* E169 here */
    return a + b;
}
