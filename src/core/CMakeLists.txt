######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0                        #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(homelinux-core OBJECT UseFlags.cpp
                                  EnvSetup.cpp
                                  Prefix.cpp
                                  HostPkgChecker.cpp
                                  QuickPackage.cpp
                                  PackageDef.cpp
                                  DepLoader.cpp
                                  PackageRequest.cpp
                                  VersionMatcher.cpp)
#SET_TARGET_PROPERTIES(homelinux-core PROPERTIES COMPILE_FLAGS -fPIC)

######################################################
if (ENABLE_TESTS)
	add_subdirectory(tests)
endif (ENABLE_TESTS)
