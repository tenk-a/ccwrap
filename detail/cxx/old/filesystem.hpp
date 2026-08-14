/**
 *  @file   _ccwrap_detail/filesystem.hpp
 *  @brief  std::filesystem backport (header-only). Stage 1: path.
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @license Boost Software License Version 1.0
 *  @note
 *      Header-only. Non-trivial internal helpers live in __ccw::fs<class D>
 *      (a dummy class template) so their non-inline definitions have vague
 *      linkage without a .cpp. Public API lives in std::filesystem.
 *
 *      Windows-oriented: path::value_type is wchar_t, preferred separator '\\'.
 *      Narrow<->wide uses the CRT (mbstowcs/wcstombs), i.e. the current locale,
 *      not UTF-8 (a codecvt-based path conversion is future work).
 *
 *      Stage 1 provides only path (pure string handling; no OS calls). Later
 *      stages add file_status, operations (exists/file_size/...) and directory
 *      iteration via self-declared Win32 in namespace __ccw.
 */
#ifndef _CCW_DETAIL_FILESYSTEM_HPP
#define _CCW_DETAIL_FILESYSTEM_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>

namespace __ccw {

// Dummy-template detail: header-only home for non-inline path helpers.
template<class D>
struct fs {
    typedef std::basic_string<wchar_t> wstr;

    static bool is_sep(wchar_t c) { return c == L'/' || c == L'\\'; }
    static bool is_alpha(wchar_t c) { return (c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z'); }

    static wstr widen(const std::string& s) {
        wstr w;
        if (s.empty()) return w;
        w.resize(s.size());
        std::size_t n = std::mbstowcs(&w[0], s.c_str(), s.size());
        if (n == (std::size_t)-1) n = 0;
        w.resize(n);
        return w;
    }
    static std::string narrow(const wstr& w) {
        std::string s;
        if (w.empty()) return s;
        s.resize(w.size() * 4 + 1);
        std::size_t n = std::wcstombs(&s[0], w.c_str(), s.size());
        if (n == (std::size_t)-1) n = 0;
        s.resize(n);
        return s;
    }

    // length of the root-name: "X:" (drive) or "//host" (UNC), else 0.
    static std::size_t root_name_len(const wstr& s) {
        std::size_t n = s.size();
        if (n >= 2 && s[1] == L':' && is_alpha(s[0]))
            return 2;
        if (n >= 2 && is_sep(s[0]) && is_sep(s[1])) {
            std::size_t i = 2;
            while (i < n && !is_sep(s[i])) ++i;   // host name
            return i;
        }
        return 0;
    }
    // length of root-name + root-directory (one separator run).
    static std::size_t root_path_len(const wstr& s) {
        std::size_t rn = root_name_len(s);
        std::size_t i = rn;
        if (i < s.size() && is_sep(s[i])) {
            ++i;
            while (i < s.size() && is_sep(s[i])) ++i;   // collapse a run
        }
        return i;
    }
    // start index of the filename component (past the last separator).
    static std::size_t filename_pos(const wstr& s) {
        std::size_t rp = root_path_len(s);
        std::size_t i = s.size();
        while (i > rp && !is_sep(s[i - 1])) --i;
        return i;
    }
    // offset of the extension dot within a filename (or npos).
    static std::size_t ext_pos(const wstr& fn) {
        if (fn == L"." || fn == L"..") return wstr::npos;
        std::size_t dot = fn.rfind(L'.');
        if (dot == wstr::npos || dot == 0) return wstr::npos;
        return dot;
    }
    static wstr to_generic(const wstr& s) {
        wstr g = s;
        for (std::size_t i = 0; i < g.size(); ++i) if (g[i] == L'\\') g[i] = L'/';
        return g;
    }
    // decompose into root-name, "has root directory", and relative elements.
    static void split(const wstr& s, wstr& rootName, bool& hasRootDir, std::vector<wstr>& elems) {
        std::size_t rn = root_name_len(s);
        rootName = s.substr(0, rn);
        std::size_t i = rn;
        hasRootDir = (i < s.size() && is_sep(s[i]));
        while (i < s.size() && is_sep(s[i])) ++i;
        while (i < s.size()) {
            std::size_t j = i;
            while (j < s.size() && !is_sep(s[j])) ++j;
            elems.push_back(s.substr(i, j - i));
            i = j;
            while (i < s.size() && is_sep(s[i])) ++i;
        }
    }
};

#if defined(_WIN32)
// Minimal self-declared Win32 (kernel32) surface, so <filesystem> needs no
// windows.h. Layout of _fs_attr_data matches WIN32_FILE_ATTRIBUTE_DATA.
struct _fs_attr_data {
    unsigned long dwFileAttributes;
    unsigned long ftCreation_lo,   ftCreation_hi;
    unsigned long ftLastAccess_lo, ftLastAccess_hi;
    unsigned long ftLastWrite_lo,  ftLastWrite_hi;
    unsigned long nFileSizeHigh;
    unsigned long nFileSizeLow;
};
// Layout matches BY_HANDLE_FILE_INFORMATION.
struct _fs_by_handle_info {
    unsigned long dwFileAttributes;
    unsigned long ftCreation_lo,   ftCreation_hi;
    unsigned long ftLastAccess_lo, ftLastAccess_hi;
    unsigned long ftLastWrite_lo,  ftLastWrite_hi;
    unsigned long dwVolumeSerialNumber;
    unsigned long nFileSizeHigh, nFileSizeLow;
    unsigned long nNumberOfLinks;
    unsigned long nFileIndexHigh, nFileIndexLow;
};
// Layout matches WIN32_FIND_DATAW.
struct _fs_find_data {
    unsigned long dwFileAttributes;
    unsigned long ftCreation_lo,   ftCreation_hi;
    unsigned long ftLastAccess_lo, ftLastAccess_hi;
    unsigned long ftLastWrite_lo,  ftLastWrite_hi;
    unsigned long nFileSizeHigh;
    unsigned long nFileSizeLow;
    unsigned long dwReserved0, dwReserved1;
    wchar_t       cFileName[260];
    wchar_t       cAlternateFileName[14];
};
extern "C" {
_CCW_WINAPI(unsigned long) GetFileAttributesW(const wchar_t*);
_CCW_WINAPI(int) GetFileAttributesExW(const wchar_t*, int, void*);
_CCW_WINAPI(int) CreateDirectoryW(const wchar_t*, void*);
_CCW_WINAPI(int) RemoveDirectoryW(const wchar_t*);
_CCW_WINAPI(int) DeleteFileW(const wchar_t*);
_CCW_WINAPI(unsigned long) GetCurrentDirectoryW(unsigned long, wchar_t*);
_CCW_WINAPI(int) MoveFileW(const wchar_t*, const wchar_t*);
_CCW_WINAPI(void*) FindFirstFileW(const wchar_t*, _fs_find_data*);
_CCW_WINAPI(int) FindNextFileW(void*, _fs_find_data*);
_CCW_WINAPI(int) FindClose(void*);
_CCW_WINAPI(int) CopyFileW(const wchar_t*, const wchar_t*, int);
_CCW_WINAPI(unsigned long) GetTempPathW(unsigned long, wchar_t*);
_CCW_WINAPI(unsigned long) GetFullPathNameW(const wchar_t*, unsigned long, wchar_t*, wchar_t**);
_CCW_WINAPI(unsigned long) GetLastError(void);
_CCW_WINAPI(int) GetDiskFreeSpaceExW(const wchar_t*, unsigned long long*, unsigned long long*, unsigned long long*);
_CCW_WINAPI(void*) CreateFileW(const wchar_t*, unsigned long, unsigned long, void*, unsigned long, unsigned long, void*);
_CCW_WINAPI(int) GetFileInformationByHandle(void*, _fs_by_handle_info*);
_CCW_WINAPI(int) CloseHandle(void*);
_CCW_WINAPI(int) CreateHardLinkW(const wchar_t*, const wchar_t*, void*);
_CCW_WINAPI(int) SetFileTime(void*, const void*, const void*, const void*);
_CCW_WINAPI(unsigned char) CreateSymbolicLinkW(const wchar_t*, const wchar_t*, unsigned long);
_CCW_WINAPI(int) WideCharToMultiByte(unsigned int, unsigned long, const wchar_t*, int, char*, int, const char*, int*);
_CCW_WINAPI(int) MultiByteToWideChar(unsigned int, unsigned long, const char*, int, wchar_t*, int);
}
// UTF-8 <-> wide (CP_UTF8 = 65001) for path::u8string / u8path.
inline std::string to_utf8(const std::basic_string<wchar_t>& w) {
    if (w.empty()) return std::string();
    int n = WideCharToMultiByte(65001u, 0, w.c_str(), int(w.size()), 0, 0, 0, 0);
    if (n <= 0) return std::string();
    std::string s(std::size_t(n), '\0');
    WideCharToMultiByte(65001u, 0, w.c_str(), int(w.size()), &s[0], n, 0, 0);
    return s;
}
inline std::basic_string<wchar_t> from_utf8(const std::string& s) {
    if (s.empty()) return std::basic_string<wchar_t>();
    int n = MultiByteToWideChar(65001u, 0, s.c_str(), int(s.size()), 0, 0);
    if (n <= 0) return std::basic_string<wchar_t>();
    std::basic_string<wchar_t> w(std::size_t(n), L'\0');
    MultiByteToWideChar(65001u, 0, s.c_str(), int(s.size()), &w[0], n);
    return w;
}
inline bool get_file_id(const wchar_t* p, _fs_by_handle_info& out) {
    /* access=0, share=READ|WRITE|DELETE, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS (dirs) */
    void* h = CreateFileW(p, 0, 7, 0, 3, 0x02000000ul, 0);
    if (h == (void*)-1) return false;
    int ok = GetFileInformationByHandle(h, &out);
    CloseHandle(h);
    return ok != 0;
}
#endif  // _WIN32

}   // namespace __ccw

// std::error_code comes from the <system_error> wrapper (native on vc10+, a
// minimal std::error_code on vc8/9), so it is uniform across all VC.
#if defined(_WIN32)
#include <system_error>
namespace __ccw { inline void set_ec(std::error_code& ec, unsigned long e) { ec.assign(int(e), std::system_category()); } }
#endif  // _WIN32

namespace std {
namespace filesystem {

class path {
public:
    typedef wchar_t                     value_type;
    typedef std::basic_string<wchar_t>  string_type;
    enum format { native_format, generic_format, auto_format };
    static const value_type preferred_separator = L'\\';

private:
    typedef __ccw::fs<void> H;
    string_type s_;

    explicit path(const string_type& s, int) : s_(s) {}   // internal, from raw

public:
    path() {}
    path(const path& p) : s_(p.s_) {}
    path(const string_type& s) : s_(s) {}
    path(const value_type* s) : s_(s) {}
    path(const std::string& s) : s_(H::widen(s)) {}
    path(const char* s) : s_(H::widen(std::string(s))) {}

    path& operator=(const path& p) { s_ = p.s_; return *this; }
    path& operator=(const string_type& s) { s_ = s; return *this; }
    path& operator=(const value_type* s) { s_ = s; return *this; }
    path& assign(const string_type& s) { s_ = s; return *this; }

    // --- append (with directory separator) ---
    path& operator/=(const path& p) {
        if (p.is_absolute() || (p.has_root_name() && p.root_name().native() != root_name().native())) {
            s_ = p.s_;
            return *this;
        }
        if (p.has_root_directory()) {
            s_.erase(H::root_name_len(s_));
        } else if (!s_.empty() && !H::is_sep(s_[s_.size() - 1]) && !p.s_.empty()) {
            s_ += preferred_separator;
        }
        s_ += p.s_;
        return *this;
    }

    // --- concat (no separator) ---
    path& operator+=(const path& p) { s_ += p.s_; return *this; }
    path& operator+=(const string_type& s) { s_ += s; return *this; }
    path& operator+=(const value_type* s) { s_ += s; return *this; }
    path& operator+=(value_type c) { s_ += c; return *this; }
    path& concat(const string_type& s) { s_ += s; return *this; }

    // --- modifiers ---
    void  clear() { s_.clear(); }
    path& make_preferred() {
        for (std::size_t i = 0; i < s_.size(); ++i) if (s_[i] == L'/') s_[i] = preferred_separator;
        return *this;
    }
    path& remove_filename() { s_.erase(H::filename_pos(s_)); return *this; }
    path& replace_filename(const path& repl) { remove_filename(); return operator/=(repl); }
    path& replace_extension(const path& repl = path()) {
        std::size_t fp = H::filename_pos(s_);
        string_type fn = s_.substr(fp);
        std::size_t ep = H::ext_pos(fn);
        if (ep != string_type::npos) s_.erase(fp + ep);
        if (!repl.s_.empty()) {
            if (repl.s_[0] != L'.') s_ += L'.';
            s_ += repl.s_;
        }
        return *this;
    }
    void swap(path& r) { s_.swap(r.s_); }

    // --- native format observers ---
    const string_type&  native() const  { return s_; }
    const value_type*   c_str() const    { return s_.c_str(); }
    operator string_type() const { return s_; }

    std::string  string()  const { return H::narrow(s_); }
    string_type  wstring() const { return s_; }
    std::string  generic_string()  const { return H::narrow(H::to_generic(s_)); }
    string_type  generic_wstring() const { return H::to_generic(s_); }
    std::string  u8string()         const { return __ccw::to_utf8(s_); }
    std::string  generic_u8string() const { return __ccw::to_utf8(H::to_generic(s_)); }

    bool empty() const { return s_.empty(); }

    // --- decomposition ---
    path root_name() const { return path(s_.substr(0, H::root_name_len(s_)), 0); }
    path root_directory() const {
        std::size_t rn = H::root_name_len(s_);
        std::size_t rp = H::root_path_len(s_);
        return path(s_.substr(rn, rp - rn), 0);
    }
    path root_path() const { return path(s_.substr(0, H::root_path_len(s_)), 0); }
    path relative_path() const { return path(s_.substr(H::root_path_len(s_)), 0); }
    path parent_path() const {
        std::size_t fp = H::filename_pos(s_);
        std::size_t rp = H::root_path_len(s_);
        if (fp <= rp) return path(s_.substr(0, rp), 0);
        std::size_t e = fp;
        while (e > rp && H::is_sep(s_[e - 1])) --e;   // drop trailing separators
        return path(s_.substr(0, e ? e : fp), 0);
    }
    path filename() const { return path(s_.substr(H::filename_pos(s_)), 0); }
    path stem() const {
        string_type fn = s_.substr(H::filename_pos(s_));
        std::size_t ep = H::ext_pos(fn);
        return path(ep == string_type::npos ? fn : fn.substr(0, ep), 0);
    }
    path extension() const {
        string_type fn = s_.substr(H::filename_pos(s_));
        std::size_t ep = H::ext_pos(fn);
        return path(ep == string_type::npos ? string_type() : fn.substr(ep), 0);
    }

    // --- queries ---
    bool has_root_name()      const { return H::root_name_len(s_) != 0; }
    bool has_root_directory() const { return H::root_path_len(s_) != H::root_name_len(s_); }
    bool has_root_path()      const { return H::root_path_len(s_) != 0; }
    bool has_relative_path()  const { return H::root_path_len(s_) != s_.size(); }
    bool has_parent_path()    const { return !parent_path().empty(); }
    bool has_filename()       const { return H::filename_pos(s_) != s_.size(); }
    bool has_stem()           const { return !stem().empty(); }
    bool has_extension()      const { return !extension().empty(); }
    bool is_absolute()        const { return has_root_name() && has_root_directory(); }
    bool is_relative()        const { return !is_absolute(); }

    // --- compare (by generic form) ---
    int compare(const path& p) const {
        string_type a = H::to_generic(s_), b = H::to_generic(p.s_);
        return a.compare(b);
    }

    // --- lexical operations (pure string logic; no OS access) ---
    path lexically_normal() const {
        if (s_.empty()) return path();
        string_type rn;
        bool rd;
        std::vector<string_type> el, out;
        H::split(s_, rn, rd, el);
        for (std::size_t k = 0; k < el.size(); ++k) {
            const string_type& e = el[k];
            if (e == L".") continue;
            if (e == L"..") {
                if (!out.empty() && out.back() != L"..") out.pop_back();
                else if (!rd) out.push_back(e);
            } else {
                out.push_back(e);
            }
        }
        string_type res = rn;
        if (rd) res += preferred_separator;
        for (std::size_t k = 0; k < out.size(); ++k) {
            if (k) res += preferred_separator;
            res += out[k];
        }
        if (res.empty()) return path(string_type(1, L'.'), 0);
        return path(res, 0);
    }
    path lexically_relative(const path& base) const {
        if (root_name().compare(base.root_name()) != 0) return path();
        if (is_absolute() != base.is_absolute()) return path();
        if (!has_root_directory() && base.has_root_directory()) return path();
        string_type rnA, rnB;
        bool rdA, rdB;
        std::vector<string_type> a, b;
        H::split(s_, rnA, rdA, a);
        H::split(base.s_, rnB, rdB, b);
        std::size_t i = 0;
        while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
        if (i == a.size() && i == b.size()) return path(string_type(1, L'.'), 0);
        int up = 0;
        for (std::size_t k = i; k < b.size(); ++k) {
            if (b[k] == L"..") --up;
            else if (b[k] != L".") ++up;
        }
        if (up < 0) return path();
        string_type res;
        for (int u = 0; u < up; ++u) { if (!res.empty()) res += preferred_separator; res += L".."; }
        for (std::size_t k = i; k < a.size(); ++k) { if (!res.empty()) res += preferred_separator; res += a[k]; }
        if (res.empty()) return path(string_type(1, L'.'), 0);
        return path(res, 0);
    }
    path lexically_proximate(const path& base) const {
        path r = lexically_relative(base);
        return r.empty() ? *this : r;
    }
};

inline void swap(path& a, path& b) { a.swap(b); }

inline bool operator==(const path& a, const path& b) { return a.compare(b) == 0; }
inline bool operator!=(const path& a, const path& b) { return a.compare(b) != 0; }
inline bool operator< (const path& a, const path& b) { return a.compare(b) <  0; }
inline bool operator> (const path& a, const path& b) { return a.compare(b) >  0; }
inline bool operator<=(const path& a, const path& b) { return a.compare(b) <= 0; }
inline bool operator>=(const path& a, const path& b) { return a.compare(b) >= 0; }

inline path operator/(const path& a, const path& b) { path t(a); t /= b; return t; }

inline path u8path(const std::string& s) { return path(__ccw::from_utf8(s)); }

// --- file_status / enums (stage 2) -------------------------------------------
enum file_type {
    none = 0, not_found = -1, regular = 1, directory = 2, symlink = 3,
    block = 4, character = 5, fifo = 6, socket = 7, unknown = 8
};
enum perms {
    perms_none = 0,
    owner_read = 0400, owner_write = 0200, owner_exec = 0100, owner_all = 0700,
    group_read = 040,  group_write = 020,  group_exec = 010,  group_all = 070,
    others_read = 04,  others_write = 02,  others_exec = 01,  others_all = 07,
    all = 0777, mask = 07777, perms_unknown = 0xFFFF
};

class file_status {
    file_type t_;
    perms     p_;
public:
    explicit file_status(file_type t = none, perms p = perms_unknown) : t_(t), p_(p) {}
    file_type type() const { return t_; }
    perms permissions() const { return p_; }
    void type(file_type t) { t_ = t; }
    void permissions(perms p) { p_ = p; }
};

typedef unsigned long long _fs_uintmax;   // std::uintmax_t stand-in

// namespace-enum (like charconv chars_format): fs::copy_options::recursive works
// on the backport (namespace member) and on native C++17 (enum class member).
namespace copy_options {
    enum copy_options {
        none = 0,
        skip_existing = 1, overwrite_existing = 2, update_existing = 4,
        recursive = 8,
        copy_symlinks = 16, skip_symlinks = 32,
        directories_only = 64, create_symlinks = 128, create_hard_links = 256
    };
}

class filesystem_error : public std::exception {
    std::string what_;
    path        p1_, p2_;
public:
    explicit filesystem_error(const std::string& w) : what_(w) {}
    filesystem_error(const std::string& w, std::error_code) : what_(w) {}
    filesystem_error(const std::string& w, const path& a, std::error_code) : what_(w), p1_(a) {}
    filesystem_error(const std::string& w, const path& a, const path& b, std::error_code) : what_(w), p1_(a), p2_(b) {}
    const char* what() const noexcept { return what_.c_str(); }
    const path& path1() const { return p1_; }
    const path& path2() const { return p2_; }
};

#if defined(_WIN32)

inline file_status status(const path& p) {
    unsigned long a = __ccw::GetFileAttributesW(p.c_str());
    if (a == 0xFFFFFFFFul) return file_status(not_found, perms_unknown);
    file_type t = (a & 0x10ul) ? directory : regular;   // FILE_ATTRIBUTE_DIRECTORY
    perms pm = (a & 0x01ul) ? perms(all & ~ (owner_write | group_write | others_write)) : all;
    return file_status(t, pm);
}
inline bool status_known(file_status s) { return s.type() != none; }
inline bool exists(file_status s) { return s.type() != not_found && s.type() != none; }
inline bool exists(const path& p) { return exists(status(p)); }
inline bool is_directory(file_status s) { return s.type() == directory; }
inline bool is_directory(const path& p) { return is_directory(status(p)); }
inline bool is_regular_file(file_status s) { return s.type() == regular; }
inline bool is_regular_file(const path& p) { return is_regular_file(status(p)); }
inline bool is_other(file_status s) { return exists(s) && !is_regular_file(s) && !is_directory(s); }

inline _fs_uintmax file_size(const path& p) {
    __ccw::_fs_attr_data d;
    if (!__ccw::GetFileAttributesExW(p.c_str(), 0 /*GetFileExInfoStandard*/, &d))
        return _fs_uintmax(-1);
    return (_fs_uintmax(d.nFileSizeHigh) << 32) | _fs_uintmax(d.nFileSizeLow);
}
inline bool create_directory(const path& p) { return __ccw::CreateDirectoryW(p.c_str(), 0) != 0; }
inline bool remove(const path& p) {
    if (is_directory(p)) return __ccw::RemoveDirectoryW(p.c_str()) != 0;
    return __ccw::DeleteFileW(p.c_str()) != 0;
}
inline void rename(const path& from, const path& to) { __ccw::MoveFileW(from.c_str(), to.c_str()); }
inline path current_path() {
    unsigned long n = __ccw::GetCurrentDirectoryW(0, 0);
    if (n == 0) return path();
    std::basic_string<wchar_t> buf(n, L'\0');
    unsigned long m = __ccw::GetCurrentDirectoryW(n, &buf[0]);
    buf.resize(m);
    return path(buf);
}

// --- directory_entry / directory_iterator (stage 3) --------------------------
class directory_entry {
    typedef std::filesystem::path pathT;
    pathT p_;
public:
    directory_entry() {}
    explicit directory_entry(const pathT& p) : p_(p) {}
    void assign(const pathT& p) { p_ = p; }
    const pathT& path() const { return p_; }
    operator const pathT&() const { return p_; }
    bool exists() const          { return std::filesystem::exists(p_); }
    bool is_directory() const    { return std::filesystem::is_directory(p_); }
    bool is_regular_file() const { return std::filesystem::is_regular_file(p_); }
    _fs_uintmax file_size() const { return std::filesystem::file_size(p_); }
    file_status status() const   { return std::filesystem::status(p_); }
};

class directory_iterator {
    struct impl {
        void*              h;
        __ccw::_fs_find_data data;
        path               base;
        directory_entry    entry;
        long               ref;
        impl() : h(0), ref(1) {}
    };
    impl* p_;   // NULL: end iterator

    void addref()  { if (p_) ++p_->ref; }
    void release() {
        if (p_ && --p_->ref == 0) {
            if (p_->h && p_->h != (void*)-1) __ccw::FindClose(p_->h);
            delete p_;
        }
        p_ = 0;
    }
    void set_entry() { p_->entry.assign(p_->base / path(std::basic_string<wchar_t>(p_->data.cFileName))); }
    void skip_dots() {
        for (;;) {
            std::basic_string<wchar_t> nm(p_->data.cFileName);
            if (nm != L"." && nm != L"..") break;
            if (!__ccw::FindNextFileW(p_->h, &p_->data)) { release(); return; }
        }
        set_entry();
    }

public:
    directory_iterator() : p_(0) {}
    explicit directory_iterator(const path& dir) : p_(0) {
        impl* im = new impl();
        im->base = dir;
        std::basic_string<wchar_t> pat = dir.native();
        if (!pat.empty() && !__ccw::fs<void>::is_sep(pat[pat.size() - 1])) pat += L'\\';
        pat += L'*';
        im->h = __ccw::FindFirstFileW(pat.c_str(), &im->data);
        if (im->h == (void*)-1 || im->h == 0) { delete im; return; }
        p_ = im;
        skip_dots();
    }
    directory_iterator(const directory_iterator& r) : p_(r.p_) { addref(); }
    directory_iterator& operator=(const directory_iterator& r) {
        if (p_ != r.p_) { release(); p_ = r.p_; addref(); }
        return *this;
    }
    ~directory_iterator() { release(); }

    const directory_entry& operator*()  const { return p_->entry; }
    const directory_entry* operator->() const { return &p_->entry; }

    directory_iterator& operator++() {
        if (p_) {
            if (!__ccw::FindNextFileW(p_->h, &p_->data)) release();
            else skip_dots();
        }
        return *this;
    }
    bool operator==(const directory_iterator& r) const { return p_ == r.p_; }
    bool operator!=(const directory_iterator& r) const { return p_ != r.p_; }
};

inline directory_iterator begin(directory_iterator it) { return it; }
inline directory_iterator end(const directory_iterator&) { return directory_iterator(); }

// --- recursive_directory_iterator (stage 5) ----------------------------------
// A stack of directory_iterators. Descends into each directory; on exhausting a
// level it pops and advances the parent past the directory it came from.
class recursive_directory_iterator {
    struct impl {
        std::vector<directory_iterator> st;
        long ref;
        impl() : ref(1) {}
    };
    impl* p_;   // NULL: end

    void addref()  { if (p_) ++p_->ref; }
    void release() { if (p_ && --p_->ref == 0) delete p_; p_ = 0; }

public:
    recursive_directory_iterator() : p_(0) {}
    explicit recursive_directory_iterator(const path& dir) : p_(0) {
        directory_iterator it(dir);
        if (it == directory_iterator()) return;   // empty dir -> end
        p_ = new impl();
        p_->st.push_back(it);
    }
    recursive_directory_iterator(const recursive_directory_iterator& r) : p_(r.p_) { addref(); }
    recursive_directory_iterator& operator=(const recursive_directory_iterator& r) {
        if (p_ != r.p_) { release(); p_ = r.p_; addref(); }
        return *this;
    }
    ~recursive_directory_iterator() { release(); }

    const directory_entry& operator*()  const { return *p_->st.back(); }
    const directory_entry* operator->() const { return &*p_->st.back(); }
    int depth() const { return p_ ? int(p_->st.size()) - 1 : 0; }

    recursive_directory_iterator& operator++() {
        if (!p_) return *this;
        const directory_entry& e = *p_->st.back();
        if (e.is_directory()) {
            directory_iterator sub(e.path());
            if (sub != directory_iterator()) { p_->st.push_back(sub); return *this; }
        }
        ++p_->st.back();
        while (!p_->st.empty() && p_->st.back() == directory_iterator()) {
            p_->st.pop_back();
            if (!p_->st.empty()) ++p_->st.back();
        }
        if (p_->st.empty()) release();
        return *this;
    }
    bool operator==(const recursive_directory_iterator& r) const { return p_ == r.p_; }
    bool operator!=(const recursive_directory_iterator& r) const { return p_ != r.p_; }
};

inline recursive_directory_iterator begin(recursive_directory_iterator it) { return it; }
inline recursive_directory_iterator end(const recursive_directory_iterator&) { return recursive_directory_iterator(); }

// --- convenience operations (stage 4) ----------------------------------------
inline bool create_directories(const path& p) {
    if (p.empty() || exists(p)) return false;
    path parent = p.parent_path();
    if (!parent.empty() && !exists(parent)) create_directories(parent);
    return create_directory(p);
}
inline _fs_uintmax remove_all(const path& p) {
    if (!exists(p)) return 0;
    _fs_uintmax count = 0;
    if (is_directory(p)) {
        std::vector<path> kids;
        for (directory_iterator it(p); it != directory_iterator(); ++it)
            kids.push_back(it->path());   // snapshot, then delete (handle closed after loop)
        for (std::size_t i = 0; i < kids.size(); ++i)
            count += remove_all(kids[i]);
    }
    if (remove(p)) ++count;
    return count;
}
inline bool copy_file(const path& from, const path& to) {
    return __ccw::CopyFileW(from.c_str(), to.c_str(), 0 /*overwrite*/) != 0;
}
inline bool copy_file(const path& from, const path& to, copy_options::copy_options opt) {
    int failIfExists = (opt & copy_options::skip_existing) && !(opt & copy_options::overwrite_existing) ? 1 : 0;
    return __ccw::CopyFileW(from.c_str(), to.c_str(), failIfExists) != 0;
}
inline void create_hard_link(const path& target, const path& link) {
    __ccw::CreateHardLinkW(link.c_str(), target.c_str(), 0);
}
// symlink creation needs privilege or Developer Mode; flag 0x2 =
// SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE, flag 0x1 = ..._DIRECTORY.
inline void create_symlink(const path& target, const path& link) {
    __ccw::CreateSymbolicLinkW(link.c_str(), target.c_str(), 0x2ul);
}
inline void create_directory_symlink(const path& target, const path& link) {
    __ccw::CreateSymbolicLinkW(link.c_str(), target.c_str(), 0x1ul | 0x2ul);
}
inline void create_symlink(const path& target, const path& link, std::error_code& ec) {
    if (!__ccw::CreateSymbolicLinkW(link.c_str(), target.c_str(), 0x2ul)) __ccw::set_ec(ec, __ccw::GetLastError()); else ec.clear();
}
inline void create_directory_symlink(const path& target, const path& link, std::error_code& ec) {
    if (!__ccw::CreateSymbolicLinkW(link.c_str(), target.c_str(), 0x1ul | 0x2ul)) __ccw::set_ec(ec, __ccw::GetLastError()); else ec.clear();
}
inline bool is_symlink(const path& p) {
    unsigned long a = __ccw::GetFileAttributesW(p.c_str());
    return a != 0xFFFFFFFFul && (a & 0x400ul) != 0;   /* FILE_ATTRIBUTE_REPARSE_POINT */
}
inline path temp_directory_path() {
    wchar_t buf[1024];
    unsigned long n = __ccw::GetTempPathW(1024, buf);
    return path(std::basic_string<wchar_t>(buf, n));
}
inline path absolute(const path& p) {
    if (p.is_absolute()) return p;
    return current_path() / p;
}
inline path weakly_canonical(const path& p) {
    wchar_t buf[1024];
    wchar_t* fp = 0;
    unsigned long n = __ccw::GetFullPathNameW(p.c_str(), 1024, buf, &fp);
    if (n == 0 || n >= 1024) return absolute(p);
    return path(std::basic_string<wchar_t>(buf, n));
}
inline path canonical(const path& p) { return weakly_canonical(absolute(p)); }

inline path relative(const path& p, const path& base) {
    return weakly_canonical(p).lexically_relative(weakly_canonical(base));
}
inline path relative(const path& p) { return relative(p, current_path()); }
inline path proximate(const path& p, const path& base) {
    path r = relative(p, base);
    return r.empty() ? p : r;
}
inline path proximate(const path& p) { return proximate(p, current_path()); }

// --- copy (recursive) --------------------------------------------------------
inline void copy(const path& from, const path& to, copy_options::copy_options opt) {
    file_status s = status(from);
    if (s.type() == regular) {
        copy_file(from, to, opt);
        return;
    }
    if (s.type() == directory) {
        create_directories(to);
        for (directory_iterator it(from); it != directory_iterator(); ++it) {
            const directory_entry& e = *it;
            path dst = to / e.path().filename();
            if (e.is_directory()) {
                if (opt & copy_options::recursive) copy(e.path(), dst, opt);
            } else {
                copy_file(e.path(), dst, opt);
            }
        }
    }
}
inline void copy(const path& from, const path& to) { copy(from, to, copy_options::none); }

inline bool is_empty(const path& p) {
    if (is_directory(p)) return directory_iterator(p) == directory_iterator();
    return file_size(p) == 0;
}

// --- space / equivalent / hard_link_count ------------------------------------
struct space_info {
    _fs_uintmax capacity;
    _fs_uintmax free;
    _fs_uintmax available;
};
inline space_info space(const path& p) {
    unsigned long long avail = 0, total = 0, tfree = 0;
    space_info si;
    si.capacity = si.free = si.available = _fs_uintmax(-1);
    if (__ccw::GetDiskFreeSpaceExW(p.c_str(), &avail, &total, &tfree)) {
        si.capacity = total; si.free = tfree; si.available = avail;
    }
    return si;
}
inline bool equivalent(const path& a, const path& b) {
    __ccw::_fs_by_handle_info ia, ib;
    if (!__ccw::get_file_id(a.c_str(), ia) || !__ccw::get_file_id(b.c_str(), ib)) return false;
    return ia.dwVolumeSerialNumber == ib.dwVolumeSerialNumber
        && ia.nFileIndexHigh == ib.nFileIndexHigh
        && ia.nFileIndexLow  == ib.nFileIndexLow;
}
inline _fs_uintmax hard_link_count(const path& p) {
    __ccw::_fs_by_handle_info info;
    if (!__ccw::get_file_id(p.c_str(), info)) return _fs_uintmax(-1);
    return info.nNumberOfLinks;
}

// --- last_write_time (read only) ---------------------------------------------
// Minimal file_time_type holding the raw FILETIME (100ns ticks since 1601). Only
// used by the backport; native C++17 uses its own chrono-based file_time_type.
class file_time_type {
    unsigned long long t_;
public:
    file_time_type() : t_(0) {}
    explicit file_time_type(unsigned long long t) : t_(t) {}
    unsigned long long _ticks() const { return t_; }
};
inline bool operator==(file_time_type a, file_time_type b) { return a._ticks() == b._ticks(); }
inline bool operator!=(file_time_type a, file_time_type b) { return a._ticks() != b._ticks(); }
inline bool operator< (file_time_type a, file_time_type b) { return a._ticks() <  b._ticks(); }
inline bool operator> (file_time_type a, file_time_type b) { return a._ticks() >  b._ticks(); }
inline bool operator<=(file_time_type a, file_time_type b) { return a._ticks() <= b._ticks(); }
inline bool operator>=(file_time_type a, file_time_type b) { return a._ticks() >= b._ticks(); }

inline file_time_type last_write_time(const path& p) {
    __ccw::_fs_attr_data d;
    if (!__ccw::GetFileAttributesExW(p.c_str(), 0, &d)) return file_time_type();
    return file_time_type((unsigned long long)(d.ftLastWrite_hi) << 32 | d.ftLastWrite_lo);
}
inline file_time_type last_write_time(const path& p, std::error_code& ec) {
    __ccw::_fs_attr_data d;
    if (!__ccw::GetFileAttributesExW(p.c_str(), 0, &d)) { __ccw::set_ec(ec, __ccw::GetLastError()); return file_time_type(); }
    ec.clear();
    return file_time_type((unsigned long long)(d.ftLastWrite_hi) << 32 | d.ftLastWrite_lo);
}
inline void last_write_time(const path& p, file_time_type t) {
    void* h = __ccw::CreateFileW(p.c_str(), 0x100ul /*FILE_WRITE_ATTRIBUTES*/, 7, 0, 3, 0x02000000ul, 0);
    if (h == (void*)-1) return;
    unsigned long long ticks = t._ticks();
    struct { unsigned long lo, hi; } ft;
    ft.lo = (unsigned long)(ticks & 0xFFFFFFFFul);
    ft.hi = (unsigned long)(ticks >> 32);
    __ccw::SetFileTime(h, 0, 0, &ft);
    __ccw::CloseHandle(h);
}

// --- error_code overloads (noexcept style; set ec from GetLastError) ---------
inline file_status status(const path& p, std::error_code& ec) {
    ec.clear();          /* not-found is reported via type(), not through ec */
    return status(p);
}
inline bool exists(const path& p, std::error_code& ec)          { return exists(status(p, ec)); }
inline bool is_directory(const path& p, std::error_code& ec)    { return status(p, ec).type() == directory; }
inline bool is_regular_file(const path& p, std::error_code& ec) { return status(p, ec).type() == regular; }
inline _fs_uintmax file_size(const path& p, std::error_code& ec) {
    __ccw::_fs_attr_data d;
    if (!__ccw::GetFileAttributesExW(p.c_str(), 0, &d)) { __ccw::set_ec(ec, __ccw::GetLastError()); return _fs_uintmax(-1); }
    ec.clear();
    return (_fs_uintmax(d.nFileSizeHigh) << 32) | _fs_uintmax(d.nFileSizeLow);
}
inline bool create_directory(const path& p, std::error_code& ec) {
    bool r = create_directory(p);
    if (!r) __ccw::set_ec(ec, __ccw::GetLastError()); else ec.clear();
    return r;
}
inline bool remove(const path& p, std::error_code& ec) {
    bool r = remove(p);
    if (!r) __ccw::set_ec(ec, __ccw::GetLastError()); else ec.clear();
    return r;
}
inline void rename(const path& from, const path& to, std::error_code& ec) {
    if (!__ccw::MoveFileW(from.c_str(), to.c_str())) __ccw::set_ec(ec, __ccw::GetLastError()); else ec.clear();
}
inline bool copy_file(const path& from, const path& to, std::error_code& ec) {
    bool r = copy_file(from, to);
    if (!r) __ccw::set_ec(ec, __ccw::GetLastError()); else ec.clear();
    return r;
}
inline path current_path(std::error_code& ec) { ec.clear(); return current_path(); }

#endif  // _WIN32

}   // namespace filesystem
}   // namespace std

#endif  // _CCW_DETAIL_FILESYSTEM_HPP
