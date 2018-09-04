/* Automatically generated file. Do not edit. Edit wrapawk_macosx/wrapfunc.inp. */
#ifndef WRAPTMPF_H
#define WRAPTMPF_H


extern int WRAP_LSTAT LSTAT_ARG(int ver, const char *file_name, struct stat *buf);
static __inline__ int NEXT_LSTAT_NOARG LSTAT_ARG(int ver, const char *file_name, struct stat *buf) __attribute__((always_inline));
static __inline__ int NEXT_LSTAT_NOARG LSTAT_ARG(int ver, const char *file_name, struct stat *buf) {
  return WRAP_LSTAT LSTAT_ARG(ver, file_name, buf);
}

extern int WRAP_STAT STAT_ARG(int ver, const char *file_name, struct stat *buf);
static __inline__ int NEXT_STAT_NOARG STAT_ARG(int ver, const char *file_name, struct stat *buf) __attribute__((always_inline));
static __inline__ int NEXT_STAT_NOARG STAT_ARG(int ver, const char *file_name, struct stat *buf) {
  return WRAP_STAT STAT_ARG(ver, file_name, buf);
}

extern int WRAP_FSTAT FSTAT_ARG(int ver, int fd, struct stat *buf);
static __inline__ int NEXT_FSTAT_NOARG FSTAT_ARG(int ver, int fd, struct stat *buf) __attribute__((always_inline));
static __inline__ int NEXT_FSTAT_NOARG FSTAT_ARG(int ver, int fd, struct stat *buf) {
  return WRAP_FSTAT FSTAT_ARG(ver, fd, buf);
}

#ifdef HAVE_FSTATAT
extern int WRAP_FSTATAT FSTATAT_ARG(int ver, int dir_fd, const char *path, struct stat *buf, int flags);
static __inline__ int NEXT_FSTATAT_NOARG FSTATAT_ARG(int ver, int dir_fd, const char *path, struct stat *buf, int flags) __attribute__((always_inline));
static __inline__ int NEXT_FSTATAT_NOARG FSTATAT_ARG(int ver, int dir_fd, const char *path, struct stat *buf, int flags) {
  return WRAP_FSTATAT FSTATAT_ARG(ver, dir_fd, path, buf, flags);
}

#endif /* HAVE_FSTATAT */

#ifdef STAT64_SUPPORT
extern int WRAP_LSTAT64 LSTAT64_ARG(int ver, const char *file_name, struct stat64 *buf);
static __inline__ int NEXT_LSTAT64_NOARG LSTAT64_ARG(int ver, const char *file_name, struct stat64 *buf) __attribute__((always_inline));
static __inline__ int NEXT_LSTAT64_NOARG LSTAT64_ARG(int ver, const char *file_name, struct stat64 *buf) {
  return WRAP_LSTAT64 LSTAT64_ARG(ver, file_name, buf);
}

extern int WRAP_STAT64 STAT64_ARG(int ver, const char *file_name, struct stat64 *buf);
static __inline__ int NEXT_STAT64_NOARG STAT64_ARG(int ver, const char *file_name, struct stat64 *buf) __attribute__((always_inline));
static __inline__ int NEXT_STAT64_NOARG STAT64_ARG(int ver, const char *file_name, struct stat64 *buf) {
  return WRAP_STAT64 STAT64_ARG(ver, file_name, buf);
}

extern int WRAP_FSTAT64 FSTAT64_ARG(int ver, int fd, struct stat64 *buf);
static __inline__ int NEXT_FSTAT64_NOARG FSTAT64_ARG(int ver, int fd, struct stat64 *buf) __attribute__((always_inline));
static __inline__ int NEXT_FSTAT64_NOARG FSTAT64_ARG(int ver, int fd, struct stat64 *buf) {
  return WRAP_FSTAT64 FSTAT64_ARG(ver, fd, buf);
}

#ifdef HAVE_FSTATAT
extern int WRAP_FSTATAT64 FSTATAT64_ARG(int ver, int dir_fd, const char *path, struct stat64 *buf, int flags);
static __inline__ int NEXT_FSTATAT64_NOARG FSTATAT64_ARG(int ver, int dir_fd, const char *path, struct stat64 *buf, int flags) __attribute__((always_inline));
static __inline__ int NEXT_FSTATAT64_NOARG FSTATAT64_ARG(int ver, int dir_fd, const char *path, struct stat64 *buf, int flags) {
  return WRAP_FSTATAT64 FSTATAT64_ARG(ver, dir_fd, path, buf, flags);
}

#endif /* HAVE_FSTATAT */
#endif /* STAT64_SUPPORT */

#ifdef __APPLE__
#ifdef __LP64__
extern int getattrlist (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned int options);
static __inline__ int next_getattrlist (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned int options) __attribute__((always_inline));
static __inline__ int next_getattrlist (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned int options) {
  return getattrlist (path, attrList, attrBuf, attrBufSize, options);
}

#ifdef HAVE_FGETATTRLIST
extern int fgetattrlist (int fd, void *attrList, void *attrBuf, size_t attrBufSize, unsigned int options);
static __inline__ int next_fgetattrlist (int fd, void *attrList, void *attrBuf, size_t attrBufSize, unsigned int options) __attribute__((always_inline));
static __inline__ int next_fgetattrlist (int fd, void *attrList, void *attrBuf, size_t attrBufSize, unsigned int options) {
  return fgetattrlist (fd, attrList, attrBuf, attrBufSize, options);
}

#endif
#else
extern int getattrlist (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options);
static __inline__ int next_getattrlist (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options) __attribute__((always_inline));
static __inline__ int next_getattrlist (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options) {
  return getattrlist (path, attrList, attrBuf, attrBufSize, options);
}

#ifdef HAVE_FGETATTRLIST
extern int fgetattrlist (int fd, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options);
static __inline__ int next_fgetattrlist (int fd, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options) __attribute__((always_inline));
static __inline__ int next_fgetattrlist (int fd, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options) {
  return fgetattrlist (fd, attrList, attrBuf, attrBufSize, options);
}

#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern int getattrlist$UNIX2003 (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options);
static __inline__ int next_getattrlist$UNIX2003 (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options) __attribute__((always_inline));
static __inline__ int next_getattrlist$UNIX2003 (const char *path, void *attrList, void *attrBuf, size_t attrBufSize, unsigned long options) {
  return getattrlist$UNIX2003 (path, attrList, attrBuf, attrBufSize, options);
}

#endif
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern int lstat$INODE64 (const char *file_name, struct stat *buf);
static __inline__ int next_lstat$INODE64 (const char *file_name, struct stat *buf) __attribute__((always_inline));
static __inline__ int next_lstat$INODE64 (const char *file_name, struct stat *buf) {
  return lstat$INODE64 (file_name, buf);
}

extern int stat$INODE64 (const char *file_name, struct stat *buf);
static __inline__ int next_stat$INODE64 (const char *file_name, struct stat *buf) __attribute__((always_inline));
static __inline__ int next_stat$INODE64 (const char *file_name, struct stat *buf) {
  return stat$INODE64 (file_name, buf);
}

extern int fstat$INODE64 (int fd, struct stat *buf);
static __inline__ int next_fstat$INODE64 (int fd, struct stat *buf) __attribute__((always_inline));
static __inline__ int next_fstat$INODE64 (int fd, struct stat *buf) {
  return fstat$INODE64 (fd, buf);
}

extern int posix_spawn (pid_t * __restrict pid, const char * __restrict path, const posix_spawn_file_actions_t *file_actions, const posix_spawnattr_t * __restrict attrp, char *const argv[ __restrict], char *const envp[ __restrict]);
static __inline__ int next_posix_spawn (pid_t * __restrict pid, const char * __restrict path, const posix_spawn_file_actions_t *file_actions, const posix_spawnattr_t * __restrict attrp, char *const argv[ __restrict], char *const envp[ __restrict]) __attribute__((always_inline));
static __inline__ int next_posix_spawn (pid_t * __restrict pid, const char * __restrict path, const posix_spawn_file_actions_t *file_actions, const posix_spawnattr_t * __restrict attrp, char *const argv[ __restrict], char *const envp[ __restrict]) {
  return posix_spawn (pid, path, file_actions, attrp, argv, envp);
}

extern int posix_spawnp (pid_t * __restrict pid, const char * __restrict path, const posix_spawn_file_actions_t *file_actions, const posix_spawnattr_t * __restrict attrp, char *const argv[ __restrict], char *const envp[ __restrict]);
static __inline__ int next_posix_spawnp (pid_t * __restrict pid, const char * __restrict path, const posix_spawn_file_actions_t *file_actions, const posix_spawnattr_t * __restrict attrp, char *const argv[ __restrict], char *const envp[ __restrict]) __attribute__((always_inline));
static __inline__ int next_posix_spawnp (pid_t * __restrict pid, const char * __restrict path, const posix_spawn_file_actions_t *file_actions, const posix_spawnattr_t * __restrict attrp, char *const argv[ __restrict], char *const envp[ __restrict]) {
  return posix_spawnp (pid, path, file_actions, attrp, argv, envp);
}

#endif
extern int execve (const char *path, char *const argv[], char *const envp[]);
static __inline__ int next_execve (const char *path, char *const argv[], char *const envp[]) __attribute__((always_inline));
static __inline__ int next_execve (const char *path, char *const argv[], char *const envp[]) {
  return execve (path, argv, envp);
}

extern int execl (const char *path, const char *arg0, ...);

extern int execle (const char *path, const char *arg0, ...);

extern int execlp (const char *path, const char *arg0, ...);

extern int execv (const char *path, char *const argv[]);
static __inline__ int next_execv (const char *path, char *const argv[]) __attribute__((always_inline));
static __inline__ int next_execv (const char *path, char *const argv[]) {
  return execv (path, argv);
}

extern int execvp (const char *file, char *const argv[]);
static __inline__ int next_execvp (const char *file, char *const argv[]) __attribute__((always_inline));
static __inline__ int next_execvp (const char *file, char *const argv[]) {
  return execvp (file, argv);
}

extern int execvP (const char *file, const char *search_path, char *const argv[]);
static __inline__ int next_execvP (const char *file, const char *search_path, char *const argv[]) __attribute__((always_inline));
static __inline__ int next_execvP (const char *file, const char *search_path, char *const argv[]) {
  return execvP (file, search_path, argv);
}

#endif /* ifdef __APPLE__ */

extern int WRAP_MKNOD MKNOD_ARG(int ver, const char *pathname, mode_t mode, dev_t XMKNOD_FRTH_ARG dev);
static __inline__ int NEXT_MKNOD_NOARG MKNOD_ARG(int ver, const char *pathname, mode_t mode, dev_t XMKNOD_FRTH_ARG dev) __attribute__((always_inline));
static __inline__ int NEXT_MKNOD_NOARG MKNOD_ARG(int ver, const char *pathname, mode_t mode, dev_t XMKNOD_FRTH_ARG dev) {
  return WRAP_MKNOD MKNOD_ARG(ver, pathname, mode, dev);
}


#ifdef HAVE_FSTATAT
#ifdef HAVE_MKNODAT
extern int WRAP_MKNODAT MKNODAT_ARG(int ver, int dir_fd, const char *pathname, mode_t mode, dev_t dev);
static __inline__ int NEXT_MKNODAT_NOARG MKNODAT_ARG(int ver, int dir_fd, const char *pathname, mode_t mode, dev_t dev) __attribute__((always_inline));
static __inline__ int NEXT_MKNODAT_NOARG MKNODAT_ARG(int ver, int dir_fd, const char *pathname, mode_t mode, dev_t dev) {
  return WRAP_MKNODAT MKNODAT_ARG(ver, dir_fd, pathname, mode, dev);
}

#endif /* HAVE_MKNODAT */
#endif /* HAVE_FSTATAT */


extern int chown (const char *path, uid_t owner, gid_t group);
static __inline__ int next_chown (const char *path, uid_t owner, gid_t group) __attribute__((always_inline));
static __inline__ int next_chown (const char *path, uid_t owner, gid_t group) {
  return chown (path, owner, group);
}

extern int lchown (const char *path, uid_t owner, gid_t group);
static __inline__ int next_lchown (const char *path, uid_t owner, gid_t group) __attribute__((always_inline));
static __inline__ int next_lchown (const char *path, uid_t owner, gid_t group) {
  return lchown (path, owner, group);
}

extern int fchown (int fd, uid_t owner, gid_t group);
static __inline__ int next_fchown (int fd, uid_t owner, gid_t group) __attribute__((always_inline));
static __inline__ int next_fchown (int fd, uid_t owner, gid_t group) {
  return fchown (fd, owner, group);
}

extern int chmod (const char *path, mode_t mode);
static __inline__ int next_chmod (const char *path, mode_t mode) __attribute__((always_inline));
static __inline__ int next_chmod (const char *path, mode_t mode) {
  return chmod (path, mode);
}

extern int fchmod (int fd, mode_t mode);
static __inline__ int next_fchmod (int fd, mode_t mode) __attribute__((always_inline));
static __inline__ int next_fchmod (int fd, mode_t mode) {
  return fchmod (fd, mode);
}

#if defined HAVE_LCHMOD
extern int lchmod (const char *path, mode_t mode);
static __inline__ int next_lchmod (const char *path, mode_t mode) __attribute__((always_inline));
static __inline__ int next_lchmod (const char *path, mode_t mode) {
  return lchmod (path, mode);
}

#endif
#if defined __APPLE__ && !defined __LP64__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern int lchown$UNIX2003 (const char *path, uid_t owner, gid_t group);
static __inline__ int next_lchown$UNIX2003 (const char *path, uid_t owner, gid_t group) __attribute__((always_inline));
static __inline__ int next_lchown$UNIX2003 (const char *path, uid_t owner, gid_t group) {
  return lchown$UNIX2003 (path, owner, group);
}

extern int chmod$UNIX2003 (const char *path, mode_t mode);
static __inline__ int next_chmod$UNIX2003 (const char *path, mode_t mode) __attribute__((always_inline));
static __inline__ int next_chmod$UNIX2003 (const char *path, mode_t mode) {
  return chmod$UNIX2003 (path, mode);
}

extern int fchmod$UNIX2003 (int fd, mode_t mode);
static __inline__ int next_fchmod$UNIX2003 (int fd, mode_t mode) __attribute__((always_inline));
static __inline__ int next_fchmod$UNIX2003 (int fd, mode_t mode) {
  return fchmod$UNIX2003 (fd, mode);
}

#endif
#endif /* if defined __APPLE__ && !defined __LP64__ */
extern int mkdir (const char *path, mode_t mode);
static __inline__ int next_mkdir (const char *path, mode_t mode) __attribute__((always_inline));
static __inline__ int next_mkdir (const char *path, mode_t mode) {
  return mkdir (path, mode);
}

extern int unlink (const char *pathname);
static __inline__ int next_unlink (const char *pathname) __attribute__((always_inline));
static __inline__ int next_unlink (const char *pathname) {
  return unlink (pathname);
}

extern int rmdir (const char *pathname);
static __inline__ int next_rmdir (const char *pathname) __attribute__((always_inline));
static __inline__ int next_rmdir (const char *pathname) {
  return rmdir (pathname);
}

extern int remove (const char *pathname);
static __inline__ int next_remove (const char *pathname) __attribute__((always_inline));
static __inline__ int next_remove (const char *pathname) {
  return remove (pathname);
}

extern int rename (const char *oldpath, const char *newpath);
static __inline__ int next_rename (const char *oldpath, const char *newpath) __attribute__((always_inline));
static __inline__ int next_rename (const char *oldpath, const char *newpath) {
  return rename (oldpath, newpath);
}


#ifdef FAKEROOT_FAKENET
extern pid_t fork (void);
static __inline__ pid_t next_fork (void) __attribute__((always_inline));
static __inline__ pid_t next_fork (void) {
  return fork ();
}

extern pid_t vfork (void);
static __inline__ pid_t next_vfork (void) __attribute__((always_inline));
static __inline__ pid_t next_vfork (void) {
  return vfork ();
}

extern int close (int fd);
static __inline__ int next_close (int fd) __attribute__((always_inline));
static __inline__ int next_close (int fd) {
  return close (fd);
}

extern int dup2 (int oldfd, int newfd);
static __inline__ int next_dup2 (int oldfd, int newfd) __attribute__((always_inline));
static __inline__ int next_dup2 (int oldfd, int newfd) {
  return dup2 (oldfd, newfd);
}

#endif /* FAKEROOT_FAKENET */


extern uid_t getuid (void);
static __inline__ uid_t next_getuid (void) __attribute__((always_inline));
static __inline__ uid_t next_getuid (void) {
  return getuid ();
}

extern gid_t getgid (void);
static __inline__ gid_t next_getgid (void) __attribute__((always_inline));
static __inline__ gid_t next_getgid (void) {
  return getgid ();
}

extern uid_t geteuid (void);
static __inline__ uid_t next_geteuid (void) __attribute__((always_inline));
static __inline__ uid_t next_geteuid (void) {
  return geteuid ();
}

extern gid_t getegid (void);
static __inline__ gid_t next_getegid (void) __attribute__((always_inline));
static __inline__ gid_t next_getegid (void) {
  return getegid ();
}

extern int setuid (uid_t id);
static __inline__ int next_setuid (uid_t id) __attribute__((always_inline));
static __inline__ int next_setuid (uid_t id) {
  return setuid (id);
}

extern int setgid (gid_t id);
static __inline__ int next_setgid (gid_t id) __attribute__((always_inline));
static __inline__ int next_setgid (gid_t id) {
  return setgid (id);
}

extern int seteuid (uid_t id);
static __inline__ int next_seteuid (uid_t id) __attribute__((always_inline));
static __inline__ int next_seteuid (uid_t id) {
  return seteuid (id);
}

extern int setegid (gid_t id);
static __inline__ int next_setegid (gid_t id) __attribute__((always_inline));
static __inline__ int next_setegid (gid_t id) {
  return setegid (id);
}

extern int setreuid (SETREUID_ARG ruid, SETREUID_ARG euid);
static __inline__ int next_setreuid (SETREUID_ARG ruid, SETREUID_ARG euid) __attribute__((always_inline));
static __inline__ int next_setreuid (SETREUID_ARG ruid, SETREUID_ARG euid) {
  return setreuid (ruid, euid);
}

extern int setregid (SETREGID_ARG rgid, SETREGID_ARG egid);
static __inline__ int next_setregid (SETREGID_ARG rgid, SETREGID_ARG egid) __attribute__((always_inline));
static __inline__ int next_setregid (SETREGID_ARG rgid, SETREGID_ARG egid) {
  return setregid (rgid, egid);
}

#if defined __APPLE__ && !defined __LP64__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern int setreuid$UNIX2003 (SETREUID_ARG ruid, SETREUID_ARG euid);
static __inline__ int next_setreuid$UNIX2003 (SETREUID_ARG ruid, SETREUID_ARG euid) __attribute__((always_inline));
static __inline__ int next_setreuid$UNIX2003 (SETREUID_ARG ruid, SETREUID_ARG euid) {
  return setreuid$UNIX2003 (ruid, euid);
}

extern int setregid$UNIX2003 (SETREGID_ARG rgid, SETREGID_ARG egid);
static __inline__ int next_setregid$UNIX2003 (SETREGID_ARG rgid, SETREGID_ARG egid) __attribute__((always_inline));
static __inline__ int next_setregid$UNIX2003 (SETREGID_ARG rgid, SETREGID_ARG egid) {
  return setregid$UNIX2003 (rgid, egid);
}

#endif
#endif /* if defined __APPLE__ && !defined __LP64__ */
#ifdef HAVE_GETRESUID
extern int getresuid (uid_t *ruid, uid_t *euid, uid_t *suid);
static __inline__ int next_getresuid (uid_t *ruid, uid_t *euid, uid_t *suid) __attribute__((always_inline));
static __inline__ int next_getresuid (uid_t *ruid, uid_t *euid, uid_t *suid) {
  return getresuid (ruid, euid, suid);
}

#endif /* HAVE_GETRESUID */
#ifdef HAVE_GETRESGID
extern int getresgid (gid_t *rgid, gid_t *egid, gid_t *sgid);
static __inline__ int next_getresgid (gid_t *rgid, gid_t *egid, gid_t *sgid) __attribute__((always_inline));
static __inline__ int next_getresgid (gid_t *rgid, gid_t *egid, gid_t *sgid) {
  return getresgid (rgid, egid, sgid);
}

#endif /* HAVE_GETRESGID */
#ifdef HAVE_SETRESUID
extern int setresuid (uid_t ruid, uid_t euid, uid_t suid);
static __inline__ int next_setresuid (uid_t ruid, uid_t euid, uid_t suid) __attribute__((always_inline));
static __inline__ int next_setresuid (uid_t ruid, uid_t euid, uid_t suid) {
  return setresuid (ruid, euid, suid);
}

#endif /* HAVE_SETRESUID */
#ifdef HAVE_SETRESGID
extern int setresgid (gid_t rgid, gid_t egid, gid_t sgid);
static __inline__ int next_setresgid (gid_t rgid, gid_t egid, gid_t sgid) __attribute__((always_inline));
static __inline__ int next_setresgid (gid_t rgid, gid_t egid, gid_t sgid) {
  return setresgid (rgid, egid, sgid);
}

#endif /* HAVE_SETRESGID */
#ifdef HAVE_SETFSUID
extern uid_t setfsuid (uid_t fsuid);
static __inline__ uid_t next_setfsuid (uid_t fsuid) __attribute__((always_inline));
static __inline__ uid_t next_setfsuid (uid_t fsuid) {
  return setfsuid (fsuid);
}

#endif /* HAVE_SETFSUID */
#ifdef HAVE_SETFSGID
extern gid_t setfsgid (gid_t fsgid);
static __inline__ gid_t next_setfsgid (gid_t fsgid) __attribute__((always_inline));
static __inline__ gid_t next_setfsgid (gid_t fsgid) {
  return setfsgid (fsgid);
}

#endif /* HAVE_SETFSGID */
extern int initgroups (const char *user, INITGROUPS_SECOND_ARG group);
static __inline__ int next_initgroups (const char *user, INITGROUPS_SECOND_ARG group) __attribute__((always_inline));
static __inline__ int next_initgroups (const char *user, INITGROUPS_SECOND_ARG group) {
  return initgroups (user, group);
}

extern int setgroups (SETGROUPS_SIZE_TYPE size, const gid_t *list);
static __inline__ int next_setgroups (SETGROUPS_SIZE_TYPE size, const gid_t *list) __attribute__((always_inline));
static __inline__ int next_setgroups (SETGROUPS_SIZE_TYPE size, const gid_t *list) {
  return setgroups (size, list);
}


#ifdef HAVE_FSTATAT
#ifdef HAVE_FCHMODAT
extern int fchmodat (int dir_fd, const char *path, mode_t mode, int flags);
static __inline__ int next_fchmodat (int dir_fd, const char *path, mode_t mode, int flags) __attribute__((always_inline));
static __inline__ int next_fchmodat (int dir_fd, const char *path, mode_t mode, int flags) {
  return fchmodat (dir_fd, path, mode, flags);
}

#endif /* HAVE_FCHMODAT */
#ifdef HAVE_FCHOWNAT
extern int fchownat (int dir_fd, const char *path, uid_t owner, gid_t group, int flags);
static __inline__ int next_fchownat (int dir_fd, const char *path, uid_t owner, gid_t group, int flags) __attribute__((always_inline));
static __inline__ int next_fchownat (int dir_fd, const char *path, uid_t owner, gid_t group, int flags) {
  return fchownat (dir_fd, path, owner, group, flags);
}

#endif /* HAVE_FCHOWNAT */
#ifdef HAVE_MKDIRAT
extern int mkdirat (int dir_fd, const char *pathname, mode_t mode);
static __inline__ int next_mkdirat (int dir_fd, const char *pathname, mode_t mode) __attribute__((always_inline));
static __inline__ int next_mkdirat (int dir_fd, const char *pathname, mode_t mode) {
  return mkdirat (dir_fd, pathname, mode);
}

#endif /* HAVE_MKDIRAT */
#ifdef HAVE_OPENAT
extern int openat (int dir_fd, const char *pathname, int flags);
static __inline__ int next_openat (int dir_fd, const char *pathname, int flags) __attribute__((always_inline));
static __inline__ int next_openat (int dir_fd, const char *pathname, int flags) {
  return openat (dir_fd, pathname, flags);
}

#endif /* HAVE_OPENAT */
#ifdef HAVE_RENAMEAT
extern int renameat (int olddir_fd, const char *oldpath, int newdir_fd, const char *newpath);
static __inline__ int next_renameat (int olddir_fd, const char *oldpath, int newdir_fd, const char *newpath) __attribute__((always_inline));
static __inline__ int next_renameat (int olddir_fd, const char *oldpath, int newdir_fd, const char *newpath) {
  return renameat (olddir_fd, oldpath, newdir_fd, newpath);
}

#endif /* HAVE_RENAMEAT */
#ifdef HAVE_UNLINKAT
extern int unlinkat (int dir_fd, const char *pathname, int flags);
static __inline__ int next_unlinkat (int dir_fd, const char *pathname, int flags) __attribute__((always_inline));
static __inline__ int next_unlinkat (int dir_fd, const char *pathname, int flags) {
  return unlinkat (dir_fd, pathname, flags);
}

#endif /* HAVE_UNLINKAT */
#endif /* HAVE_FSTATAT */

#ifdef HAVE_ACL_T
extern int acl_set_fd (int fd, acl_t acl);
static __inline__ int next_acl_set_fd (int fd, acl_t acl) __attribute__((always_inline));
static __inline__ int next_acl_set_fd (int fd, acl_t acl) {
  return acl_set_fd (fd, acl);
}

extern int acl_set_file (const char *path_p, acl_type_t type, acl_t acl);
static __inline__ int next_acl_set_file (const char *path_p, acl_type_t type, acl_t acl) __attribute__((always_inline));
static __inline__ int next_acl_set_file (const char *path_p, acl_type_t type, acl_t acl) {
  return acl_set_file (path_p, type, acl);
}

#endif /* HAVE_ACL_T */

#ifdef HAVE_FTS_READ
extern FTSENT * fts_read (FTS *ftsp);
static __inline__ FTSENT * next_fts_read (FTS *ftsp) __attribute__((always_inline));
static __inline__ FTSENT * next_fts_read (FTS *ftsp) {
  return fts_read (ftsp);
}

#ifdef __APPLE__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern FTSENT * fts_read$INODE64 (FTS *ftsp);
static __inline__ FTSENT * next_fts_read$INODE64 (FTS *ftsp) __attribute__((always_inline));
static __inline__ FTSENT * next_fts_read$INODE64 (FTS *ftsp) {
  return fts_read$INODE64 (ftsp);
}

#endif
#endif /* ifdef __APPLE__ */
#endif /* HAVE_FTS_READ */
#ifdef HAVE_FTS_CHILDREN
extern FTSENT * fts_children (FTS *ftsp, int options);
static __inline__ FTSENT * next_fts_children (FTS *ftsp, int options) __attribute__((always_inline));
static __inline__ FTSENT * next_fts_children (FTS *ftsp, int options) {
  return fts_children (ftsp, options);
}

#ifdef __APPLE__
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
extern FTSENT * fts_children$INODE64 (FTS *ftsp, int options);
static __inline__ FTSENT * next_fts_children$INODE64 (FTS *ftsp, int options) __attribute__((always_inline));
static __inline__ FTSENT * next_fts_children$INODE64 (FTS *ftsp, int options) {
  return fts_children$INODE64 (ftsp, options);
}

#endif
#endif /* ifdef __APPLE__ */
#endif /* HAVE_FTS_CHILDREN */

#ifdef __sun
extern int sysinfo (int command, char *buf, long count);
static __inline__ int next_sysinfo (int command, char *buf, long count) __attribute__((always_inline));
static __inline__ int next_sysinfo (int command, char *buf, long count) {
  return sysinfo (command, buf, count);
}

#endif
#endif
