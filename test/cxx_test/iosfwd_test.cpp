#include "test_cxx.hpp"

#include <iosfwd>

TEST_CASE(iosfwd, forward_declarations) {

    STD::ios*        p_ios   = 0;
    STD::streambuf*  p_sb    = 0;
    STD::istream*    p_is    = 0;
    STD::ostream*    p_os    = 0;
    STD::iostream*   p_ios2  = 0;
    STD::stringbuf*  p_ssb   = 0;
    STD::stringstream* p_ss  = 0;
    STD::filebuf*    p_fb    = 0;
    STD::fstream*    p_fs    = 0;
    STD::string*     p_str   = 0;

    STD::wios*        p_wios  = 0;
    STD::wstreambuf*  p_wsb   = 0;
    STD::wistream*    p_wis   = 0;
    STD::wostream*    p_wos   = 0;
    STD::wiostream*   p_wios2 = 0;
    STD::wstringbuf*  p_wssb  = 0;
    STD::wistringstream* p_wiss = 0;
    STD::wostringstream* p_woss = 0;
    STD::wstringstream*  p_wss  = 0;
    STD::wfilebuf*    p_wfb   = 0;
    STD::wifstream*   p_wifs  = 0;
    STD::wofstream*   p_wofs  = 0;
    STD::wfstream*    p_wfs   = 0;
    STD::wstring*     p_wstr  = 0;
#if _TST_HAS_CHAR16_32
    STD::u16string*   p_u16   = 0;
    STD::u32string*   p_u32   = 0;
#endif

#if _TST_HAS_CXX20_LIB_MEMBERS && _TST_HAS_SYNCSTREAM
    STD::syncbuf*     p_syb   = 0;
    STD::osyncstream* p_osy   = 0;
    STD::wsyncbuf*    p_wsyb  = 0;
    STD::wosyncstream* p_wosy = 0;
#endif
#if _TST_HAS_CXX20_LIB_MEMBERS && !defined(__GNUC__)
    STD::spanbuf*     p_spb   = 0;
    STD::ispanstream* p_isp   = 0;
    STD::ospanstream* p_osp   = 0;
    STD::spanstream*  p_sp    = 0;
    STD::wspanbuf*    p_wspb  = 0;
    STD::wspanstream* p_wsp   = 0;
#endif

    STD::istreambuf_iterator<char>* p_isbi = 0;
    STD::ostreambuf_iterator<char>* p_osbi = 0;

    STD::fpos<int>*   p_fpos = 0;
    STD::streampos*   p_spos = 0;
    STD::wstreampos*  p_wspos = 0;
#if _TST_HAS_CHAR16_32
    STD::u16streampos* p_u16pos = 0;
    STD::u32streampos* p_u32pos = 0;
#endif

    bool __all_null =
        !p_ios && !p_sb && !p_is && !p_os && !p_ios2 && !p_ssb && !p_ss && !p_fb && !p_fs && !p_str &&
        !p_wios && !p_wsb && !p_wis && !p_wos && !p_wios2 && !p_wssb && !p_wiss && !p_woss && !p_wss &&
        !p_wfb && !p_wifs && !p_wofs && !p_wfs && !p_wstr &&
#if _TST_HAS_CHAR16_32
        !p_u16 && !p_u32 &&
#endif
#if _TST_HAS_CXX20_LIB_MEMBERS && _TST_HAS_SYNCSTREAM
        !p_syb && !p_osy && !p_wsyb && !p_wosy &&
# if !defined(__GNUC__)
        !p_spb && !p_isp && !p_osp && !p_sp && !p_wspb && !p_wsp &&
# endif
#endif
        !p_isbi && !p_osbi &&
#if _TST_HAS_CHAR16_32
        !p_u16pos && !p_u32pos &&
#endif
        !p_fpos && !p_spos && !p_wspos;
    test_true( __all_null );
    test_pass("cxx03:iostream forward declarations");
}

TEST_CASE(iosfwd, named_forward_declarations) {

    test_true( sizeof(STD::ios*)        > 0 ); test_pass("cxx03:ios");
    test_true( sizeof(STD::streambuf*)  > 0 ); test_pass("cxx03:streambuf");
    test_true( sizeof(STD::istream*)    > 0 ); test_pass("cxx03:istream");
    test_true( sizeof(STD::ostream*)    > 0 ); test_pass("cxx03:ostream");
    test_true( sizeof(STD::iostream*)   > 0 ); test_pass("cxx03:iostream");
    test_true( sizeof(STD::filebuf*)    > 0 ); test_pass("cxx03:filebuf");
    test_true( sizeof(STD::ifstream*)   > 0 ); test_pass("cxx03:ifstream");
    test_true( sizeof(STD::ofstream*)   > 0 ); test_pass("cxx03:ofstream");
    test_true( sizeof(STD::fstream*)    > 0 ); test_pass("cxx03:fstream");
    test_true( sizeof(STD::stringbuf*)  > 0 ); test_pass("cxx03:stringbuf");
    test_true( sizeof(STD::istringstream*) > 0 ); test_pass("cxx03:istringstream");
    test_true( sizeof(STD::ostringstream*) > 0 ); test_pass("cxx03:ostringstream");
    test_true( sizeof(STD::stringstream*)  > 0 ); test_pass("cxx03:stringstream");

    test_true( sizeof(STD::wios*)       > 0 ); test_pass("cxx03:wios");
    test_true( sizeof(STD::wstreambuf*) > 0 ); test_pass("cxx03:wstreambuf");
    test_true( sizeof(STD::wistream*)   > 0 ); test_pass("cxx03:wistream");
    test_true( sizeof(STD::wostream*)   > 0 ); test_pass("cxx03:wostream");
    test_true( sizeof(STD::wiostream*)  > 0 ); test_pass("cxx03:wiostream");
    test_true( sizeof(STD::wfilebuf*)   > 0 ); test_pass("cxx03:wfilebuf");
    test_true( sizeof(STD::wifstream*)  > 0 ); test_pass("cxx03:wifstream");
    test_true( sizeof(STD::wofstream*)  > 0 ); test_pass("cxx03:wofstream");
    test_true( sizeof(STD::wfstream*)   > 0 ); test_pass("cxx03:wfstream");
    test_true( sizeof(STD::wstringbuf*) > 0 ); test_pass("cxx03:wstringbuf");
    test_true( sizeof(STD::wistringstream*) > 0 ); test_pass("cxx03:wistringstream");
    test_true( sizeof(STD::wostringstream*) > 0 ); test_pass("cxx03:wostringstream");
    test_true( sizeof(STD::wstringstream*)  > 0 ); test_pass("cxx03:wstringstream");

    test_true( sizeof(STD::istreambuf_iterator<char>*) > 0 ); test_pass("cxx03:istreambuf_iterator");
    test_true( sizeof(STD::ostreambuf_iterator<char>*) > 0 ); test_pass("cxx03:ostreambuf_iterator");

    test_true( sizeof(STD::streamoff)  > 0 ); test_pass("cxx03:streamoff");
    test_true( sizeof(STD::streamsize) > 0 ); test_pass("cxx03:streamsize");
    test_true( sizeof(STD::streampos)  > 0 ); test_pass("cxx03:streampos");
    test_true( sizeof(STD::wstreampos) > 0 ); test_pass("cxx03:wstreampos");
    test_true( sizeof(STD::fpos<STD::mbstate_t>*) > 0 ); test_pass("cxx03:fpos");
#if _TST_HAS_CHAR16_32
    test_true( sizeof(STD::u16streampos) > 0 ); test_pass("cxx11:u16streampos");
    test_true( sizeof(STD::u32streampos) > 0 ); test_pass("cxx11:u32streampos");
#else
    test_skip("cxx11:u16streampos"); test_skip("cxx11:u32streampos");
#endif

#if defined(__cpp_lib_syncbuf)
    test_true( sizeof(STD::syncbuf*)      > 0 ); test_pass("cxx20:syncbuf");
    test_true( sizeof(STD::osyncstream*)  > 0 ); test_pass("cxx20:osyncstream");
    test_true( sizeof(STD::wsyncbuf*)     > 0 ); test_pass("cxx20:wsyncbuf");
    test_true( sizeof(STD::wosyncstream*) > 0 ); test_pass("cxx20:wosyncstream");
#else
    test_skip("cxx20:syncbuf"); test_skip("cxx20:osyncstream");
    test_skip("cxx20:wsyncbuf"); test_skip("cxx20:wosyncstream");
#endif
#if _TST_HAS_CHAR8_NAME
    test_true( sizeof(STD::u8streampos) > 0 ); test_pass("cxx20:u8streampos");
#else
    test_skip("cxx20:u8streampos");
#endif

#if defined(__cpp_lib_spanstream)
    test_true( sizeof(STD::spanbuf*)     > 0 ); test_pass("cxx23:spanbuf");
    test_true( sizeof(STD::spanstream*)  > 0 ); test_pass("cxx23:spanstream");
    test_true( sizeof(STD::ispanstream*) > 0 ); test_pass("cxx23:ispanstream");
    test_true( sizeof(STD::ospanstream*) > 0 ); test_pass("cxx23:ospanstream");
    test_true( sizeof(STD::wspanbuf*)    > 0 ); test_pass("cxx23:wspanbuf");
#else
    test_skip("cxx23:spanbuf"); test_skip("cxx23:spanstream"); test_skip("cxx23:ispanstream");
    test_skip("cxx23:ospanstream"); test_skip("cxx23:wspanbuf");
#endif
}

TEST_CASE(iosfwd, class_templates_cxx03) {
    STD::char_traits<char>*        p_tr  = 0;
    STD::char_traits<wchar_t>*     p_wtr = 0;
    test_true( p_tr == 0 && p_wtr == 0 );
    test_pass("cxx03:char_traits");

    STD::allocator<char>*          p_al  = 0;
    STD::allocator<int>*           p_ali = 0;
    test_true( p_al == 0 && p_ali == 0 );
    test_pass("cxx03:allocator");

    STD::basic_ios<char>*          p_ios = 0;   test_true( p_ios == 0 );
    test_pass("cxx03:basic_ios");
    STD::basic_streambuf<char>*    p_sb  = 0;   test_true( p_sb == 0 );
    test_pass("cxx03:basic_streambuf");
    STD::basic_istream<char>*      p_is  = 0;   test_true( p_is == 0 );
    test_pass("cxx03:basic_istream");
    STD::basic_ostream<char>*      p_os  = 0;   test_true( p_os == 0 );
    test_pass("cxx03:basic_ostream");
    STD::basic_iostream<char>*     p_io  = 0;   test_true( p_io == 0 );
    test_pass("cxx03:basic_iostream");
    STD::basic_stringbuf<char>*    p_ssb = 0;   test_true( p_ssb == 0 );
    test_pass("cxx03:basic_stringbuf");
    STD::basic_stringstream<char>* p_ss  = 0;   test_true( p_ss == 0 );
    test_pass("cxx03:basic_stringstream");
    STD::basic_filebuf<char>*      p_fb  = 0;   test_true( p_fb == 0 );
    test_pass("cxx03:basic_filebuf");
    STD::basic_ifstream<char>*     p_ifs = 0;   test_true( p_ifs == 0 );
    test_pass("cxx03:basic_ifstream");
    STD::basic_ofstream<char>*     p_ofs = 0;   test_true( p_ofs == 0 );
    test_pass("cxx03:basic_ofstream");
    STD::basic_fstream<char>*      p_fs  = 0;   test_true( p_fs == 0 );
    test_pass("cxx03:basic_fstream");

    STD::basic_istringstream<char>* p_iss = 0;  test_true( p_iss == 0 );
    test_pass("cxx03:basic_istringstream");
    STD::basic_ostringstream<char>* p_oss = 0;  test_true( p_oss == 0 );
    test_pass("cxx03:basic_ostringstream");
}

TEST_CASE(iosfwd, class_templates_cxx20) {
#if defined(__cpp_lib_syncbuf)
    STD::basic_syncbuf<char>*      p_sy = 0;   test_true( p_sy == 0 );
    test_pass("cxx20:basic_syncbuf");
    STD::basic_osyncstream<char>*  p_oy = 0;   test_true( p_oy == 0 );
    test_pass("cxx20:basic_osyncstream");
#else
    test_skip("cxx20:basic_syncbuf");
    test_skip("cxx20:basic_osyncstream");
#endif

#if defined(__cpp_lib_spanstream)
    STD::basic_spanbuf<char>*      p_sp  = 0;  test_true( p_sp == 0 );
    test_pass("cxx23:basic_spanbuf");
    STD::basic_spanstream<char>*   p_sps = 0;  test_true( p_sps == 0 );
    test_pass("cxx23:basic_spanstream");
    STD::basic_ispanstream<char>*  p_isp = 0;  test_true( p_isp == 0 );
    test_pass("cxx23:basic_ispanstream");
    STD::basic_ospanstream<char>*  p_osp = 0;  test_true( p_osp == 0 );
    test_pass("cxx23:basic_ospanstream");
    STD::wspanstream*              p_wsp = 0;  test_true( p_wsp == 0 );
    test_pass("cxx23:wspanstream");
    STD::wispanstream*             p_wis = 0;  test_true( p_wis == 0 );
    test_pass("cxx23:wispanstream");
    STD::wospanstream*             p_wos = 0;  test_true( p_wos == 0 );
    test_pass("cxx23:wospanstream");
#else
    test_skip("cxx23:basic_spanbuf");
    test_skip("cxx23:basic_spanstream");
    test_skip("cxx23:basic_ispanstream");
    test_skip("cxx23:basic_ospanstream");
    test_skip("cxx23:wspanstream");
    test_skip("cxx23:wispanstream");
    test_skip("cxx23:wospanstream");
#endif
}

TEST_CASE(iosfwd, feature_test_macros) {
#if defined(__cpp_lib_char8_t)
    test_true( __cpp_lib_char8_t > 0L );
    test_pass("cxx20:__cpp_lib_char8_t");
#else
    test_skip("cxx20:__cpp_lib_char8_t");
#endif
#if defined(__cpp_lib_spanstream)
    test_true( __cpp_lib_spanstream > 0L );
    test_pass("cxx23:__cpp_lib_spanstream");
#else
    test_skip("cxx23:__cpp_lib_spanstream");
#endif
}
