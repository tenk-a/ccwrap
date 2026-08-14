// ccwrap <inttypes.h>
#pragma once

#include <ccwrap_common.h>

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef PRId8
#define PRId8       "d"
#define PRId16      "d"
#define PRId32      "d"
#define PRId64      _CCW_PFMT_LL "d"

#define PRIi8       "i"
#define PRIi16      "i"
#define PRIi32      "i"
#define PRIi64      _CCW_PFMT_LL "i"

#define PRIo8       "o"
#define PRIo16      "o"
#define PRIo32      "o"
#define PRIo64      _CCW_PFMT_LL "o"

#define PRIu8       "u"
#define PRIu16      "u"
#define PRIu32      "u"
#define PRIu64      _CCW_PFMT_LL "u"

#define PRIx8       "x"
#define PRIx16      "x"
#define PRIx32      "x"
#define PRIx64      _CCW_PFMT_LL "x"

#define PRIX8       "X"
#define PRIX16      "X"
#define PRIX32      "X"
#define PRIX64      _CCW_PFMT_LL "X"

#define PRIdLEAST8  PRId8
#define PRIdLEAST16 PRId16
#define PRIdLEAST32 PRId32
#define PRIdLEAST64 PRId64
#define PRIiLEAST8  PRIi8
#define PRIiLEAST16 PRIi16
#define PRIiLEAST32 PRIi32
#define PRIiLEAST64 PRIi64
#define PRIoLEAST8  PRIo8
#define PRIoLEAST16 PRIo16
#define PRIoLEAST32 PRIo32
#define PRIoLEAST64 PRIo64
#define PRIuLEAST8  PRIu8
#define PRIuLEAST16 PRIu16
#define PRIuLEAST32 PRIu32
#define PRIuLEAST64 PRIu64
#define PRIxLEAST8  PRIx8
#define PRIxLEAST16 PRIx16
#define PRIxLEAST32 PRIx32
#define PRIxLEAST64 PRIx64
#define PRIXLEAST8  PRIX8
#define PRIXLEAST16 PRIX16
#define PRIXLEAST32 PRIX32
#define PRIXLEAST64 PRIX64

#define PRIdFAST8   PRId8
#define PRIdFAST16  PRId32
#define PRIdFAST32  PRId32
#define PRIdFAST64  PRId64
#define PRIiFAST8   PRIi8
#define PRIiFAST16  PRIi32
#define PRIiFAST32  PRIi32
#define PRIiFAST64  PRIi64
#define PRIoFAST8   PRIo8
#define PRIoFAST16  PRIo32
#define PRIoFAST32  PRIo32
#define PRIoFAST64  PRIo64
#define PRIuFAST8   PRIu8
#define PRIuFAST16  PRIu32
#define PRIuFAST32  PRIu32
#define PRIuFAST64  PRIu64
#define PRIxFAST8   PRIx8
#define PRIxFAST16  PRIx32
#define PRIxFAST32  PRIx32
#define PRIxFAST64  PRIx64
#define PRIXFAST8   PRIX8
#define PRIXFAST16  PRIX32
#define PRIXFAST32  PRIX32
#define PRIXFAST64  PRIX64

#define PRIdMAX     PRId64
#define PRIiMAX     PRIi64
#define PRIoMAX     PRIo64
#define PRIuMAX     PRIu64
#define PRIxMAX     PRIx64
#define PRIXMAX     PRIX64

#if _CCW_PTR_BIT == 64
 #define PRIdPTR    PRId64
 #define PRIiPTR    PRIi64
 #define PRIoPTR    PRIo64
 #define PRIuPTR    PRIu64
 #define PRIxPTR    PRIx64
 #define PRIXPTR    PRIX64
#else
 #define PRIdPTR    PRId32
 #define PRIiPTR    PRIi32
 #define PRIoPTR    PRIo32
 #define PRIuPTR    PRIu32
 #define PRIxPTR    PRIx32
 #define PRIXPTR    PRIX32
#endif
#endif

#ifndef SCNd8
#define SCNd8       "hhd"
#define SCNd16      "hd"
#define SCNd32      "d"
#define SCNd64      _CCW_PFMT_LL "d"

#define SCNi8       "hhi"
#define SCNi16      "hi"
#define SCNi32      "i"
#define SCNi64      _CCW_PFMT_LL "i"

#define SCNo8       "hho"
#define SCNo16      "ho"
#define SCNo32      "o"
#define SCNo64      _CCW_PFMT_LL "o"

#define SCNu8       "hhu"
#define SCNu16      "hu"
#define SCNu32      "u"
#define SCNu64      _CCW_PFMT_LL "u"

#define SCNx8       "hhx"
#define SCNx16      "hx"
#define SCNx32      "x"
#define SCNx64      _CCW_PFMT_LL "x"

#define SCNdLEAST8  SCNd8
#define SCNdLEAST16 SCNd16
#define SCNdLEAST32 SCNd32
#define SCNdLEAST64 SCNd64
#define SCNiLEAST8  SCNi8
#define SCNiLEAST16 SCNi16
#define SCNiLEAST32 SCNi32
#define SCNiLEAST64 SCNi64
#define SCNoLEAST8  SCNo8
#define SCNoLEAST16 SCNo16
#define SCNoLEAST32 SCNo32
#define SCNoLEAST64 SCNo64
#define SCNuLEAST8  SCNu8
#define SCNuLEAST16 SCNu16
#define SCNuLEAST32 SCNu32
#define SCNuLEAST64 SCNu64
#define SCNxLEAST8  SCNx8
#define SCNxLEAST16 SCNx16
#define SCNxLEAST32 SCNx32
#define SCNxLEAST64 SCNx64

#define SCNdFAST8   SCNd8
#define SCNdFAST16  SCNd32
#define SCNdFAST32  SCNd32
#define SCNdFAST64  SCNd64
#define SCNiFAST8   SCNi8
#define SCNiFAST16  SCNi32
#define SCNiFAST32  SCNi32
#define SCNiFAST64  SCNi64
#define SCNoFAST8   SCNo8
#define SCNoFAST16  SCNo32
#define SCNoFAST32  SCNo32
#define SCNoFAST64  SCNo64
#define SCNuFAST8   SCNu8
#define SCNuFAST16  SCNu32
#define SCNuFAST32  SCNu32
#define SCNuFAST64  SCNu64
#define SCNxFAST8   SCNx8
#define SCNxFAST16  SCNx32
#define SCNxFAST32  SCNx32
#define SCNxFAST64  SCNx64

#define SCNdMAX     SCNd64
#define SCNiMAX     SCNi64
#define SCNoMAX     SCNo64
#define SCNuMAX     SCNu64
#define SCNxMAX     SCNx64

#if _CCW_PTR_BIT == 64
 #define SCNdPTR    SCNd64
 #define SCNiPTR    SCNi64
 #define SCNoPTR    SCNo64
 #define SCNuPTR    SCNu64
 #define SCNxPTR    SCNx64
#else
 #define SCNdPTR    SCNd32
 #define SCNiPTR    SCNi32
 #define SCNoPTR    SCNo32
 #define SCNuPTR    SCNu32
 #define SCNxPTR    SCNx32
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct imaxdiv_t {
    intmax_t    quot;
    intmax_t    rem;
} imaxdiv_t;

static __forceinline intmax_t   imaxabs(intmax_t j) { return (j < 0) ? -j : j; }
static __forceinline imaxdiv_t  imaxdiv(intmax_t number, intmax_t denom) { imaxdiv_t d = { number / denom, number % denom }; return d; }

static __forceinline intmax_t   strtoimax(const char* s, char** endp, int radix) { return (intmax_t)_strtoi64(s, endp, radix); }
static __forceinline uintmax_t  strtoumax(const char* s, char** endp, int radix) { return (uintmax_t)_strtoui64(s, endp, radix); }

static __forceinline intmax_t   wcstoimax(const wchar_t* s, wchar_t** endp, int radix) { return (intmax_t)_wcstoi64(s, endp, radix); }
static __forceinline uintmax_t  wcstoumax(const wchar_t* s, wchar_t** endp, int radix) { return (uintmax_t)_wcstoui64(s, endp, radix); }

#ifdef __cplusplus
}
#endif
