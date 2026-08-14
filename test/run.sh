#!/bin/sh
# ccwrap テストの お手軽ラッパ (Linux / macOS / msys2)。configure -> build -> test を
# 順に呼ぶだけ。中身 (コンパイラ別のフラグ / include / 強制 include / probe) は全部
# CMake 側にある。
#
#   Usage: ./run.sh [preset] [extra cmake args...]
#
#     preset : gcc (既定) / gcc-latest / clang / clang-latest ...
#              一覧は  cmake --list-presets
#
#   例:
#     ./run.sh                            g++ -std=c++03 で c_test + cxx_test
#     ./run.sh gcc-latest                 g++ -std=c++23
#     ./run.sh clang --fresh              キャッシュを捨てて (probe をやり直して)
#     ./run.sh gcc -DTST_ONLY=string      1 ファイルだけ (スイートをまたいで指定可)
#     ./run.sh gcc -DTST_SUITES=c_test    スイートを絞る
#
# 1 スイートだけを単体で回したいなら、そのフォルダにも同じ 3 手が用意してある:
#     cd c_test && cmake --preset gcc && cmake --build --preset gcc && ctest --preset gcc
set -e
cd "$(dirname "$0")"

preset="${1:-gcc}"
# set -e があるので `[ ... ] && shift` は書けない (引数ゼロのとき終了ステータス 1 で落ちる)
if [ $# -gt 0 ]; then shift; fi

cmake --preset "$preset" "$@"
cmake --build --preset "$preset"
ctest --preset "$preset"
