// ccwrap watcom: <locale.h>. ccwrap supplies the whole C locale layer here
// (struct lconv with the C99 int_* members, setlocale, localeconv), so the
// native header's guards are pre-empted and its body never runs.
#pragma once

#include <ccwrap_common.h>

#ifndef _LOCALE_H_INCLUDED
#define _LOCALE_H_INCLUDED
#endif
#ifndef _CLOCALE_INCLUDED
#define _CLOCALE_INCLUDED
#endif

#include "../../detail/c/locale_impl.h"

#ifdef __cplusplus
#ifndef _CCW_LOCALE_STD_USING
#define _CCW_LOCALE_STD_USING
namespace std {
    using ::lconv;
    using ::_ccw_setlocale;
    using ::_ccw_localeconv;
    using ::_ccw_locale_codepage;
    using ::_ccw_locale_is_utf8;
}
#endif
#endif

#ifndef setlocale
#define setlocale   _ccw_setlocale
#define localeconv  _ccw_localeconv
#endif
