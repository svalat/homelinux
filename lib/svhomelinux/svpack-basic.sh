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
PACKAGE_SLOT='0'

######################### SECTION ############################
export CFLAGS
export CXXFLAGS

######################### SECTION ############################
function do_check_vars()
{
	if [ -z "$PACKAGE_NAME" ]; then pack_var_required PACKAGE_NAME; fi
	if [ -z "$PACKAGE_VERSION" ]; then pack_var_required PACKAGE_VERSION; fi
	if [ -z "$PACKAGE_ARCHIVE" ]; then pack_var_required PACKAGE_ARCHIVE; fi
}

######################### SECTION ############################
function do_download()
{
	for url in $(get_pack_urls "${PACKAGE_ARCHIVE}")
	do
		if [ ! -f "${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE}" ]; then
			echo ">>> Downloading ${PACKAGE_ARCHIVE} <<<"
			wget "$url" -O ${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE} || rm ${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE}
		fi
	done

	if [ ! -f "${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE}" ]; then
		echo "Error: can't download archive file"
		exit 1;
	fi
}

######################### SECTION ############################
function do_extract()
{
	case "$PACKAGE_ARCHIVE" in
		*.zip)
			safe_exec unzip "${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE}"
			;;
		*.tar.xz)
			echo "xz -d -c \"${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE}\" | tar -x"
			xz -d -c "${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE}" | tar -x || exit 1
			;;
		*.tar.gz)
			safe_exec tar -xzf "${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE}"
			;;
		*.tgz)
			safe_exec tar -xzf "${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE}"
			;;
		*.tar.bz2|*.tbz2)
			safe_exec tar -xjf "${SV_HOME_LINUX_SHARED}/distfiles/${PACKAGE_ARCHIVE}"
			;;
		*)
			echo "Error: unsupported format $PACKAGE_ARCHIVE"
			exit 1
			;;
	esac
}

######################### SECTION ############################
function  do_move_to_build_dir()
{
	echo ">>> Moving to subdir ${PACKAGE_SUBDIR} <<<"
	safe_exec cd "${PACKAGE_SUBDIR}"
}

######################### SECTION ############################
function do_configure()
{
	if [ -z "$PACKAGE_VARIANT" ]; then
		safe_exec svconfigure ${PACKAGE_CONFIGURE_OPTIONS}
	else
		if [ "${PACKAGE_VARIANT}" = "noldpath" ]; then
			safe_exec svconfigure --variant="${PACKAGE_VARIANT}" ${PACKAGE_CONFIGURE_OPTIONS}
		elif [ "${PACKAGE_VARIANT}" = "${PACKAGE_NAME}" ]; then
			safe_exec svconfigure --variant="${PACKAGE_VARIANT}-${PACKAGE_VERSION}" ${PACKAGE_CONFIGURE_OPTIONS}
		else
			safe_exec svconfigure --variant="${PACKAGE_VARIANT}-${PACKAGE_NAME}-${PACKAGE_VERSION}" ${PACKAGE_CONFIGURE_OPTIONS}
		fi
	fi
}

######################### SECTION ############################
function do_test()
{
	if [ ! -z "${PACKAGE_TEST_TARGET}" ]; then
		echo ">>> Execute tests <<<"
		current="$PWD"
		if [ -d svbuild ]; then safe_exec cd svbuild ; fi
		safe_exec make ${PACKAGE_TEST_TARGET} ${MAKE_OPTS}
		safe_exec cd "$current"
	fi
}

######################### SECTION ############################
function do_pre_install()
{
	echo "Nothing to to at pre-install"
}

######################### SECTION ############################
function do_make()
{
	if [ -d svbuild ]; then safe_exec cd svbuild ; fi
	safe_exec make ${MAKE_OPTS}
}

######################### SECTION ############################
function do_install()
{
	if [ -z "$PACKAGE_VARIANT" ]; then PACKAGE_VARIANT=none; fi
	safe_exec svmakeinstall "$PACKAGE_NAME" "$PACKAGE_VERSION" "$PACKAGE_VARIANT" "$PACKAGE_SLOT"
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
