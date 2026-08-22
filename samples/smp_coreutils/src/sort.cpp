/**
 *  @file   sort.cpp
 *  @brief  line sorter.
 */
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <errno.h>
#include <limits.h>

#if defined(_WIN32)
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#undef min
#undef max
#endif

typedef unsigned long long  cw_u64;

#if defined(_WIN32)
typedef wchar_t             cw_char;
#else
typedef char                cw_char;
#endif

enum CwKeyFlag {
    CwSkipS = 0x0001,
    CwSkipE = 0x0002,
    CwDict  = 0x0004,
    CwFold  = 0x0008,
    CwGen   = 0x0010,
    CwHuman = 0x0020,
    CwPrint = 0x0040,
    CwMonth = 0x0080,
    CwNum   = 0x0100,
    CwRev   = 0x0200,
    CwVer   = 0x0400
};

static const unsigned CwOrderMask = CwSkipS | CwSkipE | CwDict | CwFold | CwGen
                                  | CwHuman | CwPrint | CwMonth | CwNum | CwVer;

static void cwToUtf8(const char* s, std::string& out) {
    out.assign(s ? s : "");
}

#if defined(_WIN32)
static void cwToUtf8(const wchar_t* s, std::string& out) {
    out.clear();
    if (!s || !*s)
        return;
    int n = ::WideCharToMultiByte(CP_UTF8, 0, s, -1, 0, 0, 0, 0);
    if (n <= 1)
        return;
    out.resize((size_t)(n - 1));
    ::WideCharToMultiByte(CP_UTF8, 0, s, -1, &out[0], n, 0, 0);
}
static FILE* cwFopen(const wchar_t* p, bool write) {
    return ::_wfopen(p, write ? L"wb" : L"rb");
}
#else
static FILE* cwFopen(const char* p, bool write) {
    return fopen(p, write ? "wb" : "rb");
}
#endif

static void cwBinaryMode(FILE* fp) {
#if defined(_WIN32)
    ::_setmode(::_fileno(fp), _O_BINARY);
#else
    (void)fp;
#endif
}

static bool cwIsBlank(unsigned char c) {
    return c == ' ' || c == '\t';
}

static bool cwIsDigit(unsigned char c) {
    return c >= '0' && c <= '9';
}

static bool cwIsAlpha(unsigned char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static bool cwIsAlnum(unsigned char c) {
    return cwIsDigit(c) || cwIsAlpha(c);
}

static char cwUpper(char c) {
    if (c >= 'a' && c <= 'z')
        return (char)(c - 'a' + 'A');
    return c;
}

static bool cwIgnored(unsigned char c, unsigned flags) {
    if ((flags & CwDict) != 0)
        return !(cwIsAlnum(c) || cwIsBlank(c));
    if ((flags & CwPrint) != 0)
        return c < 0x20 || c == 0x7F;
    return false;
}

struct Rec {
    const char* p;
    size_t      n;
};

struct Key {
    size_t   sword;
    size_t   schar;
    size_t   eword;
    size_t   echar;
    bool     sword_given;
    bool     eword_given;
    unsigned flags;

    Key() : sword(0), schar(0), eword(0), echar(0)
          , sword_given(false), eword_given(false), flags(0) {}
};

struct Cfg {
    std::vector<Key> keys;
    char             tab;
    bool             tab_given;
    bool             reverse;
    bool             stable;
    bool             unique;

    Cfg() : tab('\0'), tab_given(false), reverse(false), stable(false), unique(false) {}
};

static void cwKeyRange(const Cfg& cfg, const Key& k, const Rec& r,
                       const char** kb, const char** ke) {
    const char* line = r.p;
    const char* lim  = r.p + r.n;
    const char* ptr  = line;
    size_t      i;

    if (k.sword_given) {
        size_t sword = k.sword;
        if (cfg.tab_given) {
            while (ptr < lim && sword-- > 0) {
                while (ptr < lim && *ptr != cfg.tab)
                    ++ptr;
                if (ptr < lim)
                    ++ptr;
            }
        } else {
            while (ptr < lim && sword-- > 0) {
                while (ptr < lim && cwIsBlank((unsigned char)*ptr))
                    ++ptr;
                while (ptr < lim && !cwIsBlank((unsigned char)*ptr))
                    ++ptr;
            }
        }
        if ((k.flags & CwSkipS) != 0)
            while (ptr < lim && cwIsBlank((unsigned char)*ptr))
                ++ptr;
        for (i = 0; i < k.schar && ptr < lim; ++i)
            ++ptr;
    } else if ((k.flags & CwSkipS) != 0) {
        while (ptr < lim && cwIsBlank((unsigned char)*ptr))
            ++ptr;
    }
    const char* beg = ptr;

    const char* end = lim;
    if (k.eword_given) {
        size_t eword = k.eword;
        size_t echar = k.echar;
        ptr = line;
        if (echar == 0)
            ++eword;
        if (cfg.tab_given) {
            while (ptr < lim && eword-- > 0) {
                while (ptr < lim && *ptr != cfg.tab)
                    ++ptr;
                if (ptr < lim && (eword != 0 || echar != 0))
                    ++ptr;
            }
        } else {
            while (ptr < lim && eword-- > 0) {
                while (ptr < lim && cwIsBlank((unsigned char)*ptr))
                    ++ptr;
                while (ptr < lim && !cwIsBlank((unsigned char)*ptr))
                    ++ptr;
            }
        }
        if ((k.flags & CwSkipE) != 0)
            while (ptr < lim && cwIsBlank((unsigned char)*ptr))
                ++ptr;
        for (i = 0; i < echar && ptr < lim; ++i)
            ++ptr;
        end = ptr;
    }
    if (end < beg)
        end = beg;
    *kb = beg;
    *ke = end;
}

static int cwPlainCompare(const char* a, size_t an, const char* b, size_t bn) {
    size_t n = (an < bn) ? an : bn;
    for (size_t i = 0; i < n; ++i) {
        unsigned char ca = (unsigned char)a[i];
        unsigned char cb = (unsigned char)b[i];
        if (ca != cb)
            return ca < cb ? -1 : 1;
    }
    if (an == bn)
        return 0;
    return an < bn ? -1 : 1;
}

static int cwTextCompare(const char* a, size_t an, const char* b, size_t bn, unsigned flags) {
    if ((flags & (CwDict | CwPrint | CwFold)) == 0)
        return cwPlainCompare(a, an, b, bn);
    size_t i = 0;
    size_t j = 0;
    for (;;) {
        while (i < an && cwIgnored((unsigned char)a[i], flags))
            ++i;
        while (j < bn && cwIgnored((unsigned char)b[j], flags))
            ++j;
        if (i >= an || j >= bn)
            break;
        unsigned char ca = (unsigned char)a[i];
        unsigned char cb = (unsigned char)b[j];
        if ((flags & CwFold) != 0) {
            ca = (unsigned char)cwUpper((char)ca);
            cb = (unsigned char)cwUpper((char)cb);
        }
        if (ca != cb)
            return ca < cb ? -1 : 1;
        ++i;
        ++j;
    }
    while (i < an && cwIgnored((unsigned char)a[i], flags))
        ++i;
    while (j < bn && cwIgnored((unsigned char)b[j], flags))
        ++j;
    if (i < an)
        return 1;
    if (j < bn)
        return -1;
    return 0;
}

struct NumV {
    bool        neg;
    const char* ip;
    size_t      ilen;
    const char* fp;
    size_t      flen;
    bool        zero;
};

static void cwParseNum(const char* s, const char* e, NumV& v) {
    v.neg  = false;
    v.ip   = s;
    v.ilen = 0;
    v.fp   = s;
    v.flen = 0;
    while (s < e && cwIsBlank((unsigned char)*s))
        ++s;
    if (s < e && *s == '-') {
        v.neg = true;
        ++s;
    }
    while (s < e && *s == '0')
        ++s;
    v.ip = s;
    while (s < e && cwIsDigit((unsigned char)*s))
        ++s;
    v.ilen = (size_t)(s - v.ip);
    if (s < e && *s == '.') {
        ++s;
        v.fp = s;
        while (s < e && cwIsDigit((unsigned char)*s))
            ++s;
        v.flen = (size_t)(s - v.fp);
        while (v.flen > 0 && v.fp[v.flen - 1] == '0')
            --v.flen;
    }
    v.zero = (v.ilen == 0 && v.flen == 0);
}

static int cwMagCompare(const NumV& a, const NumV& b) {
    if (a.ilen != b.ilen)
        return a.ilen < b.ilen ? -1 : 1;
    for (size_t i = 0; i < a.ilen; ++i) {
        if (a.ip[i] != b.ip[i])
            return (unsigned char)a.ip[i] < (unsigned char)b.ip[i] ? -1 : 1;
    }
    size_t n = (a.flen < b.flen) ? a.flen : b.flen;
    for (size_t i = 0; i < n; ++i) {
        if (a.fp[i] != b.fp[i])
            return (unsigned char)a.fp[i] < (unsigned char)b.fp[i] ? -1 : 1;
    }
    if (a.flen == b.flen)
        return 0;
    return a.flen < b.flen ? -1 : 1;
}

static int cwNumCompare(const char* a, const char* ae, const char* b, const char* be) {
    NumV va;
    NumV vb;
    cwParseNum(a, ae, va);
    cwParseNum(b, be, vb);
    if (va.zero && vb.zero)
        return 0;
    if (va.zero)
        return vb.neg ? 1 : -1;
    if (vb.zero)
        return va.neg ? -1 : 1;
    if (va.neg != vb.neg)
        return va.neg ? -1 : 1;
    int m = cwMagCompare(va, vb);
    return va.neg ? -m : m;
}

static double cwToDouble(const char* s, const char* e, bool& ok) {
    char   buf[256];
    size_t n = (size_t)(e - s);
    if (n >= sizeof(buf))
        n = sizeof(buf) - 1;
    memcpy(buf, s, n);
    buf[n] = '\0';
    char* p = buf;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\v' || *p == '\f' || *p == '\r')
        ++p;
    char*  end = p;
    double v   = strtod(p, &end);
    ok = (end != p);
    return v;
}

static int cwGenCompare(const char* a, const char* ae, const char* b, const char* be) {
    bool   ok_a = false;
    bool   ok_b = false;
    double da   = cwToDouble(a, ae, ok_a);
    double db   = cwToDouble(b, be, ok_b);
    if (!ok_a)
        return ok_b ? -1 : 0;
    if (!ok_b)
        return 1;
    if (da < db)
        return -1;
    if (da > db)
        return 1;
    return 0;
}

static int cwSuffixOrder(const char* s, const char* e) {
    while (s < e && cwIsBlank((unsigned char)*s))
        ++s;
    if (s < e && (*s == '-' || *s == '+'))
        ++s;
    while (s < e && (cwIsDigit((unsigned char)*s) || *s == '.'))
        ++s;
    if (s >= e)
        return 0;
    switch (*s) {
    case 'k': case 'K': return 1;
    case 'M': return 2;
    case 'G': return 3;
    case 'T': return 4;
    case 'P': return 5;
    case 'E': return 6;
    case 'Z': return 7;
    case 'Y': return 8;
    default:  return 0;
    }
}

static int cwHumanCompare(const char* a, const char* ae, const char* b, const char* be) {
    NumV va;
    NumV vb;
    cwParseNum(a, ae, va);
    cwParseNum(b, be, vb);
    int sa = va.zero ? 0 : (va.neg ? -1 : 1);
    int sb = vb.zero ? 0 : (vb.neg ? -1 : 1);
    if (sa != sb)
        return sa < sb ? -1 : 1;
    if (sa == 0)
        return 0;
    int pa = cwSuffixOrder(a, ae);
    int pb = cwSuffixOrder(b, be);
    if (pa != pb)
        return (pa < pb) == (sa > 0) ? -1 : 1;
    int m = cwMagCompare(va, vb);
    return va.neg ? -m : m;
}

static int cwMonthValue(const char* s, const char* e) {
    static const char* mon[12] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                                   "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };
    while (s < e && cwIsBlank((unsigned char)*s))
        ++s;
    if (e - s < 3)
        return 0;
    char u[3];
    for (int i = 0; i < 3; ++i)
        u[i] = cwUpper(s[i]);
    for (int m = 0; m < 12; ++m) {
        if (u[0] == mon[m][0] && u[1] == mon[m][1] && u[2] == mon[m][2])
            return m + 1;
    }
    return 0;
}

static int cwMonthCompare(const char* a, const char* ae, const char* b, const char* be) {
    int ma = cwMonthValue(a, ae);
    int mb = cwMonthValue(b, be);
    if (ma != mb)
        return ma < mb ? -1 : 1;
    return 0;
}

static int cwVerOrder(const char* s, size_t pos, size_t len) {
    if (pos >= len)
        return 0;
    unsigned char c = (unsigned char)s[pos];
    if (cwIsDigit(c))
        return 0;
    if (cwIsAlpha(c))
        return (int)c;
    if (c == '~')
        return -1;
    return (int)c + UCHAR_MAX + 1;
}

static int cwVerRevCmp(const char* a, size_t an, const char* b, size_t bn) {
    size_t i = 0;
    size_t j = 0;
    while (i < an || j < bn) {
        int first = 0;
        while ((i < an && !cwIsDigit((unsigned char)a[i]))
            || (j < bn && !cwIsDigit((unsigned char)b[j]))) {
            int ca = cwVerOrder(a, i, an);
            int cb = cwVerOrder(b, j, bn);
            if (ca != cb)
                return ca - cb;
            ++i;
            ++j;
        }
        while (i < an && a[i] == '0')
            ++i;
        while (j < bn && b[j] == '0')
            ++j;
        while (i < an && j < bn && cwIsDigit((unsigned char)a[i]) && cwIsDigit((unsigned char)b[j])) {
            if (first == 0)
                first = (int)(unsigned char)a[i] - (int)(unsigned char)b[j];
            ++i;
            ++j;
        }
        if (i < an && cwIsDigit((unsigned char)a[i]))
            return 1;
        if (j < bn && cwIsDigit((unsigned char)b[j]))
            return -1;
        if (first != 0)
            return first;
    }
    return 0;
}

static size_t cwSuffixPos(const char* s, size_t n) {
    size_t match      = n;
    bool   read_alpha = false;
    for (size_t i = 0; i < n; ++i) {
        unsigned char c = (unsigned char)s[i];
        if (read_alpha) {
            read_alpha = false;
            if (!cwIsAlpha(c) && c != '~')
                match = n;
        } else if (c == '.') {
            read_alpha = true;
            if (match == n)
                match = i;
        } else if (!cwIsAlnum(c) && c != '~') {
            match = n;
        }
    }
    return match;
}

static int cwVerCompare(const char* a, const char* ae, const char* b, const char* be) {
    size_t an = (size_t)(ae - a);
    size_t bn = (size_t)(be - b);
    int    simple = cwPlainCompare(a, an, b, bn);
    if (simple == 0)
        return 0;
    if (an == 0)
        return -1;
    if (bn == 0)
        return 1;
    if (an == 1 && a[0] == '.')
        return -1;
    if (bn == 1 && b[0] == '.')
        return 1;
    if (an == 2 && a[0] == '.' && a[1] == '.')
        return -1;
    if (bn == 2 && b[0] == '.' && b[1] == '.')
        return 1;
    if (a[0] == '.' && b[0] != '.')
        return -1;
    if (a[0] != '.' && b[0] == '.')
        return 1;
    if (a[0] == '.' && b[0] == '.') {
        ++a;  --an;
        ++b;  --bn;
    }
    size_t alen = cwSuffixPos(a, an);
    size_t blen = cwSuffixPos(b, bn);
    if ((alen != an || blen != bn) && alen == blen && cwPlainCompare(a, alen, b, blen) == 0) {
        alen = an;
        blen = bn;
    }
    int r = cwVerRevCmp(a, alen, b, blen);
    return r != 0 ? r : simple;
}

static int cwKeyCompare(const Key& k, const char* ab, const char* ae,
                        const char* bb, const char* be) {
    unsigned f = k.flags;
    int      r;
    if ((f & CwNum) != 0)
        r = cwNumCompare(ab, ae, bb, be);
    else if ((f & CwGen) != 0)
        r = cwGenCompare(ab, ae, bb, be);
    else if ((f & CwHuman) != 0)
        r = cwHumanCompare(ab, ae, bb, be);
    else if ((f & CwMonth) != 0)
        r = cwMonthCompare(ab, ae, bb, be);
    else if ((f & CwVer) != 0)
        r = cwVerCompare(ab, ae, bb, be);
    else
        r = cwTextCompare(ab, (size_t)(ae - ab), bb, (size_t)(be - bb), f);
    return ((f & CwRev) != 0) ? -r : r;
}

static int cwCompare(const Cfg& cfg, const Rec& a, const Rec& b) {
    for (size_t i = 0; i < cfg.keys.size(); ++i) {
        const char* ab;
        const char* ae;
        const char* bb;
        const char* be;
        cwKeyRange(cfg, cfg.keys[i], a, &ab, &ae);
        cwKeyRange(cfg, cfg.keys[i], b, &bb, &be);
        int d = cwKeyCompare(cfg.keys[i], ab, ae, bb, be);
        if (d != 0)
            return d;
    }
    if (!cfg.keys.empty() && (cfg.unique || cfg.stable))
        return 0;
    int d = cwPlainCompare(a.p, a.n, b.p, b.n);
    return cfg.reverse ? -d : d;
}

struct RecLess {
    const Cfg* cfg;
    RecLess() : cfg(0) {}
    bool operator()(const Rec& a, const Rec& b) const {
        return cwCompare(*cfg, a, b) < 0;
    }
};

class App {
public:
    enum { ExOk = 0, ExDisorder = 1, ExFail = 2 };

    App()
        : check_(false)
        , check_quiet_(false)
        , delim_('\n')
        , out_name_(0)
        , status_(ExOk)
    {}

    int main(int argc, cw_char* argv[]);

private:
    bool parseArgs(int argc, cw_char* argv[]);
    bool parseLong(const std::string& arg, int& i, int argc, cw_char* argv[]);
    bool parseShort(const std::string& arg, int& i, int argc, cw_char* argv[]);
    bool parseKey(const std::string& spec);
    const char* setOrdering(const char* s, Key& k, int where);
    bool tabValue(const std::string& s, char& out);
    static int usage(int rc);
    static void version();

    bool readAll();
    bool readFile(const cw_char* path, const std::string& disp);
    void indexLines();
    void inherit();
    int  runCheck();
    void output();

private:
    Cfg                         cfg_;
    Key                         gkey_;
    bool                        check_;
    bool                        check_quiet_;
    char                        delim_;
    const cw_char*              out_name_;
    int                         status_;

    std::string                 buf_;
    std::vector<Rec>            recs_;
    std::vector<const cw_char*> files_;
    std::vector<std::string>    names_;
};

int App::usage(int rc) {
    FILE* fp = (rc == ExOk) ? stdout : stderr;
    fputs(
        "Usage: sort [OPTION]... [FILE]...\n"
        "Write sorted concatenation of all FILE(s) to standard output.\n"
        "With no FILE, or when FILE is -, read standard input.\n"
        "\n"
        "Ordering options:\n"
        "  -b, --ignore-leading-blanks  ignore leading blanks\n"
        "  -d, --dictionary-order   consider only blanks and alphanumerics\n"
        "  -f, --ignore-case        fold lower case to upper case characters\n"
        "  -g, --general-numeric-sort  compare according to general numerical value\n"
        "  -h, --human-numeric-sort  compare human readable numbers (e.g., 2K 1G)\n"
        "  -i, --ignore-nonprinting  consider only printable characters\n"
        "  -M, --month-sort         compare (unknown) < 'JAN' < ... < 'DEC'\n"
        "  -n, --numeric-sort       compare according to string numerical value\n"
        "  -r, --reverse            reverse the result of comparisons\n"
        "  -V, --version-sort       natural sort of (version) numbers within text\n"
        "\n"
        "Other options:\n"
        "  -c, --check              check for sorted input; do not sort\n"
        "  -C, --check=quiet        like -c, but do not report the first bad line\n"
        "  -k, --key=KEYDEF         sort via a key; KEYDEF gives location and type\n"
        "  -m, --merge              merge already sorted files; do not sort\n"
        "  -o, --output=FILE        write result to FILE instead of standard output\n"
        "  -s, --stable             stabilize sort by disabling last-resort comparison\n"
        "  -t, --field-separator=SEP  use SEP instead of non-blank to blank transition\n"
        "  -u, --unique             output only the first of an equal run\n"
        "  -z, --zero-terminated    line delimiter is NUL, not newline\n"
        "      --help               display this help and exit\n"
        "      --version            output version information and exit\n"
        "\n"
        "KEYDEF is F[.C][OPTS][,F[.C][OPTS]] for start and stop position, where F is\n"
        "a field number and C a character position in the field; both are origin 1.\n"
        "OPTS is one or more single-letter ordering options [bdfghinrV].\n",
        fp);
    return rc;
}

void App::version() {
    fputs("sort (ccwrap samples) 1.0\n"
          "License: Boost Software License Version 1.0\n", stdout);
}

const char* App::setOrdering(const char* s, Key& k, int where) {
    while (*s != '\0') {
        switch (*s) {
        case 'b':
            if (where <= 0)
                k.flags |= CwSkipS;
            if (where >= 0)
                k.flags |= CwSkipE;
            break;
        case 'd': k.flags |= CwDict;   break;
        case 'f': k.flags |= CwFold;   break;
        case 'g': k.flags |= CwGen;    break;
        case 'h': k.flags |= CwHuman;  break;
        case 'i': k.flags |= CwPrint;  break;
        case 'M': k.flags |= CwMonth;  break;
        case 'n': k.flags |= CwNum;    break;
        case 'r': k.flags |= CwRev;    break;
        case 'V': k.flags |= CwVer;    break;
        default:  return s;
        }
        ++s;
    }
    return s;
}

bool App::parseKey(const std::string& spec) {
    Key         k;
    const char* s = spec.c_str();
    char*       e = 0;
    long        v;

    v = strtol(s, &e, 10);
    if (e == s || v < 1) {
        fprintf(stderr, "sort: invalid field specification '%s'\n", spec.c_str());
        status_ = ExFail;
        return false;
    }
    k.sword       = (size_t)(v - 1);
    k.sword_given = true;
    s = e;
    if (*s == '.') {
        v = strtol(s + 1, &e, 10);
        if (e == s + 1 || v < 1) {
            fprintf(stderr, "sort: invalid field specification '%s'\n", spec.c_str());
            status_ = ExFail;
            return false;
        }
        k.schar = (size_t)(v - 1);
        s = e;
    }
    s = setOrdering(s, k, -1);
    if (*s == ',') {
        v = strtol(s + 1, &e, 10);
        if (e == s + 1 || v < 1) {
            fprintf(stderr, "sort: invalid field specification '%s'\n", spec.c_str());
            status_ = ExFail;
            return false;
        }
        k.eword       = (size_t)(v - 1);
        k.eword_given = true;
        s = e;
        if (*s == '.') {
            v = strtol(s + 1, &e, 10);
            if (e == s + 1 || v < 0) {
                fprintf(stderr, "sort: invalid field specification '%s'\n", spec.c_str());
                status_ = ExFail;
                return false;
            }
            k.echar = (size_t)v;
            s = e;
        }
        s = setOrdering(s, k, 1);
    }
    if (*s != '\0') {
        fprintf(stderr, "sort: invalid field specification '%s'\n", spec.c_str());
        status_ = ExFail;
        return false;
    }
    cfg_.keys.push_back(k);
    return true;
}

bool App::tabValue(const std::string& s, char& out) {
    if (s.size() == 1) {
        out = s[0];
        return true;
    }
    if (s == "\\0") {
        out = '\0';
        return true;
    }
    if (s == "\\t") {
        out = '\t';
        return true;
    }
    if (s == "\\n") {
        out = '\n';
        return true;
    }
    fprintf(stderr, "sort: multi-character tab '%s'\n", s.c_str());
    status_ = ExFail;
    return false;
}

bool App::parseLong(const std::string& arg, int& i, int argc, cw_char* argv[]) {
    std::string name(arg, 2, std::string::npos);
    std::string val;
    bool        has_val = false;
    size_t      eq      = name.find('=');
    if (eq != std::string::npos) {
        val.assign(name, eq + 1, std::string::npos);
        name.erase(eq);
        has_val = true;
    }

    if (name == "ignore-leading-blanks")   { gkey_.flags |= CwSkipS | CwSkipE; }
    else if (name == "dictionary-order")   { gkey_.flags |= CwDict; }
    else if (name == "ignore-case")        { gkey_.flags |= CwFold; }
    else if (name == "general-numeric-sort") { gkey_.flags |= CwGen; }
    else if (name == "human-numeric-sort") { gkey_.flags |= CwHuman; }
    else if (name == "ignore-nonprinting") { gkey_.flags |= CwPrint; }
    else if (name == "month-sort")         { gkey_.flags |= CwMonth; }
    else if (name == "numeric-sort")       { gkey_.flags |= CwNum; }
    else if (name == "version-sort")       { gkey_.flags |= CwVer; }
    else if (name == "reverse")            { gkey_.flags |= CwRev; cfg_.reverse = true; }
    else if (name == "stable")             { cfg_.stable = true; }
    else if (name == "unique")             { cfg_.unique = true; }
    else if (name == "merge")              { }
    else if (name == "zero-terminated")    { delim_ = '\0'; }
    else if (name == "help")               { usage(ExOk); return false; }
    else if (name == "version")            { version(); return false; }
    else if (name == "check") {
        check_ = true;
        if (has_val && (val == "quiet" || val == "silent"))
            check_quiet_ = true;
    } else if (name == "parallel" || name == "buffer-size" || name == "temporary-directory"
            || name == "compress-program" || name == "batch-size") {
        if (!has_val && i + 1 < argc)
            ++i;
    } else if (name == "key" || name == "output" || name == "field-separator") {
        const cw_char* raw = argv[i] + (arg.size() - val.size());
        if (!has_val) {
            if (i + 1 >= argc) {
                fprintf(stderr, "sort: option '%s' requires an argument\n", arg.c_str());
                status_ = ExFail;
                return false;
            }
            ++i;
            raw = argv[i];
            cwToUtf8(argv[i], val);
        }
        if (name == "key") {
            if (!parseKey(val))
                return false;
        } else if (name == "output") {
            out_name_ = raw;
        } else {
            char t = '\0';
            if (!tabValue(val, t))
                return false;
            cfg_.tab       = t;
            cfg_.tab_given = true;
        }
    } else {
        fprintf(stderr, "sort: unrecognized option '%s'\n", arg.c_str());
        usage(ExFail);
        status_ = ExFail;
        return false;
    }
    return true;
}

bool App::parseShort(const std::string& arg, int& i, int argc, cw_char* argv[]) {
    for (size_t k = 1; k < arg.size(); ++k) {
        char c = arg[k];
        switch (c) {
        case 'b': gkey_.flags |= CwSkipS | CwSkipE;  break;
        case 'd': gkey_.flags |= CwDict;             break;
        case 'f': gkey_.flags |= CwFold;             break;
        case 'g': gkey_.flags |= CwGen;              break;
        case 'h': gkey_.flags |= CwHuman;            break;
        case 'i': gkey_.flags |= CwPrint;            break;
        case 'M': gkey_.flags |= CwMonth;            break;
        case 'n': gkey_.flags |= CwNum;              break;
        case 'V': gkey_.flags |= CwVer;              break;
        case 'r': gkey_.flags |= CwRev; cfg_.reverse = true;  break;
        case 's': cfg_.stable = true;                break;
        case 'u': cfg_.unique = true;                break;
        case 'z': delim_ = '\0';                     break;
        case 'm':                                    break;
        case 'c': check_ = true;                     break;
        case 'C': check_ = true; check_quiet_ = true;  break;
        case 'k': case 'o': case 't': case 'S': case 'T': {
            std::string    rest(arg, k + 1, std::string::npos);
            const cw_char* raw = argv[i] + k + 1;
            if (rest.empty()) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "sort: option requires an argument -- '%c'\n", c);
                    status_ = ExFail;
                    return false;
                }
                ++i;
                raw = argv[i];
                cwToUtf8(argv[i], rest);
            }
            if (c == 'k') {
                if (!parseKey(rest))
                    return false;
            } else if (c == 'o') {
                out_name_ = raw;
            } else if (c == 't') {
                char t = '\0';
                if (!tabValue(rest, t))
                    return false;
                cfg_.tab       = t;
                cfg_.tab_given = true;
            }
            return true;
        }
        default:
            fprintf(stderr, "sort: invalid option -- '%c'\n", c);
            usage(ExFail);
            status_ = ExFail;
            return false;
        }
    }
    return true;
}

bool App::parseArgs(int argc, cw_char* argv[]) {
    bool        no_more = false;
    std::string arg;
    for (int i = 1; i < argc; ++i) {
        cwToUtf8(argv[i], arg);
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
        files_.push_back(argv[i]);
        names_.push_back(arg);
    }
    return true;
}

void App::inherit() {
    if (cfg_.keys.empty()) {
        if ((gkey_.flags & (CwOrderMask | CwRev)) != 0) {
            Key k = gkey_;
            k.sword_given = false;
            k.eword_given = false;
            cfg_.keys.push_back(k);
        }
        return;
    }
    for (size_t i = 0; i < cfg_.keys.size(); ++i) {
        Key& k = cfg_.keys[i];
        if ((k.flags & CwOrderMask) == 0 && (k.flags & CwRev) == 0)
            k.flags |= gkey_.flags;
    }
}

bool App::readFile(const cw_char* path, const std::string& disp) {
    FILE* fp;
    bool  is_stdin = (path == 0 || disp == "-");
    if (is_stdin) {
        fp = stdin;
        cwBinaryMode(fp);
    } else {
        fp = cwFopen(path, false);
        if (fp == 0) {
            fprintf(stderr, "sort: cannot read: %s: %s\n", disp.c_str(), strerror(errno));
            status_ = ExFail;
            return false;
        }
    }
    char   tmp[65536];
    size_t n;
    size_t start = buf_.size();
    while ((n = fread(tmp, 1, sizeof(tmp), fp)) > 0)
        buf_.append(tmp, n);
    if (buf_.size() > start && buf_[buf_.size() - 1] != delim_)
        buf_ += delim_;
    if (!is_stdin)
        fclose(fp);
    return true;
}

bool App::readAll() {
    if (files_.empty())
        return readFile(0, "-");
    bool ok = true;
    for (size_t i = 0; i < files_.size(); ++i) {
        if (!readFile(files_[i], names_[i]))
            ok = false;
    }
    return ok;
}

void App::indexLines() {
    const char* p = buf_.c_str();
    size_t      n = buf_.size();
    size_t      beg = 0;
    Rec         r;
    for (size_t i = 0; i < n; ++i) {
        if (p[i] != delim_)
            continue;
        r.p = p + beg;
        r.n = i - beg;
        recs_.push_back(r);
        beg = i + 1;
    }
    if (beg < n) {
        r.p = p + beg;
        r.n = n - beg;
        recs_.push_back(r);
    }
}

int App::runCheck() {
    for (size_t i = 1; i < recs_.size(); ++i) {
        int d = cwCompare(cfg_, recs_[i - 1], recs_[i]);
        bool bad = cfg_.unique ? (d >= 0) : (d > 0);
        if (!bad)
            continue;
        if (!check_quiet_) {
            std::string name = files_.empty() ? std::string("-") : names_[0];
            fprintf(stderr, "sort: %s:%lu: disorder: ", name.c_str(), (unsigned long)(i + 1));
            fwrite(recs_[i].p, 1, recs_[i].n, stderr);
            putc('\n', stderr);
        }
        return ExDisorder;
    }
    return ExOk;
}

void App::output() {
    FILE* fp = stdout;
    if (out_name_ != 0) {
        fp = cwFopen(out_name_, true);
        if (fp == 0) {
            fprintf(stderr, "sort: open failed: %s\n", strerror(errno));
            status_ = ExFail;
            return;
        }
    } else {
        cwBinaryMode(stdout);
    }
    for (size_t i = 0; i < recs_.size(); ++i) {
        if (cfg_.unique && i > 0 && cwCompare(cfg_, recs_[i - 1], recs_[i]) == 0)
            continue;
        if (recs_[i].n > 0)
            fwrite(recs_[i].p, 1, recs_[i].n, fp);
        putc(delim_, fp);
    }
    if (fp != stdout)
        fclose(fp);
}

int App::main(int argc, cw_char* argv[]) {
    setlocale(LC_ALL, "");

    if (!parseArgs(argc, argv))
        return status_;

    inherit();

    if (!readAll())
        return status_;
    indexLines();

    if (check_) {
        int r = runCheck();
        return (status_ != ExOk) ? status_ : r;
    }

    RecLess less;
    less.cfg = &cfg_;
    std::stable_sort(recs_.begin(), recs_.end(), less);

    output();
    return status_;
}

#if !defined(_WIN32)
int main(int argc, char* argv[]) {
    return App().main(argc, argv);
}
#else
int wmain(int argc, wchar_t* argv[]) {
    ::SetConsoleOutputCP(65001);
    return App().main(argc, argv);
}
#endif
