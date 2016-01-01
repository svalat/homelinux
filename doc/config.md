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
		}
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
