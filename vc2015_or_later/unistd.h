#ifndef UNISTD_H_INCLUDED
#define UNISTD_H_INCLUDED

#include <io.h>
#include <sys/types.h>
#include <direct.h>
#include <process.h>

#if 1
typedef long		off_t;
#endif
typedef int64_t		off64_t;

#ifdef __STDC__

static __ccwrap_force_inline	int   	access(const char * fname, int mode) { return _access(fname, mode); }
static __ccwrap_force_inline	int   	chmod(const char * fname, int mode) { return _chmod(fname, mode); }
static __ccwrap_force_inline	int   	close(int hdl) { return _close(hdl); }
static __ccwrap_force_inline	int   	creat(const char * fname, int mode) { return _creat(fname, mode); }
static __ccwrap_force_inline	int   	dup(int hdl) { return _dup(hdl); }
static __ccwrap_force_inline	int   	dup2(int srcHdl, int dstHdl) { return _dup2(srcHdl, dstHdl); }
static __ccwrap_force_inline	int   	eof(int hdl) { return _eof(hdl); }
static __ccwrap_force_inline	int   	isatty(int hdl) { return isatty(hdl); }
static __ccwrap_force_inline	off_t  	lseek(int hdl, off_t ofs, int whence) { return _lseek(hdl, ofs, whence); }
static __ccwrap_force_inline	char* 	mktemp(char * tmp_name) { return _mktemp(tmp_name); }
#ifdef __cplusplus
static __ccwrap_force_inline	int   	open(const char * fname, int flag, int mode=0) { return _open(fname, flag, mode); }
#else
static __ccwrap_force_inline	int   	open(const char * fname, int flag, int mode) { return _open(fname, flag, mode); }
#endif
static __ccwrap_force_inline	int   	read(int hdl, void * buf, unsigned bytes) { return _read(hdl, buf, bytes); }
static __ccwrap_force_inline	off_t  	tell(int hdl) { return _tell(hdl); }
static __ccwrap_force_inline	int   	write(int hdl, const void * buf, unsigned bytes) { return _write(hdl, buf, bytes); }

static __ccwrap_force_inline	char*	getcwd(char *dst, int dstSz) { return _getcwd(dst, dstSz); }
static __ccwrap_force_inline	int		chdir(char const* dir) { return _chdir(dir); }
static __ccwrap_force_inline	int 	mkdir(char const* dir) { return _mkdir(dir); }
static __ccwrap_force_inline	int 	rmdir(char const* dir) { return _rmdir(dir); }

#endif

#endif
