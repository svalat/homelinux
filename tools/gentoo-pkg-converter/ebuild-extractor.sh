#!/bin/bash

PV=1.0.0
ABI=amd64

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

use()
{
	return 0
}

inherit()
{
	ignore
}

#load file
source "$2"

#run
case "$1" in
	'home')
		echo "$HOMEPAGE"
		;;
	'deps')
		echo "$DEPEND"
		echo "$RDEPEND"
		;;
	'config')
		multilib_src_configure
		;;
	'use')
		echo $IUSE
		;;
esac
