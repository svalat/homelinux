######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0                        #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
noinst_LIBRARIES = libcore.a
libcore_a_SOURCES = DepLoader.cpp EnvSetup.cpp HostPkgChecker.cpp PackageDef.cpp PackageRequest.cpp Prefix.cpp QuickPackage.cpp UseFlags.cpp VersionMatcher.cpp
libcore_a_CPPFLAGS = -I$(srcdir)/../ -I$(srcdir)/../../extern-deps/jsoncpp/dist -DHOMELINUX_VERSION="@HOMELINUX_VERSION@"