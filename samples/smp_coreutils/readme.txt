smp_coreutils

GNU 風の ls / sort / uniq / wc。ccwrap のサンプル。
4 本を 1 つの CMakeLists.txt でまとめてビルドする。

構成:
  CMakeLists.txt   4 本ぶんの実行ファイルを作る
  bld.bat          コンパイラを選んで configure から install まで回す
  src/ls.cpp       各ツールの本体 (1 ツール 1 ファイル、共有ソースは無い)
  src/sort.cpp
  src/uniq.cpp
  src/wc.cpp
  src/mk.bat       cl だけで手早く 4 本作るとき用
  src/win/         Windows のコードページを UTF-8 にするマニフェスト / リソース

ビルド:
  bld.bat watcom          Open Watcom 2.0 (ccwrap + watcom/lib/nt/ccw-eh3r.lib)
  bld.bat vc120           Visual C++ 2013 (ccwrap の <filesystem>)
  bld.bat vc141 x64 nmake Visual C++ 2017 以降 (native <filesystem>)
  bld.bat vc143 x64       Visual C++ 2022

  出力は bld/<構成>/ に作られ、bin/<構成>/ に install される。

watcom と vc120 以下は ccwrap のヘッダを使う。watcom は先に
watcom/lib/gen.bat でプリビルドライブラリ (nt/ccw-eh3r.lib) を作っておくこと。
INCLUDE に %WATCOM%\h と %WATCOM%\h\nt の両方が要る (windows.h を使うため)。
ls だけは vc141 未満で ccwrap の <filesystem> 実装 (detail/llibcxx03/src/filesystem.cpp)
を一緒にコンパイルする。他の 3 本は追加ソースを要らない。

詳細は doc/src/samples/smp_coreutils/src/<ツール名>.cpp.md。

--------------------------------------------------------------------
ls

ファイル一覧ツール。

対応オプション (GNU coreutils の ls の部分集合):
  -a -A -B -c -C -d -f -F -g -G -h -H -i -I -k -l -L -m -n -o -p -q -Q
  -r -R -s -S -t -T -u -U -v -w -x -X -1
  --all --almost-all --block-size --classify --color[=WHEN] --dereference
  --dereference-command-line --directory --format=WORD --full-time
  --group-directories-first --help --hide=PATTERN --hide-control-chars
  --human-readable --ignore=PATTERN --ignore-backups --indicator-style=WORD
  --inode --kibibytes --no-group --numeric-uid-gid --quote-name
  --quoting-style=WORD --recursive --reverse --si --size --sort=WORD
  --tabsize=COLS --time=WORD --time-style=STYLE --version --width=COLS

出力 (-C -x -m の桁揃え、-F の記号、引用、並べ替え) は GNU ls と一致する。

GNU との相違:
  - 名前の並べ替えはバイト順 (GNU の LC_COLLATE=C 相当)。
  - Windows では st_blocks が無いので -s / total は (サイズ+1023)/1024 の概算。
    inode (-i) は 0、所有者は環境変数 USERNAME、グループは数値 gid。
    -c の時刻は作成時刻 (CRT の st_ctime がそれ)。
  - LS_COLORS は読まない (--color は内蔵の配色)。
  - 長い名前のオプションは完全名のみ (--rec のような省略は不可)。
  - ccwrap 経由の Windows ビルドではシンボリックリンクを判別しない
    (ccwrap の <filesystem> が Windows で symlink_status を持たないため)。

--------------------------------------------------------------------
sort

行ソート。

対応オプション:
  並べ替え: -b -d -f -g -h -i -M -n -r -V (と長い名前)
  その他:   -c/-C/--check[=quiet]  -k/--key=KEYDEF  -m/--merge  -o/--output=FILE
            -s/--stable  -t/--field-separator=SEP  -u/--unique
            -z/--zero-terminated  --help  --version
            -S -T --parallel --compress-program --batch-size (受け取って無視)

  KEYDEF は F[.C][OPTS][,F[.C][OPTS]]。OPTS は bdfghinrV。
  グローバル指定の継承規則も GNU と同じ (独自の並べ替え指定を持つキーは
  グローバル指定を継承しない。よって `sort -r -k1n` は -r がキーに効かない)。

GNU との相違:
  - 全部メモリに読み込んでから並べる (外部ソートをしない)。-m は「全部読んで
    並べ直す」ので、入力がソート済みでない場合に GNU と結果が変わる。
  - -R/--random-sort、--files0-from、--debug は未対応。
  - 照合はバイト順 (GNU の LC_COLLATE=C 相当)。
  - -g は 256 バイトまでを strtod に渡す。NaN の順序は GNU と異なる。
  - 千区切りや小数点はロケールを見ない ('.' 固定)。

--------------------------------------------------------------------
uniq

「隣接する重複行」フィルタ。

対応オプション:
  -c/--count  -d/--repeated  -D  --all-repeated[=none|prepend|separate]
  -f/--skip-fields=N  -s/--skip-chars=N  -w/--check-chars=N
  -i/--ignore-case  -u/--unique  -z/--zero-terminated
  --group[=separate|prepend|append|both]  --help  --version
  操作対象は [INPUT [OUTPUT]] の 2 つまで (GNU と同じ)。

GNU との相違:
  - -i の大文字小文字は ASCII だけを畳む (ロケールの多バイト文字は畳まない)。
  - 入出力はバイナリモードで扱う (改行の変換をしない)。

--------------------------------------------------------------------
wc

行数・単語数・文字数・バイト数カウンタ。

対応オプション:
  -c/--bytes  -m/--chars  -l/--lines  -w/--words  -L/--max-line-length
  --files0-from=F  --total=WHEN (auto/always/only/never)  --help  --version

出力の桁揃え (単一ファイルは詰める、複数ファイルは合計バイト数の桁、
サイズ不明な入力は 7 桁) と total 行は GNU と同じ。

GNU との相違:
  - -m と -L は常に UTF-8 として数える (GNU は LC_CTYPE 依存。UTF-8 ロケールの
    GNU と一致する)。-L のタブ幅 8、全角 2 桁は GNU と同じ。
  - 入出力はバイナリモードで扱う。CRLF の \r は 1 バイトとして数え、
    改行の変換はしない。
