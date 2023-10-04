/**
 *  @file   filesystem_path.hpp
 *  @brief  filesystem::path
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @license Boost Software Lisence Version 1.0
 *  @note
 *      Windows: Not compatible with character codes with 0x5c in the lower byte. (ex: SJIS etc)
 */
#ifndef CCWRAP_filesystem_path_hpp_INCLUDED
#define CCWRAP_filesystem_path_hpp_INCLUDED

//#define CCWRAP_FILESYSTEM_PATH_IS_UTF8   //TODO

#include <cstddef>
#include <cstring>
#include <cassert>

#include <string>
#include <iostream>

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(push)
# pragma warning(disable:4244)
#endif

#if __CCWRAP__ != 2  //__CCWRAP_ID_CCWRAP_NS
 #include <string_view>
#else
 #include <_ccwrap/string_view.hpp>
#endif
#ifndef __CCWRAP_STRINGVIEW_COPY
 #if __CCWRAP__ != 3  //__CCWRAP_ID_BOOST2STD
  #define __CCWRAP_STRINGVIEW_COPY(l,r)  (l) = (r)
  #define __CCWRAP_STRINGVIEW_ADD(l,r)   (l) += (r)
 #else
  #define __CCWRAP_STRINGVIEW_COPY(l,r)  (l).assign((r).begin(), (r).end())
  #define __CCWRAP_STRINGVIEW_ADD(l,r)   (l).append((r).begin(), (r).end())
 #endif
#endif

#include <_ccwrap/StringCodeConv.hpp>

#ifndef __ccwrap_nodiscard
 #if __cplusplus >= 202000L || defined(__CCWRAP_CXX20)
  #define __ccwrap_nodiscard   [[nodiscard]]
 #else
  #define __ccwrap_nodiscard
 #endif
#endif
#ifndef __ccwrap_constexpr_or_const
 #if __cplusplus >= 201101L || defined(CCWRAP_CXX11)
  #define __ccwrap_constexpr_or_const  constexpr
  #define __ccwrap_move_or_swap(l,r)   (l) = std::move(r)
 #else
  #define __ccwrap_constexpr_or_const  const
  #define __ccwrap_move_or_swap(l,r)   (l).swap(r)
 #endif
#endif

#ifndef CCWRAP_FILESYSTEM_PATH_STRING
 #if defined(_WIN32)
  #define CCWRAP_FILESYSTEM_PATH_STRING    wstring
  #define CCWRAP_FILESYSTEM_PATH_SEP       '\\'
  #define CCWRAP_FILESYSTEM_PATH_GENERIC_NE_NATIVE
 #else
  #define CCWRAP_FILESYSTEM_PATH_STRING    string
  #define CCWRAP_FILESYSTEM_PATH_SEP       '/'
 #endif
#endif

namespace ccwrap { namespace filesystem {

class path;

namespace __path_detail {

typedef CCWRAP_FILESYSTEM_PATH_STRING   str_t;
typedef str_t::value_type               char_t;
typedef str_t::value_type const*        charp_t;
typedef basic_string_view<char_t>       str_view_t;
typedef int                             big_char_t;

inline bool is_sep(char_t c) noexcept {
 #if defined CCWRAP_FILESYSTEM_PATH_GENERIC_NE_NATIVE
    return c == char_t('/') || c == char_t(CCWRAP_FILESYSTEM_PATH_SEP);
 #else
    return c == char_t('/');
 #endif
}

template<class DMY>
charp_t skipSep(charp_t b, charp_t e) noexcept {
    if (!b)
        return e;
    charp_t s = b;
    while (s < e && is_sep(*s))
        ++s;
    return s;
}

template<class DMY>
charp_t backSkipSep(charp_t b, charp_t p) noexcept {
    if (!p || b >= p)
        return b;
    while (--p >= b && is_sep(*p))
        ;
    ++p;
    return p;
}

template<class DMY>
charp_t skipNotSep(charp_t b, charp_t e) noexcept {
    if (!b)
        return e;
    charp_t s = b;
    while (s < e && !is_sep(*s))
        ++s;
    return s;
}

template<class DMY>
charp_t backSkipNotSep(charp_t b, charp_t p) noexcept {
    if (!p || b >= p)
        return b;
 #if defined CCWRAP_FILESYSTEM_PATH_GENERIC_NE_NATIVE
    while (--p >= b && !is_sep(*p) && *p != ':')
        ;
 #else
    while (--p >= b && !is_sep(*p))
        ;
 #endif
    ++p;
    return p;
}

template<class DMY>
inline big_char_t get_ch(charp_t& s, charp_t e) {
    if (s < e) {
        char_t c = *s++;
        if (is_sep(c)) {
            c = char_t(CCWRAP_FILESYSTEM_PATH_SEP);
            s = skipSep<void>(s, e);
        }
        return c;
    }
    return big_char_t(-1);
}

template<class DMY>
charp_t skipRootName(charp_t b, charp_t e) noexcept {
    if (!b)
        return e;
    charp_t s = b;
    if (s < e && *s != char_t(':')) {
        while (s < e && !is_sep(*s)) {
            if (*s == char_t(':'))
                return s + 1;
            ++s;
        }
    }
    return b;
}

template<class DMY>
inline charp_t skipRootPath(charp_t b, charp_t e) noexcept {
    charp_t p = skipRootName<void>(b, e);
    return skipSep<void>(p, e);
}


template<class DMY>
charp_t skipToFilename(charp_t b, charp_t e) noexcept {
    if (!b)
        return e;
    charp_t p = b; //skipRootName<void>(b, e);
    while (p < e) {
        if (is_sep(*p))
            b = p + 1;
        ++p;    // p = char_next(p);
    }
    return b;
}

template<class DMY>
charp_t skipToExtension(charp_t b, charp_t e) noexcept {
    if (!b)
        return e;
    charp_t s = skipToFilename<void>(b, e);
    while (s < e && *s == char_t('.'))
        ++s;
    charp_t a = s;
    charp_t p = s;
    while (p < e) {
        if (*p == char_t('.'))
            a = p;
        ++p;    // p = char_next(p);
    }
    if (a == s)
        a = e;
    return a;
}


template<class DMY>
bool is_absolute(str_view_t str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t p = skipRootName<void>(b, e);
        return is_sep(*p);
    }
    return false;
}

#ifndef __CCWRAP_STRING_BACK
 #if !defined(__CCWRAP_LESS_CXX11)
  #define __CCWRAP_STRING_BACK(s)        (s).back()
 #else
  #define __CCWRAP_STRING_BACK(s)        (s)[(s).size()-1]
 #endif
#endif
#ifndef __CCWRAP_STRING_POP_BACK
 #if !defined(__CCWRAP_LESS_CXX11)
  #define __CCWRAP_STRING_POP_BACK(s)    (s).pop_back()
 #else
  #define __CCWRAP_STRING_POP_BACK(s)    (s).resize((s).size()-1)
 #endif
#endif

inline void add_sep(str_t& str) {
    if (!str.empty() && !is_sep(__CCWRAP_STRING_BACK(str))) {
        str.push_back(char_t(CCWRAP_FILESYSTEM_PATH_SEP));
    }
}

template<class DMY>
void make_preferred(str_t& str) {
 #if defined CCWRAP_FILESYSTEM_PATH_GENERIC_NE_NATIVE
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        while (b < e) {
            if (*b == char_t('/'))
                *const_cast<char_t*>(b) = char_t(CCWRAP_FILESYSTEM_PATH_SEP);
            ++b;
        }
    }
 #endif
}

template<class S>
void make_generic(S& str) {
 #if defined CCWRAP_FILESYSTEM_PATH_GENERIC_NE_NATIVE
    if (!str.empty()) {
        typedef typename S::value_type ch_t;
        ch_t* b = (ch_t*)str.data();
        ch_t* e = b + str.size();
        while (b < e) {
            if (*b == char_t(CCWRAP_FILESYSTEM_PATH_SEP))
                *b = char_t('/');
            ++b;
        }
    }
 #endif
}

template<class DMY>
str_view_t root_name(str_view_t str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t p = skipRootName<void>(b, e);
        return str_view_t(b, p - b);
    }
    return str_view_t();
}

template<class DMY>
str_view_t root_directory(str_view_t str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        b         = skipRootName<void>(b, e);
        charp_t p = skipSep<void>(b, e);
        return str_view_t(b, p - b);
    }
    return str_view_t();
}

template<class DMY>
str_view_t root_path(str_view_t str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t p = skipRootName<void>(b, e);
        p         = skipSep<void>(p, e);
        return str_view_t(b, p - b);
    }
    return str_view_t();
}

template<class DMY>
str_view_t relative_path(str_view_t str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t p = skipRootPath<void>(b, e);
        return str_view_t(p, e - p);
    }
    return str_view_t();
}

template<class DMY>
str_view_t filename(str_view_t str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t p = skipToFilename<void>(skipRootPath<void>(b, e), e);
        return str_view_t(p, e - p);
    }
    return str_view_t();
}


template<class DMY>
str_view_t extension(str_view_t str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t p = skipToExtension<void>(skipRootPath<void>(b, e), e);
        return str_view_t(p, e - p);
    }
    return str_view_t();
}

template<class DMY>
str_view_t stem(str_view_t str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t s = skipToFilename<void>(skipRootPath<void>(b, e), e);
        charp_t p = skipToExtension<void>(s, e);
        return str_view_t(s, p - s);
    }
    return str_view_t();
}

template<class DMY>
str_view_t parent_path(str_view_t str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t r = skipRootPath<void>(b, e);
        charp_t f = skipToFilename<void>(r, e);
        f         = backSkipSep<void>(r, f);
        return str_view_t(b, f - b);
    }
    return str_view_t();
}

inline bool has_root_name(     str_view_t str) { return !root_name<void>(     str).empty(); }
inline bool has_root_path(     str_view_t str) { return !root_path<void>(     str).empty(); }
inline bool has_root_directory(str_view_t str) { return !root_directory<void>(str).empty(); }
inline bool has_relative_path( str_view_t str) { return !relative_path<void>( str).empty(); }
inline bool has_filename(      str_view_t str) { return !filename<void>(      str).empty(); }
inline bool has_extension(     str_view_t str) { return !extension<void>(     str).empty(); }
inline bool has_stem(          str_view_t str) { return !stem<void>(          str).empty(); }
inline bool has_parent_path(   str_view_t str) { return !parent_path<void>(   str).empty(); }


template<class DMY>
void remove_filename(str_t& str) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t r = skipRootPath<void>(b, e);
        charp_t f = skipToFilename<void>(r, e);
        str.resize(f - b);
    }
}

template<class DMY>
void replace_extension(str_t& str, str_view_t repl) {
    if (!str.empty()) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t p = skipToExtension<void>(skipRootPath<void>(b, e), e);
        size_t  t = p - b;
        bool    nopir = (repl[0] != char_t('.'));
        size_t  sz    = repl.size() + size_t(nopir);
        str.resize(t + sz);
        if (nopir)
            str[t++] = char_t('.');
        std::copy(repl.begin(), repl.end(), str.begin()+t);
    }
}

template<class DMY>
int compare(str_view_t lstr, str_view_t rstr) noexcept {
    if (lstr.empty()) {
        return (rstr.empty()) ? 0 : -1;
    } else if (rstr.empty()) {
        return 1;
    }

    charp_t lb = lstr.data();
    charp_t le = lb + lstr.size();
    charp_t lp = lb;
    charp_t rb = rstr.data();
    charp_t re = rb + rstr.size();
    charp_t rp = rb;

    big_char_t lc, rc;
    do {
        lc = get_ch<void>(lp, le);
        rc = get_ch<void>(rp, re);
        if (lc != rc)
            return lc < rc ? -1 : /*lc > rc ?*/ 1;
    } while (lc >= 0);

    if (lp == le) {
        if (rp == re)
            return 0;
        return 1;
    } else {
        return -1;
    }
}

template<class DMY>
void append(str_t& str, str_view_t add) {
    if (add.empty())
        return;
    str_view_t add_root = root_name<void>(add);
    if (add_root.size()) {
        if (is_absolute<void>(add)) {
            __CCWRAP_STRINGVIEW_COPY(str, add);  // str = add;
            return;
        }
        str_view_t str_root = root_name<void>(str);
        if (compare<void>(str_root,add_root)) {
            __CCWRAP_STRINGVIEW_COPY(str, add);  // str = add;
            return;
        }
        add = relative_path<void>(add);
    } else if (has_root_directory(add)) {
        charp_t b = str.data();
        charp_t e = b + str.size();
        charp_t p = skipRootName<void>(b, e);
        str.resize(p - b);
        str.append(add.data(), add.size());
        return;
    }
    str.reserve(1 + add.size() + 1);
    add_sep(str);
    str.append(add.data(), add.size());
}

template<class DMY>
void to_absolute(str_t& str, str_view_t base) {
    if (!is_absolute<void>(str)) {
        str_t   tmp( base.data(), base.size() );
        append<void>(tmp, str);
        __ccwrap_move_or_swap(str, tmp);
    }
}


template<class DMY>
void lexically_normal(str_t& str) {
    if (str.empty())
        return;
    charp_t b   = str.data();
    charp_t e   = b + str.size();
    charp_t s   = skipRootName<void>(b, e);
    charp_t top = s;
    char_t* d   = (char_t*)s;
  #if 0
    bool  first = true;
    char_t  sep = char_t('/');
    if (is_sep(*s)) {
     #if defined CCWRAP_FILESYSTEM_PATH_GENERIC_NE_NATIVE
        if (*s == char_t(CCWRAP_FILESYSTEM_PATH_SEP))
            sep = char_t(CCWRAP_FILESYSTEM_PATH_SEP);
     #endif
     #if 0
        ++s;
        *d++ = sep;         /* '/' '//' '///' */
        if (is_sep(*s)) {
            ++s;
            *d++ = sep;     /* '//' '///' */
        }
     #endif
    }
  #endif
    while (s < e) {
        char_t c = *s++;
        if (is_sep(c)) {    // '/' or '\\'
            //c = sep;
            if (s < e) {
                char_t c2 = *s;
                while (is_sep(c2) && s < e) {
                    c2 = *++s;
                }
                if (c2 == char_t('.')) {
                    if (s+1 >= e) { // Only "."
                        break;
                    }
                    char_t c3 = s[1];
                    if (is_sep(c3)) {   // "./"
                        ++s;
                        continue;
                    } else if (c3 == char_t('.') && (s+2 >= e || is_sep(s[2]))) {    // ".." or "../"
                        s += 2;
                        while (--d >= top && !is_sep(*d))   // remove 1 directory
                            ;
                        // ++d;
                        continue;
                    }
                }
            }
        }
        *d++ = c;
    }
    if (d < e)
        *d = char_t(0);
    str.resize(d - b);
}

template<class DMY>
void lexically_relative(str_t& str, str_view_t base, bool clear) {
    if (str.empty() || base.empty())
        return;
    if (!is_absolute<void>(base)) {
        if (clear)
            str.clear();
        return;
    }
    str_t   rstr( base.data(), base.size() );
    add_sep(rstr);
    lexically_normal<void>(rstr);
    make_generic(rstr);
    str_t   lstr = str;
    to_absolute<void>(lstr, rstr);
    lexically_normal<void>(lstr);
    make_generic(lstr);

    charp_t lb = lstr.data();
    charp_t le = lb + lstr.size();
    //charp_t lp = skipRootName<void>(lb, le);
    charp_t lp = skipRootPath<void>(lb, le);
    charp_t rb = rstr.data();
    charp_t re = rb + rstr.size();
    //charp_t rp = skipRootName<void>(rb, re);
    charp_t rp = skipRootPath<void>(rb, re);

    if (compare<void>(str_view_t(lb,lp-lb), str_view_t(rb,rp-rb)) != 0) {
        if (clear)
            str.clear();
        return;
    }

    charp_t rpSav  = rp;
    big_char_t lc, rc;
    do {
        lc = get_ch<void>(lp, le);
        rc = get_ch<void>(rp, re);
        if (lc != rc)
            break;
        if (is_sep(lc) && is_sep(rc))
            rpSav = rp;
    } while (lc >= 0);
    lp = lp-1 - (rp - rpSav);
    rp = rpSav;

    size_t  dirCnt = 0;
    charp_t rp2    = rp;
    while (rp2 < re) {
        char_t rc2 = *rp2++;
        if (is_sep(rc2))
            ++dirCnt;
    }

    str_t   tmp;
    tmp.reserve( dirCnt * 3 + le - lp + 4 );
    while (rp < re) {
        if (is_sep(*rp)) {
            static char_t const updir[3] = { char_t(','), char_t(','), char_t('/') };
            tmp.append(updir, 3);
        }
        ++rp;
    }
    tmp.append(lp, le-lp);
    __ccwrap_move_or_swap(str, tmp);
}


template<typename DMY=void>
class path_iterator {
    friend class ::ccwrap::filesystem::path;
    typename str_t::iterator    iterator;
    path_iterator(str_t const* s, bool is_end);

public:
    typedef path valut_type;
    path_iterator() noexcept : str_(nullptr), cur_(0), rootNameLen_(0), rootPathLen_(0) {}
    path_iterator(path_iterator const& r) noexcept : str_(r.str_), cur_(r.cur_), rootNameLen_(0), rootPathLen_(0) {}

    path           operator*() const;
    path_iterator& operator++();
    path_iterator  operator++(int) const { path_iterator it = *this; operator++(); return it; }
    path_iterator& operator--();
    path_iterator  operator--(int) const { path_iterator it = *this; operator--(); return it; }

    bool operator==(path_iterator const& r) const { return (str_ == r.str_) && (cur_ == r.cur_); }
    bool operator!=(path_iterator const& r) const { return !operator==(r); }

private:
    str_t const*    str_;
    size_t          cur_;
    short           rootNameLen_;
    short           rootPathLen_;
};


template<typename DMY>
path_iterator<DMY>::path_iterator(str_t const* str, bool is_end)
    : str_(str)
{
    charp_t b   = str->data();
    size_t  l   = str->size();
    cur_        = (is_end) ? l : 0;
    charp_t e   = b + l;
    charp_t r   = skipRootName<void>(b, e);
    rootNameLen_= short(r - b);
    charp_t d   = skipSep<void>(r, e);
    rootPathLen_= short(d - b);
}


template<typename DMY>
path_iterator<DMY>& path_iterator<DMY>::operator++() {
    if (cur_ < rootPathLen_) {
        cur_        = (cur_ < rootNameLen_) ? rootNameLen_ : rootPathLen_;
    } else {
        charp_t b   = str_->data();
        charp_t e   = b + str_->size();
        charp_t p   = skipNotSep<void>(b + cur_, e);
        charp_t s   = skipSep<void>(p, e);
        cur_        = s - b;
    }
    return *this;
}

template<typename DMY>
path_iterator<DMY>& path_iterator<DMY>::operator--() {
    if (cur_ <= rootPathLen_) {
        if (cur_ < rootPathLen_ || rootPathLen_ == rootNameLen_) {
            assert(cur_ > 0);
            cur_        = 0;
        } else {
            cur_ = rootNameLen_;
        }
    } else {
        charp_t b   = str_->data();
        charp_t e   = b + str_->size();
        charp_t s   = backSkipSep<void>(b, b + cur_);
        charp_t p   = backSkipNotSep<void>(b, s);
        cur_        = p - b;
    }
    return *this;
}


}   // __path_detail


class path {
    //friend class __path_detail::path_iterator<void>;
    typedef __path_detail::str_view_t   string_view_type;
public:
    typedef __path_detail::str_t            string_type;
    typedef string_type::value_type         value_type;
    typedef __path_detail::path_iterator<void>  iterator;
    typedef iterator                        const_iterator;
    enum format { native_format, generic_format, auto_format };
    static __ccwrap_constexpr_or_const value_type preferred_separator = value_type(CCWRAP_FILESYSTEM_PATH_SEP);

    path() noexcept {}
    path(path const& r) : str_(r.str_) {}
 #if !defined(__CCWRAP_LESS_CXX11)
    path(path&& r) noexcept : str_(std::move(r.str_)) {}
    path(string_type&& r, format /*fmt*/ = auto_format) : str_(std::move(r)) { /*adjust_fmt(fmt);*/ }
 #else
    path(string_type const& r, format fmt = auto_format) : str_(r) { /*adjust_fmt(fmt);*/ }
 #endif
    template <class S> path(const S& r, format /*fmt*/ = auto_format) : str_(conv_to<string_type>(r)){/*adjust_fmt(fmt);*/}
    template <class InpIte> path(InpIte f, InpIte l, format fmt = auto_format) : str_(f,l) { /*adjust_fmt(fmt);*/ }
    //TODO: template <class S> path(const S& r, const locale& loc, format fmt = auto_format);
    //TODO: template <class InpIte> path(InpIte first, InpIte last, const locale& loc, format fmt = auto_format);

    path& operator=(path const& r) { if (this != &r) str_ = r.str_; return *this; }
    template <class S> path& operator=(const S& r) { path(r).swap(*this); return *this; }
    template <class S> path& assign(const S& r) { return operator=(r); }

 #if !defined(__CCWRAP_LESS_CXX11)
    path& operator=(path&& r) noexcept { if (this != &r) str_ = std::move(r.str_); return *this; }
    path& operator=(string_type&& r) noexcept { if (&str_ != &r) str_ = std::move(r); /*adjust_fmt();*/ return *this; }
    path& assign(string_type&& r) { return operator=(r); }
 #endif
    template <class InpIte> path& assign(InpIte first, InpIte last) { path(first,last).swap(*this); return *this; }

    path& operator/=(path const& r) { __path_detail::append<void>(str_, r.str_); return *this; }
    template <class S> path& operator/=(const S& r) { return operator/=(path(r)); }
    template <class S> path& append(const S& r) { return operator/=(path(r)); }
    template <class InpIte> path& append(InpIte first, InpIte last) { return operator/=(path(first, last)); }
    path& operator+=(path const& r) { str_ += r.str_; return *this; }
    path& operator+=(const string_type& r) { str_ += r; return *this; }
    path& operator+=(basic_string_view<value_type> r) { __CCWRAP_STRINGVIEW_ADD(str_, r); return *this; } // str_ += r;
    path& operator+=(const value_type* r) { str_ += r; return *this; }
    path& operator+=(value_type r) { str_ += r; return *this; }
    template <class S> path& operator+=(const S& r) { str_ += path(r).str_; return *this; }
    template <class OC> path& operator+=(OC r) { str_ += conv_to<string_type>(r); return *this; }

    template <class S> path& concat(const S& r) { return *this += path(r).native(); }
    template <class InpIte> path& concat(InpIte first, InpIte last) { return *this += path(first, last); }
    path& make_preferred() { __path_detail::make_preferred<void>(str_); return *this; }
    path& remove_filename() { __path_detail::remove_filename<void>(str_); return *this; }
    path& replace_filename(path const& repl) { return remove_filename() /= repl; }
    path& replace_extension(path const& r=path()) {__path_detail::replace_extension<void>(str_,r.str_); return *this;}

    void clear() noexcept { str_.clear(); }
    void swap(path& r) { str_.swap(r.str_); }

    const string_type& native() const noexcept { return str_; }
    const value_type* c_str() const noexcept { return str_.c_str(); }
    operator string_type() const { return str_; }

  #if 1
    template <class OC>
    ccwrap::basic_string<OC> string() const {
        return conv_to<ccwrap::basic_string<OC> >(str_);
    }
    template <class OC, class TR>
    ccwrap::basic_string<OC, TR> string() const {
        return conv_to<ccwrap::basic_string<OC, TR> >(str_);
    }
   #if 0 //TODO
    template <class OC, class TR, class A >
    ccwrap::basic_string<OC,TR,A> string(A const& a=A()) const {
        return conv_to<ccwrap::basic_string<OC,TR,A> >(str_,a);
    }
   #endif
  #else
    template <class OC, class TR = ccwrap::char_traits<OC> >
    ccwrap::basic_string<OC, TR> string() const {
        return conv_to<ccwrap::basic_string<OC, TR> >(str_);
    }
   #if 0 //TODO
    template <class OC, class TR=ccwrap::char_traits<OC>, class A=ccwrap::allocator<OC> >
    ccwrap::basic_string<OC,TR,A> string(A const& a = A()) const {
        return conv_to<ccwrap::basic_string<OC,TR,A> >(str_,a);
    }
   #endif
  #endif
    ccwrap::string      string() const { return conv_to<ccwrap::string>(str_); }
    ccwrap::wstring     wstring() const { return conv_to<ccwrap::wstring>(str_); }
    //ccwrap::string    u8string() const { return to_u8string(str_); }
    ccwrap::u8string    u8string() const { return conv_to<ccwrap::u8string>(str_); }
    ccwrap::u16string   u16string() const { return conv_to<ccwrap::u16string>(str_); }
    ccwrap::u32string   u32string() const { return conv_to<ccwrap::u32string>(str_); }
 #if 1
    template <class OC>
    ccwrap::basic_string<OC> generic_string() const
        { return generic_conv_to< ccwrap::basic_string<OC> >( str_ ); }
    template <class OC, class TR>
    ccwrap::basic_string<OC,TR> generic_string() const
        { return generic_conv_to< ccwrap::basic_string<OC,TR> >( str_ ); }
    template <class OC, class TR, class A >
    ccwrap::basic_string<OC,TR,A> generic_string(const A& a=A()) const
        { return generic_conv_to< ccwrap::basic_string<OC,TR,A> >( str_,a ); }
 #else
    template <class OC, class TR=ccwrap::char_traits<OC>, class A=ccwrap::allocator<OC> >
    ccwrap::basic_string<OC,TR,A> generic_string(const A& a=A()) const
        { return generic_conv_to< ccwrap::basic_string<OC,TR,A> >( str_,a ); }
 #endif
    ccwrap::string generic_string() const { return generic_conv_to<ccwrap::string>(str_); }
    ccwrap::wstring generic_wstring() const { return generic_conv_to<ccwrap::wstring>(str_); }
    //ccwrap::string generic_u8string() const { return to_u8string(str_); }
    ccwrap::u8string generic_u8string() const{ return generic_conv_to<ccwrap::u8string>(str_); }
    ccwrap::u16string generic_u16string() const { return generic_conv_to<ccwrap::u16string>(str_); }
    ccwrap::u32string generic_u32string() const { return generic_conv_to<ccwrap::u32string>(str_); }

    int compare(path const& r) const noexcept { return __path_detail::compare<void>(str_, r.str_); }
    int compare(const string_type& r) const { return __path_detail::compare<void>(str_, r); }
    int compare(basic_string_view<value_type> r) const { return __path_detail::compare<void>(str_, r); }
    int compare(const value_type* r) const { return __path_detail::compare<void>(str_, r); }

    path root_name()      const { return path(__path_detail::root_name<void>(str_)); }
    path root_directory() const { return path(__path_detail::root_directory<void>(str_)); }
    path root_path()      const { return path(__path_detail::root_path<void>(str_)); }
    path relative_path()  const { return path(__path_detail::relative_path<void>(str_)); }
    path parent_path()    const { return path(__path_detail::parent_path<void>(str_)); }
    path filename()       const { return path(__path_detail::filename<void>(str_)); }
    path stem()           const { return path(__path_detail::stem<void>(str_)); }
    path extension()      const { return path(__path_detail::extension<void>(str_)); }
    __ccwrap_nodiscard bool empty() const noexcept { return str_.empty(); }

    bool has_root_name()      const { return __path_detail::has_root_name(     str_); }
    bool has_root_directory() const { return __path_detail::has_root_directory(str_); }
    bool has_root_path()      const { return __path_detail::has_root_path(     str_); }
    bool has_relative_path()  const { return __path_detail::has_relative_path( str_); }
    bool has_parent_path()    const { return __path_detail::has_parent_path(   str_); }
    bool has_filename()       const { return __path_detail::has_filename(      str_); }
    bool has_stem()           const { return __path_detail::has_stem(          str_); }
    bool has_extension()      const { return __path_detail::has_extension(     str_); }
    bool is_absolute()        const { return __path_detail::is_absolute<void>( str_); }
    bool is_relative()        const { return !__path_detail::is_absolute<void>(str_); }

    path lexically_normal() const {
        path tmp(*this);
        __path_detail::lexically_normal<void>(tmp.str_);
        return tmp;
    }
    path lexically_relative(path const& base) const {
        path tmp(*this);
        __path_detail::lexically_relative<void>(tmp.str_, base.str_, true );
        return tmp;
    }
    path lexically_proximate(path const& base) const {
        path tmp(*this);
        __path_detail::lexically_relative<void>(tmp.str_, base.str_, false);
        return tmp;
    }

    iterator begin() const { return iterator(&str_, false); }
    iterator end()   const { return iterator(&str_, true ); }

    template <class C, class TR> friend std::basic_ostream<C, TR>&
    operator<<(std::basic_ostream<C, TR>& ost, path const& r);
    template <class C, class TR> friend std::basic_istream<C, TR>&
    operator>>(std::basic_istream<C, TR>& ist, path& p);

private:
    path(string_view_type r) : str_(r.data(), r.size()) {}
    template<class S, class T> static S conv_to(T const& t) {
        return ccwrap::stringCodeConv<S>(t);
    }
    //TODO:
    template<class S, class T, class A> static S conv_to(T const& t, A const& a) {
        return ccwrap::stringCodeConv<S>(t /*, a*/);
    }
    template<class S, class T> static S generic_conv_to(T const& t) {
        S s = ccwrap::stringCodeConv<S>(t);
        __path_detail::make_generic(s);
        return s;
    }

 #if 0
    void adjust_fmt(format fmt = auto_format) {
        if (fmt != native_format)
            make_preferred();
    }
 #endif

private:
    string_type     str_;
};

inline void swap(path& l, path& r) noexcept { l.swap(r); }

inline path operator/(path const& l, path const& r) { return path(l) /= r; }
//inline path operator/(path&& l, path const& r) { return path(std::move(l)) /= r; }
//inline path operator+(path const& l, path const& r) { return path(l) += r; }

inline bool operator==(path const& l, path const& r) noexcept { return l.compare(r) == 0; }
inline bool operator!=(path const& l, path const& r) noexcept { return l.compare(r) != 0; }
inline bool operator< (path const& l, path const& r) noexcept { return l.compare(r) <  0; }
inline bool operator>=(path const& l, path const& r) noexcept { return l.compare(r) >= 0; }
inline bool operator> (path const& l, path const& r) noexcept { return l.compare(r) >  0; }
inline bool operator<=(path const& l, path const& r) noexcept { return l.compare(r) <= 0; }


template <class S>
inline path u8path(const S& s) {
 #ifdef CCWRAP_FILESYSTEM_PATH_IS_UTF8
    return path(s);
 #else
    static_assert(sizeof(typename S::value_type) == 1, "");
    return path(u8string_view((char8_t const*)s.data(), s.size()));
 #endif
}

template <class InpIte>
inline path u8path(InpIte first, InpIte last) {
 #ifdef CCWRAP_FILESYSTEM_PATH_IS_UTF8
    return path(first, last);
 #else
    return path(u8string(first, last));
 #endif
}


template <class C, class TR> std::basic_ostream<C, TR>&
operator<<(std::basic_ostream<C, TR>& ost, path const& r) {
    ost << C('"') << r.string<C>() << C('"');
    return ost;
}

#if __cplusplus >= 201100 || defined(_HAS_CXX11) || defined(CCWRAP_CXX11)
template <class C, class TR
    , class CK = typename std::enable_if<std::is_same<C,path::value_type>::value>::type
> std::basic_istream<C, TR>&
operator>>(std::basic_istream<C, TR>& ist, path& p) {
  #if defined(CCWRAP_CXX14)
    ist >> std::quoted(p.str_);
  #else
    ist >> p.str_;
    if (p.str_.size() > 1) {
        if (__CCWRAP_STRING_BACK(p.str_) == C('"'))
            __CCWRAP_STRING_POP_BACK(p.str_);
        if (p.str_[0] == C('"'))
            p.str_.erase(p.str_.begin());
    }
  #endif
    return ist;
}
#endif

template <class C, class TR
 #if __cplusplus >= 201100 || defined(_HAS_CXX11) || defined(CCWRAP_CXX11)
    //, class CK = typename std::enable_if<!std::is_same<C,path::value_type>::value>::type
 #endif
> std::basic_istream<C, TR>&
operator>>(std::basic_istream<C, TR>& ist, path& p) {
    basic_string<C> buf;
  #if defined(CCWRAP_CXX14)
    ist >> std::quoted(buf);
  #else
    ist >> buf;
    if (buf.size() > 1) {
        if (__CCWRAP_STRING_BACK(buf) == C('"'))
            __CCWRAP_STRING_POP_BACK(buf);
        if (buf[0] == C('"'))
            buf.erase(buf.begin());
    }
  #endif
    p = buf;
    return ist;
}

//TODO: std::size_t hash_value(path const& p) noexcept;


namespace __path_detail {

template<typename DMY>
path    path_iterator<DMY>::operator*() const {
    path p;
    str_t& st = const_cast<str_t&>(p.native());
    if (cur_ < rootPathLen_) {
        if (cur_ < rootNameLen_) {
            st.assign(str_->data(), rootNameLen_);
        } else {
            st.assign(str_->data()+rootNameLen_, rootPathLen_-rootNameLen_);
        }
    } else {
        charp_t b = str_->data();
        charp_t e = b + str_->size();
        charp_t s = b + cur_;
        charp_t d = skipNotSep<void>(s, e);
        st.assign(s, d - s);
    }
    return p;
}

}   // __path_detail

} } // ccwrap::filesystem

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(pop)
#endif

#endif  // CCWRAP_filesystem_path_hpp_INCLUDED
