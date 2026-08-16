# ccwrap v3

ccwrap  は 古いc/c++コンパイラ用に、c11/c++11 以降の規格の一部を真似てみる
ラッパー・ライブラリ。  
実験物。

コンパイラ・オプションの暗黙 include と include パス指定での標準ライブラリ
パス乗っ取りで実装する。

2026-8 v3: AI (主に Claude Code) 利用で、再構築中。  
llvm libc++ を元に、多くのものを追加。

※ Open Watcom C++ 公式の標準ライブラリは基本的なモノが結構欠けていて、
その代用が目的の一つになっている。


## 対象 コンパイラ

- Open Watcom C/C++ 2.0(beta) win/dos4g
- VC: MS Visual C/C++ 8～12(2005-2013)系、14.x(2015～2026)系 
- gcc, clang (c++11 対応以降での c++03 モード利用して確認)

※ 以前試していた borland c5.5.1, dmc は etc/ フォルダ下に残している。(が未確認)  
※ v1,v2の boost を用いたお試しは削除。


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

システム include パスを追加するコンパイラ・オプション( vc/clang/gcc では `-I` ) 
を用いることで、本来の標準ライブラリより先に ccwrap の置換ヘッダ・ファイルを
読み込まれるようにし、標準ヘッダを置き換えたり不足の追加を行ったりしている。

c++03 コンパイラに c++11 ～ c++26 の可能そうなライブラリを追加している。  

当然、言語仕様的に実装不可能(cのcomplexやtgmath, c++ constexpr,auto,ラムダ等)
なものは未実装だが、可変引数 template もどきや enum class もどき、
右辺値参照もどき move、等似せれるものは似せて実装を足している。
(format,print 等未実装)

どのc/c++規格までのモノを利用可能にするか、を、以下で設定可能。

```
_CCW_TARGET_C   = 1990 - 2023
_CCW_TARGET_CXX = 2003 - 2026
```


## Visual C/C++ 

vc 8.0(2005) - vc 14.5(2026) 用。

vc に関しては、vc付属の c/c++ 標準ライブラリに対する抜けの追加のみにしている。  
既存の class 等を乗っ取らないので、新しい規格の追加メンバーの類は増やせない。

vc では _CCW_TARGET_C = 2011, _CCW_TARGET_CXX = 2014 をデフォルトにしている。  
vc12以前を ccwrap で vc14.x 系まで補充、vc14以降では ccwrap を include 
しないような運用を想定。

_CCW_TARGET_CXX = 2017 にして、c++17 未対応の vc14.0 でも string_view や filesystem を
使えるようにするのも手、全 vc ccwrap で c23,c++26 関数増やすのも。

ただ ccwrap の実装は 基本的に c++03 基準なので、
modern c++ での実装たりてなかったり効率のよい実装になっていないことが多い。  

※ vc 用にライブラリ乗っ取りをやるならば、MS STL をベースにするのがベターだとは思う…


### install / コンパイラ・コマンドライン指定

ccwrap を clone して手頃なフォルダに設置したとする。

ccwrap_header.h のみ利用の場合。
```batch
cl -FI[CCWRAP]/vc/std/ccwrap_header.h src/hoge.c
```

c/c++標準ライブラリ追加を利用。
```batch
cl -I[CCWRAP]/vc/std -FIccwrap_header.h src/hoge.c
```


## gcc clang

gcc や clang は 正確な c++03 として、実装確認用に用意。  
既存 c++03 ライブラリに足りていないものを補う構成。  

msys2(mingw) と wsl(ubuntu) でお試し。


### install / コンパイラ・コマンドライン指定

ccwrap_header.h のみ利用。
```batch
gcc -include [CCWRAP]/vc/std/ccwrap_header.h src/hoge.c
```

c/c++標準ライブラリ追加を利用。
```batch
gcc -I [CCWRAP]/vc/std -include ccwrap_header.h src/hoge.c
```

※ clang のときは gcc をclang に置き換え。


## Open Watcom C/C++

Open Watcom v2.0(beta) 系を対象。 新し目のものを使っている。(2026-7頃の Releaseを仕様)

c90(c99)/c++03 コンパイラで、c 標準ライブラリについては不足を補充、
c++ 標準ライブラリの c++専用ヘッダについては watcom 付属のものは使わず、
LLVM libc++ 改造のものを使うように置き換えている。

公式 c++ ライブラリはと互換性がないので、既存の Watcom 用 c++ライブラリ(.lib, .dll)
等との併用は不可。
watcom の DLL Cランタイム も不可なので、実質 static Cランタイム用。

デフォルトで _CCW_TARGET_C = 2023, _CCW_TARGET_CXX = 2026 で、
可能な限り多くを試せる状態にしている。

c言語で _CCW_TARGET_C >= 1999 以上に設定する場合は、
c99 の一部機能(c++ と互換性のある機能) をサポートする -Za99
を指定する必要がある。

c++ はほぼ全て置換なので、規格対応は多くなっている。

ただ、locale、filesystem、thread の実装の都合、ヘッダオンリーというわけにもいかず、
ライブラリのリンクが必要になる。  
※ locale は他ライブラリで下請け的に利用されることが多いので、実質 c++ を使うときは
ライブラリ指定する。


### Watcom C++ のバグ

Watcom の C++ はコンパイラ側バグが結構あり、
特に template まわりは、使えない SFINAE 技が多い状態。  
(でも SFINAE に関しては、他の同世代のc++03 コンパイラとしてはまだマシなほうかもしれない)

また、例外処理生成にバグを抱えていて、例外利用の -xs や -xss を指定するとかなりバグを踏みやすい。

ただ -xst という 時間優先 指定にすると、頻出したハングはおきなくる。  
ので例外を使う場合は -xs(-xss) でなく -xst を指定のこと。

例外を使わないプログラムの場合は、-xd で例外無しで運用するほうがよいかもしれない。

ccwrap では、例外を使わない設定の場合は、例外発生タイミングで abort することで、標準ライブラリを使えるようにしている。


### watcom ライブラリ

[CCWRAP]/watcom/lib/gen.bat

を実行すれば、ライブラリを生成する。

ライブラリ名は libccwcxx.lib

ライブラリ名は同じで、指定するライブラリフォルダを、
OS や ビルドオプション、release|debug 等の組み合わせた名前で用意している。

watcom の static Cランタイム(-bm) 前提で、dll Cランタイム(-br) での使用は不可。  
必ず、オプションにあわせたフォルダの lib を選択のこと。


| watcom/lib/下のフォルダ   | 内容                              | wcl386 オプション                             |
|---------------------------|-----------------------------------|-----------------------------------------------|
| win32-std/                | win32 例外&rtti 有                | -bt=nt -l=nt -bm -xst -xr                     |
| win32-std-noeh/           | win32 例外&rtti 無                | -bt=nt -l=nt -bm -xd                          |
| win32-std-fs-char-xrxs/   | win32 例外&rtti 有 char path版    | -bt=nt -l=nt -bm -xst -xr -d_CCW_FS_WCHAR=0   |
| dos32-std/                | dos32 例外&rtti 有                | -bt=dos -l=dos4g -xst -xr                     |
| dos32-std-noeh/           | dos32 例外&rtti 無                | -bt=dos -l=dos4g -xd                          |
| dos32-std-lfn/            | dos32 LFN有効 例外&rtti 有        | -bt=dos -l=dos4g -D__WATCOM_LFN__ -xst -xr    |
| dos32-std-noeh-lfn/       | dos32 LFN有効 例外&rtti 無        | -bt=dos -l=dos4g -D__WATCOM_LFN__ -xd         |
| debug/win32-std/          | debug版win32-std                  | release版に -d2 を足す                        |
| debug/(その他)            |                                   | release版に -d2 を足す                        |

※ その他組み合わせ有り。

※ filesystem::path は windows では wchar_t, DOS32 では char で実装。winの fs-char 版は path を char にしたバージョン。


### install / コンパイラ・コマンドライン指定

ccwrap_header.h のみ利用。
```batch
wcl386  -fi= [CCWRAP]/watcom/std/ccwrap_header.h  src/hoge.c
```

c 標準ライブラリ追加を利用。
```batch
wcl386  -i=[CCWRAP]/watcom/std  -fi=ccwrap_header.h  src/hoge.c
```

c だけなら ライブラリ不要。

c++ の場合は予め  
```batch
 [CCWRAP]/watcom/lib/gen.bat
```
を実行して ライブラリ .lib を生成しておく。

c++ 標準ライブラリ追加を利用。例外&RTTI 有
```batch
wcl386 -bt=nt -l=nt -bm -xr -xst -i=[CCWRAP]/watcom/std -fi=ccwrap_header.h src/hoge.cpp  [CCWRAP]/watcom/lib/win32-std/libccwcxx.lib
```
あるいは
```batch
wcl386 -bt=nt -l=nt -bm -xr -xst -i=[CCWRAP]/watcom/std -fi=ccwrap_header.h src/hoge.cpp  -"LIBPATH [CCWRAP]/watcom/lib/win32-std"
```
あるいは
```batch
set "INCLUDE=[CCWRAP]/watcom/std;%INCLUDE%"
set "LIB=[CCWRAP]/watcom/lib/win32-std;%LIB%"
wcl386 -bt=nt -l=nt -bm -xr -xst -fi=ccwrap_header.h src/hoge.cpp
```

c++ 標準ライブラリ 例外&RTTI 無
```batch
wcl386 -bt=nt -l=nt -bm -xd -i=[CCWRAP]/watcom/std -fi=ccwrap_header.h src/hoge.cpp  -"LIBPATH [CCWRAP]/watcom/lib/win32-std"
```

dos4g で 例外&RTTI 有
```batch:dos4g
wcl386 -bt=dos -l=dos4g -xr -xst -i=[CCWRAP]/watcom/std -fi=ccwrap_header.h -fe=hello.exe hello.cpp [CCWRAP]/watcom/lib/dos32-std/libccwcxx.lib
```


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

<!-- ※ 正直 ライブラリ実装より、テスト実装＆チェック作るほうががたいへん... (AIが) -->


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

作ってみただけのまだまだバギー状態。  
