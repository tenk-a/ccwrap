#include "test_cxx.hpp"

#if _TST_HAS_PRINT
#include <print>
#include <cstdio>
#include <ostream>
#include <sstream>
#include <string>
#include <format>

namespace {
STD::string ccw_read_back(STD::FILE* fp) {
    STD::fflush(fp);
    STD::rewind(fp);
    char buf[256];
    STD::size_t n = STD::fread(buf, 1, sizeof buf - 1, fp);
    buf[n] = '\0';
    return STD::string(buf);
}
}

TEST_CASE(print, file_cxx23) {
    STD::FILE* fp = STD::tmpfile();
    test_ptr( fp );
    STD::print(fp, "{}-{}", 1, 2);
    test_true( ccw_read_back(fp) == "1-2" );
    test_pass("cxx23:print(FILE*, format_string, args...)");

    STD::FILE* fp2 = STD::tmpfile();
    test_ptr( fp2 );
    STD::println(fp2, "{}", 7);
    test_true( ccw_read_back(fp2) == "7\n" );
    test_pass("cxx23:println(FILE*, format_string, args...)");

    int v = 42;
    STD::FILE* fp3 = STD::tmpfile();
    test_ptr( fp3 );
    STD::vprint_unicode(fp3, "u{}", STD::make_format_args(v));
    test_true( ccw_read_back(fp3) == "u42" );
    test_pass("cxx23:vprint_unicode(FILE*, string_view, format_args)");

    STD::FILE* fp4 = STD::tmpfile();
    test_ptr( fp4 );
    STD::vprint_nonunicode(fp4, "n{}", STD::make_format_args(v));
    test_true( ccw_read_back(fp4) == "n42" );
    test_pass("cxx23:vprint_nonunicode(FILE*, string_view, format_args)");

    STD::fclose(fp); STD::fclose(fp2); STD::fclose(fp3); STD::fclose(fp4);
}

TEST_CASE(print, ostream_cxx23) {
    STD::ostringstream os;
    STD::print(os, "{}+{}", 3, 4);
    test_true( os.str() == "3+4" );
    test_pass("cxx23:print(ostream&, format_string, args...)");

    STD::ostringstream os2;
    STD::println(os2, "{}", 8);
    test_true( os2.str() == "8\n" );
    test_pass("cxx23:println(ostream&, format_string, args...)");

    int v = 5;
    STD::ostringstream os3;
    STD::vprint_unicode(os3, "u{}", STD::make_format_args(v));
    test_true( os3.str() == "u5" );
    test_pass("cxx23:vprint_unicode(ostream&, string_view, format_args)");

    STD::ostringstream os4;
    STD::vprint_nonunicode(os4, "n{}", STD::make_format_args(v));
    test_true( os4.str() == "n5" );
    test_pass("cxx23:vprint_nonunicode(ostream&, string_view, format_args)");
}

TEST_CASE(print, stdout_cxx23) {
    STD::string empty;
    test_no_throw( STD::print("{}", "") );
    test_pass("cxx23:print(format_string, args...) (stdout)");

    test_no_throw( STD::println("{}", "") );
    test_pass("cxx23:println(format_string, args...) (stdout)");

#if !defined(__cpp_lib_print) || (__cpp_lib_print) < 202406L
    test_no_throw( STD::vprint_unicode("{}", STD::make_format_args(empty)) );
    test_pass("cxx23:vprint_unicode(string_view, format_args) (stdout)");

    test_no_throw( STD::vprint_nonunicode("{}", STD::make_format_args(empty)) );
    test_pass("cxx23:vprint_nonunicode(string_view, format_args) (stdout)");
#else
    TEST_NOTE("P3107R5 replaced the stdout-only vprint_* with vprint_*_buffered");
    test_skip("cxx23:vprint_unicode(string_view, format_args) (stdout)");
    test_skip("cxx23:vprint_nonunicode(string_view, format_args) (stdout)");
#endif
}

TEST_CASE(print, println_no_args_cxx26) {
    STD::FILE* fp = STD::tmpfile();
    test_ptr( fp );
    STD::println(fp);
    test_true( ccw_read_back(fp) == "\n" );
    test_pass("cxx26:println(FILE*) (no args)");
    STD::fclose(fp);

    STD::ostringstream os;
    STD::println(os);
    test_true( os.str() == "\n" );
    test_pass("cxx26:println(ostream&) (no args)");

    test_no_throw( STD::println() );
    test_pass("cxx26:println() (no args, stdout)");
}

#else
TEST_CASE(print, file_cxx23) {
    test_skip("cxx23:print(FILE*, format_string, args...)");
    test_skip("cxx23:println(FILE*, format_string, args...)");
    test_skip("cxx23:vprint_unicode(FILE*, string_view, format_args)");
    test_skip("cxx23:vprint_nonunicode(FILE*, string_view, format_args)");
}
TEST_CASE(print, ostream_cxx23) {
    test_skip("cxx23:print(ostream&, format_string, args...)");
    test_skip("cxx23:println(ostream&, format_string, args...)");
    test_skip("cxx23:vprint_unicode(ostream&, string_view, format_args)");
    test_skip("cxx23:vprint_nonunicode(ostream&, string_view, format_args)");
}
TEST_CASE(print, stdout_cxx23) {
    test_skip("cxx23:print(format_string, args...) (stdout)");
    test_skip("cxx23:println(format_string, args...) (stdout)");
    test_skip("cxx23:vprint_unicode(string_view, format_args) (stdout)");
    test_skip("cxx23:vprint_nonunicode(string_view, format_args) (stdout)");
}
TEST_CASE(print, println_no_args_cxx26) {
    test_skip("cxx26:println(FILE*) (no args)");
    test_skip("cxx26:println(ostream&) (no args)");
    test_skip("cxx26:println() (no args, stdout)");
}
#endif

TEST_CASE(print, feature_test_macros) {
#if defined(__cpp_lib_print) && (__cpp_lib_print) >= 202207L
    test_true( (__cpp_lib_print) >= 202207L );
    test_pass("cxx23:__cpp_lib_print=202207L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_print=202207L");
#endif
#if defined(__cpp_lib_print) && (__cpp_lib_print) >= 202403L
    test_true( (__cpp_lib_print) >= 202403L );
    test_pass("cxx26:__cpp_lib_print=202403L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_print=202403L");
#endif
}


#if defined(__cpp_lib_print) && __cpp_lib_print >= 202406L
namespace {
STD::string ccw_print_read_back(STD::FILE* fp) {
    STD::fflush(fp); STD::rewind(fp);
    char buf[256];
    STD::size_t n = STD::fread(buf, 1, sizeof buf - 1, fp);
    return STD::string(buf, buf + n);
}
}
#endif
TEST_CASE(print, buffered_and_locking_cxx26) {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202406L
    int v = 9;
    STD::FILE* fp = STD::tmpfile();
    test_ptr( fp );
    STD::vprint_unicode_buffered(fp, "b{}", STD::make_format_args(v));
    test_true( ccw_print_read_back(fp) == "b9" );
    test_pass("cxx26:vprint_unicode_buffered(FILE*,string_view,format_args)");
    STD::fclose(fp);

    STD::FILE* fp2 = STD::tmpfile();
    test_ptr( fp2 );
    STD::vprint_nonunicode_buffered(fp2, "c{}", STD::make_format_args(v));
    test_true( ccw_print_read_back(fp2) == "c9" );
    test_pass("cxx26:vprint_nonunicode_buffered(FILE*,string_view,format_args)");
    STD::fclose(fp2);

    STD::string empty;
    test_no_throw( STD::vprint_unicode_buffered("{}", STD::make_format_args(empty)) );
    test_pass("cxx26:vprint_unicode_buffered(string_view,format_args)");
    test_no_throw( STD::vprint_nonunicode_buffered("{}", STD::make_format_args(empty)) );
    test_pass("cxx26:vprint_nonunicode_buffered(string_view,format_args)");
#else
    TEST_NOTE("the buffered vprint forms are C++26 (P3107); not in this library");
    test_skip("cxx26:vprint_unicode_buffered(FILE*,string_view,format_args)");
    test_skip("cxx26:vprint_nonunicode_buffered(FILE*,string_view,format_args)");
    test_skip("cxx26:vprint_unicode_buffered(string_view,format_args)");
    test_skip("cxx26:vprint_nonunicode_buffered(string_view,format_args)");
#endif

#if defined(_TST_HAS_VPRINT_LOCKING)
    STD::FILE* fp3 = STD::tmpfile();
    test_ptr( fp3 );
    STD::vprint_unicode_locking(fp3, "d{}", STD::make_format_args(v));
    test_true( ccw_print_read_back(fp3) == "d9" );
    test_pass("cxx26:vprint_unicode_locking(FILE*,string_view,format_args)");
    STD::fclose(fp3);

    STD::FILE* fp4 = STD::tmpfile();
    test_ptr( fp4 );
    STD::vprint_nonunicode_locking(fp4, "e{}", STD::make_format_args(v));
    test_true( ccw_print_read_back(fp4) == "e9" );
    test_pass("cxx26:vprint_nonunicode_locking(FILE*,string_view,format_args)");
    STD::fclose(fp4);
#else
    TEST_NOTE("vprint_*_locking is in no [print.syn]; P3107R5 renamed nothing");
    test_skip("cxx26:vprint_unicode_locking(FILE*,string_view,format_args)");
    test_skip("cxx26:vprint_nonunicode_locking(FILE*,string_view,format_args)");
#endif
}
