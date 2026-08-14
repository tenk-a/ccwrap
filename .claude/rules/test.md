---
paths:
  - "test/**"
  - "ccw/test.hpp"
---

# テスト

テストは **ヘッダ別 (per-header)** で、**gcc / vc / watcom が同じソースを共有**する。

- `test/cxx_test/*_test.cpp` … ヘッダ別テスト。`ccw/test.hpp` の
  `TEST_CASE` / `test_eq` フレームワークを使う。
- `test/cxx_test/test_cxx.hpp` … 共通プロローグ。vc/gcc は各 `ccwrap_common.h` を include するが、
  **Watcom は何も include しない**（`-fi=watcom/std/ccwrap_header.h` で強制 include され、
  libcxx03 の `<__config>` が `<ccwrap_common.h>` を引くので自動的に入る）。
- `test/cxx_test/test_cxx_vc.cpp` … 集約 main（`TEST_RUN()`）。
- ヘッダ別でない C/C++ smoke テスト（`watcom_ctest.c` / `vc_c.c` / `vc_cpp.cpp` など）は
  `test/etc_test/` に置く。

## ビルド/実行は **CMake に一元化**されている

スイートは 5 つ。**それぞれ単体で configure できる**（自分の `CMakeLists.txt` +
`CMakePresets.json` を持つ）し、**`test/` の umbrella でまとめて回せる**。

| スイート | 場所 | 中身 |
|---|---|---|
| `c_test` | `test/c_test/` | ヘッダ別 C テスト |
| `cxx_test` | `test/cxx_test/` | ヘッダ別 C++ テスト |
| `compile_fail` | `test/compile_fail/` | ネガティブテスト（**通らないこと**が合格）。ライブラリに機能が無くて成立しないケースは基準ケースを probe して自分で外す（外したら表示する） |
| `etc_vc` | `test/etc_test/vc/` | ヘッダ別でない MSVC スモーク |
| `etc_watcom` | `test/etc_test/watcom/` | ヘッダ別でない Watcom スモーク + `watcom_bugchk/`（バグカタログの再検証。ctest 名 `bugchk`） + `eh_sweep/`（全ヘッダを `-xs` 有/無の両方でコンパイル。ctest 名 `eh_sweep`） |

**ctest には載っていない外部ハーネス**: `test/etc_test/libcxx_lit/` は
**libc++ の適合性テストを ccwrap に当てる**。libc++ が
「libc++ 以外の標準ライブラリ」向けに用意している lit config の仕組みを使い、
**native と ccwrap の 2 回を回して差分**を出す（1 回だけでは libstdc++ 単体の赤が
ノイズになる）。lit を外から用意する必要があるので ctest には入れていない。
使い方は `test/etc_test/libcxx_lit/README.md`。**ccwrap 自身のテストが構造的に
踏めない形**（翻訳単位で最初に読む標準ヘッダが何か）を捕まえられる。

**コンパイラ別の配線（強制 include / include パス / `-xs` / `/FI` / probe）は
`test/bin/ccwrap_test.cmake` に集約**してある。バッチ側でフラグを組み立てない。
**`test/bin/` = ビルドが要るもの**（`ccwrap_test.cmake`、vc 用の `*_with_setcc.bat`、
ctest に登録された `check_skip_counts.py`）。**`test/scripts/` = それ以外の道具**
（`spec_gap.py` / `spec_audit.py` / `test_thin.py` などの調査、`base_prune.py` の
result いじり、`dos_ctest.ps1` / `dosbox_run.ps1` の DOSBox 実行、
`mk_h_*.py` / `move_test_comments.py`）。

```
cd test
cmake --preset watcom && cmake --build --preset watcom && ctest --preset watcom
cmake --preset vc145  && cmake --build --preset vc145  && ctest --preset vc145
cmake --preset gcc    && cmake --build --preset gcc    && ctest --preset gcc
```

お手軽ラッパ（configure→build→test を呼ぶだけ）: `test\run.bat <preset>` /
`test/run.sh <preset>`。1 スイートだけ回したいならそのフォルダで同じ 3 手を打つ。

| preset | 内容 |
|---|---|
| `clang-libcxx-cxx26` | **主環境** (2026-08-01〜)。WSL の clang 21 + libc++、C++26。**ccwrap を通さない**素の libc++ が基準。実測 125 グループ全緑・コンパイル不能 0。**libc++ に無いものは skip になるので、base の更新をこれだけでやらない** |
| `vc145` / `vc143` | VS2026 (MSVC 14.5) / VS2022。`c_test`+`cxx_test`+`etc_vc`。cxx_test 単体には ccwrap を通さない native baseline `vc145-std` もある |
| `gcc` / `clang` (`-latest` は C++23) | Ninja。Linux / macOS / msys2。`c_test`+`cxx_test`。**ビルド先はホスト別** (`_build/Linux-gcc` / `_build/Windows-gcc`) — 同じツリーを WSL と Windows の両方から叩けるように |
| `watcom` | Open Watcom 2.0。`_CCW_TARGET_C/CXX=2026`。static MT ランタイム。全 5 スイート。**`-xst -xr`** = **正式ビルドと同じ構成**で、接尾辞なしのライブラリ `win32-std` と対 |
| `watcom-xrxs` | 上の例外モデルだけ **`-xs`** にしたもの（コンパイラの既定だが A1'' を踏む側）。**通常利用は禁止で、踏み方を測るためだけに在る**。ライブラリ `win32-std-xrxs` と対で、**`gen xrxs`** を先に要求する（`gen full` でも作られない）。結果 tag に `-xrxs` が付く |
| `watcom-ccwstd` | libcxx03 を ccwstd 名前空間へ再配置した環境（`cxx_test` のみ） |
| `watcom-noeh` | **例外も RTTI も無効**（`-xs` も `-xr` も付けない）。`cxx_test` のみ。`-noeh` ライブラリと対。結果 tag に `-noeh` が付く。生の try/catch/throw を持つ case は `#if TEST_HAS_EH` で分かれている（`doc/src/test/src/test.hpp.md`） |
| `watcom-xst` | **RTTI だけ無効**（`-xst` のみ）。`-xst` ライブラリと対で `gen full` が要る。結果 tag に `-xst` が付く |

- `TST_SUITES` で絞れる（`-DTST_SUITES=c_test;compile_fail`）。既定 `auto` は
  そのコンパイラで意味のあるスイート全部。
- `TST_ONLY` は**全スイート共通の knob**。umbrella では「そのスイートに無い名前」は
  読み飛ばすので、スイートをまたいで指定できる。

### `TEST_TARGET_C` / `TEST_TARGET_CXX` = どの規格までをテスト対象にするか

**この knob の意味は「このテスト環境が、どの規格までをテストするか」**であって、
ccwrap 専用のものではない (ccwrap でも同じ値を使う、という関係)。テスト側の
`TEST_TARGET_C` / `TEST_TARGET_CXX` マクロがそのまま同じ名前で対応する。

**値は 4 桁の西暦** (`2003` / `2011` / `2014` / `2017` / `2020` / `2023` / `2026`。
C は `1990` / `1999` / …)。2026-08-07 に C++ 側を 2 桁から揃えた。**2 桁は
受け付けない** — `-DTEST_TARGET_CXX=23` と書くと 23 のまま `_CCW_TARGET_CXX` に渡り、
どのゲートも通らない状態になる (診断は出ない)。

**ただし今の実装は「ccwrap を通すか」も同じ knob で兼ねている。** 分離は今後の課題で、
今は「素のライブラリで C++17 までを対象にする」のような組み合わせを指定できない。

**ヘッダ別スイート (c_test / cxx_test) の既定は「ccwrap を通さない」**。値を渡した
ときだけ `_CCW_TARGET_C` / `_CCW_TARGET_CXX` が定義され、強制 include と ccwrap の
include パスが付く。「ccwrap を使わない」ことを表すマクロは**無い**（否定形のマクロを
テスト側にばら撒かないため）。テスト側で環境を判別したいときは
`#if defined(_CCW_TARGET_C)` のように**肯定形**で書く。

pass ログの tag もそれと一致する — `-ccw<n>` が付くのは ccwrap ビルドだけ:

| tag | 意味 |
|---|---|
| `watcom1300-x86-c1990` | 素の Open Watcom（ccwrap 無し） |
| `watcom1300-x86-c1990-ccw26` | ccwrap 経由（`_CCW_TARGET_C=2026`） |

**tag の `-ccw<n>` だけは 2 桁のまま**。過去の測定結果とフォルダ名を突き合わせられる
ようにするため、`test_env.h` が `% 100` で落としている。

preset は用途どおりの値を持つ（`watcom` = 2026、`vc145`/`gcc` = 2023）ので、通常の
`cmake --preset <p>` は従来どおり ccwrap 経由。素で回したいときは `watcom-std` /
`vc145-std`、あるいは `-DTEST_TARGET_C=` と空を渡す。

**例外**: `etc_test` と `compile_fail` は ccwrap そのもののテストなので、そこだけは
既定で ccwrap を通す（値が空だとテスト内容が成立しない）。

素の Open Watcom C ライブラリのベースライン（実測）: 30 本中 **9 本が
コンパイル不能**（`assert` `errno` `math` `stdalign` `stdbit` `stdckdint` `stddef`
`threads` `uchar`）、残り 21 グループは green。チェック数は ok=2364 / skip=122 で、
ccwrap 経由だと ok=3499 まで増える。**C++ 側は純正ライブラリがハーネスすら満たせない**
（`<string_view>` が無く、`<sstream>` に `std::ostringstream` が無い）ので素の構成は
用意していない。

- **単一 / 一部ファイルだけ**を試すには `-DTST_ONLY=<header>` を渡す（`_test.cpp` を除いた
  ベース名。`;` 区切りで複数可）。probe を飛ばして即 configure されるので速い。
  集約 main と detail/llibcxx03/src は常にリンクされるので 1 ファイルでも通しで走る。
  ```
  cmake --preset watcom -DTST_ONLY=locale
  cmake --preset watcom -DTST_ONLY=vector;string
  ```
  フルに戻すときは `cmake --preset watcom --fresh`。
- 各 `*_test.cpp` は **configure 時に個別 try_compile** され、通らないものは除外して
  `skipped (did not compile): ...` と表示される（bat/sh と同じ挙動）。判定はキャッシュ
  されるので、テストを直したら `cmake --preset <p> --fresh` で再判定する。
  **probe は `CMAKE_BUILD_TYPE` と同じ構成で走る**（`CMAKE_TRY_COMPILE_CONFIGURATION`
  を揃えてある）。try_compile の既定は Debug なので、揃えないと Release ビルドを
  Debug のフラグで判定してしまい、**そのビルドでは通るファイルが黙って消える**
  （実例: Release で `<tuple>` 系 5 本が A19 の `-d2` に当たって外れていた）。
- `ccw_htest` の終了コードは**失敗グループ数**なので ctest が失敗を検知できる。
- **Watcom はプリビルドライブラリを先に作る。`watcom/lib` で `gen`。**
  素の `gen` が作る `<os>-std` (推奨 = `-xst -xr`) と `<os>-std-noeh` が、それぞれ
  preset の `watcom` / `watcom-noeh` の対になる。**`-xrxs` (`-xs -xr`) は `gen` でも
  `gen full` でも作らない** — `-xs` は通常利用禁止 (A1'') でターゲットごと opt-in
  (`CCW_LIB_XS`) にしてあるので、preset `watcom-xrxs` を回すときだけ `gen xrxs` で
  名指しする。**`watcom-ccwstd` / `watcom-fs-char` / `watcom-xst` を回すときだけ `gen full`**
  (それらの variant は opt-in 生成)。
  足りないと configure が `prebuilt ... missing` で止まる。
- **Watcom は static ランタイム必須**（preset で `CMAKE_WATCOM_RUNTIME_LIBRARY=MultiThreaded`
  + `CMP0136 NEW`）。既定の DLL ランタイム (`-br`) だと Watcom 純正 C++ ライブラリ
  (`clbr20.dll`) が実行時にリンクされ、libcxx03 が置き換えるべき `<locale>`/`<iostream>` 等が
  **Watcom 側の実装で満たされてしまい**、テストが libcxx03 でなく純正で通ってしまう。
  static なら exe の DLL 依存は KERNEL32/USER32 のみになり、実行時に WATCOM も要らない。

## 進捗と網羅の判定 = **spec / base**（実装項目一覧の後継）

「何が実装できていて、何が残っているか」は **spec と base の 2 つの tsv** で見る。
以前あった `doc/checklist_watcom/*.tsv`（`impl` / `impl_note` 列を手で埋めるもの）は
**2026-08-05 に `old/doc/` へ退役した**。手書きの状態は腐るので、**テストが実際に
何を出したか**を分子に取る形に置き換わっている。集計スクリプト
`bin/lib_doc_status.py` も checklist を読むので用済みで、`old/bin/` へ移した。

| 役割 | 場所 | 誰が書くか |
| --- | --- | --- |
| **spec** = 規格の項目一覧（分母） | `test/{c_test,cxx_test}/spec/*.tsv` | **編集禁止**。規格と全件照合するときだけ人が更新する |
| **base** = その項目にテストがあるか（分子） | `test/result_{c_test,cxx_test}/base/*.tsv` | `testagg --update-tested` が結果から更新 |
| **結果** = 環境ごとの実測 | `test/result_{c_test,cxx_test}/<tag>/*.tsv` | テスト実行が書く。**手で直さない** |

`gap = spec - base` = 「規格にあるのにテストが 1 つも無い項目」。**これが残作業**で、
`no-test-yet` という状態は保存しない（引き算で出すので腐らない）。

**ただし gap 0 は「粒度の範囲で穴が無い」でしかない。** spec の 1 行が複数の
オーバーロードを兼ねていると、1 つ実装すれば `pass` が付き残りは数えられない
（実例: `cxx17:create_directories` が `error_code` 版 30 個を、
`cxx03:string::replace` が 7 形を隠していた）。どこにどれだけ隠れているかは
`python3 test/scripts/spec_audit.py cxx [--list]`、まとめは
`doc/test_spec_audit.md`。**spec を触るには都度の許可が要る。**

```sh
python3 test/scripts/spec_gap.py cxx                 # 全スイート、gap の小さい順
python3 test/scripts/spec_gap.py cxx barrier cfloat  # 足りない行そのものを出す
python3 test/scripts/spec_gap.py c                   # C 側
```

**2026-08-07 時点で C++ 124 スイート 9472 行 / C 31 スイート 2585 行とも gap 0。**
つまり「規格項目に対する未着手のテスト」はもう無い。次の残作業は gap ではなく
**結果 tsv の `fail`**（下記）になる。

### base の更新

**1 環境だけで更新しない。** その環境に無い機能は skip になるので、少ない側に
引きずられる。主環境 (`clang-libcxx-cxx26`) に加えて `gcc-cxx26` / `vc145` /
`watcom` を回してから、結果ディレクトリをまとめて渡す。

```sh
tools/testagg/build/Release/testagg.exe -o test/result_cxx_test/_summary \
    --spec test/cxx_test/spec --tested test/result_cxx_test/base \
    --update-tested test/result_cxx_test/<tag> [<tag>...]
```

- **`--spec` のパスは `test/<suite>_test/spec`**。古い文書には
  `test/result_cxx_test/spec` と書いてあるが、そこには既に無い。
- `--suggest` は書き込まずに報告だけする（`gap` / `orphan` = spec に無い pass /
  `stale` = base にあるのに結果に出ない pass）。**まずこれで見てから更新する。**
- base は**確度を上げる方向にしか変えない**。行の削除はツールでやらず、人が
  `status` 列に理由を書く。
- **消えた項目の行は `testagg` では落ちない。** テストから無くなっても `tested` /
  `skip` が残り、`gap` はそれを穴と見なさない (2026-08-11 に 190 行あった)。
  更新のあとに **`python3 test/scripts/base_prune.py cxx`** を回し、
  「どのテストソースも出さなくなった行」を確認してから `--apply`。
- **spec は削除禁止。** 不要になった行は消さず、`status` 列に印と理由を書く
  (`spec_gap.py` は status のある行を gap から外す)。印は 2 つ:

  | status | 意味 | `note` に書くこと |
  | --- | --- | --- |
  | `delete` | その綴りは規格に存在しない (別ヘッダの項目 / 版が誤り / 重複 / 作業メモ) | **規格に基づく理由**。「どこに正しい行があるか」まで書く |
  | `rename` | 項目は在るが綴りが変わった | **現行の綴り**と、変えた paper |

  `rename` を付けたら、**新しい綴りの行が spec にあるか必ず確かめる**。無ければ足す
  (足さないと分母から消えて、実装していないのに gap 0 に見える)。
- **`note` は規格の話だけ書く。** 「いつ誰がどのツールで入れたか」は書かない
  (2026-08-11 に 229 行の作業メモを削除した)。

### 結果 tsv の読み方 — **tag に `-ccw<n>` が無いものは素のライブラリ**

`fail` を見つけたとき、**まず tag を見る**。`-ccw<n>` が付いていないものは ccwrap を
通していない測定なので、そこの赤は **native の非準拠を記録したもの**であって ccwrap の
欠陥ではない。ccwrap が直せているかどうかは `-ccw<n>` 側で見ること。

実例: `gcc16-win-x64-cxx20`（素の msys2 gcc16）の `cstring` 5 件は、mingw-w64 が
`__CORRECT_ISO_CPP_STRING_H_PROTO` を定義せず libstdc++ が非 const 版しか補わないため
`std::strchr(const char*)` が `char*` を返す、という mingw 側の非準拠。ccwrap 経由なら
`gcc/std/ccwrap_header.h` の `_CONST_RETURN=const` で解決済み。

## スクリプト（すべて CMake を呼ぶだけの薄いラッパ）

| スクリプト | 何をするか |
|---|---|
| `test\run.bat <preset>` / `test/run.sh <preset>` | umbrella を configure→build→test |
| `test\run_watcom.bat [htest\|win\|cfail\|all]` | 上を `-DTST_SUITES=...` 付きで呼ぶ。**`dos4g`/`dos16` だけは今も wcl386 直叩き**（CMake に DOS 用 Open Watcom ツールチェインが無い。ビルドのみ） |
| `test\cxx_test\vctest.bat [std] [c++NN] [vcNNN] [x86\|x64]` | cxx_test の vc preset を選んで呼ぶ（`std`→`-DTST_STD_ONLY=ON`、`c++NN`→`-DTST_MSVC_STD=`） |
| `test\etc_test\vc\run_vc.bat` / `test\etc_test\watcom\run_watcom.bat` | それぞれの etc スイート |

**例外（CMake を呼ばない）**: `test/c_test/run_gcc.sh` と `test/cxx_test/run_gcc.sh` は
**cmake が入っていない WSL 用のフォールバック**。CMake 版と同じことを手で
（probe→リンク→リポジトリルートで実行）やる。cmake があるなら `test/run.sh` を使う。

### ctest を直接叩くときの落とし穴

- **vc は VS ジェネレータ（マルチ構成）なので `ctest` に `-C Debug` が要る。**
  付けないと全テストが `Not Run` になり、一見「全滅」に見える。preset 経由
  （`ctest --preset vc145`）なら不要。
- **Watcom を Bash から回すときは PATH を自分で通す。** CMake が `wmake` を素の名前で
  呼ぶので、通っていないと `no such file or directory` という無関係な顔のエラーになる。
  ```sh
  export WATCOM=/c/WATCOM; export PATH="/c/WATCOM/binnt64:$PATH"
  ```

### ケースを足す位置 — **`#if` の入れ子を数えてから入れる**

テストファイルの多くは `#if TEST_TARGET_CXX >= NNNN` … `#else`
(`TEST_CASE_SKIP` の列) … `#endif` という形をしている。ファイル末尾に足すと
**`#else` 側に入って、その規格を満たす環境では一度もコンパイルされない**。
`#endif` を後ろから探すのも駄目で、内側の機能マクロ判定の `#endif` に当たる。

2026-08-12 に `flat_set` / `flat_map` で 3 回続けて外した:

1. ファイル末尾に追加 → 内側の `#if defined(__cpp_lib_flat_map)` の中に入り、
   **`TEST_CASE` が関数の中**になって E322 (local class に static メンバは置けない)
2. 深さ 0 に戻る最後の `#endif` の手前 → **`#else` 側**に入り、どの環境でも走らない
   (結果 tsv に名前が出ないので gap で気づいた)
3. 正解は **外側の `#if` から前向きに深さを数え、深さ 1 の `#else` (無ければ
   その `#endif`) の手前**

`grep -n "insert_forms" <file>` で入った行を確かめ、**結果 tsv にその名前が出るか**
まで見ること。コンパイルが通っただけでは「走った」ことにならない。

### 結果 tsv は **ctest が終わってから**読む

pass ログは case ごとに追記されるので、走っている最中に読むと**途中の状態**が返る。
2026-08-12 に、同じ実行の途中で 26 件・0 件・21 件と 3 通りの赤の数を読んで
「回帰した」「消えた」と誤読した。`ctest` の終了を待ってから数えること。

### Watcom: `detail/llibcxx03/` を直したら **ライブラリを作り直す**

テスト側はプリビルドの `watcom/lib/<variant>/libccwcxx.lib` にリンクするので、
`src/*.cpp` に関数を足したら作り直さないと **E2028 (undefined reference)**、
**ヘッダのクラス定義を変えたら** (仮想関数の追加など) レイアウトが食い違って
**実行時に静かに壊れる**。
`watcom/lib` で `gen win32` (bash からは `cmd //c .\gen.bat win32` ではなく
PowerShell から `cmd /c ".\gen.bat win32"`。素の `gen.bat` は PATH に無いと言われる)。

### Watcom: libcxx03 のヘッダを直したら **テスト側も touch する**

CMake の Watcom ジェネレータは **拡張子の無いヘッダ (`detail/llibcxx03/include/vector` など) の
依存を追わない**。ヘッダだけ直して `cmake --build` しても**再コンパイルされず、古い
オブジェクトのまま**リンクされる。「直したのに結果が変わらない」「直っていないのに緑」は
まずこれを疑い、`touch test/cxx_test/<suite>_test.cpp` してから測り直すこと。

**触るのは「直したヘッダを使う全ファイル」**。1 本だけ touch すると、そのファイルだけ
新しいレイアウトで、他は古いままになる。2026-08-12 に `<locale>` へ仮想関数を 3 つ
足したとき、`locale_b_test.cpp` だけ touch して `locale_test.cpp` を忘れ、
**num_get::get の 5 ケースが赤くなった**。実装は正しく、`touch cxx_test/*.cpp` で消えた。
**ヘッダを直したら `touch test/cxx_test/*.cpp`**、迷ったら `--fresh`。

同じ理由で、**`-DTST_ONLY=` を変えた直後の実行結果も信用しない** — 再ビルドに失敗して
いると前回の exe がそのまま走り、別スイートの結果を見て「通った」と誤読する。
`[Result] Run N test-groups` の N と、走ったスイート名を必ず確かめる。

## テストのコメント = **ヘッダ・ソースと同じ扱い**（2026-08-08〜）

**テストソースにコメントは書かない。** 説明は
**`doc/src/test/<ソースと同じ相対パス>.md`** に置く。規約の本体は
`doc/ccwrap_impl_rule.md` の「ソース中のコメント」で、テストだけの特則は無い。

```text
test/cxx_test/vector_test.cpp  ->  doc/src/test/cxx_test/vector_test.cpp.md
test/c_test/stdio_test.c       ->  doc/src/test/c_test/stdio_test.c.md
test/src/test.hpp              ->  doc/src/test/src/test.hpp.md
```

**ソースに残してよいのは 3 つだけ**（ヘッダ・ソースと同じ）:

1. 先頭のライセンス表記・ファイルヘッダ（`@file` / `@author` / `@license`）。**外さない**
2. 1〜2 行の短い説明
3. `#else` / `#elif` / `#endif` の対応マーク（どの条件の裏かを示すもの）

**例外は `test/etc_test/watcom/watcom_bugchk/`**。プローブの先頭は bugchk.py が読む
`bugchk-*:` メタデータで、その周りの散文が**バグ報告そのもの**。しかも bugchk.py は
**先頭 41 行しか見ない**ので、行を減らすだけで下の方にある `bugchk-expect:` の字面が
窓に入り、**verdict が変わる**（C07 で実際に起きた）。ここは触らない。

道具は `test/scripts/move_test_comments.py`（`--report` で確認 → `--apply`）。
**正規表現でなく字句解析**なので文字列リテラル中の `//` を壊さない。さらに
**適用前に「コメントを除いた行の並びが前後で一致すること」を検査**し、一致しない
ファイルは書き換えずに `SKIPPED` と報告する。移した先の `.md` は行番号を持たず、
**注釈対象のコード 1 行**をアンカーにする（行番号は腐るため）。

**テストソースは ASCII だけ**（`doc/ccwrap_impl_rule.md` と同じ理由）。日本語を書きたく
なったら、それは `doc/src/test/` 側に書くという合図。cl は CP932 で読むので、行末が
多バイト文字だと**改行を食って次の行が消える**。

## 大域名前空間の C 標準関数を測る = `STD_NS` + `<xxx>_h_test.cpp`

C++ は `<cxxx>` が `namespace std` に置く名前を **`<xxx.h>` は大域名前空間にも置く**と
定める ([depr.c.headers])。ここが抜けていても `std::` 側のテストは通るので気づけない
(2026-08-12 に利用者から `::abs(float)` が無いと報告された。`std::abs(float)` は動いていた)。

**同じテスト源を 2 回コンパイルして両方を測る**:

1. C ライブラリの名前だけ `STD::foo` → **`STD_NS foo`** に置き換える
   (`STD::is_same` のような C++ 専用の名前は `STD::` のまま。混ぜると `<xxx.h>` 版が
   コンパイルできない)
2. `<xxx>_h_test.cpp` を足す:

```cpp
#include <string.h>
#include "test_cxx.hpp"
#undef  STD_NS
#define STD_NS                    /* 空 -> STD_NS strlen(p) は ::strlen(p) */
#undef  TEST_GROUP_SUFFIX
#define TEST_GROUP_SUFFIX _h      /* スイート名 cstring -> cstring_h */
#include "cstring_test.cpp"
```

**この 2 つは `python3 test/scripts/mk_h_variant.py <cheader> <suite>... --apply` が
機械的にやる** (`--report` で下見)。C の名前かどうかは `test/c_test/spec/<hdr>.tsv`
で判定する。C の一覧に出ないが `<cxxx>` が std に置くもの (`abs`、`lerp`、特殊数学
関数) はツール内の `EXTRA`、逆に `<xxx.h>` が大域に置かないもの (`byte` /
`to_integer`。[depr.c.headers] の `<stddef.h>` 概要はこの 2 つを含まない) は `DENY`。

- **ファイル名はヘッダに合わせ (`string_h_test.cpp`)、スイート名は `c` を残す
  (`cstring_h`)**。一覧が `cstring` の隣に並んで見やすい。
- **スイート名は `TEST_GROUP_SUFFIX` で足す。グループ名そのものを `#define` しない** —
  `ctime` のようにグループ名と同じ綴りの C 関数があると、その呼び出しまで書き換わる。
- `TEST_CASE` は**グループ名を一段展開してから貼る** (`_TeST_CASE_II`)。`##` は引数を
  展開しないので、この一段が無いとクラス名も報告名も元のままで**リンクが多重定義になる**。
- スイートが増えるので `spec/<suite>_h.tsv` が要る。**実際に出た名前から作る**のが
  確実 (手で写すと `<cxxx>` にしか無い項目まで入る) —
  `python3 test/scripts/mk_h_spec.py cxx test/result_cxx_test/<tag>...`。
  `since` / `until` は元スイートの spec から引き継ぐ。
- **`<cassert>` `<cerrno>` `<cfloat>` `<climits>` にはこの版を作らない。** マクロしか
  無く `namespace std` に置く名前が 1 つも無いので、大域版は同じ検査の写しにしかならない
  (`--report` の `STD_NS 0` がその印)。削除された `<ciso646>` `<cstdbool>`
  `<cstdalign>` `<ccomplex>` `<ctgmath>` も同じ。
- **測っているのは「`<xxx.h>` の後で大域から引けるか」まで**で、`<cxxx>` が大域を
  汚していない保証までは取れない (元のテスト源が `<cxxx>` を include するため)。
  MSVC のように両方に出す実装では、その分だけ緩い。

## 薄いチェックを見つける = `test_thin.py`

```sh
python3 test/scripts/test_thin.py cxx           # スイート順位
python3 test/scripts/test_thin.py cxx locale    # 個々の項目
python3 test/scripts/test_thin.py c --fallback  # skip したのに pass している箇所
```

`test_pass("X")` は「X を確かめた」という主張なので、**直前のマークからそこまでに
assert が何個あるか**がその主張の裏付けになる。3 通りに分けて出る:

| | 意味 |
| --- | --- |
| `never` | その項目は**どの箇所でも** assert 0。完全に空の主張 |
| `fallback` | 別の箇所では確かめているのに、この分岐は 0。**`test_skip` にすべき** |
| `one` | 幸せな経路 1 回だけ。失敗経路も境界も通っていない |

**0 と 1 が即悪いわけではない** (定数 1 個の存在確認など)。多い所から見るための道具。

**`never` の多くは「帰属」の問題**で、空とは限らない。1 ブロックのチェックの後ろに
`test_pass` が数個並んでいると、ツールは先頭に全部を帰属させるので 2 個目以降が
0 に見える。それでも**項目ごとに割り付け直す価値はある** — まとめてあると、
どれか 1 つが壊れたとき**別の項目の名前で落ちて**、結果 tsv から原因が読めない。

**本当に危ないのは「`never` かつ、どれかの環境で `pass` になっている」もの。**
全環境で `skip` なら緑の嘘にはならない (実装が無いので通らない)。結果 tsv と
突き合わせて絞ること。2026-08-12 の実測ではスタブ 67 項目を直したあと**0 件**
(残った 6 件は `CHK_PRI(...)` のように**チェックを自前マクロに包んでいる**ための
誤検出だった。ツールはそういう行を「強さの分からないチェック 1 個」として数え、
その group に `?` を付ける)。

### 踏んだ形 2 つ (2026-08-12 に 351 箇所直した)

```cpp
// (1) 実行時の fallback: 確かめていないのに pass を主張する
if (!fp) { TEST_SKIP_N(12); test_pass("c1990:fseek SEEK_SET"); ... return; }
//                          ^^^^^^^^^ test_skip でなければならない

// (2) #endif の後ろに共有された test_pass: #else を通っても緑になる
#if TEST_TARGET_CXX >= 2017
    test_eq( n3, 6 );
#else
    TEST_SKIP_N(2);
#endif
    test_pass("cxx17:for_each_n");   // <- C++17 でなくても pass
```

(2) は**分岐ごとに書き分ける**。`#if` 側の末尾に `test_pass`、`#else` 側の末尾に
`test_skip` を置く。

## **実装の経路ごとに通す** (1 呼び出し 1 チェックにしない)

**規格の 1 項目でも、実装の中で分岐するなら分岐の数だけ確かめる。** 幸せな経路を
1 回通しただけの緑は、**残りの経路が壊れていても緑のまま**で、一覧を見た人には
「その項目は出来ている」と読める。それが一番避けたい状態。

必ず見る経路 (該当するものは全部):

| 種類 | 例 |
| --- | --- |
| **失敗の経路** | 解析失敗で `failbit`、存在しないパスで `error_code` が立つ、既にあるので `false` を返す |
| **境界** | 空の範囲 (`first == last`)、要素 1 個、全体、先頭 / 末尾 / 途中 |
| **モードで分かれるもの** | `basefield` の 10/8/16 進、`copy_options` の各値、`perm_options` の replace/add/remove |
| **増減の向き** | `resize` の伸ばす / 縮める / 変わらない |
| **述語つきの版** | 比較子・述語を渡した版は、既定と**違う結果になる**入力で確かめる |
| **戻り値** | 返る反復子・個数・ノードハンドルの中身。捨てない |

```cpp
// NG: 呼べたことしか確かめていない
fs::create_directory(d2, d);
test_pass("cxx17:create_directory(const path& p, const path& existing_p)");

// OK: 作る経路と「既にある」経路の両方
test_true(  fs::create_directory(d2, d) );   // 作った -> true
test_true(  fs::is_directory(d2) );
test_true( !fs::create_directory(d2, d) );   // 既にある -> false
test_pass("cxx17:create_directory(const path& p, const path& existing_p)");
```

**書き分け (`spec_overload.py`) で足した項目ほど要注意。** 同じ形をした
オーバーロードを機械的に並べると、どれも幸せな経路 1 回で終わりやすい。

## 別のチェックにすり替えない (**skip にする**)

**その環境で通る形に書き直してよいのは「同じ趣旨のまま、規格の範囲を下げた版」だけ。**
チェック項目が要求していることと**別件**になったら、書き直さずに **skip** にする。

```cpp
// 項目は「cxx11:atomic<int> default ctor」= 既定構築で値がゼロか.
#if TEST_TARGET_CXX >= 2020
    int dv = d.load();
    test_eq( dv, 0 );          // 値初期化は C++20 (P0883) から
#else
    TEST_NOTE("value-initialisation by the default ctor is C++20 (P0883)");
    TEST_SKIP1();              // NG: d.store(0) してから読む -- それは store のチェック
#endif
```

- OK: `decltype(x)` → 引数推論 (`tst_type_is<T>(expr)`)、`decltype((tag))` → `&tag` の型、
  `decltype(obj)::type` → 特殊化を名前で書く。**同じ事実を C++03 の綴りで言っている**。
- NG: 別の手段で「動くこと」だけ確かめる (既定構築のゼロを store で置き換える、
  「同じ出力先か」を「書けるか」で置き換える)。**役割が同じでも別のチェック**。
- どの実装も満たさない・規格が要求していないと分かったら、**式は残してノート付きで
  常に skip** する (消すと「そういう問いがあった」ことごと失われる)。

## スキップ (`#if` で checkを外さない)

チェック数は case ごとに `(N = okN + errN + skipN)` と表示される。**環境都合で実施しない
チェックは `#if` で消さず、skip として数える**こと。

- **式自体はコンパイルできる**（結果だけが違う/未対応） → 直前に
  `TEST_SKIP_WAT()` / `TEST_SKIP_VC()` / `TEST_SKIP_GCC()` /
  `TEST_SKIP_PRE11()`..`TEST_SKIP_PRE26()` を置く。直後の1チェックがスキップされる。

  ```cpp
  TEST_SKIP_WAT();                       // Watcom は T[N] を部分特殊化できない
  test_true( std::is_array<int[3]>::value );
  ```

- **書いた時点で文法エラー/型が無い**（`std::is_enum` 自体が存在しない等） →
  `#if` は残し、`#else` に **チェック数ぶんの `TEST_SKIP1()`** を置く。

  ```cpp
  #if !defined(__WATCOMC__)
      test_true( std::is_enum<E>::value );
      test_true( std::is_class<C>::value );
  #else
      TEST_SKIP1(); TEST_SKIP1();
  #endif
  ```

`TEST_SKIP1()` は**カウンタ**（式レベルのskip = `exprSkip`）。後続チェックがあればそれを
1つスキップし、無ければ case 終了時に skip として計上される。

**`#else` 側の `TEST_SKIP1()` は `#if` 側のチェック数と必ず一致させる。** 数が違うと
**構成によって合計チェック数が変わり**、「どの構成でも同じ数を見ている」という前提が崩れる。

```sh
python3 test/bin/check_skip_counts.py     # 不一致があれば列挙して終了コード 1
```

**`ctest` に `skip_counts` として登録済み**なので、`ctest --preset <p>` を回せば一緒に
検査される (cxx_test / c_test の両方)。目視では見つからない — ズレていても全部 green の
ままなので、必ずツールに聞くこと。

- 数が多いときは `TEST_SKIP_N(N)`（推奨）か
  `for (int __i = 0; __i < N; ++__i) TEST_SKIP1();` でよい。
  `TEST_SKIP_N(N)` は `TEST_SKIP1()` を N 個ならべたのと等価で、
  `check_skip_counts.py` も `TEST_SKIP_N(<整数>)` を N として数える。
- **`static_assert` はコンパイル時なのでカウントされない。** `#if` 側が static_assert
  だけなら `#else` に `TEST_SKIP1()` を置いてはいけない（0 個が正しい）。
- **`#else` で case を丸ごと差し替えるときは case 名も同じにする。** 名前が変わると
  構成によって別 case として並び、比較できなくなる。
- ネストした `#if` は**実行されるのは片側だけ**なので、外側から見た数は
  「外側の直接のチェック + 各ネストの片側」。両側を足さないこと。

- **case まるごと**（ヘッダ自体が無い、型が無くて本文が一切コンパイルできない） →
  `#else` 側に **`TEST_CASE_SKIP(group, name)`**（本体・セミコロン不要）。
  case は `skp ... (case skipped)` として一覧に残り、group 集計に `N skipped.` が付く
  （success でも failure でもない）。`#if` ごと消すと**存在しなかったこと**になるので不可。

  ```cpp
  #if defined(__WATCOMC__)
  TEST_CASE_SKIP(memory, pointer_cast)
  #else
  TEST_CASE(memory, pointer_cast) { ... }
  #endif
  ```

## 標準バージョンごとにケースを分ける (C++03 から C++26 まで同じスイートを回すため)

テストは **C++03 のコンパイラでも C++26 のコンパイラでも同じソースを回す**。ある項目が
その環境に無いとき、**ファイルごとコンパイル不能にしない**のが原則。判断は次の順:

1. **C++03 で同じ保証を確かめられるか。** 確かめられるなら **skip せず、その場で分岐して
   両方走らせる**。例: `vector::data()` は C++11 のメンバだが、「連続配置で先頭要素から
   始まる」という保証自体は `&v[0]` で検証できる。
   ```cpp
   #if _TST_HAS_CXX11_LIB_MEMBERS
       test_eq( *v.data(), 10 );
       test_pass("cxx11:vector::data");
   #else
       test_eq( *(&v[0]), 10 );      // 同じ保証を C++03 の綴りで確かめる
       test_skip("cxx11:vector::data");
   #endif
   ```
2. **メンバの存在そのものが問い**なら、**そのバージョン専用のケースに切り出す**。
   ケース名は元の名前 + `_cxx11` / `_cxx14` / `_cxx17` / `_cxx20` / `_cxx23`。
   ```cpp
   // cbegin/cend は C++11 のメンバ。native のクラスに後から足せない
   #if _TST_HAS_CXX11_LIB_MEMBERS
   TEST_CASE(vector, iterators_cxx11) { ... }
   #else
   TEST_CASE_SKIP(vector, iterators_cxx11)
   #endif
   ```
   **1 ケース = 1 標準バージョン**が原則。混在していると、片方が無いだけでケース全体が
   落ちる。

### どのゲートを使うか

| 欠けているもの | ゲート | 補充できるか |
| --- | --- | --- |
| 言語機能 (rvalue 参照 / decltype / 可変長テンプレート / スコープ enum) | `_TST_HAS_RVALUE_REF` `_TST_HAS_DECLTYPE` `_TST_HAS_ALIAS_TEMPLATE` `_TST_HAS_SCOPED_ENUM` `_TST_HAS_STATIC_ASSERT1` | **不可** |
| native クラスの**メンバ** (`vector::data` `set::emplace` `numeric_limits::lowest` `type_info::hash_code`) | `_TST_HAS_CXX11_LIB_MEMBERS` | **不可** (所有していないクラスに足せない) |
| **自由関数・型・トレイト** (`std::erase` `std::invoke` `std::to_address`) | `TEST_TARGET_CXX >= YYYY` | **可** — テストを gate する前に ccwrap 側で補充する |

**3 行目が肝心**: 自由関数なら足せるので、gate する前に補充を検討する。実例として
`std::erase` / `erase_if` は `vc/vector` が補い、`operator<<(ostream&, nullptr_t)` は
`vc/ostream` が補っている。逆に上 2 つは原理的に無理なので、素直に分岐する。

**`_ccw_cplusplus` や `TEST_TARGET_CXX` を言語機能の判定に使わない。** 前者はヘッダの
gate とテストの両方から読まれるので、上げると別の副作用が出る (vc120 で実測: 88 → 77)。
後者は「ccwrap がどの規格のライブラリを提供すべきか」であって、コンパイラが何を解釈
できるかではない。

## 実装内部に依存するテスト

テストは3コンパイラ共有なので、**標準に無いものを本体ケースで使わない**。
libcxx03 の内部フック (`set::__tree_rb_valid()` / `__tree_height()` など) を使う
チェックは、**専用の case に切り出して**そのコンパイラだけで動かす。

```cpp
#if defined(__WATCOMC__)
TEST_CASE(container, libcxx03_tree_invariants) { ... s.__tree_rb_valid() ... }
#else
TEST_CASE_SKIP(container, libcxx03_tree_invariants)
#endif
```

本体ケース側には**規格で保証される観測可能な振る舞い**だけを残す
（例: 整列している / erase の戻り値 / find の結果）。

同様に、**実装依存の値を等値で断定しない**。実際に踏んだもの:

- `codecvt_utf8::max_length()` … libcxx03 は 4、MSVC は 6 → `test_lim(x, 4, 6)`
- `readsome()` 後の eofbit … `showmanyc()` が末尾で -1 を返すか 0 かは実装依存
- `path::c_str()` … MSVC では `wchar_t*` → `p.string().c_str()` を使う
- `pointer_traits::rebind` … C++11 は alias template、C++03 代替は `::other`
- `slice_array` / `mask_array` の複合代入 … 規格は valarray のみ (スカラーは拡張)

## 落とし穴

**`test_eq` 系は 1 回評価 (2026-08-03 以降)。** 以前は左右を 2 回ずつ展開していたので
副作用のある式（乱数生成器の `g()`、`container.erase()` の戻り値）を直接渡せなかったが、
`__TesT_cc_lr<T,U>` が両辺を参照で束ねて比較まで行うようになったので、そのまま書ける
（`test_str_eq` も各辺 1 回）。既存の「いったん変数に受ける」書き方はそのままで無害。

**この holder は両辺を `const&` で受ける**ので、`operator==` を**非 const メンバ**として
持つ型は比較できない（`E531` / C2678 になる）。標準型では起きない。出たらその型の
const 修飾漏れ。

C 側の `test/src/c_test.h` は `test_true((l) == (r))` なので元から 1 回評価
（値を表示しないため）。

`run_watcom.bat` は**コンパイルに失敗したファイルを failure ではなく黙って落とす**
（`# skipped (did not compile): ...` の行に列挙されるだけ）。`[Result]` だけ見ると
壊れたヘッダが緑に見えるので、`skipped` の行も必ず確認すること。

# テスト終了後
test の 上階層に *.err *.obj が残っていることがあるので削除すること
