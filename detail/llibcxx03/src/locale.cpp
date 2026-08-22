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
locale::id ctype<char>::id(_CCW_FACET_SLOT(_CCW_FACET_SLOT_CTYPE, 1));
locale::id ctype<wchar_t>::id(_CCW_FACET_SLOT(_CCW_FACET_SLOT_CTYPE, 2));
#if !defined(_MSC_VER)
const _CCW_STD::size_t ctype<char>::table_size;
#endif
locale::id numpunct<char>::id(_CCW_FACET_SLOT(_CCW_FACET_SLOT_NUMPUNCT, 1));
locale::id numpunct<wchar_t>::id(_CCW_FACET_SLOT(_CCW_FACET_SLOT_NUMPUNCT, 2));
locale::id collate<char>::id(_CCW_FACET_SLOT(_CCW_FACET_SLOT_COLLATE, 1));
locale::id collate<wchar_t>::id(_CCW_FACET_SLOT(_CCW_FACET_SLOT_COLLATE, 2));
locale::id codecvt<char, char, _CCW_STD::mbstate_t>::id(_CCW_FACET_SLOT(_CCW_FACET_SLOT_CODECVT, 1));   // the full specialisation's id
locale::id codecvt<wchar_t, char, _CCW_STD::mbstate_t>::id(_CCW_FACET_SLOT(_CCW_FACET_SLOT_CODECVT, 2));


locale::__imp* locale::__classic_imp() {
    static locale::__imp* __c = 0;
    if (!__c) {
        __c = new locale::__imp();
        __c->__add();   // held permanently
        __c->__install(_CCW_STD::ctype<char>::id.__index(),    new _CCW_STD::ctype<char>((const _CCW_STD::ctype<char>::mask*)0, false, 1));
        __c->__install(_CCW_STD::ctype<wchar_t>::id.__index(), new _CCW_STD::ctype<wchar_t>(1));
        __c->__install(_CCW_STD::numpunct<char>::id.__index(), new _CCW_STD::numpunct<char>(1));
        __c->__install(_CCW_STD::numpunct<wchar_t>::id.__index(), new _CCW_STD::numpunct<wchar_t>(1));
        __c->__install(_CCW_STD::collate<char>::id.__index(),  new _CCW_STD::collate<char>(1));
        __c->__install(_CCW_STD::collate<wchar_t>::id.__index(),  new _CCW_STD::collate<wchar_t>(1));
        __c->__install(_CCW_STD::num_put<char>::id.__index(),  new _CCW_STD::num_put<char>(1));
        __c->__install(_CCW_STD::num_put<wchar_t>::id.__index(), new _CCW_STD::num_put<wchar_t>(1));
        __c->__install(_CCW_STD::num_get<char>::id.__index(),  new _CCW_STD::num_get<char>(1));
        __c->__install(_CCW_STD::num_get<wchar_t>::id.__index(), new _CCW_STD::num_get<wchar_t>(1));
        __c->__install(_CCW_STD::time_put<char>::id.__index(), new _CCW_STD::time_put<char>(1));
        __c->__install(_CCW_STD::time_put<wchar_t>::id.__index(), new _CCW_STD::time_put<wchar_t>(1));
        __c->__install(_CCW_STD::time_get<char>::id.__index(), new _CCW_STD::time_get<char>(1));
        __c->__install(_CCW_STD::time_get<wchar_t>::id.__index(), new _CCW_STD::time_get<wchar_t>(1));
        __c->__install(_CCW_STD::moneypunct<char, false>::id.__index(), new _CCW_STD::moneypunct<char, false>(1));
        __c->__install(_CCW_STD::moneypunct<wchar_t, false>::id.__index(), new _CCW_STD::moneypunct<wchar_t, false>(1));
        __c->__install(_CCW_STD::moneypunct<char, true>::id.__index(),  new _CCW_STD::moneypunct<char, true>(1));
        __c->__install(_CCW_STD::moneypunct<wchar_t, true>::id.__index(),  new _CCW_STD::moneypunct<wchar_t, true>(1));
        __c->__install(_CCW_STD::money_put<char>::id.__index(), new _CCW_STD::money_put<char>(1));
        __c->__install(_CCW_STD::money_put<wchar_t>::id.__index(), new _CCW_STD::money_put<wchar_t>(1));
        __c->__install(_CCW_STD::money_get<char>::id.__index(), new _CCW_STD::money_get<char>(1));
        __c->__install(_CCW_STD::money_get<wchar_t>::id.__index(), new _CCW_STD::money_get<wchar_t>(1));
        __c->__install(_CCW_STD::messages<char>::id.__index(),  new _CCW_STD::messages<char>(1));
        __c->__install(_CCW_STD::messages<wchar_t>::id.__index(),  new _CCW_STD::messages<wchar_t>(1));
        __c->__install(_CCW_STD::codecvt<char, char, _CCW_STD::mbstate_t>::id.__index(), new _CCW_STD::codecvt<char, char, _CCW_STD::mbstate_t>(1));
        __c->__install(_CCW_STD::codecvt<wchar_t, char, _CCW_STD::mbstate_t>::id.__index(), new _CCW_STD::codecvt<wchar_t, char, _CCW_STD::mbstate_t>(1));
    }
    return __c;
}


locale::locale(const char* __n) : __imp_(0) {
    if (_CCW_STD::__ccw_locale_name_ok(__n)) { __imp_ = __classic_imp(); __imp_->__add(); return; }
    __imp_ = __named_imp(__n);
    if (!__imp_) _CCW_THROW(_CCW_STD::runtime_error("locale: unsupported name"));
    __imp_->__add();
}

locale::locale(const _CCW_STD::string& __n) : __imp_(0) {
    if (_CCW_STD::__ccw_locale_name_ok(__n.c_str())) { __imp_ = __classic_imp(); __imp_->__add(); return; }
    __imp_ = __named_imp(__n.c_str());
    if (!__imp_) _CCW_THROW(_CCW_STD::runtime_error("locale: unsupported name"));
    __imp_->__add();
}


#if defined(__WATCOMC__)
void __ccw_throw_range_error(const char* __what) {
    _CCW_THROW(_CCW_STD::range_error(__what));
}
#endif

}

#endif
