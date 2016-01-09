Configuration
=============

Prefix configuration
--------------------

This is about the `homelinux.json` file located into the prefix root dir.
It might contain :

 * *inherit* : List of subprefix to inherit with to not rebuild their deps.
 They will be automatically loaded into your env vriable with less priority
 than the current prefix.
 * *override* : Force reinstall of this packages ignoring the one from the system
 or inherited profiles.
 * *compiler* : compiler option to transmit to configure or cmake script. it
 contain `MAKEOPTS`, `CFLAGS` and `CXXFLAGS`.
 * *versions* : Force version for some packages.
 * *use* : Use flags to use for the prefix. If some use flags don't match
 with the inherited prefix it will induce a rebuild of the reltated packages.
 * *gentoo* : Configure the gentoo mirror to use for quickpackage fallback.
 * *packageOverride* : You can ovveride some package entries if needed for example to quicly
 patch the installation procedure if you encounter some issues. You can override all the
 entries you want. If you want to apply on a specific version use the *vspecific* sub entry.
 * *providers*, you can change the priority of the providers to use for quickpackage building.
 By default : "homelinux", "gentoo", "debian", "urls", "github".
 
Here an example :

```json
	{
		"inherit": [
			"/home/me/usr-shell-only"
		],
		"compiler": {
			"MAKEOPTS":"-j8",
			"CFLAGS":"-O3 -march=native",
			"CXXFLAGS":"$CFLAGS"
		},
		"override": [
			"app-shell/bash"
		],
		"versions": {
			"app-shell/bash": "~2.4",
			"app-shell/dash": ">2.5 <2.6"
		},
		"use": [
			"+gentoo",
			"+debug",
			"-doc"
		],
		"gentoo" : {
			"server":"gentoo.mirrors.ovh.net",
			"port": 21,
			"distfiles": "gentoo-distfiles/distfiles/"
		},
		"packageOverride": {
			"gentoo/kdiff3": {
				"module": "kdiff3-$SVERSION"
			}
		},
		"providers": [
			"homelinux", 
			"gentoo", 
			"debian", 
			"urls", 
			"github"
		]
	}
```

User config
-----------

It is the file which is located into `~/.homelinux.json` and used to configure
all the prefix you are using.

It contains :

 * *prefix* : Define the list of prefix to load as root prefix, they will 
 automatically load their inherited ones.
 * *host* : Define the current host you are using to define the way to detect 
 the system packages. Check list of available hosts into `homelinux/hosts`.
 * *modules* : List of modules to automatically load while setting env.
 * *python* : Version of python to use.
 * *homecache* : Enable usage of `~/.homelinux/cache` to store packages. It is
 usefull for developper to not redownload the archives if you remove the prefix
 to restart with a fresh version to check package dependencies.
 * *ccache* : Enable loading of ccache paths.
 * *pyEnv* : Enable python wrappers which automatically setup the good PYTHONPATH
 depending on the python version you call. It wrap all the python and pip commands
 with a bash script.
 * *prefixOverride* : You can override some prefix config from the user config.
 It is usefull for package developpers who remove their prefix to restart from clean
 state. Add a key named with the path of your prefix and provide any prefix config entries.
 * *packageOverride* : You can ovveride some package entries if needed for example to quicly
 patch the installation procedure if you encounter some issues. You can override all the
 entries you want. If you want to apply on a specific version use the *vspecific* sub entry.

