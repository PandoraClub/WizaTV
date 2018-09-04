/* -*- c++ -*-
 * dselect - selection of Debian packages
 * dselect.h - external definitions for this program
 *
 * Copyright (C) 1994,1995 Ian Jackson <ian@chiark.greenend.org.uk>
 * Copyright (C) 2001 Wichert Akkerman <wakkerma@debian.org>
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

#ifndef DSELECT_H
#define DSELECT_H

#define TOTAL_LIST_WIDTH 180
#define MAX_DISPLAY_INFO 120

#include <signal.h>
#undef ERR
#include <ncursesw/curses.h>

struct helpmenuentry {
  char key;
  const struct helpmessage *msg;
};

class keybindings;

class baselist {
protected:
  // Screen dimensions &c.
  int xmax, ymax;
  int title_height, colheads_height, list_height;
  int thisstate_height, info_height, whatinfo_height;
  int colheads_row, thisstate_row, info_row, whatinfo_row, list_row;
  int list_attr, listsel_attr, title_attr, colheads_attr, info_attr;
  int info_headattr, whatinfo_attr;
  int thisstate_attr, query_attr;
  int selstate_attr, selstatesel_attr;
  int helpscreen_attr;

  int total_width;
  
  // (n)curses stuff
  WINDOW *listpad, *infopad, *colheadspad, *thisstatepad;
  WINDOW *titlewin, *whatinfowin, *querywin;
  // If listpad is null, then we have not started to display yet, and
  // so none of the auto-displaying update routines need to display.

  // SIGWINCH handling
  struct sigaction *osigactp, nsigact;
  sigset_t *oblockedp, sigwinchset;
  void setupsigwinch();

  static baselist *signallist;
  static void sigwinchhandler(int);

  int nitems, ldrawnstart, ldrawnend, showinfo;
  int topofscreen, leftofscreen, cursorline;
  int infotopofscreen, infolines;
  varbuf whatinfovb;
  char searchstring[128];

  virtual void setheights();
  void unsizes();
  void dosearch();
  void displayhelp(const struct helpmenuentry *menu, int key);
  void displayerror(const char* str);

  void redrawall();
  void redrawitemsrange(int start /*inclusive*/, int end /*exclusive*/);
  void redraw1item(int index);
  void refreshlist();
  void refreshinfo();
  void refreshcolheads();
  void setcursor(int index);

  void itd_keys();

  virtual void redraw1itemsel(int index, int selected) =0;
  virtual void redrawcolheads() =0;
  virtual void redrawthisstate() =0;
  virtual void redrawinfo() =0;
  virtual void redrawtitle() =0;
  virtual void setwidths() =0;
  virtual const char *itemname(int index) =0;
  virtual const struct helpmenuentry *helpmenulist() =0;

  virtual int checksearch(char* str);
  virtual int matchsearch(int index);
  void wordwrapinfo(int offset, const char *string);
  
public:

  keybindings *bindings;

  void kd_up();
  void kd_down();
  void kd_redraw();
  void kd_scrollon();
  void kd_scrollback();
  void kd_scrollon1();
  void kd_scrollback1();
  void kd_panon();
  void kd_panback();
  void kd_panon1();
  void kd_panback1();
  void kd_top();
  void kd_bottom();
  void kd_iscrollon();
  void kd_iscrollback();
  void kd_iscrollon1();
  void kd_iscrollback1();
  void kd_search();
  void kd_searchagain();
  void kd_help();

  void startdisplay();
  void enddisplay();

  baselist(keybindings*);
  virtual ~baselist();
};

static inline int lesserint(int a, int b) { return a<b ? a : b; }
static inline int greaterint(int a, int b) { return a>b ? a : b; }

void displayhelp(const struct helpmenuentry *menu, int key);

void mywerase(WINDOW *win);

void curseson();
void cursesoff();

extern const char *admindir;
extern FILE *debug;
extern int expertmode;

enum screenparts {
       background,
       list,
       listsel,
       title,
       thisstate,
       selstate,
       selstatesel,
       colheads,
       query,
       info,
       info_head,
       whatinfo,
       helpscreen,
       numscreenparts,
};

struct colordata {
       int fore;
       int back;
       int attr;
};
extern colordata color[];

/* Evil recommends flag variable. */
extern int manual_install;

enum urqresult { urqr_normal, urqr_fail, urqr_quitmenu };
enum quitaction { qa_noquit, qa_quitchecksave, qa_quitnochecksave };

typedef urqresult urqfunction(void);
urqfunction urq_list, urq_quit, urq_menu;
urqfunction urq_setup, urq_update, urq_install, urq_config, urq_remove;

urqresult falliblesubprocess(const char *exepath, const char *name,
                             const char *const *args);

#endif /* DSELECT_H */
