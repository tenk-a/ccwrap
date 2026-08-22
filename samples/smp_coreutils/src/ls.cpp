/**
 *  @file   ls.cpp
 *  @brief  file lister.
 */
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32)
#include <io.h>
#include <windows.h>
#undef min
#undef max
#else
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/ioctl.h>
#endif

namespace fs = std::filesystem;

typedef unsigned long long  ls_u64;
typedef long long           ls_i64;

#if defined(_WIN32)
typedef wchar_t             ls_char;
 #if defined(__WATCOMC__)
typedef struct _stati64     ls_stat_t;
  #define LS_STAT_W(p, b)   ::_wstati64((p), (b))
  #define LS_STAT_A(p, b)   ::_stati64((p), (b))
 #else
typedef struct _stat64      ls_stat_t;
  #define LS_STAT_W(p, b)   ::_wstat64((p), (b))
  #define LS_STAT_A(p, b)   ::_stat64((p), (b))
 #endif
#else
typedef char                ls_char;
typedef struct stat         ls_stat_t;
#endif

#if !defined(S_IFMT)
 #define S_IFMT     0xF000
#endif
#if !defined(S_ISDIR)
 #define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#endif
#if !defined(S_ISREG)
 #define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#endif
#if !defined(S_ISCHR)
 #define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#endif

#if defined(_WIN32) && !defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
 #define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif

enum LsFormat  { LsFmtColumn, LsFmtLong, LsFmtSingle, LsFmtAcross, LsFmtComma };
enum LsSort    { LsSortName, LsSortTime, LsSortSize, LsSortExt, LsSortVer, LsSortNone };
enum LsTime    { LsTimeMtime, LsTimeAtime, LsTimeCtime };
enum LsInd     { LsIndNone, LsIndSlash, LsIndClassify };
enum LsQuote   { LsQLiteral, LsQC, LsQEscape, LsQShell, LsQShellAlways };
enum LsTstyle  { LsTsLocale, LsTsLongIso, LsTsFullIso, LsTsIso };
enum LsColor   { LsClrNever, LsClrAlways, LsClrAuto };
enum LsType    { LsTpOther, LsTpFile, LsTpDir, LsTpLink, LsTpFifo, LsTpSock, LsTpChr, LsTpBlk };

static void lsToUtf8(const char* s, std::string& out) {
    out.assign(s ? s : "");
}

#if defined(_WIN32)
static void lsToUtf8(const wchar_t* s, std::string& out) {
    out.clear();
    if (!s || !*s)
        return;
    int n = ::WideCharToMultiByte(CP_UTF8, 0, s, -1, 0, 0, 0, 0);
    if (n <= 1)
        return;
    out.resize((size_t)(n - 1));
    ::WideCharToMultiByte(CP_UTF8, 0, s, -1, &out[0], n, 0, 0);
}
static int lsStatRaw(const wchar_t* p, ls_stat_t* b) { return LS_STAT_W(p, b); }
static int lsStatRaw(const char* p, ls_stat_t* b)    { return LS_STAT_A(p, b); }
#endif

static int lsStatOf(const fs::path& p, ls_stat_t* b, bool follow) {
#if defined(_WIN32)
    (void)follow;
    return lsStatRaw(p.c_str(), b);
#else
    return follow ? ::stat(p.c_str(), b) : ::lstat(p.c_str(), b);
#endif
}

#if defined(_WIN32)
static ls_u64 lsFileTimeKey(const FILETIME& ft) {
    return ((ls_u64)ft.dwHighDateTime << 32) | (ls_u64)ft.dwLowDateTime;
}
static void lsTimeKeysRaw(const wchar_t* p, ls_u64* k) {
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!::GetFileAttributesExW(p, GetFileExInfoStandard, &fad))
        return;
    k[0] = lsFileTimeKey(fad.ftLastWriteTime);
    k[1] = lsFileTimeKey(fad.ftLastAccessTime);
    k[2] = lsFileTimeKey(fad.ftCreationTime);
}
static void lsTimeKeysRaw(const char* p, ls_u64* k) {
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!::GetFileAttributesExA(p, GetFileExInfoStandard, &fad))
        return;
    k[0] = lsFileTimeKey(fad.ftLastWriteTime);
    k[1] = lsFileTimeKey(fad.ftLastAccessTime);
    k[2] = lsFileTimeKey(fad.ftCreationTime);
}
static void lsTimeKeys(const fs::path& p, ls_u64* k) {
    k[0] = k[1] = k[2] = 0;
    lsTimeKeysRaw(p.c_str(), k);
}
#else
static void lsTimeKeys(const fs::path& p, ls_u64* k) {
    k[0] = k[1] = k[2] = 0;
    struct stat st;
    if (lstat(p.c_str(), &st) != 0)
        return;
 #if defined(__APPLE__)
    k[0] = (ls_u64)st.st_mtime * 1000000000u + (ls_u64)st.st_mtimespec.tv_nsec;
    k[1] = (ls_u64)st.st_atime * 1000000000u + (ls_u64)st.st_atimespec.tv_nsec;
    k[2] = (ls_u64)st.st_ctime * 1000000000u + (ls_u64)st.st_ctimespec.tv_nsec;
 #elif defined(st_mtime)
    k[0] = (ls_u64)st.st_mtim.tv_sec * 1000000000u + (ls_u64)st.st_mtim.tv_nsec;
    k[1] = (ls_u64)st.st_atim.tv_sec * 1000000000u + (ls_u64)st.st_atim.tv_nsec;
    k[2] = (ls_u64)st.st_ctim.tv_sec * 1000000000u + (ls_u64)st.st_ctim.tv_nsec;
 #endif
}
#endif

static bool lsIsSep(char c) {
#if defined(_WIN32)
    return c == '/' || c == '\\';
#else
    return c == '/';
#endif
}

static char lsSepOf(const std::string& s) {
#if defined(_WIN32)
    if (s.find('/') == std::string::npos && s.find('\\') != std::string::npos)
        return '\\';
#else
    (void)s;
#endif
    return '/';
}

static size_t lsBaseOffset(const fs::path::string_type& s) {
    size_t i = s.size();
    while (i > 0) {
        fs::path::value_type c = s[i - 1];
        if (c == (fs::path::value_type)'/')
            return i;
#if defined(_WIN32)
        if (c == (fs::path::value_type)'\\' || c == (fs::path::value_type)':')
            return i;
#endif
        --i;
    }
    return 0;
}

static fs::path lsStatablePath(const fs::path& p) {
    const fs::path::string_type& s = p.native();
    size_t n = s.size();
    while (n > 1) {
        fs::path::value_type c = s[n - 1];
        if (c != (fs::path::value_type)'/' && c != (fs::path::value_type)'\\')
            break;
#if defined(_WIN32)
        if (n == 3 && s[1] == (fs::path::value_type)':')
            break;
#endif
        --n;
    }
    if (n == s.size())
        return p;
    return fs::path(fs::path::string_type(s, 0, n));
}

static void lsU64Str(char* buf, size_t bufsz, ls_u64 v) {
    char   tmp[24];
    size_t i = 0;
    if (v == 0)
        tmp[i++] = '0';
    while (v != 0) {
        tmp[i++] = (char)('0' + (unsigned)(v % 10));
        v /= 10;
    }
    size_t j = 0;
    while (i > 0 && j + 1 < bufsz)
        buf[j++] = tmp[--i];
    buf[j] = '\0';
}

static void lsHumanStr(char* buf, size_t bufsz, ls_u64 v, bool si) {
    static const char units[] = "\0KMGTPE";
    ls_u64 base = si ? 1000u : 1024u;
    ls_u64 div  = 1;
    int    u    = 0;
    while (u < 6) {
        ls_u64 q = v / div;
        if (q < base)
            break;
        div *= base;
        ++u;
    }
    if (u == 0) {
        lsU64Str(buf, bufsz, v);
        return;
    }
    ls_u64   whole = v / div;
    ls_u64   rem   = v - whole * div;
    unsigned unit  = (unsigned char)units[u];
    if (whole < 10) {
        unsigned tenth = (unsigned)((rem * 10 + div - 1) / div);
        if (tenth >= 10) {
            ++whole;
            tenth = 0;
        }
        snprintf(buf, bufsz, "%u.%u%c", (unsigned)whole, tenth, (char)unit);
    } else {
        if (rem != 0)
            ++whole;
        snprintf(buf, bufsz, "%u%c", (unsigned)whole, (char)unit);
    }
}

static bool lsIsWideCp(unsigned long c) {
    return (c >= 0x1100 && c <= 0x115F)
        || (c >= 0x2E80 && c <= 0x303E)
        || (c >= 0x3041 && c <= 0x33FF)
        || (c >= 0x3400 && c <= 0x4DBF)
        || (c >= 0x4E00 && c <= 0x9FFF)
        || (c >= 0xA000 && c <= 0xA4CF)
        || (c >= 0xAC00 && c <= 0xD7A3)
        || (c >= 0xF900 && c <= 0xFAFF)
        || (c >= 0xFE30 && c <= 0xFE6F)
        || (c >= 0xFF00 && c <= 0xFF60)
        || (c >= 0xFFE0 && c <= 0xFFE6)
        || (c >= 0x20000 && c <= 0x3FFFD);
}

static size_t lsDispWidth(const std::string& s) {
    size_t w = 0;
    size_t i = 0;
    while (i < s.size()) {
        unsigned char  c   = (unsigned char)s[i];
        unsigned long  cp  = 0;
        size_t         len = 0;
        if (c < 0x80) {
            cp = c;  len = 1;
        } else if ((c & 0xE0) == 0xC0) {
            cp = c & 0x1F;  len = 2;
        } else if ((c & 0xF0) == 0xE0) {
            cp = c & 0x0F;  len = 3;
        } else if ((c & 0xF8) == 0xF0) {
            cp = c & 0x07;  len = 4;
        } else {
            ++i;  ++w;  continue;
        }
        if (i + len > s.size()) {
            ++i;  ++w;  continue;
        }
        for (size_t k = 1; k < len; ++k)
            cp = (cp << 6) | (unsigned long)((unsigned char)s[i + k] & 0x3F);
        i += len;
        w += lsIsWideCp(cp) ? 2 : 1;
    }
    return w;
}

static bool lsPatMatch(const char* p, const char* s) {
    for (;;) {
        char pc = *p;
        if (pc == '\0')
            return *s == '\0';
        if (pc == '*') {
            ++p;
            if (*p == '\0')
                return true;
            for (const char* q = s; ; ++q) {
                if (lsPatMatch(p, q))
                    return true;
                if (*q == '\0')
                    return false;
            }
        }
        if (*s == '\0')
            return false;
        if (pc == '?') {
            ++p;  ++s;
            continue;
        }
        if (pc == '[') {
            const char* q     = p + 1;
            bool        neg   = false;
            bool        hit   = false;
            bool        first = true;
            if (*q == '!' || *q == '^') {
                neg = true;
                ++q;
            }
            while (*q != '\0' && (*q != ']' || first)) {
                first = false;
                if (q[1] == '-' && q[2] != '\0' && q[2] != ']') {
                    if ((unsigned char)*s >= (unsigned char)q[0] && (unsigned char)*s <= (unsigned char)q[2])
                        hit = true;
                    q += 3;
                } else {
                    if (*s == *q)
                        hit = true;
                    ++q;
                }
            }
            if (*q != ']')
                return false;
            if (hit == neg)
                return false;
            p = q + 1;
            ++s;
            continue;
        }
        if (pc == '\\' && p[1] != '\0') {
            ++p;
            pc = *p;
        }
        if (pc != *s)
            return false;
        ++p;  ++s;
    }
}

static int lsStrCmp(const char* a, const char* b) {
    const unsigned char* p = (const unsigned char*)a;
    const unsigned char* q = (const unsigned char*)b;
    while (*p != 0 && *p == *q) {
        ++p;  ++q;
    }
    if (*p == *q)
        return 0;
    return *p < *q ? -1 : 1;
}

static bool lsIsDigit(char c) { return c >= '0' && c <= '9'; }

static int lsNatCmp(const char* a, const char* b) {
    while (*a != '\0' && *b != '\0') {
        if (lsIsDigit(*a) && lsIsDigit(*b)) {
            while (*a == '0' && lsIsDigit(a[1])) ++a;
            while (*b == '0' && lsIsDigit(b[1])) ++b;
            size_t na = 0, nb = 0;
            while (lsIsDigit(a[na])) ++na;
            while (lsIsDigit(b[nb])) ++nb;
            if (na != nb)
                return na < nb ? -1 : 1;
            int c = memcmp(a, b, na);
            if (c != 0)
                return c < 0 ? -1 : 1;
            a += na;
            b += nb;
            continue;
        }
        if (*a != *b)
            return (unsigned char)*a < (unsigned char)*b ? -1 : 1;
        ++a;  ++b;
    }
    if (*a == *b)
        return 0;
    return *a == '\0' ? -1 : 1;
}

static const char* lsExtOf(const std::string& s) {
    size_t n = s.size();
    for (size_t i = n; i > 1; --i) {
        if (s[i - 1] == '.')
            return s.c_str() + i - 1;
    }
    return s.c_str() + n;
}

static bool lsStdoutIsTty() {
#if defined(_WIN32)
    return ::_isatty(::_fileno(stdout)) != 0;
#else
    return ::isatty(::fileno(stdout)) != 0;
#endif
}

static size_t lsTermWidth() {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if (h != INVALID_HANDLE_VALUE && ::GetConsoleScreenBufferInfo(h, &csbi)) {
        int w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        if (w > 1)
            return (size_t)w;
    }
#elif defined(TIOCGWINSZ)
    struct winsize ws;
    if (::ioctl(1, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 1)
        return (size_t)ws.ws_col;
#endif
    const char* c = getenv("COLUMNS");
    if (c != 0) {
        long v = strtol(c, 0, 10);
        if (v > 1)
            return (size_t)v;
    }
    return 80;
}

#if defined(_WIN32)
static void lsEnableVt() {
    HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  m = 0;
    if (h != INVALID_HANDLE_VALUE && ::GetConsoleMode(h, &m))
        ::SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

struct ConsoleOutputCP {
    ConsoleOutputCP(int cp = 65001) {
        save_cp_ = ::GetConsoleOutputCP();
        ::SetConsoleOutputCP(cp);
    }
    ~ConsoleOutputCP() {
        ::SetConsoleOutputCP(save_cp_);
    }
private:
    int save_cp_;
};
#endif

struct Entry {
    fs::path        full;
    std::string     name;
    std::string     link;
    std::string     text;
    std::string     pre;
    ls_stat_t       st;
    ls_u64          tkey[3];
    size_t          width;
    int             type;
    char            ind;
    bool            has_st;
    bool            exec;

    Entry() : width(0), type(LsTpOther), ind(0), has_st(false), exec(false) {
        memset(&st, 0, sizeof(st));
        tkey[0] = tkey[1] = tkey[2] = 0;
    }
};

struct EntryLess {
    int  key;
    int  tfield;
    bool rev;
    bool dirs_first;

    EntryLess() : key(LsSortName), tfield(LsTimeMtime), rev(false), dirs_first(false) {}

    static time_t timeOf(const Entry& e, int tf) {
        if (tf == LsTimeAtime)
            return e.st.st_atime;
        if (tf == LsTimeCtime)
            return e.st.st_ctime;
        return e.st.st_mtime;
    }

    static ls_u64 keyOf(const Entry& e, int tf) {
        int i = (tf == LsTimeAtime) ? 1 : (tf == LsTimeCtime) ? 2 : 0;
        if (e.tkey[i] != 0)
            return e.tkey[i];
        return (ls_u64)timeOf(e, tf) * 1000000000u;
    }

    bool operator()(const Entry& a, const Entry& b) const {
        if (dirs_first) {
            bool da = (a.type == LsTpDir);
            bool db = (b.type == LsTpDir);
            if (da != db)
                return da;
        }
        int c = 0;
        if (key == LsSortTime) {
            ls_u64 ta = keyOf(a, tfield);
            ls_u64 tb = keyOf(b, tfield);
            c = (ta > tb) ? -1 : (ta < tb) ? 1 : 0;
        } else if (key == LsSortSize) {
            ls_i64 sa = (ls_i64)a.st.st_size;
            ls_i64 sb = (ls_i64)b.st.st_size;
            c = (sa > sb) ? -1 : (sa < sb) ? 1 : 0;
        } else if (key == LsSortExt) {
            c = lsStrCmp(lsExtOf(a.name), lsExtOf(b.name));
            if (c != 0)
                c = c < 0 ? -1 : 1;
        } else if (key == LsSortVer) {
            c = lsNatCmp(a.name.c_str(), b.name.c_str());
        }
        if (c == 0) {
            c = lsStrCmp(a.name.c_str(), b.name.c_str());
            if (c != 0)
                c = c < 0 ? -1 : 1;
        }
        return rev ? (c > 0) : (c < 0);
    }
};

class App {
public:
    enum { ExOk = 0, ExMinor = 1, ExFail = 2 };

    App()
        : format_(LsFmtColumn)
        , sort_(LsSortName)
        , tfield_(LsTimeMtime)
        , ind_(LsIndNone)
        , quote_(LsQLiteral)
        , tstyle_(LsTsLocale)
        , color_(LsClrNever)
        , all_(false)
        , almost_all_(false)
        , dirs_only_(false)
        , recursive_(false)
        , reverse_(false)
        , show_owner_(true)
        , show_group_(true)
        , numeric_ids_(false)
        , human_(false)
        , si_(false)
        , inode_(false)
        , blocks_(false)
        , follow_(false)
        , follow_arg_(false)
        , ignore_backups_(false)
        , hide_ctrl_(false)
        , group_dirs_first_(false)
        , fmt_given_(false)
        , width_(0)
        , tabsize_(8)
        , pre_ino_w_(0)
        , pre_blk_w_(0)
        , now_(0)
        , status_(ExOk)
        , first_block_(true)
        , headers_(false)
        , use_color_(false)
    {}

    int main(int argc, ls_char* argv[]);

private:
    bool parseArgs(int argc, ls_char* argv[]);
    bool parseLong(const std::string& arg, int& i, int argc, ls_char* argv[]);
    bool parseShort(const std::string& arg, int& i, int argc, ls_char* argv[]);
    bool argValue(const std::string& opt, const std::string& given, bool has_given,
                  int& i, int argc, ls_char* argv[], std::string& out);

    static int usage(int rc);
    static void version();

    void listOperands();
    void listDir(const fs::path& dir, const std::string& disp);
    bool gather(const fs::path& dir, std::vector<Entry>& out);
    void fillEntry(const fs::path& full, const std::string& name, Entry& e);
    void sortEntries(std::vector<Entry>& v);
    bool hiddenName(const std::string& name) const;
    bool ignoredName(const std::string& name) const;

    void setFormat(int f) { format_ = f; fmt_given_ = true; }
    void emit(std::vector<Entry>& v, bool with_total);
    void emitLong(std::vector<Entry>& v);
    void emitColumns(std::vector<Entry>& v, bool across);
    void emitSingle(std::vector<Entry>& v);
    void emitComma(std::vector<Entry>& v);

    void buildText(Entry& e);
    void quoteName(const std::string& in, std::string& out) const;
    void printName(const Entry& e);
    const char* colorOf(const Entry& e) const;
    void modeString(const Entry& e, char* buf) const;
    void ownerStr(const Entry& e, std::string& out) const;
    void groupStr(const Entry& e, std::string& out) const;
    void sizeStr(const Entry& e, std::string& out) const;
    void timeStr(const Entry& e, std::string& out) const;
    ls_u64 blocksOf(const Entry& e) const;
    void error(const char* what, const std::string& name);

private:
    int         format_;
    int         sort_;
    int         tfield_;
    int         ind_;
    int         quote_;
    int         tstyle_;
    int         color_;
    bool        all_;
    bool        almost_all_;
    bool        dirs_only_;
    bool        recursive_;
    bool        reverse_;
    bool        show_owner_;
    bool        show_group_;
    bool        numeric_ids_;
    bool        human_;
    bool        si_;
    bool        inode_;
    bool        blocks_;
    bool        follow_;
    bool        follow_arg_;
    bool        ignore_backups_;
    bool        hide_ctrl_;
    bool        group_dirs_first_;
    bool        fmt_given_;
    size_t      width_;
    size_t      tabsize_;
    size_t      pre_ino_w_;
    size_t      pre_blk_w_;
    time_t      now_;
    int         status_;
    bool        first_block_;
    bool        headers_;
    bool        use_color_;

    std::vector<std::string>    ignore_;
    std::vector<std::string>    hide_;
    std::vector<fs::path>       operands_;
    std::vector<std::string>    op_names_;
    std::vector<fs::path>       pending_;
    std::vector<std::string>    pending_disp_;
    std::string                 tmp_;
    std::string                 tmp2_;
};

int App::usage(int rc) {
    FILE* fp = (rc == ExOk) ? stdout : stderr;
    fputs(
        "Usage: ls [OPTION]... [FILE]...\n"
        "List information about the FILEs (the current directory by default).\n"
        "Sort entries alphabetically unless -t or -S or -U is given.\n"
        "\n"
        "  -a, --all                  do not ignore entries starting with .\n"
        "  -A, --almost-all           do not list implied . and ..\n"
        "  -B, --ignore-backups       do not list entries ending with ~\n"
        "  -c                         with -l: show ctime; with -lt: sort by it\n"
        "  -C                         list entries by columns\n"
        "  -d, --directory            list directories themselves, not contents\n"
        "  -f                         do not sort, enable -a\n"
        "  -F, --classify             append an indicator (*/=@|) to entries\n"
        "  -g                         like -l, but do not list the owner\n"
        "  -G, --no-group             in long format, do not list the group\n"
        "  -h, --human-readable       print sizes like 1K 234M 2G\n"
        "      --si                   likewise, but use powers of 1000\n"
        "  -H, --dereference-command-line  follow symlinks named on the command line\n"
        "  -i, --inode                print the index number of each file\n"
        "  -I, --ignore=PATTERN       do not list entries matching shell PATTERN\n"
        "  -k, --kibibytes            use 1024-byte blocks (the default here)\n"
        "  -l                         use a long listing format\n"
        "  -L, --dereference          show information for the link target\n"
        "  -m                         fill width with a comma separated list\n"
        "  -n, --numeric-uid-gid      like -l, but list numeric user and group IDs\n"
        "  -o                         like -l, but do not list the group\n"
        "  -p                         append / to directories\n"
        "  -q, --hide-control-chars   print ? instead of nongraphic characters\n"
        "  -Q, --quote-name           enclose entry names in double quotes\n"
        "  -r, --reverse              reverse order while sorting\n"
        "  -R, --recursive            list subdirectories recursively\n"
        "  -s, --size                 print the allocated size, in 1K blocks\n"
        "  -S                         sort by file size, largest first\n"
        "  -t                         sort by time, newest first\n"
        "  -u                         with -l: show atime; with -lt: sort by it\n"
        "  -U                         do not sort\n"
        "  -v                         natural sort of numbers within text\n"
        "  -w, --width=COLS           set output width to COLS (0 means no limit)\n"
        "  -x                         list entries by lines instead of by columns\n"
        "  -X                         sort alphabetically by entry extension\n"
        "  -1                         list one file per line\n"
        "      --color[=WHEN]         colorize the output (always, auto, never)\n"
        "      --format=WORD          across, comma, long, single-column, vertical\n"
        "      --full-time            like -l --time-style=full-iso\n"
        "      --group-directories-first  list directories before files\n"
        "      --hide=PATTERN         like -I, but ignored with -a or -A\n"
        "      --indicator-style=WORD  none, slash, classify\n"
        "      --quoting-style=WORD   literal, shell, shell-always, c, escape\n"
        "      --sort=WORD            none, size, time, version, extension\n"
        "      --time=WORD            atime, access, use, ctime, status, mtime\n"
        "      --time-style=STYLE     full-iso, long-iso, iso, locale\n"
        "      --help                 display this help and exit\n"
        "      --version              output version information and exit\n"
        "\n"
        "Exit status is 0 if OK, 1 if minor problems, 2 if serious trouble.\n",
        fp);
    return rc;
}

void App::version() {
    fputs("ls (ccwrap samples) 1.0\n"
          "License: Boost Software License Version 1.0\n", stdout);
}

void App::error(const char* what, const std::string& name) {
    fprintf(stderr, "ls: %s '%s': %s\n", what, name.c_str(), strerror(errno));
    status_ = ExFail;
}

bool App::argValue(const std::string& opt, const std::string& given, bool has_given,
                   int& i, int argc, ls_char* argv[], std::string& out) {
    if (has_given) {
        out = given;
        return true;
    }
    if (i + 1 < argc) {
        ++i;
        lsToUtf8(argv[i], out);
        return true;
    }
    fprintf(stderr, "ls: option '%s' requires an argument\n", opt.c_str());
    status_ = ExFail;
    return false;
}

bool App::parseLong(const std::string& arg, int& i, int argc, ls_char* argv[]) {
    std::string name(arg, 2, std::string::npos);
    std::string val;
    bool        has_val = false;
    size_t      eq      = name.find('=');
    if (eq != std::string::npos) {
        val.assign(name, eq + 1, std::string::npos);
        name.erase(eq);
        has_val = true;
    }

    if (name == "all")                  { all_ = true; }
    else if (name == "almost-all")      { almost_all_ = true; }
    else if (name == "ignore-backups")  { ignore_backups_ = true; }
    else if (name == "directory")       { dirs_only_ = true; }
    else if (name == "classify")        { ind_ = LsIndClassify; }
    else if (name == "no-group")        { show_group_ = false; }
    else if (name == "human-readable")  { human_ = true; si_ = false; }
    else if (name == "si")              { human_ = true; si_ = true; }
    else if (name == "dereference")     { follow_ = true; }
    else if (name == "dereference-command-line") { follow_arg_ = true; }
    else if (name == "inode")           { inode_ = true; }
    else if (name == "kibibytes")       { }
    else if (name == "numeric-uid-gid") { setFormat(LsFmtLong); numeric_ids_ = true; }
    else if (name == "hide-control-chars") { hide_ctrl_ = true; }
    else if (name == "quote-name")      { quote_ = LsQC; }
    else if (name == "reverse")         { reverse_ = true; }
    else if (name == "recursive")       { recursive_ = true; }
    else if (name == "size")            { blocks_ = true; }
    else if (name == "group-directories-first") { group_dirs_first_ = true; }
    else if (name == "full-time")       { setFormat(LsFmtLong); tstyle_ = LsTsFullIso; }
    else if (name == "help")            { usage(ExOk); return false; }
    else if (name == "version")         { version(); return false; }
    else if (name == "ignore") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        ignore_.push_back(tmp_);
    } else if (name == "hide") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        hide_.push_back(tmp_);
    } else if (name == "width") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        width_ = (size_t)strtol(tmp_.c_str(), 0, 10);
    } else if (name == "tabsize") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        tabsize_ = (size_t)strtol(tmp_.c_str(), 0, 10);
    } else if (name == "block-size") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
    } else if (name == "color") {
        if (!has_val)                   color_ = LsClrAlways;
        else if (val == "always" || val == "yes" || val == "force")  color_ = LsClrAlways;
        else if (val == "never"  || val == "no"  || val == "none")   color_ = LsClrNever;
        else if (val == "auto"   || val == "tty" || val == "if-tty") color_ = LsClrAuto;
        else {
            fprintf(stderr, "ls: invalid argument '%s' for '--color'\n", val.c_str());
            status_ = ExFail;
            return false;
        }
    } else if (name == "format") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        if (tmp_ == "across" || tmp_ == "horizontal")      setFormat(LsFmtAcross);
        else if (tmp_ == "commas")                         setFormat(LsFmtComma);
        else if (tmp_ == "comma")                          setFormat(LsFmtComma);
        else if (tmp_ == "long" || tmp_ == "verbose")      setFormat(LsFmtLong);
        else if (tmp_ == "single-column")                  setFormat(LsFmtSingle);
        else if (tmp_ == "vertical")                       setFormat(LsFmtColumn);
        else {
            fprintf(stderr, "ls: invalid argument '%s' for '--format'\n", tmp_.c_str());
            status_ = ExFail;
            return false;
        }
    } else if (name == "indicator-style") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        if (tmp_ == "none")            ind_ = LsIndNone;
        else if (tmp_ == "slash")      ind_ = LsIndSlash;
        else if (tmp_ == "classify")   ind_ = LsIndClassify;
        else if (tmp_ == "file-type")  ind_ = LsIndClassify;
        else {
            fprintf(stderr, "ls: invalid argument '%s' for '--indicator-style'\n", tmp_.c_str());
            status_ = ExFail;
            return false;
        }
    } else if (name == "quoting-style") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        if (tmp_ == "literal")             quote_ = LsQLiteral;
        else if (tmp_ == "c")              quote_ = LsQC;
        else if (tmp_ == "c-maybe")        quote_ = LsQC;
        else if (tmp_ == "escape")         quote_ = LsQEscape;
        else if (tmp_ == "shell")          quote_ = LsQShell;
        else if (tmp_ == "shell-always")   quote_ = LsQShellAlways;
        else if (tmp_ == "shell-escape")   quote_ = LsQShell;
        else {
            fprintf(stderr, "ls: invalid argument '%s' for '--quoting-style'\n", tmp_.c_str());
            status_ = ExFail;
            return false;
        }
    } else if (name == "sort") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        if (tmp_ == "none")             sort_ = LsSortNone;
        else if (tmp_ == "size")        sort_ = LsSortSize;
        else if (tmp_ == "time")        sort_ = LsSortTime;
        else if (tmp_ == "version")     sort_ = LsSortVer;
        else if (tmp_ == "extension")   sort_ = LsSortExt;
        else if (tmp_ == "name")        sort_ = LsSortName;
        else {
            fprintf(stderr, "ls: invalid argument '%s' for '--sort'\n", tmp_.c_str());
            status_ = ExFail;
            return false;
        }
    } else if (name == "time") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        if (tmp_ == "atime" || tmp_ == "access" || tmp_ == "use")        tfield_ = LsTimeAtime;
        else if (tmp_ == "ctime" || tmp_ == "status")                    tfield_ = LsTimeCtime;
        else if (tmp_ == "mtime" || tmp_ == "modification")              tfield_ = LsTimeMtime;
        else {
            fprintf(stderr, "ls: invalid argument '%s' for '--time'\n", tmp_.c_str());
            status_ = ExFail;
            return false;
        }
    } else if (name == "time-style") {
        if (!argValue(arg, val, has_val, i, argc, argv, tmp_))
            return false;
        if (tmp_ == "full-iso")        tstyle_ = LsTsFullIso;
        else if (tmp_ == "long-iso")   tstyle_ = LsTsLongIso;
        else if (tmp_ == "iso")        tstyle_ = LsTsIso;
        else if (tmp_ == "locale")     tstyle_ = LsTsLocale;
        else {
            fprintf(stderr, "ls: invalid argument '%s' for '--time-style'\n", tmp_.c_str());
            status_ = ExFail;
            return false;
        }
    } else {
        fprintf(stderr, "ls: unrecognized option '%s'\n", arg.c_str());
        usage(ExFail);
        status_ = ExFail;
        return false;
    }
    return true;
}

bool App::parseShort(const std::string& arg, int& i, int argc, ls_char* argv[]) {
    for (size_t k = 1; k < arg.size(); ++k) {
        char c = arg[k];
        switch (c) {
        case 'a': all_ = true;                          break;
        case 'A': almost_all_ = true;                    break;
        case 'B': ignore_backups_ = true;                break;
        case 'c': tfield_ = LsTimeCtime;                 break;
        case 'C': setFormat(LsFmtColumn);                 break;
        case 'd': dirs_only_ = true;                     break;
        case 'f': sort_ = LsSortNone; all_ = true;       break;
        case 'F': ind_ = LsIndClassify;                  break;
        case 'g': setFormat(LsFmtLong); show_owner_ = false;  break;
        case 'G': show_group_ = false;                   break;
        case 'h': human_ = true; si_ = false;            break;
        case 'H': follow_arg_ = true;                    break;
        case 'i': inode_ = true;                         break;
        case 'k':                                        break;
        case 'l': setFormat(LsFmtLong);                   break;
        case 'L': follow_ = true;                        break;
        case 'm': setFormat(LsFmtComma);                  break;
        case 'n': setFormat(LsFmtLong); numeric_ids_ = true;  break;
        case 'o': setFormat(LsFmtLong); show_group_ = false;  break;
        case 'p': ind_ = LsIndSlash;                     break;
        case 'q': hide_ctrl_ = true;                     break;
        case 'Q': quote_ = LsQC;                         break;
        case 'r': reverse_ = true;                       break;
        case 'R': recursive_ = true;                     break;
        case 's': blocks_ = true;                        break;
        case 'S': sort_ = LsSortSize;                    break;
        case 't': sort_ = LsSortTime;                    break;
        case 'u': tfield_ = LsTimeAtime;                 break;
        case 'U': sort_ = LsSortNone;                    break;
        case 'v': sort_ = LsSortVer;                     break;
        case 'x': setFormat(LsFmtAcross);                 break;
        case 'X': sort_ = LsSortExt;                     break;
        case '1': setFormat(LsFmtSingle);                 break;
        case 'w': case 'I': case 'T': {
            std::string rest(arg, k + 1, std::string::npos);
            std::string opt("-");
            opt += c;
            if (!argValue(opt, rest, !rest.empty(), i, argc, argv, tmp_))
                return false;
            if (c == 'w')
                width_ = (size_t)strtol(tmp_.c_str(), 0, 10);
            else if (c == 'I')
                ignore_.push_back(tmp_);
            else
                tabsize_ = (size_t)strtol(tmp_.c_str(), 0, 10);
            return true;
        }
        default:
            fprintf(stderr, "ls: invalid option -- '%c'\n", c);
            usage(ExFail);
            status_ = ExFail;
            return false;
        }
    }
    return true;
}

bool App::parseArgs(int argc, ls_char* argv[]) {
    bool        no_more = false;
    std::string arg;
    for (int i = 1; i < argc; ++i) {
        lsToUtf8(argv[i], arg);
        if (!no_more && arg.size() >= 2 && arg[0] == '-') {
            if (arg == "--") {
                no_more = true;
                continue;
            }
            if (arg[1] == '-') {
                if (!parseLong(arg, i, argc, argv))
                    return false;
            } else {
                if (!parseShort(arg, i, argc, argv))
                    return false;
            }
            continue;
        }
        operands_.push_back(fs::path(argv[i]));
        op_names_.push_back(arg);
    }
    return true;
}

bool App::hiddenName(const std::string& name) const {
    return !name.empty() && name[0] == '.';
}

bool App::ignoredName(const std::string& name) const {
    size_t i;
    if (ignore_backups_ && name.size() > 1 && name[name.size() - 1] == '~')
        return true;
    for (i = 0; i < ignore_.size(); ++i) {
        if (lsPatMatch(ignore_[i].c_str(), name.c_str()))
            return true;
    }
    if (!all_ && !almost_all_) {
        for (i = 0; i < hide_.size(); ++i) {
            if (lsPatMatch(hide_[i].c_str(), name.c_str()))
                return true;
        }
    }
    return false;
}

void App::fillEntry(const fs::path& full, const std::string& name, Entry& e) {
    e.full   = full;
    e.name   = name;
    e.link.clear();
    e.text.clear();
    e.width  = 0;
    e.ind    = 0;
    e.exec   = false;
    e.type   = LsTpOther;
    e.has_st = (lsStatOf(full, &e.st, follow_) == 0);
    lsTimeKeys(full, e.tkey);

    bool is_link = false;
    if (!follow_) {
        fs::file_status ls = fs::symlink_status(full);
        is_link = fs::is_symlink(ls);
    }
    if (is_link) {
        e.type = LsTpLink;
        fs::path tgt = fs::read_symlink(full);
        if (!tgt.empty())
            lsToUtf8(tgt.c_str(), e.link);
    } else if (e.has_st) {
        unsigned m = (unsigned)e.st.st_mode;
        if (S_ISDIR(m))
            e.type = LsTpDir;
        else if (S_ISCHR(m))
            e.type = LsTpChr;
#if defined(S_ISFIFO)
        else if (S_ISFIFO(m))
            e.type = LsTpFifo;
#endif
#if defined(S_ISBLK) && defined(S_IFBLK)
        else if (S_IFBLK != 0 && S_ISBLK(m))
            e.type = LsTpBlk;
#endif
#if defined(S_ISSOCK) && defined(S_IFSOCK)
        else if (S_IFSOCK != 0 && S_ISSOCK(m))
            e.type = LsTpSock;
#endif
        else if (S_ISREG(m))
            e.type = LsTpFile;
    }
    if (e.type == LsTpFile) {
        if ((e.st.st_mode & 0111) != 0)
            e.exec = true;
#if defined(_WIN32)
        const char* ext = lsExtOf(e.name);
        if (ext[0] == '.') {
            static const char* exts[] = { ".exe", ".com", ".bat", ".cmd", 0 };
            for (int k = 0; exts[k] != 0; ++k) {
                size_t n = strlen(exts[k]);
                if (strlen(ext) == n) {
                    size_t j = 0;
                    while (j < n) {
                        char a = ext[j];
                        if (a >= 'A' && a <= 'Z')
                            a = (char)(a - 'A' + 'a');
                        if (a != exts[k][j])
                            break;
                        ++j;
                    }
                    if (j == n) {
                        e.exec = true;
                        break;
                    }
                }
            }
        }
#endif
    }
}

bool App::gather(const fs::path& dir, std::vector<Entry>& out) {
    Entry       ent;
    std::string name;
    fs::path    child;

    if (all_) {
        static const char* dots[] = { ".", "..", 0 };
        for (int k = 0; dots[k] != 0; ++k) {
            child = dir;
            child /= fs::path(dots[k]);
            name  = dots[k];
            fillEntry(child, name, ent);
            out.push_back(ent);
        }
    }

    fs::directory_iterator it(dir);
    fs::directory_iterator end;
    for (; it != end; ++it) {
        const fs::path&               p = it->path();
        const fs::path::string_type&  s = p.native();
        lsToUtf8(s.c_str() + lsBaseOffset(s), name);
        if (name.empty())
            continue;
        if (!all_ && !almost_all_ && hiddenName(name))
            continue;
        if (ignoredName(name))
            continue;
        fillEntry(p, name, ent);
        out.push_back(ent);
    }
    return true;
}

void App::sortEntries(std::vector<Entry>& v) {
    if (sort_ == LsSortNone) {
        if (group_dirs_first_ || reverse_) {
            EntryLess less;
            less.key        = LsSortNone;
            less.tfield     = tfield_;
            less.rev        = false;
            less.dirs_first = group_dirs_first_;
            if (reverse_)
                std::reverse(v.begin(), v.end());
            if (group_dirs_first_)
                std::stable_sort(v.begin(), v.end(), less);
        }
        return;
    }
    EntryLess less;
    less.key        = sort_;
    less.tfield     = tfield_;
    less.rev        = reverse_;
    less.dirs_first = group_dirs_first_;
    std::sort(v.begin(), v.end(), less);
}

void App::quoteName(const std::string& in, std::string& out) const {
    out.clear();
    size_t i;
    bool   need = false;
    if (quote_ == LsQShell || quote_ == LsQShellAlways) {
        for (i = 0; i < in.size(); ++i) {
            unsigned char c = (unsigned char)in[i];
            if (c <= ' ' || c == '\'' || c == '"' || c == '\\' || c == '*' || c == '?'
             || c == '[' || c == ']' || c == '$' || c == '`' || c == '&' || c == ';'
             || c == '(' || c == ')' || c == '|' || c == '<' || c == '>' || c == '#') {
                need = true;
                break;
            }
        }
        if (quote_ == LsQShellAlways)
            need = true;
        if (!need) {
            out = in;
            return;
        }
        out += '\'';
        for (i = 0; i < in.size(); ++i) {
            if (in[i] == '\'')
                out += "'\\''";
            else
                out += in[i];
        }
        out += '\'';
        return;
    }
    if (quote_ == LsQC)
        out += '"';
    for (i = 0; i < in.size(); ++i) {
        unsigned char c = (unsigned char)in[i];
        if (quote_ == LsQLiteral) {
            if (hide_ctrl_ && c < 0x20)
                out += '?';
            else
                out += (char)c;
            continue;
        }
        switch (c) {
        case '\n': out += "\\n";  continue;
        case '\t': out += "\\t";  continue;
        case '\r': out += "\\r";  continue;
        case '\f': out += "\\f";  continue;
        case '\v': out += "\\v";  continue;
        case '\b': out += "\\b";  continue;
        case '\a': out += "\\a";  continue;
        case '\\': out += "\\\\"; continue;
        default:   break;
        }
        if (c == '"' && quote_ == LsQC) {
            out += "\\\"";
        } else if (c == ' ' && quote_ == LsQEscape) {
            out += "\\ ";
        } else if (c < 0x20 || c == 0x7F) {
            char b[8];
            snprintf(b, sizeof(b), "\\%03o", (unsigned)c);
            out += b;
        } else {
            out += (char)c;
        }
    }
    if (quote_ == LsQC)
        out += '"';
}

void App::buildText(Entry& e) {
    char b[32];
    quoteName(e.name, e.text);
    e.pre.clear();
    if (format_ != LsFmtLong && (inode_ || blocks_)) {
        if (inode_) {
            lsU64Str(b, sizeof(b), (ls_u64)e.st.st_ino);
            for (size_t k = strlen(b); k < pre_ino_w_; ++k)
                e.pre += ' ';
            e.pre += b;
            e.pre += ' ';
        }
        if (blocks_) {
            lsU64Str(b, sizeof(b), blocksOf(e));
            for (size_t k = strlen(b); k < pre_blk_w_; ++k)
                e.pre += ' ';
            e.pre += b;
            e.pre += ' ';
        }
    }
    e.ind = 0;
    if (ind_ == LsIndSlash) {
        if (e.type == LsTpDir)
            e.ind = '/';
    } else if (ind_ == LsIndClassify) {
        switch (e.type) {
        case LsTpDir:   e.ind = '/';  break;
        case LsTpLink:  e.ind = '@';  break;
        case LsTpFifo:  e.ind = '|';  break;
        case LsTpSock:  e.ind = '=';  break;
        default:
            if (e.exec)
                e.ind = '*';
            break;
        }
    }
    e.width = e.pre.size() + lsDispWidth(e.text) + (e.ind != 0 ? 1u : 0u);
}

const char* App::colorOf(const Entry& e) const {
    if (!use_color_)
        return 0;
    switch (e.type) {
    case LsTpDir:   return "01;34";
    case LsTpLink:  return "01;36";
    case LsTpFifo:  return "33";
    case LsTpSock:  return "01;35";
    case LsTpChr:   return "01;33";
    case LsTpBlk:   return "01;33";
    default:        break;
    }
    if (e.exec)
        return "01;32";
    const char* ext = lsExtOf(e.name);
    if (ext[0] == '.') {
        static const char* arc[] = { ".tar", ".tgz", ".zip", ".gz", ".bz2", ".xz", ".zst",
                                     ".7z", ".rar", ".lzh", ".lha", ".cab", ".arj", 0 };
        static const char* med[] = { ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".webp", ".tif",
                                     ".tiff", ".svg", ".mp3", ".mp4", ".mkv", ".avi", ".wav",
                                     ".flac", ".ogg", ".webm", ".mov", 0 };
        char low[16];
        size_t n = strlen(ext);
        if (n < sizeof(low)) {
            size_t j;
            for (j = 0; j < n; ++j) {
                char a = ext[j];
                if (a >= 'A' && a <= 'Z')
                    a = (char)(a - 'A' + 'a');
                low[j] = a;
            }
            low[n] = '\0';
            for (j = 0; arc[j] != 0; ++j) {
                if (strcmp(low, arc[j]) == 0)
                    return "01;31";
            }
            for (j = 0; med[j] != 0; ++j) {
                if (strcmp(low, med[j]) == 0)
                    return "01;35";
            }
        }
    }
    return 0;
}

void App::printName(const Entry& e) {
    const char* clr = colorOf(e);
    if (!e.pre.empty())
        fputs(e.pre.c_str(), stdout);
    if (clr != 0)
        printf("\033[%sm%s\033[0m", clr, e.text.c_str());
    else
        fputs(e.text.c_str(), stdout);
    if (e.ind != 0)
        putchar(e.ind);
}

void App::modeString(const Entry& e, char* buf) const {
    unsigned m = (unsigned)e.st.st_mode;
    switch (e.type) {
    case LsTpDir:   buf[0] = 'd';  break;
    case LsTpLink:  buf[0] = 'l';  break;
    case LsTpChr:   buf[0] = 'c';  break;
    case LsTpBlk:   buf[0] = 'b';  break;
    case LsTpFifo:  buf[0] = 'p';  break;
    case LsTpSock:  buf[0] = 's';  break;
    case LsTpFile:  buf[0] = '-';  break;
    default:        buf[0] = e.has_st ? '-' : '?';  break;
    }
    static const char* rwx = "rwxrwxrwx";
    for (int i = 0; i < 9; ++i) {
        unsigned bit = 0400u >> i;
        buf[1 + i] = (m & bit) != 0 ? rwx[i] : '-';
    }
    if ((m & 04000u) != 0)
        buf[3] = ((m & 0100u) != 0) ? 's' : 'S';
    if ((m & 02000u) != 0)
        buf[6] = ((m & 0010u) != 0) ? 's' : 'S';
    if ((m & 01000u) != 0)
        buf[9] = ((m & 0001u) != 0) ? 't' : 'T';
    buf[10] = '\0';
}

void App::ownerStr(const Entry& e, std::string& out) const {
    char b[32];
#if !defined(_WIN32)
    if (!numeric_ids_) {
        struct passwd* pw = ::getpwuid(e.st.st_uid);
        if (pw != 0 && pw->pw_name != 0) {
            out = pw->pw_name;
            return;
        }
    }
#else
    if (!numeric_ids_) {
        const char* u = getenv("USERNAME");
        if (u != 0 && *u != 0) {
            out = u;
            return;
        }
    }
#endif
    lsU64Str(b, sizeof(b), (ls_u64)e.st.st_uid);
    out = b;
}

void App::groupStr(const Entry& e, std::string& out) const {
    char b[32];
#if !defined(_WIN32)
    if (!numeric_ids_) {
        struct group* gr = ::getgrgid(e.st.st_gid);
        if (gr != 0 && gr->gr_name != 0) {
            out = gr->gr_name;
            return;
        }
    }
#endif
    lsU64Str(b, sizeof(b), (ls_u64)e.st.st_gid);
    out = b;
}

void App::sizeStr(const Entry& e, std::string& out) const {
    char b[32];
    if (!e.has_st) {
        out = "?";
        return;
    }
    if (e.type == LsTpChr || e.type == LsTpBlk) {
        lsU64Str(b, sizeof(b), (ls_u64)e.st.st_rdev);
        out = b;
        return;
    }
    if (human_)
        lsHumanStr(b, sizeof(b), (ls_u64)e.st.st_size, si_);
    else
        lsU64Str(b, sizeof(b), (ls_u64)e.st.st_size);
    out = b;
}

void App::timeStr(const Entry& e, std::string& out) const {
    char       buf[64];
    char       mon[16];
    time_t     t  = EntryLess::timeOf(e, tfield_);
    struct tm* tm = localtime(&t);

    out.clear();
    if (tm == 0) {
        out = "?";
        return;
    }
    if (tstyle_ == LsTsFullIso) {
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d.000000000",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec);
    } else if (tstyle_ == LsTsLongIso) {
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min);
    } else if (tstyle_ == LsTsIso) {
        if (t <= now_ + 3600 && t >= now_ - 15778476)
            snprintf(buf, sizeof(buf), "%02d-%02d %02d:%02d",
                tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min);
        else
            snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
                tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    } else {
        if (strftime(mon, sizeof(mon), "%b", tm) == 0)
            mon[0] = '\0';
        if (t <= now_ + 3600 && t >= now_ - 15778476)
            snprintf(buf, sizeof(buf), "%s %2d %02d:%02d",
                mon, tm->tm_mday, tm->tm_hour, tm->tm_min);
        else
            snprintf(buf, sizeof(buf), "%s %2d  %04d",
                mon, tm->tm_mday, tm->tm_year + 1900);
    }
    out = buf;
}

ls_u64 App::blocksOf(const Entry& e) const {
    if (!e.has_st)
        return 0;
#if defined(_WIN32) || defined(__WATCOMC__)
    return (ls_u64)(((ls_u64)e.st.st_size + 1023u) / 1024u);
#else
    return (ls_u64)e.st.st_blocks / 2u;
#endif
}

void App::emitLong(std::vector<Entry>& v) {
    std::vector<std::string> owners;
    std::vector<std::string> groups;
    std::vector<std::string> sizes;
    std::vector<std::string> times;
    std::vector<std::string> inos;
    std::vector<std::string> blks;
    char        b[32];
    size_t      i;
    size_t      w_link = 0, w_own = 0, w_grp = 0, w_size = 0, w_ino = 0, w_blk = 0;

    owners.resize(v.size());
    groups.resize(v.size());
    sizes.resize(v.size());
    times.resize(v.size());
    if (inode_)
        inos.resize(v.size());
    if (blocks_)
        blks.resize(v.size());

    for (i = 0; i < v.size(); ++i) {
        Entry& e = v[i];
        buildText(e);
        if (show_owner_)
            ownerStr(e, owners[i]);
        if (show_group_)
            groupStr(e, groups[i]);
        sizeStr(e, sizes[i]);
        timeStr(e, times[i]);
        lsU64Str(b, sizeof(b), (ls_u64)e.st.st_nlink);
        if (strlen(b) > w_link)
            w_link = strlen(b);
        if (owners[i].size() > w_own)
            w_own = owners[i].size();
        if (groups[i].size() > w_grp)
            w_grp = groups[i].size();
        if (sizes[i].size() > w_size)
            w_size = sizes[i].size();
        if (inode_) {
            lsU64Str(b, sizeof(b), (ls_u64)e.st.st_ino);
            inos[i] = b;
            if (inos[i].size() > w_ino)
                w_ino = inos[i].size();
        }
        if (blocks_) {
            lsU64Str(b, sizeof(b), blocksOf(e));
            blks[i] = b;
            if (blks[i].size() > w_blk)
                w_blk = blks[i].size();
        }
    }

    for (i = 0; i < v.size(); ++i) {
        Entry& e = v[i];
        char   mode[12];
        if (inode_)
            printf("%*s ", (int)w_ino, inos[i].c_str());
        if (blocks_)
            printf("%*s ", (int)w_blk, blks[i].c_str());
        modeString(e, mode);
        lsU64Str(b, sizeof(b), (ls_u64)e.st.st_nlink);
        printf("%s %*s", mode, (int)w_link, b);
        if (show_owner_)
            printf(" %-*s", (int)w_own, owners[i].c_str());
        if (show_group_)
            printf(" %-*s", (int)w_grp, groups[i].c_str());
        printf(" %*s %s ", (int)w_size, sizes[i].c_str(), times[i].c_str());
        printName(e);
        if (e.type == LsTpLink && !e.link.empty()) {
            quoteName(e.link, tmp2_);
            printf(" -> %s", tmp2_.c_str());
        }
        putchar('\n');
    }
}

void App::emitSingle(std::vector<Entry>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        buildText(v[i]);
        printName(v[i]);
        putchar('\n');
    }
}

void App::emitComma(std::vector<Entry>& v) {
    size_t pos = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        buildText(v[i]);
        size_t w = v[i].width + (i + 1 < v.size() ? 1u : 0u);
        if (pos != 0) {
            if (width_ != 0 && pos + 1 + w > width_) {
                putchar('\n');
                pos = 0;
            } else {
                putchar(' ');
                ++pos;
            }
        }
        printName(v[i]);
        pos += v[i].width;
        if (i + 1 < v.size()) {
            putchar(',');
            ++pos;
        }
    }
    if (!v.empty())
        putchar('\n');
}

void App::emitColumns(std::vector<Entry>& v, bool across) {
    size_t n = v.size();
    size_t i;
    if (n == 0)
        return;
    for (i = 0; i < n; ++i)
        buildText(v[i]);

    size_t maxw = 0;
    for (i = 0; i < n; ++i) {
        if (v[i].width > maxw)
            maxw = v[i].width;
    }
    size_t limit = (width_ != 0) ? width_ : 0;
    size_t cols  = n;
    if (limit != 0) {
        cols = limit / 3 + 1;
        if (cols > n)
            cols = n;
        if (cols == 0)
            cols = 1;
    }

    std::vector<size_t> colw;
    size_t rows = n;
    for (; cols > 1; --cols) {
        rows = (n + cols - 1) / cols;
        colw.assign(cols, 0);
        size_t sum = 0;
        for (i = 0; i < n; ++i) {
            size_t c = across ? (i % cols) : (i / rows);
            if (c >= cols)
                continue;
            if (v[i].width > colw[c])
                colw[c] = v[i].width;
        }
        for (i = 0; i < cols; ++i)
            sum += colw[i] + (i + 1 < cols ? 2u : 0u);
        if (limit == 0 || sum <= limit)
            break;
    }
    if (cols <= 1) {
        cols = 1;
        rows = n;
        colw.assign(1, maxw);
    }

    for (size_t r = 0; r < rows; ++r) {
        size_t pos   = 0;
        size_t start = 0;
        for (size_t c = 0; c < cols; ++c) {
            size_t idx = across ? (r * cols + c) : (c * rows + r);
            if (idx >= n)
                continue;
            printName(v[idx]);
            pos = start + v[idx].width;
            bool last = across ? (c + 1 == cols || idx + 1 == n)
                               : (c + 1 == cols || idx + rows >= n);
            if (last)
                break;
            start += colw[c] + 2;
            while (pos < start) {
                if (tabsize_ != 0 && start / tabsize_ > (pos + 1) / tabsize_) {
                    putchar('\t');
                    pos += tabsize_ - pos % tabsize_;
                } else {
                    putchar(' ');
                    ++pos;
                }
            }
        }
        putchar('\n');
    }
}

void App::emit(std::vector<Entry>& v, bool with_total) {
    size_t i;
    if (format_ != LsFmtLong && (inode_ || blocks_)) {
        char b[32];
        pre_ino_w_ = 0;
        pre_blk_w_ = 0;
        for (i = 0; i < v.size(); ++i) {
            if (inode_) {
                lsU64Str(b, sizeof(b), (ls_u64)v[i].st.st_ino);
                if (strlen(b) > pre_ino_w_)
                    pre_ino_w_ = strlen(b);
            }
            if (blocks_) {
                lsU64Str(b, sizeof(b), blocksOf(v[i]));
                if (strlen(b) > pre_blk_w_)
                    pre_blk_w_ = strlen(b);
            }
        }
    }
    if (with_total && (format_ == LsFmtLong || blocks_)) {
        char   b[32];
        ls_u64 total = 0;
        for (i = 0; i < v.size(); ++i)
            total += blocksOf(v[i]);
        if (human_)
            lsHumanStr(b, sizeof(b), total * 1024u, si_);
        else
            lsU64Str(b, sizeof(b), total);
        printf("total %s\n", b);
    }
    switch (format_) {
    case LsFmtLong:    emitLong(v);            break;
    case LsFmtSingle:  emitSingle(v);          break;
    case LsFmtComma:   emitComma(v);           break;
    case LsFmtAcross:  emitColumns(v, true);   break;
    default:           emitColumns(v, false);  break;
    }
}

void App::listDir(const fs::path& dir, const std::string& disp) {
    std::vector<Entry> v;

    if (headers_) {
        if (!first_block_)
            putchar('\n');
        printf("%s:\n", disp.c_str());
    } else if (!first_block_) {
        putchar('\n');
    }
    first_block_ = false;

    gather(dir, v);
    sortEntries(v);
    emit(v, true);

    if (recursive_) {
        std::vector<fs::path>    subs;
        std::vector<std::string> subd;
        for (size_t i = 0; i < v.size(); ++i) {
            if (v[i].type != LsTpDir)
                continue;
            if (v[i].name == "." || v[i].name == "..")
                continue;
            subs.push_back(v[i].full);
            tmp_ = disp;
            if (!tmp_.empty() && !lsIsSep(tmp_[tmp_.size() - 1]))
                tmp_ += lsSepOf(tmp_);
            tmp_ += v[i].name;
            subd.push_back(tmp_);
        }
        for (size_t k = subs.size(); k > 0; --k) {
            pending_.push_back(subs[k - 1]);
            pending_disp_.push_back(subd[k - 1]);
        }
    }
}

void App::listOperands() {
    std::vector<Entry>       files;
    std::vector<fs::path>    dirs;
    std::vector<std::string> dirnames;
    Entry                    ent;
    size_t                   i;

    for (i = 0; i < operands_.size(); ++i) {
        fs::path sp = lsStatablePath(operands_[i]);
        ls_stat_t st;
        memset(&st, 0, sizeof(st));
        errno = 0;
        if (lsStatOf(sp, &st, true) != 0) {
            error("cannot access", op_names_[i]);
            continue;
        }
        bool is_dir = S_ISDIR((unsigned)st.st_mode) != 0;
        if (is_dir && !dirs_only_) {
            dirs.push_back(sp);
            dirnames.push_back(op_names_[i]);
        } else {
            bool save = follow_;
            if (follow_arg_)
                follow_ = true;
            fillEntry(sp, op_names_[i], ent);
            follow_ = save;
            files.push_back(ent);
        }
    }

    headers_ = recursive_ || dirs.size() > 1 || (!files.empty() && !dirs.empty());

    if (!files.empty()) {
        sortEntries(files);
        emit(files, false);
        first_block_ = false;
    }

    if (!dirs.empty()) {
        std::vector<Entry> dv;
        dv.resize(dirs.size());
        for (i = 0; i < dirs.size(); ++i) {
            dv[i].full = dirs[i];
            dv[i].name = dirnames[i];
            dv[i].type = LsTpDir;
            lsStatOf(dirs[i], &dv[i].st, true);
            dv[i].has_st = true;
        }
        sortEntries(dv);
        for (i = dv.size(); i > 0; --i) {
            pending_.push_back(dv[i - 1].full);
            pending_disp_.push_back(dv[i - 1].name);
        }
        while (!pending_.empty()) {
            fs::path    dir  = pending_.back();
            std::string disp = pending_disp_.back();
            pending_.pop_back();
            pending_disp_.pop_back();
            listDir(dir, disp);
        }
    }
}

int App::main(int argc, ls_char* argv[]) {
    setlocale(LC_ALL, "");

    if (!parseArgs(argc, argv))
        return status_;

    if (operands_.empty()) {
        operands_.push_back(fs::path("."));
        op_names_.push_back(".");
    }

    if (!fmt_given_ && !lsStdoutIsTty())
        format_ = LsFmtSingle;
    if (width_ == 0)
        width_ = lsTermWidth();
    if (color_ == LsClrAlways)
        use_color_ = true;
    else if (color_ == LsClrAuto)
        use_color_ = lsStdoutIsTty();
#if defined(_WIN32)
    if (use_color_)
        lsEnableVt();
#endif
    if (all_)
        almost_all_ = false;
    now_ = time(0);

    listOperands();
    return status_;
}

#if !defined(_WIN32)
int main(int argc, char* argv[]) {
    return App().main(argc, argv);
}
#else
int wmain(int argc, wchar_t* argv[]) {
    ConsoleOutputCP utf8console(65001);
    return App().main(argc, argv);
}
#endif
