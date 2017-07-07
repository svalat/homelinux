######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
bin_PROGRAMS = hl
hl_SOURCES = main.cpp HomeLinux.cpp 
hl_CPPFLAGS = -I$(srcdir) -I$(srcdir)/../extern-deps/jsoncpp/dist
hl_LDADD = base/libbase.a \
           core/libcore.a \
		   portability/libportability.a  \
		   providers/libproviders.a  \
		   crawlers/libcrawlers.a  \
		   ../extern-deps/jsoncpp/dist/libjsoncpp.a  \
		   -lpthread


SUBDIRS = base core crawlers portability providers
