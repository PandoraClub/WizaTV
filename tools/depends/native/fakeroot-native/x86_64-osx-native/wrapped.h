/* Automatically generated file. Do not edit. Edit wrapawk_macosx/wrapfunc.inp. */
#ifndef WRAPPED_H
#define WRAPPED_H
#define MY_GLUE2(a,b) a ## b
#define MY_DEF(a) MY_GLUE2(my_,a)


extern int MY_DEF(WRAP_LSTAT)LSTAT_ARG(int ver, const char *file_name, struct stat *buf) __attribute__((visibility("hidden")));
extern int MY_DEF(WRAP_STAT)STAT_ARG(int ver, const char *file_name, struct stat *buf) __attribute__((visibility("hidden")));
extern int MY_DEF(WRAP_FSTAT)FSTAT_ARG(int ver, int fd, struct stat *buf) __attribute__((visibility("hidden")));
#ifdef HAVE_FSTATAT
extern int MY_DEF(WRAP_FSTATAT)FSTATAT_ARG(int ver, int dir_fd, const char *path, struct stat *buf, int flags) __attribute__((visibility("hidden")));
#endif /* HAVE_FSTATAT */

#ifdef STAT64_SUPPORT
extern int MY_DEF(WRAP_LSTAT64)LSTAT64_ARG(int ver, const char *file_name, struct stat64 *buf) __attribute__((visibility("hidden")));
extern int MY_DEF(WRAP_STAT64)STAT64_ARG(int ver, const char *file_name, struct stat64 *buf) __attribute__((visibility("hidden")));
extern int MY_DEF(WRAP_FSTAT64)FSTAT64_ARG(int ver, int fd, struct stat64 *buf) __attribute__((visibility("hidden")));
#ifdef HAVE_FSTATAT
extern int MY_DEF(WRAP_FSTATAT64)FSTATAT64_ARG(int ver, int dir_fd, const char *path, struct stat64 *buf, int flags) __attribute__((visibility("hidden")));
#endif /* HAVE_FSTATAT */
#endif /* STAT64_SUPPORT */

#ifdef __APPLE__
#ifdef __LP64__
extern int my_getattrlist (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned int options) __attribute__((visibility("hidden")));
#ifdef HAVE_FGETATTRLIST
extern int my_fgetattrlist (int fd, void *attrList, void *attrBuf, size_t attrBufSize, unsigned int options) __attribute__((visibility("hidden")));
#endif
#else
extern int my_getattrlist (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options) __attribute__((visibility("hidden")));
#ifdef HAVE_FGETATTRLIST
extern int my_fgetattrlist (int fd, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options) __attribute__((visibility("hidden")));
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern int my_getattrlist$UNIX2003 (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options) __attribute__((visibility("hidden")));
#endif
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern int my_lstat$INODE64 (const char *file_name, struct stat *buf) __attribute__((visibility("hidden")));
extern int my_stat$INODE64 (const char *file_name, struct stat *buf) __attribute__((visibility("hidden")));
extern int my_fstat$INODE64 (int fd, struct stat *buf) __attribute__((visibility("hidden")));
extern int my_posix_spawn (pid_t * __restrict pid, const char * __restrict path, const posix_spawn_file_actions_t *file_actions, const posix_spawnattr_t * __restrict attrp, char *const argv[ __restrict], char *const envp[ __restrict]) __attribute__((visibility("hidden")));
extern int my_posix_spawnp (pid_t * __restrict pid, const char * __restrict path, const posix_spawn_file_actions_t *file_actions, const posix_spawnattr_t * __restrict attrp, char *const argv[ __restrict], char *const envp[ __restrict]) __attribute__((visibility("hidden")));
#endif
extern int my_execve (const char *path, char *const argv[], char *const envp[]) __attribute__((visibility("hidden")));
extern int my_execl (const char *path, const char *arg0, ...) __attribute__((visibility("hidden")));
extern int my_execle (const char *path, const char *arg0, ...) __attribute__((visibility("hidden")));
extern int my_execlp (const char *path, const char *arg0, ...) __attribute__((visibility("hidden")));
extern int my_execv (const char *path, char *const argv[]) __attribute__((visibility("hidden")));
extern int my_execvp (const char *file, char *const argv[]) __attribute__((visibility("hidden")));
extern int my_execvP (const char *file, const char *search_path, char *const argv[]) __attribute__((visibility("hidden")));
#endif /* ifdef __APPLE__ */

extern int MY_DEF(WRAP_MKNOD)MKNOD_ARG(int ver, const char *pathname, mode_t mode, dev_t XMKNOD_FRTH_ARG dev) __attribute__((visibility("hidden")));

#ifdef HAVE_FSTATAT
#ifdef HAVE_MKNODAT
extern int MY_DEF(WRAP_MKNODAT)MKNODAT_ARG(int ver, int dir_fd, const char *pathname, mode_t mode, dev_t dev) __attribute__((visibility("hidden")));
#endif /* HAVE_MKNODAT */
#endif /* HAVE_FSTATAT */


extern int my_chown (const char *path, uid_t owner, gid_t group) __attribute__((visibility("hidden")));
extern int my_lchown (const char *path, uid_t owner, gid_t group) __attribute__((visibility("hidden")));
extern int my_fchown (int fd, uid_t owner, gid_t group) __attribute__((visibility("hidden")));
extern int my_chmod (const char *path, mode_t mode) __attribute__((visibility("hidden")));
extern int my_fchmod (int fd, mode_t mode) __attribute__((visibility("hidden")));
#if defined HAVE_LCHMOD
extern int my_lchmod (const char *path, mode_t mode) __attribute__((visibility("hidden")));
#endif
#if defined __APPLE__ && !defined __LP64__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern int my_lchown$UNIX2003 (const char *path, uid_t owner, gid_t group) __attribute__((visibility("hidden")));
extern int my_chmod$UNIX2003 (const char *path, mode_t mode) __attribute__((visibility("hidden")));
extern int my_fchmod$UNIX2003 (int fd, mode_t mode) __attribute__((visibility("hidden")));
#endif
#endif /* if defined __APPLE__ && !defined __LP64__ */
extern int my_mkdir (const char *path, mode_t mode) __attribute__((visibility("hidden")));
extern int my_unlink (const char *pathname) __attribute__((visibility("hidden")));
extern int my_rmdir (const char *pathname) __attribute__((visibility("hidden")));
extern int my_remove (const char *pathname) __attribute__((visibility("hidden")));
extern int my_rename (const char *oldpath, const char *newpath) __attribute__((visibility("hidden")));

#ifdef FAKEROOT_FAKENET
extern pid_t my_fork (void) __attribute__((visibility("hidden")));
extern pid_t my_vfork (void) __attribute__((visibility("hidden")));
extern int my_close (int fd) __attribute__((visibility("hidden")));
extern int my_dup2 (int oldfd, int newfd) __attribute__((visibility("hidden")));
#endif /* FAKEROOT_FAKENET */


extern uid_t my_getuid (void) __attribute__((visibility("hidden")));
extern gid_t my_getgid (void) __attribute__((visibility("hidden")));
extern uid_t my_geteuid (void) __attribute__((visibility("hidden")));
extern gid_t my_getegid (void) __attribute__((visibility("hidden")));
extern int my_setuid (uid_t id) __attribute__((visibility("hidden")));
extern int my_setgid (gid_t id) __attribute__((visibility("hidden")));
extern int my_seteuid (uid_t id) __attribute__((visibility("hidden")));
extern int my_setegid (gid_t id) __attribute__((visibility("hidden")));
extern int my_setreuid (SETREUID_ARG ruid, SETREUID_ARG euid) __attribute__((visibility("hidden")));
extern int my_setregid (SETREGID_ARG rgid, SETREGID_ARG egid) __attribute__((visibility("hidden")));
#if defined __APPLE__ && !defined __LP64__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern int my_setreuid$UNIX2003 (SETREUID_ARG ruid, SETREUID_ARG euid) __attribute__((visibility("hidden")));
extern int my_setregid$UNIX2003 (SETREGID_ARG rgid, SETREGID_ARG egid) __attribute__((visibility("hidden")));
#endif
#endif /* if defined __APPLE__ && !defined __LP64__ */
#ifdef HAVE_GETRESUID
extern int my_getresuid (uid_t *ruid, uid_t *euid, uid_t *suid) __attribute__((visibility("hidden")));
#endif /* HAVE_GETRESUID */
#ifdef HAVE_GETRESGID
extern int my_getresgid (gid_t *rgid, gid_t *egid, gid_t *sgid) __attribute__((visibility("hidden")));
#endif /* HAVE_GETRESGID */
#ifdef HAVE_SETRESUID
extern int my_setresuid (uid_t ruid, uid_t euid, uid_t suid) __attribute__((visibility("hidden")));
#endif /* HAVE_SETRESUID */
#ifdef HAVE_SETRESGID
extern int my_setresgid (gid_t rgid, gid_t egid, gid_t sgid) __attribute__((visibility("hidden")));
#endif /* HAVE_SETRESGID */
#ifdef HAVE_SETFSUID
extern uid_t my_setfsuid (uid_t fsuid) __attribute__((visibility("hidden")));
#endif /* HAVE_SETFSUID */
#ifdef HAVE_SETFSGID
extern gid_t my_setfsgid (gid_t fsgid) __attribute__((visibility("hidden")));
#endif /* HAVE_SETFSGID */
extern int my_initgroups (const char *user, INITGROUPS_SECOND_ARG group) __attribute__((visibility("hidden")));
extern int my_setgroups (SETGROUPS_SIZE_TYPE size, const gid_t *list) __attribute__((visibility("hidden")));

#ifdef HAVE_FSTATAT
#ifdef HAVE_FCHMODAT
extern int my_fchmodat (int dir_fd, const char *path, mode_t mode, int flags) __attribute__((visibility("hidden")));
#endif /* HAVE_FCHMODAT */
#ifdef HAVE_FCHOWNAT
extern int my_fchownat (int dir_fd, const char *path, uid_t owner, gid_t group, int flags) __attribute__((visibility("hidden")));
#endif /* HAVE_FCHOWNAT */
#ifdef HAVE_MKDIRAT
extern int my_mkdirat (int dir_fd, const char *pathname, mode_t mode) __attribute__((visibility("hidden")));
#endif /* HAVE_MKDIRAT */
#ifdef HAVE_OPENAT
extern int my_openat (int dir_fd, const char *pathname, int flags) __attribute__((visibility("hidden")));
#endif /* HAVE_OPENAT */
#ifdef HAVE_RENAMEAT
extern int my_renameat (int olddir_fd, const char *oldpath, int newdir_fd, const char *newpath) __attribute__((visibility("hidden")));
#endif /* HAVE_RENAMEAT */
#ifdef HAVE_UNLINKAT
extern int my_unlinkat (int dir_fd, const char *pathname, int flags) __attribute__((visibility("hidden")));
#endif /* HAVE_UNLINKAT */
#endif /* HAVE_FSTATAT */

#ifdef HAVE_ACL_T
extern int my_acl_set_fd (int fd, acl_t acl) __attribute__((visibility("hidden")));
extern int my_acl_set_file (const char *path_p, acl_type_t type, acl_t acl) __attribute__((visibility("hidden")));
#endif /* HAVE_ACL_T */

#ifdef HAVE_FTS_READ
extern FTSENT * my_fts_read (FTS *ftsp) __attribute__((visibility("hidden")));
#ifdef __APPLE__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern FTSENT * my_fts_read$INODE64 (FTS *ftsp) __attribute__((visibility("hidden")));
#endif
#endif /* ifdef __APPLE__ */
#endif /* HAVE_FTS_READ */
#ifdef HAVE_FTS_CHILDREN
extern FTSENT * my_fts_children (FTS *ftsp, int options) __attribute__((visibility("hidden")));
#ifdef __APPLE__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern FTSENT * my_fts_children$INODE64 (FTS *ftsp, int options) __attribute__((visibility("hidden")));
#endif
#endif /* ifdef __APPLE__ */
#endif /* HAVE_FTS_CHILDREN */

#ifdef __sun
extern int my_sysinfo (int command, char *buf, long count) __attribute__((visibility("hidden")));
#endif
#endif
