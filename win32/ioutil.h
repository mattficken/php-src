/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Anatol Belski <ab@php.net>                                   |
   +----------------------------------------------------------------------+
*/

/* This file integrates several modified parts from the libuv project, which
 * is copyrighted to
 *
 * Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */ 

#ifndef PHP_WIN32_IOUTIL_H
#define PHP_WIN32_IOUTIL_H

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

#include "win32/winutil.h"

/* Defining to 1 will force the APIs old behaviors as a fallback. */
#define PHP_WIN32_IOUTIL_ANSI_COMPAT_MODE 1

#ifdef PHP_EXPORTS
# define PW32IO __declspec(dllexport)
#else
# define PW32IO __declspec(dllimport)
#endif

#ifndef mode_t
typedef unsigned short mode_t;
#endif

typedef struct {
	DWORD access;
	DWORD share;
	DWORD disposition;
	DWORD attributes;
} php_ioutil_open_opts;

typedef struct {
	unsigned long  ReparseTag;
	unsigned short ReparseDataLength;
	unsigned short Reserved;
	union {
		struct {
			unsigned short SubstituteNameOffset;
			unsigned short SubstituteNameLength;
			unsigned short PrintNameOffset;
			unsigned short PrintNameLength;
			unsigned long  Flags;
			wchar_t        ReparseTarget[1];
		} SymbolicLinkReparseBuffer;
		struct {
			unsigned short SubstituteNameOffset;
			unsigned short SubstituteNameLength;
			unsigned short PrintNameOffset;
			unsigned short PrintNameLength;
			wchar_t        ReparseTarget[1];
		} MountPointReparseBuffer;
		struct {
			unsigned char  ReparseTarget[1];
		} GenericReparseBuffer;
	};
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

#ifdef CTL_CODE
#undef CTL_CODE
#endif
#define CTL_CODE(DeviceType,Function,Method,Access) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define FILE_DEVICE_FILE_SYSTEM 0x00000009
#define METHOD_BUFFERED		0
#define FILE_ANY_ACCESS 	0
#define FSCTL_GET_REPARSE_POINT CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 42, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE  ( 16 * 1024 )

#ifndef O_ASYNC
# define O_ASYNC 0x100000
#endif

#ifndef O_SYNC
# define O_SYNC 0x200000
#endif

#define PHP_WIN32_IOUTIL_MAXPATHLEN 32767
/* This might be tricky, but this way we override MAXPATHLEN in the files
   where this header is included, so the win32 io utils are supported. */
#if !defined(MAXPATHLEN) || MAXPATHLEN < PHP_WIN32_IOUTIL_MAXPATHLEN
# undef MAXPATHLEN
# define MAXPATHLEN PHP_WIN32_IOUTIL_MAXPATHLEN
#endif

#define PHP_WIN32_IOUTIL_DEFAULT_SLASH '\\'
#define PHP_WIN32_IOUTIL_DEFAULT_DIR_SEPARATOR	';'
#define PHP_WIN32_IOUTIL_IS_SLASH(c)	((c) == '/' || (c) == '\\')
#define PHP_WIN32_IOUTIL_IS_LETTER(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define PHP_WIN32_IOUTIL_JUNCTION_PREFIX "\\??\\"
#define PHP_WIN32_IOUTIL_JUNCTION_PREFIX_LEN 4
#define PHP_WIN32_IOUTIL_LONG_PATH_PREFIX "\\\\?\\"
#define PHP_WIN32_IOUTIL_LONG_PATH_PREFIX_LEN 4
#define PHP_WIN32_IOUTIL_UNC_PATH_PREFIX "\\\\?\\UNC\\"
#define PHP_WIN32_IOUTIL_UNC_PATH_PREFIX_LEN 8

#define PHP_WIN32_IOUTIL_IS_LONG_PATH(path, path_len) (path_len >= PHP_WIN32_IOUTIL_LONG_PATH_PREFIX_LEN \
	&& 0 == strncmp((path), PHP_WIN32_IOUTIL_LONG_PATH_PREFIX, PHP_WIN32_IOUTIL_LONG_PATH_PREFIX_LEN))
#define PHP_WIN32_IOUTIL_IS_ABSOLUTE(path, path_len) (PHP_WIN32_IOUTIL_IS_LONG_PATH(path, path_len) \
	|| path_len >= 3 && PHP_WIN32_IOUTIL_IS_LETTER(path[0]) && ':' == path[1] && IS_SLASH(path[2]))

#define PHP_WIN32_IOUTIL_DEFAULT_SLASHW L'\\'
#define PHP_WIN32_IOUTIL_DEFAULT_DIR_SEPARATORW	L';'
#define PHP_WIN32_IOUTIL_IS_SLASHW(c) ((c) == L'\\' || (c) == L'/')
#define PHP_WIN32_IOUTIL_IS_LETTERW(c) (((c) >= L'a' && (c) <= L'z') || ((c) >= L'A' && (c) <= L'Z'))
#define PHP_WIN32_IOUTIL_JUNCTION_PREFIXW L"\\??\\"
#define PHP_WIN32_IOUTIL_JUNCTION_PREFIX_LENW 4
#define PHP_WIN32_IOUTIL_LONG_PATH_PREFIXW L"\\\\?\\"
#define PHP_WIN32_IOUTIL_LONG_PATH_PREFIX_LENW 4
#define PHP_WIN32_IOUTIL_UNC_PATH_PREFIXW L"\\\\?\\UNC\\"
#define PHP_WIN32_IOUTIL_UNC_PATH_PREFIX_LENW 8

#define PHP_WIN32_IOUTIL_IS_LONG_PATHW(pathw, path_lenw) (path_lenw >= PHP_WIN32_IOUTIL_LONG_PATH_PREFIX_LENW \
	&& 0 == wcsncmp((pathw), PHP_WIN32_IOUTIL_LONG_PATH_PREFIXW, PHP_WIN32_IOUTIL_LONG_PATH_PREFIX_LENW))
#define PHP_WIN32_IOUTIL_IS_ABSOLUTEW(pathw, path_lenw) (PHP_WIN32_IOUTIL_IS_LONG_PATHW(pathw, path_lenw) \
	|| path_lenw >= 3 && PHP_WIN32_IOUTIL_IS_LETTERW(pathw[0]) && L':' == pathw[1] && IS_SLASHW(pathw[2]))

/* these are not defined in win32 headers */
#ifndef W_OK
#define W_OK 0x02
#endif
#ifndef R_OK
#define R_OK 0x04
#endif
#ifndef X_OK
#define X_OK 0x01
#endif
#ifndef F_OK
#define F_OK 0x00
#endif

PW32IO wchar_t *php_win32_ioutil_mb_to_w(const char* path);
PW32IO char *php_win32_ioutil_w_to_utf8(wchar_t* w_source_ptr);

PW32IO int php_win32_ioutil_close(int fd);
PW32IO BOOL php_win32_ioutil_posix_to_open_opts(int flags, mode_t mode, php_ioutil_open_opts *opts);
PW32IO int php_win32_ioutil_mkdir(const char *path, mode_t mode);

#if PHP_WIN32_IOUTIL_ANSI_COMPAT_MODE
PW32IO int php_win32_ioutil_sys_stat_ex_a(const char *path, zend_stat_t *buf, int lstat);
PW32IO int php_win32_ioutil_mkdir_a(const char *path, mode_t mode);
PW32IO int php_win32_ioutil_open_a(const char *path, int flags, ...);
#endif

PW32IO int php_win32_ioutil_sys_stat_ex_w(const wchar_t *path, zend_stat_t *buf, int lstat);
PW32IO int php_win32_ioutil_mkdir_w(const wchar_t *path, mode_t mode);
PW32IO int php_win32_ioutil_open_w(const wchar_t *path, int flags, ...);

#if 0
PW32IO int php_win32_ioutil_access_a(const char *path, mode_t mode);
PW32IO int php_win32_ioutil_access_w(const wchar_t *path, mode_t mode);
PW32IO int php_win32_ioutil_open(const char *path, int flags, ...);
#endif

#define PHP_WIN32_IOUTIL_INIT_W(path) \
	const char *patha = path; \
	wchar_t *pathw = php_win32_ioutil_mb_to_w(path); \
	BOOL use_w = (NULL != pathw);

#define PHP_WIN32_IOUTIL_CLEANUP_W() \
	if (pathw) { \
		free(pathw); \
	}

/* A boolean use_w has to be defined for this to work. */
#if PHP_WIN32_IOUTIL_ANSI_COMPAT_MODE
#define php_win32_ioutil_access_cond(path, mode) (use_w) ? _waccess(pathw, mode) : _access(patha, mode) 
__forceinline static int php_win32_ioutil_access(const char *path, mode_t mode)
{
	PHP_WIN32_IOUTIL_INIT_W(path);
	int ret;

	if (use_w) {
		ret = _waccess(pathw, mode);
	} else {
		ret = _access(patha, mode);
	}

	PHP_WIN32_IOUTIL_CLEANUP_W();

	return ret;
}

#define php_win32_ioutil_sys_stat_ex_cond(path, buf, lstat) (use_w) ? php_win32_ioutil_sys_stat_ex_w(pathw, buf, lstat) : php_win32_ioutil_sys_stat_ex_a(patha, buf, lstat)
__forceinline static int php_win32_ioutil_sys_stat_ex(const char *path, zend_stat_t *buf, int lstat)
{
	PHP_WIN32_IOUTIL_INIT_W(path);
	int ret;

	if (use_w) {
		ret = php_win32_ioutil_sys_stat_ex_w(pathw, buf, lstat);
	} else {
		ret = php_win32_ioutil_sys_stat_ex_a(patha, buf, lstat);
	}

	PHP_WIN32_IOUTIL_CLEANUP_W();

	if (0 > ret) {
		DWORD err = GetLastError();
		SET_ERRNO_FROM_WIN32_CODE(err);
	}

	return ret;
}

__forceinline static int php_win32_ioutil_open(const char *path, int flags, ...)
{
	mode_t mode = 0;
	PHP_WIN32_IOUTIL_INIT_W(path);
	int ret = -1;

	if (flags & O_CREAT) {
		va_list arg;

		va_start(arg, flags);
		mode = (mode_t) va_arg(arg, int);
		va_end(arg);
	}

	if (use_w) {
		ret = php_win32_ioutil_open_w(pathw, flags, mode);
	} else {
		ret = php_win32_ioutil_open_a(patha, flags, mode);
	}

	if (0 > ret) {
		DWORD err = GetLastError();
		SET_ERRNO_FROM_WIN32_CODE(err);
	}

	PHP_WIN32_IOUTIL_CLEANUP_W();

	return ret;
}

#define php_win32_ioutil_unlink_cond(path) (use_w) ? php_win32_ioutil_unlink_w(path) : php_win32_ioutil_unlink_a(path);
__forceinline static int php_win32_ioutil_unlink(const char *path)
{
	PHP_WIN32_IOUTIL_INIT_W(path);
	int ret = 0;
	DWORD err = 0;

	if (use_w) {
		if (!DeleteFileW(pathw)) {
			err = GetLastError();
			ret = -1;
		}
	} else {
		if (!DeleteFileA(patha)) {
			err = GetLastError();
			ret = -1;
		}
	}

	PHP_WIN32_IOUTIL_CLEANUP_W();

	if (0 > ret) {
		SET_ERRNO_FROM_WIN32_CODE(err);
	}

	return ret;
}

#define php_win32_ioutil_rmdir_cond(path) (use_w) ? php_win32_ioutil_rmdir_w(path) : php_win32_ioutil_rmdir_a(path);
__forceinline static int php_win32_ioutil_rmdir(const char *path)
{
	PHP_WIN32_IOUTIL_INIT_W(path);
	int ret = 0;
	DWORD err = 0;

	if (use_w) {
		if (!RemoveDirectoryW(pathw)) {
			err = GetLastError();
			ret = -1;
		}
	} else {
		if (!RemoveDirectoryA(patha)) {
			err = GetLastError();
			ret = -1;
		}
	}

	PHP_WIN32_IOUTIL_CLEANUP_W();

	if (0 > ret) {
		SET_ERRNO_FROM_WIN32_CODE(err);
	}

	return ret;
}
#else /* no ANSI compat mode */
#define php_win32_ioutil_access_cond _waccess
#define php_win32_ioutil_access _waccess
#define php_win32_ioutil_sys_stat_ex_cond php_win32_ioutil_sys_stat_ex_w
#define php_win32_ioutil_sys_stat_ex php_win32_ioutil_sys_stat_ex_w
#define php_win32_ioutil_open php_win32_ioutil_open_w
#define php_win32_ioutil_unlink_cond php_win32_ioutil_unlink_w
#define php_win32_ioutil_unlink php_win32_ioutil_unlink_w
#define php_win32_ioutil_rmdir_cond php_win32_ioutil_rmdir_w
#define php_win32_ioutil_rmdir php_win32_ioutil_rmdir_w
#endif

#if 0
/* Prepared. This should be used instead of php_win32_ioutil_mb_to_w to get paths prepended with \\?\ transparently. */
zend_always_inline static wchar_t *php_win32_ioutil_path_get_w(char *path)
{
	wchar_t *ret;



}

zend_always_inline static HANDLE php_win32_ioutil_get_file_handle(char *path, WIN32_FIND_DATA *data)
{
		HANDLE ret = FindFirstFileA(path, data);

}
#endif

#endif /* PHP_WIN32_IOUTIL_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
