#!/bin/sh
# Build & run test/c_test under gcc/clang in WSL, without cmake (not installed there).
# Mirrors test/c_test/CMakeLists.txt: probe each *_test.c, define TST_HAVE_<suite> for the
# ones that compile, and link them with test/src/c_test.c + the committed test_main.c.
#   usage: ./run_gcc.sh [cc] [std]        e.g. ./run_gcc.sh gcc c23
CC="${1:-gcc}"
STD="${2:-c23}"
ROOT=/mnt/d/proj/zatsu/ccwrap
cd "$ROOT/test/c_test" || exit 1
INC="-include $ROOT/gcc/std/ccwrap_header.h -I $ROOT/gcc/std -I $ROOT/test/src"
DEF="-D_CCW_TARGET_C=2023"
tmp="${TMPDIR:-/tmp}/ccw_c_$$"
mkdir -p "$tmp" || exit 1
trap 'rm -rf "$tmp"' EXIT HUP INT TERM
echo "# $CC -std=$STD  (_CCW_TARGET_C=2023)"

have=""
objs=""
skipped=""
for f in *_test.c; do
    n=$(basename "$f" _test.c)
    if $CC -std="$STD" -Wall $INC $DEF -c "$f" -o "$tmp/$n.o" 2>"$tmp/$n.log"; then
        have="$have -DTST_HAVE_$n"
        objs="$objs $tmp/$n.o"
    else
        skipped="$skipped $n"
    fi
done
[ -n "$skipped" ] && echo "# skipped (did not compile):$skipped"

$CC -std="$STD" $INC $DEF $have -c "$ROOT/test/src/c_test.c" -o "$tmp/c_test.o" || exit 1
$CC -std="$STD" $INC $DEF $have -c test_main.c -o "$tmp/main.o" || exit 1
$CC $tmp/main.o $tmp/c_test.o $objs -lm -o "$tmp/ccw_ctest" || exit 1
# From the repo root, so the pass logs land in the canonical test/result_c_test/<tag>
# (the binary resolves that path relative to the working directory).
cd "$ROOT" || exit 1
"$tmp/ccw_ctest"
exit $?
