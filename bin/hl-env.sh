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
node index.js "env"