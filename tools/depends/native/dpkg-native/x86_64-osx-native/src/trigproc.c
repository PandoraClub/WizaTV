/*
 * dpkg - main program for package management
 * trigproc.c - trigger processing
 *
 * Copyright (C) 2007 Canonical Ltd
 * written by Ian Jackson <ian@chiark.greenend.org.uk>
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
#include <sys/stat.h>
#include <sys/fcntl.h>

#include <dpkg.h>
#include <dpkg-db.h>

#include "main.h"
#include "filesdb.h"

/*
 * Trigger processing algorithms:
 *
 *
 * There is a separate queue (`deferred trigproc list') for triggers
 * `relevant' to what we just did; when we find something triggered `now'
 * we add it to that queue (unless --no-triggers).
 *
 *
 * We want to prefer configuring packages where possible to doing
 * trigger processing, but we want to prefer trigger processing to
 * cycle-breaking and dependency forcing.  This is achieved as
 * follows:
 *
 * Each time during configure processing where a package D is blocked by
 * only (ie Depends unsatisfied but would be satisfied by) a t-awaiter W
 * we make a note of (one of) W's t-pending, T.  (Only the last such T.)
 * (If --no-triggers and nonempty argument list and package isn't in
 * argument list then we don't do this.)
 *
 * Each time in packages.c where we increment dependtry, we instead see
 * if we have encountered such a t-pending T.  If we do, we trigproc T
 * instead of incrementing dependtry and this counts as having done
 * something so we reset sincenothing.
 *
 *
 * For --triggers-only and --configure, we go through each thing in the
 * argument queue (the add_to_queue queue) and check what its state is
 * and if appropriate we trigproc it.  If we didn't have a queue (or had
 * just --pending) we search all triggers-pending packages and add them
 * to the deferred trigproc list.
 *
 *
 * Before quitting from most operations, we trigproc each package in the
 * deferred trigproc list.  This may (if not --no-triggers) of course add
 * new things to the deferred trigproc list.
 *
 *
 * Note that `we trigproc T' must involve trigger cycle detection and
 * also automatic setting of t-awaiters to t-pending or installed.  In
 * particular, we do cycle detection even for trigger processing in the
 * configure dependtry loop (and it is OK to do it for explicitly
 * specified packages from the command line arguments; duplicates are
 * removed by packages.c:process_queue).
 *
 */

/*========== deferred trigger queue ==========*/

static PKGQUEUE_DEF_INIT(deferred);

static void
trigproc_enqueue_deferred(struct pkginfo *pend)
{
	if (f_triggers < 0)
		return;
	ensure_package_clientdata(pend);
	if (pend->clientdata->trigprocdeferred)
		return;
	pend->clientdata->trigprocdeferred = add_to_some_queue(pend, &deferred);
	debug(dbg_triggers, "trigproc_enqueue_deferred pend=%s", pend->name);
}

void
trigproc_run_deferred(void)
{
	struct pkginqueue *node;
	struct pkginfo *pkg;

	debug(dbg_triggers, "trigproc_run_deferred");
	while ((node = remove_from_some_queue(&deferred))) {
		pkg = node->pkg;
		free(node);
		if (!pkg)
			continue;
		pkg->clientdata->trigprocdeferred = NULL;
		trigproc(pkg);
	}
}

void
trig_activate_packageprocessing(struct pkginfo *pkg)
{
	debug(dbg_triggersdetail, "trigproc_activate_packageprocessing pkg=%s",
	      pkg->name);

	trig_parse_ci(pkgadminfile(pkg, TRIGGERSCIFILE), NULL,
	              trig_cicb_statuschange_activate, pkg);
}

/*========== actual trigger processing ==========*/

struct trigcyclenode {
	struct trigcyclenode *next;
	struct trigcycleperpkg *pkgs;
	struct pkginfo *then_processed;
};

struct trigcycleperpkg {
	struct trigcycleperpkg *next;
	struct pkginfo *pkg;
	struct trigpend *then_trigs;
};

static int tortoise_advance;
static struct trigcyclenode *tortoise, *hare;

void
trigproc_reset_cycle(void)
{
	tortoise_advance = 0;
	tortoise = hare = NULL;
}

/* Returns package we're to give up on. */
static struct pkginfo *
check_trigger_cycle(struct pkginfo *processing_now)
{
	struct trigcyclenode *tcn;
	struct trigcycleperpkg *tcpp, *tortoise_pkg;
	struct trigpend *hare_trig, *tortoise_trig;
	struct pkgiterator *it;
	struct pkginfo *pkg, *giveup;
	const char *sep;

	debug(dbg_triggers, "check_triggers_cycle pnow=%s", processing_now->name);

	tcn = nfmalloc(sizeof(*tcn));
	tcn->pkgs = NULL;
	tcn->then_processed = processing_now;

	it = iterpkgstart();
	while ((pkg = iterpkgnext(it))) {
		if (!pkg->trigpend_head)
			continue;
		tcpp = nfmalloc(sizeof(*tcpp));
		tcpp->pkg = pkg;
		tcpp->then_trigs = pkg->trigpend_head;
		tcpp->next = tcn->pkgs;
		tcn->pkgs = tcpp;
	}
	if (!hare) {
		debug(dbg_triggersdetail, "check_triggers_cycle pnow=%s first",
		      processing_now->name);
		tcn->next = NULL;
		hare = tortoise = tcn;
		return NULL;
	}

	tcn->next = NULL;
	hare->next = tcn;
	hare = tcn;
	if (tortoise_advance)
		tortoise = tortoise->next;
	tortoise_advance = !tortoise_advance;

	/* Now we compare hare to tortoise.
	 * We want to find a trigger pending in tortoise which is not in hare
	 * if we find such a thing we have proved that hare isn't a superset
	 * of tortoise and so that we haven't found a loop (yet).
	 */
	for (tortoise_pkg = tortoise->pkgs;
	     tortoise_pkg;
	     tortoise_pkg = tortoise_pkg->next) {
		debug(dbg_triggersdetail, "check_triggers_cycle pnow=%s tortoise=%s",
		      processing_now->name, tortoise_pkg->pkg->name);
		for (tortoise_trig = tortoise_pkg->then_trigs;
		     tortoise_trig;
		     tortoise_trig = tortoise_trig->next) {
			debug(dbg_triggersdetail,
			      "check_triggers_cycle pnow=%s tortoise=%s"
			      " tortoisetrig=%s", processing_now->name,
			      tortoise_pkg->pkg->name, tortoise_trig->name);
			/* hare is now so we can just look up in the actual
			 * data. */
			for (hare_trig = tortoise_pkg->pkg->trigpend_head;
			     hare_trig;
			     hare_trig = hare_trig->next) {
				debug(dbg_triggersstupid,
				      "check_triggers_cycle pnow=%s tortoise=%s"
				      " tortoisetrig=%s haretrig=%s",
				      processing_now->name, tortoise_pkg->pkg->name,
				      tortoise_trig->name, hare_trig->name);
				if (!strcmp(hare_trig->name, tortoise_trig->name))
					goto found_in_hare;
			}
			/* Not found in hare, yay! */
			debug(dbg_triggersdetail,
			      "check_triggers_cycle pnow=%s tortoise=%s OK",
			      processing_now->name, tortoise_pkg->pkg->name);
			return NULL;
			found_in_hare:;
		}
	}
	/* Oh dear. hare is a superset of tortoise. We are making no progress. */
	fprintf(stderr, _("%s: cycle found while processing triggers:\n chain of"
	        " packages whose triggers are or may be responsible:\n"),
	        DPKG);
	sep = "  ";
	for (tcn = tortoise; tcn; tcn = tcn->next) {
		fprintf(stderr, "%s%s", sep, tcn->then_processed->name);
		sep = " -> ";
	}
	fprintf(stderr, _("\n" " packages' pending triggers which are"
	                  " or may be unresolvable:\n"));
	for (tortoise_pkg = tortoise->pkgs;
	     tortoise_pkg;
	     tortoise_pkg = tortoise_pkg->next) {
		fprintf(stderr, "  %s", tortoise_pkg->pkg->name);
		sep = ": ";
		for (tortoise_trig = tortoise_pkg->then_trigs;
		     tortoise_trig;
		     tortoise_trig = tortoise_trig->next) {
			fprintf(stderr, "%s%s", sep, tortoise_trig->name);
		}
		fprintf(stderr, "\n");
	}

	/* We give up on the _earliest_ package involved. */
	giveup = tortoise->pkgs->pkg;
	debug(dbg_triggers, "check_triggers_cycle pnow=%s giveup=%p",
	      processing_now->name, giveup->name);
	assert(giveup->status == stat_triggersawaited ||
	       giveup->status == stat_triggerspending);
	giveup->status = stat_halfconfigured;
	modstatdb_note(giveup);
	print_error_perpackage(_("triggers looping, abandoned"), giveup->name);

	return giveup;
}

void
trigproc(struct pkginfo *pkg)
{
	static struct varbuf namesarg;

	struct trigpend *tp;
	struct pkginfo *gaveup;

	debug(dbg_triggers, "trigproc %s", pkg->name);

	if (pkg->clientdata->trigprocdeferred)
		pkg->clientdata->trigprocdeferred->pkg = NULL;
	pkg->clientdata->trigprocdeferred = NULL;

	if (pkg->trigpend_head) {
		assert(pkg->status == stat_triggerspending ||
		       pkg->status == stat_triggersawaited);

		gaveup = check_trigger_cycle(pkg);
		if (gaveup == pkg)
			return;

		printf(_("Processing triggers for %s ...\n"), pkg->name);
		log_action("trigproc", pkg);

		varbufreset(&namesarg);
		for (tp = pkg->trigpend_head; tp; tp = tp->next) {
			varbufaddc(&namesarg, ' ');
			varbufaddstr(&namesarg, tp->name);
		}
		varbufaddc(&namesarg, 0);

		pkg->status = stat_halfconfigured;
		modstatdb_note(pkg);

		if (!f_noact) {
			sincenothing = 0;
			maintainer_script_postinst(pkg, "triggered",
			                           namesarg.buf + 1, NULL);
		}

		/* This is to cope if the package triggers itself: */
		pkg->status = pkg->trigaw.head ? stat_triggersawaited :
		              pkg->trigpend_head ? stat_triggerspending :
		              stat_installed;

		post_postinst_tasks_core(pkg);
	} else {
		/* In other branch is done by modstatdb_note. */
		trig_clear_awaiters(pkg);
	}
}

/*========== transitional global activation ==========*/

static void
transitional_interest_callback_ro(const char *trig, void *user)
{
	struct pkginfo *pend = user;

	debug(dbg_triggersdetail,
	      "trig_transitional_interest_callback trig=%s pend=%s",
	      trig, pend->name);
	if (pend->status >= stat_triggersawaited)
		trig_note_pend(pend, nfstrsave(trig));
}

static void
transitional_interest_callback(const char *trig, void *user)
{
	struct pkginfo *pend = user;

	trig_cicb_interest_add(trig, pend);
	transitional_interest_callback_ro(trig, user);
}

static void
trig_transitional_activate(enum modstatdb_rw cstatus)
{
	/* cstatus might be _read if we're in --no-act mode, in which
	 * case we don't write out all of the interest files etc.
	 * but we do invent all of the activations for our own benefit.
	 */
	struct pkgiterator *it;
	struct pkginfo *pkg;

	it = iterpkgstart();

	while ((pkg = iterpkgnext(it))) {
		if (pkg->status <= stat_halfinstalled)
			continue;
		debug(dbg_triggersdetail, "trig_transitional_activate %s %s",
		      pkg->name, statusinfos[pkg->status].name);
		pkg->trigpend_head = NULL;
		trig_parse_ci(pkgadminfile(pkg, TRIGGERSCIFILE),
		              cstatus >= msdbrw_write ?
		              transitional_interest_callback :
		              transitional_interest_callback_ro, NULL, pkg);
	}
	iterpkgend(it);
	if (cstatus >= msdbrw_write) {
		modstatdb_checkpoint();
		trig_file_interests_save();
	}
}

/*========== hook setup ==========*/

static struct filenamenode *
th_proper_nn_find(const char *name, int nonew)
{
	return findnamenode(name, nonew ? fnn_nonew : 0);
}

TRIGHOOKS_DEFINE_NAMENODE_ACCESSORS

static const struct trig_hooks trig_our_hooks = {
	trigproc_enqueue_deferred,
	trig_transitional_activate,
	th_proper_nn_find,
	th_nn_interested,
	th_nn_name
};

void
trigproc_install_hooks(void)
{
	trigh = trig_our_hooks;
}

