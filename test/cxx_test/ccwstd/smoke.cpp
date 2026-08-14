#include <type_traits>
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <numeric>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <cwchar>
#include <ctime>
#include <cstdint>
#include <thread>

static int failures = 0;
static void check(bool ok, const char* what) {
    if (!ok) { failures++; ccwstd::printf("FAIL: %s\n", what); }
}
static void bump(int* p) { *p += 1; }

int main() {

    ccwstd::vector<int> v;
    v.push_back(3); v.push_back(1); v.push_back(2);
    ccwstd::sort(v.begin(), v.end());
    check(v[0] == 1 && v[1] == 2 && v[2] == 3, "vector+sort");

    ccwstd::string s("hello");
    check(s.size() == 5 && s == ccwstd::string("hello"), "string");

    ccwstd::map<int, ccwstd::string> m;
    m[1] = "one"; m[2] = "two";
    check(m.size() == 2 && m[1] == "one", "map");

    ccwstd::set<int> st;
    st.insert(5); st.insert(5); st.insert(7);
    check(st.size() == 2, "set");

    ccwstd::shared_ptr<int> p(new int(42));
    ccwstd::weak_ptr<int> w(p);
    check(*p == 42 && p.use_count() == 1 && !w.expired(), "shared_ptr/weak_ptr");

    int sum = ccwstd::accumulate(v.begin(), v.end(), 0);
    check(sum == 6, "accumulate");

    check(ccwstd::is_same<ccwstd::remove_cvref<const int&>::type, int>::value, "remove_cvref");
    check(ccwstd::is_integral<int>::value && !ccwstd::is_integral<double>::value, "is_integral");

    check(ccwstd::strlen("abc") == 3, "cstring::strlen");
    check(ccwstd::sqrt(16.0) == 4.0, "cmath::sqrt");
    check(ccwstd::abs(-5) == 5, "cstdlib::abs");
    check(ccwstd::isdigit('7') && !ccwstd::isdigit('x'), "cctype::isdigit");
    ccwstd::size_t n = 3;
    check(n == 3, "cstddef::size_t");

    ccwstd::ostringstream oss;
    oss << 42 << ' ' << 'x';
    check(oss.str() == "42 x", "ostringstream (src-backed num_put)");
    ccwstd::istringstream iss("100 7");
    int a = 0, b = 0; iss >> a >> b;
    check(a == 100 && b == 7, "istringstream (src-backed num_get)");

    int hit = 0;
    ccwstd::thread th(bump, &hit);
    th.join();
    check(hit == 1, "thread (src-backed)");
    check(ccwstd::thread::hardware_concurrency() >= 1u, "hardware_concurrency");

    ccwstd::time_t tt = ccwstd::time(0);
    check(tt != (ccwstd::time_t)-1, "ctime::time");
    ccwstd::uint32_t u32 = 0xDEADBEEFu;
    check(u32 == 0xDEADBEEFu, "cstdint::uint32_t");

    if (failures == 0) ccwstd::printf("ccwstd smoke: all checks passed\n");
    return failures;
}
