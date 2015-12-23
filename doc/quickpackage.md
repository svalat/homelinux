Quick packages
==============

This is an alternate approach to not provide full autonomous packages. When typing a package name
HomeLinux will try to use the self defined package, if it don't find them it will fallback on one of the
solutions : 

 * Gentoo distfiles : Search into the gentoo distfile files if one of them match the package name, extract the version
 and try install if automatically.
 * Search on github if your name is AUTHOR/PROJECT.

This is called quickpackage as it try to build automatically a package from a minimal information list.
Anyway in some cases you need to provide some dependencies. You can add them by hand on the command but
you can also fill the file into share/homelinux/quickdeps.json to automate the process. Fill free
to contribute to the content of this file.

You might also need to add some options to your package, you can use share/homelinux/quickconfig.json.
If you want to share your changes, try to add only required options, not optional one.

Another solution is to provide a more complete description but still simple into quickpackages.json but
this one might be removed prefering the file splitted approach.