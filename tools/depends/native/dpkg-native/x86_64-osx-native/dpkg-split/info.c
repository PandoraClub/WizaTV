/*
 * dpkg-split - splitting and joining of multipart *.deb archives
 * info.c - information about split archives
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
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ar.h>
#include <ctype.h>

#include <dpkg.h>
#include <dpkg-db.h>
#include "dpkg-split.h"

static unsigned long unsignedlong(const char *value, const char *fn, const char *what) {
  unsigned long r;
  char *endp;

  r= strtoul(value,&endp,10);
  if (*endp)
    ohshit(_("file `%.250s' is corrupt - bad digit (code %d) in %s"),fn,*endp,what);
  return r;
}

static unsigned long parseheaderlength(const char *inh, size_t len,
                                       const char *fn, const char *what) {
  char lintbuf[15];

  if (memchr(inh,0,len))
    ohshit(_("file `%.250s' is corrupt - %.250s length contains nulls"),fn,what);
  assert(sizeof(lintbuf) > len);
  memcpy(lintbuf,inh,len);
  lintbuf[len]= ' ';
  *strchr(lintbuf,' ')= 0;
  return unsignedlong(lintbuf,fn,what);
}

static char *nextline(char **ripp, const char *fn, const char *what) {
  char *newline, *rip;

  rip= *ripp;
  if (!rip) ohshit(_("file `%.250s' is corrupt - %.250s missing"),fn,what);
  newline= strchr(rip,'\n');
  if (!newline)
    ohshit(_("file `%.250s' is corrupt - missing newline after %.250s"),fn,what);
  *ripp= newline+1;
  while (newline > rip && isspace(newline[-1])) newline--;
  *newline= 0;
  return rip;
}

struct partinfo *read_info(FILE *partfile, const char *fn, struct partinfo *ir) {
  /* returns info (nfmalloc'd) if was an archive part and we read it, 0 if it wasn't */
  static char *readinfobuf= NULL;
  static size_t readinfobuflen= 0;

  size_t thisilen;
  unsigned int templong;
  char magicbuf[sizeof(PARTMAGIC)-1], *rip, *partnums, *slash;
  struct ar_hdr arh;
  int c;
  struct stat stab;
  
  if (fread(magicbuf,1,sizeof(PARTMAGIC)-1,partfile) != sizeof(PARTMAGIC)-1) {
    if (ferror(partfile)) rerr(fn); else return NULL;
  }
  if (memcmp(magicbuf,PARTMAGIC,sizeof(magicbuf))) return NULL;
  if (fseek(partfile,-sizeof(arh.ar_name),SEEK_CUR))
    ohshite(_("unable to seek back"));
  
  if (fread(&arh,1,sizeof(arh),partfile) != sizeof(arh)) rerreof(partfile,fn);
  if (memcmp(arh.ar_fmag,ARFMAG,sizeof(arh.ar_fmag)))
    ohshit(_("file `%.250s' is corrupt - bad magic at end of first header"),fn);
  thisilen= parseheaderlength(arh.ar_size,sizeof(arh.ar_size),fn,"info length");
  if (thisilen >= readinfobuflen) {
    readinfobuflen= thisilen+1;
    readinfobuf= m_realloc(readinfobuf,readinfobuflen);
  }
  if (fread(readinfobuf,1,thisilen,partfile) != thisilen) rerreof(partfile,fn);
  if (thisilen & 1) {
    c= getc(partfile);  if (c==EOF) rerreof(partfile,fn);
    if (c != '\n')
      ohshit(_("file `%.250s' is corrupt - bad padding character (code %d)"),fn,c);
  }
  readinfobuf[thisilen]= 0;
  if (memchr(readinfobuf,0,thisilen))
    ohshit(_("file `%.250s' is corrupt - nulls in info section"),fn);

  ir->filename= fn;

  rip= readinfobuf;
  ir->fmtversion= nfstrsave(nextline(&rip,fn,"format version number"));
  if (strcmp(ir->fmtversion,SPLITVERSION))
    ohshit(_("file `%.250s' is format version `%.250s' - you need a newer dpkg-split"),
           fn,ir->fmtversion);

  ir->package= nfstrsave(nextline(&rip,fn,"package name"));
  ir->version= nfstrsave(nextline(&rip,fn,"package version number"));
  ir->md5sum= nfstrsave(nextline(&rip,fn,"package file MD5 checksum"));
  if (strlen(ir->md5sum) != 32 ||
      strspn(ir->md5sum,"0123456789abcdef") != 32)
    ohshit(_("file `%.250s' is corrupt - bad MD5 checksum `%.250s'"),fn,ir->md5sum);

  ir->orglength= unsignedlong(nextline(&rip,fn,"total length"),fn,"total length");
  ir->maxpartlen= unsignedlong(nextline(&rip,fn,"part offset"),fn,"part offset");
  
  partnums= nextline(&rip,fn,"part numbers");
  slash= strchr(partnums,'/');
  if (!slash) ohshit(_("file `%.250s' is corrupt - no slash between part numbers"),fn);
  *slash++= 0;

  templong= unsignedlong(slash,fn,"number of parts");
  if (templong <= 0 || templong > INT_MAX)
    ohshit("file `%.250s' is corrupt - bad number of parts",fn);
  ir->maxpartn= templong;
  templong= unsignedlong(partnums,fn,"parts number");
  if (templong <= 0 || templong > ir->maxpartn)
    ohshit(_("file `%.250s' is corrupt - bad part number"),fn);
  ir->thispartn= templong;

  if (fread(&arh,1,sizeof(arh),partfile) != sizeof(arh)) rerreof(partfile,fn);
  if (memcmp(arh.ar_fmag,ARFMAG,sizeof(arh.ar_fmag)))
    ohshit(_("file `%.250s' is corrupt - bad magic at end of second header"),fn);
  if (strncmp(arh.ar_name,"data",4))
    ohshit(_("file `%.250s' is corrupt - second member is not data member"),fn);

  ir->thispartlen= parseheaderlength(arh.ar_size,sizeof(arh.ar_size),fn,"data length");
  ir->thispartoffset= (ir->thispartn-1)*ir->maxpartlen;

  if (ir->maxpartn != (ir->orglength+ir->maxpartlen-1)/ir->maxpartlen)
    ohshit(_("file `%.250s' is corrupt - wrong number of parts for quoted sizes"),fn);
  if (ir->thispartlen !=
      (ir->thispartn == ir->maxpartn
       ? ir->orglength - ir->thispartoffset : ir->maxpartlen))
    ohshit(_("file `%.250s' is corrupt - size is wrong for quoted part number"),fn);

  ir->filesize= (SARMAG +
                 sizeof(arh) + thisilen + (thisilen&1) +
                 sizeof(arh) + ir->thispartlen + (ir->thispartlen&1));

  if (fstat(fileno(partfile),&stab)) ohshite(_("unable to fstat part file `%.250s'"),fn);
  if (S_ISREG(stab.st_mode)) {
    /* Don't do this check if it's coming from a pipe or something.  It's
     * only an extra sanity check anyway.
     */
    if (stab.st_size < ir->filesize)
      ohshit(_("file `%.250s' is corrupt - too short"),fn);
  }

  ir->headerlen= SARMAG + sizeof(arh) + thisilen + (thisilen&1) + sizeof(arh);
    
  return ir;
}  

void mustgetpartinfo(const char *filename, struct partinfo *ri) {
  FILE *part;
  
  part= fopen(filename,"r");
  if (!part) ohshite(_("cannot open archive part file `%.250s'"),filename);
  if (!read_info(part,filename,ri))
    ohshite(_("file `%.250s' is not an archive part"),filename);
  fclose(part);
}

void print_info(const struct partinfo *pi) {
  printf(_("%s:\n"
         "    Part format version:            %s\n"
         "    Part of package:                %s\n"
         "        ... version:                %s\n"
         "        ... MD5 checksum:           %s\n"
         "        ... length:                 %lu bytes\n"
         "        ... split every:            %lu bytes\n"
         "    Part number:                    %d/%d\n"
         "    Part length:                    %zi bytes\n"
         "    Part offset:                    %lu bytes\n"
         "    Part file size (used portion):  %lu bytes\n\n"),
         pi->filename,
         pi->fmtversion,
         pi->package,
         pi->version,
         pi->md5sum,
         pi->orglength,
         pi->maxpartlen,
         pi->thispartn,
         pi->maxpartn,
         pi->thispartlen,
         pi->thispartoffset,
         (unsigned long)pi->filesize);
}

void do_info(const char *const *argv) {
  const char *thisarg;
  struct partinfo *pi, ps;
  FILE *part;

  if (!*argv) badusage(_("--info requires one or more part file arguments"));
  
  while ((thisarg= *argv++)) {
    part= fopen(thisarg,"r");
    if (!part) ohshite(_("cannot open archive part file `%.250s'"),thisarg);
    pi= read_info(part,thisarg,&ps);
    fclose(part);
    if (pi) {
      print_info(pi);
    } else {
      printf(_("file `%s' is not an archive part\n"),thisarg);
    }
    if (ferror(stdout)) werr("stdout");
  }
}
