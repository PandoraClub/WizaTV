/*
 * dselect - Debian package maintenance user interface
 * methkeys.cc - method list keybindings
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
#include "bindings.h"

const keybindings::interpretation methodlist_kinterps[] = {
  { "up",              &methodlist::kd_up,             0,    qa_noquit           },
  { "down",            &methodlist::kd_down,           0,    qa_noquit           },
  { "top",             &methodlist::kd_top,            0,    qa_noquit           },
  { "bottom",          &methodlist::kd_bottom,         0,    qa_noquit           },
  { "scrollon",        &methodlist::kd_scrollon,       0,    qa_noquit           },
  { "scrollback",      &methodlist::kd_scrollback,     0,    qa_noquit           },
  { "iscrollon",       &methodlist::kd_iscrollon,      0,    qa_noquit           },
  { "iscrollback",     &methodlist::kd_iscrollback,    0,    qa_noquit           },
  { "scrollon1",       &methodlist::kd_scrollon1,      0,    qa_noquit           },
  { "scrollback1",     &methodlist::kd_scrollback1,    0,    qa_noquit           },
  { "iscrollon1",      &methodlist::kd_iscrollon1,     0,    qa_noquit           },
  { "iscrollback1",    &methodlist::kd_iscrollback1,   0,    qa_noquit           },
  { "panon",           &methodlist::kd_panon,          0,    qa_noquit           },
  { "panback",         &methodlist::kd_panback,        0,    qa_noquit           },
  { "panon1",          &methodlist::kd_panon1,         0,    qa_noquit           },
  { "panback1",        &methodlist::kd_panback1,       0,    qa_noquit           },
  { "help",            &methodlist::kd_help,           0,    qa_noquit           },
  { "search",          &methodlist::kd_search,         0,    qa_noquit           },
  { "searchagain",     &methodlist::kd_searchagain,    0,    qa_noquit           },
  { "redraw",          &methodlist::kd_redraw,         0,    qa_noquit           },
  { "select-and-quit", &methodlist::kd_quit,           0,    qa_quitchecksave    },
  { "abort",           &methodlist::kd_abort,          0,    qa_quitnochecksave  },
  {  0,                0,                             0,    qa_noquit           }
};

#define C(x) ((x)-'a'+1)

const keybindings::orgbinding methodlist_korgbindings[]= {
  { 'j',            "down"           }, // vi style
//{ 'n',            "down"           }, // no style
  { KEY_DOWN,       "down"           },
  { 'k',            "up"             }, // vi style
//{ 'p',            "up"             }, // no style
  { KEY_UP,         "up"             },
  
  { C('f'),         "scrollon"       }, // vi style
  { 'N',            "scrollon"       },
  { KEY_NPAGE,      "scrollon"       },
  { ' ',            "scrollon"       },
  { C('b'),         "scrollback"     }, // vi style
  { 'P',            "scrollback"     },
  { KEY_PPAGE,      "scrollback"     },
  { KEY_BACKSPACE,  "scrollback"     },
  { 0177,/*DEL*/    "scrollback"     },
  { C('h'),         "scrollback"     },
  { C('n'),         "scrollon1"      },
  { C('p'),         "scrollback1"    },
  
  { 't',            "top"            },
  { KEY_HOME,       "top"            },
  { 'e',            "bottom"         },
  { KEY_LL,         "bottom"         },
  { KEY_END,        "bottom"         },
  
  { 'u',            "iscrollback"    },
  { 'd',            "iscrollon"      },
  { C('u'),         "iscrollback1"   },
  { C('d'),         "iscrollon1"     },
  
  { 'B',            "panback"        },
  { KEY_LEFT,       "panback"        },
  { 'F',            "panon"          },
  { KEY_RIGHT,      "panon"          },
  { C('b'),         "panback1"       },
  { C('f'),         "panon1"         },
                                      
  { '?',            "help"             },
  { KEY_HELP,       "help"             },
  { KEY_F(1),       "help"             },
  { '/',            "search"           },
  { 'n',            "searchagain"      },
  { '\\',           "searchagain"      },
  { C('l'),         "redraw"           },

  { KEY_ENTER,      "select-and-quit"  },
  { '\r',           "select-and-quit"  },
  { 'x',            "abort"            },
  { 'X',            "abort"            },
  { 'Q',            "abort"            },
  
  {  -1,             0                 }
};
