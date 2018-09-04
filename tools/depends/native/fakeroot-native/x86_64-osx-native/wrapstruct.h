/* Automatically generated file. Do not edit. Edit wrapawk_macosx/wrapfunc.inp. */
#ifndef WRAPSTRUCT_H
#define WRAPSTRUCT_H
typedef struct interpose_s {
  void *new_func;
  void *orig_func;
} interpose_t;
#define INTERPOSE(newf,oldf) \
  __attribute__((used)) static const interpose_t MY_GLUE2(_interpose_,oldf) \
    __attribute__((section("__DATA,__interpose"))) = {(void *)newf, (void *)oldf}



INTERPOSE(MY_DEF(WRAP_LSTAT_RAW),WRAP_LSTAT_RAW);
INTERPOSE(MY_DEF(WRAP_STAT_RAW),WRAP_STAT_RAW);
INTERPOSE(MY_DEF(WRAP_FSTAT_RAW),WRAP_FSTAT_RAW);
#ifdef HAVE_FSTATAT
INTERPOSE(MY_DEF(WRAP_FSTATAT_RAW),WRAP_FSTATAT_RAW);
#endif /* HAVE_FSTATAT */

#ifdef STAT64_SUPPORT
INTERPOSE(MY_DEF(WRAP_LSTAT64_RAW),WRAP_LSTAT64_RAW);
INTERPOSE(MY_DEF(WRAP_STAT64_RAW),WRAP_STAT64_RAW);
INTERPOSE(MY_DEF(WRAP_FSTAT64_RAW),WRAP_FSTAT64_RAW);
#ifdef HAVE_FSTATAT
INTERPOSE(MY_DEF(WRAP_FSTATAT64_RAW),WRAP_FSTATAT64_RAW);
#endif /* HAVE_FSTATAT */
#endif /* STAT64_SUPPORT */

#ifdef __APPLE__
#ifdef __LP64__
#undef getattrlist
INTERPOSE(my_getattrlist,getattrlist);
#define getattrlist my_getattrlist
#ifdef HAVE_FGETATTRLIST
#undef fgetattrlist
INTERPOSE(my_fgetattrlist,fgetattrlist);
#define fgetattrlist my_fgetattrlist
#endif
#else
#undef getattrlist
INTERPOSE(my_getattrlist,getattrlist);
#define getattrlist my_getattrlist
#ifdef HAVE_FGETATTRLIST
#undef fgetattrlist
INTERPOSE(my_fgetattrlist,fgetattrlist);
#define fgetattrlist my_fgetattrlist
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#undef getattrlist$UNIX2003
INTERPOSE(my_getattrlist$UNIX2003,getattrlist$UNIX2003);
#define getattrlist$UNIX2003 my_getattrlist$UNIX2003
#endif
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#undef lstat$INODE64
INTERPOSE(my_lstat$INODE64,lstat$INODE64);
#define lstat$INODE64 my_lstat$INODE64
#undef stat$INODE64
INTERPOSE(my_stat$INODE64,stat$INODE64);
#define stat$INODE64 my_stat$INODE64
#undef fstat$INODE64
INTERPOSE(my_fstat$INODE64,fstat$INODE64);
#define fstat$INODE64 my_fstat$INODE64
#undef posix_spawn
INTERPOSE(my_posix_spawn,posix_spawn);
#define posix_spawn my_posix_spawn
#undef posix_spawnp
INTERPOSE(my_posix_spawnp,posix_spawnp);
#define posix_spawnp my_posix_spawnp
#endif
#undef execve
INTERPOSE(my_execve,execve);
#define execve my_execve
#undef execl
INTERPOSE(my_execl,execl);
#define execl my_execl
#undef execle
INTERPOSE(my_execle,execle);
#define execle my_execle
#undef execlp
INTERPOSE(my_execlp,execlp);
#define execlp my_execlp
#undef execv
INTERPOSE(my_execv,execv);
#define execv my_execv
#undef execvp
INTERPOSE(my_execvp,execvp);
#define execvp my_execvp
#undef execvP
INTERPOSE(my_execvP,execvP);
#define execvP my_execvP
#endif /* ifdef __APPLE__ */

INTERPOSE(MY_DEF(WRAP_MKNOD_RAW),WRAP_MKNOD_RAW);

#ifdef HAVE_FSTATAT
#ifdef HAVE_MKNODAT
INTERPOSE(MY_DEF(WRAP_MKNODAT_RAW),WRAP_MKNODAT_RAW);
#endif /* HAVE_MKNODAT */
#endif /* HAVE_FSTATAT */


#undef chown
INTERPOSE(my_chown,chown);
#define chown my_chown
#undef lchown
INTERPOSE(my_lchown,lchown);
#define lchown my_lchown
#undef fchown
INTERPOSE(my_fchown,fchown);
#define fchown my_fchown
#undef chmod
INTERPOSE(my_chmod,chmod);
#define chmod my_chmod
#undef fchmod
INTERPOSE(my_fchmod,fchmod);
#define fchmod my_fchmod
#if defined HAVE_LCHMOD
#undef lchmod
INTERPOSE(my_lchmod,lchmod);
#define lchmod my_lchmod
#endif
#if defined __APPLE__ && !defined __LP64__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#undef lchown$UNIX2003
INTERPOSE(my_lchown$UNIX2003,lchown$UNIX2003);
#define lchown$UNIX2003 my_lchown$UNIX2003
#undef chmod$UNIX2003
INTERPOSE(my_chmod$UNIX2003,chmod$UNIX2003);
#define chmod$UNIX2003 my_chmod$UNIX2003
#undef fchmod$UNIX2003
INTERPOSE(my_fchmod$UNIX2003,fchmod$UNIX2003);
#define fchmod$UNIX2003 my_fchmod$UNIX2003
#endif
#endif /* if defined __APPLE__ && !defined __LP64__ */
#undef mkdir
INTERPOSE(my_mkdir,mkdir);
#define mkdir my_mkdir
#undef unlink
INTERPOSE(my_unlink,unlink);
#define unlink my_unlink
#undef rmdir
INTERPOSE(my_rmdir,rmdir);
#define rmdir my_rmdir
#undef remove
INTERPOSE(my_remove,remove);
#define remove my_remove
#undef rename
INTERPOSE(my_rename,rename);
#define rename my_rename

#ifdef FAKEROOT_FAKENET
#undef fork
INTERPOSE(my_fork,fork);
#define fork my_fork
#undef vfork
INTERPOSE(my_vfork,vfork);
#define vfork my_vfork
#undef close
INTERPOSE(my_close,close);
#define close my_close
#undef dup2
INTERPOSE(my_dup2,dup2);
#define dup2 my_dup2
#endif /* FAKEROOT_FAKENET */


#undef getuid
INTERPOSE(my_getuid,getuid);
#define getuid my_getuid
#undef getgid
INTERPOSE(my_getgid,getgid);
#define getgid my_getgid
#undef geteuid
INTERPOSE(my_geteuid,geteuid);
#define geteuid my_geteuid
#undef getegid
INTERPOSE(my_getegid,getegid);
#define getegid my_getegid
#undef setuid
INTERPOSE(my_setuid,setuid);
#define setuid my_setuid
#undef setgid
INTERPOSE(my_setgid,setgid);
#define setgid my_setgid
#undef seteuid
INTERPOSE(my_seteuid,seteuid);
#define seteuid my_seteuid
#undef setegid
INTERPOSE(my_setegid,setegid);
#define setegid my_setegid
#undef setreuid
INTERPOSE(my_setreuid,setreuid);
#define setreuid my_setreuid
#undef setregid
INTERPOSE(my_setregid,setregid);
#define setregid my_setregid
#if defined __APPLE__ && !defined __LP64__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#undef setreuid$UNIX2003
INTERPOSE(my_setreuid$UNIX2003,setreuid$UNIX2003);
#define setreuid$UNIX2003 my_setreuid$UNIX2003
#undef setregid$UNIX2003
INTERPOSE(my_setregid$UNIX2003,setregid$UNIX2003);
#define setregid$UNIX2003 my_setregid$UNIX2003
#endif
#endif /* if defined __APPLE__ && !defined __LP64__ */
#ifdef HAVE_GETRESUID
#undef getresuid
INTERPOSE(my_getresuid,getresuid);
#define getresuid my_getresuid
#endif /* HAVE_GETRESUID */
#ifdef HAVE_GETRESGID
#undef getresgid
INTERPOSE(my_getresgid,getresgid);
#define getresgid my_getresgid
#endif /* HAVE_GETRESGID */
#ifdef HAVE_SETRESUID
#undef setresuid
INTERPOSE(my_setresuid,setresuid);
#define setresuid my_setresuid
#endif /* HAVE_SETRESUID */
#ifdef HAVE_SETRESGID
#undef setresgid
INTERPOSE(my_setresgid,setresgid);
#define setresgid my_setresgid
#endif /* HAVE_SETRESGID */
#ifdef HAVE_SETFSUID
#undef setfsuid
INTERPOSE(my_setfsuid,setfsuid);
#define setfsuid my_setfsuid
#endif /* HAVE_SETFSUID */
#ifdef HAVE_SETFSGID
#undef setfsgid
INTERPOSE(my_setfsgid,setfsgid);
#define setfsgid my_setfsgid
#endif /* HAVE_SETFSGID */
#undef initgroups
INTERPOSE(my_initgroups,initgroups);
#define initgroups my_initgroups
#undef setgroups
INTERPOSE(my_setgroups,setgroups);
#define setgroups my_setgroups

#ifdef HAVE_FSTATAT
#ifdef HAVE_FCHMODAT
#undef fchmodat
INTERPOSE(my_fchmodat,fchmodat);
#define fchmodat my_fchmodat
#endif /* HAVE_FCHMODAT */
#ifdef HAVE_FCHOWNAT
#undef fchownat
INTERPOSE(my_fchownat,fchownat);
#define fchownat my_fchownat
#endif /* HAVE_FCHOWNAT */
#ifdef HAVE_MKDIRAT
#undef mkdirat
INTERPOSE(my_mkdirat,mkdirat);
#define mkdirat my_mkdirat
#endif /* HAVE_MKDIRAT */
#ifdef HAVE_OPENAT
#undef openat
INTERPOSE(my_openat,openat);
#define openat my_openat
#endif /* HAVE_OPENAT */
#ifdef HAVE_RENAMEAT
#undef renameat
INTERPOSE(my_renameat,renameat);
#define renameat my_renameat
#endif /* HAVE_RENAMEAT */
#ifdef HAVE_UNLINKAT
#undef unlinkat
INTERPOSE(my_unlinkat,unlinkat);
#define unlinkat my_unlinkat
#endif /* HAVE_UNLINKAT */
#endif /* HAVE_FSTATAT */

#ifdef HAVE_ACL_T
#undef acl_set_fd
INTERPOSE(my_acl_set_fd,acl_set_fd);
#define acl_set_fd my_acl_set_fd
#undef acl_set_file
INTERPOSE(my_acl_set_file,acl_set_file);
#define acl_set_file my_acl_set_file
#endif /* HAVE_ACL_T */

#ifdef HAVE_FTS_READ
#undef fts_read
INTERPOSE(my_fts_read,fts_read);
#define fts_read my_fts_read
#ifdef __APPLE__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#undef fts_read$INODE64
INTERPOSE(my_fts_read$INODE64,fts_read$INODE64);
#define fts_read$INODE64 my_fts_read$INODE64
#endif
#endif /* ifdef __APPLE__ */
#endif /* HAVE_FTS_READ */
#ifdef HAVE_FTS_CHILDREN
#undef fts_children
INTERPOSE(my_fts_children,fts_children);
#define fts_children my_fts_children
#ifdef __APPLE__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#undef fts_children$INODE64
INTERPOSE(my_fts_children$INODE64,fts_children$INODE64);
#define fts_children$INODE64 my_fts_children$INODE64
#endif
#endif /* ifdef __APPLE__ */
#endif /* HAVE_FTS_CHILDREN */

#ifdef __sun
#undef sysinfo
INTERPOSE(my_sysinfo,sysinfo);
#define sysinfo my_sysinfo
#endif

struct next_wrap_st next_wrap[]= {
  {NULL, NULL},
};
#endif
