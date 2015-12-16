#!/bin/bash

set -e
HLPATH=$(dirname $0)
cd $HLPATH/../lib/homelinux/
node index.js "$@"
