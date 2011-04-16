#!/bin/bash
##############################################################
#                                                            #
#         PROJECT : svHomeLinux                              #
#         VERSION : 1.1                                      #
#         DATE    : jeu. févr.  3 18:53:24 CET 2011          #
#         AUTHORS : Sébastien Valat                         #
#         LICENCE : GPLv2                                    #
#                                                            #
##############################################################

######################### SECTION ############################
function do_download()
{
	if [ -z "$PACKAGE_GIT_URL" ]; then pack_var_required PACKAGE_GIT_URL; fi
	if [ -z "$PACKAGE_GIT_BRANCH" ]; then pack_var_required PACKAGE_GIT_BRANCH; fi

	if [ ! -d "${SV_HOME_LINUX_DISTFILES}/git-repos" ]; then
		safe_exec mkdir  ${SV_HOME_LINUX_DISTFILES}/git-repos
	fi
	safe_exec cd  ${SV_HOME_LINUX_DISTFILES}/git-repos
	if [ ! -d ${PACKAGE_NAME} ]; then
		safe_exec git clone "${PACKAGE_GIT_URL}" "${PACKAGE_NAME}"
		safe_exec cd "${PACKAGE_NAME}"
	else
		safe_exec cd "${PACKAGE_NAME}"
		echo git pull
		git pull
	fi
	case ${PACKAGE_ARCHIVE} in
		*.tar.gz)
			git archive --prefix="${PACKAGE_SUBDIR}/" --format=tar "${PACKAGE_GIT_BRANCH}" | gzip > ${SV_HOME_LINUX_DISTFILES}/${PACKAGE_ARCHIVE} || exit 1
			;;
		*.tar.bz2)
			git archive --prefix="${PACKAGE_SUBDIR}/" --format=tar "${PACKAGE_GIT_BRANCH}" | bzip2 > ${SV_HOME_LINUX_DISTFILES}/${PACKAGE_ARCHIVE} || exit 1
			;;
		*)
			echo "Unsupported archive format for generation from git repository : $PACKAGE_ARVHICE" 1>&2
			exit 1
			;;
	esac
}
