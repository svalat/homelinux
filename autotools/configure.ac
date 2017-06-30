######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
AC_PREREQ([2.69])
AC_INIT([homelinux], [2.0.0-dev], [sebastien.valat-dev@orange.fr])
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([src/config.h:src/config-autotools.h.in])
AM_SILENT_RULES([yes])

######################################################
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

######################################################
# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h string.h unistd.h dirent.h sys/types.h sys/stat.h])

######################################################
# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

######################################################
#check for c++11
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

######################################################
# Checks for library functions.
AC_CHECK_FUNCS([system readdir fopen fwrite fread fseek ftell fclose])

######################################################
#set version
AC_SUBST(HOMELINUX_VERSION,[\\\"$VERSION\\\"])
AH_TEMPLATE(HOMELINUX_VERSION,[Give homelinux version])

######################################################
AC_ARG_ENABLE(colors, AS_HELP_STRING([--disable-colors], [Enable of distble usage of colors in output]), 
                      [enable_colors = $enableval], 
					  [enable_colors = yes])
AM_CONDITIONAL([ENABLE_COLORS], [test x$enable_colors = xyes])
AM_COND_IF([ENABLE_COLORS],
           [AC_DEFINE([ENABLE_COLORS],[1],[Enable or disable usage of colors])])

######################################################
AC_CONFIG_FILES([Makefile 
                 manpages/Makefile
                 src/Makefile
				 src/base/Makefile
				 src/core/Makefile
				 src/crawlers/Makefile
				 src/portability/Makefile
				 src/providers/Makefile
				 extern-deps/jsoncpp/dist/Makefile
				 extern-deps/re2-2017-06-01/Makefile
				 extern-deps/re2-2017-06-01/re2/Makefile
				 extern-deps/re2-2017-06-01/util/Makefile
				])

######################################################
#AC_CONFIG_SUBDIRS([extern-deps/gmock-1.7.0])

######################################################
AC_OUTPUT
