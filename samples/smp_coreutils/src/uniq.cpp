/**
 *  @file   uniq.cpp
 *  @brief  filter for adjacent matching lines.
 */
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <errno.h>

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

enum CwGroup { CwGroupNone, CwGroupSeparate, CwGroupPrepend, CwGroupAppend, CwGroupBoth };
enum CwDelim { CwDelimNone, CwDelimPrepend, CwDelimSeparate };

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

static char cwLower(char c) {
    if (c >= 'A' && c <= 'Z')
        return (char)(c - 'A' + 'a');
    return c;
}

static bool cwReadLine(FILE* fp, std::string& out, char delim) {
    int ch;
    out.clear();
    for (;;) {
        ch = getc(fp);
        if (ch == EOF)
            return !out.empty();
        out += (char)ch;
        if ((char)ch == delim)
            return true;
    }
}

class App {
public:
    enum { ExOk = 0, ExFail = 1 };

    App()
        : count_(false)
        , unique_(true)
        , repeated_(true)
        , all_repeated_(false)
        , ignore_case_(false)
        , skip_fields_(0)
        , skip_chars_(0)
        , check_chars_((size_t)-1)
        , delim_('\n')
        , group_(CwGroupNone)
        , gdelim_(CwDelimSeparate)
        , in_(0)
        , out_(0)
        , status_(ExOk)
    {}

    int main(int argc, cw_char* argv[]);

private:
    bool parseArgs(int argc, cw_char* argv[]);
    bool parseLong(const std::string& arg, int& i, int argc, cw_char* argv[]);
    bool parseShort(const std::string& arg, int& i, int argc, cw_char* argv[]);
    bool number(const std::string& s, size_t& out) const;
    static int usage(int rc);
    static void version();

    void run();
    void runGroup();
    size_t fieldPos(const std::string& line) const;
    bool different(const std::string& a, size_t apos, const std::string& b, size_t bpos) const;
    void writeLine(const std::string& line, cw_u64 count);
    void writeRaw(const std::string& line);

private:
    bool        count_;
    bool        unique_;
    bool        repeated_;
    bool        all_repeated_;
    bool        ignore_case_;
    size_t      skip_fields_;
    size_t      skip_chars_;
    size_t      check_chars_;
    char        delim_;
    int         group_;
    int         gdelim_;
    FILE*       in_;
    FILE*       out_;
    int         status_;

    std::vector<const cw_char*> files_;
    std::vector<std::string>    names_;
    std::string                 tmp_;
};

int App::usage(int rc) {
    FILE* fp = (rc == ExOk) ? stdout : stderr;
    fputs(
        "Usage: uniq [OPTION]... [INPUT [OUTPUT]]\n"
        "Filter adjacent matching lines from INPUT (or standard input),\n"
        "writing to OUTPUT (or standard output).\n"
        "\n"
        "  -c, --count           prefix lines by the number of occurrences\n"
        "  -d, --repeated        only print duplicate lines, one for each group\n"
        "  -D                    print all duplicate lines\n"
        "      --all-repeated[=METHOD]  like -D, but allow separating groups\n"
        "                          METHOD is none, prepend or separate\n"
        "  -f, --skip-fields=N   avoid comparing the first N fields\n"
        "      --group[=METHOD]  show all items, separating groups\n"
        "                          METHOD is separate, prepend, append or both\n"
        "  -i, --ignore-case     ignore differences in case when comparing\n"
        "  -s, --skip-chars=N    avoid comparing the first N characters\n"
        "  -u, --unique          only print unique lines\n"
        "  -z, --zero-terminated  line delimiter is NUL, not newline\n"
        "  -w, --check-chars=N   compare no more than N characters in lines\n"
        "      --help            display this help and exit\n"
        "      --version         output version information and exit\n"
        "\n"
        "A field is a run of blanks (usually spaces and/or TABs), then non-blank\n"
        "characters.  Fields are skipped before chars.\n",
        fp);
    return rc;
}

void App::version() {
    fputs("uniq (ccwrap samples) 1.0\n"
          "License: Boost Software License Version 1.0\n", stdout);
}

bool App::number(const std::string& s, size_t& out) const {
    char* end = 0;
    long  v   = strtol(s.c_str(), &end, 10);
    if (end == s.c_str() || (end != 0 && *end != '\0') || v < 0)
        return false;
    out = (size_t)v;
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

    if (name == "count")             { count_ = true; }
    else if (name == "repeated")     { unique_ = false; repeated_ = true; }
    else if (name == "unique")       { unique_ = true; repeated_ = false; }
    else if (name == "ignore-case")  { ignore_case_ = true; }
    else if (name == "zero-terminated") { delim_ = '\0'; }
    else if (name == "help")         { usage(ExOk); return false; }
    else if (name == "version")      { version(); return false; }
    else if (name == "all-repeated") {
        all_repeated_ = true;
        unique_       = false;
        if (!has_val || val == "none")     gdelim_ = CwDelimNone;
        else if (val == "prepend")         gdelim_ = CwDelimPrepend;
        else if (val == "separate")        gdelim_ = CwDelimSeparate;
        else {
            fprintf(stderr, "uniq: invalid argument '%s' for '--all-repeated'\n", val.c_str());
            status_ = ExFail;
            return false;
        }
    } else if (name == "group") {
        if (!has_val || val == "separate") group_ = CwGroupSeparate;
        else if (val == "prepend")         group_ = CwGroupPrepend;
        else if (val == "append")          group_ = CwGroupAppend;
        else if (val == "both")            group_ = CwGroupBoth;
        else {
            fprintf(stderr, "uniq: invalid argument '%s' for '--group'\n", val.c_str());
            status_ = ExFail;
            return false;
        }
    } else if (name == "skip-fields" || name == "skip-chars" || name == "check-chars") {
        if (!has_val) {
            if (i + 1 >= argc) {
                fprintf(stderr, "uniq: option '%s' requires an argument\n", arg.c_str());
                status_ = ExFail;
                return false;
            }
            cwToUtf8(argv[++i], val);
        }
        size_t v = 0;
        if (!number(val, v)) {
            fprintf(stderr, "uniq: invalid number of fields to skip: '%s'\n", val.c_str());
            status_ = ExFail;
            return false;
        }
        if (name == "skip-fields")       skip_fields_ = v;
        else if (name == "skip-chars")   skip_chars_  = v;
        else                             check_chars_ = v;
    } else {
        fprintf(stderr, "uniq: unrecognized option '%s'\n", arg.c_str());
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
        case 'c': count_ = true;                          break;
        case 'd': unique_ = false; repeated_ = true;      break;
        case 'D': all_repeated_ = true; unique_ = false; gdelim_ = CwDelimNone;  break;
        case 'i': ignore_case_ = true;                    break;
        case 'u': unique_ = true; repeated_ = false;      break;
        case 'z': delim_ = '\0';                          break;
        case 'f': case 's': case 'w': {
            std::string rest(arg, k + 1, std::string::npos);
            if (rest.empty()) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "uniq: option requires an argument -- '%c'\n", c);
                    status_ = ExFail;
                    return false;
                }
                cwToUtf8(argv[++i], rest);
            }
            size_t v = 0;
            if (!number(rest, v)) {
                fprintf(stderr, "uniq: invalid number: '%s'\n", rest.c_str());
                status_ = ExFail;
                return false;
            }
            if (c == 'f')      skip_fields_ = v;
            else if (c == 's') skip_chars_  = v;
            else               check_chars_ = v;
            return true;
        }
        default:
            if (c >= '0' && c <= '9') {
                std::string rest(arg, k, std::string::npos);
                size_t      v = 0;
                if (number(rest, v)) {
                    skip_fields_ = v;
                    return true;
                }
            }
            fprintf(stderr, "uniq: invalid option -- '%c'\n", c);
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
    if (files_.size() > 2) {
        fprintf(stderr, "uniq: extra operand '%s'\n", names_[2].c_str());
        usage(ExFail);
        status_ = ExFail;
        return false;
    }
    return true;
}

size_t App::fieldPos(const std::string& line) const {
    size_t n = line.size();
    if (n > 0 && line[n - 1] == delim_)
        --n;
    size_t i = 0;
    for (size_t f = 0; f < skip_fields_; ++f) {
        while (i < n && cwIsBlank((unsigned char)line[i]))
            ++i;
        while (i < n && !cwIsBlank((unsigned char)line[i]))
            ++i;
    }
    for (size_t c = 0; c < skip_chars_ && i < n; ++c)
        ++i;
    return i;
}

bool App::different(const std::string& a, size_t apos, const std::string& b, size_t bpos) const {
    size_t alen = a.size();
    size_t blen = b.size();
    if (alen > 0 && a[alen - 1] == delim_)
        --alen;
    if (blen > 0 && b[blen - 1] == delim_)
        --blen;
    alen = (apos < alen) ? alen - apos : 0;
    blen = (bpos < blen) ? blen - bpos : 0;
    if (check_chars_ < alen)
        alen = check_chars_;
    if (check_chars_ < blen)
        blen = check_chars_;
    if (alen != blen)
        return true;
    const char* pa = a.c_str() + apos;
    const char* pb = b.c_str() + bpos;
    if (ignore_case_) {
        for (size_t i = 0; i < alen; ++i) {
            if (cwLower(pa[i]) != cwLower(pb[i]))
                return true;
        }
        return false;
    }
    return memcmp(pa, pb, alen) != 0;
}

void App::writeRaw(const std::string& line) {
    if (line.empty())
        return;
    fwrite(line.c_str(), 1, line.size(), out_);
    if (line[line.size() - 1] != delim_)
        putc(delim_, out_);
}

void App::writeLine(const std::string& line, cw_u64 count) {
    if (count == 0 ? !unique_ : !repeated_)
        return;
    if (count_)
        fprintf(out_, "%7lu ", (unsigned long)(count + 1));
    writeRaw(line);
}

void App::runGroup() {
    std::string prev;
    std::string cur;
    size_t      prevpos  = 0;
    bool        have     = false;
    bool        first    = true;

    while (cwReadLine(in_, cur, delim_)) {
        size_t pos       = fieldPos(cur);
        bool   new_group = !have || different(cur, pos, prev, prevpos);
        if (new_group && !first) {
            if (group_ == CwGroupSeparate || group_ == CwGroupPrepend
             || group_ == CwGroupAppend   || group_ == CwGroupBoth)
                putc(delim_, out_);
        }
        if (new_group && first && (group_ == CwGroupPrepend || group_ == CwGroupBoth))
            putc(delim_, out_);
        writeRaw(cur);
        first = false;
        if (new_group) {
            prev    = cur;
            prevpos = pos;
            have    = true;
        }
    }
    if (!first && (group_ == CwGroupAppend || group_ == CwGroupBoth))
        putc(delim_, out_);
}

void App::run() {
    std::string prev;
    std::string cur;
    std::string first_of_group;
    size_t      prevpos = 0;
    cw_u64      matches = 0;
    bool        have    = false;
    bool        printed = false;
    bool        group_started = false;

    while (cwReadLine(in_, cur, delim_)) {
        size_t pos   = fieldPos(cur);
        bool   match = have && !different(cur, pos, prev, prevpos);

        if (all_repeated_) {
            if (match) {
                if (!group_started) {
                    if (printed && gdelim_ == CwDelimSeparate)
                        putc(delim_, out_);
                    if (gdelim_ == CwDelimPrepend)
                        putc(delim_, out_);
                    writeRaw(first_of_group);
                    group_started = true;
                    printed       = true;
                }
                writeRaw(cur);
            } else {
                first_of_group = cur;
                group_started  = false;
            }
            prev    = cur;
            prevpos = pos;
            have    = true;
            continue;
        }

        if (match) {
            ++matches;
            continue;
        }
        if (have)
            writeLine(prev, matches);
        prev    = cur;
        prevpos = pos;
        matches = 0;
        have    = true;
    }
    if (have && !all_repeated_)
        writeLine(prev, matches);
}

int App::main(int argc, cw_char* argv[]) {
    setlocale(LC_ALL, "");

    if (!parseArgs(argc, argv))
        return status_;

    if (count_ && all_repeated_) {
        fputs("uniq: printing all duplicated lines and repeat counts is meaningless\n", stderr);
        usage(ExFail);
        return ExFail;
    }

    in_  = stdin;
    out_ = stdout;
    if (!files_.empty() && names_[0] != "-") {
        in_ = cwFopen(files_[0], false);
        if (in_ == 0) {
            fprintf(stderr, "uniq: %s: %s\n", names_[0].c_str(), strerror(errno));
            return ExFail;
        }
    } else {
        cwBinaryMode(stdin);
    }
    if (files_.size() > 1 && names_[1] != "-") {
        out_ = cwFopen(files_[1], true);
        if (out_ == 0) {
            fprintf(stderr, "uniq: %s: %s\n", names_[1].c_str(), strerror(errno));
            return ExFail;
        }
    } else {
        cwBinaryMode(stdout);
    }

    if (group_ != CwGroupNone)
        runGroup();
    else
        run();

    if (out_ != stdout)
        fclose(out_);
    if (in_ != stdin)
        fclose(in_);
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
