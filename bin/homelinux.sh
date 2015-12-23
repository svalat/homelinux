#!/bin/bash
######################################################
#           Project : HomeLinux                      #
#           Version : 1.0.0                          #
#           Date    : 12/2015                        #
#           Licence : BSD                            #
#           Authors : Sebastien Valat                #
######################################################

set -e
HLPATH=$(dirname $0)
cd $HLPATH/../lib/homelinux/

HELP="hl {COMMAND} [OPTIONS]

Commands:
	build-cache       : Rebuild the cache file to convert short 
	                    package name to long names.
	help              : Show this help message.
	install           : Install the requested packages. Require package 
	                    name as argument.
	pinstall          : Same than install but building multiple packages at same time
	                    CAUITION, it is likely to fail if the dep-tree is not strict enougth.
	                    We recommend to enable the +gentoo flag to use this build mode.
	gen-package       : Pint generated package.
	gen-full-package  : Print fully generated package.
	gen-install       : Generate install script to forward to bash.
	update-db         : Sync the gentoo DB for unknown packages.
	fetch-versions    : Fetch versions of given package (work only
	                    for db packages.
	env               : Setup the environement variable to load.
	is-pack-installed : Check if the given package is installed or not.
	prefix-of         : Return the prefix where the package is installed or empty if not.
	ls                : List installed packages.
	search            : Search for matching packages in the available dbs.
	export            : Export the current config and list of packages.
"

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

case "$1" in
	"build-cache")
		hl-build-cache
		;;
	"help"|"--help"|"-h"|"")
		echo "$HELP"
		;;
	"install")
		shift 1
		node index.js install-ls "$@"
		yesno
		fname=$(tempfile -p homelinux)
		node index.js gen-install "$@" > $fname
		bash $fname
		rm $fname
		;;
	"pinstall")
		shift 1
		node index.js install-ls "$@"
		yesno
		tmpdir=$(mktemp -d)
		node index.js pinstall "$tmpdir" "$@"
		make -j8 -C $tmpdir
		if [ ! -z "$tmpdir" ]; then
			rm -rfv $tmpdir
		fi
		;;
	"env")
		node index.js env
		module 1>/dev/null 2>/dev/null || echo ". $(node index.js prefix-of sys-cluster/modules)/Modules/current/init/$(basename $SHELL)"
		;;
	*)
		node index.js "$@"
		;;
esac
