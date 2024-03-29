#include <ccwrap_misc/test.hpp>

#include <filesystem>
#include <string_view>

#ifdef __CCWRAP_BOOST2STD
#define USE_BOOST_PATH
#else
//#include <ccwrap/ccwrap_misc/filesystem_path.hpp>
#endif

#if __CCWRAP_CXX >= 201103L || __cplusplus >= 201103L || _MSVC_LANG >= 201103L
#define ENABLE_CXX11
#endif

#include <vector>
#include <ccwrap_misc/utfenc.hpp>


#if defined(_WIN32) //&& defined(UNICODE)
#define T(x)    L##x
#else
#define T(x)    x
#endif

#ifdef _WIN32
#define DS      L"\\"
#else
#define DS      "/"
#endif

#if __CCWRAP__ || __cplusplus >= 202000L || _HAS_CXX20
#define HAS_U8STRING    0
#endif



TEST_SUITE(filesystem_path) {

	using namespace std;
	using namespace std::filesystem;
	using std::filesystem::path;

	enum {
	    rootname,
	    rootdir,
	    foo,
	    bar,
	    baz,
	    test_txt,
	    txt,
	    dot_txt,
	    bin,
	    test,

	    rootpath,

	    foo_test_txt,
	    foo_bar_baz_test_txt,
	    rootdir_foo_bar_baz_test_txt,
	    rootpath_foo_bar_baz_test_txt,
	    rootname_foo_bar_baz_test_txt,
	    rootpath_foo_bar_baz_test_bin,
	    rootpath_foo_bar_baz_test_test,

	    rootpath_foo_bar_baz_dot_test,
	    rootpath_foo_bar_baz_dot_test_txt,
	    rootpath_foo_bar_baz_dot_test_test,

	    rootpath_foo_bar_baz_test_txt_UN1,
	    rootdir_dot_foo_dot_test_txt_UN2,

	    //foo_test,
	    //foo_bar_baz_test,
	    //rootdir_foo_bar_baz_test,
	    //rootname_foo_bar_baz_test,
	    rootpath_foo_bar_baz_test,

	    //foo_bar_baz,
	    //rootdir_foo_bar_baz,
	    //rootname_foo_bar_baz,

	    rootpath_foo_bar_baz,
	    rootpath_foo_bar_baz_sep,

	    d_drive,

	    dot_foo,
	    dot_test,
	    dot_foo_dot_test,
	    //dot_test_txt,
	    //dot_foo_dot_test_txt,
	    rootdir_dot_foo_dot_test_txt,
	    rootpath_dot_foo_dot_test_txt,

	    //foo_sep5_test_txt,
	    //foo_sep2_bar_cur_cur_baz_par_par_test_txt,

	    none,
	};

	static wchar_t const* const wchar_paths1[] = {
	    L"c:",
	    L"/",
	    L"foo",
	    L"bar",
	    L"baz",
	    L"test.txt",
	    L"txt",
	    L".txt",
	    L"bin",
	    L"test",

	    L"c:/",

	    L"foo/test.txt",
	    L"foo/bar/baz/test.txt",
	    L"/foo/bar/baz/test.txt",
	    L"c:/foo/bar/baz/test.txt",
	    L"c:foo/bar/baz/test.txt",
	    L"c:/foo/bar/baz/test.bin",
	    L"c:/foo/bar/baz/test.test",

	    L"c:/foo/bar/baz/.test",
	    L"c:/foo/bar/baz/.test.txt",
	    L"c:/foo/bar/baz/.test.test",

	    L"c:////foo/./././//bar/../bar/baz/test.txt",
	    L"/.///./foo/../.foo/test.txt/../.test.txt",

	    // L"foo/test",
	    // L"foo/bar/baz/test",
	    // L"/foo/bar/baz/test",
	    // L"c:foo/bar/baz/test",
	    L"c:/foo/bar/baz/test",

	    //L"foo/bar/baz",
	    //L"/foo/bar/baz",
	    //L"c:foo/bar/baz",

	    L"c:/foo/bar/baz",
	    L"c:/foo/bar/baz/",

	    L"d:",

	    L".foo",
	    L".test",
	    L".foo/.test",
	    //L".test.txt",
	    //L".foo/.test.txt",
	    L"/.foo/.test.txt",
	    L"c:/.foo/.test.txt",

	    //L"foo/////test.txt",
	    //L"foo//bar/././baz/../../test.txt",

	    L"",
	};
	enum { wchar_paths_size = sizeof(wchar_paths1)/sizeof(wchar_paths1[0]) };

	static wchar_t const* const wchar_paths2[] = {
	 #ifdef USE_SJIS
	    #include "filesystem_path_test_str_sjis.hh"
	 #else
	    #include "filesystem_path_test_str_utf8.hh"
	 #endif
	};
	enum { wchar_paths_size2 = sizeof(wchar_paths2)/sizeof(wchar_paths2[0]) };
	static_assert(wchar_paths_size == wchar_paths_size2, "");

	void changeDirSeps(path::string_type& s) {
	    for (size_t i = 0; i < s.size(); ++i) {
	        if (s[i] == path::value_type('/')) {
	            s[i] = path::preferred_separator;
	        }
	    }
	}

	TEST_CASE(filesystem_path, check1) {
	    path::string_type       str1(T("test"));
	    path::value_type        cc1(T('!'));
	    path::iterator          ite0;
	 #if !defined(USE_BOOST_PATH)
	    path::format            formatN = path::native_format;
	  #if __CCWRAP__
	    static_assert(path::preferred_separator == ccwrap::strpath::preferred_separator, "");
	  #endif
	 #endif
	    path::const_iterator    icbbte0;

	    path path0;
	    test_true(path0.empty());
	    test_true(path0.native().empty());
	    path    path1(path0);
	    test_true(path0 == path1);

	    path    path2(T("test.txt"));
	    test_true(path2 == T("test.txt"));
	    test_true(path2.compare(path0) != 0);
	    test_true(path2.compare(path0) > 0);
	    test_true(path2.compare(path0) >= 0);
	    test_true(path0.compare(path2) != 0);
	    test_true(path0.compare(path2) < 0);
	    test_true(path0.compare(path2) <= 0);
	    test_true(path2 != path0);
	    test_true(path2 >  path0);
	    test_true(path2 >= path0);
	    test_true(path0 != path2);
	    test_true(path0 <  path2);
	    test_true(path0 <= path2);

	    test_true(path2 != T(""));
	    test_true(path2 >  T(""));
	    test_true(path2 >= T(""));
	    test_true(T("") != path2);
	    test_true(T("") <  path2);
	    test_true(T("") <= path2);

	    path    path3(T("ddd/test.txt"));
	    //test_true(path3 == T("ddd") DS T("test.txt"));
	    test_true(path3 == T("ddd/test.txt"));
	    test_true(path3.compare(path2) != 0);
	    test_true(path3.compare(path2) < 0);
	    test_true(path3.compare(path2) <= 0);
	    test_true(path2.compare(path3) != 0);
	    test_true(path2.compare(path3) > 0);
	    test_true(path2.compare(path3) >= 0);
	    test_true(path3 != path2);
	    test_true(path3 <  path2);
	    test_true(path3 <= path2);
	    test_true(path2 != path3);
	    test_true(path2 >  path3);
	    test_true(path2 >= path3);
	    test_true(path3.compare(T("test.txt")) != 0);
	    test_true(path3.compare(T("test.txt")) < 0);
	    test_true(path3.compare(T("test.txt")) <= 0);
	    test_true(path3 != T("test.txt"));
	    test_true(path3 <  T("test.txt"));
	    test_true(path3 <= T("test.txt"));
	    test_true(T("test.txt") != path3);
	    test_true(T("test.txt") >  path3);
	    test_true(T("test.txt") >= path3);

	    basic_string<path::value_type>      sss = T("xxx/yyy/hello world.bin");
	    path    path4(sss.begin(), sss.end());
	    path   path5(path4);
	    test_true(path4.native() == path5.native());
	    //test_true(path5.native() == T("xxx") DS T("yyy") DS T("hello world.bin"));
	    test_true(path5.native() == sss);
	    test_true(path5.native() == T("xxx/yyy/hello world.bin"));
	    test_true(memcmp(path5.c_str(), T("xxx/yyy/hello world.bin"), sss.size() * sizeof(path::value_type) ) == 0);

	    test_true(path5.make_preferred() == T("xxx") DS T("yyy") DS T("hello world.bin"));
	    test_true(path5.native() == T("xxx") DS T("yyy") DS T("hello world.bin"));

	 #if __CCWRAP >= 201103L
	    path   path6(std::move(path5));
	    path1 = path2;
	    path5 = std::move(path2);
	    test_true(path6 == path4);
	    test_true(path5 == path1);
	 #endif
	    path1 = path2;
	    path2 = path4;
	    test_true(path2 == path4);
	    path5 = path1.native();
	    test_true(path5 == path1);
	    path5.clear();
	    test_true(path5.native().empty());
	    test_true(path5.empty());
	 #if !defined(USE_BOOST_PATH)
	    path5.assign(path1);
	 #else
	    path5 = path1;
	 #endif
	    test_true(path5 == path1);
	    path5.assign(sss.begin(), sss.end());
	    test_true(path5 == path4);
	    path5 = sss;
	    //test_true(path5.native() == T("xxx") DS T("yyy") DS T("hello world.bin"));
	    test_true(path5.native() == T("xxx/yyy/hello world.bin"));

	 #if !defined(USE_BOOST_PATH)
	    path path7(sss, path::native_format);
	    test_true(path7 == path4);
	    path path7b(sss, path::generic_format);
	    test_true(path7b == path4);
	    path path7c(sss, path::auto_format);
	    test_true(path7c == path4);
	 #else
	    path path7(sss);
	 #endif
	    path5.clear();
	    swap(path7, path5);
	    test_true(path5 == path4);
	    test_true(path7.empty());

	    path path8 = T("abc/def.g");
	    stringstream ss;
	    ss << path8;
	    test_true(ss.str() == "\"abc/def.g\"");
	    path path9;
	    ss >> path9;
	    test_true(path9 == T("abc/def.g"));
	}

	template<class S>
	void gen_string_paths(vector<S>& paths, wchar_t const* const wchar_paths[]) {
	    paths.clear();
	    paths.reserve(wchar_paths_size);
	    for (size_t i = 0; i < wchar_paths_size; ++i) {
	        paths.push_back(ccwrap::utfenc::to<S>(wchar_paths[i]));
	    }
	}

	void sub_codeConv(wchar_t const* const wchar_paths[], bool sep_mode) {
	    // Match pump code conv....
	    vector<string>              pathStrs;
	    vector<wstring>             pathWStrs;
	 #if 1 //HAS_U8STRING
	    vector<u8string>            pathU8Strs;
	 #endif
	    vector<u16string>           pathU16Strs;
	    vector<u32string>           pathU32Strs;
	    vector<path::string_type>   nates;
	    vector<path>                paths;
	    gen_string_paths(pathStrs, wchar_paths);
	    gen_string_paths(pathWStrs, wchar_paths);
	 #if 1 //HAS_U8STRING
	    gen_string_paths(pathU8Strs, wchar_paths);
	 #endif
	    gen_string_paths(pathU16Strs, wchar_paths);
	    gen_string_paths(pathU32Strs, wchar_paths);
	    gen_string_paths(nates, wchar_paths);
	    if (sep_mode) {
	        for (size_t i = 0; i < nates.size(); ++i)
	            changeDirSeps(nates[i]);
	    }
	    paths.clear();
	    paths.reserve(wchar_paths_size);
	    for (size_t i = 0; i < wchar_paths_size; ++i) {
	        paths.push_back(nates[i]);
	    }
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].native() == nates[i]);
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_eq(paths[i].generic_string(), pathStrs[i]);
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].generic_wstring() == pathWStrs[i]);
	#if !defined(USE_BOOST_PATH)
	 #if HAS_U8STRING
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].generic_u8string() == pathU8Strs[i]);
	 #endif
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].generic_u16string() == pathU16Strs[i]);
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].generic_u32string() == pathU32Strs[i]);
	#endif
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].string() == ccwrap::utfenc::to<string>(nates[i]));

	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].wstring() == ccwrap::utfenc::to<wstring>(nates[i]));
	#if !defined(USE_BOOST_PATH)
	 #if HAS_U8STRING
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].u8string() == ccwrap::utfenc::to<u8string>(nates[i]));
	 #endif
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].u16string() == ccwrap::utfenc::to<u16string>(nates[i]));
	    for (size_t i = 0; i < paths.size(); ++i)
	        test_true(paths[i].u32string() == ccwrap::utfenc::to<u32string>(nates[i]));

	    for (size_t i = 0; i < paths.size(); ++i) {
	     #if __CCWRAP_CXX >= 201103L
	        test_true(paths[i] == u8path((string const&)pathU8Strs[i]));
	        test_true(paths[i] == u8path((char const*)pathU8Strs[i].data(), (char const*)pathU8Strs[i].data() + pathU8Strs[i].size()));
	     #else
	        test_eq(paths[i], u8path(pathU8Strs[i]));
	        test_eq(paths[i], u8path(pathU8Strs[i].begin(), pathU8Strs[i].end()));
	     #endif
	    }
	#endif
	}

	TEST_CASE(filesystem_path, check2) {
	    sub_codeConv(wchar_paths1, false);
	    sub_codeConv(wchar_paths1, true);
	    sub_codeConv(wchar_paths2, false);
	    sub_codeConv(wchar_paths2, true);
	}

	void sub_join_remove(wchar_t const* const wchar_paths[], bool sep_mode) {
	    vector<string>              pathStrs;
	    vector<wstring>             pathWStrs;
	    vector<path::string_type>   nates;
	    vector<path>                paths;
	    gen_string_paths(pathStrs, wchar_paths);
	    gen_string_paths(pathWStrs, wchar_paths);
	    gen_string_paths(nates, wchar_paths);
	    //for (size_t i = 0; i < nates.size(); ++i)
	    //  changeDirSeps(nates[i]);
	    paths.clear();
	    paths.reserve(wchar_paths_size);
	    for (size_t i = 0; i < wchar_paths_size; ++i) {
	        paths.push_back(nates[i]);
	    }
	    if (sep_mode) {
	        for (size_t i = 0; i < nates.size(); ++i)
	            changeDirSeps(nates[i]);
	    }
	    path t_rootname = nates[rootname];
	    path t_rootdir  = nates[rootdir];
	    path t_rootpath = t_rootname;
	    t_rootpath /= t_rootdir;
	    test_true(t_rootpath == nates[rootpath]);
	    path t_path = t_rootpath / nates[foo] / nates[bar] / nates[baz];
	    test_true(t_path == nates[rootpath_foo_bar_baz]);

	    t_path = t_rootname;
	    t_path += nates[foo];
	    t_path = t_path / nates[bar] / nates[baz] / nates[test_txt];
	    test_true(t_path == nates[rootname_foo_bar_baz_test_txt]);

	    t_path /= nates[rootpath_foo_bar_baz];
	    test_true(t_path == nates[rootpath_foo_bar_baz]);

	    t_path /= nates[rootdir_dot_foo_dot_test_txt];
	    test_true(t_path == nates[rootpath_dot_foo_dot_test_txt]);

	    t_path.append(nates[rootpath_foo_bar_baz]);
	    test_true(t_path == nates[rootpath_foo_bar_baz]);
	    t_path.append(nates[rootdir_dot_foo_dot_test_txt]);
	    test_true(t_path == nates[rootpath_dot_foo_dot_test_txt]);

	    t_path.append(nates[rootpath_foo_bar_baz].begin(), nates[rootpath_foo_bar_baz].end());
	    test_true(t_path == nates[rootpath_foo_bar_baz]);
	    t_path.append(nates[rootdir_dot_foo_dot_test_txt].begin(), nates[rootdir_dot_foo_dot_test_txt].end()     );
	    test_true(t_path == nates[rootpath_dot_foo_dot_test_txt]);

	    t_path.clear();
	    t_path.concat(nates[dot_foo]);
	    t_path+= path::preferred_separator;
	    t_path.concat(nates[dot_test]);
	    test_true(t_path == nates[dot_foo_dot_test]);

	    t_path = nates[rootpath_foo_bar_baz_test_txt];
	    test_true(t_path.has_root_name());
	    test_true(t_path.has_root_path());
	    test_true(t_path.has_root_directory());
	    test_true(t_path.has_relative_path());
	    test_true(t_path.has_parent_path());
	    test_true(t_path.has_filename());
	    test_true(t_path.has_stem());
	    test_true(t_path.has_extension());
	    test_true(t_path.is_absolute());
	    test_true(!t_path.is_relative());

	    test_true(t_path.root_name() == nates[rootname]);
	    test_true(t_path.root_path() == nates[rootpath]);
	    test_true(t_path.root_directory() == nates[rootdir]);
	    test_true(t_path.relative_path() == nates[foo_bar_baz_test_txt]);
	    test_true(t_path.parent_path() == nates[rootpath_foo_bar_baz]);
	    test_true(t_path.filename() == nates[test_txt]);
	    test_true(t_path.stem() == nates[test]);
	    test_true(t_path.extension() == nates[dot_txt]);

	    path t_path2;
	    t_path.remove_filename();
	    test_true(t_path == nates[rootpath_foo_bar_baz_sep]);
	    t_path /= nates[test_txt];
	    test_true(t_path == nates[rootpath_foo_bar_baz_test_txt]);
	    t_path.replace_extension(nates[bin]);
	    test_true(t_path == nates[rootpath_foo_bar_baz_test_bin]);
	    t_path.replace_extension(nates[dot_test]);
	    test_true(t_path == nates[rootpath_foo_bar_baz_test_test]);

	#if !defined(USE_BOOST_PATH)
	    t_path.replace_filename(nates[dot_test]);
	    test_true(t_path == nates[rootpath_foo_bar_baz_dot_test]);
	#endif
	    t_path2 = t_path.extension();
	    test_true(t_path2 == nates[none]);
	    t_path.replace_extension(nates[txt]);
	    test_true(t_path == nates[rootpath_foo_bar_baz_dot_test_txt]);
	    t_path.replace_extension(nates[dot_test]);
	    test_true(t_path == nates[rootpath_foo_bar_baz_dot_test_test]);

	    t_path = nates[rootpath_foo_bar_baz_test_txt_UN1];
	    test_true(t_path == nates[rootpath_foo_bar_baz_test_txt_UN1]);
	    t_path = t_path.lexically_normal();
	    test_true(t_path == nates[rootpath_foo_bar_baz_test_txt]);
	    t_path = t_path.lexically_relative(nates[rootpath_foo_bar_baz]);
	    test_true(t_path == nates[test_txt]);

	    t_path = nates[rootdir_dot_foo_dot_test_txt_UN2];
	    test_true(t_path == nates[rootdir_dot_foo_dot_test_txt_UN2]);
	    t_path = t_path.lexically_normal();
	    test_true(t_path == nates[rootdir_dot_foo_dot_test_txt]);

	    t_path2 = nates[rootpath_foo_bar_baz_test_txt];
	    t_path = t_path2.lexically_relative(nates[rootpath]);
	    test_true(t_path == nates[foo_bar_baz_test_txt]);
	    t_path = t_path2.lexically_relative(nates[rootname]);
	    //test_true(t_path == nates[rootdir_foo_bar_baz_test_txt]);
	    test_true(t_path == nates[none]);
	    t_path = t_path2.lexically_relative(nates[d_drive]);
	    test_true(t_path == nates[none]);
	    t_path = t_path2.lexically_proximate(nates[rootpath]);
	    test_true(t_path == nates[foo_bar_baz_test_txt]);
	    t_path = t_path2.lexically_proximate(nates[rootname]);
	    test_true(t_path == nates[rootpath_foo_bar_baz_test_txt]);
	    t_path = t_path2.lexically_proximate(nates[d_drive]);
	    test_true(t_path == nates[rootpath_foo_bar_baz_test_txt]);
	}

	TEST_CASE(filesystem_path, check3) {
	    sub_join_remove(wchar_paths1, false);
	    sub_join_remove(wchar_paths1, true);
	    sub_join_remove(wchar_paths2, false);
	    sub_join_remove(wchar_paths2, true);
	}

	void sub_iterator(wchar_t const* const wchar_paths[], bool sep_mode=false, unsigned rootFlag=0) {
	    vector<path::string_type>   nates;
	    gen_string_paths(nates, wchar_paths);
	    if (sep_mode) {
	        for (size_t i = 0; i < nates.size(); ++i)
	            changeDirSeps(nates[i]);
	    }
	    vector<path::string_type>   sts;
	    if (rootFlag & 2)
	    sts.push_back(nates[rootname]);
	    if (rootFlag & 1)
	        sts.push_back(nates[rootdir]);
	    sts.push_back(nates[foo]);
	    sts.push_back(nates[bar]);
	    sts.push_back(nates[baz]);
	    sts.push_back(nates[test_txt]);
	    size_t num = sts.size();
	    static unsigned const tbl[4] = {
	        foo_bar_baz_test_txt,
	        rootdir_foo_bar_baz_test_txt,
	        rootname_foo_bar_baz_test_txt,
	        rootpath_foo_bar_baz_test_txt,
	    };
	    path    t_path = nates[tbl[rootFlag & 3]];

	    vector<path>    paths;
	    path::iterator  ite;
	    for (ite = t_path.begin(); ite != t_path.end(); ++ite) {
	        paths.push_back(*ite);
	    }
	    test_eq(paths.size(), num);
	    for (size_t i = 0; i < num && i < sts.size(); ++i) {
	        test_true(paths[i].native() == sts[i]);
	    }
	    paths.clear();
	    ite = t_path.end();
	    do {
	        --ite;
	        paths.push_back(*ite);
	    } while (ite != t_path.begin() && paths.size() < num+1);
	    test_eq(paths.size(), num);
	    for (size_t i = 0; i < sts.size() && i < num; ++i) {
	        test_true(paths[i].native() == sts[num-1-i]);
	    }
	}

	TEST_CASE(filesystem_path, check4) {
	    for (unsigned flag = 0; flag < 4; ++flag) {
	        sub_iterator(wchar_paths1, false, flag);
	        sub_iterator(wchar_paths1, true , flag);
	        sub_iterator(wchar_paths2, false, flag);
	        sub_iterator(wchar_paths2, true , flag);
	    }
	}

}
