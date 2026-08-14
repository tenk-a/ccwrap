---
paths:
  - "vc/**"
  - "gcc/**"
  - "watcom/**"
  - "detail/**"
---

# ccwrap 実装ルール

実装前に必ず `doc/ccwrap_impl_rule.md` を読むこと
（ネーミング規約、`_CCW_TARGET_CXX`、ヘッダでの宣言・定義、テストの求め方）。

**進捗と残作業は spec / base で見る**（手書きのチェックリストは退役済み）。
`python3 test/scripts/spec_gap.py cxx` = 規格にあってテストが無い項目。
仕組みは `.claude/rules/test.md`「進捗と網羅の判定 = spec / base」。

@../../doc/ccwrap_impl_rule.md
