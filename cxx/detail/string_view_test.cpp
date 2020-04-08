/**
 *  @file   string_view_test.cpp
 *  @author tenk*
 *  @date   2010,2020
 */

//#undef _DEBUG

#include <cstddef>
#include <cassert>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <algorithm>
#include <vector>

//#undef assert
//#define assert(x)       TEST_CHECK(x)

#include <iostream>
#include <string>
#include <fstream>

//#include "string_view.hpp"

#ifdef UNICODE
#define T(a)            L##a
#else
#define T(a)            a
#endif
//#define T(a)          L##a

#ifndef TEST_CHECK
#define  TEST_CHECK(cc) do {                                                            \
        try {                                                                           \
            ((cc) || printf("%s (%d) : `%s' is failed.\n", __FILE__, __LINE__, #cc));   \
        } catch (...) {                                                                 \
            printf("%s (%d) : `%s' thew.\n", __FILE__, __LINE__, #cc);                  \
        }                                                                               \
    } while (0)
#define  TEST_EQ(a,b)                                                                   \
    do {                                                                                \
        try {                                                                           \
            if ((a) != (b)) {                                                           \
                std::cout << __FILE__ << " (" << __LINE__ << ") : "                     \
                  << #a << "(" << a << ") and " << #b << "(" << b << ") are not equal.\n";  \
            }                                                                           \
        } catch (...) {                                                                 \
            std::cout << __FILE__ << " (" << __LINE__ << ") : `"                        \
                << #a << "(" << a << ") == " << #b << "(" << b << ")' are threw.\n";    \
        }                                                                               \
    } while (0)

#define TEST_THROW(exp,E)   \
    do { bool f=0; (void)f; \
        try { exp; } catch (E) { f=1; } catch (...) {}  \
        if (!f) { printf("%s (%d): (%s) threw not %s.\n", __FILE__, __LINE__, #exp, #E); } \
    } while (0)
#define TEST_NO_THROW(exp)  \
    do { bool f=0; (void)f; \
        try { exp; } catch (...) {f=1;} \
        if (f) { printf("%s (%d): (%s) threw.\n", __FILE__, __LINE__, #exp); } \
    } while (0)

#define TEST_OUT_OF_RANGE(a)        TEST_THROW(a, std::out_of_range)
#endif

template<class STRING_VIEW>
class StringView_Test {
    typedef typename STRING_VIEW::value_type char_t;
public:
    void test() {
        test_constructor();
        test_compare();
        test_etc();
        test_find();
        test_rfind();
        test_find_first_of();
        test_find_last_of();
        test_find_first_not_of();
        test_find_last_not_of();
        test_copy();
        test_swap();
        test_substr();
    }


private:

    /// コンストラクタ, empty,size,capacity,max_size,length を使用
    void test_constructor() {
        // デフォルト・コンストラクタ
        STRING_VIEW str0;
        TEST_CHECK (str0.empty());
        TEST_CHECK (str0.size() == 0);
        //TEST_CHECK (str0.capacity() >= 0);
        TEST_CHECK (str0.max_size() >= str0.size());

        const STRING_VIEW str1(T("0123456789"));
        //STRING_VIEW   str1("          ");
        TEST_CHECK (str1.empty() == false);
        TEST_CHECK (str1.size() == 10);
        TEST_CHECK (str1.length() == 10);
        //TEST_CHECK (str1.capacity() >= 10);
        TEST_CHECK (str1.max_size() >= str1.size());

        STRING_VIEW str2(T("@@@@@@@@@@@@@@@@@@@@"), 10);
        TEST_CHECK (str2.empty() == false);
        TEST_CHECK (str2.size() == 10);
        TEST_CHECK (str1.length() == 10);
        //TEST_CHECK (str2.capacity() >= 10);
        TEST_CHECK ( std::memcmp(&str2[0], T("@@@@@@@@@@"), 10) == 0 );
        //TEST_CHECK ( std::memcmp(str2.c_str(), T("@@@@@@@@@@"), 11) == 0 );
        TEST_CHECK (str2.max_size() >= str2.size());

        STRING_VIEW str3(str1);
        TEST_CHECK (str3.empty() == false);
        TEST_CHECK (str3.size() == 10);
        //TEST_CHECK (str3.capacity() >= 10);
        TEST_CHECK ( std::memcmp(str3.data() , T("0123456789"), 10) == 0 );
        //TEST_CHECK ( std::memcmp(str3.c_str(), T("0123456789"), 11) == 0 );
        TEST_CHECK (str3.max_size() >= str3.size());
        TEST_CHECK ( std::memcmp(str3.data() , T("0123456789"), 10) == 0 );
        //TEST_CHECK ( std::memcmp(str3.c_str(), T("0123456789"), 11) == 0 );

//      TEST_OUT_OF_RANGE( STRING_VIEW  str4a(str1,100) );

        //STRING_VIEW   str4(str1, 2);
        STRING_VIEW str4(&str1[2], str1.size()-2);
        TEST_CHECK (str4.empty() == false);
        TEST_CHECK (str4.size() == 8);
        //TEST_CHECK (str4.capacity() >= 8);
        TEST_CHECK ( std::memcmp(str4.data() , T("23456789"), 8) == 0 );
        //TEST_CHECK ( std::memcmp(str4.c_str(), T("23456789"), 9) == 0 );
        TEST_CHECK (str4.max_size() >= str4.size());

//      TEST_OUT_OF_RANGE( STRING_VIEW  str5a(str1, 100, 5) );

        //STRING_VIEW   str5(str1, 3, 5);
        STRING_VIEW str5(&str1[3], 5);
        TEST_CHECK (str5.empty() == false);
        TEST_CHECK (str5.size() == 5);
        //TEST_CHECK (str5.capacity() >= 5);
        TEST_CHECK ( std::memcmp(str5.data() , T("34567"), 5) == 0 );
        //TEST_CHECK ( std::memcmp(str5.c_str(), T("34567"), 6) == 0 );
        TEST_CHECK (str5.max_size() >= str5.size());

     #if 0
        STRING_VIEW str6(T("0123456789"), 6);
        TEST_CHECK (str6.empty() == false);
        TEST_CHECK (str6.size() == 6);
        TEST_CHECK (str6.capacity() >= 6);
        TEST_CHECK ( std::memcmp(str6.data(),  T("012345"), 6) == 0 );
        //TEST_CHECK ( std::memcmp(str6.c_str(), T("012345"), 7) == 0 );
        TEST_CHECK (str6.max_size() >= str6.size());
     #endif

     #if 0
        STRING_VIEW str7(str1.begin(), str1.end());
        TEST_CHECK (str7.empty() == false);
        TEST_CHECK (str7.size() == 10);
        TEST_CHECK (str7.capacity() >= 10);
        TEST_CHECK ( std::memcmp(str7.data(),  T("0123456789"), 10) == 0 );
        //TEST_CHECK ( std::memcmp(str7.c_str(), T("0123456789"), 11) == 0 );
        TEST_CHECK (str7.max_size() >= str7.size());
     #endif

        STRING_VIEW str8(T("abcd\0efghi"), 10);
        TEST_CHECK (str8.empty() == false);
        std::size_t l = str8.size();
        TEST_CHECK (l == 10);
        //TEST_CHECK (str8.capacity() >= 10);
        TEST_CHECK ( std::memcmp(str8.data(),  T("abcd\0efghi"), 10) == 0 );
        //TEST_CHECK ( std::memcmp(str8.c_str(), T("abcd"), 5) == 0 );
        TEST_CHECK (str8.max_size() >= str8.size());

     #if 0
        STRING_VIEW str9(str1.end(), str1.end());
        TEST_CHECK (str9.empty() != false);
        TEST_CHECK (str9.size() == 0);
        TEST_CHECK (str9.max_size() >= str9.size());
     #endif
    }

    /// 比較
    void test_compare()
    {
        const STRING_VIEW   str0;
        const STRING_VIEW   str1(T("0123456789"));
        const STRING_VIEW   str5(str1.data()+3, 5);
        const STRING_VIEW   str6(T("abcdefghij"));
        const STRING_VIEW   str7(T("abcdefghij"), 7);
        const STRING_VIEW   str8(str1);

        TEST_CHECK (str1 != str0);
        TEST_CHECK ((str1 == str0) == 0);
        TEST_CHECK (str0 <= str1);
        TEST_CHECK (str0 <  str1);
        TEST_CHECK (str1 >= str0);
        TEST_CHECK (str1 >  str0);

        TEST_CHECK (str8 != str7);
        TEST_CHECK ((str8 == str7) == 0);
        TEST_CHECK (str8 <= str7);
        TEST_CHECK (str8 <  str7);
        TEST_CHECK (str7 >= str8);
        TEST_CHECK (str7 >  str8);

        TEST_CHECK (str6 != str7);
        TEST_CHECK ((str6 == str7) == 0);
        TEST_CHECK (str6 >= str7);
        TEST_CHECK (str6 >  str7);
        TEST_CHECK (str7 <= str6);
        TEST_CHECK (str7 <  str6);

        TEST_CHECK (str8 == str1);
        TEST_CHECK ((str8 != str1) == 0);
        TEST_CHECK ((str8 <= str1) != 0);
        TEST_CHECK ((str8 <  str1) == 0);
        TEST_CHECK ((str8 >= str1) != 0);
        TEST_CHECK ((str8 >  str1) == 0);

        TEST_CHECK ((str0 == T("")) != 0);
        TEST_CHECK ((str0 != T("")) == 0);
        TEST_CHECK ((str0 <= T("")) != 0);
        TEST_CHECK ((str0 <  T("")) == 0);
        TEST_CHECK ((str0 >= T("")) != 0);
        TEST_CHECK ((str0 >  T("")) == 0);

        TEST_CHECK ((str1 == T("")) == 0);
        TEST_CHECK ((str1 != T("")) != 0);
        TEST_CHECK ((str1 <= T("")) == 0);
        TEST_CHECK ((str1 <  T("")) == 0);
        TEST_CHECK ((str1 >= T("")) != 0);
        TEST_CHECK ((str1 >  T("")) != 0);

        TEST_CHECK ((str8 == T("0123456789")) != 0);
        TEST_CHECK ((str8 != T("0123456789")) == 0);
        TEST_CHECK ((str8 <= T("0123456789")) != 0);
        TEST_CHECK ((str8 <  T("0123456789")) == 0);
        TEST_CHECK ((str8 >= T("0123456789")) != 0);
        TEST_CHECK ((str8 >  T("0123456789")) == 0);

        TEST_CHECK ((T("") == str0) != 0);
        TEST_CHECK ((T("") != str0) == 0);
        TEST_CHECK ((T("") <= str0) != 0);
        TEST_CHECK ((T("") < str0) == 0);
        TEST_CHECK ((T("") >= str0) != 0);
        TEST_CHECK ((T("") > str0) == 0);

        TEST_CHECK ((T("") == str1) == 0);
        TEST_CHECK ((T("") != str1) != 0);
        TEST_CHECK ((T("") <= str1) != 0);
        TEST_CHECK ((T("") < str1) != 0);
        TEST_CHECK ((T("") >= str1) == 0);
        TEST_CHECK ((T("") > str1) == 0);

        TEST_CHECK (T("0123456789") == str1);
        TEST_CHECK ((T("0123456789") != str1) == 0);
        TEST_CHECK ((T("0123456789") <= str1) != 0);
        TEST_CHECK ((T("0123456789") <  str1) == 0);
        TEST_CHECK ((T("0123456789") >= str1) != 0);
        TEST_CHECK ((T("0123456789") >  str1) == 0);

        TEST_CHECK (str8.compare(str1) == 0);
        TEST_CHECK (str8.compare(str7) < 0);
        TEST_CHECK (str7.compare(str8) > 0);
        TEST_CHECK (str6.compare(str7) > 0);
        TEST_CHECK (str7.compare(str6) < 0);
        TEST_CHECK (str8.compare(T("0123456789")) == 0);
        TEST_CHECK (str8.compare(T("0123456789a")) < 0);
        TEST_CHECK (str8.compare(T("012345678")) > 0);
        //x TEST_CHECK (str4.compare(str0,2) == 0);
        //TEST_CHECK (str5.compare(str0,3,5) == 0);
        TEST_CHECK (str1.compare(3,5,str5) == 0);
        TEST_CHECK (str1.compare(3,4,str5) <    0);
        TEST_CHECK (str1.compare(3,6,str5) >    0);
        //x TEST_CHECK (str1.compare(3,5,str5,0) == 0);
        TEST_CHECK (str1.compare(3,5,str5,0,5) == 0);
        TEST_CHECK (str1.compare(3,5,str5,0,4) >    0);
        TEST_CHECK (str1.compare(3,4,str5,0,5) <    0);

        TEST_CHECK (str1.compare(3,5,T("3456789"),5) == 0);
        TEST_CHECK (str1.compare(3,5,T("3456789"),6) <  0);
        TEST_CHECK (str1.compare(3,5,T("3456789"),12) <  0);
        TEST_CHECK (str1.compare(3,5,T("3456789"),4) >  0);
        TEST_CHECK (str1.compare(2,5,T("3456789"),5) <  0);
        TEST_CHECK (str1.compare(2,5,T("3456789"),12) <  0);

        TEST_CHECK (str1.compare(8,8,str8, 8,8) == 0);
        TEST_CHECK (str1.compare(8,8,str8, 8,2) == 0);
        TEST_CHECK (str1.compare(8,8,str8, 8,1) >  0);
        TEST_CHECK (str1.compare(8,8,str8, 7,5) >  0);
        TEST_CHECK (str1.compare(8,8,str8, 9,2) <  0);

        TEST_CHECK (str1.compare(8,8,T("89")) == 0);
        TEST_CHECK (str1.compare(8,8,T("89"),2) == 0);
        TEST_CHECK (str1.compare(8,8,T("89abcd")) < 0);
        TEST_CHECK (str1.compare(8,8,T("89abcd"),6) < 0);
        TEST_CHECK (str1.compare(8,8,T("789abcd")) > 0);
        TEST_CHECK (str1.compare(8,8,T("789abcd"),7) > 0);
        TEST_CHECK (str1.compare(8,8,T("89abcd"), 2) == 0);
        TEST_CHECK (str1.compare(8,8,T("89"), 3) != 0);
        //x TEST_CHECK (str1.compare(8,8,T("89"), 5) == 0); // T('\0')終端としては扱われない模様...

        TEST_CHECK (str1.compare(str0) > 0);
        TEST_CHECK (str0.compare(str7) < 0);
        TEST_CHECK (str0.compare(T("")) == 0);
        TEST_CHECK (str0.compare(T("012345")) < 0);

     #if 0
        #define TEST_CHECK2(x)  TEST_OUT_OF_RANGE(x)
     #else
        #define TEST_CHECK2(x)  TEST_CHECK(x)
     #endif
        TEST_CHECK2(str0.compare(0,5,str5) < 0);
        TEST_CHECK2(str5.compare(3,5,str1) > 0);
        TEST_CHECK2(str1.compare(0,5,str5,3,5) < 0);
        TEST_CHECK2(str5.compare(0,5,str1,0,5) > 0);
        TEST_CHECK2(str1.compare(9,6,str5,1,6) > 0);
        TEST_CHECK2(str1.compare(1,5,T("3456789"),5) < 0);
        TEST_CHECK2(str1.compare(1,5,T("3456789"),5,5) < 0);
        #undef TEST_CHECK2
    }

 #if 1
    /// [], at, begin, end, rbegin, rend, を使用
    void test_etc()
    {
        using namespace std;
        STRING_VIEW s0 = T("0123456789abcdefghijklmnopqrstuvwxyz");

        int k = 0;
        for (size_t i = 0; i < s0.size(); ++i)
            k += s0[i];
        for (size_t i = 0; i < s0.size(); ++i)
            k += s0.at(i);
        TEST_OUT_OF_RANGE( k = s0.at(s0.size()) );

        char_t buf[1030] = {0};
        copy(s0.begin(), s0.end(), buf);
        TEST_CHECK( memcmp(s0.data() , buf, 36) == 0 );
        TEST_CHECK(s0 == buf); //TEST_EQ(s0, buf);
        memset(buf, 0, sizeof buf);

        copy(s0.cbegin(), s0.cend(), buf);
        TEST_CHECK( memcmp(&s0[0], buf, 36) == 0 );
        TEST_CHECK(s0 == buf); // TEST_EQ(s0, buf);
        memset(buf, 0, sizeof buf);

        copy(s0.rbegin(), s0.rend(), buf);
        TEST_CHECK( memcmp(buf, T("zyxwvutsrqponmlkjihgfedcba9876543210"), 36) == 0 );
        memset(buf, 0, sizeof buf);

     #if __cplusplus > 202000 || defined CCWRAP_STRING_VIEW_HPP
        STRING_VIEW s1 = s0;
        s1.remove_prefix(10);
        TEST_EQ(s1, T("abcdefghijklmnopqrstuvwxyz"));
        s1.remove_prefix(25);
        TEST_EQ(s1, T("z"));
        s1.remove_prefix(2);
        TEST_EQ(s1, T(""));
        s1.remove_prefix(5);
        TEST_EQ(s1, T(""));

        s1 = s0;
        s1.remove_surfix(26);
        TEST_EQ(s1, T("0123456789"));
        s1.remove_surfix(9);
        TEST_EQ(s1, T("0"));
        s1.remove_surfix(2);
        TEST_EQ(s1, T(""));
        s1.remove_surfix(5);
        TEST_EQ(s1, T(""));
     #endif
    }
 #endif

 #if 0
    /// [], at, begin, end, rbegin, rend, swap を使用
    void test_etc2()
    {
        // デフォルト・コンストラクタ
        STRING_VIEW str0;
        TEST_CHECK (str0.empty());
        TEST_CHECK (str0.size() == 0);
        TEST_CHECK (str0.capacity() >= 0);
        TEST_CHECK (str0.max_size() >= str0.size());

        //      リザーブ
        str0.reserve(100);
        TEST_CHECK (str0.empty());
        TEST_CHECK (str0.length() == 0);
        TEST_CHECK (str0.capacity() >= 100);
        TEST_CHECK (str0.max_size() >= str0.size());

        // リサイズ
        str0.resize(10, T(' '));
        TEST_CHECK (str0.empty() == false);
        TEST_CHECK (str0.size() == 10);
        TEST_CHECK (str0.capacity() >= 100);
        TEST_CHECK ( std::memcmp(&str0[0], T("          "), 10) == 0 );
        TEST_CHECK ( std::memcmp(str0.data() , T("          "), 10) == 0 );
        TEST_CHECK ( std::memcmp(str0.c_str(), T("          "), 11) == 0 );
        TEST_CHECK (str0.max_size() >= str0.size());

        //      at
      #ifndef UNUSE_THROW
        try {
            for (int i = 0; i < 11; ++i)
                str0.at(i) = char(T('0')+i);
        } catch (...) {
            ;
        }
        TEST_CHECK ( std::memcmp(str0.data() , T("0123456789"), 10) == 0 );
        TEST_CHECK ( std::memcmp(str0.c_str(), T("0123456789"), 11) == 0 );
      #endif

        const STRING_VIEW str1(T("0123456789"));
        //STRING_VIEW   str1(10);
        TEST_CHECK (str1.empty() == false);
        TEST_CHECK (str1.size() == 10);
        TEST_CHECK (str1.length() == 10);
        TEST_CHECK (str1.capacity() >= 10);
        TEST_CHECK (str1.max_size() >= str1.size());

        STRING_VIEW str1b = str1;
        TEST_CHECK ( std::memcmp(str1b.data() , T("0123456789"), 10) == 0 );
        TEST_CHECK ( std::memcmp(str1b.c_str(), T("0123456789"), 11) == 0 );

      #ifndef UNUSE_THROW
        //      []  const at()
        try {
            for (int i = 0; i < 11; ++i) {
                char c = (str1.at(9-i));
                str0[i] = c;
            }
        } catch (...) {
            ;
        }
        TEST_CHECK ( std::memcmp(str0.data() , T("9876543210"), 10) == 0 );
        TEST_CHECK ( std::memcmp(str0.c_str(), T("9876543210"), 11) == 0 );
      #endif

        //      const []
        for (int i = 0; i < 10; ++i)
            str0[i] = str1[i];
        TEST_CHECK ( std::memcmp(str0.data() , T("0123456789"), 10) == 0 );
        TEST_CHECK ( std::memcmp(str0.c_str(), T("0123456789"), 11) == 0 );

        // begin,end
        STRING_VIEW str2(str0.begin(), str0.end());
        TEST_CHECK ( str2 == T("0123456789") );

      #ifndef YA_STRING_NO_MEMBER_TEMPLATES // メンバーテンプレートサポートの場合.
        // rbegin,rend
        STRING_VIEW str3(str0.rbegin(), str0.rend());
        TEST_CHECK ( str3 == T("9876543210") );
      #endif

        // const begin, const end
        STRING_VIEW str4(str1.begin(), str1.end());
        TEST_CHECK ( str4 == T("0123456789") );

      #ifndef YA_STRING_NO_MEMBER_TEMPLATES // メンバーテンプレートサポートの場合.
        // const rbegin, const rend
        STRING_VIEW str5(str1.rbegin(), str1.rend());
        TEST_CHECK ( str5 == T("9876543210") );
      #else
        STRING_VIEW str5("9876543210");
      #endif

        str4.swap(str5);
        TEST_CHECK (str4 == T("9876543210"));
        TEST_CHECK (str5 == T("0123456789"));

        std::swap( str4, str5 );
        TEST_CHECK (str4 == T("0123456789"));
        TEST_CHECK (str5 == T("9876543210"));
    }
 #endif

    /// copy
    void test_copy()
    {
        const STRING_VIEW   str0(T("0123456789"));
        char_t buf[100];
        str0.copy(buf, 4);          TEST_CHECK(std::memcmp(buf, T("0123"), 4) == 0);
        str0.copy(buf, 10);         TEST_CHECK(std::memcmp(buf, T("0123456789"), 10) == 0);
        str0.copy(buf, 32);         TEST_CHECK(std::memcmp(buf, T("0123456789"), 10) == 0);
        str0.copy(buf,  4, 1);      TEST_CHECK(std::memcmp(buf, T("1234"), 4) == 0);
        str0.copy(buf,  9, 1);      TEST_CHECK(std::memcmp(buf, T("123456789"), 9) == 0);
        str0.copy(buf, 32, 1);      TEST_CHECK(std::memcmp(buf, T("123456789"), 9) == 0);

        TEST_OUT_OF_RANGE( str0.copy(buf, 4, 100) );
        TEST_CHECK(std::memcmp(buf, T("123456789"), 9) == 0);

        TEST_NO_THROW( str0.copy(buf, 0, 0) );
        TEST_NO_THROW( str0.copy(NULL, 0, 10) );
        TEST_OUT_OF_RANGE( str0.copy(NULL, 4, 100) );
        TEST_OUT_OF_RANGE( str0.copy(NULL, 33, 32) );
    }

    // swap
    void test_swap()
    {
        STRING_VIEW str2 = T("abcdefghijklemopqrstuvwxyz");
        STRING_VIEW str3 = T("0123456789ABCDEF");
        str2.swap(str3);
        TEST_CHECK (str2 == T("0123456789ABCDEF"));
        TEST_CHECK (str3 == T("abcdefghijklemopqrstuvwxyz"));

        using std::swap;
        swap( str2, str3 );

        TEST_CHECK (str2 == T("abcdefghijklemopqrstuvwxyz"));
        TEST_CHECK (str3 == T("0123456789ABCDEF"));
    }

    void test_substr()
    {
        const STRING_VIEW str0(T("abcdefghijklmnopqrstuvwxyz"));
        TEST_EQ(str0.size(), 26);
        STRING_VIEW str1;
        str1 = str0.substr();           TEST_CHECK (str1 == T("abcdefghijklmnopqrstuvwxyz"));
        str1 = str0.substr(0, 5);       TEST_CHECK (str1 == T("abcde"));
        str1 = str0.substr(1,5);        TEST_CHECK (str1 == T("bcdef"));
        str1 = str0.substr(5);          TEST_CHECK (str1 == T("fghijklmnopqrstuvwxyz"));

        str1 = str0.substr(25);         TEST_CHECK (str1 == T("z"));

        str1 = str0.substr(25, 10);     TEST_CHECK (str1 == T("z"));

        str1 = str0.substr(26);         TEST_CHECK (str1 == T(""));


        TEST_OUT_OF_RANGE( STRING_VIEW  str2 = str1.substr(27) );
        TEST_OUT_OF_RANGE( STRING_VIEW  str3 = str1.substr(27, 10) );
    }

    void test_find()
    {
        std::size_t     n;

        const STRING_VIEW   text0;
        n = text0.find(T('0'));             TEST_EQ( n, text0.npos );
        n = text0.find(T('\0'));            TEST_EQ( n, text0.npos );
        n = text0.find(T("0"));             TEST_EQ( n, text0.npos );
        n = text0.find(T('0'), 100);        TEST_EQ( n, text0.npos );
        n = text0.find(T('\0'),100);        TEST_EQ( n, text0.npos );
        n = text0.find(T("0"), 100);        TEST_EQ( n, text0.npos );

      #if 1
        n = text0.find(T(""));              TEST_EQ( n, 0 );
        n = text0.find(T("") , 100);        TEST_EQ( n, text0.npos );
      #endif

        const STRING_VIEW text =  T("0123456789"
                                   "abcdefghij"
                                   "klmnopqrst"
                                   "uvwxyz####");

        // 正順
        n = text.find(T('0'));              TEST_EQ( n, 0 );
        n = text.find(T('7'));              TEST_EQ( n, 7 );
        n = text.find(T("0"));              TEST_EQ( n, 0 );
        n = text.find(T("89"));             TEST_EQ( n, 8 );
        n = text.find(T("klmno"));          TEST_EQ( n, 20 );
        n = text.find(T("end"));            TEST_EQ( n, text.npos );
        n = text.find(T('\0'));             TEST_EQ( n, text.npos );
        n = text.find(T(""));               TEST_EQ( n, 0 );
        //n = text.find((char*)0);          TEST_EQ( n, 0 );

        n = text.find(T('0'), 1);           TEST_EQ( n, text.npos );
        n = text.find(T('7'), 1);           TEST_EQ( n, 7 );
        n = text.find(T("0"), 1);           TEST_EQ( n, text.npos );
        n = text.find(T("67"), 8);          TEST_EQ( n, text.npos );
        n = text.find(T("78"), 8);          TEST_EQ( n, text.npos );
        n = text.find(T("89"), 8);          TEST_EQ( n, 8 );
        n = text.find(T("klmno"), 20);      TEST_EQ( n, 20 );
        n = text.find(T("end"), 1);         TEST_EQ( n, text.npos );
        n = text.find(T('\0'), 8);          TEST_EQ( n, text.npos );
        n = text.find(T(""), 8);            TEST_EQ( n, 8 );

        n = text.find(STRING_VIEW(T("0")));             TEST_EQ( n, 0 );
        n = text.find(STRING_VIEW(T("89")));            TEST_EQ( n, 8 );
        n = text.find(STRING_VIEW(T("klmno")));         TEST_EQ( n, 20 );
        n = text.find(STRING_VIEW(T("end")));           TEST_EQ( n, text.npos );
        n = text.find(STRING_VIEW(T("0")), 1);          TEST_EQ( n, text.npos );
        n = text.find(STRING_VIEW(T("67")), 8);         TEST_EQ( n, text.npos );
        n = text.find(STRING_VIEW(T("78")), 8);         TEST_EQ( n, text.npos );
        n = text.find(STRING_VIEW(T("89")), 8);         TEST_EQ( n, 8 );
        n = text.find(STRING_VIEW(T("klmno")), 20);     TEST_EQ( n, 20 );
        n = text.find(STRING_VIEW(T("klmno")), 21);     TEST_EQ( n, text.npos );
        n = text.find(STRING_VIEW(T("end")), 1);        TEST_EQ( n, text.npos );
        n = text.find(STRING_VIEW(T("")), 8);           TEST_EQ( n, 8 );

        // -----
        const STRING_VIEW text2 =  T("0123456789"
                                   "abcdefghij"
                                   "klmnopqrst"
                                   "uvwxyz####"
                                   "0123456789"
                                   "abcdefghij"
                                   "klmnopqrst"
                                   "uvwxyz####");

        n = text2.find(T('0'));             TEST_EQ( n, 0 );
        n = text2.find(T('7'));             TEST_EQ( n, 7 );
        n = text2.find(T("0"));             TEST_EQ( n, 0 );
        n = text2.find(T("89"));            TEST_EQ( n, 8 );
        n = text2.find(T("end"));           TEST_EQ( n, text2.npos );

        n = text2.find(T('0'), 1);          TEST_EQ( n, 40 );
        n = text2.find(T('7'), 1);          TEST_EQ( n, 7 );
        n = text2.find(T("0"), 1);          TEST_EQ( n, 40 );
        n = text2.find(T("78"), 8);         TEST_EQ( n, 47 );
        n = text2.find(T("89"), 8);         TEST_EQ( n, 8 );
        n = text2.find(T("end"), 1);        TEST_EQ( n, text2.npos );

        n = text2.find(STRING_VIEW(T("0")));        TEST_EQ( n, 0 );
        n = text2.find(STRING_VIEW(T("89")));       TEST_EQ( n, 8 );
        n = text2.find(STRING_VIEW(T("end")));      TEST_EQ( n, text2.npos );
        n = text2.find(STRING_VIEW(T("0")), 1);     TEST_EQ( n, 40 );
        n = text2.find(STRING_VIEW(T("78")), 8);    TEST_EQ( n, 47 );
        n = text2.find(STRING_VIEW(T("89")), 8);    TEST_EQ( n, 8 );
        n = text2.find(STRING_VIEW(T("end")), 1);   TEST_EQ( n, text2.npos );
    }


    void test_rfind()
    {
        STRING_VIEW text;
        std::size_t n;

        n = text.rfind(T('0'));             TEST_EQ( n, text.npos );
        n = text.rfind(T('\0'));            TEST_EQ( n, text.npos );
        n = text.rfind(T("0"));             TEST_EQ( n, text.npos );
        n = text.rfind(T('0'), 100);        TEST_EQ( n, text.npos );
        n = text.rfind(T('\0'),100);        TEST_EQ( n, text.npos );
        n = text.rfind(T("0"), 100);        TEST_EQ( n, text.npos );

      #if 1
        n = text.rfind(T(""));              TEST_EQ( n, 0 );
        n = text.rfind(T("") , 100);        TEST_EQ( n, 0 );
      #endif

        text =  T("0123456789")
                T("abcdefghij")
                T("klmnopqrst")
                T("uvwxyz####");

        n = text.rfind(T('0'));             TEST_EQ( n, 0 );
        n = text.rfind(T('7'));             TEST_EQ( n, 7 );
        n = text.rfind(T("0"));             TEST_EQ( n, 0 );
        n = text.rfind(T("89"));            TEST_EQ( n, 8 );
        n = text.rfind(T("klmno"));         TEST_EQ( n, 20 );
        n = text.rfind(T("end"));           TEST_EQ( n, text.npos );

        n = text.rfind(T('0'), 6);          TEST_EQ( n, 0 );
        n = text.rfind(T('7'), 6);          TEST_EQ( n, text.npos );
        n = text.rfind(T("0"), 8);          TEST_EQ( n, 0 );
        n = text.rfind(T("67"), 7 );        TEST_EQ( n, 6 );
        n = text.rfind(T("78"), 7 );        TEST_EQ( n, 7 );
        n = text.rfind(T("89"), 7 );        TEST_EQ( n, text.npos );
        n = text.rfind(T("klmno"), 30 );    TEST_EQ( n, 20 );
        n = text.rfind(T("end"), 38 );      TEST_EQ( n, text.npos );

        n = text.rfind(STRING_VIEW(T("0")));            TEST_EQ( n, 0 );
        n = text.rfind(STRING_VIEW(T("89")));           TEST_EQ( n, 8 );
        n = text.rfind(STRING_VIEW(T("klmno")));        TEST_EQ( n, 20 );
        n = text.rfind(STRING_VIEW(T("end")));          TEST_EQ( n, text.npos );
        n = text.rfind(STRING_VIEW(T("0")), 8);         TEST_EQ( n, 0 );
        n = text.rfind(STRING_VIEW(T("67")), 7 );       TEST_EQ( n, 6 );
        n = text.rfind(STRING_VIEW(T("78")), 7 );       TEST_EQ( n, 7 );
        n = text.rfind(STRING_VIEW(T("89")), 7 );       TEST_EQ( n, text.npos );
        n = text.rfind(STRING_VIEW(T("klmno")), 30 );   TEST_EQ( n, 20 );
        n = text.rfind(STRING_VIEW(T("end")), 38 );     TEST_EQ( n, text.npos );

        // -----
        text =  T( "0123456789"
                   "abcdefghij"
                   "klmnopqrst"
                   "uvwxyz####"
                   "0123456789"
                   "abcdefghij"
                   "klmnopqrst"
                   "uvwxyz####");

        n = text.rfind(T('0'));             TEST_EQ( n, 40 );
        n = text.rfind(T('7'));             TEST_EQ( n, 47 );
        n = text.rfind(T("0"));             TEST_EQ( n, 40 );
        n = text.rfind(T("89"));            TEST_EQ( n, 48 );
        n = text.rfind(T("klmno"));         TEST_EQ( n, 60 );
        n = text.rfind(T("end"));           TEST_EQ( n, text.npos );

        n = text.rfind(T('0'), 46);         TEST_EQ( n, 40 );
        n = text.rfind(T('7'), 46);         TEST_EQ( n, 7 );
        n = text.rfind(T("0"), 48);         TEST_EQ( n, 40 );
        n = text.rfind(T("67"), 47 );       TEST_EQ( n, 46 );
        n = text.rfind(T("78"), 47 );       TEST_EQ( n, 47 );
        n = text.rfind(T("89"), 47 );       TEST_EQ( n, 8  );
        n = text.rfind(T("klmno"), 30 );    TEST_EQ( n, 20 );
        n = text.rfind(T("end"), 79 );      TEST_EQ( n, text.npos );

        n = text.rfind(STRING_VIEW(T("0")));            TEST_EQ( n, 40 );
        n = text.rfind(STRING_VIEW(T("89")));           TEST_EQ( n, 48 );
        n = text.rfind(STRING_VIEW(T("klmno")));        TEST_EQ( n, 60 );
        n = text.rfind(STRING_VIEW(T("end")));          TEST_EQ( n, text.npos );
        n = text.rfind(STRING_VIEW(T("0")), 48);        TEST_EQ( n, 40 );
        n = text.rfind(STRING_VIEW(T("67")), 47 );      TEST_EQ( n, 46 );
        n = text.rfind(STRING_VIEW(T("78")), 47 );      TEST_EQ( n, 47 );
        n = text.rfind(STRING_VIEW(T("89")), 47 );      TEST_EQ( n, 8  );
        n = text.rfind(STRING_VIEW(T("klmno")), 30);    TEST_EQ( n, 20 );
        n = text.rfind(STRING_VIEW(T("end")), 79 );     TEST_EQ( n, text.npos );
    }

    void test_find_first_of()
    {
        STRING_VIEW text =  T("0123456789")
                            T("abcdefghij")
                            T("klmnopqrst")
                            T("uvwxyz###.");
        std::size_t n;

        n = text.find_first_of(T('0'));             TEST_EQ( n, 0 );
        n = text.find_first_of(T('a'));             TEST_EQ( n, 10 );
        n = text.find_first_of(T('.'));             TEST_EQ( n, 39 );
        n = text.find_first_of(T('\0'));            TEST_EQ( n, text.npos );
        n = text.find_first_of(T('?'));             TEST_EQ( n, text.npos );

        n = text.find_first_of(T('0'),10);          TEST_EQ( n, text.npos );
        n = text.find_first_of(T('a'),10);          TEST_EQ( n, 10 );
        n = text.find_first_of(T('.'),10);          TEST_EQ( n, 39 );
        n = text.find_first_of(T('\0'),10);         TEST_EQ( n, text.npos );
        n = text.find_first_of(T('?'),10);          TEST_EQ( n, text.npos );
        n = text.find_first_of(T('0'),text.npos);   TEST_EQ( n, text.npos );

        n = text.find_first_of(T('0'),100);         TEST_EQ( n, text.npos );
        n = text.find_first_of(T('a'),100);         TEST_EQ( n, text.npos );
        n = text.find_first_of(T('.'),100);         TEST_EQ( n, text.npos );
        n = text.find_first_of(T('\0'),100);        TEST_EQ( n, text.npos );
        n = text.find_first_of(T('?'),100);         TEST_EQ( n, text.npos );

        n = text.find_first_of(T("0"));             TEST_EQ( n, 0 );
        n = text.find_first_of(T("a"));             TEST_EQ( n, 10 );
        n = text.find_first_of(T("."));             TEST_EQ( n, 39 );
        n = text.find_first_of(T("?"));             TEST_EQ( n, text.npos );

        n = text.find_first_of(T("0"),10);          TEST_EQ( n, text.npos );
        n = text.find_first_of(T("a"),10);          TEST_EQ( n, 10 );
        n = text.find_first_of(T("."),10);          TEST_EQ( n, 39 );
        n = text.find_first_of(T("?"),10);          TEST_EQ( n, text.npos );
        n = text.find_first_of(T("0"),text.npos);   TEST_EQ( n, text.npos );

        n = text.find_first_of(T("0"),100);         TEST_EQ( n, text.npos );
        n = text.find_first_of(T("a"),100);         TEST_EQ( n, text.npos );
        n = text.find_first_of(T("."),100);         TEST_EQ( n, text.npos );
        n = text.find_first_of(T("?"),100);         TEST_EQ( n, text.npos );

        n = text.find_first_of(T("0123456789"));    TEST_EQ( n, 0 );
        n = text.find_first_of(T("badcfe"));        TEST_EQ( n, 10 );
        n = text.find_first_of(T(".#"));            TEST_EQ( n, 36 );

        n = text.find_first_of(T("0123456789"),5);  TEST_EQ( n, 5 );
        n = text.find_first_of(T("badcfe"), 5);     TEST_EQ( n, 10 );
        n = text.find_first_of(T("0123456789"),10); TEST_EQ( n, text.npos );
        n = text.find_first_of(T("badcfe"), 10);    TEST_EQ( n, 10 );
        n = text.find_first_of(T(".#"), 10);        TEST_EQ( n, 36 );
        n = text.find_first_of(T("a0"),text.npos);  TEST_EQ( n, text.npos );


        n = text.find_first_of(STRING_VIEW(T("0")));            TEST_EQ( n, 0 );
        n = text.find_first_of(STRING_VIEW(T("a")));            TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T(".")));            TEST_EQ( n, 39 );
        n = text.find_first_of(STRING_VIEW(T("?")));            TEST_EQ( n, text.npos );

        n = text.find_first_of(STRING_VIEW(T("0")),10);         TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("a")),10);         TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T(".")),10);         TEST_EQ( n, 39 );
        n = text.find_first_of(STRING_VIEW(T("?")),10);         TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("0")),text.npos);  TEST_EQ( n, text.npos );

        n = text.find_first_of(STRING_VIEW(T("0123456789")));   TEST_EQ( n, 0 );
        n = text.find_first_of(STRING_VIEW(T("badcfe")));       TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T(".#")));           TEST_EQ( n, 36 );

        n = text.find_first_of(STRING_VIEW(T("0123456789")),5); TEST_EQ( n, 5 );
        n = text.find_first_of(STRING_VIEW(T("badcfe")), 5);    TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T("0123456789")),10);TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("badcfe")), 10);   TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T(".#")), 10);       TEST_EQ( n, 36 );
        n = text.find_first_of(STRING_VIEW(T("a0")),text.npos); TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("")),text.npos);   TEST_EQ( n, text.npos );


        n = text.find_first_of(T("\0"));                        TEST_EQ( n, text.npos );
        n = text.find_first_of(T("\0"),10);                     TEST_EQ( n, text.npos );
        n = text.find_first_of(T("\0"),100);                    TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("")));             TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("")), 10);         TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("\0")));           TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("\0")),10);        TEST_EQ( n, text.npos );

        text =  T("0123456789")
                T("abcdefghij")
                T("klmnopqrst")
                T("uvwxyz###.")
                T("0123456789")
                T("abcdefghij")
                T("klmnopqrst")
                T("uvwxyz###.");

        n = text.find_first_of(T('0'));             TEST_EQ( n, 0 );
        n = text.find_first_of(T('a'));             TEST_EQ( n, 10 );
        n = text.find_first_of(T('.'));             TEST_EQ( n, 39 );
        n = text.find_first_of(T('\0'));            TEST_EQ( n, text.npos );
        n = text.find_first_of(T('?'));             TEST_EQ( n, text.npos );

        n = text.find_first_of(T('0'),10);          TEST_EQ( n, 40 );
        n = text.find_first_of(T('a'),10);          TEST_EQ( n, 10 );
        n = text.find_first_of(T('.'),10);          TEST_EQ( n, 39 );
        n = text.find_first_of(T('\0'),10);         TEST_EQ( n, text.npos );
        n = text.find_first_of(T('?'),10);          TEST_EQ( n, text.npos );
        n = text.find_first_of(T('0'),text.npos);   TEST_EQ( n, text.npos );

        n = text.find_first_of(T("0"));             TEST_EQ( n, 0 );
        n = text.find_first_of(T("a"));             TEST_EQ( n, 10 );
        n = text.find_first_of(T("."));             TEST_EQ( n, 39 );
        n = text.find_first_of(T("\0"));            TEST_EQ( n, text.npos );
        n = text.find_first_of(T("?"));             TEST_EQ( n, text.npos );

        n = text.find_first_of(T("0"),10);          TEST_EQ( n, 40 );
        n = text.find_first_of(T("a"),10);          TEST_EQ( n, 10 );
        n = text.find_first_of(T("."),10);          TEST_EQ( n, 39 );
        n = text.find_first_of(T("\0"),10);         TEST_EQ( n, text.npos );
        n = text.find_first_of(T("?"),10);          TEST_EQ( n, text.npos );
        n = text.find_first_of(T("0"),text.npos);   TEST_EQ( n, text.npos );

        n = text.find_first_of(T("0123456789"));    TEST_EQ( n, 0 );
        n = text.find_first_of(T("badcfe"));        TEST_EQ( n, 10 );
        n = text.find_first_of(T(".#"));            TEST_EQ( n, 36 );

        n = text.find_first_of(T("0123456789"),5);  TEST_EQ( n, 5 );
        n = text.find_first_of(T("badcfe"), 5);     TEST_EQ( n, 10 );
        n = text.find_first_of(T("0123456789"),10); TEST_EQ( n, 40 );
        n = text.find_first_of(T("badcfe"), 10);    TEST_EQ( n, 10 );
        n = text.find_first_of(T(".#"), 10);        TEST_EQ( n, 36 );
        n = text.find_first_of(T("a0"),text.npos);  TEST_EQ( n, text.npos );

        n = text.find_first_of(STRING_VIEW(T("0")));            TEST_EQ( n, 0 );
        n = text.find_first_of(STRING_VIEW(T("a")));            TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T(".")));            TEST_EQ( n, 39 );
        n = text.find_first_of(STRING_VIEW(T("\0")));           TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("?")));            TEST_EQ( n, text.npos );

        n = text.find_first_of(STRING_VIEW(T("0")),10);         TEST_EQ( n, 40 );
        n = text.find_first_of(STRING_VIEW(T("a")),10);         TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T(".")),10);         TEST_EQ( n, 39 );
        n = text.find_first_of(STRING_VIEW(T("\0")),10);        TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("?")),10);         TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("0")),text.npos);  TEST_EQ( n, text.npos );

        n = text.find_first_of(STRING_VIEW(T("0123456789")));   TEST_EQ( n, 0 );
        n = text.find_first_of(STRING_VIEW(T("badcfe")));       TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T(".#")));           TEST_EQ( n, 36 );

        n = text.find_first_of(STRING_VIEW(T("0123456789")),5); TEST_EQ( n, 5 );
        n = text.find_first_of(STRING_VIEW(T("badcfe")), 5);    TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T("0123456789")),10);TEST_EQ( n, 40 );
        n = text.find_first_of(STRING_VIEW(T("badcfe")), 10);   TEST_EQ( n, 10 );
        n = text.find_first_of(STRING_VIEW(T(".#")), 10);       TEST_EQ( n, 36 );
        n = text.find_first_of(STRING_VIEW(T("a0")),text.npos); TEST_EQ( n, text.npos );
    }

    void test_find_last_of()
    {
        STRING_VIEW text =  T("0123456789")
                            T("abcdefghij")
                            T("klmnopqrst")
                            T("uvwxyz###.");
        std::size_t n;

        n = text.find_last_of(T('0'));              TEST_EQ( n, 0 );
        n = text.find_last_of(T('a'));              TEST_EQ( n, 10 );
        n = text.find_last_of(T('.'));              TEST_EQ( n, 39 );
        n = text.find_last_of(T('\0'));             TEST_EQ( n, text.npos );
        n = text.find_last_of(T('?'));              TEST_EQ( n, text.npos );

        n = text.find_last_of(T('0'),10);           TEST_EQ( n, 0 );
        n = text.find_last_of(T('a'),10);           TEST_EQ( n, 10 );
        n = text.find_last_of(T('.'),10);           TEST_EQ( n, text.npos );
        n = text.find_last_of(T('\0'),10);          TEST_EQ( n, text.npos );
        n = text.find_last_of(T('?'),10);           TEST_EQ( n, text.npos );
        n = text.find_first_of(T('0'),0);           TEST_EQ( n, 0 );

        n = text.find_last_of(T("0"));              TEST_EQ( n, 0 );
        n = text.find_last_of(T("a"));              TEST_EQ( n, 10);
        n = text.find_last_of(T("."));              TEST_EQ( n, 39);
        n = text.find_last_of(T("\0"));             TEST_EQ( n, text.npos );
        n = text.find_last_of(T("?"));              TEST_EQ( n, text.npos );

        n = text.find_last_of(T("0"),10);           TEST_EQ( n, 0 );
        n = text.find_last_of(T("a"),10);           TEST_EQ( n, 10 );
        n = text.find_last_of(T("."),10);           TEST_EQ( n, text.npos );
        n = text.find_last_of(T("\0"),10);          TEST_EQ( n, text.npos );
        n = text.find_last_of(T("?"),10);           TEST_EQ( n, text.npos );
        n = text.find_first_of(T("0"),0);           TEST_EQ( n, 0 );

        n = text.find_last_of(T("0123456789"));     TEST_EQ( n, 9  );
        n = text.find_last_of(T("badcfe"));         TEST_EQ( n, 15 );
        n = text.find_last_of(T(".#"));             TEST_EQ( n, 39 );

        n = text.find_last_of(T("0123456789"),5);   TEST_EQ( n, 5 );
        n = text.find_last_of(T("badcfe"), 5);      TEST_EQ( n, text.npos);
        n = text.find_last_of(T("0123456789"), 9);  TEST_EQ( n, 9 );
        n = text.find_last_of(T("badcfe"), 9);      TEST_EQ( n, text.npos );
        n = text.find_last_of(T(".#"), 38 );        TEST_EQ( n, 38 );
        n = text.find_first_of(T("a0"),0);          TEST_EQ( n, 0 );

        n = text.find_last_of(STRING_VIEW(T("0")));             TEST_EQ( n, 0 );
        n = text.find_last_of(STRING_VIEW(T("a")));             TEST_EQ( n, 10);
        n = text.find_last_of(STRING_VIEW(T(".")));             TEST_EQ( n, 39);
        n = text.find_last_of(STRING_VIEW(T("\0")));            TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T("?")));             TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T("")));              TEST_EQ( n, text.npos );

        n = text.find_last_of(STRING_VIEW(T("0")),10);          TEST_EQ( n, 0 );
        n = text.find_last_of(STRING_VIEW(T("a")),10);          TEST_EQ( n, 10 );
        n = text.find_last_of(STRING_VIEW(T(".")),10);          TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T("\0")),10);         TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T("?")),10);          TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T("")),10);           TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("0")),0);          TEST_EQ( n, 0 );

        n = text.find_last_of(STRING_VIEW(T("0123456789")));        TEST_EQ( n, 9  );
        n = text.find_last_of(STRING_VIEW(T("badcfe")));            TEST_EQ( n, 15 );
        n = text.find_last_of(STRING_VIEW(T(".#")));                TEST_EQ( n, 39 );

        n = text.find_last_of(STRING_VIEW(T("0123456789")),5);  TEST_EQ( n, 5 );
        n = text.find_last_of(STRING_VIEW(T("badcfe")), 5);     TEST_EQ( n, text.npos);
        n = text.find_last_of(STRING_VIEW(T("0123456789")), 9); TEST_EQ( n, 9 );
        n = text.find_last_of(STRING_VIEW(T("badcfe")), 9);     TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T(".#")), 38 );       TEST_EQ( n, 38 );
        n = text.find_first_of(STRING_VIEW(T("a0")),0);         TEST_EQ( n, 0 );

        text =  T("0123456789")
                T("abcdefghij")
                T("klmnopqrst")
                T("uvwxyz###.")
                T("0123456789")
                T("abcdefghij")
                T("klmnopqrst")
                T("uvwxyz###.");

        n = text.find_last_of(T('0'));              TEST_EQ( n, 40 );
        n = text.find_last_of(T('a'));              TEST_EQ( n, 50 );
        n = text.find_last_of(T('.'));              TEST_EQ( n, 79 );
        n = text.find_last_of(T('\0'));             TEST_EQ( n, text.npos );
        n = text.find_last_of(T('?'));              TEST_EQ( n, text.npos );

        n = text.find_last_of(T('0'),10);           TEST_EQ( n, 0 );
        n = text.find_last_of(T('a'),10);           TEST_EQ( n, 10 );
        n = text.find_last_of(T('.'),10);           TEST_EQ( n, text.npos );
        n = text.find_last_of(T('\0'),10);          TEST_EQ( n, text.npos );
        n = text.find_last_of(T('?'),10);           TEST_EQ( n, text.npos );
        n = text.find_first_of(T('0'),0);           TEST_EQ( n, 0 );

        n = text.find_last_of(T("0"));              TEST_EQ( n, 40);
        n = text.find_last_of(T("a"));              TEST_EQ( n, 50);
        n = text.find_last_of(T("."));              TEST_EQ( n, 79);
        n = text.find_last_of(T("\0"));             TEST_EQ( n, text.npos );
        n = text.find_last_of(T("?"));              TEST_EQ( n, text.npos );

        n = text.find_last_of(T("0"),10);           TEST_EQ( n, 0 );
        n = text.find_last_of(T("a"),10);           TEST_EQ( n, 10 );
        n = text.find_last_of(T("."),10);           TEST_EQ( n, text.npos );
        n = text.find_last_of(T("\0"),10);          TEST_EQ( n, text.npos );
        n = text.find_last_of(T("?"),10);           TEST_EQ( n, text.npos );
        n = text.find_first_of(T("0"),0);           TEST_EQ( n, 0 );

        n = text.find_last_of(T("0123456789"));     TEST_EQ( n, 49 );
        n = text.find_last_of(T("badcfe"));         TEST_EQ( n, 55 );
        n = text.find_last_of(T(".#"));             TEST_EQ( n, 79 );

        n = text.find_last_of(T("0123456789"),5);   TEST_EQ( n, 5 );
        n = text.find_last_of(T("badcfe"), 5);      TEST_EQ( n, text.npos);
        n = text.find_last_of(T("0123456789"), 9);  TEST_EQ( n, 9 );
        n = text.find_last_of(T("badcfe"), 9);      TEST_EQ( n, text.npos );
        n = text.find_last_of(T(".#"), 38 );        TEST_EQ( n, 38 );
        n = text.find_first_of(T("a0"),0);          TEST_EQ( n, 0 );

        n = text.find_last_of(STRING_VIEW(T("0")));             TEST_EQ( n, 40);
        n = text.find_last_of(STRING_VIEW(T("a")));             TEST_EQ( n, 50);
        n = text.find_last_of(STRING_VIEW(T(".")));             TEST_EQ( n, 79);
        n = text.find_last_of(STRING_VIEW(T("\0")));            TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T("?")));             TEST_EQ( n, text.npos );

        n = text.find_last_of(STRING_VIEW(T("0")),10);          TEST_EQ( n, 0 );
        n = text.find_last_of(STRING_VIEW(T("a")),10);          TEST_EQ( n, 10 );
        n = text.find_last_of(STRING_VIEW(T(".")),10);          TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T("\0")),10);         TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T("?")),10);          TEST_EQ( n, text.npos );
        n = text.find_first_of(STRING_VIEW(T("0")),0);          TEST_EQ( n, 0 );

        n = text.find_last_of(STRING_VIEW(T("0123456789")));        TEST_EQ( n, 49 );
        n = text.find_last_of(STRING_VIEW(T("badcfe")));            TEST_EQ( n, 55 );
        n = text.find_last_of(STRING_VIEW(T(".#")));                TEST_EQ( n, 79 );

        n = text.find_last_of(STRING_VIEW(T("0123456789")),5);  TEST_EQ( n, 5 );
        n = text.find_last_of(STRING_VIEW(T("badcfe")), 5);     TEST_EQ( n, text.npos);
        n = text.find_last_of(STRING_VIEW(T("0123456789")), 9); TEST_EQ( n, 9 );
        n = text.find_last_of(STRING_VIEW(T("badcfe")), 9);     TEST_EQ( n, text.npos );
        n = text.find_last_of(STRING_VIEW(T(".#")), 38 );       TEST_EQ( n, 38 );
        n = text.find_first_of(STRING_VIEW(T("a0")),0);         TEST_EQ( n, 0 );
    }


    void test_find_first_not_of()
    {
        STRING_VIEW text =  T("0123456789")
                            T("abcdefghij")
                            T("klmnopqrst")
                            T("uvwxyz###.");
        std::size_t n;

        n = text.find_first_not_of(T('0'));         TEST_EQ( n, 1  );
        n = text.find_first_not_of(T('a'));         TEST_EQ( n, 0  );
        n = text.find_first_not_of(T('.'));         TEST_EQ( n, 0  );
        n = text.find_first_not_of(T('\0'));        TEST_EQ( n, 0  );
        n = text.find_first_not_of(T('?'));         TEST_EQ( n, 0  );

        n = text.find_first_not_of(T('0'),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T('a'),10);          TEST_EQ( n, 11 );
        n = text.find_first_not_of(T('.'),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T('\0'),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(T('?'),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T('0'),text.npos);   TEST_EQ( n, text.npos );

        n = text.find_first_not_of(T("0"));             TEST_EQ( n, 1  );
        n = text.find_first_not_of(T("a"));             TEST_EQ( n, 0  );
        n = text.find_first_not_of(T("."));             TEST_EQ( n, 0  );
        n = text.find_first_not_of(T("?"));             TEST_EQ( n, 0  );
        //n = text.find_first_not_of(T("\0"));          TEST_EQ( n, 0  );
        n = text.find_first_not_of(T(""));              TEST_EQ( n, 0  );

        n = text.find_first_not_of(T("0"),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("a"),10);          TEST_EQ( n, 11 );
        n = text.find_first_not_of(T("."),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("\0"),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("?"),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("0"),text.npos);   TEST_EQ( n, text.npos );
        n = text.find_first_not_of(T(""), 10);          TEST_EQ( n, 10 );

        n = text.find_first_not_of(T("0123456789"));    TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("badcfe"));        TEST_EQ( n, 0  );
        n = text.find_first_not_of(T(".#"));            TEST_EQ( n, 0  );

        n = text.find_first_not_of(T("0123456789"),5);  TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("badcfe"), 5);     TEST_EQ( n, 5  );
        n = text.find_first_not_of(T("0123456789"),10); TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("badcfe"), 10);    TEST_EQ( n, 16 );
        n = text.find_first_not_of(T(".#"), 10);        TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("a0"),text.npos);  TEST_EQ( n, text.npos );
        n = text.find_first_not_of(T(""),text.npos);    TEST_EQ( n, text.npos );

        n = text.find_first_not_of(STRING_VIEW(T("0")));            TEST_EQ( n, 1  );
        n = text.find_first_not_of(STRING_VIEW(T("a")));            TEST_EQ( n, 0  );
        n = text.find_first_not_of(STRING_VIEW(T(".")));            TEST_EQ( n, 0  );
        n = text.find_first_not_of(STRING_VIEW(T("?")));            TEST_EQ( n, 0  );
        n = text.find_first_not_of(STRING_VIEW(T("")));             TEST_EQ( n, 0  );

        n = text.find_first_not_of(STRING_VIEW(T("0")),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("a")),10);         TEST_EQ( n, 11 );
        n = text.find_first_not_of(STRING_VIEW(T(".")),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("\0")),10);        TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("?")),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("0")),text.npos);  TEST_EQ( n, text.npos );
        n = text.find_first_not_of(STRING_VIEW(T("")), 10);         TEST_EQ( n, 10 );

        n = text.find_first_not_of(STRING_VIEW(T("0123456789")));   TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("badcfe")));       TEST_EQ( n, 0  );
        n = text.find_first_not_of(STRING_VIEW(T(".#")));           TEST_EQ( n, 0  );

        n = text.find_first_not_of(STRING_VIEW(T("0123456789")),5); TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("badcfe")), 5);    TEST_EQ( n, 5  );
        n = text.find_first_not_of(STRING_VIEW(T("0123456789")),10);TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("badcfe")), 10);   TEST_EQ( n, 16 );
        n = text.find_first_not_of(STRING_VIEW(T(".#")), 10);       TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("a0")),text.npos); TEST_EQ( n, text.npos );
        n = text.find_first_not_of(STRING_VIEW(T("")),text.npos);   TEST_EQ( n, text.npos );

        text =  T( "0123456789"
                   "abcdefghij"
                   "klmnopqrst"
                   "uvwxyz####"
                   "0123456789"
                   "abcdefghij"
                   "klmnopqrst"
                   "uvwxyz####");

        n = text.find_first_not_of(T('0'));         TEST_EQ( n, 1  );
        n = text.find_first_not_of(T('a'));         TEST_EQ( n, 0  );
        n = text.find_first_not_of(T('.'));         TEST_EQ( n, 0  );
        n = text.find_first_not_of(T('\0'));        TEST_EQ( n, 0  );
        n = text.find_first_not_of(T('?'));         TEST_EQ( n, 0  );

        n = text.find_first_not_of(T('0'),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T('a'),10);          TEST_EQ( n, 11 );
        n = text.find_first_not_of(T('.'),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T('\0'),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(T('?'),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T('0'),text.npos);   TEST_EQ( n, text.npos );

        n = text.find_first_not_of(T("0"));             TEST_EQ( n, 1  );
        n = text.find_first_not_of(T("a"));             TEST_EQ( n, 0  );
        n = text.find_first_not_of(T("."));             TEST_EQ( n, 0  );
        n = text.find_first_not_of(T("?"));             TEST_EQ( n, 0  );
        //n = text.find_first_not_of(T("\0"));          TEST_EQ( n, 0  );
        n = text.find_first_not_of(T(""));              TEST_EQ( n, 0  );

        n = text.find_first_not_of(T("0"),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("a"),10);          TEST_EQ( n, 11 );
        n = text.find_first_not_of(T("."),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("\0"),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("?"),10);          TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("0"),text.npos);   TEST_EQ( n, text.npos );
        n = text.find_first_not_of(T(""), 10);          TEST_EQ( n, 10 );

        n = text.find_first_not_of(T("0123456789"));    TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("badcfe"));        TEST_EQ( n, 0  );
        n = text.find_first_not_of(T(".#"));            TEST_EQ( n, 0  );

        n = text.find_first_not_of(T("0123456789"),5);  TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("badcfe"), 5);     TEST_EQ( n, 5  );
        n = text.find_first_not_of(T("0123456789"),10); TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("badcfe"), 10);    TEST_EQ( n, 16 );
        n = text.find_first_not_of(T(".#"), 10);        TEST_EQ( n, 10 );
        n = text.find_first_not_of(T("a0"),text.npos);  TEST_EQ( n, text.npos );
        n = text.find_first_not_of(T(""),text.npos);    TEST_EQ( n, text.npos );

        n = text.find_first_not_of(STRING_VIEW(T("0")));            TEST_EQ( n, 1  );
        n = text.find_first_not_of(STRING_VIEW(T("a")));            TEST_EQ( n, 0  );
        n = text.find_first_not_of(STRING_VIEW(T(".")));            TEST_EQ( n, 0  );
        n = text.find_first_not_of(STRING_VIEW(T("?")));            TEST_EQ( n, 0  );
        n = text.find_first_not_of(STRING_VIEW(T("")));             TEST_EQ( n, 0  );

        n = text.find_first_not_of(STRING_VIEW(T("0")),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("a")),10);         TEST_EQ( n, 11 );
        n = text.find_first_not_of(STRING_VIEW(T(".")),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("\0")),10);        TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("?")),10);         TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("0")),text.npos);  TEST_EQ( n, text.npos );
        n = text.find_first_not_of(STRING_VIEW(T("")), 10);         TEST_EQ( n, 10 );

        n = text.find_first_not_of(STRING_VIEW(T("0123456789")));   TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("badcfe")));       TEST_EQ( n, 0  );
        n = text.find_first_not_of(STRING_VIEW(T(".#")));           TEST_EQ( n, 0  );

        n = text.find_first_not_of(STRING_VIEW(T("0123456789")),5); TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("badcfe")), 5);    TEST_EQ( n, 5  );
        n = text.find_first_not_of(STRING_VIEW(T("0123456789")),10);TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("badcfe")), 10);   TEST_EQ( n, 16 );
        n = text.find_first_not_of(STRING_VIEW(T(".#")), 10);       TEST_EQ( n, 10 );
        n = text.find_first_not_of(STRING_VIEW(T("a0")),text.npos); TEST_EQ( n, text.npos );
        n = text.find_first_not_of(STRING_VIEW(T("")),text.npos);   TEST_EQ( n, text.npos );
    }

    void test_find_last_not_of()
    {
        STRING_VIEW text =  T("0123456789"
                            "abcdefghij"
                            "klmnopqrst"
                            "uvwxyz###.");
        std::size_t n;

        n = text.find_last_not_of(T('0'));              TEST_EQ( n, 39 );
        n = text.find_last_not_of(T('a'));              TEST_EQ( n, 39 );
        n = text.find_last_not_of(T('.'));              TEST_EQ( n, 38 );
        n = text.find_last_not_of(T('\0'));             TEST_EQ( n, 39 );
        n = text.find_last_not_of(T('?'));              TEST_EQ( n, 39 );

        n = text.find_last_not_of(T('0'),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T('a'),10);           TEST_EQ( n, 9 );
        n = text.find_last_not_of(T('.'),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T('\0'),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(T('?'),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T('0'),text.npos);    TEST_EQ( n, 39 );
        n = text.find_last_not_of(T('0'),0);            TEST_EQ( n, text.npos );

        n = text.find_last_not_of(T("0"));              TEST_EQ( n, 39 );
        n = text.find_last_not_of(T("a"));              TEST_EQ( n, 39 );
        n = text.find_last_not_of(T("."));              TEST_EQ( n, 38 );
        n = text.find_last_not_of(T("?"));              TEST_EQ( n, 39 );
        n = text.find_last_not_of(T(""));               TEST_EQ( n, 39 );

        n = text.find_last_not_of(T("0"),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("a"),10);           TEST_EQ( n, 9 );
        n = text.find_last_not_of(T("."),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("\0"),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("?"),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("0"),text.npos);    TEST_EQ( n, 39 );

        n = text.find_last_not_of(T("0123456789"));             TEST_EQ( n, 39 );
        n = text.find_last_not_of(T("badcfe"));                 TEST_EQ( n, 39 );
        n = text.find_last_not_of(T(".#"));                     TEST_EQ( n, 35 );

        n = text.find_last_not_of(T("0123456789"),5);           TEST_EQ( n, text.npos );
        n = text.find_last_not_of(T("badcfe"), 5);              TEST_EQ( n, 5 );
        n = text.find_last_not_of(T("0123456789"),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("badcfe"), 10);             TEST_EQ( n, 9 );
        n = text.find_last_not_of(T(".#"), 10);                 TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("a0"),text.npos);           TEST_EQ( n, 39 );
        n = text.find_last_not_of(T(""),text.npos);             TEST_EQ( n, 39 );
        n = text.find_last_not_of(T(""),0);                     TEST_EQ( n, 0 );

        n = text.find_last_not_of(STRING_VIEW(T("0")));             TEST_EQ( n, 39 );
        n = text.find_last_not_of(STRING_VIEW(T("a")));             TEST_EQ( n, 39 );
        n = text.find_last_not_of(STRING_VIEW(T(".")));             TEST_EQ( n, 38 );
        n = text.find_last_not_of(STRING_VIEW(T("?")));             TEST_EQ( n, 39 );
        n = text.find_last_not_of(STRING_VIEW(T("")));              TEST_EQ( n, 39 );

        n = text.find_last_not_of(STRING_VIEW(T("0")),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("a")),10);          TEST_EQ( n, 9 );
        n = text.find_last_not_of(STRING_VIEW(T(".")),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("\0")),10);         TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("?")),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("0")),text.npos);   TEST_EQ( n, 39 );

        n = text.find_last_not_of(STRING_VIEW(T("0123456789")));        TEST_EQ( n, 39 );
        n = text.find_last_not_of(STRING_VIEW(T("badcfe")));            TEST_EQ( n, 39 );
        n = text.find_last_not_of(STRING_VIEW(T(".#")));                TEST_EQ( n, 35 );

        n = text.find_last_not_of(STRING_VIEW(T("0123456789")),5);      TEST_EQ( n, text.npos );
        n = text.find_last_not_of(STRING_VIEW(T("badcfe")), 5);         TEST_EQ( n, 5 );
        n = text.find_last_not_of(STRING_VIEW(T("0123456789")),10);     TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("badcfe")), 10);        TEST_EQ( n, 9 );
        n = text.find_last_not_of(STRING_VIEW(T(".#")), 10);            TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("a0")),text.npos);      TEST_EQ( n, 39 );
        n = text.find_last_not_of(STRING_VIEW(T("")),text.npos);        TEST_EQ( n, 39 );
        n = text.find_last_not_of(STRING_VIEW(T("")),0);                TEST_EQ( n, 0 );

        text =    T("0123456789"
                    "abcdefghij"
                    "klmnopqrst"
                    "uvwxyz###."
                    "0123456789"
                    "abcdefghij"
                    "klmnopqrst"
                    "uvwxyz###.");

        n = text.find_last_not_of(T('0'));              TEST_EQ( n, 79 );
        n = text.find_last_not_of(T('a'));              TEST_EQ( n, 79 );
        n = text.find_last_not_of(T('.'));              TEST_EQ( n, 78 );
        n = text.find_last_not_of(T('\0'));             TEST_EQ( n, 79 );
        n = text.find_last_not_of(T('?'));              TEST_EQ( n, 79 );

        n = text.find_last_not_of(T('0'),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T('a'),10);           TEST_EQ( n, 9 );
        n = text.find_last_not_of(T('.'),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T('\0'),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(T('?'),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T('0'),text.npos);    TEST_EQ( n, 79 );
        n = text.find_last_not_of(T('0'),0);            TEST_EQ( n, text.npos );

        n = text.find_last_not_of(T("0"));              TEST_EQ( n, 79 );
        n = text.find_last_not_of(T("a"));              TEST_EQ( n, 79 );
        n = text.find_last_not_of(T("."));              TEST_EQ( n, 78 );
        n = text.find_last_not_of(T("?"));              TEST_EQ( n, 79 );
        n = text.find_last_not_of(T(""));               TEST_EQ( n, 79 );

        n = text.find_last_not_of(T("0"),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("a"),10);           TEST_EQ( n, 9 );
        n = text.find_last_not_of(T("."),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("\0"),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("?"),10);           TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("0"),text.npos);    TEST_EQ( n, 79 );

        n = text.find_last_not_of(T("0123456789"));     TEST_EQ( n, 79 );
        n = text.find_last_not_of(T("badcfe"));         TEST_EQ( n, 79 );
        n = text.find_last_not_of(T(".#"));             TEST_EQ( n, 75 );

        n = text.find_last_not_of(T("0123456789"),5);   TEST_EQ( n, text.npos );
        n = text.find_last_not_of(T("badcfe"), 5);      TEST_EQ( n, 5 );
        n = text.find_last_not_of(T("0123456789"),10);  TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("badcfe"), 10);     TEST_EQ( n, 9 );
        n = text.find_last_not_of(T(".#"), 10);         TEST_EQ( n, 10 );
        n = text.find_last_not_of(T("a0"),text.npos);   TEST_EQ( n, 79 );
        n = text.find_last_not_of(T(""),text.npos);     TEST_EQ( n, 79 );
        n = text.find_last_not_of(T(""),0);             TEST_EQ( n, 0 );

        n = text.find_last_not_of(STRING_VIEW(T("0")));             TEST_EQ( n, 79 );
        n = text.find_last_not_of(STRING_VIEW(T("a")));             TEST_EQ( n, 79 );
        n = text.find_last_not_of(STRING_VIEW(T(".")));             TEST_EQ( n, 78 );
        n = text.find_last_not_of(STRING_VIEW(T("?")));             TEST_EQ( n, 79 );
        n = text.find_last_not_of(STRING_VIEW(T("")));              TEST_EQ( n, 79 );

        n = text.find_last_not_of(STRING_VIEW(T("0")),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("a")),10);          TEST_EQ( n, 9 );
        n = text.find_last_not_of(STRING_VIEW(T(".")),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("\0")),10);         TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("?")),10);          TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("0")),text.npos);   TEST_EQ( n, 79 );

        n = text.find_last_not_of(STRING_VIEW(T("0123456789")));        TEST_EQ( n, 79 );
        n = text.find_last_not_of(STRING_VIEW(T("badcfe")));            TEST_EQ( n, 79 );
        n = text.find_last_not_of(STRING_VIEW(T(".#")));                TEST_EQ( n, 75 );

        n = text.find_last_not_of(STRING_VIEW(T("0123456789")),5);      TEST_EQ( n, text.npos );
        n = text.find_last_not_of(STRING_VIEW(T("badcfe")), 5);         TEST_EQ( n, 5 );
        n = text.find_last_not_of(STRING_VIEW(T("0123456789")),10);     TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("badcfe")), 10);        TEST_EQ( n, 9 );
        n = text.find_last_not_of(STRING_VIEW(T(".#")), 10);            TEST_EQ( n, 10 );
        n = text.find_last_not_of(STRING_VIEW(T("a0")),text.npos);      TEST_EQ( n, 79 );
        n = text.find_last_not_of(STRING_VIEW(T("")),text.npos);        TEST_EQ( n, 79 );
        n = text.find_last_not_of(STRING_VIEW(T("")),0);                TEST_EQ( n, 0 );
    }
};

//int main()
int string_view_test()
{
  #ifdef UNICODE
    StringView_Test<std::wstring_view>  wstring_test;
    wstring_test.test();
  #else
    StringView_Test<std::string_view>   string_test;
    string_test.test();
  #endif
    return 0;
}
