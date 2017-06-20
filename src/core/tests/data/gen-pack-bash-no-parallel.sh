#!/bin/bash
HL_TEMP="/MY_TMP/"
HL_PACKAGE="/MY_PREFIX///homelinux/packages/"
HL_PREFIX="/MY_PREFIX/"
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
PREFIX="/MY_PREFIX/"
BUILD_OPTIONS="\"--enable-from-quick\" \"--enable-prefix-conf\" \"--with-curses\" \"--disable-profiling\" \"--with-afs\" \"--disable-net-redirections\" \"--enable-static\" \"--enable-prefix-conf-static\""
PATCHES=""
USE="+afs +static -net gentoo ls mem-scramble nls readline"
MODULE=""
STOW_NAME=""

#to mark install and keep track
PACK_INSTALLED="/MY_PREFIX///homelinux/install-db/hl_app-shells_bash_0.json"
