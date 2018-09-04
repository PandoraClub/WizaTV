/*
 * dselect - Debian package maintenance user interface
 * pkgdisplay.cc - package list display
 *
 * Copyright (C) 1994,1995 Ian Jackson <ian@chiark.greenend.org.uk>
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
 * License along with this; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
extern "C" {
#include <config.h>
}

#include <stdio.h>
#include <string.h>
#include <assert.h>

extern "C" {
#include <dpkg.h>
#include <dpkg-db.h>
}
#include "dselect.h"
#include "pkglist.h"

/* These MUST be in the same order as the corresponding enums in dpkg-db.h */
const char
  *const wantstrings[]=   { N_("new package"), 
			    N_("install"),
			    N_("hold"),
			    N_("remove"), 
			    N_("purge"),
			    0 },
/* WTA: the space is a trick to work around gettext which uses the empty
 * string to store information about the translation. DO NOT CHANGE
 * THAT IN A TRANSLATION! The code really relies on that being a single space.
 */
  *const eflagstrings[]=   { N_(" "), 
			     N_("REINSTALL"), 
			     0 },

  *const statusstrings[]= { N_("not installed"),
			    N_("removed (configs remain)"),
			    N_("half installed"),
			    N_("unpacked (not set up)"),
			    N_("failed config"),
			    N_("awaiting trigger processing"),
			    N_("triggered"),
			    N_("installed"),
			    0 },

  *const prioritystrings[]=  { N_("Required"),
			       N_("Important"),
			       N_("Standard"),
			       N_("Recommended"),
			       N_("Optional"),
			       N_("Extra"),
			       N_("Contrib"),
			       N_("!Bug!"),
			       N_("Unclassified"),
			       0 },

  *const relatestrings[]= { N_("suggests"),
			    N_("recommends"),
			    N_("depends on"),
			    N_("pre-depends on"),
			    N_("breaks"),
			    N_("conflicts with"),
			    N_("provides"),
			    N_("replaces"),
			    N_("enhances"),
			    0 },

  *const priorityabbrevs[]=  { N_("Req"),
			       N_("Imp"),
			       N_("Std"),
			       N_("Rec"),
			       N_("Opt"),
			       N_("Xtr"),
			       N_("Ctb"),
			       N_("bUG"),
			       N_("?") };

const char statuschars[] = " -IUCWt*";
const char eflagchars[]=     " R?#";
const char wantchars[]=     "n*=-_";

/* These MUST be in the same order as the corresponding enums in pkglist.h */
const char
  *const ssaabbrevs[]= { N_("Broken"),
                         N_("New"),
                         N_("Updated"),
                         N_("Obsolete/local"),
                         N_("Up-to-date"),
                         N_("Available"),
                         N_("Removed") },
  *const ssastrings[]= { N_("Brokenly installed packages"),
                         N_("Newly available packages"),
                         N_("Updated packages (newer version is available)"),
                         N_("Obsolete and local packages present on system"),
                         N_("Up to date installed packages"),
                         N_("Available packages (not currently installed)"),
                         N_("Removed and no longer available packages") };

const char
  *const sssstrings[]= { N_("Brokenly installed packages"),
                         N_("Installed packages"),
                         N_("Removed packages (configuration still present)"),
                         N_("Purged packages and those never installed") },
  *const sssabbrevs[]= { N_("Broken"),
                         N_("Installed"),
                         N_("Removed"),
                         N_("Purged") };

static int maximumstring(const char *const *array) {
  int maxlen= 0;
  while (*array) {
    int l= strlen(gettext(*array));
    const char *p= strchr(*array, '(');
    if (p && p > *array && *--p == ' ') l= p - *array;
    if (l > maxlen) maxlen= l;
    array++;
  }
  return maxlen;
}

void packagelist::setwidths() {
  if (debug) fprintf(debug,"packagelist[%p]::setwidths()\n",this);

  if (verbose) {
    status_hold_width= 9;
    status_status_width= maximumstring(statusstrings);
    status_want_width= maximumstring(wantstrings);
    status_width= status_hold_width+status_status_width+status_want_width*2+3;
    priority_width= 8;
    package_width= 16;
  } else {
    status_width= 4;
    priority_width= 3;
    package_width= 12;
  }
  section_width= 8;

  gap_width= 1;

  if (sortorder == so_section) {
    section_column= status_width + gap_width;
    priority_column= section_column + section_width + gap_width;
    package_column= priority_column + priority_width + gap_width;
  } else {
    priority_column= status_width + gap_width;
    section_column= priority_column + priority_width + gap_width;
    package_column= section_column + section_width + gap_width;
  }

  int versiondescriptioncolumn= package_column + package_width + gap_width;
  
  switch (versiondisplayopt) {
  case vdo_none:
    versioninstalled_column= versioninstalled_width= 0;
    versionavailable_column= versionavailable_width= 0;
    description_column= versiondescriptioncolumn;
    break;
  case vdo_available:
    versioninstalled_column= versioninstalled_width= 0;
    versionavailable_column= versiondescriptioncolumn;
    versionavailable_width= 11;
    description_column= versionavailable_column + versionavailable_width + gap_width;
    break;
  case vdo_both:
    versioninstalled_column= versiondescriptioncolumn;
    versioninstalled_width= 11;
    versionavailable_column= versioninstalled_column + versioninstalled_width +gap_width;
    versionavailable_width= versioninstalled_width;
    description_column= versionavailable_column + versionavailable_width + gap_width;
    break;
  default:
    internerr("unknown versiondisplayopt in setwidths");
  }
    
  total_width= TOTAL_LIST_WIDTH;
  if (total_width < COLS)
    total_width= COLS;
  description_width= total_width - description_column;
}

void packagelist::redrawtitle() {
  int x,y;
  
  if (title_height) {
    mywerase(titlewin);
    mvwaddnstr(titlewin,0,0,
               recursive ?  _("dselect - recursive package listing") :
               !readwrite ? _("dselect - inspection of package states") :
                            _("dselect - main package listing"),
               xmax);
    getyx(titlewin,y,x);
    if (x < xmax) {
      switch (sortorder) {
      case so_section:
        switch (statsortorder) {
        case sso_unsorted:
          waddnstr(titlewin, _(" (by section)"), xmax-x);
          break;
        case sso_avail:
          waddnstr(titlewin, _(" (avail., section)"), xmax-x);
          break;
        case sso_state:
          waddnstr(titlewin, _(" (status, section)"), xmax-x);
          break;
        default:
          internerr("bad statsort in redrawtitle/so_section");
        }
        break;
      case so_priority:
        switch (statsortorder) {
        case sso_unsorted:
          waddnstr(titlewin, _(" (by priority)"), xmax-x);
          break;
        case sso_avail:
          waddnstr(titlewin, _(" (avail., priority)"), xmax-x);
          break;
        case sso_state:
          waddnstr(titlewin, _(" (status, priority)"), xmax-x);
          break;
        default:
          internerr("bad statsort in redrawtitle/so_priority");
        }
        break;
      case so_alpha:
        switch (statsortorder) {
        case sso_unsorted:
          waddnstr(titlewin, _(" (alphabetically)"), xmax-x);
          break;
        case sso_avail:
          waddnstr(titlewin, _(" (by availability)"), xmax-x);
          break;
        case sso_state:
          waddnstr(titlewin, _(" (by status)"), xmax-x);
          break;
        default:
          internerr("bad statsort in redrawtitle/so_priority");
        }
        break;
        waddnstr(titlewin, _(" (alphabetically)"), xmax-x);
        break;
      case so_unsorted:
        break;
      default:
        internerr("bad sort in redrawtitle");
      }
    }
    const char *helpstring= readwrite ? (verbose ? _(" mark:+/=/- terse:v help:?")
                                                 : _(" mark:+/=/- verbose:v help:?"))
                                      : (verbose ? _(" terse:v help:?")
                                                 : _(" verbose:v help:?"));
    int l= strlen(helpstring);
    getyx(titlewin,y,x);
    if (xmax-l > 0) {
      mvwaddstr(titlewin,0,xmax-l, helpstring);
    }
    wnoutrefresh(titlewin);
  }
}
