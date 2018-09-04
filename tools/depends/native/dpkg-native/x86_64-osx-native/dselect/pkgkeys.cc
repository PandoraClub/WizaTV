/*
 * dselect - Debian package maintenance user interface
 * pkgkeys.cc - package list keybindings
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

const keybindings::interpretation packagelist_kinterps[] = {
  { "up",               0,  &packagelist::kd_up,               qa_noquit           },
  { "down",             0,  &packagelist::kd_down,             qa_noquit           },
  { "top",              0,  &packagelist::kd_top,              qa_noquit           },
  { "bottom",           0,  &packagelist::kd_bottom,           qa_noquit           },
  { "scrollon",         0,  &packagelist::kd_scrollon,         qa_noquit           },
  { "scrollback",       0,  &packagelist::kd_scrollback,       qa_noquit           },
  { "iscrollon",        0,  &packagelist::kd_iscrollon,        qa_noquit           },
  { "iscrollback",      0,  &packagelist::kd_iscrollback,      qa_noquit           },
  { "scrollon1",        0,  &packagelist::kd_scrollon1,        qa_noquit           },
  { "scrollback1",      0,  &packagelist::kd_scrollback1,      qa_noquit           },
  { "iscrollon1",       0,  &packagelist::kd_iscrollon1,       qa_noquit           },
  { "iscrollback1",     0,  &packagelist::kd_iscrollback1,     qa_noquit           },
  { "panon",            0,  &packagelist::kd_panon,            qa_noquit           },
  { "panback",          0,  &packagelist::kd_panback,          qa_noquit           },
  { "panon1",           0,  &packagelist::kd_panon1,           qa_noquit           },
  { "panback1",         0,  &packagelist::kd_panback1,         qa_noquit           },
  { "install",          0,  &packagelist::kd_select,           qa_noquit           },
  { "remove",           0,  &packagelist::kd_deselect,         qa_noquit           },
  { "purge",            0,  &packagelist::kd_purge,            qa_noquit           },
  { "hold",             0,  &packagelist::kd_hold,             qa_noquit           },
  { "unhold",           0,  &packagelist::kd_unhold,           qa_noquit           },
  { "info",             0,  &packagelist::kd_info,             qa_noquit           },
  { "toggleinfo",       0,  &packagelist::kd_toggleinfo,       qa_noquit           },
  { "verbose",          0,  &packagelist::kd_verbose,          qa_noquit           },
  { "versiondisplay",   0,  &packagelist::kd_versiondisplay,   qa_noquit           },
  { "help",             0,  &packagelist::kd_help,             qa_noquit           },
  { "search",           0,  &packagelist::kd_search,           qa_noquit           },
  { "searchagain",      0,  &packagelist::kd_searchagain,      qa_noquit           },
  { "swaporder",        0,  &packagelist::kd_swaporder,        qa_noquit           },
  { "swapstatorder",    0,  &packagelist::kd_swapstatorder,    qa_noquit           },
  { "redraw",           0,  &packagelist::kd_redraw,           qa_noquit           },
  { "quitcheck",        0,  &packagelist::kd_quit_noop,        qa_quitchecksave    },
  { "quitrejectsug",    0,  &packagelist::kd_revertdirect,     qa_quitnochecksave  },
  { "quitnocheck",      0,  &packagelist::kd_quit_noop,        qa_quitnochecksave  },
  { "abortnocheck",     0,  &packagelist::kd_revert_abort,     qa_quitnochecksave  },
  { "revert",           0,  &packagelist::kd_revert_abort,     qa_noquit           },
  { "revertsuggest",    0,  &packagelist::kd_revertsuggest,    qa_noquit           },
  { "revertdirect",     0,  &packagelist::kd_revertdirect,     qa_noquit           },
  { "revertinstalled",  0,  &packagelist::kd_revertinstalled,  qa_noquit           },
  {  0,                 0,  0,                                qa_noquit           }
};

#define C(x) ((x)-'a'+1)

const keybindings::orgbinding packagelist_korgbindings[]= {
  { 'j',            "down"             }, // vi style
  { KEY_DOWN,       "down"             },
  { 'k',            "up"               }, // vi style
  { 'p',            "up"               },
  { KEY_UP,         "up"               },
                                       
  { 'N',            "scrollon"         },
  { KEY_NPAGE,      "scrollon"         },
  { ' ',            "scrollon"         },
  { 'P',            "scrollback"       },
  { KEY_PPAGE,      "scrollback"       },
  { KEY_BACKSPACE,  "scrollback"       },
  { 0177,           "scrollback"       }, // ASCII DEL
  { C('h'),         "scrollback"       },
  { C('n'),         "scrollon1"        },
  { C('p'),         "scrollback1"      },
                                       
  { 't',            "top"              },
  { KEY_HOME,       "top"              },
  { 'e',            "bottom"           },
  { KEY_LL,         "bottom"           },
  { KEY_END,        "bottom"           },
                                       
  { 'u',            "iscrollback"      },
  { 'd',            "iscrollon"        },
  { C('u'),         "iscrollback1"     },
  { C('d'),         "iscrollon1"       },
                                       
  { 'B',            "panback"          },
  { KEY_LEFT,       "panback"          },
  { 'F',            "panon"            },
  { KEY_RIGHT,      "panon"            },
  { C('b'),         "panback1"         },
  { C('f'),         "panon1"           },
                                       
  { '+',            "install"          },
  { KEY_IC,         "install"          },
  { '-',            "remove"           },
  { KEY_DC,         "remove"           },
  { '_',            "purge"            },
  { 'H',            "hold"             },
  { '=',            "hold"             },
  { 'G',            "unhold"           },
                                       
  { '?',            "help"             },
  { KEY_HELP,       "help"             },
  { KEY_F(1),       "help"             },
  { 'i',            "info"             },
  { 'I',            "toggleinfo"       },
  { 'o',            "swaporder"        },
  { 'O',            "swapstatorder"    },
  { 'v',            "verbose"          },
  { 'V',            "versiondisplay"   },
  { C('l'),         "redraw"           },
  { '/',            "search"           },
  { 'n',            "searchagain"      },
  { '\\',           "searchagain"      },
                                       
  { KEY_ENTER,      "quitcheck"        },
  { '\r',           "quitcheck"        },
  { 'Q',            "quitnocheck"      },
  { 'x',            "abortnocheck"     },
  { 'X',            "abortnocheck"     },
  { 'R',            "revert"           },
  { 'U',            "revertsuggest"    },
  { 'D',            "revertdirect"     },
  { 'C',            "revertinstalled"  },
                                       
  {  -1,             0                 }
};
