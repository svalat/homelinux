Packages
========

This document provide synthetic documentation on how to write your own packages.

Example
-------

```json
	{
		"name": "app-shells/bash",
		"homepage": "http://tiswww.case.edu/php/chet/bash/bashtop.html",
		"inherit": "models/autotools",
		"versions": [
			"4.0",
			"4.1",
			"4.2",
			"4.3"
		],
		"provide": [
			"gfortran"
		],
		"vfetcher": {
			"mode": "ftp",
			"url": "ftp://ftp.cwru.edu/pub/bash",
			"regexp": "bash-([0-9]+.[0-9]+).tar.gz"
		},
		"md5" : {
			"4.0": "azekljqsmfkljsfmkljsdmlfkqflj",
			"4.3": "81348932d5da294953e15d4814c74dd1"
		},
		"subdir": "bash-${VERSION}",
		"urls" : [
			"ftp://ftp.cwru.edu/pub/bash/bash-${VERSION}.tar.gz",
			"ftp://otherftp.cwru.edu/pub/bash/bash-${VERSION}.tar.gz"
		],
		"deps": [
			"make",
			"gcc > 3.4 < 4.8 ! 4.5.8",
			"install = 4.5",
			"curses? curses:4[+shell,-gui,lst] >3.4 <4.8 <=4.8 !4.5.8 ~4\\.5\\.[0-9]+"
		],
		"host": {
			"default": true,
			"debian8": {
				"std": "bash",
				"dev": "bash",
				"use": ["shell"]
			}
		},
		"configure": {
			"": [ "--enable-always", "--with-curses=$(hl_prefix curses)" ],
			"debug": ["--$enable-debug"],
			"+profile": ["--enable-profile"],
			"-profile": ["--disable-profile"]
			"+ssl & +openssl": ["--enable-profile"],
		},
		"vspecific": {
			"4.2": {
				"configure": {
					"": [ "--enable-bugfix" ]
				}
			},
			":4 >3.4 <4.8 !4.5.8 ~4\\.5\\.[0-9]+": {
				"configure": {
					"": [ "!--enable-bugfix" ]
				}
			}
		},
		"steps": {
			"test": []
		},
		"slots": {
			"~": "^([0-9]+\\.[0-9]+)",
			"2.5": ">=2.5.0 <=2.6.0"
		}
		"conflict": [ "nodejs-bin" ],
		"useflags": [ "-debug", "+expat" ],
		"warn": [
			"Some warning to print (NOT YET SUPPORTED)"
		]
	}
```

Main points
-----------

 * *name* : Define the name of the package, it must be XXX/NAME where XXX is the gentoo-like category.
 * *homepage* : URL of the home page of the package.
 * *inherit* : You can inherit from other packages or from templates provided into `models/XXX`.
you mainly need `virtual` (don't do anything), `autotools`, `cmake`. `auto` is also provided
for quickpackges as it tries to automatically apply the good sub-template. For other packages
you might want to use `default` and override the required steps.
 * *versions* : List of available version for this package. By default we take the last one to
 install so take care of the ordering. It can be automatically updated with `hl fetch-versions`.
 * *provide* : (NOT YET SUPPORTED) say that this package provide another package which can be
 used as deps from other packages. It can be use for example to support gfotran.
 * *vfetcher* : Define some parameters to automatically fetch the list of available version by grabbing
 an FTP directory or a web page. You need to provide mode saying `ftp` or `http`. Then you need to
 provide an url to fetch and to grab on and a regexp with capture semantic to extract the version
 from the files or links extracted from the page. If you fail to find websites you can also use
 `gentoo` without any URL (see media-libs/libpng as example).
 * *md5* : You can provide the MD5 checksums for each version (TO BE IMPROVED BY USING FILENAMES INSTEAD).
 * *subdir* : Define the directory which is used into the archive.
 * *urls* : A list of URL to fetch the package archive. You can use $VERSION, $NAME, $SHORT_NAME or
 every variable defined by the packaging script, it will be replaced before downloading the file.
 * *deps* : A list of dependencies. You can request enabling of some use flags by using syntax `[+shell,-gui]`
 and provide some version restriction using `>3.4 < 4.8 ! 4.5.8` (applied with AND operator). 
 VERSION AND FLAGS NOT YET SUPPORTED.
 * *host* : Provide the package name for the given host distribution, default only say if it is provided
 by default or not. Each distribution name can provide a list of packages if it is splitted into multiple one.
 You might also prefer to edit this into the `packages/hosts/XXXX` files.
 * *configure* : Provide some configure options to apply. It must be a map where the index is flag.
 You can say `+flag` to apply the config option when the flag is enabled. `-flag` to enable
 when not enabled and `empty string` to always apply. You can use notation like `--$enable-feature` linked
 to the `flag` notation, it will be replaced by `enable` or `disable` depending on the status of `flag`.
 The same applies for `--$with-package`. You can also use some macros to extract prefix of packages like
 `--with-gmp=$(hl_prefix dev-libs/gmp)` and `$(hl_with gmp dev-libs/gmp)`.
 * *vspecific* : You can define here some features to apply for some specific version, you can use any keyword
 used in the root object, it will override or complete them.
 * *steps* : Define what to do for the different steps. See `default` template and `default.sh` in models
 to understand how it work.
 * *conflict* : NOT YET SUPPORTED provide a list of packages which must ne be installed with the current one.
 * *slots* : You can provide a list of values or regular expression with capture to extract part of the 
 package version and generate a slot name. It helps to install mulitple version of the package limiting
 the checking to a sub-part of the version numbers.
 