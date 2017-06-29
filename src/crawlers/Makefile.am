noinst_LIBRARIES = libcrawlers.a
libcrawlers_a_SOURCES = Crawler.cpp CrawlerFtp.cpp CrawlerGentoo.cpp CrawlerGithub.cpp CrawlerGnomeCache.cpp CrawlerHtml.cpp
libcrawlers_a_CPPFLAGS = -I$(srcdir)/../ -I$(srcdir)/../../extern-deps/jsoncpp/dist -I$(srcdir)/../../extern-deps/re2-2017-06-01/ -DEXTRACTOR_SOURCE_PATH="\"$(abs_srcdir)/html-link-extractor\""