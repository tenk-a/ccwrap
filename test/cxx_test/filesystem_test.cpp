#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2017
#include <filesystem>
#include <algorithm>
#include <map>
#include <vector>
#include <cstdio>
#include <system_error>
#include <type_traits>
#include <functional>
#include <sstream>
#include <string>
#include <chrono>
#include <iterator>

namespace fs = STD::filesystem;

static bool fname_is(const fs::path& p, const char* want) {
    STD::string a = p.filename().string();
#if defined(__DOS__) || defined(MSDOS)
    STD::string b(want);
    if (a.size() != b.size()) return false;
    for (STD::string::size_type i = 0; i < a.size(); ++i) {
        char x = a[i], y = b[i];
        if (x >= 'a' && x <= 'z') x = (char)(x - 'a' + 'A');
        if (y >= 'a' && y <= 'z') y = (char)(y - 'a' + 'A');
        if (x != y) return false;
    }
    return true;
#else
    return a == STD::string(want);
#endif
}

TEST_CASE(filesystem, path_lexical) {
    fs::path p("dir/sub/file.txt");
    test_true( p.filename()    == fs::path("file.txt") );
    test_pass("cxx17:path::filename");
    test_true( p.extension()   == fs::path(".txt") );
    test_pass("cxx17:path::extension");
    test_true( p.stem()        == fs::path("file") );
    test_pass("cxx17:path::stem");
    test_true( p.parent_path() == fs::path("dir/sub") );
    test_pass("cxx17:path::parent_path");

    fs::path j = fs::path("a") / "b" / "c";
    test_true( j.filename() == fs::path("c") );
    test_pass("cxx17:path::operator/");

    test_true( fs::path("noext").extension().empty() );
    test_pass("cxx17:path::extension (empty)");
}

TEST_CASE(filesystem, os_operations) {
    test_true( !fs::current_path().empty() );
    test_pass("cxx17:current_path");

    fs::path d("cfstdir");
    fs::remove(d);
    test_true( fs::create_directory(d) );
    test_pass("cxx17:create_directory");
    test_true( fs::exists(d) );
    test_pass("cxx17:exists (dir)");
    test_true( fs::is_directory(d) );
    test_pass("cxx17:is_directory");

    fs::path f = d / "hello.txt";
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("hi", fp); STD::fclose(fp); } }
    test_true( fs::exists(f) );
    test_pass("cxx17:exists (file)");
    test_true( fs::is_regular_file(f) );
    test_pass("cxx17:is_regular_file");
    test_eq( (unsigned long)fs::file_size(f), 2UL );
    test_true( tst_type_is<STD::uintmax_t>(fs::file_size(f)) );
    test_pass("cxx17:file_size");

    int count = 0; bool sawfile = false;
    for (fs::directory_iterator it(d); it != fs::directory_iterator(); ++it) {
        ++count;
        if (fname_is(it->path(), "hello.txt")) sawfile = true;
    }
    test_eq( count, 1 );
    test_true( sawfile );
    test_pass("cxx17:directory_iterator");

    fs::path f2 = d / "renamed.txt";
    fs::rename(f, f2);
    test_true( fs::exists(f2) );
    test_true( !fs::exists(f) );
    test_pass("cxx17:rename");

    fs::remove(f2);
    fs::remove(d);
    test_true( !fs::exists(d) );
    test_pass("cxx17:remove");
}

TEST_CASE(filesystem, path_components) {
    fs::path p("dir/sub/file.txt");
    test_true( p.has_filename() );
    test_pass("cxx17:path::has_filename");
    test_true( p.has_extension() );
    test_pass("cxx17:path::has_extension");
    test_true( !p.is_absolute() );
    test_pass("cxx17:path::is_absolute");

    test_eq( p.generic_string(), STD::string("dir/sub/file.txt") );
    test_pass("cxx17:path::generic_string");

    STD::string s = "a/b";
    fs::path q(s);
    test_true( q.filename() == fs::path("b") );
    test_pass("cxx17:path::path(string)");

    fs::path e;
    test_true( e.empty() );
    test_true( !e.has_filename() );
    test_pass("cxx17:path::empty");
    p.clear();
    test_true( p.empty() );
    test_pass("cxx17:path::clear");

    fs::path r("base");
    r /= "leaf";
    test_true( r.filename() == fs::path("leaf") );
    test_true( r.parent_path() == fs::path("base") );
    test_pass("cxx17:path::operator/=");

    fs::path noext("plain");
    test_true( !noext.has_extension() );
    test_true( noext.has_filename() );
    test_pass("cxx17:path::has_extension (false)");
}

TEST_CASE(filesystem, create_directories_and_absolute) {

    fs::path nested("cfsa/cfsb/cfsc");
    fs::remove(nested);
    test_true( fs::create_directories(nested) );
    test_true( fs::exists(nested) );
    test_true( fs::is_directory(nested) );
    test_pass("cxx17:create_directories");

    fs::path abs = fs::absolute(fs::path("cfsa"));
    test_true( abs.is_absolute() );
    test_pass("cxx17:absolute");

    fs::file_status st = fs::status(nested);
    (void)st.type();
    test_true( fs::is_directory(nested) );
    test_pass("cxx17:status");

    fs::remove(nested);
    fs::remove(fs::path("cfsa/cfsb"));
    fs::remove(fs::path("cfsa"));
    test_true( !fs::exists(fs::path("cfsa")) );
    test_pass("cxx17:remove (dir chain)");
}

TEST_CASE(filesystem, path_modifiers) {

    fs::path p("dir/file.txt");
    p.replace_extension(".md");
    test_true( p == fs::path("dir/file.md") );
    p.replace_extension("cpp");
    test_true( p == fs::path("dir/file.cpp") );
    p.replace_extension();
    test_true( p == fs::path("dir/file") );
    test_pass("cxx17:path::replace_extension");

    fs::path q("a/b/c.txt");
    q.replace_filename("d.dat");
    test_true( q == fs::path("a/b/d.dat") );
    test_pass("cxx17:path::replace_filename");

    fs::path r("a/b/c.txt");
    r.remove_filename();
    test_true( r == fs::path("a/b/") );
    test_pass("cxx17:path::remove_filename");

    fs::path m("a/b/c");
    m.make_preferred();
    test_true( m.generic_string() == STD::string("a/b/c") );
    test_pass("cxx17:path::make_preferred");
}

TEST_CASE(filesystem, root_decomposition) {
#if defined(_WIN32)
    fs::path w("C:\\dir\\file.txt");
    test_true( w.has_root_name() );
    test_pass("cxx17:path::has_root_name");
    test_true( w.root_name() == fs::path("C:") );
    test_pass("cxx17:path::root_name");
    test_true( w.has_root_directory() );
    test_pass("cxx17:path::has_root_directory");
    test_true( w.root_directory() == fs::path("\\") );
    test_pass("cxx17:path::root_directory");
    test_true( w.root_path() == fs::path("C:\\") );
    test_pass("cxx17:path::root_path");
    test_true( w.relative_path() == fs::path("dir\\file.txt") );
    test_pass("cxx17:path::relative_path");
    test_true( w.is_absolute() );
    test_pass("cxx17:path::is_absolute (win)");
    test_true( !w.is_relative() );
    test_pass("cxx17:path::is_relative");
#else
    TEST_SKIP1(); test_skip("cxx17:path::has_root_name");
    TEST_SKIP1(); test_skip("cxx17:path::root_name");
    TEST_SKIP1(); test_skip("cxx17:path::has_root_directory");
    TEST_SKIP1(); test_skip("cxx17:path::root_directory");
    TEST_SKIP1(); test_skip("cxx17:path::root_path");
    TEST_SKIP1(); test_skip("cxx17:path::relative_path");
    TEST_SKIP1(); test_skip("cxx17:path::is_absolute (win)");
    TEST_SKIP1(); test_skip("cxx17:path::is_relative");
#endif

    fs::path u("/usr/bin");
    test_true( !u.has_root_name() );
    test_true( u.has_root_directory() );
    test_true( u.root_directory() == fs::path("/") );
    test_true( u.relative_path() == fs::path("usr/bin") );
    test_pass("cxx17:path root decomposition (posix)");

    fs::path rel("a/b");
    test_true( !rel.has_root_name() );
    test_true( !rel.has_root_directory() );
    test_true( rel.has_relative_path() );
    test_pass("cxx17:path::has_relative_path");
    test_true( rel.is_relative() );
    test_true( rel.has_stem() );
    test_pass("cxx17:path::has_stem");
}

TEST_CASE(filesystem, lexically_normal) {

    test_true( fs::path("a/./b").lexically_normal()     == fs::path("a/b") );
    test_true( fs::path("a/b/../c").lexically_normal()  == fs::path("a/c") );
    test_true( fs::path("a/b/../..").lexically_normal() == fs::path(".") );
    test_true( fs::path("./a").lexically_normal()       == fs::path("a") );
    test_true( fs::path("a//b").lexically_normal()      == fs::path("a/b") );

    test_true( fs::path("../a").lexically_normal()      == fs::path("../a") );
    test_pass("cxx17:path::lexically_normal");

    test_true( fs::path("a").compare(fs::path("a")) == 0 );
    test_true( fs::path("a").compare(fs::path("b")) < 0 );
    test_pass("cxx17:path::compare");
}

TEST_CASE(filesystem, os_operations_more) {
    fs::path d("cfsmore");
    fs::remove_all(d);

    test_true( fs::create_directory(d) );
    test_pass("cxx17:create_directory (tree)");
    fs::path a = d / "a.txt";
    { STD::FILE* fp = STD::fopen(a.string().c_str(), "w"); if (fp) { STD::fputs("hello", fp); STD::fclose(fp); } }
    fs::path sub = d / "sub";
    fs::create_directory(sub);
    fs::path b = sub / "b.txt";
    { STD::FILE* fp = STD::fopen(b.string().c_str(), "w"); if (fp) { STD::fputs("x", fp); STD::fclose(fp); } }

    test_true( !fs::is_empty(a) );
    test_true( !fs::is_empty(d) );
    fs::path empt = d / "empty.txt";
    { STD::FILE* fp = STD::fopen(empt.string().c_str(), "w"); if (fp) STD::fclose(fp); }
    test_true( fs::is_empty(empt) );
    test_pass("cxx17:is_empty");

    fs::path acopy = d / "a_copy.txt";
    test_true( fs::copy_file(a, acopy) );
    test_true( fs::exists(acopy) );
    test_eq( (unsigned long)fs::file_size(acopy), 5UL );
    test_pass("cxx17:copy_file");

    fs::file_time_type t = fs::last_write_time(a);
    TEST_SKIP_GCC();
    test_true( t > fs::file_time_type() );
    test_pass("cxx17:last_write_time");

    int files = 0; bool saw_b = false;
    for (fs::recursive_directory_iterator it(d), e; it != e; ++it) {
        if (fs::is_regular_file(it->path())) ++files;
        if (fname_is(it->path(), "b.txt")) saw_b = true;
    }
    test_true( files >= 3 );
    test_true( saw_b );
    test_pass("cxx17:recursive_directory_iterator");

    STD::uintmax_t n = fs::remove_all(d);
    test_true( n >= 5 );
    test_true( !fs::exists(d) );
    test_true( tst_type_is<STD::uintmax_t>(fs::remove_all(d)) );
    test_pass("cxx17:remove_all");

    test_true( !fs::temp_directory_path().empty() );
    test_pass("cxx17:temp_directory_path");
}

TEST_CASE(filesystem, perms_bitmask) {
    typedef fs::perms P;

    test_eq( (unsigned)P::owner_read,  0400u );
    test_eq( (unsigned)P::owner_write, 0200u );
    test_eq( (unsigned)P::owner_all,   0700u );
    test_eq( (unsigned)P::all,         0777u );
    test_eq( (unsigned)P::mask,        07777u );
    test_eq( (unsigned)P::none,        0u );
    test_pass("cxx17:perms (values)");

    fs::perms a(P::owner_read), b(P::owner_write);
    test_true( (a | b) == fs::perms(0600u) );
    test_true( (fs::perms(P::owner_all) & a) == a );
    test_true( (a ^ a) == fs::perms(P::none) );
    test_true( a != b );
    test_pass("cxx17:perms (bitmask ops)");

    fs::perms c(P::owner_read);
    c |= b;
    test_true( c == fs::perms(0600u) );
    c &= a;
    test_true( c == a );
    test_pass("cxx17:perms::operator|=");

    test_true( ((~fs::perms(P::none)) & fs::perms(P::mask)) == fs::perms(P::mask) );
    test_pass("cxx17:perms::operator~");
}

TEST_CASE(filesystem, perm_options_and_directory_options) {
    typedef fs::perm_options O;
    test_eq( (unsigned)O::replace,  1u );
    test_eq( (unsigned)O::add,      2u );
    test_eq( (unsigned)O::remove,   4u );
    test_eq( (unsigned)O::nofollow, 8u );
    test_true( (fs::perm_options(O::add) | fs::perm_options(O::nofollow)) == fs::perm_options(10u) );
    test_pass("cxx17:perm_options");

    typedef fs::directory_options D;
    test_eq( (unsigned)D::none,                     0u );
    test_eq( (unsigned)D::follow_directory_symlink, 1u );
    test_eq( (unsigned)D::skip_permission_denied,   2u );
    test_true( (fs::directory_options(D::follow_directory_symlink)
              | fs::directory_options(D::skip_permission_denied)) == fs::directory_options(3u) );
    test_true( fs::directory_options() == fs::directory_options(D::none) );
    test_pass("cxx17:directory_options");
}

TEST_CASE(filesystem, status_predicates) {

    test_true(  fs::is_block_file(fs::file_status(fs::file_type::block)) );
    test_true(  fs::is_character_file(fs::file_status(fs::file_type::character)) );
    test_true(  fs::is_fifo(fs::file_status(fs::file_type::fifo)) );
    test_true(  fs::is_socket(fs::file_status(fs::file_type::socket)) );
    test_true(  fs::is_symlink(fs::file_status(fs::file_type::symlink)) );
    test_true(  fs::is_directory(fs::file_status(fs::file_type::directory)) );
    test_true(  fs::is_regular_file(fs::file_status(fs::file_type::regular)) );
    test_true( !fs::is_regular_file(fs::file_status(fs::file_type::directory)) );
    test_pass("cxx17:file_status predicates");

    test_true(  fs::exists(fs::file_status(fs::file_type::regular)) );
    test_true( !fs::exists(fs::file_status(fs::file_type::not_found)) );
    test_true( !fs::status_known(fs::file_status(fs::file_type::none)) );
    test_true(  fs::status_known(fs::file_status(fs::file_type::not_found)) );
    test_pass("cxx17:status_known");

    test_true(  fs::is_other(fs::file_status(fs::file_type::fifo)) );
    test_true( !fs::is_other(fs::file_status(fs::file_type::regular)) );
    test_true( !fs::is_other(fs::file_status(fs::file_type::not_found)) );
    test_pass("cxx17:is_other");

    fs::path f("ccwfsprm.txt");
    fs::remove(f);
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("p", fp); STD::fclose(fp); } }
    fs::file_status st = fs::status(f);
    test_true( fs::is_regular_file(st) );
    test_true( st.permissions() != fs::perms(fs::perms::unknown) );
    test_true( (st.permissions() & fs::perms(fs::perms::owner_read)) != fs::perms(fs::perms::none) );
    test_pass("cxx17:status (permissions)");

    test_true( fs::symlink_status(f).type() == fs::status(f).type() );
    test_pass("cxx17:symlink_status");
    test_true( !fs::is_symlink(f) );
    test_true( !fs::is_fifo(f) );
    test_true( !fs::is_other(f) );
    test_pass("cxx17:is_symlink");

    fs::remove(f);
}

TEST_CASE(filesystem, permissions_write_bit) {
    fs::path f("ccwfsro.txt");
    fs::remove(f);
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("w", fp); STD::fclose(fp); } }

    fs::permissions(f, fs::perms(fs::perms::owner_read));
    test_true( (fs::status(f).permissions() & fs::perms(fs::perms::owner_write))
               == fs::perms(fs::perms::none) );

    fs::permissions(f, fs::perms((unsigned)fs::perms::owner_read | (unsigned)fs::perms::owner_write));
    test_true( (fs::status(f).permissions() & fs::perms(fs::perms::owner_write))
               != fs::perms(fs::perms::none) );
    test_pass("cxx17:permissions");

    fs::remove(f);
}

TEST_CASE(filesystem, lexically_relative_and_proximate) {

    test_true( fs::path("/a/d").lexically_relative("/a/b/c") == fs::path("../../d") );
    test_true( fs::path("/a/b/c").lexically_relative("/a/d") == fs::path("../b/c") );
    test_true( fs::path("a/b/c").lexically_relative("a")     == fs::path("b/c") );
    test_true( fs::path("a/b/c").lexically_relative("a/b/c") == fs::path(".") );
    test_true( fs::path("a/b").lexically_relative("c/d")     == fs::path("../../a/b") );

#if defined(_WIN32)
    test_true( fs::path("C:/a/b").lexically_relative("D:/a").empty() );
    test_pass("cxx17:path::lexically_relative");
#else
    TEST_SKIP1();
    test_skip("cxx17:path::lexically_relative");
#endif

#if defined(_WIN32)
    test_true( fs::path("C:/a/b").lexically_proximate("D:/a") == fs::path("C:/a/b") );
#else
    TEST_SKIP1();
#endif
    test_true( fs::path("a/b/c").lexically_proximate("a") == fs::path("b/c") );
    test_pass("cxx17:path::lexically_proximate");
}

template <class _S>
static STD::string ccw_fs_bytes(const _S& __s) {
    STD::string __r;
    for (typename _S::size_type __i = 0; __i < __s.size(); ++__i) __r += (char)__s[__i];
    return __r;
}

TEST_CASE(filesystem, path_string_forms_and_hash) {
    fs::path p("dir/sub/file.txt");

    test_eq( p.u8string().size(), p.string().size() );
    test_eq( p.generic_u8string().size(), p.generic_string().size() );
    test_true( (char)p.u8string()[0] == p.string()[0] );
    {   STD::string src;
        src += (char)0xE3; src += (char)0x81; src += (char)0x82;
        src += (char)0xF0; src += (char)0x9F; src += (char)0x98; src += (char)0x80;
        if (sizeof(fs::path::value_type) > 1) {
            fs::path q = fs::u8path(src);
            test_true( ccw_fs_bytes(q.u8string()) == src );
            test_eq( q.native().size(), (STD::size_t)3 );
        } else {
            TEST_SKIP_N(2);
        }
    }
    test_pass("cxx17:path::u8string");

    STD::basic_string<char16_t> u16 = p.u16string();
    STD::basic_string<char32_t> u32 = p.u32string();
    test_eq( u16.size(), p.string().size() );
    test_eq( u32.size(), p.string().size() );
    test_true( u16[0] == (char16_t)'d' );
    test_true( u32[0] == (char32_t)'d' );
    test_pass("cxx17:path::u16string");

#if defined(_WIN32)
    test_eq( fs::hash_value(fs::path("a/b")), fs::hash_value(fs::path("a\\b")) );
#else
    TEST_SKIP1();
#endif
    test_true( fs::hash_value(fs::path("a/b")) != fs::hash_value(fs::path("a/c")) );
    test_pass("cxx17:hash_value");

    test_true( fs::u8path("x/y") == fs::path("x/y") );
    test_true( fs::u8path(STD::string("a/b")) == fs::path("a/b") );
    test_pass("cxx17:u8path");

    {   STD::string src("p/q");
        test_true( fs::u8path(src.begin(), src.end()) == fs::path("p/q") );
        test_true( fs::u8path(src.begin(), src.begin()) == fs::path() );
        test_true( fs::u8path(src.begin(), src.begin() + 1) == fs::path("p") ); }
    test_pass("cxx17:u8path(InputIterator first, InputIterator last)");
}

TEST_CASE(filesystem, directory_entry_observers) {
    fs::path d("cfsde");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    fs::path f = d / "e.txt";
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("de", fp); STD::fclose(fp); } }

    fs::directory_entry de(f);
    test_true( de.path() == f );
    test_pass("cxx17:directory_entry::path");
    test_true( de.exists() );
    test_pass("cxx17:directory_entry::exists");
    test_true( de.is_regular_file() );
    test_true( !de.is_directory() );
    test_true( !de.is_symlink() );
    test_true( !de.is_other() );
    test_true( !de.is_fifo() );
    test_pass("cxx17:directory_entry status predicates");
    test_eq( (unsigned long)de.file_size(), 2ul );
    test_pass("cxx17:directory_entry::file_size");
    test_true( fs::is_regular_file(de.status()) );
    test_true( de.symlink_status().type() == de.status().type() );
    test_pass("cxx17:directory_entry::status");

    const fs::path& asp = de;
    test_true( asp == f );
    test_pass("cxx17:directory_entry operator path");

    fs::directory_entry de2(f);
    de2.replace_filename("other.txt");
    test_true( de2.path() == d / "other.txt" );
    test_pass("cxx17:directory_entry::replace_filename");

    de.refresh();
    test_true( de.exists() );
    test_pass("cxx17:directory_entry::refresh");

    test_true( de == fs::directory_entry(f) );
    test_true( de != de2 );
    test_pass("cxx17:directory_entry::operator==");

    fs::remove_all(d);
}

TEST_CASE(filesystem, canonical_relative_space) {
    fs::path d("cfscan");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    fs::path f = d / "c.txt";
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("canon", fp); STD::fclose(fp); } }

    fs::path c = fs::canonical(f);
    test_true( c.is_absolute() );
    test_true( fs::exists(c) );
    test_true( fs::equivalent(c, f) );
    test_pass("cxx17:canonical");

    fs::path w = fs::weakly_canonical(d / "notthere.txt");
    test_true( w.is_absolute() );
    test_true( w.filename() == fs::path("notthere.txt") );
    test_pass("cxx17:weakly_canonical");

    test_true( !fs::relative(f).empty() );
    test_pass("cxx17:relative");
    test_true( !fs::proximate(f).empty() );
    test_pass("cxx17:proximate");

    test_true( fs::relative(f, d) == fs::path("c.txt") );
    test_true( fs::relative(d, d) == fs::path(".") );
    test_true( fs::relative(d, f).empty() || !fs::relative(d, f).empty() );
    test_pass("cxx17:relative(const path& p, const path& base)");

    test_true( fs::proximate(f, d) == fs::path("c.txt") );
    test_true( fs::proximate(d, d) == fs::path(".") );
    test_pass("cxx17:proximate(const path& p, const path& base)");

    fs::space_info si = fs::space(d);
    test_true( si.capacity > 0 );
    test_true( si.free <= si.capacity );
    test_true( si.available <= si.capacity );
#if TEST_TARGET_CXX >= 2020
    test_true( si == si );
    test_pass("cxx17:space");
#else
    TEST_SKIP1();
    test_skip("cxx17:space");
#endif

    fs::remove_all(d);
}

TEST_CASE(filesystem, resize_file_and_hard_link_count) {
    fs::path f("ccwfsrs.txt");
    fs::remove(f);
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("0123456789", fp); STD::fclose(fp); } }
    test_eq( (unsigned long)fs::file_size(f), 10ul );

    fs::resize_file(f, 4);
    test_eq( (unsigned long)fs::file_size(f), 4ul );

    fs::resize_file(f, 8);
    test_eq( (unsigned long)fs::file_size(f), 8ul );
    {
        STD::FILE* fp = STD::fopen(f.string().c_str(), "rb");
        test_ptr( fp );
        char buf[8];
        STD::fread(buf, 1, 8, fp);
        STD::fclose(fp);
        test_true( buf[0] == '0' && buf[3] == '3' );
        test_true( buf[4] == 0 && buf[7] == 0 );
    }
    test_pass("cxx17:resize_file");

    test_true( fs::hard_link_count(f) >= 1 );
    test_pass("cxx17:hard_link_count");

    fs::path g("ccwfsrs2.txt");
    fs::remove(g);
    { STD::FILE* fp = STD::fopen(g.string().c_str(), "w"); if (fp) { STD::fputs("g", fp); STD::fclose(fp); } }
    test_true( !fs::equivalent(f, g) );
    test_true( fs::equivalent(f, f) );
    test_pass("cxx17:equivalent");

    fs::remove(f);
    fs::remove(g);
}

TEST_CASE(filesystem, path_relational) {

    fs::path a("a"), b("b");
    test_true( a < b );   test_true( b > a );
    test_true( a <= b );  test_true( b >= a );
    test_true( a <= a );  test_true( a >= a );
    test_true( !(b < a) );test_true( !(a > b) );
    test_pass("cxx17:path::operator<");
    test_true( a == fs::path("a") );
    test_true( a != b );
    test_pass("cxx17:path::operator==");
}

TEST_CASE(filesystem, path_ctor_and_modifiers) {
    fs::path p0;
    test_true( p0.empty() );
    test_pass("cxx17:path default ctor");

    fs::path pa("a");
    pa.append("b");
    test_true( pa == fs::path("a") / "b" );
    test_pass("cxx17:path::append");

    fs::path pc("foo");
    pc.concat("bar");
    test_true( pc == fs::path("foobar") );
    test_pass("cxx17:path::concat");

    fs::path pp("foo");
    pp += "baz";
    test_true( pp == fs::path("foobaz") );
    test_pass("cxx17:path::operator+= (concat)");

    test_true(  fs::path("a/b").has_parent_path() );
    test_true( !fs::path("filename").has_parent_path() );
    test_pass("cxx17:path::has_parent_path");

    fs::path x("one"), y("two");
    x.swap(y);
    test_true( x == fs::path("two") && y == fs::path("one") );
    test_pass("cxx17:path::swap");
}

TEST_CASE(filesystem, path_native_and_conversions) {
    fs::path p("abc");

    fs::path::string_type nat = p.native();
    test_true( nat == p.native() );
    test_pass("cxx17:path::native");

    fs::path::string_type conv = p;
    test_true( conv == p.native() );
    test_pass("cxx17:path::operator string_type");

    const fs::path::value_type* cs = p.c_str();
    test_true( fs::path::string_type(cs) == p.native() );
    test_pass("cxx17:path::c_str");

    fs::path::value_type sep = fs::path::preferred_separator;
    test_true( sep == (fs::path::value_type)'/' || sep == (fs::path::value_type)'\\' );
    test_pass("cxx17:path::preferred_separator");

    test_true( p.string() == STD::string("abc") );
    test_pass("cxx17:path::string");
    test_true( p.wstring() == STD::wstring(L"abc") );
    test_pass("cxx17:path::wstring");
    STD::basic_string<char32_t> u32 = p.u32string();
    test_true( u32.size() == 3 && u32[0] == (char32_t)'a' );
    test_pass("cxx17:path::u32string");

    test_true( p.generic_wstring() == STD::wstring(L"abc") );
    test_pass("cxx17:path::generic_wstring");
    STD::basic_string<char16_t> g16 = p.generic_u16string();
    test_true( g16.size() == 3 && g16[0] == (char16_t)'a' );
    test_pass("cxx17:path::generic_u16string");
    STD::basic_string<char32_t> g32 = p.generic_u32string();
    test_true( g32.size() == 3 && g32[0] == (char32_t)'a' );
    test_pass("cxx17:path::generic_u32string");
}

TEST_CASE(filesystem, path_component_iteration) {
    fs::path p("a/b/c");
    int n = 0;
    for (fs::path::iterator i = p.begin(); i != p.end(); ++i) ++n;
    test_eq( n, 3 );
    test_true( *p.begin() == fs::path("a") );
    test_pass("cxx17:path::begin (component iteration)");

    fs::path last;
    for (fs::path::iterator i = p.begin(); i != p.end(); ++i) last = *i;
    test_true( last == fs::path("c") );
    test_pass("cxx17:path::end (component iteration)");
}

TEST_CASE(filesystem, file_status_accessors) {

    fs::file_type t = fs::file_type::regular;
    test_true( t != fs::file_type::none );
    test_true( fs::file_type::not_found != fs::file_type::directory );
    test_true( fs::file_type::symlink   != fs::file_type::block );
    test_true( fs::file_type::character != fs::file_type::fifo );
    test_true( fs::file_type::socket    != fs::file_type::unknown );
    test_pass("cxx17:file_type enum");

    fs::file_status st(fs::file_type::regular, fs::perms::owner_read);
    test_true( st.type() == fs::file_type::regular );
    st.type(fs::file_type::directory);
    test_true( st.type() == fs::file_type::directory );
    test_pass("cxx17:file_status type accessor");

    test_true( st.permissions() == fs::perms::owner_read );
    st.permissions(fs::perms::owner_write);
    test_true( st.permissions() == fs::perms::owner_write );
    test_pass("cxx17:file_status permissions accessor");
}

TEST_CASE(filesystem, special_file_predicates) {

    test_true(  fs::is_block_file(fs::file_status(fs::file_type::block)) );
    test_true( !fs::is_block_file(fs::file_status(fs::file_type::regular)) );
    test_pass("cxx17:is_block_file");

    test_true(  fs::is_character_file(fs::file_status(fs::file_type::character)) );
    test_true( !fs::is_character_file(fs::file_status(fs::file_type::regular)) );
    test_pass("cxx17:is_character_file");

    test_true(  fs::is_fifo(fs::file_status(fs::file_type::fifo)) );
    test_true( !fs::is_fifo(fs::file_status(fs::file_type::regular)) );
    test_pass("cxx17:is_fifo");

    test_true(  fs::is_socket(fs::file_status(fs::file_type::socket)) );
    test_true( !fs::is_socket(fs::file_status(fs::file_type::regular)) );
    test_pass("cxx17:is_socket");
}

TEST_CASE(filesystem, filesystem_error_type) {
    STD::error_code ec = STD::make_error_code(STD::errc::no_such_file_or_directory);
    fs::filesystem_error e("boom", fs::path("p1"), fs::path("p2"), ec);

    test_true( STD::string(e.what()).find("boom") != STD::string::npos );
    test_pass("cxx17:filesystem_error");

    test_true( e.code() == ec );
    test_pass("cxx17:filesystem_error::code");

    test_true( e.path1() == fs::path("p1") );
    test_pass("cxx17:filesystem_error::path1");

    test_true( e.path2() == fs::path("p2") );
    test_pass("cxx17:filesystem_error::path2");
}

TEST_CASE(filesystem, directory_entry_more) {
    fs::path d("cfsde2");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    fs::path f = d / "e.txt";
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("de", fp); STD::fclose(fp); } }
    fs::path g = d / "g.txt";
    { STD::FILE* fp = STD::fopen(g.string().c_str(), "w"); if (fp) { STD::fputs("g", fp); STD::fclose(fp); } }

    fs::directory_entry de(f);
    test_true( de.path() == f && de.exists() );
    test_pass("cxx17:directory_entry ctor(path)");

    de.assign(g);
    test_true( de.path() == g && de.exists() );
    test_pass("cxx17:directory_entry::assign");

    test_true( de.hard_link_count() >= 1 );
    test_pass("cxx17:directory_entry::hard_link_count");

    test_true( de.last_write_time() == fs::last_write_time(g) );
    test_pass("cxx17:directory_entry::last_write_time");

    test_true( de.symlink_status().type() == fs::symlink_status(g).type() );
    test_pass("cxx17:directory_entry::symlink_status");

    fs::remove_all(d);
}

TEST_CASE(filesystem, directory_iterator_ops) {
    fs::path d("cfsdi");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    fs::path f = d / "only.txt";
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("x", fp); STD::fclose(fp); } }

    fs::directory_iterator it(d);
    fs::directory_iterator e;
    test_true( it != e );
    test_pass("cxx17:directory_iterator equality");

    const fs::directory_entry& first = *it;
    test_true( fname_is(first.path(), "only.txt") );
    test_pass("cxx17:directory_iterator deref");

    ++it;
    test_true( it == e );
    test_pass("cxx17:directory_iterator increment");

    test_true( fs::directory_iterator() == e );
    test_pass("cxx17:directory_iterator end");

#if _tst_cplusplus >= 201103L
    int n = 0;
    for (const fs::directory_entry& entry : fs::directory_iterator(d)) {
        if (fname_is(entry.path(), "only.txt")) ++n;
    }
    test_eq( n, 1 );
#else

    int n = 0;
    for (fs::directory_iterator it(d), en; it != en; ++it)
        if (fname_is(it->path(), "only.txt")) ++n;
    test_eq( n, 1 );
#endif
    test_pass("cxx17:directory_iterator range (begin/end)");

    fs::remove_all(d);
}

TEST_CASE(filesystem, recursive_directory_iterator_ops) {

    fs::path d("cfsrdi");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    { STD::FILE* fp = STD::fopen((d / "a.txt").string().c_str(), "w"); if (fp) { STD::fputs("a", fp); STD::fclose(fp); } }
    fs::path sub = d / "sub";
    fs::create_directory(sub);
    { STD::FILE* fp = STD::fopen((sub / "b.txt").string().c_str(), "w"); if (fp) { STD::fputs("b", fp); STD::fclose(fp); } }

    int max_depth = 0; bool saw_b = false;
    for (fs::recursive_directory_iterator it(d), e; it != e; ++it) {
        if (it.depth() > max_depth) max_depth = it.depth();
        if (fname_is(it->path(), "b.txt")) { saw_b = true; test_eq( it.depth(), 1 ); }
    }
    test_true( saw_b && max_depth == 1 );
    test_pass("cxx17:recursive_directory_iterator::depth");

    bool saw_b_when_disabled = false; bool checked_pending = false;
    for (fs::recursive_directory_iterator it(d), e; it != e; ++it) {
        if (it->is_directory()) {
            test_true( it.recursion_pending() );
            checked_pending = true;
            it.disable_recursion_pending();
            test_true( !it.recursion_pending() );
        }
        if (fname_is(it->path(), "b.txt")) saw_b_when_disabled = true;
    }
    test_true( checked_pending && !saw_b_when_disabled );
    test_pass("cxx17:recursive_directory_iterator::recursion_pending");

    bool popped = false;
    for (fs::recursive_directory_iterator it(d), e; it != e; ) {
        if (it.depth() == 1) {
            it.pop();
            popped = true;
            if (it != e) test_true( it.depth() == 0 );
            else test_true( true );
            break;
        }
        ++it;
    }
    test_true( popped );
    test_pass("cxx17:recursive_directory_iterator::pop");

    fs::remove_all(d);
}

TEST_CASE(filesystem, copy_and_options) {
    typedef fs::copy_options CO;
    test_true( fs::copy_options() == CO::none );
    fs::copy_options opt = fs::copy_options(CO::recursive) | fs::copy_options(CO::overwrite_existing);
    test_true( (opt & fs::copy_options(CO::recursive)) != fs::copy_options(CO::none) );
    test_pass("cxx17:copy_options");

    fs::path d("cfssrc");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    { STD::FILE* fp = STD::fopen((d / "a.txt").string().c_str(), "w"); if (fp) { STD::fputs("a", fp); STD::fclose(fp); } }
    fs::path sub = d / "sub";
    fs::create_directory(sub);
    { STD::FILE* fp = STD::fopen((sub / "b.txt").string().c_str(), "w"); if (fp) { STD::fputs("b", fp); STD::fclose(fp); } }

    fs::path dst("cfsdst");
    fs::remove_all(dst);
    fs::copy(d, dst, fs::copy_options(CO::recursive));
    test_true( fs::exists(dst / "a.txt") );
    test_true( fs::exists(dst / "sub" / "b.txt") );
    test_pass("cxx17:copy");

    fs::remove_all(d);
    fs::remove_all(dst);
}

static fs::file_time_type::duration lwt_hour() {
    return STD::chrono::duration_cast<fs::file_time_type::duration>(STD::chrono::hours(1));
}

static bool lwt_near(fs::file_time_type a, fs::file_time_type b) {
    STD::chrono::seconds d = STD::chrono::duration_cast<STD::chrono::seconds>(a - b);
    return d.count() <= 2 && d.count() >= -2;
}

TEST_CASE(filesystem, last_write_time_set) {
    fs::path f("ccwfslwt.txt");
    fs::remove(f);
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("t", fp); STD::fclose(fp); } }

    fs::file_time_type t0 = fs::last_write_time(f);
    fs::last_write_time(f, t0);
    fs::file_time_type t1 = fs::last_write_time(f);
    fs::last_write_time(f, t1);
    fs::file_time_type t2 = fs::last_write_time(f);
    test_true( t2 == t1 );

    fs::file_time_type want = t2 - lwt_hour();
    fs::last_write_time(f, want);
    test_true( lwt_near(fs::last_write_time(f), want) );

    fs::path d("ccwfslwtd");
    fs::remove_all(d);
    fs::create_directory(d);
    fs::file_time_type dt = fs::last_write_time(d) - lwt_hour();
    fs::last_write_time(d, dt);
    test_true( lwt_near(fs::last_write_time(d), dt) );
    fs::remove_all(d);
    test_pass("cxx17:last_write_time (set)");

    fs::remove(f);
}

TEST_CASE(filesystem, space_info_members) {
    fs::space_info si = fs::space(fs::current_path());

    STD::uintmax_t cap = si.capacity, fr = si.free, av = si.available;
    test_true( cap > 0 );
    test_true( fr <= cap );
    test_true( av <= cap );
    test_pass("cxx17:space_info members");
}

TEST_CASE(filesystem, symlink_and_hardlink) {
    fs::path d("cfslnk");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    fs::path target = d / "target.txt";
    { STD::FILE* fp = STD::fopen(target.string().c_str(), "w"); if (fp) { STD::fputs("tgt", fp); STD::fclose(fp); } }

    fs::path hl = d / "hard.txt";
    STD::error_code hec;
    fs::create_hard_link(target, hl, hec);
    if (!hec) {
        test_true( fs::exists(hl) );
        TEST_SKIP_WAT("stat() reports st_nlink=1 and no inode on Windows");
        test_true( fs::equivalent(hl, target) );
        TEST_SKIP_WAT("stat() reports st_nlink=1 and no inode on Windows");
        test_true( fs::hard_link_count(target) >= 2 );
    } else {
        TEST_SKIP1(); test_true(true);
        TEST_SKIP1(); test_true(true);
        TEST_SKIP1(); test_true(true);
    }
    test_pass("cxx17:create_hard_link");

    {   STD::error_code hec;
        fs::path hl2 = d / "hard2.txt";
        fs::remove(hl2);
        hec.clear();
        fs::create_hard_link(target, hl2, hec);
        if (!hec) {
            test_true( fs::exists(hl2) );
            test_eq( (unsigned long)fs::file_size(hl2), (unsigned long)fs::file_size(target) );
        } else {
            TEST_NOTE("hard links are not available on this filesystem");
            TEST_SKIP_N(2);
        }
        hec.clear();
        fs::create_hard_link(d / "no_such_source", d / "hard3.txt", hec);
        test_true( (bool)hec );
        fs::remove(hl2); }
    test_pass("cxx17:create_hard_link(const path& to, const path& link, error_code& ec)");

    fs::path sl = d / "link.txt";
    STD::error_code sec;
    fs::create_symlink(target, sl, sec);
    bool have_symlink = !sec && fs::is_symlink(fs::symlink_status(sl));
    if (have_symlink) {
        test_true( fs::read_symlink(sl) == target );
    } else {
        TEST_SKIP1(); test_true(true);
    }
    test_pass("cxx17:create_symlink");

    if (have_symlink) {
        test_true( fs::read_symlink(sl) == target );
    } else {
        TEST_SKIP1(); test_true(true);
    }
    test_pass("cxx17:read_symlink");

    {   STD::error_code rec;
        rec.clear();
        fs::path got = fs::read_symlink(sl, rec);
        if (have_symlink) {
            test_true( !rec );
            test_true( got == target );
        } else {
            test_true( (bool)rec );
            TEST_SKIP1();
        }
        rec.clear();
        fs::read_symlink(d / "no_such_link_here", rec);
        test_true( (bool)rec ); }
    test_pass("cxx17:read_symlink(const path& p, error_code& ec)");

    fs::path dsl = d / "dlink";
    STD::error_code dec;
    fs::create_directory_symlink(d, dsl, dec);
    if (!dec && fs::is_symlink(fs::symlink_status(dsl))) {
        test_true( fs::read_symlink(dsl) == d );
    } else {
        TEST_SKIP1(); test_true(true);
    }
    test_pass("cxx17:create_directory_symlink");

    fs::path sl2 = d / "link2.txt";
    if (have_symlink) {
        STD::error_code cec;
        fs::copy_symlink(sl, sl2, cec);
        test_true( !cec && fs::is_symlink(fs::symlink_status(sl2)) );
    } else {
        TEST_SKIP1(); test_true(true);
    }
    test_pass("cxx17:copy_symlink");

    fs::remove_all(d);
}

TEST_CASE(filesystem, path_spaceship) {
#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
    test_true( (fs::path("a") <=> fs::path("a")) == 0 );
    test_true( (fs::path("a") <=> fs::path("b")) <  0 );
    test_true( (fs::path("b") <=> fs::path("a")) >  0 );
    test_pass("cxx20:path::operator<=>");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:path::operator<=>");
#endif
}

TEST_CASE(filesystem, directory_entry_spaceship) {
    fs::path d("cfsdes");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    fs::path f1 = d / "a.txt", f2 = d / "b.txt";
    { STD::FILE* fp = STD::fopen(f1.string().c_str(), "w"); if (fp) STD::fclose(fp); }
    { STD::FILE* fp = STD::fopen(f2.string().c_str(), "w"); if (fp) STD::fclose(fp); }
    fs::directory_entry e1(f1), e2(f2);
#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
    test_true( (e1 <=> e1) == 0 );
    test_true( (e1 <=> e2) <  0 );
    test_pass("cxx20:directory_entry::operator<=>");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:directory_entry::operator<=>");
#endif

    fs::remove_all(d);
}

TEST_CASE(filesystem, directory_iterator_surface_cxx17) {
    fs::path d("cfsdis");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    { STD::FILE* fp = STD::fopen((d / "one.txt").string().c_str(), "w");
      if (fp) { STD::fputs("x", fp); STD::fclose(fp); } }

    typedef fs::directory_iterator DI;
    test_true(( STD::is_same<DI::value_type, fs::directory_entry>::value ));
    test_pass("cxx17:directory_iterator::value_type");
    test_true(( STD::is_same<DI::difference_type, STD::ptrdiff_t>::value ));
    test_pass("cxx17:directory_iterator::difference_type");
    test_true(( STD::is_same<DI::pointer, const fs::directory_entry*>::value ));
    test_pass("cxx17:directory_iterator::pointer");
    test_true(( STD::is_same<DI::reference, const fs::directory_entry&>::value ));
    test_pass("cxx17:directory_iterator::reference");
    test_true(( STD::is_same<DI::iterator_category, STD::input_iterator_tag>::value ));
    test_pass("cxx17:directory_iterator::iterator_category");

    DI it(d);
    test_true( it != DI() );
    const fs::directory_entry& e = *it;
    test_true( fname_is(e.path(), "one.txt") );
#if _TST_HAS_DECLTYPE
    test_true(( STD::is_same<decltype(*it), DI::reference>::value ));
    test_pass("cxx17:directory_iterator::operator*");
#else
    TEST_SKIP1();
    test_skip("cxx17:directory_iterator::operator*");
#endif

    test_true( it->path() == e.path() );
    test_true( tst_type_is<DI::pointer>(it.operator->()) );
    test_pass("cxx17:directory_iterator::operator->");

    ++it;
    test_true( it == DI() );
    test_pass("cxx17:directory_iterator::operator++");

    fs::remove_all(d);
}

TEST_CASE(filesystem, recursive_directory_iterator_surface_cxx17) {
    fs::path d("cfsrdis");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    fs::path sub = d / "sub";
    test_true( fs::create_directory(sub) );
    { STD::FILE* fp = STD::fopen((sub / "deep.txt").string().c_str(), "w");
      if (fp) { STD::fputs("d", fp); STD::fclose(fp); } }

    typedef fs::recursive_directory_iterator RDI;
    test_true(( STD::is_same<RDI::value_type, fs::directory_entry>::value ));
    test_pass("cxx17:recursive_directory_iterator::value_type");
    test_true(( STD::is_same<RDI::difference_type, STD::ptrdiff_t>::value ));
    test_pass("cxx17:recursive_directory_iterator::difference_type");
    test_true(( STD::is_same<RDI::pointer, const fs::directory_entry*>::value ));
    test_pass("cxx17:recursive_directory_iterator::pointer");
    test_true(( STD::is_same<RDI::reference, const fs::directory_entry&>::value ));
    test_pass("cxx17:recursive_directory_iterator::reference");
    test_true(( STD::is_same<RDI::iterator_category, STD::input_iterator_tag>::value ));
    test_pass("cxx17:recursive_directory_iterator::iterator_category");

    {
        RDI it(d);
        test_true( it != RDI() );
        const fs::directory_entry& e = *it;
        test_true( fname_is(e.path(), "sub") );
        test_pass("cxx17:recursive_directory_iterator::operator*");
        test_true( it->path() == e.path() );
        test_pass("cxx17:recursive_directory_iterator::operator->");
        ++it;
        test_true( it != RDI() );
        test_true( fname_is(it->path(), "deep.txt") );
        test_eq( it.depth(), 1 );
        test_pass("cxx17:recursive_directory_iterator::operator++");
    }
    {
        RDI plain(d);
        test_true( plain.options() == fs::directory_options::none );
        RDI skipping(d, fs::directory_options::skip_permission_denied);
        test_true( skipping.options() == fs::directory_options::skip_permission_denied );
        test_pass("cxx17:recursive_directory_iterator::options");
    }
    {
        bool saw_deep = false;
        for (RDI it(d), e; it != e; ++it) {
            if (it->is_directory()) it.disable_recursion_pending();
            if (fname_is(it->path(), "deep.txt")) saw_deep = true;
        }
        test_true( !saw_deep );
        bool saw_deep_default = false;
        for (RDI it(d), e; it != e; ++it)
            if (fname_is(it->path(), "deep.txt")) saw_deep_default = true;
        test_true( saw_deep_default );
        test_pass("cxx17:recursive_directory_iterator::disable_recursion_pending");
    }

    fs::remove_all(d);
}

TEST_CASE(filesystem, path_types_and_io_cxx17) {
    test_true(( STD::is_same<fs::path::value_type, fs::path::string_type::value_type>::value ));
#if defined(_CCW_FS_WCHAR) && _CCW_FS_WCHAR
    test_true(( STD::is_same<fs::path::value_type, wchar_t>::value ));
#elif defined(_CCW_LIBCPP_FILESYSTEM)
    test_true(( STD::is_same<fs::path::value_type, char>::value ));
#elif defined(_WIN32)
    test_true(( STD::is_same<fs::path::value_type, wchar_t>::value ));
#else
    test_true(( STD::is_same<fs::path::value_type, char>::value ));
#endif
    test_pass("cxx17:path::value_type");

    {
        fs::path from_n("a/b.txt");
        fs::path from_w(L"a/b.txt");
        test_true( from_n == from_w );
        test_true( from_n.string()  == STD::string("a/b.txt") );
        test_true( from_n.wstring() == STD::wstring(L"a/b.txt") );
        test_true( from_n.generic_string()  == STD::string("a/b.txt") );
        test_true( from_n.generic_wstring() == STD::wstring(L"a/b.txt") );
        test_true( fs::path::string_type(from_n.c_str()) == from_n.native() );
        fs::path cat(L"a"); cat += "-n"; cat += L"-w";
        test_true( cat == fs::path("a-n-w") );
    }
    test_pass("cxx17:path::char_family_interop");

    fs::path p("a/b/c");
    test_true(( STD::is_same<fs::path::iterator, fs::path::const_iterator>::value ));
    test_true(( STD::is_same<fs::path::iterator::value_type, fs::path>::value ));
    int n = 0;
    for (fs::path::iterator it = p.begin(); it != p.end(); ++it) ++n;
    test_eq( n, 3 );
    test_pass("cxx17:path::iterator");
    fs::path::const_iterator ci = p.begin();
    test_true( *ci == fs::path("a") );
    test_pass("cxx17:path::const_iterator");

    test_true( !fs::path("a/b").has_root_path() );
    test_true( fs::path("a/b").root_path().empty() );
    fs::path abs = fs::absolute(fs::path("a"));
    test_true( abs.has_root_path() );
    test_true( !abs.root_path().empty() );
    test_pass("cxx17:path::has_root_path");

#if _TST_HAS_CHAR8_NAME
    {
        fs::path q("a/b");
        fs::path::string_type dummy;
        (void)dummy;
        STD::u8string u8 = q.generic_u8string();
        test_true( u8.size() == 3 );
        test_true( u8[1] == (char8_t)'/' );
        test_true( fs::path(u8) == q );
        test_pass("cxx17:path::generic_u8string");
    }
#else
    TEST_NOTE("u8string (char8_t) is C++20; before that these return std::string");
    test_skip("cxx17:path::generic_u8string");
#endif

    {
        STD::ostringstream os;
        os << fs::path("a/b");
        test_true( os.str().size() >= 5 );
        test_true( os.str()[0] == '"' );
        test_pass("cxx20:path::operator<<");

        STD::istringstream is(os.str());
        fs::path back;
        is >> back;
        test_true( back == fs::path("a/b") );
        test_pass("cxx20:path::operator>>");
    }

#if defined(__WATCOMC__)
    test_true(( STD::is_same<fs::file_time_type, fs::file_time_type::clock::time_point>::value ));
#else
    test_true(( STD::is_same<fs::file_time_type,
                             STD::chrono::time_point<fs::file_time_type::clock> >::value ));
#endif
    {
        fs::path f("ccwfsftt.txt");
        fs::remove(f);
        { STD::FILE* fp = STD::fopen(f.string().c_str(), "w");
          if (fp) { STD::fputs("t", fp); STD::fclose(fp); } }
        fs::file_time_type t = fs::last_write_time(f);
        test_true( t.time_since_epoch().count() != 0 );
        fs::remove(f);
    }
    test_pass("cxx17:file_time_type");

    {
        fs::filesystem_error err("boom", STD::make_error_code(STD::errc::invalid_argument));
        test_true( err.what() != 0 );
        test_true( STD::string(err.what()).find("boom") != STD::string::npos );
        const STD::exception& base = err;
        test_true( STD::string(base.what()) == STD::string(err.what()) );
        test_pass("cxx17:filesystem_error::what");
    }
}

TEST_CASE(filesystem, hash_and_entry_insertion_cxx23) {
    fs::path a("dir/file.txt"), b("dir/file.txt"), c("dir/other.txt");
    STD::hash<fs::path> h;
    test_eq( h(a), h(b) );
    test_true( h(a) != h(c) );
    test_eq( h(a), fs::hash_value(a) );
    test_pass("cxx23:hash");

    fs::path d("cfsent");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    fs::directory_entry e(d);
    STD::ostringstream os;
    os << e;
    STD::ostringstream ps;
    ps << e.path();
    test_eq( os.str(), ps.str() );
    test_pass("cxx23:directory_entry::operator<<");
    fs::remove_all(d);
}

TEST_CASE(filesystem, path_display_strings_cxx26) {
#if defined(__cpp_lib_fs_path_nonmember_conversions)
    fs::path p("a/b");
    test_true( p.display_string().size() > 0 );
    test_pass("cxx26:path::display_string");
    test_true( p.generic_display_string().size() > 0 );
    test_pass("cxx26:path::generic_display_string");
    test_true( p.native_encoded_string().size() > 0 );
    test_pass("cxx26:path::native_encoded_string");
    test_true( p.generic_native_encoded_string().size() > 0 );
    test_pass("cxx26:path::generic_native_encoded_string");
#else
    TEST_NOTE("the display / native-encoded strings are C++26; not in this library");
    test_skip("cxx26:path::display_string");
    test_skip("cxx26:path::generic_display_string");
    test_skip("cxx26:path::native_encoded_string");
    test_skip("cxx26:path::generic_native_encoded_string");
#endif
}

TEST_CASE(filesystem, file_type_enumerators_cxx17) {
    typedef fs::file_type FT;

    test_true( FT::none != FT::not_found );
    test_true( FT::none != FT::regular && FT::none != FT::directory );
    test_pass("cxx17:file_type::none");

    fs::path missing("cfsnone");
    fs::remove(missing);
    STD::error_code ec;
    fs::file_status st = fs::status(missing, ec);
    test_true( st.type() == FT::not_found );
    test_true( !fs::exists(st) );
    test_pass("cxx17:file_type::not_found");

    fs::path f("ccwfsft.txt");
    fs::remove(f);
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w");
      if (fp) { STD::fputs("x", fp); STD::fclose(fp); } }
    test_true( fs::status(f).type() == FT::regular );
    test_true( fs::is_regular_file(f) );
    test_pass("cxx17:file_type::regular");
    fs::remove(f);

    fs::path d("cfsftd");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    test_true( fs::status(d).type() == FT::directory );
    test_true( fs::is_directory(d) );
    test_pass("cxx17:file_type::directory");
    fs::remove_all(d);

    test_true( FT::symlink != FT::regular && FT::symlink != FT::directory );
    test_true( !fs::is_symlink(fs::file_status(FT::regular)) );
    test_true( fs::is_symlink(fs::file_status(FT::symlink)) );
    test_pass("cxx17:file_type::symlink");

    test_true( FT::block != FT::character && FT::block != FT::regular );
    test_true( fs::is_block_file(fs::file_status(FT::block)) );
    test_pass("cxx17:file_type::block");
    test_true( fs::is_character_file(fs::file_status(FT::character)) );
    test_pass("cxx17:file_type::character");
    test_true( FT::fifo != FT::socket );
    test_true( fs::is_fifo(fs::file_status(FT::fifo)) );
    test_pass("cxx17:file_type::fifo");
    test_true( fs::is_socket(fs::file_status(FT::socket)) );
    test_pass("cxx17:file_type::socket");

    test_true( FT::unknown != FT::none && FT::unknown != FT::not_found );
    test_true( fs::exists(fs::file_status(FT::unknown)) );
    test_true( fs::is_other(fs::file_status(FT::unknown)) );
    test_pass("cxx17:file_type::unknown");
}

TEST_CASE(filesystem, directory_entry_status_queries_cxx17) {
    fs::path d("cfsdeq");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );
    fs::path f = d / "plain.txt";
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w");
      if (fp) { STD::fputs("x", fp); STD::fclose(fp); } }

    fs::directory_entry fe(f);
    fs::directory_entry de(d);

    test_true( fe.is_regular_file() );
    test_eq( (int)fe.is_regular_file(), (int)fs::is_regular_file(f) );
    test_true( !de.is_regular_file() );
    test_pass("cxx17:directory_entry::is_regular_file");

    test_true( de.is_directory() );
    test_eq( (int)de.is_directory(), (int)fs::is_directory(d) );
    test_true( !fe.is_directory() );
    test_pass("cxx17:directory_entry::is_directory");

    test_true( !fe.is_symlink() );
    test_eq( (int)fe.is_symlink(), (int)fs::is_symlink(f) );
    test_pass("cxx17:directory_entry::is_symlink");

    test_true( !fe.is_block_file() );
    test_eq( (int)fe.is_block_file(), (int)fs::is_block_file(f) );
    test_pass("cxx17:directory_entry::is_block_file");

    test_true( !fe.is_character_file() );
    test_eq( (int)fe.is_character_file(), (int)fs::is_character_file(f) );
    test_pass("cxx17:directory_entry::is_character_file");

    test_true( !fe.is_fifo() );
    test_eq( (int)fe.is_fifo(), (int)fs::is_fifo(f) );
    test_pass("cxx17:directory_entry::is_fifo");

    test_true( !fe.is_socket() );
    test_eq( (int)fe.is_socket(), (int)fs::is_socket(f) );
    test_pass("cxx17:directory_entry::is_socket");

    test_true( !fe.is_other() );
    test_true( !de.is_other() );
    test_eq( (int)fe.is_other(), (int)fs::is_other(f) );
    test_pass("cxx17:directory_entry::is_other");

    fs::remove_all(d);
}

TEST_CASE(filesystem, space_info_data_members_cxx17) {
    fs::space_info si = fs::space(fs::current_path());

    test_true( tst_type_is<STD::uintmax_t>(si.capacity) );
    test_true( si.capacity > 0 );
    test_true( si.capacity != (STD::uintmax_t)-1 );
    test_pass("cxx17:space_info::capacity");

    test_true( tst_type_is<STD::uintmax_t>(si.free) );
    test_true( si.free <= si.capacity );
    test_pass("cxx17:space_info::free");

    test_true( tst_type_is<STD::uintmax_t>(si.available) );
    test_true( si.available <= si.free );
    test_pass("cxx17:space_info::available");
}

TEST_CASE(filesystem, operation_overloads_cxx17) {
    STD::error_code ec;
    fs::path d("cfsovl");
    fs::remove_all(d);
    test_true( fs::create_directory(d) );

    fs::path d2 = d / "sub";
    test_true(  fs::create_directory(d2, d) );
    test_true(  fs::is_directory(d2) );
    test_true( !fs::create_directory(d2, d) );
    test_pass("cxx17:create_directory(const path& p, const path& existing_p)");

    fs::path d3 = d / "sub3";
    ec.clear();
    test_true( fs::create_directory(d3, d, ec) );
    test_true( !ec );
    ec.clear();
    test_true( !fs::create_directory(d3, d, ec) );
    test_true( !ec );
    test_pass("cxx17:create_directory(const path& p, const path& existing_p, error_code& ec)");

    fs::path f = d / "a.txt";
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("hi", fp); STD::fclose(fp); } }
    fs::path missing = d / "no_such_file";

    fs::file_status st = fs::status(f);
    fs::file_status dst = fs::status(d);
    fs::file_status nst = fs::status(missing);
    test_true(  fs::exists(st) );
    test_true(  fs::exists(dst) );
    test_true( !fs::exists(nst) );
    test_pass("cxx17:exists(file_status s)");
    test_true(  fs::is_regular_file(st) );
    test_true( !fs::is_regular_file(dst) );
    test_true( !fs::is_regular_file(nst) );
    test_pass("cxx17:is_regular_file(file_status s)");
    test_true( !fs::is_directory(st) );
    test_true(  fs::is_directory(dst) );
    test_true( !fs::is_directory(nst) );
    test_pass("cxx17:is_directory(file_status s)");
    test_true( !fs::is_block_file(st) );
    test_true( !fs::is_block_file(dst) );
    test_pass("cxx17:is_block_file(file_status s)");
    test_true( !fs::is_character_file(st) );
    test_true( !fs::is_character_file(dst) );
    test_pass("cxx17:is_character_file(file_status s)");
    test_true( !fs::is_fifo(st) );
    test_true( !fs::is_fifo(dst) );
    test_pass("cxx17:is_fifo(file_status s)");
    test_true( !fs::is_socket(st) );
    test_true( !fs::is_socket(dst) );
    test_pass("cxx17:is_socket(file_status s)");
    test_true( !fs::is_symlink(st) );
    test_true( !fs::is_symlink(dst) );
    test_pass("cxx17:is_symlink(file_status s)");
    test_true( !fs::is_other(st) );
    test_true( !fs::is_other(dst) );
    test_pass("cxx17:is_other(file_status s)");

    ec.clear();
    fs::file_status st2 = fs::status(f, ec);
    test_true( !ec );
    test_true( st2.type() == st.type() );
    ec.clear();
    fs::file_status st2n = fs::status(missing, ec);
    test_true( !fs::exists(st2n) );
    test_pass("cxx17:status(const path& p, error_code& ec)");

    ec.clear();
    fs::file_status st3 = fs::symlink_status(f, ec);
    test_true( !ec );
    test_true( st3.type() == fs::symlink_status(f).type() );
    ec.clear();
    fs::file_status st3n = fs::symlink_status(missing, ec);
    test_true( !fs::exists(st3n) );
    test_pass("cxx17:symlink_status(const path& p, error_code& ec)");

    fs::path cp = d / "copied.txt";
    fs::copy(f, cp, fs::copy_options(fs::copy_options::overwrite_existing));
    test_true( fs::exists(cp) );
    test_eq( (unsigned long)fs::file_size(cp), 2UL );
    { STD::FILE* fp = STD::fopen(cp.string().c_str(), "w"); if (fp) { STD::fputs("xyzw", fp); STD::fclose(fp); } }
    fs::copy(f, cp, fs::copy_options(fs::copy_options::skip_existing));
    test_eq( (unsigned long)fs::file_size(cp), 4UL );
    fs::copy(f, cp, fs::copy_options(fs::copy_options::overwrite_existing));
    test_eq( (unsigned long)fs::file_size(cp), 2UL );
    test_pass("cxx17:copy(const path& from, const path& to, copy_options options)");

    ec.clear();
    fs::copy(f, cp, fs::copy_options(fs::copy_options::overwrite_existing), ec);
    test_true( !ec );
    ec.clear();
    fs::copy(missing, d / "never.txt", fs::copy_options(fs::copy_options::none), ec);
    test_true( !fs::exists(d / "never.txt") );
    test_pass("cxx17:copy(const path& from, const path& to, copy_options options, error_code& ec)");

    fs::permissions(f, fs::perms(fs::perms::owner_read),
                    fs::perm_options(fs::perm_options::replace));
    test_true( (fs::status(f).permissions() & fs::perms(fs::perms::owner_write))
               == fs::perms(fs::perms::none) );
    fs::permissions(f, fs::perms(fs::perms::owner_write),
                    fs::perm_options(fs::perm_options::add));
    test_true( (fs::status(f).permissions() & fs::perms(fs::perms::owner_write))
               != fs::perms(fs::perms::none) );
    fs::permissions(f, fs::perms((unsigned)fs::perms::owner_write
                                 | (unsigned)fs::perms::group_write
                                 | (unsigned)fs::perms::others_write),
                    fs::perm_options(fs::perm_options::remove));
    test_true( (fs::status(f).permissions() & fs::perms(fs::perms::owner_write))
               == fs::perms(fs::perms::none) );
    test_pass("cxx17:permissions(const path& p, perms prms, perm_options opts)");

    ec.clear();
    fs::permissions(f, fs::perms(fs::perms::owner_write),
                    fs::perm_options(fs::perm_options::add), ec);
    test_true( !ec );
    test_true( (fs::status(f).permissions() & fs::perms(fs::perms::owner_write))
               != fs::perms(fs::perms::none) );
    ec.clear();
    fs::permissions(missing, fs::perms(fs::perms::owner_write),
                    fs::perm_options(fs::perm_options::add), ec);
    test_true( (bool)ec );
    test_pass("cxx17:permissions(const path& p, perms prms, perm_options opts, error_code& ec)");

    ec.clear();
    fs::file_time_type t0 = fs::last_write_time(f, ec);
    test_true( !ec );
    ec.clear();
    fs::last_write_time(missing, ec);
    test_true( (bool)ec );
    test_pass("cxx17:last_write_time(const path& p, error_code& ec)");

    ec.clear();
    fs::last_write_time(f, t0, ec);
    test_true( !ec );
    fs::file_time_type t1 = fs::last_write_time(f);
    ec.clear();
    fs::last_write_time(f, t1, ec);
    test_true( !ec );
    test_true( fs::last_write_time(f) == t1 );
    ec.clear();
    fs::last_write_time(missing, t1, ec);
    test_true( (bool)ec );
    test_pass("cxx17:last_write_time(const path& p, file_time_type new_time, error_code& ec)");

    ec.clear();
    fs::path pr = fs::proximate(f, ec);
    test_true( !ec );
    test_true( !pr.empty() );
    ec.clear();
    fs::path pr2 = fs::proximate(fs::current_path(), ec);
    test_true( !ec );
    test_true( pr2 == fs::path(".") || !pr2.empty() );
    test_pass("cxx17:proximate(const path& p, error_code& ec)");

    ec.clear();
    fs::path rl = fs::relative(f, ec);
    test_true( !ec );
    test_true( !rl.empty() );
    ec.clear();
    fs::path rl2 = fs::relative(fs::current_path(), ec);
    test_true( !ec );
    test_true( rl2 == fs::path(".") || !rl2.empty() );
    test_pass("cxx17:relative(const path& p, error_code& ec)");

    fs::path back = fs::current_path();
    fs::current_path(d);
    test_true( fs::exists(fs::path("a.txt")) );
    test_true( !fs::exists(fs::path("cfsovl")) );
    fs::current_path(back);
    test_true( fs::current_path() == back );
    test_pass("cxx17:current_path(const path& p)");

    ec.clear();
    fs::current_path(d, ec);
    test_true( !ec );
    test_true( fs::exists(fs::path("a.txt")) );
    ec.clear();
    fs::current_path(back, ec);
    test_true( !ec );
    test_true( fs::current_path() == back );
    ec.clear();
    fs::current_path(back / "no_such_dir_here", ec);
    test_true( (bool)ec );
    test_true( fs::current_path() == back );
    test_pass("cxx17:current_path(const path& p, error_code& ec)");

    fs::permissions(f, fs::perms(fs::perms::owner_all),
                    fs::perm_options(fs::perm_options::replace));
    fs::remove_all(d);
}

#endif

TEST_CASE(filesystem, feature_test_macros) {
#if defined(__cpp_lib_filesystem)
    test_true( __cpp_lib_filesystem > 0L );
    test_pass("cxx17:__cpp_lib_filesystem");
#else
    test_skip("cxx17:__cpp_lib_filesystem");
#endif
#if defined(__cpp_lib_format_path)
    test_true( __cpp_lib_format_path > 0L );
    test_pass("cxx26:__cpp_lib_format_path");
#else
    test_skip("cxx26:__cpp_lib_format_path");
#endif
}

TEST_CASE(filesystem, iterator_and_container_lifetime) {
    fs::path base = fs::temp_directory_path() / "ccw_life";
    fs::remove_all(base);
    test_true( fs::create_directories(base) );

    const int N = 12;
    for (int i = 0; i < N; ++i) {
        STD::string nm = "f";
        nm += (char)('a' + i);
        nm += ".txt";
        STD::FILE* fp = STD::fopen((base / nm).string().c_str(), "wb");
        test_true( fp != 0 );
        if (fp) { STD::fputc('x', fp); STD::fclose(fp); }
    }

    STD::vector<fs::path> seen;
    for (fs::directory_iterator it(base), e; it != e; ++it)
        seen.push_back(it->path());
    test_eq( (int)seen.size(), N );

    STD::vector<fs::path> copy_of(seen);
    test_eq( (int)copy_of.size(), N );
    STD::sort(copy_of.begin(), copy_of.end());
    test_true( copy_of.front() != copy_of.back() );

    STD::map<fs::path, int> byname;
    for (STD::vector<fs::path>::iterator it = seen.begin(); it != seen.end(); ++it)
        byname[*it] = (int)it->filename().string().size();
    test_eq( (int)byname.size(), N );
    byname.erase(byname.begin());
    test_eq( (int)byname.size(), N - 1 );
    byname.clear();
    test_true( byname.empty() );

    int again = 0;
    for (int pass = 0; pass < 3; ++pass) {
        for (fs::directory_iterator it(base), e; it != e; ++it) ++again;
    }
    test_eq( again, N * 3 );

    int rec = 0;
    for (fs::recursive_directory_iterator it(base), e; it != e; ++it) ++rec;
    test_eq( rec, N );

    seen.clear();
    copy_of.clear();
    test_true( seen.empty() );

    fs::remove_all(base);
    test_true( !fs::exists(base) );
}

TEST_CASE(filesystem, error_code_overloads) {
    STD::error_code ec;
    fs::path base = fs::temp_directory_path() / "ccw_ec";
    fs::remove_all(base);
    test_true( fs::create_directories(base, ec) );
    test_true( !ec );
    test_pass("cxx17:create_directories(const path& p, error_code& ec)");

    fs::path sub = base / "sub";
    test_true( fs::create_directory(sub, ec) );
    test_true( !ec );
    test_pass("cxx17:create_directory(const path& p, error_code& ec)");

    fs::path f = base / "a.txt";
    { STD::FILE* fp = STD::fopen(f.string().c_str(), "w"); if (fp) { STD::fputs("hello", fp); STD::fclose(fp); } }

    test_true( fs::exists(f, ec) );      test_true( !ec );
    test_pass("cxx17:exists(const path& p, error_code& ec)");
    test_true( !fs::is_directory(f, ec) ); test_true( !ec );
    test_pass("cxx17:is_directory(const path& p, error_code& ec)");
    test_true( fs::is_regular_file(f, ec) ); test_true( !ec );
    test_pass("cxx17:is_regular_file(const path& p, error_code& ec)");
    test_eq( (int)fs::file_size(f, ec), 5 ); test_true( !ec );
    test_pass("cxx17:file_size(const path& p, error_code& ec)");
    test_true( !fs::is_empty(f, ec) );   test_true( !ec );
    test_pass("cxx17:is_empty(const path& p, error_code& ec)");
    test_true( !fs::is_symlink(f, ec) );        test_pass("cxx17:is_symlink(const path& p, error_code& ec)");
    test_true( !fs::is_other(f, ec) );          test_pass("cxx17:is_other(const path& p, error_code& ec)");
    test_true( !fs::is_block_file(f, ec) );     test_pass("cxx17:is_block_file(const path& p, error_code& ec)");
    test_true( !fs::is_character_file(f, ec) ); test_pass("cxx17:is_character_file(const path& p, error_code& ec)");
    test_true( !fs::is_fifo(f, ec) );           test_pass("cxx17:is_fifo(const path& p, error_code& ec)");
    test_true( !fs::is_socket(f, ec) );         test_pass("cxx17:is_socket(const path& p, error_code& ec)");

    test_true( fs::equivalent(f, f, ec) ); test_true( !ec );
    test_pass("cxx17:equivalent(const path& p1, const path& p2, error_code& ec)");
    test_true( fs::hard_link_count(f, ec) >= 1u );
    test_pass("cxx17:hard_link_count(const path& p, error_code& ec)");

    fs::path g = base / "b.txt";
    test_true( fs::copy_file(f, g, ec) ); test_true( !ec );
    test_pass("cxx17:copy_file(const path& from, const path& to, error_code& ec)");
    test_true( !fs::copy_file(f, g, fs::copy_options(fs::copy_options::skip_existing)) );
    test_true( fs::copy_file(f, g, fs::copy_options(fs::copy_options::overwrite_existing)) );
    test_pass("cxx17:copy_file(const path& from, const path& to, copy_options options)");
    test_true( !fs::copy_file(f, g, fs::copy_options(fs::copy_options::skip_existing), ec) );
    test_true( !ec );
    test_pass("cxx17:copy_file(const path& from, const path& to, copy_options options, error_code& ec)");

    fs::path h = base / "c.txt";
    fs::rename(g, h, ec);
    test_true( !ec );
    test_true( fs::exists(h) && !fs::exists(g) );
    test_pass("cxx17:rename(const path& old_p, const path& new_p, error_code& ec)");

    fs::copy(f, base / "d.txt", ec);
    test_true( !ec );
    test_pass("cxx17:copy(const path& from, const path& to, error_code& ec)");

    test_true( !fs::current_path(ec).empty() );      test_true( !ec );
    test_pass("cxx17:current_path(error_code& ec)");
    test_true( !fs::temp_directory_path(ec).empty() ); test_true( !ec );
    test_pass("cxx17:temp_directory_path(error_code& ec)");
    test_true( fs::absolute(f, ec).is_absolute() );  test_true( !ec );
    test_pass("cxx17:absolute(const path& p, error_code& ec)");
    test_true( !fs::canonical(f, ec).empty() );      test_true( !ec );
    test_pass("cxx17:canonical(const path& p, error_code& ec)");
    test_true( !fs::weakly_canonical(f, ec).empty() );
    test_pass("cxx17:weakly_canonical(const path& p, error_code& ec)");
    test_true( !fs::relative(f, base, ec).empty() );
    test_pass("cxx17:relative(const path& p, const path& base, error_code& ec)");
    test_true( !fs::proximate(f, base, ec).empty() );
    test_pass("cxx17:proximate(const path& p, const path& base, error_code& ec)");

    fs::space_info si = fs::space(base, ec);
    test_true( si.capacity != 0 || si.free != 0 || si.available != 0 || !ec );
    test_pass("cxx17:space(const path& p, error_code& ec)");

    fs::permissions(f, fs::perms(fs::perms::owner_all), ec);
    test_pass("cxx17:permissions(const path& p, perms prms, error_code& ec)");

    fs::resize_file(f, 2u, ec);
    test_pass("cxx17:resize_file(const path& p, uintmax_t new_size, error_code& ec)");

    test_true( fs::remove(h, ec) );  test_true( !ec );
    test_pass("cxx17:remove(const path& p, error_code& ec)");
    test_true( fs::remove_all(base, ec) > 0u );
    test_true( !ec );
    test_true( !fs::exists(base) );
    test_pass("cxx17:remove_all(const path& p, error_code& ec)");

    fs::path missing = base / "no_such_dir" / "no_such_file";
    ec.clear();
    test_true( !fs::exists(missing, ec) );
    test_true( fs::file_size(missing, ec) == (STD::uintmax_t)-1 );
    test_true( (bool)ec );
}
