#!/bin/bash
######################################################
#           Project : HomeLinux                      #
#           Version : 1.0.0                          #
#           Date    : 12/2015                        #
#           Licence : BSD                            #
#           Authors : Sebastien Valat                #
######################################################

#We consider availability of : 
#HL_TEMP : Root temp directory to download and build (/tmp/homelinux-$USER by default
#HL_PACKAGES : Path to homelinux packages directory
#HL_MAKEOPTS : Options for make
#HL_PREFIX : the prefix in use
#NAME : name of packaged (used to build tmp dir)
#SHORT_NAME: base name of the package
#VERSION : version of package to install
#URLS : package url to download it (list sep by space)
#MD5 : MD5 sum of package if available, empty if not
#SUBDIR : the directory into archive
#PREFIX : The target prefix to install on
#BUILD_OPTIONS : The options to pass to configure

#colors
COLOR_RED=$(echo -e "\e[31m")
COLOR_GREEN=$(echo -e "\e[32m")
COLOR_CYAN=$(echo -e "\e[36m")
COLOR_DGRAY=$(echo -e "\e[90m")
COLOR_STD=$(echo -e "\e[0m")

#setup some defaults
if [ -z "${HL_TEMP}" ]; then HL_TEMP=/tmp/homelinux-${USER}; fi

#inc
HL_CUR_PACK=$(($HL_CUR_PACK + 1))

#start stop message
function start_stop()
{
	STEPINFO="${COLOR_DGRAY}[${HL_CUR_PACK}/${HL_TOT_PACK}][$NAME-$VERSION]${COLOR_STD}"
	echo "$STEPINFO ${COLOR_GREEN}>>>>> $@ <<<<<${COLOR_STD}"
}

#error
function die()
{
	echo "${COLOR_RED}$@${COLOR_STD}" 1>&2
	exit 1
}

#info
function info()
{
	echo "$STEPINFO ${COLOR_CYAN}>>> $@${COLOR_STD}"
}

function short_name()
{
	case $NAME in
		*/*)
			echo $NAME | cut -d '/' -f 2
			;;
		*)
			echo $NAME
			;;
	esac
}

function run()
{
	echo "$STEPINFO ${COLOR_DGRAY}>> $@${COLOR_STD}"
	eval "$@" 2>&1 | sed -e "s#^#$STEPINFO #"
	if [ ${PIPESTATUS[0]} != 0 ]; then
		return 1
	fi
}

function run_sh()
{
	echo "$STEPINFO ${COLOR_DGRAY}>> $@${COLOR_STD}"
	"$@"
}

function hl_with()
{
	echo "--with-$1=$HL_PREFIX"
}

#setup temps
function hl_setup_tmp_dir()
{
	#setup final
	HL_TEMP=${HL_TEMP}/${SHORT_NAME}-${VERSION}
	run rm -rf ${HL_TEMP}
	run mkdir -p ${HL_TEMP}
	run_sh cd ${HL_TEMP}
}

function hl_github_download()
{
	DISTFILES="$PREFIX/share/homelinux/distfiles"
	DIR=$PWD
	run mkdir -p $DISTFILES
	run_sh cd $DISTFILES
	for url in ${URLS}
	do
		ARCHIVE=$SHORT_NAME-$VERSION.tar.gz
		run wget -c "${url}" -O $ARCHIVE && break
	done
	run_sh cd ${HL_TEMP}
}

function hl_download_internal()
{
	case "${url}" in
		http://*|ftp://*|https://*|sftp://*)
			ARCHIVE=$(basename ${url})
			run wget -c "${url}" || return 1
			;;
		github://*/*)
			ARCHIVE=$SHORT_NAME-$VERSION.tar.gz
			project=$(echo $url | cut -d '/' -f 3-4)
			run wget -c -O ${ARCHIVE} "https://github.com/$project/archive/v${VERSION}.tar.gz" || return 1 
			;;
		sourceforge://*/*)
			ARCHIVE=$(basename ${url})
			name=$(echo $url | cut -d '/' -f 3)
			file=$(echo $url | cut -d '/' -f 4-)
			surl="http://sourceforge.net/projects/$name/files/$file/download";
			run wget -c -O ${ARCHIVE} "$surl" || return 1
			;;
	esac
}

function hl_download()
{
	DISTFILES="$PREFIX/share/homelinux/distfiles"
	DIR=$PWD
	run mkdir -p $DISTFILES
	run_sh cd $DISTFILES

	for url in ${URLS}
	do
		hl_download_internal ${url} || continue && break
	done
	run_sh cd ${HL_TEMP}
}

function hl_check()
{
	if [ ! -z "$MD5" ]; then
		echo "$MD5	$DISTFILES/$ARCHIVE" | run md5sum -c
	fi
}

function hl_extract()
{
	case $ARCHIVE in
		*.tar.gz)
			run tar -xzf $DISTFILES/$ARCHIVE
			;;
		*.tar.bz2)
			run tar -xjf $DISTFILES/$ARCHIVE
			;;
		*.tar.xz)
			run tar -xJf $DISTFILES/$ARCHIVE
			;;
		*)
			die "Unmanaged archive format : $ARCHIVE"
			;;
	esac
}

function hl_patch()
{
	run_sh cd $HL_TEMP/$SUBDIR
	for p in $PATCHES
	do
		run patch -p1 -i $p
	done
}

function hl_configure()
{
	die "You must select autotools or cmake or python, not inherit directly from default template..."
}

function hl_prefix()
{
	echo $HL_PREFIX
}

function hl_configure_auto()
{
	run_sh cd $HL_TEMP/$SUBDIR
	if [ -f CMakeLists.txt ]; then
		hl_configure_cmake
	elif [ -f configure ]; then
		hl_configure_autotools
	elif [ -f autogen.sh ]; then
		hl_configure_autotools_autogen
	else
		die "Unknown package type, cannot detect automatically !"
	fi
}

function hl_configure_autotools_autogen()
{
	run_sh cd $HL_TEMP/$SUBDIR
	run ./autogen.sh
	hl_configure_autotools
}

function hl_configure_autotools()
{
	run_sh cd $HL_TEMP/$SUBDIR
	run ./configure --prefix=$PREFIX $BUILD_OPTIONS
}

function hl_configure_cmake()
{
	run_sh cd $HL_TEMP/$SUBDIR
	run mkdir build
	run_sh cd build
	run cmake .. -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX=$PREFIX $BUILD_OPTIONS
}

function hl_build()
{
	run make ${HL_MAKEOPTS}
}

function hl_test()
{
	run make test
}

function hl_install()
{
	run make install
}

function hl_manifest()
{
	echo "TODO"
# 	run mkdir -p $PREFIX/share/homelinux/manifests/
# 	run_sh pushd $HL_TEMP/install
# 	run find -P > $($PREFIX/share/homelinux/manifests/$(echo $NAME | sed -e s#/#_#g)-$VERSION.lst) | sed -e 's#^\./#/#g'
# 	run_sh popd
}

function hl_module()
{
	if [ ! -z "$MODULE" ];
	then
		run mkdir -p "$PREFIX/../../modulefiles"
		cat > "$PREFIX/../../modulefiles/$MODULE" << EOF
#%Module1.0#####################################################################
##
## dot modulefile
##
## modulefiles/${MODULE}
##
module-whatis   "Add support for ${MODULE}"

prepend-path    PATH    $PREFIX/bin
prepend-path    LD_LIBRARY_PATH $PREFIX/lib
prepend-path    LD_LIBRARY_PATH $PREFIX/lib64
prepend-path    PKG_CONFIG_PATH $PREFIX/lib/pkgconfig
EOF
	fi
}

function hl_merge()
{
	echo "TODO"
# 	run make install
}

function hl_postinstall()
{
	echo "Nothing to do"
}

function hl_clean()
{
	run rm -rfd "$HL_TEMP"
}

function hl_pack_finish()
{
	run mkdir -p $HL_PREFIX/share/homelinux/install-db
	echo "$PACK_JSON" > $PACK_INSTALLED
}