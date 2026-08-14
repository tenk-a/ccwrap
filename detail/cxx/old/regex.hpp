/**
 *  @file   detail/regex.hpp
 *  @brief  <regex> substitute for pre-C++11 VC (vc8/9), backed by SRELL.
 *  @license Boost Software License Version 1.0 (this glue) / SRELL is BSD-2-Clause.
 *  @note
 *      vc8/9 have no native <regex>. SRELL (https://www.akenotsuki.com/misc/srell/,
 *      thirdparty/srell/) is a std::regex-compatible, header-only engine that builds
 *      on C++03; we include it and alias its names into namespace std so std::regex
 *      / std::smatch / std::regex_search ... work unchanged. SRELL's SIMD path uses
 *      the __cpuidex intrinsic (absent on vc8/9), so SRELL_NO_SIMD is set.
 */
#ifndef _CCW_DETAIL_REGEX_HPP
#define _CCW_DETAIL_REGEX_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if _MSC_VER < 1600   // vc8/9: no native <regex>

#ifndef SRELL_NO_SIMD
#define SRELL_NO_SIMD          /* vc8/9 lack the __cpuidex intrinsic */
#endif
#include <../../thirdparty/srell/srell.hpp>

namespace std {

using srell::basic_regex;
using srell::regex;
using srell::wregex;
using srell::regex_traits;

using srell::sub_match;
using srell::csub_match;   using srell::wcsub_match;
using srell::ssub_match;   using srell::wssub_match;

using srell::match_results;
using srell::cmatch;       using srell::wcmatch;
using srell::smatch;       using srell::wsmatch;

using srell::regex_error;

using srell::regex_search;
using srell::regex_match;
using srell::regex_replace;

using srell::regex_iterator;
using srell::cregex_iterator;   using srell::wcregex_iterator;
using srell::sregex_iterator;   using srell::wsregex_iterator;

using srell::regex_token_iterator;
using srell::cregex_token_iterator;   using srell::wcregex_token_iterator;
using srell::sregex_token_iterator;   using srell::wsregex_token_iterator;

namespace regex_constants {
    using namespace srell::regex_constants;
}

}   // namespace std

#endif  // _MSC_VER < 1600

#endif  // _CCW_DETAIL_REGEX_HPP
