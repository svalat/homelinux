#!/bin/bash

PV=1.0.0
ABI=amd64
MULTILIB_USEDEP=""

PORTAGE_BIN_PATH=$PWD/portage/bin

export PATH=$PWD/sandbox:$PATH
S=$PWD/sandbox/

inherit()
{
	for i in "$@"
	do
		source eclass-override.sh
		source portage-stable/eclass/$i.eclass
		source eclass-override.sh
	done
}

source eclass-override.sh
# source $PORTAGE_BIN_PATH/ebuild.sh

#load file
if [ ! -z "$2" ]; then
	source "$2"
fi

#override
source eclass-override.sh

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
		src_configure || multilib_src_configure || ignore
		;;
	'use')
		echo $IUSE
		;;
esac
