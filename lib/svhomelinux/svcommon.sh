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
SV_HOME_LINUX_SHARED="${PREFIX}/share/svhomelinux"
SV_HOME_LINUX_INSTALLED="${SV_HOME_LINUX_SHARED}/installed"
SV_HOME_LINUX_SHARED_NP="./share/svhomelinux"
SV_HOME_LINUX_INSTALLED_NP="${SV_HOME_LINUX_SHARED_NP}/installed"
SV_HOME_LINUX_DISTFILES="${SV_HOME_LINUX_SHARED}/distfiles"
SV_HOME_LINUX_PACKAGES="${SV_HOME_LINUX_SHARED}/packages"
SV_HOME_LINUX_ENV_DIR="${SV_HOME_LINUX_SHARED}/env"
SV_HOME_LINUX_QUICK_PACKAGES="${SV_HOME_LINUX_SHARED}/quickpackages"
SV_HOME_LINUX_VARIANTES="$PREFIX/variants"

######################### FUNCTION ###########################
function safe_exec()
{
	echo "$*"
	$* || exit 1
}

######################### FUNCTION ###########################
#Check if the package is alreay installed and do error if true
#Params :
#    $1 : package name (or package-version)
#    $2 : package version
function error_if_package_is_installed()
{
	echo ">>> Check if already installed <<<"
	
	if [ -f "${SV_HOME_LINUX_INSTALLED}/${1}.flist.gz" ]
	then
		echo "ERROR : ${pname} alredy installed, please remove first"
		exit 1
	fi

	if [ ! -z "$2" ] && [ -f "${SV_HOME_LINUX_INSTALLED}/${1}-${2}.flist.gz" ]
	then
		echo "ERROR : ${pname} alredy installed, please remove first"
		exit 1
	fi
}

######################### FUNCTION ###########################
# Return the value of arguement considering the format --argname=value
# Params:
#   - complete string
#   - argname
function extract_argument()
{
	echo "$1" | sed -e "s/--$2=//g"
}

######################### FUNCTION ###########################
# Provide a Yes/No question an return 0 for yes, 1 for no
# Params:
#    $1 : Define the question to ask
#    $2 : Define the default answer ('yes' or 'no')
function yes_no_question()
{
	res=""
	tmp=''
	while [ -z "$res" ]; do
		if [ "$2" ]; then
			printf "$1 ? (Y/n) "
		else
			printf "$1 ? (y/N) "
		fi
		read tmp
		if [ -z "$tmp" ]; then
			res="$2"
		elif [ "$tmp" = "y" ] || [ "$tmp" = "Y" ]; then
			res='yes'
		elif [ "$tmp" = 'n' ] || [ "$tmp" = "N" ]; then
			res='no'
		fi
	done

	if  [ "$res" = "yes" ]; then
		return 0
	else
		return 1
	fi
}

######################### SECTION ############################
function yumCheckIfInstall()
{
	if [ -f ${SV_HOME_LINUX_INSTALLED}/${1}.flist.gz ] || [ `ls ${SV_HOME_LINUX_INSTALLED}/${1}-*.flist.gz 2>/dev/null | wc -l` -eq 0 ]; then
		return 1
	else
		return 0
	fi
}

######################### FUNCTION ###########################
# Definit un héritage d'une librairie de base
# Params :
#     $1 : lib name (withour the .sh)
function inherit()
{
	#check config file
	if [ ! -f ${PREFIX}/lib/svhomelinux/$1.sh ]; then
		echo "Error: Can't find ${PREFIX}/lib/svhomelinux/$1.sh file"
		exit 1
	fi
	source ${PREFIX}/lib/svhomelinux/$1.sh || exit 1
}

######################### SECTION ############################
#{name}
function pack_var_required()
{
	echo "The package require the definition of variable $1"
	exit 1
}

######################### SECTION ############################
#{name}
function yumFindLastVersion()
{
	res="`zcat ${SV_HOME_LINUX_SHARED}/distfiles.list.gz | egrep \"^[0-9]+\. .*/${1}-[0-9\.]+(\.src)?\.(tar|zip|tgz|xgz)\" | cut -f 2 -d ' ' | tail -n 1`"
	if [ -z "$res" ]; then
		echo "Package not found $1" 1>&2
		exit 1
	fi
	basename "$res" | sed -e "s/${1}-/${1} /g" -e "s/\.tar\.bz2//g" -e "s/\.tar\.gz//g" -e "s/\.zip//g" -e "s/\.tgz//g" -e "s/\.tar.xz//g"
}

######################### SECTION ############################
#{name} {version}
function yumFindFileName()
{
	res="`zcat ${SV_HOME_LINUX_SHARED}/distfiles.list.gz | egrep \"^[0-9]+\. .*/${1}-${2}\" | cut -f 2 -d ' ' | tail -n 1`"
	basename "$res"
}

######################### SECTION ############################
#{nb args to skip} {args} ....
function skip_args()
{
	id=0
	for tmp in $*
	do
		if [ $id -gt $1 ]
		then
			echo $tmp
		fi
		id=$(( $id +1 ))
	done
}
