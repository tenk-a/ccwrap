# watcom_bugchk — Watcom バグ・制約の再検証ハーネス

`doc/watcom/ccwrap_watcom_bugs.md` に記録した Open Watcom のバグ・制約が、**新しい
Watcom でまだ生きているか**を機械的に確かめる。Watcom を入れ替えたら、まずこれを流す。

ccwrap のコードは踏まない。**素のコンパイラだけ**を叩くので、ここが赤くなったら
原因は Watcom 側にある。

## 使い方

```sh
# 既定 (C:\WATCOM) で全プローブ
python3 test/etc_test/watcom/watcom_bugchk/bugchk.py

# 新旧を比較する ← これが本命。「治った」を推測でなく確定できる
python3 test/etc_test/watcom/watcom_bugchk/bugchk.py --watcom "C:\WATCOM" --compare "C:\WATCOM2.0.pre"

# 一部だけ
python3 test/etc_test/watcom/watcom_bugchk/bugchk.py --only A26,B07
```

`--compare` には**入れ替える前の Watcom** を渡す。Watcom を更新するときは古いツリーを
`C:\WATCOM2.0.pre` のように残しておくこと。これが無いと「治った」と「そもそも私の
プローブが再現していなかった」を区別できない。

## 判定モデル

**全プローブは「規格どおりに正しい」コード**で、正しい動作を自分で検証する。だから
結果は 2 値しかない:

| 結果 | 意味 |
| --- | --- |
| `ok` | コンパイル (と実行) が通った = **バグは無い / 機能が在る** |
| `bug` | 落ちた = **バグ・制約が再現した** |

各プローブはヘッダコメントで、カタログの現在の主張を宣言する:

```c
// bugchk-id:     A26
// bugchk-kind:   compile | run
// bugchk-expect: bug | ok
// bugchk-desc:   一行説明
// bugchk-flags:  -d2            (任意。追加のコンパイルオプション)
```

`expect` と実測が食い違うと、runner が大きく報告する:

| verdict | 意味 | やること |
| --- | --- | --- |
| `as-catalogued` | カタログどおり | 何も無し |
| `FIXED` | 旧 `bug` → 新 `ok` | **カタログを更新し、回避策を外せないか見直す** |
| `no-repro` | 新旧とも `ok` | プローブがバグを捉えていない。信用しない (下記) |
| `REGRESSION` | `expect: ok` が `bug` に | 新しい Watcom が何かを壊した |

runner の終了コードは REGRESSION の数。

### `no-repro` と GUARD について

プローブはバグの**再構成**なので、`ok` が出ただけでは治った証拠にならない。
**旧コンパイラでも `ok`** なら、そのプローブは最初から何も捉えていない。

カタログにあるが最小形で再現しなかったものは、`expect: ok` の **GUARD** に変えてある
(desc に `GUARD:` と明記)。今は通るという事実だけを固定し、将来壊れたら
REGRESSION として気づけるようにする用途。カタログの記述が**誤り**という意味ではない
— 元の失敗が、より複雑な文脈でしか出ない形だった可能性が高い。該当: A01 A04 A11
A11b A13 A23 B08 B09。

## 対象アーキテクチャ

Open Watcom は **x86 32bit のコード生成しか持たない** (amd64 用の出力は無い)。
プローブは `wpp386` / `wcc386` / `wcl386` を `-bt=nt` で使う = 32bit x86 / Win32。
`binnt64` を使うのは**コンパイラ自身が 64bit ホストのビルド**というだけで、
生成コードとは無関係。

## プローブを足すとき

1. `probes/<ID>_<短い名前>.cpp` (C のバグなら `.c`) を作る。
2. ヘッダコメントに `bugchk-id` / `kind` / `expect` / `desc` を書く。
3. **正しい動作を assert する**こと。`kind: run` なら、正しければ exit 0、
   バグが出たら非 0 を返し、`BROKEN` を含む行を出す (runner が拾って表示する)。
4. C プローブはブロックコメントでよい (runner は `//` と `/* */` の両方を読む)。
5. **TU をまたがないと出ないバグ** (修飾名が TU ローカル、等) は、2 本目のソースを
   `probes/extra/` に置いて `// bugchk-aux: extra/<名前>.cpp` と書く。runner が同じ
   コマンドラインに足してリンクまでやる。`probes/` 直下の glob には引っかからない
   ので、その 2 本目がプローブとして走ることはない。実例: A51。

落とし穴:

- `near` / `far` / `huge` / `__min` / `__max` は使えない (A15 / B11)。ヘルパは
  `near_eq` のように改名する。**プローブ自身が別のバグを踏むと結果が濁る。**
- `-xs` は C++ 専用。C で渡すと wcl386 が `-x` + ファイル名 `s.c` と解釈する。
- カンマを含むテンプレート実引数 (`ratio<2,4>`) をマクロに直接渡さない。
  先に `typedef` する。

## カバー範囲

`doc/watcom/ccwrap_watcom_bugs.md` の A / B / C 全項目のうち、自動化できるもの。
自動化していないもの:

- **A02 / A02'** — 再帰深さの設計判断であってバグではない。
- **C01** — OW1.9 のヘッダ解決。1.9 は対象外。
- **C02** — `wcl386` が `.cpp` と `.obj` を同じ行に渡すと `.obj` を落とす。
  複数ファイルが要るので未自動化 (`test/run_watcom.bat` が二段ビルドで回避済み)。
- **C03** — `windows.h` を強制コンフィグ下で通すとプリプロセッサが**停止する**。
  自動実行に混ぜると固まるので入れない。
- **C04** — ccwrap 自身の設計ミス (C ヘッダから C++ ヘッダを include) であって
  Watcom のバグではない。`test\run_watcom.bat win` が担保する。
