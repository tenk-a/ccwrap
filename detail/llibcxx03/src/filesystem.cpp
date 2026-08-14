//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: OS layer of <filesystem> (see llibcxx03/include/filesystem).
//  Implemented on the C library (stat / mkdir / remove / rename / getcwd) plus
//  _findfirst/_findnext for directory iteration -- the same "lexical in the header,
//  OS calls out of line" split libc++ uses. Open Watcom and MSVC both have those.
//===----------------------------------------------------------------------===//
#if !defined(__WATCOMC__) || __WATCOMC__ >= 1300
#include <filesystem>
#endif

#if defined(_CCW_LIBCPP_FILESYSTEM)

#include <sys/stat.h>
#include <sys/utime.h>     // last_write_time(p, t)
#include <system_error>      // the symlink family reports unsupported through error_code
#include <direct.h>
#include <io.h>
#include <cstdio>
#include <cstdlib>   // _CCW_STD::getenv (temp_directory_path), _fullpath (canonical)

#if !defined(_CCW_OS_WIN) && !defined(_CCW_OS_DOS)
# if defined(_WIN32) || defined(__NT__)
#  define _CCW_OS_WIN 1
# elif defined(__DOS__) || defined(MSDOS)
#  define _CCW_OS_DOS 1
# endif
#endif

#if defined(_CCW_OS_DOS)
# include <dos.h>      // _dos_getdiskfree (space)
#endif

#if defined(_MSC_VER) || defined(__MINGW32__)
# define _CCW_FS_MSVCRT 1
#else
# define _CCW_FS_MSVCRT 0
#endif

#if _CCW_FS_MSVCRT
# define __ccw_fileno _fileno
# define __ccw_chsize _chsize
#else
# define __ccw_fileno fileno
# define __ccw_chsize chsize
#endif

#if !defined(S_IFMT) && defined(_S_IFMT)
# define S_IFMT  _S_IFMT
# define S_IFDIR _S_IFDIR
# define S_IFREG _S_IFREG
#endif

#if _CCW_FS_MSVCRT
typedef intptr_t __ccw_find_handle_t;
#else
typedef long     __ccw_find_handle_t;
#endif

namespace __fs = _CCW_STD::filesystem;

#if _CCW_FS_WCHAR
#  include <wchar.h>
typedef wchar_t                  __ccw_fs_c;
typedef struct _wfinddata_t      __ccw_fs_finddata;
#  define __CCW_FS_S(__x)        L##__x
#  define __ccw_fs_stat(p, s)    _wstat((p), (s))
#  define __ccw_fs_mkdir(p)      _wmkdir(p)
#  define __ccw_fs_rmdir(p)      _wrmdir(p)
#  define __ccw_fs_remove(p)     _wremove(p)
#  define __ccw_fs_rename(a, b)  _wrename((a), (b))
#  define __ccw_fs_getcwd(b, n)  _wgetcwd((b), (n))
#  define __ccw_fs_chdir(p)      _wchdir(p)
#  define __ccw_fs_fopen(p, m)   _wfopen((p), (m))
#  define __ccw_fs_getenv(n)     _wgetenv(n)
#  define __ccw_fs_fullpath(b, p, n) _wfullpath((b), (p), (n))
#  define __ccw_fs_utime(p, t)   _wutime((p), (t))
#  define __ccw_fs_access(p, m)  _waccess((p), (m))
#  define __ccw_fs_chmod(p, m)   _wchmod((p), (m))
#  define __ccw_fs_findfirst(s, d) _wfindfirst((s), (d))
#  define __ccw_fs_findnext(h, d)  _wfindnext((h), (d))
#else
typedef char                     __ccw_fs_c;
typedef struct _finddata_t       __ccw_fs_finddata;
#  define __CCW_FS_S(__x)        __x
#  define __ccw_fs_stat(p, s)    stat((p), (s))
#  define __ccw_fs_mkdir(p)      _mkdir(p)
#  define __ccw_fs_rmdir(p)      _rmdir(p)
#  define __ccw_fs_remove(p)     _CCW_STD::remove(p)
#  define __ccw_fs_rename(a, b)  _CCW_STD::rename((a), (b))
#  define __ccw_fs_getcwd(b, n)  _getcwd((b), (n))
#  define __ccw_fs_chdir(p)      _chdir(p)
#  define __ccw_fs_fopen(p, m)   _CCW_STD::fopen((p), (m))
#  define __ccw_fs_getenv(n)     _CCW_STD::getenv(n)
#  define __ccw_fs_fullpath(b, p, n) _fullpath((b), (p), (n))
#  define __ccw_fs_utime(p, t)   utime((p), (t))
#  define __ccw_fs_access(p, m)  access((p), (m))
#  define __ccw_fs_chmod(p, m)   chmod((p), (m))
#  define __ccw_fs_findfirst(s, d) _findfirst((s), (d))
#  define __ccw_fs_findnext(h, d)  _findnext((h), (d))
#endif

typedef __fs::path::string_type __ccw_fs_str;

#if _CCW_FS_WCHAR && _CCW_FS_MSVCRT
typedef struct _stat             __ccw_fs_stat_t;
typedef struct _utimbuf          __ccw_fs_utimbuf_t;
#else
typedef struct stat              __ccw_fs_stat_t;
typedef struct utimbuf           __ccw_fs_utimbuf_t;
#endif

static int __ccw_stat(const __ccw_fs_c* __p, __ccw_fs_stat_t* __st) { return __ccw_fs_stat(__p, __st); }

bool __fs::exists(const __fs::path& __p) { __ccw_fs_stat_t __st; return __ccw_stat(__p.c_str(), &__st) == 0; }

bool __fs::is_directory(const __fs::path& __p) {
    __ccw_fs_stat_t __st; if (__ccw_stat(__p.c_str(), &__st) != 0) return false;
    return (__st.st_mode & S_IFMT) == S_IFDIR;
}
bool __fs::is_regular_file(const __fs::path& __p) {
    __ccw_fs_stat_t __st; if (__ccw_stat(__p.c_str(), &__st) != 0) return false;
    return (__st.st_mode & S_IFMT) == S_IFREG;
}
unsigned long __fs::file_size(const __fs::path& __p) {
    __ccw_fs_stat_t __st; if (__ccw_stat(__p.c_str(), &__st) != 0) return (unsigned long)-1;
    return (unsigned long)__st.st_size;
}
__fs::file_status __fs::status(const __fs::path& __p) {
    __ccw_fs_stat_t __st;
    if (__ccw_stat(__p.c_str(), &__st) != 0) return __fs::file_status(__fs::file_not_found);
    int __m = __st.st_mode & S_IFMT;
    __fs::perms __pm = __fs::__ccw_get_perms(__p);
    if (__m == S_IFDIR) return __fs::file_status(__fs::directory_file, __pm);
    if (__m == S_IFREG) return __fs::file_status(__fs::regular_file, __pm);
    return __fs::file_status(__fs::other_file, __pm);
}

bool __fs::create_directory(const __fs::path& __p) { return __ccw_fs_mkdir(__p.c_str()) == 0; }

bool __fs::create_directories(const __fs::path& __p) {
    if (__p.empty() || __fs::is_directory(__p)) return __fs::is_directory(__p);
    __fs::path __parent = __p.parent_path();
    if (!__parent.empty() && !__fs::exists(__parent)) __fs::create_directories(__parent);
    return __fs::create_directory(__p);
}

bool __fs::remove(const __fs::path& __p) {
    if (!__fs::exists(__p)) return false;
    if (__fs::is_directory(__p)) return __ccw_fs_rmdir(__p.c_str()) == 0;
    return __ccw_fs_remove(__p.c_str()) == 0;
}

bool __fs::rename(const __fs::path& __from, const __fs::path& __to) { return __ccw_fs_rename(__from.c_str(), __to.c_str()) == 0; }

unsigned long __fs::remove_all(const __fs::path& __p) {
    if (!__fs::exists(__p)) return 0;
    unsigned long __n = 0;
    if (__fs::is_directory(__p)) {
        for (__fs::directory_iterator __it(__p), __e; __it != __e; ++__it)
            __n += __fs::remove_all(__it->path());
    }
    if (__fs::remove(__p)) ++__n;
    return __n;
}

bool __fs::copy_file(const __fs::path& __from, const __fs::path& __to) {
    _CCW_STD::FILE* __ifp = __ccw_fs_fopen(__from.c_str(), __CCW_FS_S("rb"));
    if (!__ifp) return false;
    _CCW_STD::FILE* __ofp = __ccw_fs_fopen(__to.c_str(), __CCW_FS_S("wb"));
    if (!__ofp) { _CCW_STD::fclose(__ifp); return false; }
    char __buf[4096]; _CCW_STD::size_t __r; bool __ok = true;
    while ((__r = _CCW_STD::fread(__buf, 1, sizeof __buf, __ifp)) > 0)
        if (_CCW_STD::fwrite(__buf, 1, __r, __ofp) != __r) { __ok = false; break; }
    _CCW_STD::fclose(__ifp); _CCW_STD::fclose(__ofp);
    return __ok;
}

bool __fs::is_empty(const __fs::path& __p) {
    if (__fs::is_directory(__p)) { __fs::directory_iterator __it(__p), __e; return __it == __e; }
    return __fs::file_size(__p) == 0;
}

__fs::path __fs::temp_directory_path() {
    const __ccw_fs_c* __t = __ccw_fs_getenv(__CCW_FS_S("TMP"));
    if (!__t) __t = __ccw_fs_getenv(__CCW_FS_S("TEMP"));
    if (!__t) __t = __CCW_FS_S(".");
    return __fs::path(__ccw_fs_str(__t));
}

_CCW_STD::time_t __fs::__ccw_mtime(const __fs::path& __p) {
    __ccw_fs_stat_t __st;
    if (__ccw_stat(__p.c_str(), &__st) != 0) return (_CCW_STD::time_t)0;
    return __st.st_mtime;
}

bool __fs::__ccw_chdir(const __fs::path& __p) { return __ccw_fs_chdir(__p.c_str()) == 0; }

__fs::path __fs::current_path() {
    __ccw_fs_c __buf[1024];
    if (__ccw_fs_getcwd(__buf, (int)(sizeof(__buf) / sizeof(__buf[0]))))
        return __fs::path(__ccw_fs_str(__buf));
    return __fs::path();
}

struct __fs::directory_iterator::__impl {
    __ccw_find_handle_t  __h_;      // _findfirst handle (-1 when exhausted)
    __ccw_fs_finddata    __fd_;
    __ccw_fs_str         __base_;   // directory (with trailing separator)
    __fs::directory_entry __entry_;
    int                  __ref_;
};

static bool __ccw_is_dot(const __ccw_fs_c* __n) {
    return __n[0] == __CCW_FS_S('.') && (__n[1] == 0 || (__n[1] == __CCW_FS_S('.') && __n[2] == 0));
}

#define _CCW_FIND_DONE(im)  do {                                            \
        if ((im)->__h_ != (__ccw_find_handle_t)-1) {                        \
            _findclose((im)->__h_);                                         \
            (im)->__h_ = (__ccw_find_handle_t)-1;                           \
        }                                                                   \
    } while (0)

void __fs::directory_iterator::__release() {
    if (!__i_) return;
    if (--__i_->__ref_ == 0) { _CCW_FIND_DONE(__i_); delete __i_; }
    __i_ = 0;                    // become end
}

void __fs::directory_iterator::__advance() {
    for (;;) {
        if (!__ccw_is_dot(__i_->__fd_.name)) {
            __i_->__entry_.__assign(__fs::path(__i_->__base_ + __i_->__fd_.name));
            return;
        }
        if (__ccw_fs_findnext(__i_->__h_, &__i_->__fd_) != 0) {
            _CCW_FIND_DONE(__i_);
            __release();
            return;
        }
    }
}

__fs::directory_iterator::directory_iterator(const __fs::path& __p) : __i_(0) {
    __impl* __im = new __impl;
    __im->__ref_ = 1;
    __ccw_fs_str __b = __p.native();
    if (!__b.empty()) {
        __ccw_fs_c __c = __b[__b.size() - 1];
        if (__c != __CCW_FS_S('/') && __c != __CCW_FS_S('\\')) __b.push_back(__CCW_FS_S('\\'));
    }
    __im->__base_ = __b;
#if defined(_CCW_OS_DOS)
    __ccw_fs_str __spec = __b + __CCW_FS_S("*.*");
#else
    __ccw_fs_str __spec = __b + __CCW_FS_S("*");
#endif
    __im->__h_ = __ccw_fs_findfirst(__spec.c_str(), &__im->__fd_);
    if (__im->__h_ == (__ccw_find_handle_t)-1) { delete __im; __i_ = 0; return; }
    __i_ = __im;
    __advance();
}

__fs::directory_iterator::directory_iterator(const directory_iterator& __o) : __i_(__o.__i_) { if (__i_) ++__i_->__ref_; }

__fs::directory_iterator& __fs::directory_iterator::operator=(const directory_iterator& __o) {
    if (this != &__o) {
        __impl* __n = __o.__i_;
        if (__n) ++__n->__ref_;       // bump first: self-aliasing copies must survive
        __release();
        __i_ = __n;
    }
    return *this;
}

__fs::directory_iterator::~directory_iterator() { __release(); }

const __fs::directory_entry& __fs::directory_iterator::operator*() const { return __i_->__entry_; }
const __fs::directory_entry* __fs::directory_iterator::operator->() const { return &__i_->__entry_; }

__fs::directory_iterator& __fs::directory_iterator::operator++() {
    if (__i_) {
        if (__i_->__h_ == (__ccw_find_handle_t)-1 || __ccw_fs_findnext(__i_->__h_, &__i_->__fd_) != 0) {
            _CCW_FIND_DONE(__i_);
            __release();
        } else {
            __advance();
        }
    }
    return *this;
}

#if defined(_WIN32) || defined(__NT__)
extern "C" {
#if _CCW_FS_WCHAR
__declspec(dllimport) int  __stdcall GetDiskFreeSpaceExW(const wchar_t*, _ccw_ullong*, _ccw_ullong*, _ccw_ullong*);
__declspec(dllimport) unsigned long __stdcall GetFullPathNameW(const wchar_t*, unsigned long, wchar_t*, wchar_t**);
__declspec(dllimport) int  __stdcall CreateHardLinkW(const wchar_t*, const wchar_t*, void*);
__declspec(dllimport) unsigned long __stdcall GetFileAttributesW(const wchar_t*);
__declspec(dllimport) int  __stdcall SetFileAttributesW(const wchar_t*, unsigned long);
#define __ccw_Win32_GetDiskFreeSpaceEx GetDiskFreeSpaceExW
#define __ccw_Win32_GetFullPathName    GetFullPathNameW
#define __ccw_Win32_CreateHardLink     CreateHardLinkW
#define __ccw_Win32_GetFileAttributes  GetFileAttributesW
#define __ccw_Win32_SetFileAttributes  SetFileAttributesW
#define __ccw_Win32_CreateFile         CreateFileW
#else
__declspec(dllimport) int  __stdcall GetDiskFreeSpaceExA(const char*, _ccw_ullong*, _ccw_ullong*, _ccw_ullong*);
__declspec(dllimport) unsigned long __stdcall GetFullPathNameA(const char*, unsigned long, char*, char**);
__declspec(dllimport) int  __stdcall CreateHardLinkA(const char*, const char*, void*);
__declspec(dllimport) unsigned long __stdcall GetFileAttributesA(const char*);
__declspec(dllimport) int  __stdcall SetFileAttributesA(const char*, unsigned long);
#define __ccw_Win32_GetDiskFreeSpaceEx GetDiskFreeSpaceExA
#define __ccw_Win32_GetFullPathName    GetFullPathNameA
#define __ccw_Win32_CreateHardLink     CreateHardLinkA
#define __ccw_Win32_GetFileAttributes  GetFileAttributesA
#define __ccw_Win32_SetFileAttributes  SetFileAttributesA
#define __ccw_Win32_CreateFile         CreateFileA
#endif

struct _CCW_BY_HANDLE_FILE_INFORMATION {
    unsigned long __attrs;
    unsigned long __ctime_lo,  __ctime_hi;
    unsigned long __atime_lo,  __atime_hi;
    unsigned long __wtime_lo,  __wtime_hi;
    unsigned long __volume_serial;
    unsigned long __size_hi, __size_lo;
    unsigned long __links;
    unsigned long __index_hi, __index_lo;
};
#if _CCW_FS_WCHAR
__declspec(dllimport) void* __stdcall CreateFileW(const wchar_t*, unsigned long, unsigned long, void*,
                                                  unsigned long, unsigned long, void*);
#else
__declspec(dllimport) void* __stdcall CreateFileA(const char*, unsigned long, unsigned long, void*,
                                                  unsigned long, unsigned long, void*);
#endif
__declspec(dllimport) int  __stdcall GetFileInformationByHandle(void*, _CCW_BY_HANDLE_FILE_INFORMATION*);
__declspec(dllimport) int  __stdcall CloseHandle(void*);
}
#define _CCW_FILE_ATTRIBUTE_READONLY 0x00000001UL
#define _CCW_INVALID_FILE_ATTRIBUTES 0xFFFFFFFFUL

static bool __ccw_file_info(const __ccw_fs_c* __path, _CCW_BY_HANDLE_FILE_INFORMATION* __bi) {
    void* __h = __ccw_Win32_CreateFile(__path, 0UL, 0x00000007UL /* share read|write|delete */, 0,
                            3UL /* OPEN_EXISTING */, 0x02000000UL /* BACKUP_SEMANTICS */, 0);
    if (__h == (void*)-1) return false;
    int __ok = GetFileInformationByHandle(__h, __bi);
    CloseHandle(__h);
    return __ok != 0;
}
#endif

__fs::space_info __fs::space(const __fs::path& __p) {
    __fs::space_info __r;
    __r.capacity = __r.free = __r.available = (_ccw_ullong)-1;
#if defined(_WIN32) || defined(__NT__)
    _ccw_ullong __avail = 0, __total = 0, __free = 0;
    __ccw_fs_str __dir = __fs::is_directory(__p) ? __p.native() : __p.parent_path().native();
    if (__dir.empty()) __dir = __CCW_FS_S(".");
    if (__ccw_Win32_GetDiskFreeSpaceEx(__dir.c_str(), &__avail, &__total, &__free)) {
        __r.capacity  = __total;
        __r.free      = __free;
        __r.available = __avail;
    }
#elif defined(_CCW_OS_DOS)
    unsigned __drive = 0;
    _CCW_STD::string __s = __p.string();
    if (__s.size() >= 2 && __s[1] == ':') {
        char __c = __s[0];
        if (__c >= 'a' && __c <= 'z') __c = (char)(__c - 'a' + 'A');
        if (__c >= 'A' && __c <= 'Z') __drive = (unsigned)(__c - 'A' + 1);
    }
    struct _diskfree_t __df;
    if (_dos_getdiskfree(__drive, &__df) == 0) {
        _ccw_ullong __unit = (_ccw_ullong)__df.sectors_per_cluster * __df.bytes_per_sector;
        __r.capacity  = (_ccw_ullong)__df.total_clusters * __unit;
        __r.free      = (_ccw_ullong)__df.avail_clusters * __unit;
        __r.available = __r.free;    // DOS has no per-user quota, so free == available
    }
#else
    (void)__p;
#endif
    return __r;
}

void __fs::resize_file(const __fs::path& __p, _ccw_ullong __n) {
    if (__n > (_ccw_ullong)0x7FFFFFFFUL) return;   // chsize's length is a signed long
    __ccw_fs_stat_t __st;
    if (__ccw_stat(__p.c_str(), &__st) != 0) return;
    if (__n == (_ccw_ullong)__st.st_size) return;
    FILE* __f = __ccw_fs_fopen(__p.c_str(), __CCW_FS_S("r+b"));
    if (!__f) return;
    __ccw_chsize(__ccw_fileno(__f), (long)__n);
    fclose(__f);
}

#if defined(_CCW_OS_DOS)
static bool __ccw_same_name(const _CCW_STD::string& __a, const _CCW_STD::string& __b) {
    if (__a.size() != __b.size()) return false;
    for (_CCW_STD::string::size_type __i = 0; __i < __a.size(); ++__i) {
        char __x = __a[__i], __y = __b[__i];
        if (__x >= 'a' && __x <= 'z') __x = (char)(__x - 'a' + 'A');
        if (__y >= 'a' && __y <= 'z') __y = (char)(__y - 'a' + 'A');
        if (__x != __y) return false;
    }
    return true;
}
#endif

bool __fs::equivalent(const __fs::path& __a, const __fs::path& __b) {
    if (!__fs::exists(__a) || !__fs::exists(__b)) return false;
#if defined(_WIN32) || defined(__NT__)
    _CCW_BY_HANDLE_FILE_INFORMATION __ia, __ib;
    if (__ccw_file_info(__a.c_str(), &__ia) && __ccw_file_info(__b.c_str(), &__ib)) {
        return __ia.__volume_serial == __ib.__volume_serial
            && __ia.__index_hi      == __ib.__index_hi
            && __ia.__index_lo      == __ib.__index_lo;
    }
#endif
#if defined(_CCW_OS_DOS)
    return __ccw_same_name(__fs::canonical(__a).generic_string(),
                           __fs::canonical(__b).generic_string());
#else
    return __fs::canonical(__a).generic_string() == __fs::canonical(__b).generic_string();
#endif
}

_ccw_ullong __fs::hard_link_count(const __fs::path& __p) {
#if defined(_WIN32) || defined(__NT__)
    _CCW_BY_HANDLE_FILE_INFORMATION __bi;
    if (__ccw_file_info(__p.c_str(), &__bi)) return (_ccw_ullong)__bi.__links;
#endif
    __ccw_fs_stat_t __st;
    if (__ccw_stat(__p.c_str(), &__st) != 0) return (_ccw_ullong)-1;
    return (_ccw_ullong)__st.st_nlink;
}

bool __fs::__ccw_set_mtime(const __fs::path& __p, _CCW_STD::time_t __t) {
    __ccw_fs_stat_t __st;
    if (__ccw_stat(__p.c_str(), &__st) != 0)
        return false;
    __ccw_fs_utimbuf_t __u;
    __u.actime  = __st.st_atime;
    __u.modtime = __t;                          // file_clock's epoch is the C library's
    return __ccw_fs_utime(__p.c_str(), &__u) == 0;
}

void __fs::create_hard_link(const __fs::path& __to, const __fs::path& __link, _CCW_STD::error_code& __ec) {
    if (__fs::create_hard_link(__to, __link)) __ec.clear();
    else __ec = _CCW_STD::make_error_code(_CCW_STD::errc::operation_not_permitted);
}

static void __ccw_fs_no_symlink(_CCW_STD::error_code& __ec) {
    __ec = _CCW_STD::make_error_code(_CCW_STD::errc::function_not_supported);
}
void __fs::create_symlink(const __fs::path&, const __fs::path&, _CCW_STD::error_code& __ec) { __ccw_fs_no_symlink(__ec); }
void __fs::create_directory_symlink(const __fs::path&, const __fs::path&, _CCW_STD::error_code& __ec) { __ccw_fs_no_symlink(__ec); }
void __fs::copy_symlink(const __fs::path&, const __fs::path&, _CCW_STD::error_code& __ec) { __ccw_fs_no_symlink(__ec); }
__fs::path __fs::read_symlink(const __fs::path&, _CCW_STD::error_code& __ec) { __ccw_fs_no_symlink(__ec); return __fs::path(); }
__fs::path __fs::read_symlink(const __fs::path&) { return __fs::path(); }

bool __fs::create_hard_link(const __fs::path& __to, const __fs::path& __link) {
#if defined(_WIN32) || defined(__NT__)
    return __ccw_Win32_CreateHardLink(__link.c_str(), __to.c_str(), 0) != 0;
#else
    (void)__to; (void)__link;
    return false;
#endif
}

__fs::path __fs::canonical(const __fs::path& __p) {
#if defined(_WIN32) || defined(__NT__)
    __ccw_fs_c  __buf[1024];
    __ccw_fs_c* __fpart = 0;
    const unsigned long __cap = (unsigned long)(sizeof(__buf) / sizeof(__buf[0]));
    unsigned long __n = __ccw_Win32_GetFullPathName(__p.c_str(), __cap, __buf, &__fpart);
    if (__n > 0 && __n < __cap) return __fs::path(__ccw_fs_str(__buf, __n));
#elif defined(_CCW_OS_DOS)
    __ccw_fs_c __buf[260];
    if (__ccw_fs_fullpath(__buf, __p.c_str(), sizeof(__buf) / sizeof(__buf[0])))
        return __fs::path(__ccw_fs_str(__buf));
#endif
    return __fs::absolute(__p).lexically_normal();
}

void __fs::permissions(const __fs::path& __p, __fs::perms __prms, __fs::perm_options __opts) {
#if defined(_WIN32) || defined(__NT__)
    unsigned long __attr = __ccw_Win32_GetFileAttributes(__p.c_str());
    if (__attr == _CCW_INVALID_FILE_ATTRIBUTES) return;
    bool __writable;
    unsigned __w = (unsigned)__fs::perms::owner_write | (unsigned)__fs::perms::group_write
                 | (unsigned)__fs::perms::others_write;
    bool __want_write = (__prms.__v_ & __w) != 0;
    if (__opts.__v_ & (unsigned)__fs::perm_options::remove)      __writable = !__want_write && !(__attr & _CCW_FILE_ATTRIBUTE_READONLY);
    else if (__opts.__v_ & (unsigned)__fs::perm_options::add)    __writable = __want_write || !(__attr & _CCW_FILE_ATTRIBUTE_READONLY);
    else                                                          __writable = __want_write;
    unsigned long __new = __writable ? (__attr & ~_CCW_FILE_ATTRIBUTE_READONLY)
                                     : (__attr |  _CCW_FILE_ATTRIBUTE_READONLY);
    if (__new != __attr) __ccw_Win32_SetFileAttributes(__p.c_str(), __new);
#elif defined(_CCW_OS_DOS)
    bool __is_writable = (__ccw_fs_access(__p.c_str(), 2 /* W_OK */) == 0);
    unsigned __w = (unsigned)__fs::perms::owner_write | (unsigned)__fs::perms::group_write
                 | (unsigned)__fs::perms::others_write;
    bool __want_write = (__prms.__v_ & __w) != 0;
    bool __writable;
    if (__opts.__v_ & (unsigned)__fs::perm_options::remove)   __writable = __is_writable && !__want_write;
    else if (__opts.__v_ & (unsigned)__fs::perm_options::add) __writable = __is_writable ||  __want_write;
    else                                                       __writable = __want_write;
    if (__writable != __is_writable)
        __ccw_fs_chmod(__p.c_str(), __writable ? (S_IREAD | S_IWRITE) : S_IREAD);
#else
    (void)__p; (void)__prms; (void)__opts;
#endif
}

__fs::perms __fs::__ccw_get_perms(const __fs::path& __p) {
    unsigned __v = (unsigned)__fs::perms::owner_read | (unsigned)__fs::perms::group_read
                 | (unsigned)__fs::perms::others_read;
#if defined(_WIN32) || defined(__NT__)
    unsigned long __attr = __ccw_Win32_GetFileAttributes(__p.c_str());
    if (__attr == _CCW_INVALID_FILE_ATTRIBUTES) return __fs::perms((unsigned)__fs::perms::unknown);
    if (!(__attr & _CCW_FILE_ATTRIBUTE_READONLY))
        __v |= (unsigned)__fs::perms::owner_write | (unsigned)__fs::perms::group_write
             | (unsigned)__fs::perms::others_write;
#elif defined(_CCW_OS_DOS)
    if (__ccw_fs_access(__p.c_str(), 0 /* F_OK */) != 0) return __fs::perms((unsigned)__fs::perms::unknown);
    if (__ccw_fs_access(__p.c_str(), 2 /* W_OK */) == 0)
        __v |= (unsigned)__fs::perms::owner_write | (unsigned)__fs::perms::group_write
             | (unsigned)__fs::perms::others_write;
#else
    (void)__p;
#endif
    return __fs::perms(__v);
}

#endif  // _CCW_LIBCPP_FILESYSTEM
