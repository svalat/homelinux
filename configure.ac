#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([homelinux], [2.0.0-dev], [sebastien.valat-dev@orange.fr])
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h string.h unistd.h dirent.h sys/types.h sys/stat.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

#check for c++11
AX_CXX_COMPILE_STDCXX_11()

# Checks for library functions.
AC_CHECK_FUNCS([system readdir fopen fwrite fread fseek ftell fclose])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_CONFIG_SUBDIRS([extern-deps/gmock-1.7.0])
AC_OUTPUT
