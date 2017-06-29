bin_PROGRAMS = hl
hl_SOURCES = main.cpp HomeLinux.cpp 
hl_CPPFLAGS = -I$(srcdir) -I$(srcdir)/../extern-deps/jsoncpp/dist
hl_LDADD = base/libbase.a \
           core/libcore.a \
		   portability/libportability.a  \
		   providers/libproviders.a  \
		   crawlers/libcrawlers.a  \
		   ../extern-deps/jsoncpp/dist/libjsoncpp.a  \
		   ../extern-deps/re2-2017-06-01/re2/libre2.a \
		   ../extern-deps/re2-2017-06-01/util/libre2util.a \
		   -lpthread


SUBDIRS = base core crawlers portability providers