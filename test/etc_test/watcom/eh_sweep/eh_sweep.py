# -*- coding: utf-8 -*-
"""Compile every public header on its own, with and without exceptions.

Each header of watcom/std and watcom/ccwstd becomes a one-line translation unit

    #include <foo>
    int main(void) { return 0; }

which is compiled twice: with -xs (exceptions) and without. What matters is the
*difference*: a header that fails only in the no-exception build is a defect the
exception-disabled support has to fix. A header that fails in both is unrelated to
exceptions and is reported separately (<format> / <print> are #error stubs today).

This catches what the per-header suites structurally cannot: which standard header
a translation unit reads *first*. The suites always come in through test_cxx.hpp,
so an ordering or circular-include hazard that only fires when e.g. <stdio.h> is
the first ccwrap header stays invisible there. Two real defects were found that
way (eh_fail.hpp recursing through the C wrappers; the same include perturbing the
order of the abs/div overloads).

Exit code is 1 only when a header fails ONLY without exceptions. Headers that fail
in both builds are listed but do not fail the run -- they are somebody else's bug.

Usage:
    python3 eh_sweep.py                          # C:\\WATCOM, both namespaces
    python3 eh_sweep.py --watcom C:\\WATCOM
    python3 eh_sweep.py --ns std                 # one namespace
    python3 eh_sweep.py --only vector,string     # a subset
    python3 eh_sweep.py -v                       # list every header
"""
import argparse
import io
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", "..", ".."))
OUT = os.path.join(os.environ.get("TEMP", HERE), "ccw_eh_sweep")

# Not headers a user includes: the forced-include config and the native private one.
SKIP = {"ccwrap_common.h", "ccwrap_header.h", "_comdef.h"}


def env_for(watcom):
    e = dict(os.environ)
    e["WATCOM"] = watcom
    e["INCLUDE"] = os.path.join(watcom, "h") + ";" + os.path.join(watcom, "h", "nt")
    e["EDPATH"] = os.path.join(watcom, "eddat")
    e["PATH"] = (os.path.join(watcom, "binnt64") + ";" + os.path.join(watcom, "binnt")
                 + ";" + e.get("PATH", ""))
    return e


def compile_one(watcom, incdir, header, exceptions):
    if not os.path.isdir(OUT):
        os.makedirs(OUT)
    src = os.path.join(OUT, "ehsweep.cpp")
    io.open(src, "w").write("#include <%s>\nint main(void) { return 0; }\n" % header)
    cmd = [os.path.join(watcom, "binnt", "wpp386.exe"),
           "-bt=nt", "-of+", "-zq",
           "-i=" + incdir,
           "-fi=" + os.path.join(incdir, "ccwrap_header.h"),
           "-fo=" + os.path.join(OUT, "ehsweep.obj"), src]
    if exceptions:
        cmd.insert(1, "-xs")
    p = subprocess.run(cmd, env=env_for(watcom), cwd=OUT,
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return p.returncode, p.stdout.decode("utf-8", "replace")


def first_error(text):
    for line in text.splitlines():
        if "Error!" in line:
            return line.strip()
    return text.strip().splitlines()[0] if text.strip() else "(no output)"


def sweep(watcom, ns, only, verbose):
    incdir = os.path.join(ROOT, "watcom", ns)
    if not os.path.isdir(incdir):
        print("[%s] no such include dir: %s" % (ns, incdir))
        return 0
    headers = sorted(h for h in os.listdir(incdir)
                     if h not in SKIP and not h.endswith(".BAK")
                     and os.path.isfile(os.path.join(incdir, h)))
    if only:
        headers = [h for h in headers if h in only]

    ok, both, noeh = [], [], []
    for h in headers:
        rc_x, out_x = compile_one(watcom, incdir, h, True)
        rc_n, out_n = compile_one(watcom, incdir, h, False)
        if rc_x == 0 and rc_n == 0:
            ok.append(h)
        elif rc_x == 0 and rc_n != 0:
            noeh.append((h, out_n))
        else:
            both.append((h, out_x if rc_x else out_n))

    print("[%s] %d headers: ok(both)=%d  fail(both)=%d  FAIL(no-exceptions only)=%d"
          % (ns, len(headers), len(ok), len(both), len(noeh)))
    if verbose and ok:
        print("   ok: " + " ".join(ok))
    for h, out in both:
        print("   -- %s (fails with and without exceptions)" % h)
        print("      " + first_error(out))
    for h, out in noeh:
        print("   ** %s compiles with -xs but not without" % h)
        for line in out.strip().splitlines()[:6]:
            print("      " + line.strip())
    return len(noeh)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--watcom", default=os.environ.get("WATCOM") or r"C:\WATCOM")
    ap.add_argument("--ns", default="std,ccwstd", help="comma-separated: std,ccwstd")
    ap.add_argument("--only", default=None, help="comma-separated header names")
    ap.add_argument("-v", "--verbose", action="store_true")
    a = ap.parse_args()

    if not os.path.isfile(os.path.join(a.watcom, "binnt", "wpp386.exe")):
        print("eh_sweep: no wpp386 under %s" % a.watcom)
        return 2

    only = set(a.only.split(",")) if a.only else None
    bad = 0
    for ns in a.ns.split(","):
        bad += sweep(a.watcom, ns.strip(), only, a.verbose)
    if bad:
        print("---")
        print("%d header(s) compile only with exceptions enabled." % bad)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
