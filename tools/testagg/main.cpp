/*
 *  testagg -- ccwrap テスト pass ログ集計ツール (spec / base モデル)
 *
 *  「確度の分離」: 標準 API 項目の一覧 (spec) と、その項目にテストがあるか (base)
 *  を別フォルダで持つ。spec が分母 (規格そのもの)、base が分子 (カバレッジ).
 *  no-test-yet は保存せず spec - base で算出する (状態が腐らない).
 *
 *  複数コンパイラの pass ログ (<suite>.tsv: suite/pass/result/note) を横に連結し,
 *  ヘッダ(suite)ごとの Markdown 表と index を生成する.
 *
 *  使い方:
 *    testagg -o OUTDIR [--spec DIR] [--tested DIR] [書込オプション] RESULTDIR...
 *      -o OUTDIR         : レポート出力先 (index.md と <suite>.md)
 *      --spec DIR        : spec マスター (項目一覧。列 suite/pass/since/until/status/note)
 *      --tested DIR      : base (テスト状態表。列 suite/pass/status/note)
 *      RESULTDIR...      : 各 1 コンパイラの pass ログdir。dir 名 = コンパイララベル.
 *
 *    書込オプション (いずれも非破壊: 既存行を消さない。削除は人が status=deleted で):
 *      --update-spec     : 結果に現れ spec に無い pass を spec に追記のみ (dedup)
 *      --update-tested   : 結果から base を union 更新 (pass->tested / skip->skip /
 *                          fail->fail を追加・昇格。既存 tested は降格しない。行は消さない)
 *      --spec-out DIR    : spec の書込先 (既定 = --spec。マスターを触らず別フォルダに出す用)
 *      --tested-out DIR  : base の書込先 (既定 = --tested)
 *      --suggest         : 報告のみ (書き込まない):
 *                            gap  = spec にありテスト無し (穴)
 *                            orphan = 結果/tested にあり spec に無い (spec 追加候補)
 *                            stale  = base にあり今回どの結果にも無い (削除マーク候補)
 *
 *  【重要】base (spec/base) は情報の確度を高める方向にしか変えない。行の削除は.
 *  ツールでは行わず、人が status=deleted (理由つき) を書く。drift は --suggest で報告のみ.
 *
 *  C++03 文法の範囲で書く (ccwrap でビルド可能に)。実運用は生の vc14.5 でビルド.
 */
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <windows.h>
#include <direct.h>

typedef std::string                          Str;
typedef std::vector<Str>                      StrVec;

/* 1 セル = 1 コンパイラの 1 pass の結果 */
struct Cell {
    Str result;   /* "pass" / "fail" / "skip" / "" (無し) */
    Str note;
    Cell() {}
};

typedef std::map<Str, std::vector<Cell> >     SuiteRows;   /* pass -> per-compiler cells */

/* spec の 1 行 */
struct SpecRow {
    Str since, until, status, note;
    SpecRow() {}
};
/* base の 1 行 */
struct TestedRow {
    Str status, note;   /* status = tested / skip / fail */
    TestedRow() {}
};

/* ---- 小道具 ------------------------------------------------------------ */

static Str basename_no_slash(const Str& path) {
    Str p = path;
    while (!p.empty() && (p[p.size() - 1] == '/' || p[p.size() - 1] == '\\'))
        p.erase(p.size() - 1);
    Str::size_type s = p.find_last_of("/\\");
    return (s == Str::npos) ? p : p.substr(s + 1);
}

static Str unquote(const Str& f) {
    Str s = f;
    if (s.size() >= 2 && s[0] == '"' && s[s.size() - 1] == '"')
        s = s.substr(1, s.size() - 2);
    return s;
}

static StrVec split_tab(const Str& line) {
    StrVec out;
    Str::size_type b = 0, e;
    for (;;) {
        e = line.find('\t', b);
        if (e == Str::npos) { out.push_back(line.substr(b)); break; }
        out.push_back(line.substr(b, e - b));
        b = e + 1;
    }
    return out;
}

static StrVec list_tsv(const Str& dir) {
    StrVec out;
    Str pat = dir + "\\*.tsv";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE)
        return out;
    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            out.push_back(Str(fd.cFileName));
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return out;
}

static Str suite_of_file(const Str& fname) {
    Str s = fname;
    if (s.size() >= 4 && s.substr(s.size() - 4) == ".tsv")
        s = s.substr(0, s.size() - 4);
    return s;
}

static Str q(const Str& s) { return Str("\"") + s + "\""; }

/* pass ログ tsv を読む。pass -> Cell を返す (header 行と空行は無視)。 */
static void read_result_tsv(const Str& path, std::map<Str, Cell>& out) {
    std::ifstream ifs(path.c_str());
    if (!ifs) return;
    Str line;
    while (std::getline(ifs, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line.empty()) continue;
        StrVec f = split_tab(line);
        if (f.size() < 2) continue;
        Str pass = unquote(f[1]);
        if (pass.empty() || pass == "pass") continue;
        Cell c;
        if (f.size() >= 3) c.result = unquote(f[2]);
        if (f.size() >= 4) c.note   = unquote(f[3]);
        out[pass] = c;
    }
}

/* spec tsv を読む: pass -> SpecRow。 */
static void read_spec(const Str& path, std::map<Str, SpecRow>& out) {
    std::ifstream ifs(path.c_str());
    if (!ifs) return;
    Str line;
    while (std::getline(ifs, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line.empty()) continue;
        StrVec f = split_tab(line);
        if (f.size() < 2) continue;
        Str pass = unquote(f[1]);
        if (pass.empty() || pass == "pass") continue;
        SpecRow r;
        if (f.size() >= 3) r.since  = unquote(f[2]);
        if (f.size() >= 4) r.until  = unquote(f[3]);
        if (f.size() >= 5) r.status = unquote(f[4]);
        if (f.size() >= 6) r.note   = unquote(f[5]);
        out[pass] = r;   /* dedup: 後勝ち */
    }
}

/* base tsv を読む: pass -> TestedRow。 */
static void read_tested(const Str& path, std::map<Str, TestedRow>& out) {
    std::ifstream ifs(path.c_str());
    if (!ifs) return;
    Str line;
    while (std::getline(ifs, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line.empty()) continue;
        StrVec f = split_tab(line);
        if (f.size() < 2) continue;
        Str pass = unquote(f[1]);
        if (pass.empty() || pass == "pass") continue;
        TestedRow r;
        if (f.size() >= 3) r.status = unquote(f[2]);
        if (f.size() >= 4) r.note   = unquote(f[3]);
        out[pass] = r;
    }
}

static void write_spec(const Str& path, const Str& suite,
                       const std::map<Str, SpecRow>& rows) {
    std::ofstream o(path.c_str());
    o << q("suite") << "\t" << q("pass") << "\t" << q("since") << "\t"
      << q("until") << "\t" << q("status") << "\t" << q("note") << "\n";
    for (std::map<Str, SpecRow>::const_iterator it = rows.begin(); it != rows.end(); ++it)
        o << q(suite) << "\t" << q(it->first) << "\t" << q(it->second.since) << "\t"
          << q(it->second.until) << "\t" << q(it->second.status) << "\t"
          << q(it->second.note) << "\n";
}

static void write_tested(const Str& path, const Str& suite,
                         const std::map<Str, TestedRow>& rows) {
    std::ofstream o(path.c_str());
    o << q("suite") << "\t" << q("pass") << "\t" << q("status") << "\t" << q("note") << "\n";
    for (std::map<Str, TestedRow>::const_iterator it = rows.begin(); it != rows.end(); ++it)
        o << q(suite) << "\t" << q(it->first) << "\t" << q(it->second.status) << "\t"
          << q(it->second.note) << "\n";
}

static Str md_escape(const Str& s) {
    Str o;
    for (Str::size_type i = 0; i < s.size(); ++i) {
        if (s[i] == '|') o += "\\|";
        else             o += s[i];
    }
    return o;
}

/* 結果セル群から今回の集約 status を決める (tested > skip > fail > "")。 */
static Str agg_status(const std::vector<Cell>& cells) {
    bool ok = false, sk = false, ng = false;
    for (Str::size_type i = 0; i < cells.size(); ++i) {
        const Str& r = cells[i].result;
        if      (r == "pass") ok = true;
        else if (r == "skip") sk = true;
        else if (r == "fail") ng = true;
    }
    if (ok)  return "tested";
    if (sk)  return "skip";
    if (ng)  return "fail";
    return "";
}

/* ---- main -------------------------------------------------------------- */

int main(int argc, char** argv) {
    Str out_dir, spec_dir, tested_dir, spec_out, tested_out;
    bool update_spec = false, update_tested = false, suggest = false;
    StrVec result_dirs;

    for (int i = 1; i < argc; ++i) {
        Str a = argv[i];
        if      (a == "-o"           && i + 1 < argc) out_dir    = argv[++i];
        else if (a == "--spec"       && i + 1 < argc) spec_dir   = argv[++i];
        else if (a == "--tested"     && i + 1 < argc) tested_dir = argv[++i];
        else if (a == "--spec-out"   && i + 1 < argc) spec_out   = argv[++i];
        else if (a == "--tested-out" && i + 1 < argc) tested_out = argv[++i];
        else if (a == "--update-spec")                update_spec   = true;
        else if (a == "--update-tested")              update_tested = true;
        else if (a == "--suggest")                    suggest       = true;
        /* 後方互換: 旧 -b/--update-base/--regen-base は廃止。誤用防止に弾く。 */
        else if (a == "-b" || a == "--update-base" || a == "--regen-base") {
            std::fprintf(stderr, "testagg: '%s' is removed (spec/base model). "
                         "Use --spec/--tested with --update-spec/--update-tested/--suggest.\n",
                         a.c_str());
            return 2;
        }
        else if (!a.empty() && a[0] == '-') {
            std::fprintf(stderr, "testagg: unknown option '%s'\n", a.c_str());
            return 2;
        } else result_dirs.push_back(a);
    }
    if (out_dir.empty() || result_dirs.empty()) {
        std::fprintf(stderr,
            "usage: testagg -o OUTDIR [--spec DIR] [--tested DIR]\n"
            "               [--update-spec] [--update-tested] [--suggest]\n"
            "               [--spec-out DIR] [--tested-out DIR] RESULTDIR...\n");
        return 2;
    }
    if (spec_out.empty())   spec_out   = spec_dir;
    if (tested_out.empty()) tested_out = tested_dir;
    _mkdir(out_dir.c_str());

    StrVec comps;
    for (Str::size_type i = 0; i < result_dirs.size(); ++i)
        comps.push_back(basename_no_slash(result_dirs[i]));

    std::set<Str> suites;

    /* spec: suite -> (pass -> SpecRow) */
    std::map<Str, std::map<Str, SpecRow> > spec;
    if (!spec_dir.empty()) {
        StrVec sf = list_tsv(spec_dir);
        for (Str::size_type i = 0; i < sf.size(); ++i) {
            Str suite = suite_of_file(sf[i]);
            read_spec(spec_dir + "\\" + sf[i], spec[suite]);
            suites.insert(suite);
        }
    }
    /* base: suite -> (pass -> TestedRow) */
    std::map<Str, std::map<Str, TestedRow> > tested;
    if (!tested_dir.empty()) {
        StrVec tf = list_tsv(tested_dir);
        for (Str::size_type i = 0; i < tf.size(); ++i) {
            Str suite = suite_of_file(tf[i]);
            read_tested(tested_dir + "\\" + tf[i], tested[suite]);
            suites.insert(suite);
        }
    }

    /* results: suite -> pass -> per-compiler Cell */
    std::map<Str, SuiteRows> data;
    for (Str::size_type ci = 0; ci < result_dirs.size(); ++ci) {
        StrVec rf = list_tsv(result_dirs[ci]);
        for (Str::size_type i = 0; i < rf.size(); ++i) {
            Str suite = suite_of_file(rf[i]);
            suites.insert(suite);
            std::map<Str, Cell> rows;
            read_result_tsv(result_dirs[ci] + "\\" + rf[i], rows);
            SuiteRows& sr = data[suite];
            for (std::map<Str, Cell>::iterator it = rows.begin(); it != rows.end(); ++it) {
                SuiteRows::iterator f = sr.find(it->first);
                if (f == sr.end()) {
                    std::vector<Cell> v(comps.size());
                    v[ci] = it->second;
                    sr[it->first] = v;
                } else {
                    f->second[ci] = it->second;
                }
            }
        }
    }

    /* ---- レポート生成 -------------------------------------------------- */
    Str index_path = out_dir + "\\index.md";
    std::ofstream idx(index_path.c_str());
    idx << "# Test result summary\n\nCompilers: ";
    for (Str::size_type i = 0; i < comps.size(); ++i) { if (i) idx << ", "; idx << comps[i]; }
    idx << "\n\n";
    idx << "| Header | spec | tested | skip | gap | ";
    for (Str::size_type i = 0; i < comps.size(); ++i) idx << comps[i] << " (pass/fail/skip/-) | ";
    idx << "\n|---|---|---|---|---|";
    for (Str::size_type i = 0; i < comps.size(); ++i) idx << "---|";
    idx << "\n";

    /* --suggest / 更新の集計 */
    unsigned tot_gap = 0, tot_orphan = 0, tot_stale = 0, tot_added_spec = 0, tot_upd_tested = 0;
    Str sugg = "# Suggestions\n\n";

    for (std::set<Str>::iterator sit = suites.begin(); sit != suites.end(); ++sit) {
        Str suite = *sit;
        SuiteRows& sr = data[suite];
        std::map<Str, SpecRow>&   sp = spec[suite];
        std::map<Str, TestedRow>& ts = tested[suite];

        /* 全 pass = spec ∪ tested ∪ 結果 */
        std::set<Str> all_pass;
        for (std::map<Str, SpecRow>::iterator p = sp.begin(); p != sp.end(); ++p) all_pass.insert(p->first);
        for (std::map<Str, TestedRow>::iterator p = ts.begin(); p != ts.end(); ++p) all_pass.insert(p->first);
        for (SuiteRows::iterator r = sr.begin(); r != sr.end(); ++r) all_pass.insert(r->first);

        std::vector<int> c_ok(comps.size(), 0), c_nok(comps.size(), 0),
                         c_skip(comps.size(), 0), c_miss(comps.size(), 0);
        unsigned s_tested = 0, s_skip = 0, s_gap = 0;

        Str spath = out_dir + "\\" + suite + ".md";
        std::ofstream sf(spath.c_str());
        sf << "# " << suite << "\n\n[<- index](index.md)\n\n";
        sf << "| pass | spec | tested | ";
        for (Str::size_type i = 0; i < comps.size(); ++i) sf << comps[i] << " | note | ";
        sf << "\n|---|---|---|";
        for (Str::size_type i = 0; i < comps.size(); ++i) sf << "---|---|";
        sf << "\n";

        for (std::set<Str>::iterator p = all_pass.begin(); p != all_pass.end(); ++p) {
            Str pass = *p;
            bool in_spec = sp.count(pass) > 0;
            Str spec_mark = "y";
            if (!in_spec) { spec_mark = "**orphan**"; }
            else if (!sp[pass].status.empty()) spec_mark = sp[pass].status;  /* deleted / dup */

            /* tested 状態: base の記録 or 今回結果からの集約 */
            Str tstat;
            if (ts.count(pass)) tstat = ts[pass].status;
            SuiteRows::iterator r = sr.find(pass);
            Str run_stat = (r != sr.end()) ? agg_status(r->second) : Str();
            Str tshow = tstat.empty() ? (run_stat.empty() ? Str("-") : run_stat) : tstat;

            if (in_spec && sp[pass].status.empty()) {
                if (tshow == "tested")      s_tested++;
                else if (tshow == "skip")   s_skip++;
                else                        s_gap++;
            }

            sf << "| " << md_escape(pass) << " | " << spec_mark << " | " << tshow << " | ";
            for (Str::size_type i = 0; i < comps.size(); ++i) {
                Str res = "-", note = "";
                if (r != sr.end()) {
                    const Cell& c = r->second[i];
                    if (!c.result.empty()) { res = c.result; note = c.note; }
                }
                if      (res == "pass") c_ok[i]++;
                else if (res == "fail") c_nok[i]++;
                else if (res == "skip") c_skip[i]++;
                else                    c_miss[i]++;
                sf << res << " | " << md_escape(note) << " | ";
            }
            sf << "\n";

            /* suggest 集計 */
            if (in_spec && sp[pass].status.empty() && tshow != "tested" && tshow != "skip") {
                tot_gap++;
                if (suggest) sugg += "- gap: " + suite + " / " + pass + "\n";
            }
            if (!in_spec) {
                tot_orphan++;
                if (suggest) sugg += "- orphan (spec-add candidate): " + suite + " / " + pass + "\n";
            }
            if (ts.count(pass) && r == sr.end() && !result_dirs.empty()) {
                tot_stale++;
                if (suggest) sugg += "- stale (delete-mark candidate; absent from this run): " + suite + " / " + pass + "\n";
            }
        }
        sf.close();

        idx << "| [" << suite << "](" << suite << ".md) | " << (unsigned)sp.size()
            << " | " << s_tested << " | " << s_skip << " | " << s_gap << " | ";
        for (Str::size_type i = 0; i < comps.size(); ++i)
            idx << c_ok[i] << "/" << c_nok[i] << "/" << c_skip[i] << "/" << c_miss[i] << " | ";
        idx << "\n";

        /* ---- --update-spec: 結果 pass で spec に無いものを追記のみ ---- */
        if (update_spec && !spec_out.empty()) {
            bool changed = false;
            for (SuiteRows::iterator r2 = sr.begin(); r2 != sr.end(); ++r2)
                if (sp.find(r2->first) == sp.end()) { sp[r2->first] = SpecRow(); changed = true; tot_added_spec++; }
            if (changed) {
                _mkdir(spec_out.c_str());
                write_spec(spec_out + "\\" + suite + ".tsv", suite, sp);
            } else if (spec_out != spec_dir && !sp.empty()) {
                _mkdir(spec_out.c_str());
                write_spec(spec_out + "\\" + suite + ".tsv", suite, sp);
            }
        }

        /* ---- --update-tested: 結果から union 更新 (非破壊) ---- */
        if (update_tested && !tested_out.empty()) {
            bool changed = false;
            for (SuiteRows::iterator r2 = sr.begin(); r2 != sr.end(); ++r2) {
                Str st = agg_status(r2->second);
                if (st.empty()) continue;
                std::map<Str, TestedRow>::iterator e = ts.find(r2->first);
                if (e == ts.end()) {
                    TestedRow tr; tr.status = st; ts[r2->first] = tr; changed = true; tot_upd_tested++;
                } else if (e->second.status != "tested" && st == "tested") {
                    e->second.status = "tested"; changed = true; tot_upd_tested++;  /* 昇格 */
                } else if (e->second.status.empty() && !st.empty()) {
                    e->second.status = st; changed = true; tot_upd_tested++;
                }
                /* 既存 tested の降格はしない。既存 skip を fail/skip で上書きもしない。 */
            }
            if (changed || (tested_out != tested_dir && !ts.empty())) {
                _mkdir(tested_out.c_str());
                write_tested(tested_out + "\\" + suite + ".tsv", suite, ts);
            }
        }
    }
    idx.close();

    if (suggest) {
        std::ofstream so((out_dir + "\\_suggest.md").c_str());
        so << sugg;
    }

    std::printf("testagg: %u suites, %u compilers -> %s\n",
                (unsigned)suites.size(), (unsigned)comps.size(), index_path.c_str());
    std::printf("  gap=%u orphan=%u stale=%u", tot_gap, tot_orphan, tot_stale);
    if (update_spec)   std::printf("  +spec=%u", tot_added_spec);
    if (update_tested) std::printf("  +tested=%u", tot_upd_tested);
    std::printf("\n");
    return 0;
}
