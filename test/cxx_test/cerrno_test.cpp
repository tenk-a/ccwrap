#include "test_cxx.hpp"
#include <cerrno>

TEST_CASE(cerrno, header_compiles) { test_true( true ); test_pass("cxx03:<cerrno> header compiles"); }

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>

namespace {

struct ErrEnt { char const* name; int value; };

const ErrEnt kErrnoMacros[] = {
    { "EDOM",            EDOM            },
    { "ERANGE",          ERANGE          },
    { "EILSEQ",          EILSEQ          },
    { "E2BIG",           E2BIG           },
    { "EACCES",          EACCES          },
    { "EADDRINUSE",      EADDRINUSE      },
    { "EADDRNOTAVAIL",   EADDRNOTAVAIL   },
    { "EAFNOSUPPORT",    EAFNOSUPPORT    },
    { "EAGAIN",          EAGAIN          },
    { "EALREADY",        EALREADY        },
    { "EBADF",           EBADF           },
    { "EBADMSG",         EBADMSG         },
    { "EBUSY",           EBUSY           },
    { "ECANCELED",       ECANCELED       },
    { "ECHILD",          ECHILD          },
    { "ECONNABORTED",    ECONNABORTED    },
    { "ECONNREFUSED",    ECONNREFUSED    },
    { "ECONNRESET",      ECONNRESET      },
    { "EDEADLK",         EDEADLK         },
    { "EDESTADDRREQ",    EDESTADDRREQ    },
    { "EEXIST",          EEXIST          },
    { "EFAULT",          EFAULT          },
    { "EFBIG",           EFBIG           },
    { "EHOSTUNREACH",    EHOSTUNREACH    },
    { "EIDRM",           EIDRM           },
    { "EINPROGRESS",     EINPROGRESS     },
    { "EINTR",           EINTR           },
    { "EINVAL",          EINVAL          },
    { "EIO",             EIO             },
    { "EISCONN",         EISCONN         },
    { "EISDIR",          EISDIR          },
    { "ELOOP",           ELOOP           },
    { "EMFILE",          EMFILE          },
    { "EMLINK",          EMLINK          },
    { "EMSGSIZE",        EMSGSIZE        },
    { "ENAMETOOLONG",    ENAMETOOLONG    },
    { "ENETDOWN",        ENETDOWN        },
    { "ENETRESET",       ENETRESET       },
    { "ENETUNREACH",     ENETUNREACH     },
    { "ENFILE",          ENFILE          },
    { "ENOBUFS",         ENOBUFS         },
    { "ENODATA",         ENODATA         },
    { "ENODEV",          ENODEV          },
    { "ENOENT",          ENOENT          },
    { "ENOEXEC",         ENOEXEC         },
    { "ENOLCK",          ENOLCK          },
    { "ENOLINK",         ENOLINK         },
    { "ENOMEM",          ENOMEM          },
    { "ENOMSG",          ENOMSG          },
    { "ENOPROTOOPT",     ENOPROTOOPT     },
    { "ENOSPC",          ENOSPC          },
    { "ENOSR",           ENOSR           },
    { "ENOSTR",          ENOSTR          },
    { "ENOSYS",          ENOSYS          },
    { "ENOTCONN",        ENOTCONN        },
    { "ENOTDIR",         ENOTDIR         },
    { "ENOTEMPTY",       ENOTEMPTY       },
    { "ENOTRECOVERABLE", ENOTRECOVERABLE },
    { "ENOTSOCK",        ENOTSOCK        },
    { "ENOTSUP",         ENOTSUP         },
    { "ENOTTY",          ENOTTY          },
    { "ENXIO",           ENXIO           },
    { "EOPNOTSUPP",      EOPNOTSUPP      },
    { "EOVERFLOW",       EOVERFLOW       },
    { "EOWNERDEAD",      EOWNERDEAD      },
    { "EPERM",           EPERM           },
    { "EPIPE",           EPIPE           },
    { "EPROTO",          EPROTO          },
    { "EPROTONOSUPPORT", EPROTONOSUPPORT },
    { "EPROTOTYPE",      EPROTOTYPE      },
    { "EROFS",           EROFS           },
    { "ESPIPE",          ESPIPE          },
    { "ESRCH",           ESRCH           },
    { "ETIME",           ETIME           },
    { "ETIMEDOUT",       ETIMEDOUT       },
    { "ETXTBSY",         ETXTBSY         },
    { "EWOULDBLOCK",     EWOULDBLOCK     },
    { "EXDEV",           EXDEV           },
};
const STD::size_t kErrnoMacroCount = sizeof(kErrnoMacros) / sizeof(kErrnoMacros[0]);

bool allowed_alias(char const* a, char const* b) {
    static char const* const pairs[][2] = {
        { "EAGAIN",  "EWOULDBLOCK" },
        { "ENOTSUP", "EOPNOTSUPP"  },
    };
    for (STD::size_t i = 0; i < sizeof(pairs) / sizeof(pairs[0]); ++i) {
        if ((STD::strcmp(a, pairs[i][0]) == 0 && STD::strcmp(b, pairs[i][1]) == 0) ||
            (STD::strcmp(a, pairs[i][1]) == 0 && STD::strcmp(b, pairs[i][0]) == 0))
            return true;
    }
    return false;
}

}

TEST_CASE(cerrno, macros_are_positive) {

    int bad = 0;
    for (STD::size_t i = 0; i < kErrnoMacroCount; ++i) {
        if (kErrnoMacros[i].value <= 0)
            ++bad;
    }

    test_gt( EDOM,   0 );
    test_pass("cxx03:EDOM");
    test_gt( ERANGE, 0 );
    test_pass("cxx03:ERANGE");
    test_gt( EILSEQ, 0 );
    test_pass("cxx03:EILSEQ");

    test_eq( bad, 0 );
    test_pass("cxx11:extended errno macros present and positive (property)");
}

TEST_CASE(cerrno, macros_are_distinct) {
    int dup = 0;
    for (STD::size_t i = 0; i < kErrnoMacroCount; ++i) {
        for (STD::size_t j = i + 1; j < kErrnoMacroCount; ++j) {
            if (kErrnoMacros[i].value == kErrnoMacros[j].value &&
                !allowed_alias(kErrnoMacros[i].name, kErrnoMacros[j].name))
                ++dup;
        }
    }
    test_eq( dup, 0 );

    test_ne( EDOM,  ERANGE );
    test_ne( EDOM,  EILSEQ );
    test_ne( EINVAL, ERANGE );
    test_pass("cxx11:E* macros are distinct (only POSIX aliases share a value)");
}

TEST_CASE(cerrno, errno_is_a_modifiable_lvalue) {
    errno = 0;
    int e0 = errno;
    test_eq( e0, 0 );

    errno = EDOM;
    int e1 = errno;
    test_eq( e1, EDOM );

    errno = ERANGE;
    int e2 = errno;
    test_eq( e2, ERANGE );

    int* p = &errno;
    test_ptr( p );
    *p = EINVAL;
    int e3 = errno;
    test_eq( e3, EINVAL );

    errno = 0;
    test_pass("cxx03:errno (modifiable lvalue)");
}

TEST_CASE(cerrno, errno_set_by_the_library) {

    errno = 0;
    char* end = 0;
    long v = STD::strtol("999999999999999999999999", &end, 10);
    int  e = errno;
    test_eq( e, ERANGE );
    test_eq( v, LONG_MAX );

    errno = 0;
    long w = STD::strtol("42", &end, 10);
    int  e2 = errno;
    test_eq( w, 42L );
    test_eq( e2, 0 );

    errno = 0;
    test_pass("cxx03:errno set by the library (strtol -> ERANGE)");
}

TEST_CASE(cerrno, error_macros_a_cxx11) {
#if defined(E2BIG)
    test_true( (E2BIG) > 0 );
    test_pass("cxx11:E2BIG");
#else
    test_skip("cxx11:E2BIG");
#endif
#if defined(EACCES)
    test_true( (EACCES) > 0 );
    test_pass("cxx11:EACCES");
#else
    test_skip("cxx11:EACCES");
#endif
#if defined(EADDRINUSE)
    test_true( (EADDRINUSE) > 0 );
    test_pass("cxx11:EADDRINUSE");
#else
    test_skip("cxx11:EADDRINUSE");
#endif
#if defined(EADDRNOTAVAIL)
    test_true( (EADDRNOTAVAIL) > 0 );
    test_pass("cxx11:EADDRNOTAVAIL");
#else
    test_skip("cxx11:EADDRNOTAVAIL");
#endif
#if defined(EAFNOSUPPORT)
    test_true( (EAFNOSUPPORT) > 0 );
    test_pass("cxx11:EAFNOSUPPORT");
#else
    test_skip("cxx11:EAFNOSUPPORT");
#endif
#if defined(EALREADY)
    test_true( (EALREADY) > 0 );
    test_pass("cxx11:EALREADY");
#else
    test_skip("cxx11:EALREADY");
#endif
#if defined(EBADF)
    test_true( (EBADF) > 0 );
    test_pass("cxx11:EBADF");
#else
    test_skip("cxx11:EBADF");
#endif
#if defined(EBADMSG)
    test_true( (EBADMSG) > 0 );
    test_pass("cxx11:EBADMSG");
#else
    test_skip("cxx11:EBADMSG");
#endif
#if defined(EBUSY)
    test_true( (EBUSY) > 0 );
    test_pass("cxx11:EBUSY");
#else
    test_skip("cxx11:EBUSY");
#endif
#if defined(ECANCELED)
    test_true( (ECANCELED) > 0 );
    test_pass("cxx11:ECANCELED");
#else
    test_skip("cxx11:ECANCELED");
#endif
#if defined(ECHILD)
    test_true( (ECHILD) > 0 );
    test_pass("cxx11:ECHILD");
#else
    test_skip("cxx11:ECHILD");
#endif
#if defined(ECONNABORTED)
    test_true( (ECONNABORTED) > 0 );
    test_pass("cxx11:ECONNABORTED");
#else
    test_skip("cxx11:ECONNABORTED");
#endif
#if defined(ECONNREFUSED)
    test_true( (ECONNREFUSED) > 0 );
    test_pass("cxx11:ECONNREFUSED");
#else
    test_skip("cxx11:ECONNREFUSED");
#endif
#if defined(ECONNRESET)
    test_true( (ECONNRESET) > 0 );
    test_pass("cxx11:ECONNRESET");
#else
    test_skip("cxx11:ECONNRESET");
#endif
#if defined(EDEADLK)
    test_true( (EDEADLK) > 0 );
    test_pass("cxx11:EDEADLK");
#else
    test_skip("cxx11:EDEADLK");
#endif
#if defined(EDESTADDRREQ)
    test_true( (EDESTADDRREQ) > 0 );
    test_pass("cxx11:EDESTADDRREQ");
#else
    test_skip("cxx11:EDESTADDRREQ");
#endif
#if defined(EEXIST)
    test_true( (EEXIST) > 0 );
    test_pass("cxx11:EEXIST");
#else
    test_skip("cxx11:EEXIST");
#endif
#if defined(EFAULT)
    test_true( (EFAULT) > 0 );
    test_pass("cxx11:EFAULT");
#else
    test_skip("cxx11:EFAULT");
#endif
#if defined(EFBIG)
    test_true( (EFBIG) > 0 );
    test_pass("cxx11:EFBIG");
#else
    test_skip("cxx11:EFBIG");
#endif
#if defined(EHOSTUNREACH)
    test_true( (EHOSTUNREACH) > 0 );
    test_pass("cxx11:EHOSTUNREACH");
#else
    test_skip("cxx11:EHOSTUNREACH");
#endif
#if defined(EIDRM)
    test_true( (EIDRM) > 0 );
    test_pass("cxx11:EIDRM");
#else
    test_skip("cxx11:EIDRM");
#endif
#if defined(EINPROGRESS)
    test_true( (EINPROGRESS) > 0 );
    test_pass("cxx11:EINPROGRESS");
#else
    test_skip("cxx11:EINPROGRESS");
#endif
#if defined(EINTR)
    test_true( (EINTR) > 0 );
    test_pass("cxx11:EINTR");
#else
    test_skip("cxx11:EINTR");
#endif
#if defined(EINVAL)
    test_true( (EINVAL) > 0 );
    test_pass("cxx11:EINVAL");
#else
    test_skip("cxx11:EINVAL");
#endif
}

TEST_CASE(cerrno, error_macros_b_cxx11) {
#if defined(EIO)
    test_true( (EIO) > 0 );
    test_pass("cxx11:EIO");
#else
    test_skip("cxx11:EIO");
#endif
#if defined(EISDIR)
    test_true( (EISDIR) > 0 );
    test_pass("cxx11:EISDIR");
#else
    test_skip("cxx11:EISDIR");
#endif
#if defined(ELOOP)
    test_true( (ELOOP) > 0 );
    test_pass("cxx11:ELOOP");
#else
    test_skip("cxx11:ELOOP");
#endif
#if defined(EMFILE)
    test_true( (EMFILE) > 0 );
    test_pass("cxx11:EMFILE");
#else
    test_skip("cxx11:EMFILE");
#endif
#if defined(EMLINK)
    test_true( (EMLINK) > 0 );
    test_pass("cxx11:EMLINK");
#else
    test_skip("cxx11:EMLINK");
#endif
#if defined(EMSGSIZE)
    test_true( (EMSGSIZE) > 0 );
    test_pass("cxx11:EMSGSIZE");
#else
    test_skip("cxx11:EMSGSIZE");
#endif
#if defined(ENAMETOOLONG)
    test_true( (ENAMETOOLONG) > 0 );
    test_pass("cxx11:ENAMETOOLONG");
#else
    test_skip("cxx11:ENAMETOOLONG");
#endif
#if defined(ENETRESET)
    test_true( (ENETRESET) > 0 );
    test_pass("cxx11:ENETRESET");
#else
    test_skip("cxx11:ENETRESET");
#endif
#if defined(ENETUNREACH)
    test_true( (ENETUNREACH) > 0 );
    test_pass("cxx11:ENETUNREACH");
#else
    test_skip("cxx11:ENETUNREACH");
#endif
#if defined(ENFILE)
    test_true( (ENFILE) > 0 );
    test_pass("cxx11:ENFILE");
#else
    test_skip("cxx11:ENFILE");
#endif
#if defined(ENOBUFS)
    test_true( (ENOBUFS) > 0 );
    test_pass("cxx11:ENOBUFS");
#else
    test_skip("cxx11:ENOBUFS");
#endif
#if defined(ENODEV)
    test_true( (ENODEV) > 0 );
    test_pass("cxx11:ENODEV");
#else
    test_skip("cxx11:ENODEV");
#endif
#if defined(ENOENT)
    test_true( (ENOENT) > 0 );
    test_pass("cxx11:ENOENT");
#else
    test_skip("cxx11:ENOENT");
#endif
#if defined(ENOEXEC)
    test_true( (ENOEXEC) > 0 );
    test_pass("cxx11:ENOEXEC");
#else
    test_skip("cxx11:ENOEXEC");
#endif
#if defined(ENOLCK)
    test_true( (ENOLCK) > 0 );
    test_pass("cxx11:ENOLCK");
#else
    test_skip("cxx11:ENOLCK");
#endif
#if defined(ENOLINK)
    test_true( (ENOLINK) > 0 );
    test_pass("cxx11:ENOLINK");
#else
    test_skip("cxx11:ENOLINK");
#endif
#if defined(ENOMEM)
    test_true( (ENOMEM) > 0 );
    test_pass("cxx11:ENOMEM");
#else
    test_skip("cxx11:ENOMEM");
#endif
#if defined(ENOMSG)
    test_true( (ENOMSG) > 0 );
    test_pass("cxx11:ENOMSG");
#else
    test_skip("cxx11:ENOMSG");
#endif
#if defined(ENOPROTOOPT)
    test_true( (ENOPROTOOPT) > 0 );
    test_pass("cxx11:ENOPROTOOPT");
#else
    test_skip("cxx11:ENOPROTOOPT");
#endif
#if defined(ENOSPC)
    test_true( (ENOSPC) > 0 );
    test_pass("cxx11:ENOSPC");
#else
    test_skip("cxx11:ENOSPC");
#endif
#if defined(ENOSYS)
    test_true( (ENOSYS) > 0 );
    test_pass("cxx11:ENOSYS");
#else
    test_skip("cxx11:ENOSYS");
#endif
#if defined(ENOTDIR)
    test_true( (ENOTDIR) > 0 );
    test_pass("cxx11:ENOTDIR");
#else
    test_skip("cxx11:ENOTDIR");
#endif
#if defined(ENOTEMPTY)
    test_true( (ENOTEMPTY) > 0 );
    test_pass("cxx11:ENOTEMPTY");
#else
    test_skip("cxx11:ENOTEMPTY");
#endif
#if defined(ENOTRECOVERABLE)
    test_true( (ENOTRECOVERABLE) > 0 );
    test_pass("cxx11:ENOTRECOVERABLE");
#else
    test_skip("cxx11:ENOTRECOVERABLE");
#endif
}

TEST_CASE(cerrno, error_macros_c_cxx11) {
#if defined(ENOTSOCK)
    test_true( (ENOTSOCK) > 0 );
    test_pass("cxx11:ENOTSOCK");
#else
    test_skip("cxx11:ENOTSOCK");
#endif
#if defined(ENOTSUP)
    test_true( (ENOTSUP) > 0 );
    test_pass("cxx11:ENOTSUP");
#else
    test_skip("cxx11:ENOTSUP");
#endif
#if defined(ENOTTY)
    test_true( (ENOTTY) > 0 );
    test_pass("cxx11:ENOTTY");
#else
    test_skip("cxx11:ENOTTY");
#endif
#if defined(ENXIO)
    test_true( (ENXIO) > 0 );
    test_pass("cxx11:ENXIO");
#else
    test_skip("cxx11:ENXIO");
#endif
#if defined(EOPNOTSUPP)
    test_true( (EOPNOTSUPP) > 0 );
    test_pass("cxx11:EOPNOTSUPP");
#else
    test_skip("cxx11:EOPNOTSUPP");
#endif
#if defined(EOVERFLOW)
    test_true( (EOVERFLOW) > 0 );
    test_pass("cxx11:EOVERFLOW");
#else
    test_skip("cxx11:EOVERFLOW");
#endif
#if defined(EOWNERDEAD)
    test_true( (EOWNERDEAD) > 0 );
    test_pass("cxx11:EOWNERDEAD");
#else
    test_skip("cxx11:EOWNERDEAD");
#endif
#if defined(EPERM)
    test_true( (EPERM) > 0 );
    test_pass("cxx11:EPERM");
#else
    test_skip("cxx11:EPERM");
#endif
#if defined(EPIPE)
    test_true( (EPIPE) > 0 );
    test_pass("cxx11:EPIPE");
#else
    test_skip("cxx11:EPIPE");
#endif
#if defined(EPROTO)
    test_true( (EPROTO) > 0 );
    test_pass("cxx11:EPROTO");
#else
    test_skip("cxx11:EPROTO");
#endif
#if defined(EPROTONOSUPPORT)
    test_true( (EPROTONOSUPPORT) > 0 );
    test_pass("cxx11:EPROTONOSUPPORT");
#else
    test_skip("cxx11:EPROTONOSUPPORT");
#endif
#if defined(EPROTOTYPE)
    test_true( (EPROTOTYPE) > 0 );
    test_pass("cxx11:EPROTOTYPE");
#else
    test_skip("cxx11:EPROTOTYPE");
#endif
#if defined(EROFS)
    test_true( (EROFS) > 0 );
    test_pass("cxx11:EROFS");
#else
    test_skip("cxx11:EROFS");
#endif
#if defined(ESPIPE)
    test_true( (ESPIPE) > 0 );
    test_pass("cxx11:ESPIPE");
#else
    test_skip("cxx11:ESPIPE");
#endif
#if defined(ESRCH)
    test_true( (ESRCH) > 0 );
    test_pass("cxx11:ESRCH");
#else
    test_skip("cxx11:ESRCH");
#endif
#if defined(ETIMEDOUT)
    test_true( (ETIMEDOUT) > 0 );
    test_pass("cxx11:ETIMEDOUT");
#else
    test_skip("cxx11:ETIMEDOUT");
#endif
#if defined(ETXTBSY)
    test_true( (ETXTBSY) > 0 );
    test_pass("cxx11:ETXTBSY");
#else
    test_skip("cxx11:ETXTBSY");
#endif
#if defined(EWOULDBLOCK)
    test_true( (EWOULDBLOCK) > 0 );
    test_pass("cxx11:EWOULDBLOCK");
#else
    test_skip("cxx11:EWOULDBLOCK");
#endif
#if defined(EXDEV)
    test_true( (EXDEV) > 0 );
    test_pass("cxx11:EXDEV");
#else
    test_skip("cxx11:EXDEV");
#endif
}
