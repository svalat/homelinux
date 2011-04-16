#!/bin/bash
##############################################################
#                                                            #
#         PROJECT : svHomeLinux                              #
#         VERSION : 1.1                                      #
#         DATE    : jeu. févr.  3 18:53:24 CET 2011          #
#         AUTHORS : Sébastien Valat                          #
#         LICENCE : GPLv2                                    #
#                                                            #
##############################################################

######################### SECTION ############################
PACKAGE_NAME=''
PACKAGE_VERSION=''
PACKAGE_ARCHIVE=''
PACKAGE_SUBDIR=''
PACKAGE_VARIANT=''
PACKAGE_CONFIGURE_OPTIONS=''
PACKAGE_TEST_TARGET=''

######################### SECTION ############################
function do_check_vars()
{
	if [ -z "$PACKAGE_NAME" ]; then pack_var_required PACKAGE_NAME; fi
	if [ -z "$PACKAGE_VERSION" ]; then pack_var_required PACKAGE_VERSION; fi
}

######################### SECTION ############################
function do_download()
{
	echo "Nothing to do at download"
}

######################### SECTION ############################
function do_extract()
{
	echo "Nothing to do at extract"
}

######################### SECTION ############################
function  do_move_to_build_dir()
{
	echo "Nothing to do at build dir"
}

######################### SECTION ############################
function do_patch()
{
	echo "Nothing to do at pre configure"
}

######################### SECTION ############################
function do_configure()
{
	echo "Nothing to do at configure"
}

######################### SECTION ############################
function do_make()
{
	echo "Nothing to do at make"
}

######################### SECTION ############################
function do_test()
{
 echo "Nothing to do at test"
}

######################### SECTION ############################
function do_pre_install()
{
	echo "Nothing to to at pre-install"
}

######################### SECTION ############################
function do_install()
{
	echo "Nothing to do at install"
}

######################### SECTION ############################
function do_post_install()
{
	echo "Nothing to to at post-install"
}

######################### SECTION ############################
function do_clean()
{
	echo "Nothing to do"
}


