//#include <_ccwrap/ccwrap_common.h>
#include <_ccwrap/ccwrap_test.hpp>
#include <_ccwrap/utfenc.hpp>

#if 0
namespace __Fkstd_TestSuite_StringCodeConv {
    using namespace ccwrap;
    static __ccwrap::test::TestSuite<>& testSuite() {
        static __ccwrap::test::TestSuite<>*  s_instance = 0;
        if (!s_instance)
            s_instance =__ccwrap::test::TestMgr<>::instance().add("StringCodeConv");
        return *s_instance;
    }

struct __TestMethod_check {
    __TestMethod_check() {
        testSuite().add("check", &__TestMethod_check::run);
    }
    static void run();
private:
    static __TestMethod_check  s_instance;
};
__TestMethod_check __TestMethod_check::s_instance;
void                __TestMethod_check::run()
#else
CCWRAP_TEST_SUITE(StringCodeConv) {
    using namespace ccwrap;

    CCWRAP_TEST(check)
#endif
    {
        using namespace std;
        struct St {
            char        mbs[16];
            wchar_t     wcs[16];
            char8_t     u8s[16];
            char16_t    u16s[16];
            char32_t    u32s[16];
        };
        static St const strs[] = {
            {
                "\x83\x5c\x81\x5b\x83\x58",
                { 0x30Bd, 0x30fc, 0x30B9,0,},   //L"ソース",
                {0xE3,0x82,0xBD,0xE3,0x83,0xBC,0xE3,0x82,0xB9,0},
                { 0x30Bd, 0x30fc, 0x30B9,0,},
                { 0x30Bd, 0x30fc, 0x30B9,0,},
            },
            {
                "\x93\x4B\x93\x96",
                { 0x9069, 0x5f53, 0},   // L"適当"
                { 0xE9, 0x81, 0xA9, 0xE5, 0xBD, 0x93,0 },
                { 0x9069, 0x5f53, 0},
                { 0x9069, 0x5f53, 0},
            },
            {
                "\x8E\x64\x8E\x96",
                { 0x4ed5, 0x4e8b, 0},   //L"仕事",
                {0xE4,0xBB,0x95,0xE4,0xBA,0x8B, 0},
                { 0x4ed5, 0x4e8b, 0},
                { 0x4ed5, 0x4e8b, 0},
            },
            {
                "\x93\x4B\x93\x96\x82\xC8\x8E\x64\x8E\x96",
                { 0x9069, 0x5f53, 0x306A, 0x4ed5, 0x4e8b, 0},   //L"適当な仕事",
                { 0xE9,0x81,0xA9, 0xE5,0xBD,0x93, 0xE3,0x81,0xAA, 0xE4,0xBB,0x95, 0xE4,0xBA,0x8B, 0},
                { 0x9069, 0x5f53, 0x306A, 0x4ed5, 0x4e8b, 0},   //u16"適当な仕事",
                { 0x9069, 0x5f53, 0x306A, 0x4ed5, 0x4e8b, 0},   //u32"適当な仕事",
            },
            {
                "Ascii",
                { 0x41,0x73,0x63,0x69,0x69, 0},
                { 0x41,0x73,0x63,0x69,0x69, 0},
                { 0x41,0x73,0x63,0x69,0x69, 0},
                { 0x41,0x73,0x63,0x69,0x69, 0},
            },
        };
        string      ascs1(strs[4].mbs);
        u8string    u8s1(strs[0].u8s);
        u16string   u16s1(strs[1].u16s);
        u32string   u32s1(strs[2].u32s);
        wstring     wcs1(strs[3].wcs);

        ccwrap_test_assert(ascs1 == utfenc::to<string>(strs[4].wcs));
        ccwrap_test_assert(ascs1 == utfenc::to<string>(strs[4].u8s));
        ccwrap_test_assert(ascs1 == utfenc::to<string>(strs[4].u16s));
        ccwrap_test_assert(ascs1 == utfenc::to<string>(strs[4].u32s));
        ccwrap_test_assert(strs[4].wcs == utfenc::to<wstring>(strs[4].mbs));
        ccwrap_test_assert(strs[4].u8s == utfenc::to<u8string>(strs[4].mbs));
        ccwrap_test_assert(strs[4].u16s == utfenc::to<u16string>(strs[4].mbs));
        ccwrap_test_assert(strs[4].u32s == utfenc::to<u32string>(strs[4].mbs));

     //#if !defined(CCWRAP_CHAR_IS_UTF8)
     #if defined(CCWRAP_CHAR_IS_SJIS)
        string      mbs1(strs[3].mbs);
        ccwrap_test_assert(mbs1 == utfenc::to<string>(strs[3].wcs));
        ccwrap_test_assert(mbs1 == utfenc::to<string>(strs[3].u8s));
        ccwrap_test_assert(mbs1 == utfenc::to<string>(strs[3].u16s));
        ccwrap_test_assert(mbs1 == utfenc::to<string>(strs[3].u32s));
        ccwrap_test_assert(wcs1 == utfenc::to<wstring>(strs[3].mbs));
        ccwrap_test_assert(u8s1 == utfenc::to<u8string>(strs[0].mbs));
        ccwrap_test_assert(u16s1 == utfenc::to<u16string>(strs[1].mbs));
        ccwrap_test_assert(u32s1 == utfenc::to<u32string>(strs[2].mbs));
     #endif

        ccwrap_test_assert(wcs1 == utfenc::to<wstring>(strs[3].u8s));
        ccwrap_test_assert(wcs1 == utfenc::to<wstring>(strs[3].u16s));
        ccwrap_test_assert(wcs1 == utfenc::to<wstring>(strs[3].u32s));

        ccwrap_test_assert(u8s1 == utfenc::to<u8string>(strs[0].wcs));
        ccwrap_test_assert(u8s1 == utfenc::to<u8string>(strs[0].u16s));
        ccwrap_test_assert(u8s1 == utfenc::to<u8string>(strs[0].u32s));

        ccwrap_test_assert(u16s1 == utfenc::to<u16string>(strs[1].wcs));
        ccwrap_test_assert(u16s1 == utfenc::to<u16string>(strs[1].u8s));
        ccwrap_test_assert(u16s1 == utfenc::to<u16string>(strs[1].u32s));

        ccwrap_test_assert(u32s1 == utfenc::to<u32string>(strs[2].wcs));
        ccwrap_test_assert(u32s1 == utfenc::to<u32string>(strs[2].u8s));
        ccwrap_test_assert(u32s1 == utfenc::to<u32string>(strs[2].u16s));
    }
}
