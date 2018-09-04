/* Automatically generated file. Do not edit. Edit wrapawk_macosx/wrapfunc.inp. */
#ifndef WRAPDEF_H
#define WRAPDEF_H


#undef WRAP_LSTAT
#define WRAP_LSTAT MY_DEF(WRAP_LSTAT_RAW)
#undef WRAP_STAT
#define WRAP_STAT MY_DEF(WRAP_STAT_RAW)
#undef WRAP_FSTAT
#define WRAP_FSTAT MY_DEF(WRAP_FSTAT_RAW)
#ifdef HAVE_FSTATAT
#undef WRAP_FSTATAT
#define WRAP_FSTATAT MY_DEF(WRAP_FSTATAT_RAW)
#endif /* HAVE_FSTATAT */

#ifdef STAT64_SUPPORT
#undef WRAP_LSTAT64
#define WRAP_LSTAT64 MY_DEF(WRAP_LSTAT64_RAW)
#undef WRAP_STAT64
#define WRAP_STAT64 MY_DEF(WRAP_STAT64_RAW)
#undef WRAP_FSTAT64
#define WRAP_FSTAT64 MY_DEF(WRAP_FSTAT64_RAW)
#ifdef HAVE_FSTATAT
#undef WRAP_FSTATAT64
#define WRAP_FSTATAT64 MY_DEF(WRAP_FSTATAT64_RAW)
#endif /* HAVE_FSTATAT */
#endif /* STAT64_SUPPORT */

#ifdef __APPLE__
#ifdef __LP64__
#define getattrlist my_getattrlist
#ifdef HAVE_FGETATTRLIST
#define fgetattrlist my_fgetattrlist
#endif
#else
#define getattrlist my_getattrlist
#ifdef HAVE_FGETATTRLIST
#define fgetattrlist my_fgetattrlist
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#define getattrlist$UNIX2003 my_getattrlist$UNIX2003
#endif
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#define lstat$INODE64 my_lstat$INODE64
#define stat$INODE64 my_stat$INODE64
#define fstat$INODE64 my_fstat$INODE64
#define posix_spawn my_posix_spawn
#define posix_spawnp my_posix_spawnp
#endif
#define execve my_execve
#define execl my_execl
#define execle my_execle
#define execlp my_execlp
#define execv my_execv
#define execvp my_execvp
#define execvP my_execvP
#endif /* ifdef __APPLE__ */

#undef WRAP_MKNOD
#define WRAP_MKNOD MY_DEF(WRAP_MKNOD_RAW)

#ifdef HAVE_FSTATAT
#ifdef HAVE_MKNODAT
#undef WRAP_MKNODAT
#define WRAP_MKNODAT MY_DEF(WRAP_MKNODAT_RAW)
#endif /* HAVE_MKNODAT */
#endif /* HAVE_FSTATAT */


#define chown my_chown
#define lchown my_lchown
#define fchown my_fchown
#define chmod my_chmod
#define fchmod my_fchmod
#if defined HAVE_LCHMOD
#define lchmod my_lchmod
#endif
#if defined __APPLE__ && !defined __LP64__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#define lchown$UNIX2003 my_lchown$UNIX2003
#define chmod$UNIX2003 my_chmod$UNIX2003
#define fchmod$UNIX2003 my_fchmod$UNIX2003
#endif
#endif /* if defined __APPLE__ && !defined __LP64__ */
#define mkdir my_mkdir
#define unlink my_unlink
#define rmdir my_rmdir
#define remove my_remove
#define rename my_rename

#ifdef FAKEROOT_FAKENET
#define fork my_fork
#define vfork my_vfork
#define close my_close
#define dup2 my_dup2
#endif /* FAKEROOT_FAKENET */


#define getuid my_getuid
#define getgid my_getgid
#define geteuid my_geteuid
#define getegid my_getegid
#define setuid my_setuid
#define setgid my_setgid
#define seteuid my_seteuid
#define setegid my_setegid
#define setreuid my_setreuid
#define setregid my_setregid
#if defined __APPLE__ && !defined __LP64__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#define setreuid$UNIX2003 my_setreuid$UNIX2003
#define setregid$UNIX2003 my_setregid$UNIX2003
#endif
#endif /* if defined __APPLE__ && !defined __LP64__ */
#ifdef HAVE_GETRESUID
#define getresuid my_getresuid
#endif /* HAVE_GETRESUID */
#ifdef HAVE_GETRESGID
#define getresgid my_getresgid
#endif /* HAVE_GETRESGID */
#ifdef HAVE_SETRESUID
#define setresuid my_setresuid
#endif /* HAVE_SETRESUID */
#ifdef HAVE_SETRESGID
#define setresgid my_setresgid
#endif /* HAVE_SETRESGID */
#ifdef HAVE_SETFSUID
#define setfsuid my_setfsuid
#endif /* HAVE_SETFSUID */
#ifdef HAVE_SETFSGID
#define setfsgid my_setfsgid
#endif /* HAVE_SETFSGID */
#define initgroups my_initgroups
#define setgroups my_setgroups

#ifdef HAVE_FSTATAT
#ifdef HAVE_FCHMODAT
#define fchmodat my_fchmodat
#endif /* HAVE_FCHMODAT */
#ifdef HAVE_FCHOWNAT
#define fchownat my_fchownat
#endif /* HAVE_FCHOWNAT */
#ifdef HAVE_MKDIRAT
#define mkdirat my_mkdirat
#endif /* HAVE_MKDIRAT */
#ifdef HAVE_OPENAT
#define openat my_openat
#endif /* HAVE_OPENAT */
#ifdef HAVE_RENAMEAT
#define renameat my_renameat
#endif /* HAVE_RENAMEAT */
#ifdef HAVE_UNLINKAT
#define unlinkat my_unlinkat
#endif /* HAVE_UNLINKAT */
#endif /* HAVE_FSTATAT */

#ifdef HAVE_ACL_T
#define acl_set_fd my_acl_set_fd
#define acl_set_file my_acl_set_file
#endif /* HAVE_ACL_T */

#ifdef HAVE_FTS_READ
#define fts_read my_fts_read
#ifdef __APPLE__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#define fts_read$INODE64 my_fts_read$INODE64
#endif
#endif /* ifdef __APPLE__ */
#endif /* HAVE_FTS_READ */
#ifdef HAVE_FTS_CHILDREN
#define fts_children my_fts_children
#ifdef __APPLE__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#define fts_children$INODE64 my_fts_children$INODE64
#endif
#endif /* ifdef __APPLE__ */
#endif /* HAVE_FTS_CHILDREN */

#ifdef __sun
#define sysinfo my_sysinfo
#endif
#endif
