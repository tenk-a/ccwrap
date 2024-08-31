#include <../ccwrap/ccwrap_misc/test.hpp>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <iterator>
#include <cassert>


TEST_SUITE(iterator_sub) {

	namespace {
	 #if __CCWRAP__ == 2
	    using namespace ccwrap;
	 #else
	    using namespace std;
	 #endif
	    typedef std::vector<int>        VecInt;
	    typedef std::string             String;
	    typedef std::list<float>        ListFlt;
	    typedef std::map<String,int>    MapStrInt;

	    template<class C>
	    struct Chk {
	        C   conta;
	        Chk() {
	            test_true(begin(conta) == end(conta));
	            test_true(rbegin(conta) == rend(conta));
	          #if __CCWRAP_CXX >= 201402L
	            test_true(cbegin(conta) == cend(conta));
	            test_true(crbegin(conta) == crend(conta));
	          #endif
	            test_true(empty(conta));
	            test_true(size(conta) == 0);
	            test_true(ssize(conta) == 0);
	        }
	        Chk(Chk const&) { assert(0); }
	        Chk& operator=(Chk const&) { assert(0); return *this; }
	    };
	    template<class C>
	    struct Chk2 {
	        C   conta;
	        typedef typename C::iterator        iterator;
	        typedef typename C::const_iterator  const_iterator;
	        typedef typename C::const_pointer   const_pointer;
	        Chk2(const_pointer t, size_t n) : conta(t, t+n) {
	            test_true(begin(conta) != end(conta));
	            test_true(rbegin(conta) != rend(conta));
	          #if 1 //__CCWRAP_CXX >= 201402L	// #if !defined(__CCWRAP_NO_HEADER_CXX14_ITERATOR)
	            test_true(cbegin(conta) != cend(conta));
	            test_true(crbegin(conta) != crend(conta));
	          #endif
	            test_true(!empty(conta));
	            test_true(size(conta) > 0);
	            test_true(ssize(conta) > 0);
	            test_true(data(conta) == &conta[0]);
	            test_true(size(conta) == conta.size());
	            test_true(ssize(conta) == ptrdiff_t(conta.size()));
	         #if 1 //__CCWRAP_CXX >= 201402L	// #if !defined(__CCWRAP_NO_HEADER_CXX14_ITERATOR)
	            const_iterator  ite = cbegin(conta);
	            ite = next(ite);
	            ite = next(ite, n-1);
	            test_true(ite == cend(conta));
	            ite = prev(ite);
	            ite = prev(ite, n-1);
	            test_true(ite == cbegin(conta));
	         #endif
	        }
	        Chk2(Chk2 const&) { assert(0); }
	        Chk2& operator=(Chk2 const&) { assert(0); return *this; }
	    };

	    template<class A, unsigned N>
	    struct AryChk {
	        A   conta[N];
	        AryChk() {
	            test_true(begin(conta) != end(conta));
	            test_true(rbegin(conta) != rend(conta));
	         #if __CCWRAP_CXX >= 201402L	// #if !defined(__CCWRAP_NO_HEADER_CXX14_ITERATOR)
	            test_true(cbegin(conta) != cend(conta));
	            test_true(crbegin(conta) != crend(conta));
	         #endif
	            test_true(empty(conta) == 0);
	            test_true(size(conta) > 0);
	            test_true(ssize(conta) > 0);
	        }
	        AryChk(AryChk const&) { assert(0); }
	        AryChk& operator=(AryChk const&) { assert(0); return *this; }
	    };
	    template<class A, unsigned N>
	    struct AryChk2 {
	        A   conta[N];
	        AryChk2(A const* t, size_t n) {
	            std::copy(t, t+n, conta);
	            test_true(begin(conta) != end(conta));
	            test_true(rbegin(conta) != rend(conta));
	         #if !defined(__CCWRAP_NO_HEADER_CXX14_ITERATOR)
	            test_true(cbegin(conta) != cend(conta));
	            test_true(crbegin(conta) != crend(conta));
	         #endif
	            test_true(!empty(conta));
	            test_true(size(conta) > 0);
	            test_true(ssize(conta) > 0);
	            test_true(data(conta) == &conta[0]);
	            test_true(size(conta) == N);
	            test_true(ssize(conta) == ptrdiff_t(N));
	         #if __CCWRAP_CXX >= 201402L	// #if !defined(__CCWRAP_NO_HEADER_CXX14_ITERATOR)
	            A const* ite = cbegin(conta);
	            ite = next(ite);
	            ite = next(ite, n-1);
	            test_true(ite == cend(conta));
	            ite = prev(ite);
	            ite = prev(ite, n-1);
	            test_true(ite == cbegin(conta));
	         #endif
	        }
	        AryChk2(AryChk2 const&) { assert(0); }
	        AryChk2& operator=(AryChk2 const&) { assert(0); return *this; }
	    };
	}

	TEST_CASE(iterator_sub, check) {
	    Chk<VecInt>();
	    Chk<String>();
	    Chk<ListFlt>();
	    Chk<MapStrInt>();
	    Chk<const VecInt>();
	    Chk<const String>();
	    Chk<const ListFlt>();
	    Chk<const MapStrInt>();
	    AryChk<int,100>();
	    AryChk<float, 50>();

	    static int i_tbl[] = { 1, 2, 3, 4, 5 };
	    Chk2<VecInt>(i_tbl, 5);
	    Chk2<String>("test-test", 9);
	    Chk2<const VecInt>(i_tbl, 5);
	    Chk2<const String>("Test-test", 9);
	    AryChk2<int, 5>(i_tbl, 5);
	    AryChk2<char, 9>("test-test", 9);
	}
}
