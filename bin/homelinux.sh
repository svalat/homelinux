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
"

case "$1" in
	"build-cache")
		hl-build-cache
		;;
	"help"|"--help"|"-h")
		echo "$HELP"
		;;
	*)
		node index.js "$@"
		;;
esac
