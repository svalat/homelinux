#!/bin/bash

HLPATH=$(dirname $0)
cd $HLPATH/../lib/homelinux/
node index.js "$@"
