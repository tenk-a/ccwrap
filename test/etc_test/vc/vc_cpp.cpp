#include <cstddef>
#include <iso646.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <stdbit.h>
#include <stdckdint.h>
#include <uchar.h>

#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cuchar>
#include <cwchar>
#include <cwctype>
#include <cstdalign>
#include <cinttypes>

#include <array>
#include <bit>
#include <memory>
#include <string_view>
#include <type_traits>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <string>
#include <utility>
#include <functional>
#include <tuple>
#include <optional>
#include <variant>
#include <charconv>
#include <span>
#include <numbers>
#include <expected>
#include <filesystem>
#include <fstream>
#include <version>
#include <ratio>
#include <random>
#include <format>
#include <ranges>
#include <any>
#include <print>
#include <cstdio>
#include <spanstream>
#include <sstream>
#include <memory_resource>
#include <vector>
#include <flat_set>
#include <flat_map>
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <shared_mutex>
#include <syncstream>
#define _TST_TEST_HAS_MUTEX 1

struct alignas(8) test_align {
    int value;
};

static_assert(alignof(test_align) >= 8, "alignof failed");

static bool is_even(int x) { return x % 2 == 0; }

struct sum_fn {
    int* sum;
    void operator()(int x) const { *sum += x; }
};

struct calc_t {
    int add(int a, int b) const { return a + b; }
};

struct esft_t : std::enable_shared_from_this<esft_t> {
    int v;
    esft_t() : v(0) {}
};

enum color_t { color_red = 1, color_green = 2, color_blue = 7 };

struct ab_t { int a; int b; };
static int square_fn(int x) { return x * x; }

static int g_co_calls = 0;
static void co_fn() { ++g_co_calls; }

static int g_thr_val = 0;
static void thr_fn(int x) { g_thr_val = x; }

static bool pred_true() { return true; }

int main()
{
    test_align v = { 1 };
    bool ok = true and not false;
    if (!ok || v.value != 1)
        return 1;

    {
        unsigned int bits = 0x28u;
        int checked = 0;
        char8_t  c8  = (char8_t)'A';
        char16_t c16 = (char16_t)0x3042u;
        char32_t c32 = (char32_t)0x1f600u;

        if (std::stdc_count_ones(bits) != 2)
            return 2;
        if (std::stdc_bit_width_ui(bits) != 6)
            return 3;
        if (std::ckd_add(&checked, 100, 23) || checked != 123)
            return 4;
        if (!std::ckd_add(&checked, (int)INT32_MAX, 1))
            return 5;
        if (sizeof(c8) != 1 || sizeof(c16) < 2 || sizeof(c32) < 4)
            return 6;
        if ((unsigned char)c8 != 'A')
            return 7;
        (void)c16;
        (void)c32;
    }

    {
        if (std::round(2.5) != 3.0 || std::trunc(2.7) != 2.0)
            return 8;
        if (std::fmin(2.0, 3.0) != 2.0 || std::fmax(2.0, 3.0) != 3.0)
            return 9;
        if (!std::isnan(std::nan("")) || std::isnan(1.0))
            return 10;
        if (!std::isinf(INFINITY) || !std::isfinite(1.0))
            return 11;
        if (std::exp2(3.0) != 8.0)
            return 12;
        {
            float f = std::hypotf(3.0f, 4.0f);
            if (f < 4.999f || f > 5.001f)
                return 13;
        }
    }

    {
        char buf[8];
        std::timespec ts;
        if (std::snprintf(buf, sizeof(buf), "%d", 42) != 2)
            return 14;
        if (std::timespec_get(&ts, TIME_UTC) != TIME_UTC || ts.tv_sec <= 0)
            return 15;
    }

    {
        std::array<int, 3> a;
        a.fill(7);
        a[1] = 5;
        if (a.size() != 3 || a.front() != 7 || a[1] != 5 || a.back() != 7)
            return 16;
        if (a.at(2) != 7)
            return 17;
    }

    {
        std::uint32_t x = 0x28u;
        if (std::popcount(x) != 2)
            return 18;
        if (std::bit_width(x) != 6)
            return 19;
        if (!std::has_single_bit(0x80u))
            return 20;
        if (std::countr_zero(x) != 3 || std::countl_zero((std::uint8_t)0x10u) != 3)
            return 21;
        if (std::bit_floor(x) != 0x20u || std::bit_ceil(x) != 0x40u)
            return 22;
        if (std::rotl((std::uint8_t)0x80u, 1) != 0x01u)
            return 23;
        if (std::byteswap((std::uint16_t)0x1234u) != 0x3412u)
            return 24;
        if (std::bit_cast<std::uint32_t>((std::int32_t)-1) != 0xFFFFFFFFu)
            return 25;
    }

    {
        std::unique_ptr<int> p(new int(42));
        if (!p || *p != 42)
            return 26;
        *p = 7;
        if (p.get() == 0 || *p.get() != 7)
            return 27;
        p.reset();
        if (p)
            return 28;
#if !defined(_MSC_VER) || _MSC_VER >= 1600
        {
            std::unique_ptr<int> mu = std::make_unique<int>(99);
            if (!mu || *mu != 99)
                return 66;
        }
#endif
        {
            std::shared_ptr<int> sp = std::make_shared<int>(7);
            if (!sp || *sp != 7 || sp.use_count() != 1)
                return 67;
            std::shared_ptr<int> sp2 = sp;
            if (sp.use_count() != 2 || *sp2 != 7)
                return 68;
            sp2.reset();
            if (sp.use_count() != 1)
                return 69;
            std::shared_ptr<int> sp3(new int(3));
            *sp3 = 5;
            if (*sp3 != 5)
                return 70;
        }
        {
            std::shared_ptr<int> sp = std::make_shared<int>(11);
            std::weak_ptr<int> wp = sp;
            if (wp.expired() || wp.use_count() != 1)
                return 71;
            std::shared_ptr<int> lk = wp.lock();
            if (!lk || *lk != 11 || sp.use_count() != 2)
                return 72;
            sp.reset();
            lk.reset();
            if (!wp.expired() || wp.lock())
                return 73;
        }
        {
            std::shared_ptr<esft_t> e = std::make_shared<esft_t>();
            e->v = 42;
            std::shared_ptr<esft_t> e2 = e->shared_from_this();
            if (!e2 || e2->v != 42 || e.use_count() != 2)
                return 74;
        }
    }

    {
        std::string_view sv("hello world");
        if (sv.size() != 11 || sv[0] != 'h')
            return 29;
        if (sv.substr(0, 5) != std::string_view("hello"))
            return 30;
        if (sv.find("world") != 6)
            return 31;
        if (sv.find('z') != std::string_view::npos)
            return 32;
    }

    {
        if (!std::is_same<int, int>::value || std::is_same<int, long>::value)
            return 33;
        if (!std::is_integral<int>::value || std::is_integral<test_align>::value)
            return 34;
        if (!std::is_class<test_align>::value || std::is_class<int>::value)
            return 35;
        if (!std::is_pointer<int*>::value || std::is_pointer<int>::value)
            return 36;
        typedef std::remove_reference<int&>::type ref_removed;
        if (!std::is_same<ref_removed, int>::value)
            return 37;
    }

    {
        int arr[5] = { 2, 4, 6, 8, 10 };
        int dst[5];
        if (std::begin(arr) != arr || std::end(arr) != arr + 5)
            return 38;
        if (*std::next(std::begin(arr), 2) != 6 || *std::prev(std::end(arr), 1) != 10)
            return 39;
        if (!std::all_of(arr, arr + 5, is_even) || std::none_of(arr, arr + 5, is_even))
            return 40;
        if (!std::any_of(arr, arr + 5, is_even) || std::find_if_not(arr, arr + 5, is_even) != arr + 5)
            return 41;
        std::copy_n(arr, 5, dst);
        if (dst[2] != 6)
            return 42;
        if (std::gcd(12, 18) != 6 || std::lcm(4, 6) != 12)
            return 43;
    }

    {
        int seq[4];
        int total = 0;
        sum_fn adder;
        adder.sum = &total;
        if (std::to_string(123) != "123" || std::to_string(-5) != "-5")
            return 44;
        if (std::stoi("42") != 42 || std::stol("7") != 7L)
            return 45;
        if (std::stoll("100000000000") != 100000000000LL)
            return 46;
        if (std::stod("2.5") != 2.5)
            return 47;
        std::iota(seq, seq + 4, 10);
        if (seq[0] != 10 || seq[3] != 13 || !std::is_sorted(seq, seq + 4))
            return 48;
        if (std::clamp(5, 1, 10) != 5 || std::clamp(15, 1, 10) != 10 || std::clamp(-3, 1, 10) != 1)
            return 49;
        std::for_each_n(seq, 3, adder);
        if (total != 10 + 11 + 12)
            return 50;
    }

    {
        int arr[3] = { 1, 2, 3 };
        if (std::size(arr) != 3 || std::empty(arr))
            return 51;
        if (*std::cbegin(arr) != 1 || std::cend(arr) != arr + 3)
            return 52;
        int x = 10;
        if (std::exchange(x, 20) != 10 || x != 20)
            return 53;
        const int& cr = std::as_const(x);
        if (cr != 20)
            return 54;
        std::array<int, 3> a3;
        a3.fill(9);
        if (std::size(a3) != 3 || *std::data(a3) != 9)
            return 55;
    }

    {
        int v[5] = { 3, 1, 4, 1, 5 };
        int a = 7, b = 2;
        if (std::minmax(a, b).first != 2 || std::minmax(a, b).second != 7)
            return 56;
        std::pair<int*, int*> me = std::minmax_element(v, v + 5);
        if (*me.first != 1 || *me.second != 5)
            return 57;
        if (std::reduce(v, v + 5, 0) != 14 || std::reduce(v, v + 5) != 14)
            return 58;
        if (std::midpoint(10, 20) != 15 || std::midpoint(20, 10) != 15)
            return 59;
    }

    {
        int n = 5;
        std::reference_wrapper<int> r = std::ref(n);
        r.get() = 8;
        if (n != 8)
            return 60;
        std::reference_wrapper<const int> cr = std::cref(n);
        if (cr.get() != 8)
            return 61;
    }

#if !defined(_MSC_VER) || _MSC_VER >= 1700
    {
        std::byte b = std::byte(0x0f);
        b |= std::byte(0xf0);
        if (std::to_integer<int>(b) != 0xff)
            return 62;
        std::byte c = std::byte(1) << 4;
        if (std::to_integer<int>(c) != 0x10 || std::to_integer<int>(b & std::byte(0x0f)) != 0x0f)
            return 63;
    }
#endif

#if !defined(_MSC_VER) || _MSC_VER >= 1600
    {
        calc_t calc;
        if (std::invoke(is_even, 4) != true || std::invoke(is_even, 3) != false)
            return 64;
        if (std::invoke(&calc_t::add, calc, 3, 4) != 7)
            return 65;
    }
#endif

    {
        std::tuple<int, double, char> t = std::make_tuple(5, 2.5, 'x');
        if (std::get<0>(t) != 5 || std::get<1>(t) != 2.5 || std::get<2>(t) != 'x')
            return 75;
        if (std::tuple_size<std::tuple<int, double, char> >::value != 3)
            return 76;
        std::get<0>(t) = 9;
        if (std::get<0>(t) != 9)
            return 77;
    }

    {
        std::optional<int> o;
        if (o.has_value() || o || o != std::nullopt)
            return 78;
        o = 42;
        if (!o.has_value() || !o || *o != 42 || o.value() != 42)
            return 79;
        if (o.value_or(7) != 42)
            return 80;
        std::optional<int> o2 = o;
        o.reset();
        if (o.has_value() || o.value_or(7) != 7 || o2.value() != 42)
            return 81;
        std::optional<int> o3 = std::make_optional(5);
        o3.emplace(9);
        if (*o3 != 9 || o3 != std::optional<int>(9))
            return 82;
        std::optional<std::string> os(std::in_place, "hi");
        if (!os || *os != "hi")
            return 83;
        bool threw = false;
        try { std::optional<int>().value(); }
        catch (const std::bad_optional_access&) { threw = true; }
        if (!threw)
            return 84;
    }

    {
        std::variant<int, std::string> v;
        if (v.index() != 0 || std::get<0>(v) != 0)
            return 85;
        if (!std::holds_alternative<int>(v) || std::holds_alternative<std::string>(v))
            return 86;
        v = 42;
        if (std::get<int>(v) != 42 || std::get<0>(v) != 42)
            return 87;
        v = std::string("hi");
        if (v.index() != 1 || std::get<std::string>(v) != "hi")
            return 88;
        if (std::get_if<int>(&v) != 0 || *std::get_if<std::string>(&v) != "hi")
            return 89;
        std::variant<int, std::string> v2 = v;
        if (!(v2 == v) || v2 != v)
            return 90;
        v2 = 7;
        if (v2 == v || std::get<int>(v2) != 7)
            return 91;
        bool threw = false;
        try { std::get<int>(v); }
        catch (const std::bad_variant_access&) { threw = true; }
        if (!threw)
            return 92;
        std::variant<int, std::string> v3;
        v3.emplace<std::string>(3u, 'x');
        if (v3.index() != 1 || std::get<std::string>(v3) != "xxx")
            return 93;
        if (std::variant_size<std::variant<int, std::string> >::value != 2)
            return 94;
    }

    {
        int arr[4] = { 1, 2, 3, 4 };
        if (*std::rbegin(arr) != 4 || *(std::rend(arr) - 1) != 1)
            return 95;
        if (*std::crbegin(arr) != 4)
            return 96;
        int bad[4] = { 1, 3, 2, 4 };
        if (std::is_sorted_until(arr, arr + 4) != arr + 4)
            return 97;
        if (std::is_sorted_until(bad, bad + 4) != bad + 2)
            return 98;
    }
    {
        int part[5] = { 2, 4, 1, 3, 5 };
        if (!std::is_partitioned(part, part + 5, is_even))
            return 99;
        if (std::partition_point(part, part + 5, is_even) != part + 2)
            return 100;
    }
    {
        int v[4] = { 1, 2, 3, 4 };
        int w[4] = { 4, 3, 2, 1 };
        int scan[4];
        if (std::transform_reduce(v, v + 4, w, 0) != 1*4 + 2*3 + 3*2 + 4*1)
            return 101;
        std::exclusive_scan(v, v + 4, scan, 0);
        if (scan[0] != 0 || scan[1] != 1 || scan[2] != 3 || scan[3] != 6)
            return 102;
        std::inclusive_scan(v, v + 4, scan);
        if (scan[0] != 1 || scan[1] != 3 || scan[2] != 6 || scan[3] != 10)
            return 103;
    }
    {
        if (!std::cmp_less(-1, 1u) || std::cmp_less(1u, -1))
            return 104;
        if (std::cmp_equal(-1, 0xffffffffu) || !std::cmp_greater(2, 1u))
            return 105;
        if (!std::cmp_less_equal(3, 3u) || !std::cmp_greater_equal(3u, 3))
            return 106;
        if (!std::in_range<unsigned char>(255) || std::in_range<unsigned char>(-1) || std::in_range<unsigned char>(256))
            return 107;
    }
    {
        if (std::not_fn(is_even)(4) != false || std::not_fn(is_even)(3) != true)
            return 108;
    }

    {
        if (std::to_underlying(color_blue) != 7 || std::to_underlying(color_red) != 1)
            return 109;
    }

    {
        int arr[5] = { 10, 20, 30, 40, 50 };
        std::span<int> s(arr);
        if (s.size() != 5 || s.front() != 10 || s.back() != 50 || s[2] != 30)
            return 120;
        if (s.data() != arr || s.empty() || s.size_bytes() != 5 * sizeof(int))
            return 121;
        int sum = 0;
        for (std::span<int>::iterator it = s.begin(); it != s.end(); ++it) sum += *it;
        if (sum != 150)
            return 122;
        std::span<int> mid = s.subspan(1, 3);
        if (mid.size() != 3 || mid.front() != 20 || mid.back() != 40)
            return 123;
        std::span<int> f2 = s.first(2);
        std::span<int> l2 = s.last(2);
        if (f2.size() != 2 || f2.back() != 20 || l2.front() != 40 || l2.back() != 50)
            return 124;
        std::span<int> sc(arr + 1, 2);
        if (sc.size() != 2 || sc[0] != 20 || *sc.rbegin() != 30)
            return 125;
        std::array<int, 3> a3;
        a3.fill(9);
        std::span<int> sa(a3);
        if (sa.size() != 3 || sa[1] != 9)
            return 126;
    }

    {
        if (std::numbers::pi < 3.14159 || std::numbers::pi > 3.14160)
            return 127;
        if (std::numbers::e < 2.71828 || std::numbers::e > 2.71829)
            return 128;
        if (std::numbers::sqrt2 < 1.41421 || std::numbers::sqrt2 > 1.41422)
            return 129;
    }

    {
        int istore = 0;
        int* p = std::construct_at(&istore, 77);
        if (*p != 77)
            return 130;
        std::destroy_at(p);

        int raw[3] = { 1, 2, 3 };
        std::uninitialized_value_construct_n(raw, 3);
        if (raw[0] != 0 || raw[1] != 0 || raw[2] != 0)
            return 131;
        std::destroy_n(raw, 3);

        int seq[4];
        std::uninitialized_default_construct(seq, seq + 4);
        std::destroy(seq, seq + 4);
    }

    {
        if (std::lerp(0.0, 10.0, 0.5) != 5.0 || std::lerp(2.0, 4.0, 0.0) != 2.0 || std::lerp(2.0, 4.0, 1.0) != 4.0)
            return 132;
        if (std::lerp(1.0f, 3.0f, 0.5f) != 2.0f)
            return 133;
        int arr[6] = { 0, 0, 0, 0, 0, 0 };
        std::array<int, 3> a3s;
        if (std::ssize(arr) != 6 || std::ssize(a3s) != 3)
            return 134;
    }

    {
        int sa[6] = { 1, 2, 3, 4, 5, 6 };
        int* le = std::shift_left(sa, sa + 6, 2);
        if (le != sa + 4 || sa[0] != 3 || sa[1] != 4 || sa[2] != 5 || sa[3] != 6)
            return 135;
        int sb[6] = { 1, 2, 3, 4, 5, 6 };
        int* re = std::shift_right(sb, sb + 6, 2);
        if (re != sb + 2 || sb[2] != 1 || sb[3] != 2 || sb[4] != 3 || sb[5] != 4)
            return 136;
    }

#if !defined(_MSC_VER) || _MSC_VER >= 1700

    {
        unsigned int val = 0x12345678u;
        std::span<unsigned int> vs(&val, 1);
        std::span<const std::byte> bs = std::as_bytes(vs);
        if (bs.size() != sizeof(unsigned int) || std::to_integer<int>(bs[0]) != 0x78)
            return 137;
        std::span<std::byte> wb = std::as_writable_bytes(vs);
        wb[0] = std::byte(0xFF);
        if ((val & 0xFFu) != 0xFFu)
            return 138;
    }
#endif

    {
        char buf[32];
        std::to_chars_result tr = std::to_chars(buf, buf + sizeof(buf), 12345);
        if (int(tr.ec) != 0 || (tr.ptr - buf) != 5 || std::memcmp(buf, "12345", 5) != 0)
            return 110;
        tr = std::to_chars(buf, buf + sizeof(buf), -42);
        if (int(tr.ec) != 0 || std::memcmp(buf, "-42", 3) != 0)
            return 111;
        tr = std::to_chars(buf, buf + sizeof(buf), 255u, 16);
        if (int(tr.ec) != 0 || (tr.ptr - buf) != 2 || std::memcmp(buf, "ff", 2) != 0)
            return 112;

        char small[2];
        tr = std::to_chars(small, small + 2, 1000);
        if (tr.ec != std::errc::value_too_large)
            return 113;

        int iv = 0;
        const char* s = "6789xyz";
        std::from_chars_result fr = std::from_chars(s, s + 7, iv);
        if (int(fr.ec) != 0 || iv != 6789 || *fr.ptr != 'x')
            return 114;
        long hv = 0;
        const char* h = "ff";
        fr = std::from_chars(h, h + 2, hv, 16);
        if (int(fr.ec) != 0 || hv != 255)
            return 115;

        int bad = -1;
        const char* z = "zzz";
        fr = std::from_chars(z, z + 3, bad);
        if (fr.ec != std::errc::invalid_argument || fr.ptr != z)
            return 116;

        signed char sc = 0;
        const char* big = "999";
        fr = std::from_chars(big, big + 3, sc);
        if (fr.ec != std::errc::result_out_of_range)
            return 117;

        {
            char rb[32];
            long long orig = -1234567890123LL;
            std::to_chars_result t2 = std::to_chars(rb, rb + sizeof(rb), orig);
            long long back = 0;
            std::from_chars(rb, t2.ptr, back);
            if (back != orig)
                return 118;
        }

        {
            char fb[64];
            double dv = 3.14159265358979;
            std::to_chars_result t3 = std::to_chars(fb, fb + sizeof(fb), dv);
            double dback = 0;
            std::from_chars(fb, t3.ptr, dback);
            if (dback != dv)
                return 119;
        }
    }

    {
        std::expected<int, std::string> e(42);
        if (!e || !e.has_value() || *e != 42 || e.value() != 42 || e.value_or(0) != 42)
            return 139;
        std::expected<int, std::string> u = std::unexpected<std::string>("err");
        if (u || u.has_value() || u.error() != "err" || u.value_or(-1) != -1)
            return 140;
        bool threw = false;
        try { u.value(); }
        catch (const std::bad_expected_access<std::string>& ex) { threw = (ex.error() == "err"); }
        if (!threw)
            return 141;
        e = std::unexpected<std::string>("bad");
        if (e.has_value() || e.error() != "bad")
            return 142;
        e = 7;
        if (!e.has_value() || *e != 7)
            return 143;
        std::expected<int, std::string> e2(std::in_place, 9);
        std::expected<int, std::string> e3(std::unexpect, "x");
        if (*e2 != 9 || e3.error() != "x" || e2 == e3)
            return 144;
        if (!(e2 == std::expected<int, std::string>(9)) || !(e2 == 9))
            return 145;

        std::expected<void, int> ev;
        if (!ev || !ev.has_value())
            return 146;
        std::expected<void, int> eve = std::unexpected<int>(5);
        if (eve || eve.error() != 5)
            return 147;
        bool vthrew = false;
        try { eve.value(); }
        catch (const std::bad_expected_access<int>&) { vthrew = true; }
        if (!vthrew)
            return 148;

        _ccw::expected<int, std::string> ae = e2;
        if (!ae.has_value() || *ae != 9)
            return 302;
        _ccw::expected<int, std::string> au = _ccw::make_unexpected(std::string("z"));
        if (au.has_value() || au.error() != "z")
            return 303;
    }

    {
        namespace fs = std::filesystem;
        fs::path p("C:\\dir\\sub\\file.txt");
        if (p.filename() != fs::path("file.txt") || p.stem() != fs::path("file") || p.extension() != fs::path(".txt"))
            return 149;
        if (p.parent_path() != fs::path("C:\\dir\\sub") || p.root_name() != fs::path("C:"))
            return 150;
        if (!p.is_absolute() || p.is_relative() || !p.has_extension() || !p.has_filename())
            return 151;

        fs::path q = fs::path("a") / "b" / "c";
        if (q != fs::path("a/b/c"))
            return 152;
        if (q.native() != std::wstring(L"a\\b\\c"))
            return 153;

        fs::path r("dir/file.txt");
        if (r.is_absolute())
            return 154;
        r.replace_extension(".md");
        if (r != fs::path("dir/file.md"))
            return 155;
        r.make_preferred();
        if (r.native() != std::wstring(L"dir\\file.md"))
            return 156;

        fs::path root("C:\\");
        if (!root.has_root_directory() || root.root_directory() != fs::path("\\") || root.relative_path() != fs::path())
            return 157;
        fs::path e;
        if (!e.empty() || e.has_filename())
            return 158;
    }

    {
        namespace fs = std::filesystem;
        if (fs::current_path().empty())
            return 159;
        fs::path d = fs::current_path() / "ccw_fs_t";
        fs::path f = d / "f.txt";
        fs::remove(f);
        fs::remove(d);
        if (!fs::create_directory(d) || !fs::exists(d) || !fs::is_directory(d) || fs::is_regular_file(d))
            return 160;
        {
            std::ofstream ofs(f.string().c_str());
            ofs << "hello";
        }
        if (!fs::exists(f) || !fs::is_regular_file(f) || fs::file_size(f) != 5)
            return 161;
        fs::remove(f);
        if (fs::exists(f))
            return 162;
        fs::remove(d);
        if (fs::exists(d))
            return 163;
    }

    {
        namespace fs = std::filesystem;
        fs::path d = fs::current_path() / "ccw_fs_it";
        fs::remove(d / "a.txt");
        fs::remove(d / "b.txt");
        fs::remove(d);
        if (!fs::create_directory(d))
            return 164;
        { std::ofstream ofs((d / "a.txt").string().c_str()); ofs << "x"; }
        { std::ofstream ofs((d / "b.txt").string().c_str()); ofs << "yy"; }

        int count = 0;
        bool sawA = false, sawB = false;
        for (fs::directory_iterator it(d); it != fs::directory_iterator(); ++it) {
            ++count;
            std::string fn = it->path().filename().string();
            if (fn == "a.txt") sawA = true;
            if (fn == "b.txt") sawB = true;
            if (!it->is_regular_file())
                return 165;
        }
        if (count != 2 || !sawA || !sawB)
            return 166;

        fs::remove(d / "a.txt");
        fs::remove(d / "b.txt");
        fs::remove(d);
        if (fs::exists(d))
            return 167;
    }

    {
        namespace fs = std::filesystem;
        fs::path base = fs::current_path() / "ccw_fs_deep";
        fs::remove_all(base);
        fs::path nested = base / "a" / "b" / "c";
        if (!fs::create_directories(nested) || !fs::is_directory(nested))
            return 168;
        { std::ofstream ofs((base / "a" / "b" / "f.txt").string().c_str()); ofs << "data"; }
        fs::path cp = base / "a" / "g.txt";
        if (!fs::copy_file(base / "a" / "b" / "f.txt", cp) || !fs::is_regular_file(cp) || fs::file_size(cp) != 4)
            return 169;
        unsigned long long removed = fs::remove_all(base);
        if (removed == 0 || fs::exists(base))
            return 170;
        if (fs::temp_directory_path().empty())
            return 171;
        if (!fs::absolute(fs::path("x")).is_absolute())
            return 172;
    }

    {
        namespace fs = std::filesystem;
        fs::path base = fs::current_path() / "ccw_fs_rec";
        fs::remove_all(base);
        fs::create_directories(base / "sub");
        { std::ofstream ofs((base / "top.txt").string().c_str());       ofs << "1"; }
        { std::ofstream ofs((base / "sub" / "deep.txt").string().c_str()); ofs << "22"; }

        int files = 0, dirs = 0;
        bool sawDeep = false;
        for (fs::recursive_directory_iterator it(base); it != fs::recursive_directory_iterator(); ++it) {
            if (it->is_directory()) ++dirs;
            else {
                ++files;
                if (it->path().filename() == fs::path("deep.txt")) sawDeep = true;
            }
        }
        if (files != 2 || dirs != 1 || !sawDeep)
            return 173;

        fs::path c = fs::canonical(base / "sub" / ".." / "top.txt");
        if (c.filename() != fs::path("top.txt") || !c.is_absolute())
            return 174;

        fs::remove_all(base);
        if (fs::exists(base))
            return 175;
    }

    {
        namespace fs = std::filesystem;
        if (fs::path("a/./b/../c").lexically_normal() != fs::path("a/c"))
            return 198;
        if (fs::path("a/b/../..").lexically_normal() != fs::path("."))
            return 199;
        if (fs::path("C:/a/./x").lexically_normal() != fs::path("C:/a/x"))
            return 200;
        if (fs::path("/a/b/c").lexically_relative("/a/d") != fs::path("../b/c"))
            return 201;
        if (fs::path("C:/a/b/c").lexically_relative("C:/a/b/c") != fs::path("."))
            return 202;
        if (fs::path("C:/x").lexically_relative("D:/y") != fs::path())
            return 203;
        if (fs::path("a/b").lexically_proximate("c/d") != fs::path("../../a/b"))
            return 204;
    }

    {
        namespace fs = std::filesystem;
        std::error_code ec;
        fs::path missing = fs::current_path() / "ccw_no_such_file_xyz";
        (void)fs::file_size(missing, ec);
        if (!ec)
            return 215;
        ec.clear();
        if (fs::exists(missing, ec) || ec)
            return 216;

        fs::path d = fs::current_path() / "ccw_ec_dir";
        fs::remove(d, ec);
        if (!fs::create_directory(d, ec) || ec || !fs::is_directory(d, ec))
            return 217;
        fs::remove(d, ec);
        if (ec)
            return 218;

        bool threw = false;
        try { throw fs::filesystem_error("boom", d, std::error_code()); }
        catch (const fs::filesystem_error& e) {

            threw = (std::string(e.what()).find("boom") != std::string::npos && e.path1() == d);
        }
        if (!threw)
            return 219;
    }

    {
        namespace fs = std::filesystem;
        fs::space_info si = fs::space(fs::current_path());
        if (si.capacity == 0 || si.capacity == (unsigned long long)-1 || si.free > si.capacity)
            return 220;

        fs::path dir = fs::current_path();
        if (!fs::equivalent(dir, dir))
            return 221;
        fs::path f = fs::current_path() / "ccw_eq.txt";
        fs::remove(f);
        { std::ofstream ofs(f.string().c_str()); ofs << "x"; }
        if (!fs::equivalent(f, f) || fs::equivalent(f, dir))
            return 222;
        if (fs::hard_link_count(f) < 1)
            return 223;
        fs::remove(f);

        fs::path wf = fs::current_path() / "ccw_wt.txt";
        fs::remove(wf);
        { std::ofstream ofs(wf.string().c_str()); ofs << "x"; }
        fs::file_time_type t = fs::last_write_time(wf);
        if (t == fs::file_time_type() || !(t > fs::file_time_type()))
            return 224;
        fs::remove(wf);
    }

    {
        namespace fs = std::filesystem;
        fs::path src = fs::current_path() / "ccw_src";
        fs::path dst = fs::current_path() / "ccw_dst";
        fs::remove_all(src);
        fs::remove_all(dst);
        fs::create_directories(src / "sub");
        { std::ofstream ofs((src / "a.txt").string().c_str());          ofs << "1"; }
        { std::ofstream ofs((src / "sub" / "deep.txt").string().c_str()); ofs << "22"; }

        fs::copy(src, dst, fs::copy_options::recursive);
        if (!fs::exists(dst / "a.txt") || !fs::exists(dst / "sub" / "deep.txt"))
            return 225;
        if (fs::file_size(dst / "sub" / "deep.txt") != 2)
            return 226;

        fs::path orig = fs::current_path() / "ccw_orig.txt";
        fs::path link = fs::current_path() / "ccw_link.txt";
        fs::remove(link);
        fs::remove(orig);
        { std::ofstream ofs(orig.string().c_str()); ofs << "x"; }
        fs::create_hard_link(orig, link);
        if (!fs::exists(link) || fs::hard_link_count(orig) < 2)
            return 227;

        fs::remove(link);
        fs::remove(orig);
        fs::remove_all(src);
        fs::remove_all(dst);
    }

    {
        namespace fs = std::filesystem;
        fs::path f = fs::current_path() / "ccw_wt2.txt";
        fs::remove(f);
        { std::ofstream ofs(f.string().c_str()); ofs << "hi"; }
        fs::file_time_type t = fs::last_write_time(f);
        fs::last_write_time(f, t);
        if (fs::last_write_time(f) != t)
            return 228;
        if (fs::is_empty(f))
            return 229;
        fs::remove(f);

        fs::path ed = fs::current_path() / "ccw_empty_dir";
        fs::remove_all(ed);
        fs::create_directory(ed);
        if (!fs::is_empty(ed))
            return 230;
        fs::remove_all(ed);

        fs::path tgt = fs::current_path() / "ccw_sym_tgt.txt";
        fs::path lnk = fs::current_path() / "ccw_sym_lnk.txt";
        fs::remove(lnk);
        fs::remove(tgt);
        { std::ofstream ofs(tgt.string().c_str()); ofs << "x"; }
        std::error_code sec;
        fs::create_symlink(tgt, lnk, sec);
        if (!sec) {
            if (!fs::exists(lnk) || !fs::is_symlink(lnk) || fs::is_symlink(tgt))
                return 234;
        }
        fs::remove(lnk);
        fs::remove(tgt);

#if __cplusplus < 202002L
        fs::path up = fs::u8path(std::string("dir/file.txt"));
        std::string u8 = up.generic_u8string();
        if (u8 != "dir/file.txt")
            return 235;
#endif
    }

    {
        std::string_view sv("hello world");
        if (sv.compare("hello world") != 0 || sv.compare("hello") <= 0)
            return 176;
        if (sv.rfind('o') != 7 || sv.find_first_of("aeiou") != 1 || sv.find_last_of("aeiou") != 7)
            return 177;
        if (sv.find_first_not_of("hel") != 4)
            return 178;
        std::string_view sv2 = sv;
        sv2.remove_prefix(6);
        if (sv2 != std::string_view("world"))
            return 179;
        sv2.remove_suffix(1);
        if (sv2 != std::string_view("worl") || sv2.front() != 'w' || sv2.back() != 'l')
            return 180;
        if (sv.at(0) != 'h' || sv.substr(6) != std::string_view("world"))
            return 181;
    }

    {
        if (!std::is_arithmetic<int>::value || std::is_arithmetic<int*>::value)
            return 182;
        if (!std::is_floating_point<double>::value || std::is_floating_point<int>::value)
            return 183;
        if (!std::is_signed<int>::value || std::is_signed<unsigned>::value)
            return 184;
        if (!std::is_unsigned<unsigned>::value || std::is_unsigned<int>::value)
            return 185;
        typedef std::conditional<true, int, long>::type c1;
        typedef std::conditional<false, int, long>::type c2;
        if (!std::is_same<c1, int>::value || !std::is_same<c2, long>::value)
            return 186;
        if (!std::is_same<std::remove_pointer<int*>::type, int>::value)
            return 187;
        if (!std::is_same<std::add_pointer<int>::type, int*>::value)
            return 188;
        if (!std::is_same<std::decay<int[3]>::type, int*>::value)
            return 189;
        if (!std::is_convertible<int, double>::value || std::is_convertible<int*, double>::value)
            return 190;
    }

    {
        std::tuple<int, double, char, long> t4 = std::make_tuple(1, 2.0, 'c', 4L);
        if (std::get<3>(t4) != 4L || std::get<2>(t4) != 'c')
            return 191;
        if (std::tuple_size<std::tuple<int, double, char, long> >::value != 4)
            return 192;
        if (!std::is_same<std::tuple_element<1, std::tuple<int, double, char, long> >::type, double>::value)
            return 193;
    }

    {
        std::optional<int> a(1), b(1), c(2), n;
        if (!(a == b) || a == c || a == n || !(n == std::nullopt))
            return 194;
        a.swap(c);
        if (*a != 2 || *c != 1)
            return 195;

        std::variant<int, double, char> v(2.5);
        if (v.index() != 1 || std::get_if<int>(&v) != 0 || *std::get_if<double>(&v) != 2.5)
            return 196;
        std::variant<std::monostate, int> mv;
        if (mv.index() != 0 || !std::holds_alternative<std::monostate>(mv))
            return 197;

        std::variant<char, short, int, long, float, double, unsigned, bool, std::string, void*> vb;
        vb = std::string("nine");
        if (vb.index() != 8 || std::get<8>(vb) != "nine" || !std::holds_alternative<std::string>(vb))
            return 306;
        if (std::variant_size<std::variant<char, short, int, long, float, double, unsigned, bool, std::string, void*> >::value != 10)
            return 307;
#if !defined(_MSC_VER) || _MSC_VER < 1600 || _MSC_VER >= 1800
        std::tuple<int, int, int, int, int, int, int, int, int, int> t10 =
            std::make_tuple(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
        if (std::get<0>(t10) != 0 || std::get<9>(t10) != 9 || std::get<5>(t10) != 5)
            return 308;
        if (std::tuple_size<std::tuple<int, int, int, int, int, int, int, int, int, int> >::value != 10)
            return 309;
#endif
    }

    {
        std::shared_ptr<ab_t> sp(new ab_t());
        sp->a = 1;
        sp->b = 2;
        std::shared_ptr<int> pa(sp, &sp->a);
        if (*pa != 1 || sp.use_count() != 2)
            return 205;
        pa.reset();
        if (sp.use_count() != 1)
            return 206;
        std::shared_ptr<int> sp2(new int(7));
        sp2.reset(new int(9));
        if (*sp2 != 9)
            return 207;
        int x = 42;
        if (std::addressof(x) != &x)
            return 208;
    }

    {
        char buf[40];
        std::to_chars_result tr = std::to_chars(buf, buf + sizeof(buf), 255, 2);
        if (int(tr.ec) != 0 || std::string(buf, tr.ptr) != "11111111")
            return 209;
        tr = std::to_chars(buf, buf + sizeof(buf), 35, 36);
        if (int(tr.ec) != 0 || std::string(buf, tr.ptr) != "z")
            return 210;
        int v = 0;
        const char* s = "-2147483648";
        std::from_chars_result fr = std::from_chars(s, s + 11, v);
        if (int(fr.ec) != 0 || v != (-2147483647 - 1))
            return 211;
        unsigned int u = 0;
        const char* h = "deadBEEF";
        fr = std::from_chars(h, h + 8, u, 16);
        if (int(fr.ec) != 0 || u != 0xdeadbeefu)
            return 212;
    }

    {
        int v[4] = { 1, 2, 3, 4 };
        int scan[4];
        if (std::transform_reduce(v, v + 4, 0, std::plus<int>(), square_fn) != 1 + 4 + 9 + 16)
            return 213;
        std::inclusive_scan(v, v + 4, scan, std::plus<int>());
        if (scan[0] != 1 || scan[3] != 10)
            return 214;
    }

    {
        std::errc e = std::errc::result_out_of_range;
        if (e != std::errc::result_out_of_range || e == std::errc::invalid_argument)
            return 231;
        if (int(std::errc()) != 0)
            return 232;
        std::error_code ec;
        if (ec)
            return 233;
    }

    {
        if (std::ratio<2, 6>::num != 1 || std::ratio<2, 6>::den != 3)
            return 234;
        typedef std::ratio_add<std::ratio<1, 2>, std::ratio<1, 3> >::type sum;
        if (sum::num != 5 || sum::den != 6)
            return 235;
        typedef std::ratio_multiply<std::ratio<2, 3>, std::ratio<3, 4> >::type prod;
        if (prod::num != 1 || prod::den != 2)
            return 236;
        if (std::milli::den != 1000 || std::kilo::num != 1000)
            return 237;
    }

    {
        std::atomic<int> a(5);
        if (a.load() != 5 || a != 5)
            return 243;
        a.store(10);
        if (a.fetch_add(3) != 10 || a.load() != 13)
            return 244;
        if (a.exchange(20) != 13 || a != 20)
            return 245;
        int exp = 20;
        if (!a.compare_exchange_strong(exp, 7) || a != 7)
            return 246;
        exp = 99;
        if (a.compare_exchange_strong(exp, 0) || exp != 7)
            return 247;
        ++a;
        a += 2;
        if (a != 10)
            return 248;
        std::atomic_flag f;
        f.clear();
        if (f.test_and_set() || !f.test_and_set())
            return 249;
        f.clear();
    }

    {
        std::mt19937 g(12345u);
        std::mt19937 g2(12345u);
        unsigned int v1 = g();
        if (v1 != g2())
            return 256;
        {
            std::mt19937 gc;
            for (int i = 0; i < 9999; ++i) gc();
            if (gc() != 4123659995u)
                return 261;
        }
        std::uniform_int_distribution<int> di(1, 6);
        for (int i = 0; i < 200; ++i) {
            int r = di(g);
            if (r < 1 || r > 6)
                return 257;
        }
        std::uniform_real_distribution<double> dr(0.0, 1.0);
        for (int i = 0; i < 200; ++i) {
            double r = dr(g);
            if (r < 0.0 || r >= 1.0)
                return 258;
        }
        std::bernoulli_distribution db(1.0);
        if (!db(g))
            return 259;
        std::minstd_rand lc(1u);
        if (lc() == 0u)
            return 260;
        std::random_device rd;
        (void)rd();
    }

#if defined(_TST_TEST_HAS_MUTEX)
    {
        std::mutex m;
        { std::lock_guard<std::mutex> g(m); }
        {
            std::unique_lock<std::mutex> ul(m);
            if (!ul.owns_lock() || !ul)
                return 250;
            ul.unlock();
            if (ul.owns_lock())
                return 251;
            ul.lock();
        }
        std::recursive_mutex rm;
        rm.lock(); rm.lock(); rm.unlock(); rm.unlock();

        g_co_calls = 0;
        std::once_flag of;
        std::call_once(of, co_fn);
        std::call_once(of, co_fn);
        if (g_co_calls != 1)
            return 252;
    }

    {
        g_thr_val = 0;
        std::thread t(thr_fn, 42);
        if (!t.joinable())
            return 253;
        t.join();
        if (t.joinable() || g_thr_val != 42)
            return 254;
        std::thread t2(thr_fn, 7);
        t2.detach();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        std::thread::id me = std::this_thread::get_id();
        if (me != std::this_thread::get_id())
            return 255;
    }

    {
        std::mutex m;
        std::condition_variable cv;
        std::unique_lock<std::mutex> lk(m);
        if (!cv.wait_for(lk, std::chrono::milliseconds(1), pred_true))
            return 262;
        cv.notify_one();
        cv.notify_all();
    }

    {
        std::shared_mutex sm;
        sm.lock_shared();
        sm.unlock_shared();
        sm.lock();
        sm.unlock();
        {
            std::shared_lock<std::shared_mutex> sl(sm);
            if (!sl.owns_lock() || !sl)
                return 263;
        }
        {
            std::unique_lock<std::shared_mutex> ul(sm);
            if (!ul.owns_lock())
                return 264;
        }
    }
#endif

    {
        std::chrono::seconds s(2);
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(s);
        if (ms.count() != 2000)
            return 238;
        std::chrono::milliseconds a(500), b(300);
        if ((a + b).count() != 800 || (a - b).count() != 200 || !(b < a))
            return 239;
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(2500)).count() != 2)
            return 240;
        std::chrono::steady_clock::time_point st0 = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point st1 = std::chrono::steady_clock::now();
        if (st1 < st0)
            return 241;
    }

    {
        if (std::format("{} + {} = {}", 1, 2, 3) != "1 + 2 = 3")
            return 265;
        if (std::format("{0}{1}{0}", std::string("a"), std::string("b")) != "aba")
            return 266;
        if (std::format("{{}}") != "{}")
            return 267;
        if (std::format("plain") != "plain")
            return 268;

        if (std::format("{}{}{}{}{}{}{}{}{}{}", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9) != "0123456789")
            return 304;
        if (std::format("{9}{0}", 'a', 1, 2, 3, 4, 5, 6, 7, 8, 'b') != "ba")
            return 305;
    }

    {
        std::array<int, 5> ra;
        ra[0] = 3; ra[1] = 1; ra[2] = 4; ra[3] = 1; ra[4] = 5;
        if (std::ranges::size(ra) != 5)
            return 269;
        std::ranges::sort(ra);
        if (ra[0] != 1 || ra[4] != 5)
            return 270;
        if (*std::ranges::find(ra, 4) != 4)
            return 271;
        if (std::ranges::count(ra, 1) != 2)
            return 272;
        if (*std::ranges::max_element(ra) != 5 || *std::ranges::min_element(ra) != 1)
            return 273;
    }

    {
        std::any a = 42;
        if (!a.has_value())
            return 274;
        if (std::any_cast<int>(a) != 42)
            return 275;
        a = std::string("hi");
        if (std::any_cast<std::string>(a) != "hi")
            return 276;
        if (std::any_cast<int>(&a) != 0)
            return 277;
        bool threw = false;
        try { (void)std::any_cast<int>(a); } catch (const std::bad_any_cast&) { threw = true; }
        if (!threw)
            return 278;
        a.reset();
        if (a.has_value())
            return 279;
        std::any b;
        b.emplace<int>(7);
        if (std::any_cast<int>(b) != 7)
            return 280;
    }

    {
        std::FILE* tf = std::fopen("ccw_print_test.tmp", "w+b");
        if (!tf)
            return 281;
        std::print(tf, "{}-{}", 1, 2);
        std::println(tf, "[{}]", std::string("x"));
        std::fflush(tf);
        std::rewind(tf);
        char buf[64];
        std::size_t got = std::fread(buf, 1, sizeof(buf) - 1, tf);
        buf[got] = 0;
        std::fclose(tf);
        std::remove("ccw_print_test.tmp");
        if (std::string(buf) != "1-2[x]\n")
            return 282;
    }

    {
        char sbuf[32];
        std::ospanstream os(std::span<char>(sbuf, sizeof(sbuf)));
        os << "n=" << 42;
        std::span<char> w = os.span();
        if (std::string(w.data(), w.size()) != "n=42")
            return 283;

        char in[] = "7 x";
        std::ispanstream is(std::span<char>(in, 3));
        int iv = 0;
        std::string is_s;
        is >> iv >> is_s;
        if (iv != 7 || is_s != "x")
            return 284;
    }

#if defined(_TST_TEST_HAS_MUTEX)
    {
        std::ostringstream oss;
        {
            std::osyncstream os(oss);
            os << "a" << 1;
            os.emit();
            if (oss.str() != "a1")
                return 285;
            os << "-" << 2;
        }
        if (oss.str() != "a1-2")
            return 286;
    }
#endif

    {
        char pbuf[512];
        std::pmr::monotonic_buffer_resource mr(pbuf, sizeof(pbuf));
        std::vector<int, std::pmr::polymorphic_allocator<int> >
            v((std::pmr::polymorphic_allocator<int>(&mr)));
        for (int i = 0; i < 10; ++i)
            v.push_back(i * i);
        if (v.size() != 10 || v[9] != 81)
            return 287;
        long d = (long)((char*)&v[0] - pbuf);
        if (d < 0 || d >= (long)sizeof(pbuf))
            return 288;

        std::pmr::memory_resource* nd = std::pmr::new_delete_resource();
        void* p = nd->allocate(64);
        if (!p)
            return 289;
        nd->deallocate(p, 64);
        if (std::pmr::get_default_resource() != nd)
            return 290;
    }

    {
        std::flat_map<int, std::string> fm;
        fm[3] = "three";
        fm[1] = "one";
        fm[2] = "two";
        if (fm.size() != 3)
            return 291;
        std::flat_map<int, std::string>::iterator it = fm.begin();
        if (it->first != 1 || it->second != "one")
            return 292;
        if (fm.at(2) != "two")
            return 293;
        if (!fm.contains(3) || fm.contains(4))
            return 294;
        if (fm.count(1) != 1)
            return 295;
        fm.erase(2);
        if (fm.contains(2) || fm.size() != 2)
            return 296;
    }

    {
        std::flat_set<int> fs;
        fs.insert(5);
        fs.insert(1);
        fs.insert(5);
        fs.insert(3);
        if (fs.size() != 3)
            return 297;
        std::flat_set<int>::const_iterator si = fs.begin();
        if (*si != 1)
            return 298;
        if (!fs.contains(3) || fs.contains(2))
            return 299;
        if (fs.count(5) != 1)
            return 300;
        fs.erase(1);
        if (fs.contains(1) || fs.size() != 2)
            return 301;
    }

    return 0;
}
