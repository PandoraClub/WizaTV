# Copyright © 2008-2009 Raphaël Hertzog <hertzog@debian.org>
#
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

package Dpkg::Source::Package::V3::quilt;

use strict;
use warnings;

# Based on wig&pen implementation
use base 'Dpkg::Source::Package::V2';

use Dpkg;
use Dpkg::Gettext;
use Dpkg::ErrorHandling qw(error syserr warning usageerr subprocerr info);
use Dpkg::Source::Patch;
use Dpkg::Source::Functions qw(erasedir);
use Dpkg::IPC;

use POSIX;
use File::Basename;
use File::Spec;
use File::Path;

our $CURRENT_MINOR_VERSION = "0";

sub init_options {
    my ($self) = @_;
    $self->{'options'}{'allow-version-of-quilt-db'} = []
        unless exists $self->{'options'}{'allow-version-of-quilt-db'};

    $self->SUPER::init_options();
}

sub parse_cmdline_option {
    my ($self, $opt) = @_;
    return 1 if $self->SUPER::parse_cmdline_option($opt);
    if ($opt =~ /^--allow-version-of-quilt-db=(.*)$/) {
        push @{$self->{'options'}{'allow-version-of-quilt-db'}}, $1;
        return 1;
    }
    return 0;
}

sub can_build {
    my ($self, $dir) = @_;
    my ($code, $msg) = $self->SUPER::can_build($dir);
    return ($code, $msg) if $code eq 0;
    my $pd = File::Spec->catdir($dir, "debian", "patches");
    if (-e $pd and not -d _) {
        return (0, sprintf(_g("%s should be a directory or non-existing"), $pd));
    }
    my $series_vendor = $self->get_series_file($dir);
    my $series_main = File::Spec->catfile($pd, "series");
    foreach my $series ($series_vendor, $series_main) {
        if (defined($series) and -e $series and not -f _) {
            return (0, sprintf(_g("%s should be a file or non-existing"), $series));
        }
    }
    return (1, "");
}

sub get_autopatch_name {
    my ($self) = @_;
    return "debian-changes-" . $self->{'fields'}{'Version'};
}

sub get_series_file {
    my ($self, $dir) = @_;
    my $pd = File::Spec->catdir($dir, "debian", "patches");
    # TODO: replace "debian" with the current distro name once we have a
    # way to figure it out
    foreach (File::Spec->catfile($pd, "debian.series"),
             File::Spec->catfile($pd, "series")) {
        return $_ if -e $_;
    }
    return undef;
}

sub read_patch_list {
    my ($self, $file, %opts) = @_;
    return () if not defined $file or not -f $file;
    $opts{"warn_options"} = 0 unless defined($opts{"warn_options"});
    $opts{"skip_auto"} = 0 unless defined($opts{"skip_auto"});
    my @patches;
    my $auto_patch = $self->get_autopatch_name();
    open(SERIES, "<" , $file) || syserr(_g("cannot read %s"), $file);
    while(defined($_ = <SERIES>)) {
        chomp; s/^\s+//; s/\s+$//; # Strip leading/trailing spaces
        s/(^|\s+)#.*$//; # Strip comment
        next unless $_;
        if (/^(\S+)\s+(.*)$/) {
            $_ = $1;
            if ($2 ne '-p1') {
                warning(_g("the series file (%s) contains unsupported " .
                           "options ('%s', line %s), dpkg-source might " .
                           "fail when applying patches."),
                        $file, $2, $.) if $opts{"warn_options"};
            }
        }
        next if $opts{"skip_auto"} and $_ eq $auto_patch;
        error(_g("%s contains an insecure path: %s"), $file, $_) if m{(^|/)\.\./};
        push @patches, $_;
    }
    close(SERIES);
    return @patches;
}

sub create_quilt_db {
    my ($self, $dir) = @_;
    my $db_dir = File::Spec->catdir($dir, ".pc");
    if (not -d $db_dir) {
        mkdir $db_dir or syserr(_g("cannot mkdir %s"), $db_dir);
    }
    my $version_file = File::Spec->catfile($db_dir, ".version");
    if (not -e $version_file) {
        open(VERSION, ">", $version_file);
        print VERSION "2\n";
        close(VERSION);
    }
}

sub apply_quilt_patch {
    my ($self, $dir, $patch, %opts) = @_;
    $opts{"verbose"} = 0 unless defined($opts{"verbose"});
    $opts{"timestamp"} = time() unless defined($opts{"timestamp"});
    my $path = File::Spec->catfile($dir, "debian", "patches", $patch);
    my $obj = Dpkg::Source::Patch->new(filename => $path);

    info(_g("applying %s"), $patch) if $opts{"verbose"};
    $obj->apply($dir, timestamp => $opts{"timestamp"},
                force_timestamp => 1, create_dirs => 1, remove_backup => 0,
                options => [ '-s', '-t', '-F', '0', '-N', '-p1', '-u',
                             '-V', 'never', '-g0', '-E', '-b',
                             '-B', ".pc/$patch/" ]);
}

sub get_patches {
    my ($self, $dir, %opts) = @_;
    my $series = $self->get_series_file($dir);
    return $self->read_patch_list($series, %opts);
}

sub apply_patches {
    my ($self, $dir, %opts) = @_;

    if ($opts{'usage'} eq 'unpack') {
        $opts{'verbose'} = 1;
    } elsif ($opts{'usage'} eq 'build') {
        $opts{'warn_options'} = 1;
        $opts{'verbose'} = 0;
    }

    my $patches = $opts{"patches"};

    # Update debian/patches/series symlink if needed to allow quilt usage
    my $series = $self->get_series_file($dir);
    return unless $series; # No series, no patches
    my $basename = basename($series);
    if ($basename ne "series") {
        my $dest = File::Spec->catfile($dir, "debian", "patches", "series");
        unlink($dest) if -l $dest;
        unless (-f _) { # Don't overwrite real files
            symlink($basename, $dest) ||
                syserr(_g("can't create symlink %s"), $dest);
        }
    }

    unless (defined($patches)) {
        $patches = [ $self->get_patches($dir, %opts) ];
    }
    return unless scalar(@$patches);

    # Apply patches
    $self->create_quilt_db($dir);
    my $pc_applied = File::Spec->catfile($dir, ".pc", "applied-patches");
    my @applied = $self->read_patch_list($pc_applied);
    my @patches = $self->read_patch_list($self->get_series_file($dir));
    open(APPLIED, '>>', $pc_applied) || syserr(_g("cannot write %s"), $pc_applied);
    $opts{"timestamp"} = time();
    foreach my $patch (@$patches) {
        $self->apply_quilt_patch($dir, $patch, %opts);
        print APPLIED "$patch\n";
    }
    close(APPLIED);
}

sub prepare_build {
    my ($self, $dir) = @_;
    $self->SUPER::prepare_build($dir);
    # Skip .pc directories of quilt by default and ignore difference
    # on debian/patches/series symlinks and d/p/.dpkg-source-applied
    # stamp file created by ourselves
    my $func = sub {
        return 1 if $_[0] =~ m{^debian/patches/series$} and -l $_[0];
        return 1 if $_[0] =~ /^.pc(\/|$)/;
        return 1 if $_[0] =~ /$self->{'options'}{'diff_ignore_regexp'}/;
        return 0;
    };
    $self->{'diff_options'}{'diff_ignore_func'} = $func;
}

sub do_build {
    my ($self, $dir) = @_;
    my $pc_ver = File::Spec->catfile($dir, ".pc", ".version");
    if (-f $pc_ver) {
        open(VER, "<", $pc_ver) || syserr(_g("cannot read %s"), $pc_ver);
        my $version = <VER>;
        chomp $version;
        close(VER);
        if ($version != 2) {
            if (scalar grep { $version eq $_ }
                @{$self->{'options'}{'allow-version-of-quilt-db'}})
            {
                warning(_g("unsupported version of the quilt metadata: %s"),
                        $version);
            } else {
                error(_g("unsupported version of the quilt metadata: %s"),
                      $version);
            }
        }
    }
    $self->SUPER::do_build($dir);
}

sub check_patches_applied {
    my ($self, $dir) = @_;
    my $pc_applied = File::Spec->catfile($dir, ".pc", "applied-patches");
    my @applied = $self->read_patch_list($pc_applied);
    my @patches = $self->read_patch_list($self->get_series_file($dir));
    my @to_apply;
    foreach my $patch (@patches) {
        next if scalar grep { $_ eq $patch } @applied;
        push @to_apply, $patch;
    }
    if (scalar(@to_apply)) {
        my $first_patch = File::Spec->catfile($dir, "debian", "patches",
                                              $to_apply[0]);
        my $patch_obj = Dpkg::Source::Patch->new(filename => $first_patch);
        if ($patch_obj->check_apply($dir)) {
            warning(_g("patches have not been applied, applying them now " .
                       "(use --no-preparation to override)"));
            $self->apply_patches($dir, usage => 'preparation', verbose => 1,
                                 patches => \@to_apply);
        }
    }
}

sub register_autopatch {
    my ($self, $dir) = @_;

    sub add_line {
        my ($file, $line) = @_;
        open(FILE, ">>", $file) || syserr(_g("cannot write %s"), $file);
        print FILE "$line\n";
        close(FILE);
    }

    sub drop_line {
        my ($file, $re) = @_;
        open(FILE, "<", $file) || syserr(_g("cannot read %s"), $file);
        my @lines = <FILE>;
        close(FILE);
        open(FILE, ">", $file) || syserr(_g("cannot write %s"), $file);
        print(FILE $_) foreach grep { not /^\Q$re\E\s*$/ } @lines;
        close(FILE);
    }

    my $auto_patch = $self->get_autopatch_name();
    my @patches = $self->get_patches($dir);
    my $has_patch = (grep { $_ eq $auto_patch } @patches) ? 1 : 0;
    my $series = $self->get_series_file($dir);
    $series ||= File::Spec->catfile($dir, "debian", "patches", "series");
    my $applied = File::Spec->catfile($dir, ".pc", "applied-patches");
    my $patch = File::Spec->catfile($dir, "debian", "patches", $auto_patch);

    if (-e $patch) {
        $self->create_quilt_db($dir);
        # Add auto_patch to series file
        if (not $has_patch) {
            add_line($series, $auto_patch);
            add_line($applied, $auto_patch);
        }
        # Ensure quilt meta-data are created and in sync with some trickery:
        # reverse-apply the patch, drop .pc/$patch, re-apply it
        # with the correct options to recreate the backup files
        my $patch_obj = Dpkg::Source::Patch->new(filename => $patch);
        $patch_obj->apply($dir, add_options => ['-R', '-E']);
        erasedir(File::Spec->catdir($dir, ".pc", $auto_patch));
        $self->apply_quilt_patch($dir, $auto_patch);
    } else {
        # Remove auto_patch from series
        if ($has_patch) {
            drop_line($series, $auto_patch);
            drop_line($applied, $auto_patch);
            erasedir(File::Spec->catdir($dir, ".pc", $auto_patch));
        }
        # Clean up empty series
        unlink($series) if not -s $series;
    }
}

# vim:et:sw=4:ts=8
1;
