#!/usr/bin/env python3
"""test/ の `#if ... #else TEST_SKIP1() ... #endif` で数が合っているかを検査する。

テストは「どの構成でも同じ数のチェックを見ている」ことを前提に読む。`#if` の片側だけ
実行される形で **`#else` 側の TEST_SKIP1() の数が `#if` 側のチェック数と違う**と、
コンパイラや標準バージョンによって合計が動いてしまい、その前提が崩れる。

    #if defined(__cpp_lib_three_way_comparison)
        test_true( (a <=> b) == 0 );
        test_true( (a <=> c) <  0 );
        ...                                  # 5 チェック
    #else
        TEST_SKIP1();                        # 1 個しかない -> 構成で 4 個ずれる
    #endif

規約は .claude/rules/test.md。要点だけ再掲:

- `#else` の TEST_SKIP1() は `#if` 側のチェック数と一致させる。
  数が多ければ `TEST_SKIP_N(N)` (推奨) か
  `for (int __i = 0; __i < N; ++__i) TEST_SKIP1();` でよい
  (このツールはどちらも N として数える)。
- **static_assert はコンパイル時なのでカウントされない**。`#if` 側が static_assert
  だけなら `#else` に TEST_SKIP1() を置いてはいけない (0 個が正しい)。
- ネストした `#if` は**片側しか実行されない**ので、外から見た数は
  「その階層の直接のチェック + 各ネストの片側」。両側を足すと誤検出になる。
- `#else` で case を丸ごと差し替える形 (TEST_CASE_SKIP / 別の TEST_CASE) は
  このツールの対象外 (case 単位の skip として別に集計されるため)。

使い方 (ccwrap/ のルートから。ctest に skip_counts として登録してあるので test/bin/):

    python3 test/bin/check_skip_counts.py            # 既定のテストツリーを検査
    python3 test/bin/check_skip_counts.py -q         # 不一致だけ出す
    python3 test/bin/check_skip_counts.py PATH ...   # ファイル/ディレクトリを指定

不一致があれば終了コード 1。
"""
import argparse
import glob
import io
import os
import re
import sys

DEFAULT_GLOBS = ["test/cxx_test/*.cpp", "test/cxx_test/*.hpp", "test/c_test/*.c"]

# test/src/test.hpp と test/src/c_test.h が提供する実行時チェック。static_assert は入れない
# (コンパイル時なのでフレームワークのカウンタを通らない)。
CHECK = re.compile(
    r"\b(test_true|test_false|test_eq|test_ne|test_lt|test_le|test_gt|test_ge|"
    r"test_lim|test_ptr|test_ptr0|test_str_eq|test_str_ne|test_fail|"
    r"test_no_throw|test_throw|test_throw_any)\s*\(")
SKIP1 = re.compile(r"\bTEST_SKIP1\s*\(\s*\)")
# TEST_SKIP_N(k): a single call that stands in for k TEST_SKIP1()s.
SKIPN = re.compile(r"\bTEST_SKIP_N\s*\(\s*(\d+)\s*\)")
FOR_SKIP = re.compile(r"for\s*\([^;]*;\s*\w+\s*<\s*(\d+)\s*;[^)]*\)\s*TEST_SKIP1")


def _blocks(lines):
    """各 #if/#else/#endif を (行番号, if側テキスト, else側テキスト) で返す。

    ネストは **if 側だけ**を親に畳み込む: 実行されるのは片側だけなので、両側を足すと
    実際より多く数えてしまう。
    """
    out, stack = [], []
    for i, line in enumerate(lines):
        t = line.strip()
        # `#  ifndef` のように # と語の間に空白を置けるので正規化する
        # (これを見落とすとネストを検出できず、両側を足して誤検出になる)
        if t.startswith("#"):
            t = "#" + t[1:].lstrip()
        if t.startswith("#if"):
            stack.append({"ln": i + 1, "a": [], "b": [], "cur": "a"})
        elif (t.startswith("#else") or t.startswith("#elif")) and stack:
            stack[-1]["cur"] = "b"
        elif t.startswith("#endif") and stack:
            f = stack.pop()
            a, b = "\n".join(f["a"]), "\n".join(f["b"])
            if stack:
                stack[-1][stack[-1]["cur"]].append(a)
            out.append((f["ln"], a, b))
        elif stack:
            stack[-1][stack[-1]["cur"]].append(line)
    return out


def check_file(path):
    """[(行番号, if側チェック数, else側skip数)] を返す (不一致のみ)。"""
    try:
        lines = io.open(path, encoding="utf-8").read().split("\n")
    except (IOError, UnicodeDecodeError):
        return []
    bad = []
    for ln, a, b in _blocks(lines):
        n_skip = len(SKIP1.findall(b))
        m = FOR_SKIP.search(b)
        if m:                              # for(...i<N...) TEST_SKIP1 -> N (not the 1 literal)
            n_skip = int(m.group(1))
        n_skip += sum(int(k) for k in SKIPN.findall(b))  # TEST_SKIP_N(k) -> +k
        if n_skip == 0:
            continue                       # skip を置いていない = この検査の対象外
        if CHECK.search(b) or "TEST_CASE_SKIP" in b or "TEST_CASE(" in b:
            continue                       # else 側が実チェック / case 差し替え
        n_check = len(CHECK.findall(a))
        if n_check != n_skip:
            bad.append((ln, n_check, n_skip))
    return bad


def expand(paths):
    out = []
    for p in paths:
        if os.path.isdir(p):
            for dp, _, fn in os.walk(p):
                out += [os.path.join(dp, f) for f in fn
                        if f.endswith((".cpp", ".hpp", ".c", ".h"))]
        elif any(ch in p for ch in "*?"):
            out += glob.glob(p)
        else:
            out.append(p)
    return sorted(set(out))


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("paths", nargs="*", help="検査するファイル / ディレクトリ / glob")
    ap.add_argument("-q", "--quiet", action="store_true", help="不一致だけ出力する")
    args = ap.parse_args()

    files = expand(args.paths) if args.paths else expand(DEFAULT_GLOBS)
    if not files:
        sys.stderr.write("検査対象がありません (ccwrap/ のルートから実行していますか)\n")
        return 2

    total = 0
    for p in files:
        bad = check_file(p)
        if not bad:
            continue
        total += len(bad)
        for ln, nc, ns in bad:
            print("%s:%d: チェック %d 個に対して TEST_SKIP1 が %d 個 (差 %+d)"
                  % (p.replace("\\", "/"), ln, nc, ns, nc - ns))

    if not args.quiet:
        print("---")
        print("%d ファイルを検査、不一致 %d 箇所" % (len(files), total))
        if total:
            print("直し方は .claude/rules/test.md の TEST_SKIP1 の項を参照。")
    return 1 if total else 0


if __name__ == "__main__":
    sys.exit(main())
