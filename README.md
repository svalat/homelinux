HomeLinux
=========


What it is ?
------------

Homelinux is a simple package manager to handle installation of packages in your home directory by rebuilding them from sources.
The main use case if a user (or developper) running into a non root environnement and needs some extra-package he can't or don't want
to install into the system. Thanks to HomeLinux you can handle the installation of the source package by checking the dependencies
available on the current system and automatically installing the missing one.

Pre-requist
-----------

Homelinux is currently configured to be used on top of debian8 but there is no strict restiction which limit it's usage on other
platform except that it will not autodetect the presence of native packages. In this case it will tend to rebuild more packages
than required.

The only dependency is `nodejs` and `npm`.

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
	
	#Whatch generated quickpackages
	hl gen-package gentoo/htop
	hl gen-full-package gentoo/htop
	
	#whatch generated install script
	hl gen-install gentoo/htop
```

Licence
-------

HomeLinux is distributed under BSD licence and currently mainly developped by
Sébastien Valat.
