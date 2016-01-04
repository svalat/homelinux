#!/bin/bash

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

MULTILIB_USEDEP=""
