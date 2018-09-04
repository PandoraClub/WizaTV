# _DPKG_ARCHITECTURE([DEB_VAR], [sh_var])
# ---------------------------------------
# Use dpkg-architecture from the source tree to set sh_var using DEB_VAR for
# the target architecture, to avoid duplicating its logic.
AC_DEFUN([_DPKG_ARCHITECTURE], [
AC_REQUIRE([AC_CANONICAL_SYSTEM])dnl
$2=`cd $srcdir/scripts; \
    PERL5LIB=$(pwd) ./dpkg-architecture.pl -t$target -q$1 2>/dev/null`
])# _DPKG_ARCHITECURE

# DPKG_CPU_TYPE
# -------------
# Parse the target cpu name and check it against the cputable to determine
# the Debian name for it.  Sets ARCHITECTURE_CPU.
AC_DEFUN([DPKG_CPU_TYPE],
[AC_MSG_CHECKING([dpkg cpu type])
_DPKG_ARCHITECTURE([DEB_HOST_ARCH_CPU], [cpu_type])
if test "x$cpu_type" = "x"; then
	cpu_type=$target_cpu
	AC_MSG_RESULT([$cpu_type])
	AC_MSG_WARN([$target_cpu not found in cputable])
else
	AC_MSG_RESULT([$cpu_type])
fi
AC_DEFINE_UNQUOTED(ARCHITECTURE_CPU, "${cpu_type}",
	[Set this to the canonical dpkg CPU name.])
])# DPKG_CPU_TYPE

# DPKG_OS_TYPE
# ------------
# Parse the target operating system name and check it against a list of
# special cases to determine what type it is.  Sets ARCHITECTURE_OS.
AC_DEFUN([DPKG_OS_TYPE],
[AC_MSG_CHECKING([dpkg operating system type])
_DPKG_ARCHITECTURE([DEB_HOST_ARCH_OS], [os_type])
if test "x$os_type" = "x"; then
	os_type=$target_os
	AC_MSG_RESULT([$os_type])
	AC_MSG_WARN([$target_os not found in ostable])
else
	AC_MSG_RESULT([$os_type])
fi
AC_DEFINE_UNQUOTED(ARCHITECTURE_OS, "${os_type}",
	[Set this to the canonical dpkg system name.])
])# DPKG_OS_TYPE

# DPKG_ARCHITECTURE
# ------------------------
# Determine the Debian name for the target operating system,
# sets ARCHITECTURE.
AC_DEFUN([DPKG_ARCHITECTURE],
[DPKG_CPU_TYPE
DPKG_OS_TYPE
AC_MSG_CHECKING([dpkg architecture name])
_DPKG_ARCHITECTURE([DEB_HOST_ARCH], [dpkg_arch])
AC_MSG_RESULT([$dpkg_arch])
AC_DEFINE_UNQUOTED(ARCHITECTURE, "${dpkg_arch}",
	[Set this to the canonical dpkg architecture name.])
])# DPKG_ARCHITECTURE
