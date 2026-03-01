# Autoconf Requirements
# Initialize
AC_INIT([truffle], [0.1], [hello@chrisphoenix.dev])
# Force autoconf to be at least this version number:
AC_PREREQ([2.68])
# Safety check - list a source file that wouldn't be in another directory
AC_CONFIG_SRCDIR([src/main.c])
# Put autotools auxiliary files in a subdir
AC_CONFIG_AUX_DIR([build-aux])

# Enable 'automake'
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])
AC_CONFIG_FILES([Makefile])
AM_MAINTAINER_MODE([enable])

# Look for a C compiler
AC_PROG_CC

# Check for dependencies
PKG_CHECK_MODULES([gl_libs], [gl, glfw3 >= 3.0])

# Set up config header
AC_CONFIG_HEADERS([src/config.h])

AC_OUTPUT
