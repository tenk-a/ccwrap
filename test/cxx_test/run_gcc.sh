#!/bin/sh
# Build & run the ccwrap test suite under g++/clang++ (exploratory gcc port,
# companion to vc/std/). The ccwrap backports in detail/cxx/*.hpp are routed through gcc/std/
# and reached under the chosen -std=. Any -std= from c++03 up works: the forwarders
# use the native library where it already provides a feature and fall back to the
# backports (and gap-fills) where it does not.
#
#   Usage: ./run_gcc.sh [CXX] [STD]
#   e.g.   ./run_gcc.sh g++     c++03      (msys2 ucrt64 / WSL Ubuntu)
#          ./run_gcc.sh g++     c++17
#          ./run_gcc.sh clang++ c++20
#
# Each *_test.cpp is probed with a compile-only pass first; the ones that build at
# the given (compiler, -std) are linked into one binary and run. Tests that need a
# feature not available at that language level (or a platform header such as
# <intrin.h> for <atomic>) are skipped and listed.
#
# Verified: msys2 ucrt64 g++ 16.1, WSL Ubuntu 24.04 g++ 13.3 (2026-07).
CXX="${1:-g++}"
STD="${2:-c++03}"
cd "$(dirname "$0")"
INC="-include ../../gcc/std/ccwrap_header.h -I ../../gcc/std -D_CCW_TARGET_CXX=2023"
tmp="${TMPDIR:-/tmp}/ccw_cxx_$$"
mkdir -p "$tmp" || exit 1
trap 'rm -rf "$tmp"' EXIT HUP INT TERM

echo "# $CXX -std=$STD  (_CCW_TARGET_CXX=2023)"
if ! $CXX -std="$STD" $INC -c test_cxx_vc.cpp -o "$tmp/test_cxx_vc.o" 2>"$tmp/ccw.err"; then
    echo "FATAL: harness (test_cxx_vc.cpp) failed to compile:"; cat "$tmp/ccw.err"; exit 1
fi

LINK_EXCLUDE="codecvt_test.cpp"

objs="$tmp/test_cxx_vc.o"; skipped=""; linkskip=""
for f in *_test.cpp; do
    b=$(basename "$f" .cpp)
    case " $LINK_EXCLUDE " in *" $f "*) linkskip="$linkskip ${b%_test}"; continue;; esac
    if $CXX -std="$STD" $INC -c "$f" -o "$tmp/$b.o" 2>/dev/null; then
        objs="$objs $tmp/$b.o"
    else
        skipped="$skipped ${b%_test}"
    fi
done
[ -n "$linkskip" ] && echo "# excluded (compiles but does not link natively):$linkskip"

[ -n "$skipped" ] && echo "# skipped (not available at this level):$skipped"
$CXX -std="$STD" $INC $objs -pthread -latomic -o "$tmp/ccw_gcc_test" || exit 1
cd ../..
if [ -x "$tmp/ccw_gcc_test" ]; then "$tmp/ccw_gcc_test"; else "$tmp/ccw_gcc_test.exe"; fi
exit $?
