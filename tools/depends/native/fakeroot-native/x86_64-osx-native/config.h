/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* for packed */
#define FAKEROOT_ATTR(x) __attribute__ ((x))

/* store path in the database instead of inode and device */
/* #undef FAKEROOT_DB_PATH */

/* use TCP instead of SysV IPC */
/* #undef FAKEROOT_FAKENET */

/* acl_t data type and associated functions are provided by OS */
#define HAVE_ACL_T 1

/* Define to 1 if the compiler understands __builtin_expect. */
#define HAVE_BUILTIN_EXPECT 1

/* Define to 1 if you have the declaration of `setenv', and to 0 if you don't.
   */
#define HAVE_DECL_SETENV 1

/* Define to 1 if you have the declaration of `unsetenv', and to 0 if you
   don't. */
#define HAVE_DECL_UNSETENV 1

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <endian.h> header file. */
/* #undef HAVE_ENDIAN_H */

/* Define to 1 if you have the `fchmodat' function. */
#define HAVE_FCHMODAT 1

/* Define to 1 if you have the `fchownat' function. */
#define HAVE_FCHOWNAT 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <features.h> header file. */
/* #undef HAVE_FEATURES_H */

/* Define to 1 if you have the `fgetattrlist' function. */
#define HAVE_FGETATTRLIST 1

/* Define to 1 if you have the `fstatat' function. */
#define HAVE_FSTATAT 1

/* Define to 1 if you have the `fts_children' function. */
#define HAVE_FTS_CHILDREN 1

/* Define to 1 if you have the <fts.h> header file. */
#define HAVE_FTS_H 1

/* Define to 1 if you have the `fts_read' function. */
#define HAVE_FTS_READ 1

/* Define to 1 if you have the `getresgid' function. */
/* #undef HAVE_GETRESGID */

/* Define to 1 if you have the `getresuid' function. */
/* #undef HAVE_GETRESUID */

/* Define to 1 if you have the <grp.h> header file. */
#define HAVE_GRP_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `lchmod' function. */
#define HAVE_LCHMOD 1

/* Define to 1 if you have the `dl' library (-ldl). */
#define HAVE_LIBDL 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
/* #undef HAVE_LIBPTHREAD */

/* Define to 1 if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mkdirat' function. */
#define HAVE_MKDIRAT 1

/* Define to 1 if you have the `mknodat' function. */
/* #undef HAVE_MKNODAT */

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the `openat' function. */
/* #undef HAVE_OPENAT */

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have the `renameat' function. */
#define HAVE_RENAMEAT 1

/* have the semun union */
#define HAVE_SEMUN_DEF 1

/* Define to 1 if you have the `setenv' function. */
#define HAVE_SETENV 1

/* Define to 1 if you have the `setfsgid' function. */
/* #undef HAVE_SETFSGID */

/* Define to 1 if you have the `setfsuid' function. */
/* #undef HAVE_SETFSUID */

/* Define to 1 if you have the `setresgid' function. */
/* #undef HAVE_SETRESGID */

/* Define to 1 if you have the `setresuid' function. */
/* #undef HAVE_SETRESUID */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the <sys/acl.h> header file. */
#define HAVE_SYS_ACL_H 1

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/feature_tests.h> header file. */
/* #undef HAVE_SYS_FEATURE_TESTS_H */

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sysmacros.h> header file. */
/* #undef HAVE_SYS_SYSMACROS_H */

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `unlinkat' function. */
#define HAVE_UNLINKAT 1

/* second argument of initgroups */
#define INITGROUPS_SECOND_ARG int

/* path to libc shared object */
#define LIBCPATH "/usr/lib/libSystem.dylib"

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "fakeroot"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "clint@debian.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "fakeroot"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "fakeroot macosx-v3.3"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "fakeroot"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "macosx-v3.3"

/* type of readlink bufsize */
#define READLINK_BUFSIZE_TYPE size_t

/* type of readlink buf */
#define READLINK_BUF_TYPE char

/* type of readlink return value */
#define READLINK_RETVAL_TYPE ssize_t

/* type of setgroups size */
#define SETGROUPS_SIZE_TYPE int

/* argument of setregid */
#define SETREGID_ARG gid_t

/* argument of setreuid */
#define SETREUID_ARG gid_t

/* second argument of stat */
#define STAT_SECOND_ARG struct stat *

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* stat-struct conversion hackery */
/* #undef STUPID_ALPHA_HACK */

/* Version number of package */
#define VERSION "macosx-v3.3"

/* Stuff.  */
#define WRAP_STAT stat
#define WRAP_STAT_QUOTE "stat"
#define WRAP_STAT_RAW stat
#define TMP_STAT tmp_stat
#define NEXT_STAT_NOARG next_stat

#define WRAP_LSTAT_QUOTE "lstat"
#define WRAP_LSTAT lstat
#define WRAP_LSTAT_RAW lstat
#define TMP_LSTAT tmp_lstat
#define NEXT_LSTAT_NOARG next_lstat

#define WRAP_FSTAT_QUOTE "fstat"
#define WRAP_FSTAT fstat
#define WRAP_FSTAT_RAW fstat
#define TMP_FSTAT tmp_fstat
#define NEXT_FSTAT_NOARG next_fstat

#define WRAP_FSTATAT_QUOTE "fstatat"
#define WRAP_FSTATAT fstatat
#define WRAP_FSTATAT_RAW fstatat
#define TMP_FSTATAT tmp_fstatat
#define NEXT_FSTATAT_NOARG next_fstatat

#define WRAP_STAT64_QUOTE "stat64"
#define WRAP_STAT64 stat64
#define WRAP_STAT64_RAW stat64
#define TMP_STAT64 tmp_stat64
#define NEXT_STAT64_NOARG next_stat64

#define WRAP_LSTAT64_QUOTE "lstat64"
#define WRAP_LSTAT64 lstat64
#define WRAP_LSTAT64_RAW lstat64
#define TMP_LSTAT64 tmp_lstat64
#define NEXT_LSTAT64_NOARG next_lstat64

#define WRAP_FSTAT64_QUOTE "fstat64"
#define WRAP_FSTAT64 fstat64
#define WRAP_FSTAT64_RAW fstat64
#define TMP_FSTAT64 tmp_fstat64
#define NEXT_FSTAT64_NOARG next_fstat64

#define WRAP_FSTATAT64_QUOTE "fstatat64"
#define WRAP_FSTATAT64 fstatat64
#define WRAP_FSTATAT64_RAW fstatat64
#define TMP_FSTATAT64 tmp_fstatat64
#define NEXT_FSTATAT64_NOARG next_fstatat64

#define WRAP_MKNOD_QUOTE "mknod"
#define WRAP_MKNOD mknod
#define WRAP_MKNOD_RAW mknod
#define TMP_MKNOD tmp_mknod
#define NEXT_MKNOD_NOARG next_mknod

#define WRAP_MKNODAT_QUOTE  __amknodat
#define WRAP_MKNODAT  __amknodat
#define WRAP_MKNODAT_RAW  __amknodat
#define TMP_MKNODAT  __amknodat
#define NEXT_MKNODAT_NOARG  next___amknodat


/* fifth argument of __xmknodat */
#define XMKNODAT_FIFTH_ARG /**/

/* fourth argument of __xmknod */
#define XMKNOD_FRTH_ARG /**/

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `long' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `unsigned' if <sys/types.h> does not define. */
/* #undef size_t */

#if ! HAVE_BUILTIN_EXPECT
#define __builtin_expect(x, expected_value) (x)
#endif
