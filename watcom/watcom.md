# ccwrap watcom

(2026-09)

Open Watcom C/C++ 2.0 (`__WATCOMC__` = 1300) で ccwrap を使ったときに、
標準ヘッダがどこまで使えそうかのヘッダ別のまとめ。

C++ 専用のヘッダは ccwrap の llibcxx03 (libc++ を C++03 向けに移植したもの) が
提供する。C のヘッダと `<cxxx>` は Open Watcom 付属のものを包んで、足りないものを補う
(`<locale.h>` のように丸ごと差し替えたものもある)。

## 数字の出どころ

表の数字は `test/` のテストの実測。規格の項目一覧 (`test/cxx_test/spec/*.tsv` /
`test/c_test/spec/*.tsv`) の 1 行ごとに、Watcom でのテスト結果を突き合わせて数えた。

| | 結果タグ | 条件 | 測定日 |
| --- | --- | --- | --- |
| C++ | `watcom1300-x86-cxx03-ccw26` | Win32、preset `watcom` (`-xst -xr`、static MT ランタイム)、`_CCW_TARGET_CXX=2026` | 2026-08-30 |
| C | `watcom1300-x86-c1990-ccw26` | Win32、既定の C 方言 (`-za99` なし)、`_CCW_TARGET_C=2026` | 2026-08-30 |

測り直すときは `test/` で `cmake --preset watcom` → `cmake --build --preset watcom` →
`ctest --preset watcom`。結果は `test/result_{cxx,c}_test/<結果タグ>/` に出る
(git には入れていない)。

### 記号

| 記号 | 意味 |
| --- | --- |
| 項目 | 規格の項目数。**C++26 (C は C23) までの全部が分母**なので、古い規格で入ったヘッダでも新しい規格での追加分が数に入る |
| ○ | pass。Watcom でテストが通った |
| △ | skip。テストがその項目を確かめなかった |
| × | fail。テストが通らなかった |
| ― | 結果なし。その項目を出すテストが Watcom では回っていない |
| ○率 | ○ / 項目 |

`.` は 0。備考の `(A12)` などは `doc/watcom/ccwrap_watcom_bugs.md` の項目番号。

### △ と ― は「使えない」とは限らない

△ と ― には次の 4 種類が混ざっている。**使えるかどうかは ○ と備考で判断する。**

1. 言語機能が無く、規格の形では書けない。右辺値参照、波括弧の初期化子リスト、
   可変長テンプレート、別名テンプレート、変数テンプレート、`constexpr`、`<=>`、
   concepts、コルーチン、ユーザ定義リテラルが該当する。
2. 実装はあるが、部分実装なので `__cpp_lib_*` を定義しておらず、テストがそれを見て確かめていない。
3. テストが C++11 の文法で書かれていて、Watcom ではそのブロックごと回っていない。
4. 実装が無い。

## 規格別のまとめ

C++ (`<xxx.h>` を C++ から使う分は `<cxxx>` と重なるので除く):

| 項目が入った規格 | 項目 | ○ | △ | × | ― | ○率 |
|---|--:|--:|--:|--:|--:|--:|
| C++98/03 | 2434 | 2358 | 54 | 2 | 20 | 97% |
| C++11 | 2667 | 2168 | 232 | . | 267 | 81% |
| C++14 | 131 | 79 | 44 | . | 8 | 60% |
| C++17 | 1031 | 721 | 267 | . | 43 | 70% |
| C++20 | 1555 | 633 | 349 | . | 573 | 41% |
| C++23 | 816 | 159 | 513 | . | 144 | 19% |
| C++26 | 1324 | 293 | 1018 | . | 13 | 22% |
| **計** | **9958** | **6411** | **2477** | **2** | **1068** | **64%** |

C (任意機能の項目を除く):

| 項目が入った規格 | 項目 | ○ | △ | × | ― | ○率 |
|---|--:|--:|--:|--:|--:|--:|
| C90 | 444 | 440 | 4 | . | . | 99% |
| C95 | 240 | 211 | 11 | 18 | . | 88% |
| C99 | 612 | 546 | 30 | 7 | 29 | 89% |
| C11 | 41 | 34 | 7 | . | . | 83% |
| C23 | 467 | 178 | 289 | . | . | 38% |
| **計** | **1804** | **1409** | **341** | **25** | **29** | **78%** |

- C++98/03 で入った項目は 97% が ○。残りは Watcom の制約で書けないもの (2 引数の `operator delete`、
  生配列を渡す `std::begin` など) と、テストが確かめていないもの。
- C++11〜C++17 は 6〜8 割。△ と ― の多くは上の 1 (右辺値参照・初期化子リスト・変数テンプレート) か 3 (テストの文法)。
- C++20 以降は、`<ranges>` / `<format>` / `<concepts>` のように言語機能を前提にしたヘッダが大きく、○ は 2〜4 割。
- × は C++ で 2 件、C で 25 件。どれも Open Watcom の CRT の書式 I/O の不具合 (A24 / A44 / A44')。

## 提供していないヘッダ

次のヘッダは Watcom では使えない。下の表にも行はあるが、○ は 0。

| ヘッダ | 規格 | 状態 | 理由 |
| --- | --- | --- | --- |
| `<format>` | C++20 | include すると `#error` | 未実装 (部分集合を作る予定) |
| `<print>` | C++23 | include すると `#error` | 未実装 (`<format>` が前提) |
| `<mdspan>` | C++23 | ヘッダはあるが中身が空 | 未実装 |
| `<concepts>` | C++20 | ヘッダが無い | concepts は言語機能 |
| `<coroutine>` | C++20 | ヘッダが無い | コルーチンは言語機能 |
| `<generator>` | C++23 | ヘッダが無い | コルーチンが要る |
| `<contracts>` | C++26 | ヘッダが無い | 言語機能 |
| `<meta>` | C++26 | ヘッダが無い | 静的リフレクションは言語機能 |
| `<simd>` | C++26 | ヘッダが無い | 未実装 |
| `<linalg>` | C++26 | ヘッダが無い | 未実装 (`<mdspan>` が前提) |
| `<hazard_pointer>` | C++26 | ヘッダが無い | 未実装 |
| `<rcu>` | C++26 | ヘッダが無い | 未実装 |
| `<complex.h>` (C から) | C99 | 使えない | コンパイラに `_Complex` が無い。C++ の `<complex>` を使う |
| `<stdatomic.h>` (C から) | C11 | include すると `#error` | コンパイラに `_Atomic` が無い。C++ の `<atomic>` を使う |
| `<tgmath.h>` (C から) | C99 | ヘッダが無い | C に `_Generic` が無い (A16) |

ヘッダはあるが、中身がごく一部しか無いもの:

| ヘッダ | 規格 | あるもの |
| --- | --- | --- |
| `<ranges>` | C++20 | 範囲アクセス (`ranges::begin` / `end` / `size` / `data` 等) だけ。view と range concepts は無い |
| `<stdfloat>` | C++23 | ヘッダだけ。`float16_t` 等の型はコンパイラに無いので定義されない |
| `<initializer_list>` | C++11 | 型はあるが、波括弧の初期化子リストが言語に無いので実質使えない |
| `<execution>` | C++17 | 実行ポリシーだけ (中身は逐次実行)。C++26 の sender / receiver は無い |

## C++ ヘッダ

### C++98/03 からあるヘッダ

| ヘッダ | 項目 | ○ | △ | × | ― | ○率 | 備考 |
|---|--:|--:|--:|--:|--:|--:|---|
| `<algorithm>` | 313 | 153 | 7 | . | 153 | 49% | C++03 から C++17 まで (並列版を含む) のアルゴリズムはある。C++20 の `ranges::` 版は無い (結果型だけある)。― の多くは C++11 の文法で書いたテストで、Watcom では回っていない |
| `<bitset>` | 73 | 60 | 13 | . | . | 82% | C++11 までは全部。C++23 の constexpr 化、C++26 の `string_view` からの構築は無い |
| `<complex>` | 136 | 128 | 4 | . | 4 | 94% | C++11 までは全部。`1.0i` 等のリテラル (C++14) はユーザ定義リテラルが無いので書けない |
| `<deque>` | 131 | 111 | 6 | . | 14 | 85% | C++03 は全部。initializer_list での構築と CTAD は言語機能が無い。`pmr::deque` は別名テンプレートが無いので名前を定義できない |
| `<exception>` | 43 | 30 | 7 | . | 6 | 70% | `exception_ptr` / `current_exception` / `rethrow_exception` / `make_exception_ptr` はある (ハンドラの外での `current_exception` は未検証)。`nested_exception` 系は部分実装で、ハンドラの外で既定構築するとプロセスが終わる (Watcom に送出中の例外を取る口が無いため)。テストは回していない。`uncaught_exceptions()` は無い (ランタイムに数える口が無い) |
| `<fstream>` | 220 | 147 | 62 | . | 11 | 67% | C++03 の範囲 (seek を含む) と C++17 の `filesystem::path` 版 open はある。move (C++11) は右辺値参照が無いので無い。`swap` と `string` 版 open は実装があるが未検証。C++23 の `noreplace` は無い |
| `<functional>` | 120 | 79 | 39 | . | 2 | 66% | `function` / `bind` / `mem_fn` / `reference_wrapper` / `hash`、C++17 の検索器 (`boyer_moore_searcher` 等) はある。`bind_front` (C++20) / `move_only_function` (C++23) / `function_ref` (C++26) は無い。`bind1st` 等 (C++17 で削除) は残してあるが、テストは確かめていない |
| `<iomanip>` | 16 | 11 | 5 | . | . | 69% | C++03 は全部。`quoted` (C++14) と `put_money` は実装があるが未検証 |
| `<ios>` | 130 | 120 | 10 | . | . | 92% | C++03 はほぼ全部。C++11 の `basic_ios::move` / `swap` は未検証。C++23 の `noreplace` は無い |
| `<iosfwd>` | 72 | 52 | 20 | . | . | 72% | syncstream (C++20) と spanstream (C++23) の前方宣言は未検証 |
| `<iostream>` | 12 | 12 | . | . | . | 100% | 全部 |
| `<istream>` | 61 | 57 | 4 | . | . | 93% | move (C++11) は無い。`swap` は実装があるが未検証。ほかは全部 |
| `<iterator>` | 270 | 133 | 115 | . | 22 | 49% | C++03 の反復子と C++11 の `next` / `prev` 等はある。生配列を渡す `std::begin(arr)` / `std::size(arr)` は Watcom が `T[N]` を推論できないので使えない (A12)。C++20 の反復子 concepts・`counted_iterator`・`common_iterator` は無い。`move_iterator` は実装があるが未検証 |
| `<limits>` | 106 | 105 | 1 | . | . | 99% | 全部 (`char8_t` を除く) |
| `<list>` | 111 | 90 | 20 | . | 1 | 81% | C++11 まで (initializer_list を除く) はある。`remove` / `unique` は C++03 の形 (void を返す)。C++23 の範囲系は無い |
| `<locale>` | 314 | 303 | 10 | . | 1 | 96% | facet 一式 (`num_get` / `num_put`、money、time、messages、`*_byname`、`codecvt`) がそろう。ロケールの実データは Win32 の NLS API から取る。`wstring_convert` / `wbuffer_convert` もある |
| `<map>` | 133 | 116 | 17 | . | . | 87% | C++17 のノードハンドル・`try_emplace`・`insert_or_assign` まではある。initializer_list・`<=>`・C++23 の範囲からの構築は無い |
| `<memory>` | 181 | 117 | 62 | . | 2 | 65% | `unique_ptr` / `shared_ptr` / `weak_ptr` / `allocator_traits` / `pointer_traits` / `uninitialized_*` はある。`static_pointer_cast` / `const_pointer_cast` はあるが、`dynamic_pointer_cast` は制限付き (制御ブロックが要素型ごとの作りで、型の違う `shared_ptr` 同士で共有できない)。C++20 の `ranges::uninitialized_*`・`atomic<shared_ptr>`、C++23 の `out_ptr` は無い |
| `<new>` | 47 | 16 | 31 | . | . | 34% | `bad_alloc` / `set_new_handler` / 配置 new / `launder` / destroying delete はある。2 引数の `operator delete` を宣言できないので nothrow 版・配置版の delete は無い (A7)。C++17 の aligned new は無い |
| `<numeric>` | 64 | 46 | 18 | . | . | 72% | C++17 の `gcd` / `lcm` / `reduce` / `*_scan` / `transform_reduce`、C++26 の飽和演算 (`add_sat` 等) まである。並列版は未検証。`ranges::iota` (C++23) は無い |
| `<ostream>` | 69 | 57 | 12 | . | . | 83% | move (C++11) と `print` / `println` (C++23) は無い。`swap` は実装があるが未検証。`operator<<(const volatile void*)` は入れていない (入れると `os << p` が曖昧になる。A49) |
| `<queue>` | 62 | 49 | 13 | . | . | 79% | C++11 までは全部。C++23 の範囲・反復子対からの構築は無い |
| `<set>` | 120 | 107 | 13 | . | . | 89% | C++17 のノードハンドルまではある。initializer_list・`<=>`・C++23 の範囲からの構築は無い |
| `<sstream>` | 92 | 72 | 10 | . | 10 | 78% | C++03 は全部。move / swap (C++11) は未検証。`view()` (C++20) と `string_view` 版 `str()` (C++26) は無い |
| `<stack>` | 29 | 20 | 9 | . | . | 69% | C++11 までは全部。C++23 の範囲・反復子対からの構築は無い |
| `<stdexcept>` | 14 | 14 | . | . | . | 100% | 全部 |
| `<streambuf>` | 48 | 48 | . | . | . | 100% | 全部 |
| `<string>` | 205 | 144 | 61 | . | . | 70% | C++03 と C++11 の大半 (`to_string` / `stoi` 等) はある。move 構築・initializer_list・`"..."s` リテラルは言語機能が無い。C++17 以降の追加分は未検証のものが多い |
| `<strstream>` | 23 | 22 | 1 | . | . | 96% | ほぼ全部 |
| `<typeinfo>` | 8 | 8 | . | . | . | 100% | 全部 |
| `<utility>` | 69 | 47 | 21 | . | 1 | 68% | `pair` / `swap` / 疑似 `move` / `exchange` / `as_const` / `in_place` はある。`forward` / `declval` は右辺値参照が無い。`integer_sequence` (C++14) は無い |
| `<valarray>` | 114 | 111 | 3 | . | . | 97% | 全部 (initializer_list を除く) |
| `<vector>` | 100 | 85 | 11 | . | 4 | 85% | C++03 は全部 (`vector<bool>` を含む)。initializer_list・`<=>`・C++23 の範囲系は無い |

### C++11 で入ったヘッダ

| ヘッダ | 項目 | ○ | △ | × | ― | ○率 | 備考 |
|---|--:|--:|--:|--:|--:|--:|---|
| `<array>` | 89 | 74 | 4 | . | 11 | 83% | ある。`array<int,3> a = {1,2,3}` の形を試すテストは C++11 の文法なので未検証。空の `= {}` は Watcom が受け付けない (A36)。`to_array` (C++20) は無い |
| `<atomic>` | 356 | 327 | 29 | . | . | 92% | C++20 の `wait` / `notify`・`atomic_ref` まである。`atomic<構造体>` の既定構築はゼロにならない (Watcom が集成体を値初期化しない。A37)。C++26 の `fetch_max` / `fetch_min` 等は無い |
| `<chrono>` | 470 | 229 | 10 | . | 231 | 49% | C++20 のカレンダー (`year_month_day` 等) と `hh_mm_ss` はある。タイムゾーン (`zoned_time` / tzdb) と `format` 連携は無い。C++11 の `duration` / `time_point` / 時計は実装があるが、テストが C++11 の文法なので未検証 (―) |
| `<codecvt>` | 43 | 42 | 1 | . | . | 98% | ほぼ全部 |
| `<condition_variable>` | 45 | 9 | 7 | . | 29 | 20% | `condition_variable` / `condition_variable_any` は実装があるが、テストの大半が C++11 の文法なので未検証。`stop_token` を取る版 (C++20) は無い |
| `<forward_list>` | 88 | 69 | 19 | . | . | 78% | initializer_list を除いてある。`remove` が個数を返す形 (C++20) と C++23 の範囲系は無い |
| `<future>` | 71 | . | 15 | . | 56 | 0% | `promise` / `future` / `shared_future` / `packaged_task` / `async` は実装があるが、テストを Watcom で回していない (未検証) |
| `<initializer_list>` | 29 | 1 | 21 | . | 7 | 3% | 波括弧の初期化子リストが言語に無いので、実質使えない |
| `<mutex>` | 81 | 68 | 12 | . | 1 | 84% | `mutex` / `recursive_mutex` / `timed_mutex` / `lock_guard` / `unique_lock` / `call_once` / `scoped_lock` はある。`native_handle` は無い |
| `<random>` | 132 | 114 | 18 | . | . | 86% | エンジンと分布は全部ある。`linear_congruential_engine<T, a, c, m>` は結果型が `unsigned long` に固定 (B8)。名前付きの typedef (`minstd_rand` 等) はそのまま使える。`philox_engine` (C++26) は無い |
| `<ratio>` | 47 | 32 | 15 | . | . | 68% | 演算・比較と SI 接頭辞はある。64bit に収まらない `yocto` 等と、`_v` の変数テンプレート (C++17) は無い |
| `<regex>` | 206 | 199 | 7 | . | . | 97% | ほぼ全部 |
| `<scoped_allocator>` | 81 | 64 | 16 | . | 1 | 79% | 内側のアロケータは 2 個まで (可変長テンプレートが無いため)。move 系は無い |
| `<system_error>` | 71 | 61 | 10 | . | . | 86% | ある。`error_code(errc::...)` の暗黙変換は入れていない (`enum class` の代用型に足すと比較が曖昧になる。B4')。`make_error_code` を明示的に呼ぶ |
| `<thread>` | 44 | 21 | 22 | . | 1 | 48% | `thread` / `this_thread` はある。`native_handle` と `jthread` (C++20) は無い |
| `<tuple>` | 46 | 25 | 21 | . | . | 54% | `tuple` / `get` / `make_tuple` はある (要素数は固定の上限まで)。`tie` / `tuple_cat` / `forward_as_tuple` / `apply` は未検証 |
| `<type_traits>` | 250 | 97 | 153 | . | . | 39% | C++11 の型特性はほぼある。ただし `is_union` と `has_virtual_destructor` は書けず、有界配列 `T[N]` の判定 (`is_array` / `extent` / `rank`) もできない (A12)。trivial 系は保守的 (スカラとその配列だけ true)。`_t` 別名 (C++14) と `_v` 変数テンプレート (C++17) は言語機能が無い。`is_invocable` / `invoke_result` は無い |
| `<typeindex>` | 7 | 6 | 1 | . | . | 86% | 全部 (`<=>` を除く) |
| `<unordered_map>` | 105 | 96 | 7 | . | 2 | 91% | C++17 の `try_emplace` とノードハンドルまではある。initializer_list と C++23 の範囲からの構築は無い |
| `<unordered_set>` | 104 | 92 | 12 | . | . | 88% | C++17 のノードハンドルまではある。initializer_list と C++23 の範囲からの構築は無い |

### C++14 で入ったヘッダ

| ヘッダ | 項目 | ○ | △ | × | ― | ○率 | 備考 |
|---|--:|--:|--:|--:|--:|--:|---|
| `<shared_mutex>` | 47 | 42 | 5 | . | . | 89% | `shared_mutex` / `shared_timed_mutex` / `shared_lock` はある |

### C++17 で入ったヘッダ

| ヘッダ | 項目 | ○ | △ | × | ― | ○率 | 備考 |
|---|--:|--:|--:|--:|--:|--:|---|
| `<any>` | 54 | 48 | 3 | . | 3 | 89% | ある。`in_place_type<T>` は変数テンプレートなので無い (`in_place_type_t<T>()` と書く) |
| `<charconv>` | 25 | 20 | 5 | . | . | 80% | `to_chars` / `from_chars` (整数・浮動小数) はある |
| `<execution>` | 113 | 13 | 100 | . | . | 12% | 実行ポリシー (`seq` / `par` / `par_unseq` / `unseq`) はある (中身は逐次実行)。C++26 の sender / receiver は無い |
| `<filesystem>` | 261 | 248 | 13 | . | . | 95% | ほぼ全部。シンボリックリンクを作る関数は未検証。`path::value_type` は Windows では `wchar_t`、DOS では `char` |
| `<memory_resource>` | 62 | 25 | 37 | . | . | 40% | `memory_resource` / `polymorphic_allocator` / `monotonic_buffer_resource` / `new_delete_resource` はある。プール (`synchronized_pool_resource` / `unsynchronized_pool_resource`) は無い。`pmr::vector` 等の別名は別名テンプレートが無いので無い |
| `<optional>` | 59 | 43 | 14 | . | 2 | 73% | C++17 の範囲はある。monadic 操作 (`and_then` 等。C++23) は無い |
| `<string_view>` | 103 | 89 | 14 | . | . | 86% | ある。`"..."sv` リテラルは無い。C++23 以降の追加分は未検証 |
| `<variant>` | 53 | 32 | 21 | . | . | 60% | ある (候補型の数は固定の上限まで)。`emplace<I>` は Watcom では無い (A13'')。`visit` 等は未検証 |

### C++20 で入ったヘッダ

| ヘッダ | 項目 | ○ | △ | × | ― | ○率 | 備考 |
|---|--:|--:|--:|--:|--:|--:|---|
| `<barrier>` | 18 | 17 | 1 | . | . | 94% | ある。△ はコピー禁止 (`= delete`) の確認で、C++03 では試せない |
| `<bit>` | 76 | 59 | 17 | . | . | 78% | C++20 の関数はある。`byteswap` (C++23) は未検証。C++26 の追加は無い |
| `<compare>` | 73 | 9 | 8 | . | 56 | 12% | `strong_ordering` 等の比較カテゴリ型はある。`<=>` 演算子が言語に無いので、それを前提にした部分は使えない |
| `<concepts>` | 38 | . | . | . | 38 | 0% | **提供なし** (言語機能が要る) |
| `<coroutine>` | 38 | . | . | . | 38 | 0% | **提供なし** (言語機能が要る) |
| `<format>` | 147 | . | 50 | . | 97 | 0% | **提供なし** (include すると `#error`) |
| `<latch>` | 12 | 10 | 2 | . | . | 83% | ある。△ は `__cpp_lib_latch` とコピー禁止 (`= delete`) の確認 |
| `<numbers>` | 28 | 14 | 14 | . | . | 50% | `pi` 等の定数はある。`pi_v<T>` 等は変数テンプレートなので無い |
| `<ranges>` | 157 | 15 | 20 | . | 122 | 10% | 範囲アクセス (`ranges::begin` / `end` / `size` / `data` 等) と `dangling` / `from_range_t` だけの部分集合。view と range concepts は無い |
| `<semaphore>` | 20 | 19 | 1 | . | . | 95% | ある。△ は `__cpp_lib_semaphore` の確認 |
| `<source_location>` | 10 | 7 | 2 | . | 1 | 70% | 型はあるが、`current()` は呼び出し位置を取れない (行は 0、ファイル名は空。A18) |
| `<span>` | 46 | 33 | 13 | . | . | 72% | 動的・静的 extent ともある。`at` (C++26) は無い |
| `<stop_token>` | 42 | 13 | 29 | . | . | 31% | `stop_token` / `stop_source` / `stop_callback` はある。C++26 の `inplace_stop_token` 等は無い |
| `<syncstream>` | 33 | 11 | 21 | . | 1 | 33% | `osyncstream` は実装があるが、大半が未検証 |
| `<version>` | 8 | 3 | 5 | . | . | 38% | 実装した機能に対応する `__cpp_lib_*` だけを定義する |

### C++23 で入ったヘッダ

| ヘッダ | 項目 | ○ | △ | × | ― | ○率 | 備考 |
|---|--:|--:|--:|--:|--:|--:|---|
| `<expected>` | 58 | 14 | 44 | . | . | 24% | `expected` / `unexpected` の基本 (構築・値とエラーの取得・代入・swap・`error_or`・`expected<void, E>`) はある。monadic 操作 (`and_then` 等) は無い。`__cpp_lib_expected` を定義していないので、テストが確かめていない項目が多い |
| `<flat_map>` | 124 | 27 | 72 | . | 25 | 22% | 実装はあるが `__cpp_lib_flat_map` を定義していないので、テストの大半が回っていない |
| `<flat_set>` | 113 | 25 | 68 | . | 20 | 22% | 実装はあるが `__cpp_lib_flat_set` を定義していないので、テストの大半が回っていない |
| `<generator>` | 25 | . | 25 | . | . | 0% | **提供なし** (コルーチンが要る) |
| `<mdspan>` | 115 | . | 83 | . | 32 | 0% | **提供なし** (ヘッダはあるが中身が空) |
| `<print>` | 23 | . | 23 | . | . | 0% | **提供なし** (include すると `#error`) |
| `<spanstream>` | 45 | 38 | 1 | . | 6 | 84% | ある。代入・swap は右辺値参照が要るので無い |
| `<stacktrace>` | 56 | 38 | 18 | . | . | 68% | `stacktrace::current()` 等はある。**`-of+` を付けてビルドしないと空になる** (EBP フレームを辿るため)。`pmr::stacktrace` は無い |
| `<stdfloat>` | 10 | 1 | 9 | . | . | 10% | `float16_t` 等の拡張浮動小数型は無い (コンパイラに無い) |

### C++26 で入ったヘッダ

| ヘッダ | 項目 | ○ | △ | × | ― | ○率 | 備考 |
|---|--:|--:|--:|--:|--:|--:|---|
| `<contracts>` | 25 | . | 25 | . | . | 0% | **提供なし** |
| `<debugging>` | 5 | 4 | 1 | . | . | 80% | ある (`breakpoint` / `is_debugger_present` 等) |
| `<hazard_pointer>` | 19 | . | 19 | . | . | 0% | **提供なし** |
| `<hive>` | 77 | 45 | 32 | . | . | 58% | 基本操作はある。ブロック容量の指定 (`hive_limits` / `reshape`)・`assign`・`splice` / `sort` / `unique`・範囲系は無い |
| `<inplace_vector>` | 77 | 54 | 23 | . | . | 70% | 基本操作はある。move・`emplace(pos, ...)`・範囲系・`erase_if` は無い |
| `<linalg>` | 61 | . | 61 | . | . | 0% | **提供なし** |
| `<meta>` | 253 | . | 253 | . | . | 0% | **提供なし** |
| `<rcu>` | 13 | . | 13 | . | . | 0% | **提供なし** |
| `<simd>` | 189 | . | 189 | . | . | 0% | **提供なし** |
| `<text_encoding>` | 17 | 16 | 1 | . | . | 94% | ある |

## C のヘッダを C++ から使う

`<cxxx>` と `<xxx.h>` を並べた行は、両方で同じ数字になる (`<cwchar>` / `<wchar.h>` だけ違う。備考を参照)。
`<xxx.h>` を並べていない行は、C++ からは `<cxxx>` だけを測っている。
名前は `<cxxx>` なら `std::` に、`<xxx.h>` なら大域にも入る。

| ヘッダ | 項目 | ○ | △ | × | ― | ○率 | 備考 |
|---|--:|--:|--:|--:|--:|--:|---|
| `<cassert>` | 6 | 4 | 2 | . | . | 67% | C++26 の可変引数 `assert` は無い |
| `<ccomplex>` | 1 | 1 | . | . | . | 100% | `<complex>` を引くだけ (C++20 で削除) |
| `<cctype>` `<ctype.h>` | 16 | 16 | . | . | . | 100% | 全部 |
| `<cerrno>` | 75 | 75 | . | . | . | 100% | 全部。Watcom に無い C11 の errno 名 (`EADDRINUSE` 等) を補っている |
| `<cfenv>` `<fenv.h>` | 27 | 27 | . | . | . | 100% | 全部 |
| `<cfloat>` | 47 | 35 | 3 | . | 9 | 74% | `FLT_MAX` / `DBL_MIN` / `*_EPSILON` 等は Watcom のものがそのまま使えるが、テストの TU で Watcom が E172 を出すので未検証。`*_SNAN` (C23) は定数が書けないので無い (A17) |
| `<cinttypes>` `<inttypes.h>` | 50 | 31 | 19 | . | . | 62% | `SCN*MAX` / `SCN*PTR` と `imaxdiv_t` は未検証。C23 の 2 進 (`PRIb*` / `SCNb*`) は無い |
| `<ciso646>` | 1 | 1 | . | . | . | 100% | 全部 |
| `<climits>` | 38 | 38 | . | . | . | 100% | 全部 (C23 の `*_WIDTH` を含む) |
| `<clocale>` `<locale.h>` | 38 | 38 | . | . | . | 100% | 全部。`struct lconv` は C99 の `int_*` メンバまで持つ形に差し替えてある |
| `<cmath>` `<math.h>` | 292 | 194 | 52 | . | 46 | 66% | C99 由来の関数はほぼある (`erf` / `tgamma` / `nextafter` 等の一部は、`decltype` を使うテストなので未検証)。C++17 の特殊関数 (`beta` / `legendre` 等) と `lerp` もあるが、特殊関数は同じ理由で大半が未検証。`remainder` / `remquo` / `rint` は CRT の値が IEEE とずれる (A25) |
| `<csetjmp>` `<setjmp.h>` | 7 | 7 | . | . | . | 100% | 全部 |
| `<csignal>` `<signal.h>` | 14 | 14 | . | . | . | 100% | 全部 |
| `<cstdalign>` | 3 | 3 | . | . | . | 100% | 全部 (C++20 で削除) |
| `<cstdarg>` `<stdarg.h>` | 7 | 7 | . | . | . | 100% | 全部 |
| `<cstdbool>` | 2 | 2 | . | . | . | 100% | 全部 (C++20 で削除) |
| `<cstddef>` `<stddef.h>` | 24 | 22 | . | . | 2 | 92% | `nullptr_t` と `byte` は未検証 (Watcom には `nullptr` も `enum class` も無い) |
| `<cstdint>` `<stdint.h>` | 126 | 125 | 1 | . | . | 99% | ほぼ全部 |
| `<cstdio>` `<stdio.h>` | 141 | 138 | 3 | . | . | 98% | `printf` の `%a` は未検証 |
| `<cstdlib>` `<stdlib.h>` | 67 | 58 | 9 | . | . | 87% | `aligned_alloc` は無い (CRT の `free()` で解放できる形を作れない)。C23 の `strfromd` 等は無い |
| `<cstring>` `<string.h>` | 31 | 28 | 3 | . | . | 90% | C23 の `memset_explicit` / `strdup` / `strndup` は無い |
| `<ctgmath>` | 3 | 3 | . | . | . | 100% | 全部 (C++20 で削除) |
| `<ctime>` `<time.h>` | 36 | 30 | 6 | . | . | 83% | C11 の `timespec_get` まである。C23 の `gmtime_r` / `localtime_r` / `timespec_getres` は無い |
| `<cuchar>` `<uchar.h>` | 16 | 14 | 2 | . | . | 88% | ある。`__STDC_UTF_16__` / `__STDC_UTF_32__` は定義していない |
| `<cwchar>` `<wchar.h>` | 73 | 71 | . | 2 | . | 97% | ある。`<cwchar>` 側の × 2 件 (`swprintf` / `vswprintf` の `%f`) はリンク時の配置で出たり消えたりする CRT の不具合 (A24)。同じ回の `<wchar.h>` 側は ○ |
| `<cwctype>` `<wctype.h>` | 24 | 24 | . | . | . | 100% | 全部 |
| `<stdbit.h>` | 90 | 74 | 16 | . | . | 82% | C++26。ある。幅別の `stdc_has_single_bit_uc` / `stdc_bit_floor_*` / `stdc_bit_ceil_*` は無い |
| `<stdckdint.h>` | 4 | 3 | 1 | . | . | 75% | C++26。ある |

## C のヘッダを C から使う

「任意機能」は規格が任意としている項目 (複素数・アトミック・スレッド・`_FloatN`・
10 進浮動小数・Annex K など)。「項目」以降の列はそれを除いて数えた。

| ヘッダ | 項目 | ○ | △ | × | ― | ○率 | 任意機能 (○/項目) | 備考 |
|---|--:|--:|--:|--:|--:|--:|--:|---|
| `<assert.h>` | 6 | 3 | 3 | . | . | 50% | . | `static_assert` は C11 のキーワードなので、C90/C99 のコンパイラには足せない |
| `<complex.h>` | 0 | . | . | . | . | - | 0/82 | C の複素数型 (`_Complex`) がコンパイラに無いので使えない。C++ の `<complex>` を使う |
| `<ctype.h>` | 17 | 17 | . | . | . | 100% | . | 全部 |
| `<errno.h>` | 7 | 7 | . | . | . | 100% | 1/1 | 全部 |
| `<fenv.h>` | 26 | . | 11 | . | 15 | 0% | 0/18 | Watcom の `<fenv.h>` は C99 モード (`-za99`) でないと中身が空なので、既定の方言では `#error` で止める。この測定は既定の方言なので未検証 |
| `<float.h>` | 52 | 49 | 3 | . | . | 94% | 0/28 | 任意機能 (10 進浮動小数) を除けば、`*_SNAN` (C23) 以外は全部 |
| `<inttypes.h>` | 204 | 161 | 43 | . | . | 79% | . | C99 の範囲は全部。C23 の 2 進 (`PRIb*` / `SCNb*`) は無い |
| `<iso646.h>` | 11 | 11 | . | . | . | 100% | . | 全部 |
| `<limits.h>` | 35 | 32 | 3 | . | . | 91% | . | C23 の `BOOL_MAX` と `_BitInt` 関連は無い |
| `<locale.h>` | 19 | 13 | 6 | . | . | 68% | . | `struct lconv` の `int_*` (C99) は提供しているが、C 側のテストが確かめていない (C++ 側では ○) |
| `<math.h>` | 339 | 207 | 132 | . | . | 61% | 0/394 | C99 まではほぼ全部。C23 の追加は `roundeven` / `fmaximum` 系 / `issignaling` 等の一部だけで、`acospi` 等の π 系・`rootn` / `pown` 等は無い。任意機能 (`_FloatN` / 10 進) は無い。`remainder` / `remquo` / `rint` は CRT の値が IEEE とずれる (A25) |
| `<setjmp.h>` | 6 | 5 | 1 | . | . | 83% | . | 全部 (△ は C23 の版数マクロ) |
| `<signal.h>` | 14 | 14 | . | . | . | 100% | . | 全部 |
| `<stdalign.h>` | 6 | 4 | 2 | . | . | 67% | . | `alignas` は受け付けるが、局所変数には効かない (A33) |
| `<stdarg.h>` | 9 | 7 | 2 | . | . | 78% | . | C23 の 1 引数 `va_start` は無い |
| `<stdatomic.h>` | 0 | . | . | . | . | - | 0/90 | C からは使えない (`_Atomic` がコンパイラに無い)。C++ から `<atomic>` を使う |
| `<stdbit.h>` | 103 | 103 | . | . | . | 100% | . | 全部 |
| `<stdbool.h>` | 4 | 4 | . | . | . | 100% | . | 全部 |
| `<stdckdint.h>` | 7 | 7 | . | . | . | 100% | . | 全部 |
| `<stddef.h>` | 10 | 6 | 4 | . | . | 60% | 0/1 | `max_align_t` (C11) と C23 の `nullptr_t` / `unreachable` は、コンパイラの C 方言が古いので無い |
| `<stdint.h>` | 81 | 81 | . | . | . | 100% | 44/44 | 全部 |
| `<stdio.h>` | 280 | 249 | 30 | 1 | . | 89% | 19/23 | C99 まではほぼ全部。`sscanf` の `%l[...]` が誤る CRT の不具合で × (A44')。scanf 側に `%a` が無い。C23 の `%b` 等は無い |
| `<stdlib.h>` | 76 | 61 | 15 | . | . | 80% | 1/18 | `aligned_alloc` (C11) は無い。C23 の `strfromd` / `free_sized` 等は無い |
| `<stdnoreturn.h>` | 2 | 2 | . | . | . | 100% | . | 全部 |
| `<string.h>` | 39 | 36 | 3 | . | . | 92% | 1/13 | C23 の `memset_explicit` / `strdup` / `strndup` は無い。Annex K (任意) は対象外 |
| `<tgmath.h>` | 109 | 36 | 59 | . | 14 | 33% | 0/16 | `<tgmath.h>` は無い (C に `_Generic` が無い。A16)。○ は `<math.h>` の double 版を呼んで確かめた分 |
| `<threads.h>` | 0 | . | . | . | . | - | 43/46 | C11 のスレッドを Win32 上に実装。`mtx_timedlock` / `cnd_timedwait` と `thread_local` は無い。DOS では使えない |
| `<time.h>` | 37 | 33 | 4 | . | . | 89% | 1/9 | C11 の `timespec_get` まである。C23 の `gmtime_r` / `localtime_r` / `timespec_getres` は無い |
| `<uchar.h>` | 15 | 15 | . | . | . | 100% | . | 全部 |
| `<wchar.h>` | 267 | 223 | 20 | 24 | . | 84% | 1/30 | × は CRT の不具合。ワイド書式の I/O でナロー文字列 (`%s` / `%c` / `%[`) の変換が誤る (A44)、`swscanf` に `%a` が無い、`swprintf` の `%a` がリンク時の配置で出たり消えたりする (A24)。C11 以降の追加は未検証が多い |
| `<wctype.h>` | 23 | 23 | . | . | . | 100% | . | 全部 |

## ターゲット・構成による違い

### DOS (`watcom1300-dos-x86-cxx03-ccw26` / `watcom1300-dos-x86-c1990-ccw26`)

**スレッドを使うヘッダは DOS では使えない**: `<thread>` / `<mutex>` / `<shared_mutex>` /
`<condition_variable>` / `<future>` / `<latch>` / `<barrier>` / `<semaphore>` /
`<stop_token>` と、C の `<threads.h>`。スタックを Win32 API で取る `<stacktrace>` と
`<debugging>` も使えない。

`filesystem::path::value_type` は DOS では `char` になる (Windows では `wchar_t`)。

ほかのヘッダも Win32 より ○ が少し少ないが、DOS の測定は 2026-08-16 で Win32 より古く、
差のどこまでが DOS に固有かは切り分けていない。

### 例外・RTTI を無効にしたビルド (`watcom-noeh`)

`-xs` も `-xr` も付けない構成 (2026-08-30 測定)。

- RTTI が無いので `<typeinfo>` は使えない (`typeid` / `dynamic_cast` が無い)。
- ライブラリが例外を投げる場面では、そのことを stderr に出して `abort()` する。
- 例外を投げる経路を試すテストは外れるので、その分 ○ が減る
  (`<locale>` 303 → 117、`<regex>` 199 → 172、`<optional>` 43 → 23、`<exception>` 30 → 11 など)。
