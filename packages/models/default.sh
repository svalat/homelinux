#!/bin/bash

#We consider availability of : 
#HL_TEMP : Root temp directory to download and build (/tmp/homelinux-$USER by default
#HL_PACKAGES : Path to homelinux packages directory
#PACKAGE_NAME : name of packaged (used to build tmp dir)
#PACKAGE_VERSION : version of package to install
#PACKAGE_URL : package url to download it
#PACKAGE_MD5 : MD5 sum of package if available, empty if not

#setup some defaults
if [ -z "${HL_TEMP}" ]; then HL_TEMP=/tmp/homelinux-${USER}; fi

#setup temps
function hl-setup-tmp-dir()
{
	#setup final
	HL_TEMP=${HL_TEMP}/${PACKAGE_NAME}-${PACKAGE_VERSION}
	mkdir -p ${HL_TEMP}
	cd ${HL_TEMP}
}

function hl-download()
{
	wget "${PACKAGE_URL}"
}

function hl-check()
{
	echo "TODO ${PACKAGE_MD5}"
}

function hl-extract()
{
	
}
