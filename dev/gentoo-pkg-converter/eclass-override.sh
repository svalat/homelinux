#!/bin/bash

set +e

ignore()
{
	echo > /dev/null
}

function tc-export()
{
	ignore
}

econf()
{
	echo "$@" > /tmp/hl_pkg_convert_conf.txt
}

emake()
{
	ignore
}

usex()
{
	if [ ! -z "$2" ]; then
		echo "@on@+$1@$2@$4"
	fi
	if [ ! -z "$3" ]; then
		echo "@on@-$1@$3@$5"
	fi
}

use_with()
{
	if [ -z "$2" ]; then
		echo "@with@$1@$1"
	else
		echo "@with@$1@$2@$3"
	fi
}

use_enable()
{
	if [ -z "$2" ]; then
		echo "@enable@$1@$1"
	else
		echo "@enable@$1@$2"
	fi
}

cmake-utils_use_with()
{
	flag=$1
	value=$2
	if [ -z "$value" ]; then
		value=$(echo $flag | awk '{print toupper($0)}')
	fi
	echo "@on@${flag}@-DWITH_${value}=\$ON"
}

cmake-utils_use_enable()
{
	flag=$1
	value=$2
	if [ -z "$value" ]; then
		value=$(echo $flag | awk '{print toupper($0)}')
	fi
	echo "@on@${flag}@-DENABLE_${value}=\$ON"
}

cmake-utils_use_find_package()
{
	flag=$1
	value=$2
	if [ -z "$value" ]; then
		value=$(echo $flag | awk '{print toupper($0)}')
	fi
	echo "@on@${flag}@-DCMAKE_DISABLE_FIND_PACKAGE_Lib${value}=\$OFF"
}

cmake-utils_use_has()
{
	flag=$1
	value=$2
	if [ -z "$value" ]; then
		value=$(echo $flag | awk '{print toupper($0)}')
	fi
	echo "@on@${flag}@-DHAVE_${value}=\$ON"
}

has() {
	local needle=$1
	shift

	local x
	for x in "$@"; do
		[ "${x}" = "${needle}" ] && return 0
	done
	return 1
}

die()
{
	echo "$@" 1>&2
	exit 1
}

use()
{
	return 0
}

EXPORT_FUNCTIONS()
{
	ignore
}

debug-print()
{
	ignore
}

debug-print-function()
{
	ignore
}

pushd()
{
	ignore
}

popd()
{
	ignore
}

einfo()
{
	ignore
}

best_version()
{
	ignore
}

db_ver_to_slot()
{
	ignore
}

tc-getCC()
{
	ignore
}

inherit()
{
	ignore
}

gnome2_src_configure()
{
	econf "$@"
}

multilib_native_use_enable()
{
	ignore
}

strip-unsupported-flags()
{
	ignore
}

tc-is-static-only()
{
	ignore
}

db_findver()
{
	ignore
}

multilib_is_native_abi()
{
	return 1
}

alias cd=true

cmake-utils_src_configure()
{
	echo "${mycmakeargs[*]}" > /tmp/hl_pkg_convert_conf.txt
}

kde4-base_src_configure()
{
	echo "${mycmakeargs[*]}" > /tmp/hl_pkg_convert_conf.txt
}

gnome2_src_configure()
{
	echo "$@" > /tmp/hl_pkg_convert_conf.txt
}

MULTILIB_USEDEP=""
