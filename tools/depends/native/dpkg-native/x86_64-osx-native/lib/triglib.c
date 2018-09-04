/*
 * libdpkg - Debian packaging suite library routines
 * triglib.c - trigger handling
 *
 * Copyright (C) 2007 Canonical Ltd
 * Written by Ian Jackson <ian@chiark.greenend.org.uk>
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

#include <assert.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dpkg.h>
#include <dpkg-db.h>
#include <dlist.h>

const char *
illegal_triggername(const char *p)
{
	int c;

	if (!*p)
		return _("empty trigger names are not permitted");

	while ((c = *p++)) {
		if (c <= ' ' || c >= 0177)
			return _("trigger name contains invalid character");
	}

	return NULL;
}

/*========== recording triggers ==========*/

/*---------- noting trigger activation in memory ----------*/

/* Called via trig_*activate* et al from:
 *   - trig_incorporate: reading of Unincorp (explicit trigger activations)
 *   - various places: processing start (`activate' in triggers ci file)
 *   - namenodetouse: file triggers during unpack / remove
 *   - deferred_configure: file triggers during config file processing
 *
 * Not called from trig_transitional_activation; that runs
 * trig_note_pend directly which means that (a) awaiters are not
 * recorded (how would we know?) and (b) we don't enqueue them for
 * deferred processing in this run.
 *
 * We add the trigger to Triggers-Pending first.  This makes it
 * harder to get into the state where Triggers-Awaited for aw lists
 * pend but Triggers-Pending for pend is empty.  (See also the
 * comment in deppossi_ok_found regarding this situation.)
 */

/* aw might be NULL, and trig is not copied! */
static void
trig_record_activation(struct pkginfo *pend, struct pkginfo *aw, char *trig)
{
	if (pend->status < stat_triggersawaited)
		/* Not interested then. */
		return;

	if (trig_note_pend(pend, trig))
		modstatdb_note_ifwrite(pend);

	if (trigh.enqueue_deferred)
		trigh.enqueue_deferred(pend);

	if (aw && pend->status > stat_configfiles)
		if (trig_note_aw(pend, aw)) {
			if (aw->status > stat_triggersawaited)
				aw->status = stat_triggersawaited;
			modstatdb_note_ifwrite(aw);
		}
}

/* NB that this is also called from fields.c where *pend is a temporary! */
int
trig_note_pend_core(struct pkginfo *pend, char *trig /*not copied!*/)
{
	struct trigpend *tp;

	for (tp = pend->trigpend_head; tp; tp = tp->next)
		if (!strcmp(tp->name, trig))
			return 0;

	tp = nfmalloc(sizeof(*tp));
	tp->name = trig;
	tp->next = pend->trigpend_head;
	pend->trigpend_head = tp;

	return 1;
}

/* Returns: 1 for done, 0 for already noted. */
int
trig_note_pend(struct pkginfo *pend, char *trig /*not copied!*/)
{
	if (!trig_note_pend_core(pend, trig))
		return 0;

	pend->status = pend->trigaw.head ? stat_triggersawaited :
	               stat_triggerspending;

	return 1;
}

/* Returns: 1 for done, 0 for already noted. */
/* NB that this is called also from fields.c where *aw is a temporary
 * but pend is from findpackage()! */
int
trig_note_aw(struct pkginfo *pend, struct pkginfo *aw)
{
	struct trigaw *ta;

	/* We search through aw's list because that's probably shorter. */
	for (ta = aw->trigaw.head; ta; ta = ta->sameaw.next)
		if (ta->pend == pend)
			return 0;

	ta = nfmalloc(sizeof(*ta));
	ta->aw = aw;
	ta->pend = pend;
	ta->nextsamepend = pend->othertrigaw_head;
	pend->othertrigaw_head = ta;
	LIST_LINK_TAIL_PART(aw->trigaw, ta, sameaw.);

	return 1;
}

void
trig_clear_awaiters(struct pkginfo *notpend)
{
	struct trigaw *ta;
	struct pkginfo *aw;

	assert(!notpend->trigpend_head);

	ta = notpend->othertrigaw_head;
	notpend->othertrigaw_head = NULL;
	for (; ta; ta = ta->nextsamepend) {
		aw = ta->aw;
		if (!aw)
			continue;
		LIST_UNLINK_PART(aw->trigaw, ta, sameaw.);
		if (!aw->trigaw.head && aw->status == stat_triggersawaited) {
			aw->status = aw->trigpend_head ? stat_triggerspending :
			             stat_installed;
			modstatdb_note(aw);
		}
	}
}

/* FIXME: switch to use the generic pkg list for lenny+1 */
struct pkg_list {
	struct pkg_list *next;
	struct pkginfo *pkg;
};

static struct pkg_list *trig_awaited_pend_head;

void
trig_enqueue_awaited_pend(struct pkginfo *pend)
{
	struct pkg_list *tp;

	for (tp = trig_awaited_pend_head; tp; tp = tp->next)
		if (tp->pkg == pend)
			return;

	tp = nfmalloc(sizeof(*tp));
	tp->pkg = pend;
	tp->next = trig_awaited_pend_head;
	trig_awaited_pend_head = tp;
}

/*
 * Fix up packages in state triggers-awaited w/o the corresponding package
 * with pending triggers. This can happen when dpkg was interrupted
 * while in modstatdb_note, and the package in triggers-pending had its
 * state modified but dpkg could not clearing the awaiters.
 *
 * XXX: possibly get rid of some of the checks done somewhere else for
 *      this condition at run-time.
 */
void
trig_fixup_awaiters(enum modstatdb_rw cstatus)
{
	struct pkg_list *tp;

	if (cstatus < msdbrw_write)
		return;

	for (tp = trig_awaited_pend_head; tp; tp = tp->next)
		if (!tp->pkg->trigpend_head)
			trig_clear_awaiters(tp->pkg);
	trig_awaited_pend_head = NULL;
}

/*---------- generalised handling of trigger kinds ----------*/

static const struct trigkindinfo tki_explicit, tki_file, tki_unknown;

struct trigkindinfo {
	/* Only for trig_activate_start. */
	void (*activate_start)(void);

	/* Rest are for everyone: */
	void (*activate_awaiter)(struct pkginfo *pkg /* may be NULL */);
	void (*activate_done)(void);
	void (*interest_change)(const char *name, struct pkginfo *pkg, int signum);
};

#define TKI_DEFINE(kindname)				\
  static const struct trigkindinfo tki_##kindname= {	\
    trk_##kindname##_activate_start,			\
    trk_##kindname##_activate_awaiter,			\
    trk_##kindname##_activate_done,			\
    trk_##kindname##_interest_change			\
  };

static const struct trigkindinfo *dtki;

/* As passed into activate_start. */
static const char *trig_activating_name;

static const struct trigkindinfo *
trig_classify_byname(const char *name)
{
	if (name[0] == '/') {
		const char *slash;

		slash = name;
		while (slash) {
			if (slash[1] == 0 || slash[1] == '/')
				goto invalid;

			slash = strchr(slash + 2, '/');
		}
		return &tki_file;
	}

	if (!illegal_packagename(name, NULL) && !strchr(name, '_'))
		return &tki_explicit;

invalid:
	return &tki_unknown;
}

/* Calling sequence is:
 *  trig_activate_start(triggername)
 *  dtki->activate_awaiter(awaiting_package) } zero or more times
 *  dtki->activate_awaiter(0)                }  in any order
 *  dtki->activate_done(0)
 */
static void
trig_activate_start(const char *name)
{
	dtki = trig_classify_byname(name);
	trig_activating_name = name;
	dtki->activate_start();
}

/*---------- unknown trigger kinds ----------*/

static void
trk_unknown_activate_start(void)
{
}

static void
trk_unknown_activate_awaiter(struct pkginfo *aw)
{
}

static void
trk_unknown_activate_done(void)
{
}

static void
trk_unknown_interest_change(const char *trig, struct pkginfo *pkg, int signum)
{
	ohshit(_("invalid or unknown syntax in trigger name `%.250s'"
	         " (in trigger interests for package `%.250s')"),
	       trig, pkg->name);
}

TKI_DEFINE(unknown);

/*---------- explicit triggers ----------*/

static FILE *trk_explicit_f;
static struct varbuf trk_explicit_fn;
static char *trk_explicit_trig;

static void
trk_explicit_activate_done(void)
{
	if (trk_explicit_f) {
		fclose(trk_explicit_f);
		trk_explicit_f = NULL;
	}
}

static void
trk_explicit_start(const char *trig)
{
	trk_explicit_activate_done();

	varbufreset(&trk_explicit_fn);
	varbufaddstr(&trk_explicit_fn, triggersdir);
	varbufaddstr(&trk_explicit_fn, trig);
	varbufaddc(&trk_explicit_fn, 0);

	trk_explicit_f = fopen(trk_explicit_fn.buf, "r");
	if (!trk_explicit_f) {
		if (errno != ENOENT)
			ohshite(_("failed to open trigger interest list file `%.250s'"),
			        trk_explicit_fn.buf);
	}
}

static int
trk_explicit_fgets(char *buf, size_t sz)
{
	return fgets_checked(buf, sz, trk_explicit_f, trk_explicit_fn.buf);
}

static void
trk_explicit_activate_start(void)
{
	trk_explicit_start(trig_activating_name);
	trk_explicit_trig = nfstrsave(trig_activating_name);
}

static void
trk_explicit_activate_awaiter(struct pkginfo *aw)
{
	char buf[1024];
	const char *emsg;
	struct pkginfo *pend;

	if (!trk_explicit_f)
		return;

	if (fseek(trk_explicit_f, 0, SEEK_SET))
		ohshite(_("failed to rewind trigger interest file `%.250s'"),
		        trk_explicit_fn.buf);

	while (trk_explicit_fgets(buf, sizeof(buf)) >= 0) {
		if ((emsg = illegal_packagename(buf, NULL)))
			ohshit(_("trigger interest file `%.250s' syntax error; "
			         "illegal package name `%.250s': %.250s"),
			       trk_explicit_fn.buf, buf, emsg);
		pend = findpackage(buf);
		trig_record_activation(pend, aw, trk_explicit_trig);
	}
}

static void
trk_explicit_interest_change(const char *trig,  struct pkginfo *pkg, int signum)
{
	static struct varbuf newfn;
	char buf[1024];
	FILE *nf;

	trk_explicit_start(trig);
	varbufreset(&newfn);
	varbufprintf(&newfn, "%s/%s.new", triggersdir, trig);
	varbufaddc(&newfn, 0);

	nf = fopen(newfn.buf, "w");
	if (!nf)
		ohshite(_("unable to create new trigger interest file `%.250s'"),
		        newfn.buf);
	push_cleanup(cu_closefile, ~ehflag_normaltidy, NULL, 0, 1, nf);

	while (trk_explicit_f && trk_explicit_fgets(buf, sizeof(buf)) >= 0) {
		if (!strcmp(buf, pkg->name))
			continue;
		fprintf(nf, "%s\n", buf);
	}
	if (signum > 0)
		fprintf(nf, "%s\n", pkg->name);
	if (ferror(nf) || fclose(nf))
		ohshite(_("unable to write new trigger interest file `%.250s'"),
		        newfn.buf);
	pop_cleanup(ehflag_normaltidy);

	if (rename(newfn.buf, trk_explicit_fn.buf))
		ohshite(_("unable to install new trigger interest file `%.250s'"),
		        trk_explicit_fn.buf);
}

TKI_DEFINE(explicit);

/*---------- file triggers ----------*/

static struct {
	struct trigfileint *head, *tail;
} filetriggers;

/*
 * Values:
 *  -1: not read,
 *   0: not edited
 *   1: edited
 */
static int filetriggers_edited = -1;

/* Called by various people with signum -1 and +1 to mean remove and add
 * and also by trig_file_interests_ensure with signum +2 meaning add
 * but die if already present.
 */
static void
trk_file_interest_change(const char *trig, struct pkginfo *pkg, int signum)
{
	struct filenamenode *fnn;
	struct trigfileint **search, *tfi;

	fnn = trigh.namenode_find(trig, signum <= 0);
	if (!fnn) {
		assert(signum < 0);
		return;
	}

	for (search = trigh.namenode_interested(fnn);
	     (tfi = *search);
	     search = &tfi->samefile_next)
		if (tfi->pkg == pkg)
			goto found;

	/* not found */
	if (signum < 0)
		return;

	tfi = nfmalloc(sizeof(*tfi));
	tfi->pkg = pkg;
	tfi->fnn = fnn;
	tfi->samefile_next = *trigh.namenode_interested(fnn);
	*trigh.namenode_interested(fnn) = tfi;

	LIST_LINK_TAIL_PART(filetriggers, tfi, inoverall.);
	goto edited;

found:
	if (signum > 1)
		ohshit(_("duplicate file trigger interest for filename `%.250s' "
		         "and package `%.250s'"), trig, pkg->name);
	if (signum > 0)
		return;

	/* remove it: */
	*search = tfi->samefile_next;
	LIST_UNLINK_PART(filetriggers, tfi, inoverall.);
edited:
	filetriggers_edited = 1;
}

void
trig_file_interests_save(void)
{
	struct trigfileint *tfi;
	FILE *nf;

	if (filetriggers_edited <= 0)
		return;

	nf = fopen(triggersnewfilefile, "w");
	if (!nf)
		ohshite(_("unable to create new file triggers file `%.250s'"),
		        triggersnewfilefile);
	push_cleanup(cu_closefile, ~ehflag_normaltidy, NULL, 0, 1, nf);

	for (tfi = filetriggers.head; tfi; tfi = tfi->inoverall.next)
		fprintf(nf, "%s %s\n", trigh.namenode_name(tfi->fnn),
		        tfi->pkg->name);

	if (ferror(nf) || fclose(nf))
		ohshite(_("unable to write new file triggers file `%.250s'"),
		        triggersnewfilefile);
	pop_cleanup(ehflag_normaltidy);

	if (rename(triggersnewfilefile, triggersfilefile))
		ohshite(_("unable to install new file triggers file as `%.250s'"),
		        triggersfilefile);

	filetriggers_edited = 0;
}

void
trig_file_interests_ensure(void)
{
	FILE *f;
	char linebuf[1024], *space;
	struct pkginfo *pkg;
	const char *emsg;

	if (filetriggers_edited >= 0)
		return;

	f = fopen(triggersfilefile, "r");
	if (!f) {
		if (errno == ENOENT)
			goto ok;
		ohshite(_("unable to read file triggers file `%.250s'"),
		        triggersfilefile);
	}

	push_cleanup(cu_closefile, ~0, NULL, 0, 1, f);
	while (fgets_checked(linebuf, sizeof(linebuf), f, triggersfilefile) >= 0) {
		space = strchr(linebuf, ' ');
		if (!space || linebuf[0] != '/')
			ohshit(_("syntax error in file triggers file `%.250s'"),
			       triggersfilefile);
		*space++ = 0;
		if ((emsg = illegal_packagename(space, NULL)))
			ohshit(_("file triggers record mentions illegal "
			         "package name `%.250s' (for interest in file "
				 "`%.250s'): %.250s"), space, linebuf, emsg);
		pkg = findpackage(space);
		trk_file_interest_change(linebuf, pkg, +2);
	}
	pop_cleanup(ehflag_normaltidy);
ok:
	filetriggers_edited = 0;
}

static struct filenamenode *filetriggers_activating;

void
trig_file_activate_byname(const char *trig, struct pkginfo *aw)
{
	struct filenamenode *fnn = trigh.namenode_find(trig, 1);

	if (fnn)
		trig_file_activate(fnn, aw);
}

void
trig_file_activate(struct filenamenode *trig, struct pkginfo *aw)
{
	struct trigfileint *tfi;

	for (tfi = *trigh.namenode_interested(trig); tfi;
	     tfi = tfi->samefile_next)
		trig_record_activation(tfi->pkg, aw, (char*)trigh.namenode_name(trig));
}

static void
trk_file_activate_start(void)
{
	filetriggers_activating = trigh.namenode_find(trig_activating_name, 1);
}

static void
trk_file_activate_awaiter(struct pkginfo *aw)
{
	if (!filetriggers_activating)
		return;
	trig_file_activate(filetriggers_activating, aw);
}

static void
trk_file_activate_done(void)
{
}

TKI_DEFINE(file);

/*---------- trigger control info file ----------*/

static void
trig_cicb_interest_change(const char *trig, struct pkginfo *pkg, int signum)
{
	const struct trigkindinfo *tki = trig_classify_byname(trig);

	assert(filetriggers_edited >= 0);
	tki->interest_change(trig, pkg, signum);
}

void
trig_cicb_interest_delete(const char *trig, void *user)
{
	trig_cicb_interest_change(trig, user, -1);
}

void
trig_cicb_interest_add(const char *trig, void *user)
{
	trig_cicb_interest_change(trig, user, +1);
}

void
trig_cicb_statuschange_activate(const char *trig, void *user)
{
	struct pkginfo *aw = user;

	trig_activate_start(trig);
	dtki->activate_awaiter(aw);
	dtki->activate_done();
}

static void
parse_ci_call(const char *file, const char *cmd, trig_parse_cicb *cb,
              const char *trig, void *user)
{
	const char *emsg;

	emsg = illegal_triggername(trig);
	if (emsg)
		ohshit(_("triggers ci file `%.250s' contains illegal trigger "
		         "syntax in trigger name `%.250s': %.250s"),
		       file, trig, emsg);
	if (cb)
		cb(trig, user);
}

void
trig_parse_ci(const char *file, trig_parse_cicb *interest,
              trig_parse_cicb *activate, void *user)
{
	FILE *f;
	char linebuf[MAXTRIGDIRECTIVE], *cmd, *spc, *eol;
	int l;

	f = fopen(file, "r");
	if (!f) {
		if (errno == ENOENT)
			/* No file is just like an empty one. */
			return;
		ohshite(_("unable to open triggers ci file `%.250s'"), file);
	}
	push_cleanup(cu_closefile, ~0, NULL, 0, 1, f);

	while ((l = fgets_checked(linebuf, sizeof(linebuf), f, file)) >= 0) {
		for (cmd = linebuf; cisspace(*cmd); cmd++);
		if (*cmd == '#')
			continue;
		for (eol = linebuf + l; eol > cmd && cisspace(eol[-1]); eol--);
		if (eol == cmd)
			continue;
		*eol = 0;

		for (spc = cmd; *spc && !cisspace(*spc); spc++);
		if (!*spc)
			ohshit(_("triggers ci file contains unknown directive syntax"));
		*spc++ = 0;
		while (cisspace(*spc))
			spc++;
		if (!strcmp(cmd, "interest")) {
			parse_ci_call(file, cmd, interest, spc, user);
		} else if (!strcmp(cmd, "activate")) {
			parse_ci_call(file, cmd, activate, spc, user);
		} else {
			ohshit(_("triggers ci file contains unknown directive `%.250s'"),
			       cmd);
		}
	}
	pop_cleanup(ehflag_normaltidy); /* fclose */
}

/*---------- Unincorp file incorporation ----------*/

static void
tdm_incorp_trig_begin(const char *trig)
{
	trig_activate_start(trig);
}

static void
tdm_incorp_package(const char *awname)
{
	struct pkginfo *aw = strcmp(awname, "-") ? findpackage(awname) : NULL;

	dtki->activate_awaiter(aw);
}

static void
tdm_incorp_trig_end(void)
{
	dtki->activate_done();
}

static const struct trigdefmeths tdm_incorp = {
	tdm_incorp_trig_begin,
	tdm_incorp_package,
	tdm_incorp_trig_end
};

void
trig_incorporate(enum modstatdb_rw cstatus, const char *admindir)
{
	int ur;
	enum trigdef_updateflags tduf;

	trigdef = &tdm_incorp;
	trig_file_interests_ensure();

	tduf = tduf_nolockok;
	if (cstatus >= msdbrw_write) {
		tduf |= tduf_write;
		if (trigh.transitional_activate)
			tduf |= tduf_writeifenoent;
	}

	ur = trigdef_update_start(tduf, admindir);
	if (ur == -1 && cstatus >= msdbrw_write) {
		if (mkdir(triggersdir, 0755)) {
			if (errno != EEXIST)
				ohshite(_("unable to create triggers state"
				          " directory `%.250s'"), triggersdir);
		} else if (chown(triggersdir, 0, 0)) {
			ohshite(_("unable to set ownership of triggers state"
			          " directory `%.250s'"), triggersdir);
		}
		ur = trigdef_update_start(tduf, admindir);
	}
	switch (ur) {
	case -2:
		return;
	case -1:
	case -3:
		if (!trigh.transitional_activate)
			return;
	/* Fall through. */
	case 1:
		trigh.transitional_activate(cstatus);
		break;
	case 2:
		/* Read and incorporate triggers. */
		trigdef_yylex();
		break;
	default:
		abort();
	}

	/* Right, that's it. New (empty) Unincopr can be installed. */
	trigdef_process_done();
}

/*---------- default hooks ----------*/

struct filenamenode {
	struct filenamenode *next;
	const char *name;
	struct trigfileint *trig_interested;
};

static struct filenamenode *trigger_files;

static struct filenamenode *
th_simple_nn_find(const char *name, int nonew)
{
	struct filenamenode *search;

	for (search = trigger_files; search; search = search->next)
		if (!strcmp(search->name, name))
			return search;
	/* Not found. */
	if (nonew)
		return NULL;

	search = nfmalloc(sizeof(*search));
	search->name = nfstrsave(name);
	search->trig_interested = NULL;
	search->next = trigger_files;
	trigger_files = search;

	return search;
}

TRIGHOOKS_DEFINE_NAMENODE_ACCESSORS

struct trig_hooks trigh = {
	NULL,
	NULL,
	th_simple_nn_find,
	th_nn_interested,
	th_nn_name
};

