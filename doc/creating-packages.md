Packages
========

This document provide synthetic documentation on how to write your own packages.

Conversion from Gentoo
----------------------

Before starting to write you own package by hand look if the package exist on Gentoo (https://packages.gentoo.org/).
If it is the case you can use the semi-automated script to translate the Gentoo package into an HomeLinux package.
It saves a lot of work, mostly the handling of dependencies and use flags/options.

To proceed go into the `./dev/gentoo-pkg-converter` in the source code. For this you need `nodejs`. The first time
you go in that directory you need to import the nodejs modules we are using in the script :

```sh
npm install
```

Then you can convert you package, let say you want to convert `app-shells/bash` :

```
./convert.sh app-shells/bash
```

It will print the Gentoo package so you can look on, then will make the automatic conversion and print the result.
It will then ask you if you are ok. When done it will try to open the package into `kate`.

Consider the conversion as *semi-automatic*, there is still some bugs you will have to manually fix mostly on the deps definition.

You also habe to provide the URLs and REGEXP for the version crawling which *cannot be deduced automatically* from the Gentoo packaeg.

Example
-------

Remark that the mandatory fields are only :

 * name
 * homepage (not used by homelinux, but better to get it)
 * versions
 * vfetcher (and all its content)
 * subdir
 * urls
 
All the orther entries are optional.

```json
	{
		"api": 1,
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
			"curses? curses:4[+shell,-gui,lst,#requireIfParentHas] >3.4 <4.8 <=4.8 !4.5.8 ~4.5"
		],
		"flags": {
			"CFLAGS":[ "-L$(hl prefix-of gcc)/lib"],
			"LDFLAGS":[ "-L$(hl prefix-of gcc)/lib"]
		},
		"host": {
			"default": true,
			"debian8": [ "bash" ],
			"centos7": [ "bash" ],
			"gentoo": [ "app-shells/bash" ]
		},
		"configure": {
			"": [ "--enable-always", "--with-curses=$(hl_prefix curses)" ],
			"debug": ["--$enable-debug"],
			"+profile": ["--enable-profile"],
			"-profile": ["--disable-profile"],
			"+ssl&+openssl": ["--enable-profile"],
		},
		"vspecific": {
			"4.2": {
				"configure": {
					"": [ "--enable-bugfix" ]
				}
			},
			":4 >3.4 <4.8 !4.5.8 ~4.5": {
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
			"2.5": ">=2.5.0 <2.6.0"
		},
		"conflict": [ "nodejs-bin" ],
		"use": [ "-debug", "-expat" ],
		"gentooUse": [ "+expat" ],
		"warn": [
			"Some warning to print (NOT YET SUPPORTED)"
		],
		"module": "gcc-${VERSION}",
		"vars": {
			"PYTHON_SLOT": "3"
		},
		"scripts": [ "models/scripts/default.sh" ]
	}
```

Main points
-----------

*name* : Define the name of the package, it must be XXX/NAME where XXX is the gentoo-like category. Use a canonic
name similar to Gentoo (use the same name if package exist).

```json
{
	"name": "app-shells/bash"
}
```

*homepage* : URL of the home page of the package.

```json
{
	"homepage": "https://www.gnu.org/software/bash/"
}
```

*inherit* : You can inherit from other packages or from templates provided into `models/XXX`.
you mainly need :

 * `virtual` : don't do anything" 
 * `autotools` : apply autotools (configure, make, make install).
 * `autotools-autogen` : To be used on project where autoconfig/automake has not been run (mostly when taking sources from github)/
 * `cmake` : for project using cmake. If Ninja it available it will also use Ninja instead of make.
 * `perl` : To be used on perl packages with `Makefile.pl` files inside.
 * `python` : To be used on python packages with `setup.py` files inside.
 * `qmake` : To be used on QT projects with `*.pro` files inside.
 * `auto` is also provided for quickpackges as it tries to automatically apply the good sub-template. 
 * For other packages you might want to use `default` and override the required steps.

```json
{
	"inherit": "models/cmake"
}
```

*versions* : List of available version for this package. By default we take the last one to
install so take care of the ordering.

```json
{
	"versions": [ "1.0.4", "1.0.5" ]
}
```

*vfetcher* : Define some parameters to automatically fetch the list of available version by grabbing
 an FTP directory or a web page. You need to select a mode : 
 * `ftp` : read list of files or directories into an FTP address and match with the given regexp.
 * `http` or `html` : Fetch an html page from the given address and crawl inside looking by default
 on links text to match the given regexp. Remark than you can select the kind of tag you want to
 search on by adding a `tag` entry (see example).
 * `http-gnome-cache` : Gnome packages provide a `cache.json` file in their repository with list of all
 avilable version. You need to provide the URL of this file. Regexp will not be used.
 * `gentoo` : If you fail to find a website you can crawl inside the gentoo distfile matching the file names
 with the given regexp.
 * `github` : Look on github releases or tags. The URL must be provided as `github://USER/PROJECT`
 provide an url to fetch and to grab on and a regexp with capture semantic to extract the version

 Remark you need to use capture semantic in regexp to extract the vesrion from the matched string, meaning
 using `()` arround the version patther.

 Here some examples

```json
{
	"vfetcher": {
		"mode": "ftp",
		"urls": [
			"ftp.gnu.org/gnu/bash/"
		],
		"regexp": "bash-([0-9]+.[0-9]+.?[0-9]*.?[0-9]*).(tar.bz2|tar.gz|tbz|tar.xz|zip|tbz2)"
	}
}
```

```json
{
	"vfetcher": {
		"mode": "http",
		"urls": [
			"mirror://http-gnu/gnu/bash/"
		],
		"regexp": "bash-([0-9]+.[0-9]+.?[0-9]*.?[0-9]*).(tar.bz2|tar.gz|tbz|tar.xz|zip|tbz2)",
		"tag": "a"
	}
}
```

```json
{
	"vfetcher": {
		"mode": "http-gnome-cache",
		"urls": [
			"http://ftp.gnome.org/pub/GNOME/sources/pygtk/cache.json"
		],
		"name": "pygtk"
	}
}
```

```json
{
	"vfetcher": {
		"mode": "gentoo",
		"regexp": "bash-([0-9]+.[0-9]+.?[0-9]*.?[0-9]*).(tar.bz2|tar.gz|tbz|tar.xz|zip|tbz2)"
	}
}
```

```json
{
	"vfetcher": {
		"mode": "github",
		"urls": "github://svalat/homelinux",
		"regexp": "v([0-9]+.[0-9]+.?[0-9]*.?[0-9]*)"
	}
}
```

In some caes you can encounter some situation where you need to first crawl directories with two digit versions
then you get the final three digit verison inside the driectory (eg. qt), this can be solved by using the subdir semantic:

```json
	"vfetcher": {
			"subdir": {
					"mode": "html",
					"url": [
							"http://download.qt.io/archive/qt/"
					],
					"regexp": "([0-9]+.[0-9]+.?[0-9]*.?[0-9]*)/"
			},
			"mode": "html",
			"url": [
					"http://download.qt.io/archive/qt/${SVERSION}/"
			],
			"regexp": "([0-9]+.[0-9]+.?[0-9]*.?[0-9]*)/"
	}
}
```




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
 * *module* : If present, the package will be installed into PREFIX/Modules/installed/$MODULE_NAME and
 a module configuration file will be setup.
 * *gentooUse* : If gentoo enable some flags by default you can list them here, it will be enable
 if `+gentoo` flag is enabled to produce a gentoo-like build.
 * *vars* : Used to define some extra variables which can be used by the command used to build the package,
 in the example code, it define the python version to use to install a python module.
 * *scripts* : Load a bash script library of functions in addition to the existing one. The path must
 be relative to `homelinux/packages` subdirectory.
 
About commands
--------------

When you provide some commands into the `step` part, try to prefix them by
`run` or `run_sh`, it will print the command and prefix the output with the
package name.

About version filtering
-----------------------

On various part of the file you can filter version, the syntax is (do not add spaces between operator and value) :

 * `~REGEXP`
 * `>VERSION`
 * `>=VERSION`
 * `<VERSION`
 * `<=VERSION`
 * `!VERSION`
 * `=VERSION`
 * `:SLOT`
 
Remark that it is defined as a string and spaces between groups of operator and values are replaced by AND operator.

About use
--------------

The flag system of HomeLinux is directly inpirated from the Gentoo one. As a user you can setup the use flags you want
int `PREFIX/homelinux.json`. You can setup the default flags by filling key `all`. You can setup `+svg` or `svg` to enable 
usage of SVG in packages. You need to use `-svg` to disable. Similarly you can setup specific use per packages
by using the full package name as key. Example :

```json
	{
		"use": {
			"all": [ "+svg", "pdf", "-debug" ],
			"sys-devel/gcc": [ "+debug" ]
		}
	}
```

The use are used in packages into two parts, for dependencies and for configure options. You also need to declare
the useflag list used by package and provide their default values.
Here an example :

```json
	{
		"use": [ "svg", "debug", "+png", "-jpeg" ],
		"configure": {
			"always": [ "--enable-print" ],
			"+svg": [ "--enable-svg" ],
			"-jpeg" : [ "--disable-jpeg" ],
			"debug" : [ "--$enable-debug" ],
			"png & jpeg": [ "--$enable-png" ]
		},
		"deps": [
			"sys-devel/gcc",
			"svg? dev-libs/libsvg",
			"png? dev-libs/libpng"
		]
	}
```

Here, in configure, the use can be listed as :

 * `always` or empty are applied in any case.
 * `+svg` is applied only if svg flag is enabled.
 * `-jpeg` is applied only if jpeg flag is disabled.
 * `debug` is applied if flag is enabled or disabled. Depending on the status the $enable value is replaced by the 
 required state. If debug is not enable or disable the line is skiped.
 * You can use the `&` close to match multiple flags. Caution, or semantic is not provided.

About gentoo flag
-----------------

On each packages, try to use the `gentoo` it you add dependencies from the gentoo packages, it provide a nice
way to generate the full build from gentoo tree but still keeping the default mode
using less options as possible on the packages.

Script and variables to build options
-------------------------------------

 * `--DENABLE_PROPERTY=$ON` : To setup ON or OFF depending on the useflag status for CMake.
 * `--DENABLE_PROPERTY=$OFF` : To setup ON or OFF depending on the useflag status (opposite of) for CMake.
 * `--$with-package` : Enable usage of package (replaced by with ou without depending on flag status).
 * `--$without-package` : Enable usage of package (replaced by with ou without depending on opposite flag status).
 * `--$enable-package` : Enable usage of package (replaced by with ou without depending on flag status).
 * `--$disable-package` : Enable usage of package (replaced by with ou without depending on opposite flag status).
 * `$(hl_option_if_config_has enable-debug)` : Search in configure if option is available, and if true, setup --${OPTION}).
 * `$(hl_with $with sys-devel/cairo)` : Apply with or without and also add path to prefix of requested package as value.
 * `--$with-cairo=$(hl_prefix sys-devel/cairo)` : Alternative to previous semantic.

 Quicly convert Gentoo packages
 ------------------------------
 
 You can find an helper script to convert gentoo packages into HL packages,
 at least for the dependencies, useflags and some config options. Please
 check what it provide and fill the missing field. Also check :
 
  * the host compatibility name which is choose only based to the name of the package.
  * The inheritance which it let to default value. But you need to setup kde, gnome2...
  by yourself.
  * Check the dependencies which might have some mistakes. The `!` tag for
  incompatibility checking is not yet supported to replace them by `-TODO`.
 
 You can find the script into `dev/gentoo-pkg-converter`.
 