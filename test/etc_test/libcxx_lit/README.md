# libcxx_lit — libc++ の適合性テストを ccwrap に当てる

libc++ のテストスイートは **libc++ 以外の標準ライブラリ**に対しても回せる
(`test/configs/stdlib-libstdc++.cfg.in` / `stdlib-native.cfg.in` がそれ)。
ここはその仕組みを使って、**コンパイラ自身のライブラリ + ccwrap** を測る。

## テストツリー

**`detail/llibcxx03/{test,utils}`**。libc++ からのコピー:

| | 元 | 中身 |
| --- | --- | --- |
| `test/` | libc++ の `test` | 64MB / 約 10,100 ファイル |
| `utils/` | libc++ の `utils` の一部 | `libcxx/` (lit の設定が import する `libcxx.test.*`) と `run.py` (lit の既定 executor)。210KB / 23 ファイル |

**上流の `libcxx/{test,utils}` という構成をそのまま保っている。** `test/std` の
`.gen.py` 3 本が `%{libcxx-dir}/utils` を要求するので、`utils/` は `test/` と同じ
階層でなければ動かない。

**libc++ 由来ファイルは原則いじらない。改変は 2 箇所だけ**で、どちらも
`-std=c++03` で lit が設定段階で死ぬのを直すもの (プローブの中の `nullptr` →
`NULL`)。理由は `doc/src/detail/llibcxx03/utils/libcxx/test/dsl.py.md`。

実際に使うのは `test/std` (56MB) と `test/support` のみ。**残りは消さずに置き、
lit の discovery から外してある** (`ccwrap.cfg.py` の `config.excludes`):

- **`libcxx` / `libcxx-03`** — libc++ 自身の内部テスト。ccwrap には無意味。加えて
  `test/libcxx/**/lit.local.cfg` が自分のパスの `libcxx/test/libcxx` を
  `libcxx/test/std` に置換する形で書かれており、`libcxx/` という名前のフォルダの
  外では置換が起きず**自分自身を読み込んで無限再帰する** (`RecursionError` で全滅)。
  14 本直せば済むが、無改変を優先して除外にした。
- `benchmarks` / `selftest` / `extensions` / `configs` / `tools` — ベンチマーク、
  lit の自己テスト、CMake 用の設定雛形。

ccwrap はそれ自体が標準ライブラリではなく**上に載る層**なので、1 回走らせても
意味が薄い — libstdc++ 単体でも libc++ 固有のテストは大量に落ちる。答えになるのは

| | |
| --- | --- |
| `native` | コンパイラ自身のライブラリだけ |
| `ccwrap` | 同じものに `-I <tree>/std -include ccwrap_header.h -D_CCW_TARGET_CXX=<年>` |

の **差分**。`run.py` が両方を回して差だけを出す。

## 使い方

```sh
# lit だけは外から用意する (LLVM のツリーから取る。PyPI の lit は 18 で止まっていて古い)
python3 test/etc_test/libcxx_lit/fetch_lit.py <llvm-ref> test/etc_test/libcxx_lit/_lit

# 差分を見る
python3 test/etc_test/libcxx_lit/run.py --std c++20 --target-cxx 2023 \
        --filter "std/(utilities|strings)/"

# 壊しているものだけ (CI 向け。1 件でもあれば終了コード 1)
python3 test/etc_test/libcxx_lit/run.py --list-broken --filter ...
```

出力はこの 2 つ:

- **`ccwrap breaks N test(s) that pass natively`** … ccwrap の欠陥。**これが本題**
- **`ccwrap fixes N test(s) that fail natively`** … ccwrap の補充が効いている証拠

`_work/{native,ccwrap}.json` に lit の生レポートが残る (失敗したテストの
コンパイルコマンドとエラー全文が入っている)。

## 前提と制約

- **Linux で回す。** libc++ のテストスイートは POSIX 前提。
- **`--filter` を必ず付ける。** `test/std` だけで 1 万近くあり、2 回走らせると長い。
- **libc++ 由来のファイルは触らない。** 走らせない部分は消さず、
  `config.excludes` で discovery から外してある (理由は上の「テストツリー」)。
- **テストツリーと作業ディレクトリは、速いファイルシステムに置く。**
  1 回の実行で**約 1 万ファイルの探索**と**テスト本数ぶんのコンパイル生成物の
  書き込み**が起きるので、そこが遅いと全体がそれで律速になる。
  `--libcxx-dir` (ツリーの複製) と `--work` (生成物) の両方を移せる。
  ccwrap 側のヘッダは移さなくてよい (読む本数が桁違いに少ない)。
- **`_lit/` と `_work/` は .gitignore 済み** (lit は外部のもの、`_work` は生成物)。
- **PyPI の `lit` は 18.1.8 が最後**で、この libc++ ツリー (LLVM 23) の
  `utils/libcxx` が使う API に届かない (`lit_config.dbg` /
  `LitConfig(diagnostic_level=...)`)。`fetch_lit.py` が GitHub の contents API で
  `llvm/utils/lit` だけを取ってくる (34 ファイル、pure Python)。
- **`-Werror` は既定で切ってある** (`--warnings` で戻せる)。ccwrap のヘッダは
  native が出さない警告を出すことがあり、それ自体は欠陥ではないため。
- `--stdlib` は libc++ のテストが XFAIL / UNSUPPORTED を判断するのに使う。
  gcc / clang + libstdc++ なら `libstdc++` (既定)。**ここを既定の `llvm-libc++` の
  ままにすると、libc++ 固有の期待が効いて差分がノイズだらけになる。**

## これで見つかったもの

- **native `<iterator>` を引くフォワーダが libstdc++ の locale 構築と循環する**
  (`doc/c++03_tec.md` §10)。`<complex>` や `<sstream>` を最初に include した
  翻訳単位が丸ごと壊れていた。ccwrap 自身のテストは `test_cxx.hpp` が先に
  `<string>` を読ませるので一度も踏んでいなかった。
- **`gcc/std/type_traits` が `__cpp_lib_is_scoped_enum` を `__cplusplus >= 201103L`
  だけで定義し、`is_scoped_enum` を `std` に直接置いていた**。C++23 の追加
  (P1048R1) なので `_CCW_STD23` に入れるのが規約 (vc 側はそうなっていた)。
