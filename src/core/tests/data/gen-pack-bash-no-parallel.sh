#!/bin/bash
HL_TEMP="/tmp/homelinux-svalat"
HL_PACKAGE="/home/svalat/Projects/homelinux/src/core/tests/data//homelinux/packages/"
HL_PREFIX="/home/svalat/Projects/homelinux/src/core/tests/data"
HL_HOMECACHE="false"

#Compiler flags
HL_MAKEOPTS="-j8"
HL_CFLAGS="-O3 -march=native"
HL_CXXFLAGS="$CFLAGS"
HL_FFLAGS=""
HL_LDFLAGS=""

#Pack infos
NAME="hl/app-shells/bash"
SHORT_NAME="bash"
VERSION="4.3.48"
SVERSION="4.3"
URLS="http://ftp.gnu.org/gnu/bash//bash-${VERSION}.tar.gz"
URLS=""
SUBDIR="bash-${VERSION}"
SLOT="0"
PREFIX="/home/svalat/Projects/homelinux/src/core/tests/data"
