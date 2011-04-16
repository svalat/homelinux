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
#    $3 : package slot (0 per default)
function error_if_package_is_installed()
{
	echo ">>> Check if already installed <<<"
	if yumCheckIfInstall "${1}" "${3}" "${2}"; then
		echo "ERROR : ${1} alredy installed, please remove first" 1>&2
		exit 1
	fi
}

######################### SECTION ############################
#{name} {version} [slot]
function markVersionAsInstalled()
{
	if [ -z "$3" ]; then
		s=0
	else
		s=$3
	fi

	echo $2 > ${SV_HOME_LINUX_INSTALLED}/${1}-slot-${s}.version
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
#[env config]
function getInheritedPrefix()
{
	if [ -z "$1" ]; then
		prefix_conf=$HOME/.svconfig
		subcfg=${INHERIT_SVENV}
	else
		prefix_conf=$1
		subcfg=""
		bash -c "$(cat $prefix_conf); echo \$PREFIX"
		subcfg=$(bash -c "$(cat $prefix_conf); echo \$INHERIT_SVENV")
	fi
	#echo "DEBUG : subcfg = $subcfg" 1>&2
	for cfg in $subcfg
	do
		getInheritedPrefix $cfg
	done
}

######################### SECTION ############################
#{name}
function packageMustIgnoreInheritance()
{
	if [ -z "$(echo " ${IGNORE_INHERIT_PACKAGES} " | grep " ${1} ")" ]; then
		return 1
	else
		return 0
	fi
}

######################### SECTION ############################
#{name} {slot} [version]
function yumCheckIfInstall()
{
	#calc slot
	if [ -z "$2" ]; then s=0; else s=$2; fi

	if packageMustIgnoreInheritance "$1"
	then
		plist="${PREFIX}"
	else
		plist="${PREFIX} $(getInheritedPrefix)"
	fi
	
	#echo "DEBUG : Prefixes to check : $plist" 1>&2
	
	for prefix in $plist
	do
		if [ -f "${prefix}/${SV_HOME_LINUX_INSTALLED_NP}/${1}-slot-${s}.version" ]; then return 0; fi
		if [ ! -z "$3" ] && [ -f "${prefix}/${SV_HOME_LINUX_INSTALLED_NP}/${1}-${3}.flist.gz" ]; then return 0; fi
	done

	return 1
}

######################### FUNCTION ###########################
# Definit un héritage d'une librairie de base
# Params :
#     $1 : lib name (withour the .sh)
function inherit()
{
	#check config file
	if [ ! -f ${PREFIX}/lib/svhomelinux/$1.sh ]; then
		echo "Error: Can't find ${PREFIX}/lib/svhomelinux/$1.sh file" 1>&2
		exit 1
	fi
	source ${PREFIX}/lib/svhomelinux/$1.sh || exit 1
}

######################### SECTION ############################
#{name}
function pack_var_required()
{
	echo "The package require the definition of variable $1" 1>&2
	exit 1
}

######################### SECTION ############################
#{name} [slot]
function yumGetVersionRegexp()
{
	n=$(echo $1 | sed -e "s/\+/\\+/g")
	if [ -z "$2" ]; then
		res=$(grep "^$n " "${SV_HOME_LINUX_SHARED}/quickpackages.version" | cut -f 2 -d ' ')
	else
		res=$(grep "^$n .* $2$" ${SV_HOME_LINUX_SHARED}/quickpackages.slots | cut -f 2 -d ' ')
	fi
	if [ -z "$res" ]; then
		echo '[0-9\.]+'
	else
		echo "$res" | sed -e 's/\./\\./g'
	fi
}

######################### SECTION ############################
#{name}
function yumGetVersionSeparator()
{
	n=$(echo $1 | sed -e "s/\+/\\\\+/g")
	res=$(cat "${SV_HOME_LINUX_SHARED}/quickpackages.version" | grep "^$n " | cut -f 3 -d ' ')
	if [ -z "$res" ]; then
		echo '-'
	else
		echo "$res"
	fi
}

######################### SECTION ############################
#{name} [regexp]
function yumFindLastVersion()
{
	sep=$(yumGetVersionSeparator "$1")
	if [ -z "$2" ]; then
		vregexp=$(yumGetVersionRegexp "$1")
	else
		vregexp="$2"
	fi
	n=$(echo $1 | sed -e "s/\+/\\\\+/g")
	res="`zcat ${SV_HOME_LINUX_SHARED}/distfiles.list.gz | egrep \"^[0-9]+\. .*/${n}${sep}${vregexp}(\.src)?\.(tar|zip|tgz|xgz)\" | cut -f 2 -d ' ' | tail -n 1`"
	if [ -z "$res" ]; then
		echo "Package not found $1" 1>&2
		exit 1
	fi
	n=$(echo $1 | sed -e "s/\+/\\+/g")
	basename "$res" | sed -e "s/${n}${sep}/${1} /g" -e "s/\.tar\.bz2//g" -e "s/\.tar\.gz//g" -e "s/\.zip//g" -e "s/\.tgz//g" -e "s/\.tar.xz//g"
}

######################### SECTION ############################
#{name} {version}
function yumFindFileName()
{
	n=$(echo $1 | sed -e "s/\+/\\\\+/g")
	res="`zcat ${SV_HOME_LINUX_SHARED}/distfiles.list.gz | egrep \"^[0-9]+\. .*/${n}-${2}\" | cut -f 2 -d ' ' | tail -n 1`"
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

######################### SECTION ############################
#{pack_archive}
function get_pack_urls()
{
	if [ -f "${SV_HOME_LINUX_SHARED}/distfiles.extra" ]; then
		grep "/${1}$" ${SV_HOME_LINUX_SHARED}/distfiles.extra
	fi

	for serv in ${GENTOO_MIRROR}
	do
		echo "$serv/distfiles/${1}"
	done
}

######################### SECTION ############################
function getAllSlots()
{
	cat ${SV_HOME_LINUX_SHARED}/quickpackages.slots | while read p v s
	do
		echo $p-$s $s
	done
}

######################### SECTION ############################
#{name}
function getSlot()
{
	pn=$(echo $1 | sed -e 's/\+/\+/g')
	res=$(getAllSlots | grep "^$pn " | cut -d ' ' -f 2)
	if [ ! -z "$res" ]; then
		echo $res
	fi
}

######################### SECTION ############################
#{name} {slot}
function getSlotPName()
{
	if [ -z "$2" ]; then
		echo "$1"
	else
		echo $1 | sed -e "s/-$2$//g"
	fi
}

######################### SECTION ############################
#{name} [version] [--noinherit]
function genAutoPackage()
{
	if [ -z "$2" ]; then
		pv=none
	else
		pv=$2
	fi
	slot=$(getSlot $1)
	pname=$(getSlotPName $1 $slot)
	if [ "$3" = "--noinherit" ]; then
		fname="${SV_HOME_LINUX_QUICK_PACKAGES}/noinherit-${1}.svquickpackage"
	else
		fname="${SV_HOME_LINUX_QUICK_PACKAGES}/${1}.svquickpackage"
	fi
	if [ ! -f $fname ] || [ ${SV_HOME_LINUX_SHARED}/distfiles.list.gz -nt $fname ] \
		|| [ ${SV_HOME_LINUX_SHARED}/quickpackages.version -nt $fname ] \
		|| [ ${SV_HOME_LINUX_SHARED}/quickpackages.deps -nt $fname ] \
		|| [ ${SV_HOME_LINUX_SHARED}/quickpackages.type -nt $fname ] \
		|| [ ${SV_HOME_LINUX_SHARED}/quickpackages.configure -nt $fname ] \
		|| [ ${SV_HOME_LINUX_SHARED}/quickpackages.slots -nt $fname ] \
		|| [ ${SV_HOME_LINUX_SHARED}/quickpackages.variant -nt $fname ] \
		|| [ ${SV_HOME_LINUX_SHARED}/distfiles.list.gz -nt $fname ] \
		|| [ ${PREFIX}/bin/svyum-gen -nt $fname ]
	then
		echo "-- Generate quickpackage for $1 -> $pname $pv$3 --" 1>&2
		if [ "$3" = "--noinherit" ]; then
			svyum-gen "$pname" "$pv" "${slot}" --noinherit > $fname || exit 1
		else
			svyum-gen "$pname" "$pv" "${slot}" > $fname || exit 1
		fi
	fi
	echo $fname
}

######################### SECTION ############################
function getPackageFile()
{
	if [ -f "${SV_HOME_LINUX_PACKAGES}/$1.svpackage" ]
	then
		echo "${SV_HOME_LINUX_PACKAGES}/$1.svpackage"
	else
		genAutoPackage "$1"
	fi
}

######################### SECTION ############################
function getPackageVersion()
{
	svyum-pack version "$1"
}

######################### SECTION ############################
function getPackageDeps()
{
	svyum-pack deps "$1"
}

######################### SECTION ############################
function getPackageArchive()
{
	svyum-pack archive "$1"
}

######################### SECTION ############################
function getPackageLastVersion()
{
	svyum-pack last-version "$1"
}

######################### SECTION ############################
function getPackageName()
{
	svyum-pack name "$1"
}

######################### SECTION ############################
function getPackageSlot()
{
	svyum-pack slot "$1"
}
