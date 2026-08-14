#!/usr/bin/env python3
"""Run the libc++ conformance suite twice -- native and ccwrap -- and diff.

The libc++ test suite can be pointed at a standard library that is not libc++
(that is what test/configs/stdlib-libstdc++.cfg.in and stdlib-native.cfg.in do).
ccwrap is not a standard library of its own, it is a layer on top of one, so a
single run says very little: libstdc++ alone already fails a lot of libc++'s
tests.  What answers "did ccwrap break or fix anything" is the DELTA between

    native  = the compiler's own library
    ccwrap  = the same, with -I <tree>/std -include ccwrap_header.h -D_CCW_TARGET_*

Usage:
    run.py --compiler g++ --std c++20 --filter <regex> [--jobs N]
    run.py --list-broken            # only the regressions, one per line

lit is not vendored.  Point --lit at a checkout of llvm/utils/lit (fetch_lit.py
downloads one), or set CCWRAP_LIT.  PyPI's `lit` is stuck at 18 and is too old
for this libc++ tree.
"""
import argparse
import json
import os
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
# The test tree is in the repository and self-contained (test/ + utils/).
LIBCXX = os.path.join(ROOT, "detail", "llibcxx03")
CFG = os.path.join(HERE, "ccwrap.cfg.py")

SITE_CFG = """import os
config.ccwrap_work = os.path.dirname(os.path.abspath(__file__))
lit_config.load_config(config, os.environ['CCWRAP_CFG'])
"""


def make_work(work, name):
    d = os.path.join(work, name)
    if os.path.isdir(d):
        shutil.rmtree(d)
    os.makedirs(d)
    with open(os.path.join(d, "lit.site.cfg.py"), "w") as f:
        f.write(SITE_CFG)
    return d


def run_lit(a, work_dir, out_json, ccwrap):
    env = dict(os.environ)
    env["PYTHONPATH"] = a.lit + (os.pathsep + env["PYTHONPATH"] if env.get("PYTHONPATH") else "")
    env["CCWRAP_LIBCXX_DIR"] = a.libcxx_dir
    env["CCWRAP_CFG"] = CFG
    params = [
        "--param", "compiler=" + a.compiler,
        "--param", "target_triple=" + a.triple,
        "--param", "std=" + a.std,
        "--param", "stdlib=" + a.stdlib,
        "--param", "ccwrap_root=" + (ROOT if ccwrap else ""),
        "--param", "ccwrap_target_cxx=" + (a.target_cxx if ccwrap else ""),
        "--param", "ccwrap_dir=" + a.tree,
        "--param", "enable_warnings=" + ("True" if a.warnings else "False"),
    ]
    cmd = [sys.executable, "-c",
           "import sys; sys.argv[0]='lit'; from lit.main import main; main()",
           "-s", "--no-progress-bar",
           "-j", str(a.jobs), "--output", out_json] + params
    if a.timeout:                      # needs psutil; off by default
        cmd += ["--timeout", str(a.timeout)]
    if a.filter:
        cmd += ["--filter", a.filter]
    cmd.append(work_dir)
    log = os.path.splitext(out_json)[0] + ".log"
    with open(log, "w") as f:
        f.write(" ".join(cmd) + "\n\n")
        f.flush()
        r = subprocess.run(cmd, env=env, stdout=f, stderr=subprocess.STDOUT)
    if not os.path.exists(out_json):
        tail = open(log).read()[-4000:]
        sys.stderr.write(tail + "\n")
        raise SystemExit("lit produced no report (%s); full log in %s"
                         % ("ccwrap" if ccwrap else "native", log))
    return r


def load(path):
    with open(path) as f:
        data = json.load(f)
    return {t["name"]: t for t in data["tests"]}


GOOD = ("PASS", "XFAIL", "FLAKYPASS")


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--compiler", default="g++")
    p.add_argument("--triple", default="")
    p.add_argument("--std", default="c++20")
    p.add_argument("--stdlib", default="libstdc++",
                   choices=["llvm-libc++", "apple-libc++", "libstdc++", "msvc"])
    p.add_argument("--tree", default="gcc", help="which wrapper tree: gcc | vc")
    p.add_argument("--target-cxx", dest="target_cxx", default="2023",
                   help="_CCW_TARGET_CXX, 4-digit year")
    p.add_argument("--filter", default="")
    p.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    p.add_argument("--timeout", type=int, default=0,
                   help="per-test timeout in seconds; needs psutil, so 0 (off) by default")
    p.add_argument("--warnings", action="store_true",
                   help="keep libc++'s -Werror set (off by default: ccwrap headers "
                        "warn where the native ones do not, which is not a defect)")
    p.add_argument("--work", default=os.path.join(HERE, "_work"))
    p.add_argument("--lit", default=os.environ.get("CCWRAP_LIT", os.path.join(HERE, "_lit")))
    p.add_argument("--libcxx-dir", dest="libcxx_dir", default=LIBCXX,
                   help="tree holding the libc++ test/ and utils/ directories")
    p.add_argument("--list-broken", action="store_true")
    a = p.parse_args()

    if not os.path.isdir(os.path.join(a.lit, "lit")):
        raise SystemExit("no lit at %s -- run fetch_lit.py <ref> %s" % (a.lit, a.lit))
    if not a.triple:
        a.triple = subprocess.check_output([a.compiler, "-dumpmachine"]).decode().strip()

    os.makedirs(a.work, exist_ok=True)
    results = {}
    for name, ccwrap in (("native", False), ("ccwrap", True)):
        d = make_work(a.work, name)
        out = os.path.join(a.work, name + ".json")
        if os.path.exists(out):
            os.remove(out)
        run_lit(a, d, out, ccwrap)
        results[name] = load(out)
        codes = {}
        for t in results[name].values():
            codes[t["code"]] = codes.get(t["code"], 0) + 1
        if not a.list_broken:
            print("%-7s %s" % (name, " ".join("%s=%d" % kv for kv in sorted(codes.items()))))

    nat, ccw = results["native"], results["ccwrap"]
    broken, fixed = [], []
    for name in sorted(set(nat) | set(ccw)):
        n = nat.get(name, {}).get("code")
        c = ccw.get(name, {}).get("code")
        if n == c:
            continue
        if n in GOOD and c not in GOOD:
            broken.append((name, n, c))
        elif n not in GOOD and c in GOOD:
            fixed.append((name, n, c))

    if a.list_broken:
        for name, _, _ in broken:
            print(name)
        return 1 if broken else 0

    print("\n== ccwrap breaks %d test(s) that pass natively ==" % len(broken))
    for name, n, c in broken:
        print("  %-6s -> %-10s %s" % (n, c, name))
    print("\n== ccwrap fixes %d test(s) that fail natively ==" % len(fixed))
    for name, n, c in fixed:
        print("  %-6s -> %-10s %s" % (n, c, name))
    print("\nreports: %s/{native,ccwrap}.json" % a.work)
    return 1 if broken else 0


if __name__ == "__main__":
    sys.exit(main())
