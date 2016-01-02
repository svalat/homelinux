#!/bin/bash
######################################################
#           Project : HomeLinux                      #
#           Version : 1.1.0-dev                      #
#           Date    : 01/2016                        #
#           Licence : BSD                            #
#           Authors : Sebastien Valat                #
######################################################

######################################################
# Used to setup the homelinux environnement variables.

######################################################
set -e
HLPATH=$(dirname $0)
cd $HLPATH/../lib/homelinux/
$HLPATH/homelinux "env"