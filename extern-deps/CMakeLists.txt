######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0                        #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#disable re2 tests
set(RE2_BUILD_TESTING OFF)

######################################################
add_subdirectory(gmock-1.7.0)
add_subdirectory(jsoncpp/dist)
