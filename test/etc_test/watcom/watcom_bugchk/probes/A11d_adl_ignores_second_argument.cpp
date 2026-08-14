// bugchk-id:     A11d
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   ADL ignores the second argument's namespace, so inside a template `s << m` falls back to the member operator<<(unsigned)
//
// A11'''. Argument-dependent lookup does not consider the associated namespace of
// the SECOND operand. Two symptoms, one cause:
//
//   1. At namespace scope, `sink << m` does not find
//      `st::sub::operator<<(Sink<C>&, const M&)` unless a using-directive brings
//      st::sub in: ADL only looked in st (the first operand's namespace).
//      -> E157 "left expression must be integral" (it fell back to the built-in
//         shift after converting through M::operator unsigned()).
//
//   2. From inside a TEMPLATE the call is dependent, so ordinary lookup at the
//      definition context is not used and only the broken ADL remains -- even
//      though the free operator is declared right above, in the same namespace.
//      There the member `Sink::operator<<(unsigned)` is found (Sink is in the
//      first operand's namespace), M converts to unsigned, and it silently wins.
//      This one is the dangerous half: it compiles and prints the wrong thing.
//
// Standard [basic.lookup.argdep]: every argument contributes its associated
// namespaces, so st::sub::operator<< is a candidate in both places, and it is an
// exact match on the second argument where the member needs a user-defined
// conversion. It must win.
//
// Found while adding operator<< for the C++20 calendar (libcxx03
// __chrono/calendar_io.h): chrono::month printed as "2" instead of "Feb", but
// only when reached through operator<<(month_day) -- a template, as every stream
// inserter has to be. The test's `using namespace std::chrono;` hid symptom 1.
//
// Workaround: qualify every internal call -- chrono::operator<<(os, m) -- so that
// no lookup or overload resolution happens.
//
// The namespace nesting matters: Sink stands for std::basic_ostream and st::sub
// for std::chrono. Putting the class and the operator in the SAME namespace does
// not reproduce (ADL on the first argument then finds it anyway).
//
// No iostreams: Open Watcom's own <sstream> does not provide std::ostringstream,
// and the defect has nothing to do with streams.
#include <cstdio>

namespace st {

// Stands for std::basic_ostream.
template <class _CharT>
struct Sink {
    _CharT __buf_[32];
    int    __n_;
    Sink() : __n_(0) { __buf_[0] = (_CharT)'\0'; }
    void __put(char __c) { __buf_[__n_++] = (_CharT)__c; __buf_[__n_] = (_CharT)'\0'; }
    // The built-in path, the one basic_ostream also offers.
    Sink& operator<<(unsigned __v) { __put('#'); __put((char)('0' + __v % 10u)); return *this; }
};

// Stands for std::chrono.
namespace sub {

struct M {
    unsigned __v_;
    M(unsigned __v) : __v_(__v) {}
    operator unsigned() const { return __v_; }
};

// Exact match on the second argument: this must win over Sink's member.
template <class _CharT>
Sink<_CharT>& operator<<(Sink<_CharT>& __s, const M& __m) {
    __s.__put('M');
    __s.__put((char)('0' + __m.__v_));
    return __s;
}

struct Pair {
    M __m_;
    Pair(unsigned __v) : __m_(__v) {}
};

// The composite inserter -- a template, so `__s << __p.__m_` is a dependent call.
template <class _CharT>
Sink<_CharT>& operator<<(Sink<_CharT>& __s, const Pair& __p) {
    __s << __p.__m_;          // <- the call under test
    return __s;
}

} // namespace sub
} // namespace st

int main() {
    using namespace st::sub;   // without this, symptom 1 makes main itself fail

    st::Sink<char> direct;
    direct << st::sub::M(2);            // namespace scope: correct ("M2")

    st::Sink<char> nested;
    nested << st::sub::Pair(2);         // through a template: wrong on Watcom ("#2")

    int ok = (direct.__buf_[0] == 'M' && direct.__buf_[1] == '2')
          && (nested.__buf_[0] == 'M' && nested.__buf_[1] == '2');
    if (!ok) {
        std::printf("BROKEN: direct=\"%s\" nested=\"%s\" (both should be \"M2\")\n",
                    direct.__buf_, nested.__buf_);
        return 1;
    }
    std::printf("ok\n");
    return 0;
}
