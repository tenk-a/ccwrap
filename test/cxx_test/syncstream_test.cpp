#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020 && _TST_HAS_SYNCSTREAM
#include <syncstream>
#include <sstream>
#include <string>
#include <ostream>

TEST_CASE(syncstream, buffered_then_emitted) {
    STD::ostringstream os;
    {
        STD::osyncstream sync(os);
        sync << "sync " << 7;
        test_true( os.str().empty() );
        sync.emit();
        test_eq( os.str(), STD::string("sync 7") );
        sync << "!";
    }
    test_eq( os.str(), STD::string("sync 7!") );
    test_pass("cxx20:buffered then emitted");
}

TEST_CASE(syncstream, get_wrapped) {
    STD::ostringstream o2;
    STD::osyncstream s2(o2);
    test_true( s2.get_wrapped() == o2.rdbuf() );
    test_pass("cxx20:get wrapped");
}

TEST_CASE(syncstream, rdbuf_and_emit_on_sync) {
    STD::ostringstream os;
    STD::osyncstream sync(os);

    STD::osyncstream::syncbuf_type* sb = sync.rdbuf();
    test_ptr( sb );
    test_true( sb->get_wrapped() == os.rdbuf() );

    (void)sb->get_allocator();

    sb->set_emit_on_sync(true);
    sync << "data";
    test_true( os.str().empty() );
    sync << STD::flush;
    test_eq( os.str(), STD::string("data") );
    test_pass("cxx20:rdbuf and emit on sync");
}

TEST_CASE(syncstream, syncbuf_direct) {

    STD::ostringstream os;
    {
        STD::syncbuf sb(os.rdbuf());
        test_true( sb.get_wrapped() == os.rdbuf() );
        sb.sputn("hello", 5);
        test_true( os.str().empty() );
        bool ok = sb.emit();
        test_true( ok );
        test_eq( os.str(), STD::string("hello") );
    }
    test_pass("cxx20:syncbuf direct");
}

#if _TST_HAS_RVALUE_REF
TEST_CASE(syncstream, osyncstream_move) {
    STD::ostringstream dest;
    {
        STD::osyncstream a(dest);
        a << "sync";
        STD::osyncstream b(STD::move(a));
        test_true( b.get_wrapped() == dest.rdbuf() );
        b << 9;
        b.emit();
        test_eq( dest.str(), STD::string("sync9") );
    }

    STD::ostringstream d2;
    {
        STD::osyncstream a(d2); a << "p";
        STD::osyncstream c(dest); c = STD::move(a);
        c.emit();
        test_eq( d2.str(), STD::string("p") );
    }
    test_pass("cxx20:osyncstream move");
}
#else
TEST_CASE_SKIP(syncstream, osyncstream_move)
#endif

TEST_CASE(syncstream, member_coverage) {
    {
        STD::ostringstream os;
        STD::osyncstream sync(os);
        sync << "OE";
        test_true( os.str().empty() );
        sync.emit();
        test_eq( os.str(), STD::string("OE") );
        test_pass("cxx20:osyncstream::emit");
    }
    {
        STD::ostringstream os;
        STD::syncbuf sb(os.rdbuf());
        test_true( sb.get_wrapped() == os.rdbuf() );
        test_pass("cxx20:syncbuf::get_wrapped");
        sb.sputn("SE", 2);
        test_true( os.str().empty() );
        bool ok = sb.emit();
        test_true( ok );
        test_eq( os.str(), STD::string("SE") );
        test_pass("cxx20:syncbuf::emit");
    }
    {
        STD::ostringstream os;
        STD::syncbuf sb(os.rdbuf());
        STD::allocator<char> a = sb.get_allocator();
        (void)a;
        test_true( true );
        test_pass("cxx20:syncbuf::get_allocator");
    }
    {
        STD::ostringstream o1, o2;
        STD::syncbuf a(o1.rdbuf());
        STD::syncbuf b(o2.rdbuf());
        a.swap(b);
        test_true( a.get_wrapped() == o2.rdbuf() );
        test_true( b.get_wrapped() == o1.rdbuf() );
        a.sputn("to2", 3);  a.emit();
        b.sputn("to1", 3);  b.emit();
        test_eq( o2.str(), STD::string("to2") );
        test_eq( o1.str(), STD::string("to1") );
        test_pass("cxx20:syncbuf::swap");
    }
}

TEST_CASE(syncstream, wide_typedefs) {
    STD::wostringstream wos;
    {
        STD::wsyncbuf wsb(wos.rdbuf());
        test_true( wsb.get_wrapped() == wos.rdbuf() );
        wsb.sputn(L"wb", 2);
        wsb.emit();
        test_true( wos.str() == L"wb" );
        test_pass("cxx20:wsyncbuf");
    }
    {
        STD::wostringstream wo2;
        STD::wosyncstream wsync(wo2);
        wsync << L"ws";
        wsync.emit();
        test_true( wo2.str() == L"ws" );
        test_pass("cxx20:wosyncstream");
    }
}
#endif


#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_syncbuf)
TEST_CASE(syncstream, syncbuf_types_cxx20) {
    typedef STD::basic_syncbuf<char> SB;
    STD::ostringstream out;
    SB sb(out.rdbuf());

    test_true(( STD::is_same<STD::syncbuf, SB>::value ));
    test_true(( STD::is_same<STD::wsyncbuf, STD::basic_syncbuf<wchar_t> >::value ));
    test_pass("cxx20:basic_syncbuf");

    test_true(( STD::is_same<SB::char_type, char>::value ));
    test_pass("cxx20:basic_syncbuf::char_type");
    test_true(( STD::is_same<SB::int_type, STD::char_traits<char>::int_type>::value ));
    test_pass("cxx20:basic_syncbuf::int_type");
    test_true(( STD::is_same<SB::pos_type, STD::char_traits<char>::pos_type>::value ));
    test_pass("cxx20:basic_syncbuf::pos_type");
    test_true(( STD::is_same<SB::off_type, STD::char_traits<char>::off_type>::value ));
    test_pass("cxx20:basic_syncbuf::off_type");
    test_true(( STD::is_same<SB::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx20:basic_syncbuf::traits_type");
    test_true(( STD::is_same<SB::allocator_type, STD::allocator<char> >::value ));
    test_pass("cxx20:basic_syncbuf::allocator_type");
    test_true(( STD::is_same<SB::streambuf_type, STD::streambuf>::value ));
    test_true( sb.get_wrapped() == out.rdbuf() );
    test_pass("cxx20:basic_syncbuf::streambuf_type");

    STD::ostream os(&sb);
    os << "held";
    test_true( out.str().empty() );
    sb.set_emit_on_sync(true);
    os << STD::flush;
    test_true( out.str() == "held" );
    test_pass("cxx20:basic_syncbuf::set_emit_on_sync");

    test_true( !STD::is_copy_assignable<SB>::value );
    test_true( STD::is_move_assignable<SB>::value );
    test_pass("cxx20:basic_syncbuf::operator=");
}

TEST_CASE(syncstream, osyncstream_types_cxx20) {
    typedef STD::basic_osyncstream<char> OS;
    STD::ostringstream out;

    test_true(( STD::is_same<STD::osyncstream, OS>::value ));
    test_true(( STD::is_same<STD::wosyncstream, STD::basic_osyncstream<wchar_t> >::value ));
    {
        OS os(out);
        os << "line";
        test_true( out.str().empty() );
    }
    test_true( out.str() == "line" );
    test_pass("cxx20:basic_osyncstream");

    test_true(( STD::is_same<OS::char_type, char>::value ));
    test_pass("cxx20:basic_osyncstream::char_type");
    test_true(( STD::is_same<OS::int_type, STD::char_traits<char>::int_type>::value ));
    test_pass("cxx20:basic_osyncstream::int_type");
    test_true(( STD::is_same<OS::pos_type, STD::char_traits<char>::pos_type>::value ));
    test_pass("cxx20:basic_osyncstream::pos_type");
    test_true(( STD::is_same<OS::off_type, STD::char_traits<char>::off_type>::value ));
    test_pass("cxx20:basic_osyncstream::off_type");
    test_true(( STD::is_same<OS::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx20:basic_osyncstream::traits_type");
    test_true(( STD::is_same<OS::allocator_type, STD::allocator<char> >::value ));
    test_pass("cxx20:basic_osyncstream::allocator_type");
    test_true(( STD::is_same<OS::streambuf_type, STD::streambuf>::value ));
    test_pass("cxx20:basic_osyncstream::streambuf_type");
    test_true(( STD::is_same<OS::syncbuf_type, STD::basic_syncbuf<char> >::value ));
    STD::ostringstream out2;
    OS os2(out2);
    test_true( os2.rdbuf()->get_wrapped() == out2.rdbuf() );
    test_pass("cxx20:basic_osyncstream::syncbuf_type");

    test_true( !STD::is_copy_assignable<OS>::value );
    test_true( STD::is_move_assignable<OS>::value );
    test_pass("cxx20:basic_osyncstream::operator=");
}
#else
TEST_CASE(syncstream, syncbuf_types_cxx20) {
    test_skip("cxx20:basic_syncbuf");
    test_skip("cxx20:basic_syncbuf::char_type");
    test_skip("cxx20:basic_syncbuf::int_type");
    test_skip("cxx20:basic_syncbuf::pos_type");
    test_skip("cxx20:basic_syncbuf::off_type");
    test_skip("cxx20:basic_syncbuf::traits_type");
    test_skip("cxx20:basic_syncbuf::allocator_type");
    test_skip("cxx20:basic_syncbuf::streambuf_type");
    test_skip("cxx20:basic_syncbuf::set_emit_on_sync");
    test_skip("cxx20:basic_syncbuf::operator=");
}
TEST_CASE(syncstream, osyncstream_types_cxx20) {
    test_skip("cxx20:basic_osyncstream");
    test_skip("cxx20:basic_osyncstream::char_type");
    test_skip("cxx20:basic_osyncstream::int_type");
    test_skip("cxx20:basic_osyncstream::pos_type");
    test_skip("cxx20:basic_osyncstream::off_type");
    test_skip("cxx20:basic_osyncstream::traits_type");
    test_skip("cxx20:basic_osyncstream::allocator_type");
    test_skip("cxx20:basic_osyncstream::streambuf_type");
    test_skip("cxx20:basic_osyncstream::syncbuf_type");
    test_skip("cxx20:basic_osyncstream::operator=");
}
#endif

TEST_CASE(syncstream, feature_test_macros) {
#if defined(__cpp_lib_syncbuf) && (__cpp_lib_syncbuf) >= 201803L
    test_true( (__cpp_lib_syncbuf) >= 201803L );
    test_pass("cxx20:__cpp_lib_syncbuf=201803L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_syncbuf=201803L");
#endif
}
