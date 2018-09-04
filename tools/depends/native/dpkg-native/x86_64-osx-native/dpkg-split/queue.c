/*
 * dpkg-split - splitting and joining of multipart *.deb archives
 * queue.c - queue management
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

/*
 * Queue, in /var/lib/dpkg/parts, is a plain directory with one
 * file per part.
 *
 * parts are named
 *  <md5sum>.<maxpartlen>.<thispartn>.<maxpartn>
 * all numbers in hex
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

#include <dpkg.h>
#include <dpkg-db.h>
#include "dpkg-split.h"

static int decompose_filename(const char *filename, struct partqueue *pq) {
  const char *p;
  char *q;

  if (strspn(filename,"0123456789abcdef") != 32 || filename[32] != '.') return 0;
  q= nfmalloc(33);
  memcpy(q,filename,32); q[32]= 0;
  pq->info.md5sum= q;
  p= filename+33;
  pq->info.maxpartlen= strtol(p,&q,16); if (q==p || *q++ != '.') return 0;
  p=q; pq->info.thispartn= (int)strtol(p,&q,16); if (q==p || *q++ != '.') return 0;
  p=q; pq->info.maxpartn= (int)strtol(p,&q,16); if (q==p || *q) return 0;
  return 1;
}

void scandepot(void) {
  DIR *depot;
  struct dirent *de;
  struct partqueue *pq;
  char *p;

  assert(!queue);
  depot= opendir(depotdir);
  if (!depot) ohshite(_("unable to read depot directory `%.250s'"),depotdir);
  while ((de= readdir(depot))) {
    if (de->d_name[0] == '.') continue;
    pq= nfmalloc(sizeof(struct partqueue));
    pq->info.fmtversion= pq->info.package= pq->info.version= NULL;
    pq->info.orglength= pq->info.thispartoffset= pq->info.thispartlen= 0;
    pq->info.headerlen= 0;
    p= nfmalloc(strlen(depotdir)+strlen(de->d_name)+1);
    strcpy(p,depotdir);
    strcat(p,de->d_name);
    pq->info.filename= p;
    if (!decompose_filename(de->d_name,pq)) {
      pq->info.md5sum= NULL;
      pq->info.maxpartlen= pq->info.thispartn= pq->info.maxpartn= 0;
    }
    pq->nextinqueue= queue;
    queue= pq;
  }
}

static int partmatches(struct partinfo *pi, struct partinfo *refi) {
  return (pi->md5sum &&
          !strcmp(pi->md5sum,refi->md5sum) &&
          pi->maxpartn == refi->maxpartn &&
          pi->maxpartlen == refi->maxpartlen);
}

void do_auto(const char *const *argv) {
  const char *partfile;
  struct partinfo *pi, *refi, *npi, **partlist, *otherthispart;
  struct partqueue *pq;
  unsigned int i;
  int j, ap;
  long nr;
  FILE *part;
  void *buffer;
  char *p, *q;

  if (!outputfile) badusage(_("--auto requires the use of the --output option"));
  if (!(partfile= *argv++) || *argv)
    badusage(_("--auto requires exactly one part file argument"));

  refi= nfmalloc(sizeof(struct partqueue));
  part= fopen(partfile,"r");
  if (!part) ohshite(_("unable to read part file `%.250s'"),partfile);
  if (!read_info(part,partfile,refi)) {
    if (!npquiet)
      printf(_("File `%.250s' is not part of a multipart archive.\n"),partfile);
    if (fclose(stdout)) werr("stdout");
    exit(1);
  }
  fclose(part);
  scandepot();
  partlist= nfmalloc(sizeof(struct partinfo*)*refi->maxpartn);
  for (i = 0; i < refi->maxpartn; i++)
    partlist[i] = NULL;
  for (pq= queue; pq; pq= pq->nextinqueue) {
    pi= &pq->info;
    if (!partmatches(pi,refi)) continue;
    npi= nfmalloc(sizeof(struct partinfo));
    mustgetpartinfo(pi->filename,npi);
    addtopartlist(partlist,npi,refi);
  }
  /* If we already have a copy of this version we ignore it and prefer the
   * new one, but we still want to delete the one in the depot, so we
   * save its partinfo (with the filename) for later.  This also prevents
   * us from accidentally deleting the source file.
   */
  otherthispart= partlist[refi->thispartn-1];
  partlist[refi->thispartn-1]= refi;
  for (j=refi->maxpartn-1; j>=0 && partlist[j]; j--);

  if (j>=0) {

    part= fopen(partfile,"r");
    if (!part) ohshite(_("unable to reopen part file `%.250s'"),partfile);
    buffer= nfmalloc(refi->filesize);
    nr= fread(buffer,1,refi->filesize,part);
    if (nr != refi->filesize) rerreof(part,partfile);
    if (getc(part) != EOF) ohshit(_("part file `%.250s' has trailing garbage"),partfile);
    if (ferror(part)) rerr(partfile);
    fclose(part);
    p= nfmalloc(strlen(depotdir)+50);
    q= nfmalloc(strlen(depotdir)+200);
    sprintf(p,"%st.%lx",depotdir,(long)getpid());
    sprintf(q,"%s%s.%lx.%x.%x",depotdir,refi->md5sum,
            refi->maxpartlen,refi->thispartn,refi->maxpartn);
    part= fopen(p,"w");
    if (!part) ohshite(_("unable to open new depot file `%.250s'"),p);
    nr= fwrite(buffer,1,refi->filesize,part);
    if (nr != refi->filesize) werr(p);
    if (fclose(part)) werr(p);
    if (rename(p,q)) ohshite(_("unable to rename new depot file `%.250s' to `%.250s'"),p,q);

    printf(_("Part %d of package %s filed (still want "),refi->thispartn,refi->package);
    /* There are still some parts missing. */
    for (i=0, ap=0; i<refi->maxpartn; i++)
      if (!partlist[i])
        printf("%s%d", !ap++ ? "" : i == (unsigned int)j ? _(" and ") : ", ", i + 1);
    printf(").\n");

  } else {

    /* We have all the parts. */
    reassemble(partlist,outputfile);

    /* OK, delete all the parts (except the new one, which we never copied). */
    partlist[refi->thispartn-1]= otherthispart;
    for (i=0; i<refi->maxpartn; i++)
      if (partlist[i])
        if (unlink(partlist[i]->filename))
          ohshite(_("unable to delete used-up depot file `%.250s'"),partlist[i]->filename);

  }

  if (ferror(stderr)) werr("stderr");
}

void do_queue(const char *const *argv) {
  struct partqueue *pq, *qq;
  struct partinfo ti;
  const char *head;
  struct stat stab;
  unsigned long bytes;
  unsigned int i;

  if (*argv) badusage(_("--listq does not take any arguments"));
  scandepot();

  head= N_("Junk files left around in the depot directory:\n");
  for (pq= queue; pq; pq= pq->nextinqueue) {
    if (pq->info.md5sum) continue;
    fputs(gettext(head),stdout); head= "";
    if (lstat(pq->info.filename,&stab))
      ohshit(_("unable to stat `%.250s'"),pq->info.filename);
    if (S_ISREG(stab.st_mode)) {
      bytes= stab.st_size;
      printf(_(" %s (%lu bytes)\n"),pq->info.filename,bytes);
    } else {
      printf(_(" %s (not a plain file)\n"),pq->info.filename);
    }
  }
  if (!*head) putchar('\n');
  
  head= N_("Packages not yet reassembled:\n");
  for (pq= queue; pq; pq= pq->nextinqueue) {
    if (!pq->info.md5sum) continue;
    mustgetpartinfo(pq->info.filename,&ti);
    fputs(gettext(head),stdout); head= "";
    printf(" Package %s: part(s) ",ti.package);
    bytes= 0;
    for (i=0; i<ti.maxpartn; i++) {
      for (qq= pq;
           qq && !(partmatches(&qq->info,&ti) && qq->info.thispartn == i+1);
           qq= qq->nextinqueue);
      if (qq) {
        printf("%d ",i+1);
        if (lstat(qq->info.filename,&stab))
          ohshite(_("unable to stat `%.250s'"),qq->info.filename);
        if (!S_ISREG(stab.st_mode))
          ohshit(_("part file `%.250s' is not a plain file"),qq->info.filename);
        bytes+= stab.st_size;
        qq->info.md5sum= NULL; /* don't find this package again */
      }
    }
    printf(_("(total %lu bytes)\n"),bytes);
  }
  if (fclose(stdout)) werr("stdout");
}

enum discardwhich { ds_junk, ds_package, ds_all };

static void discardsome(enum discardwhich which, const char *package) {
  struct partqueue *pq;

  for (pq= queue; pq; pq= pq->nextinqueue) {
    switch (which) {
    case ds_junk:
      if (pq->info.md5sum) continue;
      break;
    case ds_package:
      if (!pq->info.md5sum || strcasecmp(pq->info.package,package)) continue;
      break;
    case ds_all:
      break;
    default: internerr("bad discardsome which");
    }
    if (unlink(pq->info.filename))
      ohshite(_("unable to discard `%.250s'"),pq->info.filename);
    printf(_("Deleted %s.\n"),pq->info.filename);
  }
}

void do_discard(const char *const *argv) {
  const char *thisarg;
  struct partqueue *pq;

  scandepot();
  if (*argv) {
    for (pq= queue; pq; pq= pq->nextinqueue)
      if (pq->info.md5sum)
        mustgetpartinfo(pq->info.filename,&pq->info);
    discardsome(ds_junk,NULL);
    while ((thisarg= *argv++)) discardsome(ds_package,thisarg);
  } else {
    discardsome(ds_all,NULL);
  }
}
