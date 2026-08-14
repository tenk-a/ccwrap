#!/usr/bin/env python3
"""Fetch llvm/utils/lit at a given ref into a directory (lit is pure Python)."""
import json
import os
import sys
import urllib.request

REF = sys.argv[1]
DEST = sys.argv[2]
BASE = "https://api.github.com/repos/llvm/llvm-project/contents/"
ROOT = "llvm/utils/lit/lit"


def get(url):
    req = urllib.request.Request(url, headers={"User-Agent": "ccwrap"})
    return json.load(urllib.request.urlopen(req))


def raw(url):
    req = urllib.request.Request(url, headers={"User-Agent": "ccwrap"})
    return urllib.request.urlopen(req).read()


count = [0]


def walk(path):
    for e in get("%s%s?ref=%s" % (BASE, path, REF)):
        if e["type"] == "dir":
            walk(e["path"])
        elif e["type"] == "file":
            rel = e["path"][len("llvm/utils/lit/"):]
            out = os.path.join(DEST, rel)
            d = os.path.dirname(out)
            if d and not os.path.isdir(d):
                os.makedirs(d)
            open(out, "wb").write(raw(e["download_url"]))
            count[0] += 1
            if count[0] % 15 == 0:
                print("  %d files" % count[0])


walk(ROOT)
print("wrote %d files to %s" % (count[0], DEST))
