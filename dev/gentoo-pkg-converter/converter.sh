#!/bin/bash

if [ ! -d portage-stable ]; then
	wget ftp://gentoo.mirrors.ovh.net/gentoo-distfiles/snapshots/portage-latest.tar.bz2
	tar -xf portage-latest.tar.bz2
	mv portage portage-stable
	#git clone git@github.com:coreos/portage-stable.git
	git clone git@github.com:gentoo/portage.git
fi

node main.js "$@"
exit $?
