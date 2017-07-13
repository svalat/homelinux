HomeLinux
=========


What is it ?
------------

Homelinux is a simple package manager to handle package installation in your home directory by rebuilding them from sources.
The main use case if a user (or developper) running into a non root environnement and needs some extra-package he can't or don't want
to install into the system. Thanks to HomeLinux you can handle the installation of the source package by checking the dependencies
available on the current system and automatically installing the missing one.

The goal is to provide a quick access to the latest version available for all packaging without regarding on the stability.
It make HomeLinux an interesting tool to test usage of all up-to-date version of packages together to find new build issues.

Features
--------

It is inspirated from gentoo package manager and import most of its semantic.

 * Automatic download of sources
 * Automatic update of version DB
 * Package dependencies
 * Useflags
 * Package slots
 * Check host system package to not rebuild the whole tree if not necessary
 (currently supported centos, debian and gentoo).
 * Inheritance between prefix
 * Support of module
 * Build packages in parallel (experimental)

Pre-requist
-----------

The only dependencies are a C++ compiler complient with C++98 and the necessary tools to download and build basic source packages (`bash`,`tar`,`curl`,`wget`,`gcc`, `make`...).

If you want to be able to uninstall the packages or make clean update with previous removal of the files, you might want to enable
usage of GNU stow in the config file. You previously need to install `stow` in your system or into the prefix as a first package to
be installed.

Installation
------------

HomeLinux can be compiled with cmake of with autotools, the configure script will choose automatically depending 
on what is available on your system :

    mkdir build
    cd build
    ../configure --prefix=$HOME/usr
    make -j8
    make install

You can then load the prefix into your environnement :

    source $PREFIX/source.sh

Then configure your prefix if needed by editing the file. You can find documentation by using
`man hl-config`:

	nano $PREFIX/homelinux.json

You can also setup global user configuration by editing (look on `man hl-config` to get help:

	nano ~/.homelinux.json

If you don't want HomeLinux to interfer with your default environenement, instead of loading
the environnement into your bashrc, you can jump into the HomeLinux sessions with :

	$PREFIX/bin/hl jump          #start a shell with HL env
	$PREFIX/bin/hl jump firefox  #run the HL firefix (if installed).

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
    
    #Crawl the web to getch last version of every packages
    hl crawl
	
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

Enabling a more recent GCC
--------------------------

By using homelinux you can easily install a recent version of GCC. This GCC will be added to the
avilable module list and need to be loaded to be used.

Although you need to consider that when enabling this compiler you will then compiler all the
homelinux packages with it. If the gap with your distribution GCC is to large you can encounter
some issues. This is for example with C++ libraries if you system is GCC less then 5 and you want
to use GCC newer than 5 due to an incompatibility between the two versions.

This can work but it require some care to really recompile all the C++ dependencies which can come
from the host system.

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
 * `HL_PREFIX_PATH`
 
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

About libxml2 and Python
------------------------

Libxml2 has a flag to enable python support which is required by some packages which can be enabled
by editing PREFIX/homelinux.conf :

```json
{
	"use": {
		"all": [],
		"hl/dev-libs/libxml2": ["python"]
	}
}
```

While proceeding like this you will encounter an issue as libxml will try to install the python
plugin into your system python (/usr....) which of course fail as you are not root. The solution
is to install python:2 into homelinux :

```sh
hl install libxml2 python:2
```

Similar tools
-------------

Here you can find some realy interesting similar tools. If I miss some interesting tools here, please notify me.

 * https://nixos.org/nix/
 * https://hpcugent.github.io/easybuild/
 * https://github.com/LLNL/spack

Licence
-------

HomeLinux is distributed under CeCill-C licence and currently mainly developped by
Sébastien Valat.