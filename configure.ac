dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(Scratch3-splash, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CXX

PKG_CHECK_MODULES([GTKMM], [gtkmm-3.0 >= 3.24.0])
PKG_CHECK_MODULES([CURL], [libcurl >= 7.64.0])

s3splashdatadir="$datadir/$PACKAGE/"

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

