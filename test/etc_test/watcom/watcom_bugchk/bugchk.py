# -*- coding: utf-8 -*-
"""Open Watcom bug/limit re-check harness.

Every probe under probes/ is a self-contained program that is *valid* C or C++ and
that asserts the standard-correct behaviour. So the outcome is binary:

    ok   -- the probe compiles (and, for run probes, prints only OK) => bug absent
    bug  -- the probe fails                                          => bug reproduces

Each probe declares, in its header comment, what doc/watcom/ccwrap_watcom_bugs.md
currently claims:

    // bugchk-id:     A05
    // bugchk-kind:   compile | run
    // bugchk-expect: bug | ok
    // bugchk-desc:   one line
    // bugchk-flags:  -d2            (optional, extra compiler flags)

The runner compares expect vs. the observed result and shouts on any mismatch:

    FIXED       expect=bug  got=ok    -> update the catalog, drop the workaround
    REGRESSION  expect=ok   got=bug   -> a new Watcom broke something

Because a probe is a *reconstruction* of a bug, "got=ok" alone does not prove the
bug is gone -- the probe may simply not reproduce it. Pass --compare <OTHER_WATCOM>
to run the same probes against an older install; a probe that is `bug` on the old
compiler and `ok` on the new one is a confirmed fix, whereas ok/ok means the probe
never reproduced anything and should not be trusted.

Usage:
    python3 bugchk.py                              # C:\\WATCOM
    python3 bugchk.py --watcom C:\\WATCOM
    python3 bugchk.py --compare C:\\WATCOM2.0.pre  # new vs old, confirms fixes
    python3 bugchk.py --only A26,B07               # a subset
"""
import argparse
import io
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROBES = os.path.join(HERE, "probes")
OUT = os.path.join(os.environ.get("TEMP", HERE), "ccw_bugchk")

# Accepts both // and /* */ comment styles (the C probes use block comments).
META = re.compile(r"bugchk-(\w+):\s*(.*?)\s*(?:\*/)?\s*$")


def load(path):
    m = {"flags": "", "kind": "compile", "expect": "bug", "desc": "", "id": os.path.basename(path)}
    for n, line in enumerate(io.open(path, encoding="utf-8")):
        if n > 40:
            break
        g = META.search(line)
        if g:
            m[g.group(1)] = g.group(2)
    m["path"] = path
    m["lang"] = "c" if path.endswith(".c") else "cxx"
    return m


def env_for(watcom):
    e = dict(os.environ)
    e["WATCOM"] = watcom
    e["INCLUDE"] = os.path.join(watcom, "h") + ";" + os.path.join(watcom, "h", "nt")
    e["EDPATH"] = os.path.join(watcom, "eddat")
    e["PATH"] = (os.path.join(watcom, "binnt64") + ";" + os.path.join(watcom, "binnt")
                 + ";" + e.get("PATH", ""))
    return e


def probe(p, watcom):
    """Return (result, detail) where result is 'ok' or 'bug'."""
    e = env_for(watcom)
    # Give each Watcom root its own scratch dir: with --compare the same probe is built twice,
    # and sharing one .exe path let a stale/locked binary be run instead of the fresh one.
    out = os.path.join(OUT, re.sub(r"[^A-Za-z0-9]", "_", watcom))
    if not os.path.isdir(out):
        os.makedirs(out)
    stem = os.path.join(out, os.path.splitext(os.path.basename(p["path"]))[0])
    flags = p["flags"].split()
    # bugchk-aux: extra sources, relative to probes/. A bug that only shows up ACROSS
    # translation units (a mangled name that is not a function of the type) needs a
    # second one. They live in probes/extra/ so the glob above does not take them for
    # probes in their own right.
    srcs = [p["path"]] + [os.path.join(PROBES, s) for s in p.get("aux", "").split()]

    if p["kind"] == "compile":
        cc = "wpp386.exe" if p["lang"] == "cxx" else "wcc386.exe"
        cmd = [os.path.join(watcom, "binnt64", cc), "-q", "-bt=nt",
               "-fo=" + stem + ".obj"] + flags + srcs
        r = subprocess.run(cmd, capture_output=True, text=True, cwd=out, env=e)
        if r.returncode == 0:
            return "ok", ""
        return "bug", first_error(r.stdout or r.stderr or "")

    # kind == run: build, then execute. The program exits 0 only if it behaves correctly.
    # -xs (exception handling) is C++ only; in C mode wcl386 parses it as -x plus a file name.
    lang_flags = ["-xs"] if p["lang"] == "cxx" else []
    cmd = ([os.path.join(watcom, "binnt64", "wcl386.exe"), "-q"] + lang_flags
           + ["-bt=nt", "-l=nt", "-fe=" + stem + ".exe"] + flags + srcs)
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=out, env=e)
    if r.returncode != 0:
        return "bug", "did not build: " + first_error(r.stdout or "")
    try:
        x = subprocess.run([stem + ".exe"], capture_output=True, text=True, cwd=out, env=e, timeout=120)
    except subprocess.TimeoutExpired:
        return "bug", "timed out"
    detail = " / ".join(l.strip() for l in (x.stdout or "").splitlines() if "BROKEN" in l)
    if x.returncode == 0:
        return "ok", ""
    return "bug", detail or ("exit=%d" % x.returncode)


def first_error(text):
    for line in text.splitlines():
        if "Error!" in line or "error" in line.lower():
            return " ".join(line.split())[:150]
    return " ".join(text.split())[:150]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--watcom", default=os.environ.get("WATCOM") or r"C:\WATCOM")
    ap.add_argument("--compare", default=None, help="a second (usually older) Watcom root")
    ap.add_argument("--only", default=None, help="comma-separated probe ids")
    a = ap.parse_args()

    probes = [load(os.path.join(PROBES, f)) for f in sorted(os.listdir(PROBES))
              if f.endswith((".c", ".cpp"))]
    if a.only:
        want = set(s.strip().upper() for s in a.only.split(","))
        probes = [p for p in probes if p["id"].upper() in want]

    print("new: %s" % a.watcom)
    if a.compare:
        print("old: %s" % a.compare)
    print()
    hdr = "%-6s %-8s %-6s %-6s" % ("id", "catalog", "now", "was") if a.compare else "%-6s %-8s %-6s" % ("id", "catalog", "now")
    print(hdr + "  verdict      desc")
    print("-" * 110)

    changed, regressed = [], []
    for p in probes:
        now, d1 = probe(p, a.watcom)
        was, d2 = (probe(p, a.compare) if a.compare else (None, ""))

        if now == p["expect"]:
            verdict = "as-catalogued"
        elif p["expect"] == "bug" and now == "ok":
            verdict = "FIXED" if was == "bug" else ("no-repro" if was == "ok" else "FIXED?")
            (changed if verdict.startswith("FIXED") else []).append(p)
        else:
            verdict = "REGRESSION"
            regressed.append(p)

        cols = ("%-6s %-8s %-6s %-6s" % (p["id"], p["expect"], now, was or "-")) if a.compare \
            else ("%-6s %-8s %-6s" % (p["id"], p["expect"], now))
        print("%s  %-12s %s" % (cols, verdict, p["desc"]))
        if d1:
            print("%s  %s" % (" " * len(cols), d1[:100]))

    print()
    print("%d probes. %d newly fixed, %d regressions." % (len(probes), len(changed), len(regressed)))
    if changed:
        print("FIXED -> update doc/watcom/ccwrap_watcom_bugs.md and revisit the workaround:")
        for p in changed:
            print("   %s  %s" % (p["id"], p["desc"]))
    return 1 if regressed else 0


if __name__ == "__main__":
    sys.exit(main())
