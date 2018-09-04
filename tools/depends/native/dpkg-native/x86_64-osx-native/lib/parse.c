/*
 * libdpkg - Debian packaging suite library routines
 * parse.c - database file parsing, main package/field loop
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
#include <config.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include <dpkg.h>
#include <dpkg-db.h>
#include "parsedump.h"

#ifdef HAVE_MMAP
#include <sys/mman.h>
#endif

const struct fieldinfo fieldinfos[]= {
  /* NB: capitalisation of these strings is important. */
  { "Package",          f_name,            w_name                                     },
  { "Essential",        f_boolean,         w_booleandefno,   PKGIFPOFF(essential)     },
  { "Status",           f_status,          w_status                                   },
  { "Priority",         f_priority,        w_priority                                 },
  { "Section",          f_section,         w_section                                  },
  { "Installed-Size",   f_charfield,       w_charfield,      PKGIFPOFF(installedsize) },
  { "Origin",           f_charfield,       w_charfield,      PKGIFPOFF(origin)        },
  { "Maintainer",       f_charfield,       w_charfield,      PKGIFPOFF(maintainer)    },
  { "Bugs",             f_charfield,       w_charfield,      PKGIFPOFF(bugs)          },
  { "Architecture",     f_charfield,       w_charfield,      PKGIFPOFF(architecture)  },
  { "Source",           f_charfield,       w_charfield,      PKGIFPOFF(source)        },
  { "Version",          f_version,         w_version,        PKGIFPOFF(version)       },
  { "Revision",         f_revision,        w_null                                     },
  { "Config-Version",   f_configversion,   w_configversion                            },
  { "Replaces",         f_dependency,      w_dependency,     dep_replaces             },
  { "Provides",         f_dependency,      w_dependency,     dep_provides             },
  { "Depends",          f_dependency,      w_dependency,     dep_depends              },
  { "Pre-Depends",      f_dependency,      w_dependency,     dep_predepends           },
  { "Recommends",       f_dependency,      w_dependency,     dep_recommends           },
  { "Suggests",         f_dependency,      w_dependency,     dep_suggests             },
  { "Breaks",           f_dependency,      w_dependency,     dep_breaks               },
  { "Conflicts",        f_dependency,      w_dependency,     dep_conflicts            },
  { "Enhances",         f_dependency,      w_dependency,     dep_enhances             },
  { "Conffiles",        f_conffiles,       w_conffiles                                },
  { "Filename",         f_filecharf,       w_filecharf,      FILEFOFF(name)           },
  { "Size",             f_filecharf,       w_filecharf,      FILEFOFF(size)           },
  { "MD5sum",           f_filecharf,       w_filecharf,      FILEFOFF(md5sum)         },
  { "MSDOS-Filename",   f_filecharf,       w_filecharf,      FILEFOFF(msdosname)      },
  { "Description",      f_charfield,       w_charfield,      PKGIFPOFF(description)   },
  { "Triggers-Pending", f_trigpend,        w_trigpend                                 },
  { "Triggers-Awaited", f_trigaw,          w_trigaw                                   },
  /* Note that aliases are added to the nicknames table in parsehelp.c. */
  {  NULL   /* sentinel - tells code that list is ended */                               }
};
#define NFIELDS (sizeof(fieldinfos)/sizeof(struct fieldinfo))
const int nfields= NFIELDS;

int parsedb(const char *filename, enum parsedbflags flags,
            struct pkginfo **donep, FILE *warnto, int *warncount) {
  /* warnto, warncount and donep may be null.
   * If donep is not null only one package's information is expected.
   */
  
  static int fd;
  struct pkginfo newpig, *pigp;
  struct pkginfoperfile *newpifp, *pifp;
  struct arbitraryfield *arp, **larpp;
  struct trigaw *ta;
  int lno;
  int pdone;
  int fieldencountered[NFIELDS];
  const struct fieldinfo *fip;
  const struct nickname *nick;
  char *data, *dataptr, *endptr;
  const char *fieldstart, *valuestart;
  char *value= NULL;
  int fieldlen= 0, valuelen= 0;
  int *ip, c;
  struct stat stat;

  if (warncount) *warncount= 0;
  newpifp= (flags & pdb_recordavailable) ? &newpig.available : &newpig.installed;
  fd= open(filename, O_RDONLY);
  if (fd == -1) ohshite(_("failed to open package info file `%.255s' for reading"),filename);

  push_cleanup(cu_closefd, ~ehflag_normaltidy, NULL, 0, 1, &fd);

  if (fstat(fd, &stat) == -1)
    ohshite(_("can't stat package info file `%.255s'"),filename);

  if (stat.st_size > 0) {
#ifdef HAVE_MMAP
    if ((dataptr= (char *)mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
      ohshite(_("can't mmap package info file `%.255s'"),filename);
#else
    dataptr = m_malloc(stat.st_size);

    fd_buf_copy(fd, dataptr, stat.st_size, _("copy info file `%.255s'"),filename);
#endif
    data= dataptr;
    endptr= dataptr + stat.st_size;
  } else {
    data= dataptr= endptr= NULL;
  }

  lno= 1;
  pdone= 0;
#define EOF_mmap(dataptr, endptr)	(dataptr >= endptr)
#define getc_mmap(dataptr)		*dataptr++;
#define ungetc_mmap(c, dataptr, data)	dataptr--;

  for (;;) { /* loop per package */
    memset(fieldencountered, 0, sizeof(fieldencountered));
    blankpackage(&newpig);
    blankpackageperfile(newpifp);
/* Skip adjacent new lines */
    while(!EOF_mmap(dataptr, endptr)) {
      c= getc_mmap(dataptr); if (c!='\n' && c!=MSDOS_EOF_CHAR ) break;
      lno++;
    }
    if (EOF_mmap(dataptr, endptr)) break;
    for (;;) { /* loop per field */
      fieldstart= dataptr - 1;
      while (!EOF_mmap(dataptr, endptr) && !isspace(c) && c!=':' && c!=MSDOS_EOF_CHAR)
        c= getc_mmap(dataptr);
      fieldlen= dataptr - fieldstart - 1;
      while (!EOF_mmap(dataptr, endptr) && c != '\n' && isspace(c)) c= getc_mmap(dataptr);
      if (EOF_mmap(dataptr, endptr))
        parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                 _("EOF after field name `%.*s'"),fieldlen,fieldstart);
      if (c == '\n')
        parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                 _("newline in field name `%.*s'"),fieldlen,fieldstart);
      if (c == MSDOS_EOF_CHAR)
        parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                 _("MSDOS EOF (^Z) in field name `%.*s'"),fieldlen,fieldstart);
      if (c != ':')
        parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                 _("field name `%.*s' must be followed by colon"),fieldlen,fieldstart);
/* Skip space after ':' but before value and eol */
      while(!EOF_mmap(dataptr, endptr)) {
        c= getc_mmap(dataptr);
        if (c == '\n' || !isspace(c)) break;
      }
      if (EOF_mmap(dataptr, endptr))
        parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                 _("EOF before value of field `%.*s' (missing final newline)"),
                 fieldlen,fieldstart);
      if (c == MSDOS_EOF_CHAR)
        parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                 _("MSDOS EOF char in value of field `%.*s' (missing newline?)"),
                 fieldlen,fieldstart);
      valuestart= dataptr - 1;
      for (;;) {
        if (c == '\n' || c == MSDOS_EOF_CHAR) {
          lno++;
	  if (EOF_mmap(dataptr, endptr)) break;
          c= getc_mmap(dataptr);
/* Found double eol, or start of new field */
          if (EOF_mmap(dataptr, endptr) || c == '\n' || !isspace(c)) break;
          ungetc_mmap(c,dataptr, data);
          c= '\n';
        } else if (EOF_mmap(dataptr, endptr)) {
          parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                   _("EOF during value of field `%.*s' (missing final newline)"),
                   fieldlen,fieldstart);
        }
        c= getc_mmap(dataptr);
      }
      valuelen= dataptr - valuestart - 1;
/* trim ending space on value */
      while (valuelen && isspace(*(valuestart+valuelen-1)))
 valuelen--;
      for (nick= nicknames; nick->nick && (strncasecmp(nick->nick,fieldstart, fieldlen) || nick->nick[fieldlen] != 0); nick++);
      if (nick->nick) {
	fieldstart= nick->canon;
	fieldlen= strlen(fieldstart);
      }
      for (fip= fieldinfos, ip= fieldencountered;
           fip->name && strncasecmp(fieldstart,fip->name, fieldlen);
           fip++, ip++);
      if (fip->name) {
	value= realloc(value,valuelen+1);
	memcpy(value,valuestart,valuelen);
	*(value+valuelen)= 0;
        if (*ip++)
          parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                   _("duplicate value for `%s' field"), fip->name);
        fip->rcall(&newpig,newpifp,flags,filename,lno-1,warnto,warncount,value,fip);
      } else {
        if (fieldlen<2)
          parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                   _("user-defined field name `%.*s' too short"), fieldlen,fieldstart);
        larpp= &newpifp->arbs;
        while ((arp= *larpp) != NULL) {
          if (!strncasecmp(arp->name,fieldstart,fieldlen))
            parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                     _("duplicate value for user-defined field `%.*s'"), fieldlen,fieldstart);
          larpp= &arp->next;
        }
        arp= nfmalloc(sizeof(struct arbitraryfield));
        arp->name= nfstrnsave(fieldstart,fieldlen);
        arp->value= nfstrnsave(valuestart,valuelen);
        arp->next= NULL;
        *larpp= arp;
      }
      if (EOF_mmap(dataptr, endptr) || c == '\n' || c == MSDOS_EOF_CHAR) break;
    } /* loop per field */
    if (pdone && donep)
      parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
               _("several package info entries found, only one allowed"));
    parsemustfield(NULL,filename,lno, warnto,warncount,&newpig,0,
                   &newpig.name, "package name");
    if ((flags & pdb_recordavailable) || newpig.status != stat_notinstalled) {
      parsemustfield(NULL,filename,lno, warnto,warncount,&newpig,1,
                     (const char **)&newpifp->description, "description");
      parsemustfield(NULL,filename,lno, warnto,warncount,&newpig,1,
                     (const char **)&newpifp->maintainer, "maintainer");
      if (newpig.status != stat_halfinstalled)
        parsemustfield(NULL,filename,lno, warnto,warncount,&newpig,0,
                       &newpifp->version.version, "version");
    }
    if (flags & pdb_recordavailable)
      parsemustfield(NULL,filename,lno, warnto,warncount,&newpig,1,
                     (const char **)&newpifp->architecture, "architecture");

    /* Check the Config-Version information:
     * If there is a Config-Version it is definitely to be used, but
     * there shouldn't be one if the package is `installed' (in which case
     * the Version and/or Revision will be copied) or if the package is
     * `not-installed' (in which case there is no Config-Version).
     */
    if (!(flags & pdb_recordavailable)) {
      if (newpig.configversion.version) {
        if (newpig.status == stat_installed || newpig.status == stat_notinstalled)
          parseerr(NULL,filename,lno, warnto,warncount,&newpig,0,
                   _("Configured-Version for package with inappropriate Status"));
      } else {
        if (newpig.status == stat_installed) newpig.configversion= newpifp->version;
      }
    }

    if (newpig.trigaw.head &&
        (newpig.status <= stat_configfiles ||
         newpig.status >= stat_triggerspending))
      parseerr(NULL, filename, lno, warnto, warncount, &newpig, 0,
               _("package has status %s but triggers are awaited"),
               statusinfos[newpig.status].name);
    else if (newpig.status == stat_triggersawaited && !newpig.trigaw.head)
      parseerr(NULL, filename, lno, warnto, warncount, &newpig, 0,
               _("package has status triggers-awaited but no triggers awaited"));

    if (!(newpig.status == stat_triggerspending ||
          newpig.status == stat_triggersawaited) &&
        newpig.trigpend_head)
      parseerr(NULL, filename, lno, warnto, warncount, &newpig, 0,
               _("package has status %s but triggers are pending"),
               statusinfos[newpig.status].name);
    else if (newpig.status == stat_triggerspending && !newpig.trigpend_head)
      parseerr(NULL, filename, lno, warnto, warncount, &newpig, 0,
               _("package has status triggers-pending but no triggers pending"));

    /* There was a bug that could make a not-installed package have
     * conffiles, so we check for them here and remove them (rather than
     * calling it an error, which will do at some point -- fixme).
     */
    if (!(flags & pdb_recordavailable) &&
        newpig.status == stat_notinstalled &&
        newpifp->conffiles) {
      parseerr(NULL,filename,lno, warnto,warncount,&newpig,1,
               _("Package which in state not-installed has conffiles, forgetting them"));
      newpifp->conffiles= NULL;
    }

    pigp= findpackage(newpig.name);
    pifp= (flags & pdb_recordavailable) ? &pigp->available : &pigp->installed;
    if (!pifp->valid) blankpackageperfile(pifp);

    /* Copy the priority and section across, but don't overwrite existing
     * values if the pdb_weakclassification flag is set.
     */
    if (newpig.section && *newpig.section &&
        !((flags & pdb_weakclassification) && pigp->section && *pigp->section))
      pigp->section= newpig.section;
    if (newpig.priority != pri_unknown &&
        !((flags & pdb_weakclassification) && pigp->priority != pri_unknown)) {
      pigp->priority= newpig.priority;
      if (newpig.priority == pri_other) pigp->otherpriority= newpig.otherpriority;
    }

    /* Sort out the dependency mess. */
    copy_dependency_links(pigp,&pifp->depends,newpifp->depends,
                          (flags & pdb_recordavailable) ? 1 : 0);
    /* Leave the `depended' pointer alone, we've just gone to such
     * trouble to get it right :-).  The `depends' pointer in
     * pifp was indeed also updated by copy_dependency_links,
     * but since the value was that from newpifp anyway there's
     * no need to copy it back.
     */
    newpifp->depended= pifp->depended;

    /* Copy across data */
    memcpy(pifp,newpifp,sizeof(struct pkginfoperfile));
    if (!(flags & pdb_recordavailable)) {
      pigp->want= newpig.want;
      pigp->eflag= newpig.eflag;
      pigp->status= newpig.status;
      pigp->configversion= newpig.configversion;
      pigp->files= NULL;

      pigp->trigpend_head = newpig.trigpend_head;
      pigp->trigaw = newpig.trigaw;
      for (ta = pigp->trigaw.head; ta; ta = ta->sameaw.next) {
        assert(ta->aw == &newpig);
        ta->aw = pigp;
        /* ->othertrigaw_head is updated by trig_note_aw in *(findpackage())
         * rather than in newpig */
      }

    } else if (!(flags & pdb_ignorefiles)) {
      pigp->files= newpig.files;
    }

    if (donep) *donep= pigp;
    pdone++;
    if (EOF_mmap(dataptr, endptr)) break;
    if (c == '\n') lno++;
  }
  if (data != NULL) {
#ifdef HAVE_MMAP
    munmap(data, stat.st_size);
#else
    free(data);
#endif
  }
  free(value);
  pop_cleanup(ehflag_normaltidy);
  if (close(fd)) ohshite(_("failed to close after read: `%.255s'"),filename);
  if (donep && !pdone) ohshit(_("no package information in `%.255s'"),filename);

  return pdone;
}

void copy_dependency_links(struct pkginfo *pkg,
                           struct dependency **updateme,
                           struct dependency *newdepends,
                           int available) {
  /* This routine is used to update the `reverse' dependency pointers
   * when new `forwards' information has been constructed.  It first
   * removes all the links based on the old information.  The old
   * information starts in *updateme; after much brou-ha-ha
   * the reverse structures are created and *updateme is set
   * to the value from newdepends.
   *
   * Parameters are:
   * pkg - the package we're doing this for.  This is used to
   *       construct correct uplinks.
   * updateme - the forwards dependency pointer that we are to
   *            update.  This starts out containing the old forwards
   *            info, which we use to unthread the old reverse
   *            links.  After we're done it is updated.
   * newdepends - the value that we ultimately want to have in
   *              updateme.
   * It is likely that the backward pointer for the package in
   * question (`depended') will be updated by this routine,
   * but this will happen by the routine traversing the dependency
   * data structures.  It doesn't need to be told where to update
   * that; I just mention it as something that one should be
   * cautious about.
   */
  struct dependency *dyp;
  struct deppossi *dop;
  struct pkginfoperfile *addtopifp;
  
  /* Delete `backward' (`depended') links from other packages to
   * dependencies listed in old version of this one.  We do this by
   * going through all the dependencies in the old version of this
   * one and following them down to find which deppossi nodes to
   * remove.
   */
  for (dyp= *updateme; dyp; dyp= dyp->next) {
    for (dop= dyp->list; dop; dop= dop->next) {
      if (dop->backrev)
        dop->backrev->nextrev= dop->nextrev;
      else
        if (available)
          dop->ed->available.depended= dop->nextrev;
        else
          dop->ed->installed.depended= dop->nextrev;
      if (dop->nextrev)
        dop->nextrev->backrev= dop->backrev;
    }
  }
  /* Now fill in new `ed' links from other packages to dependencies listed
   * in new version of this one, and set our uplinks correctly.
   */
  for (dyp= newdepends; dyp; dyp= dyp->next) {
    dyp->up= pkg;
    for (dop= dyp->list; dop; dop= dop->next) {
      addtopifp= available ? &dop->ed->available : &dop->ed->installed;
      if (!addtopifp->valid) blankpackageperfile(addtopifp);
      dop->nextrev= addtopifp->depended;
      dop->backrev= NULL;
      if (addtopifp->depended)
        addtopifp->depended->backrev= dop;
      addtopifp->depended= dop;
    }
  }
  /* Finally, we fill in the new value. */
  *updateme= newdepends;
}
