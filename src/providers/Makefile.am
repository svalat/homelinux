######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
noinst_LIBRARIES = libproviders.a
libproviders_a_SOURCES = Provider.cpp ProviderDebian.cpp ProviderGentoo.cpp ProviderGithub.cpp ProviderHomelinux.cpp ProviderModels.cpp ProviderUrls.cpp
libproviders_a_CPPFLAGS = -I$(srcdir)/../ -I$(srcdir)/../../extern-deps/jsoncpp/dist -DHOMELINUX_VERSION="@HOMELINUX_VERSION@"