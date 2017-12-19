######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0                        #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(homelinux-providers OBJECT Provider.cpp
                                       ProviderUrls.cpp
                                       ProviderModels.cpp
                                       ProviderGithub.cpp
                                       ProviderGentoo.cpp
                                       ProviderDebian.cpp
                                       ProviderHomelinux.cpp)
#SET_TARGET_PROPERTIES(homelinux-providers PROPERTIES COMPILE_FLAGS -fPIC)

######################################################
if (ENABLE_TESTS)
	add_subdirectory(tests)
endif (ENABLE_TESTS)
