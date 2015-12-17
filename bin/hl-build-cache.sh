#!/bin/bash

set -e
HLPATH=$(dirname $0)
cd $HLPATH/../share/homelinux/packages/db
#cd $HLPATH/../packages/db

function gen()
{
	echo "{"
	find ./ -iname "*.json" | cut -d '/' -f 2-3 | while read line
	do
		line=$(echo $line | sed -e "s/\\.json//g")
		echo "	\"$line\":true,"
	done
	echo "	\"__ignore_to_skip_last_comma__\":true"
	echo "}"
}

echo "Generate $HLPATH/../share/homelinux/packages/db/cache.json"
gen > $HLPATH/../share/homelinux/packages/db/cache.json