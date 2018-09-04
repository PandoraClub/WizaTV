/*
 * dpkg - main program for package management
 * filesdb.h - management of database of files installed on system
 *
 * Copyright (C) 1995 Ian Jackson <ian@chiark.greenend.org.uk>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with dpkg; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef FILESDB_H
#define FILESDB_H

/*
 * Data structure here is as follows:
 * 
 * For each package we have a `struct fileinlist *', the head of
 * a list of files in that package.  They are in `forwards' order.
 * Each entry has a pointer to the `struct filenamenode'.
 *
 * The struct filenamenodes are in a hash table, indexed by name.
 * (This hash table is not visible to callers.)
 *
 * Each filenamenode has a (possibly empty) list of `struct
 * filepackage', giving a list of the packages listing that
 * filename.
 *
 * When we read files contained info about a particular package
 * we set the `files' member of the clientdata struct to the
 * appropriate thing.  When not yet set the files pointer is
 * made to point to `fileslist_uninited' (this is available only
 * internally, withing filesdb.c - the published interface is
 * ensure_*_available).
 */

struct pkginfo;

/* flags to findnamenode() */

enum fnnflags {
    fnn_nocopy=                 000001, /* do not need to copy filename */
    fnn_nonew =                 000002, /* findnamenode may return NULL */
};

struct filenamenode {
  struct filenamenode *next;
  const char *name;
  struct filepackages *packages;
  struct diversion *divert;
  struct filestatoverride *statoverride;
  /* Fields from here on are used by archives.c &c, and cleared by
   * filesdbinit.
   */
  enum {
    fnnf_new_conff=           000001, /* in the newconffiles list */
    fnnf_new_inarchive=       000002, /* in the new filesystem archive */
    fnnf_old_conff=           000004, /* in the old package's conffiles list */
    fnnf_obs_conff=           000100, /* obsolete conffile */
    fnnf_elide_other_lists=   000010, /* must remove from other packages' lists */
    fnnf_no_atomic_overwrite= 000020, /* >=1 instance is a dir, cannot rename over */
    fnnf_placed_on_disk=      000040, /* new file has been placed on the disk */
  } flags; /* Set to zero when a new node is created. */
  const char *oldhash; /* valid iff this namenode is in the newconffiles list */
  struct stat *filestat;
  struct trigfileint *trig_interested;
};
 
struct fileinlist {
  struct fileinlist *next;
  struct filenamenode *namenode;
};

struct filestatoverride {
  /* We allow the administrator to override the owner, group and mode of
   * a file. If such an override is present we use that instead of the
   * stat information stored in the archive.
   *
   * This functionality used to be in the suidmanager package.
   */
  uid_t uid;
  gid_t gid;
  mode_t mode;
};

struct diversion {
  /* When we deal with an `overridden' file, every package except
   * the overriding one is considered to contain the other file
   * instead.  Both files have entries in the filesdb database, and
   * they refer to each other via these diversion structures.
   *
   * The contested filename's filenamenode has an diversion entry
   * with useinstead set to point to the redirected filename's
   * filenamenode; the redirected filenamenode has camefrom set to the
   * contested filenamenode.  Both sides' diversion entries will
   * have pkg set to the package (if any) which is allowed to use the
   * contended filename.
   *
   * Packages that contain either version of the file will all
   * refer to the contested filenamenode in their per-file package lists
   * (both in core and on disk).  References are redirected to the other
   * filenamenode's filename where appropriate.
   */
  struct filenamenode *useinstead;
  struct filenamenode *camefrom;
  struct pkginfo *pkg;
  struct diversion *next;
  /* The `contested' halves are in this list for easy cleanup. */
};

#define PERFILEPACKAGESLUMP 10

struct filepackages {
  struct filepackages *more;
  struct pkginfo *pkgs[PERFILEPACKAGESLUMP];
  /* pkgs is a null-pointer-terminated list; anything after the first null
   * is garbage
   */
};

struct fileiterator;
struct fileiterator *iterfilestart(void);
struct filenamenode *iterfilenext(struct fileiterator *i);
void iterfileend(struct fileiterator *i);

void ensure_package_clientdata(struct pkginfo *pkg);

void ensure_diversions(void);
void ensure_statoverrides(void);

void ensure_packagefiles_available(struct pkginfo *pkg);
void ensure_allinstfiles_available(void);
void ensure_allinstfiles_available_quiet(void);
void note_must_reread_files_inpackage(struct pkginfo *pkg);
struct filenamenode *findnamenode(const char *filename, enum fnnflags flags);
void write_filelist_except(struct pkginfo *pkg, struct fileinlist *list, int leaveout);

struct reversefilelistiter { struct fileinlist *todo; };

void reversefilelist_init(struct reversefilelistiter *iterptr, struct fileinlist *files);
struct filenamenode *reversefilelist_next(struct reversefilelistiter *iterptr);
void reversefilelist_abort(struct reversefilelistiter *iterptr);

#endif /* FILESDB_H */
