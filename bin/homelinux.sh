#!/bin/bash

set -e
HLPATH=$(dirname $0)
cd $HLPATH/../lib/homelinux/

if [ "$1" = "build-cache" ]; then
	hl-build-cache
else
	node index.js "$@"
fi
