#include "c_test.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

typedef struct ErrEnt { char const* name; int value; } ErrEnt;

static const ErrEnt kErrnoMacros[] = {
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
    { "EXDEV",           EXDEV           }
};
#define ERRNO_MACRO_COUNT (sizeof(kErrnoMacros) / sizeof(kErrnoMacros[0]))

static int allowed_alias(char const* a, char const* b) {
    static char const* const pairs[][2] = {
        { "EAGAIN",  "EWOULDBLOCK" },
        { "ENOTSUP", "EOPNOTSUPP"  }
    };
    size_t i;
    for (i = 0; i < sizeof(pairs) / sizeof(pairs[0]); ++i) {
        if ((strcmp(a, pairs[i][0]) == 0 && strcmp(b, pairs[i][1]) == 0) ||
            (strcmp(a, pairs[i][1]) == 0 && strcmp(b, pairs[i][0]) == 0))
            return 1;
    }
    return 0;
}

TEST_CASE(errno, macros_are_positive) {

    int bad = 0;
    size_t i;
    for (i = 0; i < ERRNO_MACRO_COUNT; ++i) {
        if (kErrnoMacros[i].value <= 0)
            ++bad;
    }

    test_gt( EDOM,   0 );
    test_pass("c1990:EDOM");
    test_gt( ERANGE, 0 );
    test_pass("c1990:ERANGE");

    test_gt( EILSEQ, 0 );
    test_pass("c1995:EILSEQ");

    test_eq( bad, 0 );
    test_pass("c2011:extended errno macros present and positive (property)");
}

TEST_CASE(errno, macros_are_distinct) {
    int dup = 0;
    size_t i, j;
    for (i = 0; i < ERRNO_MACRO_COUNT; ++i) {
        for (j = i + 1; j < ERRNO_MACRO_COUNT; ++j) {
            if (kErrnoMacros[i].value == kErrnoMacros[j].value &&
                !allowed_alias(kErrnoMacros[i].name, kErrnoMacros[j].name))
                ++dup;
        }
    }
    test_eq( dup, 0 );

    test_ne( EDOM,   ERANGE );
    test_ne( EDOM,   EILSEQ );
    test_ne( EINVAL, ERANGE );
    test_pass("c2011:E* macros are distinct (only POSIX aliases share a value)");
}

TEST_CASE(errno, errno_is_a_modifiable_lvalue) {
    int  e0, e1, e2, e3;
    int* p;

    errno = 0;
    e0 = errno;
    test_eq( e0, 0 );

    errno = EDOM;
    e1 = errno;
    test_eq( e1, EDOM );

    errno = ERANGE;
    e2 = errno;
    test_eq( e2, ERANGE );

    p = &errno;
    test_ptr( p );
    *p = EINVAL;
    e3 = errno;
    test_eq( e3, EINVAL );

    errno = 0;
    test_pass("c1990:errno (modifiable lvalue)");
}

TEST_CASE(errno, errno_set_by_the_library) {
    char* end;
    long  v, w;
    int   e, e2;

    errno = 0;
    end = 0;
    v = strtol("999999999999999999999999", &end, 10);
    e = errno;
    test_eq( e, ERANGE );
    test_eq( v, LONG_MAX );

    errno = 0;
    w = strtol("42", &end, 10);
    e2 = errno;
    test_eq( w, 42L );
    test_eq( e2, 0 );

    errno = 0;
    test_pass("c1990:errno set by the library (strtol -> ERANGE)");
}

TEST_CASE(errno, stdc_version_c23) {
#ifdef __STDC_VERSION_ERRNO_H__
    test_ge( __STDC_VERSION_ERRNO_H__, 202311L );
    test_pass("c2023:__STDC_VERSION_ERRNO_H__");
#else
    TEST_SKIP1(); TEST_NOTE("__STDC_VERSION_ERRNO_H__ not defined");
    test_skip("c2023:__STDC_VERSION_ERRNO_H__");
#endif
}


TEST_CASE(errno, errno_t_c2011) {
#if defined(__STDC_LIB_EXT1__)
    {
        errno_t e = (errno_t)0;
        test_true( e == 0 );
        test_true( sizeof(errno_t) == sizeof(int) );
    }
    test_pass("c2011:opt:errno_t");
#else
    TEST_NOTE("Annex K is optional; this library does not provide it");
    test_skip("c2011:opt:errno_t");
#endif
}

TEST_SUITE(errno)
    TEST_ADD(macros_are_positive)
    TEST_ADD(macros_are_distinct)
    TEST_ADD(errno_is_a_modifiable_lvalue)
    TEST_ADD(errno_set_by_the_library)
    TEST_ADD(stdc_version_c23)
    TEST_ADD(errno_t_c2011)
TEST_SUITE_END(errno)
