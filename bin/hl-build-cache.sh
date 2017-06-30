#!/bin/bash
######################################################
#           Project : HomeLinux                      #
#           Version : 1.1.0                          #
#           Date    : 06/2017                        #
#           Licence : BSD                            #
#           Authors : Sebastien Valat                #
######################################################

######################################################
# Build cache simply build the homelinux/db/cache.json file
# which avoid to use ls/find to search list of available packages

######################################################
#Setup vars
set -e
HLPATH=$(dirname $0)
cd $HLPATH/../homelinux/packages/db
#cd $HLPATH/../packages/db

######################################################
# Call the generator function, the datas will be generated
# in output using JSON format.
function gen()
{
	echo "{"
	find ./ -iname "*.json" | cut -d '/' -f 2-3 | while read line
	do
		case $line in
			'cache.json'|'gentoo.json'|'versions.json')
				;;
			*)
				line=$(echo $line | sed -e "s/\\.json//g")
				echo "	\"$line\":true,"
				;;
		esac
	done
	echo "	\"__ignore_to_skip_last_comma__\":true"
	echo "}"
}

######################################################
echo "Generate $HLPATH/../homelinux/packages/db/cache.json"
gen > $HLPATH/../homelinux/packages/db/cache.json