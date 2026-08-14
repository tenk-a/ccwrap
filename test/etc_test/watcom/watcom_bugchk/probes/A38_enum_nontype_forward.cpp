// bugchk-id:     A38
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   an enum-typed non-type template parameter cannot be forwarded verbatim (E139)
//
// Forwarding a non-type template parameter whose type is an enum to ANOTHER template,
// spelled as the bare parameter name, is rejected:
//
//     Error! E139: enumeration variable is not assigned a constant from its enumeration
//     Note!  N630: source conversion type is 'int'
//     Note!  N631: target conversion type is 'mode_t'
//
// An explicit cast to the enum type fixes it, and the value arrives intact (see the
// companion run-probe expectations in the catalogue). Relative of A20, which is the
// 64-bit version of the same weakness -- there the value is silently corrupted rather
// than diagnosed, and the fix is `+ 0` instead of a cast.
//
// Hit in libcxx03's <codecvt>, where codecvt_utf8_utf16 forwards its `codecvt_mode
// _Mode` parameter to its codecvt_utf8 base.
//
// Raw compiler only -- no ccwrap headers.
enum mode_t { m_none = 0, m_le = 1, m_gh = 2 };

template <class E, unsigned long Max, mode_t Mode>
struct base_ { enum { mv = Mode, mx = Max }; };

// The offending shape: `Mode` passed straight through.
template <class E, unsigned long Max, mode_t Mode>
struct fwd_verbatim : base_<E, Max, Mode> {};

int main()
{
    return (int)fwd_verbatim<char, 0x10ffff, m_le>::mv;
}
