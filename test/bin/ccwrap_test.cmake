# ---------------------------------------------------------------------------
# ccwrap のテストスイート共通モジュール
#
# 4 スイート (c_test / cxx_test / etc_test/vc / etc_test/watcom) が共有する
#   - コンパイラ別の配線 (include パス / オプション / 定義)   … ccwrap_test_routing()
#   - TST_ONLY によるファイル選択                              … ccwrap_test_select()
#   - ファイル単位の try_compile probe                         … ccwrap_test_probe()
#   - gcc/clang/macOS でのリンク補助                           … ccwrap_test_link_libs()
# を1か所に集める。従来 *.bat / *.sh が持っていたコンパイラ別のフラグ決めは
# すべてここに来る (bat/sh は configure→build→test を呼ぶだけの薄いラッパ)。
#
# 各スイートの CMakeLists.txt は
#     include("${CMAKE_CURRENT_LIST_DIR}/../bin/ccwrap_test.cmake")
# のように相対 include するだけでよく、単体 configure でも umbrella
# (test/CMakeLists.txt) 経由でも同じに動く。
# ---------------------------------------------------------------------------

# Open Watcom で RTTI (-xr) を有効にするか。**既定は ON** (C++ の既定)。
# OFF にすると -noxr 側のプリビルドと対になる。
# option() はキャッシュを作るだけなので、早期 return の前に置いて必ず通す。
option(TEST_WATCOM_RTTI "Open Watcom: RTTI (-xr) を有効にする" ON)

# Exception model. OFF = -xst (the shipping build); ON = -xs, test-only (A1''), preset watcom-xrxs.
option(TEST_WATCOM_XS "Open Watcom: use the -xs exception model (OFF = -xst)" OFF)

# 関数はグローバルなので、umbrella が先に include していれば再定義は不要。
if(COMMAND ccwrap_test_routing)
    return()
endif()

# このファイルの場所 (関数の中では CMAKE_CURRENT_LIST_DIR が呼び出し側になるので、
# include 時に捕まえておく)。
set(CCWRAP_TEST_SCRIPT_DIR "${CMAKE_CURRENT_LIST_DIR}")

# ---------------------------------------------------------------------------
# ccwrap_test_compiler(<out>)
#   使っているツールチェインを
#   "msvc" / "watcom" / "dmc" / "orangec" / "gnu" に正規化して返す。
#   C だけ / C++ だけ有効なプロジェクトでも効くよう、両方の COMPILER_ID を見る。
# ---------------------------------------------------------------------------
function(ccwrap_test_compiler out)
    if(MSVC)
        set(${out} "msvc" PARENT_SCOPE)
    elseif(CMAKE_C_COMPILER_ID STREQUAL "OpenWatcom"
           OR CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom")
        set(${out} "watcom" PARENT_SCOPE)
    elseif(CMAKE_C_COMPILER_ID STREQUAL "OrangeC"
           OR CMAKE_CXX_COMPILER_ID STREQUAL "OrangeC")
        set(${out} "orangec" PARENT_SCOPE)
    elseif(DEFINED DMC_ROOT
           OR CMAKE_C_COMPILER MATCHES "(^|[/\\])dmc(\\.exe)?$")
        set(${out} "dmc" PARENT_SCOPE)
    else()
        set(${out} "gnu" PARENT_SCOPE)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# ccwrap_test_cxx_std_year(<out>)
#   CMAKE_CXX_STANDARD を **4 桁の西暦**にして返す。CMake 側の綴りは 98 / 11 / 14 /
#   17 / 20 / 23 / 26 の 2 桁固定 (CMake の API なので変えられない) で、しかも
#   **98 が一番古いのに数としては一番大きい**ため、そのままでは大小比較できない。
#   ccwrap は規格版を全て 4 桁で扱うので、受け取った直後にここで直す。
#   **下限は 2003** — ccwrap の最低ラインが C++03 なので 98 は 2003 に写す (1998 は使わない)。
#   未設定なら空を返す (呼ぶ側で「判定しない」と読むこと)。
#   **今は呼び出し元が無い**が消さないこと — 規格版を比べるときは必ずここを通す。
# ---------------------------------------------------------------------------
function(ccwrap_test_cxx_std_year out)
    set(y "")
    if(CMAKE_CXX_STANDARD STREQUAL "98")
        set(y 2003)
    elseif(CMAKE_CXX_STANDARD MATCHES "^[0-9][0-9]$")
        set(y "20${CMAKE_CXX_STANDARD}")
    endif()
    set(${out} "${y}" PARENT_SCOPE)
endfunction()

# ---------------------------------------------------------------------------
# ccwrap_test_routing(LANG <C|CXX>
#                     [LEVEL <n>]        _CCW_TARGET_C / _CCW_TARGET_CXX の値。
#                                        **空なら ccwrap を通さない** (素のコンパイラだけ)
#                     [NS <std|ccwstd>]  Watcom の名前空間環境 (既定 std)
#                     [C_CONFORM]        vc で _CCW_C_CONFORM を定義する (LEVEL 指定時のみ)
#                     OUT_INCS <v> OUT_OPTS <v> OUT_DEFS <v>)
#
#   ccwrap を使うための最小配線 = 「強制 include (ccwrap_header.h)」+
#   「コンパイラ別フォルダを include パス先頭に置く」+「_CCW_TARGET_* を定義」。
#   probe (try_compile) と実ターゲットの両方で同じ値を使うこと。
# ---------------------------------------------------------------------------
function(ccwrap_test_routing)
    cmake_parse_arguments(A "C_CONFORM"
                            "LANG;LEVEL;NS;MSVC_STD;OUT_INCS;OUT_OPTS;OUT_DEFS" "" ${ARGN})
    if(NOT A_LANG)
        message(FATAL_ERROR "ccwrap_test_routing: LANG (C|CXX) is required")
    endif()
    if(NOT A_NS)
        set(A_NS "std")
    endif()
    get_filename_component(root "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../.." ABSOLUTE)
    ccwrap_test_compiler(cc)

    set(incs "")
    set(opts "")
    set(defs "")

    # **LEVEL の有無が唯一のスイッチ**。空 (既定) なら ccwrap は一切通さず、素の
    # コンパイラのライブラリだけで組む。値があるときだけ _CCW_TARGET_<LANG> を定義し、
    # 強制 include と ccwrap の include パスを足す。「ccwrap を使わない」ことを表す
    # マクロは無い (否定形のマクロをテスト側にばら撒かないため)。
    # **規格版は 4 桁の西暦**。2 桁 (26 / 23 …) を渡すとそのまま _CCW_TARGET_* になり、
    # ccwrap のゲートが 1 つも通らないのに**診断が出ない**。古いビルドツリーの
    # キャッシュに 2 桁が残っていることがあるので (set(... CACHE) は上書きしない)、
    # ここで弾く。
    if(A_LEVEL AND A_LEVEL MATCHES "^[0-9]+$" AND A_LEVEL LESS 1000)
        message(FATAL_ERROR
            "TEST_TARGET_${A_LANG}=${A_LEVEL}: 規格版は 4 桁の西暦で渡すこと "
            "(C++ は 2003/2011/2014/2017/2020/2023/2026、C は 1990/1999/2011/2017/2023)。"
            "ビルドツリーのキャッシュに古い 2 桁が残っている場合は "
            "`cmake --preset <preset> --fresh` で作り直す。")
    endif()
    if(A_LEVEL)
        list(APPEND defs "_CCW_TARGET_${A_LANG}=${A_LEVEL}")
    endif()

    if(cc STREQUAL "msvc")
        list(APPEND opts /W3)
        list(APPEND defs _CRT_SECURE_NO_WARNINGS)
        # In a WIDE format the UCRT reads %s / %c / %[ as wchar_t by default -- the
        # opposite of C99 7.24.2, which says they take multibyte and get converted.
        # Microsoft ships the conforming behaviour behind this opt-in macro, so the
        # tests ask for it (measured on vc145: 18 wchar rows fail without it, and
        # swscanf writes wide characters into the char[] buffers -- a real overrun
        # that /RTCs catches). Pre-UCRT vc just ignores the macro.
        list(APPEND defs _CRT_STDIO_ISO_WIDE_SPECIFIERS)
        if(A_LANG STREQUAL "CXX")
            list(APPEND opts /EHsc)
            # 古い cl が知らないスイッチは、対応する版でだけ足す。vc90 (19.00 未満) は
            # /utf-8 も /std: も /Zc:__cplusplus も無い C++03 コンパイラ。
            if(MSVC_VERSION GREATER_EQUAL 1900)
                # 既定は c++latest。MSVC_STD で c++14 / c++17 / c++20 に落とせる
                # (旧 vctest.bat の c++NN 引数に相当)。
                if(NOT A_MSVC_STD)
                    set(A_MSVC_STD "c++latest")
                endif()
                list(APPEND opts /utf-8 "/std:${A_MSVC_STD}")
            endif()
            if(MSVC_VERSION GREATER_EQUAL 1914)
                list(APPEND opts /Zc:__cplusplus)
            endif()
            list(APPEND defs _SCL_SECURE_NO_WARNINGS
                             _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
                             _CRT_NON_CONFORMING_SWPRINTFS)
        endif()
        if(A_C_CONFORM AND A_LEVEL)
            # _CCW_C_CONFORM: C 標準準拠側の上書き (snprintf の戻り値、asctime の日付詰め等)
            # を有効にする opt-in。既定の ccwrap は vc 間互換のため native 挙動を保つ。
            list(APPEND defs _CCW_C_CONFORM)
        endif()
        if(A_LEVEL)
            if(A_NS STREQUAL "ccwstd")
                # ccwstd 再配置: C++ を native std でなく llibcxx03 (namespace ccwstd) から
                # 取る。vc/ccwstd/ が vc/std/ を**置き換えて** include パス先頭に立ち、
                # C 側は vc/std/ へ薄く転送、C++ 側は llibcxx03 へ向かう
                # (vc/ccwstd/ccwrap_header.h)。動機と成立性は
                # doc/vc/ccwstd_vc_feasibility.md を参照.
                # **-I は vc/ccwstd/ の 1 本だけ**。vc/std も detail/llibcxx03/include も
                # 足さない (ユーザーに 2 本渡させない、が方針)。成立する理由:
                #  - ccwstd/ は vc/std/ の**全ヘッダ**に対して転送を持つ (`"../std/<h>"`。
                #    引用符なのでファイル相対に解決し、-I を要らない)
                #  - vc/std/ の中の `<../../detail/...>` は -I 相対だが、ccwstd/ と std/ は
                #    どちらもルートから 2 階層なので、ccwstd/ 起点でも同じ場所に解決する
                #  - llibcxx03 の内部 include は自分のディレクトリからの相対 ("__config" 形式)
                list(APPEND incs "${root}/vc/ccwstd")
                list(APPEND opts "/FI${root}/vc/ccwstd/ccwrap_header.h")
            else()
                # detail/llibcxx03/include は **-I に足さない**。vc の native ヘッダ解決は
                # <../include/x> という自己参照トリック (「include という名のディレクトリが
                # 自分自身を指す」) なので、同じく include という名の llibcxx03/include を
                # 置くと native のつもりが llibcxx03 に当たる (実測: vc145 が全滅)。
                # MSVC に無いヘッダは vc/std/<h> が llibcxx03 を相対パスで直接 include する。
                list(APPEND incs "${root}/vc/std")
                list(APPEND opts "/FI${root}/vc/std/ccwrap_header.h")
            endif()
        endif()

    elseif(cc STREQUAL "watcom")
        if(A_LEVEL)
            # 強制 include は native パス (バックスラッシュ) で渡すこと。wcl386 は
            # 先頭の '/' をオプションと読むので -fi= の中の '/' が誤解析される。
            if(A_NS STREQUAL "ccwstd")
                file(TO_NATIVE_PATH "${root}/watcom/ccwstd/ccwrap_header.h" fi)
                list(APPEND incs "${root}/watcom/ccwstd")
                # ccwstd 構成は predefined macro では判別できないので、結果 tag を
                # 分けるにはここから伝えるしかない (無いと std 構成と上書きし合う)。
                list(APPEND defs _TST_ENV_CCWSTD=1)
            else()
                file(TO_NATIVE_PATH "${root}/watcom/std/ccwrap_header.h" fi)
                list(APPEND incs "${root}/watcom/std")
            endif()
            list(APPEND opts "-fi=${fi}")
            # **-i はどちらの構成でも 1 本だけ** (watcom/std か watcom/ccwstd)。
            # detail/llibcxx03/include は足さない -- llibcxx03 の内部 include は自分の
            # ディレクトリからの相対 ("__config" 形式) なので要らない。
            # ccwstd 構成で watcom/std を足さないのも同じ方針で、ccwstd/ 側が
            # watcom/std/ の**全ヘッダ**に対する転送を持つことで成立させている
            # (`"../std/<h>"` は引用符なのでファイル相対に解決し、-i を要らない。
            #  watcom/std/ の中の `<../../detail/...>` は -i 相対だが、ccwstd/ と std/ は
            #  どちらもルートから 2 階層なので、ccwstd/ 起点でも同じ場所に解決する)。
        endif()
        # -of+ (EBP フレーム) は **stacktrace_test.cpp だけ**に付ける。スイート全体に
        # 付けると A1'' を踏んで落ちる (ccwrap_watcom_bugs.md)。付ける場所は
        # test/cxx_test/CMakeLists.txt。
        if(A_LANG STREQUAL "CXX")
            # 例外を有効に (ccwrap の有無に関係なく)。モデルは TEST_WATCOM_XS で選ぶ。
            if(TEST_WATCOM_XS)
                list(APPEND opts -xs)
                # -xs and -xst share the predefined macros, so the tag suffix comes from here.
                list(APPEND defs _TST_ENV_XRXS=1)
            else()
                list(APPEND opts -xst)
            endif()
            # -DTEST_NO_EH=ON のときは -xd で打ち消す。**CMake の Open Watcom
            # プラットフォームが CMAKE_CXX_FLAGS に -xs を焼いている**ので、ここで
            # -xs を足さないだけでは消えない (実測: _CPPUNWIND が立ったままになる)。
            # -xs -xd の順で渡すと _CPPUNWIND が未定義になり try/catch も E625 になる。
            # このときは prebuilt libccwcxx.lib も接尾辞なしの variant を選ぶこと。
            if(TEST_NO_EH)
                list(APPEND opts -xd)
            endif()
            # RTTI は既定 ON (= C++ の既定。typeid / dynamic_cast を使う typeinfo /
            # functional::target_type / any::type が要求する)。-noxr 側は opt-out。
            # **プログラム全体で揃えること** — prebuilt libccwcxx.lib にも RTTI 有無の
            # 変種があり (watcom/lib/win32-<ns> は -xst -xr、-noxr は -xst のみ)、混ぜるのは想定外。
            if(TEST_WATCOM_RTTI)
                list(APPEND opts -xr)
            endif()
        endif()

    elseif(cc STREQUAL "dmc")
        # DMC uses its native options; in particular it rejects GNU's -Wall.
        # This is the native equivalent that dmc-cc used to translate -Wall to.
        list(APPEND opts -w)

    elseif(cc STREQUAL "orangec")
        # OrangeC's native spelling for enabling all warnings.  Keeping it out
        # of the GNU branch also avoids probing Unix-only link libraries.
        list(APPEND opts /w+)

    else()   # GNU / Clang (Linux / macOS / msys2)
        list(APPEND opts -Wall)
        # -stdlib=libc++ は preset の CMAKE_CXX_FLAGS 側で渡す。ここではその事実を
        # テストへ伝えるだけ (pass ログの tag に -libcxx が付く)。同じ clang でも
        # ライブラリが違えば別の結果なので、tag が同じだと上書きし合ってしまう。
        if(TST_STDLIB STREQUAL "libcxx" AND A_LANG STREQUAL "CXX")
            list(APPEND defs TST_STDLIB_LIBCXX)
        endif()
        if(A_LEVEL)
            list(APPEND incs "${root}/gcc/std")
            list(APPEND opts "-include" "${root}/gcc/std/ccwrap_header.h")
            # msys2's mingw is gcc on the SAME CRT as MSVC, so it inherits the same
            # non-conformances (asctime zero-pads the day of month). The corrections are
            # the same opt-in there as on vc.
            if(A_C_CONFORM AND WIN32)
                list(APPEND defs _CCW_C_CONFORM)
            endif()
        endif()
    endif()

    if(A_OUT_INCS)
        set(${A_OUT_INCS} ${incs} PARENT_SCOPE)
    endif()
    if(A_OUT_OPTS)
        set(${A_OUT_OPTS} ${opts} PARENT_SCOPE)
    endif()
    if(A_OUT_DEFS)
        set(${A_OUT_DEFS} ${defs} PARENT_SCOPE)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# ccwrap_test_select(EXT <.c|.cpp> DIRS <dir>... [ONLY <name>...]
#                    OUT_SRC <v> OUT_PROBE <v>)
#
#   ONLY が空なら DIRS 以下の *_test<EXT> を全部 (ソート済み)、probe は ON。
#   ONLY が指定されていれば <name>_test<EXT> だけを拾い、probe は OFF
#   (名指ししたファイルは、黙って落とさずコンパイルエラーを見せる)。
#
#   umbrella (CCWRAP_TEST_UMBRELLA) では TST_ONLY が全スイート共通の knob になるので、
#   他スイートの名前が混じる。そこでは「見つからない名前」は FATAL にせず読み飛ばし、
#   1つも残らなければ OUT_SRC を空にして返す (呼び側がスイートごと畳む)。
# ---------------------------------------------------------------------------
function(ccwrap_test_select)
    cmake_parse_arguments(A "" "EXT;OUT_SRC;OUT_PROBE" "DIRS;ONLY" ${ARGN})
    set(srcs "")
    if(A_ONLY)
        set(missing "")
        foreach(n IN LISTS A_ONLY)
            set(found "")
            foreach(d IN LISTS A_DIRS)
                if(EXISTS "${d}/${n}_test${A_EXT}")
                    set(found "${d}/${n}_test${A_EXT}")
                    break()
                endif()
            endforeach()
            if(found)
                list(APPEND srcs "${found}")
            else()
                list(APPEND missing "${n}")
            endif()
        endforeach()
        if(missing AND NOT CCWRAP_TEST_UMBRELLA)
            string(REPLACE ";" " " m "${missing}")
            message(FATAL_ERROR "TST_ONLY: no such test file(s): ${m}")
        endif()
        set(${A_OUT_PROBE} OFF PARENT_SCOPE)
    else()
        foreach(d IN LISTS A_DIRS)
            file(GLOB g CONFIGURE_DEPENDS "${d}/*_test${A_EXT}")
            list(APPEND srcs ${g})
        endforeach()
        list(SORT srcs)
        set(${A_OUT_PROBE} ON PARENT_SCOPE)
    endif()
    set(${A_OUT_SRC} ${srcs} PARENT_SCOPE)
endfunction()

# ---------------------------------------------------------------------------
# ccwrap_test_probe(SOURCES <src>... INCS <..> OPTS <..> DEFS <..>
#                   PREFIX <cache-var-prefix> LABEL <表示名>
#                   OUT_SRC <v> [OUT_SKIPPED <v>] [REQUIRE_ANY])
#
#   各ファイルを単独で try_compile し、通らないものを外す (一覧を表示)。
#   判定はキャッシュされるので、テストを直したら --fresh で再判定すること。
#   REQUIRE_ANY … 1 本も残らなかったら FATAL_ERROR にする (ヘッダ別スイート用)。
# ---------------------------------------------------------------------------
function(ccwrap_test_probe)
    cmake_parse_arguments(A "REQUIRE_ANY" "PREFIX;LABEL;OUT_SRC;OUT_SKIPPED"
                            "SOURCES;INCS;OPTS;DEFS" ${ARGN})
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)   # テストファイルに main は無い
    # probe は**実ビルドと同じ構成**で測ること。try_compile の既定は Debug なので、
    # そのままだと Release ビルドを Debug のフラグで判定してしまう。
    if(CMAKE_BUILD_TYPE)
        set(CMAKE_TRY_COMPILE_CONFIGURATION "${CMAKE_BUILD_TYPE}")
    endif()
    set(args "")
    foreach(d IN LISTS A_DEFS)
        list(APPEND args "-D${d}")
    endforeach()
    list(APPEND args ${A_OPTS})

    set(ok "")
    set(skipped "")
    foreach(src IN LISTS A_SOURCES)
        get_filename_component(name "${src}" NAME_WE)
        string(TOUPPER "${name}" NAME_UC)
        set(var "${A_PREFIX}_${NAME_UC}")
        if(NOT DEFINED ${var})
            try_compile(${var}
                SOURCES "${src}"
                COMPILE_DEFINITIONS ${args}
                CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${A_INCS}")
            set(${var} ${${var}} CACHE INTERNAL "${A_LABEL} ${name} compiles")
        endif()
        if(${var})
            list(APPEND ok "${src}")
        else()
            list(APPEND skipped "${name}")
        endif()
    endforeach()

    if(skipped)
        string(REPLACE ";" " " s "${skipped}")
        message(STATUS "${A_LABEL}: skipped (did not compile): ${s}")
    endif()
    # ヘッダ別スイートで 1 本も残らないのは「そのファイルが通らない」ではなく
    # ツールチェインが壊れている側の話。黙って続けると 0 グループの exe が全緑を
    # 報告する (実例: vc143/vc145 の x86 NMake preset で try_compile の lib ステップが
    # lib.exe を見つけられない)。compile_fail は 1 本単位で probe するので対象外。
    if(A_REQUIRE_ANY AND NOT ok)
        list(LENGTH A_SOURCES n)
        message(FATAL_ERROR
            "${A_LABEL}: none of the ${n} test files compiled -- the toolchain is broken, "
            "not the tests. Check the try_compile output in "
            "${CMAKE_BINARY_DIR}/CMakeFiles/CMakeConfigureLog.yaml")
    endif()
    set(${A_OUT_SRC} ${ok} PARENT_SCOPE)
    if(A_OUT_SKIPPED)
        set(${A_OUT_SKIPPED} ${skipped} PARENT_SCOPE)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# ccwrap_test_link_libs(<target> LANG <C|CXX>)
#
#   gcc/clang でしか要らないリンク時の面倒を見る (Windows のコンパイラは自前で解決する)。
#     - libm      : C の数学関数 (glibc は別ライブラリ)
#     - Threads   : <threads.h> / <thread> / <mutex>。-pthread かどうかは CMake が決める
#     - libatomic : clang が <atomic> の一部を __atomic_* 呼び出しに落とす。
#                   **Apple/BSD には存在しない**ので、在るときだけ足す。
# ---------------------------------------------------------------------------
function(ccwrap_test_link_libs tgt)
    cmake_parse_arguments(A "" "LANG" "" ${ARGN})
    ccwrap_test_compiler(cc)
    if(NOT cc STREQUAL "gnu")
        return()
    endif()

    include(CheckLibraryExists)
    find_package(Threads QUIET)
    if(Threads_FOUND)
        target_link_libraries(${tgt} PRIVATE Threads::Threads)
    endif()

    if(NOT APPLE)
        check_library_exists(m sqrt "" CCWRAP_HAVE_LIBM)
        if(CCWRAP_HAVE_LIBM)
            target_link_libraries(${tgt} PRIVATE m)
        endif()
        check_library_exists(atomic __atomic_load_8 "" CCWRAP_HAVE_LIBATOMIC)
        if(CCWRAP_HAVE_LIBATOMIC)
            target_link_libraries(${tgt} PRIVATE atomic)
        endif()
    endif()

    # <stacktrace> (C++23) is header-declared but its backend lives in a separate
    # library: libstdc++ puts it in stdc++exp, libc++ in c++experimental. Without it
    # every std::stacktrace::current() is an undefined reference at link time.
    # HINTS: libstdc++exp.a sits in the compiler's own directory
    # (/usr/lib/gcc/<triple>/<ver>), which is NOT on the default find_library path.
    if(A_LANG STREQUAL "CXX")
        find_library(CCWRAP_STACKTRACE_LIB NAMES stdc++exp c++experimental
                     HINTS ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})
        if(CCWRAP_STACKTRACE_LIB)
            target_link_libraries(${tgt} PRIVATE "${CCWRAP_STACKTRACE_LIB}")
        endif()
    endif()
endfunction()

# ---------------------------------------------------------------------------
# ccwrap_test_utf8_acp(<target>)
#
#   MSVC のとき、実行ファイルに「プロセスの ANSI コードページは UTF-8」というマニフェスト
#   (test/src/utf8_acp.manifest) を埋め込む。Windows 10 1903 以降で効く。
#
#   これが無いと ACP は環境まかせ (日本語 Windows なら CP932) で、CRT のマルチバイト側
#   (setlocale / mbrtowc / mbrtoc16 / mbrtoc32 / wcstombs / コンソール) が UTF-8 でなく
#   なり、UTF-8 を扱うテストが軒並み skip になる。**API を UTF-8 にするだけ**で、
#   コンパイラが narrow 文字列リテラルをどう符号化するかは変わらない (それは /utf-8 の
#   担当で、vc90 等には無い) --- テスト側は UTF-8 のバイト列を手で組むか wide リテラル
#   から変換すること。char リテラルに直接 UTF-8 を書かない。
#
#   渡し方は **`.manifest` をターゲットのソースに足すだけ**。CMake がそれを収集して
#   マニフェストツール (mt.exe) に渡し、リンカが生成する分とマージしてくれる。
#   `/MANIFEST:EMBED /MANIFESTINPUT:` を自分で足してはいけない --- CMake は Makefile 系
#   ジェネレータで自前の `manifest.res` を作ってリンクするので、EMBED と二重になり
#   **`CVT1100: 重複するリソースです type:MANIFEST` → `LNK1123`** でリンクが落ちる
#   (vc120 + NMake で実測。VS ジェネレータの vc145/vc143 では出ないので気づきにくい)。
#
#   **古い mt.exe は知らない要素を黙って捨てる。** VS2013 付属のものは
#   `81010002 Unrecognized Element activeCodePage` と警告した上で、その要素を落とした
#   マニフェストを書く (= 埋め込んだつもりで何も効いていない)。そのため
#   `test/bin/mt_with_setcc.bat` は **Windows 10 SDK の mt.exe を優先**する。
#   効くのは Windows 10 1903 以降なので、古い OS では単に無視される。
# ---------------------------------------------------------------------------
function(ccwrap_test_utf8_acp tgt)
    ccwrap_test_compiler(cc)
    if(NOT cc STREQUAL "msvc")
        return()
    endif()
    set(mf "${CCWRAP_TEST_SCRIPT_DIR}/../src/utf8_acp.manifest")
    get_filename_component(mf "${mf}" ABSOLUTE)
    target_sources(${tgt} PRIVATE "${mf}")
endfunction()

# ---------------------------------------------------------------------------
# ccwrap_test_skip_counts()
#
#   #if の反対側に置く TEST_SKIP1() の数が、置き換えたチェック数と合っているかの静的検査。
#   実行時には気づけない (どの構成でも green のまま) ので ctest に別テストとして登録する。
#   umbrella では 1 回だけ登録する (スイートごとに登録すると名前が衝突する)。
# ---------------------------------------------------------------------------
function(ccwrap_test_skip_counts)
    # グローバルプロパティで「この configure 中に登録済みか」を見る (キャッシュだと
    # 2 回目の configure で登録が消える)。
    get_property(done GLOBAL PROPERTY CCWRAP_TEST_SKIP_COUNTS_REGISTERED)
    if(done)
        return()
    endif()
    get_filename_component(root "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../.." ABSOLUTE)
    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME skip_counts
                 COMMAND "${Python3_EXECUTABLE}" "${root}/test/bin/check_skip_counts.py" -q
                 WORKING_DIRECTORY "${root}")
        set_property(GLOBAL PROPERTY CCWRAP_TEST_SKIP_COUNTS_REGISTERED ON)
    endif()
endfunction()
