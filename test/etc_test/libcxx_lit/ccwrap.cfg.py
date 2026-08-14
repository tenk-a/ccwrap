#
# Run the libc++ conformance test suite against a standard library that is NOT
# libc++ -- here, the compiler's own library with
# ccwrap layered on top.  This is the same mechanism libc++ ships as
# test/configs/stdlib-native.cfg.in and stdlib-libstdc++.cfg.in; only the
# compile flags differ.
#
# The point is the DELTA against the same run without ccwrap: a bare run against
# libstdc++ fails a great many libc++-specific tests, which says nothing about
# ccwrap.  run.py drives both and diffs them.
#
# Params (all optional except ccwrap_root):
#   ccwrap_root=<path>        repository root (the directory holding gcc/ vc/ ...)
#   ccwrap_target_cxx=<year>  4-digit year -> ccwrap is engaged. Empty -> native
#                             baseline (this is exactly the knob the ccwrap test
#                             suites use; see .claude/rules/test.md).
#   ccwrap_dir=<name>         which wrapper tree to put on the include path
#                             (gcc | vc). Default gcc.
#
import os
import shlex
import site
import sys

import lit.formats

# The tree lives in the repository (detail/llibcxx03) and keeps libc++'s own
# shape: `test/` beside `utils/`, the latter holding the lit framework the
# configs import (libcxx.test.*) and run.py, the default executor.
LIBCXX = os.environ.get("CCWRAP_LIBCXX_DIR")
if not LIBCXX:
    lit_config.fatal("CCWRAP_LIBCXX_DIR must point at detail/llibcxx03 (test/ + utils/)")

site.addsitedir(os.path.join(LIBCXX, "utils"))
import libcxx.test.config
import libcxx.test.dsl
import libcxx.test.features
import libcxx.test.format
import libcxx.test.params

config.name = "ccwrap-vs-libcxx-suite"
config.test_source_root = os.path.join(LIBCXX, "test")
config.test_exec_root = os.path.join(config.ccwrap_work, "run")
config.test_format = libcxx.test.format.CxxStandardLibraryTest()
config.recursiveExpansionLimit = 10

# Only test/std (the standard conformance suite) is meaningful against a library
# that is not libc++; test/libcxx tests libc++'s own internals.  Excluding them
# by name keeps the copied tree byte-identical to upstream -- which matters,
# because test/libcxx/**/lit.local.cfg rewrites its own path by replacing the
# literal "libcxx/test/libcxx" with "libcxx/test/std".  Outside a directory
# actually named libcxx/ that substring is absent, the path is unchanged, and
# the config loads ITSELF until the recursion limit.  lit skips excluded names
# during discovery, so those files are never executed.
config.excludes = [
    "libcxx", "libcxx-03",      # libc++ internals; and the self-loading configs above
    "benchmarks", "selftest",   # benchmark harness / lit's own selftests
    "extensions", "configs", "tools",
]

config.substitutions.append(("%{libcxx-dir}", LIBCXX))
config.substitutions.append(("%{bin-dir}", config.ccwrap_work))
config.substitutions.append(("%{install-prefix}", config.ccwrap_work))
config.substitutions.append(("%{include-dir}", os.path.join(config.ccwrap_work, "include")))
config.substitutions.append(("%{target-include-dir}", os.path.join(config.ccwrap_work, "include")))
config.substitutions.append(("%{lib-dir}", os.path.join(config.ccwrap_work, "lib")))
config.substitutions.append(("%{module-dir}", os.path.join(config.ccwrap_work, "modules")))
config.substitutions.append(("%{test-tools-dir}", os.path.join(config.ccwrap_work, "test-tools")))
config.substitutions.append(("%{benchmark_flags}", ""))
config.substitutions.append(("%{python}", shlex.quote(sys.executable)))

CCWRAP_PARAMETERS = [
    libcxx.test.dsl.Parameter(
        name="ccwrap_root",
        type=str,
        default="",
        help="Repository root of ccwrap. Empty runs the native baseline.",
        actions=lambda path: [libcxx.test.dsl.AddSubstitution("%{ccwrap-root}", path)],
    ),
    libcxx.test.dsl.Parameter(
        name="ccwrap_target_cxx",
        type=str,
        default="",
        help="_CCW_TARGET_CXX as a 4-digit year. Empty means do not engage ccwrap.",
        actions=lambda year: [libcxx.test.dsl.AddFeature("ccwrap")] if year else [],
    ),
    libcxx.test.dsl.Parameter(
        name="ccwrap_dir",
        type=str,
        default="gcc",
        help="Which wrapper tree to put on the include path (gcc | vc).",
        actions=lambda d: [],
    ),
]

root = lit_config.params.get("ccwrap_root", "")
year = lit_config.params.get("ccwrap_target_cxx", "")
tree = lit_config.params.get("ccwrap_dir", "gcc")

compile_flags = ["-I", os.path.join(LIBCXX, "test", "support")]
if root and year:
    std_dir = os.path.join(root, tree, "std")
    compile_flags += [
        "-I", std_dir,
        "-include", os.path.join(std_dir, "ccwrap_header.h"),
        "-D_CCW_TARGET_CXX=" + year,
        "-D_CCW_TARGET_C=" + year,
    ]

config.substitutions.append(("%{flags}", "-pthread"))
config.substitutions.append(("%{compile_flags}", " ".join(shlex.quote(f) for f in compile_flags)))
config.substitutions.append(("%{link_flags}", ""))
config.substitutions.append(("%{exec}", "%{executor} --execdir %{temp} -- "))

libcxx.test.config.configure(
    libcxx.test.params.DEFAULT_PARAMETERS + CCWRAP_PARAMETERS,
    libcxx.test.features.DEFAULT_FEATURES,
    config,
    lit_config,
)
