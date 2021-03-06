HomeLinux concepts
==================

Here a small page to discuss some of the main concepts handled by HomeLinux.

Inheritance
-----------

HomeLinux manage inheritance between prefix. It mostly permit to build a first prefix which can be provided
by the system administrator with some base packages. Then a user can inherit from this prefix to not rebuild
those packages but add his own packages on top of it.

I originaly used it to build a stable prefix with program and libs I'm sure to be able to run with. Then
I create a less safe prefix in which I can test and restart from scratch if I messup too much with
problematic packages.

Use flags
---------

Similarly to Gentoo, HomeLinux provide use flag management to enable or disable features on packages.
You can enable with `+flag` and disable with `-flag`. In package file `flag` declare the flag but make
it disable.

This can be configured from the prefix configure file `$PREFIX/homelinuxj.json` globaly or per package :

```json
{
	"use": {
		"all": ["X","qt"],
		"hl/sci-visualization/gnuplot": ["gtk"]
	}
}
```

Use flags inside packages
-------------------------

The use flags are applied in `dep` field to enable a dependency only if flag is enabled or disabled.
You can use `flag` or `+flag` to search the dep if flag is enabled, `-flag` to search on disable.

```json
{
	"deps": {
		"X? x11-libs/libX11"
	}
}
```

It is also used into `configure` field to add some config options. `+flag` and `-flag` will add the option
only if enable or disable, otherwise it ignore and not push the option. `flag` will add the option only
if explicitly enabled or disabled. If the flag is let by default (defined by `flag` (without `+` or `-`) 
in `use` of package) then it will ignore the option.

```json
{
	"configure": {
		"X": [ "--$enable-x11" ]
	}
}
```

Slots
-----

Like in Gentoo the packages use slots to authorize installation of multiple versions at same time if their
API to not overlap. Slot value is assigned depending on the version of the package. If you install a package
with a slot already in use it will replace the current installation.

```sh
#install python slot 3
hl install python:3
```

Modules
-------

HomeLinux can handle usage of Module and can install some packages in Modules directory and create a Module
definition so you can optionally use it by calling the `module` command in your shell. This is enabled by
adding a `module` field to packages. It can also be declared into prefix config.

Usage of stow
-------------

HomeLinux only manage uninstall if you enable usage of GNU stow. This tool permit to install each package
into a separate directory and re-established to flat prefix by setting-up symlinks. It helps to get
the list of files to uninstall in a trivial way instead of playing with non always supported DESTDIR approach
on `make install`. It also avoid to handle package list files.

If you don't enable stow in prefix config file you cannot uninstall packages and update will work by overriding
the existing file, it might let some old unneeded files into the directory tree. If you want to use
update it is safer to enable stow otherwise it might be better to rebuild the full tree from scratch.

Package provider
----------------

HomeLinux use multiple package provider. The default provider is the internal package database which is currently
quite small (~375 packages). Those packages are well defined with full use flags and dependency checking. It also
support checking of host system package available to not rebuild what is already available.

In addition to this HomeLinux can search unknown packages into some external databases :

 * Gentoo. It can search in the gentoo distfile repository to fetch the last version of the requested package.
 Remark that in this case you have no dependency checking by default except if you fillup the files
 into `homelinux/quickpackages`. You can force by using `gentoo/NAME`.
 * urls. This is a custom list of archive URLs to provide special packages. You need to use `urls/NAME`
 * Github. If you provide the package name with `github/USER/NAME` it will automatically fetch the last
 release (based on release or tags) of the package.
 
If you provide a simple name it will first search in the local database then search in gentoo, then
in url and finally to github.

Version crawler
---------------

The internal package database also define URLs to grab the last version available. It support :

 * Http page by searching over internal link text with a regexp.
 * Ftp to serach in files into a directory with a regexp.
 * Github to search in tags with a regexp
 * Gentoo version
 
It is used by `hl crawl` to automatically fetch the last available version of each defined packages
and keep you update without any human manual work.

Quickpackages
-------------

It is linked to Gentoo, Urls and github providers. It is a way to quicly provide the most important information
of a package without create a complete package description. It is based on simple flat text files located
into `homelinux/quickackages`. You can setup :

 * Version regexp to help seraching the last version of your package if it use non standard numbering (like mplayer).
 * Dependencies to support deps of your package.
 * Config to provide some config options.
 * Type to define the type of package to inheratit (by default `auto` which tries to use the good sub template).
 * Module to force installation into Module subsystem, not as default available package.
 * subdir to define the directory inside the archive if non standard.
 * patch if you want to apply some patches.

Remark, historically this was the canonical way of running homelinux before the introduction of the well done packages.

Package provider
----------------

The quickpackage system is bases on multiple providers, currently homelinux support checked in order :

 * Homelinux (this is the default package database to search on).
 * Gentoo, use the `distfile` directory from gentoo mirrors to find the source package and last available version.
 * Urls, use an url list file.
 * Github, search on gitbhub to find the sources and last version (based on tags).
 
Host checking
-------------
 
HomeLinux can check if a package is available into the host it is running on. It break the dependency tree to not
install packages which are already available in the system. You can force the installation by adding the package
name to your command line if needed.
 
To run it need to know the corresponding name for the distribution you are using. It can be found into `host` section
of packages or into `packages/hosts` subdirectory.

The current host is configures into `~/.homelinux.json` and we currently support :

 * default : we apply a default policy considering that some packages are provided by most distribution by default and force
 rebuilding all other packages.
 * true: consider that all packages are already provided by the distribution (mostly useless, but it is there...)
 * false: force rebuilding all packages by considering that the host system does not provide any packages.
 * centos7 : consider a centos7 system and will use `rpm -V $PACKAGE` to chaeck if installed or not.
 * debian8 : consider a debian base distribution and will check with `apt-get` if package is installed.
 * gentoo : look for gentoo packages in `/var/`
 
Parallel build
--------------
 
HomeLinux support parallelisation of configure, downloading and build to get best performance of your machine. It is based
on the `make` tool to handle the task management and define the full dependency tree. You need to ensure that the packages
you are building have strict dependency defintion otherwise you will get some crash due to lack of required packages.
 
It is a good way to enforce dep checking. You can use it by calling `pinstall` instead of `install`.

Caution, this feature really work only with well defined packages with full dependecies defined.
 
Package inheritance
-------------------
 
For the internal packages. A package ca inherit from a model (or another package). In that case it will extend the already
available dependencies, use flags, configure options, slots.... You can also override the building steps to provide you own
commands.