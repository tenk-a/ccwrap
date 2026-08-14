---
paths:
  - "watcom/**"
  - "detail/llibcxx03/**"
---

# Open Watcom ターゲット

作業方針は `doc/watcom/watcom_work.md`。

**次に何をやるか / どう進めるかは `doc/watcom/watcom_plan.md` に書いてある。**
1ヘッダ = 1サイクル（移植 → Watcom で通す → テスト → base 更新 → コミット）で、
**確認を求めずに作業が無くなるまで進めてよい。** 残作業は
`python3 test/scripts/spec_gap.py cxx` で出る（規格にあってテストが無い項目）。
手書きのチェックリスト (`doc/checklist_watcom`) と `bin/lib_doc_status.py` は
**退役済み**。判定の仕組みは `.claude/rules/test.md`「進捗と網羅の判定 = spec / base」。

Open Watcom は**コンパイラのバグ・制約が非常に多い**。実装前に
`doc/watcom/ccwrap_watcom_bugs.md`（A: コード生成バグ / B: 言語制約 /
C: ツールチェインの癖）に目を通し、既知の落とし穴を踏まないこと。
**新しいバグに出会ったら、そのカタログに追記する。**

特徴的な作業をしたら `doc/watcom/watcom_ai_memo.md` に追記する。

@../../doc/watcom/watcom_plan.md
@../../doc/watcom/watcom_work.md
@../../doc/watcom/ccwrap_watcom_bugs.md
