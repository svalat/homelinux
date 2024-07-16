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
DEV_DIR=$(dirname $0)
SOURCE_DIR=$(realpath "${DEV_DIR}/..")

######################################################
# build image
podman run -ti --userns=keep-id --rm --name ubuntu-homelinux-dev --workdir ${SOURCE_DIR} -v ${SOURCE_DIR}:${SOURCE_DIR} ubuntu-homelinux-dev
