# ccwrap

ccwrap  は 古いコンパイラで、c11/c++11以降の規格の極々一部を
簡易に真似てみるラッパー・ライブラリ。
かなり、実験物。

対象コンパイラは  Windows 用で
- vc: MS Visual C/C++ (8～12(2005-2013)系、14.x(2015～2019)系) 
- clang, mingw (c++11 対応以降での c++03 モード利用して確認)

※ 以前 試していた open watcom, borland c5.5.1, dmc は etc/ フォルダ下を参照。


## やれること

可能ならコンパイラ独自名 (__alignof等) を標準名 (alignof等) に #define し、
実装可能なら代用品を用意(static_assert等)、機能無くとも大半を誤魔化
せるならば空定義を行う。

- c++用:  
    alignas, alignof, char8_t, char16_t, char32_t, constexpr, final,
    __ func __, noexcept, nullptr, override, static_assert, thread_local

- c 用:  
    _Alignas, _Alignof, _Bool, inline, _Noreturn, _Pragma, restrict,
    _Static_assert, _Thread_local  
    (※vc では __restrict を restrict に #define すると問題有で未定義)

ヘッダについては、

- c 用:  
    stdint.h, stdalign.h, stdbool.h, stdnoreturn.h, (unistd.h)

- c++ 用:  
    cstdint
    array, string_view, type_traits

の追加や c++03 既存ヘッダへの補完を若干行っている。


vc++ では、別の方法として boost を用いることで 
（namespace std に using ::boost::????; して）
対応を増やせる。  
※ 現状 include してみただけでほぼ未使用... (やれそうだからやってみた程度)


## 使い方

### 暗黙include＆includeパス

ccwrap は、本来の標準ライブラリより先に ccwrap の置換ファイルが
読み込まれることで実現している。
ので、各コンパイラ別のディレクトリと ccwrap/ccwrap/ を
コンパイラ・オプションで指定する必要がある。  
( vc/clang/gcc では -I  )

  ccwrap/vc/      vc2005～vc2022(vc8～vc14.3)   
  ccwrap/gcc/     msys2 clang++,g++ 用  

また、 各コンパイラ別フォルダにある  

  ccwrap_header.h

が、暗黙include される必要がある。  
(vc では -FI、clang/gcc では -include )


例えば X:\ccwrap だとして、  

  cl -IX:\ccwrap\vc -IX:\ccwrap\ccwrap -Flccwrap_header.h example.c

c++ の場合は

  cl -IX:\ccwrap\vc -IX:\ccwrap\ccwrap -Flccwrap_header.h -EHsc -Zc:wchar_t -Zc:forScope example.cpp

(c++ 標準寄りの文法になるようにオプション設定しておく)


### vc での boost の利用.

※ 最近の boost は c++11 以前を切り捨てているので、対応している頃
の boost を用いる必要がある。  作成時に用いていたのは 1.6.x ～ 1.7.2 

boost を利用する場合は、include されるディレクトリとして

- boost2std/
- (boostのディレクトリ)

をコンパイラオプション指定する。  
※ コンパイラ別フォルダやccrwap/ccwrap フォルダは指定してはいけない。

また、暗黙の include として、boost2std/ 下の

  boost2std.hpp

を必ず指定する必要がある。

たとえば X:\ccwrap、 X:\boost_1_72_0 があるとして、

  cl -lX:\ccwrap\boost2std  -IX:\boost_1_72_0 -Flboost2std.hpp -EHsc -Zc:wchar_t -Zc:forScope example.cpp

c++ 標準寄りの文法になるようにオプション設定しておく。

なお、std のヘッダをラップして boost ヘッダを include しているため、
boost 内から include される標準ライブラリは不整合が起きやすい。

とくに  

  memory utility string ios istream ostream sstream streambuf  

あたりはエラーが出やすい。

ただ、他の c++ 標準ヘッダに先んじて
```
    #include <algorithm>
```
をしておくと回避できることが多い模様。

※ boost を利用するが、不整合を起こしやすいので
boost ライブラリをboostとして併用するのは厳しいかもしれない。


## ccwrap 固有関係

ccwrap 関係の名前は
- プレフィックスに ccwrap_ CCWRAP_ __ccwrap_ __CCWRAP_ をつけるか
- namespace ccwrap   _ccwrap  __ccwrap に格納

している.

"__" で始まる名前は ほぼccwrapライブラリ内だけで使うものだが、
"__" の付かない ccwrap ccwrap_ CCWRAP_ は標準ライブラリとは別のライブラリとしての利用も想定している。
（std 実装に用いている内部処理を単独利用することがあれば）


##  おわりに

やれそうだからやってみただけの代物なのでバギー。  
vc comunity edition や msys2 があるので、今どきの Windows 環境で使うことはなく。  
古い環境向けに古いコンパイラしか使えない場合に、多少なりとも今風に近づける...かもしれない。  
