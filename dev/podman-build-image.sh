#!/bin/bash
######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0                        #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
set -x
set -e
set -u

######################################################
# extract ./dev dir path
CURRENT_DIR=$(dirname $0)

######################################################
# build image
podman build -t ubuntu-homelinux-dev ${CURRENT_DIR}/docker/
