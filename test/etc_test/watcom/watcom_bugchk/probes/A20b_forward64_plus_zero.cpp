// bugchk-id:     A20b
// bugchk-kind:   run
// bugchk-expect: ok
// bugchk-desc:   control for A20/B07: pinpoints that only *verbatim* forwarding of a 64-bit non-type param breaks
//
// A20 ("64-bit static const read through ::type is garbage") and B07 ("ratio cannot reduce")
// are two faces of one defect: passing a 64-bit non-type template *parameter* on as a
// template *argument*. This probe isolates it, and pins down the workaround that the rest
// of the tree relies on -- writing `V + 0` instead of `V` restores the value.
//
// If this ever fails, the `+ 0` workaround is gone and <ratio> must be revisited.
#include <stdio.h>

typedef long long im;

template <im V> struct llv { static const im value = V; };
template <int V> struct iv { static const int value = V; };

template <im V>  struct no_forward   { static const im value = V; };              // ok
template <im V>  struct fwd_verbatim { static const im value = llv<V>::value; };  // BROKEN
template <im V>  struct fwd_plus_0   { static const im value = llv<V + 0>::value; };  // ok: the workaround
template <im V>  struct fwd_typedef  { typedef llv<V> t; static const im value = t::value; };  // BROKEN
template <int V> struct fwd_int      { static const int value = iv<V>::value; };  // ok: 32-bit is fine

int main() {
    int bad = 0;
    long long a = no_forward<5>::value;
    long long b = fwd_plus_0<5>::value;
    long long c = fwd_int<5>::value;
    long long d = fwd_plus_0<(im)1 << 40>::value;

    if (a != 5)             { ++bad; printf("A20b no-forward = %lld want 5 -> BROKEN\n", a); }
    if (b != 5)             { ++bad; printf("A20b llv<V+0> (the workaround) = %lld want 5 -> BROKEN\n", b); }
    if (c != 5)             { ++bad; printf("A20b 32-bit forward = %lld want 5 -> BROKEN\n", c); }
    if (d != ((im)1 << 40)) { ++bad; printf("A20b llv<V+0> large = %lld -> BROKEN\n", d); }

    // Report (do not assert) the broken paths, so the asymmetry stays visible.
    printf("A20b for reference -- verbatim llv<V>: %lld, via typedef: %lld (both want 5, both known bad)\n",
           (long long)fwd_verbatim<5>::value, (long long)fwd_typedef<5>::value);
    printf("A20b `V + 0` workaround -> %s\n", bad == 0 ? "OK" : "BROKEN");
    return bad == 0 ? 0 : 1;
}
