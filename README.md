HomeLinux
=========


What is it ?
------------

Homelinux is a simple package manager to handle installation of packages in your home directory by rebuilding them from sources.
The main use case if a user (or developper) running into a non root environnement and needs some extra-package he can't or don't want
to install into the system. Thanks to HomeLinux you can handle the installation of the source package by checking the dependencies
available on the current system and automatically installing the missing one.

The goal is to provide a quick access to the latest version available for all packaging without regarding on the stability.
It make HomeLinux an interesting tool to test usage of all up-to-date version of packages together to find new build issues.

Another usage of HomeLinux can be for HPC datacenters to build quicly some fixed prefix to provide new version to the users without
changing the operating system. Thanks to HomeLinux it is easier to build a new prefix with updated version in another directory
and making the user moving slowly to new versions by chaning the prefix they inherit from for their home personal prefix.

Features
--------

It is inspirated from gentoo package manager and import most of its semantic.

 * Automatic download of sources
 * Automatic update of version DB
 * Package dependencies
 * Useflags
 * Package slots
 * Check host system package to not rebuild the whole tree if not necessary
 (can support multiple system if compatibility name list is provided).
 * Inheritance between prefix (Not yet finished)
 * Support of module
 * Build packages in parallel (experimental)

Pre-requist
-----------

Homelinux is currently configured to be used on top of debian8 but there is no strict restiction which limit it's usage on other
platform except that it will not autodetect the presence of native packages. In this case it will tend to rebuild more packages
than required.

The only dependencies are `nodejs`, `npm` and the necessary to download and build basic packages (`tar`,`wget`,`gcc`, `make`...).

CAUTION, you need a recent vesion of `nodejs` otherwise homelinux silently fail to detect the
system packages due to lack of `child_process.execSync()`.

If you want to be able to uninstall the packages or make clean update with previous removal of the files, you might want to enable
usage of GNU stow in the config file. You previously need to install `stow` in your system or into the prefix as a first package to
be install with stow disabled.

About X libraries
-----------------

HomeLinux build part his dependency tree from Gentoo so in principle it can build a full
XLibs stack to build graphical packages. But it is likely to get compilation errors and
version mismatching with your system. Hence, it is recommended to at least install
the X, mesa and OpenGL developpement libraries if you want to compile graphical
programs.

On debian it might be recommended to install some dependencies which are likely to fail
in HomeLinux :

	sudo apt-get install mesa-common-dev mesa-utils libx11-dev libx11-xcb-dev

Installation
------------

First fetch the NPM dependencies :

	npm install

Then, install and create your prefix with make command :

	make install PREFIX=$HOME/usr

Then configure your prefix if needed by editing the file. You can find documentation about the 
configuration into [doc/config.md](doc/config.md).

	nano $HOME/usr/homelinux.json

You can also setup global user configuration by copying `userconf.json` :

	cp userconf.json ~/.homelinux.json
	nano ~/.homelinux.json

Now you can load the configuration into your `~/.bashrc` :

	eval "`$HOME/usr/bin/hl env`"

If you don't want HomeLinux to interfer with your default environenement, you can
jump to the HomeLinux sessions with :

	$HOME/usr/bin/hl jump          #start a shell with HL env
	$HOME/usr/bin/hl jump firefox  #run the HL firefix (if installed).

Before usage you need to create the cache with :

	hl update-db
	hl build-cache

If you want to be able to uninstall the packages, make clean update or create bin packages,
you need to install `stow` (or use the one from the system).

	hl install stow
	#Then enable usage of stow in config file of prefix
	nano $HOME/usr/homelinux.json

If you use inheritance and want to install a package into an inherited prefix, you need to switch
to this prefix first to make it as active prefix.

	eval "`hl switch $HOME/usrbase`"

Basic usage
-----------

You can use the given commands to install search or sync your repos :

```sh
	#provide environnement variables
	hl env
	
	#update your package DB (fetch gentoo...)
	hl update-db
	
	#install package
	hl install bash                      # use name, automatic search db
	hl install app-shells/bash           # force subdir in gentoo way
	hl install gentoo/htop               # use the gentoo archive (nodeps)
	hl install github/svalat/svUnitTest  # from github repos, use last release
	hl install urls/htop                 # Use from packages/urls.lst
	
	#for non HL packages (gentooo, github...) you can provide some deps
	#infos and conf options into homelinux/packages/quickpackages/, see examples.
	
	#you can force the vesion to install with
	hl install htop=4.8   #exact version
	hl install htop<4.8   #less than
	hl install htop<=4.8  #less eq than
	hl install 'htop!4.8' #no this one
	hl install htop~4.8   #regexpn allow all 4.8.X, take last avail
	hl install htop:4     #slot based
	
	#search in avail packages
	hl search htop
	
	#list installed packages
	hl ls
	
	#uninstall htop (only if you enable stow support in prefix config)
	hl unstinall htop
	
	#Jump into the HL env
	hl jump
	
	#Run a command from hl env (without setting the global env)
	#You can use alias to export HL commands to your shell
	#without setting the full HL env
	hl jump gcc-5.2
	
	#Whatch generated quickpackages
	hl gen-package gentoo/htop
	hl gen-full-package gentoo/htop
	
	#whatch generated install script
	hl gen-install gentoo/htop
```

You can find more information about HomeLinux concepts into [concepts](doc/concepts.md).

What it setup
-------------

Currently HomeLinux setup the environenement variables for you to fully configure the prefix : 

 * `PATH`
 * `LD_LIBRARY_PATH`
 * `LD_RUN_PATH`
 * `CPATH`
 * `PERL5LIB`
 * `PYTHONPATH`
 * `PKG_CONFIG_PATH`
 * `MANPATH`
 * `MODULEPATH`
 * `CMAKE_PREFIX_PATH`
 
Providers
---------

Homelinux use multiple sources to find packages, which are called providers. You can change in which order they are used
or skip somes by editing you prefix config file and setting up the `providers` field. The most interesting change
is the priority of debian versus gentoo depending on which distribution your prefer as fallback for non HomeLinux packages.

```json
	{
		"providers": [
			"homelinux", 
			"gentoo", 
			"debian", 
			"urls", 
			"github"
		],
	}
```

Create packages
---------------

To create package, check [doc/package.md](doc/package.md). If you don't want to provide full packages but just want to use
as most as possible the default options, you can use the quick package approach defined into [doc/quickpackage.md](doc/quickpackage.md).

Run unit tests
--------------

HomeLinux provide some internal unit tests which can by run with mocha :

```sh
	npm install mocha
	./node_modules/.bin/mocha ./tests
```

Similar tools
-------------

If I miss some interesting tools here, please notify me.

 * https://nixos.org/nix/
 * https://hpcugent.github.io/easybuild/

Licence
-------

HomeLinux is distributed under BSD licence and currently mainly developped by
Sébastien Valat.
