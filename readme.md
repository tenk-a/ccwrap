# ccwrap v3

ccwrap  は 古いc/c++コンパイラ用に、c11/c++11 以降の規格の一部を真似てみる
ラッパー・ライブラリ。  
実験物。

コンパイラ・オプションの暗黙 include と include パス指定での標準ライブラリ
パス乗っ取りで実装する。

2026-8 v3: AI (主に Claude Code) 利用で、再構築中。  
llvm libc++ を元に、多くのものを追加。

※ Open Watcom C++ 公式の標準ライブラリは基本的なモノが結構欠けていて、
現状、その代用が主目的になっている。


## 対象 コンパイラ

- Open Watcom C/C++ 2.0(beta) win/dos4g
- VC: MS Visual C/C++ 8～12(2005-2013)系、14.x(2015～2026)系 
- gcc, clang (c++11 対応以降での c++03 モード利用して確認)

※ 以前試していた borland c5.5.1, dmc は etc/ フォルダ下に残している。(が未確認)  
※ boost を用いたお試しは削除。


## 基本言語機能のラップ

コンパイラ独自拡張名 (__alignof等) を標準名 (alignof等) に #define し、
実装可能なら代用品を用意(static_assert等)、機能無くとも大半を誤魔化
せるならば空定義を行う。

- c++用:  
    `alignas`, `alignof`, `char8_t`, `char16_t`, `char32_t`, `constexpr`, `final`,
    `__func__`, `noexcept`, `nullptr`, `override`, `static_assert`, `thread_local`

- c 用:  
    `_Alignas`, `_Alignof`, `_Bool`, `inline`, `_Noreturn`, `_Pragma`, `restrict`,
    `_Static_assert`, `_Thread_local`  
    (※vc では `__restrict` を `restrict` に #define すると問題有で未定義)

コンパイラ別に ccwrap_header.h に実装している。  
コンパイラの暗黙の include を使うことで、これらについては標準ライブラリ・パス
を小細工しなくても試すことができる。

※ 暗黙のinclude 指定は、vc では `-FI`、clang/gcc では `-include`、watcom では `-fi=`


## 標準 c/c++ ヘッダ乗っ取り

システム include パスを追加するコンパイラ・オプション( vc/gcc/clang/watcom で `-I` ) 
を用いることで、本来の標準ライブラリより先に ccwrap の置換ヘッダ・ファイルを
読み込まれるようにし、標準ヘッダを置き換えたり不足の追加を行ったりしている。

c90 コンパイラに c99 ～ c23、c++03 コンパイラに c++11 ～ c++26 の
可能そうなライブラリを追加している。  

当然、言語仕様的に実装不可能(cのcomplexやtgmath, c++ constexpr,auto,ラムダ等)
なものは未実装だが、可変引数 template もどきや enum class もどき、
右辺値参照もどき move、等似せれるものは似せて実装を足している。

一応 modern c++ での機能追加もあるが、基本 c++03 ベースなので modern c++ 用
としては足りてなかったり効率のよい実装になっていないことも多い。

watcom 以外は現状、ヘッダーオンリー。  
（ただ、本来実体ファイルに置くべき関数もinlineで無理やり対応した状態）

どのc/c++規格までのモノを利用可能にするか、を、以下のマクロで指定可能、かもしれない。

```
_CCW_TARGET_C   = 1999 ～ 2023
_CCW_TARGET_CXX = 2011 ～ 2026
```

デフォルトは _CCW_TARGET_C=2023、_CCW_TARGET_CXX=2026。


## Visual C/C++ 

vc 8.0(2005) - vc 14.5(2026) 用。

vc に関しては、vc付属の c/c++ 標準ライブラリに対する抜けの追加のみにしている。  
既存の class 等を乗っ取らないので、新しい規格の追加メンバーの類は増やせない。

※ vc 用にライブラリ乗っ取りをやるならば、MS STL をベースにするのがベターだとは思う…


### install / コンパイラ・コマンドライン指定

ccwrap を clone して手頃なフォルダに設置したとする。

仮に環境変数 CCWRAP_ROOT に ccwrap ディレクトリ・フルパスが設定済みとして。

ccwrap_header.h のみ利用の場合。
```batch
cl -FI%CCWAP_ROOT%/vc/std/ccwrap_header.h hello.c
```

c/c++標準ライブラリ追加を利用。
```batch
cl -I%CCWAP_ROOT%/vc/std -FIccwrap_header.h hello.c
```


## gcc clang

gcc や clang は 正確な c++03 として、実装確認用に用意。  
既存 c++03 ライブラリに足りていないものを補う構成。  

msys2(mingw) と wsl(ubuntu) でお試し。


### install / コンパイラ・コマンドライン指定

仮に環境変数 CCWRAP_ROOT に ccwrap ディレクトリ・フルパスが設定済みとして。

ccwrap_header.h のみ利用。
```batch
gcc -include %CCWAP_ROOT%/vc/std/ccwrap_header.h hello.c
```

c/c++標準ライブラリ追加を利用。
```batch
gcc -I %CCWAP_ROOT%/vc/std -include ccwrap_header.h hello.c
```

※ clang のときは gcc をclang に置き換え。


## Open Watcom C/C++

Open Watcom v2.0(beta) 系を対象。 新し目のものを使っている。(2026-7頃の Releaseを仕様)

c90(c99)/c++03 コンパイラで、c 標準ライブラリについては不足を補充、
c++ 標準ライブラリの c++専用ヘッダについては watcom 付属のものは使わず、
LLVM libc++ 改造のものを使うように置き換えている。

公式 c++ ライブラリとは互換性がないので、既存の Watcom 用 c++ライブラリ(.lib, .dll)
等との併用は不可。  
watcom の DLL ランタイム も不可なので、実質 static ランタイム用。

c言語で _CCW_TARGET_C >= 1999 以上に設定する場合は、
c99 の一部機能(c++ と互換性のある機能) をサポートする -Za99
を指定する必要があるかもしれない。

locale、filesystem、thread は実装の都合、ヘッダオンリーというわけにもいかず、
ライブラリのリンクが必要になる。  
locale は他ライブラリで下請け的に利用されることもあり、
結局 c++ では、デフォルトでライブラリをリンクする #pragma library 指定をしている。


### Watcom C++ のバグ

Watcom の C++ はコンパイラ側バグが結構あり、
特に template まわりは、使えない SFINAE 技が多い状態。  
(でも SFINAE に関しては、他の同世代のc++03 コンパイラとしてはまだマシなほうかもしれない)

また、例外処理生成にバグを抱えていて、例外利用の -xs や -xss を指定するとかなりバグを踏みやすい。

ただ -xst という 時間優先 指定にすると、頻出したハングはおきなくる。  
ので例外を使う場合は -xs(-xss) でなく -xst を指定のこと。

例外を使わないプログラムの場合は、-xd で例外無しで運用するほうがよいかもしれない。

例外を使わない設定では、例外発生タイミングで abort/terminate することで、標準ライブラリを使えるようにしている。


### install / コンパイラ・コマンドライン指定

仮に環境変数 CCWRAP_ROOT に ccwrap ディレクトリ・フルパスが設定済みとして。

`・`ccwrap_header.h のみ利用。
```batch
wcl386  -fi=%CCWAP_ROOT%/watcom/std/ccwrap_header.h  hello.c
```

`・`c 標準ライブラリを利用。
```batch
wcl386  -i=%CCWAP_ROOT%/watcom/std  -fi=%CCWAP_ROOT%/watcom/std/ccwrap_header.h  hello.c
```

-fi=フルパス。  

すべてのソースが標準ヘッダ/watcom ヘッダの何れかをincludeしているなら -fi=ccwrap_header.h は無くても可。

以後の例では省略する。（が、付けたほうが安心）

`・`c++ の場合は予め  
```batch
 %CCWAP_ROOT%/watcom/lib/gen.bat
```
を実行して ライブラリ .lib を生成しておく。

`・`c++ 標準ライブラリ 例外&RTTI 有、で、とりあえずビルド
```batch
wcl386 -bt=nt -l=nt -bm -xr -xst -i=%CCWAP_ROOT%/watcom/std hello.cpp %CCWAP_ROOT%/watcom/lib/nt/ccw-eh3r.lib
```
win32（-bt=nt -l=nt）、multithread用（-bm）、RTTI有（-xr）、例外有（-xst）

`・`最適化をして、未使用ルーチン削除等する指定は

```batch
wcl386 -bt=nt -l=nt -bm -xr -xst -ot -xm -xv -DNDEBUG -i=%CCWAP_ROOT%/watcom/std hello.cpp -"LIBPATH %CCWAP_ROOT%/watcom/lib/nt option eliminate option vfremoval"
```

Releaseビルド（NDEBUG）、時間優先最適化（-ot）、未使用ルーチン削除されやすく（-xm -xv）、未使用ルーチン削除（-"option eliminate option vfremoval"）  
ついでに ライブラリ指定を、ライブラリ検索パス指定（-"LIBPATH %CCWAP_ROOT%/watcom/lib/nt"） に変更。（ライブラリ名自体は ccwrap ヘッダ内で #pragma library で指定済）

※最適化オプションはお好みで -ot でなく -os にするなり -ox 追加するなり。
最適化は、必ずよくなる、というわけでもないので、アプリごとに合ったものを選ぶことになる。

`・`あるいは環境変数を用い
```batch
set "INCLUDE=%CCWAP_ROOT%/watcom/std;%INCLUDE%"
set "LIB=%CCWAP_ROOT%/watcom/lib/nt;%LIB%"
wcl386 -bt=nt -l=nt -bm -xr -xst -ot -xm -xv -DNDEBUG hello.cpp -"option eliminate option vfremoval"
```

`・`c++ 標準ライブラリ 例外&RTTI 有 でのデバッグビルドは
```batch
wcl386 -bt=nt -l=nt -bm -xr -xst -d2 -i=%CCWAP_ROOT%/watcom/std hello.cpp -"LIBPATH %CCWAP_ROOT%/watcom/lib/debug/nt"
```

オプション -d2 か -d1 を指定。-d2 は稀にビルド失敗するので、そういうときは諦めて -d1 を指定。


`・`c++ 標準ライブラリ で 例外&RTTI 無にするには、
```batch
wcl386 -bt=nt -l=nt -bm -xd -ot -xm -xv -DNDEBUG -i=%CCWAP_ROOT%/watcom/std hello.cpp -"LIBPATH %CCWAP_ROOT%/watcom/lib/nt option eliminate option vfremoval"
```
オプション -xr -xst(-xs,-xss) を外して -xd に変更


`・`dos4g で 例外&RTTI 無 は
```batch:dos4g
wcl386 -bt=dos -l=dos4g -xd -ot -xm -xv -DNDEBUG -i=%CCWAP_ROOT%/watcom/std -fe=helloD32.exe hello.cpp -"LIBPATH %CCWAP_ROOT%/watcom/lib/dos option eliminate option vfremoval"
```
オプション -bt -l を -bt=dos -l=dos4g に変更。（dos はシングルスレッド環境で -bm 無）

`・`dos4g LFN(Long File Name)有 で 例外&RTTI 有
```batch:dos4g
wcl386 -bt=dos -l=dos4g -xr -xst -ot -xm -xv -D__WATCOM_LFN__ -DNDEBUG -i=%CCWAP_ROOT%/watcom/std -fe=helloLFN.exe hello.cpp -"LIBPATH %CCWAP_ROOT%/watcom/lib/dos option eliminate option vfremoval"
```

マクロ `__WATCOM_LFN__` を定義してビルドすれば watcom の lfn用ライブラリがリンクされる。





### watcom ライブラリ

%CCWAP_ROOT%/watcom/lib/gen.bat

を実行して、ライブラリを生成する。

ライブラリは、WATCOM 付属ライブラリに似せて、ターゲット OS をフォルダ名にし、他はファイル名に含めている。

| ファイル               | 内容                              | wcl386 オプション                           |
|------------------------|-----------------------------------|---------------------------------------------|
| nt/ccw-eh3r.lib        | win32 例外&rtti 有                | -bt=nt -l=nt -bm -xst -xr                   |
| nt/ccw-ne3r.lib        | win32 例外&rtti 無                | -bt=nt -l=nt -bm -xd                        |
| dos/ccw-eh3r.lib       | dos32 例外&rtti 有                | -bt=dos -l=dos4g -xst -xr                   |
| dos/ccw-ne3r.lib       | dos32 例外&rtti 無                | -bt=dos -l=dos4g -xd                        |
| dos/ccw-lfn-eh3r.lib   | dos32 LFN対応 例外&rtti 有        | -bt=dos -l=dos4g -D__WATCOM_LFN__ -xst -xr  |
| dos/ccw-lfn-ne3r.lib   | dos32 LFN対応 例外&rtti 無        | -bt=dos -l=dos4g -D__WATCOM_LFN__ -xd       |
| debug/nt/ccw-…….lib  | 上の debug 版                     | release版から NDEBUG を外し -d1 を足す      |

3r を 3s にした -3s オプション版も同時に生成。  

その他ビルド共通オプションは、  
コンパイラ： `-3r` or `-3s` `-DNDEBUG` `-ot` `-zm` `-zv`
リンカー　： -"option eliminate   option vfremoval"

wcl386 のデフォルトは 3r なので、無指定時は 3r のものがリンクされる。

`出力名`
| 要素    |                                                    |
|---------|----------------------------------------------------|
|フォルダ |                                                    |
| `nt/`   | Win32(-bt=nt -l=nt)                                |
| `dos/`  | Dos32(-bt=dos -l=dos4g)                            |
| `debug/`| debugビルド                                        |
|         |                                                    |
| ファイル| 基本                                               |
| `ccw`   | 基本名                                             |
| `-lfn`  | Dos  LFN(Long File Name) 対応時                    |
| `-eh`   | 例外＆RTTI 有効(-xst -xr)                          |
| `-ne`   | 例外＆RTTI 無効(-xd)                               |
| `3r`    | WATCOM ABI 関数引数がレジスタ渡し ※ デフォルト.   |
| `3s`    | 関数引数がスタック渡し                             |
|         |                                                    |
| 追加    | gen full 等で生成                                  |
| `-fsc`  | Win32 で filesystem を char ベースで構築           |
| `-xr`   | 例外無 RTTI 有効(-xr)                              |
| `-xst`  | 例外有(-xst) RTTI無                                |
|`_ccwstd`| std の代わりに namespace _ccwstd を使う場合        |
|         |                                                    |
| テスト用| -xs(-xss) はバグ有で通常使わないが テスト用に生成  |
| `-xrxs` | 例外＆RTTI 有効 (バグ持)                           |
| `-xs`   | 例外有 RTTI無 (バグ持)                             |


## ccwrap 固有関係

ccwrap 関係の名前は

- プレフィックスに `_ccw_` `_CCW_` `__ccw_` `__CCW_` `ccw_` `CCW_` をつけるか
- namespace `_ccw` `__ccw` `ccw` に格納

している.

- "_" の名前は ユーザー側が使うかもしれないもの
- "__" で始まる名前は ほぼccwrapライブラリ内だけで使うもの
- '_' がつかない ccw CCW は、標準ライブラリ外の ccwrap のライブラリ(予定)


## test

test/ フォルダは主に ccwrap のテスト用。

だいたい、標準ヘッダファイル単位でのテストになっている。

ただ test 自体のチェック用として、ccwrap を使わずコンパイラ付属の
c/c++ ライブラリのチェックも行えるようにしている。  
※ gcc, clang, vc の最新で test がある程度通ることを確認。

未知のコンパイラの自動テスト用にはなっていないので、新規コンパイラを対象にしたときは、調整が必要。
(そのコンパイラのライブラリがもっていない関数がテストにあると、当然ビルドが通らない)

どの言語仕様までのライブラリをチェックするかは

```
TEST_TARGET_C     1990 - 2023
TEST_TARGET_CXX   2003 - 2026
```

で指定できる(はず)。  
指定がなければ、`__STC_VERSION__` や `__cplusplus` の値から決定。  
あるいは ccwrap を使っているなら `_CCW_TARGEAT_???` の値を採用。

- test_c/     c標準ライビラリのテスト
- test_cxx/   c++標準ライブラリのテスト
- etc_test/   コンパイラ固有等その他のテスト


test の実行は、 test/ または test/c_test/、 test/cxx_test/ フォルダで、

```sh
cmake --preset <preset>
cmake --build --preset <preset>
ctest --preset <preset>
```

を実行、結果は test/result_* フォルダ。

preset は CMakePresets.json を直にみるか、  
cmake --list-presets  
で確認のこと。

vc は自動でコンパイラ選択になるが、watcom は ここに書いたPATH(か環境変数WATCOM)を使うので、環境似合わせて修正。

※ test/ 直下の cmakeは、vc gcc clang watcom 用で、c_test, cxx_test, etc_test のテストを纏めて実行する用。

その他個別指定
```
cmake --preset vc145 -DTEST_TARGET_CXX=             :: cwrap を使わない指定
cmake --preset vc120-x86 -DTST_ONLY=locale;string   :: ファイルを絞る
cmake --preset vc145 -DTST_SUITES=c_test            :: スイートを絞る
cmake --preset vc141-x86 -DTST_MSVC_STD=c++14       :: 言語水準
```

##  License

無保証。  

- Apache-2.0 WITH LLVM-exception
- Boost Software License Version 1.0

ソースよって違うがバイナリ配布時に言及不要/してもしなくてもよいライセンスのはず。  
LLVM libc++ ベースが多いので そのライセンスで、boost由来や己の書いたモノは boost。  

ただ、AI おまかせソースなので、どの程度安心かは不明。  


##  おわり

AI 任せ。  
AI がんばる。  

作ってみただけのまだまだバギー。  
(locale 関係いろいろ誤。)
