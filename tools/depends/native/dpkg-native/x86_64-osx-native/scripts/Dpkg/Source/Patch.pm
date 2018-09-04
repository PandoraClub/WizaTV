# Copyright 2008 Raphaël Hertzog <hertzog@debian.org>

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

package Dpkg::Source::Patch;

use strict;
use warnings;

use Dpkg;
use Dpkg::Source::CompressedFile;
use Dpkg::Source::Compressor;
use Dpkg::Compression;
use Dpkg::Gettext;
use Dpkg::IPC;
use Dpkg::ErrorHandling qw(error errormsg syserr warning subprocerr internerr);

use POSIX;
use File::Find;
use File::Basename;
use File::Spec;
use File::Path;
use Fcntl ':mode';
#XXX: Needed for sub-second timestamps, require recent perl
#use Time::HiRes qw(stat);

use base 'Dpkg::Source::CompressedFile';

sub create {
    my ($self, %opts) = @_;
    $self->{'handle'} = $self->open_for_write();
    $self->{'errors'} = 0;
    if ($opts{'old'} and $opts{'new'}) {
        $opts{'old'} = "/dev/null" unless -e $opts{'old'};
        $opts{'new'} = "/dev/null" unless -e $opts{'new'};
        if (-d $opts{'old'} and -d $opts{'new'}) {
            $self->add_diff_directory($opts{'old'}, $opts{'new'}, %opts);
        } elsif (-f $opts{'old'} and -f $opts{'new'}) {
            $self->add_diff_file($opts{'old'}, $opts{'new'}, %opts);
        } else {
            $self->_fail_not_same_type($opts{'old'}, $opts{'new'});
        }
        $self->finish() unless $opts{"nofinish"};
    }
}

sub add_diff_file {
    my ($self, $old, $new, %opts) = @_;
    $opts{"include_timestamp"} = 0 unless exists $opts{"include_timestamp"};
    my $handle_binary = $opts{"handle_binary_func"} || sub {
        my ($self, $old, $new) = @_;
        $self->_fail_with_msg($new, _g("binary file contents changed"));
    };
    # Default diff options
    my @options;
    if ($opts{"options"}) {
        push @options, @{$opts{"options"}};
    } else {
        push @options, '-p';
    }
    # Add labels
    if ($opts{"label_old"} and $opts{"label_new"}) {
	if ($opts{"include_timestamp"}) {
	    my $ts = (stat($old))[9];
	    my $t = POSIX::strftime("%Y-%m-%d %H:%M:%S", gmtime($ts));
	    $opts{"label_old"} .= sprintf("\t%s.%09d +0000", $t,
					    ($ts-int($ts))*1000000000);
	    $ts = (stat($new))[9];
	    $t = POSIX::strftime("%Y-%m-%d %H:%M:%S", gmtime($ts));
	    $opts{"label_new"} .= sprintf("\t%s.%09d +0000", $t,
					    ($ts-int($ts))*1000000000);
	} else {
	    # Space in filenames need special treatment
	    $opts{"label_old"} .= "\t" if $opts{"label_old"} =~ / /;
	    $opts{"label_new"} .= "\t" if $opts{"label_new"} =~ / /;
	}
        push @options, "-L", $opts{"label_old"},
                       "-L", $opts{"label_new"};
    }
    # Generate diff
    my $diffgen;
    my $diff_pid = fork_and_exec(
        'exec' => [ 'diff', '-u', @options, '--', $old, $new ],
        'env' => { LC_ALL => 'C', LANG => 'C', TZ => 'UTC0' },
        'to_pipe' => \$diffgen
    );
    # Check diff and write it in patch file
    my $difflinefound = 0;
    my $binary = 0;
    while (<$diffgen>) {
        if (m/^binary/i) {
            $binary = 1;
            &$handle_binary($self, $old, $new);
            last;
        } elsif (m/^[-+\@ ]/) {
            $difflinefound++;
        } elsif (m/^\\ No newline at end of file$/) {
            warning(_g("file %s has no final newline (either " .
                       "original or modified version)"), $new);
        } else {
            chomp;
            internerr(_g("unknown line from diff -u on %s: `%s'"),
                      $new, $_);
        }
        print({ $self->{'handle'} } $_) || syserr(_g("failed to write"));
    }
    close($diffgen) or syserr("close on diff pipe");
    wait_child($diff_pid, nocheck => 1,
               cmdline => "diff -u @options -- $old $new");
    # Verify diff process ended successfully
    # Exit code of diff: 0 => no difference, 1 => diff ok, 2 => error
    # Ignore error if binary content detected
    my $exit = WEXITSTATUS($?);
    unless (WIFEXITED($?) && ($exit == 0 || $exit == 1 || $binary)) {
        subprocerr(_g("diff on %s"), $new);
    }
    return ($exit == 0 || $exit == 1);
}

sub add_diff_directory {
    my ($self, $old, $new, %opts) = @_;
    # TODO: make this function more configurable
    # - offer to disable some checks
    my $basedir = $opts{"basedirname"} || basename($new);
    my $inc_removal = $opts{"include_removal"} || 0;
    my $diff_ignore;
    if ($opts{"diff_ignore_func"}) {
        $diff_ignore = $opts{"diff_ignore_func"};
    } elsif ($opts{"diff_ignore_regexp"}) {
        $diff_ignore = sub { return $_[0] =~ /$opts{"diff_ignore_regexp"}/o };
    } else {
        $diff_ignore = sub { return 0 };
    }

    my %files_in_new;
    my $scan_new = sub {
        my $fn = File::Spec->abs2rel($_, $new);
        return if &$diff_ignore($fn);
        $files_in_new{$fn} = 1;
        lstat("$new/$fn") || syserr(_g("cannot stat file %s"), "$new/$fn");
        my $mode = S_IMODE((lstat(_))[2]);
        my $size = (lstat(_))[7];
        if (-l _) {
            unless (-l "$old/$fn") {
                $self->_fail_not_same_type("$old/$fn", "$new/$fn");
                return;
            }
            defined(my $n = readlink("$new/$fn")) ||
                syserr(_g("cannot read link %s"), "$new/$fn");
            defined(my $n2 = readlink("$old/$fn")) ||
                syserr(_g("cannot read link %s"), "$old/$fn");
            unless ($n eq $n2) {
                $self->_fail_not_same_type("$old/$fn", "$new/$fn");
            }
        } elsif (-f _) {
            my $old_file = "$old/$fn";
            if (not lstat("$old/$fn")) {
                $! == ENOENT ||
                    syserr(_g("cannot stat file %s"), "$old/$fn");
                $old_file = '/dev/null';
            } elsif (not -f _) {
                $self->_fail_not_same_type("$old/$fn", "$new/$fn");
                return;
            }

            my $label_old = "$basedir.orig/$fn";
            if ($opts{'use_dev_null'}) {
                $label_old = $old_file if $old_file eq '/dev/null';
            }
            my $success = $self->add_diff_file($old_file, "$new/$fn",
                label_old => $label_old,
                label_new => "$basedir/$fn",
                %opts);

            if ($success and ($old_file eq "/dev/null")) {
                if (not $size) {
                    warning(_g("newly created empty file '%s' will not " .
                               "be represented in diff"), $fn);
                } else {
                    if ($mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                        warning(_g("executable mode %04o of '%s' will " .
                                   "not be represented in diff"), $mode, $fn)
                            unless $fn eq 'debian/rules';
                    }
                    if ($mode & (S_ISUID | S_ISGID | S_ISVTX)) {
                        warning(_g("special mode %04o of '%s' will not " .
                                   "be represented in diff"), $mode, $fn);
                    }
                }
            }
        } elsif (-p _) {
            unless (-p "$old/$fn") {
                $self->_fail_not_same_type("$old/$fn", "$new/$fn");
            }
        } elsif (-b _ || -c _ || -S _) {
            $self->_fail_with_msg("$new/$fn",
                _g("device or socket is not allowed"));
        } elsif (-d _) {
            if (not lstat("$old/$fn")) {
                $! == ENOENT ||
                    syserr(_g("cannot stat file %s"), "$old/$fn");
            } elsif (not -d _) {
                $self->_fail_not_same_type("$old/$fn", "$new/$fn");
            }
        } else {
            $self->_fail_with_msg("$new/$fn", _g("unknown file type"));
        }
    };
    my $scan_old = sub {
        my $fn = File::Spec->abs2rel($_, $old);
        return if &$diff_ignore($fn);
        return if $files_in_new{$fn};
        lstat("$old/$fn") || syserr(_g("cannot stat file %s"), "$old/$fn");
        if (-f _) {
            if ($inc_removal) {
                $self->add_diff_file("$old/$fn", "/dev/null",
                    label_old => "$basedir.orig/$fn",
                    label_new => "/dev/null",
                    %opts);
            } else {
                warning(_g("ignoring deletion of file %s"), $fn);
            }
        } elsif (-d _) {
            warning(_g("ignoring deletion of directory %s"), $fn);
        } elsif (-l _) {
            warning(_g("ignoring deletion of symlink %s"), $fn);
        } else {
            $self->_fail_not_same_type("$old/$fn", "$new/$fn");
        }
    };

    find({ wanted => $scan_new, no_chdir => 1 }, $new);
    find({ wanted => $scan_old, no_chdir => 1 }, $old);
}

sub finish {
    my ($self) = @_;
    close($self->{'handle'}) ||
            syserr(_g("cannot close %s"), $self->get_filename());
    delete $self->{'handle'};
    $self->cleanup_after_open();
    return not $self->{'errors'};
}

sub register_error {
    my ($self) = @_;
    $self->{'errors'}++;
}
sub _fail_with_msg {
    my ($self, $file, $msg) = @_;
    errormsg(_g("cannot represent change to %s: %s"), $file, $msg);
    $self->register_error();
}
sub _fail_not_same_type {
    my ($self, $old, $new) = @_;
    my $old_type = get_type($old);
    my $new_type = get_type($new);
    errormsg(_g("cannot represent change to %s:"), $new);
    errormsg(_g("  new version is %s"), $new_type);
    errormsg(_g("  old version is %s"), $old_type);
    $self->register_error();
}

# check diff for sanity, find directories to create as a side effect
sub analyze {
    my ($self, $destdir, %opts) = @_;

    my $diff = $self->get_filename();
    my $diff_handle = $self->open_for_read();
    my %filepatched;
    my %dirtocreate;
    my $diff_count = 0;

    sub getline {
        my $handle = shift;
        my $line = <$handle>;
        if (defined $line) {
            # Strip end-of-line chars
            chomp($line);
            $line =~ s/\r$//;
        }
        return $line;
    }
    sub strip_ts { # Strip timestamp
        my $header = shift;
        # Tab is the official separator, it's always used when
        # filename contain spaces. Try it first, otherwise strip on space
        # if there's no tab
        $header =~ s/\s.*// unless ($header =~ s/\t.*//);
        return $header;
    }
    $_ = getline($diff_handle);

  HUNK:
    while (defined($_) || not eof($diff_handle)) {
	my ($fn, $fn2);
	# skip comments leading up to patch (if any)
	until (/^--- /) {
	    last HUNK if not defined($_ = getline($diff_handle));
	}
	$diff_count++;
	# read file header (---/+++ pair)
	unless(s/^--- //) {
	    error(_g("expected ^--- in line %d of diff `%s'"), $., $diff);
	}
        $_ = strip_ts($_);
        if ($_ eq '/dev/null' or s{^[^/]+/}{$destdir/}) {
            $fn = $_;
	    error(_g("%s contains an insecure path: %s"), $diff, $_) if m{/\.\./};
        }
	if (/\.dpkg-orig$/) {
	    error(_g("diff `%s' patches file with name ending .dpkg-orig"), $diff);
	}

	unless (defined($_ = getline($diff_handle))) {
	    error(_g("diff `%s' finishes in middle of ---/+++ (line %d)"), $diff, $.);
	}
	unless (s/^\+\+\+ //) {
	    error(_g("line after --- isn't as expected in diff `%s' (line %d)"), $diff, $.);
	}
        $_ = strip_ts($_);
        if ($_ eq '/dev/null' or s{^[^/]+/}{$destdir/}) {
            $fn2 = $_;
	    error(_g("%s contains an insecure path: %s"), $diff, $_) if m{/\.\./};
        } else {
            unless (defined $fn) {
                error(_g("none of the filenames in ---/+++ are relative in diff `%s' (line %d)"),
                      $diff, $.);
            }
        }

        if (defined($fn) and $fn eq '/dev/null') {
            error(_g("original and modified files are /dev/null in diff `%s' (line %d)"),
                  $diff, $.) if (defined($fn2) and $fn2 eq '/dev/null');
            $fn = $fn2;
        } elsif (defined($fn2) and $fn2 ne '/dev/null') {
            $fn = $fn2 unless defined $fn;
            $fn = $fn2 if ((not -e $fn) and -e $fn2);
        } elsif (defined($fn2) and $fn2 eq '/dev/null') {
            error(_g("file removal without proper filename in diff `%s' (line %d)"),
                  $diff, $. - 1) unless defined $fn;
            warning(_g("diff %s removes a non-existing file %s (line %d)"),
                    $diff, $fn, $.) unless -e $fn;
        }

	my $dirname = $fn;
	if ($dirname =~ s{/[^/]+$}{} && not -d $dirname) {
	    $dirtocreate{$dirname} = 1;
	}

	# Sanity check, refuse to patch through a symlink
	$dirname = $fn;
	while (1) {
	    if (-l $dirname) {
		error(_g("diff %s modifies file %s through a symlink: %s"),
		      $diff, $fn, $dirname);
	    }
	    last unless $dirname =~ s{/[^/]+$}{};
	}

	if (-e $fn and not -f _) {
	    error(_g("diff `%s' patches something which is not a plain file"), $diff);
	}

	if ($filepatched{$fn}) {
	    error(_g("diff `%s' patches file %s twice"), $diff, $fn);
	}
	$filepatched{$fn} = 1;

	# read hunks
	my $hunk = 0;
	while (defined($_ = getline($diff_handle))) {
	    # read hunk header (@@)
	    next if /^\\ No newline/;
	    last unless (/^@@ -\d+(,(\d+))? \+\d+(,(\d+))? @\@( .*)?$/);
	    my ($olines, $nlines) = ($1 ? $2 : 1, $3 ? $4 : 1);
	    # read hunk
	    while ($olines || $nlines) {
		unless (defined($_ = getline($diff_handle))) {
                    if (($olines == $nlines) and ($olines < 3)) {
                        warning(_g("unexpected end of diff `%s'"), $diff);
                        last;
                    } else {
                        error(_g("unexpected end of diff `%s'"), $diff);
                    }
		}
		next if /^\\ No newline/;
		# Check stats
		if    (/^ / || /^$/)  { --$olines; --$nlines; }
		elsif (/^-/)  { --$olines; }
		elsif (/^\+/) { --$nlines; }
		else {
		    error(_g("expected [ +-] at start of line %d of diff `%s'"),
		          $., $diff);
		}
	    }
	    $hunk++;
	}
	unless($hunk) {
	    error(_g("expected ^\@\@ at line %d of diff `%s'"), $., $diff);
	}
    }
    close($diff_handle);
    unless ($diff_count) {
	warning(_g("diff `%s' doesn't contain any patch"), $diff);
    }
    $self->cleanup_after_open();
    $self->{'analysis'}{$destdir}{"dirtocreate"} = \%dirtocreate;
    $self->{'analysis'}{$destdir}{"filepatched"} = \%filepatched;
    return $self->{'analysis'}{$destdir};
}

sub prepare_apply {
    my ($self, $analysis, %opts) = @_;
    if ($opts{"create_dirs"}) {
	foreach my $dir (keys %{$analysis->{'dirtocreate'}}) {
	    eval { mkpath($dir, 0, 0777); };
	    syserr(_g("cannot create directory %s"), $dir) if $@;
	}
    }
}

sub apply {
    my ($self, $destdir, %opts) = @_;
    # Set default values to options
    $opts{"force_timestamp"} = 1 unless exists $opts{"force_timestamp"};
    $opts{"remove_backup"} = 1 unless exists $opts{"remove_backup"};
    $opts{"create_dirs"} = 1 unless exists $opts{"create_dirs"};
    $opts{"options"} ||= [ '-s', '-t', '-F', '0', '-N', '-p1', '-u',
            '-V', 'never', '-g0', '-b', '-z', '.dpkg-orig'];
    $opts{"add_options"} ||= [];
    push @{$opts{"options"}}, @{$opts{"add_options"}};
    # Check the diff and create missing directories
    my $analysis = $self->analyze($destdir, %opts);
    $self->prepare_apply($analysis, %opts);
    # Apply the patch
    my $diff_handle = $self->open_for_read();
    fork_and_exec(
	'exec' => [ 'patch', @{$opts{"options"}} ],
	'chdir' => $destdir,
	'env' => { LC_ALL => 'C', LANG => 'C' },
	'delete_env' => [ 'POSIXLY_CORRECT' ], # ensure expected patch behaviour
	'wait_child' => 1,
	'from_handle' => $diff_handle
    );
    $self->cleanup_after_open();
    # Reset the timestamp of all the patched files
    # and remove .dpkg-orig files
    my $now = $opts{"timestamp"} || time;
    foreach my $fn (keys %{$analysis->{'filepatched'}}) {
	if ($opts{"force_timestamp"}) {
	    utime($now, $now, $fn) || $! == ENOENT ||
		syserr(_g("cannot change timestamp for %s"), $fn);
	}
	if ($opts{"remove_backup"}) {
	    $fn .= ".dpkg-orig";
	    unlink($fn) || syserr(_g("remove patch backup file %s"), $fn);
	}
    }
}

# Verify if check will work...
sub check_apply {
    my ($self, $destdir, %opts) = @_;
    # Set default values to options
    $opts{"create_dirs"} = 1 unless exists $opts{"create_dirs"};
    $opts{"options"} ||= [ '--dry-run', '-s', '-t', '-F', '0', '-N', '-p1', '-u',
            '-V', 'never', '-g0', '-b', '-z', '.dpkg-orig'];
    $opts{"add_options"} ||= [];
    push @{$opts{"options"}}, @{$opts{"add_options"}};
    # Check the diff and create missing directories
    my $analysis = $self->analyze($destdir, %opts);
    $self->prepare_apply($analysis, %opts);
    # Apply the patch
    my $diff_handle = $self->open_for_read();
    my $error;
    my $patch_pid = fork_and_exec(
	'exec' => [ 'patch', @{$opts{"options"}} ],
	'chdir' => $destdir,
	'env' => { LC_ALL => 'C', LANG => 'C' },
	'delete_env' => [ 'POSIXLY_CORRECT' ], # ensure expected patch behaviour
	'from_handle' => $diff_handle,
        'to_file' => '/dev/null',
        'error_to_file' => '/dev/null',
    );
    wait_child($patch_pid, nocheck => 1);
    my $exit = WEXITSTATUS($?);
    subprocerr("patch --dry-run") unless WIFEXITED($?);
    $self->cleanup_after_open();
    return ($exit == 0);
}

# Helper functions
sub get_type {
    my $file = shift;
    if (not lstat($file)) {
        return _g("nonexistent") if $! == ENOENT;
        syserr(_g("cannot stat %s"), $file);
    } else {
        -f _ && return _g("plain file");
        -d _ && return _g("directory");
        -l _ && return sprintf(_g("symlink to %s"), readlink($file));
        -b _ && return _g("block device");
        -c _ && return _g("character device");
        -p _ && return _g("named pipe");
        -S _ && return _g("named socket");
    }
}

1;
# vim: set et sw=4 ts=8
