# ccwrap prebuilt libraries: compiles the llibcxx03 out-of-line sources (thread / locale /
# filesystem) into a static libccwcxx.lib, once per variant. Link the matching variant
# into an app instead of adding the .cpp to every build:
#
#   watcom/lib/<os>-<ns>[-fs-char][-<opts>]/libccwcxx.lib
#
#   <os>    win32 | dos32
#   <ns>    std | ccwstd
#   -fs-char  filesystem::path::value_type forced to char (see below)
#   <opts>  **推奨の既定から何を変えたか**。C++ は例外も RTTI もあるのが既定で、
#           例外モデルは **-xst** を採る (`-xs` / `-xss` は A1'' を踏む。
#           doc/watcom/ccwrap_watcom_bugs.md)。よって `-xst -xr` が**無印**:
#             (なし)  -xst -xr      推奨。正式ビルドはこれ
#             -xrxs   -xs -xr       規格どおりだが A1'' を踏む。**テスト用**
#             -xst    -xst のみ     RTTI 無し
#             -xr     -xr のみ      例外無し
#             -noeh   どちらも無し
#           取り違えるとリンクは通って実行時に壊れるので、名前がどれかを言う。
#           **例外モデル (-xst / -xs) だけは ABI ではない** -- -xst の実行ファイルに
#           -xrxs のライブラリを混ぜても動く (実測)。分けてあるのは、ライブラリ自身の
#           フレームも測りたいテストのため。
#
#   watcom/lib/win32-std/libccwcxx.lib           namespace std,    -xst -xr
#   watcom/lib/win32-std-xrxs/libccwcxx.lib      namespace std,    -xs -xr
#   watcom/lib/win32-std-xst/libccwcxx.lib       namespace std,    -xst
#   watcom/lib/win32-std-noeh/libccwcxx.lib      namespace std,    (どちらも無し)
#   watcom/lib/win32-ccwstd/libccwcxx.lib        namespace ccwstd, -xst -xr
#   watcom/lib/win32-ccwstd-xrxs/libccwcxx.lib   namespace ccwstd, -xs -xr
#
# The name is libccwcxx (C++ side). A C-only companion would be libccwc.
#
# Each is built at the DEFAULT filesystem::path character type for its target:
# wchar_t on Win32 (matching MSVC), char on DOS. A program that overrides that with
# -D_CCW_FS_WCHAR=0 must link the -fs-char sibling (win32-std-fs-char, ...). That is an
# ABI switch -- path keeps its mangling but changes layout -- so mixing links cleanly
# and then misbehaves, exactly like the RTTI split.
#
# Debug builds go under watcom/lib/debug/<same name>/ so that the release set stays flat
# at the top (doc/todo.md 2026-08-02: lib/<os-種類>/ and lib/debug/<os-種類>/).
#
# 使う側 (wcl386 のコマンドライン。実測):
#
#   推奨 (例外 + RTTI、-xst) -- 無印の variant
#     wcl386 -bt=nt -l=nt -bm -xst -xr -cc++ -i=<ccwrap>\watcom\std ^
#            -fi=<ccwrap>\watcom\std\ccwrap_header.h ^
#            hello.cpp <ccwrap>\watcom\lib\win32-std\libccwcxx.lib
#
#   例外も RTTI も無し -- -noeh の variant (-xd で例外を切り、-xr を付けない)
#     wcl386 -bt=nt -l=nt -bm -xd -cc++ -i=<ccwrap>\watcom\std ^
#            -fi=<ccwrap>\watcom\std\ccwrap_header.h ^
#            hello.cpp <ccwrap>\watcom\lib\win32-std-noeh\libccwcxx.lib
#
#   -bm (static MT ランタイム) はライブラリ側と揃える。-cc++ が無いと wcl386 は
#   .cpp を C として処理する。-fi= は '/' 始まりを誤解析するので **native パス**で渡す。
#
# **全部まとめて作るなら `gen.bat`** (win32/dos32 x release/debug の 4 configure)。
# configure は 4 つで足りる: watcom / watcom-debug / watcom-dos32 / watcom-dos32-debug。
#
# **variant を絞るのは build preset か target**。configure は共通で、何を作るかだけが違う:
#
#   cmake --build --preset watcom-default     win32 release の無印 (-xst -xr) 4 本だけ
#   cmake --build --preset watcom-debug-noeh  win32 debug の -noeh 4 本だけ
#   gen win32 noeh                            上の release + debug をまとめて
#
#   default / xrxs / noeh を 4 configure すべてに用意してある
#   (watcom-default / watcom-dos32-debug-noeh …)。-xst は既定の派生なので preset は無し。
#
# **1 本だけなら target 名を指定する。target 名は出力フォルダ名の `-` を `_` にしたもの**:
#
#   cmake --build --preset watcom --target ccwrap_win32_std_fs_char_xst
#
# Each namespace variant is compiled with the corresponding force-include + include path,
# so its out-of-line symbols land in the right namespace (the same per-build model the
# headers use).
#
# Why RTTI still gets its own variant even though the default is on: -xr costs code size
# and vtable space in every translation unit, and Open Watcom is targeted at small
# binaries, so dropping it is a real option (-xst). But typeid / dynamic_cast are needed
# by <typeinfo>, std::function::target_type and std::any's type(), and a program that uses
# them must be built with -xr THROUGHOUT -- mixing an -xr program with a non-RTTI
# libccwcxx.lib is not a supported combination. Hence one library per answer, and the
# consumer picks. test/ selects with TEST_WATCOM_RTTI (既定 ON = 無印。OFF にすると
# -xst)。
cmake_minimum_required(VERSION 3.20)

# CMP0136 (>=3.27) is what makes CMAKE_WATCOM_RUNTIME_LIBRARY effective: under its OLD
# behaviour the Open Watcom platform hardcodes "-br -bm" (the DLL runtime) into the base
# flags, so the library would come out DLL-runtime while test/ (and any ordinary app)
# links statically -- they do not mix (undefined __cnvs2d_ / _EFG_Format_, W1171).
if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()

project(ccwrap_lib CXX)

# CMake's Open Watcom platform bakes -xs into the base CXX flags. Every variant below
# states its own exception model, so take it out of the base. The NO_EH variants also
# pass -xd explicitly (below) -- that is what actually turns exceptions off.
string(REGEX REPLACE "(^| )-xs($| )" "\\2" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# このファイルは watcom/lib/ にあるので、リポジトリのルートは 2 つ上。
get_filename_component(CCWRAP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

set(LLIBCXX03_SRCS
    "${CCWRAP_ROOT}/detail/llibcxx03/src/locale.cpp"
    "${CCWRAP_ROOT}/detail/llibcxx03/src/filesystem.cpp")

# thread.cpp is the one that cannot follow: DOS has no threads at all, and <thread>
# #errors on a non-Win32 Watcom target. filesystem.cpp does build for DOS -- it carries
# both OS layers (see the _CCW_OS_WIN / _CCW_OS_DOS branches there).
if(NOT CMAKE_SYSTEM_NAME STREQUAL "DOS")
    list(APPEND LLIBCXX03_SRCS "${CCWRAP_ROOT}/detail/llibcxx03/src/thread.cpp")
endif()

# wcl386 mis-parses a leading '/' inside -fi=, so hand it native (backslash) paths.
file(TO_NATIVE_PATH "${CCWRAP_ROOT}/watcom/std/ccwrap_header.h"    FI_STD)
file(TO_NATIVE_PATH "${CCWRAP_ROOT}/watcom/ccwstd/ccwrap_header.h" FI_CCWSTD)

# Debug は lib/debug/ の下へ。それ以外 (Release / RelWithDebInfo / 未指定) は lib/ 直下。
set(LIB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/debug")
endif()

# ccwrap_lib_variant(<force-include> <include dir> <output subdir base> [RTTI] [FS_CHAR] [NO_EH] [XS])
#
#   **ターゲット名は出力フォルダ名から機械的に決まる**: `ccwrap_` + フォルダ名の `-` を
#   `_` にしたもの。
#
#       win32-std-fs-char-xst   ->  ccwrap_win32_std_fs_char_xst
#
#   なので 1 本だけ作りたいときは、フォルダ名がそのまま target 名になる:
#       cmake --build --preset watcom --target ccwrap_win32_std_xst
#   よく使う組は buildPreset にしてある (watcom-default / watcom-noeh …)。
function(ccwrap_lib_variant fi incdir outdir)
    cmake_parse_arguments(A "RTTI;FS_CHAR;NO_EH;XS" "" "" ${ARGN})
    # The folder name carries the options the objects were built with, because
    # mixing them is silent breakage: -xs / -xr change what the objects expect
    # without changing any mangled name.
    set(opts "-fi=${fi}")
    if(A_RTTI)
        list(APPEND opts -xr)                   # -xr: typeid / dynamic_cast
    endif()
    if(A_NO_EH)
        list(APPEND opts -xd)                   # -xd: no exceptions. Say it, do not rely
                                                # on merely omitting -xs (the platform
                                                # file puts one in the base flags).
    elseif(A_XS)
        list(APPEND opts -xs)                   # -xs: exceptions, table-driven cleanup
    else()
        list(APPEND opts -xst)                  # -xst: exceptions, cleanup as real code
    endif()
    # 接尾辞は「**推奨の既定から何を変えたか**」。例外も RTTI もあるのが C++ の既定で、
    # 例外モデルは A1'' を避ける -xst を採る。よって -xst -xr が無印。
    #   -xst -xr -> (なし)     -xs -xr -> -xrxs      -xst のみ -> -xst
    #   -xr のみ -> -xr        どちらも無し -> -noeh
    if(A_NO_EH AND A_RTTI)
        set(sfx "-xr")
    elseif(A_NO_EH)
        set(sfx "-noeh")
    elseif(NOT A_RTTI)
        set(sfx "-xst")
    elseif(A_XS)
        set(sfx "-xrxs")
    else()
        set(sfx "")
    endif()
    if(A_FS_CHAR)
        set(outdir "${outdir}-fs-char")
    endif()
    set(outdir "${outdir}${sfx}")
    string(REPLACE "-" "_" tgt "ccwrap_${outdir}")

    add_library(${tgt} STATIC ${LLIBCXX03_SRCS})
    target_compile_definitions(${tgt} PRIVATE _CCW_TARGET_CXX=2026)
    if(A_FS_CHAR)
        # filesystem::path::value_type = char, against the target default. The OS
        # layer in filesystem.cpp follows the same macro (narrow CRT instead of _w*).
        target_compile_definitions(${tgt} PRIVATE _CCW_FS_WCHAR=0)
    endif()
    target_compile_options(${tgt} PRIVATE ${opts})
    # watcom/std (または watcom/ccwstd) だけでよい: llibcxx03 の内部 include は相対なので、
    # その置き場を -i に足す必要は無い。
    target_include_directories(${tgt} PRIVATE "${incdir}")
    set_target_properties(${tgt} PROPERTIES
        OUTPUT_NAME libccwcxx PREFIX ""
        ARCHIVE_OUTPUT_DIRECTORY "${LIB_PREFIX}/${outdir}")
endfunction()

# ---------------------------------------------------------------------------
# What gets built. Default: <os>-std (-xst -xr) and <os>-std-noeh. Everything else
# is opt-in; `*-full` turns on the first three, which test/ needs. CCW_LIB_XS is NOT
# part of full: -xs is banned outside the suites (A1''), so `gen xrxs` alone asks
# for it. -xr (RTTI, no exceptions) is spelled in the suffix rule but never built.
# ---------------------------------------------------------------------------
option(CCW_LIB_CCWSTD  "also build the ccwstd-namespace variant (win32 only)" OFF)
option(CCW_LIB_FS_CHAR "also build the filesystem::path::value_type=char variant (win32 only)" OFF)
option(CCW_LIB_XST     "also build the -xst variant (-xst only, no RTTI)" OFF)
option(CCW_LIB_XS      "also build the -xrxs variant (-xs; test only, see A1'')" OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "DOS")
    # One namespace only: ccwstd exists to dodge a native std the toolchain already has,
    # and on DOS ccwrap is the whole C++ library either way. DOS の path は元から narrow
    # なので -fs-char も無い。
    set(CCW_NS_LIST "std")
    set(CCW_FS_LIST "")
    set(CCW_OUT_PREFIX "dos32")
else()
    set(CCW_NS_LIST "std")
    if(CCW_LIB_CCWSTD)
        list(APPEND CCW_NS_LIST "ccwstd")
    endif()
    set(CCW_FS_LIST "")
    if(CCW_LIB_FS_CHAR)
        list(APPEND CCW_FS_LIST "FS_CHAR")
    endif()
    set(CCW_OUT_PREFIX "win32")
endif()

foreach(ns IN LISTS CCW_NS_LIST)
    if(ns STREQUAL "ccwstd")
        set(fi "${FI_CCWSTD}")
        set(incdir "${CCWRAP_ROOT}/watcom/ccwstd")
    else()
        set(fi "${FI_STD}")
        set(incdir "${CCWRAP_ROOT}/watcom/std")
    endif()
    # "" = path の既定文字型、"FS_CHAR" = char 固定
    foreach(fs "" ${CCW_FS_LIST})
        ccwrap_lib_variant("${fi}" "${incdir}" "${CCW_OUT_PREFIX}-${ns}" RTTI     ${fs})
        ccwrap_lib_variant("${fi}" "${incdir}" "${CCW_OUT_PREFIX}-${ns}" NO_EH    ${fs})
        if(CCW_LIB_XS)
            ccwrap_lib_variant("${fi}" "${incdir}" "${CCW_OUT_PREFIX}-${ns}" RTTI XS ${fs})
        endif()
        if(CCW_LIB_XST)
            ccwrap_lib_variant("${fi}" "${incdir}" "${CCW_OUT_PREFIX}-${ns}" ${fs})
        endif()
    endforeach()
endforeach()
