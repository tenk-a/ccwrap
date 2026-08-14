//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: out-of-line definitions for <locale> -- the per-facet
//  locale::id static members (one definition per program, like libstdc++/libc++
//  keep them in their compiled locale TU) and locale::__classic_imp().
#if (defined(__WATCOMC__) && __WATCOMC__ >= 1300) || defined(_MSC_VER)
#include <sstream>    // the complete ios_base + string streams
#include <iterator>   // o/istreambuf_iterator (the num facets' default iter_type)
#include <locale>

namespace _CCW_STD {
locale::id ctype<char>::id;
locale::id ctype<wchar_t>::id;
#if !defined(_MSC_VER)
const _CCW_STD::size_t ctype<char>::table_size;
#endif
locale::id numpunct<char>::id;
locale::id collate<char>::id;
locale::id codecvt<char, char, _CCW_STD::mbstate_t>::id;   // the full specialisation's id

locale::__imp* locale::__classic_imp() {
    static locale::__imp* __c = 0;
    if (!__c) {
        __c = new locale::__imp();
        __c->__add();   // held permanently
        __c->__install(_CCW_STD::ctype<char>::id.__index(),    new _CCW_STD::ctype<char>((const _CCW_STD::ctype<char>::mask*)0, false, 1));
        __c->__install(_CCW_STD::ctype<wchar_t>::id.__index(), new _CCW_STD::ctype<wchar_t>(1));
        __c->__install(_CCW_STD::numpunct<char>::id.__index(), new _CCW_STD::numpunct<char>(1));
        __c->__install(_CCW_STD::collate<char>::id.__index(),  new _CCW_STD::collate<char>(1));
        __c->__install(_CCW_STD::num_put<char>::id.__index(),  new _CCW_STD::num_put<char>(1));
        __c->__install(_CCW_STD::num_get<char>::id.__index(),  new _CCW_STD::num_get<char>(1));
        __c->__install(_CCW_STD::time_put<char>::id.__index(), new _CCW_STD::time_put<char>(1));
        __c->__install(_CCW_STD::time_get<char>::id.__index(), new _CCW_STD::time_get<char>(1));
        __c->__install(_CCW_STD::moneypunct<char, false>::id.__index(), new _CCW_STD::moneypunct<char, false>(1));
        __c->__install(_CCW_STD::moneypunct<char, true>::id.__index(),  new _CCW_STD::moneypunct<char, true>(1));
        __c->__install(_CCW_STD::money_put<char>::id.__index(), new _CCW_STD::money_put<char>(1));
        __c->__install(_CCW_STD::money_get<char>::id.__index(), new _CCW_STD::money_get<char>(1));
        __c->__install(_CCW_STD::messages<char>::id.__index(),  new _CCW_STD::messages<char>(1));
        __c->__install(_CCW_STD::codecvt<char, char, _CCW_STD::mbstate_t>::id.__index(), new _CCW_STD::codecvt<char, char, _CCW_STD::mbstate_t>(1));
    }
    return __c;
}
}
#endif
