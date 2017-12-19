######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0                        #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
noinst_LIBRARIES = libcrawlers.a
libcrawlers_a_SOURCES = Crawler.cpp CrawlerFtp.cpp CrawlerGentoo.cpp CrawlerGithub.cpp CrawlerGnomeCache.cpp CrawlerHtml.cpp
libcrawlers_a_CPPFLAGS = -I$(srcdir)/../ -I$(srcdir)/../../extern-deps/jsoncpp/dist -DEXTRACTOR_SOURCE_PATH="\"$(abs_srcdir)/html-link-extractor\""

######################################################
#install
install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/homelinux
	cp -r $(srcdir)/html-link-extractor $(DESTDIR)$(prefix)/share/homelinux
