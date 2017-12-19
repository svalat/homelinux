######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0                        #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(homelinux-crawlers OBJECT Crawler.cpp
                                     CrawlerFtp.cpp
									 CrawlerGentoo.cpp
									 CrawlerGithub.cpp
									 CrawlerHtml.cpp
									 CrawlerGnomeCache.cpp)
SET_TARGET_PROPERTIES(homelinux-crawlers PROPERTIES COMPILE_FLAGS -fPIC)

######################################################
add_definitions(-DEXTRACTOR_SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/html-link-extractor")

######################################################
if (ENABLE_TESTS)
	add_subdirectory(tests)
endif (ENABLE_TESTS)

######################################################
install(DIRECTORY html-link-extractor DESTINATION share/homelinux)
