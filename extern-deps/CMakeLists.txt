######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#disable re2 tests
set(RE2_BUILD_TESTING no)

######################################################
add_subdirectory(gmock-1.7.0)
add_subdirectory(jsoncpp/dist)
add_subdirectory(re2-2017-06-01)
