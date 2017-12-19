######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0                        #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
noinst_LIBRARIES = libbase.a
libbase_a_SOURCES = Config.cpp Debug.cpp FormattedMessage.cpp Helper.cpp
libbase_a_CPPFLAGS = -I$(srcdir)/../ -I$(srcdir)/../../extern-deps/jsoncpp/dist -DHOMELINUX_VERSION="@HOMELINUX_VERSION@" -DPREFIX="\"$(prefix)\""