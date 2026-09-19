#watcom sample ビルド

c++03 でビルドできる オープンソースの古いバージョンを
いくつかビルドを試す環境.

open watcom 2.0 beta + ccwrap 用の修正は
ほぼ AI (claude code) まかせ.


clone_for_watcom.bat で、修正済みの git リポジトリを clone.

owSetEnv4ccw.bat か setcc.bat 等で、
ccwap のパスを設定した open-watcom 環境変数を設定後、

各フォルダの build_watcom.bat を実行.
