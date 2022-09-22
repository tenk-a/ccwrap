# ccwrap _etc

ccwrap  は c11/c++11以降の規格の極々一部を、古いコンパイラ向けに、
簡易に補完するためのラッパー・ライブラリで、../readme.md 参照を参照のこと。

_etc/ フォルダにあるのは

   - borland c/c++ v5.5.1 (bc55) Embarcadero C++ 7.2 (bcc32c) 
   - open watcom c/c++ v1.9                                  (ow)
   - digital mars c/c++                                           (dmc)

用。
（だが、現状、手元の環境ではコンパイラをインストールしていないものもあるため
動作不明のものもあり）

可能であれば、コンパイラ独自の相当機能、通常文法範囲での実装、
空定義等で #define して代用品を用意して、

- c++用:  
    alignas, alignof, char16_t, char32_t, final, __ func __,  
    noexcept, nullptr, override, _Pragma, static_assert, thread_local
- c 用:  
    _Alignas, _Alignof, _Bool, inline, _Noreturn, restrict,  
    _Static_assert, _Thread_local  
- ヘッダ関係は stdint.h(cstdint) 等が無ければ追加.

で、簡易な標準化(共通化)を行う。


##  使い方

- コンパイラオプションの 暗黙のinclude 指定を用いて、初っ端に

  ccwrap_header.h

がincludeされるように指定する。 (ow:-fi=  dmc:-HI  bc55:無し)

- ccwrap/??/ ディレクトリを、他より先に検索されるように include パス設定する。
(ow:-i=  dmc:-I   bc55:-I)  
標準ライブラリをフックして定義を追加している。  
ので、必ず標準ライブラリを include するようにすれば、暗黙のinclude を指定しなくても ok.
※ bc55 では暗黙のincludeが行えないため。

例えば X:\ccwrap があるとして、

- wcl386 -cc++ -xs -xr -bcl=nt -I=X:\ccwrap\_etc\ow -fi=ccwrap_header.h smp.cpp
- dmc -IX:\ccwrap\_etc\dmc -Hlccwrap_header.h smp.cpp kernel32.lib gdi32.lib
- bcc32 -IX:\ccwrap\_etc\borland smp.cpp
