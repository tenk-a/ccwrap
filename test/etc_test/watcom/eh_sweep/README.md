# eh_sweep — 全ヘッダを「例外あり / なし」の両方でコンパイルする

`watcom/std` と `watcom/ccwstd` のヘッダを **1 本ずつ独立した翻訳単位**にして、
`-xs` 有りと無しの 2 回コンパイルする。

```cpp
#include <foo>
int main(void) { return 0; }
```

判定は**差**だけを見る。

| | 意味 |
| --- | --- |
| 両方通る | 正常 |
| **-xs 有りは通るのに無しだと落ちる** | **失敗** (例外無効ビルドの欠陥)。終了コード 1 |
| 両方落ちる | 例外と無関係。報告はするが失敗にしない |

現状、両方落ちるのは `<format>` と `<print>` の 2 本だけで、どちらも
「まだ実装していない」という `#error` スタブ。

## なぜヘッダ別スイートと別に要るのか

**翻訳単位が最初に読む標準ヘッダが何か**を変えられるのがここだけだから。
`test/cxx_test` は必ず `test_cxx.hpp` から入るので、
「`<stdio.h>` を最初に include したときだけ壊れる」形の事故は**構造的に踏めない**。
`test/etc_test/libcxx_lit/README.md` が挙げているのと同じ理由。

実際にこの形で 2 件見つかっている:

- `detail/cxx/eh_fail.hpp` が C/C++ 共用のプリリュードから `<stdio.h>` を引いて
  **循環**していた (`assert.h` / `stdio.h` / `stdlib.h` / `cstdio` / `cstdlib` の 5 本が
  例外無効時だけ落ちた)。
- 同じ include が `abs(long)` と `abs(long long)` の**宣言順を入れ替え**、
  `std::abs(-5L)` が曖昧 (E227) になった。

## 使い方

`ctest --preset watcom` に `eh_sweep` として登録済み。単体で叩くならリポジトリの
ルートから:

```sh
python3 test/etc_test/watcom/eh_sweep/eh_sweep.py
python3 test/etc_test/watcom/eh_sweep/eh_sweep.py --ns std
python3 test/etc_test/watcom/eh_sweep/eh_sweep.py --only vector,string
python3 test/etc_test/watcom/eh_sweep/eh_sweep.py -v        # 通ったヘッダも並べる
python3 test/etc_test/watcom/eh_sweep/eh_sweep.py --watcom "C:\WATCOM2.0.pre"
```

144 ヘッダ × 2 名前空間 × 2 モードで約 580 コンパイル。実測で 2 分弱。

## 関連

- 例外無効ビルドで**テストが通るか**は `cmake --preset watcom-noeh`
  (`test/cxx_test`)。こちらは「動くか」、eh_sweep は「そもそも通るか」を見る。
- 例外無効での `_CCW_THROW` の落ち方は `detail/cxx/eh_fail.hpp`。
- テスト側の書き方 (`TEST_HAS_EH`) は `doc/src/test/src/test.hpp.md`。
