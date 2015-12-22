HomeLinux
=========


What it is ?
------------

Homelinux is a simple package manager to handle installation of packages in your home directory by rebuilding them from sources.
The main use case if a user (or developper) running into a non root environnement and needs some extra-package he can't or don't want
to install into the system. Thanks to HomeLinux you can handle the installation of the source package by checking the dependencies
available on the current system and automatically installing the missing one.

The goal is to provide a quick access to the latest version available for all packaging without regarding on the stability.
It make HomeLinux an interesting tool to test usage of all up-to-date version of packages together to find new build issues.

Another usage of HomeLinux can be for HPC datacenters to build quicly some fixed prefix to provide new version to the userse without
changing the operating system. Thanks to HomeLinux it is easier to build a new prefix with updated version in another directory
and making the user moving slowly to new versions.

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

Pre-requist
-----------

Homelinux is currently configured to be used on top of debian8 but there is no strict restiction which limit it's usage on other
platform except that it will not autodetect the presence of native packages. In this case it will tend to rebuild more packages
than required.

The only dependencies are `nodejs`, `npm` and the necessary to download and build basic packages (`tar`,`wget`,`gcc`, `make`...).

Installation
------------

First fetch the NPM dependencies :

	npm install

Then, install and create your prefix with make command :

	make install PREFIX=$HOME/usr

Then configure your prefix if needed by editing :

	nano $HOME/usr/homelinux.json

You can also setup global user configuration by copying `userconf.json` :

	cp userconf.json ~/.homelinux.json
	nano ~/.homelinux.json

Now you can load the configuration into your `~/.bashrc` :

	eval "`$HOME/usr/bin/hl env`"

Before usage you need to create the cache with :

	hl update-db
	hl build-cache

Basic usage
-----------

You can use the given commands to install search or sync your repos :

```sh
	#provide environnement variables
	hl env
	
	#update your package DB (fetch gentoo...)
	hl update-db
	
	#install package
	hl install app-shells/bash
	hl install gentoo/htop
	hl install github/svalat/svUnitTest
	
	#will search automatically in db, quickpackages and gentoo db
	hl install htop
	
	#you can force the vesion to install with
	hl install htop=4.8   #exact version
	hl install htop<4.8   #less than
	hl install htop<=4.8  #less eq than
	hl install 'htop!4.8' #no this one
	hl install htop:4     #slot based
	
	#search in avail packages
	hl search htop
	
	#list installed packages
	hl ls
	
	#Whatch generated quickpackages
	hl gen-package gentoo/htop
	hl gen-full-package gentoo/htop
	
	#whatch generated install script
	hl gen-install gentoo/htop
```

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

Create packages
---------------

To create package, check [doc/package.md](doc/package.md).

Licence
-------

HomeLinux is distributed under BSD licence and currently mainly developped by
Sébastien Valat.
