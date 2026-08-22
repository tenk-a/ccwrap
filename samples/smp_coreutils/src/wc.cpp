/**
 *  @file   wc.cpp
 *  @brief  line, word, character and byte counter.
 */
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32)
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#undef min
#undef max
#else
#include <unistd.h>
#endif

typedef unsigned long long  cw_u64;

#if defined(_WIN32)
typedef wchar_t             cw_char;
 #if defined(__WATCOMC__)
typedef struct _stati64     cw_stat_t;
  #define CW_STAT_W(p, b)   ::_wstati64((p), (b))
  #define CW_FSTAT(f, b)    ::_fstati64((f), (b))
 #else
typedef struct _stat64      cw_stat_t;
  #define CW_STAT_W(p, b)   ::_wstat64((p), (b))
  #define CW_FSTAT(f, b)    ::_fstat64((f), (b))
 #endif
#else
typedef char                cw_char;
typedef struct stat         cw_stat_t;
#endif

#if !defined(S_IFMT)
 #define S_IFMT     0xF000
#endif
#if !defined(S_ISREG)
 #define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#endif

enum CwTotal { CwTotalAuto, CwTotalAlways, CwTotalOnly, CwTotalNever };

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
static FILE* cwFopenRead(const wchar_t* p) { return ::_wfopen(p, L"rb"); }
static int   cwStatOf(const wchar_t* p, cw_stat_t* b) { return CW_STAT_W(p, b); }
#else
static FILE* cwFopenRead(const char* p) { return fopen(p, "rb"); }
static int   cwStatOf(const char* p, cw_stat_t* b) { return ::stat(p, b); }
#endif

static void cwBinaryMode(FILE* fp) {
#if defined(_WIN32)
    ::_setmode(::_fileno(fp), _O_BINARY);
#else
    (void)fp;
#endif
}

static bool cwIsSpace(unsigned char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

static bool cwIsWideCp(unsigned long c) {
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

static void cwU64Str(char* buf, size_t bufsz, cw_u64 v) {
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

static size_t cwDigits(cw_u64 v) {
    size_t n = 1;
    while (v >= 10) {
        v /= 10;
        ++n;
    }
    return n;
}

struct Counts {
    cw_u64 lines;
    cw_u64 words;
    cw_u64 chars;
    cw_u64 bytes;
    cw_u64 maxlen;

    Counts() : lines(0), words(0), chars(0), bytes(0), maxlen(0) {}

    void add(const Counts& o) {
        lines += o.lines;
        words += o.words;
        chars += o.chars;
        bytes += o.bytes;
        if (o.maxlen > maxlen)
            maxlen = o.maxlen;
    }
};

class App {
public:
    enum { ExOk = 0, ExFail = 1 };

    App()
        : lines_(false)
        , words_(false)
        , chars_(false)
        , bytes_(false)
        , maxlen_(false)
        , total_(CwTotalAuto)
        , width_(1)
        , nfiles_(0)
        , status_(ExOk)
    {}

    int main(int argc, cw_char* argv[]);

private:
    bool parseArgs(int argc, cw_char* argv[]);
    bool parseLong(const std::string& arg, int& i, int argc, cw_char* argv[]);
    bool parseShort(const std::string& arg);
    bool filesFrom(const std::string& name);
    static int usage(int rc);
    static void version();

    bool countFile(const cw_char* path, const std::string& disp, Counts& c);
    void countStream(FILE* fp, Counts& c);
    void report(const Counts& c, const std::string& name);
    void computeWidth();

private:
    bool        lines_;
    bool        words_;
    bool        chars_;
    bool        bytes_;
    bool        maxlen_;
    int         total_;
    size_t      width_;
    size_t      nfiles_;
    int         status_;

    std::vector<const cw_char*> files_;
    std::vector<std::string>    names_;
    std::vector<std::string>    from_buf_;
    std::vector<std::wstring>   from_wide_;
    std::string                 tmp_;
};

int App::usage(int rc) {
    FILE* fp = (rc == ExOk) ? stdout : stderr;
    fputs(
        "Usage: wc [OPTION]... [FILE]...\n"
        "Print newline, word, and byte counts for each FILE, and a total line if\n"
        "more than one FILE is specified.  With no FILE, or when FILE is -, read\n"
        "standard input.\n"
        "\n"
        "  -c, --bytes            print the byte counts\n"
        "  -m, --chars            print the character counts\n"
        "  -l, --lines            print the newline counts\n"
        "      --files0-from=F    read input from the files specified by\n"
        "                           NUL-terminated names in file F\n"
        "  -L, --max-line-length  print the maximum display width\n"
        "  -w, --words            print the word counts\n"
        "      --total=WHEN       when to print a line with total counts;\n"
        "                           WHEN is auto, always, only or never\n"
        "      --help             display this help and exit\n"
        "      --version          output version information and exit\n",
        fp);
    return rc;
}

void App::version() {
    fputs("wc (ccwrap samples) 1.0\n"
          "License: Boost Software License Version 1.0\n", stdout);
}

bool App::parseShort(const std::string& arg) {
    for (size_t k = 1; k < arg.size(); ++k) {
        char c = arg[k];
        switch (c) {
        case 'c': bytes_ = true;   break;
        case 'm': chars_ = true;   break;
        case 'l': lines_ = true;   break;
        case 'w': words_ = true;   break;
        case 'L': maxlen_ = true;  break;
        default:
            fprintf(stderr, "wc: invalid option -- '%c'\n", c);
            usage(ExFail);
            status_ = ExFail;
            return false;
        }
    }
    return true;
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

    if (name == "bytes")            { bytes_ = true; }
    else if (name == "chars")       { chars_ = true; }
    else if (name == "lines")       { lines_ = true; }
    else if (name == "words")       { words_ = true; }
    else if (name == "max-line-length") { maxlen_ = true; }
    else if (name == "help")        { usage(ExOk); return false; }
    else if (name == "version")     { version(); return false; }
    else if (name == "files0-from") {
        if (!has_val) {
            if (i + 1 >= argc) {
                fprintf(stderr, "wc: option '--files0-from' requires an argument\n");
                status_ = ExFail;
                return false;
            }
            cwToUtf8(argv[++i], val);
        }
        if (!filesFrom(val))
            return false;
    } else if (name == "total") {
        if (!has_val) {
            if (i + 1 >= argc) {
                fprintf(stderr, "wc: option '--total' requires an argument\n");
                status_ = ExFail;
                return false;
            }
            cwToUtf8(argv[++i], val);
        }
        if (val == "auto")        total_ = CwTotalAuto;
        else if (val == "always") total_ = CwTotalAlways;
        else if (val == "only")   total_ = CwTotalOnly;
        else if (val == "never")  total_ = CwTotalNever;
        else {
            fprintf(stderr, "wc: invalid argument '%s' for '--total'\n", val.c_str());
            status_ = ExFail;
            return false;
        }
    } else {
        fprintf(stderr, "wc: unrecognized option '%s'\n", arg.c_str());
        usage(ExFail);
        status_ = ExFail;
        return false;
    }
    return true;
}

bool App::filesFrom(const std::string& name) {
    FILE* fp = 0;
    if (name == "-") {
        fp = stdin;
        cwBinaryMode(fp);
    } else {
        fp = fopen(name.c_str(), "rb");
    }
    if (fp == 0) {
        fprintf(stderr, "wc: cannot open '%s' for reading: %s\n", name.c_str(), strerror(errno));
        status_ = ExFail;
        return false;
    }
    std::string cur;
    int         ch;
    for (;;) {
        ch = getc(fp);
        if (ch == EOF) {
            if (!cur.empty())
                from_buf_.push_back(cur);
            break;
        }
        if (ch == 0) {
            from_buf_.push_back(cur);
            cur.clear();
            continue;
        }
        cur += (char)ch;
    }
    if (fp != stdin)
        fclose(fp);
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
                if (!parseShort(arg))
                    return false;
            }
            continue;
        }
        files_.push_back(argv[i]);
        names_.push_back(arg);
    }
    return true;
}

void App::countStream(FILE* fp, Counts& c) {
    static char buf[65536];
    size_t      n;
    bool        in_word = false;
    cw_u64      linepos = 0;
    unsigned long cp    = 0;
    int         pend    = 0;

    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
        c.bytes += (cw_u64)n;
        for (size_t i = 0; i < n; ++i) {
            unsigned char ch = (unsigned char)buf[i];
            if (ch == '\n')
                ++c.lines;
            if (cwIsSpace(ch)) {
                in_word = false;
            } else if (!in_word) {
                in_word = true;
                ++c.words;
            }
            if ((ch & 0xC0) != 0x80)
                ++c.chars;
            if (!maxlen_)
                continue;
            if (ch == '\n' || ch == '\r' || ch == '\f') {
                if (linepos > c.maxlen)
                    c.maxlen = linepos;
                linepos = 0;
                pend    = 0;
            } else if (ch == '\t') {
                linepos += 8 - (linepos % 8);
                pend     = 0;
            } else if (ch < 0x80) {
                ++linepos;
                pend = 0;
            } else if ((ch & 0xC0) == 0x80) {
                if (pend > 0) {
                    cp = (cp << 6) | (unsigned long)(ch & 0x3F);
                    if (--pend == 0)
                        linepos += cwIsWideCp(cp) ? 2 : 1;
                }
            } else {
                if ((ch & 0xE0) == 0xC0)      { cp = ch & 0x1F;  pend = 1; }
                else if ((ch & 0xF0) == 0xE0) { cp = ch & 0x0F;  pend = 2; }
                else if ((ch & 0xF8) == 0xF0) { cp = ch & 0x07;  pend = 3; }
                else                          { ++linepos;       pend = 0; }
            }
        }
    }
    if (maxlen_ && linepos > c.maxlen)
        c.maxlen = linepos;
}

bool App::countFile(const cw_char* path, const std::string& disp, Counts& c) {
    FILE* fp;
    bool  is_stdin = (disp == "-");
    if (is_stdin) {
        fp = stdin;
        cwBinaryMode(fp);
    } else {
        fp = cwFopenRead(path);
        if (fp == 0) {
            fprintf(stderr, "wc: %s: %s\n", disp.c_str(), strerror(errno));
            status_ = ExFail;
            return false;
        }
    }
    countStream(fp, c);
    if (!is_stdin)
        fclose(fp);
    return true;
}

void App::computeWidth() {
    cw_u64 total  = 0;
    bool   known  = true;
    size_t n      = files_.empty() ? 1u : files_.size();
    size_t ncount = (lines_ ? 1u : 0u) + (words_ ? 1u : 0u) + (chars_ ? 1u : 0u)
                  + (bytes_ ? 1u : 0u) + (maxlen_ ? 1u : 0u);

    for (size_t i = 0; i < n; ++i) {
        cw_stat_t st;
        memset(&st, 0, sizeof(st));
        if (files_.empty() || names_[i] == "-") {
#if defined(_WIN32)
            if (CW_FSTAT(::fileno(stdin), &st) != 0 || !S_ISREG((unsigned)st.st_mode))
                known = false;
            else
                total += (cw_u64)st.st_size;
#else
            if (::fstat(::fileno(stdin), &st) != 0 || !S_ISREG((unsigned)st.st_mode))
                known = false;
            else
                total += (cw_u64)st.st_size;
#endif
            continue;
        }
        if (cwStatOf(files_[i], &st) != 0)
            continue;
        if (!S_ISREG((unsigned)st.st_mode))
            known = false;
        else
            total += (cw_u64)st.st_size;
    }

    if (files_.size() <= 1 && total_ != CwTotalAlways && total_ != CwTotalOnly && ncount <= 1) {
        width_ = 1;
        return;
    }
    width_ = known ? cwDigits(total) : 7;
    if (width_ < 1)
        width_ = 1;
}

void App::report(const Counts& c, const std::string& name) {
    char   b[32];
    bool   first = true;
    size_t i;
    for (i = 0; i < 5; ++i) {
        cw_u64 v;
        if (i == 0) { if (!lines_)  continue; v = c.lines; }
        else if (i == 1) { if (!words_)  continue; v = c.words; }
        else if (i == 2) { if (!chars_)  continue; v = c.chars; }
        else if (i == 3) { if (!bytes_)  continue; v = c.bytes; }
        else             { if (!maxlen_) continue; v = c.maxlen; }
        cwU64Str(b, sizeof(b), v);
        if (!first)
            putchar(' ');
        first = false;
        printf("%*s", (int)width_, b);
    }
    if (!name.empty()) {
        putchar(' ');
        fputs(name.c_str(), stdout);
    }
    putchar('\n');
}

int App::main(int argc, cw_char* argv[]) {
    setlocale(LC_ALL, "");

    if (!parseArgs(argc, argv))
        return status_;

    if (!from_buf_.empty()) {
        for (size_t i = 0; i < from_buf_.size(); ++i) {
            names_.push_back(from_buf_[i]);
#if defined(_WIN32)
            std::wstring w;
            int n = ::MultiByteToWideChar(CP_UTF8, 0, from_buf_[i].c_str(), -1, 0, 0);
            if (n > 1) {
                w.resize((size_t)(n - 1));
                ::MultiByteToWideChar(CP_UTF8, 0, from_buf_[i].c_str(), -1, &w[0], n);
            }
            from_wide_.push_back(w);
#endif
        }
#if defined(_WIN32)
        for (size_t i = 0; i < from_wide_.size(); ++i)
            files_.push_back(from_wide_[i].c_str());
#else
        for (size_t i = 0; i < from_buf_.size(); ++i)
            files_.push_back(from_buf_[i].c_str());
#endif
    }

    if (!lines_ && !words_ && !chars_ && !bytes_ && !maxlen_) {
        lines_ = true;
        words_ = true;
        bytes_ = true;
    }
    cwBinaryMode(stdout);

    computeWidth();

    Counts total;
    if (files_.empty()) {
        Counts c;
        countFile(0, "-", c);
        if (total_ != CwTotalOnly)
            report(c, "");
        total.add(c);
        nfiles_ = 1;
    } else {
        for (size_t i = 0; i < files_.size(); ++i) {
            Counts c;
            if (!countFile(files_[i], names_[i], c))
                continue;
            if (total_ != CwTotalOnly)
                report(c, names_[i]);
            total.add(c);
            ++nfiles_;
        }
    }

    bool show_total = (total_ == CwTotalAlways || total_ == CwTotalOnly
                    || (total_ == CwTotalAuto && files_.size() > 1));
    if (show_total)
        report(total, total_ == CwTotalOnly ? std::string() : std::string("total"));

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
