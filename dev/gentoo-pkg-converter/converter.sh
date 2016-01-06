#!/bin/bash

######################################################
cd $(dirname $0)

######################################################
# Used to ask question to user before continuing.
# Args :
#  - $@ : QUestion to ask to user, as string.
function yesno()
{
	rep='?'
	while [ "$rep" != 'y' ] && [ "$rep" != "n" ] && [ "$rep" != '' ]
	do
		read -p "Continue (Y/n) ? " rep
	done

	if [ "$rep" = 'n' ]; then
		exit 0
	else
		#skip one line before start
		echo
	fi
}

######################################################
if [ ! -d portage-stable ]; then
	wget ftp://gentoo.mirrors.ovh.net/gentoo-distfiles/snapshots/portage-latest.tar.bz2
	tar -xf portage-latest.tar.bz2
	mv portage portage-stable
	#git clone git@github.com:coreos/portage-stable.git
	git clone git@github.com:gentoo/portage.git
fi

######################################################
#get last version
fname=$(ls portage-stable/$1/*.ebuild | tail -n 1)
less $fname

######################################################
node main.js "$fname" | less

######################################################
echo "=========================================================="
node main.js "$fname"
echo "=========================================================="
yesno "Is it OK (Y/n)"

######################################################
pdir=$(dirname $1)
mkdir -p ../../packages/db/$pdir
node main.js "$fname" > ../../packages/db/$1.json
kate ../../packages/db/$1.json
