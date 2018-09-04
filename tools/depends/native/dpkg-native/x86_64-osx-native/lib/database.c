/*
 * libdpkg - Debian packaging suite library routines
 * dpkg-db.h - Low level package database routines (hash tables, etc.)
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

#include <ctype.h>
#include <string.h>

#include <dpkg.h>
#include <dpkg-db.h>

#define BINS 8191
 /* This must always be a prime for optimal performance.
  * With 4093 buckets, we glean a 20% speedup, for 8191 buckets
  * we get 23%. The nominal increase in memory usage is a mere
  * sizeof(void*)*8063 (I.E. less than 32KB on 32bit systems)
  */

static struct pkginfo *bins[BINS];
static int npackages;

#define FNV_offset_basis 2166136261ul
#define FNV_mixing_prime 16777619ul

/* Fowler/Noll/Vo -- simple string hash.
 * For more info, see http://www.isthe.com/chongo/tech/comp/fnv/index.html
 * */

static unsigned int hash(const char *name) {
  register unsigned int h = FNV_offset_basis;
  register unsigned int p = FNV_mixing_prime;
  while( *name ) {
    h *= p;
    h ^= *name++;
  }
  return h;
}

void blankversion(struct versionrevision *version) {
  version->epoch= 0;
  version->version= version->revision= NULL;
}

void blankpackage(struct pkginfo *pigp) {
  pigp->name= NULL;
  pigp->status= stat_notinstalled;
  pigp->eflag= eflagv_ok;
  pigp->want= want_unknown;
  pigp->priority= pri_unknown;
  pigp->otherpriority = NULL;
  pigp->section= NULL;
  blankversion(&pigp->configversion);
  pigp->files= NULL;
  pigp->installed.valid= 0;
  pigp->available.valid= 0;
  pigp->clientdata= NULL;
  pigp->color= white;
  pigp->trigaw.head = pigp->trigaw.tail = NULL;
  pigp->othertrigaw_head = NULL;
  pigp->trigpend_head = NULL;
  blankpackageperfile(&pigp->installed);
  blankpackageperfile(&pigp->available);
}

void blankpackageperfile(struct pkginfoperfile *pifp) {
  pifp->essential= 0;
  pifp->depends= NULL;
  pifp->depended= NULL;
  pifp->description= pifp->maintainer= pifp->source= pifp->installedsize= pifp->bugs= pifp->origin= NULL;
  pifp->architecture= NULL;
  blankversion(&pifp->version);
  pifp->conffiles= NULL;
  pifp->arbs= NULL;
  pifp->valid= 1;
}

static int nes(const char *s) { return s && *s; }

int informative(struct pkginfo *pkg, struct pkginfoperfile *info) {
  /* Used by dselect and dpkg query options as an aid to decide
   * whether to display things, and by dump to decide whether to write them
   * out.
   */
  if (info == &pkg->installed &&
      (pkg->want != want_unknown ||
       pkg->eflag != eflagv_ok ||
       pkg->status != stat_notinstalled ||
       informativeversion(&pkg->configversion)))
    /* We ignore Section and Priority, as these tend to hang around. */
    return 1;
  if (!info->valid) return 0;
  if (info->depends ||
      nes(info->description) ||
      nes(info->maintainer) ||
      nes(info->origin) ||
      nes(info->bugs) ||
      nes(info->installedsize) ||
      nes(info->source) ||
      informativeversion(&info->version) ||
      info->conffiles ||
      info->arbs) return 1;
  return 0;
}

struct pkginfo *findpackage(const char *inname) {
  struct pkginfo **pointerp, *newpkg;
  char *name = m_strdup(inname), *p;

  p= name;
  while(*p) { *p= tolower(*p); p++; }
  
  pointerp= bins + (hash(name) % (BINS));
  while (*pointerp && strcasecmp((*pointerp)->name,name))
    pointerp= &(*pointerp)->next;
  if (*pointerp) { free(name); return *pointerp; }

  newpkg= nfmalloc(sizeof(struct pkginfo));
  blankpackage(newpkg);
  newpkg->name= nfstrsave(name);
  newpkg->next= NULL;
  *pointerp= newpkg;
  npackages++;

  free(name);
  return newpkg;
}

int countpackages(void) {
  return npackages;
}

struct pkgiterator {
  struct pkginfo *pigp;
  int nbinn;
};

struct pkgiterator *iterpkgstart(void) {
  struct pkgiterator *i;
  i= m_malloc(sizeof(struct pkgiterator));
  i->pigp= NULL;
  i->nbinn= 0;
  return i;
}

struct pkginfo *iterpkgnext(struct pkgiterator *i) {
  struct pkginfo *r;
  while (!i->pigp) {
    if (i->nbinn >= BINS) return NULL;
    i->pigp= bins[i->nbinn++];
  }
  r= i->pigp; i->pigp= r->next; return r;
}

void iterpkgend(struct pkgiterator *i) {
  free(i);
}

void resetpackages(void) {
  int i;
  nffreeall();
  npackages= 0;
  for (i=0; i<BINS; i++) bins[i]= NULL;
}

void hashreport(FILE *file) {
  int i, c;
  struct pkginfo *pkg;
  int *freq;

  freq= m_malloc(sizeof(int)*npackages+1);
  for (i=0; i<=npackages; i++) freq[i]= 0;
  for (i=0; i<BINS; i++) {
    for (c=0, pkg= bins[i]; pkg; c++, pkg= pkg->next);
    fprintf(file,"bin %5d has %7d\n",i,c);
    freq[c]++;
  }
  for (i=npackages; i>0 && freq[i]==0; i--);
  while (i>=0) { fprintf(file,_("size %7d occurs %5d times\n"),i,freq[i]); i--; }
  if (ferror(file)) ohshite(_("failed write during hashreport"));
  free(freq);
}

/*
 * Test dataset package names were:
 *
 * agetty bash bc bdflush biff bin86 binutil binutils bison bsdutils
 * byacc chfn cron dc dictionaries diff dlltools dpkg e2fsprogs ed
 * elisp19 elm emacs emacs-nox emacs-x emacs19 file fileutils find
 * flex fsprogs gas gawk gcc gcc1 gcc2 gdb ghostview ghstview glibcdoc
 * gnuplot grep groff gs gs_both gs_svga gs_x gsfonts gxditviw gzip
 * hello hostname idanish ifrench igerman indent inewsinn info inn
 * ispell kbd kern1148 language ldso less libc libgr libgrdev librl
 * lilo linuxsrc login lout lpr m4 mailx make man manpages more mount
 * mtools ncurses netbase netpbm netstd patch perl4 perl5 procps
 * psutils rcs rdev sed sendmail seyon shar shellutils smail svgalib
 * syslogd sysvinit tar tcpdump tcsh tex texidoc texinfo textutils
 * time timezone trn unzip uuencode wenglish wu-ftpd x8514 xaxe xbase
 * xbdm2 xcomp xcoral xdevel xfig xfnt100 xfnt75 xfntbig xfntscl
 * xgames xherc xmach32 xmach8 xmono xnet xs3 xsvga xtexstuff xv
 * xvga16 xxgdb zip
 */
