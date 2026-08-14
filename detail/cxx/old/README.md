# detail/old — 役目を終えた代用実装

ここにあるのは、**llibcxx03 の実装に置き換わって使われなくなった** `detail/*.hpp`。
ビルドからは外れている (どこからも include されていない)。消さずに残しているのは、
llibcxx03 版が古い vc で通らないと分かったときに戻せるようにするため。

## 方針

`vc/*` / `gcc/*` のラッパーで「native にヘッダが無い」ときの実装は、二重に持たず
**llibcxx03 に寄せる** (Open Watcom と同じ本体を使う)。`detail/` に残るのは:

- **native のヘッダはあるが中身が足りない**ものへの補充 (`algorithm` `memory`
  `string` `utility` `numeric` `functional` `iterator` `system_error` `expected`)
- **C 言語側**の補充 (`math_c23.h` `errno_c11.h` `stdint_width.h` など)
- llibcxx03 自身が使う部品 (`fnctmpl.hpp` `move03.hpp` `tuple.hpp`)

## 移設の経緯

2026-07-25 の時点では、llibcxx03 を vc から include すると 1 ファイルあたり 7〜102 個の
エラーが出て、**16 個の代用実装が移せなかった**。原因は 2 つあり、どちらも
2026-07-26 に解消した (`llibcxx03/include/__config` の該当節を参照):

1. `__type_traits/integral_constant.h` が **`namespace std` を開いたまま** native の
   `<type_traits>` を include していた。標準ライブラリ全体が `std::std` に入り、
   MSVC 自身の `using ::ptrdiff_t` / `using _STD add_const` が軒並み失敗する。
   これが「1 ファイル 102 エラー」の正体。
2. llibcxx03 の土台ヘッダが、native が既に持つ名前を**再定義**していた
   (`char_traits` / `iterator_traits` / iterator タグ / `allocator` / `pair` /
   `swap` / `hash` / `addressof` / `size` …)。

いまは `_CCW_LIBCPP_NATIVE_STD` / `_CCW_LIBCPP_HAS_NATIVE_CXX11_LIB` /
`_CCW_LIBCPP_HAS_NATIVE_ITERATOR_TRAITS` / `_CCW_LIBCPP_HAS_NATIVE_CHAR_TRAITS` で
**「native (またはこのツリーの detail 補充) が持っているものは定義しない」**ように
なっており、16 個とも vc120 で 0 エラーで include できる。

先行して移設済み (2026-07-25):
`atomic` `bit` `debugging` `flat_map` `flat_set` `memory_resource` `numbers`
`random` `ranges` `regex`

**2026-07-26 に「移設する」と決めた分が、実際には移動されないまま `detail/cxx/` 直下に
残っていた**。2026-08-02 のフォルダ再構成のついでに、include を実際に解決して
「どこからも引かれていない」ことを確認した上で移動した:

`any` `array` `charconv` `chrono` `filesystem` `forward_list` `hashtable` `optional`
`ratio` `span` `type_traits` `typeindex` `unordered_map` `unordered_set` `variant`

- `hashtable.hpp` は `unordered_map` / `unordered_set` からしか引かれていないので
  一緒に移動した (3 つで 1 つの塊)。
- **`string_view.hpp` は移していない**。2026-07-26 の一覧には入っていたが、
  テストハーネス `test/src/test.hpp` が実際に引いている。
- `has_allocator_type.hpp` も残る (`detail/cxx/memory.hpp` が引いている)。

判定は「相対 include を実際に解決して数える」方法で行った。**同じディレクトリの
引用符 include (`"has_allocator_type.hpp"`) を数え忘れると誤って未使用に見える**ので注意。

## regex は戻せない

`detail/old/regex.hpp` は **SRELL (`thirdparty/srell/`) の薄い using ラッパ**だった。
SRELL は 2026-07-26 に破棄した (ユーザー方針: llibcxx03 に regex があるのだからそれを
使う) ので、**このファイルを戻すだけでは動かない**。戻すなら git 履歴から
`thirdparty/srell/` も一緒に復元すること。

**機能後退が 1 つある**: SRELL は wide (`wregex` / `wsmatch` / `wcmatch`) を持っていたが、
`llibcxx03/include/regex` のエンジンは **char 専用**で `wregex` が無い
(`llibcxx03/include/regex` の basic_regex の注記、およびチェックリストに記載済み)。
`test/cxx_test/regex_test.cpp` は wide を使うので、native `<regex>` を持たない環境
(gcc の C++98 モード、vc8/9) では**このテストがコンパイルできない**。
wide を戻したいなら llibcxx03 側のエンジンを wchar_t 対応にするのが筋。
