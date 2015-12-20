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
	build-cache      : Rebuild the cache file to convert short 
	                   package name to long names.
	help             : Show this help message.
	install          : Install the requested packages. Require package 
	                   name as argument.
	gen-package      : Pint generated package.
	gen-full-package : Print fully generated package.
	gen-install      : Generate install script to forward to bash.
	update-db        : Sync the gentoo DB for unknown packages.
	fetch-versions   : Fetch versions of given package (work only
	                   for db packages.
	env              : Setup the environement variable to load.
"

function yesno()
{
	rep=''
	while [ "$rep" != 'y' ] && [ "$rep" != "n" ]
	do
		read -p "Continue (y/n) ? " rep
	done

	if [ "$rep" = 'n' ]; then
		exit 0
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
		node index.js gen-install "$@" | bash
		;;
	"env")
		node index.js env
		module 1>/dev/null 2>/dev/null || echo ". $(node index.js prefix-of sys-cluster/modules)/Modules/current/init/$(basename $SHELL)"
		;;
	*)
		node index.js "$@"
		;;
esac
