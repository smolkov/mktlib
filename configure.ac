#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])


dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************

m4_define(mkt_configure_ac)

#
# The following version number definitions
# Making releases:
#   mktlib_micro_version += 1;
#   mktlib_interface_age += 1;
#   mktlib_binary_age += 1;
# if any functions have been added, set interface_age to 0.
# if backwards compatibility has been broken,
# set binary_age _and_ interface_age to 0.
#
# in easier to understand terms:
#
# <mclasen> on the stable branch, interface age == micro
# <mclasen> on the unstable (ie master), interface age = 0

m4_define([mktlib_major_version], [2])
m4_define([mktlib_minor_version], [3])
m4_define([mktlib_micro_version], [0])
m4_define([mktlib_interface_age], [0])
m4_define([mktlib_binary_age],
          [m4_eval(100 * mktlib_minor_version + mktlib_micro_version)])
m4_define([mktlib_version],
          [mktlib_major_version.mktlib_minor_version.mktlib_micro_version])

# libtool version related macros
m4_define([mktlib_lt_release], [mktlib_major_version.mktlib_minor_version])
m4_define([mktlib_lt_current],
          [m4_eval(100 * mktlib_minor_version + mktlib_micro_version - mktlib_interface_age)])
m4_define([mktlib_lt_revision], [mktlib_interface_age])
m4_define([mktlib_lt_age], [m4_eval(mktlib_binary_age - mktlib_interface_age)])
m4_define([mktlib_lt_current_minus_age],
          [m4_eval(mktlib_lt_current - mktlib_lt_age)])

# if the minor version number is odd, then we want debugging.  Otherwise
# we only want minimal debugging support.
m4_define([mktlib_debug_default],
          [m4_if(m4_eval(mktlib_minor_version % 2), [1], [yes], [minimum])])dnl


AC_INIT(mktlib, [mktlib_version],
        [http://lar.com/enter_bug.cgi?product=mktlib])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/mktlib.h])
AC_CONFIG_MACRO_DIR([m4macros])

# Save this value here, since automake will set cflags later
cflags_set=${CFLAGS:+set}

AM_INIT_AUTOMAKE([1.11 -Wno-portability no-define no-dist-gzip dist-xz tar-ustar])
AM_MAINTAINER_MODE([enable])

# Support silent build rules. Disable
# by either passing --disable-silent-rules to configure or passing V=1
# to make
AM_SILENT_RULES([yes])

# libtool versioning
LT_RELEASE=mktlib_lt_release
LT_CURRENT=mktlib_lt_current
LT_REVISION=mktlib_lt_revision
LT_AGE=mktlib_lt_age
LT_CURRENT_MINUS_AGE=mktlib_lt_current_minus_age
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)
AC_SUBST(LT_CURRENT_MINUS_AGE)

MKTLIB_MAJOR_VERSION=mktlib_major_version
MKTLIB_MINOR_VERSION=mktlib_minor_version
MKTLIB_MICRO_VERSION=mktlib_micro_version
MKTLIB_INTERFACE_AGE=mktlib_interface_age
MKTLIB_BINARY_AGE=mktlib_binary_age
MKTLIB_VERSION=mktlib_version

AC_SUBST(MKTLIB_MAJOR_VERSION)
AC_SUBST(MKTLIB_MINOR_VERSION)
AC_SUBST(MKTLIB_MICRO_VERSION)
AC_SUBST(MKTLIB_VERSION)
AC_SUBST(MKTLIB_INTERFACE_AGE)
AC_SUBST(MKTLIB_BINARY_AGE)

AC_DEFINE(MKTLIB_MAJOR_VERSION, [mktlib_major_version],
	  [Define to the MKTLIB major version])
AC_DEFINE(MKTLIB_MINOR_VERSION, [mktlib__minor_version],
	  [Define to the MKTLIB minor version])
AC_DEFINE(MKTLIB_MICRO_VERSION, [mktlib__micro_version],
	  [Define to the MKTLIB micro version])
AC_DEFINE(MKTLIB_INTERFACE_AGE, [mktlib__interface_age],
	  [Define to the MKTLIB interface age])
AC_DEFINE(MKTLIB_BINARY_AGE, [mktlib__binary_age],
	  [Define to the MKTLIB binary age])



dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_USE_SYSTEM_EXTENSIONS

AM_CONDITIONAL(HAVE_GCC, [test "$GCC" = "yes"])

AC_CANONICAL_HOST

dnl

dnl Check for a working C++ compiler, but do not bail out, if none is found.
AC_CHECK_TOOLS(CXX, [$CCC c++ g++ gcc CC cxx cc++ cl], [gcc])
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE(,[class a { int b; } c;], ,CXX=)
AM_CONDITIONAL(HAVE_CXX, [test "$CXX" != ""])
AC_LANG_RESTORE

AM_PROG_CC_C_O
AC_PROG_INSTALL

PKG_PROG_PKG_CONFIG(0.20)

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.40.0 libssl libcrypto gobject-2.0 gio-2.0 gmodule-2.0 json-glib-1.0 libsoup-2.4 hiredis])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(DB, [sqlite3 >= 3.8.0 ])
AC_SUBST(DB_CFLAGS)
AC_SUBST(DB_LIBS)


ALL_LINGUAS="`grep -v '^#' "$srcdir/po/LINGUAS" | tr '\n' ' '`"
AC_SUBST([CONFIG_STATUS_DEPENDENCIES],['$(top_srcdir)/po/LINGUAS'])

GETTEXT_PACKAGE=mktlib
AC_SUBST([GETTEXT_PACKAGE], [mktlib])
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, ["mktlib"],
  [Define the gettext package to be used])


dnl Initialize libtool
LT_PREREQ([2.2])
LT_INIT([disable-static win32-dll])


AM_GLIB_GNU_GETTEXT

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)
AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal)
AC_PATH_PROG(GLIB_MKENUMS, glib-mkenums)


dnl Compiler flags; macro originates from systemkt
dnl See https://bugzilla.gnome.org/show_bug.cgi?id=608953
AC_ARG_ENABLE(compile-warnings,[AS_HELP_STRING([--disable-compile-warnings],[Don't use builtin compiler warnings])],,enable_compile_warnings=yes)

EXTRA_WARN_CFLAGS="-Wall"
# -Wstrict-prototypes -Werror=declaration-after-statement -Werror=missing-prototypes -Werror=implicit-function-declaration \
#  -Werror=pointer-arith -Werror=init-self -Werror=format-security \
#  -Werror=format=2 -Werror=missing-include-dirs"

if test "x$enable_compile_warnings" = xyes; then
	WARN_CFLAGS="$EXTRA_WARN_CFLAGS"
fi

AC_SUBST(WARN_CFLAGS)

GLIB_GSETTINGS


AC_PREFIX_DEFAULT("/usr")


AC_CONFIG_FILES([Makefile
					src/mktlib-2.0.pc
					src/Makefile
					po/Makefile.in
					])

AC_OUTPUT()
