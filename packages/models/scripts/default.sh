#!/bin/bash
######################################################
#           Project : HomeLinux                      #
#           Version : 2.2.0-dev                      #
#           Date    : 01/2016                        #
#           Licence : CeCILL-C                       #
#           Authors : Sebastien Valat                #
######################################################

#We consider availability of : 
#HL_TEMP : Root temp directory to download and build (/tmp/homelinux-$USER by default
#HL_PACKAGES : Path to homelinux packages directory
#HL_MAKEOPTS : Options for make
#HL_PREFIX : the prefix in use
#HL_HOMECACHE : Enable usage of homecache in ~/.homelinux/cache to avoid redownloading if removing prefix
#NAME : name of packaged (used to build tmp dir)
#SHORT_NAME: base name of the package
#VERSION : version of package to install
#URLS : package url to download it (list sep by space)
#MD5 : MD5 sum of package if available, empty if not
#SUBDIR : the directory into archive
#PREFIX : The target prefix to install on
#BUILD_OPTIONS : The options to pass to configure
#STOW_NAME : Name of stow subdir if enabled, empty if not
#PYTHON_VERSION : Current version of python in use (eg: 2.7)
#PYTHON_PACKAGE_DIR: Directory where to install python modules

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

function setup_vars()
{
	STEPINFO="${COLOR_DGRAY}[${HL_CUR_PACK}/${HL_TOT_PACK}][$NAME-$VERSION]${COLOR_STD}"
	HL_TEMP=${HL_TEMP}/${SHORT_NAME}-${VERSION}
	DISTFILES="$HL_PREFIX/homelinux/distfiles"
	HL_BUILDDIR=$HL_TEMP/$SUBDIR
	HL_PACKDIR=$HL_TEMP/$SUBDIR
	PYTHON_VERSION=$(python --version 2>&1 | cut -d ' ' -f 2 | cut -d '.' -f 1-2)
	PYTHON_PACKAGE_DIR=${PREFIX}/lib/python${PYTHON_VERSION}/site-packages/
	if [ -f $HL_PACKDIR/hl-is-cmake.notify ] && [ "${PINSTALL}" = "true" ]; then
		HL_BUILDDIR=$HL_BUILDDIR/cmakebuild
	fi
	hl_pack_extra_vars
}

#start stop message
function start_stop()
{
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

function run_nocheck()
{
	echo "$STEPINFO ${COLOR_DGRAY}>> $@${COLOR_STD}"
	set +e
	eval "$@" 2>&1 | sed -e "s#^#$STEPINFO #" || return
	set -e
}

function run_echo()
{
	echo "$STEPINFO ${COLOR_DGRAY}>> $@${COLOR_STD}"
}

function run()
{
	echo "$STEPINFO ${COLOR_DGRAY}>> $@${COLOR_STD}"
	eval "$@" 2>&1 | sed -e "s#^#$STEPINFO #"
	if [ ${PIPESTATUS[0]} != 0 ]; then
		return 1
	fi
}

function display()
{
	echo "$STEPINFO ${COLOR_DGRAY}>> $@${COLOR_STD}"
}

function run_sh()
{
	echo "$STEPINFO ${COLOR_DGRAY}>> $@${COLOR_STD}"
	"$@"
}

function hl_cmake_find_package()
{
	if [ "$1" = "enable" ]; then
		shift 1
		echo "-DCMAKE_DISABLE_FIND_PACKAGE_$1=ON"
	elif [ "$1" = "disable" ]; then
		shift 1
		echo "-DCMAKE_DISABLE_FIND_PACKAGE_$1=OFF"
	else
		echo "-DCMAKE_DISABLE_FIND_PACKAGE_$1=ON"
	fi
}

function hl_cmake_enable()
{
	if [ "$1" = "enable" ]; then
		shift 1
		echo "-DENABLE_$1=ON"
	elif [ "$1" = "disable" ]; then
		shift 1
		echo "-DENABLE_$1=OFF"
	else
		echo "-DENABLE_$1=ON"
	fi
}

function hl_option_if_config_has()
{
	cd $HL_PACKDIR
	if grep -q "$1"; then
		echo "--$1"
	fi
}

function hl_cmake_with()
{
	if [ "$1" = "with" ]; then
		shift 1
		echo "-DWITH_$1=ON"
	elif [ "$1" = "without" ]; then
		shift 1
		echo "-DWITH_$1=OFF"
	else
		echo "-DWITH_$1=ON"
	fi
}


function hl_with()
{
	if [ "$1" = "with" ]; then
		shift 1
		echo "--with-$1=$(hl_prefix $2)"
	elif [ "$1" = "without" ]; then
		shift 1
		echo "--without-$1=$(hl_prefix $2)"
	else
		echo "--with-$1=$(hl_prefix $2)"
	fi
}

#setup temps
function hl_setup_tmp_dir()
{
	#setup final
	run rm -rfd ${HL_TEMP}
	run mkdir -p ${HL_TEMP}
	run_sh cd ${HL_TEMP}
}

function hl_github_download()
{
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
	#get filename
	case "${url}" in
		http://*|ftp://*|https://*|sftp://*)
			ARCHIVE=$(basename ${url})
			;;
		github://*/*|git://*)
			ARCHIVE=$SHORT_NAME-$VERSION.tar.gz
			;;
		sourceforge://*/*)
			ARCHIVE=$(basename ${url})
			;;
		*)
			die "Invalid url type : $url"
			;;
	esac
	
	#spacial case
	if [ "$ARCHIVE" = "master.zip" ]; then
		ARCHIVE=$SHORT_NAME-$VERSION.zip
	fi
	
	#hceck cache
	if [ -f "$ARCHIVE" ]; then
		return;
	fi

	#home cache
	if [ "$HL_HOMECACHE" = 'true' ] && [ -f $HOME/.homelinux/cache/$ARCHIVE ] ; then
		ln -sf $HOME/.homelinux/cache/$ARCHIVE $ARCHIVE
		return
	fi
	
	#download
	case "${url}" in
		http://*|ftp://*|https://*|sftp://*)
			if ! run wget -c "${url}" -O ${ARCHIVE}; then
				rm -f "${ARCHIVE}"
				return 1
			fi
			;;
		git://*)
			DIR=$PWD
			s=$(basename ${url} | sed -e 's/.git//g')
			run_sh cd /tmp
			if [ ! -d "$s" ]; then
				run git clone ${url}
			fi
			run_sh cd $s
			(run_echo git archive --format=tar --prefix=$SUBDIR/ v${VERSION} '>' $SHORT_NAME-$VERSION.tar && git archive --format=tar --prefix=$SUBDIR/ ${VERSION} > v$SHORT_NAME-$VERSION.tar) \
				|| (run_echo run git archive --format=tar --prefix=$SUBDIR/ ${VERSION} '>' $SHORT_NAME-$VERSION.tar && run git archive --format=tar --prefix=$SUBDIR/ ${VERSION} > $SHORT_NAME-$VERSION.tar)
			run gzip $SHORT_NAME-$VERSION.tar
			run mv $ARCHIVE $DISTFILES/$ARCHIVE
			run_sh cd $DIR
			run rm -rf /tmp/$s
			;;
		github://*/*)
			project=$(echo $url | cut -d '/' -f 3-4)
			run wget -c -O ${ARCHIVE} "https://github.com/$project/archive/v${VERSION}.tar.gz" || \
				run wget -c -O ${ARCHIVE} "https://github.com/$project/archive/${VERSION}.tar.gz" || \
				run wget -c -O ${ARCHIVE} "https://github.com/$project/archive/${SHORT_NAME}-${VERSION}.tar.gz" || \
				run wget -c -O ${ARCHIVE} "https://github.com/$project/archive/release-${VERSION}.tar.gz" || \
				rm ${ARCHIVE} && \
				return 1 
			;;
		sourceforge://*/*)
			name=$(echo $url | cut -d '/' -f 3)
			file=$(echo $url | cut -d '/' -f 4-)
			surl="http://sourceforge.net/projects/$name/files/$file/download";
			run wget -c -O ${ARCHIVE} "$surl" || return 1
			;;
		*)
			die "Invalid url type : $url"
			;;
	esac
	
	#home cache
	if [ "$HL_HOMECACHE" = 'true' ]; then
		run mkdir -p $HOME/.homelinux/cache
		run cp $ARCHIVE $HOME/.homelinux/cache
	fi
}

function hl_download()
{
	DIR=$PWD
	run mkdir -p $DISTFILES
	run_sh cd $DISTFILES

	for url in ${URLS}
	do
		hl_download_internal ${url} && break
	done

	test -f ${ARCHIVE} || die "Fail to download archive !"

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
		*.tar.gz|*.tgz)
			run tar -xzf $DISTFILES/$ARCHIVE
			;;
		*.tar.bz2)
			run tar -xjf $DISTFILES/$ARCHIVE
			;;
		*.tar.xz)
			run tar -xJf $DISTFILES/$ARCHIVE
			;;
		*.tar.lz)
			run tar --lzip -xf $DISTFILES/$ARCHIVE
			;;
		*.zip)
			run unzip $DISTFILES/$ARCHIVE
			;;
		*.7z)
			run 7za x $DISTFILES/$ARCHIVE
			;;
		*)
			die "Unmanaged archive format : $ARCHIVE"
			;;
	esac
}

function hl_patch()
{
	run_sh cd $HL_PACKDIR
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
	hl prefix-of $1 || exit 1
}

function hl_extract_other_pack()
{
	pack=$1
	hl gen-install-p $pack > __hl-pack__.sh
	run bash __hl-pack__.sh hl_start
	run bash __hl-pack__.sh hl_pack_prebuild
}

function hl_configure_auto()
{
	run_sh cd $HL_PACKDIR
	if [ -f CMakeLists.txt ]; then
		hl_configure_cmake
	elif [ -f configure ]; then
		hl_configure_autotools
	elif [ -f autogen.sh ]; then
		hl_configure_autotools_autogen
	elif [ -f setup.py ]; then
		hl_configure_python
	elif [ -f Makefile.PL ]; then
		hl_configure_perl
	elif [ ! -z "$(ls *.pro)" ]; then
		hl_configure_qmake
	else
		die "Unknown package type, cannot detect automatically !"
	fi
}

function hl_configure_perl()
{
	#two types because we encoutered an issue on centos7
	run perl Makefile.PL PREFIX=${PREFIX} $BUILD_OPTIONS || run perl Makefile.PL INSTALL_BASE=${PREFIX} $BUILD_OPTIONS
}

function hl_configure_python()
{
	echo ">> Generate Makefile for installation <<"
	echo "all:" > Makefile
	echo "	python${PYTHON_SLOT} setup.py build $BUILD_OPTIONS" >> Makefile
	echo "" >> Makefile
	echo "install:" >> Makefile
	echo "	python${PYTHON_SLOT} setup.py install --prefix=${PREFIX}" >> Makefile
}

function hl_configure_autotools_autogen_nocheck()
{
	run_sh cd $HL_PACKDIR
	run_nocheck ./bootstrap.sh
	run_nocheck ./autogen.sh
	hl_configure_autotools
}

function hl_configure_autotools_autogen()
{
	run_sh cd $HL_PACKDIR
	if [ -f ./autogen.sh ]; then
		run ./autogen.sh --prefix=$PREFIX $BUILD_OPTIONS
	elif [ -f ./bootstrap.sh ]; then
		run ./bootstrap.sh --prefix=$PREFIX $BUILD_OPTIONS
	else
		run aclocal
		run autoconf
		run automake --add-missing
	fi
	hl_configure_autotools
}

function hl_configure_autotools()
{
	run_sh cd $HL_PACKDIR
	
	display CFLAGS=\"${CFLAGS} ${HL_CFLAGS}\"
	display CXXFLAGS=\"${CXXFLAGS} ${HL_CXXFLAGS}\"
	display LDFLAGS=\"${LDFLAGS} ${HL_LDFLAGS}\"
	display FFLAGS=\"${FFLAGS} ${HL_FFLAGS}\"
	
	CXXFLAGS="${CXXFLAGS} ${HL_CXXFLAGS}"\
	CFLAGS="${CFLAGS} ${HL_CFLAGS}" \
	FFLAGS="${FFLAGS} ${HL_FFLAGS}" \
	LDFLAGS="${LDFLAGS} ${HL_LDFLAGS}" \
	run ./configure --prefix=$PREFIX \
				$BUILD_OPTIONS
}

function hl_configure_cmake()
{
	run mkdir -p $HL_BUILDDIR
	run_sh cd $HL_BUILDDIR
	run touch hl-is-cmake.notify
	run rm -rfd cmakebuild
	run mkdir cmakebuild
	run_sh cd cmakebuild
	if which ninja > /dev/null 2> /dev/null && [ "${PINSTALL}" = "false" ]; then
		ninja="-GNinja"
	else
		ninja=""
	fi
	
	run cmake .. ${ninja} -DCMAKE_BUILD_TYPE="Release" \
	             -DCMAKE_INSTALL_PREFIX=$PREFIX\
	             -DCMAKE_CXX_FLAGS="\"${CXXFLAGS} ${HL_CXXFLAGS}\""\
	             -DCMAKE_C_FLAGS="\"${CFLAGS} ${HL_CFLAGS}\"" \
	             -DCMAKE_F_FLAGS="\"${FFLAGS} ${HL_FFLAGS}\"" \
	             -DCMAKE_MODULE_LINKER_FLAGS="\"${LDFLAGS} ${HL_LDFLAGS}\""\
	             -DCMAKE_SHARED_LINKER_FLAGS="\"${LDFLAGS} ${HL_LDFLAGS}\""\
	             -DCMAKE_STATIC_LINKER_FLAGS="\"${LDFLAGS} ${HL_LDFLAGS}\""\
	             $BUILD_OPTIONS
}

function hl_configure_qmake()
{
	run qmake -set prefix $PREFIX $BUILD_OPTIONS
}

function hl_build()
{
	#if has arg, this is due to call for parallel install
	#in that cas, just create hl-pmake.notify to say that
	#wa can to the command into the makefile itself to keep
	#parallelism management
	if [ ! -z "$1" ]; then
		echo $HL_BUILDDIR > "$1"
	elif [ -f rules.ninja ] && [ ! -f Makefile ]; then
		run ninja
	else
		run make ${HL_MAKEOPTS}
	fi
}

function hl_test()
{
	run_sh cd $HL_BUILDDIR
	run make test
}

function hl_install()
{
	run_sh cd $HL_BUILDDIR
	if [ -d cmakebuild ]; then
		run_sh cd cmakebuild
	fi
	if [ -f rules.ninja ] && [ ! -f Makefile ]; then
		run ninja install
	else
		run make install
	fi
}

function hl_vim_plugin_cp()
{
	vimpath=$(ls ${HL_PREFIX}/share/vim/ | egrep "[0-9]+" | sort | tail -n 1)
	run cp -r "$@" ${HL_PREFIX}/share/vim/$vimpath/plugin
}

function hl_vim_install()
{
	vimpath=$(ls ${HL_PREFIX}/share/vim/ | egrep "[0-9]+" | sort | tail -n 1)
	run cp -r $HL_PACKDIR/* ${HL_PREFIX}/share/vim/$vimpath
	
# 	dir=$(basename ${HL_TEMP}/*)
# 	for tmp in ${HL_PREFIX}/share/vim/$vimpath/$dir/autoload/*.vim
# 	do
# 		run ln -sf $tmp ${HL_PREFIX}/share/vim/$vimpath/
# 	done
}

function hl_manifest()
{
	echo "TODO"
# 	run mkdir -p $PREFIX/homelinux/manifests/
# 	run_sh pushd $HL_TEMP/install
# 	run find -P > $($PREFIX/homelinux/manifests/$(echo $NAME | sed -e s#/#_#g)-$VERSION.lst) | sed -e 's#^\./#/#g'
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
	if [ ! -z "$STOW_NAME" ]; then
		run stow --dir=$HL_PREFIX/stow --target=$HL_PREFIX -S $STOW_NAME
	fi
}

function hl_uninstall()
{
	#TODO: need to check if used at install time on package
	if [ -z "$STOW_NAME" ]; then die "Cannot uninstall packages if stow is not enabled, check prefix config file."; fi
	
	#remove from stow
	run stow --dir=$HL_PREFIX/stow --target=$HL_PREFIX -D $STOW_NAME
	
	#delete
	if [ ! -z "$PREFIX" ]; then
		run rm -rfd "$PREFIX"
	fi
	
	#remove package descr
	run rm $PACK_INSTALLED
}

function hl_postinstall()
{
	echo "Nothing to do"
}

function hl_clean()
{
	cd
	run rm -rfd "$HL_TEMP"
}

function hl_pack_finish()
{
	run mkdir -p $HL_PREFIX/homelinux/install-db
	echo "$PACK_JSON" > $PACK_INSTALLED
}

function hl_start()
{
	start_stop "Start $NAME-$VERSION"
	hl_setup_tmp_dir
}

function hl_stop()
{
	hl_pack_finish
	start_stop "Finish $NAME-$VERSION"
}
