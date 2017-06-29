#!/bin/bash

#Progress tracking
HL_TOT_PACK=2
HL_CUR_PACK=0

#!/bin/bash
HL_TEMP="/TMP/"
HL_PACKAGE="/TEST///homelinux/packages/"
HL_PREFIX="/TEST/"
HL_HOMECACHE="false"

#Compiler flags
HL_MAKEOPTS="-j8"
HL_CFLAGS="-O3 -march=native"
HL_CXXFLAGS="$CFLAGS"
HL_FFLAGS=""
HL_LDFLAGS=""

#Pack infos
NAME="hl/sys-libs/ncurses"
SHORT_NAME="ncurses"
VERSION="6.0"
SVERSION="6.0"
URLS="ftp://ftp.gnu.org/pub/gnu/ncurses/ncurses-${VERSION}.tar.bz2 ftp://ftp.gnu.org/pub/gnu/ncurses/ncurses-${VERSION}.tar.gz"
MD5=""
SUBDIR="ncurses-${VERSION}"
SLOT="0"
PREFIX="/TEST/"
BUILD_OPTIONS="\"--with-shared\""
PATCHES=""
USE="+cxx ada debug doc gpm minimal profile static-libs test threads tinfo trace unicode"
MODULE=""
STOW_NAME=""

#to mark install and keep track
PACK_INSTALLED="/TEST///homelinux/install-db/hl_sys-libs_ncurses_0.json"
PACK_JSON="
{
	\"api\" : 1,
	\"configure\" : 
	{
		\"\" : [ \"--with-shared\" ]
	},
	\"conflicts\" : null,
	\"deps\" : [ \"gpm? sys-libs/gpm\" ],
	\"flags\" : 
	{
		\"CFLAGS\" : [ \"-O3 -march=native\" ],
		\"CXXFLAGS\" : [ \"\$CFLAGS\" ],
		\"FFLAGS\" : null,
		\"LDFLAGS\" : null,
		\"MAKEOPTS\" : [ \"-j8\" ]
	},
	\"homepage\" : \"https://www.gnu.org/software/ncurses/ http://dickey.his.com/ncurses/\",
	\"host\" : null,
	\"inherit\" : \"models/auto\",
	\"md5\" : 
	{
		\"6.0\" : \"\"
	},
	\"module\" : \"\",
	\"name\" : \"hl/sys-libs/ncurses\",
	\"patch\" : null,
	\"scripts\" : [ \"models/scripts/default.sh\" ],
	\"slots\" : null,
	\"steps\" : 
	{
		\"build\" : [ \"hl_build\" ],
		\"check\" : [ \"hl_check\" ],
		\"clean\" : [ \"hl_clean\" ],
		\"configure\" : [ \"hl_configure_autotools\" ],
		\"download\" : [ \"hl_download\" ],
		\"extract\" : [ \"hl_extract\" ],
		\"install\" : [ \"hl_install\" ],
		\"main\" : [ \"@prebuild\", \"@build\", \"@postbuild\" ],
		\"manifest\" : [ \"hl_manifest\" ],
		\"merge\" : [ \"hl_merge\" ],
		\"module\" : [ \"hl_module\" ],
		\"patch\" : [ \"hl_patch\" ],
		\"postbuild\" : 
		[
			\"@test\",
			\"@install\",
			\"@manifest\",
			\"@merge\",
			\"@module\",
			\"@postinstall\",
			\"@clean\"
		],
		\"postinstall\" : null,
		\"prebuild\" : 
		[
			\"@download\",
			\"@check\",
			\"@preinstall\",
			\"@extract\",
			\"@patch\",
			\"@prepare\",
			\"@configure\"
		],
		\"preinstall\" : null,
		\"prepare\" : null,
		\"test\" : null
	},
	\"subdir\" : \"ncurses-\${VERSION}\",
	\"urls\" : 
	[
		\"ftp://ftp.gnu.org/pub/gnu/ncurses/ncurses-\${VERSION}.tar.bz2\",
		\"ftp://ftp.gnu.org/pub/gnu/ncurses/ncurses-\${VERSION}.tar.gz\"
	],
	\"use\" : 
	[
		\"+cxx\",
		\"ada\",
		\"debug\",
		\"doc\",
		\"gpm\",
		\"minimal\",
		\"profile\",
		\"static-libs\",
		\"test\",
		\"threads\",
		\"tinfo\",
		\"trace\",
		\"unicode\"
	],
	\"vars\" : null,
	\"version\" : \"6.0\",
	\"versions\" : [ \"6.0\" ],
	\"vfetcher\" : 
	{
		\"mode\" : \"ftp\",
		\"regexp\" : \"ncurses-([0-9]+\\\\.[0-9]+\\\\.?[0-9]*\\\\.?[0-9]*).(tar.bz2|tar.gz|tbz|tar.xz)\",
		\"url\" : \"ftp://ftp.gnu.org/pub/gnu/ncurses/\"
	},
	\"vspecific\" : null,
	\"warn\" : null
}
"

#load scripts
source "/TEST///homelinux/packages/models/scripts/default.sh"

#extra vars
function hl_pack_extra_vars()
{
	true
}

#package steps functions
function hl_pack_build()
{
	info 'build'
	hl_build
}
function hl_pack_check()
{
	info 'check'
	hl_check
}
function hl_pack_clean()
{
	info 'clean'
	hl_clean
}
function hl_pack_configure()
{
	info 'configure'
	hl_configure_autotools
}
function hl_pack_download()
{
	info 'download'
	hl_download
}
function hl_pack_extract()
{
	info 'extract'
	hl_extract
}
function hl_pack_install()
{
	info 'install'
	hl_install
}
function hl_pack_main()
{
	info 'main'
	hl_pack_prebuild
	hl_pack_build
	hl_pack_postbuild
}
function hl_pack_manifest()
{
	info 'manifest'
	hl_manifest
}
function hl_pack_merge()
{
	info 'merge'
	hl_merge
}
function hl_pack_module()
{
	info 'module'
	hl_module
}
function hl_pack_patch()
{
	info 'patch'
	hl_patch
}
function hl_pack_postbuild()
{
	info 'postbuild'
	hl_pack_test
	hl_pack_install
	hl_pack_manifest
	hl_pack_merge
	hl_pack_module
	hl_pack_postinstall
	hl_pack_clean
}
function hl_pack_postinstall()
{
	info 'postinstall'
	echo 'Nothing to do'
}
function hl_pack_prebuild()
{
	info 'prebuild'
	hl_pack_download
	hl_pack_check
	hl_pack_preinstall
	hl_pack_extract
	hl_pack_patch
	hl_pack_prepare
	hl_pack_configure
}
function hl_pack_preinstall()
{
	info 'preinstall'
	echo 'Nothing to do'
}
function hl_pack_prepare()
{
	info 'prepare'
	echo 'Nothing to do'
}
function hl_pack_test()
{
	info 'test'
	echo 'Nothing to do'
}

#Main calls to run
set -e
setup_vars
hl_start
hl_pack_main
hl_stop


####################################################

#!/bin/bash
HL_TEMP="/TMP/"
HL_PACKAGE="/TEST///homelinux/packages/"
HL_PREFIX="/TEST/"
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
MD5=""
SUBDIR="bash-${VERSION}"
SLOT="0"
PREFIX="/TEST/"
BUILD_OPTIONS="\"--enable-from-quick\" \"--enable-prefix-conf\" \"--with-curses\" \"--disable-profiling\" \"--with-afs\" \"--disable-net-redirections\" \"--enable-static\" \"--enable-prefix-conf-static\""
PATCHES=""
USE="+afs +static -net gentoo ls mem-scramble nls readline"
MODULE=""
STOW_NAME=""

#to mark install and keep track
PACK_INSTALLED="/TEST///homelinux/install-db/hl_app-shells_bash_0.json"
PACK_JSON="
{
	\"api\" : 1,
	\"configure\" : 
	{
		\"\" : [ \"--enable-from-quick\", \"--enable-prefix-conf\" ],
		\"+gentoo\" : [ \"--with-curses\", \"--disable-profiling\" ],
		\"afs\" : [ \"--\$with-afs\" ],
		\"mem-scramble\" : [ \"--\$enable-mem-scramble\", \"--\$with-bash-malloc\" ],
		\"net\" : [ \"--\$enable-net-redirections\" ],
		\"readline\" : [ \"--\$enable-readline\", \"--\$enable-history\", \"--\$enable-bang-history\" ],
		\"static\" : [ \"--enable-static\", \"--enable-prefix-conf-static\" ]
	},
	\"conflicts\" : null,
	\"deps\" : 
	[
		\"sys-libs/ncurses\",
		\"readline? sys-libs/readline\",
		\"nls? virtual/libintl\"
	],
	\"flags\" : 
	{
		\"CFLAGS\" : [ \"-O3 -march=native\" ],
		\"CXXFLAGS\" : [ \"\$CFLAGS\" ],
		\"FFLAGS\" : null,
		\"LDFLAGS\" : null,
		\"MAKEOPTS\" : [ \"-j8\" ]
	},
	\"homepage\" : \"http://tiswww.case.edu/php/chet/bash/bashtop.html\",
	\"host\" : null,
	\"inherit\" : \"models/auto\",
	\"md5\" : 
	{
		\"4.3.48\" : \"\"
	},
	\"module\" : \"\",
	\"name\" : \"hl/app-shells/bash\",
	\"patch\" : null,
	\"scripts\" : [ \"models/scripts/default.sh\" ],
	\"slots\" : null,
	\"steps\" : 
	{
		\"build\" : [ \"hl_build\" ],
		\"check\" : [ \"hl_check\" ],
		\"clean\" : [ \"hl_clean\" ],
		\"configure\" : [ \"hl_configure_autotools\" ],
		\"download\" : [ \"hl_download\" ],
		\"extract\" : [ \"hl_extract\" ],
		\"install\" : [ \"hl_install\" ],
		\"main\" : [ \"@prebuild\", \"@build\", \"@postbuild\" ],
		\"manifest\" : [ \"hl_manifest\" ],
		\"merge\" : [ \"hl_merge\" ],
		\"module\" : [ \"hl_module\" ],
		\"patch\" : [ \"hl_patch\" ],
		\"postbuild\" : 
		[
			\"@test\",
			\"@install\",
			\"@manifest\",
			\"@merge\",
			\"@module\",
			\"@postinstall\",
			\"@clean\"
		],
		\"postinstall\" : null,
		\"prebuild\" : 
		[
			\"@download\",
			\"@check\",
			\"@preinstall\",
			\"@extract\",
			\"@patch\",
			\"@prepare\",
			\"@configure\"
		],
		\"preinstall\" : null,
		\"prepare\" : null,
		\"test\" : null
	},
	\"subdir\" : \"bash-\${VERSION}\",
	\"urls\" : [ \"http://ftp.gnu.org/gnu/bash//bash-\${VERSION}.tar.gz\" ],
	\"use\" : 
	[
		\"+afs\",
		\"+static\",
		\"-net\",
		\"gentoo\",
		\"ls\",
		\"mem-scramble\",
		\"nls\",
		\"readline\"
	],
	\"vars\" : null,
	\"version\" : \"4.3.48\",
	\"versions\" : [ \"4.3.48\" ],
	\"vfetcher\" : 
	{
		\"mode\" : \"ftp\",
		\"regexp\" : \"bash-([0-9]+\\\\.[0-9]+).tar.gz\",
		\"url\" : \"http://ftp.gnu.org/gnu/bash/\"
	},
	\"vspecific\" : null,
	\"warn\" : null
}
"

#load scripts
source "/TEST///homelinux/packages/models/scripts/default.sh"

#extra vars
function hl_pack_extra_vars()
{
	true
}

#package steps functions
function hl_pack_build()
{
	info 'build'
	hl_build
}
function hl_pack_check()
{
	info 'check'
	hl_check
}
function hl_pack_clean()
{
	info 'clean'
	hl_clean
}
function hl_pack_configure()
{
	info 'configure'
	hl_configure_autotools
}
function hl_pack_download()
{
	info 'download'
	hl_download
}
function hl_pack_extract()
{
	info 'extract'
	hl_extract
}
function hl_pack_install()
{
	info 'install'
	hl_install
}
function hl_pack_main()
{
	info 'main'
	hl_pack_prebuild
	hl_pack_build
	hl_pack_postbuild
}
function hl_pack_manifest()
{
	info 'manifest'
	hl_manifest
}
function hl_pack_merge()
{
	info 'merge'
	hl_merge
}
function hl_pack_module()
{
	info 'module'
	hl_module
}
function hl_pack_patch()
{
	info 'patch'
	hl_patch
}
function hl_pack_postbuild()
{
	info 'postbuild'
	hl_pack_test
	hl_pack_install
	hl_pack_manifest
	hl_pack_merge
	hl_pack_module
	hl_pack_postinstall
	hl_pack_clean
}
function hl_pack_postinstall()
{
	info 'postinstall'
	echo 'Nothing to do'
}
function hl_pack_prebuild()
{
	info 'prebuild'
	hl_pack_download
	hl_pack_check
	hl_pack_preinstall
	hl_pack_extract
	hl_pack_patch
	hl_pack_prepare
	hl_pack_configure
}
function hl_pack_preinstall()
{
	info 'preinstall'
	echo 'Nothing to do'
}
function hl_pack_prepare()
{
	info 'prepare'
	echo 'Nothing to do'
}
function hl_pack_test()
{
	info 'test'
	echo 'Nothing to do'
}

#Main calls to run
set -e
setup_vars
hl_start
hl_pack_main
hl_stop


####################################################

