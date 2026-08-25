/**
 *  @file   _ccwrap_detail/fmt_runtime.hpp
 *  @brief  Runtime format-string engine shared by <format> and <print>.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_FMT_RUNTIME_HPP
#define _CCW_DETAIL_FMT_RUNTIME_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <string>
#include <sstream>
#include <cstddef>
#include <cstring>

namespace std {
namespace __ccw_fmt {

template<class T> struct __ccw_oss { typedef std::basic_ostringstream<char> type; };

template<class T>
inline std::string to_str(const T& v) { typename __ccw_oss<T>::type os; os << v; return os.str(); }

inline std::string vformat(const char* fmt, const std::string* args, int nargs) {
    std::string out;
    int auto_idx = 0;
    std::size_t n = std::strlen(fmt);
    for (std::size_t i = 0; i < n; ++i) {
        char c = fmt[i];
        if (c == '{') {
            if (i + 1 < n && fmt[i + 1] == '{') { out += '{'; ++i; continue; }
            std::size_t j = i + 1;
            int idx = -1;
            if (j < n && fmt[j] >= '0' && fmt[j] <= '9') {
                idx = 0;
                while (j < n && fmt[j] >= '0' && fmt[j] <= '9') idx = idx * 10 + (fmt[j++] - '0');
            }
            while (j < n && fmt[j] != '}') ++j;   /* skip an optional :spec */
            int use = (idx >= 0) ? idx : auto_idx++;
            if (use >= 0 && use < nargs) out += args[use];
            i = j;   /* land on '}' (or end) */
        } else if (c == '}') {
            if (i + 1 < n && fmt[i + 1] == '}') { out += '}'; ++i; continue; }
            out += '}';
        } else {
            out += c;
        }
    }
    return out;
}

}   // namespace __ccw_fmt
}   // namespace std

#endif  /* _CCW_DETAIL_FMT_RUNTIME_HPP */
