# ccwrap

ccwrap  は 古いコンパイラで、c11/c++11以降の規格の極々一部を
簡易に真似てみるラッパー・ライブラリ。

対象コンパイラは  Windows 用で
- vc: MS Visual C/C++ (8～12(2005-2013)系、14.x(2015～2019)系) 
- clang, mingw (c++11 対応以降での c++03 モード利用して確認)

※ 以前 試していた open watcom, borland c5.5.1, dmc は _etc/ フォルダ下を参照。


## やれること

可能ならコンパイラ独自名 (__alignof等) を標準名 (alignof等) に #define し、
実装可能なら代用品を用意(static_assert等)、機能無くとも大半を誤魔化
せるならば空定義(constexpr等)を行う。

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


vc++ では、boost を用いることで 
（namespace std に using ::boost::????; して）
対応を増やせる。  
※ 現状 include してみただけでほぼ未使用... (やれそうだからやってみた程度)


## 使い方

### 暗黙include＆includeパス

ccwrap は、本来の標準ライブラリより先に ccwrap の置換ファイルが
読み込まれることで実現している。
ので、各コンパイラ別のディレクトリをコンパイラ・オプションで指定する必要がある。  
( vc/clang/gcc では -I  )

  ccwrap/vc2013_or_earlier/      vc2005～vc2013(vc8～vc12)   
  ccwrap/vc2015_or_later/        vc2015以降(～vc2019)   
  ccwrap/clang/                      msys2 clang++,g++ のc++11以降用  
  ccwrap/clang_cxx03/             msys2 clang++,g++ のc++03用  

また、 各コンパイラ別フォルダにある  

  ccwrap_header.h

が、暗黙include される必要がある。  
(vc では -FI、clang/gcc では -include )


例えば X:\ccwrap だとして、  

  cl -IX:\ccwrap\vc2013_or_earlier -Flccwrap_header.h example.c

c++ の場合は

  cl -IX:\ccwrap\vc2013_or_earlier -Flccwrap_header.h -EHsc -Zc:wchar_t -Zc:forScope example.cpp

(c++ 標準寄りの文法になるようにオプション設定しておく)


### boost の利用.

※ 最近の boost は c++11 以前を切り捨てているので、対応している頃
の boost を用いる必要がある。  作成時に用いていたのは 1.6.x ～ 1.7.2 

boost を利用する場合は、include されるディレクトリとして

- boost2std/
- (ccwrap コンパイラ別のディレクトリ)
- (boostのディレクトリ)

をコンパイラオプション指定する。  

'ccwrap/boost2std/'' が (ccwrapコンパイラ別ディレクトリ) より先に '
検索される必要があるため、指定順序は注意のこと。

また、暗黙の include として、boost2std/ 下の

  boost2std.hpp

を必ず指定する必要がある。

たとえば X:\ccwrap、 X:\boost_1_72_0 があるとして、

  cl -lX:\ccwrap\boost2std  -IX:\boost_1_72_0 -IX:\ccwrap\vc2013_or_earlier -Flboost2std.hpp -EHsc -Zc:wchar_t -Zc:forScope example.cpp

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
- namespace ccwrap か namespace __ccwrap に格納

している.

"__" で始まる名前は ほぼccwrapライブラリ内だけで使うものだが、
"__" の付かない ccwrap ccwrap_ CCWRAP_ は標準ライブラリとは別のライブラリとしての利用も想定している。
（std 実装に用いている内部処理を単独利用することがあれば）


##  おわりに

やれそうだからやってみただけの代物なのでバギー。  
vc comunity edition や msys2 があるので、今どきの Windows 環境で使うことはなく。  
古い環境向けに古いコンパイラしか使えない場合に、多少なりとも今風に近づける...かもしれない。  
