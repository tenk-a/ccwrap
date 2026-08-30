#include "test_cxx.hpp"

#include <string>
#include <cstring>
#include <type_traits>
#include <sstream>
#if defined(__cpp_lib_string_view)
#include <string_view>
#endif
#if defined(__cpp_lib_memory_resource)
#include <memory_resource>
#endif

TEST_CASE(string, construct_capacity) {
    STD::string a;                          test_true( a.empty() );
    test_pass("cxx03:basic_string()");
    STD::string b("hello");                 test_eq( b.size(), 5u );
    test_pass("cxx03:basic_string(const char*)");
    STD::string c("hello", 3);              test_eq( c, STD::string("hel") );
    test_pass("cxx03:basic_string(const char*, n)");
    STD::string d(4, 'x');                  test_eq( d, STD::string("xxxx") );
    test_pass("cxx03:basic_string(n, char)");
    STD::string e(b);                       test_eq( e, b );
    test_pass("cxx03:basic_string(const basic_string&)");
    STD::string f(b, 1, 3);                 test_eq( f, STD::string("ell") );
    test_pass("cxx03:basic_string(str, pos, n)");
    STD::string g(b.begin(), b.end());      test_eq( g, b );
    test_pass("cxx03:basic_string(first, last)");

    test_eq( b.size(), 5u );
    test_pass("cxx03:string::size");
    test_eq( b.length(), 5u );
    test_pass("cxx03:string::length");
    test_true( b.capacity() >= 5u );
    test_pass("cxx03:string::capacity");
    test_true( b.max_size() > 0u );
    test_pass("cxx03:string::max_size");
    test_true( !b.empty() );
    test_pass("cxx03:string::empty");
    b.reserve(64);
    test_true( b.capacity() >= 64u );
    test_pass("cxx03:string::reserve");
    b.resize(3);
    test_eq( b, STD::string("hel") );
    b.resize(5, 'o');
    test_eq( b, STD::string("heloo") );
    test_pass("cxx03:string::resize");
    b.clear();
    test_true( b.empty() );
    test_pass("cxx03:string::clear");
}

TEST_CASE(string, access_modify) {
    STD::string s = "hello";
    test_eq( s[0], 'h' );
    test_pass("cxx03:string::operator[]");
    test_eq( s.at(4), 'o' );
    test_pass("cxx03:string::at");
    s[0] = 'H';
    test_eq( s, STD::string("Hello") );
    test_throw( (void)s.at(99) );
    test_pass("cxx03:string::at (out_of_range)");
    test_eq( STD::strcmp(s.c_str(), "Hello"), 0 );
    test_pass("cxx03:string::c_str");
    test_eq( *s.data(), 'H' );
    test_pass("cxx03:string::data");

    s += " world";
    test_eq( s, STD::string("Hello world") );
    test_pass("cxx03:string::operator+=");
    s.append("!!");
    test_eq( s, STD::string("Hello world!!") );
    test_pass("cxx03:string::append");
    s.append(2, '?');
    test_eq( s.size(), 15u );
    test_pass("cxx03:string::append (n, char)");
    {
        STD::string a = "abc";
        a.append(STD::string("XYZW"), 1, 2);
        test_eq( a, STD::string("abcYZ") );
        test_pass("cxx03:string::append(const basic_string& str, size_type pos, size_type n)");
        const char* src = "pq";
        a.append(src, src + 2);
        test_eq( a, STD::string("abcYZpq") );
        test_pass("cxx03:string::append(InputIterator first, InputIterator last)");

        STD::string b = "zzz";
        b.assign(STD::string("XYZW"), 1, 2);
        test_eq( b, STD::string("YZ") );
        test_pass("cxx03:string::assign(const basic_string& str, size_type pos, size_type n)");
        b.assign(src, src + 2);
        test_eq( b, STD::string("pq") );
        test_pass("cxx03:string::assign(InputIterator first, InputIterator last)");

        STD::string c = "abcdef";
        test_true( c.compare(1, 2, STD::string("xbcx"), 1, 2) == 0 );
        test_pass("cxx03:string::compare(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2)");
        test_true( c.compare(1, 2, "xbcx" + 1, 2) == 0 );
        test_pass("cxx03:string::compare(size_type pos1, size_type n1, const charT* s, size_type n2)");
    }

    STD::string x = "abc";
    x.insert(1, "XY");
    test_eq( x, STD::string("aXYbc") );
    test_pass("cxx03:string::insert");
    {
        STD::string i1 = "abc";
        i1.insert(1, STD::string("XYZ"), 1, 1);
        test_eq( i1, STD::string("aYbc") );
        test_pass("cxx03:string::insert(size_type pos1, const basic_string& str, size_type pos2, size_type n)");
        STD::string::size_type at0 = 0;
        i1.insert(at0, 3, '-');
        test_eq( i1, STD::string("---aYbc") );
        test_pass("cxx03:string::insert(size_type pos, size_type n, charT c)");
        STD::string::iterator it = i1.insert(i1.begin() + 3, '*');
        test_eq( i1, STD::string("---*aYbc") );
        test_eq( *it, '*' );
        test_pass("cxx03:string::insert(iterator p, charT c)");
        i1.insert(i1.begin(), 2, '#');
        test_eq( i1, STD::string("##---*aYbc") );
        test_pass("cxx03:string::insert(iterator p, size_type n, charT c)");
        const char* src = "pq";
        i1.insert(i1.end(), src, src + 2);
        test_eq( i1, STD::string("##---*aYbcpq") );
        test_pass("cxx03:string::insert(iterator p, InputIterator first, InputIterator last)");
    }
    x.erase(1, 2);
    test_eq( x, STD::string("abc") );
    test_pass("cxx03:string::erase");
    x.replace(0, 1, "AA");
    test_eq( x, STD::string("AAbc") );
    test_pass("cxx03:string::replace");
    {
        STD::string r1 = "abcdef";
        r1.replace(1, 2, 3, '*');
        test_eq( r1, STD::string("a***def") );
        test_pass("cxx03:string::replace(size_type pos1, size_type n1, size_type n2, charT c)");
        r1.replace(0, 1, STD::string("XYZ"), 1, 2);
        test_eq( r1, STD::string("YZ***def") );
        test_pass("cxx03:string::replace(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2)");
        r1.replace(r1.begin(), r1.begin() + 2, STD::string("--"));
        test_eq( r1, STD::string("--***def") );
        test_pass("cxx03:string::replace(iterator i1, iterator i2, const basic_string& str)");
        r1.replace(r1.begin(), r1.begin() + 2, "==", 2u);
        test_eq( r1, STD::string("==***def") );
        test_pass("cxx03:string::replace(iterator i1, iterator i2, const charT* s, size_type n)");
        r1.replace(r1.begin(), r1.begin() + 2, "@");
        test_eq( r1, STD::string("@***def") );
        test_pass("cxx03:string::replace(iterator i1, iterator i2, const charT* s)");
        r1.replace(r1.begin(), r1.begin() + 1, 2, '%');
        test_eq( r1, STD::string("%%***def") );
        test_pass("cxx03:string::replace(iterator i1, iterator i2, size_type n, charT c)");
        const char* rs = "ab";
        r1.replace(r1.begin(), r1.begin() + 2, rs, rs + 2);
        test_eq( r1, STD::string("ab***def") );
        test_pass("cxx03:string::replace(iterator i1, iterator i2, InputIterator first, InputIterator last)");
        r1.replace(0, 2, r1);
        test_eq( r1, STD::string("ab***def***def") );
    }
    x.assign("zzz");
    test_eq( x, STD::string("zzz") );
    test_pass("cxx03:string::assign");

    STD::string cat = STD::string("ab") + "cd" + STD::string("ef");
    test_eq( cat, STD::string("abcdef") );
    test_pass("cxx03:operator+(string)");

    {
        STD::string a = "0123456789abcdef";
        a += a;
        test_eq( a, STD::string("0123456789abcdef0123456789abcdef") );
        STD::string b = "0123456789abcdef";
        b.append(b.c_str(), 4);
        test_eq( b, STD::string("0123456789abcdef0123") );
        STD::string c = "0123456789abcdef";
        c.assign(c);
        test_eq( c, STD::string("0123456789abcdef") );
        STD::string d = "0123456789abcdef";
        d.assign(d.c_str() + 4, 4);
        test_eq( d, STD::string("4567") );
        STD::string e = "0123456789abcdef";
        e = e.c_str();
        test_eq( e, STD::string("0123456789abcdef") );
        STD::string f = "0123456789abcdef";
        f.insert(0, f);
        test_eq( f, STD::string("0123456789abcdef0123456789abcdef") );
        STD::string g = "0123456789abcdef";
        g.replace(0, 2, g);
        test_eq( g, STD::string("0123456789abcdef23456789abcdef") );
    }
}

TEST_CASE(string, search_substr) {
    STD::string s = "abcdefabc";
    test_eq( (int)s.find("cd"), 2 );
    test_pass("cxx03:string::find");
    test_eq( (int)s.find('a'), 0 );
    test_pass("cxx03:string::find (char)");
    test_eq( (int)s.rfind("abc"), 6 );
    test_pass("cxx03:string::rfind");
    test_eq( (int)s.find_first_of("cd"), 2 );
    test_pass("cxx03:string::find_first_of");
    test_eq( (int)s.find_last_of("ab"), 7 );
    test_pass("cxx03:string::find_last_of");
    test_eq( (int)s.find_first_not_of("ab"), 2 );
    test_pass("cxx03:string::find_first_not_of");
    test_eq( (int)s.find_last_not_of("bc"), 6 );
    test_pass("cxx03:string::find_last_not_of");
    test_eq( (int)s.find_last_not_of('c'), 7 );
    test_pass("cxx03:string::find_last_not_of(charT c)");
    test_eq( (int)s.find_last_not_of("bczz", STD::string::npos, 2), 6 );
    test_pass("cxx03:string::find_last_not_of(const charT* s, size_type pos, size_type n)");
    test_true( s.find("zzz") == STD::string::npos );
    test_pass("cxx03:string::npos");

    test_eq( s.substr(3, 3), STD::string("def") );
    test_pass("cxx03:string::substr");
    test_true( s.compare("abcdefabc") == 0 );
    test_true( s.compare("abd") < 0 );
    test_pass("cxx03:string::compare");

    char buf[4] = { 0, 0, 0, 0 };
    STD::size_t n = s.copy(buf, 3, 0);
    test_eq( n, 3u );
    test_eq( STD::strcmp(buf, "abc"), 0 );
    test_pass("cxx03:string::copy");
}

TEST_CASE(string, iterate_compare_swap) {
    STD::string s = "abcd";
    int cnt = 0;
    for (STD::string::iterator it = s.begin(); it != s.end(); ++it) ++cnt;
    test_eq( cnt, 4 );
    test_pass("cxx03:string::begin");
    test_true( 1 );
    test_pass("cxx03:string::end");
    test_eq( *s.rbegin(), 'd' );
    test_pass("cxx03:string::rbegin");
    test_eq( *(s.rend() - 1), 'a' );
    test_pass("cxx03:string::rend");

    STD::string a = "aaa", b = "bbb";
    a.swap(b);
    test_eq( a, STD::string("bbb") );
    test_pass("cxx03:string::swap");
    STD::swap(a, b);
    test_eq( a, STD::string("aaa") );
    test_pass("cxx03:swap(string)");

    test_true( STD::string("abc") == STD::string("abc") );
    test_pass("cxx03:operator==(string)");
    test_true( STD::string("abc") != STD::string("abd") );
    test_pass("cxx03:operator!=(string)");
    test_true( STD::string("abc") <  STD::string("abd") );
    test_pass("cxx03:operator<(string)");
    test_true( STD::string("abd") >  STD::string("abc") );
    test_pass("cxx03:operator>(string)");
    test_true( STD::string("abc") <= STD::string("abc") );
    test_pass("cxx03:operator<=(string)");
    test_true( STD::string("abc") >= STD::string("abc") );
    test_pass("cxx03:operator>=(string)");
}

TEST_CASE(string, to_string_sto) {
    test_eq( STD::to_string(123), STD::string("123") );
    test_eq( STD::to_string(-45), STD::string("-45") );
    test_eq( STD::to_string((long)1000000), STD::string("1000000") );
    test_eq( STD::to_string((unsigned)7), STD::string("7") );
    test_pass("cxx11:to_string");
    test_true( STD::to_wstring(42) == STD::wstring(L"42") );
    test_true( STD::to_wstring(-45) == STD::wstring(L"-45") );
    test_true( STD::to_wstring((unsigned)7) == STD::wstring(L"7") );
    test_true( STD::to_wstring((long)1000000) == STD::wstring(L"1000000") );
    test_true( STD::to_wstring((long long)9000000000LL) == STD::wstring(L"9000000000") );

#if defined(__cpp_lib_to_string) && __cpp_lib_to_string >= 202306L
    test_true( STD::to_wstring(1.5) == STD::wstring(L"1.5") );
#else
    test_true( STD::to_wstring(1.5) == STD::wstring(L"1.500000") );
#endif
    test_pass("cxx11:to_wstring");

    test_eq( STD::stoi("42"), 42 );
    test_eq( STD::stoi("-17"), -17 );
    test_pass("cxx11:stoi");
    test_eq( STD::stol("100000"), 100000L );
    test_pass("cxx11:stol");
    test_eq( STD::stoul("4000000000"), 4000000000UL );
    test_pass("cxx11:stoul");
    test_eq( STD::stoll("9000000000"), (long long)9000000000LL );
    test_pass("cxx11:stoll");
    test_eq( STD::stoull("18000000000"), (unsigned long long)18000000000ULL );
    test_pass("cxx11:stoull");
    test_eq( STD::stoi("ff", 0, 16), 255 );
    test_pass("cxx11:stoi (base)");

    STD::size_t pos = 0;
    test_eq( STD::stoi("123abc", &pos), 123 );
    test_eq( pos, 3u );
    test_pass("cxx11:stoi (pos)");

    test_true( STD::stod("3.5") == 3.5 );
    test_pass("cxx11:stod");
    test_true( STD::stof("1.5") == 1.5f );
    test_pass("cxx11:stof");
    test_true( STD::stold("2.5") == 2.5L );
    test_pass("cxx11:stold");
    test_throw( (void)STD::stoi("xyz") );
#if TEST_HAS_EH
    {
        bool ivt = false;
        try { (void)STD::stoi("xyz"); } catch (STD::invalid_argument&) { ivt = true; } catch (...) {}
        test_true( ivt );
        bool oor = false;
        try { (void)STD::stoi("99999999999999999999"); } catch (STD::out_of_range&) { oor = true; } catch (...) {}
        test_true( oor );
        bool oorl = false;
        try { (void)STD::stol("99999999999999999999"); } catch (STD::out_of_range&) { oorl = true; } catch (...) {}
        test_true( oorl );
        bool oord = false;
        try { (void)STD::stod("1e999999"); } catch (STD::out_of_range&) { oord = true; } catch (...) {}
        test_true( oord );
    }
#else
    TEST_SKIP_N(4);
    test_true( true ); test_true( true ); test_true( true ); test_true( true );
#endif
    test_pass("cxx11:stoi (invalid_argument)");
}

namespace {
long g_str_allocs = 0;
template <class T> struct SSOAlloc {
    typedef T value_type; typedef T* pointer; typedef const T* const_pointer;
    typedef T& reference; typedef const T& const_reference;
    typedef STD::size_t size_type; typedef STD::ptrdiff_t difference_type;
    template <class U> struct rebind { typedef SSOAlloc<U> other; };
    SSOAlloc() {} template <class U> SSOAlloc(const SSOAlloc<U>&) {}
    pointer allocate(size_type n) { ++g_str_allocs; return (pointer)::operator new(n * sizeof(T)); }
    void deallocate(pointer p, size_type) { ::operator delete((void*)p); }
    size_type max_size() const { return (size_type)-1 / sizeof(T); }
    void construct(pointer p, const T& v) { *p = v; }
    void destroy(pointer) {}
    bool operator==(const SSOAlloc&) const { return true; }
    bool operator!=(const SSOAlloc&) const { return false; }
};
typedef STD::basic_string<char, STD::char_traits<char>, SSOAlloc<char> > SStr;
}

#if defined(__WATCOMC__)
TEST_CASE(string, libcxx03_sso_layout) {
    test_eq( sizeof(STD::string), 3 * sizeof(void*) );
    test_eq( sizeof(STD::u16string), 3 * sizeof(void*) );
    test_eq( sizeof(STD::u32string), 3 * sizeof(void*) );

    STD::string e;
    test_eq( e.capacity(), 3 * sizeof(void*) - 2 );
}
#else
TEST_CASE_SKIP(string, libcxx03_sso_layout)
#endif

TEST_CASE(string, small_string_optimization) {

    SStr probe;
    const STD::size_t __cap = probe.capacity();
    test_true( __cap > 0 );

    long __base;
    g_str_allocs = 0;
    { SStr z; (void)z.size(); }
    __base = g_str_allocs;

    g_str_allocs = 0;
    {
        SStr a(__cap, 'a');
        test_eq( a.size(), __cap );
        test_true( a.capacity() >= __cap );
    }
    test_eq( g_str_allocs, __base );

    g_str_allocs = 0;
    {
        SStr a(__cap + 1, 'b');
        test_eq( a.size(), __cap + 1 );
    }
    test_true( g_str_allocs > __base );

    {
        SStr a("abc");
        for (int i = 0; i < 40; ++i) a.push_back('x');
        test_eq( a.size(), 43u );
        test_true( a[0] == 'a' && a[1] == 'b' && a[2] == 'c' && a[42] == 'x' );
        test_pass("cxx11:string::push_back");
    }

#if _TST_HAS_CXX11_LIB_MEMBERS
    {
        SStr a(40, 'z');
        a.resize(2);
        a.shrink_to_fit();
        test_eq( a.size(), 2u );
        test_true( a == SStr("zz") );
        test_eq( a.capacity(), __cap );
        test_pass("cxx11:string::shrink_to_fit");
    }
#else
    test_skip("cxx11:string::shrink_to_fit");
#endif

    {
        SStr s("hi");
        SStr l("a string that is definitely longer than the sso threshold");
        s.swap(l);
        test_true( s == SStr("a string that is definitely longer than the sso threshold") );
        test_true( l == SStr("hi") );
    }
}

namespace {
STD::size_t __ro_fill(char* __p, STD::size_t __n) {
    for (STD::size_t __i = 0; __i < __n; ++__i) __p[__i] = (char)('a' + __i % 26);
    return __n >= 2 ? __n - 2 : __n;
}
}
#if _TST_HAS_CHAR16_32
TEST_CASE(string, u16_u32_and_resize_overwrite) {
    STD::u16string a;
    a.push_back((char16_t)'h'); a.push_back((char16_t)'i');
    test_eq( a.size(), 2u );
    test_true( a[0] == (char16_t)'h' && a[1] == (char16_t)'i' );
    char16_t __lit[4] = { (char16_t)'a', (char16_t)'b', (char16_t)'c', 0 };
    STD::u16string b(__lit);
    test_eq( b.size(), 3u );
    for (int __i = 0; __i < 20; ++__i) b.push_back((char16_t)'x');
    test_eq( b.size(), 23u );
    test_true( b[0] == (char16_t)'a' && b[22] == (char16_t)'x' );
    test_pass("cxx11:u16string");

    STD::u32string c;
    c.push_back((char32_t)0x1F600);
    test_eq( c.size(), 1u );
    test_true( c[0] == (char32_t)0x1F600 );
    test_pass("cxx11:u32string");

#if defined(__cpp_lib_string_resize_and_overwrite)
    STD::string s;
    s.resize_and_overwrite(10, __ro_fill);
    test_eq( s.size(), 8u );
    test_true( s[0] == 'a' && s[7] == 'h' );
    test_eq( s.c_str()[8], '\0' );
    test_pass("cxx23:string::resize_and_overwrite");
#else
    (void)__ro_fill; TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx23:string::resize_and_overwrite");
#endif
}
#else
TEST_CASE_SKIP(string, u16_u32_and_resize_overwrite)
#endif

TEST_CASE(string, assign_allocator) {
    STD::string b("hello");

    STD::string aa((STD::allocator<char>()));
    test_true( aa.empty() );
    test_pass("cxx03:basic_string(const Allocator&)");

    STD::string sp(b, 2);
    test_eq( sp, STD::string("llo") );
    test_pass("cxx03:basic_string(str, pos)");

    STD::allocator<char> al = b.get_allocator();
    (void)al;
    test_pass("cxx03:string::get_allocator");

    STD::string s;
    s = b;                                       test_eq( s, b );
    test_pass("cxx03:string::operator=(const basic_string&)");
    s = "world";                                 test_eq( s, STD::string("world") );
    test_pass("cxx03:string::operator=(const char*)");
    s = 'Z';                                     test_eq( s, STD::string("Z") );
    test_pass("cxx03:string::operator=(char)");
}

TEST_CASE(string, stream_io) {
    STD::ostringstream os;
    os << STD::string("hello");
    test_eq( os.str(), STD::string("hello") );
    test_pass("cxx03:operator<<(string)");

    STD::istringstream is("alpha beta");
    STD::string w;
    is >> w;
    test_eq( w, STD::string("alpha") );
    test_pass("cxx03:operator>>(string)");

    STD::istringstream is2("line1\nline2");
    STD::string ln;
    STD::getline(is2, ln);
    test_eq( ln, STD::string("line1") );
    test_pass("cxx03:getline(string)");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(string, element_access_c11) {
    STD::string b("hello");
    test_eq( b.front(), 'h' );
    test_pass("cxx11:string::front");
    test_eq( b.back(), 'o' );
    test_pass("cxx11:string::back");
    b.pop_back();
    test_eq( b, STD::string("hell") );
    test_pass("cxx11:string::pop_back");

    STD::string c("abc");
    test_eq( *c.cbegin(), 'a' );
    test_pass("cxx11:string::cbegin");
    test_eq( (int)(c.cend() - c.cbegin()), 3 );
    test_pass("cxx11:string::cend");
    test_eq( *c.crbegin(), 'c' );
    test_pass("cxx11:string::crbegin");
    test_true( c.crbegin() != c.crend() );
    test_pass("cxx11:string::crend");
}
#else
TEST_CASE_SKIP(string, element_access_c11)
#endif

#if _TST_HAS_RVALUE_REF && _TST_HAS_INIT_LIST && _TST_HAS_CHAR16_32
TEST_CASE(string, move_ilist_hash) {

    STD::string a("hello");
    STD::string b(STD::move(a));
    test_true( b == "hello" );
    test_pass("cxx11:basic_string(basic_string&&)");
    STD::string c("foo"), d;
    d = STD::move(c);
    test_true( d == "foo" );
    test_pass("cxx11:string::operator=(basic_string&&)");
    STD::string e("bar"), f;
    f.assign(STD::move(e));
    test_true( f == "bar" );
    test_pass("cxx11:string::assign(basic_string&&)");

    STD::string il = { 'a', 'b', 'c' };
    test_true( il == "abc" );
    test_pass("cxx11:basic_string(initializer_list)");
    il = { 'x', 'y' };
    test_true( il == "xy" );
    test_pass("cxx11:string::operator=(initializer_list)");
    il.append({ 'z', 'w' });
    test_true( il == "xyzw" );
    test_pass("cxx11:string::append(initializer_list)");
    il.assign({ 'p', 'q' });
    test_true( il == "pq" );
    test_pass("cxx11:string::assign(initializer_list)");
    il.insert(il.begin() + 1, { 'r', 's' });
    test_true( il == "prsq" );
    test_pass("cxx11:string::insert(initializer_list)");

    STD::string src("copyme");
    STD::string ax(src, STD::allocator<char>());
    STD::string ax2("lit", STD::allocator<char>());
    test_true( ax == "copyme" && ax2 == "lit" );
    test_pass("cxx11:basic_string allocator-extended ctors");

    STD::string line;
    STD::getline(STD::istringstream("one\ntwo"), line);
    test_true( line == "one" );
    test_pass("cxx11:getline(istream&&, string)");

    test_true( STD::hash<STD::string>()(STD::string("k")) == STD::hash<STD::string>()(STD::string("k")) );
    test_pass("cxx11:hash<string>");
    test_true( STD::hash<STD::wstring>()(STD::wstring(L"k")) == STD::hash<STD::wstring>()(STD::wstring(L"k")) );
    test_pass("cxx11:hash<wstring>");
    test_true( STD::hash<STD::u16string>()(STD::u16string()) == STD::hash<STD::u16string>()(STD::u16string()) );
    test_pass("cxx11:hash<u16string>");
    test_true( STD::hash<STD::u32string>()(STD::u32string()) == STD::hash<STD::u32string>()(STD::u32string()) );
    test_pass("cxx11:hash<u32string>");
}
#else
TEST_CASE(string, move_ilist_hash) {
    test_skip("cxx11:basic_string(basic_string&&)"); test_skip("cxx11:string::operator=(basic_string&&)");
    test_skip("cxx11:string::assign(basic_string&&)"); test_skip("cxx11:basic_string(initializer_list)");
    test_skip("cxx11:string::operator=(initializer_list)"); test_skip("cxx11:string::append(initializer_list)");
    test_skip("cxx11:string::assign(initializer_list)"); test_skip("cxx11:string::insert(initializer_list)");
    test_skip("cxx11:basic_string allocator-extended ctors"); test_skip("cxx11:getline(istream&&, string)");
    test_skip("cxx11:hash<string>"); test_skip("cxx11:hash<wstring>");
    test_skip("cxx11:hash<u16string>"); test_skip("cxx11:hash<u32string>");
}
#endif

TEST_CASE(string, string_udl) {
#if defined(__cpp_lib_string_udls)
    using namespace STD::string_literals;
    STD::string s = "hi"s;
    test_true( s == "hi" && s.size() == 2 );
    test_pass("cxx14:literal operator s-suffix (std::string UDL)");
#else
    test_skip("cxx14:literal operator s-suffix (std::string UDL)");
#endif
}

TEST_CASE(string, string_view_interop_c17) {
#if defined(__cpp_lib_string_view)
    STD::string_view sv("hello");
    STD::string s(sv);
    test_true( s == "hello" );
    test_pass("cxx17:basic_string(const T&) [string_view-like]");
    STD::string s2(sv, 1, 3);
    test_true( s2 == "ell" );
    test_pass("cxx17:basic_string(const T&, pos, n) [string_view-like]");
    STD::string s3;
    s3 = sv;
    test_true( s3 == "hello" );
    test_pass("cxx17:string::operator=(const T&) [string_view-like]");
    STD::string_view v2 = s;
    test_true( v2 == "hello" );
    test_pass("cxx17:string::operator basic_string_view");
    STD::string s4("abc");
    char* dp = s4.data();
    dp[0] = 'X';
    test_true( s4 == "Xbc" );
    test_pass("cxx17:string::data() (non-const)");
#else
    test_skip("cxx17:basic_string(const T&) [string_view-like]");
    test_skip("cxx17:basic_string(const T&, pos, n) [string_view-like]");
    test_skip("cxx17:string::operator=(const T&) [string_view-like]");
    test_skip("cxx17:string::operator basic_string_view");
    test_skip("cxx17:string::data() (non-const)");
#endif

#if defined(__cpp_lib_memory_resource)
    STD::pmr::string ps("pmr-hi");
    test_true( ps == "pmr-hi" );
    test_pass("cxx17:pmr::string");
#else
    test_skip("cxx17:pmr::string");
#endif
}

TEST_CASE(string, string_cxx20) {
#if defined(__cpp_lib_starts_ends_with)
    STD::string s("hello");
    test_true(  s.starts_with("he") );
    test_true( !s.starts_with("xy") );
    test_pass("cxx20:string::starts_with");
    test_true(  s.ends_with("lo") );
    test_true( !s.ends_with("xy") );
    test_pass("cxx20:string::ends_with");
#else
    test_skip("cxx20:string::starts_with");
    test_skip("cxx20:string::ends_with");
#endif

#if defined(__cpp_lib_erase_if)
    STD::string e("hello");
    STD::erase(e, 'l');
    test_true( e == "heo" );
    test_pass("cxx20:erase(string)");
    STD::string e2("banana");
    STD::erase_if(e2, [](char ch){ return ch == 'a'; });
    test_true( e2 == "bnn" );
    test_pass("cxx20:erase_if(string)");
#else
    test_skip("cxx20:erase(string)");
    test_skip("cxx20:erase_if(string)");
#endif

#if defined(__cpp_lib_three_way_comparison)
    STD::string lo("abc"), hi("abd");
    test_true( (lo <=> hi) < 0 );
    test_true( (lo <=> lo) == 0 );
    test_pass("cxx20:operator<=>(string)");
#else
    test_skip("cxx20:operator<=>(string)");
#endif

#if _TST_HAS_CHAR8_NAME
#  if _TST_HAS_CHAR8_T
    STD::u8string u8s = u8"abc";
#  else
    const char8_t __u8lit[4] = { (char8_t)'a', (char8_t)'b', (char8_t)'c', 0 };
    STD::u8string u8s(__u8lit);
#  endif
    test_true( u8s.size() == 3 && u8s[0] == (char8_t)'a' && u8s[2] == (char8_t)'c' );
    test_pass("cxx20:u8string");
#else
    test_skip("cxx20:u8string");
#endif
}

TEST_CASE(string, string_cxx23) {
#if defined(__cpp_lib_string_contains)
    STD::string s("hello");
    test_true(  s.contains("ell") );
    test_true( !s.contains("xyz") );
    test_pass("cxx23:string::contains");
#else
    test_skip("cxx23:string::contains");
#endif

#if defined(__cpp_lib_containers_ranges)
    int chs[3] = { 'a', 'b', 'c' };
    STD::string fr(STD::from_range, chs);
    test_true( fr == "abc" );
    test_pass("cxx23:basic_string(from_range_t, R&&)");
    STD::string ar("X");
    ar.append_range(chs);
    test_true( ar == "Xabc" );
    test_pass("cxx23:string::append_range");
    STD::string as("Y");
    as.assign_range(chs);
    test_true( as == "abc" );
    test_pass("cxx23:string::assign_range");
    STD::string ins("PQ");
    ins.insert_range(ins.begin() + 1, chs);
    test_true( ins == "PabcQ" );
    test_pass("cxx23:string::insert_range");
    STD::string rep("12345");
    rep.replace_with_range(rep.begin() + 1, rep.begin() + 4, chs);
    test_true( rep == "1abc5" );
    test_pass("cxx23:string::replace_with_range");
#else
    test_skip("cxx23:basic_string(from_range_t, R&&)");
    test_skip("cxx23:string::append_range");
    test_skip("cxx23:string::assign_range");
    test_skip("cxx23:string::insert_range");
    test_skip("cxx23:string::replace_with_range");
#endif

    test_skip("cxx23:basic_string(nullptr_t)=delete");
    test_skip("cxx23:string::operator=(nullptr_t)=delete");
}


TEST_CASE(string, member_types_cxx03) {
    typedef STD::basic_string<char> S;
    S s("hello");
    const S& cs = s;

    test_true(( STD::is_same<S::value_type, char>::value ));
    test_true(( STD::is_same<STD::basic_string<wchar_t>::value_type, wchar_t>::value ));
    test_pass("cxx03:basic_string::value_type");

    S::reference r = s[0];
    r = 'H';
    test_true( s == "Hello" );
    test_true(( STD::is_same<S::reference, char&>::value ));
    test_pass("cxx03:basic_string::reference");

    S::pointer p = &s[0];
    test_eq( *p, 'H' );
    test_true(( STD::is_same<S::pointer, char*>::value ));
    test_pass("cxx03:basic_string::pointer");

    S::size_type n = s.size();
    test_eq( n, (STD::size_t)5 );
    test_true( !STD::is_signed<S::size_type>::value );
    test_true( S::npos == (S::size_type)-1 );
    test_pass("cxx03:basic_string::size_type");

    S::iterator it = s.begin();
    *it = 'J';
    test_eq( s[0], 'J' );
    test_pass("cxx03:basic_string::iterator");

    S::const_iterator ci = cs.begin();
    test_eq( *ci, 'J' );
    test_pass("cxx03:basic_string::const_iterator");

    S::reverse_iterator ri = s.rbegin();
    test_eq( *ri, 'o' );
    *ri = 'O';
    test_eq( s[4], 'O' );
    test_pass("cxx03:basic_string::reverse_iterator");

    test_true( STD::char_traits<char>::eq('a', 'a') );
    test_true( !STD::char_traits<char>::lt('b', 'a') );
    test_eq( (int)STD::char_traits<char>::length("abc"), 3 );
    test_eq( STD::char_traits<char>::compare("abc", "abd", 3) < 0, true );
    test_true( STD::char_traits<wchar_t>::eq(L'x', L'x') );
    test_pass("cxx03:char_traits");
}

TEST_CASE(string, member_types_cxx11) {
    typedef STD::basic_string<char> S;
    S s("hello");
    const S& cs = s;

    test_true(( STD::is_same<S::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx03:basic_string::traits_type");

    test_true(( STD::is_same<S::allocator_type, STD::allocator<char> >::value ));
    S::allocator_type al = s.get_allocator();
    (void)al;
    test_pass("cxx03:basic_string::allocator_type");

    S::const_reference cr = cs[1];
    test_eq( cr, 'e' );
    test_true(( STD::is_same<S::const_reference, const char&>::value ));
    test_pass("cxx03:basic_string::const_reference");

    S::const_pointer cp = cs.data();
    test_eq( *cp, 'h' );
    test_true(( STD::is_same<S::const_pointer, const char*>::value ));
    test_pass("cxx03:basic_string::const_pointer");

    S::const_reverse_iterator cri = cs.rbegin();
    test_eq( *cri, 'o' );
    test_pass("cxx03:basic_string::const_reverse_iterator");

    test_true( STD::is_signed<S::difference_type>::value );
    S::difference_type d = s.end() - s.begin();
    test_eq( (long)d, 5L );
    test_pass("cxx03:basic_string::difference_type");
}

TEST_CASE(string, subview_cxx26) {
#if defined(__cpp_lib_string_view_subview)
    STD::string s("hello");
    test_true( s.subview(1, 3) == "ell" );
    test_true( s.subview(2) == "llo" );
    test_pass("cxx26:basic_string::subview");
#else
    TEST_NOTE("basic_string::subview is C++26; this library does not have it yet");
    test_skip("cxx26:basic_string::subview");
#endif
}

TEST_CASE(string, feature_test_macros) {
#if defined(__cpp_lib_starts_ends_with) && (__cpp_lib_starts_ends_with) >= 201711L
    test_true( (__cpp_lib_starts_ends_with) >= 201711L );
    test_pass("cxx20:__cpp_lib_starts_ends_with=201711L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_starts_ends_with=201711L");
#endif
#if defined(__cpp_lib_string_contains) && (__cpp_lib_string_contains) >= 202011L
    test_true( (__cpp_lib_string_contains) >= 202011L );
    test_pass("cxx23:__cpp_lib_string_contains=202011L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_string_contains=202011L");
#endif
#if defined(__cpp_lib_containers_ranges) && (__cpp_lib_containers_ranges) >= 202202L
    test_true( (__cpp_lib_containers_ranges) >= 202202L );
    test_pass("cxx23:__cpp_lib_containers_ranges=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_containers_ranges=202202L");
#endif
#if defined(__cpp_lib_string_view) && (__cpp_lib_string_view) >= 202403L
    test_true( (__cpp_lib_string_view) >= 202403L );
    test_pass("cxx26:__cpp_lib_string_view=202403L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_string_view=202403L");
#endif
}

TEST_CASE(string, insert_replace_const_iterator_cxx11) {
#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::string s = "abc";
    STD::string::const_iterator cp = s.begin();
    STD::string::iterator r = s.insert(cp + 1, 'X');
    test_eq( s, STD::string("aXbc") );
    test_eq( *r, 'X' );
    test_pass("cxx11:string::insert(const_iterator p, charT c)");

    cp = s.begin();
    r = s.insert(cp, 2, '#');
    test_eq( s, STD::string("##aXbc") );
    test_eq( *r, '#' );
    test_pass("cxx11:string::insert(const_iterator p, size_type n, charT c)");

    const char* src = "pq";
    cp = s.end();
    r = s.insert(cp, src, src + 2);
    test_eq( s, STD::string("##aXbcpq") );
    test_eq( *r, 'p' );
    test_pass("cxx11:string::insert(const_iterator p, InputIterator first, InputIterator last)");

    STD::string t = "abcdef";
    STD::string::const_iterator c1 = t.begin();
    STD::string::const_iterator c2 = t.begin() + 2;
    t.replace(c1, c2, STD::string("--"));
    test_eq( t, STD::string("--cdef") );
    test_pass("cxx11:string::replace(const_iterator i1, const_iterator i2, const basic_string& str)");

    c1 = t.begin(); c2 = t.begin() + 2;
    t.replace(c1, c2, "==", 2u);
    test_eq( t, STD::string("==cdef") );
    test_pass("cxx11:string::replace(const_iterator i1, const_iterator i2, const charT* s, size_type n)");

    c1 = t.begin(); c2 = t.begin() + 2;
    t.replace(c1, c2, "@");
    test_eq( t, STD::string("@cdef") );
    test_pass("cxx11:string::replace(const_iterator i1, const_iterator i2, const charT* s)");

    c1 = t.begin(); c2 = t.begin() + 1;
    t.replace(c1, c2, 2, '%');
    test_eq( t, STD::string("%%cdef") );
    test_pass("cxx11:string::replace(const_iterator i1, const_iterator i2, size_type n, charT c)");

    const char* rs = "ab";
    c1 = t.begin(); c2 = t.begin() + 2;
    t.replace(c1, c2, rs, rs + 2);
    test_eq( t, STD::string("abcdef") );
    test_pass("cxx11:string::replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last)");
#else
    TEST_NOTE("the const_iterator forms of insert/replace are C++11");
    TEST_SKIP_N(11);
    test_skip("cxx11:string::insert(const_iterator p, charT c)");
    test_skip("cxx11:string::insert(const_iterator p, size_type n, charT c)");
    test_skip("cxx11:string::insert(const_iterator p, InputIterator first, InputIterator last)");
    test_skip("cxx11:string::replace(const_iterator i1, const_iterator i2, const basic_string& str)");
    test_skip("cxx11:string::replace(const_iterator i1, const_iterator i2, const charT* s, size_type n)");
    test_skip("cxx11:string::replace(const_iterator i1, const_iterator i2, const charT* s)");
    test_skip("cxx11:string::replace(const_iterator i1, const_iterator i2, size_type n, charT c)");
    test_skip("cxx11:string::replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last)");
#endif
}

TEST_CASE(string, constexpr_members_cxx20) {
#if defined(__cpp_lib_constexpr_string) && (__cpp_lib_constexpr_string) >= 201907L
    { constexpr bool ok = []() -> bool {
        STD::string s("abc"); s.append(STD::string("wxyz"), 1, 2); return s == "abcxy"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::append(const basic_string& str, size_type pos, size_type n)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abc"); const char* p = "de"; s.append(p, p + 2); return s == "abcde"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::append(InputIterator first, InputIterator last)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abc"); s.assign(STD::string("wxyz"), 1, 2); return s == "xy"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::assign(const basic_string& str, size_type pos, size_type n)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abc"); const char* p = "de"; s.assign(p, p + 2); return s == "de"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::assign(InputIterator first, InputIterator last)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcdef"); return s.compare(1, 2, STD::string("xbcx"), 1, 2) == 0; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::compare(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcdef"); return s.compare(1, 2, "xbcx" + 1, 2) == 0; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::compare(size_type pos1, size_type n1, const charT* s, size_type n2)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcab"); return s.find_last_not_of('b') == 3u; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::find_last_not_of(charT c)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcab"); return s.find_last_not_of("bxz", 4, 1) == 3u; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::find_last_not_of(const charT* s, size_type pos, size_type n)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abc"); STD::string::const_iterator p = s.begin();
        return *s.insert(p + 1, 'X') == 'X' && s == "aXbc"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::insert(const_iterator p, charT c)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abc"); const char* p = "pq";
        s.insert(s.cend(), p, p + 2); return s == "abcpq"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::insert(const_iterator p, InputIterator first, InputIterator last)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abc"); s.insert(s.cbegin(), 2, '#'); return s == "##abc"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::insert(const_iterator p, size_type n, charT c)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abc"); s.insert(STD::string::size_type(0), 3, '-'); return s == "---abc"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::insert(size_type pos, size_type n, charT c)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abc"); s.insert(1, STD::string("XYZ"), 1, 1); return s == "aYbc"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::insert(size_type pos1, const basic_string& str, size_type pos2, size_type n)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcdef"); s.replace(s.cbegin(), s.cbegin() + 2, STD::string("--"));
        return s == "--cdef"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, const basic_string& str)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcdef"); s.replace(s.cbegin(), s.cbegin() + 2, "==", 2u);
        return s == "==cdef"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, const charT* s, size_type n)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcdef"); s.replace(s.cbegin(), s.cbegin() + 2, "@");
        return s == "@cdef"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, const charT* s)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcdef"); const char* p = "yz";
        s.replace(s.cbegin(), s.cbegin() + 2, p, p + 2); return s == "yzcdef"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcdef"); s.replace(s.cbegin(), s.cbegin() + 1, 2, '%');
        return s == "%%bcdef"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, size_type n, charT c)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcdef"); s.replace(0, 1, STD::string("XYZ"), 1, 2); return s == "YZbcdef"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::replace(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2)");

    { constexpr bool ok = []() -> bool {
        STD::string s("abcdef"); s.replace(1, 2, 3, '*'); return s == "a***def"; }();
      test_true( ok ); }
    test_pass("cxx20:constexpr string::replace(size_type pos1, size_type n1, size_type n2, charT c)");
#else
    TEST_NOTE("constexpr basic_string is C++20 (__cpp_lib_constexpr_string)");
    TEST_SKIP_N(20);
    test_skip("cxx20:constexpr string::append(const basic_string& str, size_type pos, size_type n)");
    test_skip("cxx20:constexpr string::append(InputIterator first, InputIterator last)");
    test_skip("cxx20:constexpr string::assign(const basic_string& str, size_type pos, size_type n)");
    test_skip("cxx20:constexpr string::assign(InputIterator first, InputIterator last)");
    test_skip("cxx20:constexpr string::compare(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2)");
    test_skip("cxx20:constexpr string::compare(size_type pos1, size_type n1, const charT* s, size_type n2)");
    test_skip("cxx20:constexpr string::find_last_not_of(charT c)");
    test_skip("cxx20:constexpr string::find_last_not_of(const charT* s, size_type pos, size_type n)");
    test_skip("cxx20:constexpr string::insert(const_iterator p, charT c)");
    test_skip("cxx20:constexpr string::insert(const_iterator p, InputIterator first, InputIterator last)");
    test_skip("cxx20:constexpr string::insert(const_iterator p, size_type n, charT c)");
    test_skip("cxx20:constexpr string::insert(size_type pos, size_type n, charT c)");
    test_skip("cxx20:constexpr string::insert(size_type pos1, const basic_string& str, size_type pos2, size_type n)");
    test_skip("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, const basic_string& str)");
    test_skip("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, const charT* s, size_type n)");
    test_skip("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, const charT* s)");
    test_skip("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last)");
    test_skip("cxx20:constexpr string::replace(const_iterator i1, const_iterator i2, size_type n, charT c)");
    test_skip("cxx20:constexpr string::replace(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2)");
    test_skip("cxx20:constexpr string::replace(size_type pos1, size_type n1, size_type n2, charT c)");
#endif
}

#if defined(__cpp_lib_to_string) && __cpp_lib_to_string >= 202306L
#define _TST_TOSTR_15  "1.5"
#define _TST_TOWSTR_15 L"1.5"
#else
#define _TST_TOSTR_15  "1.500000"
#define _TST_TOWSTR_15 L"1.500000"
#endif

TEST_CASE(string, to_string_overloads_cxx11) {
    test_eq( STD::to_string((int)123), STD::string("123") );
    test_pass("cxx11:to_string(int val)");
    test_eq( STD::to_string((unsigned)7), STD::string("7") );
    test_pass("cxx11:to_string(unsigned val)");
    test_eq( STD::to_string((long)1000000), STD::string("1000000") );
    test_pass("cxx11:to_string(long val)");
    test_eq( STD::to_string((unsigned long)42), STD::string("42") );
    test_pass("cxx11:to_string(unsigned long val)");
    test_eq( STD::to_string((long long)9000000000LL), STD::string("9000000000") );
    test_pass("cxx11:to_string(long long val)");
    test_eq( STD::to_string((unsigned long long)18), STD::string("18") );
    test_pass("cxx11:to_string(unsigned long long val)");
    test_true( STD::to_string(1.5f) == STD::string(_TST_TOSTR_15) );
    test_pass("cxx11:to_string(float val)");
    test_true( STD::to_string(1.5) == STD::string(_TST_TOSTR_15) );
    test_pass("cxx11:to_string(double val)");
    test_true( STD::to_string((long double)1.5) == STD::string(_TST_TOSTR_15) );
    test_pass("cxx11:to_string(long double val)");
    test_true( STD::to_wstring((int)123) == STD::wstring(L"123") );
    test_pass("cxx11:to_wstring(int val)");
    test_true( STD::to_wstring((unsigned)7) == STD::wstring(L"7") );
    test_pass("cxx11:to_wstring(unsigned val)");
    test_true( STD::to_wstring((long)1000000) == STD::wstring(L"1000000") );
    test_pass("cxx11:to_wstring(long val)");
    test_true( STD::to_wstring((unsigned long)42) == STD::wstring(L"42") );
    test_pass("cxx11:to_wstring(unsigned long val)");
    test_true( STD::to_wstring((long long)9000000000LL) == STD::wstring(L"9000000000") );
    test_pass("cxx11:to_wstring(long long val)");
    test_true( STD::to_wstring((unsigned long long)18) == STD::wstring(L"18") );
    test_pass("cxx11:to_wstring(unsigned long long val)");
    test_true( STD::to_wstring(1.5f) == STD::wstring(_TST_TOWSTR_15) );
    test_pass("cxx11:to_wstring(float val)");
    test_true( STD::to_wstring(1.5) == STD::wstring(_TST_TOWSTR_15) );
    test_pass("cxx11:to_wstring(double val)");
    test_true( STD::to_wstring((long double)1.5) == STD::wstring(_TST_TOWSTR_15) );
    test_pass("cxx11:to_wstring(long double val)");
}
