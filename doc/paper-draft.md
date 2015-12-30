HomeLinux, A Home Source Package Manager
========================================

Abstract
--------

*TODO*

Introduction
------------

As a scientific user we most of the time work on servers or workstation we don't manage ourself. In this case, without
adminitrativ right we cannot use the system package manager to install the extra software we might need to acheive our work.
The solution is to download the sources of the package on the dedicated website, then compile it into our home directory.

This approach work fine but quicly become a nightmare when we need some packages with many dependencies. In that case we enter
in a loop to search last availble version of the package, downlaod, start to build, see errors about dependencies. Then start
again the loop about that dependencies. This as to be done until we get all the dependency tree built, then we need to
come up remembering the install order to apply.

With HomeLinux we wanted to offer a solution to provide a package manager wich first search automatically the last version
of a package, search its dependencies and install them and automatically build and install the package. This is a source package manager
do can be compared to what the Gentoo distribution offers. This distribution also offer an eprefix approach to build the gentoo system
into a subdirectoy of your choice. In that sens it cover our use case. But in practice they force the user to rebuild the full dependency
tree. As a developper we might want to rebuild only the missing dependency an rely on the host system to get what they already provide. This
what we offer with HomeLinux as it automatically detect the packages install on the host system.

HomeLinux consider two use cases. The first one is a developper wanted to get a source package he need for his projects. The second one
is for HPC administrator who must provide a compleete updated software stack for their user.