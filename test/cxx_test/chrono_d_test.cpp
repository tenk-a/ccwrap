#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020 && _tst_cplusplus >= 201103L
#include <chrono>
#include <compare>
#include <format>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

using _ccw::string_view;

namespace {

STD::chrono::sys_seconds fixed_utc() {
    using namespace STD::chrono;
    return sys_seconds(sys_days(2024y / February / 29d)) + hours(12) + minutes(34) + seconds(56);
}

}

#if _TST_HAS_CHRONO_TZDB
TEST_CASE(chrono, tzdb_and_tzdb_list) {
    using namespace STD::chrono;

    const tzdb& db = get_tzdb();
    test_true( !db.version.empty() );
    test_pass("cxx20:get_tzdb");

    test_true( !db.zones.empty() );
    test_true( !db.leap_seconds.empty() );
    (void)db.links;
    test_true( (STD::is_same<STD::remove_cvref_t<decltype(db.zones)>,
                             STD::vector<time_zone> >::value) );
    test_true( (STD::is_same<STD::remove_cvref_t<decltype(db.links)>,
                             STD::vector<time_zone_link> >::value) );
    test_true( (STD::is_same<STD::remove_cvref_t<decltype(db.leap_seconds)>,
                             STD::vector<leap_second> >::value) );
    test_pass("cxx20:tzdb");

    const time_zone* tokyo = db.locate_zone("Asia/Tokyo");
    test_true( tokyo != nullptr );
    test_str_eq( tokyo->name(), "Asia/Tokyo" );
    test_pass("cxx20:tzdb::locate_zone");

    const time_zone* cur = db.current_zone();
    test_true( cur != nullptr );
    test_pass("cxx20:tzdb::current_zone");

    test_true( locate_zone("Asia/Tokyo") == tokyo );
    test_true( locate_zone("UTC") != nullptr );

    test_throw( (void)locate_zone("Nowhere/Nozone") );
    test_pass("cxx20:locate_zone");

    test_true( current_zone() == cur );
    test_pass("cxx20:current_zone");

    tzdb_list& tl = get_tzdb_list();
    test_true( &tl.front() == &db );
    test_pass("cxx20:get_tzdb_list");

    tzdb_list::const_iterator it = tl.begin();
    test_true( it != tl.end() );
    test_true( &*it == &db );
    test_true( tl.cbegin() == tl.begin() );
    test_true( tl.cend() == tl.end() );

    tzdb_list::const_iterator (tzdb_list::*erase_after_p)(tzdb_list::const_iterator)
        = &tzdb_list::erase_after;
    test_true( erase_after_p != nullptr );
    test_pass("cxx20:tzdb_list");

    test_no_throw( (void)remote_version() );
    test_pass("cxx20:remote_version");

    const tzdb& rl = reload_tzdb();
    test_true( !rl.version.empty() );
    test_true( locate_zone("Asia/Tokyo") != nullptr );
    test_pass("cxx20:reload_tzdb");
}

TEST_CASE(chrono, time_zone_and_sys_info) {
    using namespace STD::chrono;

    const time_zone* tz = locate_zone("Asia/Tokyo");
    test_str_eq( tz->name(), "Asia/Tokyo" );
    test_pass("cxx20:time_zone::name");

    sys_seconds inst = fixed_utc();

    sys_info si = tz->get_info(inst);
    test_eq( si.offset.count(), 9 * 3600LL );
    test_eq( si.save.count(), 0LL );
    test_true( si.begin <= inst );
    test_true( inst < si.end );
    test_pass("cxx20:time_zone::get_info(sys_time)");

    test_true( !si.abbrev.empty() );
    test_true( (STD::is_same<decltype(si.begin), sys_seconds>::value) );
    test_true( (STD::is_same<decltype(si.save), minutes>::value) );
    test_true( (STD::is_same<decltype(si.abbrev), STD::string>::value) );
    test_pass("cxx20:sys_info");

    local_seconds lt = tz->to_local(inst);
    test_eq( lt.time_since_epoch().count(),
             inst.time_since_epoch().count() + 9 * 3600LL );
    STD::ostringstream os;  os << lt;
    test_str_eq( os.str(), "2024-02-29 21:34:56" );
    test_pass("cxx20:time_zone::to_local");

    sys_seconds back = tz->to_sys(lt);
    test_true( back == inst );

    test_true( tz->to_sys(lt, choose::earliest) == inst );
    test_true( tz->to_sys(lt, choose::latest) == inst );
    test_pass("cxx20:time_zone::to_sys");

    local_info li = tz->get_info(lt);
    test_eq( li.result, (int)local_info::unique );
    test_eq( li.first.offset.count(), 9 * 3600LL );
    test_pass("cxx20:time_zone::get_info(local_time)");

    test_eq( li.second.offset.count(), 0LL );
    test_eq( (int)local_info::nonexistent, 1 );
    test_eq( (int)local_info::ambiguous, 2 );
    test_pass("cxx20:local_info");

    const time_zone* utc = locate_zone("UTC");
    sys_info usi = utc->get_info(inst);
    test_eq( usi.offset.count(), 0LL );
    test_eq( utc->to_local(inst).time_since_epoch().count(), inst.time_since_epoch().count() );
    test_pass("cxx20:time_zone (UTC)");

    STD::string sif = STD::format("{}", si);
    TEST_SKIP_GCC("libstdc++ sys_info formatting / tzdata differs on Linux");
    test_true( sif.find("offset: 32400s") != STD::string::npos );
    TEST_SKIP_GCC("libstdc++ sys_info formatting / tzdata differs on Linux");
    test_true( sif.find("abbrev: JST") != STD::string::npos );
    test_pass("cxx20:format sys_info");

    STD::string lif = STD::format("{}", li);
    TEST_SKIP_GCC("libstdc++ local_info formatting differs on Linux");
    test_true( lif.find("result: unique") != STD::string::npos );
    test_pass("cxx20:format local_info");
}

#if TEST_HAS_EH
TEST_CASE(chrono, time_zone_dst_transitions) {
    using namespace STD::chrono;

    const time_zone* ny = locate_zone("America/New_York");

    local_seconds gone = local_seconds(local_days(2024y / March / 10d)) + hours(2) + minutes(30);
    local_info gi = ny->get_info(gone);
    test_eq( gi.result, (int)local_info::nonexistent );
    test_throw( (void)ny->to_sys(gone) );

    sys_seconds ge = ny->to_sys(gone, choose::earliest);
    sys_seconds gl = ny->to_sys(gone, choose::latest);
    test_true( ge == gl );
    test_true( ge == gi.first.end );
    test_pass("cxx20:local_info nonexistent");

    local_seconds twice = local_seconds(local_days(2024y / November / 3d)) + hours(1) + minutes(30);
    local_info ai = ny->get_info(twice);
    test_eq( ai.result, (int)local_info::ambiguous );
    test_throw( (void)ny->to_sys(twice) );
    sys_seconds early = ny->to_sys(twice, choose::earliest);
    sys_seconds late  = ny->to_sys(twice, choose::latest);
    test_true( early < late );
    test_eq( (late - early).count(), 3600LL );

    test_eq( ai.first.offset.count(),  -4 * 3600LL );
    test_eq( ai.second.offset.count(), -5 * 3600LL );
    test_eq( ai.first.save.count(), 60LL );
    test_eq( ai.second.save.count(), 0LL );
    test_pass("cxx20:local_info ambiguous");

    test_throw( throw nonexistent_local_time(gone, gi) );
    try {
        (void)ny->to_sys(gone);
        test_true( false );
    } catch (const nonexistent_local_time& e) {
        test_true( STD::string(e.what()).find("gap") != STD::string::npos );
    }

    test_true( (STD::is_base_of<STD::runtime_error, nonexistent_local_time>::value) );
    test_pass("cxx20:nonexistent_local_time");

    test_throw( throw ambiguous_local_time(twice, ai) );
    try {
        (void)ny->to_sys(twice);
        test_true( false );
    } catch (const ambiguous_local_time& e) {
        test_true( STD::string(e.what()).find("ambiguous") != STD::string::npos );
    }

    test_true( (STD::is_base_of<STD::runtime_error, ambiguous_local_time>::value) );
    test_pass("cxx20:ambiguous_local_time");

    test_true( choose::earliest != choose::latest );
    test_pass("cxx20:choose");
}
#else   // !TEST_HAS_EH
TEST_CASE(chrono, time_zone_dst_transitions) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx20:local_info nonexistent");
    test_skip("cxx20:local_info ambiguous");
    test_skip("cxx20:nonexistent_local_time");
    test_skip("cxx20:ambiguous_local_time");
    test_skip("cxx20:choose");
}
#endif  // TEST_HAS_EH

TEST_CASE(chrono, time_zone_link_and_leap_second) {
    using namespace STD::chrono;

    const tzdb& db = get_tzdb();

    if (!db.links.empty()) {
        const time_zone_link& lk = db.links.front();
        test_true( !STD::string(lk.name()).empty() );
        test_true( !STD::string(lk.target()).empty() );

        test_true( lk == lk );

        test_true( locate_zone(lk.target()) != nullptr );

        test_true( locate_zone(lk.name()) == locate_zone(lk.target()) );
    } else {
        test_true( db.links.empty() );
    }
    test_pass("cxx20:time_zone_link");

    const leap_second& first = db.leap_seconds.front();

    test_true( first.date() == sys_seconds(sys_days(1972y / July / 1d)) );
    test_pass("cxx20:leap_second date");
    test_true( first.value() == seconds(1) );
    test_pass("cxx20:leap_second value");

    const leap_second& last_ls = db.leap_seconds.back();
    test_true( first == first );
    test_true( first != last_ls );
    test_true( first < last_ls );
    test_true( last_ls > first );
    test_true( (first <=> first) == STD::strong_ordering::equal );
    test_true( (last_ls <=> first) == STD::strong_ordering::greater );
    test_pass("cxx20:leap_second comparisons");

    sys_seconds d72 = sys_seconds(sys_days(1972y / July / 1d));
    sys_seconds d71 = sys_seconds(sys_days(1971y / July / 1d));
    test_true( first == d72 );
    test_true( d72 == first );
    test_true( first != d71 );
    test_true( d71 != first );
    test_true( d71 < first );
    test_true( first > d71 );
    test_true( (first <=> d71) == STD::strong_ordering::greater );
    test_true( (d71 <=> first) == STD::strong_ordering::less );
    test_pass("cxx20:leap_second comparisons (sys_time)");

    test_ge( db.leap_seconds.size(), (STD::size_t)27 );
    test_pass("cxx20:tzdb::leap_seconds count");
}

TEST_CASE(chrono, zoned_traits_and_zoned_time_ctors) {
    using namespace STD::chrono;

    typedef zoned_traits<const time_zone*> ZT;
    test_true( ZT::default_zone() == locate_zone("UTC") );
    test_true( ZT::locate_zone("Asia/Tokyo") == locate_zone("Asia/Tokyo") );
    test_pass("cxx20:zoned_traits");

    const time_zone* tokyo = locate_zone("Asia/Tokyo");
    sys_seconds inst = fixed_utc();
    local_seconds ltok = tokyo->to_local(inst);

    test_true( (STD::is_same<zoned_time<seconds>::duration, seconds>::value) );
    test_true( (STD::is_same<zoned_time<milliseconds>::duration, milliseconds>::value) );
    test_pass("cxx20:zoned_time::duration");

    zoned_time<seconds> z0;
    test_true( z0.get_time_zone() == locate_zone("UTC") );
    test_eq( z0.get_sys_time().time_since_epoch().count(), 0LL );
    test_pass("cxx20:zoned_time default ctor");

    zoned_time<seconds> z1(inst);
    test_true( z1.get_sys_time() == inst );
    zoned_time<seconds> z2(ZT::default_zone(), local_seconds(inst.time_since_epoch()));
    test_true( z2.get_sys_time() == inst );
    test_pass("cxx20:zoned_time(sys_time)");

    zoned_time<seconds> z3(tokyo);
    test_true( z3.get_time_zone() == tokyo );
    test_pass("cxx20:zoned_time(TimeZonePtr)");

    zoned_time<seconds> z4(tokyo, inst);
    test_true( z4.get_sys_time() == inst );
    test_true( z4.get_local_time() == ltok );
    test_pass("cxx20:zoned_time(TimeZonePtr, sys_time)");

    zoned_time<seconds> z5(tokyo, ltok);
    test_true( z5.get_sys_time() == inst );
    test_pass("cxx20:zoned_time(TimeZonePtr, local_time)");

    zoned_time<seconds> z6(tokyo, ltok, choose::earliest);
    test_true( z6.get_sys_time() == inst );
    const time_zone* ny = locate_zone("America/New_York");
    local_seconds twice = local_seconds(local_days(2024y / November / 3d)) + hours(1) + minutes(30);
    zoned_time<seconds> ze(ny, twice, choose::earliest);
    zoned_time<seconds> zl(ny, twice, choose::latest);
    test_eq( (zl.get_sys_time() - ze.get_sys_time()).count(), 3600LL );
    test_pass("cxx20:zoned_time(TimeZonePtr, local, choose)");

    zoned_time<seconds> s1(STD::string_view("Asia/Tokyo"));
    test_true( s1.get_time_zone() == tokyo );
    zoned_time<seconds> s2("Asia/Tokyo", inst);
    test_true( s2.get_sys_time() == inst );
    zoned_time<seconds> s3("Asia/Tokyo", ltok);
    test_true( s3.get_sys_time() == inst );
    zoned_time<seconds> s4("America/New_York", twice, choose::latest);
    test_true( s4.get_sys_time() == zl.get_sys_time() );
    test_pass("cxx20:zoned_time(string_view)");

    zoned_time<milliseconds> zm(tokyo, sys_time<milliseconds>(inst));
    zoned_time<milliseconds> zconv(zm);
    test_true( zconv.get_sys_time() == zm.get_sys_time() );
    zoned_time<milliseconds> widened(z4);
    test_true( widened.get_sys_time() == sys_time<milliseconds>(inst) );
    test_true( widened.get_time_zone() == tokyo );
    test_pass("cxx20:zoned_time converting ctor");
}

TEST_CASE(chrono, zoned_time_members) {
    using namespace STD::chrono;

    const time_zone* tokyo = locate_zone("Asia/Tokyo");
    sys_seconds inst = fixed_utc();
    local_seconds ltok = tokyo->to_local(inst);

    zoned_time<seconds> z(tokyo, inst);

    test_true( z.get_time_zone() == tokyo );
    test_true( z.get_sys_time() == inst );
    test_true( z.get_local_time() == ltok );
    test_eq( (z.get_local_time().time_since_epoch()
              - z.get_sys_time().time_since_epoch()).count(), 9 * 3600LL );
    test_pass("cxx20:zoned_time accessors");

    sys_info si = z.get_info();
    test_eq( si.offset.count(), 9 * 3600LL );
    test_eq( si.save.count(), 0LL );
    test_pass("cxx20:zoned_time::get_info");

    sys_seconds cs = z;
    test_true( cs == inst );

    local_seconds cl = static_cast<local_seconds>(z);
    test_true( cl == ltok );
    test_pass("cxx20:zoned_time conversion operators");

    zoned_time<seconds> w(tokyo);
    w = inst;
    test_true( w.get_sys_time() == inst );
    test_true( w.get_time_zone() == tokyo );
    w = ltok + hours(1);
    test_true( w.get_sys_time() == inst + hours(1) );
    test_true( w.get_time_zone() == tokyo );
    test_pass("cxx20:zoned_time::operator=");

    STD::string s = STD::format("{}", z);
    test_true( s.find("2024-02-29 21:34:56") != STD::string::npos );
    test_str_eq( STD::format("{:%F %T}", z), "2024-02-29 21:34:56" );
    test_str_eq( STD::format("{:%z}", z), "+0900" );
    test_str_eq( STD::format("{:%Ez}", z), "+09:00" );
    test_true( !STD::format("{:%Z}", z).empty() );
    test_pass("cxx20:zoned_time format");

    STD::ostringstream os;  os << z;
    test_true( os.str().find("2024-02-29 21:34:56") != STD::string::npos );
    test_pass("cxx20:zoned_time operator<<");

    zoned_time<milliseconds> zm(tokyo, sys_time<milliseconds>(inst) + milliseconds(250));
    test_str_eq( STD::format("{:%F %T}", zm), "2024-02-29 21:34:56.250" );
    test_pass("cxx20:zoned_time format (subseconds)");
}
#else
TEST_CASE(chrono, tzdb_and_tzdb_list) {
    TEST_NOTE("this library ships no tz database (get_tzdb / time_zone / zoned_time)");
    test_skip("cxx20:get_tzdb");
    test_skip("cxx20:tzdb");
    test_skip("cxx20:tzdb::locate_zone");
    test_skip("cxx20:tzdb::current_zone");
    test_skip("cxx20:locate_zone");
    test_skip("cxx20:current_zone");
    test_skip("cxx20:get_tzdb_list");
    test_skip("cxx20:tzdb_list");
    test_skip("cxx20:remote_version");
    test_skip("cxx20:reload_tzdb");
}
TEST_CASE(chrono, time_zone_and_sys_info) {
    TEST_NOTE("this library ships no tz database (get_tzdb / time_zone / zoned_time)");
    test_skip("cxx20:time_zone::name");
    test_skip("cxx20:time_zone::get_info(sys_time)");
    test_skip("cxx20:sys_info");
    test_skip("cxx20:time_zone::to_local");
    test_skip("cxx20:time_zone::to_sys");
    test_skip("cxx20:time_zone::get_info(local_time)");
    test_skip("cxx20:local_info");
    test_skip("cxx20:time_zone (UTC)");
    test_skip("cxx20:format sys_info");
    test_skip("cxx20:format local_info");
}
TEST_CASE(chrono, time_zone_dst_transitions) {
    TEST_NOTE("this library ships no tz database (get_tzdb / time_zone / zoned_time)");
    test_skip("cxx20:local_info nonexistent");
    test_skip("cxx20:local_info ambiguous");
    test_skip("cxx20:nonexistent_local_time");
    test_skip("cxx20:ambiguous_local_time");
    test_skip("cxx20:choose");
}
TEST_CASE(chrono, time_zone_link_and_leap_second) {
    TEST_NOTE("this library ships no tz database (get_tzdb / time_zone / zoned_time)");
    test_skip("cxx20:time_zone_link");
    test_skip("cxx20:leap_second date");
    test_skip("cxx20:leap_second value");
    test_skip("cxx20:leap_second comparisons");
    test_skip("cxx20:leap_second comparisons (sys_time)");
    test_skip("cxx20:tzdb::leap_seconds count");
}
TEST_CASE(chrono, zoned_traits_and_zoned_time_ctors) {
    TEST_NOTE("this library ships no tz database (get_tzdb / time_zone / zoned_time)");
    test_skip("cxx20:zoned_traits");
    test_skip("cxx20:zoned_time::duration");
    test_skip("cxx20:zoned_time default ctor");
    test_skip("cxx20:zoned_time(sys_time)");
    test_skip("cxx20:zoned_time(TimeZonePtr)");
    test_skip("cxx20:zoned_time(TimeZonePtr, sys_time)");
    test_skip("cxx20:zoned_time(TimeZonePtr, local_time)");
    test_skip("cxx20:zoned_time(TimeZonePtr, local, choose)");
    test_skip("cxx20:zoned_time(string_view)");
    test_skip("cxx20:zoned_time converting ctor");
}
TEST_CASE(chrono, zoned_time_members) {
    TEST_NOTE("this library ships no tz database (get_tzdb / time_zone / zoned_time)");
    test_skip("cxx20:zoned_time accessors");
    test_skip("cxx20:zoned_time::get_info");
    test_skip("cxx20:zoned_time conversion operators");
    test_skip("cxx20:zoned_time::operator=");
    test_skip("cxx20:zoned_time format");
    test_skip("cxx20:zoned_time operator<<");
    test_skip("cxx20:zoned_time format (subseconds)");
}
#endif

#if !defined(__GNUC__) && _TST_HAS_CHRONO_TZDB
TEST_CASE(chrono, local_time_format_and_parse) {
    using namespace STD::chrono;

    const time_zone* tokyo = locate_zone("Asia/Tokyo");
    sys_seconds inst = fixed_utc();
    local_seconds ltok = tokyo->to_local(inst);

    STD::string abbrev = "JST";
    seconds offset = hours(9);
    auto ltf = local_time_format(ltok, &abbrev, &offset);
    test_str_eq( STD::format("{:%F %T %Z}", ltf), "2024-02-29 21:34:56 JST" );
    test_str_eq( STD::format("{:%z}", ltf), "+0900" );
    test_str_eq( STD::format("{:%Ez}", ltf), "+09:00" );

    auto plain = local_time_format(ltok);
    test_str_eq( STD::format("{:%F %T}", plain), "2024-02-29 21:34:56" );
    test_pass("cxx20:local_time_format");

    STD::istringstream i1("2024-02-29 12:34:56");
    sys_seconds t1{};
    i1 >> parse("%F %T", t1);
    test_true( !i1.fail() );
    test_true( t1 == inst );
    test_pass("cxx20:parse(sys_time)");

    STD::istringstream i2("2024-02-29");
    year_month_day ymd{};
    i2 >> parse("%F", ymd);
    test_true( !i2.fail() );
    test_true( ymd == (2024y / February / 29d) );
    test_pass("cxx20:parse(calendar)");

    STD::istringstream i3("42");
    minutes mn{};
    i3 >> parse("%M", mn);
    test_true( !i3.fail() );
    test_eq( mn.count(), 42 );
    test_pass("cxx20:parse(duration)");

    STD::istringstream i4("2024-02-29 12:34:56 JST");
    sys_seconds t4{};
    STD::string ab;
    i4 >> parse("%F %T %Z", t4, ab);
    test_true( !i4.fail() );
    test_true( t4 == inst );
    test_str_eq( ab, "JST" );
    test_pass("cxx20:parse(abbrev)");

    STD::istringstream i5("2024-02-29 21:34:56 +0900");
    sys_seconds t5{};
    minutes off{};
    i5 >> parse("%F %T %z", t5, off);
    test_true( !i5.fail() );
    test_eq( off.count(), 540 );
    test_true( t5 == inst );
    test_pass("cxx20:parse(offset)");

    STD::istringstream i6("2024-02-29 21:34:56 JST +0900");
    sys_seconds t6{};
    STD::string ab6;
    minutes off6{};
    i6 >> parse("%F %T %Z %z", t6, ab6, off6);
    test_true( !i6.fail() );
    test_str_eq( ab6, "JST" );
    test_eq( off6.count(), 540 );
    test_true( t6 == inst );
    test_pass("cxx20:parse(abbrev, offset)");

    STD::istringstream i7("2024-02-29 21:34:56");
    local_seconds l7{};
    i7 >> parse("%F %T", l7);
    test_true( !i7.fail() );
    test_true( l7 == ltok );
    test_pass("cxx20:parse(local_time)");

    STD::istringstream i8("2024-02-29");
    sys_seconds bad{};
    i8 >> parse("%T", bad);
    test_true( i8.fail() );
    test_pass("cxx20:parse failbit");
}
#else
TEST_CASE_SKIP(chrono, local_time_format_and_parse)
#endif


#if TEST_HAS_EH
TEST_CASE(chrono, zoned_time_accessors_each) {
#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_chrono) && (__cpp_lib_chrono) >= 201907L && \
    !defined(_TST_NO_TZDB)
    using namespace STD::chrono;

    test_true(( STD::is_same<zoned_seconds, zoned_time<seconds> >::value ));
    test_pass("cxx20:zoned_seconds");

    try {
        sys_seconds when = sys_days(2024y / March / 10) + hours(12);
        zoned_time<seconds> z("UTC", when);

        const time_zone* tz = z.get_time_zone();
        test_ptr( (void*)tz );
        test_true( !STD::string(tz->name()).empty() );
        test_true( tz->get_info(when).offset == seconds(0) );
        test_pass("cxx20:zoned_time::get_time_zone");

        sys_seconds got = z.get_sys_time();
        test_true( got == when );
        test_eq( (long long)got.time_since_epoch().count(),
                 (long long)when.time_since_epoch().count() );
        test_pass("cxx20:zoned_time::get_sys_time");

        local_seconds loc = z.get_local_time();
        test_eq( (long long)loc.time_since_epoch().count(),
                 (long long)when.time_since_epoch().count() );
        test_pass("cxx20:zoned_time::get_local_time");
    } catch (const STD::runtime_error&) {
        TEST_NOTE("no tzdb available at run time");
        test_skip("cxx20:zoned_time::get_time_zone");
        test_skip("cxx20:zoned_time::get_sys_time");
        test_skip("cxx20:zoned_time::get_local_time");
    }
#else
    test_skip("cxx20:zoned_seconds");
    test_skip("cxx20:zoned_time::get_time_zone");
    test_skip("cxx20:zoned_time::get_sys_time");
    test_skip("cxx20:zoned_time::get_local_time");
#endif

    TEST_NOTE("not a public member of zoned_time in any implementation in reach");
    test_skip("cxx20:zoned_time::time_zone_ptr");
    test_skip("cxx20:zoned_time::sys_time");
    test_skip("cxx20:zoned_time::local_time");

    TEST_NOTE("exposition-only member of local-time-format-t; a program cannot name it");
    test_skip("cxx20:offset_sec");

    TEST_NOTE("std::hash is still deleted for chrono types in libstdc++ 15.2");
    test_skip("cxx26:hash");
}
#else   // !TEST_HAS_EH
TEST_CASE(chrono, zoned_time_accessors_each) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx20:zoned_seconds");
    test_skip("cxx20:zoned_time::get_time_zone");
    test_skip("cxx20:zoned_time::get_sys_time");
    test_skip("cxx20:zoned_time::get_local_time");
    test_skip("cxx20:zoned_time::time_zone_ptr");
    test_skip("cxx20:zoned_time::sys_time");
    test_skip("cxx20:zoned_time::local_time");
    test_skip("cxx20:offset_sec");
    test_skip("cxx26:hash");
}
#endif  // TEST_HAS_EH
#else
TEST_CASE_SKIP(chrono, tzdb_and_tzdb_list)
TEST_CASE_SKIP(chrono, time_zone_and_sys_info)
TEST_CASE_SKIP(chrono, time_zone_dst_transitions)
TEST_CASE_SKIP(chrono, time_zone_link_and_leap_second)
TEST_CASE_SKIP(chrono, zoned_traits_and_zoned_time_ctors)
TEST_CASE_SKIP(chrono, zoned_time_members)
TEST_CASE_SKIP(chrono, local_time_format_and_parse)
TEST_CASE_SKIP(chrono, zoned_time_accessors_each)
#endif
