# ネガティブテスト (compile-fail tests)

**「コンパイルが通らないこと」を確認するテスト**を置くフォルダ。

`test/cxx_test/` の通常テストは実行時にチェックするので、
**「この呼び出しは ill-formed でなければならない」という保証を検証できない**。
たとえば規格は `std::gcd(true, false)` を不適格と定めているが、これは実行して
確かめられない。制約が消えても実行時テストは全部 green のままになる。

## 実行

```
test\run_watcom.bat cfail     ネガティブテストだけ
test\run_watcom.bat htest     通常テスト + ネガティブテスト (htest に含まれる)
```

`[Result] compile-fail tests: N checks, all ok.` が出れば成功。
失敗時は `COMPILE_FAIL_TEST:` 行に理由が出て、終了コードが 1 になる。

## 書き方

1ケース = `X.cpp` + `X.lst` の 2 ファイル。

- **`X.cpp`** … マクロ未定義で **必ずコンパイルが通ること**。
  弾かせたいコードは `#ifdef REJ_XXX` … `#endif` で囲む。
- **`X.lst`** … マクロ名を1行に1つ。各マクロを `-d` で定義したとき
  **コンパイルが失敗しなければならない**。`#` 始まりの行はコメント。

`X.cpp` にはマクロ無しで通る **健全性チェック**（同じ関数を非 bool で呼ぶ等）も
入れておくこと。これが無いと、ファイルが無関係な理由で壊れたときに全ケースが
「弾けた」と誤って成功してしまう。

```cpp
// X.cpp
#include <numeric>
int main() {
    int ok = std::gcd(12, 18);   // 健全性: これは常に通らないといけない
    (void)ok;
#ifdef REJ_GCD
    bool t = true, f = false;
    std::gcd(t, f);              // 規格は bool を除外 -> 弾かれるべき
#endif
    return 0;
}
```

```
# X.lst
REJ_GCD
```

新規ファイルは glob で拾われるので、バッチへの追記は不要。

## 現在のケース

| ファイル | 内容 |
| --- | --- |
| `numeric_bool.cpp` | `gcd` / `lcm` / `midpoint` / `add_sat` / `sub_sat` / `mul_sat` / `div_sat` / `saturate_cast` が **bool を弾く**こと。Open Watcom は SFINAE/concepts が無い (B1) ので、戻り値型に `typename __ccw_not_bool<T>::type` を置く代用で候補を拒否させている (`libcxx03/include/__numeric/arith_constraint.h`)。この代用が効いているかは実行時には見えないため、ここで検証する |

## 注意

これは **Watcom 用の仕組み**。`test/cxx_test/` と違い gcc/vc とは共有していない
（`no` 判定の根拠が Watcom 固有の代用実装なので）。他コンパイラにも要るように
なったら、その時点で共通化する。
