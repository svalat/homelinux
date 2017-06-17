######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(homelinux-hosts OBJECT /dev/null)
#SET_TARGET_PROPERTIES(homelinux-hosts PROPERTIES COMPILE_FLAGS -fPIC)

######################################################
#if (ENABLE_TESTS)
#	add_subdirectory(tests)
#endif (ENABLE_TESTS)
