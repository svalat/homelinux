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
	echo "$@"
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
		echo "@with@$1@$2"
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

MULTILIB_USEDEP=""
