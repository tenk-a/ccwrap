#include "test_cxx.hpp"

#include <cstdio>
#include <cstdarg>
#include <cstring>

TEST_CASE(cstdio, sprintf_formats) {
    char buf[64];
    STD_NS sprintf(buf, "%d-%s-%x", 42, "hi", 255);
    test_eq( STD::strcmp(buf, "42-hi-ff"), 0 );
    STD_NS sprintf(buf, "%05d", 7);
    test_eq( STD::strcmp(buf, "00007"), 0 );
    STD_NS sprintf(buf, "%.2f", 3.14159);
    test_eq( STD::strcmp(buf, "3.14"), 0 );
    STD_NS sprintf(buf, "%c%o%u", 'A', 8, 100u);
    test_eq( STD::strcmp(buf, "A10100"), 0 );
    STD_NS sprintf(buf, "%+d %-4d|", 5, 3);
    test_eq( STD::strcmp(buf, "+5 3   |"), 0 );
    test_pass("cxx03:<cstdio> sprintf formatting reachable (smoke)");
}

TEST_CASE(cstdio, snprintf_sscanf) {
    char buf[8];
    int n = STD_NS snprintf(buf, sizeof(buf), "%d", 12345);
    test_eq( n, 5 );
    test_eq( STD::strcmp(buf, "12345"), 0 );
    int t = STD_NS snprintf(buf, sizeof(buf), "%d", 1234567890);
    test_true( t < 0 || t >= (int)sizeof(buf) );

    int a = 0, b = 0;
    int got = STD_NS sscanf("12 34", "%d %d", &a, &b);
    test_eq( got, 2 );
    test_eq( a, 12 );
    test_eq( b, 34 );

    unsigned x = 0;
    STD_NS sscanf("ff", "%x", &x);
    test_eq( (int)x, 255 );
    double d = 0;
    STD_NS sscanf("3.5", "%lf", &d);
    test_true( d == 3.5 );
    char word[16];
    STD_NS sscanf("hello world", "%s", word);
    test_eq( STD::strcmp(word, "hello"), 0 );
    test_pass("cxx11:<cstdio> snprintf and sscanf reachable (smoke)");
}

static int fmt(char* dst, STD_NS size_t n, const char* f, ...) {
    STD::va_list ap; va_start(ap, f);
    int r = STD_NS vsnprintf(dst, n, f, ap);
    va_end(ap);
    return r;
}

TEST_CASE(cstdio, vsnprintf) {
    char buf[32];
    int r = fmt(buf, sizeof(buf), "%d+%d=%d", 1, 2, 3);
    test_eq( r, 5 );
    test_eq( STD::strcmp(buf, "1+2=3"), 0 );
    test_pass("cxx11:<cstdio> vsnprintf reachable (smoke)");
}

#if TEST_TARGET_CXX >= 2011
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cerrno>
#include <string>
#include <type_traits>

#if defined(_MSC_VER)
#include <io.h>
#endif

using _ccw::string_view;

#if defined(_MSC_VER)
#  include <io.h>
#  define __TST_DUP(fd)        _dup(fd)
#  define __TST_DUP2(a, b)     _dup2((a), (b))
#  define __TST_CLOSE(fd)      _close(fd)
#  define __TST_FILENO(fp)     _fileno(fp)
#  define __TST_HAS_DUP        1
#elif defined(__WATCOMC__)
#  include <io.h>
#  define __TST_DUP(fd)        dup(fd)
#  define __TST_DUP2(a, b)     dup2((a), (b))
#  define __TST_CLOSE(fd)      close(fd)
#  define __TST_FILENO(fp)     fileno(fp)
#  define __TST_HAS_DUP        1
#else

#  include <unistd.h>
#  define __TST_DUP(fd)        dup(fd)
#  define __TST_DUP2(a, b)     dup2((a), (b))
#  define __TST_CLOSE(fd)      close(fd)
#  define __TST_FILENO(fp)     fileno(fp)
#  define __TST_HAS_DUP        1
#endif

namespace {

STD::string temp_path(const char* tag) {
    const char* dir = STD::getenv("TEMP");
    if (!dir) dir = STD::getenv("TMP");
    if (!dir) dir = STD::getenv("TMPDIR");
    STD::string p = dir ? STD::string(dir) : STD::string(".");
#if defined(__DOS__) || defined(MSDOS)
    p += "\\q";
    STD::string t(tag);
    p += t.size() > 7 ? t.substr(0, 7) : t;
#elif defined(_WIN32)
    p += "\\ccw_chk37_";
    p += tag;
#else
    p += "/ccw_chk37_";
    p += tag;
#endif
    p += ".tmp";
    return p;
}

STD::string slurp(const char* path) {
    STD::string out;
    STD_NS FILE* fp = STD_NS fopen(path, "rb");
    if (!fp) return out;
    char buf[256];
    STD_NS size_t n;
    while ((n = STD_NS fread(buf, 1, sizeof buf, fp)) > 0)
        out.append(buf, n);
    STD_NS fclose(fp);
    return out;
}

static int __ccw_dup_fd(STD_NS FILE* fp) {
#if __TST_HAS_DUP
    return __TST_DUP(__TST_FILENO(fp));
#else
    (void)fp; return -1;
#endif
}

static void __ccw_restore_fd(STD_NS FILE* fp, int saved) {
#if __TST_HAS_DUP
    if (saved >= 0) { __TST_DUP2(saved, __TST_FILENO(fp)); __TST_CLOSE(saved); }
#else
    (void)fp; (void)saved;
#endif
}

class Redirect {
public:
    Redirect(STD_NS FILE* stream, const char* path, const char* mode)
        : stream_(stream), saved_(-1), ok_(0) {
        STD_NS fflush(stream);
        saved_ = __ccw_dup_fd(stream);
        ok_ = STD_NS freopen(path, mode, stream);
    }
    ~Redirect() {
        STD_NS fflush(stream_);
        __ccw_restore_fd(stream_, saved_);
        STD_NS clearerr(stream_);
    }
    bool ok() const { return ok_ != 0; }
private:
    Redirect(const Redirect&);
    Redirect& operator=(const Redirect&);
    STD_NS FILE*  stream_;
    int         saved_;
    STD_NS FILE*  ok_;
};

int v_fprintf(STD_NS FILE* fp, const char* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int r = STD_NS vfprintf(fp, fmt, ap);
    va_end(ap);
    return r;
}
int v_printf(const char* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int r = STD_NS vprintf(fmt, ap);
    va_end(ap);
    return r;
}
int v_sprintf(char* buf, const char* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int r = STD_NS vsprintf(buf, fmt, ap);
    va_end(ap);
    return r;
}
int v_snprintf(char* buf, STD_NS size_t n, const char* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int r = STD_NS vsnprintf(buf, n, fmt, ap);
    va_end(ap);
    return r;
}
int v_fscanf(STD_NS FILE* fp, const char* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int r = STD_NS vfscanf(fp, fmt, ap);
    va_end(ap);
    return r;
}
int v_scanf(const char* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int r = STD_NS vscanf(fmt, ap);
    va_end(ap);
    return r;
}
int v_sscanf(const char* s, const char* fmt, ...) {
    STD::va_list ap; va_start(ap, fmt);
    int r = STD_NS vsscanf(s, fmt, ap);
    va_end(ap);
    return r;
}

}

TEST_CASE(cstdio, types) {

    test_true( (STD::is_unsigned<STD_NS size_t>::value) );

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<STD_NS size_t, decltype(sizeof(int))>::value) );
    test_pass("cxx03:size_t");
#else
    TEST_SKIP1();
    test_skip("cxx03:size_t");
#endif

    STD_NS FILE* fp = STD_NS tmpfile();
    test_ptr( fp );
#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<STD_NS FILE*, decltype(fp)>::value) );
    test_pass("cxx03:FILE");
#else
    TEST_SKIP1();
    test_skip("cxx03:FILE");
#endif

    STD_NS fpos_t pos;
    STD::memset(&pos, 0, sizeof pos);
    int gp = STD_NS fgetpos(fp, &pos);
    test_eq( gp, 0 );
    test_true( sizeof(STD_NS fpos_t) > 0 );
    STD_NS fclose(fp);
    test_pass("cxx03:fpos_t");
}

TEST_CASE(cstdio, macros) {

    STD_NS FILE* p = NULL;
    test_true( p == 0 );
    test_pass("cxx03:NULL");

    test_lt( EOF, 0 );
    test_pass("cxx03:EOF");

    test_ne( _IOFBF, _IOLBF );
    test_pass("cxx03:_IOFBF");
    test_ne( _IOLBF, _IONBF );
    test_pass("cxx03:_IOLBF");
    test_ne( _IOFBF, _IONBF );
    test_pass("cxx03:_IONBF");

    test_gt( (long)BUFSIZ, 0L );
    test_pass("cxx03:BUFSIZ");
    test_ge( (long)FOPEN_MAX, 8L );
    test_pass("cxx03:FOPEN_MAX");
    test_gt( (long)FILENAME_MAX, 0L );
    test_pass("cxx03:FILENAME_MAX");
    test_gt( (long)L_tmpnam, 0L );
    test_pass("cxx03:L_tmpnam");
    test_ge( (long)TMP_MAX, 25L );
    test_pass("cxx03:TMP_MAX");

    test_ne( SEEK_SET, SEEK_CUR );
    test_pass("cxx03:SEEK_SET");
    test_ne( SEEK_CUR, SEEK_END );
    test_pass("cxx03:SEEK_CUR");
    test_ne( SEEK_SET, SEEK_END );
    test_pass("cxx03:SEEK_END");

    test_ptr( stdin );
    test_pass("cxx03:stdin");
    test_ptr( stdout );
    test_true( stdin != stdout );
    test_pass("cxx03:stdout");
    test_ptr( stderr );
    test_true( stdout != stderr );
    test_pass("cxx03:stderr");
}

TEST_CASE(cstdio, fopen_fclose_remove_rename) {
    STD::string a = temp_path("open");
    STD::string b = temp_path("renamed");
    STD_NS remove(a.c_str());
    STD_NS remove(b.c_str());

    STD_NS FILE* fp = STD_NS fopen(a.c_str(), "w");
    test_ptr( fp );
    STD_NS FILE* miss = STD_NS fopen(temp_path("no_such_file_at_all").c_str(), "r");
    test_true( miss == NULL );
    test_pass("cxx03:fopen");

    int put = STD_NS fputs("hello", fp);
    test_ge( put, 0 );
    int cl = STD_NS fclose(fp);
    test_eq( cl, 0 );
    STD_NS FILE* rp = STD_NS fopen(a.c_str(), "r");
    test_ptr( rp );
    char buf[16] = {0};
    char* g = STD_NS fgets(buf, sizeof buf, rp);
    test_ptr( g );
    test_str_eq( buf, "hello" );
    int cl2 = STD_NS fclose(rp);
    test_eq( cl2, 0 );
    test_pass("cxx03:fclose");

    int rn = STD_NS rename(a.c_str(), b.c_str());
    test_eq( rn, 0 );
    test_true( STD_NS fopen(a.c_str(), "r") == NULL );
    STD::string body = slurp(b.c_str());
    test_str_eq( body.c_str(), "hello" );
    test_pass("cxx03:rename");

    int rm = STD_NS remove(b.c_str());
    test_eq( rm, 0 );
    test_true( STD_NS fopen(b.c_str(), "r") == NULL );
    int rm2 = STD_NS remove(b.c_str());
    test_ne( rm2, 0 );
    test_pass("cxx03:remove");
}

TEST_CASE(cstdio, tmpfile_tmpnam) {

    STD_NS FILE* fp = STD_NS tmpfile();
    test_ptr( fp );
    STD_NS size_t w = STD_NS fwrite("xy", 1, 2, fp);
    test_eq( w, (STD_NS size_t)2 );
    STD_NS rewind(fp);
    int c = STD_NS fgetc(fp);
    test_eq( c, (int)'x' );
    int cl = STD_NS fclose(fp);
    test_eq( cl, 0 );
    test_pass("cxx03:tmpfile");

    char name[L_tmpnam + 1];
    char* r = STD_NS tmpnam(name);
    test_true( r == name );
    test_gt( STD::strlen(name), (STD_NS size_t)0 );

    char* r2 = STD_NS tmpnam(NULL);
    test_ptr( r2 );
    test_gt( STD::strlen(r2), (STD_NS size_t)0 );

    test_pass("cxx03:tmpnam");
}

TEST_CASE(cstdio, freopen_setbuf_setvbuf) {
    STD::string p1 = temp_path("reop1");
    STD::string p2 = temp_path("reop2");

    STD_NS FILE* fp = STD_NS fopen(p1.c_str(), "wb");
    test_ptr( fp );
    char mybuf[BUFSIZ];
    int sv = STD_NS setvbuf(fp, mybuf, _IOFBF, sizeof mybuf);
    test_eq( sv, 0 );
    STD_NS fputs("one", fp);

    int fl = STD_NS fflush(fp);
    test_eq( fl, 0 );
    STD::string s1 = slurp(p1.c_str());
    test_str_eq( s1.c_str(), "one" );
    test_pass("cxx03:setvbuf");

    STD_NS FILE* fp2 = STD_NS freopen(p2.c_str(), "wb", fp);
    test_ptr( fp2 );
    test_true( fp2 == fp );
    test_pass("cxx03:freopen");

    STD_NS setbuf(fp, NULL);
    STD_NS fputs("two", fp);
    int cl = STD_NS fclose(fp);
    test_eq( cl, 0 );
    STD::string s2 = slurp(p2.c_str());
    test_str_eq( s2.c_str(), "two" );
    test_pass("cxx03:setbuf");

    STD_NS FILE* fp3 = STD_NS fopen(p1.c_str(), "wb");
    test_ptr( fp3 );
    int sv3 = STD_NS setvbuf(fp3, NULL, _IONBF, 0);
    test_eq( sv3, 0 );
    STD_NS fputs("three", fp3);
    STD_NS fclose(fp3);
    STD::string s3 = slurp(p1.c_str());
    test_str_eq( s3.c_str(), "three" );

    STD_NS FILE* fp4 = STD_NS fopen(p1.c_str(), "wb");
    test_ptr( fp4 );
    int sv4 = STD_NS setvbuf(fp4, NULL, _IOLBF, BUFSIZ);
    test_eq( sv4, 0 );
    STD_NS fputs("four\n", fp4);
    STD_NS fclose(fp4);
    STD::string s4 = slurp(p1.c_str());
    test_str_eq( s4.c_str(), "four\n" );
    test_pass("cxx03:setvbuf buffering modes");

    int fla = STD_NS fflush(NULL);
    test_eq( fla, 0 );
    test_pass("cxx03:fflush");

    STD_NS remove(p1.c_str());
    STD_NS remove(p2.c_str());
}

TEST_CASE(cstdio, fread_fwrite) {
    STD::string p = temp_path("block");
    STD_NS FILE* fp = STD_NS fopen(p.c_str(), "wb");
    test_ptr( fp );

    const int src[4] = { 1, 2, 3, 4 };
    STD_NS size_t w = STD_NS fwrite(src, sizeof(int), 4, fp);
    test_eq( w, (STD_NS size_t)4 );
    STD_NS size_t w0 = STD_NS fwrite(src, sizeof(int), 0, fp);
    test_eq( w0, (STD_NS size_t)0 );
    int cl = STD_NS fclose(fp);
    test_eq( cl, 0 );
    test_pass("cxx03:fwrite");

    STD_NS FILE* rp = STD_NS fopen(p.c_str(), "rb");
    test_ptr( rp );
    int dst[6] = { 0, 0, 0, 0, 0, 0 };
    STD_NS size_t r = STD_NS fread(dst, sizeof(int), 6, rp);
    test_eq( r, (STD_NS size_t)4 );
    test_eq( dst[0], 1 );
    test_eq( dst[3], 4 );
    test_eq( dst[4], 0 );
    test_ne( STD_NS feof(rp), 0 );
    STD_NS fclose(rp);

    STD_NS remove(p.c_str());
    test_pass("cxx03:fread");
}

TEST_CASE(cstdio, char_and_line_io_file) {
    STD::string p = temp_path("chars");
    STD_NS FILE* fp = STD_NS fopen(p.c_str(), "w+");
    test_ptr( fp );

    int a = STD_NS fputc('a', fp);
    test_eq( a, (int)'a' );
    test_pass("cxx03:fputc");
    int b = STD_NS putc('b', fp);
    test_eq( b, (int)'b' );
    test_pass("cxx03:putc");
    int s = STD_NS fputs("cd\nef\n", fp);
    test_ge( s, 0 );
    test_pass("cxx03:fputs");

    STD_NS rewind(fp);

    int c1 = STD_NS fgetc(fp);
    test_eq( c1, (int)'a' );
    test_pass("cxx03:fgetc");
    int c2 = STD_NS getc(fp);
    test_eq( c2, (int)'b' );
    test_pass("cxx03:getc");

    int u = STD_NS ungetc(c2, fp);
    test_eq( u, (int)'b' );
    int c3 = STD_NS fgetc(fp);
    test_eq( c3, (int)'b' );
    test_pass("cxx03:ungetc");

    char line[16];
    char* g = STD_NS fgets(line, sizeof line, fp);
    test_ptr( g );
    test_str_eq( line, "cd\n" );

    char small[3];
    char* g2 = STD_NS fgets(small, 3, fp);
    test_ptr( g2 );
    test_str_eq( small, "ef" );
    char* g3 = STD_NS fgets(small, 3, fp);
    test_ptr( g3 );
    test_str_eq( small, "\n" );

    int e = STD_NS fgetc(fp);
    test_eq( e, EOF );
    char* g4 = STD_NS fgets(line, sizeof line, fp);
    test_true( g4 == NULL );

    STD_NS fclose(fp);
    STD_NS remove(p.c_str());
    test_pass("cxx03:fgets");
}

TEST_CASE(cstdio, stdout_family) {

    STD::string p = temp_path("stdout");
    bool ok = false;
    int n1 = 0, n2 = 0, c = 0, s = 0;
    {
        Redirect r(stdout, p.c_str(), "wb");
        ok = r.ok();
        n1 = STD_NS printf("[%d,%s]", 42, "x");
        n2 = v_printf("(%c)", 'z');
        c  = STD_NS putchar('P');
        s  = STD_NS puts("done");
        STD_NS fflush(stdout);
    }
    STD::string out = slurp(p.c_str());
    test_true( ok );

    test_eq( n1, 6 );
    test_pass("cxx03:printf");

    test_eq( n2, 3 );
    test_pass("cxx03:vprintf");

    test_eq( c, (int)'P' );
    test_pass("cxx03:putchar");

    test_ge( s, 0 );
    test_str_eq( out.c_str(), "[42,x](z)Pdone\n" );
    STD_NS remove(p.c_str());
    test_pass("cxx03:puts");
}

TEST_CASE(cstdio, stdin_family) {

    STD::string p = temp_path("stdin");
    {
        STD_NS FILE* fp = STD_NS fopen(p.c_str(), "w");
        test_ptr( fp );
        STD_NS fputs("12 ab\n7\nQ", fp);
        STD_NS fclose(fp);
    }
    {
        Redirect r(stdin, p.c_str(), "r");
        test_true( r.ok() );

        int  i = 0;
        char word[16] = {0};
        int  got = STD_NS scanf("%d %15s", &i, word);
        test_eq( got, 2 );
        test_eq( i, 12 );
        test_str_eq( word, "ab" );
        test_pass("cxx03:scanf");

        int j = 0;
        int got2 = v_scanf("%d", &j);
        test_eq( got2, 1 );
        test_eq( j, 7 );
        test_pass("cxx03:vscanf");

        int c = STD_NS getchar();
        test_eq( c, (int)'\n' );
        int c2 = STD_NS getchar();
        test_eq( c2, (int)'Q' );

        int c3 = STD_NS getc(stdin);
        test_eq( c3, EOF );
        test_pass("cxx03:getchar");
    }
    STD_NS remove(p.c_str());
}

TEST_CASE(cstdio, printf_to_string) {
    char buf[32];

    int n = STD_NS sprintf(buf, "%d-%s-%c", 12, "ab", 'z');
    test_eq( n, 7 );
    test_str_eq( buf, "12-ab-z" );

    char wide[32];
    int n6 = STD_NS sprintf(wide, "%5d|%-5s|%x|%%", 7, "ab", 255);
    test_eq( n6, 16 );
    test_str_eq( wide, "    7|ab   |ff|%" );
    test_pass("cxx03:sprintf");

    int n2 = v_sprintf(buf, "%05.2f", 1.5);
    test_eq( n2, 5 );
    test_str_eq( buf, "01.50" );
    test_pass("cxx03:vsprintf");

    char small[4];
    int n3 = STD_NS snprintf(small, sizeof small, "%s", "abcdef");
    test_eq( n3, 6 );
    test_str_eq( small, "abc" );

    int n4 = STD_NS snprintf(NULL, 0, "%d", 12345);
    test_eq( n4, 5 );
    test_pass("cxx11:snprintf");

    int n5 = v_snprintf(small, sizeof small, "%s", "xy");
    test_eq( n5, 2 );
    test_str_eq( small, "xy" );
    test_pass("cxx11:vsnprintf");
}

TEST_CASE(cstdio, printf_to_file) {
    STD::string p = temp_path("fprintf");
    STD_NS FILE* fp = STD_NS fopen(p.c_str(), "wb+");
    test_ptr( fp );

    int n1 = STD_NS fprintf(fp, "%d %s\n", 3, "abc");
    test_eq( n1, 6 );
    test_pass("cxx03:fprintf");

    int n2 = v_fprintf(fp, "%.3f\n", 0.5);
    test_eq( n2, 6 );
    STD_NS fflush(fp);

    test_str_eq( slurp(p.c_str()).c_str(), "3 abc\n0.500\n" );
    test_pass("cxx03:vfprintf");

    STD_NS fclose(fp);
    STD_NS remove(p.c_str());
}

TEST_CASE(cstdio, scanf_family) {

    int  i = 0;
    char s[16] = {0};
    int  got = STD_NS sscanf("42 hello", "%d %15s", &i, s);
    test_eq( got, 2 );
    test_eq( i, 42 );
    test_str_eq( s, "hello" );

    int x = 0;
    int got3 = STD_NS sscanf("zz", "%d", &x);
    test_eq( got3, 0 );

    int y = 0;
    int got4 = STD_NS sscanf("", "%d", &y);
    test_eq( got4, EOF );
    test_pass("cxx03:sscanf");

    double d = 0.0;
    int got2 = v_sscanf("2.5", "%lf", &d);
    test_eq( got2, 1 );
    test_true( d > 2.49 && d < 2.51 );
    test_pass("cxx11:vsscanf");

    STD::string p = temp_path("scanf");
    {
        STD_NS FILE* fp = STD_NS fopen(p.c_str(), "w");
        test_ptr( fp );
        STD_NS fputs("10 20\n1.5\n", fp);
        STD_NS fclose(fp);
    }
    STD_NS FILE* rp = STD_NS fopen(p.c_str(), "r");
    test_ptr( rp );

    int a = 0, b = 0;
    int gf = STD_NS fscanf(rp, "%d %d", &a, &b);
    test_eq( gf, 2 );
    test_eq( a, 10 );
    test_eq( b, 20 );
    test_pass("cxx03:fscanf");

    double f = 0.0;
    int gv = v_fscanf(rp, "%lf", &f);
    test_eq( gv, 1 );
    test_true( f > 1.49 && f < 1.51 );
    test_pass("cxx11:vfscanf");

    int dummy = 0;
    int ge = STD_NS fscanf(rp, "%d", &dummy);
    test_eq( ge, EOF );
    test_pass("cxx03:fscanf (EOF)");

    STD_NS fclose(rp);
    STD_NS remove(p.c_str());
}

TEST_CASE(cstdio, printf_integer) {
    char b[64];

    STD_NS snprintf(b, sizeof b, "%d", 42);   test_str_eq(b, "42");   test_pass("cxx03:printf %d");
    STD_NS snprintf(b, sizeof b, "%i", 42);   test_str_eq(b, "42");   test_pass("cxx03:printf %i");
    STD_NS snprintf(b, sizeof b, "%u", 42u);  test_str_eq(b, "42");   test_pass("cxx03:printf %u");
    STD_NS snprintf(b, sizeof b, "%o", 8);    test_str_eq(b, "10");   test_pass("cxx03:printf %o");
    STD_NS snprintf(b, sizeof b, "%x", 255);  test_str_eq(b, "ff");   test_pass("cxx03:printf %x");
    STD_NS snprintf(b, sizeof b, "%X", 255);  test_str_eq(b, "FF");   test_pass("cxx03:printf %X");

    STD_NS snprintf(b, sizeof b, "%5d", 42);    test_str_eq(b, "   42"); test_pass("cxx03:printf %5d");
    STD_NS snprintf(b, sizeof b, "%-5d", 42);   test_str_eq(b, "42   "); test_pass("cxx03:printf %-5d");
    STD_NS snprintf(b, sizeof b, "%05d", 42);   test_str_eq(b, "00042"); test_pass("cxx03:printf %05d");
    STD_NS snprintf(b, sizeof b, "%+d", 42);    test_str_eq(b, "+42");   test_pass("cxx03:printf %+d");
    STD_NS snprintf(b, sizeof b, "% d", 42);    test_str_eq(b, " 42");   test_pass("cxx03:printf % d");
    STD_NS snprintf(b, sizeof b, "%#x", 255);   test_str_eq(b, "0xff");  test_pass("cxx03:printf %#x");
    STD_NS snprintf(b, sizeof b, "%#o", 8);     test_str_eq(b, "010");   test_pass("cxx03:printf %#o");
    STD_NS snprintf(b, sizeof b, "%.3d", 5);    test_str_eq(b, "005");   test_pass("cxx03:printf %.3d");
    STD_NS snprintf(b, sizeof b, "%8.3d", 5);   test_str_eq(b, "     005"); test_pass("cxx03:printf %8.3d");
}

TEST_CASE(cstdio, printf_length_mods) {
    char b[64];

    STD_NS snprintf(b, sizeof b, "%hd", (short)-3);          test_str_eq(b, "-3"); test_pass("cxx03:printf %hd");
    STD_NS snprintf(b, sizeof b, "%hhd", (signed char)-4);   test_str_eq(b, "-4"); test_pass("cxx11:printf %hhd");
    STD_NS snprintf(b, sizeof b, "%ld", 123456789L);         test_str_eq(b, "123456789"); test_pass("cxx03:printf %ld");
    STD_NS snprintf(b, sizeof b, "%lx", 255L);               test_str_eq(b, "ff"); test_pass("cxx03:printf %lx");
    STD_NS snprintf(b, sizeof b, "%lld", -9000000000LL);     test_str_eq(b, "-9000000000"); test_pass("cxx11:printf %lld");
    STD_NS snprintf(b, sizeof b, "%llu", 12000000000ULL);    test_str_eq(b, "12000000000"); test_pass("cxx11:printf %llu");

    STD_NS snprintf(b, sizeof b, "%ju", (STD::uintmax_t)7);
    TEST_SKIP_VC120("pre-UCRT MSVC CRT prints the z/t/j length modifiers literally");
    test_str_eq(b, "7"); test_pass("cxx11:printf %ju");

    STD_NS snprintf(b, sizeof b, "%zu", (STD_NS size_t)7);
    TEST_SKIP_VC120("pre-UCRT MSVC CRT prints the z/t/j length modifiers literally");
    test_str_eq(b, "7"); test_pass("cxx11:printf %zu");

    STD_NS snprintf(b, sizeof b, "%td", (STD::ptrdiff_t)-5);
    TEST_SKIP_VC120("pre-UCRT MSVC CRT prints the z/t/j length modifiers literally");
    test_str_eq(b, "-5"); test_pass("cxx11:printf %td");
}

TEST_CASE(cstdio, printf_float) {
    char b[64];

    STD_NS snprintf(b, sizeof b, "%f", 1.5);   test_str_eq(b, "1.500000"); test_pass("cxx03:printf %f");

    STD_NS snprintf(b, sizeof b, "%.2f", 3.14159);    test_str_eq(b, "3.14");       test_pass("cxx03:printf %.2f");
    STD_NS snprintf(b, sizeof b, "%10.3f", 3.14159);  test_str_eq(b, "     3.142"); test_pass("cxx03:printf %10.3f");
    STD_NS snprintf(b, sizeof b, "%-10.3f", 3.14159); test_str_eq(b, "3.142     "); test_pass("cxx03:printf %-10.3f");
    STD_NS snprintf(b, sizeof b, "%+.2f", 3.14159);   test_str_eq(b, "+3.14");      test_pass("cxx03:printf %+.2f");
    STD_NS snprintf(b, sizeof b, "%010.3f", 3.14159); test_str_eq(b, "000003.142"); test_pass("cxx03:printf %010.3f");

    STD_NS snprintf(b, sizeof b, "%F", 1.5);
    TEST_SKIP_VC120("%F (uppercase fixed) is a C99 addition the pre-UCRT MSVC CRT lacks");
    TEST_SKIP_DOS("%F (uppercase fixed) is not recognised by Open Watcom's DOS CRT");
    test_str_eq(b, "1.500000"); test_pass("cxx03:printf %F");

    STD_NS snprintf(b, sizeof b, "%e", 1500.0);
    TEST_SKIP_VC120("pre-UCRT MSVC prints a 3-digit exponent (e+003)");
    test_str_eq(b, "1.500000e+03"); test_pass("cxx03:printf %e");

    STD_NS snprintf(b, sizeof b, "%E", 1500.0);
    TEST_SKIP_VC120("pre-UCRT MSVC prints a 3-digit exponent (E+003)");
    test_str_eq(b, "1.500000E+03"); test_pass("cxx03:printf %E");

    STD_NS snprintf(b, sizeof b, "%g", 0.0001);   test_str_eq(b, "0.0001"); test_pass("cxx03:printf %g");
    STD_NS snprintf(b, sizeof b, "%g", 1000000.0);
    TEST_SKIP_VC120("pre-UCRT MSVC prints a 3-digit exponent (e+006)");
    test_str_eq(b, "1e+06"); test_pass("cxx03:printf %g exp");

    STD_NS snprintf(b, sizeof b, "%G", 1000000.0);
    TEST_SKIP_VC120("pre-UCRT MSVC prints a 3-digit exponent (E+006)");
    test_str_eq(b, "1E+06"); test_pass("cxx03:printf %G");

    STD_NS snprintf(b, sizeof b, "%.6g", 3.14159);  test_str_eq(b, "3.14159"); test_pass("cxx03:printf %.6g");
    STD_NS snprintf(b, sizeof b, "%#g", 1.5);       test_str_eq(b, "1.50000"); test_pass("cxx03:printf %#g");
}

TEST_CASE(cstdio, printf_combinations) {
    char b[64];

    STD_NS snprintf(b, sizeof b, "%-10.6g", 3.14159); test_str_eq(b, "3.14159   "); test_pass("cxx03:printf %-10.6g");
    STD_NS snprintf(b, sizeof b, "%+08.2f", 3.14159); test_str_eq(b, "+0003.14");   test_pass("cxx03:printf %+08.2f");
    STD_NS snprintf(b, sizeof b, "%#010x", 255);      test_str_eq(b, "0x000000ff"); test_pass("cxx03:printf %#010x");

    STD_NS snprintf(b, sizeof b, "%10.3e", 12345.678);
    TEST_SKIP_VC120("pre-UCRT MSVC prints a 3-digit exponent (e+004)");
    test_str_eq(b, " 1.235e+04"); test_pass("cxx03:printf %10.3e");

    STD_NS snprintf(b, sizeof b, "% 08.2f", 3.14159); test_str_eq(b, " 0003.14"); test_pass("cxx03:printf % 08.2f");
}

TEST_CASE(cstdio, printf_char_string) {
    char b[64];

    STD_NS snprintf(b, sizeof b, "%c", 'A');           test_str_eq(b, "A");     test_pass("cxx03:printf %c");
    STD_NS snprintf(b, sizeof b, "%s", "hi");          test_str_eq(b, "hi");    test_pass("cxx03:printf %s");
    STD_NS snprintf(b, sizeof b, "%5s", "hi");         test_str_eq(b, "   hi"); test_pass("cxx03:printf %5s");
    STD_NS snprintf(b, sizeof b, "%-5s", "hi");        test_str_eq(b, "hi   "); test_pass("cxx03:printf %-5s");
    STD_NS snprintf(b, sizeof b, "%.3s", "abcdef");    test_str_eq(b, "abc");   test_pass("cxx03:printf %.3s");
    STD_NS snprintf(b, sizeof b, "%%");                test_str_eq(b, "%");     test_pass("cxx03:printf %%");

    void* pv = (void*)b;
    STD_NS snprintf(b, sizeof b, "%p", pv);
    test_gt( STD::strlen(b), (STD_NS size_t)0 );
    test_pass("cxx03:printf %p");

    STD_NS snprintf(b, sizeof b, "%a", 1.0);
    TEST_SKIP_WAT("hex-float %a formatting is not provided");
    TEST_SKIP_VC120("%a (hex float) is a C99 addition the pre-UCRT MSVC CRT lacks");
    test_true( STD::strchr(b, 'p') != 0 );
    test_pass("cxx11:printf %a");

    STD_NS snprintf(b, sizeof b, "%A", 1.0);
    TEST_SKIP_WAT("hex-float %A formatting is not provided");
    TEST_SKIP_VC120("%A (hex float) is a C99 addition the pre-UCRT MSVC CRT lacks");
    test_true( STD::strchr(b, 'P') != 0 );
    test_pass("cxx11:printf %A");
}

TEST_CASE(cstdio, sscanf_conversions) {
    char word[32];
    int  r;

    { int v = 0;      r = STD_NS sscanf("42", "%d", &v);  test_true(r == 1 && v == 42);  test_pass("cxx03:sscanf %d"); }
    { int v = 0;      r = STD_NS sscanf("42", "%i", &v);  test_true(r == 1 && v == 42);  test_pass("cxx03:sscanf %i"); }
    { unsigned v = 0; r = STD_NS sscanf("42", "%u", &v);  test_true(r == 1 && v == 42u); test_pass("cxx03:sscanf %u"); }
    { unsigned v = 0; r = STD_NS sscanf("17", "%o", &v);  test_true(r == 1 && v == 15u); test_pass("cxx03:sscanf %o"); }
    { unsigned v = 0; r = STD_NS sscanf("ff", "%x", &v);  test_true(r == 1 && v == 255u);test_pass("cxx03:sscanf %x"); }
    { float v = 0.f;  r = STD_NS sscanf("3.5",   "%f", &v); test_true(r == 1 && v > 3.49f && v < 3.51f);    test_pass("cxx03:sscanf %f"); }
    { float v = 0.f;  r = STD_NS sscanf("1.5e2", "%e", &v); test_true(r == 1 && v > 149.0f && v < 151.0f);  test_pass("cxx03:sscanf %e"); }
    { float v = 0.f;  r = STD_NS sscanf("2.5",   "%g", &v); test_true(r == 1 && v > 2.49f && v < 2.51f);    test_pass("cxx03:sscanf %g"); }
    { char c = 0;     r = STD_NS sscanf("Z", "%c", &c);   test_true(r == 1 && c == 'Z'); test_pass("cxx03:sscanf %c"); }
    { r = STD_NS sscanf("hello world", "%31s", word);     test_true(r == 1); test_str_eq(word, "hello"); test_pass("cxx03:sscanf %s"); }

    { r = STD_NS sscanf("abcxyz", "%31[abc]", word);      test_true(r == 1); test_str_eq(word, "abc");   test_pass("cxx03:sscanf %[...]"); }
    { r = STD_NS sscanf("hello-world", "%31[^-]", word);  test_true(r == 1); test_str_eq(word, "hello"); test_pass("cxx03:sscanf %[^-]"); }

    { int v = 0; r = STD_NS sscanf("9 8", "%*d %d", &v);  test_true(r == 1 && v == 8);     test_pass("cxx03:sscanf %*d"); }
    { int v = 0; r = STD_NS sscanf("123456", "%5d", &v);  test_true(r == 1 && v == 12345); test_pass("cxx03:sscanf %5d"); }

#if defined(_MSC_VER) && _MSC_VER < 1900
    { TEST_NOTE("pre-UCRT MSVC scanf lacks the %hh length modifier and writes past the object");
      TEST_SKIP1(); test_skip("cxx11:sscanf %hhd"); }
#else
    { signed char v = 0;
      r = STD_NS sscanf("-4", "%hhd", &v); test_true(r == 1 && v == -4); test_pass("cxx11:sscanf %hhd"); }
#endif
    { long v = 0; r = STD_NS sscanf("123456", "%ld", &v); test_true(r == 1 && v == 123456L); test_pass("cxx03:sscanf %ld"); }
    { long long v = 0;
      TEST_SKIP_VC120("pre-UCRT MSVC scanf uses %I64d, not the C99 %lld");
      r = STD_NS sscanf("-9000000000", "%lld", &v); test_true(r == 1 && v == -9000000000LL); test_pass("cxx11:sscanf %lld"); }
    { double v = 0.0; r = STD_NS sscanf("3.5", "%lf", &v); test_true(r == 1 && v > 3.49 && v < 3.51); test_pass("cxx03:sscanf %lf"); }

    { int a = 0, bb = 0, c = 0; int n = STD_NS sscanf("1 2 3", "%d %d %d", &a, &bb, &c);
      test_true(n == 3 && a == 1 && bb == 2 && c == 3); test_pass("cxx03:sscanf return value"); }
}

TEST_CASE(cstdio, positioning) {
    STD::string p = temp_path("seek");
    STD_NS FILE* fp = STD_NS fopen(p.c_str(), "wb+");
    test_ptr( fp );

    STD_NS fwrite("0123456789", 1, 10, fp);
    long end = STD_NS ftell(fp);
    test_eq( end, 10L );
    test_pass("cxx03:ftell");

    int s1 = STD_NS fseek(fp, 3, SEEK_SET);
    test_eq( s1, 0 );
    long t1 = STD_NS ftell(fp);
    test_eq( t1, 3L );
    int c1 = STD_NS fgetc(fp);
    test_eq( c1, (int)'3' );

    int s2 = STD_NS fseek(fp, 2, SEEK_CUR);
    test_eq( s2, 0 );
    long t2 = STD_NS ftell(fp);
    test_eq( t2, 6L );
    int c2 = STD_NS fgetc(fp);
    test_eq( c2, (int)'6' );

    int s3 = STD_NS fseek(fp, -2, SEEK_END);
    test_eq( s3, 0 );
    long t3 = STD_NS ftell(fp);
    test_eq( t3, 8L );
    int c3 = STD_NS fgetc(fp);
    test_eq( c3, (int)'8' );
    test_pass("cxx03:fseek");

    STD_NS fseek(fp, 4, SEEK_SET);
    STD_NS fpos_t pos;
    int gp = STD_NS fgetpos(fp, &pos);
    test_eq( gp, 0 );
    int c4 = STD_NS fgetc(fp);
    test_eq( c4, (int)'4' );
    test_pass("cxx03:fgetpos");

    STD_NS fseek(fp, 0, SEEK_SET);
    int sp = STD_NS fsetpos(fp, &pos);
    test_eq( sp, 0 );
    long t4 = STD_NS ftell(fp);
    test_eq( t4, 4L );
    int c5 = STD_NS fgetc(fp);
    test_eq( c5, (int)'4' );
    test_pass("cxx03:fsetpos");

    STD_NS fseek(fp, 0, SEEK_END);
    int e = STD_NS fgetc(fp);
    test_eq( e, EOF );
    test_ne( STD_NS feof(fp), 0 );
    STD_NS rewind(fp);
    test_eq( STD_NS feof(fp), 0 );
    long t5 = STD_NS ftell(fp);
    test_eq( t5, 0L );
    int c6 = STD_NS fgetc(fp);
    test_eq( c6, (int)'0' );

    STD_NS fclose(fp);
    STD_NS remove(p.c_str());
    test_pass("cxx03:rewind");
}

TEST_CASE(cstdio, error_handling) {
    STD::string p = temp_path("err");
    {
        STD_NS FILE* fp = STD_NS fopen(p.c_str(), "w");
        test_ptr( fp );
        STD_NS fputs("a", fp);
        STD_NS fclose(fp);
    }

    STD_NS FILE* fp = STD_NS fopen(p.c_str(), "r");
    test_ptr( fp );

    test_eq( STD_NS feof(fp), 0 );
    test_eq( STD_NS ferror(fp), 0 );

    int c = STD_NS fgetc(fp);
    test_eq( c, (int)'a' );
    int e = STD_NS fgetc(fp);
    test_eq( e, EOF );
    test_ne( STD_NS feof(fp), 0 );
    test_eq( STD_NS ferror(fp), 0 );
    test_pass("cxx03:feof");

    STD_NS clearerr(fp);
    test_eq( STD_NS feof(fp), 0 );
    test_eq( STD_NS ferror(fp), 0 );
    test_pass("cxx03:clearerr");

    int w = STD_NS fputc('x', fp);
    test_eq( w, EOF );
    test_ne( STD_NS ferror(fp), 0 );
    STD_NS clearerr(fp);
    test_eq( STD_NS ferror(fp), 0 );
    test_pass("cxx03:ferror");

    STD_NS fclose(fp);
    STD_NS remove(p.c_str());

    STD::string pe = temp_path("perror");
    {
        Redirect r(stderr, pe.c_str(), "w");
        test_true( r.ok() );
        errno = EDOM;
        STD_NS perror("ccw");
        STD_NS perror(NULL);
        STD_NS perror("");
        STD_NS fflush(stderr);
    }
    errno = 0;
    STD::string txt = slurp(pe.c_str());
    test_gt( txt.size(), (STD_NS size_t)0 );
    test_true( txt.find("ccw") != STD::string::npos );
    STD_NS remove(pe.c_str());
    test_pass("cxx03:perror");
}
#else
TEST_CASE_SKIP(cstdio, types)
TEST_CASE_SKIP(cstdio, macros)
TEST_CASE_SKIP(cstdio, fopen_fclose_remove_rename)
TEST_CASE_SKIP(cstdio, tmpfile_tmpnam)
TEST_CASE_SKIP(cstdio, freopen_setbuf_setvbuf)
TEST_CASE_SKIP(cstdio, fread_fwrite)
TEST_CASE_SKIP(cstdio, char_and_line_io_file)
TEST_CASE_SKIP(cstdio, stdout_family)
TEST_CASE_SKIP(cstdio, stdin_family)
TEST_CASE_SKIP(cstdio, printf_to_string)
TEST_CASE_SKIP(cstdio, printf_to_file)
TEST_CASE_SKIP(cstdio, scanf_family)
TEST_CASE_SKIP(cstdio, printf_integer)
TEST_CASE_SKIP(cstdio, printf_length_mods)
TEST_CASE_SKIP(cstdio, printf_float)
TEST_CASE_SKIP(cstdio, printf_combinations)
TEST_CASE_SKIP(cstdio, printf_char_string)
TEST_CASE_SKIP(cstdio, sscanf_conversions)
TEST_CASE_SKIP(cstdio, positioning)
TEST_CASE_SKIP(cstdio, error_handling)
#endif

TEST_CASE(cstdio, gap_fill) {

    test_skip("cxx03:gets");
}
